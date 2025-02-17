# Matching Engine

## Get the net position of each party

```bash
mark@ubuntu ~/r/D/H/matching_engine (master)> make
FF_DOS=1 ./matching_engine 1 < data/orders.csv > res1.csv
volume: 201932
python3 verify.py
┌──────────┬────────┬───────────────────┬────────────────────┐
│  party   │ shares │     earnings      │     avg_price      │
│ varchar  │ int64  │      double       │       double       │
├──────────┼────────┼───────────────────┼────────────────────┤
│ Inara    │  -1937 │         131514.29 │  67.89586473928756 │
│ Jayne    │    365 │ -51680.1800000001 │ 141.58953424657562 │
│ Kaylee   │   2652 │        -179810.83 │  67.80197209653092 │
│ Mal      │  -2410 │         195250.08 │  81.01663070539419 │
│ River    │   7862 │        -573170.83 │  72.90394683286695 │
│ Shepherd │   5671 │        -414483.96 │  73.08833715394111 │
│ Simon    │    275 │ -21753.1599999999 │  79.10239999999965 │
│ Wash     │  -1729 │         113120.02 │   65.4251127819549 │
│ Zoe      │ -10749 │         801014.57 │  74.51991534096194 │
└──────────┴────────┴───────────────────┴────────────────────┘

┌─────────────┬────────────────────────┐
│ sum(shares) │     sum(earnings)      │
│   int128    │         double         │
├─────────────┼────────────────────────┤
│           0 │ 1.4551915228366852e-11 │
└─────────────┴────────────────────────┘
```

## Find the matching price that maximizes the total transaction amount
### The formula
Ask set

$$
A = \lbrace
    (ap_1, aq_1),
    (ap_2, aq_2),
    \cdots,
    (ap_m, aq_m)
\rbrace
$$

Bid set

$$
B = \lbrace
    (bp_1, bq_1),
    (bp_2, bq_2),
    \cdots,
    (bp_n, bq_n)
\rbrace
$$

```
            |    |                                ^  higher price
            |    |                                |
            |    |           bids                 |
            |    |          +----+                |
            |    |          |    |                |
            |    |          |    |
            |    |          | BA |
            |    |          |    |
            |    |          |    |
            |    |          |    |
    ---------------------------------------- L, the matching price
            |    |          |    |
            |    |          |    |
            |    |          |    |
            | AB |          |    |
            |    |          |    |
            |    |          |    |
            +----+          |    |
             asks           |    |
                            |    |
```
Let's place the ask orders in ascending order and place the bid orders in
descending order. We can see that only the ask orders at or below the matching
price $p$ (the area $AB$) are glad to sell their shares, and that only the bid
orders at or above the matching price (the area $BA$) are happy to buy shares
at such price. All orders from $AB$ and $BA$ are traded at price $p$.

So, the total transaction amount $T$ is bounded by the lower one of the selling
quantity of $AB$ and the buying quantity of $BA$. That is,

$$
T = p \times \min(Q_A(p), Q_B(p)),
$$

where
$Q_A(p)$ is the cumulative quantity of asks at or below price $p$, and
$Q_B(p)$ is the cumulative quantity of bids at or above price $p$.

### Observation
A naive algorithm we can easily come up with is to try each price in the
tradable price range $[a,b]$, where $a$ is the minimum ask price and $b$ is
the maximum bid price. But prices aren't continuous, how can or should we
try all prices?

```
            |~
            |~        bids
            |~         *| <-- b
            |~         *|
            |          *|
            |~         *| <-- p'
            |           |
            |-----------|<--- p
            |~          |
            |~         *|
            |~         *|
            |~         *| <-- a
           asks        *|
                       *|
```
The thing is if we have found the ideal price $p$ that is neither a bid price
nor an ask price, we can keep increasing the price $p$ until we encounter the
first bid or ask price, marking it as $p'$. Then we have

$$
p' > p, \\
Q_A(p') \geq Q_A(p), \\
Q_B(p') = Q_B(p),
$$

which indicates

$$
T' = p' \times \min(Q_A(p'), Q_B(p')) > p \times \min(Q_A(p), Q_B(p)) = T.
$$

This suggests that the matching price $p$ that maximizes the total transaction
amount must be a bid or ask price!

### Visualization
![](img/matching_price_graph.png)

We can compute the final matching price with a single SQL query using DuckDB!
```sql
with df as (
    select *
    from read_csv('data/orders.csv', names=['id', 'party', 'price', 'quantity', 'timestamp', 'side'])
)
select *
from (
    select distinct price as test_price
    from df
    where price <= (
            select max(price) from df where side='BUY'
        ) and price >= (
            select min(price) from df where side='SELL'
        )
), lateral (
    with cte1 as (
        select sum(quantity) as Q_A_p
        from df
        where side='SELL' and price <= test_price
    ), cte2 as (
        select sum(quantity) as Q_B_p
        from df
        where side='BUY' and price >= test_price
    )
    select Q_A_p, Q_B_p, test_price * least(Q_A_p, Q_B_p) as transaction_amount
    from cte1, cte2
)
order by test_price desc
```

```bash
mark@ubuntu ~/r/D/H/matching_engine (master)> make P2
FF_DOS=1 ./matching_engine 2 < data/orders.csv
matching price: 75.33, max transaction amount: 9450223.83
```
![](img/p2_result.png)
