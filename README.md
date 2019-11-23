# LZW Decompression
Decompression algorithm for the LZW scheme, written over a couple of days over the summer as a friendly contest where I was interning. I was given a few test cases to run it against, which can be found here: https://mega.nz/#F!a5IwDS7C!A8c2f1G4-tAPGec0vXvMyg

It runs against the compressed text files expectedly fast (~128 milliseconds for the largest, 917KB uncompressed), but is dissapointingly slow against the larger video file in the test case (~31.258 seconds, 3.4GB uncompressed). I'm well aware it is RAM hungry, and I do have ideas for optimisation (particularly the dictionary/output) but ran out of time before implementing. 

Runtimes are those given by the std::chrono library, compiled with MSVC with the O2 option, and ran on a CPU @ 2.4GHz.
