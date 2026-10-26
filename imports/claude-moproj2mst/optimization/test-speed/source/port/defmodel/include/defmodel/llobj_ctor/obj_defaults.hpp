// defmodel/llobj_ctor/obj_defaults.hpp: LLOBJ_DATA defaults of the property (0x1ea0), movement
// (0x2160) and mesh-generation (0x2d10) sub-objects, their owner setters, and the small
// helpers they call. Packet kw-g2obj (Phase 2: the object ctor and defaults are assigned to it).
//
// The sub-objects are p1-04's classes with byte-slot layouts (defmodel/llobj/cls_*.hpp), so these
// functions take the sub-object's address and store at the binary's offsets, like the decompile.
// Specs: docs/re/keywords/group2-helpers.md ("Object defaults") and each keyword's spec.
#pragma once

#include <cstddef>
#include <cstdint>

namespace def::model {
class DB_STEP_DATA;
}

namespace def::model::llobj_ctor {

// @src 140186200  defaults of the property sub-object (LLOBJ_DATA+0x1ea0); owner at +0x2b8
void props_defaults(std::byte* p);
// @src 1401861c0  owner = m, then 140186200
void props_set_owner(std::byte* p, DB_STEP_DATA* m);

// @src 14018b500  defaults of the movement sub-object (LLOBJ_DATA+0x2160); owner at +0xa80
void movement_defaults(std::byte* p);
// @src 14018b370  owner = m, then 14018b500
void movement_set_owner(std::byte* p, DB_STEP_DATA* m);

// @src 140189790  defaults of the mesh-generation sub-object (LLOBJ_DATA+0x2d10); owner at +0
void meshgen_defaults(std::byte* p, bool with_windows);
// @src 1401894d0  owner = m, 140189790(p, 1), 14026a120(p+0x40, m)
void meshgen_set_owner(std::byte* p, DB_STEP_DATA* m);

// @src 1401889f0  defaults of the sub-object at LLOBJ_DATA+0x2be8 (GEO23 block)
void geo23_defaults(std::byte* p);
// @src 140189200  +8 = *v; +0x3c = (int)(*v / 3) or 10 when that is <= 10; +0x10 = (180 - *v) / 2
// when byte +0x80 is set
void geo23_set_angle(std::byte* p, const double* v);
// @src 1401890e0  +0x120 = m
void geo23_set_owner(std::byte* p, DB_STEP_DATA* m);

// Cls_140269250 (0x30-byte window-shape object used by the mesh windows and the BC lists)
// p1-06 140269250 (Cls_140269250): ctor: +0x20 = m, then 140269290
void shape_construct(std::byte* q, DB_STEP_DATA* m);
// p1-06 140269290 (Cls_140269250): reset: +0, +8 = 0, +0x14 = 0, +0x18 = 2, +0x28 = 0, +0x10 = -1; +0x18 = dim
// when an owner is set
void shape_reset(std::byte* q);
// p1-06 14026a120 (Cls_140269250): +0x20 = m; +0x18 = dim(m) when m is set
void shape_set_owner(std::byte* q, DB_STEP_DATA* m);
// p1-06 14026a180 (Cls_140269250): +0x18 = *dim
void shape_set_dim(std::byte* q, const std::int32_t* dim);

// @src 140257c20  +0, +8 = 0, int +0x10 = 0 (also the body of the list ctor 140257bf0)
void list3_reset(std::byte* p);
// @src 140259860  the same three stores (body of the ctor 140259830)
void list3_reset_b(std::byte* p);

// @src 14015f9a0  REFTMP default by unit system: `unit` 0 means "ask the owner model"
// (FUN_1401c8f40); 1 -> +0x1eb8 = 20.0, else 68.0; +0x1ec0 = 1.0 either way. `obj` is the
// LLOBJ_DATA (owner pointer at +0x3a98).
void object_unit_defaults(std::byte* obj, std::int32_t unit);
// @src 1401c8f40  the model's UNIT (simulation control +0xfc via FUN_1401f4090, p1-03)
std::int32_t model_unit(const DB_STEP_DATA* m);

// @src 140165500  element kind +0x890 = kind; 2 -> +0x894/898/89c = 4, 4, 3; 4 -> 8, 6, 4
void set_element_kind(std::byte* obj, std::int32_t kind);
// @src 140199a00  +0x39d4 = 0, +0x39d8 = 0, +0x39e0..+0x39ec = 0, +0x39f0 = 1
void reset_u39d4(std::byte* obj);
// @src 140199af0  +0x39c0 = v
void set_u39c0(std::byte* obj, std::int32_t v);

// The model's dimension (FUN_140158e90: model+0x1c), as the sub-objects read it from their owner.
std::int32_t owner_dimension(const DB_STEP_DATA* m);

// FUN_140235c10: 1 (SI) when the unit flag is 0, else 2 (p1-12, defrt/ftn/units.hpp).
std::int32_t unit_system();

} // namespace def::model::llobj_ctor
