# Move-semantics

This project serves several purposes. Primarily, it provides some useful classes for debugging construction/destruction issues; however, it also serves as an example of C++11 move-sematics coding. Here is a description of the files:

FILENAME         | DESCRIPTION
--------         | -----------
`CMakeLists.txt` | Build targets to self-test every header
`debug.hpp`      | macros to display debugging messages with verbosity controls
`expect.hpp`     | macros to test expectations
`noisy1.hpp`     | basic tracking of constsruction/destruction
`noisy2.hpp`     | fancier version that with more information
`print.hpp`      | convenience macros for output (select from iostream, printf or fmt
`to_string.hpp`  | convenience converts containers to strings
`uniqueid.hpp`   | provide serial numbers for classes - used in `noisy2.hpp`
usage.cpp | testing and usage 

The easiest way to learn how to use these is to look into the files themselves. See the `usage.cpp` for examples.
