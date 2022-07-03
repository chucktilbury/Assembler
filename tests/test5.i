#line 1 "test5.s"
 


#line 1 "traps.inc"

 









#line 5 "test5.s"

int value1 = 1234
string value2 = "ploop"
string formatted = "this is a {value1} string with a {value2} and no nl"

_start:
    load r0, formatted
    trap  2 
    trap  0 
    exit

