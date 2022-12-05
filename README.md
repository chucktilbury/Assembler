# Goldfish

Goldfish is a C-like, multi-pass, compiled programming language that runs on its own virtual machine. The structure of the suite is very old-school in that the compiler compiles to assembler, then the assembler writes a binary which the VM uses. These stages of compilation are all totally separate and independent, making is possible to write assembly code for the VM without using the compiler at all. This makes the code more simple and also aids in debugging, but does not diminish runtime performance. It also allows the distribution of portable binary executable files instead to distributing the source code as required by some interpreted languages. 

The design goals of the Goldfish language are simplicity and efficiency. The language is very simple to learn and read but also robust and complete. Every effort is made to make coding shortcuts superfluous and to have exactly one way to accomplish some specific task. The design of the compiler and other infrastructure is to be very extensible. Extensions can be easily written and compiled in using C. The internal data structures of the assembler are very simple and easy to understand and a minimum of code needs to be written in Goldfish to interface an extension.  Internally extensions are implemented by the notion of a trap, which is a very clean and efficient way to pass state to the external library and receive state back upon completion of the call. The goal of that is to have as little code as possible run in the VM and as much as possible run natively.

Goldfish source code is structured to be processed using a dedicated C preprocessor. The preprocessor can handle macros and includes as expected. Instead of linking goldfish modules together, the preprocessor ``#include``statement is to bring in other modules. Naming issues are resolved using namespaces. 

## Comments

Comments are the save as C. Both C and C++ comments are acceptable. All comments are stripped out by the preprocessor.

```C
/*
 	This is a comment
 */

// So is this
```



## Namespaces

A namespace can appear anywhere in code that is outside of any other definition except a namespace. When name spaces are nested, then the namespace name must be used to access symbols that are used within the nested namespace. They define a naming prefix that is intended solely for the purpose of resolving name conflicts. When a name is defined within a namespace, the namespace name does not need to be specified and if a name is duplicated within a namespace then that is a syntax error. When something appears outside of any namespace, then the default namespace is used, which is ``root``.  A namespace does not name a type and you cannot refer to it as a variable.

```C
number something = 0
namespace some_name {
    number something = 123
    // stuff goes here
    // The name "something" refers to some_name.something, whereas the 
    // name "root.something" referrs to the one defined outside of the name space.
    namespace another {
        number something = 234
        // something refers to "root.some_name.another.something"
        // and so on
    }
    // the name something here refers to root.some_name.something.
} // semicolons are treated as whitespace
```

## Structs

A struct names a simple type that does not have a constructor or destructor. It can appear anywhere in a module, including inside another struct. The syntax of using a struct is a same as any other type and creating an instance of a struct defines the contents of it as cleared unless an initilizer is provided. A struct can have function pointers but not function definitions.

```C
struct something {} // empty struct
namespace some_name {
    something another // same as "root.something another" and defines an instance
    struct something {  // defines a local type of struct "something". This changes the
    					// meaning of the previous line 
        struct another {
            // define a nested struct
        }
}
some_name.something var // this refers to the on in the name space.
```

## Classes

A class is a more complicated struct that has constructors and destructors.  It names a type that, when an object of that type is created or destroyed, a specific function is called. Single inheritance is supported. Monkey patching is specifically not supported. The object model for Goldfish is similar to Python where to override a method or attribute, you simply define it for the child class.  To access the parent version of a method or attribute, simply use the class name. Classes may be nested, but the preferred idea is to use a struct and initialize it explicitly.

```c
// define a class in the root namespace
class outside {
    // state and methods as usual
}

namespace a_name {
    // define a class inside a namespace
    class inside(outside) {
        // the contents of outside are accesible as if they are local and defining
        // a duplicate name overrides the definition in outside.
    }
}
```

## Methods

A method is a construct that holds executable code. It can appear anywhere in the source and adheres to the naming conventions described above. See examples for contents of a method. Methods return a value of a particular type (or nothing) and the parameters of a method are strongly typed. Method names, including those defined outside of a class are decorated with the parameter types to support overriding the method name. 

```C
...
// general syntax
return_type_name method_name(method parameters) { method contents }
...
```





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
- Run the setup script: ```./setup <build type>``` where build type is debug, release, or profile.
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

There are many other build and run options. Look in the source code and in the CMakeLists.txt files. All of the utilities also support a help command parameter as ```-h```.

For the current plan and other information, including random thoughts and probably wrong thinking, then take a look at the notes.md file in this directory.
