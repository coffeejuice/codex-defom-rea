// def_pre/ext_p10_02.hpp: the calls from menus 5-6 (packet p10-02) into code of other packets,
// through seams so that p10-02 builds and tests before those are on its branch.
//
// Each function calls the hook when a test set one, else the owner's code when it is in the tree
// (__has_include), else DEF_NOT_PORTED.
//
//   interference_position  1401a0a10 (p67-2, defcmd/ifd/ifd_internal.hpp ifd::fun_1401a0a10)
//   genctc_3d              1401a6c80 (p67-6, defcmd/contact_cmds.hpp on claude/p67-gate)
//   mesh_rigid_die         the _MeshGenBase sequence of 140002690 (14013e790 ctor, 140148840 bind,
//                          slot +0x08 14013ee00(0), when DAT_1406e2d74 == 1 slot +0x10
//                          140140110(1), 14013e840 dtor; p8-06 / p8-07)
#pragma once

#include <cstdint>

#include <defrt/port.hpp>

#include <defcmd/ifd/ifd_internal.hpp>

#if __has_include(<defcmd/meshgen/meshgen_base.hpp>) && __has_include(<defcmd/meshgen/mg_globals.hpp>)
#include <defcmd/meshgen/meshgen_base.hpp>
#include <defcmd/meshgen/mg_globals.hpp>
#define DEF_P1002_HAS_MESHGEN 1
#endif
#if __has_include(<defcmd/contact_cmds.hpp>)
#include <defcmd/contact_cmds.hpp>
#define DEF_P1002_HAS_GENCTC3D 1
#endif

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
} // namespace def::model

namespace def::pre::ext {

using InterferenceFn = bool (*)(model::DB_STEP_DATA* m, model::LLOBJ_DATA* a, model::LLOBJ_DATA* b,
                                double* dir, double* tol, double* dist, double* disp, char flag,
                                char* one);
using Genctc3dFn = void (*)(model::DB_STEP_DATA* m, double tol);
using MeshRigidDieFn = void (*)(model::LLOBJ_DATA* obj, model::DB_STEP_DATA* m, const char* prob);

struct Hooks1002 {
    InterferenceFn interference = nullptr;
    Genctc3dFn genctc_3d = nullptr;
    MeshRigidDieFn mesh_rigid_die = nullptr;
};
inline Hooks1002 g_hooks1002;

// 1401a0a10(m, a, b, dir, tol, dist, disp, flag, one): the 2D / 3D interference kernel, then on
// success the move of a by disp (14019eb60). Menu 6 passes `one` as a pointer to a byte 0; the
// callee reads it as the char flag of 14021d0b0, so the int pointer only carries its address.
inline bool interference_position(model::DB_STEP_DATA* m, model::LLOBJ_DATA* a, model::LLOBJ_DATA* b,
                                  double* dir, double* tol, double* dist, double* disp, char flag,
                                  char* one)
{
    if (g_hooks1002.interference != nullptr) {
        return g_hooks1002.interference(m, a, b, dir, tol, dist, disp, flag, one);
    }
    // the byte is widened to an int holding its value, so the low byte the callee uses is the same
    std::int32_t one_int = static_cast<unsigned char>(*one);
    return cmd::ifd::fun_1401a0a10(m, a, b, dir, tol, dist, disp, flag, &one_int);
}

// 1401a6c80(m, tol): GENCTC over the objects of a 3D model
inline void genctc_3d(model::DB_STEP_DATA* m, double tol)
{
    if (g_hooks1002.genctc_3d != nullptr) {
        return g_hooks1002.genctc_3d(m, tol);
    }
#if defined(DEF_P1002_HAS_GENCTC3D)
    return cmd::genctc_3d(m, tol);
#else
    (void)m;
    (void)tol;
    DEF_NOT_PORTED(0x1401a6c80);
#endif
}

// the _MeshGenBase part of 140002690 (see the file comment)
inline void mesh_rigid_die(model::LLOBJ_DATA* obj, model::DB_STEP_DATA* m, const char* prob)
{
    if (g_hooks1002.mesh_rigid_die != nullptr) {
        return g_hooks1002.mesh_rigid_die(obj, m, prob);
    }
#if defined(DEF_P1002_HAS_MESHGEN)
    // owners p8-07 (14013e790 ctor, 140148840 bind, slot +0x08 14013ee00, 14013e840 dtor, the
    // state 1406e2d74) and p8-06 (slot +0x10 140140110)
    def::cmd::MeshGenBase mg;                       // 14013e790
    def::cmd::meshgen_bind(mg, obj, m, prob);       // 140148840
    mg.surface_step(0);                             // vtable +0x08: 14013ee00(0)
    if (def::cmd::g_mesher_state == 1) {            // DAT_1406e2d74
        mg.volume_step(1);                          // vtable +0x10: 140140110(1)
    }
#else
    (void)obj;
    (void)m;
    (void)prob;
    DEF_NOT_PORTED(0x14013ee00);
#endif
}

} // namespace def::pre::ext
