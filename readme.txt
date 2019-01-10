This is meant to improve poor tours. Segments are sorted by size, and first-improvement 2-opt is run on the largest segments.
Initially, the work complexity is probablistically O(n). But as the local optimum is reached, the runtime will approach O(n ^ 2).

This is intended to initialize tours for further optimization with fast-2-opt, which can be slow on poor tours (segments are less likely to placed in deep nodes).

Compilation:
1. Make sure "CXX" in "makefile" is set to the desired compiler.
2. Run "make".

Running:
1. Run "./2-opt.out" for usage details.

Style notes:
1. Namespaces follow directory structure. If an entire namespace is in a single header file, the header file name will be the namespace name.
2. Headers are grouped from most to least specific to this repo (e.g. repo header files will come before standard library headers).
3. Put one line break in between function definitions for convenient vim navigation via ctrl + { and ctrl + }.

TODO:
1. Check if output directory exists before writing out paths to file.
