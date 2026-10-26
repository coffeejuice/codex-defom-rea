// defmodel/linked/record_e8_list.hpp: list of 0xe8-byte records (link at +0xe0), embedded at
// +0x200 of the object class whose clear is 140186510 / dtor 140186130 (next to a
// DynAryLinkedList at +0x218).
//
//   node Cls_140258580 (0xe8, no vtable): ctor 140258580 -> 1402585b0 (not Phase 1, not
//        ported), dtor 140258990 (empty), scalar deleting dtor 14025bf40, copy 140258750.
//   list Cls_140258580List (0x18): add 140259950 (operator new(0xe8); id = argument, or the new
//        count when it is 0), clear 140259c70, dtor 1402598a0, copy 1402598c0, nth 140259b60.
//   p1-06 ports the dtors and the clear; the rest belongs to the node's class (not Phase 1).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model::linked {

class Cls_140258580 {
public:
    // 140258580 ctor: not Phase 1 (DEF_NOT_PORTED)
    Cls_140258580();
    // @src 140258990  dtor (empty)
    // @src 14025bf40  scalar deleting dtor
    ~Cls_140258580();

    std::int32_t id;          // 0x00 (140259950)
    std::byte body[0xdc];     // 0x04 not recovered
    Cls_140258580* next;      // 0xe0
};

static_assert(sizeof(Cls_140258580) == 0xe8);
DEF_LAYOUT_OFFSET(Cls_140258580, next, 0xe0);

class Cls_140258580List {
public:
    Cls_140258580List() = default;
    // @src 1402598a0  dtor: clear
    ~Cls_140258580List();
    Cls_140258580List(const Cls_140258580List&) = delete;
    Cls_140258580List& operator=(const Cls_140258580List&) = delete;

    // @src 140259c70  delete every node, then head = tail = 0, count = 0
    void clear();

    Cls_140258580* head = nullptr;  // 0x00
    Cls_140258580* tail = nullptr;  // 0x08
    std::int32_t count = 0;         // 0x10
    std::byte pad_14[4] = {};       // 0x14
};

static_assert(sizeof(Cls_140258580List) == 0x18);
DEF_LAYOUT_OFFSET(Cls_140258580List, count, 0x10);

} // namespace def::model::linked
