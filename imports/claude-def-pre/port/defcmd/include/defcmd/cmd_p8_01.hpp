// defcmd/cmd_p8_01.hpp: dispatcher entry points of packet p8-01 for FUN_140378e00 (p5-01).
// Each is the case body, starting on the rest of the keyword line, and returns what the case
// returns (0 or 2; the dispatcher prints "Reading Error" for 2). Messages go to stdout.
//   case 0xb      EXECMD   (docs/re/commands/EXECMD.md)
//   case 0xf      GENINT   (GENINT.md)
//   case 0x10     IMPLUB   (IMPLUB.md)
//   case 0x11     IMPEQP   (IMPEQP.md)
//   case 0x15     ARMCTL   (ARMCTL.md)
//   0x11f + 10    MORP     (MORP.md)
//   0x11f + 0xb   GFREAD   (GFREAD.md)
#pragma once

#include <cstdio>

namespace def::model {
class DB_STEP_DATA;
} // namespace def::model

namespace def::cmd {

int cmd_execmd(model::DB_STEP_DATA* m, std::FILE* f);
int cmd_genint(model::DB_STEP_DATA* m, std::FILE* f);
int cmd_implub(model::DB_STEP_DATA* m, std::FILE* f);
int cmd_impeqp(model::DB_STEP_DATA* m, std::FILE* f);
int cmd_armctl(model::DB_STEP_DATA* m, std::FILE* f);
int cmd_morp(model::DB_STEP_DATA* m, std::FILE* f);
int cmd_gfread(model::DB_STEP_DATA* m, std::FILE* f);

} // namespace def::cmd
