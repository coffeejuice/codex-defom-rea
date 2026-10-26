// p8-02 seams: common helpers. Every call into a function another packet owns goes through a
// function pointer `ext::fun_<addr>` (the Phase 1 pending.hpp scheme). The default is the owner's
// function when its header is visible (__has_include), else a DEF_NOT_PORTED stub; tier-1 tests
// install fakes and call reset_ext() afterwards.
//
// owner_call() converts the arguments to the owner's parameter types (pointers by
// reinterpret_cast, numbers by static_cast) so that the wiring survives the owner naming its
// structs differently from stl_types.hpp; the layouts are the binary's either way.
#pragma once

#include <cstdint>
#include <type_traits>

#include <defrt/port.hpp>

namespace def::cmd::stlnas::ext {

template <std::uint64_t Address, class R, class... A>
R not_ported(A...)
{
    DEF_NOT_PORTED(Address);
}

template <class T, class U>
T pcast(U u)
{
    if constexpr (std::is_pointer_v<T> && std::is_pointer_v<U>) {
        return reinterpret_cast<T>(u);
    } else if constexpr (std::is_pointer_v<T> && std::is_integral_v<U>) {
        return reinterpret_cast<T>(static_cast<std::intptr_t>(u));
    } else if constexpr (std::is_integral_v<T> && std::is_pointer_v<U>) {
        return static_cast<T>(reinterpret_cast<std::intptr_t>(u));
    } else if constexpr (std::is_reference_v<T>) {
        return reinterpret_cast<T>(*u);
    } else {
        return static_cast<T>(u);
    }
}

template <class R, class... P, class... A>
R owner_call(R (*f)(P...), A... a)
{
    static_assert(sizeof...(P) == sizeof...(A), "owner signature differs in arity");
    return f(pcast<P>(a)...);
}

}  // namespace def::cmd::stlnas::ext

// One seam entry: default_<addr> (the program-build target) and the hookable pointer fun_<addr>.
#define P802_EXT(addr, target, R, ...)                                  \
    inline constexpr R (*default_##addr)(__VA_ARGS__) = target;         \
    inline R (*fun_##addr)(__VA_ARGS__) = default_##addr
