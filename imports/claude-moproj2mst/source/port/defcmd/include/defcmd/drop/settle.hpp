// defcmd/drop/settle.hpp: equilibrium test 140369c19 and rigid update / step search 14036d3fd of
// the OBJPOS drop settling solver (packet p67-3).
// Spec: docs/re/commands/OBJPOS_drop_solver.md §5.12, §5.13; corrections from the listing in
// docs/re/commands/OBJPOS_drop_settle_notes.md.
// Fortran calling style: every argument by pointer, the binary's argument order (read at the call
// sites 0x14035f0ec, 0x140361402, 0x140361839 and in the callees' stack use), names fun_<address>.
// Bodies in src/drop/settle_eqm.cpp and src/drop/settle_step.cpp.
#pragma once

#include <defcmd/drop/state.hpp>

namespace def::cmd::drop {

// @src 140369c19  equilibrium test: *stop = 1 when the contact set holds the body, *shift = 1
// when no contact supports the weight (the body is then moved by ((d + Fm) * p15) * 10).
// Used: x (arg 3), bc (8), nrm (11), nn (17), gap (19), d (21), p15 (25), stop (26), nk (27),
// k (28), l840 (29), shift (30), verbose (31); the others are passed through by the caller only.
void fun_140369c19(double* rt, double* pt, double* x, double* xu, i32* it, i32* f, i32* fu,
                   i32* bc, i32* sw, i32* fid, double* nrm, double* xe, i32* flags, double* vn,
                   i32* nb, i32* nb2, i32* nn, i32* nodes, double* gap, double* xb, double* d,
                   double* w, double* kc, double* damp, double* p15, i32* stop, i32* nk,
                   double* k, i32* l840, i32* shift, i32* verbose);

// @src 14036d3fd  rigid update: axis of the contact set, step size from the angular velocity,
// trial moves with contact detection, bisection on the penetration of fresh contacts, then the
// accepted rigid move of x (and of the tracked points). No contact: initial drop 140368889.
// ea0, db0, dd0 (args 28-30) are passed by the caller and never touched.
void fun_14036d3fd(double* rt, double* pt, double* x, double* xu, i32* it, i32* f, i32* fu,
                   i32* bc, i32* sw, i32* fid, double* nrm, double* xe, i32* flags, double* vn,
                   double* h, i32* nb, i32* nb2, i32* nn, i32* nodes, i32* ins, double* gap,
                   double* xb, double* d, double* w, double* kc, double* damp, double* p15,
                   double* ea0, double* db0, double* dd0, i32* nk, double* k, i32* l840,
                   i32* l890, i32* l8e0, i32* nv, double* v, double* g, i32* redrop,
                   i32* verbose);

// ---- lead's routines (defined in src/drop/detect.cpp, src/drop/initial_drop.cpp) ----
// Argument lists read at the calls 0x14036f14d (28 args) and 0x14036d5ea (21 args).

// @src 1403704c9  contact detection of the moving body
void fun_1403704c9(double* rt, double* pt, double* x, double* xu, i32* it, i32* f, i32* fu,
                   i32* bc, i32* sw, i32* fid, double* nrm, double* xe, i32* flags, double* vn,
                   double* tolscale, i32* nb, i32* nb2, i32* nn, i32* nodes, double* gap,
                   double* xb, double* w, double* kc, double* damp, double* p15, i32* nc,
                   i32* flag, i32* verbose);

// @src 140368889  initial drop
void fun_140368889(double* rt, double* pt, double* x, double* xu, i32* it, i32* f, i32* fu,
                   i32* bc, i32* sw, i32* fid, double* nrm, double* xe, i32* flags, double* vn,
                   i32* nb, i32* nb2, i32* nodes, i32* ins, i32* redrop, double* d, double* p15);

} // namespace def::cmd::drop
