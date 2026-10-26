// defrt/stdlib/legacy_printf.hpp: the printf family with the floating-point formatting of the
// VS2008 static CRT that DEF_PRE_64 is linked with (packet p1-01).
//
// Why: the 2014 CRT does not print the exact decimal value of a double. _fltout2 asks
// $I10_OUTPUT for 17 significant digits (computed in 80-bit arithmetic with the CRT's own
// power-of-ten tables and rounded half up), and _fptostr then rounds that digit string half up
// to the requested precision. So "%.20f" of 0.1 is "0.10000000000000001000", "%f" of 1e23 is
// "99999999999999992000000.000000", and a value such as 1015.07125 (stored as
// 1015.0712499999999863...) prints "%.7E" as "1.0150713E+003" where the UCRT prints
// "1.0150712E+003". init_stdio_compat() cannot fix this; these functions do.
//
// Use def::rt::crt::fprintf / sprintf / snprintf / printf wherever the original formats doubles
// with %e %E %f %g %G and the output is compared with DEF_PRE's. Every other conversion (and
// infinities and NaNs, which the UCRT's legacy mode already prints as "1.#INF00" etc.) is
// passed to the UCRT unchanged, so call init_stdio_compat() at start-up as well.
//
// Not supported: %n (aborts), positional arguments.
#pragma once

#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <string>

namespace def::rt::crt {

int printf(const char* fmt, ...);
int fprintf(std::FILE* f, const char* fmt, ...);
int vfprintf(std::FILE* f, const char* fmt, std::va_list ap);
// Like the CRT's sprintf: the caller's buffer must be large enough.
int sprintf(char* buf, const char* fmt, ...);
// C99 semantics: writes at most n - 1 characters and a NUL, returns the full length.
int snprintf(char* buf, std::size_t n, const char* fmt, ...);
int vsnprintf(char* buf, std::size_t n, const char* fmt, std::va_list ap);
std::string format(const char* fmt, ...);
std::string vformat(const char* fmt, std::va_list ap);

// One floating-point conversion as _output_l produces it, before width/sign padding:
// conv is one of e E f g G; precision < 0 means the default (6). The result may start with '-'.
std::string format_double(double value, char conv, int precision, bool alternate);

// ---- the CRT internals, exposed for tier-1 tests ----

// _LDBL12: 96-bit value, words little endian: w[0] extension, w[1..4] 64-bit mantissa with the
// explicit leading bit, w[5] sign and 15-bit exponent (bias 0x3fff).
struct Ldbl12 {
    std::uint16_t w[6];
};

// The CRT's power-of-ten tables _pow10pos (14063c0a0) / _pow10neg (14063c200), 29 entries each
// (owned by legacy_printf.cpp): neg = false gives _pow10pos.
const Ldbl12* pow10_table(bool neg);

// FOS, the result of $I10_OUTPUT.
struct Fos {
    std::int16_t exp;    // decimal exponent: value = 0.d1d2d3... * 10^exp
    char sign;           // ' ' or '-'
    std::uint8_t len;    // number of digits in man
    char man[22];        // digits, NUL-terminated
};

// _strflt, the result of _fltout2.
struct StrFlt {
    int sign;            // ' ' or '-'
    int decpt;
    int flag;            // $I10_OUTPUT's return value (0 for INF/NaN)
    char* mantissa;
};

Ldbl12 dtold(double x);                                                     // __dtold
void ld12mul(Ldbl12* px, const Ldbl12* py);                                 // ld12mul
void multtenpow12(Ldbl12* pld12, int pow, bool mult12);                     // __multtenpow12
int i10_output(const Ldbl12& ld, int ndigits, unsigned flags, Fos* fos);    // $I10_OUTPUT
StrFlt* fltout2(double x, StrFlt* flt, char* result, std::size_t size);     // _fltout2
int fptostr(char* buf, std::size_t size, int digits, StrFlt* flt);          // _fptostr

} // namespace def::rt::crt
