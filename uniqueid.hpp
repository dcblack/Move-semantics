#pragma once

/** @brief Serialize your class objects with unique identifiers
  
 Add serial numbers to your classes by simply instantiating or inheriting this
 class template. Nominally uses CRT to derive a unique serial number class.
 You can also create shared serial numbers. See `uniqueid.cpp` for examples. It
 is **not** recommended to inherit since this class overloads `operator()` and
 might be unexpected.

 `UniqueId`'s are unique, so copying _moves_ the id and invalidates
 the value on the source object. Checking is enabled by default, but you can turn
 it off when requested by specifying the optional check as false.

 Moving is not worth the effort.

*/

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
  //............................................................................
  explicit UniqueId(const std::string& prefix="")
  : m_id(s_next())
  , m_self(new UniqueId_ptr)
  {
    *m_self = this;
    if( m_id == start ) s_prefix = prefix;
   static Error e{"UniqueId: not allowed to change prefix"};
    if( (prefix != s_prefix) and not prefix.empty() )
      throw e;
  }

  //............................................................................
  // Copy-constructor
  UniqueId( const UniqueId& rhs )
  : m_id( rhs.m_id )
  , m_self( rhs.m_self )
  {
    *m_self = this; //< become owner
    rhs.m_self = nullptr;
  }

  //............................................................................
  // Copy-assignment
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

  //............................................................................
  // Destructor
  ~UniqueId()
  {
    if( valid() ) delete m_self; //< only owner deletes
  }

  //............................................................................
  virtual bool valid() const noexcept final
  {
    return m_self != nullptr and *m_self == this;
  }

  //............................................................................
  virtual void validate() const final
  {
   static Error e{"UniqueId: invalid UniqueId"};
   if ( not valid() ) throw e;
  }

  //............................................................................
  virtual std::string name(bool check=true) const final
  {
    if ( check ) validate();
    return s_prefix + (valid()?"":"~") + std::to_string(m_id);
  }

  //............................................................................
  virtual size_t id(bool check=true) const final
  {
    if ( check ) validate();
    return m_id;
  }

  //............................................................................
  size_t operator()(bool check=true) const { return id(check); }

private:
  size_t m_id;
  mutable UniqueId_ptr* m_self{nullptr};
  inline static std::string s_prefix = "";
  size_t s_next() const { static size_t id=start; return id++; }
};

//TAF! vim:nospell
