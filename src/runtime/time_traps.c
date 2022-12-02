
#include "common.h"
#include "vmachine.h"
#include <time.h>

// store the results of time() in R0
bool get_time_trap()
{
    Value val;
    val.type = NUM;
    val.data.num = (double)time(NULL);

    pushVal(val);

    return false;
}

// print the date that is found in R0
bool show_time_trap()
{
    Value val = popVal();

    if(val.type != NUM) {
        runtimeError("print_time_trap: invalid time");
        return true;
    }

    const time_t t = (time_t)val.data.num;
    printf("%s", ctime(&t));

    return false;
}

// Place the clock() count in R0 as an int.
bool get_clock_trap()
{
    Value val;
    val.type = NUM;
    val.data.num = (double)clock();

    pushVal(val);

    return false;
}

// The start time is in R1 and the end time is in R2 as ints. The result is
// placed in R0 as a float. (TODO: error checking)
bool dif_clock_trap()
{
    Value end = popVal();
    Value start = popVal();
    // TODO: check the types of the stack items

    // printf("end = %0.4f, start = %0.4f\n", end.data.fnum, start.data.fnum);
    Value out;
    out.type = NUM;
    out.data.num = (end.data.num - start.data.num) / (double)CLOCKS_PER_SEC;

    pushVal(out);

    return false;
}

