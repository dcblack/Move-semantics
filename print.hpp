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
 *  INFO( "text" )  | Displays a simple informational statement tagged as "Info:"
 *  SHOW( expr )    | Displays expression text and its resultant value
 *  SHOW_PTR( p )   | Displays a pointer address
 *  ECHO( "text" )  | Raw echo with line number
 *  DO( stmt )      | Echos and executes a statement with line number
 *  __________;     | Displays a horizontal line
 *  BLANK_LINE;     | Displays a blank line
 */
namespace {
  const int line_width = 80;
#ifndef NOCOLOR
  const char* BOLD="\033[01m";
  const char* UNDR="\033[04m";
  const char* CBLK="\033[30m";
  const char* CRED="\033[31m";
  const char* CGRN="\033[32m";
  const char* CYLW="\033[33m";
  const char* CBLU="\033[34m";
  const char* CMAG="\033[35m";
  const char* CCYN="\033[36m";
  const char* CWHT="\033[37m";
  const char* BBLK="\033[40m";
  const char* BRED="\033[41m";
  const char* BGRN="\033[42m";
  const char* BYLW="\033[43m";
  const char* BBLU="\033[44m";
  const char* BMAG="\033[45m";
  const char* BCYN="\033[46m";
  const char* BWHT="\033[47m";
  const char* NONE="\033[00m";
#else
  const char* NONE=""
  const char* BOLD=""
  const char* UNDR=""
  const char* CBLK=""
  const char* CRED=""
  const char* CGRN=""
  const char* CYLW=""
  const char* CBLU=""
  const char* CMAG=""
  const char* CCYN=""
  const char* CWHT=""
  const char* BBLK=""
  const char* BRED=""
  const char* BGRN=""
  const char* BYLW=""
  const char* BBLU=""
  const char* BMAG=""
  const char* BCYN=""
  const char* BWHT=""
#endif
}

#ifdef USE_IOSTREAM
  #include <iostream>
  #define INFO(mesg)    std::cout << CGRN << "Info: " << NONE << (mesg) << '\n'
  #define SHOW(expr)    std::cout << #expr << " = " << (expr) << '\n'
  #define SHOW_PTR(PTR) std::cout << #PTR << " = *" << (PTR) << '\n'
  #define ECHO( text )  std::cout << __LINE__ << ": " << (text) << '\n'
  #define DO( stmt )    std::cout << __LINE__ << ": " << #stmt << '\n'; stmt
  #define __________    std::cout << std::string(line_width,'_') << '\n'
  #define BLANK_LINE    std::cout << '\n'
#elif defined(USE_PRINTF)
  #include <cstdio>
  #define INFO(mesg)    std::printf("%sInfo: %s%s\n", CGRN, NONE, mesg)
  #define SHOW(expr)    std::printf("%s = %u\n", #expr, static_cast<uint32_t>(expr))
  #define SHOW_PTR(PTR) std::printf("%s = *%p\n", #PTR, PTR)
  #define ECHO( text )  std::printf("%d: %s\n", __LINE__, text )
  #define DO( stmt )    std::printf("%d: %s\n", __LINE__, #stmt ); stmt
  #define __________    std::printf("%s\n", std::string(line_width,'_').c_str())
  #define BLANK_LINE    std::printf("\n")
#else
  #include <string>
  #include <fmt/format.h>
  #define INFO(mesg)    fmt::print("{}Info: {}{}\n", CGRN, NONE, mesg)
  #define SHOW(expr)    fmt::print("{} = {}\n", #expr, expr)
  #define SHOW_PTR(PTR) fmt::print("{} = *{}\n", #PTR, fmt::ptr(PTR))
  #define ECHO( text )  fmt::print("%d: %s\n", __LINE__, text )
  #define DO( stmt )    fmt::print("%d: %s\n", __LINE__, #stmt ); stmt
  #define __________    fmt::print("{}\n", std::string(line_width,'_'))
  #define BLANK_LINE    fmt::print("\n")
#endif

//TAF! vim:nospell
