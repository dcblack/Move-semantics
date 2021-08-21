#pragma once

/** @brief Conveniences for printing
 *
 *  - none of the following are likely useful beyond testing/educational value
 *  - these illustrate three types of output because sometimes we don't have access to fmt library
 *
 *  Macro           | Description
 *  -----           | -----------
 *  USE_IOSTREAM    | define if you want to use traditional C++ I/O
 *  USE_PRINTF      | define if you want to use C-style printf
 *  NOISE( "text" ) | Displays the object address and a text message
 *  INFO( "text" )  | Displays a simple informational statement tagged as "Info:"
 *  SHOW( expr )    | Displays expression text and its resultant value
 *  SHOW_PTR( p )   | Displays a pointer address
 *  ECHO( code )    | Raw echo with line number
 *  DO( stmt )      | Echos and executes a statement with line number
 *  __________;     | Displays a horizontal line
 */
namespace {
  const int line_width = 40;
}
#ifdef USE_IOSTREAM
    #include <iostream>
    #define NOISE(mesg)   std::cout << this << ": " << mesg << '\n'
    #define INFO(mesg)    std::cout << "Info: " << mesg << '\n'
    #define SHOW(expr)    std::cout << #expr << " = " << expr << '\n'
    #define SHOW_PTR(PTR) std::cout << #PTR << " = @" << PTR << '\n'
    #define ECHO( expr )  std::cout << __LINE__ << ": " << #expr << '\n'
    #define DO( expr )    std::cout << __LINE__ << ": " << #expr << '\n'; expr
    #define __________    std::cout << std::string(line_width,'_') << '\n'
#elif defined(USE_PRINTF)
    #include <cstdio>
    #define NOISE(mesg)   std::printf("%p: %s\n", this, mesg)
    #define INFO(mesg)    std::printf("Info: %s\n", mesg)
    #define SHOW(expr)    std::printf("%s = %lu\n", #expr, static_cast<uint32_t>(expr))
    #define SHOW_PTR(PTR) std::printf("%s = @%p\n", #PTR, PTR)
    #define ECHO( expr )  std::printf("%d: %s\n", __LINE__, #expr )
    #define DO( expr )    std::printf("%d: %s\n", __LINE__, #expr ); expr
    #define __________    std::printf("%s\n", std::string(line_width,'_').c_str())
#else
    #include <string>
    #include <fmt/format.h>
    #define NOISE(mesg)   fmt::print("{}: {}\n", fmt::ptr(this), mesg)
    #define INFO(mesg)    fmt::print("Info: {}\n", mesg)
    #define SHOW(expr)    fmt::print("{} = {}\n", #expr, expr)
    #define SHOW_PTR(PTR) fmt::print("{} = @{}\n", #PTR, fmt::ptr(PTR))
    #define ECHO( expr )  fmt::print("%d: %s\n", __LINE__, #expr ); expr
    #define DO( expr )    fmt::print("%d: %s\n", __LINE__, #expr ); expr
    #define __________    fmt::print("{}\n", std::string(line_width,'_'))
#endif
