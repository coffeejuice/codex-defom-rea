// P06 internal: fscanf of the static MSVC CRT that DEF_GUI_MO_64 links (0x141472ac8 fscanf,
// 0x14146dfd4 sscanf), for the directives the data-file readers use.
#pragma once

#include <initializer_list>
#include <string>

#include "m2m/datafiles.hpp"

namespace m2m::datafiles {

// Directives: white space (skips any input white space), literal bytes, and
//   %s      word after skipping white space          -> std::string*
//   %[^x]   scan set (no white-space skip)            -> std::string*
//   %d      decimal int, 32-bit wrap                  -> 4 bytes written at void* (memcpy)
//   %f %e %g          float  (MSVC _fassign)          -> 4 bytes written at void*
//   %lf %le %lg       double                          -> 8 bytes written at void*
//   %*...   conversion without assignment (no argument)
// A conversion that fails leaves its target untouched, pushes the offending byte back and ends
// the call, as the CRT's _input does. Returns the number of assigned fields, or -1 when the input
// ended before the first conversion.
int cscan(TextFile& f, const char* fmt, std::initializer_list<void*> args);

// @src 0x1407fb960 (behind 0x1407fba10, which returns strlen): getc loop, the same result as fgets.
inline bool fgetsCog(TextFile& f, std::string& buf, int n) { return f.fgets(buf, n); }

}  // namespace m2m::datafiles
