// defcmd/ctc3d/pipeline.hpp: Fortran pipeline FUN_1400512aa / FUN_14005150d and stages 1, 2, 4, 6, 7, GENCTC_3d_search.md sec. 2, sec. 6, sec. 7.7, sec. 8.
// Packet p67-7, group "pipeline". Declarations follow the binary's argument lists (genctc3d.hpp
// explains the conventions). Bodies in src/ctc3d/pipeline/.
#pragma once

// 14005150d: declared and ported in drop/solver.hpp (p67-3).

#include <defcmd/ctc3d/common.hpp>

namespace def::cmd::ctc3d {

// @src 14002cc15  stage 4: EI2 = 0 for valid slave polygons whose nodes share a positive BC code
void fun_14002cc15(i32* obj, i32* inst, const i32* nface, const i32* bc, const i32* ndof);
// @src 14003a580  distinct common BC codes of the nodes conn(1..*n): *count pairs
// out[2*(i-1)] = code, out[2*(i-1)+1] = direction (bc(k, node) = bc[(node-1)*(*ndof) + k-1])
void fun_14003a580(const i32* bc, const i32* conn, const i32* n, const i32* ndof, i32* count,
                   i32* out);
// @src 14003fadc  stage 7: NI7 feature flags of the partners' nodes (dbg goes to MDSPS2_REC)
void fun_14003fadc(i32* obj, i32* inst, const i32* n, const i32* nface, i32* dbg);
// @src 1400501d7  NI7 (1,0) / (2,object) of one partner's nodes (single edges of EI2 < 1 polygons)
void fun_1400501d7(i32* obj, i32* inst, const i32* npts, const i32* nface);
// @src 1400511e4
void fun_1400511e4(std::int32_t* one, const std::int32_t* n, const std::int32_t* partners);
// @src 1400512aa
void fun_1400512aa(i32* obj, i32* one, double* coords, i32* bcc, i32* ndof, i32* one2, i32* nnode, i32* nface, i32* one3, i32* zero, i32* ierr, i32* zero2, double* tol);
// @src 140052f49  stage 2: NI5 feature class of the slave's nodes, critical corner nodes
void fun_140052f49(i32* obj, i32* inst, const i32* n, const i32* bc, const i32* ndof);
// @src 14005c3a7  stage 1: EI3 per-edge BC class of the slave's polygons
void fun_14005c3a7(i32* obj, i32* inst, const i32* nface, const i32* bc, const i32* ndof);
// @src 140080a83  box(6) around the square of half-diagonal *r centred at c(3) in the plane of
// unit normal n(3), widened by 1e-3f
void fun_140080a83(const double* c, const double* n, const double* r, double* box);
// @src 140082ed6
void fun_140082ed6(const std::int32_t* obj, const std::int32_t* one);
// @src 140088e99  stage 6: NR1[4] = 0 for the nodes of partner polygons with EI2 != 1
void fun_140088e99();
// @src 14008a8c1  empty
void fun_14008a8c1();
// @src 14008a8c3  empty
void fun_14008a8c3();
// @src 14008a8c4  empty
void fun_14008a8c4();
// @src 14008b65b  sort the *ncol columns of a(*nrow, *) lexicographically, ascending if *dir == 1
void fun_14008b65b(i32* a, const i32* nrow, const i32* ncol, const i32* dir);
// @src 14008ba00  swap columns *i and *j of a(*nrow, *)
void fun_14008ba00(i32* a, const i32* nrow, const i32* i, const i32* j);
// @src 14008bb26  quicksort columns *lo..*hi of a(*nrow, *) on row *key, descending
void fun_14008bb26(i32* a, const i32* nrow, const i32* key, const i32* lo, const i32* hi);
// @src 14008bd37  twin of 14008bb26
void fun_14008bd37(i32* a, const i32* nrow, const i32* key, const i32* lo, const i32* hi);
// @src 14008bf48  quicksort columns *lo..*hi of a(*nrow, *) on row *key, ascending
void fun_14008bf48(i32* a, const i32* nrow, const i32* key, const i32* lo, const i32* hi);
// @src 14008c159  twin of 14008bf48
void fun_14008c159(i32* a, const i32* nrow, const i32* key, const i32* lo, const i32* hi);

} // namespace def::cmd::ctc3d
