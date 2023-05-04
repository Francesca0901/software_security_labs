1. You did not change `is_png_chunk_valid` in this lab, does it affect the performance of libFuzzer's default mutator? What about the example PNG mutator and your custom YOLO PNG mutator?
Yes it do affect the default mutator's performance, because the default mutator can not be aware of the PNG format, and still will mutate the input into format which can't pass valid check. 
For the other two mutators it won't affect. Because the libfuzzer is Structure-Aware Fuzzing and when we add the example PNG mutator and custom mutator, we inform the fuzzer of the possible mutations within the valid input space, so our input should pass the png chunk valid check now.

2. How many PCs are covered within `100000 fuzzing runs` with your `fuzzer_store_png_rgba`, equiped with libFuzzer's default mutator, the example PNG mutator, and your custom YOLO PNG mutator separately? Do these numbers reflect the extent to which `store_png` has been teseted?
- default: 99
- PNG: 114
- YOLO PNG: 100
Yes, the higher the number is, the more paths we have tested, the more extensively we have tested. 
However, in the previous answer I have found more path constrains through PNG_mutator, because it generate more structured PNG input and is more likely to enter certain deep paths, instead of directly lead to those error handling routes.
The reason my YOLO PNG mutator triggered less PCs might because the variety of generated input is limited, and won't discover certain paths.

3. Do you think the mutator you customized (that aims at only generating well-formatted YOLO PNG inputs) helpful for finding bugs in `load_png`? Why?
It will help to some extend, because it generate valid YOLO inputs, and will make our tests dive deeper, and will also made our tests more efficient.
However it's not sufficient to only generate valid YOLO PNG format, because real world senario doesn't only contains valid PNG input which will pass through "success" path, and if we are not successfully dealing with invalid input, the bug can't be triggered using this customized mutator.