// p8-02 internal types: the C containers of the STL reader (defCELL record list, defFPT float
// point list, defLOC point locator, the surface checker of DEF_STL.OUT and its patch records) and
// the reader objects (_DEF_STL, the PDA/UNV SCRATCH.KEY read-back objects, _DEF_NASTRAN).
// Members sit at the binary's offsets (docs/port-conventions.md section 4).
// Spec: docs/re/commands/GFREAD_STL_GEO_NAS.md §2, §4; GFREAD.md §6.1.
//
// CellList and PointList are shared with the owners of their basic operations (cell list:
// p67 "cube" group, FUN_1404050b0..1404057a0; point list: p8-09, FUN_140410fd0..1404115d0).
// They are declared here because no owner has published a header; the seams in ext_*.hpp take
// pointers to these structs (see the report of packet p8-02).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/sdctl/cow.hpp>

namespace def::cmd::stlnas {

using i32 = std::int32_t;

// defCELL record list (0x20 bytes): count records of `width` ints each.
struct CellList {
    i32 count = 0;   // 0x00
    i32 cap = 0;     // 0x04
    i32 width = 0;   // 0x08 record size (ints)
    i32 width2 = 0;  // 0x0c the same (FUN_140405700 reads this one)
    i32 inc = 0;     // 0x10 growth in records
    i32 pad14 = 0;   // 0x14
    i32* data = nullptr;  // 0x18
};
static_assert(sizeof(CellList) == 0x20);
static_assert(offsetof(CellList, data) == 0x18);

// defFPT float point list (0x30 bytes): 3 floats per point, 1-based ids.
struct PointList {
    i32 count = 0;       // 0x00
    i32 cap = 0;         // 0x04
    i32 grow = 0;        // 0x08
    i32 dirty = 0;       // 0x0c bounds must be recomputed
    float bounds[6] = {};  // 0x10 min x, max x, min y, max y, min z, max z
    float* data = nullptr;  // 0x28
};
static_assert(sizeof(PointList) == 0x30);
static_assert(offsetof(PointList, data) == 0x28);

// defLOC point locator (stack object of 0x90 bytes in FUN_140244fa0).
struct Locator {
    i32 dims[3];         // 0x00 100, 100, 100
    i32 nb;              // 0x0c number of buckets
    i32 inc;             // 0x10 bucket list growth (5)
    i32 level;           // 0x14 insertion level
    i32 u18;             // 0x18 1
    i32 count;           // 0x1c merged point count
    float tol;           // 0x20
    float tol2;          // 0x24
    float h[3];          // 0x28
    float range[6];      // 0x34 min, max per axis
    i32 pad4c;           // 0x4c
    PointList* points;   // 0x50 merged point list
    CellList* buckets;   // 0x58 nb lists of 1-int records
    CellList neigh;      // 0x60 neighbour buckets (3-int records)
    CellList* buckets2;  // 0x80 allocated, not used
};
static_assert(offsetof(Locator, points) == 0x50);
static_assert(offsetof(Locator, neigh) == 0x60);
static_assert(offsetof(Locator, buckets2) == 0x80);
static_assert(sizeof(Locator) == 0x88);

// patch record (0x88 bytes) of the surface checker
struct PatchData {
    CellList polys;   // 0x00 cells of the patch
    CellList points;  // 0x20 boundary points
    CellList starts;  // 0x40 curve starts
    CellList ends;    // 0x60 curve ends
    i32 ncurves;      // 0x80
    i32 pad84;
};
static_assert(sizeof(PatchData) == 0x88);

// surface checker (stack object of 0x130 bytes in FUN_140244fa0)
struct SurfCheck {
    PointList* points;   // 0x00
    CellList* cells;     // 0x08
    CellList* links;     // 0x10 per node: the cells containing it
    PatchData* patches;  // 0x18
    CellList invalid;    // 0x20 cells with a non-manifold edge
    CellList reversed;   // 0x40 reversed cells
    CellList pending;    // 0x60 cells deferred by the depth limit
    CellList small;      // 0x80 cells with a small area
    i32* patch;          // 0xa0 patch number per cell
    i32 nlinks;          // 0xa8
    i32 npatch;          // 0xac
    i32 npatchdata;      // 0xb0
    i32 visits;          // 0xb4
    i32 depth;           // 0xb8
    i32 maxdepth;        // 0xbc
    i32 limit;           // 0xc0 1000
    i32 exceed;          // 0xc4
    float small_area;    // 0xc8
    float area;          // 0xcc
    float min_area;      // 0xd0
    float max_area;      // 0xd4
    float min_edge;      // 0xd8
    float max_edge;      // 0xdc
    float t_start[10];   // 0xe0 GetTickCount values (0xe0..0xf4 used)
    float t_end[10];     // 0x108 (0x108..0x11c used)
};
static_assert(offsetof(SurfCheck, patch) == 0xa0);
static_assert(offsetof(SurfCheck, limit) == 0xc0);
static_assert(offsetof(SurfCheck, t_start) == 0xe0);
static_assert(offsetof(SurfCheck, t_end) == 0x108);
static_assert(sizeof(SurfCheck) == 0x130);

// _DEF_STL (0x70 bytes)
struct StlObj {
    i32 tolerant = 1;          // 0x00 1: tolerant locator, 0: exact
    i32 binary = 0;            // 0x04
    i32 nn = 0;                // 0x08
    i32 ne = 0;                // 0x0c
    double* nodes = nullptr;   // 0x10
    i32* cells = nullptr;      // 0x18 3 ints per triangle
    PointList raw_points;      // 0x20
    CellList raw_cells;        // 0x50
};
static_assert(offsetof(StlObj, raw_points) == 0x20);
static_assert(offsetof(StlObj, raw_cells) == 0x50);
static_assert(sizeof(StlObj) == 0x70);

// PDA/PAT and UNV SCRATCH.KEY read-back object (0x20 bytes; _DEF_PATRAN / _DEF_UNV)
struct ScratchObj {
    i32 mode = 1;              // 0x00 1 surface, 2 volume mesh
    i32 npe = 4;               // 0x04 nodes per element
    i32 nn = 0;                // 0x08
    i32 ne = 0;                // 0x0c
    double* nodes = nullptr;   // 0x10
    i32* conn = nullptr;       // 0x18
};
static_assert(sizeof(ScratchObj) == 0x20);

// _DEF_NASTRAN (0x20 bytes)
struct NasObj {
    i32 mode = 1;                                   // 0x00 1 CTRIA3, 2 CTETRA, 4 CHEXA
    i32 npe = 4;                                    // 0x04
    i32 nn = 0;                                     // 0x08
    i32 ne = 0;                                     // 0x0c
    def::model::CowVector<double> nodes;            // 0x10 (14019a6b0)
    def::model::CowVector<std::int32_t> conn;       // 0x18 (140159ad0)
};
static_assert(sizeof(NasObj) == 0x20);

// token reader of p8-09 (FUN_140411920: one int, 0)
struct TokenReader {
    i32 cut63 = 0;
};

}  // namespace def::cmd::stlnas
