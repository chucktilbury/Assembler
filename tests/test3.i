#line 1 "test3.s"

 


#line 1 "traps.inc"

 









#line 6 "test3.s"

test3:
    load r5, 123
    load r6, 2
    mul  r0,r5,r6
    trap  0 
    exit

