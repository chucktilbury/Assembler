
#include "common.h"
#include "vmachine.h"

/*
 * String to format is on top of the stack and the result is pushed on the
 * stack when done.
 *
 * TODO: Strings should be handled as pointers at runtime and stored in values
 * like any other value. When the program is serialized, then strings have to
 * be stored as a static buffer. How does this interface work?
 */
bool format_str_trap()
{
    Value inv = popVal();
    Value outv;

    outv.type = STRING;
    if(inv.type == STRING)
        outv.data.str = addStr(formatStr(getStr(inv.data.str)));
    else
        outv.data.str = addStr(valToStr(&inv));

    pushVal(outv);

    // if(registers[1].type != STRING) {
    //     registers[0].type = STRING;
    //     registers[0].data.str = addStr(valToStr(&registers[1]));
    // }
    // else {
    //     registers[0].type = STRING;
    //     const char* str = getStr(registers[1].data.str);
    //     registers[0].data.str = addStr(formatStr(str));
    // }

    return false;
}
