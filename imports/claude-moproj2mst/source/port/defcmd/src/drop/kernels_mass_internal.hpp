// drop/kernels_mass_internal.hpp: internal callees of the mass-property routine 140362db9
// (packet p67-3). Spec: docs/re/commands/OBJPOS_drop_solver.md §5.3.1-5.3.4.
// Binary argument order, all by pointer. Bodies in kernels_mass.cpp.
#pragma once

#include <defcmd/drop/state.hpp>

namespace def::cmd::drop {

// @src 140364241  face vector of one face (nodes = its npf node numbers) into out(3)
void fun_140364241(double* x, i32* nodes, double* out, i32* npf);
// @src 1403649bb  *s = sum over triangles f_first..f_first+nf-1 of area * integrand(case)
void fun_1403649bb(double* x, i32* faces, i32* f_first, i32* nf, i32* npf, double* fv, double* s,
                   i32* kase);
// @src 140364e60  *s = sum over quads (one Gauss point) of J * integrand(case)
void fun_140364e60(double* x, i32* faces, i32* f_first, i32* nf, i32* npf, double* s, i32* kase);
// @src 1403653fe  integrand of case 1..10 at (x, y, z) with unit normal n -> *f
void fun_1403653fe(double* x, double* y, double* z, double* f, double* n, i32* kase);
// @src 1403660b9  8-node brick shape functions N(8) and derivatives dN(3,8) at (xi, eta, zeta)
void fun_1403660b9(double* xi, double* eta, double* zeta, double* n, double* dn);
// @src 140366542  first fundamental form (E, F, G) of a quad at (xi, eta), zeta = 1
void fun_140366542(i32* fidx, double* xi, double* eta, i32* nodes, double* x, double* e, double* fm,
                   double* g);
// @src 1403666cb  tangents T(3,2) = sum_m X(:,node m) * dN(j, m+4), j = 1 (xi), 2 (eta)
void fun_1403666cb(i32* fidx, i32* nodes, double* x, double* dn, double* t);

} // namespace def::cmd::drop
