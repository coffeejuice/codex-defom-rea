// defcmd/p67_5.hpp: command keywords TCYCLE, GENAXS, GENSUB and GENBCC (packet p67-5), cases of
// the command dispatcher FUN_140378e00. Specs: docs/re/commands/{TCYCLE,GENAXS,GENSUB,GENBCC}.md.
//
// Each function is the body of one dispatcher case: it starts on the rest of the keyword's line
// (the dispatcher reads no argument first) and returns what the case leaves for the dispatcher,
// i.e. what FUN_140378e00 returns for that keyword: 0, or 2 ("Reading Error"). The dispatcher
// shell itself is wired at the P6-7 integration gate. `m` is the model FUN_1401d81b0 hands to the
// group parsers.
#pragma once

#include <cstdio>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
namespace linked {
class Cls_140258580;
}
} // namespace def::model

namespace def::cmd {

// case 0x12 (id 18): TCYCLE <k> <n> <T> <d2> <d3>. Always 0.
int cmd_tcycle(model::DB_STEP_DATA& m, std::FILE* f);

// case 0x13 (id 19): GENAXS <layout> <tolA> <tolB>. Always 0. Layout 7 with one of the objects
// 1, 2, 4, 5, 6, 7 missing dereferences NULL in the original (access violation, see genaxs.cpp).
int cmd_genaxs(model::DB_STEP_DATA& m, std::FILE* f);

// case 0x14 (id 20): GENSUB <objNo> <value>. 0, or 2 for a missing object.
int cmd_gensub(model::DB_STEP_DATA& m, std::FILE* f);

// high case 8 (id 295): GENBCC <objNo>. 0, or 2 for a read error or a missing object.
int cmd_genbcc(model::DB_STEP_DATA& m, std::FILE* f);

// WPAXIS record setters (src/p67_5/genaxs.cpp; also called by CNVT3D, p9-01)
// 140258ed0: record +0x08 = 3 doubles; 140258f50: +0x20 = 3 doubles (null source: no-op)
void wpaxis_set_origin(model::linked::Cls_140258580* r, const double* v);
void wpaxis_set_direction(model::linked::Cls_140258580* r, const double* v);

namespace genbcc {
// 140204d20: clear the thermal BCs of an object (src/p67_5/genbcc_bc.cpp; also called by the GUI's
// HTFurnace object-tree slot, packet MDT L2, GUI twin 0x140878b80)
void reset_thermal_bc(model::LLOBJ_DATA& obj);
} // namespace genbcc

} // namespace def::cmd
