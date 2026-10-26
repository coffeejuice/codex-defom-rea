// Drop solver kernels (packet p67-3): projection onto a plane / an axis and the mirror helpers.
// Spec: docs/re/commands/OBJPOS_drop_solver.md §5.3 (projection helpers), §5.12 (mirror).
// The frame T of each routine is a module static that survives between calls (FUN_1404cf5c0
// leaves it unchanged for a zero direction): globals_a.hpp. The local-coordinate buffers
// (0x1407052c0, 0x140705330, 0x1407053a0, 0x140705410) are written by FUN_1404ded0c before they
// are read, so they are locals.
#include <defrt/ftn/geoutl.hpp>
#include <defcmd/drop/globals_a.hpp>
#include <defcmd/drop/kernels.hpp>

namespace def::cmd::drop {

namespace geo = def::rt::geoutl;

// @src 140365a10  c := projection of c onto the plane through p with normal n:
// L = T(c - p); L3 = 0; c = T^T L + p
void fun_140365a10(double* c, double* p, double* n)
{
    geo::fun_1404cf5c0(n, g_t_140705270);
    i32 np = 1;
    i32 mode = 1;
    double l[3];
    geo::fun_1404ded0c(c, l, p, g_t_140705270, &np, &mode);
    mode = 2;
    np = 1;
    l[2] = 0.0;
    geo::fun_1404ded0c(l, c, p, g_t_140705270, &np, &mode);
}

// @src 140365b02  c := projection of c onto the axis through p with direction n (L1 = L2 = 0)
void fun_140365b02(double* c, double* p, double* n)
{
    geo::fun_1404cf5c0(n, g_t_1407052e0);
    i32 np = 1;
    i32 mode = 1;
    double l[3];
    geo::fun_1404ded0c(c, l, p, g_t_1407052e0, &np, &mode);
    np = 1;
    l[0] = 0.0;
    mode = 2;
    l[1] = 0.0;
    geo::fun_1404ded0c(l, c, p, g_t_1407052e0, &np, &mode);
}

// @src 140365bfc  m := rotation of m by pi about the axis (p, n): L = T(m - p); L1 = -L1,
// L2 = -L2 (sign flips, 1407053a8 then 1407053a0); m = T^T L + p. Only m(1:3) is used (np = 1).
void fun_140365bfc(double* m6, double* p, double* n)
{
    geo::fun_1404cf5c0(n, g_t_140705350);
    i32 np = 1;
    i32 mode = 1;
    double l[3];
    geo::fun_1404ded0c(m6, l, p, g_t_140705350, &np, &mode);
    l[1] = -l[1];
    l[0] = -l[0];
    mode = 2;
    np = 1;
    geo::fun_1404ded0c(l, m6, p, g_t_140705350, &np, &mode);
}

// @src 140365d0c  x := mirror image of x in the plane (p, n): L = T(x - p); L3 = -L3; x = T^T L + p
void fun_140365d0c(double* x, double* p, double* n)
{
    geo::fun_1404cf5c0(n, g_t_1407053c0);
    i32 np = 1;
    i32 mode = 1;
    double l[3];
    geo::fun_1404ded0c(x, l, p, g_t_1407053c0, &np, &mode);
    l[2] = -l[2];
    mode = 2;
    np = 1;
    geo::fun_1404ded0c(l, x, p, g_t_1407053c0, &np, &mode);
}

} // namespace def::cmd::drop
