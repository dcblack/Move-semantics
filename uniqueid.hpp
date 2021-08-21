/** @brief Serialize your class objects with a unique identifiers
  
 Add serial numbers to your classes by simply instantiating or inheriting this
 class template. Nominally uses CRT to derived a unique serial number class.
 You can also created shared serial numbers. See `uniqueid.cpp` for examples. It
 is **not** recommended to inherit since this class overloads `operator()` and
 might be unexpected.

 `UniqueId`'s intentionally are unique, so copying _moves_ the id and invalidates
 the value on the source object. Checking is enabled by default, but you can turn
 it off when requesting by specifying the optional check as false.

 Moving is not worth the effort.

*/
#ifndef UNIQUEID_HPP
#define UNIQUEID_HPP
#include <cstddef>
#include <string>
#include <stdexcept>
class UniqueId_base {};

template<class T, size_t start=0u>
class UniqueId: public UniqueId_base
{
public:
  using Error = std::runtime_error;
  using UniqueId_ptr = UniqueId_base*;
  UniqueId(const std::string& prefix="")
  : m_id(s_next())
  , m_self(new UniqueId_ptr)
  {
    *m_self = this;
    if( m_id == start ) s_prefix = prefix;
   static Error e{"UniqueId: not allowed to change prefix"};
    if( (prefix != s_prefix) and not prefix.empty() )
      throw e;
  }

  // Copy constructor
  UniqueId( const UniqueId& rhs )
  : m_id( rhs.m_id )
  , m_self( rhs.m_self )
  {
    *m_self = this; //< become owner
    rhs.m_self = nullptr;
  }

  // Copy assignment
  UniqueId& operator=( const UniqueId& rhs )
  {
    if ( &rhs != this ) {
      m_id    = rhs.m_id;
      m_self  = rhs.m_self;
      *m_self = this; //< become owner
      rhs.m_self = nullptr;
    }
    return *this;
  }

  // Destructor
  ~UniqueId()
  {
    if( valid() ) delete m_self; //< only owner deletes
  }

  virtual bool valid() const noexcept final
  {
    return m_self != nullptr and *m_self == this;
  }

  virtual void validate() const final
  {
   static Error e{"UniqueId: invalid UniqueId"};
   if ( not valid() ) throw e;
  }

  virtual std::string name(bool check=true) const final
  {
    if ( check ) validate();
    return s_prefix + (valid()?"":"~") + std::to_string(m_id);
  }

  virtual size_t id(bool check=true) const final
  {
    if ( check ) validate();
    return m_id;
  }

  size_t operator()(bool check=true) const { return id(check); }

private:
  size_t m_id;
  mutable UniqueId_ptr* m_self{nullptr};
  inline static std::string s_prefix = "";
  size_t s_next() const { static size_t id=start; return id++; }
};
#endif/*UNIQUEID_HPP*/

#ifdef UNIQUEID_SELFTEST

// Use composition
class Composed
{
public:
  UniqueId<Composed,201> id;
  UniqueId<int,1'000> shared;
};

// Use inheritance - not recommended
class Derived : public UniqueId<Derived, 1001>
{
public:
  size_t operator()() const { return 42; }
#if defined( BAD0 ) || defined( BADALL )
  // Attempt override
  size_t id(bool check=true) const override
  {
    return m_id ^ 0xDEADBEEFu;
  }
#endif
};

#include <iostream>
#include <array>
#include "expect.hpp"
using namespace std;
using namespace std::literals;
#define SHOW(expr) std::cout << #expr << " = " << expr << std::endl
int main() {
  std::cout << "Self-test for UniqueId class" << std::endl;
  std::cout << std::string(80,'x') << std::endl;
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

    // Test Derived class
    Derived darray[3];
    for( auto& derived : darray )
    {
      static size_t i=0;
      std::cout << "[" << i++ << "] = ";
      SHOW( derived.id() );
    }
    SHOW( darray[0]() );
  } catch( ... ) {
#pragma clang diagnostic pop
    std::cout << "oops..." << std::endl;
  }
  std::cout << std::string(80,'x') << std::endl;
  return Expect::summary("UniqId test");
}

#endif/*UNIQUEID_SELFTEST*/
