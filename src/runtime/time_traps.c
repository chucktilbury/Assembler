
#include "common.h"
#include "vmachine.h"
#include <time.h>

// store the results of time() in R0
bool get_time_trap()
{
    Value val;
    val.type = UINT;
    val.data.unum = time(NULL);

    //memcpy(&registers[0], &val, sizeof(val));
    pushVal(val);

    return false;
}

// print the date that is found in R0
bool show_time_trap()
{
    Value val = popVal();
    //memcpy(&val, &registers[0], sizeof(val));
    if(val.type != INT) {
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
    val.type = INT;
    val.data.num = clock();

    //memcpy(&registers[0], &val, sizeof(val));
    pushVal(val);

    return false;
}

// The start time is in R1 and the end time is in R2 as ints. The result is
// placed in R0 as a float. (TODO: error checking)
bool dif_clock_trap()
{
    // int64_t start;
    // int64_t end;
    Value end = popVal();
    Value start = popVal();

    // start = registers[1].data.num;
    // end = registers[2].data.num;

    // fprintf(stderr, "\nend: %ld - start: %ld\n", end, start);

    // registers[0].type = FLOAT;
    Value out;
    out.type = FLOAT;
    out.data.fnum = ((double)end.data.num - (double)start.data.num) / (double)CLOCKS_PER_SEC;
    pushVal(out);
    // registers[0].data.fnum = ((double)end - (double)start) / (double)CLOCKS_PER_SEC;
    // fprintf(stderr, "%0.4f = (%0.4f - %0.4f) / %0.4f\n",
    //         (double)(end - start) / (double)CLOCKS_PER_SEC,
    //         (double)end, (double)start, (double)CLOCKS_PER_SEC);

    return false;
}

