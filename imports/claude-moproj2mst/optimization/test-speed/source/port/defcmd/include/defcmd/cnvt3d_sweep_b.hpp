// defcmd/cnvt3d_sweep_b.hpp: CNVT3D in-process brick sweep, 3D mesh build, state-variable
// transfer and the DEF_INT step (packet p9-03, half b). Spec: docs/re/commands/CNVT3D_SWEEP.md
// (sections 2.1, 3-6), CNVT3D.md 6.3. The other half (entry 140147b70, geometry builders) is
// declared in defcmd/cnvt3d_sweep.hpp, same namespace; the two headers merge mechanically.
//
// Names are fun_<address>, arguments in the binary's order. `cvt` is the CNVT3D converter, a
// _MeshGenBase (defcmd/meshgen/meshgen_base.hpp): +0x08 current (2D) object, +0x10 bound copy,
// +0x3f40 interpolation mode, +0x3f48 2D model, +0x3f50 new 3D model, +0x4360 log stem,
// +0x4770 GEO23 c (double).
#pragma once

#include <cstdint>
#include <list>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
} // namespace def::model

namespace def::cmd {
class MeshGenBase;
}

namespace def::cmd::cnvt3d_sweep {

using model::DB_STEP_DATA;
using model::LLOBJ_DATA;

// ---- DEF_INT step (CNVT3D.md 6.3) ----------------------------------------------------------
// @src 140146050  DEF_INT.ACT, DEF_REM.INI, NEW.MSH, STEP.DAT; launch DEF_INT; reload <stem>_REM.KEY
void fun_140146050(MeshGenBase* cvt);
// @src 140146560  read the interpolated KEY into a temporary 2D model, copy its object into cvt+8
bool fun_140146560(MeshGenBase* cvt, const char* key_name);
// @src 140148960  brick path: 2D remesh to `target` elements, then DEF_INT interpolation
void fun_140148960(MeshGenBase* cvt, std::int32_t target);
// @src 140148a20  tet path: project the 3D mesh to 2D, DEF_INT on it, copy back (140151400)
void fun_140148a20(MeshGenBase* cvt, LLOBJ_DATA* tet);
// @src 140151400  tet path: 3D object from `tet`, state variables from the interpolated 2D object
void fun_140151400(MeshGenBase* cvt, LLOBJ_DATA* tet);

// ---- sweep and 3D mesh build (CNVT3D_SWEEP.md 3-6) ------------------------------------------
// @src 140148f70  revolve sweep (NBCD.OUT when ang < 360)
void fun_140148f70(MeshGenBase* cvt, std::int32_t nsec, double start, double ang, double c,
                   LLOBJ_DATA* obj);
// @src 14014c0a0  extrude sweep
void fun_14014c0a0(MeshGenBase* cvt, std::int32_t nsec, double start, double len, LLOBJ_DATA* obj);
// @src 14014c730  compaction of the swept arrays (lists by value, as the binary copies them)
void fun_14014c730(MeshGenBase* cvt, LLOBJ_DATA* obj, char no_axis, double th0, double dth,
                   std::int32_t n, double* P, std::int32_t* E, std::int32_t* axis,
                   std::list<std::int32_t> keep, std::list<std::int32_t> unused,
                   std::list<std::int32_t> node2d, std::list<std::int32_t> elem2d);
// @src 14014d730  3D object + state variables (revolve)
void fun_14014d730(MeshGenBase* cvt, std::int32_t nn3, std::int32_t ne3, double* x3,
                   std::int32_t* c3, std::int32_t a, double th0, double dth, std::int32_t* n2,
                   std::int32_t* e2);
// @src 14014fae0  3D object + state variables (extrude)
void fun_14014fae0(MeshGenBase* cvt, std::int32_t nn3, std::int32_t ne3, double* x,
                   std::int32_t* c, LLOBJ_DATA* obj, double dz);

// ---- model helpers --------------------------------------------------------------------------
// @src 140161fd0  number of entries of the SV id table 1405caa80 (0x2e)
std::int32_t fun_140161fd0(const LLOBJ_DATA* obj);
// @src 1401627e0  state-variable array `id` of the object; *nodal = node (1) / element (0) array
double* fun_1401627e0(LLOBJ_DATA* obj, std::int32_t id, bool* nodal);
// @src 1401d1660  recompute the model's SV widths (1401d0320), remap object `id`'s arrays
void fun_1401d1660(DB_STEP_DATA* m, std::int32_t id);
// @src 1401d72e0  model+0x86c[i] (STNOUT counts) for 0 <= i <= 6, else 0
std::int32_t fun_1401d72e0(DB_STEP_DATA* m, std::int32_t i);
// @src 1401cbc20  node name array (+0x33a0)
char** fun_1401cbc20(DB_STEP_DATA* m);
// @src 1401cbce0  element name array (+0x33b0)
char** fun_1401cbce0(DB_STEP_DATA* m);
// @src 1401cbd40  strncpy(node name *i, s, 0x40)
void fun_1401cbd40(DB_STEP_DATA* m, const std::int32_t* i, const char* s);
// @src 1401cbd90  strncpy(element name *i, s, 0x40)
void fun_1401cbd90(DB_STEP_DATA* m, const std::int32_t* i, const char* s);

// ---- state-variable accessors of the node set (obj+0x8a0) and element set (obj+0x9e0) --------
// Each allocates the variable for the set's rows, marks it in the owner model, returns the slot.
double* fun_140180ac0(void* set);  // id 0x04, +0x28
double* fun_140180b40(void* set);  // id 0x05, +0x30
double* fun_140180bc0(void* set);  // id 0x06, +0x38
double* fun_140180d30(void* set);  // id 0x09, +0x48
double* fun_140180db0(void* set);  // id 0x0a, +0x50
double* fun_140180e30(void* set);  // id 0x0b, +0x58
double* fun_140180eb0(void* set);  // id 0x0c, +0x60
double* fun_140181010(void* set);  // id 0x0e, +0x70
double* fun_140181110(void* set);  // id 0x10, +0x80
double* fun_140181220(void* set);  // id 0x19, +0x90
double* fun_1401813a0(void* set);  // id 0x1a, +0xa8
double* fun_140184270(void* set);  // id 0x30, +0x70
double* fun_140184370(void* set);  // id 0x32, +0x80
double* fun_140184400(void* set);  // id 0x33, +0x88
double* fun_140184490(void* set);  // id 0x34, +0x90
double* fun_140184760(void* set);  // id 0x39, +0xb8
double* fun_1401847f0(void* set);  // id 0x3a, +0xc0
double* fun_140184880(void* set);  // id 0x3b, +0xc8
double* fun_140184910(void* set);  // id 0x3e, +0xd0
// Accessors of the same family ported by kw-g2sv (defmodel/statevar_kw.hpp, svkw::*); these
// names forward to them (no @src here):
double* fun_1401815b0(void* set);                    // id 0x11 USRNOD, +0xc8
double* fun_140181190(void* set, const char* mark);  // id 0x18, +0x88 (mark when *mark)
double* fun_1401812a0(void* set);                    // id 0x1c, +0xa0
double* fun_140181420(void* set, const char* skip);  // id 0x1d, +0xb0 (mark when !*skip)
double* fun_1401840d0(void* set);                    // id 0x2d DAMAGE, +0x58
double* fun_140184150(void* set, const char* mark);  // id 0x2e MATAXI, +0x60 (mark when *mark)
double* fun_140184520(void* set);                    // id 0x35, +0x98
double* fun_1401845b0(void* set);                    // id 0x36, +0xa0
double* fun_140184640(void* set);                    // id 0x37, +0xa8
double* fun_140184a30(void* set);                    // id 0x3c USRELM, +0xe0
double* fun_1401849a0(void* set);                    // id 0x3f, +0xd8

} // namespace def::cmd::cnvt3d_sweep
