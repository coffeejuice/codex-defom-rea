// defmodel/llobj_ctor/llobj_ctor.hpp: LLOBJ_DATA construction and initialisation, the object
// list's add / get-or-create, emptying an object, and the per-object state remap. Packet kw-g2obj
// (Phase 2; the LLOBJ_DATA ctor family is assigned to it). The ctor itself is
// LLOBJ_DATA::LLOBJ_DATA (layout/llobj_data.hpp); the member ctors of the p1-04 sub-object
// classes are defined in src/llobj_ctor/member_ctors.cpp.
//
// The wrappers in namespace ext name the calls into other packets' classes (p1-06, p1-07).
#pragma once

#include <cstddef>
#include <cstdint>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
class Cls_14017a6c0;
}

namespace def::model::llobj_ctor {

// @src 14015ef60  owner = m, the sub-objects' owners (each re-applies its defaults), then
// 14015f0b0
void set_owner(LLOBJ_DATA* obj, DB_STEP_DATA* m);
// @src 14015f0b0  object initialisation (type 0, empty name, defaults of every sub-object,
// the 100 geometry slots, the REFTMP default by unit system)
void init(LLOBJ_DATA* obj);

// @src 14017a880  object list add: operator new(0x3f28), LLOBJ_DATA(list owner), link at the tail
LLOBJ_DATA* list_add(Cls_14017a6c0* list);
// @src 14019a280  object list count (+0x10)
std::int32_t list_count(const Cls_14017a6c0* list);
// @src 14019ccc0  get-or-create object `no` in model+0x1380 (null for no < 1); missing lower
// numbers become placeholder objects of type 7
LLOBJ_DATA* get_or_create(DB_STEP_DATA* m, std::int32_t no);

// @src 140161260  empty the object (OBJTYP code -> internal type 7)
void make_empty(LLOBJ_DATA* obj);

// @src 140199650  re-derive the nodal (+0x8a0) and element (+0x9e0) state arrays for new sizes
void remap_state(LLOBJ_DATA* obj, const std::int32_t* sizes);

// 140301950: the object at LLOBJ_DATA+0x3998: a CowList<GEO2D_AND_MESH2D> with one element
void* new_u3998();

namespace ext {
// 1402f9020: OBJ_GEO3D ctor (p1-07)
void geo3d_construct(void* p);
// virtual slot 2 of an _OBJ_GEO3D (vtable +0x10), called by 14015f0b0 and 140161260
void geo3d_slot2(void* p);
// 14025ae30: OpStopLinkedList ctor (p1-06)
void opstop_list_construct(void* p);
} // namespace ext

} // namespace def::model::llobj_ctor
