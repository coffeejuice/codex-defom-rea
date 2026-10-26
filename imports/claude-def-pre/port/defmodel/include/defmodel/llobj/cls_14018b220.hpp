// Cls_14018b220 (no RTTI): LLOBJ_DATA+0x2160, sixteen DYNARY tables (byte slots, destroyed
// explicitly in the binary's order) and a flag at 0x8b8.
//
//   ctor     14018b220 (Phase 2, not ported here)
//   dtor     14018b3b0 (reset 14018bc30, then the 16 DYNARY dtors from 0x9e0 down to 0x70)
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class Cls_14018b220 {
public:
    Cls_14018b220();         // 14018b220, Phase 2 (DEF_NOT_PORTED)
    ~Cls_14018b220();
    Cls_14018b220(const Cls_14018b220&) = delete;
    Cls_14018b220& operator=(const Cls_14018b220&) = delete;

    void reset();            // 14018bc30

    // Offsets of the 16 DYNARY tables, ascending.
    static constexpr std::uint32_t kTables[16] = {0x070, 0x0e8, 0x208, 0x2a0, 0x338, 0x3b0, 0x448, 0x518,
                                                  0x590, 0x660, 0x6d8, 0x790, 0x808, 0x8c0, 0x968, 0x9e0};

    std::byte raw[0xa88];    // tables at kTables, flag byte at 0x8b8, 0xa80 written by the ctor
};

static_assert(sizeof(Cls_14018b220) == 0xa88);

} // namespace def::model
