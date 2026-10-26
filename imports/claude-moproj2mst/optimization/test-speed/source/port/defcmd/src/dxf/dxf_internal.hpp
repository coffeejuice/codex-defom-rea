// defcmd/src/dxf/dxf_internal.hpp: the DXF reader's internal records and functions (p8-04).
// Layouts are the binary's (DXF_READER.md section 14); the work struct is calloc'd and freed
// with free() like the original.
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>

namespace def::model {
class OBJ_GEO2D;
} // namespace def::model

namespace def::cmd::dxf {

// LINE record, 0x38 bytes
struct LineRec {
    double x1, y1, x2, y2;  // 0x00
    double ext[3];          // 0x20 extrusion (210, 220, 230)
};
static_assert(sizeof(LineRec) == 0x38);

// polyline vertex, 0x18 bytes
struct Vertex {
    double x, y, bulge;
};
static_assert(sizeof(Vertex) == 0x18);

// polyline record (POLYLINE, LWPOLYLINE, SPLINE), 0x30 bytes
struct PolyRec {
    std::int32_t n_vert;    // 0x00
    std::int32_t pad_04;
    Vertex* v;              // 0x08
    std::int32_t flag;      // 0x10 closed
    std::int32_t pad_14;
    double ext[3];          // 0x18
};
static_assert(sizeof(PolyRec) == 0x30);
static_assert(offsetof(PolyRec, flag) == 0x10);
static_assert(offsetof(PolyRec, ext) == 0x18);

// ARC record (ARC, CIRCLE), 0x40 bytes
struct ArcRec {
    double cx, cy, r, a50, a51;  // 0x00
    double ext[3];               // 0x28
};
static_assert(sizeof(ArcRec) == 0x40);

// work struct, 0x60 bytes (140422f10)
struct Work {
    std::uint64_t u00;       // 0x00 the argument (0)
    std::int32_t in_entities;// 0x08
    std::int32_t u0c;        // 0x0c = 1
    std::int32_t n_lines;    // 0x10
    std::int32_t pad_14;
    LineRec* lines;          // 0x18
    std::int32_t n_polys;    // 0x20
    std::int32_t pad_24;
    PolyRec* polys;          // 0x28
    std::int32_t n_arcs;     // 0x30
    std::int32_t pad_34;
    ArcRec* arcs;            // 0x38
    std::int32_t u40;        // 0x40 count of the +0x48 array (never filled by the reader)
    std::int32_t pad_44;
    void* u48;               // 0x48
    PolyRec* cur_poly;       // 0x50 current polyline
    std::int32_t in_poly;    // 0x58
    std::int32_t pad_5c;
};
static_assert(sizeof(Work) == 0x60);
static_assert(offsetof(Work, in_entities) == 0x08);
static_assert(offsetof(Work, lines) == 0x18);
static_assert(offsetof(Work, polys) == 0x28);
static_assert(offsetof(Work, arcs) == 0x38);
static_assert(offsetof(Work, cur_poly) == 0x50);
static_assert(offsetof(Work, in_poly) == 0x58);

// group-code table entry, 16 bytes {char* code, int idx}
struct CodeEntry {
    const char* code;
    std::int32_t idx;
};
// entity table entry, 16 bytes {char* name, handler}
using Handler = std::int32_t (*)(std::FILE* f, Work* w);
struct EntityEntry {
    const char* name;
    Handler handler;
};
extern const EntityEntry k_entity_table[10];  // 0x14062bdb0

// helpers
std::int32_t fun_140428650(const char* a, const char* b);   // case-insensitive compare
char* fun_140427df0(char* s, std::int32_t skip_lead);         // trim
std::int32_t fun_140424c80(const char* code, const CodeEntry* table, std::int32_t n,
                           std::int32_t* idx, std::int32_t* term);
std::int32_t fun_140428b10(const char* name);                // fopen "rt" test
Work* fun_140422f10(std::uint64_t arg);
void fun_140424390(Work** w);
std::int32_t fun_140422f70(std::FILE* f, Work* w);           // pair loop
std::int32_t fun_1404231c0(std::int32_t* n_seg, std::int32_t* n_pts, double** data, Work* w);
std::int32_t fun_140422ab0(const char* name, std::int32_t* n_pts, std::int32_t* n_seg, double** data);
double* fun_140422d40(std::int32_t n);
void fun_140422d90(double* p);
// _OBJ_GEO2D helpers in this packet
std::int64_t fun_140277420(def::model::OBJ_GEO2D* g, std::int32_t n, const double* data);
std::int64_t fun_14027ba50(def::model::OBJ_GEO2D* g, double* size);

// small vectors (by value, like the binary's 0x18 / 0x0c byte structs)
struct Vec3 {
    double x, y, z;
};
struct Vec2f {
    float x, y, z;  // 0x0c bytes; z is never written (copied as garbage in the original)
};
Vec2f fun_1403fb460(float x, float y);
Vec2f fun_1403078f0(Vec2f start, Vec2f center, float angle, std::int32_t degrees);

// entity handlers
std::int32_t fun_1404245b0(std::FILE* f, Work* w);  // SECTION
std::int32_t fun_1404246c0(std::FILE* f, Work* w);  // ENDSEC
std::int32_t fun_140424700(std::FILE* f, Work* w);  // LINE
std::int32_t fun_140424da0(std::FILE* f, Work* w);  // POLYLINE
std::int32_t fun_1404251e0(std::FILE* f, Work* w);  // LWPOLYLINE
std::int32_t fun_1404259c0(std::FILE* f, Work* w);  // VERTEX
std::int32_t fun_140425cf0(std::FILE* f, Work* w);  // SEQEND
std::int32_t fun_140425d50(std::FILE* f, Work* w);  // ARC
std::int32_t fun_1404262e0(std::FILE* f, Work* w);  // CIRCLE
std::int32_t fun_140426810(std::FILE* f, Work* w);  // SPLINE

std::int32_t fun_140409ae0();

} // namespace def::cmd::dxf
