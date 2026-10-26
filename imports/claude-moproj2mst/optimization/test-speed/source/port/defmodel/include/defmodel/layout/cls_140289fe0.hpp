// Cls_140289fe0 (no RTTI, not polymorphic): anisotropy block of a Material (Material+0x1e40),
// data of keyword ANISO (docs/re/keywords/ANISO.md).
//
//   ctor     140289fe0 (DYNARY at 8, t = 0, ref = 0)
//   dtor     14029cff0
//   size     0x88: last write is the int at 0x80 (param_1[0x20]); Material's next field (FRAE2H)
//            is at 0x1ec8 = 0x1e40 + 0x88.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class Cls_140289fe0 {
public:
    // @src 140289fe0  ctor
    Cls_140289fe0();
    // @src 14029cff0  dtor
    ~Cls_140289fe0();

    std::int32_t t;              // 0x000 =0; ANISO t (mat+0x1e40)
    std::byte pad_004[4];        // 0x004
    DYNARY d;                    // 0x008 ANISO table, types 0x68/0x69/0x67/0x65 (mat+0x1e48)
    std::int32_t ref;            // 0x080 =0; ANISO -t (mat+0x1ec0)
    std::byte pad_084[4];        // 0x084
};

static_assert(sizeof(Cls_140289fe0) == 0x88);
DEF_LAYOUT_OFFSET(Cls_140289fe0, d, 0x008);
DEF_LAYOUT_OFFSET(Cls_140289fe0, ref, 0x080);

} // namespace def::model
