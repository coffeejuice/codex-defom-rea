// defcmd/iges.hpp: in-process IGES reader for 2D objects (GFREAD .IGS), packet p8-03.
// Spec: docs/re/commands/IGES_READER.md (supersedes DXF_IGES_READERS.md §3).
//
// Public API for the other Phase 8 packets:
//   p8-01  fun_140409af0 (GFREAD 2D front end 140244500 / 1402442f0), the class
//          Uic2CObjectIgesFile (constructed on the stack there)
//   p8-04  fun_140409af0, fun_14040c220 (pass 2 on the DXF segment table, mode 700),
//          fun_140463374 (rational B-spline sampling for DXF SPLINE), the context g_iges_ctx and
//          the work arrays g_iges_rw / g_iges_iw
#pragma once

#include <cstddef>
#include <cstdint>

#include <defcmd/uic2c_object.hpp>
#include <defmodel/layout/layout_check.hpp>

namespace def::model {
class LLOBJ_DATA;
}

namespace def::cmd::iges {

// Uic2CObjectIgesFile (RTTI .?AVUic2CObjectIgesFile@@), vtable 0x140564680:
//   slot 0  14040cda0 scalar deleting dtor (dtor 140409a90: base dtor 140454980)
//   slot 1  14040cd90 "Uic2CObjectIgesFile"
//   ctor    1404099d0(this, name): base ctor, vptr, u18 = u1c = 0, u20 = 0, then 140409a50
//           (the same three stores again). Every caller passes name = 0.
//   size    0x28 (140244500 keeps it in a 40-byte stack slot).
class Uic2CObjectIgesFile : public Uic2CObject {
public:
    // @src 1404099d0  ctor: base ctor, vptr, u18 = u1c = 0, u20 = 0, 140409a50
    explicit Uic2CObjectIgesFile(const char* name = nullptr);
    // @src 14040cda0  slot 0 (dtor 140409a90: vptr, base dtor 140454980)
    ~Uic2CObjectIgesFile() override;
    // @src 14040cd90  slot 1
    const char* class_name() const override;

    // @src 140409a50  u18 = u1c = 0, u20 = 0
    void reset();
    // The accessors p8-01 owns (gfread.hpp) on this layout: 14040ce20 u18 = v, 14040ce40 u18 = 1
    // (pass 2 returned 200: several loops), 14040ce60 u1c = v (boundary count), 1402566b0 u1c.

    std::int32_t u18;    // 0x18 1 after a code-200 read (several loops)
    std::int32_t u1c;    // 0x1c boundary count (GFREAD copies it to obj+0xbc8)
    double u20;          // 0x20 point tolerance of the last read
};

static_assert(sizeof(Uic2CObjectIgesFile) == 0x28);
DEF_LAYOUT_OFFSET(Uic2CObjectIgesFile, u18, 0x18);
DEF_LAYOUT_OFFSET(Uic2CObjectIgesFile, u1c, 0x1c);
DEF_LAYOUT_OFFSET(Uic2CObjectIgesFile, u20, 0x20);

// 14070bc20  reader context (layout; the @data owner is g_iges_ctx below) (0x430 bytes; 14040ba80 memsets exactly 0x430)
struct IgesCtx {
    char name[0x400];        // +0x000 file name (strcpy in pass 1)
    std::int32_t n_points;   // +0x400 c020 output points (3 doubles each in RW)
    std::int32_t n_records;  // +0x404 c024 entity records; after pass 2 the segment count
    std::int32_t n100;       // +0x408 c028 arcs
    std::int32_t n110;       // +0x40c c02c lines
    std::int32_t n112;       // +0x410 c030 parametric splines
    std::int32_t n126;       // +0x414 c034 rational B-splines
    std::int32_t n124;       // +0x418 c038 transformation matrices
    std::byte pad_41c[4];    // +0x41c
    double tol;              // +0x420 c040 point tolerance
    double* table;           // +0x428 c048 segment table, 6 doubles per segment
};
static_assert(sizeof(IgesCtx) == 0x430);
DEF_LAYOUT_OFFSET(IgesCtx, n_points, 0x400);
DEF_LAYOUT_OFFSET(IgesCtx, tol, 0x420);
DEF_LAYOUT_OFFSET(IgesCtx, table, 0x428);

// @data 14070bc20
inline IgesCtx g_iges_ctx{};
// @data 14070c050  Fortran real work array RW (g_iges_rw_size doubles), owned by the drivers
inline double* g_iges_rw = nullptr;
// @data 14070c058  Fortran integer work array IW (g_iges_iw_size ints)
inline std::int32_t* g_iges_iw = nullptr;
// @data 14070c060  text-route segment object (0x88 bytes) while 14040b100 runs, else 0
inline void* g_iges_text = nullptr;
// @data 140628568  size of RW (1 500 000)
inline std::int32_t g_iges_rw_size = 1500000;
// @data 140628554  size of IW (1 500 000)
inline std::int32_t g_iges_iw_size = 1500000;
// @data 140628550  number of rational B-spline samples (30)
inline std::int32_t g_iges_nurbs_samples = 30;

// @src 140409af0  Uic2CObjectIgesFile read: IGES file `name` -> curve 0 of object `obj`.
// mode 300 (GFREAD: p5 = &x, p6 = 2, p7 = 0, p8 = 1) or 400 (re-conversion of the existing
// geometry, 1402442f0). p7 != 0 exchanges the tolerance with *p5 (p8 == 1: out, else in).
// Returns 0, or -1 (no object / bad mode / no table), -2 -3 -12 (name/prefix), the pass codes
// -4..-11, -14 -15 (text route), the 14040b100 codes.
int fun_140409af0(Uic2CObjectIgesFile* iges, const char* name, const char* prefix,
                  model::LLOBJ_DATA* obj, double* p5, std::int32_t p6, char p7, std::int32_t p8,
                  std::int32_t mode);

// @src 14040c220  pass dispatcher. pass 1: read `name` into RW/IW (codes -4..-8). pass 2: match,
// trace and write the segment table ctx->table; mode 500 (flag 1, table mode 1), 600 (mode 2,
// flag 0: DEF_IGS.OUT), 700 (mode 3, flag 1; DXF), other (mode 1, flag 0, no table).
// Returns 0, 200 (several loops), -9, -10, -11.
int fun_14040c220(const char* name, IgesCtx* ctx, std::int32_t pass, std::int32_t mode);

// @src 140463374  rational B-spline sampling (Fortran, IGES 126): `nsamp` points at
// u = i*((v1-v0)/(nsamp-1)) + v0, i = 0..nsamp-1, written as 3 doubles from out[(nout-1)*3];
// *nout is advanced by one per point and decremented once at the end. ctrl: control points
// (3 doubles each, from index 0), knots: t(1..) = T(-M).., weights: W(0).., k = K, m = M.
void fun_140463374(const double* ctrl, const double* knots, const double* weights,
                   const std::int32_t* k, const std::int32_t* m, double* out,
                   const std::int32_t* nsamp, std::int32_t* nout, const double* v0,
                   const double* v1);

// Test support: frees the work arrays and zeroes g_iges_ctx and the Fortran COMMON state.
void reset_globals();

} // namespace def::cmd::iges
