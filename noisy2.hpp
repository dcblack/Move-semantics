/** @brief Debug class to display construction/destruction
 *
 * This class is useful for debugging new classes. Just instantiate from it to
 * watch the various operations. You may need to add mods to various
 * constructors and assign operations to see those properly too.
 *
 * Notice that it uses UniqueId to ensure that movements are tracked. Output
 * includes the address of the original object to aid debug when segfaults
 * from other causes occur. This class was designed to not throw.
 */
#ifndef NOISY_HPP
#define NOISY_HPP
#include <iostream>
#include <sstream>
#include <ostream>
#include <utility>
#include <string>
#include <cstdint>
#include "uniqueid.hpp"

#ifndef NNOISE
// Macro allows appropriate use of __func__ (and __FILE__ and __LINE__ if added)
#define NOISE2(msg)\
  std::cout\
    << "DEBUG{"\
    <<   msg << " "\
    <<   __func__ << " "\
    <<   noisy_name() << " "\
    <<   this \
    << "}"\
    << std::endl
#else
#define NOISE2(msg)
#endif

class Noisy
{
public:
  using Str = std::string;
  UniqueId<Noisy> id{"Noisy"};
  //----------------------------------------------------------------------------
  Noisy() //< default constructor
  {
    NOISE2("Default-construct");
  }
  //----------------------------------------------------------------------------
  explicit Noisy(Str v)               //< string constructor
  : m_value(std::move(v))
  {
    NOISE2("String-construct");
  }
  //----------------------------------------------------------------------------
  Noisy(const Noisy& rhs)             //< copy constructor
  : m_value(rhs.m_value)
  {
    m_v = rhs.m_v+uint8_t(1);
    NOISE2("Copy-construct");
  }
  //----------------------------------------------------------------------------
  Noisy& operator=(const Noisy& rhs)  //< copy assign
  {
    if( this != &rhs ) {
      m_value = rhs.m_value;
      ++m_v;
      NOISE2("Copy-assign");
    } else {
      NOISE2("Copy-assign self");
    }
    return *this;
  }
  //----------------------------------------------------------------------------
  ~Noisy() //< destructor
  {
    NOISE2("Destruct");
  }
  //----------------------------------------------------------------------------
  Noisy(Noisy&& rhs) noexcept   //< move constructor
  : id(rhs.id)
  , m_v(std::exchange(rhs.m_v,rhs.m_v - ' '))
  , m_value(std::exchange(rhs.m_value,std::string{}))
  {
    ++m_v;
    NOISE2("Move-construct");
  }
  //----------------------------------------------------------------------------
  Noisy& operator=(Noisy&& rhs) noexcept //< move assign
  {
    if( this != &rhs ) {
      m_value = std::exchange(rhs.m_value,std::string{});
      m_v = std::exchange(rhs.m_v, rhs.m_v - ' ');
      ++m_v;
      NOISE2("Move-assign");
    } else {
      NOISE2("Move-assign self!");
    }
    return *this;
  }
  //----------------------------------------------------------------------------
  bool operator==(const Noisy& rhs) noexcept { NOISE2("Same"); return (m_value == rhs.m_value); }
  bool operator< (const Noisy& rhs) noexcept { NOISE2("Less"); return (m_value < rhs.m_value); }
  [[maybe_unused, nodiscard]] Str  noisy_get () const noexcept { NOISE2("Get"); return m_value; }
  [[maybe_unused]] void noisy_set (const Str& value) noexcept { m_value = value; ++m_v; NOISE2("Set"); }
  [[maybe_unused]] void noisy_who () const noexcept { NOISE2("Info"); }
  [[nodiscard]] Str noisy_name() const
  {
    auto result = m_value;
    if ( !id.valid() ) result += "(was)";
    result += std::to_string(id(false));
    return result + char(m_v);
  }
  //----------------------------------------------------------------------------
  explicit operator std::string() const
  {
    std::ostringstream os;
    os << noisy_name() << ":{m_value=" << m_value << "}";
    return os.str();
  }
  friend std::ostream& operator<< ( std::ostream& os, const Noisy& rhs )
  {
    os << Str(rhs);
    return os;
  }

  //----------------------------------------------------------------------------
private:
  uint8_t       m_v{'a'-1}; //< this violates guidelines, but is used to improve visibility of operations
  Str           m_value{"Noisy"};
};

#endif/*NOISY_HPP*/
