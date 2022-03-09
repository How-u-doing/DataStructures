import sys

def fib(n):
    if n == 0: return 0
    a, b = 0, 1
    for _ in range(n):
        a, b = b, a+b
        # note that it's not equivalent to
        #   a = b
        #   b = a+b
        # or with the order exchaned, a copy of
        # a and b are saved before assignment
        # equivalent version:
        #   t = a
        #   a = b
        #   b = t+b
    return a

print(fib(int(sys.argv[1])))
