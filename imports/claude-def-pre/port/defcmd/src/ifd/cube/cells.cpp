// cells.cpp: the containers cfnCUBE is built on: the defCELL record lists, the defDPT point set,
// the 16-byte element arrays, the C allocation wrappers and the fatal-error routine.
// Spec: docs/re/commands/OBJPOS_interference_kernels.md section 2.
#include <defcmd/ifd/cube_lib.hpp>

#include <defmodel/bc/boundary_topology.hpp>

#include <defrt/stdlib/legacy_printf.hpp>

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace def::cmd::cube {

namespace {
// FUN_14047903c() + 0x30 is &_iob[1], the CRT's stdout.
std::FILE* out() { return stdout; }
} // namespace

// @src 14039d450  fatal error: the message through 14039d570, the SFTC note, exit(1)
// @src 14039d570  vsprintf into a 4096-byte buffer, strcat "\n", fflush(stdout), fputs, fflush(NULL)
// @src 14039d2d0  exit(code) (FUN_14047ab18)
void fun_14039d450(const char* fmt, ...)
{
    char buf[4096];
    std::va_list ap;
    va_start(ap, fmt);
    def::rt::crt::vsnprintf(buf, sizeof buf, fmt, ap); // FUN_14048104c (vsprintf)
    va_end(ap);
    std::strcat(buf, "\n"); // FUN_140484ef0 with DAT_1406111c4
    std::fflush(out());
    std::fputs(buf, out());
    std::fflush(nullptr);
    // str 1406111d0
    std::fprintf(out(), "\tThe above error should not have happened\n\tIf this error occurs repeatedly,\n"
                        "\tPlease contact SFTC and report the conditions\n\tunder which it occurs.\n");
    std::exit(1);
}

// 14039d3a0: ported in defmodel/bc/boundary_topology.hpp (bc::alloc_message)
void fun_14039d3a0(const char* fmt, const char* a, const char* b, std::int32_t n)
{
    model::bc::alloc_message(fmt, a, b, n);
}

// 140256dc0: ported in defmodel/bc/boundary_topology.hpp (bc::calloc_ints): calloc(n, 4), NULL for
// n == 0, message when calloc fails
std::int32_t* fun_140256dc0(std::int32_t n) { return model::bc::calloc_ints(n); }

// @src 140256e20  realloc(p, n * 4), NULL (p kept, not freed) for n == 0, message when it fails
std::int32_t* fun_140256e20(std::int32_t* p, std::int32_t n)
{
    if (n == 0) {
        return nullptr;
    }
    void* q = std::realloc(p, static_cast<std::size_t>(static_cast<std::int64_t>(n) << 2));
    if (q == nullptr) {
        // str 1405e4e90, 1405e4e80, 1405e4e3c
        fun_14039d3a0("%s Error when re-allocating %s memory of size %d\n", "ERROR    ", "INT", n);
    }
    return static_cast<std::int32_t*>(q);
}

// 140257150: ported in defmodel/bc/boundary_topology.hpp (bc::calloc_doubles): calloc(n, 8), NULL
// for n == 0, message when calloc fails
double* fun_140257150(std::int32_t n) { return model::bc::calloc_doubles(n); }

// ---- defCELL -----------------------------------------------------------------------------------

// @src 1404050b0  init a record list: count 0, cap, width (twice), increment, data = cap*width ints
void fun_1404050b0(CellList* c, std::int32_t cap, std::int32_t inc, std::int32_t width)
{
    c->count = 0;
    c->cap = cap;
    c->width = width;
    c->width2 = width;
    c->inc = inc;
    c->data = nullptr;
    if (0 < cap) {
        c->data = fun_140256dc0(c->cap * c->width);
    }
}

// @src 140405150  reset: count = cap = 0, data freed (width and increment kept)
void fun_140405150(CellList* c)
{
    c->count = 0;
    c->cap = 0;
    if (c->data != nullptr) {
        std::free(c->data);
    }
    c->data = nullptr;
}

// @src 140405350  grow: cap += grow, data = realloc(data, cap * width ints)
void fun_140405350(CellList* c, std::int32_t grow)
{
    c->cap = c->cap + grow;
    c->data = fun_140256e20(c->data, c->cap * c->width);
}

// @src 140405540  defCELL_InsertCell: append one record of npts ints (grow by inc when full)
std::int32_t fun_140405540(CellList* c, std::int32_t npts, const std::int32_t* rec)
{
    if (c->width < npts) {
        // str 140627448. The binary passes no arguments: EDX still holds npts and R8 the record
        // pointer, so the second %d prints the pointer's low 32 bits.
        fun_14039d450("defCELL_InsertCell : npts %d > CellSize %d, not valid\n", npts,
                      static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(rec)));
    }
    if (c->count < c->cap) {
        const std::int32_t base = c->count * c->width;
        for (std::int32_t i = 0; i < npts; ++i) {
            c->data[base + i] = rec[i];
        }
        c->count = c->count + 1;
    } else {
        fun_140405350(c, c->inc);
        fun_140405540(c, npts, rec);
    }
    return 0;
}

// @src 140405720  defCELL_GetCellData: record k (1-based); NULL when the list is empty
std::int32_t* fun_140405720(CellList* c, std::int32_t k)
{
    if (k < 1 || c->count < k) {
        fun_14039d450("defCELL_GetCellData : %d not valid, Max %d\n", k, c->count); // str 140627480
    }
    if (c->count == 0) {
        return nullptr;
    }
    return c->data + (k - 1) * c->width;
}

// @src 1404056c0  record count
std::int32_t fun_1404056c0(const CellList* c) { return c->count; }

// @src 1404056e0  record width
std::int32_t fun_1404056e0(const CellList* c) { return c->width; }

// @src 1404057a0  calloc n record lists (0x20 bytes each), NULL for n == 0
CellList* fun_1404057a0(std::int32_t n)
{
    if (n == 0) {
        return nullptr;
    }
    void* p = std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(n)), 0x20);
    if (p == nullptr) {
        // str 1406274d0, 1406274c0, 1406274b0
        fun_14039d3a0("%s Error when allocating %s memory of size %d\n", "ERROR    ", "CELL_DATA", n);
    }
    return static_cast<CellList*>(p);
}

// ---- defDPT ------------------------------------------------------------------------------------

// @src 140405c80  init a point set: count 0, cap, increment, dirty, bounds 0, data = cap*3 doubles
void fun_140405c80(PointSet* p, std::int32_t cap, std::int32_t inc)
{
    p->count = 0;
    p->cap = cap;
    p->inc = inc;
    p->data = nullptr;
    p->dirty = 1;
    for (int i = 0; i < 6; ++i) {
        p->bounds[i] = 0.0;
    }
    if (0 < cap) {
        p->data = fun_140257150(p->cap * 3);
    }
}

// @src 1404062e0  point count
std::int32_t fun_1404062e0(const PointSet* p) { return p->count; }

// @src 140406300  defDPT_GetPoint: point k (1-based)
double* fun_140406300(PointSet* p, std::int32_t k)
{
    if (k < 1 || p->count < k) {
        fun_14039d450("defDPT_GetPoint : %d not valid, Max %d\n", k, p->count); // str 140627640
    }
    return p->data + (k - 1) * 3;
}

// @src 140406360  bounds (recomputed when dirty): start 1e29 / -1e29, strict compares
double* fun_140406360(PointSet* p)
{
    if (p->dirty != 0) {
        p->bounds[4] = 1e29; // DAT_14052c598
        p->bounds[2] = 1e29;
        p->bounds[0] = 1e29;
        p->bounds[5] = -1e29; // DAT_14052c5a0
        p->bounds[3] = -1e29;
        p->bounds[1] = -1e29;
        for (std::int32_t i = 0; i < fun_1404062e0(p); ++i) {
            const double* q = fun_140406300(p, i + 1);
            for (int c = 0; c < 3; ++c) {
                if (p->bounds[c * 2] > q[c]) { // comisd min, q; jbe
                    p->bounds[c * 2] = q[c];
                }
                if (q[c] > p->bounds[c * 2 + 1]) { // comisd q, max; jbe
                    p->bounds[c * 2 + 1] = q[c];
                }
            }
        }
        p->dirty = 0;
    }
    return p->bounds;
}

// @src 140406510  bounds of the point set (140406360)
double* fun_140406510(PointSet* p)
{
    fun_140406360(p);
    return p->bounds;
}

// ---- element arrays (cube +0x180 / +0x190) -----------------------------------------------------

// @src 1404047f0  calloc n elements of 16 bytes, NULL for n < 1
ElemLists* fun_1404047f0(std::int32_t n)
{
    if (n < 1) {
        return nullptr;
    }
    void* p = std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(n)), 0x10);
    if (p == nullptr) {
        // str 140627268, 140627258, 140627240
        fun_14039d3a0("%s Error when allocating %s memory of size %d\n", "ERROR    ", "Polygon/Element Data", n);
    }
    return static_cast<ElemLists*>(p);
}

// @src 1404044c0  allocate n elements and clear both list heads
void fun_1404044c0(ElemArray* a, std::int32_t n)
{
    a->count = n;
    a->data = fun_1404047f0(n);
    if (a->data != nullptr) {
        for (std::int32_t i = 0; i < n; ++i) {
            a->data[i].a = nullptr;
            a->data[i].b = nullptr;
        }
    }
}

// @src 140404c40  free a linked list (each node's data, then the node)
void fun_140404c40(ListNode* p)
{
    while (p != nullptr) {
        ListNode* next = p->next;
        if (p->data != nullptr) {
            std::free(p->data);
        }
        p->data = nullptr;
        p->next = nullptr;
        std::free(p);
        p = next;
    }
}

// @src 140405010  free a linked list (same code as 140404c40)
void fun_140405010(ListNode* p)
{
    while (p != nullptr) {
        ListNode* next = p->next;
        if (p->data != nullptr) {
            std::free(p->data);
        }
        p->data = nullptr;
        p->next = nullptr;
        std::free(p);
        p = next;
    }
}

// @src 140404850  free both lists of each of n elements, then the array
void fun_140404850(ElemLists* p, std::int32_t n)
{
    for (std::int32_t i = 0; i < n; ++i) {
        fun_140404c40(p[i].a);
        fun_140405010(p[i].b);
        p[i].a = nullptr;
        p[i].b = nullptr;
    }
    if (p != nullptr) {
        std::free(p);
    }
}

// @src 1404047a0  free an element array, count = 0
void fun_1404047a0(ElemArray* a)
{
    fun_140404850(a->data, a->count);
    a->count = 0;
    a->data = nullptr;
}

// @src 140404780  element count
std::int32_t fun_140404780(const ElemArray* a) { return a->count; }

} // namespace def::cmd::cube
