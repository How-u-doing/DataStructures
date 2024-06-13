import duckdb

df1 = duckdb.query("""
    select *, abs(earnings/shares) as avg_price
    from read_csv(res1.csv, names=[party, shares, earnings])
    order by party
""")
print(df1)

df1 = duckdb.query("""
    select sum(shares), sum(earnings)
    from read_csv(res1.csv, names=[party, shares, earnings])
""")
print(df1)
