/** @brief Simple test methodology
 *
 * `EXPECT( expression );` expects the expression to be true, but if not an
 * error count will incremented and a message issued to `std::cerr`.
 *
 * Obtain a summary and a proper exit value for main by calling
 * `Expect::summary()`.
 *
 * To issue a special error, call Expect::error(...);
 *
 * It is acceptable to directly manipulate the `Expect::errors()` (e.g. to reset
 * it or decrement it as needs dictate..
 *
 */
#ifndef EXPECT_HPP
#define EXPECT_HPP

#include <iostream>
#include <string>

class Expect
{
public:
  static size_t& checks() { static size_t count = 0u; return count; }
  static size_t& errors() { static size_t count = 0u; return count; }
  static bool&   passed() { static bool status = true; return status; }

  static void error(std::string message, const std::string& file="", int line=0)
  {
    ++Expect::errors();
    if( auto pos = message.find_first_of('#'); pos != std::string::npos )
    {
      auto n = std::to_string( Expect::errors() );
      message.insert(pos+1, n);
    }
    std::cerr << "Error: " << message;
    if ( not file.empty() ) std::cerr << " in " << file;
    if ( line != 0 )        std::cerr << " at line " << line;
    std::cerr << std::endl;
  }

  static int summary(const std::string& prefix="")
  {
    std::cout << Expect::checks() << " checks performed." << std::endl;
    std::cout << Expect::errors() << " errors detected." << std::endl;
    if( not prefix.empty()  ) std::cout << prefix << " ";
    if( Expect::errors() == 0 ) std::cout << "PASS" << std::endl;
    else                      std::cout << "FAIL" << std::endl;
    return (Expect::errors() == 0)?0:1;
  }
};

#define EXPECT(expr) do { \
  ++Expect::checks(); \
  if (Expect::passed() = (expr); not Expect::passed() ) \
    Expect::error( std::string("unexpected ")+ #expr, __FILE__, __LINE__ ); \
} while(0)

#endif/*EXPECT_HPP*/
