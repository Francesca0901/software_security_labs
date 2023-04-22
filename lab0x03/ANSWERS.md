# Why did you need to change `is_png_chunk_valid`?
Because this is AFL++ in “Dumb” Mode, and it uses mutation-based fuzzing to generate input data that is likely to trigger bugs in the target binary, by making random modifications to the input files in *alf_in*. Not all of the modifications meet the requirment of is_png_chunk_valid, to keep our blackbox fuzzer working, we just remove the input format check.

# Why did you have to use `afl-clang` to compile the source (and not e.g. ordinary clang)?
To instrument the target, we need to select a compiler. As said in [Fuzzing with AFL++](https://github.com/AFLplusplus/AFLplusplus/blob/stable/docs/fuzzing_in_depth.md#a-selecting-the-best-afl-compiler-for-instrumenting-the-target), AFL++ comes with a central compiler afl-cc that incorporates various different kinds of compiler targets and instrumentation options. 
We are using a symlink to afl-cc: afl-gcc, afl-g++, afl-clang, afl-clang++, afl-clang-fast, afl-clang-fast++, afl-clang-lto, afl-clang-lto++, afl-gcc-fast, afl-g++-fast to select the mode for the afl-cc compiler.

# How long did you fuzz? How many crashes in total did AFL++ produce? How many unique crashes?
I fuzzed for 24 min, 48 sec. 
total crashes : 47.5k
unique crashes : 14 **to be fix**

# Why are hangs counted as bugs in AFL++? Which type of attack can they be used for?
Hangs means that it has entered an infinite loop or is stuck waiting for some resource that is not available, and it's exact a unintended performance, or a *bug*.
They can be used to triggle DoS.

# Which interface of `libpngparser` remains untested by AFL++ (take a look at `pngparser.h`)?

