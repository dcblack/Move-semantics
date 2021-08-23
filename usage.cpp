#if defined( NOISY1_SELFTEST )
  #include "noisy1.hpp"
#elif defined( NOISY2_SELFTEST )
  #include "noisy2.hpp"
#endif/*NOISY1_SELFTEST*/
#if defined( UNIQUEID_SELFTEST)
  #include "uniqueid.hpp"
#endif
#include "print.hpp"
#include "to_string.hpp"
#include "expect.hpp"
#include <array>
#include <iostream>
using namespace std::literals;

// A few simple classes illustrating usage
#if defined( UNIQUEID_SELFTEST )
class Composed
{
public:
  UniqueId<Composed,201> id;
  UniqueId<int,1'000> shared;
};
#endif

class Base {
public:
  Base() = default;
  virtual ~Base() = default;
  Base( const Base& ) = default;
  Base( Base&& )  = default;
  Base& operator=( const Base& ) = default;
  Base& operator=( Base&& ) = default;
private:
  #if defined( NOISY1_SELFTEST ) || defined( NOISY2_SELFTEST )
  [[maybe_unused]] Noisy noise{"Base"};
  #endif
};

class Derived : Base {
public:
  #if defined( NOISY1_SELFTEST ) || defined( NOISY2_SELFTEST )
  [[maybe_unused]] Noisy noise{ "Derived" };
  #endif
  Derived() : Base(), id( nextid() ) {}
  explicit operator std::string() const { return std::to_string(id); }
  int id; //< basic, but unsafe (see uniqueid for better)
private:
  static int nextid() { static int i; return i++; }
};

#include <vector>
int main()
{

////////////////////////////////////////////////////////////////////////////////
#ifdef EXPECT_SELFTEST

#endif

////////////////////////////////////////////////////////////////////////////////
#ifdef UNIQUEID_SELFTEST
  __________;
  INFO( "Test UniqueId class" );
  __________;
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wunknown-pragmas"
  try { //< braces aid detection of destruct time
    // Test fundamentals
    UniqueId<int,1'000> unique1{"uniq#"};
    SHOW( unique1.id() );
    EXPECT( unique1.id() == 1'000 );
    UniqueId<int,1'000> unique2;
    SHOW( unique2.name() );
    EXPECT( unique2.id() == 1'001 );
    bool threw = false;
    try {
      UniqueId<int,1'000> unique3{"change"};
    } catch( std::exception& e ) {
      std::cout << "Line#" << __LINE__-2 << " properly caught: " << e.what() << std::endl;
      threw = true;
    }
    EXPECT( threw );
    UniqueId<int,1'000> unique4;
    SHOW( unique4() );

    const UniqueId<int,1>   unique5;
    SHOW( unique5() );
    EXPECT( unique5.id() == 1   );
    EXPECT( unique5() == 1 );

    #if defined( BAD1 ) || defined( BADALL )
    [[maybe_unused]]UniqueId<int,-20> negative; //< Negative id's disallowed
    #endif

    EXPECT( unique4.valid() );
    std::cout << "Attempt assignment" << std::endl;

    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "UnusedValue"
    unique4 = unique1;
    #pragma clang diagnostic pop

    EXPECT( not unique1.valid() );
    std::cout << "Invalidated unique1 " << unique1.name(false) << std::endl;
    threw = false;
    try {
      [[maybe_unused]]auto bad = unique1();  //< was moved
    } catch( std::exception& e ) {
      std::cout << "Line#" << __LINE__-2 << " properly caught: " << e.what() << std::endl;
      threw = true;
    } catch( ... ) {
      EXPECT( false );
    }
    EXPECT( threw );

    #if defined( BAD2 ) || defined( BADALL )
    UniqueId<int,1> unique6;
    UniqueId<char,1> unique7;
    unique5 = unique6; //< Not allowed to modify a constant
    unique6 = unique2; //< Different types
    unique6 = unique7; //< Different types
    #endif

    // Test composed class
    std::array<Composed,3> carray;
    for( auto& composed : carray )
    {
      static size_t i=0;
      std::cout << "[" << i++ << "] = ";
      SHOW( composed.id() );
      SHOW( composed.shared() );
    }

    #if defined( BAD3 ) || defined( BADALL )
    SHOW( carray[0]() ); //< Did not define operator() -- works implicitly only if inherited
    #endif
  } catch( ... ) {
    std::cout << "oops..." << std::endl;
  }
  std::cout << std::string(80,'x') << std::endl;
  return Expect::summary("UniqId test");
#endif/*UNIQUEID_SELFTEST*/

////////////////////////////////////////////////////////////////////////////////
#if defined( NOISY1_SELFTEST ) || defined( NOISY2_SELFTEST )
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
#endif/*NOISY1_SELFTEST||NOISY2_SELFTEST*/

  __________;
  INFO("Done");
return 0;
}

//TAF! vim:nospell
