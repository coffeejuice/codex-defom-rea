#include <defrt/stdlib/stdio_compat.hpp>

#include <cstdio>

#ifndef _CRT_INTERNAL_PRINTF_STANDARD_ROUNDING
#define _CRT_INTERNAL_PRINTF_STANDARD_ROUNDING (1ULL << 5) // as legacy_stdio_float_rounding.cpp
#endif

namespace def::rt {

void init_stdio_compat()
{
#ifdef _UCRT
    std::uint64_t& options = *__local_stdio_printf_options();
    options |= _CRT_INTERNAL_PRINTF_LEGACY_THREE_DIGIT_EXPONENTS |
               _CRT_INTERNAL_PRINTF_LEGACY_MSVCRT_COMPATIBILITY;
    // What legacy_stdio_float_rounding.obj does at start-up: round the decimal digits half away
    // from zero ("%.0f" of 0.5 is "1", of 2.5 is "3") instead of to nearest-even on exact ties.
    options &= ~static_cast<std::uint64_t>(_CRT_INTERNAL_PRINTF_STANDARD_ROUNDING);
#endif
}

std::uint64_t stdio_printf_options()
{
#ifdef _UCRT
    return *__local_stdio_printf_options();
#else
    return 0;
#endif
}

} // namespace def::rt
