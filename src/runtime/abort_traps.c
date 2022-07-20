
#include "common.h"
#include "vmachine.h"

bool abort_trap()
{
    Value val = popVal();
    printf("\n\n");
    printVal(&val);
    printf("\n\n");

    return true;
}