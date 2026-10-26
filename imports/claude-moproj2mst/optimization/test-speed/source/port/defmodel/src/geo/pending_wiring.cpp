// Default targets of p1-07's pending calls (defmodel/geo/pending.hpp), wired to the owners'
// functions at the Phase 1 gate. Each adapter keeps p1-07's call signature and forwards to the
// owner, which carries the @src tag.
#include <defmodel/geo/pending.hpp>

#include <defmodel/layout/cls_14042a870.hpp>
#include <defmodel/layout/def_edge.hpp>
#include <defmodel/layout/def_face.hpp>
#include <defmodel/layout/def_loop.hpp>
#include <defmodel/layout/def_vertex.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/obj_geo2d_digitized.hpp>
#include <defmodel/layout/obj_mesh2d.hpp>
#include <defmodel/linked/accessors.hpp>
#include <defmodel/linked/axis_point_list.hpp>
#include <defmodel/linked/matrix4.hpp>
#include <defmodel/llobj/accessors.hpp>
#include <defmodel/members/ftn_geom.hpp>
#include <defmodel/mesh2d/geo2d_mesh_ops.hpp>
#include <defmodel/sdctl/cow.hpp>
#include <defmodel/geo_kw/surface_model.hpp>
#include <defmodel/statevar/geo2d_kernels.hpp>
#include <defmodel/topo/ftn_geom.hpp>

#include <cstring>
#include <iterator>
#include <new>

namespace def::model::pending::wired {

namespace {



} // namespace

// ---- p1-11: OBJ_GEO2D_DIGITIZED and the Fortran outline routine ----
OBJ_GEO2D_DIGITIZED* fun_14041dcc0(void* mem) { return ::new (mem) OBJ_GEO2D_DIGITIZED(); }
void fun_14041e090(OBJ_GEO2D_DIGITIZED* dst, OBJ_GEO2D_DIGITIZED* src) { dst->copy_points(src); }
void fun_14041f590(std::int32_t* dof, std::int32_t* n, double* a) { topo::reverse_point_rows(dof, n, a); }

// ---- p1-09 / p1-10: Fortran outline conversion and rotation ----
void fun_14032ac75(std::int32_t* nout, double* out, std::int32_t* nin, double* in)
{
    geo2d::entities_to_points(nout, out, nin, in);
}
void fun_14032b34b(std::int32_t* nin, double* in, std::int32_t* nout, double* out)
{
    geo2d::points_to_entities(nin, in, nout, out);
}
void fun_140345f5b(double* in, std::int32_t* n) { model::fun_140345f5b(in, n); }
void fun_14032ab90(double* pt, double* angle, const double* ctr) { geo2d::rotate_point_deg(pt, angle, ctr); }

// ---- p1-09: vector helpers ----
Vec3* fun_140308730(Vec3* out, const double* xy)
{
    geo2d::copy_vec(out->v, xy);
    return out;
}
void fun_1403077e0(Vec3* a, Vec3* b, Vec3* out, double ang) { geo2d::rotate_difference_deg(a->v, b->v, out->v, ang); }
Vec3* fun_140308a20(Vec3* out, Vec3* a, Vec3* b)
{
    geo2d::add_vec(out->v, a->v, b->v);
    return out;
}
double fun_140309550(std::int32_t n, double* xy) { return geo2d::polygon_area(n, xy); }

// ---- p1-06: point list of _OBJ_GEO3D (+0x28) ----
// 140257bf0 -> 140257c20: head = tail = 0, count = 0 (the 4 bytes after count untouched)
void* fun_140257bf0(void* list)
{
    const std::uint64_t z8 = 0;
    const std::uint32_t z4 = 0;
    std::memcpy(static_cast<char*>(list) + 0x08, &z8, 8);
    std::memcpy(static_cast<char*>(list) + 0x00, &z8, 8);
    std::memcpy(static_cast<char*>(list) + 0x10, &z4, 4);
    return list;
}
void fun_140257c60(void* list) { static_cast<linked::Cls_140257340List*>(list)->~Cls_140257340List(); }
void fun_1402581b0(void* list) { static_cast<linked::Cls_140257340List*>(list)->clear(); }
void fun_140257c80(void* dst, void* src)
{
    static_cast<linked::Cls_140257340List*>(dst)->copy_from(*static_cast<linked::Cls_140257340List*>(src));
}
void fun_140258400(void* list, const double* center, const double* axis, double angle)
{
    static_cast<linked::Cls_140257340List*>(list)->rotate_all(center, axis, angle);
}

// ---- p1-06: 4x4 matrix ----
void* fun_140220540(void* m) { return ::new (m) linked::Matrix4(); }
void fun_140220560(void* m) { static_cast<linked::Matrix4*>(m)->~Matrix4(); }
void fun_140220570(void* m) { static_cast<linked::Matrix4*>(m)->set_identity(); }
void* fun_140220670(void* dst, void* src)
{
    return &static_cast<linked::Matrix4*>(dst)->assign(*static_cast<linked::Matrix4*>(src));
}
void* fun_1402214c0(void* out, const double* center, const double* axis, double* angle)
{
    return ::new (out) linked::Matrix4(linked::rotation_about_axis(center, axis, angle));
}

// ---- the two lists of _OBJ_GEO3D at +0x50 / +0x58: die point / die edge sets (kw-g2geo,
// defmodel/geo_kw/surface_model.hpp; the records hold COW lists, so they are typed there) ----
// @src 1402fedd0  handle ctor: new 0x18, body ctor 1402ffa60 (refs 1, sentinel, size 0)
void** fun_1402fedd0(void** h)
{
    ::new (h) geo_kw::DiePointSets();
    return h;
}
// @src 1402fef20  handle ctor: new 0x18, body ctor 1402ffd30
void** fun_1402fef20(void** h)
{
    ::new (h) geo_kw::DieEdgeSets();
    return h;
}
// @src 1402fee40  dtor -> release 1402ff3b0 (refs--, at 0 deleting dtor 1402ff4b0)
// @src 1402ff3b0  release
void fun_1402fee40(void** h) { reinterpret_cast<geo_kw::DiePointSets*>(h)->~CowList(); }
// @src 1402fef90  dtor -> release 1402ff410 (refs--, at 0 deleting dtor 1402ff4f0)
// @src 1402ff410  release
void fun_1402fef90(void** h) { reinterpret_cast<geo_kw::DieEdgeSets*>(h)->~CowList(); }
// @src 1402ff680  clear: refs == 1 -> free the nodes in place (1403006c0), else refs-- and a new
//                 empty body (1402ffa60)
void fun_1402ff680(void** h) { reinterpret_cast<geo_kw::DiePointSets*>(h)->clear(); }
// @src 1402ff790  the same for the edge sets (140300900 in place, new body 1402ffd30)
void fun_1402ff790(void** h) { reinterpret_cast<geo_kw::DieEdgeSets*>(h)->clear(); }

// ---- p1-14: tree container at _OBJ_GEO3D +0x80 ----
void* fun_14042a7e0(void* tree) { return ::new (tree) Cls_14042a870(); }
void fun_14042a870(void* tree) { static_cast<Cls_14042a870*>(tree)->~Cls_14042a870(); }

// ---- p1-13: OBJ_MESH2D ----
void* fun_1403ddf20(void* mem) { return ::new (mem) OBJ_MESH2D(); }
void fun_1403de460(void* dst, void* src)
{
    static_cast<OBJ_MESH2D*>(dst)->copy_from(static_cast<const OBJ_MESH2D*>(src));
}
void delete_result_set(void* obj) { delete static_cast<OBJ_MESH2D*>(obj); }

// ---- p1-04 / p1-06 / p1-08: GEO2D_AND_MESH2D and _DEF_FACE methods ----
bool fun_140158ec0(GEO2D_AND_MESH2D* g) { return llobj::first_item_has_items(g) != 0; }
void fun_1402847d0(GEO2D_AND_MESH2D* g, const bool* v) { geo2d_set_b8(g, reinterpret_cast<const char*>(v)); }
DEF_FACE* fun_1402838d0(GEO2D_AND_MESH2D* g) { return geo2d_add_face(g); }
void* fun_140283980(GEO2D_AND_MESH2D* g) { return geo2d_add_loop(g); }
void* fun_140283a30(GEO2D_AND_MESH2D* g) { return geo2d_add_vertex(g); }
DEF_FACE* fun_140283f80(GEO2D_AND_MESH2D* g, std::int32_t* i) { return geo2d_face_at(g, i); }
DEF_LOOP* fun_140284030(GEO2D_AND_MESH2D* g, std::int32_t* i) { return geo2d_loop_at(g, i); }
DEF_VERTEX* fun_1402840e0(GEO2D_AND_MESH2D* g, std::int32_t* i) { return geo2d_vertex_at(g, i); }
void fun_140284190(GEO2D_AND_MESH2D* g) { geo2d_renumber_vertices(g); }
void fun_140284220(GEO2D_AND_MESH2D* g) { geo2d_renumber_edges(g); }
void fun_1402842b0(GEO2D_AND_MESH2D* g) { geo2d_renumber_loops(g); }
void fun_140284340(GEO2D_AND_MESH2D* g) { geo2d_renumber_faces(g); }
OBJ_GEO_BASE* fun_140284990(DEF_EDGE* e) { return edge_curve(e); }
void fun_1402849e0(DEF_LOOP* l, bool v) { loop_set_1c(l, static_cast<std::uint8_t>(v)); }
bool fun_140284a00(DEF_LOOP* l) { return loop_get_1c(l) != 0; }
bool fun_140284a10(DEF_EDGE* e) { return edge_get_34(e) != 0; }
bool fun_140284a20(DEF_EDGE* e) { return edge_get_35(e) != 0; }
void fun_140284a30(DEF_EDGE* e, bool v) { edge_set_34(e, static_cast<std::uint8_t>(v)); }
void fun_140284a50(DEF_EDGE* e, bool v) { edge_set_35(e, static_cast<std::uint8_t>(v)); }
void fun_140284a70(DEF_VERTEX* v, bool b) { vertex_set_20(v, static_cast<std::uint8_t>(b)); }
bool fun_140284a90(DEF_VERTEX* v) { return vertex_get_20(v) != 0; }
std::int32_t fun_140256720(DEF_LOOP* l) { return linked::get_i32_18(l); }
void fun_140256730(DEF_EDGE* e, DEF_VERTEX* v)
{
    linked::set_u64_18(e, static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(v)));
}
void fun_140256750(DEF_EDGE* e, DEF_VERTEX* v)
{
    linked::set_u64_20(e, static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(v)));
}
std::int32_t fun_140256770(DEF_VERTEX* v) { return linked::get_i32_24(v); }

// ---- p1-11: _DEF_EDGE / _DEF_LOOP / _DEF_VERTEX methods ----
DEF_EDGE* fun_14041c300(void* mem) { return ::new (mem) DEF_EDGE(); }
void fun_14041c4a0(DEF_EDGE* e, OBJ_GEO_BASE* g) { e->set_geo(g); }
void fun_14041c5f0(DEF_EDGE* e, std::int32_t* id) { e->set_id(id); }
std::int32_t fun_14041c610(DEF_EDGE* e) { return e->get_id(); }
IVertex* fun_14041c590(DEF_EDGE* e, std::int32_t* i) { return e->vertex(i); }
std::int32_t fun_14041c5c0(DEF_EDGE* e) { return e->loop_count(); }
ILoop* fun_14041c630(DEF_EDGE* e, std::int32_t* i) { return e->loop_at(i); }
void fun_14041c6c0(DEF_EDGE* e, DEF_LOOP* l) { e->add_loop(l); }
std::uint8_t fun_14041cc10(DEF_EDGE* e) { return e->fun_14041cc10() ? 1 : 0; }
void fun_14041cc70(DEF_EDGE* e, std::uint8_t*) { e->fun_14041cc70(); }
bool fun_14041ccd0(DEF_EDGE* e) { return e->curve_flag28(); }
void fun_14041cd40(DEF_EDGE* e, bool v) { e->set_curve_flag28(v); }
void fun_14041cf80(DEF_VERTEX* v, void* position) { v->set_xy(static_cast<const double*>(position)); }
void* fun_14041d000(DEF_VERTEX* v) { return v->get_xy(); }
IEdge* fun_14041d020(DEF_VERTEX* v, std::int32_t* i) { return v->edge_at(i); }
void fun_14041d0b0(DEF_VERTEX* v, DEF_EDGE* e) { v->add_edge(e); }
IEdge* fun_140412150(DEF_LOOP* l, std::int32_t* i) { return l->edge_at(i); }
void fun_1404121e0(DEF_LOOP* l, DEF_EDGE* e) { l->add_edge(e); }
IFace* fun_1404123b0(DEF_LOOP* l, std::int32_t* i) { return l->face_at(i); }
void fun_140412440(DEF_LOOP* l, DEF_FACE* f) { l->add_face(f); }
std::int32_t fun_1404125e0(DEF_LOOP* l) { return l->face_count(); }

// 140412550: remove face f from the loop: find its index i (1404123b0 over face_count), then
// erase entry i (1404124e0: detach, walk i nodes, unlink)
void fun_140412550(DEF_LOOP* l, DEF_FACE* f)
{
    const std::int32_t n = l->face_count();
    std::int32_t i = 0;
    while (true) {
        if (n <= i) {
            return;
        }
        if (l->face_at(&i) == static_cast<IFace*>(f)) {
            break;
        }
        i = i + 1;
    }
    auto& items = l->faces.mutable_items();
    items.erase(std::next(items.begin(), i));
}

// RTTI casts
DEF_EDGE* cast_def_edge(IEdge* p) { return dynamic_cast<DEF_EDGE*>(p); }
DEF_LOOP* cast_def_loop(ILoop* p) { return dynamic_cast<DEF_LOOP*>(p); }
DEF_VERTEX* cast_def_vertex(IVertex* p) { return dynamic_cast<DEF_VERTEX*>(p); }

// virtual calls the original makes through the vtable
std::int32_t vertex_edge_count(DEF_VERTEX* v) { return v->edge_count(); }
std::int32_t loop_edge_count(DEF_LOOP* l) { return l->edge_count(); }

} // namespace def::model::pending::wired
