#line 1 "test2.s"

 


#line 1 "traps.inc"

 









#line 6 "test2.s"

string thevar = "some string"

load r0, 0xdeadbeef
push r0
load r0, thevar
trap  0 
pop r0
trap  0 
addr_of_the_exit_instr:
exit

