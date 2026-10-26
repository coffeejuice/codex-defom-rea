// defrt/stdlib/stdio_compat.hpp: make the UCRT's printf family format like the CRT that
// DEF_PRE_64 was linked with (VS2008 static CRT), where the outputs can see the difference
// (packet p1-01).
//
// Call def::rt::init_stdio_compat() once at process start, before anything is printed:
// def_pre's main does it, and tier-1 tests that check formatted output include
// <defrt/stdlib/stdio_compat_test.hpp>, which does it before main.
//
// What it sets (the options word of __local_stdio_printf_options(), shared by every translation
// unit of the executable):
//   _CRT_INTERNAL_PRINTF_LEGACY_THREE_DIGIT_EXPONENTS  %e/%E/%g print three exponent digits,
//       "1.0000000000E+000", as the old CRT did (the UCRT prints "E+00").
//   _CRT_INTERNAL_PRINTF_LEGACY_MSVCRT_COMPATIBILITY   infinities and NaNs print as the old
//       CRT's "1.#INF00", "-1.#IND00", "1.#QNAN0" instead of "inf", "-nan(ind)", "nan".
//   clears _CRT_INTERNAL_PRINTF_STANDARD_ROUNDING     (what legacy_stdio_float_rounding.obj
//       does) decimal digits round half away from zero, "%.0f" of 2.5 is "3", not "2".
//
// Known differences it does not remove are listed in docs/re/status/p1-01-printf.md.
#pragma once

#include <cstdint>

namespace def::rt {

void init_stdio_compat();

// The options word as init_stdio_compat() leaves it (for tests and diagnostics).
std::uint64_t stdio_printf_options();

} // namespace def::rt
