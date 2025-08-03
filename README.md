# MilyVM
An mlog emulator written in C++

# Build
## Linux:
1. gcc is required, but is most likely already installed
2. Run bash.sh
3. The build output `milyvm` should appear in the .out directory

# Running
## Linux
1. cd to the location of `milyvm`
2. run `./milyvm <file.mlog>`, e.g. `./milyvm ../tests/main mlog_`
## Run Flags
- `-v` - Verbose mode. Prints out the state of execution: variable states and the current line. Slows down execution by a large amount.