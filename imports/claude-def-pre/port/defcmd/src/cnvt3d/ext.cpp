// Calls of packet p9-01 into other packets (defcmd/cnvt3d.hpp, Cnvt3dExt). Each default forwards
// to the owner's function when the owner's header is in the build, else stops in DEF_NOT_PORTED.
#include <defcmd/cnvt3d.hpp>

#include <string>

#include <defcmd/launch.hpp>
#include <defcmd/meshgen/p8_seams.hpp>
#include <defmodel/geoprim/fixgeo.hpp>
#include <defrt/port.hpp>

// p9-03: the mesh build 140147b70
#if __has_include(<defcmd/cnvt3d_build.hpp>)
#include <defcmd/cnvt3d_build.hpp>
#define P901_HAVE_P9_03 1
#endif
// p5-01: the KEY writer 1401c5810 (write_key_file)
#if __has_include(<defcmd/dispatch.hpp>)
#include <defcmd/dispatch.hpp>
#define P901_HAVE_P5_01 1
#endif
// p8-08: simulation-control parts, material and phase-pair list copies (defcmd/interp.hpp)
#if __has_include(<defcmd/interp.hpp>)
#include <defcmd/interp.hpp>
#define P901_HAVE_P8_08 1
#endif
// p8-07: 1402062b0 (src/meshgen/meshgen_3d.cpp surface_has_bc_codes)
#if __has_include("../meshgen/mg_internal.hpp")
#include "../meshgen/mg_internal.hpp"
#define P901_HAVE_P8_07 1
#endif
// p9-04: the GEOPRIM builder 1402ba500
#if __has_include(<defcmd/geoprim/builder.hpp>)
#include <defcmd/geoprim/builder.hpp>
#define P901_HAVE_P9_04 1
#endif

namespace def::cmd::cnvt3d {

namespace {

void d_generate(MeshGenBase& cvt, std::int32_t id, std::int32_t nsec, std::int32_t m, double start,
                double ang, double c, std::int32_t kind)
{
#ifdef P901_HAVE_P9_03
    fun_140147b70(cvt, id, nsec, m, start, ang, c, kind);
#else
    (void)cvt, (void)id, (void)nsec, (void)m, (void)start, (void)ang, (void)c, (void)kind;
    DEF_NOT_PORTED(0x140147b70);
#endif
}

void d_exchange_cleanup(MeshGenBase& cvt, bool flag) { def::cmd::exchange_cleanup(cvt, flag); }

void d_str_left(std::string& out, const std::string& s, std::int32_t n) { out = fun_1401b1520(s, n); }

#ifdef P901_HAVE_P8_07
bool d_has_patch_ids(LLOBJ_DATA* o) { return def::cmd::surface_has_bc_codes(o); }
#else
bool d_has_patch_ids(LLOBJ_DATA*) { DEF_NOT_PORTED(0x1402062b0); }
#endif

std::int32_t d_read_nbcd(MeshGenBase& cvt) { return fun_140141580(cvt); }

#ifdef P901_HAVE_P8_08
void d_sim_a(model::Cls_1401e7b10* d, model::Cls_1401e7b10* s) { def::cmd::sim_ctrl_copy_names(d, s); }
void d_sim_b(model::Cls_1401e7b10* d, model::Cls_1401e7b10* s) { def::cmd::sim_ctrl_copy_steps(d, s); }
void d_sim_c(model::Cls_1401e7b10* d, model::Cls_1401e7b10* s) { def::cmd::sim_ctrl_copy_environment(d, s); }
void d_material_copy(void* d, void* s)
{
    def::cmd::material_list_copy(static_cast<model::MaterialList*>(d), static_cast<model::MaterialList*>(s));
}
void d_phase_copy(void* d, void* s)
{
    def::cmd::phase_pair_list_copy(static_cast<model::PhasePairList*>(d), static_cast<model::PhasePairList*>(s));
}
#else
void d_sim_a(model::Cls_1401e7b10*, model::Cls_1401e7b10*) { DEF_NOT_PORTED(0x1401e8040); }
void d_sim_b(model::Cls_1401e7b10*, model::Cls_1401e7b10*) { DEF_NOT_PORTED(0x1401e84f0); }
void d_sim_c(model::Cls_1401e7b10*, model::Cls_1401e7b10*) { DEF_NOT_PORTED(0x1401ea900); }
void d_material_copy(void*, void*) { DEF_NOT_PORTED(0x14029b630); }
void d_phase_copy(void*, void*) { DEF_NOT_PORTED(0x140339d30); }
#endif

std::int32_t d_write_key(DB_STEP_DATA* m, const char* name, const std::uint32_t* flags)
{
#ifdef P901_HAVE_P5_01
    return def::cmd::write_key_file(m, name, flags);
#else
    (void)m, (void)name, (void)flags;
    DEF_NOT_PORTED(0x1401c5810);
#endif
}

bool d_build_primitive(void* prim, LLOBJ_DATA* o3, const char* write_key, DB_STEP_DATA* model3)
{
#ifdef P901_HAVE_P9_04
    return geoprim::fun_1402ba500(static_cast<model::geoprim::DEF_GEOPRIM*>(prim), o3, write_key, model3,
                                  nullptr);
#else
    (void)prim, (void)o3, (void)write_key, (void)model3;
    DEF_NOT_PORTED(0x1402ba500);
#endif
}

void d_window_geo3d_assign(model::LLWINDEF_DATA*, model::OBJ_GEO3D*) { DEF_NOT_PORTED(0x1402f9b60); }

Cnvt3dExt make_default()
{
    Cnvt3dExt e{};
    e.generate = d_generate;
    e.exchange_cleanup = d_exchange_cleanup;
    e.str_left = d_str_left;
    e.has_patch_ids = d_has_patch_ids;
    e.read_nbcd = d_read_nbcd;
    e.sim_ctrl_copy_a = d_sim_a;
    e.sim_ctrl_copy_b = d_sim_b;
    e.sim_ctrl_copy_c = d_sim_c;
    e.material_list_copy = d_material_copy;
    e.phase_pair_copy = d_phase_copy;
    e.write_key = d_write_key;
    e.build_primitive = d_build_primitive;
    e.window_geo3d_assign = d_window_geo3d_assign;
    return e;
}

} // namespace

Cnvt3dExt& ext()
{
    static Cnvt3dExt e = make_default();
    return e;
}

void reset_ext() { ext() = make_default(); }

void bind_fixgeo_launch() { model::geoprim::g_fixgeo_launch = &launch_program_2d; }

} // namespace def::cmd::cnvt3d
