// defmodel/linked/axis_point_list.hpp: list of 0x40-byte records {id, point, direction} that the
// 3D objects rotate with the object (1402fb550 rotates _OBJ_GEO3D+0x28; 14021f830 the list at
// +0x18 of another object, 3D only) and that 1402059a0 / 140205e40 fill with surface normals
// (id + 600 is stored per face). Several classes embed one (1401958b0 destroys two at +0x18 and
// +0x30, _OBJ_GEO3D's dtor 1402f91f0 one at +0x28).
//
//   node Cls_140257340 (0x40): ctor 140257340, copy 1402573c0, dtor 140257440 (empty), scalar
//        deleting dtor 14025bf00, rotate 140257750; arrays of nodes are built with
//        `eh vector constructor iterator` (1402f2a30).
//   list Cls_140257340List (0x18, no vtable): add 140257e90, clear 1402581b0, nth 140258090
//        (1-based), copy 140257c80, dtor 140257c60, rotate all 140258400; count is read through
//        the generic getter 140158e70 (+0x10, p1-04).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model::linked {

class Cls_140257340 {
public:
    // @src 140257340  ctor: id = 0, point = direction = 0, next = 0
    Cls_140257340();
    // @src 140257440  dtor (empty)
    // @src 14025bf00  scalar deleting dtor
    ~Cls_140257340();

    // @src 1402573c0  copy id, point and direction (next is not copied)
    void copy_from(const Cls_140257340* other);
    // @src 140257750  rotate by angle (degrees) about the axis given by two 3-vectors a, b (the
    //                 Fortran rotation 140325691, p1-10): point is rotated, direction is
    //                 rotated, made relative to the rotated origin and normalised. Nothing happens
    //                 when a or b is null.
    void rotate(const double* a, const double* b, double angle);

    std::int32_t id;          // 0x00
    std::byte pad_04[4];      // 0x04
    double point[3];          // 0x08
    double direction[3];      // 0x20
    Cls_140257340* next;      // 0x38
};

static_assert(sizeof(Cls_140257340) == 0x40);
DEF_LAYOUT_OFFSET(Cls_140257340, point, 0x08);
DEF_LAYOUT_OFFSET(Cls_140257340, direction, 0x20);
DEF_LAYOUT_OFFSET(Cls_140257340, next, 0x38);

class Cls_140257340List {
public:
    Cls_140257340List() = default; // owners zero the three fields in their own ctors
    // @src 140257c60  dtor: clear
    ~Cls_140257340List();
    Cls_140257340List(const Cls_140257340List&) = delete;
    Cls_140257340List& operator=(const Cls_140257340List&) = delete;

    // @src 140257e90  append a new node with the given id; returns it
    Cls_140257340* add(std::int32_t id);
    // @src 1402581b0  delete every node, then head = tail = 0, count = 0
    void clear();
    // @src 140258090  node k (1-based; k <= 1 gives the head), null past the end
    Cls_140257340* nth(std::int32_t k) const;
    // @src 140257c80  clear, then append a copy of every node of other
    void copy_from(const Cls_140257340List& other);
    // @src 140258400  rotate every node (Cls_140257340::rotate)
    void rotate_all(const double* a, const double* b, double angle);
    // 140158e70 (p1-04): the count at +0x10
    std::int32_t size() const { return count; }

    Cls_140257340* head = nullptr;  // 0x00
    Cls_140257340* tail = nullptr;  // 0x08
    std::int32_t count = 0;         // 0x10
    std::byte pad_14[4] = {};       // 0x14
};

static_assert(sizeof(Cls_140257340List) == 0x18);
DEF_LAYOUT_OFFSET(Cls_140257340List, tail, 0x08);
DEF_LAYOUT_OFFSET(Cls_140257340List, count, 0x10);

} // namespace def::model::linked
