#if defined( NOISY1_SELFTEST )
#include "noisy1.hpp"
#elif defined( NOISY2_SELFTEST )
#include "noisy2.hpp"
#endif/*NOISY1_SELFTEST*/
#include "print.hpp"
#include "to_string.hpp"

// A few simple classes illustrating usage
class Base {
public:
  Base() = default;
  virtual ~Base() = default;
  Base( const Base& ) = default;
  Base( Base&& )  = default;
  Base& operator=( const Base& ) = default;
  Base& operator=( Base&& ) = default;
private:
  [[maybe_unused]] Noisy noise{"Base"};
};

class Derived : Base {
public:
  [[maybe_unused]] Noisy noise{ "Derived" };
  Derived() : Base(), id( nextid() ) {}
  explicit operator std::string() const { return std::to_string(id); }
  int id; //< basic, but unsafe (see uniqueid for better)
private:
  static int nextid() { static int i; return i++; }
};

#include <vector>
int main()
{
  {
    BLANK_LINE;
    __________;
    INFO( "Noisy object behaviors" );
    __________;
    DO( { Noisy n0; } )
    DO( Noisy n1{"explicit"}; )
    DO( Noisy n2{ n1 }; )
    DO( Noisy n3; )
    DO( n3 = n1; )
    DO( n3 = std::move(n2); )
  }

  {
    BLANK_LINE;
    __________;
    INFO( "Simple object behaviors" );
    __________;
    DO( Derived d1; )
    DO( Derived d2{d1}; )
    DO( auto d3 = d1; )
    DO( d3 = d2; )
    DO( Derived d4; )
    DO( d4 = std::move(d1); )
  }

  {
    BLANK_LINE;
    __________;
    INFO( "Basic container behaviors" );
    __________;
    DO( std::vector<Derived> v1{}; )
    DO( std::vector<Derived> v2{2}; )
    DO( v2.reserve( 3 ); )
    DO( v2.resize( 3 ); )
    SHOW( sizeof( v2 ) ); // NOLINT(bugprone-sizeof-container)
    SHOW( v2.size() );
    SHOW( v2.capacity() );
    SHOW_PTR( v2.data() );
    SHOW( to_string( v2 ) );

    __________;
    DO( v2 = v1; ) //< copies
    // Following needed because DO cannot handle commas (possible to fix with variadic macros?)
    Derived d5;
    Derived d6;
    Derived d7;
    ECHO(  "std::vector<Derived> v3 = { d5, d6, d7 };" );
    std::vector<Derived> v3 = { d5, d6, d7 };
    DO( v3.emplace_back( Derived{} ); ) //< constructs in-place
    DO( v3.push_back( d5 ); ) //< copies
    DO( auto v4{v3}; ) //< copies
    DO( std::vector<Derived> v5; )
    SHOW( to_string( v5 ) );
    DO( v5 = std::move( v4 ); )

    DO( v3.push_back( Derived{} ); ) //< extra activity NOLINT(modernize-use-emplace)
    DO( v5.pop_back(); )

    BLANK_LINE;
    __________;
    INFO("Destroying");
  }

  BLANK_LINE;
  __________;
  INFO("Done");
  __________;
return 0;
}
