// LLOBJ_DATA ctor 14015ec80, owner/init 14015ef60 / 14015f0b0, the object list add 14017a880 and
// get-or-create 14019ccc0, emptying 140161260, state remap 140199650 (llobj_ctor.hpp).
// Decompiles: deform-rea-work/out/g2obj/ctor_closure.c (closure of 14015ec80), copy_closure.c.
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>

#include <new>

#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/cls_14017a6c0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/ext.hpp>
#include <defmodel/llobj/llobj_parts.hpp>
#include <defmodel/llobj_ctor/obj_defaults.hpp>
#include <defmodel/sdctl/cow.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/layout/op_stop_linked_list.hpp>

namespace def::model {

namespace {

std::byte* raw(void* p) { return static_cast<std::byte*>(p); }
template <class T>
T& at(void* p, std::size_t off)
{
    return *reinterpret_cast<T*>(raw(p) + off);
}
std::int32_t dimension_of(const DB_STEP_DATA* m) { return llobj_ctor::owner_dimension(m); }

// @src 140199ad0  SdctlPtrList<_OBJ_GEO2D>: owns (+8) = v (the list's slot 4 then deletes the items)
void set_owns(SdctlPtrCollection* list, std::uint8_t v) { list->owns = v; }

// rdata 140522f48  5.0   rdata 140522f40  8.0   rdata 140522f38  7.0
constexpr double k5 = 5.0, k8 = 8.0, k7 = 7.0;

// COW handle types at 0x3950..0x3978 (element types as p1-04's dtor uses them)
using CowP = CowVector<void*>;
using CowI = CowVector<std::int32_t>;

} // namespace

// @src 14015ec80  ctor. The members the layout header types (ub70 StateVarBase(-1, 0) 140300c90,
// uba8 DynAryLinkedList 14025b9d0, u3598 DYNARY 14012d050, u3a60 SdctlPtrList 14019aae0) are
// constructed by C++ before this body, i.e. before the byte-slot sub-objects instead of between
// them; none of these ctors has an effect outside its own object.
LLOBJ_DATA::LLOBJ_DATA(DB_STEP_DATA* owner_) : ub70(-1, nullptr)
{
    // @src 140199970  the 0x10 sub-object: +0, +4, +0x808, +0x80c, +0x810 = 0
    u010 = 0;
    u014 = 0;
    u818 = 0;
    u81c = 0;
    u820 = 0;
    using namespace llobj::parts;
    new (s8a0) Cls_14017ef20();
    new (s9e0) Cls_140182880();
    new (sae8) Cls_140185570();
    new (sb58) Cls_140185c60();
    for (int i = 0; i < 100; ++i) new (sbe0[i]) Cls_14017e740();  // eh vector ctor iterator
    new (s1ea0) Cls_1401860a0();
    new (s2160) Cls_14018b220();
    new (s2be8) Cls_1401889a0();
    new (s2d10) Cls_140189440();
    new (s2e68) Cls_1401956d0();
    llobj_ctor::ext::geo3d_construct(s3328);
    llobj_ctor::ext::geo3d_construct(s3458);
    new (s3920) Cls_140197a50();
    new (&u3950) CowP();  // 14019a8a0
    new (&u3958) CowP();  // 14019a8a0
    new (&u3960) CowP();  // 14019a8a0
    new (&u3968) CowI();  // 140159ad0
    new (&u3970) CowP();  // 14019a960
    new (&u3978) CowP();  // 14019aa20
    owner = nullptr;      // param_1[0x753]
    clear_u39c8();        // 1401999d0
    llobj_ctor::reset_u39d4(raw(this));  // 140199a00
    u39a0 = 0;
    u3998 = nullptr;
    u3bf8 = 0;
    for (int i = 0; i < 100; ++i) u3c00[i] = nullptr;
    if (owner_ != nullptr) llobj_ctor::set_owner(this, owner_);
    u3f20 = 1;
    u3f21 = 0;
    u3f22 = 0;
}

namespace llobj_ctor {

// @src 140195980  reset the BC lists: the seven window shapes, the DYNARY list (1401364d0), and in
// 3D the list at 0x18 (140257c20)
static void bc_lists_reset(std::byte* p)
{
    shape_reset(p + 0x48);
    shape_reset(p + 0x78);
    shape_reset(p + 0xa8);
    shape_reset(p + 0x168);
    shape_reset(p + 0xd8);
    shape_reset(p + 0x108);
    shape_reset(p + 0x138);
    reinterpret_cast<Cls_1401364a0*>(p)->init();
    DB_STEP_DATA* owner = at<DB_STEP_DATA*>(p, 0x198);
    if (dimension_of(owner) == 3) list3_reset(p + 0x18);
}

// @src 1401957b0  owner of the BC lists: +0x198 = m, the shapes' owners, then 140195980
static void bc_lists_set_owner(std::byte* p, DB_STEP_DATA* m)
{
    at<DB_STEP_DATA*>(p, 0x198) = m;
    shape_set_owner(p + 0x48, m);
    shape_set_owner(p + 0x78, m);
    shape_set_owner(p + 0xa8, m);
    shape_set_owner(p + 0x168, m);
    shape_set_owner(p + 0xd8, m);
    shape_set_owner(p + 0x108, m);
    shape_set_owner(p + 0x138, m);
    bc_lists_reset(p);
}

// @src 14015ef60  owner, sub-object owners, then 14015f0b0
void set_owner(LLOBJ_DATA* obj, DB_STEP_DATA* m)
{
    using namespace llobj::parts;
    obj->owner = m;
    // @src 14017efc0  nodal state owner +0x130 = m; dimension -> 14019a370
    Cls_14017ef20& nod = part_8a0(*obj);
    nod.u130 = obj->owner;
    if (nod.u130 != nullptr) nod.set_u138(dimension_of(obj->owner));
    // @src 140182920  element state owner +0xf0 = m; dimension -> 14019a3f0
    Cls_140182880& elm = part_9e0(*obj);
    elm.u0f0 = reinterpret_cast<LLOBJ_DATA*>(obj->owner);
    if (elm.u0f0 != nullptr) elm.set_u0f8(dimension_of(obj->owner));
    // @src 1401855d0  +0x60 = m; dimension -> 14019a4b0; then 140185630
    Cls_140185570& edg = part_ae8(*obj);
    edg.u060 = obj->owner;
    if (edg.u060 != nullptr) edg.set_u068(dimension_of(obj->owner));
    edg.init();
    // @src 140199a90  custom state-variable set: +8 = m
    // @src 140199ab0  custom state-variable set: +0x10 = the object
    at<DB_STEP_DATA*>(obj->sb58, 8) = obj->owner;
    at<LLOBJ_DATA*>(obj->sb58, 0x10) = obj;
    props_set_owner(obj->s1ea0, obj->owner);
    movement_set_owner(obj->s2160, obj->owner);
    geo23_set_owner(obj->s2be8, obj->owner);
    meshgen_set_owner(obj->s2d10, obj->owner);
    bc_lists_set_owner(obj->s2e68, obj->owner);
    init(obj);
}

// @src 14015f0b0  object initialisation
void init(LLOBJ_DATA* obj)
{
    using namespace llobj::parts;
    std::byte* p = raw(obj);
    obj->next = nullptr;
    at<std::int32_t>(p, 8) = 0;
    at<std::int32_t>(p, 0xc) = 0;
    obj->name[0] = '\0';
    obj->u3990 = 0;
    obj->u87c[0] = 0;
    obj->u87c[1] = 0;
    obj->u87c[2] = 0;
    at<std::int32_t>(p, 0xbc8) = -1;
    at<std::uint64_t>(p, 0xbd0) = 0;
    obj->u87c[3] = 0;  // 0x888
    obj->u87c[4] = 0;  // 0x88c
    at<std::int32_t>(p, 0x3984) = 0;
    at<std::int32_t>(p, 0x3988) = 0;
    at<std::int32_t>(p, 0x3588) = 0;
    at<std::int32_t>(p, 0x358c) = 0;
    at<std::int32_t>(p, 0x398c) = 0;
    set_u39c0(p, dimension_of(obj->owner));  // 140158e90, 140199af0
    if (dimension_of(obj->owner) != 2) {
        set_element_kind(p, 2);  // 140165500
    } else {
        at<std::int32_t>(p, 0x890) = 0;
        at<std::int32_t>(p, 0x894) = 4;
        at<std::int32_t>(p, 0x898) = 0;
        at<std::int32_t>(p, 0x89c) = 0;
    }
    part_8a0(*obj).init();  // 14017f060
    part_9e0(*obj).init();  // 1401829c0
    part_ae8(*obj).init();  // 140185630
    at<std::int32_t>(p, 0x3994) = 0;
    at<std::int32_t>(p, 0xbd8) = 0;
    for (int i = 0; i < 100; ++i) {
        void* g = ::operator new(0x130, std::nothrow);  // 1404797d8
        if (g != nullptr) ext::geo3d_construct(g);
        obj->u3c00[i] = g;
        ext::geo3d_slot2(obj->u3c00[i]);
        part_be0(*obj, i).init();                   // 14017e7b0
        part_be0(*obj, i).set_owner(obj->get_owner());  // 140158f50, 14017e770
    }
    props_defaults(obj->s1ea0);         // 140186200
    movement_defaults(obj->s2160);      // 14018b500
    meshgen_defaults(obj->s2d10, true); // 140189790(p, 1)
    bc_lists_reset(obj->s2e68);         // 140195980
    at<std::int32_t>(p, 0x3590) = 0;
    dynary_init(&obj->u3598, 0x7c);  // 14012d2a0
    at<std::uint8_t>(p, 0x3610) = 0;
    at<std::uint8_t>(p, 0x3611) = 0;
    at<std::uint8_t>(p, 0x3612) = 0;
    at<std::uint8_t>(p, 0x3613) = 0;
    at<std::int32_t>(p, 0x3914) = 0;
    at<std::int32_t>(p, 0x3918) = 0;
    at<std::int32_t>(p, 0x391c) = 0;
    if (obj->u3998 == nullptr) obj->u3998 = new_u3998();
    obj->free_u39c8();                // 140199b10
    object_unit_defaults(p, 0);       // 14015f9a0(obj, 0)
    reset_u39d4(p);                   // 140199a00
    at<std::uint64_t>(p, 0x3a38) = 0;
    for (std::size_t off = 0x39f8; off <= 0x3a30; off += 8) at<std::uint64_t>(p, off) = 0;
    at<std::int32_t>(p, 0x3a40) = 0;
    at<std::uint64_t>(p, 0x3a48) = 0;
    at<std::int32_t>(p, 0x3a50) = 0;
    at<std::uint64_t>(p, 0x3a58) = 0;
    set_owns(&obj->u3a60, 1);  // 140199ad0: the list deletes its items
    at<std::int32_t>(p, 0x3980) = 0;
    obj->u39a0 = 0;
    at<std::uint8_t>(p, 0x3948) = 0;
    at<std::int32_t>(p, 0x3aa0) = -1;
    at<std::int32_t>(p, 0x3aa4) = -1;
    at<std::int32_t>(p, 0x3b50) = -1;
    at<std::uint8_t>(p, 0x3aac) = 1;
    for (int i = 0; i < 0x14; ++i) at<std::uint64_t>(p, 0x3ab0 + 8 * i) = 0;
    for (int i = 0; i < 0x14; ++i) at<std::uint64_t>(p, 0x3b58 + 8 * i) = 0;
    at<double>(p, 0x3ab0) = k5;
    at<double>(p, 0x3ab8) = k8;
    at<double>(p, 0x3ac0) = k7;
    at<double>(p, 0x3b58) = k5;
    at<double>(p, 0x3b60) = k8;
    at<double>(p, 0x3b68) = k7;
    // @src 14019a930 (clear of a COW vector: detach 14019ae90, free the buffer 14019b140)
    // @src 14019a9f0 (the same: 14019aec0, 14019b240)
    // @src 14019aab0 (the same: 14019aef0, 14019b370)
    // @src 14019ae90 (inlined: detach)
    // @src 14019aec0 (inlined: detach)
    // @src 14019aef0 (inlined: detach)
    // @src 14019b140 (inlined: free, null the three pointers)
    // @src 14019b370 (inlined: the same)
    // @src 14019b240 (inlined: vector-delete the elements instead of free; a new object's
    //                 vector is empty, so CowVector::clear is equivalent here)
    reinterpret_cast<CowP*>(&obj->u3960)->clear();
    reinterpret_cast<CowP*>(&obj->u3950)->clear();
    reinterpret_cast<CowP*>(&obj->u3958)->clear();
    reinterpret_cast<CowP*>(&obj->u3960)->clear();
    reinterpret_cast<CowI*>(&obj->u3968)->clear();  // 14019a7d0
    reinterpret_cast<CowP*>(&obj->u3970)->clear();
    reinterpret_cast<CowP*>(&obj->u3978)->clear();
    at<std::int32_t>(p, 0x3f24) = 0;
}

// @src 14017a880  object list add
LLOBJ_DATA* list_add(Cls_14017a6c0* list)
{
    void* mem = ::operator new(0x3f28, std::nothrow);  // 1404797d8
    LLOBJ_DATA* node = mem == nullptr ? nullptr : new (mem) LLOBJ_DATA(list->owner);
    if (list->count == 0) {
        list->tail = node;
        list->head = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
    list->count = list->count + 1;
    return node;
}

// @src 14019a280  object list count
std::int32_t list_count(const Cls_14017a6c0* list) { return list->count; }

// @src 14019ccc0  get-or-create object `no`
LLOBJ_DATA* get_or_create(DB_STEP_DATA* m, std::int32_t no)
{
    // part of @src 14017aaf0 (lookup, inlined; also def::key::g2obj::find_object)
    if (no < 1) return nullptr;
    Cls_14017a6c0* list = &m->objects;
    // 14017aaf0: first node whose number (+8) is `no`
    LLOBJ_DATA* obj = list->head;
    while (obj != nullptr && at<std::int32_t>(obj, 8) != no) obj = obj->next;
    if (obj == nullptr) {
        while (list_count(list) < no - 1) {
            LLOBJ_DATA* p = list_add(list);
            at<std::int32_t>(p, 8) = list_count(list);
            at<std::int32_t>(p, 0xc) = 7;
        }
        obj = list_add(list);
        at<std::int32_t>(obj, 8) = no;
        m->u1378 = list_count(list);
    }
    return obj;
}

// @src 140161260  empty the object
void make_empty(LLOBJ_DATA* obj)
{
    using namespace llobj::parts;
    const char full = 0;
    obj->reset(&full);  // 14015fbf0
    for (int i = 0; i < 100; ++i) {
        if (obj->u3c00[i] != nullptr) ext::geo3d_slot2(obj->u3c00[i]);
        part_be0(*obj, i).init();                       // 14017e7b0
        part_be0(*obj, i).set_owner(obj->get_owner());  // 14017e770
    }
    at<std::int32_t>(obj, 0xc) = 7;
    obj->name[0] = '\0';
    obj->u3990 = 0;
    for (int i = 0; i < 5; ++i) obj->u87c[i] = 0;
}

// @src 140199650  state remap for new sizes
void remap_state(LLOBJ_DATA* obj, const std::int32_t* sizes)
{
    using namespace llobj::parts;
    Cls_14017ef20& nod = part_8a0(*obj);
    nod.alloc(nod.count(), sizes);  // 140182840, 140180150
    Cls_140182880& elm = part_9e0(*obj);
    std::int32_t m = elm.width();   // 140185530
    elm.alloc(elm.count(), sizes, m);  // 1401854f0, 140183770 (the binary also passes -1)
}

// 140301950: the object at +0x3998, kw-g2geo's geo_kw::new_geo_list (a CowList<GEO2D_AND_MESH2D>
// with one default entry)
void* new_u3998() { return geo_kw::new_geo_list(); }

namespace ext {
// 1402f9020: p1-07's OBJ_GEO3D ctor
void geo3d_construct(void* p) { new (p) OBJ_GEO3D(); }
// virtual slot 2 of OBJ_GEO3D: reset (1402f92b0)
void geo3d_slot2(void* p) { static_cast<OBJ_GEO3D*>(p)->reset(); }
// 14025ae30: p1-06's OpStopLinkedList ctor
void opstop_list_construct(void* p) { new (p) OpStopLinkedList(); }
} // namespace ext

} // namespace llobj_ctor

// Entry points other packets declared for 1401613a0 / 140199650 (kw-g4b,
// defmodel/src/material/object_deps.hpp on claude/project-thread-hca1wm).
void llobj_remap_state(LLOBJ_DATA* o, const std::int32_t* sizes, const std::int32_t* /*old_sizes*/)
{
    llobj_ctor::remap_state(o, sizes);
}

} // namespace def::model
