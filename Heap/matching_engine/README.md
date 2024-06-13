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
Ask set

$$
A = \{
    (ap_1, aq_1),
    (ap_2, aq_2),
    \cdots,
    (ap_m, aq_m)
\}
$$

Bid set

$$
B = \{
    (bp_1, bq_1),
    (bp_2, bq_2),
    \cdots,
    (bp_n, bq_n)
\}
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

So, the total transaction amount $T$ is bounded by lower one of the selling
quantity of $AB$ and the buying quantity of $BA$. That is,

$$
T = p \times \min(Q_A(p), Q_B(p)),
$$

where
$Q_A(p)$ is the cumulative quantity of asks at or below ($\leq$) $p$, and
$Q_B(p)$ is the cumulative quantity of bids at or above ($\geq$) $p$.

Math time!
...
