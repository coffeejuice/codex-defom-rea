// defrt/ftnstr/ftnstr.hpp: C <-> Fortran fixed-length string helpers (packet p1-10).
//
// DEF_PRE passes strings between C++ and the PathScale Fortran code as fixed-length,
// blank-padded buffers. These three helpers convert between the two forms; many modules call
// them (DB file names, keyword texts, labels).
#pragma once

#include <cstdint>

namespace def::rt {

// FUN_1403239f0: cuts trailing blanks, tabs, CR, LF and NULs of s[0..n) by writing a NUL after
// the last other character. Quirk kept from the original: when s[0..n) holds only such
// characters, the NUL goes to s[1] (s[0] is never examined as the cut point), so a buffer of
// blanks becomes " ". n <= 0 writes s[0] = 0.
void trim_fortran(char* s, std::int32_t n);

// FUN_140323b90: blank-pads s from strlen(s) up to n characters. No terminator is written; if
// strlen(s) >= n nothing changes. s must be NUL-terminated (the original calls strlen).
void blank_pad(char* s, std::int32_t n);

// FUN_140323bf0: copies the C string src into the Fortran field dst[0..n) (strncpy, then
// blank_pad) and returns strlen(src) as an unsigned 32-bit value.
std::uint32_t to_fortran(char* dst, const char* src, std::int32_t n);

} // namespace def::rt
