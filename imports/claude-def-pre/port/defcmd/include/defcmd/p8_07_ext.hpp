// defcmd/p8_07_ext.hpp: callees of packet p8-07 that other packets own and that are not on this
// branch yet. Each stub (src/p8_07_ext.cpp) stops with DEF_NOT_PORTED and names the owner; the
// integration replaces the body with a call of the owner's function.
#pragma once

#include <cstdint>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
class LLWINDEF_DATA;
}

namespace def::cmd::ext {

// 1401d7f40 (P2, no owner yet): after a KEY read with reset: 1401ee510(m+0x750), 1401d6250(m),
// 1401d63c0(m, 0), then the restart step number from m+0x8b0 when m+0x2f88 is set.
// Only reached by read_keyword_file with *reset != 0 (not from DEFAMG).
void fun_1401d7f40(def::model::DB_STEP_DATA* m);

// 1401fc730 (owner p67-1, objpos/centre3d.cpp): build the object's geometry from its mesh
// boundary (2D curves / 3D surface); *compact as passed (1 here).
void fun_1401fc730(def::model::LLOBJ_DATA* obj, const char* compact);

// 140268290 (owner p67-1, objpos/transform4x4.cpp window_xf4): transform a window's points by
// the 4x4 matrix m16 (row-major, rows 0x20 apart).
void fun_140268290(def::model::LLWINDEF_DATA* w, const double* m16);

// 140202a20 (owner p67-5, genbcc_bc.cpp min_edge_length): shortest element edge of the object.
double fun_140202a20(def::model::LLOBJ_DATA* obj);

// 140158ca0 (owner p67-5, inlined there as object_is_3d): obj+0x39c0 == 3. Two-line getter: the
// stub carries the body (140158ce0 + compare) until the integration forwards it to p67-5.
bool fun_140158ca0(def::model::LLOBJ_DATA* obj);

} // namespace def::cmd::ext
