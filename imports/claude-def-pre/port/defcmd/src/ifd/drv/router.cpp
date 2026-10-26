// Interference router and direction/coupled-minimum logic (packet p67-2): 14019fa00, 1401a0300,
// 14018cd10 and the thin wrappers 1401a0a10, 1401a3a00, 1401a0bd0, 14019eb60, 1401a0b00.
// Spec: docs/re/commands/OBJPOS_interference.md sections 4.1, 4.2, 9.3.
#include <cmath>
#include <cstdint>

#include <defcmd/ifd/ifd2d.hpp>
#include <defcmd/objpos_ifd.hpp>
#include <defdbio/objcheck/objcheck.hpp>
#include <defmodel/geo_kw/curve_model.hpp>
#include <defmodel/layout/cls_14017a6c0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defrt/rdata.hpp>

#include "ifd3d.hpp"
#include "vec_interim.hpp"

namespace def::model {
bool llobj_not_kind7(const LLOBJ_DATA* o);  // 140160f10 (port/defmodel/src/material/state_remap.cpp)
}

namespace def::cmd::ifd {

namespace {

template <class T, class C>
T& at(const C* base, std::size_t off)
{
    return *reinterpret_cast<T*>(const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(base)) + off);
}

bool is_2d(const DB_STEP_DATA* m) { return at<std::int32_t>(m, 0x1c) == 2; }  // 1400015e0
bool is_3d(const DB_STEP_DATA* m) { return at<std::int32_t>(m, 0x1c) == 3; }  // 1400015f0

// rdata 14051f260  1e-12 (zero-direction tests)
constexpr double kZeroDir = def::rt::rdata::d_14051f260;

} // namespace

// 14018cd10: ported in defdbio/objcheck/objcheck.hpp (movement_direction_set): the MOVCTL
// direction is not exactly 0.0 (UCOMISD with DAT_14051bed8 = 0.0, so a NaN component counts as
// present)
bool movctl_dir_present(void* mv) { return dbio::objcheck::movement_direction_set(mv); }

// @src 1401a0b00  kernel switch: 2D 1401a0c70, 3D 1401a3aa0; moves nothing
bool fun_1401a0b00(DB_STEP_DATA* m, LLOBJ_DATA* a, LLOBJ_DATA* b, double* dir, double* tol,
                   double* dist, double* disp, char flag)
{
    if (!is_2d(m)) return fun_1401a3aa0(m, a, b, dir, tol, dist, disp, flag);
    return fun_1401a0c70(m, a, b, dir, tol, dist, disp, flag);
}

// @src 14019eb60  move the mover by disp when both are given (14021d0b0 with flag 1)
static void fun_14019eb60(DB_STEP_DATA* /*m*/, LLOBJ_DATA* a, double* disp, std::int32_t* one)
{
    if (a != nullptr && disp != nullptr) {
        char c = 1;
        ext::translate(a, disp, one, &c);
    }
}

// @src 1401a3a00  3D kernel, then move the mover on success
static bool fun_1401a3a00(DB_STEP_DATA* m, LLOBJ_DATA* a, LLOBJ_DATA* b, double* dir, double* tol,
                   double* dist, double* disp, char flag, std::int32_t* one)
{
    const bool ok = fun_1401a3aa0(m, a, b, dir, tol, dist, disp, flag);
    if (ok) fun_14019eb60(m, a, disp, one);
    return ok;
}

// @src 1401a0bd0  2D kernel, then move the mover on success
static bool fun_1401a0bd0(DB_STEP_DATA* m, LLOBJ_DATA* a, LLOBJ_DATA* b, double* dir, double* tol,
                   double* dist, double* disp, char flag, std::int32_t* one)
{
    const bool ok = fun_1401a0c70(m, a, b, dir, tol, dist, disp, flag);
    if (ok) fun_14019eb60(m, a, disp, one);
    return ok;
}

// @src 1401a0a10  2D / 3D kernel with the move
bool fun_1401a0a10(DB_STEP_DATA* m, LLOBJ_DATA* a, LLOBJ_DATA* b, double* dir, double* tol,
                   double* dist, double* disp, char flag, std::int32_t* one)
{
    if (!is_2d(m)) return fun_1401a3a00(m, a, b, dir, tol, dist, disp, flag, one);
    return fun_1401a0bd0(m, a, b, dir, tol, dist, disp, flag, one);
}

// @src 1401a0300  direction choice (token direction or the mover's MOVCTL direction), then the
// kernel; with coupled objects the smallest signed distance of the mover and the coupled objects
// decides, and the mover is moved by that displacement (spec 4.2).
static bool fun_1401a0300(DB_STEP_DATA* m, LLOBJ_DATA* a, LLOBJ_DATA* b, double* dir, double* tol,
                          double* dist, double* disp, char flag, std::int32_t* nc,
                          std::int32_t* list, std::int32_t* one)
{
    double zero[3] = {0.0, 0.0, 0.0};
    double dvec[3] = {0.0, 0.0, 0.0};
    bool zero_dir = false;
    if (!is_2d(m) || !model::geo_kw::points_near(dir, zero, kZeroDir)) {  // 140308d60
        if (iv::near3(dir, zero, kZeroDir)) zero_dir = true;           // 14023ace0
    } else {
        zero_dir = true;
    }
    if (!zero_dir) {
        dvec[0] = dir[0];
        dvec[1] = dir[1];
        if (is_3d(m)) dvec[2] = dir[2];
    } else {
        if (!movctl_dir_present(&at<unsigned char>(a, 0x2160))) return false;
        dvec[0] = at<double>(a, 0x21b0);
        dvec[1] = at<double>(a, 0x21b8);
        if (is_3d(m)) dvec[2] = at<double>(a, 0x21c0);
    }
    if (*nc == 0 || list == nullptr)
        return fun_1401a0a10(m, a, b, dvec, tol, dist, disp, flag, one);
    double dc = 0.0;
    double dispc[3] = {0.0, 0.0, 0.0};
    double dm = 0.0;
    double dispm[3] = {0.0, 0.0, 0.0};
    if (!fun_1401a0b00(m, a, b, dvec, tol, &dm, dispm, flag)) return false;
    if (iv::dot(dvec, dispm) < 0.0) dm = -dm;  // xor with the sign bit (14051bec0)
    for (std::int32_t i = 0; i < *nc; ++i) {
        double dcv[3] = {0.0, 0.0, 0.0};
        if (0 < list[i]) {
            const std::int32_t id = list[i] < 0 ? -list[i] : list[i];  // 140484ab8
            LLOBJ_DATA* c = m->objects.at(id);                         // 14017aa80: n-th object
            if (c != nullptr && model::llobj_not_kind7(c)) {
                if (!zero_dir || !movctl_dir_present(&at<unsigned char>(c, 0x2160))) {
                    dcv[0] = dvec[0];
                    dcv[1] = dvec[1];
                    if (is_3d(m)) dcv[2] = dvec[2];
                } else {
                    dcv[0] = at<double>(c, 0x21b0);
                    dcv[1] = at<double>(c, 0x21b8);
                    if (is_3d(m)) dcv[2] = at<double>(c, 0x21c0);
                }
                if (fun_1401a0b00(m, c, b, dcv, tol, &dc, dispc, flag)) {
                    if (iv::dot(dcv, dispc) < 0.0) dc = -dc;
                    if (dc < dm) {
                        dm = dc;
                        if (!zero_dir || !movctl_dir_present(&at<unsigned char>(a, 0x2160))) {
                            for (int k = 0; k < 3; ++k) dispm[k] = dispc[k];
                        } else {
                            ext::move_along_movctl(&at<unsigned char>(a, 0x2160), 0.0, std::fabs(dm), dispm);
                        }
                    }
                }
            }
        }
    }
    *dist = dm;
    *dist = std::fabs(*dist);  // 140484558
    for (int k = 0; k < 3; ++k) disp[k] = dispm[k];
    char c1 = 1;
    ext::translate(a, disp, one, &c1);
    return true;
}

} // namespace def::cmd::ifd

namespace def::cmd::objpos {

// @src 14019fa00  spec 4.1
bool fun_14019fa00(model::DB_STEP_DATA* m, model::LLOBJ_DATA* a, model::LLOBJ_DATA* ref,
                   double* dir, double* p, double* out5cc8, double* G, char flag, int* nc,
                   int* list, char* tchg, int* one)
{
    *tchg = 0;
    if (!ifd::fun_14019ec30(m, a, nc, list))
        return ifd::fun_1401a0300(m, a, ref, dir, p, out5cc8, G, flag, nc, list, one);
    *tchg = 1;
    return ifd::fun_14019fb50(m, a, ref, dir, p, out5cc8, G, flag, nc, list);
}

} // namespace def::cmd::objpos
