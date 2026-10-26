// Cls_14028bcc0 (no RTTI, not polymorphic): four DYNARYs at Material+0x18f0, the data of
// keywords BURGRS, ALPHA, NDISFM and RECVRY (docs/re/keywords/<KW>.md).
//
//   ctor     14028bcc0 (4 x DYNARY ctor, all type 0)
//   dtor     14028bd20
//   size     0x1e0: 4 x 0x78; Material's next member is at 0x1ad0 = 0x18f0 + 0x1e0.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class Cls_14028bcc0 {
public:
    // @src 14028bcc0  ctor
    Cls_14028bcc0();
    // @src 14028bd20  dtor
    ~Cls_14028bcc0();

    DYNARY burgrs;               // 0x000 (mat+0x18f0) BURGRS
    DYNARY alpha;                // 0x078 (mat+0x1968) ALPHA
    DYNARY ndisfm;               // 0x0f0 (mat+0x19e0) NDISFM
    DYNARY recvry;               // 0x168 (mat+0x1a58) RECVRY
};

static_assert(sizeof(Cls_14028bcc0) == 0x1e0);
DEF_LAYOUT_OFFSET(Cls_14028bcc0, alpha, 0x078);
DEF_LAYOUT_OFFSET(Cls_14028bcc0, ndisfm, 0x0f0);
DEF_LAYOUT_OFFSET(Cls_14028bcc0, recvry, 0x168);

} // namespace def::model
