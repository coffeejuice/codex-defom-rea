// kw-g2obj: KEY write handlers (registry column 2) and data accessors (column 3) of this
// packet's keywords, the direct writers FUN_14029e250 calls (OBJTYP, OBJNAM, OTPRNG line) and the
// movement block FUN_1402a7c80. Decompiles: deform-rea-work/out/b_kwall.c, out/g2/objw.c,
// out/g2/writers.c, out/g2/K_x1.c.
//
// Every handler starts like the original: n = ref[0]; obj = FUN_14017aaf0(model+0x1380, n) (not
// checked for null, as in the binary); n = obj+0x3f24 when that is > 0 (renumbering).
#include <cmath>
#include <cstdio>
#include <cstring>

#include <defrt/stdlib/libm.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defmodel/keyword_registry.hpp>
#include <defrt/rdata.hpp>

#include "dispatch.hpp"
#include "internal.hpp"

namespace def::key::g2obj {

namespace {

// rdata 14051bed8  0.0
constexpr double kZero = def::rt::rdata::d_14051bed8;
// rdata 14051f260  1e-12
constexpr double kTiny = def::rt::rdata::d_14051f260;
// rdata 140522fd8  180/pi
constexpr double kRadToDeg = def::rt::rdata::d_140522fd8;

const char* kw(int id) { return model::keyword_name(id); }

struct Target {
    LLOBJ_DATA* obj;
    std::int32_t n;
};

// the common prologue of the object handlers
Target target(DB_STEP_DATA& m, const KeyRef& ref)
{
    std::int32_t n = ref[0];
    LLOBJ_DATA* obj = find_object(m, n);
    if (0 < at<std::int32_t>(*obj, 0x3f24)) n = at<std::int32_t>(*obj, 0x3f24);
    return {obj, n};
}

// FUN_140484ab8
std::int32_t iabs(std::int32_t v)
{
    return v < 0 ? static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(v)) : v;
}

// @src 140309230  length of a 2-vector; 0 when the squared length is exactly 0.0
double length2(const double* v)
{
    double s = v[0] * v[0] + v[1] * v[1];
    if (s == kZero) return 0.0;
    return std::sqrt(s);  // FUN_140484644
}

// length of a 3-vector, as FUN_14023b610 (p1-12) computes it; 0 when the squared length is 0.0
double length3(const double* v)
{
    double s = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    if (s == kZero) return 0.0;
    return std::sqrt(s);
}

// @src 140198c80  "object has a stop vector" (OBJSTP is written): false for internal type 0;
// 2D: |obj+0x2a00 (2)| > 1e-12; 3D: |obj+0x2a00 (3)| > 1e-12; other dimensions: true.
bool has_object_stop(const LLOBJ_DATA& obj)
{
    if (at<std::int32_t>(obj, 0xc) == 0) return false;
    const DB_STEP_DATA& m = *at<DB_STEP_DATA*>(obj, 15000);  // obj+0x3a98 owner model
    if (!is_2d(m)) {
        if (is_3d(m) && length3(&at<double>(obj, 0x2a00)) <= kTiny) return false;
    } else {
        if (length2(&at<double>(obj, 0x2a00)) <= kTiny) return false;
    }
    return true;
}

// @src 140186da0  number of entries of type 6 in the list at obj+0x1ea0+0x200 (WPAXIS records,
// 0xe8 bytes, linked at +0xe0; count FUN_14019a530 -> FUN_14019a550 = list+0x10, n-th
// FUN_140259b60)
int count_type6_axes(const LLOBJ_DATA& obj)
{
    const std::size_t list = 0x1ea0 + 0x200;
    int count = at<std::int32_t>(obj, list + 0x10);
    int k = 0;
    if (count < 1) return 0;
    for (int i = 1; i <= count; ++i) {
        // n-th record, as 140259b60 (llobj_copy_lists.cpp) walks it: the head for n <= 1
        const std::byte* e = at<const std::byte*>(obj, list);
        int j = 0;
        while (e != nullptr && (++j, j < i)) e = *reinterpret_cast<const std::byte* const*>(e + 0xe0);
        if (*reinterpret_cast<const std::int32_t*>(e + 4) == 6) ++k;
    }
    return k;
}

int write_scalar(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref, int id, std::size_t off)
{
    Target t = target(m, ref);
    rt::crt::fprintf(f, "%-6s %7i %20.10E\n", kw(id), t.n, at<double>(*t.obj, off));
    return 0;
}

// The two-method angular table writers ANGMOV (1402b0610) and ANGMO2 (1402afe80).
int write_angular(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref, int id, std::size_t moff,
                  std::size_t t0, std::size_t t1)
{
    Target t = target(m, ref);
    LLOBJ_DATA& obj = *t.obj;
    std::int32_t method = at<std::int32_t>(obj, moff);
    if (method != 0 && method != 1) return 0;
    DYNARY* tab = table_at(obj, method == 0 ? t0 : t1);
    if (tab->ndim == 0) {
        rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E\n", kw(id), t.n, method, 0, tab->value);
    } else {
        if (tab->ndim != 1) return 3;
        int k;
        if (tab->var[0] == 1) {
            k = 1;
        } else {
            if (tab->var[0] != 10) return 3;
            k = 2;
        }
        rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", kw(id), t.n, method, k, tab->n[0]);
    }
    io::table_write(f, tab, 0);
    return 0;
}

// Sub field and value of the MOVCTL types 0, 7, 8 (T.ndim 0 -> 0 and constant, ndim 1 -> 1 for
// var 2, 2 for var 1, then the count). 0 = constant form, 1 = table form, 3 = unknown shape.
int sub_form_078(const DYNARY* t, int* k)
{
    if (t->ndim == 0) return 0;
    if (t->ndim != 1) return 3;
    if (t->var[0] == 2) {
        *k = 1;
    } else {
        if (t->var[0] != 1) return 3;
        *k = 2;
    }
    return 1;
}

} // namespace

// ---- object header ------------------------------------------------------------------------

// @src 1402a25a0  OBJTYP: internal type 0..6 -> code 1..7, 7 -> 0, other -> 3 (nothing written)
int write_objtyp(DB_STEP_DATA&, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t no)
{
    std::int32_t code;
    switch (at<std::int32_t>(obj, 0xc)) {
    case 0: code = 1; break;
    case 1: code = 2; break;
    case 2: code = 3; break;
    case 3: code = 4; break;
    case 4: code = 5; break;
    case 5: code = 6; break;
    case 6: code = 7; break;
    case 7: code = 0; break;
    default: return 3;
    }
    rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", kw(0x91), no, code, at<std::int32_t>(obj, 0x2154));
    return 0;
}

// @src 1402a2530  OBJNAM
int write_objnam(DB_STEP_DATA&, std::FILE* f, LLOBJ_DATA* obj, std::uint32_t no)
{
    if (obj != nullptr) rt::crt::fprintf(f, "%-6s %7i\n%s\n", kw(0x90), no, &at<char>(*obj, 0x828));
    return 0;
}

// @src 1402af420  AVGSTR
int write_avgstr(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref) { return write_scalar(f, m, ref, 0x92, 0x1ea8); }
// @src 1402af4f0  LMTSTR
int write_lmtstr(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref) { return write_scalar(f, m, ref, 0x93, 0x1ea0); }
// @src 1402af350  REFTMP
int write_reftmp(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref) { return write_scalar(f, m, ref, 0x96, 0x1eb8); }
// @src 1402af6a0  TMPLMT
int write_tmplmt(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref) { return write_scalar(f, m, ref, 0xca, 0x1f10); }
// @src 1402b49d0  RMDPTH
int write_rmdpth(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref) { return write_scalar(f, m, ref, 0xb4, 0x2dc8); }
// @src 1402b4aa0  RMSTRK
int write_rmstrk(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref) { return write_scalar(f, m, ref, 0xb5, 0x2dd0); }
// @src 1402b4b70  RMTIME
int write_rmtime(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref) { return write_scalar(f, m, ref, 0xb6, 0x2de0); }

// @src 1402af5c0  ZSTR (2D)
int write_zstr(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    Target t = target(m, ref);
    rt::crt::fprintf(f, "%-6s %7i %7i %20.10E\n", kw(0x94), t.n, at<std::int32_t>(*t.obj, 0x1f30),
                 at<double>(*t.obj, 0x1f28));
    return 0;
}

// @src 1402b4400  PENVOL, 2D form (the 3D line is written inline by FUN_14029e250)
int write_penvol(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    Target t = target(m, ref);
    rt::crt::fprintf(f, "%-6s %7i %20.10E %7i\n", kw(0x95), t.n, at<double>(*t.obj, 0x1eb0),
                 at<std::int32_t>(*t.obj, 0x1f20));
    return 0;
}

// @src 1402abd50  OTPRNG line (mode 3 is written as -(obj+0x1ef0))
int write_otprng_line(DB_STEP_DATA&, std::FILE* f, LLOBJ_DATA* obj, std::uint32_t no)
{
    if (f != nullptr && obj != nullptr) {
        std::int32_t mode = at<std::int32_t>(*obj, 0x1eec) == 3
                                ? -at<std::int32_t>(*obj, 0x1ef0)
                                : at<std::int32_t>(*obj, 0x1eec);
        rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E %7i\n", kw(0xf0), no, mode,
                     at<double>(*obj, 0x1ef8), at<double>(*obj, 0x1f00),
                     at<std::int32_t>(*obj, 0x1f08));
    }
    return 0;
}

// @src 1402af770  OTPRNG handler: nothing when model+0x1744 & 0x20000
int write_otprng(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    Target t = target(m, ref);
    if ((writer_flags(m) & 0x20000) == 0) {
        write_otprng_line(m, f, t.obj, static_cast<std::uint32_t>(t.n));
    }
    return 0;
}

// @src 1402af830  CSFREQ (T@1f60): constant, or a table over var 1 / 0x1c / 0x1d; else 3
int write_csfreq(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    Target t = target(m, ref);
    DYNARY* tab = table_at(*t.obj, 0x1f60);
    if (tab->ndim == 0) {
        rt::crt::fprintf(f, "%-6s %7i %7i %20.10E\n", kw(0xeb), t.n, 0, tab->value);
    } else if (tab->ndim == 1 && tab->var[0] == 1) {
        rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", kw(0xeb), t.n, 1, tab->n[0]);
    } else if (tab->ndim == 1 && tab->var[0] == 0x1c) {
        rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", kw(0xeb), t.n, 2, tab->n[0]);
    } else {
        if (tab->ndim != 1 || tab->var[0] != 0x1d) return 3;
        rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", kw(0xeb), t.n, 3, tab->n[0]);
    }
    io::table_write(f, tab, 0);
    return 0;
}

// @src 1402afb10  VOLCRG (class +0x1fd8 -> base 0/10/20, T@1fe0)
int write_volcrg(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    Target t = target(m, ref);
    // The original leaves `base` uninitialised for a class other than 0/1/2 (the reader never
    // stores one); the port starts it at 0.
    std::int32_t base = 0;
    if (at<std::int32_t>(*t.obj, 0x1fd8) == 0) {
        base = 0;
    } else if (at<std::int32_t>(*t.obj, 0x1fd8) == 1) {
        base = 10;
    } else if (at<std::int32_t>(*t.obj, 0x1fd8) == 2) {
        base = 0x14;
    }
    DYNARY* tab = table_at(*t.obj, 0x1fe0);
    if (tab->ndim == 0) {
        rt::crt::fprintf(f, "%-6s %7i %7i %20.10E\n", kw(0xec), t.n, base, tab->value);
    } else {
        if (tab->ndim != 1 || tab->var[0] != 1) return 3;
        rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", kw(0xec), t.n, base + 1, tab->n[0]);
    }
    io::table_write(f, tab, 0);
    return 0;
}

// ---- movement block -----------------------------------------------------------------------

// @src 14018c810  2D movement angle: atan2(+0x58, +0x50) of the sub-structure obj+0x2160, in degrees
double movement_angle_2d(const LLOBJ_DATA& obj)
{
    // the static CRT atan2 140485d6c, then mulsd [140522fd8]
    return def::rt::libm::crt_atan2(at<double>(obj, 0x2160 + 0x58), at<double>(obj, 0x2160 + 0x50)) * kRadToDeg;
}

// @src 1402b1400  MOVCTL
int write_movctl(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    Target tg = target(m, ref);
    LLOBJ_DATA& o = *tg.obj;
    const std::int32_t n = tg.n;
    const char* name = kw(0x97);
    const double dx = at<double>(o, 0x21b0), dy = at<double>(o, 0x21b8), dz = at<double>(o, 0x21c0);
    const bool neg = at<std::int32_t>(o, 0x2780) != 0;
    const std::int32_t negsub = neg ? -iabs(at<std::int32_t>(o, 0x2784)) : 0;
    int k = 0;
    if (!is_2d(m)) {
        switch (at<std::int32_t>(o, 0x21c8)) {
        case 0: {
            DYNARY* t = table_at(o, 0x21d0);
            if (neg) {
                rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E %20.10E %20.10E\n", name, n, 0, negsub,
                             dx, dy, dz);
                break;
            }
            int form = sub_form_078(t, &k);
            if (form == 3) return 3;
            if (form == 0) {
                rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E %20.10E %20.10E %20.10E\n", name, n, 0,
                             0, dx, dy, dz, at<double>(o, 0x21d8));
            } else {
                rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E %20.10E %20.10E %7i\n", name, n, 0, k,
                             dx, dy, dz, at<std::int32_t>(o, 0x21ec));
                io::table_write(f, t, 0);
            }
            break;
        }
        case 1: {
            DYNARY* t = table_at(o, 0x2248);
            if (neg) {
                rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E %20.10E %20.10E\n", name, n, 1, negsub,
                             dx, dy, dz);
            } else if (0 < at<std::int32_t>(o, 0x22c0)) {
                rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E %20.10E %20.10E %20.10E\n", name, n, 1,
                             at<std::int32_t>(o, 0x22c0) + 1000, dx, dy, dz, at<double>(o, 0x22c8));
            } else if (t->ndim == 0) {
                rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E %20.10E %20.10E %20.10E\n", name, n, 1,
                             0, dx, dy, dz, at<double>(o, 0x2250));
            } else {
                if (t->ndim != 1) return 3;
                if (t->var[0] == 2) {
                    k = 1;
                } else if (t->var[0] == 1) {
                    k = 2;
                } else {
                    if (t->var[0] != 5) return 3;
                    k = 4;
                }
                rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E %20.10E %20.10E %7i\n", name, n, 1, k,
                             dx, dy, dz, at<std::int32_t>(o, 0x2264));
                io::table_write(f, t, 0);
            }
            break;
        }
        case 2: {
            DYNARY* t = table_at(o, 0x2368);
            if (t->ndim == 0) {
                rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E %20.10E\n", name, n, 2, dx, dy, dz);
                rt::crt::fprintf(f, " %20.10E %20.10E %20.10E %7i\n", at<double>(o, 0x2350),
                             at<double>(o, 0x2358), at<double>(o, 0x2348),
                             at<std::int32_t>(o, 0x2360));
            } else if (t->ndim == 1) {
                if (t->var[0] != 5) return 3;
                rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E %20.10E\n", name, n, 2, dx, dy, dz);
                rt::crt::fprintf(f, " %20.10E %20.10E %20.10E %7i %7i %7i\n", at<double>(o, 0x2350),
                             at<double>(o, 0x2358), at<double>(o, 0x2348),
                             at<std::int32_t>(o, 0x2360), 4, at<std::int32_t>(o, 0x2384));
                io::table_write(f, t, 0);
            }
            break;
        }
        case 3: {
            DYNARY* t = table_at(o, 0x2400);
            if (t->ndim == 0) {
                rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E %20.10E\n", name, n, 3, dx, dy, dz);
                rt::crt::fprintf(f, " %20.10E %20.10E %20.10E %20.10E\n", at<double>(o, 0x23e0),
                             at<double>(o, 0x23e8), at<double>(o, 0x23f0), at<double>(o, 0x23f8));
            } else if (t->ndim == 1) {
                if (t->var[0] != 5) return 3;
                rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E %20.10E\n", name, n, 3, dx, dy, dz);
                rt::crt::fprintf(f, " %20.10E %20.10E %20.10E %20.10E %7i %7i\n",
                             at<double>(o, 0x23e0), at<double>(o, 0x23e8), at<double>(o, 0x23f0),
                             at<double>(o, 0x23f8), 4, at<std::int32_t>(o, 0x241c));
                io::table_write(f, t, 0);
            }
            break;
        }
        case 4: {
            std::int32_t cnt = at<std::uint8_t>(o, 0x2490) == 0 ? 0 : at<std::int32_t>(o, 0x24b4);
            rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E %20.10E\n", name, n, 4, dx, dy, dz);
            rt::crt::fprintf(f, " %20.10E %20.10E %20.10E %7i\n", at<double>(o, 0x2478),
                         at<double>(o, 0x2480), at<double>(o, 0x2488), cnt);
            if (at<std::uint8_t>(o, 0x2490) != 0) io::table_write(f, table_at(o, 0x2498), 0);
            break;
        }
        case 5: {
            DYNARY* t = table_at(o, 0x2510);
            rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E %20.10E\n", name, n, 5, dx, dy, dz);
            if (t->ndim == 0) {
                rt::crt::fprintf(f, " %7i %20.10E %20.10E %20.10E %20.10E %7i %7i\n", 0,
                             at<double>(o, 0x2518), at<double>(o, 0x2588), at<double>(o, 0x2590),
                             at<double>(o, 0x2598), at<std::int32_t>(o, 0x25a0),
                             at<std::int32_t>(o, 0x25a4));
            } else {
                if (t->ndim != 1) return 3;
                rt::crt::fprintf(f, " %7i %7i %20.10E %20.10E %20.10E %7i %7i\n", 1,
                             at<std::int32_t>(o, 0x252c), at<double>(o, 0x2588),
                             at<double>(o, 0x2590), at<double>(o, 0x2598),
                             at<std::int32_t>(o, 0x25a0), at<std::int32_t>(o, 0x25a4));
                io::table_write(f, t, 0);
            }
            break;
        }
        case 6: {
            if (at<std::int32_t>(o, 0x2670) == 0) {
                k = at<std::int32_t>(o, 0x25b8) == 0x1a ? 2 : 1;
                rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i %7i\n", name, n, 6,
                             at<std::int32_t>(o, 0x2620), k, at<std::int32_t>(o, 0x2624),
                             at<std::int32_t>(o, 0x25c4));
            } else {
                rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i %7i\n", name, n, 6,
                             at<std::int32_t>(o, 0x2620), -iabs(at<std::int32_t>(o, 0x2674)),
                             at<std::int32_t>(o, 0x2624), 0);
            }
            if (at<std::int32_t>(o, 0x2670) == 0) {
                if (at<std::int32_t>(o, 0x2620) == 2) {
                    rt::crt::fprintf(f, " %20.10E %20.10E %20.10E\n", at<double>(o, 0x2628),
                                 at<double>(o, 0x2630), at<double>(o, 0x2638));
                    rt::crt::fprintf(f, " %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E\n",
                                 at<double>(o, 0x2640), at<double>(o, 0x2648),
                                 at<double>(o, 0x2650), at<double>(o, 0x2658),
                                 at<double>(o, 0x2660), at<double>(o, 0x2668));
                }
                io::table_write(f, table_at(o, 0x25a8), 0);
            }
            break;
        }
        case 7: {
            DYNARY* t = table_at(o, 0x2678);
            const double p1 = at<double>(o, 0x2768), p2 = at<double>(o, 0x2770);
            if (neg) {
                rt::crt::fprintf(f,
                             "%-6s %7i %7i %7i %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E\n",
                             name, n, 7, negsub, dx, dy, dz, 0.0, p1, p2);
                break;
            }
            int form = sub_form_078(t, &k);
            if (form == 3) return 3;
            if (form == 0) {
                rt::crt::fprintf(f,
                             "%-6s %7i %7i %7i %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E\n",
                             name, n, 7, 0, dx, dy, dz, at<double>(o, 0x2680), p1, p2);
            } else {
                rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E %20.10E %20.10E %7i %20.10E %20.10E\n",
                             name, n, 7, k, dx, dy, dz, at<std::int32_t>(o, 0x2694), p1, p2);
                io::table_write(f, t, 0);
            }
            break;
        }
        case 8: {
            DYNARY* t = table_at(o, 0x26f0);
            const double p1 = at<double>(o, 0x2768);
            if (neg) {
                rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E %20.10E %20.10E %20.10E %20.10E\n",
                             name, n, 8, negsub, dx, dy, dz, 0.0, p1);
                break;
            }
            int form = sub_form_078(t, &k);
            if (form == 3) return 3;
            if (form == 0) {
                rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E %20.10E %20.10E %20.10E %20.10E\n",
                             name, n, 8, 0, dx, dy, dz, at<double>(o, 0x26f8), p1);
            } else {
                rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E %20.10E %20.10E %7i %20.10E\n", name, n,
                             8, k, dx, dy, dz, at<std::int32_t>(o, 0x270c), p1);
                io::table_write(f, t, 0);
            }
            break;
        }
        case 9:
            rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E %20.10E %20.10E\n", name, n, 9, dx, dy,
                         dz, at<double>(o, 0x2768));
            break;
        case 10:
            rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E %20.10E %20.10E %20.10E\n", name, n, 10,
                         dx, dy, dz, at<double>(o, 0x2768), at<double>(o, 0x2778));
            break;
        case 0xb:
            rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E %20.10E\n", name, n, 0xb,
                         at<double>(o, 0x22e8), at<double>(o, 0x22f0), at<double>(o, 0x22f8));
            break;
        default:
            break;
        }
        return 0;
    }

    const double a = movement_angle_2d(o);  // FUN_14018c810(obj+0x2160)
    switch (at<std::int32_t>(o, 0x21c8)) {
    case 0: {
        DYNARY* t = table_at(o, 0x21d0);
        if (neg) {
            rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E\n", name, n, 0, negsub, a);
            break;
        }
        int form = sub_form_078(t, &k);
        if (form == 3) return 3;
        if (form == 0) {
            rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E %20.10E\n", name, n, 0, 0, a,
                         at<double>(o, 0x21d8));
        } else {
            rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E %7i\n", name, n, 0, k, a,
                         at<std::int32_t>(o, 0x21ec));
            io::table_write(f, t, 0);
        }
        break;
    }
    case 1: {
        DYNARY* t = table_at(o, 0x2248);
        if (neg) {
            rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E\n", name, n, 1, negsub, a);
        } else if (0 < at<std::int32_t>(o, 0x22c0)) {
            rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E %20.10E\n", name, n, 1,
                         at<std::int32_t>(o, 0x22c0) + 1000, a, at<double>(o, 0x22c8));
        } else if (t->ndim == 0) {
            rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E %20.10E\n", name, n, 1, 0, a,
                         at<double>(o, 0x2250));
        } else {
            if (t->ndim != 1) return 3;
            if (t->var[0] == 2) {
                k = 1;
            } else if (t->var[0] == 1) {
                k = 2;
            } else {
                if (t->var[0] != 5) return 3;
                k = 4;
            }
            rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E %7i\n", name, n, 1, k, a,
                         at<std::int32_t>(o, 0x2264));
            io::table_write(f, t, 0);
        }
        break;
    }
    case 2: {
        DYNARY* t = table_at(o, 0x2368);
        if (t->ndim == 0) {
            rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E %20.10E %20.10E %7i\n", name, n, 2, a,
                         at<double>(o, 0x2350), at<double>(o, 0x2358), at<double>(o, 0x2348),
                         at<std::int32_t>(o, 0x2360));
        } else if (t->ndim == 1) {
            if (t->var[0] != 5) return 3;
            rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E %20.10E %20.10E %7i %7i %7i\n", name, n,
                         2, a, at<double>(o, 0x2350), at<double>(o, 0x2358), at<double>(o, 0x2348),
                         at<std::int32_t>(o, 0x2360), 4, at<std::int32_t>(o, 0x2384));
            io::table_write(f, t, 0);
        }
        break;
    }
    case 3: {
        DYNARY* t = table_at(o, 0x2400);
        if (t->ndim == 0) {
            rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E %20.10E %20.10E %20.10E\n", name, n, 3,
                         a, at<double>(o, 0x23e0), at<double>(o, 0x23e8), at<double>(o, 0x23f0),
                         at<double>(o, 0x23f8));
        } else if (t->ndim == 1) {
            if (t->var[0] != 5) return 3;
            rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E %20.10E %20.10E %20.10E %7i %7i\n", name,
                         n, 3, a, at<double>(o, 0x23e0), at<double>(o, 0x23e8),
                         at<double>(o, 0x23f0), at<double>(o, 0x23f8), 4,
                         at<std::int32_t>(o, 0x241c));
            io::table_write(f, t, 0);
        }
        break;
    }
    case 4: {
        std::int32_t cnt = at<std::uint8_t>(o, 0x2490) == 0 ? 0 : at<std::int32_t>(o, 0x24b4);
        rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E %20.10E %20.10E %7i\n", name, n, 4, a,
                     at<double>(o, 0x2478), at<double>(o, 0x2480), at<double>(o, 0x2488), cnt);
        if (at<std::uint8_t>(o, 0x2490) != 0) io::table_write(f, table_at(o, 0x2498), 0);
        break;
    }
    case 5: {
        DYNARY* t = table_at(o, 0x2510);
        rt::crt::fprintf(f, "%-6s %7i %7i %20.10E\n", name, n, 5, a);
        if (t->ndim == 0) {
            rt::crt::fprintf(f, " %7i %20.10E %20.10E %20.10E %20.10E %7i %7i\n", 0,
                         at<double>(o, 0x2518), at<double>(o, 0x2588), at<double>(o, 0x2590),
                         at<double>(o, 0x2598), at<std::int32_t>(o, 0x25a0),
                         at<std::int32_t>(o, 0x25a4));
        } else {
            if (t->ndim != 1) return 3;
            rt::crt::fprintf(f, " %7i %7i %20.10E %20.10E %20.10E %7i %7i\n", 1,
                         at<std::int32_t>(o, 0x252c), at<double>(o, 0x2588), at<double>(o, 0x2590),
                         at<double>(o, 0x2598), at<std::int32_t>(o, 0x25a0),
                         at<std::int32_t>(o, 0x25a4));
            io::table_write(f, t, 0);
        }
        break;
    }
    case 6:
        if (at<std::int32_t>(o, 0x2670) == 0) {
            k = at<std::int32_t>(o, 0x25b8) == 0x1a ? 2 : 1;
            rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i\n", name, n, 6, k,
                         at<std::int32_t>(o, 0x2624), at<std::int32_t>(o, 0x25c4));
        } else {
            rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i\n", name, n, 6,
                         -iabs(at<std::int32_t>(o, 0x2674)), at<std::int32_t>(o, 0x2624), 0);
        }
        if (at<std::int32_t>(o, 0x2670) == 0) io::table_write(f, table_at(o, 0x25a8), 0);
        break;
    case 7: {
        DYNARY* t = table_at(o, 0x2678);
        const double p1 = at<double>(o, 0x2768), p2 = at<double>(o, 0x2770);
        if (neg) {
            rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E %20.10E %20.10E %20.10E\n", name, n, 7,
                         negsub, a, 0.0, p1, p2);
            break;
        }
        int form = sub_form_078(t, &k);
        if (form == 3) return 3;
        if (form == 0) {
            rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E %20.10E %20.10E %20.10E\n", name, n, 7, 0, a,
                         at<double>(o, 0x2680), p1, p2);
        } else {
            rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E %7i %20.10E %20.10E\n", name, n, 7, k, a,
                         at<std::int32_t>(o, 0x2694), p1, p2);
            io::table_write(f, t, 0);
        }
        break;
    }
    case 8: {
        DYNARY* t = table_at(o, 0x26f0);
        const double p1 = at<double>(o, 0x2768);
        if (neg) {
            rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E %20.10E %20.10E\n", name, n, 8, negsub, a,
                         0.0, p1);
            break;
        }
        int form = sub_form_078(t, &k);
        if (form == 3) return 3;
        if (form == 0) {
            rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E %20.10E %20.10E\n", name, n, 8, 0, a,
                         at<double>(o, 0x26f8), p1);
        } else {
            rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E %7i %20.10E\n", name, n, 8, k, a,
                         at<std::int32_t>(o, 0x270c), p1);
            io::table_write(f, t, 0);
        }
        break;
    }
    case 9:
        rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E\n", name, n, 9, a, at<double>(o, 0x2768));
        break;
    case 10:
        rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E %20.10E\n", name, n, 10, a,
                     at<double>(o, 0x2768), at<double>(o, 0x2778));
        break;
    case 0xb:
        rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E\n", name, n, 0xb, at<double>(o, 0x22e8),
                     at<double>(o, 0x22f0));
        break;
    default:
        break;
    }
    return 0;
}

// @src 1402b11e0  SPDLMT: table form when +0x2a18 == 1, else the limit alone when > 0.0
int write_spdlmt(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    Target t = target(m, ref);
    LLOBJ_DATA& o = *t.obj;
    if (at<std::uint8_t>(o, 0x2a18) == 1) {
        rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E\n", kw(0x9b), t.n, at<std::int32_t>(o, 0x2a3c),
                     at<double>(o, 0x2a98), at<double>(o, 0x2aa0));
        io::table_write(f, table_at(o, 0x2a20), 0);
    } else if (kZero < at<double>(o, 0x2aa0)) {
        // the middle value is a literal 0.0 (XORPD), not +0x2a98
        rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E\n", kw(0x9b), t.n, 0, 0.0,
                     at<double>(o, 0x2aa0));
    }
    return 0;
}

// @src 1402afd70  PRSTRE: only when +0x2aa8 > 0.0
int write_prstre(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    Target t = target(m, ref);
    if (kZero < at<double>(*t.obj, 0x2aa8)) {
        rt::crt::fprintf(f, "%-6s %7i %20.10E %20.10E %20.10E\n", kw(0x100), t.n,
                     at<double>(*t.obj, 0x2aa8), at<double>(*t.obj, 0x2ab0),
                     at<double>(*t.obj, 0x2ab8));
    }
    return 0;
}

// @src 1402b0350  STROKE: not for internal type 5
int write_stroke(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    Target t = target(m, ref);
    if (at<std::int32_t>(*t.obj, 0xc) != 5) {
        if (!is_2d(m)) {
            rt::crt::fprintf(f, "%-6s %7i %20.10E %20.10E %20.10E\n", kw(0x99), t.n,
                         at<double>(*t.obj, 0x22d0), at<double>(*t.obj, 0x22d8),
                         at<double>(*t.obj, 0x22e0));
        } else {
            rt::crt::fprintf(f, "%-6s %7i %20.10E %20.10E\n", kw(0x99), t.n,
                         at<double>(*t.obj, 0x22d0), at<double>(*t.obj, 0x22d8));
        }
    }
    return 0;
}

// @src 1402b04b0  FORCE: not for internal type 5
int write_force(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    Target t = target(m, ref);
    if (at<std::int32_t>(*t.obj, 0xc) != 5) {
        if (!is_2d(m)) {
            rt::crt::fprintf(f, "%-6s %7i %20.10E %20.10E %20.10E\n", kw(0x12f), t.n,
                         at<double>(*t.obj, 0x2300), at<double>(*t.obj, 0x2308),
                         at<double>(*t.obj, 0x2310));
        } else {
            rt::crt::fprintf(f, "%-6s %7i %20.10E %20.10E\n", kw(0x12f), t.n,
                         at<double>(*t.obj, 0x2300), at<double>(*t.obj, 0x2308));
        }
    }
    return 0;
}

// @src 1402b0610  ANGMOV
int write_angmov(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    return write_angular(f, m, ref, 0x9c, 0x2788, 0x27c0, 0x2838);
}

// @src 1402afe80  ANGMO2
int write_angmo2(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    return write_angular(f, m, ref, 0x104, 0x28b8, 0x28f0, 0x2968);
}

// @src 1402b0a40  ANGMVY (2D): only when GEOTYP (model+0x850) is 3, or 1 with a type-6 axis
int write_angmvy(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    Target t = target(m, ref);
    LLOBJ_DATA& o = *t.obj;
    const std::int32_t geotyp = at<std::int32_t>(m, 0x850);
    if (!(geotyp == 3 || (geotyp == 1 && 0 < count_type6_axes(o)))) return 0;
    const std::int32_t method = at<std::int32_t>(o, 0x2ac0);
    const double r = at<double>(o, 0x2bd0);
    if (method == 0 || method == 1) {
        DYNARY* tab = table_at(o, method == 0 ? 0x2ac8 : 0x2b40);
        if (tab->ndim == 0) {
            rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E %20.10E\n", kw(0xcb), t.n, method, 0,
                         tab->value, r);
        } else {
            if (tab->ndim != 1) return 3;
            int k;
            if (tab->var[0] == 1) {
                k = 1;
            } else {
                if (tab->var[0] != 10) return 3;
                k = 2;
            }
            rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %20.10E\n", kw(0xcb), t.n, method, k,
                         tab->n[0], r);
        }
        io::table_write(f, tab, 0);
    } else if (method == 2) {
        rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E %20.10E %20.10E %20.10E\n", kw(0xcb), t.n, 2, 0,
                     at<double>(o, 0x2bb8), at<double>(o, 0x2bc0), at<double>(o, 0x2bc8), r);
    }
    return 0;
}

// @src 1402b1000  CNTRAX: not for internal type 5 unless byte +0x2bd8 is set
int write_cntrax(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    Target t = target(m, ref);
    LLOBJ_DATA& o = *t.obj;
    if (at<std::int32_t>(o, 0xc) != 5 || at<std::uint8_t>(o, 0x2bd8) != 0) {
        if (!is_2d(m)) {
            rt::crt::fprintf(f, "%-6s %7i %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E\n",
                         kw(0x9d), t.n, at<double>(o, 0x2790), at<double>(o, 0x2798),
                         at<double>(o, 0x27a0), at<double>(o, 0x27a8), at<double>(o, 0x27b0),
                         at<double>(o, 0x27b8), at<double>(o, 0x28b0));
        } else {
            rt::crt::fprintf(f, "%-6s %7i %20.10E %20.10E %20.10E\n", kw(0x9d), t.n,
                         at<double>(o, 0x2790), at<double>(o, 0x2798), at<double>(o, 0x28b0));
        }
    }
    return 0;
}

// @src 1402b01e0  CNTRA2 (3D; the movement block calls it only in 3D)
int write_cntra2(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    Target t = target(m, ref);
    LLOBJ_DATA& o = *t.obj;
    if (at<std::int32_t>(o, 0xc) != 5 || at<std::uint8_t>(o, 0x2bd8) != 0) {
        rt::crt::fprintf(f, "%-6s %7i %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E\n",
                     kw(0x105), t.n, at<double>(o, 0x28c0), at<double>(o, 0x28c8),
                     at<double>(o, 0x28d0), at<double>(o, 0x28d8), at<double>(o, 0x28e0),
                     at<double>(o, 0x28e8), at<double>(o, 0x29e0));
    }
    return 0;
}

// @src 1402a7c80  movement block: PRSNAM, MOVCTL, SPDLMT, PRSTRE, STROKE, FORCE, ANGMOV,
// [2D ANGMVY], CNTRAX, [3D ANGMO2, CNTRA2], OBJUPD, [OBJSTP]; stops at the first non-zero handler
int write_movement_block(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA* obj, std::int32_t no)
{
    if (obj == nullptr) return 0;
    std::int32_t n = no;
    if (no < 1) n = at<std::int32_t>(*obj, 8);
    KeyRef ref;
    ref.push_back(at<std::int32_t>(*obj, 8));
    rt::crt::fprintf(f, "%-6s %7i\n%s\n", kw(0x101), n, &at<char>(*obj, 0x2160));
    int st;
    if ((st = write_movctl(f, m, ref)) != 0) return st;
    if ((st = write_spdlmt(f, m, ref)) != 0) return st;
    if ((st = write_prstre(f, m, ref)) != 0) return st;
    if ((st = write_stroke(f, m, ref)) != 0) return st;
    if ((st = write_force(f, m, ref)) != 0) return st;
    if ((st = write_angmov(f, m, ref)) != 0) return st;
    if (is_2d(m) && (st = write_angmvy(f, m, ref)) != 0) return st;
    if ((st = write_cntrax(f, m, ref)) != 0) return st;
    if (is_3d(m)) {
        if ((st = write_angmo2(f, m, ref)) != 0) return st;
        if ((st = write_cntra2(f, m, ref)) != 0) return st;
    }
    rt::crt::fprintf(f, "%-6s %7i %7i\n", kw(0x10b), n, at<std::int32_t>(*obj, 0x1f1c));
    if (has_object_stop(*obj)) {
        if (!is_2d(m)) {
            rt::crt::fprintf(f, "%-6s %7i %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E\n", kw(0x10c),
                         n, at<double>(*obj, 0x29e8), at<double>(*obj, 0x29f0),
                         at<double>(*obj, 0x29f8), at<double>(*obj, 0x2a00),
                         at<double>(*obj, 0x2a08), at<double>(*obj, 0x2a10));
        } else {
            rt::crt::fprintf(f, "%-6s %7i %20.10E %20.10E %20.10E %20.10E\n", kw(0x10c), n,
                         at<double>(*obj, 0x29e8), at<double>(*obj, 0x29f0),
                         at<double>(*obj, 0x2a00), at<double>(*obj, 0x2a08));
        }
    }
    return 0;
}

// ---- registry column 3 ----------------------------------------------------------------------

// @src 1402b3da0  MOVCTL tables by name (strncmp prefixes, as in the binary)
int table_movctl(DB_STEP_DATA* m, const char* name, const KeyRef& ref, void** out)
{
    LLOBJ_DATA* o = find_object(*m, ref[0]);
    if (o == nullptr) return 3;
    std::size_t off;
    if (std::strncmp(name, "Speed", 5) == 0) {
        off = 0x2248;
    } else if (std::strncmp(name, "Force", 5) == 0) {
        off = 0x21d0;
    } else if (std::strncmp(name, "Hammer Blow efficiency", 0x16) == 0) {
        off = 0x2368;
    } else if (std::strncmp(name, "Screw Press efficiency", 0x16) == 0) {
        off = 0x2400;
    } else if (std::strncmp(name, "Spring Stiffness", 0x10) == 0) {
        off = 0x2510;
    } else if (std::strncmp(name, "Hydrolic Strain rate", 0x14) == 0) {
        off = 0x2678;
    } else if (std::strncmp(name, "Stroke profile", 0xe) == 0) {
        off = 0x2498;
    } else {
        return 3;
    }
    *out = table_at(*o, off);
    return 0;
}

// @src 1402b1360  SPDLMT table T@2a20
int table_spdlmt(DB_STEP_DATA* m, const char*, const KeyRef& ref, void** out)
{
    LLOBJ_DATA* o = find_object(*m, ref[0]);
    if (o == nullptr) return 3;
    *out = table_at(*o, 0x2a20);
    return 0;
}

// @src 1402b0970  ANGMOV table of the current method (none stored for another method)
int table_angmov(DB_STEP_DATA* m, const char*, const KeyRef& ref, void** out)
{
    LLOBJ_DATA* o = find_object(*m, ref[0]);
    if (o == nullptr) return 3;
    if (at<std::int32_t>(*o, 0x2788) == 0) {
        *out = table_at(*o, 0x27c0);
    } else if (at<std::int32_t>(*o, 0x2788) == 1) {
        *out = table_at(*o, 0x2838);
    }
    return 0;
}

// @src 1402b0ef0  ANGMVY tables by name (strcmp)
int table_angmvy(DB_STEP_DATA* m, const char* name, const KeyRef& ref, void** out)
{
    LLOBJ_DATA* o = find_object(*m, ref[0]);
    if (o == nullptr) return 3;
    if (std::strcmp(name, "Torque") == 0) {
        *out = table_at(*o, 0x2ac8);
        return 0;
    }
    if (std::strcmp(name, "Angular Velocity") == 0) {
        *out = table_at(*o, 0x2b40);
        return 0;
    }
    return 3;
}

// @src 1402afa70  CSFREQ table T@1f60
int table_csfreq(DB_STEP_DATA* m, const char*, const KeyRef& ref, void** out)
{
    LLOBJ_DATA* o = find_object(*m, ref[0]);
    if (o == nullptr) return 3;
    *out = table_at(*o, 0x1f60);
    return 0;
}

// @src 1402afcd0  VOLCRG table T@1fe0
int table_volcrg(DB_STEP_DATA* m, const char*, const KeyRef& ref, void** out)
{
    LLOBJ_DATA* o = find_object(*m, ref[0]);
    if (o == nullptr) return 3;
    *out = table_at(*o, 0x1fe0);
    return 0;
}

// ---- handler2 by id ---------------------------------------------------------------------------

bool handles_write(int id)
{
    switch (id) {
    case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0xca: case 0xf0: case 0xeb:
    case 0xec: case 0x97: case 0x9b: case 0x100: case 0x99: case 0x12f: case 0x9c: case 0xcb:
    case 0x9d: case 0x104: case 0x105: case 0xb4: case 0xb5: case 0xb6:
        return true;
    default:
        return false;
    }
}

int write_keyword(int id, std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    switch (id) {
    case 0x92: return write_avgstr(f, m, ref);
    case 0x93: return write_lmtstr(f, m, ref);
    case 0x94: return write_zstr(f, m, ref);
    case 0x95: return write_penvol(f, m, ref);
    case 0x96: return write_reftmp(f, m, ref);
    case 0xca: return write_tmplmt(f, m, ref);
    case 0xf0: return write_otprng(f, m, ref);
    case 0xeb: return write_csfreq(f, m, ref);
    case 0xec: return write_volcrg(f, m, ref);
    case 0x97: return write_movctl(f, m, ref);
    case 0x9b: return write_spdlmt(f, m, ref);
    case 0x100: return write_prstre(f, m, ref);
    case 0x99: return write_stroke(f, m, ref);
    case 0x12f: return write_force(f, m, ref);
    case 0x9c: return write_angmov(f, m, ref);
    case 0xcb: return write_angmvy(f, m, ref);
    case 0x9d: return write_cntrax(f, m, ref);
    case 0x104: return write_angmo2(f, m, ref);
    case 0x105: return write_cntra2(f, m, ref);
    case 0xb4: return write_rmdpth(f, m, ref);
    case 0xb5: return write_rmstrk(f, m, ref);
    case 0xb6: return write_rmtime(f, m, ref);
    default: return 0;  // FUN_1402f75d0
    }
}

} // namespace def::key::g2obj
