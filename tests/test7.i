#line 1 "/home/chuck/Src/goldfish/testing/asm/test7.s"



#line 1 "/home/chuck/Src/goldfish/src/runtime/traps.inc"











int plart = 0
uint bacon = 0
string passed = "test passed"
string failed = "test failed"




_start:
    NOP
goob:
ploob:


    load R0, passed
    push r0
    jmp destination1
    load r0, failed
    trap  0

destination1:
    pop r0
    trap  0

    jmp destination2
    load r0, failed
    trap  0

destination2:

    trap  0
int pazopr = 0

    jmp destination3
    nop nop
    load r0, failed
    trap  0
nop nop
destination3:
nop nop

    trap  0

_end:
    exit

