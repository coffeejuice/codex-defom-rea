// defcmd/objpos_drop.hpp: OBJPOS drop positioning, 3D modes 7, 8, 9 (packet p67-3).
// The OBJPOS frame (packet p67-1, defcmd/objpos.hpp) calls drop_mode() for these modes.
// Spec: docs/re/commands/OBJPOS_drop.md, OBJPOS_drop_solver.md.
#pragma once

#include <cstdint>

#if __has_include(<defcmd/objpos.hpp>)
#include <defcmd/objpos.hpp>
#else
namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
} // namespace def::model
namespace def::cmd::objpos {
// Shapes agreed with p67-1 (its objpos.hpp replaces these when it lands).
struct Args {
    int obj_no;
    int mode;
    double v[8]; // tokens 2..9 of the OBJPOS line
    int n_coupled;
    int coupled[100];
};
struct State {
    bool moved, rotated, mirrored, dropped;
    double drop_m[16]; // local_5628
};
} // namespace def::cmd::objpos
#endif

namespace def::model {
class OBJ_GEO3D;
}

namespace def::cmd::objpos {

// part of @src 140378e00  OBJPOS cases 6, 7, 8 of the 3D branch (modes 7, 8, 9): build the drop
// arguments, run the drop, print the result. Returns 0 (the frame goes on to post-processing);
// st.dropped and st.drop_m are set for the coupled-object loop.
int drop_mode(model::DB_STEP_DATA& m, model::LLOBJ_DATA& obj, Args& a, State& st);

// @src 1401a7d60  drop wrapper: moving data, fixed surface union, constraints, solver, matrix,
// apply to the object. `dir` is normalised in place by the solver set-up.
bool fun_1401a7d60(model::DB_STEP_DATA* model, model::LLOBJ_DATA* a, double* dir,
                   const char* f8, double* v6, double p15, double* m16, const std::int32_t* n,
                   const std::int32_t* list);

// 14016c780 (ported in defdbio/objcheck/objcheck.hpp, obj_remove_unused_symmetry_planes)  3D: drop the BC windows of the object that no node BC code 6 refers to
void fun_14016c780(model::LLOBJ_DATA* a);
// @src 140195b00  number of BC windows (count of the list at sub-object + 0x18)
std::int32_t fun_140195b00(void* windows);
// @src 140186bc0  rotational symmetry: the owner model is 3D and value +0x1b8 >= 1e-20
bool fun_140186bc0(void* process);

} // namespace def::cmd::objpos
