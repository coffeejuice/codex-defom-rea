// ACTION_DB_STEP (RTTI .?AV_ACTION_DB_STEP@@, base class only, not polymorphic): second base of
// DB_STEP_DATA, at DB_STEP_DATA+0x140.
//
//   ctor     1401d8380 (writes byte 0 = 0)       dtor 1401d83a0 (empty)
//   vtable   none
//   size     0x200 (assumed, see open question). Evidence: command keyword 0x11e in
//            FUN_140378e00 calls 140382320(step+0x140, line) = strcpy into this+0 (line read with
//            fgets(..., 0x400)); DB_STEP_DATA's own char buffer at 0x340 is filled with
//            strncpy(step+0x340, s, 0x400) in the same function, so this class ends at or before
//            0x340-0x140 = 0x200.
//   method   140382320 sets the text (strcpy); not declared here (layout only).
//   open     whether the 0x400-byte buffer at DB_STEP_DATA+0x340 (and the ints at 0x740..0x748)
//            belong to this class or to DB_STEP_DATA cannot be told from the code seen: the
//            ctor only touches byte 0. Byte offsets in DB_STEP_DATA are the same either way.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class ACTION_DB_STEP {
public:
    // @src 1401d8380  ctor: text[0] = 0
    ACTION_DB_STEP();
    // @src 1401d83a0  dtor (empty)
    ~ACTION_DB_STEP();

    char text[0x200];  // 0x000 cleared by 1401d8380; strcpy by 140382320 (KEY command 0x11e)
};

static_assert(sizeof(ACTION_DB_STEP) == 0x200);
DEF_LAYOUT_OFFSET(ACTION_DB_STEP, text, 0x000);

} // namespace def::model
