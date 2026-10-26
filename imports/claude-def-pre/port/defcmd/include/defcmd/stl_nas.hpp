// defcmd/stl_nas.hpp: public API of packet p8-02 (GFREAD in-process 3D readers: STL, NASTRAN, and
// the PDA/UNV external-program read-back; the 3D GEO case goes to p8-09's _DEF_AMGGEO).
// Spec: docs/re/commands/GFREAD_STL_GEO_NAS.md §2, §4; GFREAD.md §3 (3D table).
#pragma once

#include <cstdint>

namespace def::model {
class LLOBJ_DATA;
}

namespace def::cmd::stlnas {

// The GFREAD loader object of p8-01 (FUN_1402437c0, def::cmd::gfread::Loader, 0x18 bytes):
// +0x00 double STL merge tolerance (1.0E-5), +0x08 double scale (1.0), +0x10 an optional callback
// `char (*)(int)` (0 in GFREAD). The parameter is untyped so that p8-01's Loader* converts to it
// without either header including the other; the layout above is what fun_140243a70 reads.
struct LoaderView {
    double tol;                       // +0x00
    double scale;                     // +0x08
    char (*callback)(std::int32_t);   // +0x10
};
static_assert(sizeof(LoaderView) == 0x18);

// @src 140243a70  GFREAD 3D table: type 2 GEO, 3 STL, 4 PDA/PAT, 5 UNV, 10 NAS; other types do
// nothing and succeed. Returns the reader/store result (true = Done).
bool fun_140243a70(const void* loader, const char* file, def::model::LLOBJ_DATA* obj, std::int32_t type);
inline bool gfread_3d_table(const void* loader, const char* file, def::model::LLOBJ_DATA* obj,
                            std::int32_t type) {
    return fun_140243a70(loader, file, obj, type);
}

}  // namespace def::cmd::stlnas
