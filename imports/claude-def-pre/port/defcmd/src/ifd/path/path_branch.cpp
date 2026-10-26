// The "arbitrary path" branch of OBJPOS interference positioning: the all-path test the router
// uses (14019ec30), the path search of one object (14019ffc0) and the branch driver (14019fb50).
// Spec: docs/re/commands/OBJPOS_interference.md sections 4.1, 8.1, 8.2;
// OBJPOS_interference_kernels.md section 5 (backup / restore).
#include <defcmd/ifd/path.hpp>

#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj_ctor/llobj_copy.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defrt/ftn/helpers.hpp>
#include <defrt/port.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include <cstddef>
#include <cstdio>
#include <memory>

namespace def::cmd::ifd {

namespace path {

bool (*g_kernel_hook)(DB_STEP_DATA*, LLOBJ_DATA*, LLOBJ_DATA*, double*, double*, double*, double*,
                      char) = nullptr;
void (*g_copy_hook)(LLOBJ_DATA*, const LLOBJ_DATA*) = nullptr;

namespace {

std::int32_t kind(const LLOBJ_DATA* o)
{
    return *reinterpret_cast<const std::int32_t*>(reinterpret_cast<const std::byte*>(o) + 0xc);
}

// FUN_140484ab8: CRT abs (abs(INT_MIN) == INT_MIN)
std::int32_t iabs(std::int32_t v)
{
    return v < 0 ? static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(v)) : v;
}

// 14017aa80(model+0x1380, n): n-th object of the list
LLOBJ_DATA* nth_object(DB_STEP_DATA* m, std::int32_t n) { return m->objects.at(n); }

bool kernel(DB_STEP_DATA* m, LLOBJ_DATA* a, LLOBJ_DATA* b, double* dir, double* tol, double* dist,
            double* disp, char flag)
{
    if (g_kernel_hook != nullptr) return g_kernel_hook(m, a, b, dir, tol, dist, disp, flag);
    return fun_1401a0b00(m, a, b, dir, tol, dist, disp, flag);
}

// 1401613a0(dst, src, model+0x15f8, &1, &1, &1, &1, &0, &1) (flag order from the listing's stack
// stores: p4 = local_23, p5..p9 = local_24..local_28)
void copy_object(DB_STEP_DATA* m, LLOBJ_DATA* dst, const LLOBJ_DATA* src)
{
    if (g_copy_hook != nullptr) return g_copy_hook(dst, src);
    const bool p4 = true, p5 = true, p6 = true, p7 = true, p8 = false, p9 = true;
    model::llobj_copy_from(dst, src, m->u15f8, &p4, &p5, &p6, &p7, &p8, &p9);
}

} // namespace

// @src 14019ffc0  path search of one object. Pa / Pb are cleared once before the loop (2D paths
// fill 2 components). The kernel runs with flag 0. Compares from the listing: segment taken when
// !(0.0 >= len) (COMISD + JC: a NaN length is taken), contact accepted when d <= len (ordered).
bool fun_14019ffc0(DB_STEP_DATA* m, LLOBJ_DATA* x, LLOBJ_DATA* b, double* tol, double* d,
                   double* disp, double* tout)
{
    const char flag = 0;
    if (!movement_on_path(&x->s2160)) return false;
    DYNARY curve;                             // 14012d050
    movement_path_curve(&x->s2160, &curve);   // 14021ee40
    bool found = false;
    double ta = 0.0;
    double tb = 0.0;
    double pa[3] = {0.0, 0.0, 0.0};
    double pb[3] = {0.0, 0.0, 0.0};
    double seg[3] = {0.0, 0.0, 0.0};
    double len = 0.0;
    const std::int32_t npts = curve.n[0];
    for (std::int32_t k = 0; k < npts - 1; k = k + 1) {
        ta = curve.axis[0][k];
        tb = curve.axis[0][k + 1];
        path_point(&curve, &k, pa);
        const std::int32_t k1 = k + 1;
        path_point(&curve, &k1, pb);
        rt::ftn::vec3_sub(pb, pa, seg);       // 14023b1f0
        len = rt::ftn::vec3_normalize(seg);   // 14023b720
        if (!(0.0 >= len)) {
            place_on_path(x, &ta);            // 1402007d0
            const bool ok = kernel(m, x, b, seg, tol, d, disp, flag); // 1401a0b00
            if (ok && *d <= len) {
                *tout = ta + (*d / len) * (tb - ta);
                place_on_path(x, tout);
                found = true;
                break;
            }
        }
    }
    return found;
    // curve destroyed here (14012f410)
}

} // namespace path

// @src 14019ec30  the mover and every coupled object found (n-th in the list, abs id) and not
// empty move on a non-zero arbitrary path
bool fun_14019ec30(DB_STEP_DATA* m, LLOBJ_DATA* a, std::int32_t* n, std::int32_t* list)
{
    if (!path::movement_on_path(&a->s2160)) return false;
    for (std::int32_t i = 0; i < *n; i = i + 1) {
        LLOBJ_DATA* c = path::nth_object(m, path::iabs(list[i]));
        if (c != nullptr && path::kind(c) != 7 && !path::movement_on_path(&c->s2160)) {
            return false;
        }
    }
    return true;
}

// @src 14019fb50  path branch: back up A, search A's contact time along its path; on failure
// restore A and return 0; else take the minimum with the coupled objects' times (each found and
// not empty object whose search succeeds), set the model time and return 1. dir, dist, disp and
// flag are not used: d and disp are locals here.
bool fun_14019fb50(DB_STEP_DATA* m, LLOBJ_DATA* a, LLOBJ_DATA* b, double* dir, double* tol,
                   double* dist, double* disp, char flag, std::int32_t* n, std::int32_t* list)
{
    (void)dir;
    (void)dist;
    (void)disp;
    (void)flag;
    bool ok = true;
    double d = 0.0;
    double tc_disp[4] = {0.0, 0.0, 0.0, 0.0}; // local_3fa8: [0] coupled time, [1..3] disp
    double tnew = 0.0;
    const double t0 = sim_time(&m->sim_ctrl);
    rt::crt::fprintf(stdout, "  Arbitrary path has been detected.\n");
    rt::crt::fprintf(stdout, "  Current TNow is %lf.\n", t0);
    // 14015ec80(local, 0), 14015ef60(local, model): a 16 KB stack object in the original
    auto backup = std::make_unique<LLOBJ_DATA>(nullptr);
    model::llobj_ctor::set_owner(backup.get(), m);
    path::copy_object(m, backup.get(), a);
    ok = path::fun_14019ffc0(m, a, b, tol, &d, tc_disp + 1, &tnew);
    if (!ok) {
        rt::crt::fprintf(stdout, "  Failed in interference positioning\n");
        path::copy_object(m, a, backup.get());
        return false; // backup destroyed (14015f790)
    }
    for (std::int32_t i = 0; i < *n; i = i + 1) {
        LLOBJ_DATA* c = path::nth_object(m, path::iabs(list[i]));
        if (c == nullptr) continue;
        if (path::kind(c) == 7) continue; // 140160f10
        ok = path::fun_14019ffc0(m, c, b, tol, &d, tc_disp + 1, tc_disp);
        if (ok && tnew > tc_disp[0]) {
            tnew = tc_disp[0];
        }
    }
    rt::crt::fprintf(stdout, "  Calculated new time = %lf\n", tnew);
    set_sim_time(&m->sim_ctrl, &tnew);
    return true; // backup destroyed (14015f790)
}

} // namespace def::cmd::ifd

