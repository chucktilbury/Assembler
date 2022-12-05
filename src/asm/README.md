# ASM
This is a simple stand-alone utility that binds the pre-processor and the assembler together. It is only used when actually writing code in the VM assembly language.

```
use: asm <parameters> <file(s)>
This is the assembler driver program.
It coordinates the operation of the preprocessor,
the assembler, and the virtual machine.
-o <str> output file name
-p <str> output file path
-r ----- run the result in the VM
-k ----- keep intermediate file(s)
-v <num> verbosity level
-D <str> preprocessor definition
-I <str> prepend preprocessor include path
-J <str> append preprocessor include path
-h ----- display help information
```
