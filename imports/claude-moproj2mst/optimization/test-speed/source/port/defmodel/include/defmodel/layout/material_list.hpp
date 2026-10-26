// MaterialList (no RTTI, not polymorphic; ctor 14029b4c0): singly linked list of Material owned
// by DB_STEP_DATA (+0x1328), with two number<->index arrays that are built only while the
// material set changes (_common_g457.md section 5).
//
//   ctor     14029b4c0(this, owner) (owner, then 14029b500: head = tail = 0, count = 0,
//            arrays 0x18/0x20 = 0)
//   reset    14029b500
//   clear    14029c310 (deletes every node through 14029e210(node, 1), then resets)
//   dtor     14029b550 (14029c310, then free numbers / index_of)
//   add      14029c060 (operator new(0x2218), 14028c6f0(node, owner); links via node+0x2200)
//   find     14029c2c0(list, number): walk until node->number == number
//   nth      14029c260(list, i) (1-based, used by 14029ca10); count 14029cd50 = +0x10
//   rebuild  14029ca10(list, free_only): frees both arrays; if free_only == 0, callocs count
//            ints for each and fills numbers[i] = nth(i+1)->number, index_of[i] =
//            14029c870(list, number) (helper not decompiled; the spec calls the pair
//            "number->index arrays")
//   size     0x30: owner at +0x28 (ctor, 1401d8300); DB_STEP_DATA's next member is at
//            0x1358 = 0x1328 + 0x30.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class DB_STEP_DATA;
class Material;

class MaterialList {
public:
    // @src 14029b4c0  ctor (DB_STEP_DATA passes owner = 0, then sets it with 1401d8300)
    explicit MaterialList(DB_STEP_DATA* owner = nullptr);
    // @src 14029b550  dtor
    ~MaterialList();

    Material* head;              // 0x00 first node
    Material* tail;              // 0x08 last node (new nodes are linked at tail+0x2200)
    std::int32_t count;          // 0x10 number of nodes (14029cd50)
    std::byte pad_14[4];         // 0x14
    std::int32_t* numbers;       // 0x18 material number per index (14029ca10)
    std::int32_t* index_of;      // 0x20 14029c870(list, number) per index (14029ca10)
    DB_STEP_DATA* owner;         // 0x28 ctor / 1401d8300; passed to each new node
};

static_assert(sizeof(MaterialList) == 0x30);
DEF_LAYOUT_OFFSET(MaterialList, head, 0x00);
DEF_LAYOUT_OFFSET(MaterialList, tail, 0x08);
DEF_LAYOUT_OFFSET(MaterialList, count, 0x10);
DEF_LAYOUT_OFFSET(MaterialList, numbers, 0x18);
DEF_LAYOUT_OFFSET(MaterialList, index_of, 0x20);
DEF_LAYOUT_OFFSET(MaterialList, owner, 0x28);

} // namespace def::model
