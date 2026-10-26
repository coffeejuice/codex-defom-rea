// defcmd/ctc3d/decide.hpp: per-node decision and write-back FUN_14005190e, GENCTC_3d_decide.md.
// Packet p67-7, group "decide". Fortran routines: every argument by pointer, arrays 1-based in
// the comments; bc = BCCDEF (ndof codes per node), x = node coordinates (3 per node). All locals
// of these routines are SAVE statics in the image (common.hpp).
#pragma once

#include <defcmd/ctc3d/common.hpp>

namespace def::cmd::ctc3d {

// @src 14003a978  common BC code of the nodes conn(1..*n) (body in src/ctc3d/pipeline/bccode.cpp)
void fun_14003a978(const i32* bcc, const i32* conn, const i32* n, const i32* ndof, i32* code, i32* dir);
// @src 14003abfd  code of node list(1) shared by all of list(2..n): *code (and *dir for code 1)
void fun_14003abfd(i32* bc, i32* list, i32* n, i32* ndof, i32* code, i32* dir);
// @src 140040d77  decision: new contacts of node *node with the partner objects; codes(3) and
// pn(3) out, *nacc = number of accepted partners, *sep = object to skip
void fun_140040d77(double* p, i32* obj, i32* inst, i32* bc, i32* ndof, i32* node, i32* dbg,
                   i32* flag, double* tol, double* pn, i32* codes, i32* mode, i32* nacc, i32* sep);
// @src 1400446b4  corner hit -> edge of a polygon around the corner node (k in/out, poly in/out,
// conn(4) out, ni7a(2), ni7b(2) out)
void fun_1400446b4(i32* obj, i32* inst, i32* poly, i32* conn, i32* k, i32* ni7a, i32* ni7b);
// @src 14004522c  contact polygon switch for a node with two contact codes (*changed = 1)
void fun_14004522c(i32* obj, i32* inst, double* x, i32* bcrow, i32* node, i32* changed, double* tol);
// @src 140046471  common NI4 code of the nodes of polygon *poly of (obj, inst)
void fun_140046471(i32* obj, i32* inst, i32* poly, i32* code);
// @src 140046625  drop the accepted contacts with object *evict (slots compacted)
void fun_140046625(i32* evict, i32* ni4, i32* codes, i32* slot_dir, i32* nacc, double* q);
// @src 140046a93  *sg = +1 when ER1(a)·ER1(b) >= -0.9f, else -1
void fun_140046a93(i32* obj_a, i32* inst_a, i32* poly_a, i32* obj_b, i32* inst_b, i32* poly_b, double* sg);
// @src 1400483ed  contact normal: ER1(poly) for poly >= 0, node NR1(-poly) otherwise
void fun_1400483ed(i32* obj, i32* inst, double* nrm, i32* poly);
// @src 14004946f
void fun_14004946f(double* x, i32* obj, i32* inst, i32* bc, i32* ndof, i32* node, i32* p7, i32* p8, i32* p9);
// @src 140049b2d  existing contacts of the node: re-projection, release, object change
void fun_140049b2d(double* x, i32* obj, i32* inst, i32* bc, i32* ndof, i32* node, double* p,
                   double* len, i32* dbg, i32* flag, i32* sep);
// @src 14004ad46  separation / change of the contact object of direction *dir
void fun_14004ad46(i32* p, i32* e, i32* obj, i32* inst, double* x, double* q, double* lc, i32* poly,
                   i32* bc, i32* ndof, i32* dir, i32* node, double* len, i32* sep);
// @src 14004bc99  corner -> edge through a neighbour feature node (second attempt)
void fun_14004bc99(i32* obj, i32* inst, i32* node, i32* k, i32* poly, i32* conn);
// @src 14004c429  pairs of equal negative codes in c(3)
void fun_14004c429(i32* c, i32* obj, i32* ndup, i32* r, i32* pos);
// @src 14004c564  second direction with the same contact object
void fun_14004c564(i32* obj, i32* inst, i32* p, i32* e, i32* rdup, i32* r, double* lc, i32* ni4,
                   double* nr4);
// @src 14004ce7a  contact normal from the node's polygons whose common code is -*p
void fun_14004ce7a(i32* obj, i32* inst, i32* bc, i32* ndof, i32* p, i32* list, i32* cnt, double* nrm);
// @src 14004e10c  node normal of a constrained node (NI5 classes 1 and 3)
void fun_14004e10c(i32* obj, i32* inst, i32* node, i32* c);
// @src 14005190e
void fun_14005190e(i32* obj, i32* inst, double* x, i32* bc, i32* ndof, i32* node, i32* move, i32* mode, i32* dbg, i32* flag, double* tol);
// @src 14005266d  final correction of the projected point for fixed / symmetry directions
void fun_14005266d(i32* obj, i32* inst, double* x, double* pn, i32* codes, i32* node);
// @src 14005ee88  CRVPLY: polygon and edge around corner node *node along the curve lists
void fun_14005ee88(i32* obj, i32* inst, i32* node, i32* crv, i32* poly, i32* edge);

} // namespace def::cmd::ctc3d
