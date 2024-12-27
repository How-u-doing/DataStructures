import pandas as pd
import polars as pl
import duckdb
import time

N = 100_000
# [N x10, (N-1) x10, ..., 1 x10]
a = [i for i in range(N, 0, -1) for _ in range(10)]
k = 500_000

df = pd.DataFrame({"x" : a})
pldf = pl.DataFrame({"x": a})

t1 = time.time()
xf = df.rolling(k).max()
print("pandas:", time.time() - t1)
print(xf[k-1:])

t1 = time.time()
# DuckDB uses a segment tree that can compute the maximum of any subarray in O(log n) time
xf = duckdb.query(f"""
    select max(x) 
        over (rows between {k-1} preceding and current row) as rolling_max
    from df
""").df()
print("duckdb:", time.time() - t1)
print(xf[k-1:])

t1 = time.time()
rolling_max = pldf.select(
    pl.col("x").rolling_max(window_size=k)
)
print("polars:", time.time() - t1)
print(rolling_max[k-1:])
