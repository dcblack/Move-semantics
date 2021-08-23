#pragma once

/** @brief Debug class to display and track construction/destruction
 *
 * This class is useful for debugging new classes. Just instantiate from it to
 * watch the various operations. You may need to add mods to various
 * constructors and assign operations to see those properly too.
 *
 * Notice that it uses UniqueId to ensure that movements are tracked. Output
 * includes the address of the original object to aid debug when segfaults
 * from other causes occur. This class was designed to not throw.
 */

#include <sstream>
#include <ostream>
#include <utility>
#include <string>
#include <cstdint>
#include <string_view>
#include <iostream>
#include "uniqueid.hpp"

class Noisy
{
public:
  using Str = std::string;
  enum [[maybe_unused]] State_t { Reset, DfltCtor, ExplCtor, Dtor, CpCtor, 
                                  MvCtor, CpAsgn, MvAsgn, MvFrom, CpSelf, MvSelf };
  //----------------------------------------------------------------------------
  // Constructors and other special members
  UniqueId<Noisy> id{"Noisy"};
  //............................................................................
  explicit Noisy( Str s )                  //< explicit-constructor
  : m_state( ExplCtor )
  , m_label( std::move( s ) )
  {
    noise();
  }
  //............................................................................
  Noisy()                                //< default-constructor
  : m_state( DfltCtor )
  {
    noise();
  }
  //............................................................................
  ~Noisy()                               //< destructor
  {
    m_state = Dtor;
    noise();
  }
  //............................................................................
  Noisy( const Noisy& rhs )                //< copyi constructor
  : m_state( CpCtor )
  , m_label( rhs.m_label )
  {
    m_v = rhs.m_v+uint8_t( 1 );
    noise();
  }
  //............................................................................
  Noisy& operator=( const Noisy& rhs )     //< copy-assign
  {
    if( this != &rhs ) {
      m_state = CpAsgn;
      m_label = rhs.m_label;
      ++m_v;
      noise();
    } else {
      m_state = CpSelf;
      noise();
    }
    return *this;
  }
  //............................................................................
  Noisy( Noisy&& rhs ) noexcept            //< move-constructor
  : id( rhs.id )
  , m_state( MvCtor )
  , m_label( std::exchange( rhs.m_label,std::string{} ) )
  , m_v( std::exchange( rhs.m_v,rhs.m_v - ' ' ) )
  {
    ++m_v;
    noise();
  }
  //............................................................................
  Noisy& operator=( Noisy&& rhs ) noexcept //< move-assign
  {
    if( this != &rhs ) {
      m_state = MvAsgn;
      m_label = std::exchange( rhs.m_label,std::string{} );
      m_v = std::exchange( rhs.m_v, rhs.m_v - ' ' );
      ++m_v;
      noise();
    } else {
      m_state = MvSelf;
      noise();
    }
    return *this;
  }
  //............................................................................
  [[maybe_unused]] void reset() { m_state=Reset; }
  //----------------------------------------------------------------------------
  // Accessors
  //............................................................................
  bool operator==( const Noisy& rhs ) noexcept {
    if ( m_label == rhs.m_label ) {
      noise( "same" );
      return true;
    } else {
      noise( "different" );
      return false;
    }
  }
  //............................................................................
  bool operator< ( const Noisy& rhs ) noexcept {
    if ( m_label < rhs.m_label ) {
      noise( "less-than" );
      return true;
    } else {
      noise( "greater-or-equal" );
      return false;
    }
  }
  //............................................................................
  [[maybe_unused]]            void set ( const Str& value ) noexcept { m_state = Reset; m_label = value; ++m_v; noise( "Set" ); }
  [[maybe_unused, nodiscard]] Str  get ()  const noexcept { noise( "get" ); return m_label; }
  [[maybe_unused, nodiscard]] bool valid() const { return id.valid(); }
  //............................................................................
  [[maybe_unused, nodiscard]] Str  state() const {
    switch( m_state ) {
      case Reset:    return "reset";
      case DfltCtor: return "default-constructed";
      case ExplCtor: return "explict-constructed";
      case Dtor:     return "deconstructed";
      case CpCtor:   return "copy-constructed";
      case MvCtor:   return "move-constructed";
      case CpAsgn:   return "copy-assigned";
      case MvAsgn:   return "move-assigned";
      case MvFrom:   return "moved-from";
      case CpSelf:   return "copied-self!";
      case MvSelf:   return "moved-self!";
    }
  }
  //............................................................................
  [[maybe_unused]]            void info() const noexcept { noise(); }
  //............................................................................
  explicit operator std::string() const {
    static std::ostringstream os;
    os.str("");
    os << this;
    return os.str();
  }
  friend std::ostream& operator<< ( std::ostream& os, const Noisy& rhs )
  {
    os << std::string( rhs );
    return os;
  }

//------------------------------------------------------------------------------
private:
  mutable State_t m_state{};
  Str     m_label{"Noisy"};
  uint8_t m_v{'a'-1}; //< this violates guidelines, but is used to improve visibility of operations
  // Display information useful to debug in a consistent format
  void noise( const Str& alt="" ) const noexcept {
    std::cout
      << "Noisy{ "
      <<   this << ": "
      <<   ( m_label.empty() ? "<<empty>>" : m_label ) << ' '
      <<   std::to_string( id( false ) ) << char( m_v ) << ' '
      <<   ( alt.empty() ? state() : alt ) << ' '
      << "}"
      << std::endl;
  }
};

//TAF! vim:nospell
