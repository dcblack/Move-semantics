#ifndef DEBUG_HPP

// Simple debug macro
// - Set DEBUG_LEVEL or accept default of 50
// - Call DEBUG(stream) or DEBUG(stream,level)
//   - where stream is an ostream without std::cout nor std::endl
//   - where if level <= DEBUG level then message is output
// - Example with one argument:  DEBUG("Data is " << data);
// - Example with two arguments: DEBUG("Data is " << data, DEBUG_HIGH);

#ifndef XDEBUG
#  include <iostream>
#  include <string>
#  define DEBUG_ENABLED true /* Useful to detect if debug is on */
#  define DEBUG_NEVER  -1 /* Only for use with DEBUG_LEVEL */
#  define DEBUG_ALWAYS 0
#  define DEBUG_LOW    25
#  define DEBUG_MEDIUM 50
#  define DEBUG_HIGH   75
#  define DEBUG_MAX    100
#  ifndef DEBUG_LEVEL
#    define DEBUG_LEVEL DEBUG_MEDIUM
#  endif
#  ifndef DEBUG_SHORTEN_PATH_TO
#    define DEBUG_SHORTEN_PATH_TO 2 /* 0 means don't shorten */
#  endif
#  define DEBUG_1(stream) DEBUG_2(stream,DEBUG_MEDIUM)
#  define DEBUG_2(stream, level) do { \
     if constexpr ( level <= DEBUG_LEVEL ) {\
       std::string source{ __FILE__ };\
       if(DEBUG_SHORTEN_PATH_TO){ /* Advanced feature to shorten path */\
         auto pos = source.size(); \
         for(int n=DEBUG_SHORTEN_PATH_TO;n-- && pos != std::string::npos;) pos = source.find_last_of("/\\",pos-1);\
         if( pos != std::string::npos ) source.replace(0,pos,"..");\
       }\
       std::cout << "DEBUG(" << source << ":" << __LINE__ << "): " << stream << std::endl; \
     }\
   } while(0)
#  define DEBUG_GET3RD(arg1,arg2,arg3,...) arg3
#  define DEBUG_X(...) DEBUG_GET3RD(__VA_ARGS__, DEBUG_2, DEBUG_1,)
#  define DEBUG(...) DEBUG_X(__VA_ARGS__)(__VA_ARGS__)
#else
#  define DEBUG(...) /* INTENTIONALLY EMPTY */
#  define DEBUG_ENABLED false /* Useful to detect if debug is on */
#endif

#define DEBUG_HPP
#endif/*DEBUG_HPP*/
