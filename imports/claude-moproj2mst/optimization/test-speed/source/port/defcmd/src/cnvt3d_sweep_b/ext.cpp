// Defaults of the p9-03b ext table (see ext.hpp).
#include <defcmd/step_dat.hpp>
#include "ext.hpp"

#include <defcmd/meshgen/p8_seams.hpp>
#include <defrt/port.hpp>

#if __has_include(<defcmd/cnvt3d_sweep.hpp>)
#include <defcmd/cnvt3d_sweep.hpp>
#define P903B_HAVE_A 1
#else
#define P903B_HAVE_A 0
#endif

// Owners merged on the p9/p10 integration branch
#if __has_include("../meshgen/mg_internal.hpp")
#include "../meshgen/mg_internal.hpp"   // p8-07: 140155340 geo23_clear, 140144a70 mesh2d_run
#define P903B_HAVE_P8_07 1
#endif
#if __has_include(<defcmd/interp.hpp>)
#include <defcmd/interp.hpp>            // p8-08: 1401563c0, 1401f3660, 14029b630, 140180a40
#define P903B_HAVE_P8_08 1
#endif
#if __has_include(<defcmd/key_file.hpp>)
#include <defcmd/key_file.hpp>          // 1401b5f00 read_keyword_file
#define P903B_HAVE_KEY_FILE 1
#endif
#if __has_include(<defcmd/cnvt3d.hpp>)
#include <defcmd/cnvt3d.hpp>            // p9-01: 14013c6b0 log_message
#define P903B_HAVE_P9_01 1
#endif

namespace def::cmd::cnvt3d_sweep::ext {

namespace {

// 1402ac8a0: owner p9-ac8a0 (defcmd/step_dat.hpp)
void d_1402ac8a0(DB_STEP_DATA* m, const char* db, LLOBJ_DATA* o) { ::def::cmd::step_dat::write_step_dat(m, db, o); }

#if P903B_HAVE_A
void d_14013d7e0(std::int32_t n, const std::int32_t* v) { ::def::cmd::cnvt3d_sweep::fun_14013d7e0(n, v); }
void d_14013d7c0() { ::def::cmd::cnvt3d_sweep::fun_14013d7c0(); }
std::int32_t d_140212460(LLOBJ_DATA* o, std::int32_t i) { return ::def::cmd::cnvt3d_sweep::fun_140212460(o, i); }
#else
void d_14013d7e0(std::int32_t, const std::int32_t*) { DEF_NOT_PORTED(0x14013d7e0); }
void d_14013d7c0() { DEF_NOT_PORTED(0x14013d7c0); }
std::int32_t d_140212460(LLOBJ_DATA*, std::int32_t) { DEF_NOT_PORTED(0x140212460); }
#endif

#ifdef P903B_HAVE_P8_07
void d_140155340(MeshGenBase* cvt, LLOBJ_DATA* o, double c) { ::def::cmd::geo23_clear(*cvt, o, c); }
void d_140144a70(MeshGenBase* cvt, const char* f) { (void)::def::cmd::mesh2d_run(*cvt, f); }
#else
void d_140155340(MeshGenBase*, LLOBJ_DATA*, double) { DEF_NOT_PORTED(0x140155340); }
void d_140144a70(MeshGenBase*, const char*) { DEF_NOT_PORTED(0x140144a70); }
#endif
#ifdef P903B_HAVE_KEY_FILE
std::int32_t d_1401b5f00(DB_STEP_DATA* m, const char* file, const char* flag, const std::int32_t* opt)
{
    return ::def::cmd::read_keyword_file(m, file, flag, opt);
}
#else
std::int32_t d_1401b5f00(DB_STEP_DATA*, const char*, const char*, const std::int32_t*) { DEF_NOT_PORTED(0x1401b5f00); }
#endif
#ifdef P903B_HAVE_P8_08
bool d_1401563c0(MeshGenBase* cvt, const char* name) { return ::def::cmd::write_mesh_2d(*cvt, name); }
void d_1401f3660(void* d, const void* s)
{
    ::def::cmd::sim_ctrl_copy(static_cast<model::Cls_1401e7b10*>(d), static_cast<const model::Cls_1401e7b10*>(s));
}
void d_14029b630(void* d, const void* s)
{
    ::def::cmd::material_list_copy(static_cast<model::MaterialList*>(d),
                                   static_cast<model::MaterialList*>(const_cast<void*>(s)));
}
double* d_140180a40(void* set) { return ::def::cmd::node_set_array3(static_cast<model::Cls_14017ef20*>(set)); }
#else
bool d_1401563c0(MeshGenBase*, const char*) { DEF_NOT_PORTED(0x1401563c0); }
void d_1401f3660(void*, const void*) { DEF_NOT_PORTED(0x1401f3660); }
void d_14029b630(void*, const void*) { DEF_NOT_PORTED(0x14029b630); }
double* d_140180a40(void*) { DEF_NOT_PORTED(0x140180a40); }
#endif
std::string d_1401b1520(std::string s, int n) { return ::def::cmd::fun_1401b1520(std::move(s), n); }
#ifdef P903B_HAVE_P9_01
void d_14013c6b0(const char* msg, const char* stem) { ::def::cmd::cnvt3d::log_message(msg, stem); }
#else
void d_14013c6b0(const char*, const char*) { DEF_NOT_PORTED(0x14013c6b0); }
#endif

Table defaults()
{
    return Table{&d_14013d7e0, &d_14013d7c0, &d_1402ac8a0, &d_140212460, &d_140155340, &d_140144a70,
                 &d_1401b5f00, &d_1401563c0, &d_1401f3660, &d_14029b630, &d_140180a40, &d_1401b1520,
                 &d_14013c6b0};
}

} // namespace

Table g = defaults();

void reset_ext() { g = defaults(); }

} // namespace def::cmd::cnvt3d_sweep::ext
