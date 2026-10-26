// defrt/stdlib/stdio_compat_test.hpp: include in a tier-1 test file (once per test executable)
// to run def::rt::init_stdio_compat() before main, so that formatted output matches def_pre's.
#pragma once

#include <defrt/stdlib/stdio_compat.hpp>

namespace def::rt::detail {
inline const bool g_stdio_compat_for_tests = (init_stdio_compat(), true);
} // namespace def::rt::detail
