# TODO list

- Think about storing debug info, including symbol table.
- Startup code needs to capture the command line.
- Generate the system.inc file at build time from source code.
- Add variable and immediate variants for PUSH and a immediate variant for the STORE instruction.
- Runtime support for lists and dicts. (as traps)?
- Look at using more instructions that handle variable a variable index size.
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
- Add break instruction that does a hard break in the debugger, but is a NOP in the regular VM.
- Looking at using pointers for Values instead of indexes at runtime and only using integer indexes when serializing the object file.

# current task
- Tests.

# questions
- Needs to create and destroy variables in ASM code in order to support high level code. Examples are class objects and local variables. This can mostly be done with traps, but creating class methods and such has to be in ASM code. What does this look like?
- Need to think about exception handling. Thinking about a try{} stack. In emitted code, the handlers are defined before the try block and the try is like a setjmp() destination and the handlers are like an upsides down if/else construct.

# finished
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

# Other thoughts:
- Traps: there is a notion of a trap and sub-trap. The sub trap index is passed in R0 when the trap is entered.

- The assembler needs to support creating objects and manipulating them via assembly code. Since methods are non-mutable, they can go into the main code stream with the name mangles to show which class they belong to. When an object is created, a normal Value is created with a type of Object. The object contains a table that points to the methods that are defined for it, as well as a normal list of vars. These vars are only accessible by the methods that are defined for the object and the methods access these vars by default. Have to decide how shadowing a global works. The classes are created by the compiler, but objects (of course) are created at run time. This is done using a trap. Since object values are never serialized (except by explicitly defined code) there is no need to keep them in an indexed array. They can be accessed directly using pointers. An object can contain any other type, including other objects.
  - When a object is created, it is issued a trap number. The trap number is then used to access the methods, which are implemented as sub-traps. ???
  - The default services can be overridden with user defined methods. Default Services for objects:
    - toString Default conversion
    - isEqual Texts if it is the same object.
    - Note that a default constructor is called automagically, and a user defined constructor can be called after that.

- Strings, lists, and dicts are not objects. They are treated as native types for the sake of efficiency.

- Strings are implemented as a native type and manipulated with traps.
  - Services:
    - Create empty
    - Create with value
    - Compare
    - Catenate
    - Format
    - Substring
    - Convert to int
    - Convert to uint (i.e. 0x0000)
    - Convert to float
    - SubString
    - Iterate (by characters)

- Lists are implemented as a native type using traps to implement services. A list can contain any other type, including lists and objects. Lists use a counting number (signed or unsigned) as an index. A negative index accesses the list from the end.
  - Services:
    - Create empty list
    - Append
    - Insert
    - Index (aka, Get)
    - Remove entry
    - Length
    - Assign existing entry
    - Reset iterator
    - Iterate

- Dicts are implemented as a native type using traps. A dict can contain any other type of Value. Dicts are indexed using a string and are stored in no particular order. Internally they are stored in a simple binary tree.
  - Services:
    - Create empty
    - Add
    - Get
    - Remove entry
    - Size
    - Assign existing entry
    - Reset iterator
    - Iterate
    - Iterate lexically

# System traps
- File streams
- Math functions
- File system (i.e. glob(), etc.)
- FLTK (GUI interface)
- Networking (incl. web services?)
- ZLIB
- TIME
- SQLITE
- Curses??
- Regular expressions would be nice.
