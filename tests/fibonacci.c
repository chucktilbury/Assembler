#include <stdio.h>
int fib(int n) {
    if(n < 2)
        return n;
    else
        return fib(n-1) + fib(n-2);
}

int main() {
    printf("the 35th number in the fibonacci series is %d\n", fib(35));
    return 0;
}
