// defcmd/drop/mode8.hpp: the mode-8 axis point of the OBJPOS drop (packet p67-3): the centroid and
// the symmetry axis of an object's closed surface, from the surface mass-property routine
// 1403e8510 (Fortran) and its callees. Spec: docs/re/commands/OBJPOS_drop_mode8.md (and
// OBJPOS_geo_kernels.md §2 for the integration rules).
//
// Fortran routines keep the binary's argument list (everything by pointer); arrays are
// column-major, 1-based in the comments: X(3,nP) points, F(4,nF) faces (node numbers),
// A(ld,*) matrices.
#pragma once

#include <cstdint>

namespace def::model {
class LLOBJ_DATA;
} // namespace def::model

namespace def::cmd::drop {

// ---- driver (mode8_axis.cpp) ----

// @src 1401fe7d0  centroid c and symmetry axis a of the surface of obj (geometry surface 0, or a
// temporary surface built from the mesh when the object has a mesh and no surface; p67-1
// helpers 1401fc730 / 1401ffed0, defcmd/objpos_centre.hpp). false when
// the object has neither, when surface 0 is missing, or when no axis was found (a = 0 then).
bool axis_point(model::LLOBJ_DATA* obj, double* c, double* a);

// @src 1403e8510  surface mass properties: c = volume centroid, a = the principal axis whose
// moment differs from the two others (err = 0), else a = 0 and err = 1. *dbg > 0 selects
// debug WRITEs (not ported; the only caller passes 0).
void mass_properties(std::int32_t* np, std::int32_t* nf, double* x, std::int32_t* f, double* c,
                     double* a, std::int32_t* err, std::int32_t* dbg);

// ---- integrals (mode8_massprop.cpp) ----

// @src 1403ea82d  per-face normals and areas, then the volume integrals k = 1..10 into
// g_massprop (mode 0; modes 1..3 not ported, unreachable). p9 is unused in mode 0.
void surface_integrals(double* x, std::int32_t* f, std::int32_t* first, std::int32_t* nf,
                       std::int32_t* type, double* nrm, double* area, std::int32_t* mode,
                       double* p9, std::int32_t* dbg, double* gp, double* gw);
// @src 1403ec3cc  unit normal n(3) and area of one face fc(type)
void face_normal(double* x, std::int32_t* fc, double* n, double* area, std::int32_t* type);
// @src 1403ed5af  2x2 Gauss rule of integrand *k over faces first..first+nf-1 (bilinear faces)
void quad_rule(double* x, std::int32_t* f, std::int32_t* first, std::int32_t* nf,
               std::int32_t* type, double* sum, std::int32_t* k, double* gp, double* gw,
               double* nrm, double* area);
// @src 1403ed06d  3-point triangle rule of integrand *k over faces first..first+nf-1
void tri_rule(double* x, std::int32_t* f, std::int32_t* first, std::int32_t* nf,
              std::int32_t* type, double* nrm, double* sum, std::int32_t* k);
// @src 1403edb4f  integrand *k (1..10) at (x, y, z) with normal n: *out = F.n
void integrand(double* x, double* y, double* z, double* out, double* n, std::int32_t* k);
// @src 1403ee73e  8-node brick shape functions N(8) and derivatives dN(3,8) at (xi, eta, zeta)
void brick_shape(double* xi, double* eta, double* zeta, double* nf, double* dn);
// @src 1403eed50  tangents t(3,2) of a 4-node face fc from dN(3,8) (face nodes = brick 5..8)
void face_tangents(std::int32_t* face_no, std::int32_t* fc, double* x, double* dn, double* t);
// @src 1403eebc7  first fundamental form E, F, G of face fc at (xi, eta)
void face_metric(std::int32_t* face_no, double* xi, double* eta, std::int32_t* fc, double* x,
                 double* e, double* f, double* g);
// @src 1403ee538  Gauss abscissae gp(4,4) and weights gw(4,4) (column = rule order)
void gauss_tables(double* gp, double* gw);

// ---- linear algebra (mode8_linalg.cpp) ----

// @src 1403f0b89  Jacobi eigenvalues d(n) and eigenvectors v(np,n) of the symmetric a(np,n)
// (cyclic Jacobi, at most 50 sweeps; a's upper triangle is destroyed); *nrot = rotations
void jacobi_eigen(double* a, std::int32_t* n, std::int32_t* np, double* d, double* v,
                  std::int32_t* nrot);
// @src 1403eef06  c = a x b
void cross3(double* a, double* b, double* c);
// @src 1403f0590  *r = sum_{i=1..n} a(i)*b(i), from 0.0
void dot_n(double* a, double* b, double* r, std::int32_t* n);

} // namespace def::cmd::drop
