# Assembler

This is the assembler and virtual machine for the Goldfish language. The idea
is to create an assembler and virtual machine, along with support programs that
will allow clear definition of any construct. It should be as simple as
possible and still allow for robust programming. The Goldfish compiler will
compile to assembly for this assembler and VM.

The assembler makes several passes on the source code in order to allow for
things like referencing labels before they are defined.
- Multiple passes over the AST allows for optimizations.
- Garbage collection is implemented using the Boehm GC library. (https://en.wikipedia.org/wiki/Boehm_garbage_collector)
- Robust string formatting and substitutions are supported.
- Running arbitrary C code is supported through a system of traps.
- The output of the assembler is completely serialized.
- A C preprocessor is provided with this source code, but any one will work.