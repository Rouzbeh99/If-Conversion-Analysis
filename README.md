# If-Conversion Analysis

If-Conversion is an LLVM transformation that converts if-then-else statements into predicated code. In this project we investigate its effect on cache behaviour and branch predictor unit.

## Step by step guide to reproduce results:

### Prerequisites

You need followings installed on your system:

- [LLVM](https://llvm.org/docs/GettingStarted.html)
- [Clang](https://llvm.org/docs/GettingStarted.html)
- [Perf](https://www.brendangregg.com/perf.html)
- [Python](https://www.python.org/downloads)

### Running static code analyzer
- Open /test/Compiled_files/run_analysis.sh file
- Change LLVM_BUILD_DIR variable to point to LLVM installation direcory
- By default it will be executed on dikjesta.c file, if you want to test ona another file, change FILENAME variable
- Save changes and execute run_analysis.sh
