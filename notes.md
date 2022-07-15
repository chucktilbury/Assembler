# general notes

- Add a "magic" number at beginning of binary file to make the VM fail on an invalid input.
- Save file name at beginning of the binary for error messaging in VM.
- Change command line handler to better library.
  - Add command line flags for mutual excl or reqd and groups.
  - Add command line flags to control separator requirements.
  - Add command line option to warn extra files in the file list.
- Use keyword for entry point of the program.
- Modify gasm so that it appends multiple files if specified on the command line.
