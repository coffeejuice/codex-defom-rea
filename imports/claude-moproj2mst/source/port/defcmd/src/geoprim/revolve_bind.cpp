// Binder of the p9-05 hooks (P905Ext). Owners, by hook:
//   vec3_cross 14023a4b0     P4 (p4-02a window_orient.cpp, p67 ctc3d/cube.hpp)
//   vec3_length 140239a30    p67-1 defcmd/vec3.hpp (v3::len)
//   fixgeo* 14027ece0, 140220390, 140220420   p9-01 (defaults in revolve.cpp; launcher bound here)
//   model_copy_u15f8 1401d7320   p8-08 (defcmd/interp.hpp)
//   mesh_alloc 140202700     p8-09
//   node_coords 1401808d0    p67-3 defmodel/node_access.hpp
//   surface_update 1402fc1b0 p67-1 defcmd/objpos_centre.hpp (surface_compact_nodes)
// A hook is bound only when its owner's header is in the build (__has_include).
#include <defcmd/geoprim/revolve_bind.hpp>

#include <defmodel/geoprim/revolve.hpp>

#if __has_include(<defcmd/vec3.hpp>)
#include <defcmd/vec3.hpp>
#endif
#if __has_include(<defmodel/node_access.hpp>)
#include <defmodel/node_access.hpp>
#endif
#if __has_include(<defcmd/objpos_centre.hpp>)
#include <defcmd/objpos_centre.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#endif
// Integration on claude/p9-03: the owners now in the build for the remaining hooks
#include <new>
#if __has_include(<defcmd/ifd/cube_lib.hpp>)
#include <defcmd/ifd/cube_lib.hpp>  // p67-2: 14023a4b0 (src/ifd/cube/vec.cpp)
#define P905_HAVE_CUBE_VEC 1
#endif
#if __has_include(<defmodel/geoprim/fixgeo.hpp>)
#include <defmodel/geoprim/fixgeo.hpp>     // p9-01: 140220390 / 140220420 / 14027ece0
#define P905_HAVE_FIXGEO 1
#endif
#if __has_include(<defcmd/geolib.hpp>)
#include <defcmd/geolib.hpp>               // p8-09: 140202700
#define P905_HAVE_GEOLIB 1
#endif
#if __has_include(<defcmd/cnvt3d.hpp>)
#include <defcmd/cnvt3d.hpp>               // p9-01: bind_fixgeo_launch (140224100)
#endif
#if __has_include(<defcmd/interp.hpp>)
#include <defcmd/interp.hpp>               // p8-08: 1401d7320 model_copy_layout_table
#define P905_HAVE_INTERP 1
#endif

namespace def::cmd::geoprim {

void bind_p905_ext()
{
    [[maybe_unused]] auto& e = def::model::geoprim::p905_ext();
#if __has_include(<defcmd/vec3.hpp>)
    e.vec3_length = [](const double* v) { return def::cmd::v3::len(def::cmd::v3::make(v)); };
#endif
#if __has_include(<defmodel/node_access.hpp>)
    e.node_coords = [](void* set) {
        return def::model::fun_1401808d0(static_cast<def::model::Cls_14017ef20*>(set));
    };
#endif
#if __has_include(<defcmd/cnvt3d.hpp>)
    def::cmd::cnvt3d::bind_fixgeo_launch();   // 140224100 for FIXGEO (p9-01)
#endif
#ifdef P905_HAVE_INTERP
    e.model_copy_u15f8 = [](def::model::DB_STEP_DATA* m, std::int32_t* dst) {
        def::cmd::model_copy_layout_table(m, dst);   // 1401d7320 (p8-08)
    };
#endif
#if __has_include(<defcmd/objpos_centre.hpp>)
    e.surface_update = [](void* s) {
        def::cmd::objpos::surface_compact_nodes(static_cast<def::model::OBJ_GEO3D*>(s));
    };
#endif
#ifdef P905_HAVE_CUBE_VEC
    e.vec3_cross = [](double* out, const double* a, const double* b) {
        const def::cmd::cube::Vec3 r =
            def::cmd::cube::fun_14023a4b0(def::cmd::cube::Vec3{a[0], a[1], a[2]}, def::cmd::cube::Vec3{b[0], b[1], b[2]});
        out[0] = r.x;
        out[1] = r.y;
        out[2] = r.z;
    };
#endif
#ifdef P905_HAVE_FIXGEO
    using def::model::geoprim::FixgeoOptions;
    e.fixgeo_par_init = [](void* p) { new (p) FixgeoOptions(); };
    e.fixgeo_par_free = [](void* p) { static_cast<FixgeoOptions*>(p)->~FixgeoOptions(); };
    e.fixgeo = [](def::model::OBJ_GEO2D* g, void* p, std::int32_t* k, std::int32_t mode, char flag) {
        def::model::geoprim::fixgeo(g, static_cast<const FixgeoOptions*>(p), k, mode, flag);
    };
#endif
#ifdef P905_HAVE_GEOLIB
    e.mesh_alloc = [](def::model::LLOBJ_DATA* o, std::int32_t nn, std::int32_t ne, std::int32_t nper, std::int32_t* layout,
                      std::int32_t dim) { def::cmd::geolib::fun_140202700(o, nn, ne, nper, layout, dim); };
#endif
}

} // namespace def::cmd::geoprim
