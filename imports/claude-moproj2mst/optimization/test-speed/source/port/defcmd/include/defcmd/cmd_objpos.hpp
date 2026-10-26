// defcmd/cmd_objpos.hpp: entry point of OBJPOS (case 7 of the command dispatcher FUN_140378e00)
// for the dispatcher port; a thin wrapper over def::cmd::objpos::run (defcmd/objpos.hpp).
#pragma once

#include <cstdio>

namespace def::model {
class DB_STEP_DATA;
}

namespace def::cmd {

// Reads OBJPOS's arguments from `f` (positioned after the keyword) and runs the command.
// Returns the case status: 0, 2 (Reading Error) or 3 (Data Error).
int cmd_objpos(model::DB_STEP_DATA* m, std::FILE* f);

} // namespace def::cmd
