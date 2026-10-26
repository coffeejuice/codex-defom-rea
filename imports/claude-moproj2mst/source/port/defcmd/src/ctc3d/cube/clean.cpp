// cfnCUBE_Clean_ObjEntity (packet p67-7, group "cube").
#include <defcmd/ctc3d/cube.hpp>

#include <cstdio>

namespace def::cmd::ctc3d {

// @src 140225cd0  cfnCUBE_Clean_ObjEntity: every record of (obj, inst): forget the aliased
// point / polygon arrays, free the result list, the attribute lists and the locator cells
void fun_140225cd0(i32* obj, i32* inst, i32* verbose)
{
    using namespace def::cmd::cube;
    const i32 v = *verbose;
    const i32 o = *obj;
    const i32 e = *inst;
    if (v != 0) {
        std::fprintf(stdout, "cfnCUBE_Clean_ObjEntity : Entering function\n"); // str 1405df328
    }
    for (i32 i = 0; i < g_cube_used; ++i) {
        if (g_cubes[i].id == o && g_cubes[i].sub == e) {
            Cube* c = &g_cubes[i];
            c->points.cap = 0;
            c->points.count = 0;
            c->points.data = nullptr;
            c->polys.count = 0;
            c->polys.cap = 0;
            c->polys.data = nullptr;
            fun_140405150(&c->result);
            fun_1404047a0(&c->a180);
            fun_1404047a0(&c->a190);
            fun_1403feec0(&c->loc);
        }
    }
    if (v != 0) {
        std::fprintf(stdout, "...leaving function\n"); // str 1405df358
    }
}

} // namespace def::cmd::ctc3d
