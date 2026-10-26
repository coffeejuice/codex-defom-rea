// Cls_140269250 (no RTTI, not polymorphic): owning singly linked list of LLWINDEF_DATA nodes
// (0x920 bytes, polymorphic, link at node+0x840), embedded in the inter-object record
// (Cls_1401f46e0+0x188) and in several object classes (1401958b0, 140189520 destroy 7 and 3 of
// them). Owner is the DB_STEP_DATA whose dimension the list caches.
//
//   ctor     140269250(this, owner) (owner, then 140269290 init)
//   init     140269290 (head = tail = 0, count = 0, u18 = 2, u28 = 0, u10 = -1; u18 = dimension
//            when owner is set, through 14026a180)
//   owner    14026a120(this, owner) (stores owner; u18 = owner dimension when set)
//   clear    1402699c0 (deletes every node through its virtual slot 0, then head = tail = 0 and
//            count = 0; u10, u18, u28 kept)
//   dtor     140269320 (clear)
//   add      1402694a0 (operator new(0x920), LLWINDEF_DATA ctor 14025c100(node, owner), link
//            1402693f0); nth 140269900 (1-based, walks node+0x840); copy 140269340 copies u18, u28
//   size     0x30: Cls_1401f46e0's next field is at 0x1b8 = 0x188 + 0x30.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class DB_STEP_DATA;
class LLWINDEF_DATA;

class Cls_140269250 {
public:
    // @src 140269250  ctor
    explicit Cls_140269250(DB_STEP_DATA* owner = nullptr);
    // @src 140269320  dtor
    ~Cls_140269250();

    // @src 140269290  init
    void reset();
    // @src 14026a120  set the owner and take its dimension
    void set_owner(DB_STEP_DATA* owner);
    // @src 14026a180  u18 = *dim
    void set_dimension(const std::int32_t* dim);
    // @src 1402699c0  delete all nodes
    void clear();

    LLWINDEF_DATA* head;      // 0x00
    LLWINDEF_DATA* tail;      // 0x08
    std::int32_t u10;         // 0x10 =-1 (init)
    std::int32_t count;       // 0x14 node count
    std::int32_t u18;         // 0x18 dimension (=2 until an owner is known)
    std::byte pad_1c[4];      // 0x1c
    DB_STEP_DATA* owner;      // 0x20
    std::uint64_t u28;        // 0x28 =0 (copied by 140269340)
};

static_assert(sizeof(Cls_140269250) == 0x30);
DEF_LAYOUT_OFFSET(Cls_140269250, tail, 0x08);
DEF_LAYOUT_OFFSET(Cls_140269250, u10, 0x10);
DEF_LAYOUT_OFFSET(Cls_140269250, count, 0x14);
DEF_LAYOUT_OFFSET(Cls_140269250, u18, 0x18);
DEF_LAYOUT_OFFSET(Cls_140269250, owner, 0x20);
DEF_LAYOUT_OFFSET(Cls_140269250, u28, 0x28);

} // namespace def::model
