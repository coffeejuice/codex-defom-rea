// defcmd/cmd_p8_05.hpp: packet p8-05, the BRDEXT command keyword (registry 292, dispatcher case
// 0x11f+5 of FUN_140378e00). Included by the dispatcher (p5-01) through __has_include.
// Spec: docs/re/commands/BRDEXT.md. The border extraction itself (1401a9ac0, called by REMESH too)
// is def::cmd::brdext_object in defcmd/meshgen/p8_seams.hpp; the internals are in defcmd/brdext.hpp.
#pragma once

#include <cstdio>

namespace def::model {
class DB_STEP_DATA;
}

namespace def::cmd {

// part of @src 140378e00  case 0x11f+5 (BRDEXT): "%i" object number from the rest of the line
// (140413de0, skip_blank 0); reader failure -> 2; object not found (1401d6ea0) -> 2; otherwise
// 1401a9ac0 (result ignored) and 0.
int cmd_brdext(def::model::DB_STEP_DATA* m, std::FILE* f);

} // namespace def::cmd
