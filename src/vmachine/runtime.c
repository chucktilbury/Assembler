
#include "vmachine.h"
#include "runtime.h"
#include <time.h>

#include "../system.inc"

extern Value registers[16];

// print the value given in R0
static void print_trap()
{
    Value* val = &registers[0];
    printVal(val);
    printf("\n");
}

// print the value given in R0 with the type
static void dbg_print_trap()
{
    Value* val = &registers[0];
    printf("(%s)", valTypeToStr(val->type));
    printVal(val);
    printf("\n");
}

// store the results of time() in R0
static void get_time_trap()
{
    Value val;
    val.type = UINT;
    val.data.unum = time(NULL);

    memcpy(&registers[0], &val, sizeof(val));
}

// print the date that is found in R0
static void show_time_trap()
{
    Value val;
    memcpy(&val, &registers[0], sizeof(val));
    if(val.type != UINT) {
        runtimeError("print_time_trap: invalid time");
        return;
    }

    const time_t t = (time_t)val.data.unum;
    printf("%s", ctime(&t));
}

// Place the clock() count in R0 as an int.
static void get_clock_trap()
{
    Value val;
    val.type = INT;
    val.data.num = clock();

    memcpy(&registers[0], &val, sizeof(val));
}

// The start time is in R1 and the end time is in R2 as ints. The result is
// placed in R0 as a float. (TODO: error checking)
static void dif_clock_trap()
{
    int64_t start;
    int64_t end;

    start = registers[1].data.num;
    //printf("start: %ld\n", start);
    end = registers[2].data.num;
    //printf("end: %ld\n", end);
    registers[0].type = FLOAT;
    registers[0].data.fnum = (double)(end - start) / (double)CLOCKS_PER_SEC;
}

// trap dispatcher
bool handleTrap(uint16_t tno)
{
    switch(tno) {
        case DBG_PRINT:     dbg_print_trap();   break;
        case PRINT:         print_trap();       break;
        case GET_TIME:      get_time_trap();    break;
        case SHOW_TIME:     show_time_trap();   break;
        case GET_CLOCK:     get_clock_trap();   break;
        case DIF_CLOCK:     dif_clock_trap();   break;
        default:
            runtimeError("unhandled trap number: %04X", tno);
            return true;
    }
    return false;
}
