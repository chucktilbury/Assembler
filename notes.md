# TODO list

- Think about storing debug info, including symbol table.
- Generate the system.inc file at build time from source code.
- Add variable and immediate variants for PUSH and a immediate variant for the STORE instruction.
- Runtime support for lists and dicts. (as traps)?
- Look at using more instructions that handle variable a variable index size.
- Implement equality test and concatenation for strings. (as traps?)
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
