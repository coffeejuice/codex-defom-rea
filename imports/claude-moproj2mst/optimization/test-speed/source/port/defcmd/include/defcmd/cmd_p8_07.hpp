// defcmd/cmd_p8_07.hpp: command keyword entry of packet p8-07 (DEFAMG), included by the group 6
// dispatcher (p5-01, defcmd/dispatch.cpp) through __has_include.
#pragma once

#include <cstdio>

namespace def::model {
class DB_STEP_DATA;
}

namespace def::cmd {

// part of @src 140378e00  case 0x11f+6 (registry 293, DEFAMG): "%i" object number (140413de0);
// 2 when it cannot be read or the object does not exist, else 1401a9be0 and 0
int cmd_defamg(def::model::DB_STEP_DATA* m, std::FILE* f);

} // namespace def::cmd
