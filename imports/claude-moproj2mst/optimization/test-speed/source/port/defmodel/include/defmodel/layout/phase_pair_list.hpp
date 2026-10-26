// PhasePairList (no RTTI, not polymorphic; ctor 140339c90): singly linked list of PhasePair
// (inter-material / phase-transformation records) owned by DB_STEP_DATA (+0x1358).
//
//   ctor     140339c90(this, owner) (owner, then 140339cd0: head = tail = 0, count = 0)
//   reset    140339cd0
//   clear    14033a120 (deletes every node through 14033a990(node, 1), then resets)
//   dtor     140339d10 (-> 14033a120)
//   add      140339df0 (operator new(0x780), 140333f60(node, owner); links via node+0x770)
//   find     14033a040(list, from, to): walk until node->from == from && node->to == to
//   count    1401b05f0 returns +0x10
//   size     0x20: owner at +0x18 (ctor, 1401d8320); DB_STEP_DATA's next field is at
//            0x1378 = 0x1358 + 0x20.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class DB_STEP_DATA;
class PhasePair;

class PhasePairList {
public:
    // @src 140339c90  ctor (DB_STEP_DATA passes owner = 0, then sets it with 1401d8320)
    explicit PhasePairList(DB_STEP_DATA* owner = nullptr);
    // @src 140339d10  dtor
    ~PhasePairList();
    PhasePairList(const PhasePairList&) = delete;
    PhasePairList& operator=(const PhasePairList&) = delete;

    // @src 140339cd0  head = tail = 0, count = 0 (no free)
    void reset();
    // @src 14033a120  delete every node, then reset
    void clear();

    PhasePair* head;             // 0x00 first node
    PhasePair* tail;             // 0x08 last node (new nodes are linked at tail+0x770)
    std::int32_t count;          // 0x10 number of nodes (1401b05f0)
    std::byte pad_14[4];         // 0x14
    DB_STEP_DATA* owner;         // 0x18 ctor / 1401d8320; passed to each new node
};

static_assert(sizeof(PhasePairList) == 0x20);
DEF_LAYOUT_OFFSET(PhasePairList, head, 0x00);
DEF_LAYOUT_OFFSET(PhasePairList, tail, 0x08);
DEF_LAYOUT_OFFSET(PhasePairList, count, 0x10);
DEF_LAYOUT_OFFSET(PhasePairList, owner, 0x18);

} // namespace def::model
