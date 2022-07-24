
def fib(n):
    if n < 2:
        return n
    else:
        return fib(n-1) + fib(n-2)

print("the 35th number in the fibonacci series is ", fib(35))
