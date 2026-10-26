// gendb3d/ext.hpp: calls from packet p4-05 into functions other packets own.
//
// Each wrapper names the binary function and the owner's port it calls. hooked() offers the call
// to g_hook first (tier-1 tests install one); without a hook, or when the hook declines, the
// wrapper calls the owner (Phase 4 gate: wired in the program build, gate ports of functions no
// packet owned in defdbio/gendb/gate_ports.hpp). call() is the hook-or-DEF_NOT_PORTED form for a
// wrapper whose owner is not ported; none is left in this packet.
//
// The group files add their wrappers in their own headers (ext_<group>.hpp) on top of this core.
#pragma once

#include <bit>
#include <cstdint>
#include <initializer_list>

namespace def::dbio::g3d::ext {

// Test hook: called for every wrapped call first. It returns true when it handled the call and
// stored the function's result in *result (0 for void functions; doubles as their bit pattern),
// false to let the wrapper run its default body.
using Hook = bool (*)(std::uint64_t addr, const std::uint64_t* args, int nargs, std::uint64_t* result);
inline Hook g_hook = nullptr;

// The hook's result, else DEF_NOT_PORTED(addr) (wrappers without a default body).
std::uint64_t call(std::uint64_t addr, std::initializer_list<std::uint64_t> args);
// True (and *result) when the hook handled the call; false: the wrapper runs its default body.
bool hooked(std::uint64_t addr, std::initializer_list<std::uint64_t> args, std::uint64_t* result);

inline std::uint64_t u(const void* p) { return reinterpret_cast<std::uintptr_t>(p); }
inline std::uint64_t i(std::int64_t v) { return static_cast<std::uint64_t>(v); }
inline std::uint64_t d(double v) { return std::bit_cast<std::uint64_t>(v); }
inline double as_d(std::uint64_t v) { return std::bit_cast<double>(v); }
template <class T> inline T* as_p(std::uint64_t v) { return reinterpret_cast<T*>(static_cast<std::uintptr_t>(v)); }

} // namespace def::dbio::g3d::ext
