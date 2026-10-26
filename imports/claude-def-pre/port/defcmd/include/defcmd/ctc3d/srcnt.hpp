// defcmd/ctc3d/srcnt.hpp: SRCNT stage FUN_14003b304 (tolerance, SRCNT2/4/5, partner-partner contact, symmetry, DIEGEO), GENCTC_3d_search.md sec. 7, sec. 11.
// Packet p67-7, group "srcnt". Declarations follow the binary's argument lists (genctc3d.hpp
// explains the conventions). Signatures still marked "provisional" belong to routines handed to packet p67-3
// (SRCNT4 subtree); the others are the binary's argument lists, tightened by the srcnt porter.
#pragma once

#include <defcmd/ctc3d/common.hpp>

namespace def::cmd::ctc3d {

// @src 14002a857
// SYMGRP2: symmetry planes of the slave from its polygons with a common fixed code 1
void fun_14002a857(i32* obj, i32* inst, double* x, i32* nface, i32* bc, i32* ndof, double* pnt, double* nrm, double* ptol, i32* nplane, double* size, i32* n);
// @src 14002b89c
// writes DIE_00k.GEO (unit 81) of one partner: points and polygons; name_len is the hidden
// CHARACTER length (11)
void fun_14002b89c(i32* obj, i32* inst, i32* npts, i32* npoly, char* name, i32* dbgpoly, std::int64_t name_len);
// @src 14002cdee
// SRCNT2 on the partner obj: polygons touching the slave oobj get EI2 = -k (box = slave box)
void fun_14002cdee(i32* obj, i32* inst, i32* npoly, double* tol, double* box, i32* oobj, i32* p7);
// @src 14002de5d
// SRCNT2 subdivision test of polygon *poly of obj against object *kobj; sets *ei2 = -kobj
void fun_14002de5d(i32* obj, i32* inst, i32* poly, i32* kobj, i32* oobj, double* xp, i32* nsub, double* h, double* tol, i32* nsubmax, i32* npmax, i32* ei2);
// @src 14002f091  SRCNT4 (interference); p9 unused (SRCNT's 11th argument)
void fun_14002f091(i32* obj, i32* inst, i32* bcc, i32* ndof, i32* nnode, i32* nface, double* x, double* tol, i32* p9, i32* dbg, i32* dbg_obj, i32* dbg_inst, i32* dbg_poly);
// @src 140030ae1
void fun_140030ae1(double* x, double* box, double* margin, i32* n, i32* idx, i32* mode);
// @src 140030d0e
// SRCNT5 on the slave's polygons: EI2 = 0 / -k from the common contact code
void fun_140030d0e(i32* obj, i32* inst, i32* nface, double* tol, i32* bc, i32* ndof, i32* p7);
// @src 140032a3e
void fun_140032a3e(i32* obj, i32* inst, i32* list, i32* n, i32* found, i32* npoly, i32* flag);
// @src 140032ce4
void fun_140032ce4(i32* obj, i32* inst, i32* list, i32* n, double* box, double* tol);
// @src 14003329f
void fun_14003329f(i32* obj, i32* inst, i32* sobj, i32* sinst, double* x, i32* npoly, i32* nnode, double* margin, double* box, i32* found);
// @src 140033d76
void fun_140033d76(i32* obj, i32* inst, i32* npoly, i32* start, double* dir, i32* value, i32* flag);
// @src 140034138
void fun_140034138(i32* obj, i32* inst, i32* npoly, i32* sobj, i32* sinst, i32* nnode, double* dir, i32* value, i32* flag, i32* bcc, i32* ndof);
// @src 140039ef9
void fun_140039ef9(i32* obj, i32* inst, i32* poly, i32* stack, i32* cnt, i32* flag, double* dir, i32* mode);
// @src 14003a4be
void fun_14003a4be(i32* a, i32* b, i32* found);
// @src 14003b0c6
// diagonal |max - min| of a box(6)
void fun_14003b0c6(const double* box, double* diag);
// @src 14003b130
// *flag = 1 when a partner polygon has a negative last node
void fun_14003b130(i32* obj, i32* flag);
// @src 14003b304
// SRCNT stage (GENCTC_3d_search.md sec. 7)
void fun_14003b304(double* x, i32* obj, i32* inst, i32* n, i32* nface, i32* bc, i32* ndof, i32* p8, double* tol, i32* p10, i32* p11);
// @src 14003c742
// debug print (list-directed on unit *unit) of a polygon: NBCDL of its nodes, ICURVE, IVISB
void fun_14003c742(i32* obj, i32* inst, i32* poly, i32* unit);
// @src 14003cd08
// DIEGEO.DAT: DIE_00k.GEO for every partner k
void fun_14003cd08(i32* obj, i32* inst, i32* dbgpoly);
// @src 14003d2ee
// symmetry planes: EI2 = 0 for the polygons of obj lying in one of the *nplane planes
void fun_14003d2ee(i32* obj, i32* inst, i32* npoly, double* pnt, double* nrm, double* ptol, i32* nplane, double* size);
// @src 14003dabe
// symmetry die (DAT_14076abb8 planes; no plane is ever defined in DEF_PRE)
void fun_14003dabe(i32* obj, i32* inst, i32* npoly, double* pnt, double* nrm, i32* nplane, i32* pobj, i32* flag, double* tol, double* box);
// @src 14003e7f6
// partner-partner contact: points of a within *tol of b get code -b in NI4
void fun_14003e7f6(i32* a, i32* b, i32* ainst, i32* binst, i32* an, i32* bnpoly, double* tol, i32* p8);
// @src 14005d44f
// EI3 (edge classes) of obj's polygons recomputed from the neighbours' EI2
void fun_14005d44f(i32* obj, i32* inst, i32* npoly);

} // namespace def::cmd::ctc3d
