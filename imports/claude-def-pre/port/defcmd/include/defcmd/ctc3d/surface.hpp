// defcmd/ctc3d/surface.hpp: master surface build FUN_14004f56b and its Fortran helpers, GENCTC_3d_search.md sec. 5.
// Packet p67-7, group "surface". Declarations follow the binary's argument lists (genctc3d.hpp
// explains the conventions): every argument by pointer, arrays 1-based in the comments.
//
// Cube attribute fields (Get/Init NI, NR, EI, ER of cube.hpp) written here: ER1 normal, ER2
// area, ER3 dihedral angles, EI1 neighbours + edge classes, EI2 status, EI3 edge BC class
// (0, or 1/2 at disabled polygons), EI4 patch, EI5 curve point lists, EI6 curve links, EI20
// half-edge table (polygon 1), NI1 node -> polygons, NI2 neighbours, NI3 on-surface flag, NI6
// curve ids, NR1 normal + convexity, NR2 nodal area, NR5 cos range.
#pragma once

#include <defcmd/ctc3d/common.hpp>

namespace def::cmd::ctc3d {

// @src 140029d47  NI3 = 1 for every node of a polygon (0 first), EI2 = 1 / 0 (last node < 0) /
// -1000 (area <= 1e-10)
void fun_140029d47(i32* obj, i32* inst, const i32* npts, const i32* npoly);
// @src 14003c506  NI3 = 0 for nodes all of whose polygons have EI2 in {0, -2000}
void fun_14003c506(i32* obj, i32* inst, const i32* npts);
// @src 1400407e7  ER(*fnorm) = unit normal, ER(*farea) = area of every polygon; *flag = 1 when a
// normal is degenerate (npts, zero unused)
void fun_1400407e7(i32* obj, i32* inst, i32* fnorm, i32* farea, const i32* npts,
                   const i32* npoly, const i32* zero, i32* flag);
// @src 14004f56b  master surface of (obj, inst): cube, counts, box, ER1-3, EI1-6, NI1-3, NI6,
// NR1/2/5; x(3, npts), faces(nvert, npoly)
void fun_14004f56b(i32* obj, i32* inst, double* x, i32* faces, i32* nvert, i32* base, i32* npts,
                   i32* npoly, i32* zero, i32* flag);
// @src 14005110a  point / polygon counts, nodes per polygon, bounding box of (obj, inst)
void fun_14005110a(i32* obj, i32* inst, double* x, i32* faces, i32* npts, i32* npoly, i32* nvert);
// @src 14005815c  ININDP: NR1 (area-weighted normal, convexity), NR2 (nodal area), NR5, NI2
// (npoly unused)
void fun_14005815c(i32* obj, i32* inst, const i32* npts, const i32* npoly);
// @src 14005a6bf  remove repeated entries of list(1..*n) in place (first kept, order kept)
void fun_14005a6bf(i32* list, i32* n);
// @src 14005bbf2  edge classes (EI1(nv+k)), EI3 at disabled polygons, curves (14005d848)
void fun_14005bbf2(i32* obj, const i32* npoly, const i32* npts, const i32* nvert, i32* inst,
                   i32* fer3, i32* fei1, i32* table);
// @src 14005d848  FTCURV: curve records, EI5/EI6/NI6, *ncurve, *maxpts
void fun_14005d848(i32* obj, i32* inst, const i32* npts, const i32* npoly, i32* rec,
                   i32* ncurve, i32* maxpts);
// @src 14005fe15  nodes used by != 2 open edges are added to the special-node list aa(2, *)
void fun_14005fe15(i32* obj, i32* inst, const i32* rec, const i32* nrec, const i32* npts,
                   i32* cnt, i32* naa, i32* aa);
// @src 140060038  *found = 1 when *node is one of aa(1, 1..*n)
void fun_140060038(const i32* aa, const i32* n, const i32* node, i32* found);
// @src 1400600bb  one curve from the node *start: points p(6, *), *np points, curve *ncurve
void fun_1400600bb(i32* obj, i32* inst, i32* rec, const i32* nrec, i32* p, i32* ncurve, i32* np,
                   i32* start, i32* aa, i32* naa);
// @src 1400610f2  ER(*fer3) = dihedral angle per polygon edge over the edge table (npts, fer2
// unused)
void fun_1400610f2(i32* obj, const i32* npts, const i32* npoly, i32* inst, i32* fer2,
                   i32* fer3, i32* fei1, i32* table, const i32* nedge);
// @src 140062fff  dihedral angle at the edge (k[0] of xa, k[1] of xb) of two polygons
void fun_140062fff(const double* xa, const double* xb, double* angle, const i32* k,
                   const i32* nv);
// @src 14006f089  out(1..*nout) = distinct entries of a(1..*na) that also occur in b(1..*nb)
void fun_14006f089(const i32* na, const i32* a, const i32* nb, const i32* b, i32* nout, i32* out);
// @src 140075036  area of the triangle (mode 0: vertices 1, 2, 3; mode 1: |conn|)
void fun_140075036(const double* x, double* area, const i32* conn, const i32* mode);
// @src 1400798bf  half-edge table (8 ints per half-edge), reversed half-edges paired
void fun_1400798bf(i32* obj, const i32* npoly, i32* inst, i32* fni1, i32* table, i32* nedge,
                   i32* flag);
// @src 14007aa22  edge table (8 ints per edge) and EI1(nv+k) edge ids
void fun_14007aa22(i32* obj, i32* inst, i32* table, i32* nedge, const i32* npoly, const i32* nvert);
// @src 14007b1ea  patches (EI4) and neighbour lists (EI1) (fei1 unused: EI1 is field 1)
void fun_14007b1ea(i32* obj, i32* inst, i32* fei1, i32* table, const i32* nedge,
                   const i32* npoly, const i32* nvert, i32* e, i32* stack, i32* nb, i32* npatch);
// @src 14007c24c  polygon of patch *patch holding the half-edge (*n2, *n1): *poly (0 none), *k
void fun_14007c24c(i32* obj, i32* inst, i32* n1, i32* n2, const i32* patch, i32* poly, i32* k);
// @src 14007c697  ND2ELM: NI(*field) = ascending list of the valid polygons at each node
void fun_14007c697(i32* obj, i32* inst, i32* field, const i32* npts, const i32* npoly,
                   const i32* zero, i32* flag);
// @src 14007d97c  folding check: EI2 = -1000 for invisible / folded polygons, NR1(4) = 2
// (x unused)
void fun_14007d97c(double* x, const i32* npts, const i32* npoly, i32* obj, i32* inst);
// @src 14007e28d  node -> polygon bins: *get != 0 copies node *node's list to out, else appends *val
void fun_14007e28d(i32* nbin, i32* bins, i32* cnt, const i32* node, const i32* val,
                   const i32* get, i32* out, const i32* m);
// @src 14007e62b  b(1..*m, 1..*n) = a (REAL*8)
void fun_14007e62b(const double* a, double* b, const i32* m, const i32* n);
// @src 14007ea01  signed angle in (0, 2 pi] from the 2D point p1 to p2 (0 for a zero vector)
void fun_14007ea01(const double* p1, const double* p2, double* angle);

} // namespace def::cmd::ctc3d
