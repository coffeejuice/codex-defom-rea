// Defaults of the p9-03a seam (ext.hpp): owners on this branch, adapters, or DEF_NOT_PORTED.
#include "ext.hpp"

#include <cstdlib>
#include <cstring>
#include <string>

#include <defcmd/cnvt3d_sweep.hpp>
#include <defrt/stdlib/str_ends_with.hpp>
#include <defcmd/cnvt3d.hpp>
#include <defcmd/geolib.hpp>
#include <defcmd/geoprim/builder.hpp>
#include <defcmd/meshgen/meshgen_base.hpp>
#include <defcmd/meshgen/p8_seams.hpp>
#include <defcmd/meshgen/mg_globals.hpp>
#if __has_include("../meshgen/mg_internal.hpp")
#include "../meshgen/mg_internal.hpp"   // p8-07: 140155340 geo23_clear
#define P903A_HAVE_P8_07_INTERNAL 1
#endif
#include <defcmd/objpos.hpp>
#include <defcmd/objpos_centre.hpp>
#include <defdbio/gendb/counts_frame.hpp>
#include <defdbio/gendb/ext_p4_06.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/misc_model.hpp>
#include <defmodel/hdrstats/object_stats.hpp>
#include <defmodel/statevar_kw/statevar_kw.hpp>
#include <defdbio/objcheck/objcheck.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/llobj_ctor/llobj_copy.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defmodel/phase/phase_ops.hpp>
#include <defmodel/step/name_arrays.hpp>
#include <defmodel/window/window_list.hpp>
#include <defrt/port.hpp>

#include "../ifd/drv/ifd3d.hpp"
#include "internal.hpp"

namespace def::cmd::cnvt3d_sweep {

namespace {


// 1401b0cc0: def::rt::str_ends_with (the one body; owner p4-01)
bool ends_with_adapter(const char* sp, const char* p) { return def::rt::str_ends_with(std::string(sp), p); }

template <std::uint64_t A, class R, class... P>
R not_ported(P...)
{
    DEF_NOT_PORTED(A);
}

Ext make_default()
{
    Ext e{};
    e.fun_1401d6ea0 = [](DB_STEP_DATA* m, std::int32_t id) { return model::object_list_find(&m->objects, id); };
    e.fun_1401ee4c0 = [](const void* s) { return model::geo_kw::simctl_geo_kind(s); };
    e.fun_14019ccc0 = [](DB_STEP_DATA* m, std::int32_t no) { return model::llobj_ctor::get_or_create(m, no); };
    e.fun_1401d8120 = [](DB_STEP_DATA* m) { return model::companion_model(m); };
    e.fun_140160d90 = [](LLOBJ_DATA* o) { return model::geo_kw::has_surface_flag(*o); };
    e.fun_140160cc0 = [](LLOBJ_DATA* o) { return objpos::has_geometry(o); };
    e.fun_140189b70 = [](void* d, const void* s, const char* a, const char* b) {
        model::llobj_copy::meshgen_copy(static_cast<std::byte*>(d), static_cast<const std::byte*>(s), a, b);
    };
    e.fun_140189580 = [](void* blk) {
        return model::winlist_count(reinterpret_cast<const model::Cls_140269250*>(static_cast<std::byte*>(blk) + 0x40));
    };
    e.fun_140269900 = [](void* l, std::int32_t n) -> void* {
        return model::winlist_nth(static_cast<const model::Cls_140269250*>(l), n);
    };
    e.fun_1400034d0 = [](void* g) { return static_cast<model::OBJ_GEO3D*>(g)->node_count(); };
    e.fun_140003500 = [](void* g) { return static_cast<model::OBJ_GEO3D*>(g)->elem_count(); };
    e.fun_140006410 = [](void* g) { return static_cast<model::OBJ_GEO3D*>(g)->coords.mutable_begin(); };  // as geo_kw 140006410
    e.fun_140005510 = [](void* g) { return static_cast<model::OBJ_GEO3D*>(g)->elems.mutable_begin(); };  // as geo_kw 140005510

    e.fun_140148840 = [](MeshGenBase* mg, LLOBJ_DATA* o, DB_STEP_DATA* m, const char* p) { meshgen_bind(*mg, o, m, p); };
    e.fun_14013ee00 = [](MeshGenBase* mg, char f) { return static_cast<std::int32_t>(mg->surface_step(f)); };  // vtable +0x08 (p8-07)
    e.fun_140140110 = [](MeshGenBase* mg, char f) { return static_cast<std::int32_t>(mesh3d_volume_step(*mg, f)); };
    e.dat_1406e2d74 = [] { return &g_mesher_state; };  // 1406e2d74 (owner p8-07, mg_globals.hpp)
    e.dat_1406e2d84 = [] { return &cnvt3d::g_obj_index; };
    e.dat_1406e2d88 = [] { return &cnvt3d::g_obj_count; };
#ifdef P903A_HAVE_P8_07_INTERNAL
    e.fun_140155340 = [](MeshGenBase* mg, LLOBJ_DATA* o, double c) { geo23_clear(*mg, o, c); };  // p8-07
#else
    e.fun_140155340 = &not_ported<0x140155340, void, MeshGenBase*, LLOBJ_DATA*, double>;
#endif

    e.fun_1401b0cc0 = &ends_with_adapter;
    e.fun_1401b1520 = [](std::string s, std::int32_t n) { return fun_1401b1520(std::move(s), n); };

    e.fun_140198d30 = [](LLOBJ_DATA* o, double s, double a) { cnvt3d::mark_cut_faces(o, s, a); };
    e.fun_1402ba500 = [](model::geoprim::DEF_GEOPRIM* p, LLOBJ_DATA* o, const char* f, DB_STEP_DATA* m, void* c) {
        return geoprim::fun_1402ba500(p, o, f, m, c);
    };
    e.fun_14021d2f0 = [](LLOBJ_DATA* o, const double* p, const double* d, double a) { objpos::rotate(o, p, d, a); };
    e.fun_1401fc730 = [](LLOBJ_DATA* o, char* f) { objpos::fun_1401fc730(o, f); };
    e.fun_1401ffd80 = [](LLOBJ_DATA* o) { geolib::fun_1401ffd80(o); };
    e.fun_1401808d0 = [](void* s) { return ifd::node_coords(s); };
    e.fun_140183cb0 = [](void* s) { return ifd::elem_conn(s); };
    e.fun_14027ece0 = [](model::OBJ_GEO2D* g, const FixgeoPar* p, std::int32_t* o, std::int32_t d, char c) {
        return model::geoprim::fixgeo(g, p, o, d, c);
    };

    e.fun_1401d72e0 = &cnvt3d_sweep::fun_1401d72e0;
    e.fun_140148960 = &cnvt3d_sweep::fun_140148960;
    e.fun_140148a20 = &cnvt3d_sweep::fun_140148a20;
    e.fun_140148f70 = &cnvt3d_sweep::fun_140148f70;
    e.fun_14014c0a0 = &cnvt3d_sweep::fun_14014c0a0;
    return e;
}

Ext& storage()
{
    static Ext e = make_default();
    return e;
}

} // namespace

Ext& seam() { return storage(); }

void reset_seam()
{
    storage() = make_default();
    g_mesher_state = 0;
    cnvt3d::g_obj_index = 0;
    cnvt3d::g_obj_count = 0;
}

} // namespace def::cmd::cnvt3d_sweep
