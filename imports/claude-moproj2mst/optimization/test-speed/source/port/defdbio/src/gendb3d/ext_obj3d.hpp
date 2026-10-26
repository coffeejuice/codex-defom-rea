// gendb3d/ext_obj3d.hpp: external calls of the p4-05 driver and the node/element/face/LLBND/fold
// writers (see ext.hpp for the rules). Each wrapper offers the call to the test hook first and
// otherwise calls the owner's port (named next to it); the Phase 4 gate ports of functions no
// packet owned are in defdbio/gendb/gate_ports.hpp.
#pragma once

#include <cstdint>
#include <cstring>

#include "gendb3d/access.hpp"
#include "gendb3d/ext.hpp"

#include <defdbio/gendb/counts_frame.hpp>
#include <defdbio/gendb/gate_ports.hpp>
#include <defdbio/gendb/obj_counts.hpp>
#include <defdbio/objcheck/objcheck.hpp>
#include <defkey/gendb_deps.hpp>
#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/bc/local_bc.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/misc_model.hpp>
#include <defmodel/geo_kw/surface_model.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defmodel/material/material_ctor.hpp>
#include <defmodel/material/state_remap.hpp>
#include <defmodel/phase/phase_ops.hpp>
#include <defmodel/statevar_kw/statevar_kw.hpp>

namespace def::dbio::g3d::ext {

// ---- p4-02a / p4-02b (counts) ------------------------------------------------------------
// 140199c10 (p4-02a): return *(int *)(m + 0x78)  (GENDB material count) -> gendb::step_u078
inline std::int32_t f140199c10(model::DB_STEP_DATA* m)
{
    std::uint64_t r;
    if (hooked(0x140199c10, {u(m)}, &r)) return static_cast<std::int32_t>(r);
    return gendb::step_u078(m);
}
// 14029c410 (p4-02b): largest material number of the list (m + 0x1328)
// -> gendb::material_list_max_number
inline std::int32_t f14029c410(void* materials)
{
    std::uint64_t r;
    if (hooked(0x14029c410, {u(materials)}, &r)) return static_cast<std::int32_t>(r);
    return gendb::material_list_max_number(static_cast<model::MaterialList*>(materials));
}
// 1401f3cb0 (p4-02a): return *(int *)(sim + 0x118) -> gendb::simctrl_u118
inline std::int32_t f1401f3cb0(void* sim)
{
    std::uint64_t r;
    if (hooked(0x1401f3cb0, {u(sim)}, &r)) return static_cast<std::int32_t>(r);
    return gendb::simctrl_u118(sim);
}
// 1401f3c30 (P2): return *(int *)(sim + 0x108) -> key::g1a::ops::trans1 (TRANS[1])
inline std::int32_t f1401f3c30(void* sim)
{
    std::uint64_t r;
    if (hooked(0x1401f3c30, {u(sim)}, &r)) return static_cast<std::int32_t>(r);
    return key::g1a::ops::trans1(*static_cast<const model::Cls_1401e7b10*>(sim));
}
// 1401f3d90 (p4-02a): sim-control predicate (1401f3c10 == 0, 1401f3c30 == 1, 1401f3c50 == 0, ...)
// -> gendb::simctrl_only_deformation
inline bool f1401f3d90(void* sim)
{
    std::uint64_t r;
    if (hooked(0x1401f3d90, {u(sim)}, &r)) return (r & 0xff) != 0;
    return gendb::simctrl_only_deformation(static_cast<model::Cls_1401e7b10*>(sim));
}
// 14001cd90 (p4-02b): error report; empty body in this build (ret) -> gendb::diag_14001cd90
inline void f14001cd90(const std::int32_t* a, const void* b, const std::uint32_t* c, const std::int32_t* d)
{
    std::uint64_t r;
    if (hooked(0x14001cd90, {u(a), u(b), u(c), u(d)}, &r)) return;
    gendb::diag_14001cd90(a, static_cast<const char*>(b), c, d);
}
// 140136a10 (p4-02a): largest function id (+0x54) of the BC function list -> gendb::dynary_list_max_id
inline std::int32_t f140136a10(void* list)
{
    std::uint64_t r;
    if (hooked(0x140136a10, {u(list)}, &r)) return static_cast<std::int32_t>(r);
    return gendb::dynary_list_max_id(list);
}
// 1402039d0 (p4-02b): 2D object geometry check -> gendb::object_geo2d_usable
inline bool f1402039d0(void* obj)
{
    std::uint64_t r;
    if (hooked(0x1402039d0, {u(obj)}, &r)) return (r & 0xff) != 0;
    return gendb::object_geo2d_usable(static_cast<model::LLOBJ_DATA*>(obj));
}

// ---- p4-03 -------------------------------------------------------------------------------
// 1402582b0 (p4-03): largest id of the symmetry-plane list (obj + 0x2e80) -> objcheck::plane_list_max_id
inline std::int32_t f1402582b0(void* list)
{
    std::uint64_t r;
    if (hooked(0x1402582b0, {u(list)}, &r)) return static_cast<std::int32_t>(r);
    return objcheck::plane_list_max_id(list);
}
// 1402575d0 (p4-03): if (dst) memcpy(dst, p + 0x20, 0x18) -> objcheck::plane_direction
inline void f1402575d0(void* p, void* dst)
{
    std::uint64_t r;
    if (hooked(0x1402575d0, {u(p), u(dst)}, &r)) return;
    objcheck::plane_direction(p, static_cast<double*>(dst));
}

// ---- P2 / P3 model code ------------------------------------------------------------------
// 14017aaf0 (P2): object whose number (+8) is n: walk head .. +0x3940 -> model::object_list_find
inline model::LLOBJ_DATA* f14017aaf0(model::Cls_14017a6c0* l, std::int32_t n)
{
    std::uint64_t r;
    if (hooked(0x14017aaf0, {u(l), i(n)}, &r)) return as_p<model::LLOBJ_DATA>(r);
    return model::object_list_find(l, n);
}
// 14019a280 (P3): return *(int *)(list + 0x10) -> model::llobj_ctor::list_count
inline std::int32_t f14019a280(model::Cls_14017a6c0* l)
{
    std::uint64_t r;
    if (hooked(0x14019a280, {u(l)}, &r)) return static_cast<std::int32_t>(r);
    return model::llobj_ctor::list_count(l);
}
// 140130290 (P2): LLFNC length of a DYNARY -> gendb::dynary_words (gate port)
inline std::int32_t f140130290(void* f)
{
    std::uint64_t r;
    if (hooked(0x140130290, {u(f)}, &r)) return static_cast<std::int32_t>(r);
    return gendb::dynary_words(static_cast<const model::DYNARY*>(f));
}
// 140165170 (P2): boundary faces of the object (3D +0x88c after 140165d30) -> bc::edge_face_slot_count
inline std::int32_t f140165170(model::LLOBJ_DATA* obj)
{
    std::uint64_t r;
    if (hooked(0x140165170, {u(obj)}, &r)) return static_cast<std::int32_t>(r);
    return model::bc::edge_face_slot_count(*obj);
}
// 140184a30 (P2): USRELM buffer of the element set (obj + 0x9e0), rebuilt from the state variable
// -> model::svkw::usrelm_values
inline double* f140184a30(void* elems)
{
    std::uint64_t r;
    if (hooked(0x140184a30, {u(elems)}, &r)) return as_p<double>(r);
    return model::svkw::usrelm_values(elems);
}
// 1401d5820 (P2): DB_STEP_DATA region sync (called by 140183cb0 / 140183d00 with 1 / 2)
// -> gendb::step_read_mar_on_demand (gate port; a no-op in this build)
inline void f1401d5820(void* owner, const std::int32_t* what)
{
    std::uint64_t r;
    if (hooked(0x1401d5820, {u(owner), u(what)}, &r)) return;
    gendb::step_read_mar_on_demand(static_cast<model::DB_STEP_DATA*>(owner), what);
}
// 1401d6450 (P3): count the active phase relations (m + 0x13f4) -> model::count_active_phase_pairs
inline void f1401d6450(model::DB_STEP_DATA* m)
{
    std::uint64_t r;
    if (hooked(0x1401d6450, {u(m)}, &r)) return;
    model::count_active_phase_pairs(m);
}
// 1401d0fc0 (P3): rebuild the state-variable width table W[] -> model::step_remap_material_state
// (the binary's flag is a char: only the low byte counts)
inline void f1401d0fc0(model::DB_STEP_DATA* m, std::int32_t flag)
{
    std::uint64_t r;
    if (hooked(0x1401d0fc0, {u(m), i(flag)}, &r)) return;
    model::step_remap_material_state(*m, static_cast<std::uint8_t>(flag) != 0);
}
// 14029c870 (P2): column of material n in the phase table -> model::material_list_index_of
inline std::int32_t f14029c870(void* materials, std::int32_t n)
{
    std::uint64_t r;
    if (hooked(0x14029c870, {u(materials), i(n)}, &r)) return static_cast<std::int32_t>(r);
    return model::material_list_index_of(static_cast<const model::MaterialList*>(materials), n);
}
// 1401368c0 (P3): BC function with id n (+0x54) of the list, walk +0x60 -> model::dynary_list_find
inline void* f1401368c0(void* list, std::int32_t n)
{
    std::uint64_t r;
    if (hooked(0x1401368c0, {u(list), i(n)}, &r)) return as_p<void>(r);
    return model::dynary_list_find(static_cast<const model::Cls_1401364a0*>(list), n);
}
// 1402580f0 (P2): symmetry plane with id n (+0) of the list, walk +0x38 -> model::geo_kw::symsuf_find
inline void* f1402580f0(void* list, std::int32_t n)
{
    std::uint64_t r;
    if (hooked(0x1402580f0, {u(list), i(n)}, &r)) return as_p<void>(r);
    return model::geo_kw::symsuf_find(*static_cast<model::linked::Cls_140257340List*>(list), n);
}
// 140203a20 (P2): 3D object geometry check -> model::geo_kw::object_has_surface_mesh
inline bool f140203a20(void* obj)
{
    std::uint64_t r;
    if (hooked(0x140203a20, {u(obj)}, &r)) return (r & 0xff) != 0;
    return model::geo_kw::object_has_surface_mesh(*static_cast<model::LLOBJ_DATA*>(obj));
}
// 140160e50 / 140160eb0 (P2): FLDGEO (+0x3328) / FLWNET (+0x3458) has nodes and elements (1402f9360)
// -> model::geo_kw::has_fldgeo / has_flwnet
inline bool f140160e50(model::LLOBJ_DATA* obj)
{
    std::uint64_t r;
    if (hooked(0x140160e50, {u(obj)}, &r)) return (r & 0xff) != 0;
    return model::geo_kw::has_fldgeo(*obj);
}
inline bool f140160eb0(model::LLOBJ_DATA* obj)
{
    std::uint64_t r;
    if (hooked(0x140160eb0, {u(obj)}, &r)) return (r & 0xff) != 0;
    return model::geo_kw::has_flwnet(*obj);
}
// 140167d70 / 140167e00 (p4-02b): fold node / face counts (FLDGEO + FLWNET)
// -> gendb::object_geo3d_nodes / object_geo3d_elems
inline std::int32_t f140167d70(model::LLOBJ_DATA* obj)
{
    std::uint64_t r;
    if (hooked(0x140167d70, {u(obj)}, &r)) return static_cast<std::int32_t>(r);
    return gendb::object_geo3d_nodes(obj);
}
inline std::int32_t f140167e00(model::LLOBJ_DATA* obj)
{
    std::uint64_t r;
    if (hooked(0x140167e00, {u(obj)}, &r)) return static_cast<std::int32_t>(r);
    return gendb::object_geo3d_elems(obj);
}
// 140006410 (P2): &coords[0] for writing (140005e70(p + 0x10, 0)) -> model::geo_kw::geo3d_coords
inline double* f140006410(model::OBJ_GEO3D* g)
{
    std::uint64_t r;
    if (hooked(0x140006410, {u(g)}, &r)) return as_p<double>(r);
    return model::geo_kw::geo3d_coords(g);
}
// 140005510 (P2): &elems[0] for writing (140005100(p + 0x18, 0)) -> model::geo_kw::geo3d_elems
inline std::int32_t* f140005510(model::OBJ_GEO3D* g)
{
    std::uint64_t r;
    if (hooked(0x140005510, {u(g)}, &r)) return as_p<std::int32_t>(r);
    return model::geo_kw::geo3d_elems(g);
}
// 140199fa0 (P2): &elem_ids[0] for writing (140005100(p + 0x68, 0)) -> model::geo_kw::geo3d_elem_ids
inline std::int32_t* f140199fa0(model::OBJ_GEO3D* g)
{
    std::uint64_t r;
    if (hooked(0x140199fa0, {u(g)}, &r)) return as_p<std::int32_t>(r);
    return model::geo_kw::geo3d_elem_ids(g);
}
// 140198620 (P2): return *(obj + 0x3c00 + *k * 8) -> model::geo_kw::surface
inline model::OBJ_GEO3D* f140198620(model::LLOBJ_DATA* obj, const std::int32_t* k)
{
    std::uint64_t r;
    if (hooked(0x140198620, {u(obj), u(k)}, &r)) return as_p<model::OBJ_GEO3D>(r);
    return model::geo_kw::surface(*obj, *k);
}
// 1402fcfe0 (P2): geometry surface count (with an optional rebuild 1402fced0)
// -> gendb::geo3d_surface_patch_count (gate port)
inline std::int32_t f1402fcfe0(model::OBJ_GEO3D* g, const char* rebuild)
{
    std::uint64_t r;
    if (hooked(0x1402fcfe0, {u(g), u(rebuild)}, &r)) return static_cast<std::int32_t>(r);
    return gendb::geo3d_surface_patch_count(g, rebuild);
}
// 1402fd3e0 / 1402fd740 (P2): item n (1-based) of the lists at +0x50 / +0x58 (default item when out
// of range) -> model::geo_kw::geo3d_point_set / geo3d_edge_set
inline void* f1402fd3e0(model::OBJ_GEO3D* g, const std::int32_t* n)
{
    std::uint64_t r;
    if (hooked(0x1402fd3e0, {u(g), u(n)}, &r)) return as_p<void>(r);
    return model::geo_kw::geo3d_point_set(g, n);
}
inline void* f1402fd740(model::OBJ_GEO3D* g, const std::int32_t* n)
{
    std::uint64_t r;
    if (hooked(0x1402fd740, {u(g), u(n)}, &r)) return as_p<void>(r);
    return model::geo_kw::geo3d_edge_set(g, n);
}
// 14019a080 / 14019a0a0 (p4-02a): item counts of the lists at +0x50 / +0x58 (*(*(p + off)) + 0x10)
// -> gendb::surface_list_a_size / surface_list_b_size
inline std::int32_t f14019a080(model::OBJ_GEO3D* g)
{
    std::uint64_t r;
    if (hooked(0x14019a080, {u(g)}, &r)) return static_cast<std::int32_t>(r);
    return gendb::surface_list_a_size(g);
}
inline std::int32_t f14019a0a0(model::OBJ_GEO3D* g)
{
    std::uint64_t r;
    if (hooked(0x14019a0a0, {u(g)}, &r)) return static_cast<std::int32_t>(r);
    return gendb::surface_list_b_size(g);
}
// 1402f87a0 / 1402f8d30 (p4-02a): LLFNC length of a +0x50 / +0x58 item (count * per + 10, or 0)
// -> gendb::surface_a_words / surface_b_words
inline std::int32_t f1402f87a0(void* item)
{
    std::uint64_t r;
    if (hooked(0x1402f87a0, {u(item)}, &r)) return static_cast<std::int32_t>(r);
    return gendb::surface_a_words(item);
}
inline std::int32_t f1402f8d30(void* item)
{
    std::uint64_t r;
    if (hooked(0x1402f8d30, {u(item)}, &r)) return static_cast<std::int32_t>(r);
    return gendb::surface_b_words(item);
}
// 1402f8760 / 1402f8ce0 (p4-02a): values per entry of a +0x50 item (5 when type 1) / +0x58 item
// (1 when type 1..2) -> gendb::surface_a_ncomp / surface_b_ncomp
inline std::int32_t f1402f8760(void* item)
{
    std::uint64_t r;
    if (hooked(0x1402f8760, {u(item)}, &r)) return static_cast<std::int32_t>(r);
    return gendb::surface_a_ncomp(item);
}
inline std::int32_t f1402f8ce0(void* item)
{
    std::uint64_t r;
    if (hooked(0x1402f8ce0, {u(item)}, &r)) return static_cast<std::int32_t>(r);
    return gendb::surface_b_ncomp(item);
}
// 14019a000 / 14019a060 (P2): entry count of an item's list at +0x10 (*(*(p + 0x10)) + 0x10)
// -> DiePointSet::point_count / DieEdgeSet::node_count
inline std::int32_t f14019a000(void* item)
{
    std::uint64_t r;
    if (hooked(0x14019a000, {u(item)}, &r)) return static_cast<std::int32_t>(r);
    return static_cast<const model::geo_kw::DiePointSet*>(item)->point_count();
}
inline std::int32_t f14019a060(void* item)
{
    std::uint64_t r;
    if (hooked(0x14019a060, {u(item)}, &r)) return static_cast<std::int32_t>(r);
    return static_cast<const model::geo_kw::DieEdgeSet*>(item)->node_count();
}
// 1402b7470 / 1402b74d0 (P2): return *(int *)(item + 4) -> DiePointSet / DieEdgeSet::get_type
inline std::int32_t f1402b7470(void* item)
{
    std::uint64_t r;
    if (hooked(0x1402b7470, {u(item)}, &r)) return static_cast<std::int32_t>(r);
    return static_cast<const model::geo_kw::DiePointSet*>(item)->get_type();
}
inline std::int32_t f1402b74d0(void* item)
{
    std::uint64_t r;
    if (hooked(0x1402b74d0, {u(item)}, &r)) return static_cast<std::int32_t>(r);
    return static_cast<const model::geo_kw::DieEdgeSet*>(item)->get_type();
}
// 1402b7480 / 1402b74e0 (P2): *(int *)(item + 8) when *(int *)(item + 4) == 1, else 0
// -> DiePointSet / DieEdgeSet::extra_if_type1
inline std::int32_t f1402b7480(void* item)
{
    std::uint64_t r;
    if (hooked(0x1402b7480, {u(item)}, &r)) return static_cast<std::int32_t>(r);
    return static_cast<const model::geo_kw::DiePointSet*>(item)->extra_if_type1();
}
inline std::int32_t f1402b74e0(void* item)
{
    std::uint64_t r;
    if (hooked(0x1402b74e0, {u(item)}, &r)) return static_cast<std::int32_t>(r);
    return static_cast<const model::geo_kw::DieEdgeSet*>(item)->extra_if_type1();
}
// 1402b74b0 / 1402b7510 (P2): return item + 0x10 (the entry list). The owner (kw-g2geo) has no
// separate function for these (it uses the member); the default is the binary's body.
inline void* f1402b74b0(void* item)
{
    std::uint64_t r;
    if (hooked(0x1402b74b0, {u(item)}, &r)) return as_p<void>(r);
    return at(item, 0x10);
}
inline void* f1402b7510(void* item)
{
    std::uint64_t r;
    if (hooked(0x1402b7510, {u(item)}, &r)) return as_p<void>(r);
    return at(item, 0x10);
}
// 1402b7560 (P2): entry n of a list of 5-double entries (detach, walk, + 0x10) -> model::geo_kw::points_at
inline double* f1402b7560(void* list, std::int64_t n)
{
    std::uint64_t r;
    if (hooked(0x1402b7560, {u(list), i(n)}, &r)) return as_p<double>(r);
    auto& l = *static_cast<model::CowList<model::geo_kw::DiePoint>*>(list);
    return model::geo_kw::points_at(l, static_cast<std::int32_t>(n)).v;
}
// 140159a70 (P2): entry n of a list of ints (detach, walk, + 0x10) -> model::geo_kw::ints_at
inline std::int32_t* f140159a70(void* list, std::int64_t n)
{
    std::uint64_t r;
    if (hooked(0x140159a70, {u(list), i(n)}, &r)) return as_p<std::int32_t>(r);
    return &model::geo_kw::ints_at(*static_cast<model::CowList<std::int32_t>*>(list), static_cast<std::int32_t>(n));
}
// 1402ffcd0 / 1402fffa0 (P2): node n of the list body (walk n links from head->next, no detach).
// The owner (kw-g2geo point_sets_at / edge_sets_at) absorbs these walks into the detaching
// accessors 1402feeb0 / 1402ff000, so the default here is the binary's walk over the body:
//   local_18 = **(body + 8); for (k = 0; k < n; k++) local_18 = *local_18; return local_18;
template <class Rec>
inline void* walk_body_nodes(void* body, std::uint64_t n)
{
    auto& l = static_cast<model::CowListBody<Rec>*>(body)->l;
    auto it = l.begin();
    for (std::uint64_t k = 0; k < n; k = k + 1) ++it;
    // the node: next, prev, then the record at +0x10
    return reinterpret_cast<char*>(&*it) - 0x10;
}
inline void* f1402ffcd0(void* list, std::uint64_t n)
{
    std::uint64_t r;
    if (hooked(0x1402ffcd0, {u(list), n}, &r)) return as_p<void>(r);
    return walk_body_nodes<model::geo_kw::DiePointSet>(list, n);
}
inline void* f1402fffa0(void* list, std::uint64_t n)
{
    std::uint64_t r;
    if (hooked(0x1402fffa0, {u(list), n}, &r)) return as_p<void>(r);
    return walk_body_nodes<model::geo_kw::DieEdgeSet>(list, n);
}
// 1402f9780 (P2): resize nodes (1402f97c0) and elements (1402f9810) -> gendb::geo3d_resize (gate port)
inline void f1402f9780(model::OBJ_GEO3D* g, const std::int32_t* nodes, const std::int32_t* elems)
{
    std::uint64_t r;
    if (hooked(0x1402f9780, {u(g), u(nodes), u(elems)}, &r)) return;
    gendb::geo3d_resize(g, nodes, elems);
}
// 1402fe9d0 (P1): &elems[0] read-only (1402feb90(p + 0x18, 0): 1402ff1a0 body first, no detach)
// -> CowVector::begin (p1-09 classes 1402fe9d0 / 1402feb90 / 1402ff1a0 as the cow.hpp template)
inline const std::int32_t* f1402fe9d0(model::OBJ_GEO3D* g)
{
    std::uint64_t r;
    if (hooked(0x1402fe9d0, {u(g)}, &r)) return as_p<const std::int32_t>(r);
    return g->elems.begin();
}

} // namespace def::dbio::g3d::ext
