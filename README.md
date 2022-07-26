# Assembler

This is a register based virtual machine and the assembler for the Goldfish language. The idea is to create an assembler and virtual machine, along with support programs that will allow clear definition of any construct. It should be as simple as possible and still allow for robust programming. For efficiency and speed, only the most basic functions are implemented in assembly. As much functionality as possible is moved out to native code. So the actual assembly language is limited to flow control, arithmetic, and the native code interface. The Goldfish compiler will compile to assembly for this assembler and VM.

The assembler makes several passes on the source code in order to allow for things like referencing labels before they are defined.
- Multiple passes over the AST allows for optimizations.
- Garbage collection is implemented using the Boehm GC library. (https://en.wikipedia.org/wiki/Boehm_garbage_collector)
- Robust string formatting and substitutions are supported.
- Running arbitrary C code is supported through a system of traps.
- The output of the assembler is completely serialized.
- A C preprocessor is provided with this source code, but any one will work.

The only external requirement is the garbage collector.

Linux is the only supported operating system for now. The system has not been compiled under Windows or MacOS. There are no plans to support them, since I don't have those systems.

# Build
- Clone the repository
- Edit the .project_env file so that your source code directory is represented in the _PROJECT_LOC environment variable.
- Run the setup script.
- Run make from the build directory.

# Run
There are several programs.
- cpp - A dedicated C language pre-processor. This step is required for the test programs. We use it mostly to create macros and hide the comments.
- gasm - The assembler that takes pre-processed text and outputs a binary file for the virtual machine. This is a work in progress. It should be impossible to create incorrect output but there are generation errors that the assembler does not catch. That having been said, correct input produces correct output. (as far as I can tell)
- disasm - Decompile a binary that was output by the assembler.
- vmachine - The virtual machine. This runs the binary that was created.
- asm - This is a simple shell that builds and runs the program.

Using these utilities is intended to be easy, but there are many options. The "-v" option is for verbosity and allows quite a bit of introspection of these applications. Note that if you actually have used the setup script, these utilities will be in your path.

## Build and run the fibonacci benchmark the hard way.
- ```cd ../tests```
- ```../bin/cpp -o fibonacci.i fibonacci.s -I../src/runtime```
- ```../bin/gasm -o fibonacci.bin fibonacci.i```
- ```../bin/vmachine fibonacci.bin```

## Build and run the easy way
(assuming you have used the setup utility)
- ```asm -r fibonacci.s```

## Build and run, with seeing a lot of state information from the assembler.
- ```asm -r -v20 fibonacci.s```

There are many other build and run options. Look in the source code and in the CMakeLists.txt files.

For the current plan and other information, including random thoughts and probably wrong thinking, then take a look at the notes.md file in this directory.
