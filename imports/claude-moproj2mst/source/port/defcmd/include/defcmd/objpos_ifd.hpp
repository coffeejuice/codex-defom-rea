// defcmd/objpos_ifd.hpp: OBJPOS interference positioning, modes 2, 12, 102, 112 (packet p67-2).
// Spec: docs/re/commands/OBJPOS_interference.md, OBJPOS_interference_kernels.md.
//
// Split with p67-1 (OBJPOS frame, include/defcmd/objpos.hpp): p67-1's case-7 frame reads the
// grammar, checks the moving object and switches on the mode; for 3D cases 1/0xb/0x65/0x6f and the
// matching 2D cases it calls interference_mode(), then runs the shared post-switch code (STROKE
// update, coupled loop) with st.moved and g_last_disp (DAT_140706bb0). Mode 14 sub 0 calls
// fun_14019fa00 directly.
#pragma once

#include <cstdint>

#if __has_include(<defcmd/objpos.hpp>)
#include <defcmd/objpos.hpp>
#define DEF_P672_HAVE_OBJPOS 1
#endif

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
} // namespace def::model

namespace def::cmd::objpos {

#ifndef DEF_P672_HAVE_OBJPOS
// Interim copies of p67-1's types (objpos.hpp), used until that header is on this branch.
struct Args {
    int obj_no;
    int mode;
    double v[8];
    int n_coupled;
    int coupled[100];
};
struct State {
    bool moved, rotated, mirrored, dropped;
    double drop_m[16];
};
// 140706bb0: OBJPOS displacement G is g_last_disp in defcmd/objpos.hpp (p67-1 owns it)
#endif

// part of @src 140378e00  the body of OBJPOS 3D cases 1/0xb/0x65/0x6f (modes 2, 12, 102, 112)
// and of the matching 2D cases (spec OBJPOS_interference.md section 4, steps 2-5, and section 6).
// `obj` is the moving object (already looked up and checked by the frame). Prints its own
// messages. Returns 0 to go on to the post-switch code (st.moved and g_last_disp set) or 3 when
// the command returns 3 ("Data Error").
int interference_mode(model::DB_STEP_DATA& m, model::LLOBJ_DATA& obj, Args& a, State& st);

// @src 14019fa00  router: path branch (14019fb50) when the mover and its coupled objects move on
// an arbitrary path, else 1401a0300. Binary argument order: model, mover, target, direction (3
// doubles), &tol, &dist (out), G (out, 3 doubles), flag, &ncoupled, coupled list, &timeChanged
// (out), &one.
bool fun_14019fa00(model::DB_STEP_DATA* m, model::LLOBJ_DATA* a, model::LLOBJ_DATA* ref,
                   double* dir, double* p, double* out5cc8, double* G, char flag, int* nc,
                   int* list, char* tchg, int* one);

} // namespace def::cmd::objpos
