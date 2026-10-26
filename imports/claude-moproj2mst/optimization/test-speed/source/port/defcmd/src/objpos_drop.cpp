// OBJPOS drop positioning (3D modes 7, 8, 9): case body of the command dispatcher and the drop
// wrapper 1401a7d60. Spec: docs/re/commands/OBJPOS_drop.md §2-4, §7.
#include <defcmd/objpos_drop.hpp>
#include <defcmd/objpos_mat4.hpp>
#include <defcmd/drop/mode8.hpp>
#include <defcmd/drop/solver.hpp>
#include <defcmd/objpos_centre.hpp>
#include <defcmd/objpos_mirror.hpp>

#include <defdbio/objcheck/objcheck.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/misc_model.hpp>
#include <defmodel/geo_kw/surface_model.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/linked/axis_point_list.hpp>
#include <defmodel/node_access.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/ftn/helpers.hpp>

#include <defrt/stdlib/legacy_printf.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace def::cmd::objpos {

namespace {

using model::LLOBJ_DATA;
using i32 = std::int32_t;

template <class T>
T& at(void* base, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<std::byte*>(base) + off);
}

// FUN_1400015f0: the model is 3D (+0x1c == 3)
bool model_is_3d(const void* m)
{
    return *reinterpret_cast<const i32*>(static_cast<const std::byte*>(m) + 0x1c) == 3;
}

model::linked::Cls_140257340List& window_list(LLOBJ_DATA* a)
{
    return at<model::linked::Cls_140257340List>(a, 0x2e80);
}

// 1402582b0 / 140257f70 (plane list max id / remove): ported in defdbio/objcheck/objcheck.hpp
// (plane_list_max_id / plane_list_remove), used through 14016c780 below

} // namespace

// 14016c780: ported in defdbio/objcheck/objcheck.hpp (obj_remove_unused_symmetry_planes): prune
// the BC windows, keep only those referenced by a node BC code 6
void fun_14016c780(LLOBJ_DATA* a) { dbio::objcheck::obj_remove_unused_symmetry_planes(a); }

// @src 140195b00  window count: 140158e70 on +0x18
i32 fun_140195b00(void* windows) { return at<i32>(windows, 0x18 + 0x10); }

// @src 140186bc0  owner (+0x2b8) is 3D and +0x1b8 >= 1e-20 (DAT_140522fa0)
bool fun_140186bc0(void* process)
{
    if (!model_is_3d(at<void*>(process, 0x2b8))) return false;
    const double r = at<double>(process, 0x1b8);
    return 1e-20 < r || 1e-20 == r;
}

// @src 1401a7d60  drop wrapper
bool fun_1401a7d60(model::DB_STEP_DATA* mdl, LLOBJ_DATA* a, double* dir, const char* f8,
                   double* v6, double p15, double* m16, const i32* n, const i32* list)
{
    if (a == nullptr) return false;
    i32 nb = 2;
    i32 nn = 0, nf = 0, nu = 0, nuf = 0, nk = 0, itmax = 200, verbose = 0, status = 1;
    double* x = nullptr;
    i32* faces = nullptr;
    double* xu = nullptr;
    i32* fu = nullptr;
    double g = 0.5; // DAT_14051f240
    double t[3] = {0.0, 0.0, 0.0};
    double r[12] = {};
    // K(6,3): only the slots the branches below set are defined in the original (uninitialised
    // stack otherwise, OBJPOS_drop_solver §3); zero here. Room for windows past the third, which
    // overflow the original's stack array (OBJPOS_drop §4 step 4).
    double k[36] = {};

    bool use_surf;
    if (at<i32>(a, 0xc) == 0) use_surf = model::geo_kw::has_surface_flag(*a);
    else use_surf = at<i32>(a, 0x87c) == 0;
    const bool mesh = !use_surf; // local_2c8
    if (mesh) {
        nn = at<i32>(a, 0x880);
        nf = at<i32>(a, 0xbf0);
        x = model::fun_1401808d0(&at<model::Cls_14017ef20>(a, 0x8a0));
        faces = at<i32*>(a, 0xbf8);
    } else {
        nn = model::geo_kw::surface(*a, 0)->node_count();
        nf = model::geo_kw::surface(*a, 0)->elem_count();
        x = model::geo_kw::geo3d_coords(model::geo_kw::surface(*a, 0));
        faces = model::geo_kw::geo3d_elems(model::geo_kw::surface(*a, 0));
    }
    fun_14016c780(a);

    const bool use_list = !(*n < 1 || list == nullptr);
    const i32 count = use_list ? *n : mdl->u1378;
    model::OBJ_GEO3D u; // FUN_1402f9020 (local_518), destroyed by FUN_1402f91f0 below
    bool any = false;   // local_378
    for (i32 i = 0; i < count; ++i) {
        auto* c = static_cast<LLOBJ_DATA*>(
            model::step_object_at(*mdl, use_list ? list[i] : i + 1));
        if (c == nullptr) continue;
        if (at<i32>(c, 0xc) == 7) continue; // FUN_140160f10
        if (!(model::geo_kw::has_surface_flag(*c) || at<i32>(c, 0x87c) != 0)) continue;
        if (at<i32>(a, 8) == at<i32>(c, 8)) continue;
        bool temp = false;
        if (!model::geo_kw::has_surface_flag(*c)) {
            if (at<i32>(c, 0x87c) == 0) continue;
            char one = 1;
            fun_1401fc730(c, &one);
            temp = true;
        }
        model::OBJ_GEO3D* s = model::geo_kw::surface(*c, 0);
        if (s != nullptr) {
            fun_1402fa880(&u, s);
            if (temp) fun_1401ffed0(c);
            any = true;
        }
    }

    if (any) {
        void* proc = &at<std::byte>(a, 0x1ea0);
        const i32 w = fun_140195b00(&at<std::byte>(a, 0x2e68));
        if (*f8 == 0) {
            if (w == 0) {
                if (!fun_140186bc0(proc) || !node_bc_code_7_or_8(a)) {
                    nk = 0;
                } else {
                    nk = 3;
                    for (int i = 0; i < 3; ++i) {
                        k[12 + i] = at<double>(proc, 0x1c0 + 8 * i);
                        k[15 + i] = at<double>(proc, 0x1d8 + 8 * i);
                    }
                }
            } else {
                i32 m = 0;
                for (i32 i = 0; i < w; ++i) {
                    auto* win = window_list(a).nth(i + 1);
                    const double len = rt::ftn::vec3_length(win->direction);
                    if (1e-12 <= len) { // DAT_14051f260
                        if (m < 6) {
                            for (int j = 0; j < 3; ++j) {
                                k[m * 6 + j] = win->point[j];
                                k[m * 6 + 3 + j] = win->direction[j];
                            }
                        }
                        ++m;
                    }
                }
                nk = m;
            }
        } else {
            double ca[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
            drop::axis_point(a, ca, ca + 3);
            nk = 3;
            for (int i = 0; i < 3; ++i) {
                k[12 + i] = ca[i];
                k[15 + i] = dir[i];
            }
        }
        nu = u.node_count();
        nuf = u.elem_count();
        xu = model::geo_kw::geo3d_coords(&u);
        fu = model::geo_kw::geo3d_elems(&u);
        verbose = 0;
        drop::fun_140359a70(&nb, &nn, &nf, x, faces, &nu, &nuf, xu, fu, dir, &nk, k, v6, &g, &p15,
                            &itmax, &verbose, t, r, &status);
    }
    const bool ok = status == 0;
    if (ok) {
        double T[16], Rm[16], Cm[16], Cp[16];
        fun_140240fc0(T);
        for (int i = 0; i < 3; ++i) T[12 + i] = t[i];
        fun_140240fc0(Rm);
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j) Rm[i * 4 + j] = r[i * 3 + j];
        fun_140240fc0(Cm);
        fun_140240fc0(Cp);
        for (int i = 0; i < 3; ++i) {
            Cm[12 + i] = -r[9 + i]; // DAT_14051bec0 sign mask
            Cp[12 + i] = r[9 + i];
        }
        fun_140240fc0(m16);
        fun_140241110(m16, Cp);
        fun_140241110(m16, Rm);
        fun_140241110(m16, Cm);
        fun_140241110(m16, T);
        if (!mesh) {
            if (at<i32>(a, 0x87c) != 0) fun_1401fe310(a, m16);
        } else {
            if (model::geo_kw::has_surface_flag(*a)) fun_1401fe290(a, m16);
            fun_1401fe3a0(a, m16);
        }
        fun_1401fd730(a, m16);
    }
    return ok;
}

// part of @src 140378e00  OBJPOS 3D cases 6-8 (bm_cmd.c 847-879): modes 7, 8, 9
int drop_mode(model::DB_STEP_DATA& m, model::LLOBJ_DATA& obj, Args& a, State& st)
{
    const double p15 = a.v[0]; // local_5468 = token 2
    double v6[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; // local_5460..5438
    fun_140240fc0(st.drop_m);
    char f8 = 0;
    if (a.mode == 8) {
        f8 = 1;
    } else if (a.mode == 9) {
        v6[0] = a.v[4];
        v6[1] = a.v[5];
        v6[2] = a.v[6];
    }
    const i32 n0 = 0; // local_194
    st.dropped = fun_1401a7d60(&m, &obj, &a.v[1], &f8, v6, p15, st.drop_m, &n0, nullptr);
    if (!st.dropped) def::rt::crt::printf("Failure in drop-positioning object %d.\n", at<i32>(&obj, 8));
    else def::rt::crt::printf("Object %d has been dropped successfully.\n", at<i32>(&obj, 8));
    return 0;
}

} // namespace def::cmd::objpos
