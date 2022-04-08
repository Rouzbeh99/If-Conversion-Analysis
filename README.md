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

### Running analysis on if-converted version
- run /test/Compiled_files/run_perf_on_full_opt_version.sh file
- results will be saved in results directory

### Running analysis on version without if-conversion

To compile the code with if-conversion disabled, you need to make following changes in your LLVM library files:

- in the file [https://github.com/llvm/llvm-project/blob/main/llvm/include/llvm/Transforms/Utils/SimplifyCFGOptions.h#L32](https://github.com/llvm/llvm-project/blob/main/llvm/include/llvm/Transforms/Utils/SimplifyCFGOptions.h#L32) change FoldTwoEntryPHINode to false  at line 32
- in the file [https://github.com/llvm/llvm-project/blob/main/llvm/lib/Transforms/Scalar/SimplifyCFGPass.cpp#L402-L403](https://github.com/llvm/llvm-project/blob/main/llvm/lib/Transforms/Scalar/SimplifyCFGPass.cpp#L402-L403) comment line 402
- run /test/Compiled_files/run_perf_on_no_select_version.sh file
- results will be saved in results directory
