// defrt/stdlib/str_ends_with.hpp: FUN_1401b0cc0, the case-folded "ends with" of DEF_PRE_64's
// std::string helpers (0x1401b0870..0x1401b1bbf, see str_helpers.hpp). Ported by packet p8-08
// for DEFINT/DEFAMG (the "<db>.DB" -> "<db>_REM.KEY" name); def_pre/src/menu7.cpp (p4-01) forwards
// its def::pre::ends_with here.
#pragma once

#include <string>

namespace def::rt {

// FUN_1401b0cc0: true when (int)s.size() - strlen(p) >= 1 and the last strlen(p) characters of
// s equal p with A-Z folded to lower case on both sides (strncmp). s by value.
bool str_ends_with(std::string s, const char* p);

} // namespace def::rt
