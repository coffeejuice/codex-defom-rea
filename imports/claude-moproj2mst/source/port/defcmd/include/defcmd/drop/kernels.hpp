// defcmd/drop/kernels.hpp: numerical kernels of the OBJPOS drop solver (packet p67-3):
// vector helpers, mass properties, node velocities, 6x6 solve, stiffness, rigid move.
// Spec: docs/re/commands/OBJPOS_drop_solver.md §5.1-5.4, 5.7, 5.10, 5.13 (move), 5.15.
// Fortran calling style: every argument by pointer, the binary's argument order; names
// fun_<address>. Bodies in src/drop/kernels_*.cpp.
#pragma once

#include <defcmd/drop/state.hpp>

namespace def::cmd::drop {

// ---- small helpers (§5.15) ----
// @src 140367f0b  *out = dot(a, b) over *n components: s = 0.0; s = a_k*b_k + s
void fun_140367f0b(double* a, double* b, double* out, i32* n);
// @src 140366881  c = a x b
void fun_140366881(double* a, double* b, double* c);
// @src 140367df1  y = A x for an n x n column-major A, sums from 0.0
void fun_140367df1(double* A, double* x, double* y, i32* n);
// @src 140367c86  C = A B (n x n, column-major), sums from 0.0
void fun_140367c86(double* A, double* B, double* C, i32* n);
// @src 140375025  copy *n doubles: b = a
void fun_140375025(double* a, double* b, i32* n);
// @src 1403735bd  zero *n ints
void fun_1403735bd(i32* a, i32* n);

// ---- set-up (§5.1, §5.2) ----
// @src 140365e0b  Gauss-Legendre table into g_gauss_p / g_gauss_w
void fun_140365e0b();
// @src 1403750cf  characteristic length of face group(s) -> *len
void fun_1403750cf(double* x, i32* faces, i32* npf, i32* ngroups, double* len);

// ---- mass properties (§5.3) ----
// @src 140362db9  mass, centre and inertia of the moving body into g_body; constraints nk, K
void fun_140362db9(double* x, i32* faces, i32* f_first, i32* f_count, i32* npf, double* fv,
                   i32* nk, double* k);
// @src 140365a10  project point c onto the plane (p, n)
void fun_140365a10(double* c, double* p, double* n);
// @src 140365b02  project point c onto the axis (p, n)
void fun_140365b02(double* c, double* p, double* n);
// @src 140365bfc  mirror helper of the equilibrium test (§5.12, nk = 1)
void fun_140365bfc(double* m6, double* p, double* n);
// @src 140365d0c  mirror a point in the plane (p, n) (§5.12, nk = 1)
void fun_140365d0c(double* x, double* p, double* n);

// ---- kinematics and dynamics ----
// @src 1403668de  node velocities VN(3,n) from g_body.v, g_body.w about g_body.c (§5.4)
void fun_1403668de(double* x, double* vn, i32* n);
// @src 140366ac3  solve A x = b in place (Gauss, row pivoting); *err = 1 when singular (§5.10)
void fun_140366ac3(double* a, double* b, i32* n, i32* err);
// @src 140367fe2  contact stiffness and damping (§5.7)
void fun_140367fe2(double* k, double* damp, double* w, double* p15);
// @src 140367448  rigid move of n points: rotate by ang about from, translate to `to`;
// track > 0 also moves g_body.a and g_body.ct (§5.13)
void fun_140367448(double* x, double* from, double* to, double* ang, i32* n, i32* track);

} // namespace def::cmd::drop
