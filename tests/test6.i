#line 1 "test6.s"
 





#line 1 "traps.inc"

 









#line 8 "test6.s"

uint time = 0
string time_str = ""
string str = "the time is {time_str}"


_start:
    trap  3 
    store time, R0
    trap  4 
    trap  0 
    store time_str, R0
    load r0, str
    trap  2 
    trap  0        
    exit

