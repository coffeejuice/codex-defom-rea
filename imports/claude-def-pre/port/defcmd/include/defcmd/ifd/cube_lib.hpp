// defcmd/ifd/cube_lib.hpp (public since the P6-7 gate: GENCTC 3D uses it too): the C library "cfnCUBE" (bucket search grid) in DEF_PRE and the helper
// layers it is built on (defLOCD locator, defCELL cell lists, defDPT point set, small vector
// helpers, Green & Hatch pcube). Spec: docs/re/commands/OBJPOS_interference_kernels.md section 2.
//
// The structs below sit at the binary's offsets (docs/port-conventions.md section 4): the
// decompile addresses everything as base + offset, and the cube table is one calloc'ed array of
// 0x1a0-byte objects. Memory is managed with calloc/realloc/free as in the original; the point
// and polygon arrays of a built cube are the caller's (borrowed, never freed).
#pragma once

#include <cstddef>
#include <cstdint>

namespace def::cmd::cube {

// ---- defCELL: list of fixed-width int records (0x20 bytes) ----------------------------------
struct CellList {
    std::int32_t count;   // +0x00 records in use
    std::int32_t cap;     // +0x04 records allocated
    std::int32_t width;   // +0x08 ints per record
    std::int32_t width2;  // +0x0c second copy of the width (1404050b0 stores param_4 twice)
    std::int32_t inc;     // +0x10 growth increment (records)
    std::int32_t pad14;   // +0x14 never written
    std::int32_t* data;   // +0x18
};
static_assert(sizeof(CellList) == 0x20);
static_assert(offsetof(CellList, inc) == 0x10);
static_assert(offsetof(CellList, data) == 0x18);

// ---- defDPT: point set (0x48 bytes) ------------------------------------------------------------
struct PointSet {
    std::int32_t count;   // +0x00
    std::int32_t cap;     // +0x04
    std::int32_t inc;     // +0x08
    std::int32_t dirty;   // +0x0c bounds must be recomputed (140406360)
    double bounds[6];     // +0x10 min x, max x, min y, max y, min z, max z
    double* data;         // +0x40 x, y, z per point
};
static_assert(sizeof(PointSet) == 0x48);
static_assert(offsetof(PointSet, data) == 0x40);

// ---- 16-byte element arrays at cube +0x180 / +0x190 (1404044c0, 1404047a0) ---------------------
struct ListNode {          // node of the two linked lists freed by 140404c40 / 140405010
    void* u00;             // +0x00 not touched by the free routines
    void* data;            // +0x08 freed
    ListNode* next;        // +0x10
};
struct ElemLists {         // one element: two list heads
    ListNode* a;           // +0x00 freed with 140404c40
    ListNode* b;           // +0x08 freed with 140405010
};
static_assert(sizeof(ElemLists) == 0x10);
struct ElemArray {
    std::int32_t count;    // +0x00
    std::int32_t pad04;
    ElemLists* data;       // +0x08
};
static_assert(sizeof(ElemArray) == 0x10);

// ---- defLOCD: the bucket locator (0xf0 bytes, cube +0x08) -------------------------------------
struct Locator {
    std::int32_t type;          // +0x00 0..7 (1 = polygons)
    std::int32_t n[3];          // +0x04 divisions per axis
    std::int32_t ncells;        // +0x10 n[0]*n[1]*n[2] (50 before 1403ff970: overwritten there)
    std::int32_t cell_inc;      // +0x14 initial cell list increment (5)
    std::int32_t ins_level;     // +0x18 insertion level (1403ff970)
    std::int32_t u1c;           // +0x1c set to 1 (1403fed00)
    std::int32_t u20;           // +0x20 set to 0 (1403fed00, 1403ff970)
    std::int32_t pad24;
    double tol;                 // +0x28 insertion tolerance ((double)1e-6f)
    double tol2;                // +0x30 tol * tol
    double h[3];                // +0x38 cell size per axis
    double box[6];              // +0x50 lo x, hi x, lo y, hi y, lo z, hi z
    CellList set80;             // +0x80 int set (width 3, increment 100), never filled here
    PointSet* pts;              // +0xa0 the cube's point set
    CellList* cells_a8;         // +0xa8 point cells (types 0, 3, 4, 6)
    CellList* cells_poly;       // +0xb0 polygon cells (types 1, 3, 5, 6)
    CellList* cells_b8;         // +0xb8 element cells (types 2, 4)
    CellList* cells_c0;         // +0xc0 type 7 cells (width 2)
    std::int32_t offset;        // +0xc8 point id of pts(:, 1), at least 1
    std::int32_t padcc;
    void* ud0[4];               // +0xd0 freed by 1403feec0, never allocated here
};
static_assert(sizeof(Locator) == 0xf0);
static_assert(offsetof(Locator, tol) == 0x28);
static_assert(offsetof(Locator, h) == 0x38);
static_assert(offsetof(Locator, box) == 0x50);
static_assert(offsetof(Locator, set80) == 0x80);
static_assert(offsetof(Locator, pts) == 0xa0);
static_assert(offsetof(Locator, cells_poly) == 0xb0);
static_assert(offsetof(Locator, offset) == 0xc8);
static_assert(offsetof(Locator, ud0) == 0xd0);

// ---- cfnCUBE object (0x1a0 bytes) --------------------------------------------------------------
struct Cube {
    std::int32_t id;            // +0x000 object id (-1 when free)
    std::int32_t sub;           // +0x004 entity set id (-1 when free)
    Locator loc;                // +0x008
    CellList polys;             // +0x0f8 polygon set: count = cap = nPoly, width = nodes/poly, data = caller's
    PointSet points;            // +0x118 point set: count = cap = nPts, data = caller's
    CellList result;            // +0x160 result list of the last query
    ElemArray a180;             // +0x180
    ElemArray a190;             // +0x190
};
static_assert(sizeof(Cube) == 0x1a0);
static_assert(offsetof(Cube, loc) == 0x8);
static_assert(offsetof(Cube, polys) == 0xf8);
static_assert(offsetof(Cube, points) == 0x118);
static_assert(offsetof(Cube, result) == 0x160);
static_assert(offsetof(Cube, a180) == 0x180);
static_assert(offsetof(Cube, a190) == 0x190);

// ---- globals ---------------------------------------------------------------------------------
// @data 1406fe4f0  cube table (calloc'ed array of Cube)
inline Cube* g_cubes = nullptr;
// @data 1406fe4f8  number of cubes allocated
inline std::int32_t g_cube_size = 0;
// @data 1406fe4fc  number of cubes in use
inline std::int32_t g_cube_used = 0;


// ---- error and allocation helpers --------------------------------------------------------------
[[noreturn]] void fun_14039d450(const char* fmt, ...);  // fatal error: message, SFTC note, exit(1)
void fun_14039d3a0(const char* fmt, const char* a, const char* b, std::int32_t n);  // message only
std::int32_t* fun_140256dc0(std::int32_t n);                // calloc n ints
std::int32_t* fun_140256e20(std::int32_t* p, std::int32_t n);  // realloc to n ints
double* fun_140257150(std::int32_t n);                      // calloc n doubles
Cube* fun_140231b10(std::int32_t n);                        // cube table allocation

// ---- defCELL / defDPT / element arrays ---------------------------------------------------------
void fun_1404050b0(CellList* c, std::int32_t cap, std::int32_t inc, std::int32_t width);
void fun_140405150(CellList* c);
void fun_140405350(CellList* c, std::int32_t grow);
std::int32_t fun_140405540(CellList* c, std::int32_t npts, const std::int32_t* rec);
std::int32_t* fun_140405720(CellList* c, std::int32_t k);
std::int32_t fun_1404056c0(const CellList* c);
std::int32_t fun_1404056e0(const CellList* c);
CellList* fun_1404057a0(std::int32_t n);
void fun_140405c80(PointSet* p, std::int32_t cap, std::int32_t inc);
std::int32_t fun_1404062e0(const PointSet* p);
double* fun_140406300(PointSet* p, std::int32_t k);
double* fun_140406360(PointSet* p);
double* fun_140406510(PointSet* p);
void fun_1404044c0(ElemArray* a, std::int32_t n);
ElemLists* fun_1404047f0(std::int32_t n);
void fun_1404047a0(ElemArray* a);
void fun_140404850(ElemLists* p, std::int32_t n);
void fun_140404c40(ListNode* p);
void fun_140405010(ListNode* p);
std::int32_t fun_140404780(const ElemArray* a);

// ---- defLOCD -----------------------------------------------------------------------------------
void fun_1403fed00(Locator* l, std::int32_t type, double tol, std::int32_t offset);
void fun_1403ff890(Locator* l, std::int32_t nx, std::int32_t ny, std::int32_t nz);
void fun_1403ff970(Locator* l, PointSet* pts, const double* bounds, std::int32_t verbose);
void fun_1403feec0(Locator* l);
std::int32_t fun_1403ff870(const Locator* l);
std::int64_t fun_140401050(Locator* l, const std::int32_t* nodes, std::int32_t np, std::int32_t id,
                           std::int32_t verbose);

// ---- vectors (C++ Vec3 helpers in the binary, returned by value) -------------------------------
struct Vec3 {
    double x, y, z;
};
Vec3 fun_140239700(double x, double y, double z);           // construct
Vec3 fun_140239fb0(const double* a, const double* b);       // a - b
Vec3 fun_14023a4b0(const Vec3& a, const Vec3& b);           // a x b
Vec3 fun_14023a1b0(const Vec3& a, const Vec3& b);           // a + b
double fun_1402399b0(const Vec3& a);                        // a . a
double fun_140239a30(const Vec3& a);                        // |a|
Vec3 fun_140239b40(const Vec3& a);                          // a / |a|, 0 when |a| <= 1e-12
Vec3 fun_140240d80(const double* v, std::int32_t np);       // unit polygon normal
std::int32_t fun_14023aa90(const Vec3& a, const Vec3& b, double eps);  // |a_i - b_i| < eps, all i
std::int32_t fun_140240b40(const double* a, const double* b);  // boxes (min3, max3) overlap

// ---- pcube (Green & Hatch) ---------------------------------------------------------------------
void fun_140454110(double* n, std::int32_t nv, const double* v);  // unnormalised polygon normal
std::int32_t fun_1404537f0(std::int32_t nv, const double* v, const double* n, std::int32_t vout,
                           std::int32_t eout);
std::int32_t fun_140453010(std::int32_t nv, const double* v, std::int32_t vout);
std::int32_t fun_140453e80(std::int32_t nv, const double* v, const double* n, std::int32_t eout);
std::uint32_t fun_1404531b0(const double* p, std::uint32_t mask);
std::uint32_t fun_140453300(const double* p, std::uint32_t mask);
std::uint32_t fun_1404535e0(const double* p, std::uint32_t mask);
std::int32_t fun_140453850(std::int32_t nv, const double* v, const double* n, const double* p);
std::int32_t fun_140453c40(const double* v0, const double* v1);

// ---- cfnCUBE -----------------------------------------------------------------------------------
void fun_140225af0(std::int32_t* idx, std::int32_t* clean_elems, std::int32_t* verbose);
void fun_140226150(std::int32_t* id, std::int32_t* sub, std::int32_t* type, std::int32_t* npts,
                   double* pts, std::int32_t* npoly, std::int32_t* npp, std::int32_t* polys,
                   std::int32_t* offset, std::int32_t* create, std::int32_t* verbose);
int fun_140231d00(const void* a, const void* b);

// Parts of 140226150 split out so that tier-1 tests can check them without CRT pow.
// Division count bc from the polygon count.
std::int32_t division_count(std::int32_t npoly);
// Divisions per axis from the extents a[3], bc, the polygon count and pw = pow(fabs((a0 a1) a2),
// 0.3333333); the 1.1 limit loop for bc >= 50. sp receives pw / bc.
void divisions(const double* a, std::int32_t bc, std::int32_t npoly, double pw, std::int32_t* n,
               double* sp);

} // namespace def::cmd::cube
