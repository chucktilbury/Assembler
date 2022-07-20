
#include "common.h"
#include "vmachine.h"

#include "system.inc"
#include "traps.h"

/*
 * This is a switch/case instead of a table to make it easier to see if the
 * trap number is valid or not. There can be up to USHORT_MAX traps, and they
 * should not have to be consecutively numbered.
 */
bool handleTrap(uint16_t tno)
{
    switch(tno) {
        case ABORT:         return abort_trap();
        case DBG_PRINT:     return dbg_print_trap();
        case PRINT:         return print_trap();
        case GET_TIME:      return get_time_trap();
        case SHOW_TIME:     return show_time_trap();
        case GET_CLOCK:     return get_clock_trap();
        case DIF_CLOCK:     return dif_clock_trap();
        case FMT_STR:       return format_str_trap();
        default:
            runtimeError("unhandled trap number: %04X", tno);
            return true;
    }
    return false;
}
