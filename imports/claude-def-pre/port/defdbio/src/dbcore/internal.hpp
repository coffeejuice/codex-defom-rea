// Helpers shared by the DB core sources (not part of the public interface).
#pragma once

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cmath>

#include <emmintrin.h>

namespace def::dbio::dbcore::detail {

// CVTTSD2SI: truncation, 0x80000000 for NaN and out-of-range values (what the original's
// conversions produce; a plain C++ cast would be undefined there).
inline std::int32_t cvtt(double x) { return _mm_cvttsd_si32(_mm_set_sd(x)); }

// Fortran NINT as the compiled code does it: x itself when |x| >= 2^52, else x +- 0.5, then
// truncated (constants DAT_140bb0540 = 0.5, 548 = -0.5, 558 = abs mask, 560 = 2^52).
inline std::int32_t nint(double x)
{
    double y;
    if (std::fabs(x) >= 4503599627370496.0) {
        y = x;
    } else {
        y = (0.0 <= x ? 0.5 : -0.5) + x;
    }
    return cvtt(y);
}

// List-directed WRITE(*,*) / WRITE(6,*) of one character constant (the Fortran runtime starts
// each record with a blank). The runtime's output buffering is not reproduced: the line is
// written to the C stdout at once.
inline void ftn_write_text(const char* s) { std::printf(" %s\n", s); }
// ... of a character constant and an INTEGER (I12-style field, as the runtime writes INTEGER*4
// in list-directed output). Not checked against the oracle: no corpus case reaches these texts.
inline void ftn_write_text_int(const char* s, std::int32_t v) { std::printf(" %s%12d\n", s, v); }

} // namespace def::dbio::dbcore::detail

namespace def::dbio::dbcore {
// The step search of FUN_1403b6679, shared with FUN_1403bcf76 (dbops.cpp).
bool find_step_entry(const std::int32_t* unit, std::int32_t step, std::int32_t* slot,
                     std::int32_t* pos, std::int32_t* ierr);
} // namespace def::dbio::dbcore
