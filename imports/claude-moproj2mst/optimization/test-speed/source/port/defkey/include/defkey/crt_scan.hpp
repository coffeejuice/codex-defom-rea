// defkey/crt_scan.hpp: sscanf/fscanf that read the binary's scan formats the way its CRT
// (Visual C++ 2010, statically linked) did.
//
// The binary's formats use the MSVC size prefixes "F"/"N" (far/near), e.g. "%Flf". The old CRT
// ignored them. UCRT reads "%F" as a float conversion: sscanf("1.5", "%Flf", &d) stores a float
// into the low 4 bytes of d and returns 1, and no UCRT scanf option changes that (checked on
// UCRT 10.0, also with _CRT_INTERNAL_SCANF_LEGACY_MSVCRT_COMPATIBILITY). These wrappers drop the
// prefixes and call UCRT, so ported code keeps the binary's format strings byte for byte:
//
//     def::key::crt::sscanf(buf, "%i %Flf", &i, &d);   // as in 1404154c0
//
// Output needs no wrapper: plain std::fprintf with the process-wide legacy printf options that
// def::rt::init_stdio_compat() sets (p1-01): three-digit exponents (0x10), old INF/NaN text (0x08)
// and old half-up rounding (0x20 cleared). Tests that print set the same options themselves.
//
// Number parsing inside scanf is UCRT's strtod (correctly rounded). The old CRT's _fltin could
// differ in the last bit for inputs with more than 17 significant digits; not seen in KEY files.
#pragma once

#include <cstdarg>
#include <cstdio>
#include <string>

namespace def::key::crt {

// The format as the old CRT read it: "F" and "N" size prefixes removed ("%Flf" -> "%lf",
// "%*Fs" -> "%*s"). Scan sets ("%[...]") and "%%" are copied unchanged.
std::string scan_format(const char* fmt);

int vsscanf(const char* s, const char* fmt, std::va_list ap);
int sscanf(const char* s, const char* fmt, ...);
int vfscanf(std::FILE* f, const char* fmt, std::va_list ap);
int fscanf(std::FILE* f, const char* fmt, ...);

} // namespace def::key::crt
