// p9-03b: the DEF_INT state-variable step of CNVT3D (CNVT3D.md 6.3, CNVT3D_SWEEP.md 2.1).
#define _CRT_SECURE_NO_WARNINGS
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include <defcmd/cnvt3d_sweep_b.hpp>
#include <defcmd/geolib.hpp>
#include <defcmd/launch.hpp>
#include <defcmd/objpos_centre.hpp>
#include <defdbio/gendb/gendb.hpp>
#include <defdbio/step/db_info_read.hpp>
#include <defmodel/geo_kw/misc_model.hpp>
#include <defmodel/llobj/accessors.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/llobj_ctor/llobj_copy.hpp>
#include <defmodel/material/state_remap.hpp>

#include "common.hpp"
#include "ext.hpp"

namespace def::cmd::cnvt3d_sweep {

using namespace detail;

namespace {

inline void remove_file(const char* n) { dbio::gendb::remove_if_exists(n); }  // 1401b85d0

// 140159300: std::string::rfind(text) from npos (VS2008 basic_string), as an int
std::int32_t rfind_int(const std::string& s, const char* t)
{
    const std::size_t p = s.rfind(t);
    return static_cast<std::int32_t>(p);  // npos -> -1
}

} // namespace

// @src 140146050  _MeshGenBase method (CNVT3D): DEF_INT on the 2D quad mesh of cvt+0x10
void fun_140146050(MeshGenBase* cvt)
{
    cvt->u3f40 = 1;
    const std::uint32_t mode = 2;
    char msh[1024];
    std::sprintf(msh, "NEW.MSH");
    char act[304];
    std::sprintf(act, "\n5\nY\n1\n%s\n\n6\n\n", msh);
    std::FILE* f = std::fopen("DEF_INT.ACT", "wt");  // 14047b934, mode str 14051dcb4
    if (f == nullptr) return;
    std::fprintf(f, "%s", act);
    std::fclose(f);
    std::FILE* g = std::fopen("DEF_REM.INI", "wt");  // mode str 14051dd24
    if (g == nullptr) return;
    std::fprintf(g, "%s\n", cvt_stem(cvt));
    std::fprintf(g, "%d\n", at<std::int32_t>(cvt_m2(cvt), 0x744));
    std::fprintf(g, "%d\n", mode);
    std::fprintf(g, "%d\n", obj_id(cvt_bound(cvt)));
    std::fclose(g);
    std::string s(cvt_stem(cvt));                                 // 1400066a0
    const std::int32_t dot = rfind_int(s, ".");                   // str 14051ddbc
    s = ext::g.fun_1401b1520(std::string(s), dot);                // 140005dd0, 1401b1520, 1400066f0
    char db[128];
    std::sprintf(db, "%s.DB", s.c_str());
    const std::int32_t slash = rfind_int(s, "/");                 // str 14051ddcc
    if (0 < slash) s = s.substr(static_cast<std::size_t>(slash));  // 140006760 (pos, npos), 1400066f0
    s += "_REM.KEY";                                              // 140006730
    char key[128];
    std::sprintf(key, "%s", s.c_str());
    if (!ext::g.fun_1401563c0(cvt, msh)) {
        remove_file("DEF_INT.ACT");
        remove_file("DEF_REM.INI");
        return;
    }
    remove_file("DEF_INT.OUT");
    ext::g.fun_1402ac8a0(cvt_m2(cvt), db, cvt_bound(cvt));
    char cmd[208];
    std::sprintf(cmd, "%s < %s > %s", "DEF_INT", "DEF_INT.ACT", "DEF_INT.OUT");
    ext::g.fun_14013c6b0("Interpolate SV On 2D Quad Mesh ...", cvt_stem(cvt));
    launch_program_2d(1, cmd);  // 140224100
    remove_file(msh);
    remove_file("DEF_INT_NO_DB.DAT");
    if (!dbio::file_exists(key)) {  // 1401b8560
        ext::g.fun_14013c6b0("Interpolate failed ...", cvt_stem(cvt));
        return;
    }
    ext::g.fun_14013c6b0("Interpolate succeed ...", cvt_stem(cvt));
    const bool ok = fun_140146560(cvt, key);
    remove_file(key);
    if (!ok) ext::g.fun_14013c6b0("Load interpolated data failed ...", cvt_stem(cvt));
    else ext::g.fun_14013c6b0("Load interpolated data succeed ...", cvt_stem(cvt));
}

// @src 140146560  _MeshGenBase method (CNVT3D): read `key_name` into a temporary 2D model with the
// 2D model's simulation control and materials, copy its object (id of cvt+0x10) into cvt+8
bool fun_140146560(MeshGenBase* cvt, const char* key_name)
{
    if (!dbio::file_exists(key_name)) return false;  // 1401b8560
    auto* t = new DB_STEP_DATA(2);                   // 1404797d8(0x3408), 1401c8950
    const std::uint8_t one = 1;
    model::llobj::set_step_u1b5c(t, &one);           // 140158fa0
    DB_STEP_DATA* m2 = cvt_m2(cvt);
    ext::g.fun_1401f3660(&t->sim_ctrl, &m2->sim_ctrl);
    ext::g.fun_14029b630(&t->materials, &m2->materials);
    t->n_materials = m2->n_materials;
    model::step_remap_material_state(*t, false);     // 1401d0fc0(t, 0)
    const std::int32_t opt = 0;
    const char flag = 1;
    ext::g.fun_1401b5f00(t, key_name, &flag, &opt);
    model::llobj_copy::reset_keep_geometry(cvt_obj(cvt));  // 140158f70
    const bool p9 = true, p8 = false, p7 = true, p6 = true, p5 = true, p4 = true;
    LLOBJ_DATA* src = model::object_list_find(&t->objects, obj_id(cvt_bound(cvt)));  // 1401d6ea0
    model::llobj_copy_from(cvt_obj(cvt), src, t->u15f8, &p4, &p5, &p6, &p7, &p8, &p9);  // 1401613a0
    at<std::int32_t>(cvt_obj(cvt), 0xc) = at<std::int32_t>(cvt_bound(cvt), 0xc);
    std::strncpy(cvt_obj(cvt)->name, cvt_bound(cvt)->name, 0x40);
    geolib::fun_140158fc0(t, 1);
    return true;
}

// @src 140148960  _MeshGenBase method (CNVT3D): brick path 2D remesh to `target` elements and
// DEF_INT interpolation, then re-find the object
void fun_140148960(MeshGenBase* cvt, std::int32_t target)
{
    char compact = 1;
    objpos::fun_1401fc730(cvt_obj(cvt), &compact);  // owner p67 (objpos/centre3d.cpp)
    at<std::int32_t>(cvt_obj(cvt), 0x2d80) = target;
    const char flag = 0;
    ext::g.fun_140144a70(cvt, &flag);
    ext::g.fun_14013d7e0(10, &at<std::int32_t>(cvt_m2(cvt), 0x888));
    fun_140146050(cvt);
    cvt_obj(cvt) = model::object_list_find(&cvt_m2(cvt)->objects, obj_id(cvt_bound(cvt)));  // 1401d6ea0
    ext::g.fun_14013d7c0();
}

// @src 140148a20  _MeshGenBase method (CNVT3D): tet path: the 2D object's mesh becomes the tet
// mesh projected to (sqrt(x^2 + y^2), z) (revolve) or (x, y) (extrude), DEF_INT, then 140151400
void fun_140148a20(MeshGenBase* cvt, LLOBJ_DATA* tet)
{
    DB_STEP_DATA* m2 = cvt_m2(cvt);
    if (m2 == nullptr) return;
    const std::int32_t kd = model::geo_kw::simctl_geo_kind(&m2->sim_ctrl);  // 1401ee4c0
    std::vector<double> xy(static_cast<std::size_t>(obj_nn(tet) << 1));
    for (std::int32_t i = 1; i <= obj_nn(tet); ++i) {
        if (kd == 1) {
            const double* X = coords(tet);
            const double x = X[(i - 1) * 3];
            const double y = X[(i - 1) * 3 + 1];
            xy[i * 2 - 2] = std::sqrt(x * x + X[(i - 1) * 3 + 1] * y);  // 140484644
            xy[i * 2 - 1] = coords(tet)[(i - 1) * 3 + 2];
        } else if (kd == 2) {
            xy[i * 2 - 2] = coords(tet)[(i - 1) * 3];
            xy[i * 2 - 1] = coords(tet)[(i - 1) * 3 + 1];
        }
    }
    std::vector<std::int32_t> c4(static_cast<std::size_t>(obj_ne(tet) << 2));
    for (std::int32_t i = 1; i <= obj_ne(tet); ++i)
        for (std::int32_t k = 0; k < 4; ++k) c4[i * 4 - 4 + k] = conn(tet)[i * 4 - 4 + k];
    model::llobj_copy::reset_keep_geometry(cvt_obj(cvt));  // 140158f70
    geolib::fun_140202700(cvt_obj(cvt), obj_nn(tet), obj_ne(tet), 0, m2->u15f8, 2);
    std::memcpy(coords(cvt_obj(cvt)), xy.data(), static_cast<std::size_t>(obj_nn(tet) << 1) << 3);
    std::memcpy(conn(cvt_obj(cvt)), c4.data(), static_cast<std::size_t>(obj_ne(tet) << 2) << 2);
    ext::g.fun_14013d7e0(10, &at<std::int32_t>(m2, 0x888));
    fun_140146050(cvt);
    ext::g.fun_14013d7c0();
    fun_140151400(cvt, tet);
}

} // namespace def::cmd::cnvt3d_sweep
