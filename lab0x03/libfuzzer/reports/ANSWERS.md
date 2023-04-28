1. You did not change `is_png_chunk_valid` in this lab, does it affect the performance of libFuzzer's default mutator? What about the example PNG mutator and your custom YOLO PNG mutator?
Yes it do affect the default mutator's performance, because the default mutator can not be aware of the PNG format, and still will mutate the input into format which can't pass valid check. 
For the other two mutators it won't affect. Because the libfuzzer is Structure-Aware Fuzzing and when we add the example PNG mutator and custom mutator, we inform the fuzzer of the possible mutations within the valid input space, so our input should pass the png chunk valid check now.

2. How many PCs are covered within `100000 fuzzing runs` with your `fuzzer_store_png_rgba`, equiped with libFuzzer's default mutator, the example PNG mutator, and your custom YOLO PNG mutator separately? Do these numbers reflect the extent to which `store_png` has been teseted?


3. Do you think the mutator you customized (that aims at only generating well-formatted YOLO PNG inputs) helpful for finding bugs in `load_png`? Why?
It will help to some extend, because it generate valid YOLO inputs, and will make our tests dive deeper, and will also made our tests more efficient.
However it's not sufficient to only generate valid YOLO PNG format, because real world senario doesn't only contains valid PNG input, and if we are not successfully dealing with invalid input, the bug can't be triggered using this customized mutator.