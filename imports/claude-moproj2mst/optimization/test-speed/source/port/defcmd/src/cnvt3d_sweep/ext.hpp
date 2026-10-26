// Calls of packet p9-03a (cnvt3d_sweep, half a) into functions of other packets.
//
// Every call goes through a function pointer named after the callee's address. The default is
// the owner's function when it is on this branch, else an adapter, else DEF_NOT_PORTED (owner
// named in the comment). Tests replace pointers and call reset_seam() (called seam, not ext: p9-03b owns the namespace cnvt3d_sweep::ext); at integration the merge
// points a DEF_NOT_PORTED entry at the owner (one line in ext.cpp).
#pragma once

#include <cstdint>
#include <cstdio>
#include <string>

#include <defmodel/geoprim/fixgeo.hpp>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
class OBJ_GEO2D;
class DYNARY;
namespace geoprim {
class DEF_GEOPRIM;
}
} // namespace def::model

namespace def::cmd {
class MeshGenBase;
}

namespace def::cmd::cnvt3d_sweep {

using model::DB_STEP_DATA;
using model::LLOBJ_DATA;

// FIXGEO option record (140220390 ctor / 140220420 dtor, owner p9-01, defmodel/geoprim/fixgeo.hpp):
// constructed on the caller's stack as in the binary (14041e270 RSP+0x38..0x68).
using FixgeoPar = model::geoprim::FixgeoOptions;

struct Ext {
    // ---- model and object lookup ----
    // 1401d6ea0 (P2): object `id` of the model's list +0x1380
    LLOBJ_DATA* (*fun_1401d6ea0)(DB_STEP_DATA* m, std::int32_t id);
    // 1401ee4c0 (geo_kw): sim control (model+0x750) -> 1 revolve, 2 extrude
    std::int32_t (*fun_1401ee4c0)(const void* simctl);
    // 14019ccc0 (llobj_ctor): find or create object `no`
    LLOBJ_DATA* (*fun_14019ccc0)(DB_STEP_DATA* m, std::int32_t no);
    // 1401d8120 (phase_ops): previous-step (companion) model
    DB_STEP_DATA* (*fun_1401d8120)(DB_STEP_DATA* m);
    // 140160d90 (geo_kw): byte obj+0x3990
    bool (*fun_140160d90)(LLOBJ_DATA* obj);
    // 140160cc0 (p67-1 objpos): object has geometry
    bool (*fun_140160cc0)(LLOBJ_DATA* obj);
    // 140189b70 (llobj_copy): copy the mesh-generation block +0x2d10 (dst, src, &shapes, &by_dim)
    void (*fun_140189b70)(void* dst, const void* src, const char* shapes, const char* by_dim);
    // 140189580 (window_list): window count of the block +0x2d10
    std::int32_t (*fun_140189580)(void* blk);
    // 140269900 (window_list): window n (1-based) of the list at blk+0x40
    void* (*fun_140269900)(void* list, std::int32_t n);
    // 1400034d0 / 140003500 / 140006410 / 140005510 (OBJ_GEO3D at window+0x220)
    std::int32_t (*fun_1400034d0)(void* geo3d);
    std::int32_t (*fun_140003500)(void* geo3d);
    double* (*fun_140006410)(void* geo3d);
    std::int32_t* (*fun_140005510)(void* geo3d);

    // ---- _MeshGenBase (p8-07) and the meshers ----
    // 140148840 (p8-07): bind obj / model / prob to mg
    void (*fun_140148840)(MeshGenBase* mg, LLOBJ_DATA* obj, DB_STEP_DATA* m, const char* prob);
    // 14013ee00 (p8-07, vtable slot +0x08): surface mesh, DEF_AMG3_GUI
    std::int32_t (*fun_14013ee00)(MeshGenBase* mg, char flag);
    // 140140110 (p8-06, slot +0x10): solid mesh
    std::int32_t (*fun_140140110)(MeshGenBase* mg, char flag);
    // DAT_1406e2d74 (p8-07): set to 1 by a successful surface mesh
    std::int32_t* (*dat_1406e2d74)();
    // DAT_1406e2d84 / DAT_1406e2d88 (p9-01 driver, cnvt3d::g_obj_index / g_obj_count): object i of n
    std::int32_t* (*dat_1406e2d84)();
    std::int32_t* (*dat_1406e2d88)();
    // 140155340 (p8-07): axis snapping of the 2D source (cvt, obj, c)
    void (*fun_140155340)(MeshGenBase* cvt, LLOBJ_DATA* obj, double c);

    // ---- log helpers ----
    // 1401b0cc0 (def_pre menu7, ported there as ends_with): s ends with p, case folded, longer
    bool (*fun_1401b0cc0)(const char* s, const char* p);
    // 1401b1520 (p8-08): the first n characters when 0 < n < length
    std::string (*fun_1401b1520)(std::string s, std::int32_t n);

    // ---- geometry ----
    // 140198d30 (p9-01, defcmd/cnvt3d.hpp mark_cut_faces): symmetry faces / surfaces of a partial revolve
    void (*fun_140198d30)(LLOBJ_DATA* o3, double start, double ang);
    // 1402ba500 (p9-04, defcmd/geoprim/builder.hpp): GEOPRIM builder (prim, o3, &flag, model3, ctx)
    bool (*fun_1402ba500)(model::geoprim::DEF_GEOPRIM* prim, LLOBJ_DATA* o3, const char* flag,
                          DB_STEP_DATA* model3, void* ctx);
    // 14021d2f0 (p67-1): rotate an object about (origin, dir) by deg
    void (*fun_14021d2f0)(LLOBJ_DATA* o, const double* origin, const double* dir, double deg);
    // 1401fc730 (p67-1): boundary geometry from the mesh
    void (*fun_1401fc730)(LLOBJ_DATA* o, char* flag);
    // 1401ffd80 (p8-09 geolib)
    void (*fun_1401ffd80)(LLOBJ_DATA* o);
    // 1401808d0 / 140183cb0 (p67-2 ifd): node coordinates / element connectivity
    double* (*fun_1401808d0)(void* nodes);
    std::int32_t* (*fun_140183cb0)(void* elems);
    // 14027ece0 (p9-01, defmodel/geoprim/fixgeo.hpp): FIXGEO of a profile (geo, par, &out, delPnt,
    // recount)
    std::int32_t (*fun_14027ece0)(model::OBJ_GEO2D* g, const FixgeoPar* par, std::int32_t* out,
                                  std::int32_t del_pnt, char recount);

    // p9-03b entry points (defaults: its bodies, defcmd/cnvt3d_sweep_b.hpp; pointers for the tests)
    void (*fun_140148960)(MeshGenBase* cvt, std::int32_t target);
    void (*fun_140148a20)(MeshGenBase* cvt, LLOBJ_DATA* tet);
    void (*fun_140148f70)(MeshGenBase* cvt, std::int32_t nsec, double start, double ang, double c,
                          LLOBJ_DATA* obj);
    void (*fun_14014c0a0)(MeshGenBase* cvt, std::int32_t nsec, double start, double len, LLOBJ_DATA* obj);
    // 1401d72e0 (p9-03b): STNCMP block offset of a model
    std::int32_t (*fun_1401d72e0)(DB_STEP_DATA* m, std::int32_t i);
};

Ext& seam();
void reset_seam();

} // namespace def::cmd::cnvt3d_sweep
