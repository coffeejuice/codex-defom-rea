// Cls_1401364a0 (no RTTI, not polymorphic): intrusive singly linked list head used by the
// furnace model (Cls_1403753f0+0x40). Nodes link through node+0x60 and are deleted with their
// scalar deleting dtor 14013c170(node, 1) (the same deleter DYNARY uses for its 0x68 object).
//
//   ctor     1401364a0 (-> 1401364d0: head = tail = 0, count = 0)
//   dtor     140136510 (-> 140136910: delete every node, reset)
//   add      140136670 (appends a node, returns it; used by 140375600)
//   nodes    DYNARY (operator new(0x78) + 14012d050 in 140136670); the link is DYNARY+0x60
//   size     0x18: three fields written by 1401364d0; the furnace class zeroes its own bytes from
//            0x58 = 0x40 + 0x18 on (140375470).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class DYNARY;

class Cls_1401364a0 {
public:
    // @src 1401364a0  ctor
    Cls_1401364a0();
    // @src 140136510  dtor
    ~Cls_1401364a0();

    // Methods: defmodel/src/llobj/dynary_table.cpp
    // @src 1401364d0  head = tail = 0, count = 0
    void init();
    // @src 140136670  append a new DYNARY, return it
    DYNARY* add();
    // @src 140136860  n-th node, 1-based (n < 2: head)
    DYNARY* at(std::int32_t n) const;
    // @src 140136910  delete all nodes, reset
    void clear();
    // @src 14013c1b0  count
    std::int32_t size() const;

    void* head;          // 0x00 first node (next pointer at node+0x60)
    void* tail;          // 0x08 last node
    std::int32_t count;  // 0x10 number of nodes
    std::byte pad_14[4]; // 0x14
};

static_assert(sizeof(Cls_1401364a0) == 0x18);
DEF_LAYOUT_OFFSET(Cls_1401364a0, head, 0x00);
DEF_LAYOUT_OFFSET(Cls_1401364a0, tail, 0x08);
DEF_LAYOUT_OFFSET(Cls_1401364a0, count, 0x10);

} // namespace def::model
