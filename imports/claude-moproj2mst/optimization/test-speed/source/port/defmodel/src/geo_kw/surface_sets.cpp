// kw-g2geo surface family, model side (defmodel/geo_kw/surface_model.hpp): die point / die edge
// set records and lists, OBJ_GEO3D sizing, element ids and the patch list.
// Decompiles: deform-rea-work/out/g2geo/fam_surface.c.
#include <defmodel/geo_kw/surface_model.hpp>

#include <cstdint>

#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>

namespace def::model::geo_kw {

namespace {

// OBJ_GEO3D +0x40 holds 8-byte list values (ctor 140159860); 1402fced0 stores an int there and
// 1402ff8a0 compares only the low 4 bytes, so the port keeps the int in the low 32 bits.
// (The original copies 8 bytes from a stack int[2] whose upper half is never set.)
std::int32_t patch_value(std::int32_t v) { return v; }
std::int32_t patch_int(std::int32_t p) { return p; }

}  // namespace

// ---- DiePoint ---------------------------------------------------------------------------------

// @src 1402fe850
void DiePoint::set_xyz(double x, double y, double z)
{
    v[0] = x;
    v[1] = y;
    v[2] = z;
}
// @src 1402fe8a0
void DiePoint::set_p4(double p) { v[3] = p; }
// @src 1402fe8c0
void DiePoint::set_p5(double p) { v[4] = p; }
// @src 1402b7420
double DiePoint::x() const { return v[0]; }
// @src 1402b7430
double DiePoint::y() const { return v[1]; }
// @src 1402b7440
double DiePoint::z() const { return v[2]; }
// @src 1402b7460
double DiePoint::p4() const { return v[3]; }
// @src 1402b7450
double DiePoint::p5() const { return v[4]; }

// ---- DiePointSet --------------------------------------------------------------------------------
// @src 1402fea10  ctor (default member initialisers; list handle 1402fecf0, body 1402ff930)
// @src 1402fecf0  list handle ctor: new 0x18, body 1402ff930
// @src 1402ff930  list body ctor: refs = 1, sentinel node (0x38, 1402b7830), size 0
// @src 1402fea60  dtor: clear the list (1402ff5f0), release it (1402fed60)
// @src 1402fed60  list handle dtor -> release 1402ff350
// @src 1402ff350  release: refs-- (140001010); at 0 the body's deleting dtor 1402ff470
// @src 1402ff470  body scalar deleting dtor -> 1402ff9e0, free
// @src 1402ff9e0  body dtor: free every node, then the sentinel
// (all of these are CowList<DiePoint>'s ctor / dtor)

// @src 1402feaa0
void DiePointSet::set_id(const std::int32_t* v) { id = *v; }
// @src 1402feac0
std::int32_t DiePointSet::get_id() const { return id; }
// @src 1402fe8e0
void DiePointSet::set_type(const std::int32_t* v) { type = *v; }
// @src 1402b7470
std::int32_t DiePointSet::get_type() const { return type; }
// @src 1402fe900
void DiePointSet::set_extra(const std::int32_t* v) { extra = *v; }
// @src 1402b7480
std::int32_t DiePointSet::extra_if_type1() const
{
    if (get_type() == 1) return extra;
    return 0;
}
// @src 1402b74b0  this + 0x10 (the points member)
// @src 14019a000  -> 14019a800 (body + 0x10) on +0x10
// @src 14019a800  list size
std::int32_t DiePointSet::point_count() const { return static_cast<std::int32_t>(points.size()); }

// ---- DieEdgeSet ---------------------------------------------------------------------------------
// @src 1402fead0  ctor (default member initialisers; list 140159860)
// @src 1402feb20  dtor: clear the list (14015a620), release it (140159930)

// @src 1402feb60
void DieEdgeSet::set_id(const std::int32_t* v) { id = *v; }
// @src 1402feb80
std::int32_t DieEdgeSet::get_id() const { return id; }
// @src 1402fe930
void DieEdgeSet::set_type(const std::int32_t* v) { type = *v; }
// @src 1402b74d0
std::int32_t DieEdgeSet::get_type() const { return type; }
// @src 1402fe950
void DieEdgeSet::set_extra(const std::int32_t* v) { extra = *v; }
// @src 1402b74e0
std::int32_t DieEdgeSet::extra_if_type1() const
{
    if (get_type() == 1) return extra;
    return 0;
}
// @src 1402b7510  this + 0x10 (the nodes member)
// @src 14019a060  -> 140159950 (body + 0x10) on +0x10
// @src 140159950  list size
std::int32_t DieEdgeSet::node_count() const { return static_cast<std::int32_t>(nodes.size()); }

// ---- list operations ------------------------------------------------------------------------------

// @src 1402fee60  push_back: detach (1402ff270), end() (1402ff230, iterator 1402ff530), insert
//                 before the sentinel (1402ffbe0: node 0x28, 1403000c0 -> record copy 140300220)
// @src 1402ff270  detach when refs > 1 (1402ff710)
// @src 1402ff710  refs--, new body cloned from the old one (140300540)
// @src 140300540  body clone: sentinel (1403000f0), each record copied in order (140300a50,
//                 140300a30, 1403009d0, 1402ffbe0)
// @src 1402ff230  end(), detaching
// @src 1402ff530  iterator ctor
// @src 1402ffbe0  insert a copy of the record before the iterator, size++
// @src 1403000c0  node ctor: copy the record at +0x10 (140300220)
// @src 1403000f0  sentinel node ctor: default record (1402fea10) at +0x10
// @src 140300220  record copy: three ints, then the list handle (refs++, 1403002e0)
// @src 1403002e0  list handle copy: share the body, refs++ (140159910)
// @src 1403009d0  iterator copy
// @src 140300a30  iterator -> node + 0x10
// @src 140300a50  iterator post-increment
// @src 1403009f0  iterator != (clone loop)
void point_sets_push_back(DiePointSets& l, const DiePointSet& r) { l.push_back(r); }

// @src 1402feeb0  entry i: detach (1402ff270), walk i nodes from head->next (1402ffcd0), node + 0x10
// @src 1402ffcd0  walk i nodes
DiePointSet& point_sets_at(DiePointSets& l, std::int32_t i)
{
    return l.mutable_at(static_cast<std::size_t>(static_cast<std::int64_t>(i)));
}

// @src 1402fefb0  push_back on the edge-set list: detach (1402ff2e0), end() (1402ff2a0, iterator
//                 1402ff550), insert (1402ffeb0: node 0x28, 140300120 -> record copy 140300280)
// @src 1402ff2e0  detach when refs > 1 (1402ff820)
// @src 1402ff820  refs--, new body cloned from the old one (140300780)
// @src 140300780  body clone: sentinel (140300150), each record copied in order (140300b20,
//                 140300b00, 140300aa0, 1402ffeb0)
// @src 1402ff2a0  end(), detaching
// @src 1402ff550  iterator ctor
// @src 1402ffeb0  insert a copy of the record before the iterator, size++
// @src 140300120  node ctor: copy the record at +0x10 (140300280)
// @src 140300150  sentinel node ctor: default record (1402fead0) at +0x10
// @src 140300280  record copy: three ints, then the list handle (refs++)
// @src 140300aa0  iterator copy
// @src 140300ac0  iterator !=
// @src 140300b00  iterator -> node + 0x10
// @src 140300b20  iterator post-increment
void edge_sets_push_back(DieEdgeSets& l, const DieEdgeSet& r) { l.push_back(r); }

// @src 1402ff000  entry i: detach (1402ff2e0), walk i nodes (1402fffa0), node + 0x10
// @src 1402fffa0  walk i nodes
DieEdgeSet& edge_sets_at(DieEdgeSets& l, std::int32_t i)
{
    return l.mutable_at(static_cast<std::size_t>(static_cast<std::int64_t>(i)));
}

// @src 1402ff5f0  clear: refs == 1 frees the nodes in place (1403004a0), else refs-- (140001010)
//                 and a new empty body (1402ff930)
// @src 1403004a0  body clear: size 0, free the nodes, relink the sentinel to itself
void points_clear(CowList<DiePoint>& l) { l.clear(); }

// @src 1402fed80  push_back: detach (1402b75a0), end() (1402ff1f0), insert before it (1402b79a0:
//                 node 0x38, value copy 1402b7a90)
// @src 1402b75a0  detach when refs > 1 (1402b75d0)
// @src 1402b75d0  refs--, new body cloned from the old one (1402b76b0)
// @src 1402b76b0  body clone: sentinel (1402b7830), each point copied in order (1402b7950,
//                 1402b7930, 1402b78d0, 1402b79a0)
// @src 1402ff1f0  end(), detaching (iterator 1402b78b0)
// @src 1402b78b0  iterator ctor
// @src 1402b78d0  iterator copy
// @src 1402b78f0  iterator !=
// @src 1402b7930  iterator -> node + 0x10
// @src 1402b7950  iterator post-increment
// @src 1402b79a0  insert a copy of the point before the iterator, size++
// @src 1402b7a90  node ctor: copy the 0x28-byte point to +0x10
void points_push_back(CowList<DiePoint>& l, const DiePoint& p) { l.push_back(p); }

// @src 1402b7560  entry i: detach (1402b75a0), walk i nodes (1402b7650), node + 0x10
// @src 1402b7650  walk i nodes
DiePoint& points_at(CowList<DiePoint>& l, std::int32_t i)
{
    return l.mutable_at(static_cast<std::size_t>(static_cast<std::int64_t>(i)));
}

// @src 140159a70  entry i: detach (140159f90), walk i nodes (14015ac20), node + 0x10
// @src 14015ac20  walk i nodes
std::int32_t& ints_at(CowList<std::int32_t>& l, std::int32_t i)
{
    return l.mutable_at(static_cast<std::size_t>(static_cast<std::int64_t>(i)));
}

// ---- OBJ_GEO3D -----------------------------------------------------------------------------------

// @src 140006410  &coords[0] for writing (140005e70 on +0x10)
double* geo3d_coords(OBJ_GEO3D* s) { return s->coords.mutable_begin(); }

// @src 140005510  &elems[0] for writing (140005100 on +0x18)
std::int32_t* geo3d_elems(OBJ_GEO3D* s) { return s->elems.mutable_begin(); }

// @src 140199fa0  &elem_ids[0] for writing (140005100 on +0x68)
std::int32_t* geo3d_elem_ids(OBJ_GEO3D* s) { return s->elem_ids.mutable_begin(); }

// @src 1402fcd90
void geo3d_size(OBJ_GEO3D* s, const std::int32_t* nnode, const std::int32_t* nelem)
{
    s->resize_nodes(nnode);   // 1402f9600
    s->resize_elems(nelem);   // 1402f96a0
    s->alloc_elem_flags();    // 1402f9970
    s->resize_elem_ids();     // 1402f9a50
}

// @src 1402fbb30  empty
void geo3d_fun_1402fbb30(OBJ_GEO3D*) {}

// @src 1402fbb70  +0x48
std::int32_t geo3d_patch_filter(OBJ_GEO3D* s) { return s->u48; }

// @src 1402fd8a0
bool is_patch_type(std::int32_t v)
{
    bool r = false;
    if (1000 < v && v < 2000) r = true;
    return r;
}

// @src 1402fd7f0
// @src 1402feb90  &ids[i]: 1402ff1a0 (body + 8 = first, no detach) + i * 4
// @src 1402ff1a0  first element of a CowVector, read-only
bool geo3d_elem_in_patch(OBJ_GEO3D* s, std::int32_t i1)
{
    const std::int32_t n = s->elem_count();
    if (i1 < 1 || n < i1) return false;
    const std::int32_t v = s->elem_ids.begin()[i1 - 1];
    if (is_patch_type(v)) {
        if (geo3d_patch_filter(s) < 1) return true;
        if (geo3d_patch_filter(s) == v - 1000) return true;
    }
    return false;
}

// @src 1402fce80
// @src 140159ab0  list size (body + 0x10)
void geo3d_clear_patches(OBJ_GEO3D* s)
{
    if (s->u40.size() != 0) s->u40.clear();  // 14015a620
    s->u48 = 0;
}

// @src 1402fced0
// @src 1402febf0  find(begin, end, v): detach (140159f90), 1402ff8a0 from head->next
// @src 1402ff8a0  linear search comparing the low 4 bytes of each value (140300180, 1403001e0,
//                 140300200: iterator ctor, value, ++)
// @src 140300180  iterator ctor
// @src 1403001e0  iterator -> node + 0x10
// @src 140300200  iterator ++
// @src 140159ef0  end(): detach (140159f90), iterator(head) (14015a5b0)
// @src 14015a5b0  iterator ctor
// @src 1401599e0  insert before end: detach, 14015aa90
// @src 1402ff090  iterator ==
// @src 1403001a0  iterator != (search loop)
void geo3d_rebuild_patches(OBJ_GEO3D* s)
{
    geo3d_clear_patches(s);
    std::int32_t v = 0;
    const std::int32_t n = s->elem_count();
    if (0 < n) {
        std::int32_t* ids = geo3d_elem_ids(s);
        for (std::int32_t i = 0; i < n; i = i + 1) {
            if (geo3d_elem_in_patch(s, i + 1)) {
                v = ids[i] + -1000;
                auto& l = s->u40.mutable_items();
                auto it = l.begin();
                while (it != l.end() && patch_int(*it) != v) ++it;
                if (it == l.end()) s->u40.push_back(patch_value(v));
            }
        }
    }
}

std::int32_t geo3d_patch_count(OBJ_GEO3D* s) { return static_cast<std::int32_t>(s->u40.size()); }
std::int32_t geo3d_patch_at(OBJ_GEO3D* s, std::int32_t i)
{
    return patch_int(s->u40.at(static_cast<std::size_t>(static_cast<std::int64_t>(i))));
}

// @src 1402f93b0  14019a820(+0x50) != 0
// @src 14019a820  list size (body + 0x10)
bool geo3d_has_point_sets(OBJ_GEO3D* s) { return die_point_sets(s).size() != 0; }

// @src 1402f93f0  14019a840(+0x58) != 0
// @src 14019a840  list size (body + 0x10)
bool geo3d_has_edge_sets(OBJ_GEO3D* s) { return die_edge_sets(s).size() != 0; }

// @src 1402fd020
DiePointSet* geo3d_point_set_for(OBJ_GEO3D* s, const std::int32_t* id, const std::int32_t* type)
{
    DiePointSets& l = die_point_sets(s);
    const std::int32_t n = static_cast<std::int32_t>(l.size());
    std::int32_t found = -1;
    for (std::int32_t i = 0; i < n; i = i + 1) {
        if (point_sets_at(l, i).get_id() == *id) {
            if (point_sets_at(l, i).get_type() != *type) point_sets_at(l, i).set_type(type);
            found = i;
            break;
        }
    }
    if (found < 0) {
        DiePointSet r;  // 1402fea10
        r.set_id(id);
        r.set_type(type);
        point_sets_push_back(l, r);
        found = static_cast<std::int32_t>(l.size()) + -1;
    }  // 1402fea60
    return &point_sets_at(l, found);
}

// @data 1406fed80  the empty point-set record 1402fd3e0 returns for an out-of-range index
// (built on first use, guard bit DAT_1406fed98, atexit 140504840)
static DiePointSet g_empty_point_set;
// @data 1406feda0  the same for 1402fd740 (guard DAT_1406fedb8, atexit 140504860)
static DieEdgeSet g_empty_edge_set;

// @src 1402fd3e0
DiePointSet* geo3d_point_set(OBJ_GEO3D* s, const std::int32_t* k)
{
    const std::int32_t n = static_cast<std::int32_t>(die_point_sets(s).size());
    if (*k < 1 || n < *k) return &g_empty_point_set;
    return &point_sets_at(die_point_sets(s), *k + -1);
}

// @src 1402fd490
DieEdgeSet* geo3d_edge_set_for(OBJ_GEO3D* s, const std::int32_t* id, const std::int32_t* type)
{
    DieEdgeSets& l = die_edge_sets(s);
    const std::int32_t n = static_cast<std::int32_t>(l.size());
    std::int32_t found = -1;
    for (std::int32_t i = 0; i < n; i = i + 1) {
        if (edge_sets_at(l, i).get_id() == *id) {
            if (edge_sets_at(l, i).get_type() != *type) edge_sets_at(l, i).set_type(type);
            found = i;
            break;
        }
    }
    if (found < 0) {
        DieEdgeSet r;  // 1402fead0
        r.set_id(id);
        r.set_type(type);
        edge_sets_push_back(l, r);
        found = static_cast<std::int32_t>(l.size()) + -1;
    }  // 1402feb20
    return &edge_sets_at(l, found);
}

// @src 1402fd740
DieEdgeSet* geo3d_edge_set(OBJ_GEO3D* s, const std::int32_t* k)
{
    const std::int32_t n = static_cast<std::int32_t>(die_edge_sets(s).size());
    if (*k < 1 || n < *k) return &g_empty_edge_set;
    return &edge_sets_at(die_edge_sets(s), *k + -1);
}

// ---- LLOBJ_DATA predicates -----------------------------------------------------------------------

// @src 140160db0
bool object_has_die_points(LLOBJ_DATA& obj)
{
    if (!has_surface_flag(obj)) return false;  // 140160d90
    if (obj.u3c00[0] == nullptr) return false;
    return geo3d_has_point_sets(static_cast<OBJ_GEO3D*>(obj.u3c00[0]));
}

// @src 140160e00
bool object_has_die_edges(LLOBJ_DATA& obj)
{
    if (!has_surface_flag(obj)) return false;
    if (obj.u3c00[0] == nullptr) return false;
    return geo3d_has_edge_sets(static_cast<OBJ_GEO3D*>(obj.u3c00[0]));
}

// @src 140203a20
bool object_has_surface_mesh(LLOBJ_DATA& obj)
{
    bool r = has_surface_flag(obj);
    if (r) {
        if (0 < surface(obj, 0)->node_count() && 0 < surface(obj, 0)->elem_count()) return true;
        r = false;
    }
    return r;
}

}  // namespace def::model::geo_kw
