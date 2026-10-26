// Cls_1401fa020 (no RTTI, not polymorphic): intrusive singly linked list of Cls_1401f46e0 owned by DB_STEP_DATA
// (inter-object data, DB_STEP_DATA+0x13c8). Same shape as the other DB_STEP_DATA lists: head, tail, count, owner.
//
//   ctor     1401fa020(this, owner) (stores owner, then 1401fa080 resets head/tail/count)
//   reset    1401fa080 (head = tail = 0, count = 0; no free)
//   clear    1401fade0 (deletes every node through 1401fb200(node, 1), then resets)
//   dtor     1401fa060
//   add      1401faaa0 (operator new(0x3a0), 1401f46e0(node, owner); links via node+0x390)
//   size     0x20: owner at +0x18 (ctor, 1401d8360); DB_STEP_DATA's next field is at 0x13e8 = 0x13c8 + 0x20.
//   open     none (Ghidra labels the element dtor 1401f4950 ~CBitmapButton: a FID false match).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class DB_STEP_DATA;
class Cls_1401f46e0;

class Cls_1401fa020 {
public:
    // @src 1401fa020  ctor (DB_STEP_DATA passes owner = 0, then sets it with 1401d8360)
    explicit Cls_1401fa020(DB_STEP_DATA* owner = nullptr);
    // @src 1401fa060  dtor
    ~Cls_1401fa020();

    // @src 1401fa080  head = tail = 0, count = 0 (no free)
    void reset();
    // @src 1401fade0  delete every node (1401fb200: dtor 1401f4950 + free), then reset
    void clear();

    Cls_1401f46e0* head;            // 0x00 first node
    Cls_1401f46e0* tail;            // 0x08 last node (new nodes are linked at tail+0x390)
    std::int32_t count;  // 0x10 number of nodes
    std::byte pad_14[4]; // 0x14
    DB_STEP_DATA* owner; // 0x18 ctor / 1401d8360; passed to each new node
};

static_assert(sizeof(Cls_1401fa020) == 0x20);
DEF_LAYOUT_OFFSET(Cls_1401fa020, head, 0x00);
DEF_LAYOUT_OFFSET(Cls_1401fa020, tail, 0x08);
DEF_LAYOUT_OFFSET(Cls_1401fa020, count, 0x10);
DEF_LAYOUT_OFFSET(Cls_1401fa020, owner, 0x18);

} // namespace def::model
