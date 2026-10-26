// defmodel/geo/pending.hpp: calls from the geometry code (p1-07) into functions owned by packets
// that have not landed yet (p1-04, p1-06, p1-08, p1-09, p1-10, p1-11, p1-13, p1-14).
//
// Each call goes through a function pointer named after the callee's address. The default
// target aborts in DEF_NOT_PORTED; tier-1 tests install fakes; when the owning packet lands, the
// merge points the pointer at the owner's function (one line per entry, no symbol clashes since
// this namespace is p1-07's). Signatures follow the decompile: Fortran routines take every
// argument by address; `self` is the object the original passes in RCX.
//
// reset_pending() restores every default (tests call it between cases).
#pragma once

#include <cstdint>

#include <defmodel/port.hpp>

namespace def::model {

class OBJ_GEO_BASE;
class OBJ_GEO2D_DIGITIZED;
class GEO2D_AND_MESH2D;
class DEF_FACE;
class IFace;
// Topology classes of p1-11 (layout headers not written yet).
class ILoop;
class IEdge;
class IVertex;
class DEF_LOOP;
class DEF_EDGE;
class DEF_VERTEX;

} // namespace def::model

namespace def::model::pending {

template <std::uint64_t Address, class R, class... A>
R not_ported(A...)
{
    DEF_NOT_PORTED(Address);
}

// 3-vector returned by value (0x18 bytes) by the p1-09 vector helpers.
struct Vec3 {
    double v[3];
};

// Phase 1 gate: each pointer's default is now the owner's function, through the adapter
// wired::<name> (src/geo/pending_wiring.cpp); tests still install fakes and reset_pending()
// restores the wired defaults.
#define DEF_PENDING(addr, name, R, ...)                                                            \
    inline R (*name)(__VA_ARGS__) = &wired::name;
#define DEF_PENDING_WIRED_DECL(addr, name, R, ...) R name(__VA_ARGS__);

#define DEF_PENDING_LIST(X)                                                                        \
    /* ---- p1-11: OBJ_GEO2D_DIGITIZED and the Fortran outline routine ---- */                    \
    X(0x14041dcc0, fun_14041dcc0, OBJ_GEO2D_DIGITIZED*, void* /*mem 0x50*/)                              \
    X(0x14041e090, fun_14041e090, void, OBJ_GEO2D_DIGITIZED* /*dst*/, OBJ_GEO2D_DIGITIZED* /*src*/)             \
    X(0x14041f590, fun_14041f590, void, std::int32_t* /*dof*/, std::int32_t* /*n*/, double*)       \
    /* ---- p1-09 / p1-10: Fortran outline conversion and rotation ---- */                        \
    X(0x14032ac75, fun_14032ac75, void, std::int32_t* /*nout*/, double* /*out*/,                    \
      std::int32_t* /*nin*/, double* /*in*/)                                                       \
    X(0x14032b34b, fun_14032b34b, void, std::int32_t* /*nin*/, double* /*in*/,                      \
      std::int32_t* /*nout*/, double* /*out*/)                                                     \
    X(0x140345f5b, fun_140345f5b, void, double* /*in*/, std::int32_t* /*n*/)                       \
    X(0x14032ab90, fun_14032ab90, void, double* /*pt*/, double* /*angle*/, const double* /*ctr*/)  \
    /* ---- p1-09: 3-vector helpers ---- */                                                        \
    X(0x140308730, fun_140308730, Vec3*, Vec3* /*out*/, const double* /*xy*/)                      \
    X(0x1403077e0, fun_1403077e0, void, Vec3* /*a*/, Vec3* /*b*/, Vec3* /*out*/, double /*ang*/)   \
    X(0x140308a20, fun_140308a20, Vec3*, Vec3* /*out*/, Vec3* /*a*/, Vec3* /*b*/)                  \
    X(0x140309550, fun_140309550, double, std::int32_t /*n*/, double* /*xy*/)                      \
    /* ---- p1-06: point list of _OBJ_GEO3D (+0x28, 0x18 bytes) ---- */                            \
    X(0x140257bf0, fun_140257bf0, void*, void* /*list*/)                                           \
    X(0x140257c60, fun_140257c60, void, void* /*list*/)                                            \
    X(0x1402581b0, fun_1402581b0, void, void* /*list*/)                                            \
    X(0x140257c80, fun_140257c80, void, void* /*dst*/, void* /*src*/)                              \
    X(0x140258400, fun_140258400, void, void* /*list*/, const double* /*center*/,                  \
      const double* /*axis*/, double /*angle*/)                                                    \
    /* ---- p1-06: 4x4 matrix (0x80 bytes) ---- */                                                 \
    X(0x140220540, fun_140220540, void*, void* /*m*/)                                              \
    X(0x140220560, fun_140220560, void, void* /*m*/)                                               \
    X(0x140220570, fun_140220570, void, void* /*m*/)                                               \
    X(0x140220670, fun_140220670, void*, void* /*dst*/, void* /*src*/)                             \
    X(0x1402214c0, fun_1402214c0, void*, void* /*out*/, const double* /*center*/,                  \
      const double* /*axis*/, double* /*angle rad*/)                                               \
    /* ---- p1-09: the two lists of _OBJ_GEO3D at +0x50 / +0x58 (handles) ---- */                   \
    X(0x1402fedd0, fun_1402fedd0, void**, void** /*h*/)                                            \
    X(0x1402fef20, fun_1402fef20, void**, void** /*h*/)                                            \
    X(0x1402fee40, fun_1402fee40, void, void** /*h*/)                                              \
    X(0x1402fef90, fun_1402fef90, void, void** /*h*/)                                              \
    X(0x1402ff680, fun_1402ff680, void, void** /*h*/)                                              \
    X(0x1402ff790, fun_1402ff790, void, void** /*h*/)                                              \
    /* ---- p1-14: tree container Cls_14042a870 at _OBJ_GEO3D +0x80 (0xb0 bytes) ---- */            \
    X(0x14042a7e0, fun_14042a7e0, void*, void* /*tree*/)                                           \
    X(0x14042a870, fun_14042a870, void, void* /*tree*/)                                            \
    /* ---- p1-13: result set Cls_1403ddf20 (0x1700 bytes) ---- */                                 \
    X(0x1403ddf20, fun_1403ddf20, void*, void* /*mem*/)                                            \
    X(0x1403de460, fun_1403de460, void, void* /*dst*/, void* /*src*/)                              \
    /* (**vtable[0])(obj, 1): the result set's scalar deleting dtor */                             \
    X(0x1403de190, delete_result_set, void, void* /*obj*/)                                         \
    /* ---- p1-04 / p1-06 / p1-08: GEO2D_AND_MESH2D and _DEF_FACE methods at their addresses ---- */\
    X(0x140158ec0, fun_140158ec0, bool, GEO2D_AND_MESH2D* /*face 0 has loops*/)                    \
    X(0x1402847d0, fun_1402847d0, void, GEO2D_AND_MESH2D*, const bool*)                            \
    X(0x1402838d0, fun_1402838d0, DEF_FACE*, GEO2D_AND_MESH2D* /*add face*/)                       \
    X(0x140283980, fun_140283980, void*, GEO2D_AND_MESH2D* /*add loop*/)                           \
    X(0x140283a30, fun_140283a30, void*, GEO2D_AND_MESH2D* /*add vertex*/)                         \
    X(0x140283f80, fun_140283f80, DEF_FACE*, GEO2D_AND_MESH2D*, std::int32_t* /*face i*/)          \
    X(0x140284030, fun_140284030, DEF_LOOP*, GEO2D_AND_MESH2D*, std::int32_t* /*loop i*/)          \
    X(0x1402840e0, fun_1402840e0, DEF_VERTEX*, GEO2D_AND_MESH2D*, std::int32_t* /*vertex i*/)      \
    X(0x140284190, fun_140284190, void, GEO2D_AND_MESH2D* /*number vertices*/)                     \
    X(0x140284220, fun_140284220, void, GEO2D_AND_MESH2D* /*number edges*/)                        \
    X(0x1402842b0, fun_1402842b0, void, GEO2D_AND_MESH2D* /*number loops*/)                        \
    X(0x140284340, fun_140284340, void, GEO2D_AND_MESH2D* /*number faces*/)                        \
    X(0x140284990, fun_140284990, OBJ_GEO_BASE*, DEF_EDGE* /*edge geometry, +0x08*/)               \
    X(0x1402849e0, fun_1402849e0, void, DEF_LOOP*, bool /*+0x1c*/)                                 \
    X(0x140284a00, fun_140284a00, bool, DEF_LOOP* /*+0x1c*/)                                       \
    X(0x140284a10, fun_140284a10, bool, DEF_EDGE* /*+0x34*/)                                       \
    X(0x140284a20, fun_140284a20, bool, DEF_EDGE* /*+0x35*/)                                       \
    X(0x140284a30, fun_140284a30, void, DEF_EDGE*, bool /*+0x34*/)                                 \
    X(0x140284a50, fun_140284a50, void, DEF_EDGE*, bool /*+0x35*/)                                 \
    X(0x140284a70, fun_140284a70, void, DEF_VERTEX*, bool /*+0x20*/)                               \
    X(0x140284a90, fun_140284a90, bool, DEF_VERTEX* /*+0x20*/)                                     \
    X(0x140256720, fun_140256720, std::int32_t, DEF_LOOP* /*id, +0x18*/)                           \
    X(0x140256730, fun_140256730, void, DEF_EDGE*, DEF_VERTEX* /*start, +0x18*/)                   \
    X(0x140256750, fun_140256750, void, DEF_EDGE*, DEF_VERTEX* /*end, +0x20*/)                     \
    X(0x140256770, fun_140256770, std::int32_t, DEF_VERTEX* /*id, +0x24*/)                         \
    /* ---- p1-11: _DEF_EDGE / _DEF_LOOP / _DEF_VERTEX methods ---- */                             \
    X(0x14041c300, fun_14041c300, DEF_EDGE*, void* /*mem 0x38*/)                                   \
    X(0x14041c4a0, fun_14041c4a0, void, DEF_EDGE*, OBJ_GEO_BASE* /*geometry*/)                     \
    X(0x14041c5f0, fun_14041c5f0, void, DEF_EDGE*, std::int32_t* /*id*/)                           \
    X(0x14041c610, fun_14041c610, std::int32_t, DEF_EDGE* /*id*/)                                  \
    X(0x14041c590, fun_14041c590, IVertex*, DEF_EDGE*, std::int32_t* /*end 0/1*/)                  \
    X(0x14041c5c0, fun_14041c5c0, std::int32_t, DEF_EDGE* /*loop count*/)                          \
    X(0x14041c630, fun_14041c630, ILoop*, DEF_EDGE*, std::int32_t* /*loop i*/)                     \
    X(0x14041c6c0, fun_14041c6c0, void, DEF_EDGE*, DEF_LOOP* /*add loop*/)                         \
    X(0x14041cc10, fun_14041cc10, std::uint8_t, DEF_EDGE*)                                         \
    X(0x14041cc70, fun_14041cc70, void, DEF_EDGE*, std::uint8_t*)                                  \
    X(0x14041ccd0, fun_14041ccd0, bool, DEF_EDGE*)                                                 \
    X(0x14041cd40, fun_14041cd40, void, DEF_EDGE*, bool)                                           \
    X(0x14041cf80, fun_14041cf80, void, DEF_VERTEX*, void* /*position*/)                           \
    X(0x14041d000, fun_14041d000, void*, DEF_VERTEX* /*position*/)                                 \
    X(0x14041d020, fun_14041d020, IEdge*, DEF_VERTEX*, std::int32_t* /*edge i*/)                   \
    X(0x14041d0b0, fun_14041d0b0, void, DEF_VERTEX*, DEF_EDGE* /*add edge*/)                       \
    X(0x140412150, fun_140412150, IEdge*, DEF_LOOP*, std::int32_t* /*edge i*/)                     \
    X(0x1404121e0, fun_1404121e0, void, DEF_LOOP*, DEF_EDGE* /*add edge*/)                         \
    X(0x1404123b0, fun_1404123b0, IFace*, DEF_LOOP*, std::int32_t* /*face i*/)                     \
    X(0x140412440, fun_140412440, void, DEF_LOOP*, DEF_FACE* /*add face*/)                         \
    X(0x1404125e0, fun_1404125e0, std::int32_t, DEF_LOOP* /*face count*/)                          \
    X(0x140412550, fun_140412550, void, DEF_LOOP*, DEF_FACE* /*remove face*/)                      \
    /* RTTI: __RTDynamicCast(p, 0, &I*::RTTI_Type_Descriptor, &_DEF_*::RTTI_Type_Descriptor, 0) */\
    X(0x140478f88, cast_def_edge, DEF_EDGE*, IEdge*)                                               \
    X(0x140478f88, cast_def_loop, DEF_LOOP*, ILoop*)                                               \
    X(0x140478f88, cast_def_vertex, DEF_VERTEX*, IVertex*)                                         \
    /* virtual calls on a vertex / loop the original makes through the vtable */                  \
    X(0x0, vertex_edge_count, std::int32_t, DEF_VERTEX* /*(**vtbl+0x10)*/)                         \
    X(0x0, loop_edge_count, std::int32_t, DEF_LOOP* /*(**vtbl+0x18)*/)

namespace wired {
DEF_PENDING_LIST(DEF_PENDING_WIRED_DECL)
} // namespace wired

DEF_PENDING_LIST(DEF_PENDING)

// Restores every pointer to its default (the owner's function).
void reset_pending();

} // namespace def::model::pending
