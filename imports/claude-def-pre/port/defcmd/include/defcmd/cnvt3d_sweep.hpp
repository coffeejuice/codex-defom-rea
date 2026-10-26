// defcmd/cnvt3d_sweep.hpp: packet p9-03 (CNVT3D mesh build and sweep), split in two halves:
//   p9-03a  entry 140147b70, geometry builders 140153710 / 140153910 / 140153c00 / 140154270,
//           message log 14013c340, DEF_INT_3DTET.DAT, STEP.DAT writer 1402ac8a0, the digitized
//           profile clean-up (1402778c0 -> 14041e270, PNTGEO.GEO), the quad split 14023d410 and
//           the small getters / ostream helpers of the packet list.
//   p9-03b  in-process sweep, state transfer and DEF_INT (defcmd/cnvt3d_sweep_b.hpp, included below).
// Specs: docs/re/commands/CNVT3D_SWEEP.md, CNVT3D.md, common.md.
//
// cvt is the CNVT3D converter (_MeshGenBase, owner p8-07). The members CNVT3D adds and p8-07 does
// not declare are read at their raw offsets: +0x3f50 the new 3D model, +0x4770 the double GEO23 c.
// Arguments are in the binary's order. Calls into other packets go through the seam in
// src/cnvt3d_sweep/ext.hpp.
#pragma once

#include <cstdint>
#include <cstdio>
#include <string>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
class OBJ_GEO2D;
class OBJ_GEO2D_DIGITIZED;
namespace geoprim {
class DEF_GEOPRIM;
}
} // namespace def::model

namespace def::cmd {
class MeshGenBase;
}

namespace def::cmd::cnvt3d_sweep {

using model::DB_STEP_DATA;
using model::LLOBJ_DATA;

// ================================ p9-03a =======================================================

// @src 140147b70  per-object generation: kind 4 brick sweep, 2 tet mesh, 5 geometry only
void fun_140147b70(MeshGenBase* cvt, std::int32_t id, std::int32_t nsec, std::int32_t m,
                   double start, double ang, double c, std::int32_t kind);

// @src 14013c340  append msg (with "[i/n] " when n > 1) to CONV_STATUS.DAT and <stem>.LOG, echo
void fun_14013c340(const char* msg, const char* stem);
// @src 14013d7c0  delete DEF_INT_3DTET.DAT
void fun_14013d7c0();
// @src 14013d7e0  DEF_INT_3DTET.DAT ("w"): n lines "%1d\n"
void fun_14013d7e0(std::int32_t n, const std::int32_t* vals);

// @src 140153710  geometry of the 3D object from the 2D object (revolve / extrude)
void fun_140153710(MeshGenBase* cvt, std::int32_t kd, double start, double ang, LLOBJ_DATA* obj,
                   MeshGenBase* mg, double c);
// @src 140153910  extrude: shift the type-3 mesh-window z by -/+ 0.001*|len| at the two ends
void fun_140153910(MeshGenBase* cvt, std::int32_t kd, double start, double len, LLOBJ_DATA* mgobj);
// @src 140153c00  GEOPRIM from the boundary geometry (loop 0 of geometry 0)
bool fun_140153c00(MeshGenBase* cvt, LLOBJ_DATA* obj, LLOBJ_DATA* o3, DB_STEP_DATA* model3);
// @src 140154270  GEOPRIM from the 2D mesh boundary, then the face split and the -90 rotation
bool fun_140154270(MeshGenBase* cvt, LLOBJ_DATA* obj, LLOBJ_DATA* o3, DB_STEP_DATA* model3);

// @src 140159030  floorf (140485048)
float fun_140159030(float x);
// @src 1401590d0  digitized profile +0x18 = tol
void fun_1401590d0(model::OBJ_GEO2D_DIGITIZED* d, double tol);
// @src 1401590f0  digitized profile point count (+0x38)
std::int32_t fun_1401590f0(const model::OBJ_GEO2D_DIGITIZED* d);
// @src 140159100  digitized profile points (+0x40)
double* fun_140159100(const model::OBJ_GEO2D_DIGITIZED* d);
// @src 140159110  curve has a digitized profile with at least one point
bool fun_140159110(const model::OBJ_GEO2D* g);
// @src 140159150  curve's digitized profile (+0x20)
model::OBJ_GEO2D_DIGITIZED* fun_140159150(const model::OBJ_GEO2D* g);
// @src 1401591b0  GEOPRIM +0x24fc && +0x24fd (both face kinds were produced)
bool fun_1401591b0(const model::geoprim::DEF_GEOPRIM* p);

// @src 140212460  STNCMP block offset n of the object's model (0 without a model)
std::int32_t fun_140212460(LLOBJ_DATA* obj, std::int32_t i);
// @src 14023cdb0  triangle (a, b, c): returns sqrt(s*|...|), *q = quality (0 for s <= 1e-9)
double fun_14023cdb0(const double* a, const double* b, const double* c, double* q);
// @src 14023d410  split face f (1-based, nper ints per face) into two triangles t1, t2
void fun_14023d410(const double* xyz, const std::int32_t* faces, std::int32_t f, std::int32_t nper,
                   std::int32_t* t1, std::int32_t* t2);

// @src 1402778c0  FIXGEO clean-up of a curve's digitized profile (14041e270(ext, 0, 1))
void fun_1402778c0(model::OBJ_GEO2D* g);
// @src 14041e1f0  sqrt((x2-x1)^2 + (y2-y1)^2)
double fun_14041e1f0(double x1, double y1, double x2, double y2);
// @src 14041e270  FIXGEO on the owner curve, read PNTGEO.GEO back, close the polygon
std::int32_t fun_14041e270(model::OBJ_GEO2D_DIGITIZED* d, std::int32_t code, char close);
// @src 14041e4d0  read PNTGEO.GEO ("rt") into the digitized points; 0 or a negative code
std::int32_t fun_14041e4d0(model::OBJ_GEO2D_DIGITIZED* d);
// @src 14041ea80  append the first point when the last is farther than |tol| from it
std::int32_t fun_14041ea80(model::OBJ_GEO2D_DIGITIZED* d);
// @src 14041f0f0  "PNTGEO.GEO"
const char* fun_14041f0f0();
// @src 14041f100  "rt"
const char* fun_14041f100();
// @src 14045a040  delete name when it can be opened; 1 when gone or absent, 0 when remove failed
std::int32_t fun_14045a040(const char* name);

// 1402a7b60 / 1402ac8a0: owner p9-ac8a0 (defcmd/step_dat.hpp write_step_dat)

// ---- MSVC 2008 iostream pieces of the packet list (used by the NBCD.OUT writer) ----------------
// The ios_base state the binary's ostream keeps (flags +0x18, precision +0x20, width +0x28).
struct IosState {
    std::int32_t flags = 0x201;     // skipws | dec
    std::int64_t precision = 6;
    std::int64_t width = 0;
    char fill = ' ';
};
// @src 140159050  std::ios_base::setf(int): flags |= f & 0xffff, returns the old flags
std::int32_t fun_140159050(IosState& s, std::int32_t f);
// @src 140159090  std::ios_base::precision(n): returns the old precision
std::int64_t fun_140159090(IosState& s, std::int64_t n);
// @src 14015a260  num_put::put(double) text for the state (width / fill applied)
std::string fun_14015a260(IosState& s, double v);
// @src 1401595b0  ostream << double: the text of 14015a260 written to f
void fun_1401595b0(std::FILE* f, IosState& s, double v);

// ================================ p9-03b =======================================================
// The other half (sweep, state transfer, DEF_INT, the SV accessors) is declared in
// defcmd/cnvt3d_sweep_b.hpp, included at the end of this header.

} // namespace def::cmd::cnvt3d_sweep

#include <defcmd/cnvt3d_sweep_b.hpp>
