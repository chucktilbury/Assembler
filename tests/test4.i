#line 1 "test4.s"

 


#line 1 "traps.inc"

 









#line 6 "test4.s"

string the_value = "this is a string"
float test_value = 143.3487

_start:
    nop
    load r0,test_value
    load r1,the_value
    call the_func
    trap  0    
    exit

 
 
the_func:
    push r0      
    load r0,r1   
    trap  0 
    pop r0       
    return

