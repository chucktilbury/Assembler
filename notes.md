# general notes

- Change command line handler to better library. (WIP: bug fixes required)
  - Add command line flags for mutual excl or reqd and groups.
  - Add command line flags to control separator requirements.
  - Add command line option to warn extra files in the file list.
- Use keyword for entry point of the program.
  - Default to emit a jump to the entry point at the beginning of a program.
  - Command line option to turn it off.
- Store strings as a static list in the executable, but handle them as pointers
  at run time to facilitate garbage collection.
- Think about storing debug info, including symbol table.
- Generate the system.inc file at build time from source code.
- Add register and immediate variants for PUSH and STORE.

# current task
- Tests.

# finished
- Add a "magic" number at beginning of binary file to make the VM fail on an invalid input.
- Save file name at beginning of the binary for error messaging in VM.
- Turn the ABORT instruction into a trap.
- Move traps to it's own library in the runtime directory.
  - Traps should accept stack params instead of dedicating a register by
    convention.
