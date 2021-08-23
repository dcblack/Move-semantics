#pragma once

/**
 * This class simply announces different types of construction. It is self-contained (as opposed to noisy2.hpp). Usage is simple: Just add a noisy member to your class.
 */

#include <string>
#include <sstream>
#include <iostream>

class Noisy {
public:
  using Str = std::string;
  enum [[maybe_unused]] State_t { Reset, DfltCtor, ExplCtor, Dtor, CpCtor, 
                                  MvCtor, CpAsgn, MvAsgn, MvFrom, CpSelf, MvSelf };
  //----------------------------------------------------------------------------
  // Constructors and other special members
  explicit Noisy( Str s )   : m_state( DfltCtor ), m_label( std::move( s ) ) { noise(); }
  Noisy()                   : m_state( ExplCtor ) { noise(); }
  ~Noisy()                                        { m_state=Dtor; noise(); }
  Noisy( const Noisy& )     : m_state( CpCtor )   { noise(); }
  Noisy( Noisy&& ) noexcept : m_state( MvCtor )   { noise(); }
  Noisy&   operator=( const Noisy& )              { m_state=CpAsgn; noise(); return *this; }
  Noisy&   operator=( Noisy&& rhs ) noexcept      { m_state=MvAsgn; noise(); rhs.m_state=MvFrom; return *this; }
  [[maybe_unused]] void reset()                   { m_state=Reset; }
  //----------------------------------------------------------------------------
  // Accessors
  //............................................................................
  explicit operator std::string() const { std::ostringstream os; os << this; return os.str(); }
  [[maybe_unused]]            void set( const Str& s ) { m_state=Reset; m_label = s; }
  [[maybe_unused, nodiscard]] Str  get()   const { return m_label; }
  [[maybe_unused, nodiscard]] bool valid() const { return m_state != MvFrom; } //< detect problems with this
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
  [[maybe_unused]] void info() const { noise(); }
private:
  mutable State_t m_state{};
  Str     m_label{};
  //............................................................................
  void noise( const Str& alt = "" ) const noexcept {
    std::cout
      << "Noisy{ "
      <<   this << ": "
      <<   ( m_label.empty() ? "<<empty>>" : m_label ) << ' '
      <<   ( alt.empty() ? state() : alt ) << ' '
      << "}"
      << std::endl;
  }
};

//TAF! vim:nospell:nowrap
