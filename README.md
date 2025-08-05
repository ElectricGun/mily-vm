# MilyVM
An mlog emulator written in C++

# Building
## Linux:
1. gcc is required, but is most likely already installed
2. Run bash.sh
3. The build output `milyvm` should appear in the .out directory

# Running
## Linux
1. cd to the location of `milyvm`
2. run `./milyvm <file.mlog>`, e.g. `./milyvm ../tests/main.mlog`
## Run Flags
- `-v` - Verbose mode. Prints out the state of execution: variable states and the current line. Slows down execution by a large amount.
- `-bm` - Benchmark mode. Limits execution to 1 million instructions for measuring speed.