#Common
These are the common routines that bind all of the executables in this project together. They are designed to all be compatible and reusable.

## Cmdline
The generic command line parser. This is a stand-alone functionality for the most part.

## Errors
The error handling and string formatting routines that it uses.

## Instruction buffer
This set or routines handles the actual instruction stream. It implements a byte stream that can be written or read as arbitrary sized chunks. It also handles serializing the instruction stream.

## Opcodes
This module defines the VM opcodes and allows the error and disassembler functions to print them.

## Registers
This module defined the VM registers.

## String Table
THis module implements the string storage strategy.

## String Utilities
This module implements the various things that can be done to strings, including string expansion to variables.

## Value Table
This module implements storage of values in the assembler and the VM.
