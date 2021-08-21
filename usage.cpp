#if defined( NOISY1_SELFTEST )
#include "noisy1.hpp"
#elif defined( NOISY2_SELFTEST )
#include "noisy2.hpp"
#endif/*NOISY1_SELFTEST*/
#include "print.hpp"
#include "to_string.hpp"
#include <iomanip>
#include <cassert>

// A few simple classes illustrating usage
class Base {
public:
  virtual ~Base() = default;
  Base( const Base& ) = default;
  Base( Base&& )  = default;
  Base& operator=( const Base& ) = default;
  Base& operator=( Base&& ) = default;
  Noisy{"Base"} noise;
};

class Derived : Base {
public:
  Noisy noise{ "Derived" };
  Derived() : id( nextid() ) {}
  int id; //< basic, but unsafe (see uniqueid for better)
private:
  static int nextid() { static int i; return i++; }
}

#include <vector>
int main()
{
  INFO( "Noisy object behaviors\n" );
  DO( { Noisy n0; } )
  DO( Noisy n1{"explicit"}; )

  __________;
  INFO( "Simple object behaviors\n" );
  DO( Derived d1; )
  DO( Derived d2{d1}; )
  DO( auto d3 = d1; )
  DO( d3 = d2; )

  __________;
  INFO( "Basic container behaviors\n" );
  DO( std::vector<Derived> v1{}; )
  DO( std::vector<Derived> v2{2}; )
  DO( v2.reserve( 3 ); )
  DO( v2.resize( 3 ); )
  SHOW( sizeof( v2 ); )
  SHOW( v2.size(); )
  SHOW( v2.capacity(); )
  SHOW_PTR( v2.data(); )

  __________;
  DO( v2 = v2; ) //< copies
  DO( std::vector<Derived> v3 = { d1, d2, d3 }; )
  DO( v3.emplace_back( Derived{} ); ) //< constructs in-place
  DO( v3.push_back( d1 ); ) //< copies
  DO( auto v4{v3}; ) //< copies
  DO( std::vector<Derived> v5; )
  DO( v5 = std::move( v4 ); )

  DO( v3.push_back( Noisy{} ); ) //< extra activity
  DO( v5.pop_back(); )

  __________;
  INFO("Destroying");
  return 0;
}
