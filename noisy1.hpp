#pragma once

/**
 * This simple class tracks different types of construction. It is self-contained (as opposed to noisy2.hpp). Usage is simple: Just add a noisy member to your class.
 */

#include <string>
#include <cstdio>
#include <iostream>
#include <sstream>

#define NOISE1(mesg) std::cout << "DEBUG{" << this << ": " << parent << ' ' << mesg << "}\n"

struct Noisy {
    using Str = std::string;
    enum [[maybe_unused]] State_t { Reset, DfltCtor, ExplCtor, Dtor, CpCtor, MvCtor, CpAsgn, MvAsgn, Idled };
    // Constructors and other special members
    explicit Noisy(Str s) : state(DfltCtor), parent(std::move(s)) { NOISE1("explicit-construct"); }
    Noisy()             : state(ExplCtor)    { NOISE1("default-construct"); }
    ~Noisy()                                 { NOISE1("destruct"); state=Dtor; }
    Noisy(const Noisy&) : state(CpCtor)      { NOISE1("copy-construct"); }
    Noisy(Noisy&&) noexcept : state(MvCtor)  { NOISE1("move-construct"); }
    Noisy&   operator=(const Noisy&)         { NOISE1("copy-assign"); state=CpAsgn; return *this; }
    Noisy&   operator=(Noisy&& rhs) noexcept { NOISE1("move-assign"); state=MvAsgn; rhs.state=Idled; return *this; }
    // Use reset if reassigning to an idled object (as a result of a move operation)
    [[maybe_unused]] void reset() { state=Reset; }
    // Accessors
    explicit operator std::string() const { std::ostringstream os; os << this; return os.str(); }
    [[maybe_unused, nodiscard]] Str     info()       const { std::ostringstream os; os <<  parent << ' ' <<  state; return os.str(); }
    [[maybe_unused, nodiscard]] State_t get_state()  const { return state; }
    [[maybe_unused, nodiscard]] Str     get_parent() const { return parent; }
    [[maybe_unused, nodiscard]] bool    useable()    const { return state != Idled; } //< detect problems with this
private:
    State_t state{};
    Str     parent{};
};

// vim:nospell:nowrap