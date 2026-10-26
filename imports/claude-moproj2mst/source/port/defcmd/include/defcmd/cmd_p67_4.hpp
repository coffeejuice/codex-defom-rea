// defcmd/cmd_p67_4.hpp: dispatcher entry points of packet p67-4 for FUN_140378e00 (p5-01):
// cases 9 WRTOBJ, 0xc SVSCAL, 0xd CPYOBJ, 0xe DOESCL. Each returns the case status (0, 1 or 2;
// see defcmd/obj_cmds.hpp). Messages go to stdout like the original's printf.
#pragma once

#include <cstdio>

#include <defcmd/obj_cmds.hpp>

namespace def::cmd {

inline int cmd_wrtobj(model::DB_STEP_DATA* m, std::FILE* f) { return objcmd::wrtobj(*m, f); }
inline int cmd_svscal(model::DB_STEP_DATA* m, std::FILE* f) { return objcmd::svscal(*m, f); }
inline int cmd_cpyobj(model::DB_STEP_DATA* m, std::FILE* f) { return objcmd::cpyobj(*m, f); }
inline int cmd_doescl(model::DB_STEP_DATA* m, std::FILE* f) { return objcmd::doescl(*m, f, stdout); }

} // namespace def::cmd
