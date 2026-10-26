// defrt/port.hpp: markers every ported function uses (docs/port-conventions.md).
//
// Source citation is a comment, not a macro, so that it costs nothing and a script can read it:
//
//     // @src 1401c8950            DB_STEP_DATA::DB_STEP_DATA (FUN_1401c8950)
//     DB_STEP_DATA::DB_STEP_DATA(int dim) { ... }
//
// DEF_NOT_PORTED marks a body that exists only so that a vtable or declaration links; reaching it
// at run time is a bug in the port (a path we believed unreachable, or a packet not done yet).
#pragma once

#include <cstdint>

namespace def::rt {

// Prints "def_pre port: function 0x... not ported (file:line)" to stderr and aborts.
[[noreturn]] void not_ported(std::uint64_t address, const char* file, int line);

// Same for code that the original reaches but the port deliberately omits (licence checks).
[[noreturn]] void omitted(std::uint64_t address, const char* why, const char* file, int line);

} // namespace def::rt

#define DEF_NOT_PORTED(addr) ::def::rt::not_ported((addr), __FILE__, __LINE__)
