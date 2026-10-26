// _DEF_GEOPRIM object, setup records and arrays (packet p9-02).
// Layout and roles: defmodel/geoprim/def_geoprim.hpp. Decompiles: deform-rea-work/out/p902/.
#include <defmodel/geoprim/def_geoprim.hpp>

#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/llobj/ext.hpp>
#include <defmodel/port.hpp>

#include <cstdio>
#include <cstring>

#if __has_include(<defmodel/obj_mesh2d/mesh_access.hpp>)
#include <defmodel/obj_mesh2d/mesh_access.hpp>
#endif

namespace def::model::geoprim {

namespace {

// The OBJ_MESH2D accessors are ported on the Phase 6-7 gate (defmodel/obj_mesh2d/mesh_access.hpp,
// claude/p67-gate @9694a8e). Its source needs further gate-only code (node_access.hpp and
// node_access.cpp -> statevar_kw), so it is not taken here: the defaults call the gate's
// functions once the gate is merged and stop in DEF_NOT_PORTED until then.
#if __has_include(<defmodel/obj_mesh2d/mesh_access.hpp>)
void stub_alloc_nodes(OBJ_MESH2D* m, const std::int32_t* n) { fun_1403de680(m, n); }
void stub_alloc_elems(OBJ_MESH2D* m, const std::int32_t* ne, const std::int32_t* nper)
{
    fun_1403de6e0(m, ne, nper);
}
double* stub_coords(OBJ_MESH2D* m) { return fun_140220440(m); }
std::int32_t* stub_conn(OBJ_MESH2D* m) { return fun_140220460(m); }
#else
void stub_alloc_nodes(OBJ_MESH2D*, const std::int32_t*) { DEF_NOT_PORTED(0x1403de680); }
void stub_alloc_elems(OBJ_MESH2D*, const std::int32_t*, const std::int32_t*) { DEF_NOT_PORTED(0x1403de6e0); }
double* stub_coords(OBJ_MESH2D*) { DEF_NOT_PORTED(0x140220440); }
std::int32_t* stub_conn(OBJ_MESH2D*) { DEF_NOT_PORTED(0x140220460); }
#endif
void default_obj_delete(LLOBJ_DATA* o) { llobj::ext::llobj_delete(o); }
// 140158fc0: DB_STEP_DATA dtor (1401cb2c0) then free = delete
void default_model_delete(DB_STEP_DATA* m) { delete m; }

Ext g_ext = {stub_alloc_nodes, stub_alloc_elems, stub_coords, stub_conn, default_obj_delete,
             default_model_delete};

// operator new (14047780c thunk of 1404797d8) with the saturating byte count of the callers
// (SUB168(ZEXT816(k) * n) checks): malloc that throws.
template <class T>
T* alloc_n(std::size_t n)
{
    return cow_detail::alloc<T>(n);
}

} // namespace

Ext& ext() { return g_ext; }

// ---- ProfileArray ---------------------------------------------------------------------------

// @src 1402e3f70  init
// @src 1402e3f20  ctor (init)
void ProfileArray::init()
{
    n = 0;
    pts = nullptr;
    u10 = 0;
}

// @src 1402e4030  clear
// @src 1402e3f50  dtor (clear)
void ProfileArray::clear()
{
    n = 0;
    std::free(pts);
    pts = nullptr;
    u10 = 0;
}

// @src 1402e3fa0  resize
void ProfileArray::resize(const std::int32_t* count)
{
    clear();
    n = *count;
    if (0 < n + 1) {
        // 2 * (n + 1) doubles, the byte count saturated twice as in the binary
        pts = alloc_n<double>(static_cast<std::size_t>(static_cast<std::int64_t>(n + 1)) * 2);
    }
}

// @src 1402e4080  assign
void ProfileArray::assign(const std::int32_t* count, const double* xy)
{
    resize(count);
    for (std::int32_t i = 0; i < *count; ++i) {
        std::memcpy(pts + static_cast<std::int64_t>(i + 1) * 2, xy + static_cast<std::int64_t>(i * 2), 0x10);
    }
}

// ---- IntArray / IntArrayList ----------------------------------------------------------------

// @src 1402e9090  init
void IntArray::init()
{
    n = 0;
    data = nullptr;
}

// @src 1402e90c0  clear
void IntArray::clear()
{
    n = 0;
    std::free(data);
    data = nullptr;
}

// @src 1402e9110  resize
void IntArray::resize(std::int32_t count)
{
    clear();
    n = count;
    if (0 < count) data = alloc_n<std::int32_t>(static_cast<std::size_t>(static_cast<std::int64_t>(count)));
}

// @src 1402e9180  assign
void IntArray::assign(const std::int32_t* count, const std::int32_t* src)
{
    resize(*count);
    std::memcpy(data, src, static_cast<std::size_t>(static_cast<std::int64_t>(*count) << 2));
}

// @src 1402e9290  init
// @src 1402e9240  ctor (init)
void IntArrayList::init()
{
    n = 0;
    items = nullptr;
}

// @src 1402e92c0  clear
// @src 1402e9270  dtor (clear)
// @src 1402f0940  vector deleting dtor (flag 3) of the items
void IntArrayList::clear()
{
    for (std::int32_t i = 0; i < n; ++i) items[i].clear();
    if (items != nullptr) delete[] items;
    items = nullptr;
    n = 0;
}

// @src 1402e9370  resize
// @src 1402e94d0  resize (wrapper)
void IntArrayList::resize(const std::int32_t* count)
{
    clear();
    n = *count;
    if (0 < *count) items = new IntArray[static_cast<std::size_t>(*count)];   // eh vector ctor 1402e9040
}

// @src 1402e9500  assign list k
void IntArrayList::assign(const std::int32_t* k, const std::int32_t* count, const std::int32_t* src)
{
    if (0 < *k && *k <= n) items[*k - 1].assign(count, src);
}

// ---- records ----------------------------------------------------------------------------------

// @src 1402e3820  RevolveRec ctor
RevolveRec::RevolveRec()
{
    profile.init();
    init();
}

// @src 1402e3860  RevolveRec dtor
// @src 1402f0760  RevolveRec scalar deleting dtor (dtor, free) = delete
RevolveRec::~RevolveRec() { profile.clear(); }

// @src 1402e3880  RevolveRec init
void RevolveRec::init()
{
    u020[0] = 0;
    u420[0] = 0;
    u820 = 0;
    axis_pt[0] = 0.0;   // 140239700(0, 0, 0)
    axis_pt[1] = 0.0;
    axis_pt[2] = 0.0;
    axis_dir[0] = 0.0;  // 140239700(0, 0, 1.0)
    axis_dir[1] = 0.0;
    axis_dir[2] = 1.0;  // DAT_140509c90
    start = 0.0;
    angle = 0.0;
    sections = 0x24;
    u870 = 0;
    axis_kind = 0;
}

// The section vector of ExtrudeRec is a CowVector<RevolveRec*> (defmodel/sdctl/cow.hpp); its
// instantiation in the binary:
// @src 1402f0ab0  ctor: operator new(0x20), body ctor 1402f1560
// @src 1402f1560  body ctor: refs = 1 (140001000), three null pointers
// @src 1402f0b20  dtor: release 1402f15b0
// @src 1402f15b0  release: refs-- (140001010); at 0 the body scalar deleting dtor 1402f19b0
// @src 1402f19b0  body scalar deleting dtor (body dtor 1402f1b80, free)
// @src 1402f1b80  body dtor: free(first)
// @src 1402f0b40  size (1402f1610 on the body)
// @src 1402f1610  body size ((last - first) >> 3)
// @src 1402f13f0  detach: when refs > 1 (unsigned), 1402f1d80
// @src 1402f1d80  refs-- (140001010, result ignored), body = clone (1402f2490)
// @src 1402f2490  body clone: count = (int)size; count < 1: null pointers, else exactly count
// @src 1402f13c0  detach, return first
// @src 1402f0b60  operator[](i) for writing: detach, &first[i]
// @src 1402f0c40  clear: detach, body clear 1402f1630
// @src 1402f1630  body clear: free(first), three null pointers

// @src 1402e2b10  ExtrudeRec ctor
ExtrudeRec::ExtrudeRec() { init(); }

// @src 1402e2b50  ExtrudeRec dtor
// @src 1402f0720  ExtrudeRec scalar deleting dtor (dtor, free) = delete
ExtrudeRec::~ExtrudeRec()
{
    for (std::int32_t i = 0; i < static_cast<std::int32_t>(sections.size()); ++i) {
        RevolveRec* r = sections.mutable_at(static_cast<std::int64_t>(i));
        if (r != nullptr) delete r;
    }
    // sections dtor (1402f0b20) follows as the member dtor
}

// @src 1402e2c00  ExtrudeRec init
void ExtrudeRec::init()
{
    u000[0] = 0;
    u400[0] = 0;
    u800 = 0;
    dir[0] = 0.0;   // 140239700(0, 0, 1.0)
    dir[1] = 0.0;
    dir[2] = 1.0;
    length = 0.0;
    count = 10;
    u82c = 0;
    u840 = 0;
    u848 = 0;
    for (std::int32_t i = 0; i < static_cast<std::int32_t>(sections.size()); ++i) {
        RevolveRec* r = sections.mutable_at(static_cast<std::int64_t>(i));
        if (r != nullptr) delete r;   // the pointer stays in the vector
    }
    sections.clear();
    RevolveRec* const null_rec = nullptr;
    for (std::int32_t i = 0; i < 100; ++i) sections.push_back(null_rec);
}

// @src 1402f0660  Rec2338 scalar deleting dtor (empty dtor 1402e2840, free) = delete
// @src 1402f06a0  Rec2340 scalar deleting dtor (dtor 1402e2960, free) = delete
// @src 1402f06e0  Rec2348 scalar deleting dtor (dtor 1402e2a50, free) = delete
// (the dtors are inline in the header)

// The three RcArray handles at 0x24e0..0x24f0 (defmodel/obj_mesh2d/rc_array.hpp):
// @src 1402f0c70  0x24e0 ctor: operator new(0x20), block ctor 1402f1680
// @src 1402f1680  block ctor: refs = 1, three null words
// @src 1402f0ce0  0x24e0 dtor: release 1402f16d0
// @src 1402f16d0  release: refs--; at 0 1402f19f0
// @src 1402f19f0  block scalar deleting dtor (Destroy 1402f1bb0, free)
// @src 1402f1bb0  Destroy: when data, vector deleting dtor 1402f1cb0 (flag 3)
// @src 1402f1cb0  vector deleting dtor of 0xa0-byte elements (dtor 1402f0890)
// @src 1402f0e10  0x24e8 ctor (block ctor 1402f1760)
// @src 1402f1760  block ctor
// @src 1402f0e80  0x24e8 dtor: release 1402f17b0
// @src 1402f17b0  release: at 0 1402f1a30
// @src 1402f1a30  block scalar deleting dtor (block dtor 1402f1c00, free)
// @src 1402f1c00  block dtor: free(data)
// @src 1402f0fd0  0x24f0 ctor (block ctor 1402f1860)
// @src 1402f1860  block ctor
// @src 1402f1040  0x24f0 dtor: release 1402f18b0
// @src 1402f18b0  release: at 0 1402f1a70
// @src 1402f1a70  block scalar deleting dtor (Destroy 1402f1c60, free)
// @src 1402f1c60  Destroy: when data, vector deleting dtor 1402f1ab0 (flag 3)
// @src 1402f1ab0  vector deleting dtor of 0x40-byte elements (dtor 140257440)

// ---- DEF_GEOPRIM ------------------------------------------------------------------------------

// @src 1402b7ad0  ctor
DEF_GEOPRIM::DEF_GEOPRIM()
    : OBJ_MESH2D(),
      h24e0(mesh2d::new_rc_array<Elem24e0>()),
      h24e8(mesh2d::new_rc_array<std::byte>()),
      h24f0(mesh2d::new_rc_array<linked::Cls_140257340>())
{
    // 1402b7aee: the profile (a base constructed before OBJ_MESH2D in the binary), then
    // 1402b7b0b: the int-list base; the vectors 0x2390..0x24a0, the handles and the list
    // 0x2518 are constructed above in the binary's order.
    profile.init();
    int_lists.init();
    u24fb = 0;
    std::sprintf(key_name, "DEF_GEOPRIM.KEY");
    u1b30[0] = 0;
    u1f30[0] = 0;
    kind = 0;
    mode_geo = 0;
    mode_mesh = 0;
    u2338 = nullptr;
    u2340 = nullptr;
    u2348 = nullptr;
    extrude = nullptr;
    revolve = nullptr;
    profile.init();
    OBJ_MESH2D::reset();   // 1403de2a0
    int_lists.init();
    u2360 = 0.0;
    u2368 = 0.0;
    u2370 = 0;
    angle = 0.0;
    sections = 0x12;
    owns_obj = 0;
    obj = nullptr;
    owns_model = 0;
    model = nullptr;
    u23c8 = 0;
    u23c9 = 0;
    u23cc = 0;
    u23d0 = 0;
    u2388 = 1.0;   // DAT_140509c90
    u23d1 = 0;
    u24b8 = 0;
    ctx = nullptr;
    u24a8 = nullptr;
    u24b0 = nullptr;
    u23e4 = 0;
    u23e0 = 0;
    u23e8 = 0;
    u23ec = 0;
    u23f0 = 0.0;
    u23f8 = 0.0;
    u2400 = 0.0;
    u2408 = 0.0;
    u2410 = 0.0;
    u2418 = 1.0;   // DAT_140509c90
    u2420 = 0.0;
    u2428 = 0.0;
    u2460 = 0;
    u2464 = 0;
    u2468 = 0;
    u246c = 0;
    u2470 = 0;
    u2458 = 0;
    u2448 = -1.0;  // DAT_140522fc8
    u2450 = -1.0;
    u2478 = 0;
    u2480 = 0;
    u2488 = 0;
    u24b9 = 0;
    u24ba = 0;
    u24bc = 3;
    u24c0 = 120.0; // DAT_140541428
    u24c8 = 120.0;
    u24d0 = 120.0;
    u24d8 = 0;
    u24d9 = 0;
    u24da = 0;
    u24db = 0;
    u24f8 = 0;
    u24f9 = 0;
    u24fa = 0;
    u24fc = 0;
    u24fd = 0;
    u24fe = 0;
    u1718 = 0.0004; // DAT_140541420
    u2500 = 0.001;  // DAT_14051f1e8
    u2508 = 0.3;    // DAT_140541418
    u2510 = 5.233;  // DAT_140541410
}

// @src 1402b8090  dtor
// @src 1402f0620  scalar deleting dtor (vtable slot 0) = delete
DEF_GEOPRIM::~DEF_GEOPRIM()
{
    reset();
    // members in reverse: the list 0x2518 and the vectors are released as member dtors after
    // this body (140159930, 140159b40, 14019a720); the handles here (1402f1040, 1402f0e80,
    // 1402f0ce0), then the int-list base (1402e9270) and, after OBJ_MESH2D (1403de190), the
    // profile base (1402e3f50).
    mesh2d::release_rc_array(h24f0);
    mesh2d::release_rc_array(h24e8);
    mesh2d::release_rc_array(h24e0);
    int_lists.clear();
    profile.clear();
}

// @src 1402b81a0  reset
void DEF_GEOPRIM::reset()
{
    kind = 0;
    mode_geo = 0;
    mode_mesh = 0;
    if (u2338 != nullptr) delete u2338;     // 1402f0660
    if (u2340 != nullptr) delete u2340;     // 1402f06a0
    if (u2348 != nullptr) delete u2348;     // 1402f06e0
    if (extrude != nullptr) delete extrude; // 1402f0720
    // 1402b8347: revolve is deleted unless it is the record at 0x2340 (compared as pointers)
    if (static_cast<void*>(revolve) != static_cast<void*>(u2340) && revolve != nullptr) {
        delete revolve;                       // 1402f0760
    }
    // the pointers are not cleared
    clear_arrays();
    if (owns_obj != 0 && obj != nullptr) ext().obj_delete(obj);
    if (owns_model != 0 && model != nullptr) ext().model_delete(model);
    std::free(u24a8);
    std::free(u24b0);
    if (!l2518.empty()) l2518.clear();      // 1401599b0 (size == 0), 14015a620
}

// @src 1402b84e0  clear the arrays
void DEF_GEOPRIM::clear_arrays()
{
    profile.clear();
    OBJ_MESH2D::clear();   // 1403de3d0
    int_lists.clear();
}

// @src 1402e9aa0  2D mesh
void DEF_GEOPRIM::set_mesh2d(const std::int32_t* nn, const double* xy, const std::int32_t* ne,
                             const std::int32_t* conn)
{
    ext().mesh_alloc_nodes(this, nn);
    const std::int32_t nper = 4;
    ext().mesh_alloc_elems(this, ne, &nper);
    double* dst = ext().mesh_coords(this);
    std::int32_t* dst_conn = ext().mesh_conn(this);
    std::memcpy(dst, xy, static_cast<std::size_t>(static_cast<std::int64_t>(*nn << 1) << 3));
    std::memcpy(dst_conn, conn, static_cast<std::size_t>(static_cast<std::int64_t>(*ne << 2) << 2));
}

// @src 1402dd0d0  angle and sections from the revolve record
void DEF_GEOPRIM::take_revolve_params(const RevolveRec* rec)
{
    if (rec != nullptr) {
        angle = rec->angle;
        sections = rec->sections;
    }
}

// The two folded wrappers 1402b8520 calls on v2490 / v2498 (CowVector<double>):
// @src 1402f0a40  size (140003090) = CowVector::size
// @src 1402f0a80  push_back (140256800, p8-09; same code as 1402f0ba0) = CowVector::push_back

// @src 1402b8520  extrude setup
void DEF_GEOPRIM::setup_extrude(std::int32_t mode_geo_, std::int32_t mode_mesh_, const double* length,
                                const std::int32_t* count_, const std::int32_t* n_prof, const double* xy_prof,
                                const std::int32_t* nn, const double* xy, const std::int32_t* ne,
                                const std::int32_t* conn, const std::int32_t* n_list, const std::int32_t* list,
                                const char* b23c8, const char* b23c9, const std::int32_t* i23cc,
                                const double* d2388, const double* p2490, const double* p2498,
                                const double* d2448, const double* d2450, const double* dir)
{
    kind = 1;
    mode_geo = mode_geo_;
    mode_mesh = mode_mesh_;
    if (extrude == nullptr) {
        extrude = new ExtrudeRec();   // operator new(0x850), 1402e2b10
    } else {
        extrude->init();              // 1402e2c00
    }
    extrude->length = *length;
    extrude->count = *count_;
    if (dir != nullptr) {
        extrude->dir[0] = dir[0];
        extrude->dir[1] = dir[1];
        extrude->dir[2] = dir[2];
    }
    u2360 = extrude->length;
    u2368 = 0.0;
    sections = extrude->count;
    profile.assign(n_prof, xy_prof);          // 1402e4080
    set_mesh2d(nn, xy, ne, conn);             // 1402e9aa0
    const std::int32_t one = 1;
    int_lists.resize(&one);                   // 1402e94d0
    const std::int32_t k = 1;
    int_lists.assign(&k, n_list, list);       // 1402e9500
    u23c8 = *b23c8;
    u23c9 = *b23c9;
    u23cc = *i23cc;
    extrude->u830 = u23cc;
    u2388 = *d2388;
    if (v2490.size() == 0) {                  // 1402f0a40 (140003090)
        v2490.push_back(*p2490);              // 1402f0a80 -> 140256800
        v2498.push_back(*p2498);
    } else {
        v2490.mutable_at(0) = *p2490;         // 140005e70
        v2498.mutable_at(0) = *p2498;
    }
    u2448 = *d2448;
    u2450 = *d2450;
}

// @src 1402b8980  revolve setup
void DEF_GEOPRIM::setup_revolve(std::int32_t mode_geo_, std::int32_t mode_mesh_, const double* start,
                                const double* angle_, const double* axis_dir, const double* axis_pt,
                                const std::int32_t* sections_, const std::int32_t* n_prof, const double* xy_prof,
                                const std::int32_t* nn, const double* xy, const std::int32_t* ne,
                                const std::int32_t* conn, const std::int32_t* n_list, const std::int32_t* list)
{
    kind = 2;
    mode_geo = mode_geo_;
    mode_mesh = mode_mesh_;
    if (revolve == nullptr) {
        revolve = new RevolveRec();   // operator new(0x888), 1402e3820
    } else {
        revolve->init();              // 1402e3880
    }
    OBJ_MESH2D::clear();              // 1403de3d0
    revolve->start = *start;
    revolve->angle = *angle_;
    double tmp[3];
    std::memcpy(tmp, axis_dir, 0x18);  // memcpy, then the 140239800 copy
    std::memcpy(revolve->axis_dir, tmp, 0x18);
    std::memcpy(tmp, axis_pt, 0x18);
    std::memcpy(revolve->axis_pt, tmp, 0x18);
    revolve->sections = *sections_;
    take_revolve_params(revolve);      // 1402dd0d0
    profile.assign(n_prof, xy_prof);   // 1402e4080
    set_mesh2d(nn, xy, ne, conn);      // 1402e9aa0
    const std::int32_t one = 1;
    int_lists.resize(&one);            // 1402e94d0
    const std::int32_t k = 1;
    int_lists.assign(&k, n_list, list); // 1402e9500
}

} // namespace def::model::geoprim
