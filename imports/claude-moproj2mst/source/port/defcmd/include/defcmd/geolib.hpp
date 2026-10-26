// defcmd/geolib.hpp: packet p8-09, the geometry support library of the Phase 8 readers:
// _DEF_AMGGEO (GEO 2D/3D reader and store), the float point list and the surface checker's
// setup (DEF_STL.OUT pass), the line/token reader 140411ad0, and shared model helpers.
// Spec: docs/re/commands/GFREAD_STL_GEO_NAS.md sections 2.6, 3, 4.6, 5 (topology: 5.4).
//
// Every function another packet calls is declared here as fun_<addr> with the binary's argument
// order. Callers: p8-01 (GFREAD), p8-02 (STL/NAS), p8-03/p8-04 (IGES/DXF), p8-05..p8-08.
// Bodies live in port/defcmd/src/geolib/. A body that is not ported yet ends in DEF_NOT_PORTED.
//
// Layouts (docs/port-conventions.md section 4): the structs below sit at the binary's offsets.
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <string>

#include <defmodel/layout/def_amggeo.hpp>
#include <defmodel/layout/layout_check.hpp>
#include <defmodel/sdctl/cow.hpp>
#include <defrt/port.hpp>

namespace def::model {
class LLOBJ_DATA;
class DB_STEP_DATA;
class GEO2D_AND_MESH2D;
} // namespace def::model

namespace def::cmd::geolib {

using def::model::DEF_AMGGEO;
using def::model::LLOBJ_DATA;

// ---- defCELL record list (0x20 bytes) -------------------------------------------------------
// Layout only: the functions (1404050b0 init, 140405150 free, 140405540 append, 1404056c0 count,
// 1404056e0 width, 140405720 get, 1404057a0 array alloc, 140405800 reverse) are Phase 6-7 code
// (claude/p67-2 port/defcmd/src/ifd/cube/cells.cpp, struct def::cmd::cube::CellList, same
// layout). p8-09 calls them through src/geolib/ext_p67.hpp. To be unified at integration.
struct CellList {
    std::int32_t count;   // +0x00 records in use
    std::int32_t cap;     // +0x04 records allocated
    std::int32_t width;   // +0x08 ints per record
    std::int32_t width2;  // +0x0c
    std::int32_t inc;     // +0x10 growth increment
    std::int32_t pad14;
    std::int32_t* data;   // +0x18
};
static_assert(sizeof(CellList) == 0x20);

// ---- float point list (0x30 bytes): 140410fd0 init, 1404113b0/1404115d0 append, 140411300 grow,
// 140411620 count, 140411080 free; 140411360 trim, 140411640 get, 1404116a0/140411850 bounds are
// p8-02's.
struct FltPointList {
    std::int32_t count;    // +0x00 points in use (ids are 1-based: point k is data[3*(k-1)])
    std::int32_t cap;      // +0x04 points allocated
    std::int32_t grow;     // +0x08 growth increment (points)
    std::int32_t dirty;    // +0x0c 1 after an append (bounds must be recomputed)
    float bounds[6];       // +0x10 set to 0 by the init; filled by the bounds routine (p8-02)
    float* data;           // +0x28 x, y, z per point (calloc / realloc, 4-byte floats)
};
static_assert(sizeof(FltPointList) == 0x30);
DEF_LAYOUT_OFFSET(FltPointList, bounds, 0x10);
DEF_LAYOUT_OFFSET(FltPointList, data, 0x28);

// ---- surface checker (DEF_STL.OUT pass). The binary keeps it on the stack at 0xc0 below the
// locator at 0x1f0 (140244fa0), so it is at most 0x130 bytes; 140244fa0's ctor 14040e620 writes
// up to 0x11c. ctor/dtor/links are p8-09's; the walk, area, patches and summary (14040f4e0,
// 14040f9f0, 14040fa70, 14040ff60, 140410130, 140410260, 140410760, 140410d40, 14040e980) are
// p8-02's, which may name the u-members.
struct PatchRec;  // 0x88-byte patch record (4 CellLists + 8 bytes), freed by 140459d00
struct SurfCheck {
    FltPointList* pts;     // +0x00
    CellList* cells;       // +0x08 triangles (width 3)
    CellList* links;       // +0x10 links[v]: cells of node v (width 1), count = u_a8 entries
    std::byte* patches;    // +0x18 u_b0 patch records of 0x88 bytes
    CellList list20;       // +0x20 init (0, 10, 1)
    CellList list40;       // +0x40 init (0, 10, 1)
    CellList list60;       // +0x60 init (0, 5, 1)
    CellList list80;       // +0x80 init (0, 10, 1)
    void* ua0;             // +0xa0 freed by the dtor
    std::int32_t nlinks;   // +0xa8 node count when links were built (14040fde0)
    std::int32_t uac;      // +0xac
    std::int32_t npatch;   // +0xb0 patch records
    std::int32_t ub4;
    std::int32_t ub8;
    std::int32_t ubc;
    std::int32_t depth_limit;  // +0xc0 1000
    std::int32_t uc4;
    float small_area;      // +0xc8 the ctor's 4th argument ((float)1E-9 from STL)
    std::int32_t ucc;
    std::int32_t ud0;      // +0xd0 not written by the ctor
    std::int32_t ud4;      // +0xd4 not written by the ctor
    std::int32_t ud8;      // +0xd8
    std::int32_t udc;      // +0xdc
    std::int32_t ue0[5];   // +0xe0
    std::int32_t uf4[5];   // +0xf4 not written by the ctor
    std::int32_t u108[5];  // +0x108
    std::int32_t u11c;     // +0x11c not written by the ctor
};
static_assert(sizeof(SurfCheck) == 0x120);
DEF_LAYOUT_OFFSET(SurfCheck, list20, 0x20);
DEF_LAYOUT_OFFSET(SurfCheck, list80, 0x80);
DEF_LAYOUT_OFFSET(SurfCheck, ua0, 0xa0);
DEF_LAYOUT_OFFSET(SurfCheck, nlinks, 0xa8);
DEF_LAYOUT_OFFSET(SurfCheck, npatch, 0xb0);
DEF_LAYOUT_OFFSET(SurfCheck, depth_limit, 0xc0);
DEF_LAYOUT_OFFSET(SurfCheck, small_area, 0xc8);
DEF_LAYOUT_OFFSET(SurfCheck, ue0, 0xe0);
DEF_LAYOUT_OFFSET(SurfCheck, u108, 0x108);

// ---- token reader object (4 bytes; the line and token table are globals, see geolib.cpp) ----
struct TokenReader {
    std::int32_t fixed63;  // +0x00 1: cut every line at column 63 (never set by the callers)
};
static_assert(sizeof(TokenReader) == 4);

// Token types of fun_140411ad0 (GFREAD_STL_GEO_NAS.md section 4.6).
enum TokenType : std::int32_t {
    tok_bool = 1,     // atol != 0 -> bool (1 byte)
    tok_upper = 2,    // upper-cased string
    tok_string = 3,
    tok_raw = 4,      // string; the line is cut only at \r \n (no // comment, no tokens)
    tok_int = 5,      // atol -> int32
    tok_int6 = 6,     // atoi -> int32
    tok_int7 = 7,
    tok_uint = 8,     // strtoul(., 10) -> 32-bit int (the store is an integer MOV)
    tok_float = 9,    // D -> E, atof -> float
    tok_double = 10,  // D -> E, strtod -> double
    tok_de = 11,      // D -> E in place, nothing stored
    tok_line = 12,    // the whole next physical line
};
inline constexpr std::int32_t tok_new_line = -5000;

// =============================== shared model helpers =====================================

// 140006730 is std::string::append(const char*) (strlen, then 140005f40 append(p, n)):
// not-needed (stl), use std::string. Alias kept for callers that follow the binary.
inline std::string* fun_140006730(std::string* s, const char* text)
{
    s->append(text);
    return s;
}

// @src 140158fc0  DB_STEP_DATA scalar deleting dtor: dtor 1401cb2c0, free when (flags & 1)
void* fun_140158fc0(def::model::DB_STEP_DATA* step, std::uint32_t flags);

// @src 140198270  make sure geometry 0 exists (140301ad0 on obj+0x3998 when 140197f40(obj,0) is
// null), then add one curve to it (14027fce0)
void fun_140198270(LLOBJ_DATA* obj);

// @src 1401ffd80  delete the mesh of an object: the 0xbd8 sub-lists, the node/element arrays,
// counts +0x87c/+0x880/+0x884 (and +0x88c in a 2D model) = 0
void fun_1401ffd80(LLOBJ_DATA* obj);

// @src 140202700  allocate a mesh: dimension (dim, or the model's when dim == 0), element type,
// node count, element count (dims = the model's array dimensions), then the material group
void fun_140202700(LLOBJ_DATA* obj, std::int32_t nnode, std::int32_t nelem, std::int32_t elem_type,
                   const std::int32_t* dims, std::int32_t dim);

// @src 1402027e0  set the object's material group (+0x3980 via 1402029e0); with apply and a
// mesh, also the element groups (+0x9f8): all of them, or (only_old) those equal to the old group
void fun_1402027e0(LLOBJ_DATA* obj, std::int32_t group, bool only_old, bool apply);
// @src 1402029e0  obj+0x3980 = group
void fun_1402029e0(LLOBJ_DATA* obj, std::int32_t group);

// @src 14023a380  dot product a[0]*b[0] + a[1]*b[1] + a[2]*b[2] (left to right)
double fun_14023a380(const double* a, const double* b);

// 140256800 push_back a double: not-needed, same code as 1402f0ba0 = CowVector<T>::push_back
// (added by p9-02 to defmodel/sdctl/cow.hpp); 140256950 / 1402569e0 (its reserve / reallocate)
// are the same code as 1402f2240 / 1402f2580. Inline forward kept for the callers.
namespace detail {
template <class V>
inline void cow_push_back(V* v, const double* value)
{
    if constexpr (requires { v->push_back(*value); }) {
        v->push_back(*value);
    } else {
        DEF_NOT_PORTED(0x1402f0ba0);  // CowVector::push_back: p9-02 (origin/claude/p9-02), not merged
    }
}
} // namespace detail
inline void fun_140256800(def::model::CowVector<double>* v, const double* value)
{
    detail::cow_push_back(v, value);
}

// Outside the packet list; forwards to DBREAD's ports (defdbio/dbread, the owner):
// 1402571b0  realloc to n doubles; null for 0; message on failure
double* fun_1402571b0(double* p, std::int32_t n);
// 140198230  set the curve count of geometry 0 (14027fe20)
void fun_140198230(LLOBJ_DATA* obj, std::int32_t n);
// 14027fe20  set the edge count of a geometry (grow with 14027fce0, shrink with 140280290)
void fun_14027fe20(def::model::GEO2D_AND_MESH2D* g, const std::int32_t* n);
// 140280290  remove the edges at the indexes idx[0..*n-1] (curve deleted, edge freed), renumber
bool fun_140280290(def::model::GEO2D_AND_MESH2D* g, const std::int32_t* idx, const std::int32_t* n);

// @src 140257080  calloc(n, 4) as floats; null for 0; message on failure
float* fun_140257080(std::int32_t n);
// @src 1402570e0  realloc(p, n * 4) as floats; null for 0; message on failure
float* fun_1402570e0(float* p, std::int32_t n);

// =============================== _DEF_AMGGEO =============================================

// @src 140246d00  ctor (placement on caller storage): returns g
DEF_AMGGEO* fun_140246d00(DEF_AMGGEO* g);
// @src 140246d40  dtor (frees curves and surface)
void fun_140246d40(DEF_AMGGEO* g);
// @src 1402566d0  scalar deleting dtor (vtable slot 0)
void* fun_1402566d0(DEF_AMGGEO* g, std::uint32_t flags);

// @src 140246f70  read a GEO file: mode 2 or 4 -> 2D reader 1402478f0(g, file, close, mode, obj),
// else the 3D reader 1402484c0(g, file); returns the reader's result. close: *close != 0 closes
// each kept curve with a copy of its first point (GFREAD passes 0).
bool fun_140246f70(DEF_AMGGEO* g, const char* file, const char* close, std::int32_t mode,
                   LLOBJ_DATA* obj);
// @src 14024bac0  store into obj: mode 2 or 4 -> 2D curves 14024bb30, else 3D surface 14024be40
bool fun_14024bac0(DEF_AMGGEO* g, LLOBJ_DATA* obj, std::int32_t mode);

// @src 1402478f0  2D GEO reader (section 5.2); stores via 14024bac0(g, obj, 4) and reads the
// topology after 99 (140249700)
bool fun_1402478f0(DEF_AMGGEO* g, const char* file, const char* close, std::int32_t mode,
                   LLOBJ_DATA* obj);
// @src 140248390  2D pre-check: line 2 has exactly 2 tokens
bool fun_140248390(const char* file);
// @src 1402484c0  3D GEO reader (section 3.2)
bool fun_1402484c0(DEF_AMGGEO* g, const char* file);
// @src 1402488a0  3D pre-check: line 2 has exactly 4 tokens, the first one atol == 1
bool fun_1402488a0(const char* file);
// @src 140249700  2D topology section after 99 (section 5.4 / docs/re/commands/GEO_TOPOLOGY.md)
void fun_140249700(DEF_AMGGEO* g, std::FILE* f, LLOBJ_DATA* obj);
// @src 14024ae10  geometry 0 of obj, null when obj is null or has no geometry
void* fun_14024ae10(DEF_AMGGEO* g, LLOBJ_DATA* obj);
// @src 14024bb30  2D store (section 5.3)
bool fun_14024bb30(DEF_AMGGEO* g, LLOBJ_DATA* obj, std::int32_t mode);
// @src 14024be40  3D store (section 3.3)
bool fun_14024be40(DEF_AMGGEO* g, LLOBJ_DATA* obj);
// @src 1402567e0  curve->u0c = code (the _OBJ_GEO2D's boundary code)
void fun_1402567e0(void* curve, std::int32_t code);

// =============================== point list ==============================================

// @src 140410fd0  init: count 0, cap, grow, data null, dirty 1, bounds 0; cap > 0 -> calloc 3*cap
void fun_140410fd0(FltPointList* l, std::int32_t cap, std::int32_t grow);
// @src 140411080  free: count 0, cap 0, dirty 1, grow 100, free data
void fun_140411080(FltPointList* l);
// @src 140411300  grow: cap += grow, realloc data to 3*cap floats
void fun_140411300(FltPointList* l);
// @src 1404113b0  append (x, y, z); grows first when count >= cap - 1; returns the new 1-based id
std::int32_t fun_1404113b0(FltPointList* l, float x, float y, float z);
// @src 1404115d0  append p[0..2] (1404113b0)
std::int32_t fun_1404115d0(FltPointList* l, const float* p);
// @src 140411620  count
std::int32_t fun_140411620(const FltPointList* l);

// =============================== surface checker =========================================

// @src 14040e620  ctor: pts, cells, small_area; lists 0x20/0x40/0x80 (0, 10, 1), 0x60 (0, 5, 1),
// depth limit 1000, every counter 0
void fun_14040e620(SurfCheck* s, FltPointList* pts, CellList* cells, float small_area);
// @src 14040e810  dtor: the four lists, the links, the patch records (140459d00), +0xa0
void fun_14040e810(SurfCheck* s);
// @src 14040fde0  links: links[v] = the 1-based cells that contain node v, in cell order
void fun_14040fde0(SurfCheck* s);
// @src 140459d00  free one patch record (its four CellLists at +0, +0x20, +0x40, +0x60)
void fun_140459d00(std::byte* patch);

// =============================== token reader ============================================

// @src 140411920  ctor: fixed63 = 0
TokenReader* fun_140411920(TokenReader* r);
// @src 140411940  dtor (empty)
void fun_140411940(TokenReader* r);
// @src 140411970  replace character from by to in s (exact == 1: that case only; else both cases)
char* fun_140411970(TokenReader* r, char* s, char from, char to, std::int32_t exact);
// @src 140411a60  upper-case s in place
char* fun_140411a60(TokenReader* r, char* s);
// @src 140411ad0  read a token (section 4.6). pos: tok_new_line (-5000) = first token of the next
// non-comment line (returns its token count), 0 = next token (a new line when used up), k > 0 =
// token k of the current line. Returns 0 / count, -1 (no token k), -2 (EOF; type 2 and 12 store
// "DONE"). out: int32*, float*, double*, bool* or char* by type.
std::int32_t fun_140411ad0(TokenReader* r, std::FILE* f, std::int32_t type, void* out,
                           std::int32_t pos);

// Test support: the reader's globals (line buffer, token table, counters) back to load state.
void reset_token_reader();

} // namespace def::cmd::geolib
