#pragma once

// Convenience for printing
namespace {
  const int line_width = 40;
}
#ifdef USE_IOSTREAM
    #include <iostream>
    #define NOISE(mesg)   std::cout << this << ": " << mesg << '\n'
    #define INFO(mesg)    std::cout << "Info: " << mesg << '\n'
    #define SHOW(expr)    std::cout << #expr << " = " << mesg << '\n'
    #define SHOW_PTR(PTR) std::cout << #expr << " = @" << PTR << '\n'
    #define DO( expr )    std::cout << __LINE__ << ": " << #expr << '\n'; expr
    #define __________    std::cout << std::string(line_width,'_') << '\n'
#elif defined(USE_PRINTF)
    #include <cstdio>
    #define NOISE(mesg)   std::printf("%p: %s\n", this, mesg)
    #define INFO(mesg)    std::printf("Info: %s\n", mesg)
    #define SHOW(expr)    std::printf("%s = %lu\n", #expr, static_cast<uint32_t>(expr))
    #define SHOW_PTR(PTR) std::printf("%s = @%p\n", #PTR, PTR)
    #define DO( expr )    std::printf("%d: %s\n", __LINE__, #expr ); expr
    #define __________    std::printf("%s\n", std::string(line_width,'_').c_str())
#else
    #include <string>
    #include <fmt/format.h>
    #define NOISE(mesg)   fmt::print("{}: {}\n", fmt::ptr(this), mesg)
    #define INFO(mesg)    fmt::print("Info: {}\n", mesg)
    #define SHOW(expr)    fmt::print("{} = {}\n", #expr, expr)
    #define SHOW_PTR(PTR) fmt::print("{} = @{}\n", #PTR, fmt::ptr(PTR))
    #define DO( expr )    fmt::print("%d: %s\n", __LINE__, #expr ); expr
    #define __________    fmt::print("{}\n", std::string(line_width,'_'))
#endif
