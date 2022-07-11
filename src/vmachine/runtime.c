
#include "vmachine.h"
#include "runtime.h"
#include <time.h>

#include "../system.inc"

extern Value registers[16];

// print the value given in R0
static void print_trap()
{
    Value* val = &registers[0];
    printf("\n");
    printVal(val);
    printf("\n");
}

// print the value given in R0 with the type
static void dbg_print_trap()
{
    Value* val = &registers[0];
    printf("\n(%s)", valTypeToStr(val->type));
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
    end = registers[2].data.num;
    // fprintf(stderr, "\nend: %ld - start: %ld\n", end, start);

    registers[0].type = FLOAT;
    registers[0].data.fnum = ((double)end - (double)start) / (double)CLOCKS_PER_SEC;
    // fprintf(stderr, "%0.4f = (%0.4f - %0.4f) / %0.4f\n",
    //         (double)(end - start) / (double)CLOCKS_PER_SEC,
    //         (double)end, (double)start, (double)CLOCKS_PER_SEC);
}

// The string to format is in R1 and the destination is placed in R0.
static void format_str_trap()
{
    if(registers[1].type != STRING) {
        registers[0].type = STRING;
        registers[0].data.str = addStr(valToStr(&registers[1]));
    }
    else {
        registers[0].type = STRING;
        const char* str = getStr(registers[1].data.str);
        registers[0].data.str = addStr(formatStr(str));
    }
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
        case FMT_STR:       format_str_trap();  break;
        default:
            runtimeError("unhandled trap number: %04X", tno);
            return true;
    }
    return false;
}
