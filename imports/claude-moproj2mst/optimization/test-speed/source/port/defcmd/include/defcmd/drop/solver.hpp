// defcmd/drop/solver.hpp: the OBJPOS drop settling solver (packet p67-3): set-up 140359a70,
// main routine 14035b861, initial drop 140368889, contact detection 1403704c9 / 1403722fc, and
// the drop variant of the GENCTC 3D pipeline (14005150d, 140049361).
// Spec: docs/re/commands/OBJPOS_drop.md §5, OBJPOS_drop_solver.md §2-4, 5.5, 5.6.
// Fortran calling style: every argument by pointer, the binary's argument order, names
// fun_<address>. Array arguments: see state.hpp.
#pragma once

#include <defcmd/drop/state.hpp>
#include <defcmd/genctc3d.hpp>

namespace def::cmd::drop {

// @src 140359a70  set-up: body tables, direction and constraint checks, then 14035b861
void fun_140359a70(i32* nb, i32* nn, i32* nf, double* x, i32* f, i32* nu, i32* nuf, double* xu,
                   i32* fu, double* dir, i32* nk, double* k, double* v, double* g, double* p15,
                   i32* itmax, i32* verbose, double* t, double* r, i32* status);

// @src 14035b861  settling solver main routine
void fun_14035b861(double* x, double* rt, double* xu, i32* it, i32* f, i32* fu, i32* nodes,
                   i32* nb, i32* nb2, i32* nn, i32* nf2, i32* nf, i32* nn2, i32* nuf, i32* nu,
                   double* pt, double* nrm, double* xe, i32* bc, i32* fid, i32* flags, i32* sw,
                   double* d, i32* nk, double* k, i32* nv, double* v, double* g, double* p15,
                   i32* itmax, i32* verbose, double* t, double* r, i32* status);

// @src 1403704c9  contact detection of the moving body (backup X, engine, restore, gaps, nc)
void fun_1403704c9(double* rt, double* pt, double* x, double* xu, i32* it, i32* f, i32* fu,
                   i32* bc, i32* sw, i32* fid, double* nrm, double* xe, i32* flags, double* vn,
                   double* tolscale, i32* nb, i32* nb2, i32* nn, i32* nodes, double* gap,
                   double* xb, double* w, double* kc, double* damp, double* p15, i32* nc,
                   i32* flag, i32* verbose);

// @src 1403722fc  engine calls: insert / update cubes, search, decide, extract (mode 1 or 2)
void fun_1403722fc(double* rt, double* pt, double* x, double* xu, i32* it, i32* f, i32* fu,
                   i32* bc, i32* mode, i32* sw, i32* fid, double* nrm, double* xe, i32* flags,
                   double* vn, double* tolscale, i32* nb, i32* nb2, i32* flag);

// @src 140368889  initial drop: ray casts along d (and back along -d), rigid translation
void fun_140368889(double* rt, double* pt, double* x, double* xu, i32* it, i32* f, i32* fu,
                   i32* bc, i32* sw, i32* fid, double* nrm, double* xe, i32* flags, double* vn,
                   i32* nb, i32* nb2, i32* nodes, i32* ins, i32* redrop, double* d, double* p15);

// @data 140705b20  NR1 buffer of 140368889 (4 doubles; a node without NR1 keeps the previous
// node's values, also across calls); @data 140705b40  its count
inline double g_nr1_buf[4]{};
inline i32 g_nr1_cnt = 0;

} // namespace def::cmd::drop

namespace def::cmd::ctc3d {

// @src 14005150d  drop variant of the GENCTC 3D pipeline 1400512aa (stages 1-7, plus 140049361)
void fun_14005150d(i32* obj, i32* inst, double* x, i32* bc, i32* ndof, i32* first, i32* nn,
                   i32* nf, i32* p9, i32* p10, i32* p11, i32* flag, double* tol, i32* p14);
// @src 140049361  constraint rows 14004946f for nodes 1..*n
void fun_140049361(double* x, i32* obj, i32* inst, i32* bc, i32* ndof, i32* n, i32* p7, i32* p8,
                   i32* p9);

// @data 1405c76a0  NO_DIEGEO.DAT state: 0 untested, 1 absent, -1 present (search disabled)
inline i32 g_no_diegeo = 0;
// @data 14066fbc0  node counter of 140049361
inline i32 g_node_14066fbc0 = 0;

} // namespace def::cmd::ctc3d
