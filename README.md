## Install

_For development on non-Linux_:
```bash 
# Clone the repository
git clone https://github.com/augustryder/Cache-Simulator.git

# Build the docker image
docker build -t cachelab-env .

# Run docker container with bash terminal
docker run -it --rm -v "$(pwd):/work" -w /work cachelab-env bash

```
Valgrind seems to run too slowly in the Docker container (or something), so to test the large matrices (64x64 and 61x67) I needed to use a real Linux machine.


## Running the autograders:

Before running the autograders, compile your code:
```bash
    linux> make
```

Check the correctness of your simulator:
```bash
    linux> ./test-csim
```

Check the correctness and performance of your transpose functions:
```bash
    linux> ./test-trans -M 32 -N 32
    linux> ./test-trans -M 64 -N 64
    linux> ./test-trans -M 61 -N 67
```

Check everything at once (this is the program that your instructor runs):
```bash
    linux> python3 ./driver.py    
```

## Files:

### You will modifying and handing in these two files
- csim.c       Your cache simulator

- trans.c      Your transpose function

### Tools for evaluating your simulator and transpose function
- Makefile:     Builds the simulator and tools

- README.md:    This file

- driver.py*:   The driver program, runs test-csim and test-trans

- cachelab.c:   Required helper functions

- cachelab.h:   Required header file

- csim-ref*:    The executable reference cache simulator

- test-csim*:   Tests your cache simulator

- test-trans.c: Tests your transpose function

- tracegen.c:   Helper program used by test-trans

- traces/:      Trace files used by test-csim.c
