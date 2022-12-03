

# TODO list

- Rework things like strings and numbers to be objects in the assembler. Calls serialization into question. Save instances of arbitrary objects.
- Update all documentations
- Think about storing debug info, including symbol table. Probably store the debug information in a separate file. Just store all of the information that the assembler throws away when it writes the binary.
- Startup code needs to capture the command line. (requires lists)
- Implement equality test and concatenation for strings, as well as other string services, as traps.
- Implement simple CLI debugger.
  - Step
  - Continue
  - Quit
  - Load
    - Program
    - Symbols
    - Both
    - State
  - Store
    - Settings
    - Program state
  - Show
    - Variables
      - By index
      - By name
      - As a list, using less
    - Breakpoints
      - By index
      - As a list, using less, with indexes
    - Source
      - As single line
      - As block of lines
        - From center at current line
        - From current line down
    - Registers
      - Single
      - All
    - Help
      - As text, using less
      - By topic
  - Breakpoints
    - Set by address
    - Set by label name
    - Set by source code line number
    - Set by value change
    - Set by register change
    - Clear all
    - Clear by index
  - Set
    - Number of lines to show for source and lists.
    - Whether to sho meta-data for variables.
    - Save settings.
- Looking at using pointers for Values instead of indexes at runtime and only using integer indexes when serializing the object file.
- Write the assembler user manual.

# Current task
- Tests.

# Questions
- Needs to create and destroy variables in ASM code in order to support high level code. Examples are class objects and local variables. This can mostly be done with traps, but creating class methods and such has to be in ASM code. What does this look like? (see converting to objects in the TODO list)
- Need to think about exception handling. Thinking about a try{} stack. In emitted code, the handlers are defined before the try block and the try is like a setjmp() destination and the handlers are like an upsides down if/else construct.
- ~~Look at keeping pointers in uint32_t instead of uint64_t sized memory object.~~ This could save time when manipulating pointers by reducing the amount of data that is read from memory when executing a data stream. However, it also requires that a pointer be reconstituted when it is used. Some sort of caching could be implemented, but this also breaks garbage collections and requires a special implementation. Rejecting the feature.
- ~~See if a Value data structure will fit into a uint64_t.~~ The only way this can work is to use truncated pointers as indicated in the previous bullet point. Since that feature is rejected, then this feature is also rejected.
- This cannot be made to work. ~~Look at changing the VM so that it loads variables as pointers instead of indexes, for efficiency at runtime.~~ The problem is with linking the pointer to actual instructions, as instructions are loaded as a single block. Note that addresses in Linux are 48 bits where the lower 32 bits are the actual index and the upper 16 bits are the segment selector. The segment selector does not change for the life of the program, so this lower 32 bits can be used as an index. Variable indexes are already 32 bits in the VM. There are implications for the object model...

# Finished TODO items
- Merge the garbage collector to this tree. ~~Fix the build so that it builds with the rest of the code~~.
- Merge the goldfish tree to this tree.
- Delete INTEGER and UNSIGNED types and replace with NUMBER, which is always a double. This is a fairly sweeping change that will require changes to most files.
- Add a "magic" number at beginning of binary file to make the VM fail on an invalid input.
- Save file name at beginning of the binary for error messaging in VM.
- Turn the ABORT instruction into a trap.
- Move traps to it's own library in the runtime directory.
  - Traps should accept stack params instead of dedicating a register by
    convention.
- Store strings as a static list in the executable, but handle them as pointers
  at run time to facilitate garbage collection.
- Use keyword for entry point of the program.
  - Default to emit a jump to the entry point at the beginning of a program.
  - Command line option to turn it off.
- Moved startup code to include file.
- Change command line handler to better library. (WIP: bug fixes required)
  - ~~Add command line flags for mutual excl or reqd and groups.~~
  - ~~Add command line flags to control separator requirements.~~
  - Add command line option to warn extra files in the file list.
- Add PUSH instruction for immediate and variables.
- Add STORE instruction for immediate.
- Add BREAK instruction that is a hard break in the debugger and a NOP in normal code.
- Figure out how to do different builds with cmake!!!
- Add break instruction that does a hard break in the debugger, but is a NOP in the regular VM.

# Other thoughts:
- Traps: there is a notion of a trap and sub-trap. Currently, the trap number is a uint16_t, but with sub-traps, maybe that should be 2 uint8_t instead. There would be a total of 256 trap numbers and 256 sub-traps for each one of them. Or maybe move a bit over so that there are 512 traps and 128 sub-traps.

- The assembler needs to support creating objects and manipulating them via assembly code. Since methods are non-mutable, they can go into the main code stream with the name mangles to show which class they belong to. When an object is created, a normal Value is created with a type of Object. The object contains a table that points to the methods that are defined for it, as well as a normal list of vars. These vars are only accessible by the methods that are defined for the object and the methods access these vars by default. Have to decide how shadowing a global works. The classes are created by the compiler, but objects (of course) are created at run time. This is done using a trap. Since object values are never serialized (except by explicitly defined code) there is no need to keep them in an indexed array. They can be accessed directly using pointers. An object can contain any other type, including other objects.
  - When a object is created, it is issued a trap number. The trap number is then used to access the methods, which are implemented as sub-traps. ???
  - The default services can be overridden with user defined methods. Default Services for objects:
    - toString Default conversion
    - isEqual Texts if it is the same object.
    - Note that a default constructor is called automagically, and a user defined constructor can be called after that.

# Traps
A trap is a software construct where the program is made to enter a pre-defined thread of execution. In the Goldfish VM, traps are used to execute code that was written and compiled in C.

Strings, lists, and dicts are not objects. They are treated as native types for the sake of efficiency.

## Strings
Are implemented as a native type and manipulated with traps.
### Services
- Create empty
- Create with value
- Length
- Compare
- Catenate
- Format
- Replace
- Strip
- Substring
- Convert to int
- Convert to uint (i.e. 0x0000)
- Convert to float
- SubString
- Iterate (by characters)

## Lists
Implemented as a native type using traps to implement services. A list can contain any other type, including lists and objects. Lists use a counting number (signed or unsigned) as an index. A negative index accesses the list from the end.
### Services
- Create empty list
- Append
- Insert
- Index (aka, Get)
- Remove entry
- Length
- Assign existing entry
- Reset iterator
- Iterate

## Dicts
Implemented as a native type using traps. A dict can contain any other type of Value. Dicts are indexed using a string and are stored in no particular order. Internally they are stored in a simple binary tree.
### Services
- Create empty
- Add
- Get
- Remove entry
- Size
- Assign existing entry
- Reset iterator
- Iterate
- Iterate lexically

## Objects
Implemented as user defined types using traps. An object is implemented using a data structure that has all of the variables that are defined for the state of the type as well as a table of call addresses for the methods that the type implements. Inheritance is implemented by making changes to the table. All method implementations are compiled into the main instruction stream with name mangling to differentiate different classes. Methods and variables are accessed using traps. The object data structure keeps these things in lists and access is accomplished using indexes into those lists. Provision is made to allow "monkey patching" of objects. The variables are accessed using a trap and the trap number is a sub-sub-trap number.
### Services
- These services cannot be overridden.
  - Create object
  - Create var
  - Create method
  - Get Var (+ var number)
  - Set Var (+ var number)
  - Call method (+ method number)
- These services may be over ridden.
  - Compare (for same object by default)
  - toString (object name, or override)
- Numbers for user-defined services are started after this.

# System traps (planned)
- Error handling
- Strings
- Printing
- TIME
- File streams
- Math functions
- File system (i.e. glob(), etc.)
- FLTK (GUI interface)
- Networking (incl. web services?)
- ZLIB
- SQLITE
- Curses??
- Regular expressions would be nice.

# Type conversion matrix
This is a matrix that describes all type conversions for binary arithmetic operations. Unary arithmetic operations result in the type of the operand or an ERROR. All binary and unary comparison operations result in a boolean value. Valid types are ERROR, NOTHING, NUMBER, STRING, BOOLEAN, LIST, HASH, and USER. The USER type is defined by the user and the resulting type is always USER. In cases where the user desires the result to be a native type, then they must provide the conversion method.

Note that HASH and LIST types "hold" other Values and the type of those values is used in the matrix.

|         | ERROR | NOTHING | NUMBER     | STRING     | BOOLEAN    | LIST       | HASH       | USER    |
| ------- | ----- | ------- | ---------- | ---------- | ---------- | ---------- | ---------- | ------- |
| ERROR   | ERROR | ERROR   | ERROR      | ERROR      | ERROR      | ERROR      | ERROR      | ERROR   |
| NOTHING | ERROR | NOTHING | ERROR      | ERROR      | ERROR      | ERROR      | ERROR      | USER(5) |
| NUMBER  | ERROR | ERROR   | NUMBER     | STRING     | BOOLEAN(1) | LIST(3)    | HASH(3)(4) | USER(5) |
| STRING  | ERROR | ERROR   | STRING     | STRING     | BOOLEAN(2) | LIST(3)    | HASH(3)(4) | USER(5) |
| BOOLEAN | ERROR | ERROR   | BOOLEAN(1) | BOOLEAN(2) | BOOLEAN    | LIST(3)    | HASH(3)(4) | USER(5) |
| LIST    | ERROR | ERROR   | LIST(3)    | LIST(3)    | LIST(3)    | LIST(3)    | HASH(3)(4) | USER(5) |
| HASH    | ERROR | ERROR   | HASH(3)(4) | HASH(3)(4) | HASH(3)(4) | HASH(3)(4) | HASH(3)(4) | USER(5) |
| USER    | ERROR | USER(5) | USER(5)    | USER(5)    | USER(5)    | USER(5)    | USER(5)    | USER(5) |

(1) If the number is not **exactly** zero then it's boolean TRUE. (2) A string is always boolean TRUE. (3) Addition only. (4) Must supply a text key. (5) User must supply implementation for supported operator. Default type result is ERROR.

Also note that casting is supported. To find the result of a cast, then find the type casted TO on the top row and then find the type of the value to cast in the left column and take the intersection.

|         | ERROR | NOTHING | NUMBER    | STRING    | BOOLEAN    | LIST    | HASH    | USER    |
| ------- | ----- | ------- | --------- | --------- | ---------- | ------- | ------- | ------- |
| ERROR   | ERROR | ERROR   | ERROR     | ERROR     | ERROR      | ERROR   | ERROR   | ERROR   |
| NOTHING | ERROR | ERROR   | ERROR     | ERROR     | ERROR      | ERROR   | ERROR   | ERROR   |
| NUMBER  | ERROR | ERROR   | NUMBER    | STRING    | BOOLEAN    | LIST(1) | ERROR   | USER(2) |
| STRING  | ERROR | ERROR   | NUMBER    | STRING    | BOOLEAN    | LIST(1) | ERROR   | USER(2) |
| BOOLEAN | ERROR | ERROR   | NUMBER    | STRING    | BOOLEAN    | LIST(1) | ERROR   | USER(2) |
| LIST    | ERROR | ERROR   | ERROR     | ERROR     | ERROR      | LIST    | ERROR   | USER(2) |
| HASH    | ERROR | ERROR   | ERROR     | ERROR     | ERROR      | LIST(3) | HASH    | USER(2) |
| USER    | ERROR | ERROR   | NUMBER(2) | STRING(2) | BOOLEAN(2) | LIST(2) | HASH(2) | USER(2) |

(1) Creates a list with one item in the it. (2) User must supply the conversion to the type. The default conversion is to ERROR. (3) Flatten the HASH into a LIST.
