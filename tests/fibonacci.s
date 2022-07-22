/*
 * This test recursively computes the fibonacci series up to the given limit.
 * It is not intended to be efficient. It is intended to test recursion and
 * to write a somewhat complete program in Goldfish assembly.
 *
 * Equivalent C code
 *
 * int fibo(int n)
 * {
 *     if(n < 2)
 *         return n;
 *     else {
 *         int x = fibo(n-1) + fibo(n-2);
 *         return x;
 *     }
 * }
 *
 * int main()
 * {
 *     printf("value: %d\n", fibo(12));
 * }
 *
 */
#include "system.inc"

// Must compile with entry enabled.

string timer_string = "elapsed time is {timer} seconds\n"
float timer
string result_string = "the {value}th value of the fibonacci series is {result}\n"
int result
int value = 35

/*
 * fibonacci function.
 * input parameter is on the stack and the return vlaue is in R0
 *
 *  Functions use the cdecl calling convention. Registers r0 - r8 are
 *  considered volitile.
 */
fibonacci:
    peek    r0,r15,1    // get the function parameter (n)
    load    r1,2
    gte     r0,r1       // if n >= 2 then jump
    br      _fibo_skip_1
    return              // return value is in R0

_fibo_skip_1:
    push    r0          // save the original value (n)
    load    r1,1
    sub     r2,r0,r1    // r2 = n - 1

    push    r2          // function parameter
    sidx    r15         // base of stack
    call    fibonacci   // recursive call
    pop     r15         // balance the stack
    load    r3,r0       // save the result in R3

    pop     r0          // restore the original value
    load    r1,2
    sub     r2,r0,r1    // r2 = n - 2
    // this function is recursive, so a temp value like this must be stacked
    push    r3          // save the previous result in the stack

    push    r2
    sidx    r15
    call    fibonacci   // recursive call, results in r0
    pop     r15         // balance the stack
    pop     r3          // restore the previous result

    // add the results of the 2 func calls with result in R0
    add     r0,r0,r3
    return

/*
 *  Start routine is the same as the main() routine in C.
 */
_start:
    // get the start time
    trap    GET_CLOCK
    pop     r0
    store   timer,r0

    // This is the start condition. Compute the factoral of this number
    load    r0,value

    // call the factoral routine
    push    r0          // function parameter
    sidx    r15         // base of stack in R15
    call    fibonacci   // result returned in R0
    pop     r15         // balance the stack

    // print out the results
    store   result,r0       // store the result for the formatter
    load    r0,result_string  // get the string to format
    push    r0
    trap    FMT_STR         // format the string
    trap    PRINT           // print it out

    // get the end time
    load    r1,timer
    push    r1
    trap    GET_CLOCK   // end clock is on TOS
    trap    DIF_CLOCK
    pop     r0
    store   timer,r0
    load    r0,timer_string
    push    r0
    trap    FMT_STR
    trap    PRINT       // print out the elapsed time
    load    r0,0        // success return code
    return
/* end of _start routine */

