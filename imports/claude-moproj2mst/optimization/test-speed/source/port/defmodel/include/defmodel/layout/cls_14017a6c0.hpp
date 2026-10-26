// Cls_14017a6c0 (no RTTI, not polymorphic): intrusive singly linked list of LLOBJ_DATA owned by DB_STEP_DATA
// (objects; two instances at DB_STEP_DATA+0x1380 and +0x13a8). Same shape as the other DB_STEP_DATA lists: head, tail, count, owner.
//
//   ctor     14017a6c0(this, owner) (stores owner, then 14017a700 resets head/tail/count)
//   reset    14017a700 (head = tail = 0, count = 0; no free)
//   clear    14017ab70 (deletes every node through 14019a290(node, 1), then resets)
//   dtor     14017a740
//   add      14017a880 (operator new(0x3f28), 14015ec80(node, owner); links via node+0x3940)
//   size     0x20: owner at +0x18 (ctor, 1401d8340); 0x13a8 - 0x1380 = 0x28 leaves 8 unknown bytes in DB_STEP_DATA.
//   open     the role of the second list (0x13a8); 14019a290 is LLOBJ_DATA's non-virtual scalar deleting dtor (dtor 14015f790); 14017aa80(list, n) returns node n.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class DB_STEP_DATA;
class LLOBJ_DATA;

class Cls_14017a6c0 {
public:
    // @src 14017a6c0  ctor (DB_STEP_DATA passes owner = 0, then sets it with 1401d8340)
    explicit Cls_14017a6c0(DB_STEP_DATA* owner = nullptr);
    // @src 14017a740  dtor
    ~Cls_14017a6c0();

    // Methods: defmodel/src/llobj/llobj_list.cpp
    // @src 14017a700  head = tail = 0, count = 0 (no free)
    void reset();
    // @src 14017aa80  n-th node, 1-based (n < 2: head)
    LLOBJ_DATA* at(std::int32_t n) const;
    // @src 14017ab70  delete every node (14019a290(node, 1)), then reset
    void clear();

    LLOBJ_DATA* head;            // 0x00 first node
    LLOBJ_DATA* tail;            // 0x08 last node (new nodes are linked at tail+0x3940)
    std::int32_t count;  // 0x10 number of nodes
    std::byte pad_14[4]; // 0x14
    DB_STEP_DATA* owner; // 0x18 ctor / 1401d8340; passed to each new node
};

static_assert(sizeof(Cls_14017a6c0) == 0x20);
DEF_LAYOUT_OFFSET(Cls_14017a6c0, head, 0x00);
DEF_LAYOUT_OFFSET(Cls_14017a6c0, tail, 0x08);
DEF_LAYOUT_OFFSET(Cls_14017a6c0, count, 0x10);
DEF_LAYOUT_OFFSET(Cls_14017a6c0, owner, 0x18);

} // namespace def::model
