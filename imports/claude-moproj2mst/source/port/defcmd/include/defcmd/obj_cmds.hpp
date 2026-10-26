// defcmd/obj_cmds.hpp: the object command keywords WRTOBJ (id 9), SVSCAL (12), CPYOBJ (13) and
// DOESCL (14), cases 9, 0xc, 0xd and 0xe of the command dispatcher FUN_140378e00 (packet p67-4).
// Specs: docs/re/commands/WRTOBJ.md, SVSCAL.md, CPYOBJ.md, DOESCL.md, _common.md.
//
// Each case function is the case body: it starts on the rest of the keyword's line and returns
// what FUN_140378e00 returns for it (0, 1 = stop the KEY loop, or 2 = "Reading Error"; the
// dispatcher maps a non-zero case status to 2, which these cases never leave). `m` is the model
// the dispatcher receives (FUN_1401d81b0 of the KEY reader's model). Messages the original
// prints with printf go to `out`.
#pragma once

#include <cstdint>
#include <cstdio>

namespace def::model {
class DB_STEP_DATA;
}

namespace def::cmd::objcmd {

// The dispatcher ids handled here: 9, 12, 13, 14.
bool handles(int id);
// The case for `id` (one of the above).
int run_case(model::DB_STEP_DATA& m, std::FILE* f, int id, std::FILE* out = stdout);

int wrtobj(model::DB_STEP_DATA& m, std::FILE* f);                          // case 9
int svscal(model::DB_STEP_DATA& m, std::FILE* f);                          // case 0xc
int cpyobj(model::DB_STEP_DATA& m, std::FILE* f);                          // case 0xd
int doescl(model::DB_STEP_DATA& m, std::FILE* f, std::FILE* out = stdout); // case 0xe

// 1401ac710: the DOESCL action on keyword `id` with factor *factor and the line's integers
// ints[0..6] (n = 7). Prints the "not defined" / "undefined keyword" errors to `out`; returns 1
// when the target exists, else 0 (DOESCL ignores it).
std::int32_t doe_scale(model::DB_STEP_DATA& m, std::int32_t id, const double* factor, std::int32_t n,
                       std::int32_t* ints, std::FILE* out = stdout);

} // namespace def::cmd::objcmd
