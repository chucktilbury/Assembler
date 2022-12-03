/*
 *  This is a simple test program to find the factorial of a given integer.
 *
 *  It is not intended to be efficient. It's more of a benchmark so I can
 *  tell if a change has impacted performance.
 *
 *  Equivalent C code:
 *
 *  int factorial(int n) {
 *     //base case
 *     if(n == 0) {
 *        return 1;
 *     } else {
 *        return n * factorial(n-1);
 *     }
 *  }
 *
 *  int main()
 *  {
 *      printf("factorial: %d\n", factorial(12));
 *  }
 */
#include "system.inc"
#include "startup.inc"

string timer_string = "elapsed time is {timer} seconds\n"
number timer
string fact_string = "factorial of {value} is {factor}\n"
number factor
number value

_start:

    // get the start time
    trap    GET_CLOCK
    pop     r0
    store   timer,r0

    // This is the start condition. Compute the factoral of this number
    load    r0,18
    store   value,r0

    // call the factoral routine
    push    r0          // function parameter
    sidx    r15         // base of stack in R15
    call    factorial   // result returned in R0
    pop     r15         // balance the stack

    // print out the results
    store   factor,r0       // store the result for the formatter
    load    r0,fact_string  // get the string to format
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
    trap    PRINT
    load    r0,0        // success return code
    return
/* end of _start routine */

/*
 *  This is the factoral function. It recursively calculates the factoral
 *  value of the parameter. The result is in R0.
 */
factorial:
    peek    r0,r15,1
    load    r1,0
    // if r0 == 0 then return 1
    neq     r0,r1
    br      _fact_skip_1
    load    r0,1
    return

_fact_skip_1:
    // R0 still has the function parameter
    push    r0  // save the value
    load    r1,1

    sub     r2,r0,r1    // r2 = r0 - 1
    push    r2          // push the new parameter
    sidx    r15
    call    factorial
    pop     r15         // balance the stack

    // result of previous call is in R0
    pop     r1          // restore the original parameter
    mul     r0,r0,r1    // r0 = r0 * r1, where R0 is the return value
    return
/* end of factorial function */