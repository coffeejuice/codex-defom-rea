// Cls_140289930 (no RTTI, not polymorphic): diffusion-coefficient block of a Material
// (Material+0x950), data of keyword DIFCOE (docs/re/keywords/DIFCOE.md).
//
//   ctor     140289930 (3 DYNARYs, mode = 0, types 0x29/0x27/0x28, routine = 0)
//   dtor     14029ce90
//   size     0x178: last write is the int at 0x170 (param_1[0x5c]); Material's next member is at
//            0xac8 = 0x950 + 0x178.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class Cls_140289930 {
public:
    // @src 140289930  ctor
    Cls_140289930();
    // @src 14029ce90  dtor
    ~Cls_140289930();

    std::int32_t mode;           // 0x000 =0; DIFCOE mode 0..4 (mat+0x950)
    std::byte pad_004[4];        // 0x004
    DYNARY d0;                   // 0x008 type 0x29; DIFCOE t0 constant / t1 table (mat+0x958)
    DYNARY d2;                   // 0x080 type 0x27; DIFCOE t2 (mat+0x9d0)
    DYNARY d3;                   // 0x0f8 type 0x28; DIFCOE t3 (mat+0xa48)
    std::int32_t user_routine;   // 0x170 =0; DIFCOE -t (mat+0xac0)
    std::byte pad_174[4];        // 0x174
};

static_assert(sizeof(Cls_140289930) == 0x178);
DEF_LAYOUT_OFFSET(Cls_140289930, d0, 0x008);
DEF_LAYOUT_OFFSET(Cls_140289930, d2, 0x080);
DEF_LAYOUT_OFFSET(Cls_140289930, d3, 0x0f8);
DEF_LAYOUT_OFFSET(Cls_140289930, user_routine, 0x170);

} // namespace def::model
