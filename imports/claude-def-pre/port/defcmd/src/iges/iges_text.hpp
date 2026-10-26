// The DEF_IGS.OUT text-route data (packet p8-03): the 0x88-byte loop/segment/point object that
// FUN_140455e60 fills and FUN_14040b100 turns into curves. Reached when the segment table cannot
// be allocated (mode 300) or always after mode 400 (IGES_READER.md §8).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model {
class LLOBJ_DATA;
}

namespace def::cmd::iges {

class Uic2CObjectIgesFile;

struct IgsLoop {             // 0x30 (140454d00)
    std::int32_t n_seg;      // 0x00
    std::byte pad_04[4];
    std::int32_t* seg;       // 0x08 segment indices (0-based)
    float bb_min[3];         // 0x10
    float bb_max[3];         // 0x1c
    std::uint8_t f28[7];     // 0x28 0, 0, 1, 1, 1, 1, 1
    std::byte pad_2f[1];
};
static_assert(sizeof(IgsLoop) == 0x30);

struct IgsSeg {              // 0x20 (140454da0)
    std::uint8_t b00;        // 0x00 0
    std::byte pad_01[3];
    std::int32_t type;       // 0x04 1 line, 2 arc
    std::int32_t p1;         // 0x08 first point (0-based)
    std::int32_t p2;         // 0x0c second point (line end / arc centre)
    std::int32_t pm;         // 0x10 arc mid point (-1 for lines)
    std::byte pad_14[4];
    double a;                // 0x18 arc angle in degrees (0 for lines)
};
static_assert(sizeof(IgsSeg) == 0x20);

struct IgsItem {             // 0x10 (140454e00)
    std::int32_t v[4];
};

struct IgsText {             // 0x88
    std::int32_t n_loops;    // 0x00
    std::byte pad_04[4];
    IgsLoop* loops;          // 0x08
    std::int32_t n_seg;      // 0x10
    std::int32_t seg_cap;    // 0x14
    IgsSeg* segs;            // 0x18
    std::int32_t n_pts;      // 0x20
    std::int32_t pt_cap;     // 0x24
    double* pts;             // 0x28 x, y per point
    float bb_min[3];         // 0x30
    float bb_max[3];         // 0x3c
    float bb_min2[3];        // 0x48
    float bb_max2[3];        // 0x54
    std::int32_t u60;        // 0x60 0
    std::int32_t u64;        // 0x64 1
    std::int32_t u68;        // 0x68 0
    std::int32_t u6c;        // 0x6c 0
    std::int32_t u70, u74, u78, u7c; // 0x70..0x7c 0
    IgsItem* items;          // 0x80
};
static_assert(sizeof(IgsText) == 0x88);
DEF_LAYOUT_OFFSET(IgsText, segs, 0x18);
DEF_LAYOUT_OFFSET(IgsText, pts, 0x28);
DEF_LAYOUT_OFFSET(IgsText, bb_max2, 0x54);
DEF_LAYOUT_OFFSET(IgsText, items, 0x80);

void fun_140454bc0(IgsText* t);
void fun_140454d00(IgsLoop* l);
void fun_140454da0(IgsSeg* s);
void fun_140454e00(IgsItem* it);
void fun_140454e40(IgsText* t);
void fun_140454ee0(IgsText* t, std::int32_t n, std::int32_t idx, std::int32_t what);
void fun_140455150(IgsText* t, std::int32_t old_n, std::int32_t n, std::int32_t idx, std::int32_t what);
void fun_1404553c0(IgsText* t, std::int32_t idx, std::int32_t what);
void fun_1404557a0(IgsText* t);
void fun_140455aa0(IgsText* t, std::int32_t start, std::int32_t all);
std::int32_t fun_140455e60(IgsText* t, const char* name);
std::int32_t fun_1404564c0(IgsText* t, const double* p);
void fun_1404565c0(IgsText* t, std::int32_t type, std::int32_t p1, std::int32_t p2, std::int32_t pm, double a);
void fun_140456720(IgsText* t, std::int32_t v);
void fun_140456770(IgsText* t, std::int32_t v);
void fun_140456790(IgsText* t, std::int32_t v);
void fun_1404567a0(IgsText* t, std::int32_t v);
void fun_14046a810(const double* p, std::int32_t n, float* bb_min, float* bb_max);
std::int32_t fun_14046aa40(const double* p, std::int32_t n, const double* q, double tol);
void fun_14046b2d0(const double* p, const double* c, const double* a, double* out);
void fun_140476bb1(const double* p, const double* c, const double* a, double* out);
std::int32_t fun_14040b100(Uic2CObjectIgesFile* iges, def::model::LLOBJ_DATA* obj, std::int32_t kind);

} // namespace def::cmd::iges
