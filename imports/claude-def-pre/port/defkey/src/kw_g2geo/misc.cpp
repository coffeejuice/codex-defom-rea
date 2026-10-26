// kw-g2geo misc family: SYMSUF, WPAXIS, CRDSYS, GEO23, GEOSEC, MSHSEC (parser cases 0x76, 0x78,
// 0x8a..0x8d of FUN_14038b8a0) and the SYMSUF / WPAXIS writers. Specs: SYMSUF.md, WPAXIS.md,
// CRDSYS.md, GEO23.md, GEOSEC.md, MSHSEC.md. Decompiles: deform-rea-work/out/g2geo/parser.c,
// fam_misc.c.
#define _CRT_SECURE_NO_WARNINGS
#include "internal.hpp"

#include <defmodel/geo_kw/misc_model.hpp>
#include <defmodel/linked/axis_point_list.hpp>
#include <defmodel/linked/record_e8_list.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

namespace def::key::g2geo::misc {

using model::linked::Cls_140257340;
using model::linked::Cls_140257340List;
using model::linked::Cls_140258580;
using model::linked::Cls_140258580List;
namespace gk = def::model::geo_kw;

namespace {

Cls_140257340List& symsuf_list(LLOBJ_DATA& obj) { return at<Cls_140257340List>(obj, 0x2e80); }
Cls_140258580List& wpaxis_list(LLOBJ_DATA& obj) { return at<Cls_140258580List>(obj, 0x20a0); }

template <class T>
T& rec(Cls_140258580* e, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<std::byte*>(e) + off);
}

// part of @src 14038b8a0  case 0x76 SYMSUF (3D only; nothing read otherwise)
int read_symsuf(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj)
{
    if (!is_3d(m)) return 0;
    std::int32_t id = 0, ignored = 0;
    if (read_ii(f, &id, &ignored) != 0) return 2;
    Cls_140257340* e = gk::symsuf_find(symsuf_list(obj), id);
    if (e == nullptr) e = symsuf_list(obj).add(id);  // 140257e90
    // the first line's status is overwritten without a check
    (void)read_d3(f, &e->point[0], &e->point[1], &e->point[2]);
    if (read_d3(f, &e->direction[0], &e->direction[1], &e->direction[2]) != 0) return 2;
    return 0;
}

// part of @src 14038b8a0  case 0x78 WPAXIS
int read_wpaxis(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj)
{
    std::int32_t id = 0, type = 0, sub = 0;
    if (read_iii(f, &id, &type, &sub, nullptr) != 0) return 2;
    if (!(is_3d(m) || type == 5 || type == 6)) return 0;
    Cls_140258580* e = gk::wpaxis_get(wpaxis_list(obj), id);
    // id <= 0 on an empty list gives null in the original, which then writes to +4 and faults;
    // the port reports a reading error instead.
    if (e == nullptr) return 2;
    std::int32_t& t = rec<std::int32_t>(e, 4);
    t = type;
    if (t < 0 || 7 < t) t = 0;
    int st = 0;
    switch (t) {
    case 0:
    case 1:
    case 2: {
        std::int32_t& s = rec<std::int32_t>(e, 0x38);
        s = sub;
        if (s < 0 || 1 < s) s = 0;
        (void)read_d3(f, &rec<double>(e, 0x08), &rec<double>(e, 0x10), &rec<double>(e, 0x18));
        (void)read_d3(f, &rec<double>(e, 0x20), &rec<double>(e, 0x28), &rec<double>(e, 0x30));
        if (read_d4(f, &rec<double>(e, 0x40), &rec<double>(e, 0x48), &rec<double>(e, 0x50),
                    &rec<double>(e, 0x58)) != 0)
            return 2;
        st = 0;
        break;
    }
    case 3:
        rec<std::int32_t>(e, 0x60) = sub;
        (void)read_d3(f, &rec<double>(e, 0x08), &rec<double>(e, 0x10), &rec<double>(e, 0x18));
        if (read_d3(f, &rec<double>(e, 0x20), &rec<double>(e, 0x28), &rec<double>(e, 0x30)) != 0) return 2;
        st = 0;
        break;
    case 4:
        rec<std::int32_t>(e, 0xb0) = sub;
        (void)read_d3(f, &rec<double>(e, 0x68), &rec<double>(e, 0x70), &rec<double>(e, 0x78));
        (void)read_d3(f, &rec<double>(e, 0x80), &rec<double>(e, 0x88), &rec<double>(e, 0x90));
        (void)read_d3(f, &rec<double>(e, 0x98), &rec<double>(e, 0xa0), &rec<double>(e, 0xa8));
        if (read_d(f, &rec<double>(e, 0xb8)) != 0) return 2;
        st = 0;
        break;
    case 5:
        rec<double>(e, 0x20) = 0.0;
        rec<double>(e, 0x28) = 0.0;
        rec<double>(e, 0x30) = 0.0;
        (void)read_d3(f, &rec<double>(e, 0x20), &rec<double>(e, 0x28), &rec<double>(e, 0x30));
        st = read_d(f, &rec<double>(e, 0xb8));
        break;
    case 7: {
        (void)read_d3(f, &rec<double>(e, 0x20), &rec<double>(e, 0x28), &rec<double>(e, 0x30));
        (void)read_d3(f, &rec<double>(e, 0x08), &rec<double>(e, 0x10), &rec<double>(e, 0x18));
        double a = 0.0, b = 0.0;
        double* const out[5] = {&rec<double>(e, 0x40), &a, &b, &rec<double>(e, 0xd0), &rec<double>(e, 0xd8)};
        st = read_d5(f, out);
        rec<std::int32_t>(e, 0xc8) = static_cast<std::int32_t>(a);  // cvttsd2si
        rec<std::int32_t>(e, 0xcc) = static_cast<std::int32_t>(b);
        if (st != 0) return 2;
        break;
    }
    default:  // 6: no data lines
        break;
    }
    return st != 0 ? 2 : 0;
}

// part of @src 14038b8a0  case 0x8a CRDSYS
int read_crdsys(std::FILE* f, LLOBJ_DATA& obj)
{
    std::int32_t v = 0;
    if (read_i(f, &v, false) != 0) return 2;
    at<std::int32_t>(obj, 0x2c38) = v;
    return 0;
}

// part of @src 14038b8a0  case 0x8b GEO23
int read_geo23(std::FILE* f, LLOBJ_DATA& obj)
{
    std::int32_t cnt = 0;
    if (read_tokens(f, &cnt, 2) != 0) return 2;
    void* g = &at<std::byte>(obj, 0x2be8);
    std::int32_t mode = token_int(0);
    if (mode == 0) {
        gk::geo23_set_mode_from_geotyp(g, false);
    } else if (mode == 1) {
        gk::geo23_set_mode_from_geotyp(g, true);
        if (1 < cnt) at<std::int32_t>(obj, 0x2c08) = token_int(1);
        double a = 0.0, b = 0.0, c = 0.0;
        if (read_d3(f, &a, &b, &c) != 0) return 2;
        std::int32_t one = 1;
        gk::geo23_set_kind(g, &one);
        at<double>(obj, 0x2bf0) = a;
        at<double>(obj, 0x2bf8) = b;
        at<double>(obj, 0x2c00) = c;
    } else if (mode == 2) {
        gk::geo23_set_mode_from_geotyp(g, true);
        double a = 0.0, b = 0.0;
        if (read_dd(f, &a, &b) != 0) return 2;
        std::int32_t two = 2;
        gk::geo23_set_kind(g, &two);
        at<double>(obj, 0x2c10) = a;
        at<double>(obj, 0x2c18) = b;
    }
    return 0;
}

// part of @src 14038b8a0  cases 0x8c GEOSEC / 0x8d MSHSEC: count at `count_off` (default `dflt`),
// flag byte at `flag_off`
int read_sections(std::FILE* f, LLOBJ_DATA& obj, std::size_t count_off, std::size_t flag_off,
                  std::int32_t dflt)
{
    std::int32_t cnt = 0;
    if (read_tokens(f, &cnt, 2) != 0) return 2;
    std::int32_t n = token_int(0);
    at<std::int32_t>(obj, count_off) = n < 1 ? dflt : n;
    at<std::uint8_t>(obj, flag_off) = 0 < n ? 1 : 0;
    if (1 < cnt) at<std::uint8_t>(obj, flag_off) = token_int(1) == 1 ? 1 : 0;
    return 0;
}

}  // namespace

bool handles(int id)
{
    return id == kSYMSUF || id == kWPAXIS || id == kCRDSYS || id == kGEO23 || id == kGEOSEC ||
           id == kMSHSEC;
}

int read_case(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, int id)
{
    switch (id) {
    case kSYMSUF: return read_symsuf(m, f, obj);
    case kWPAXIS: return read_wpaxis(m, f, obj);
    case kCRDSYS: return read_crdsys(f, obj);
    case kGEO23: return read_geo23(f, obj);
    case kGEOSEC: return read_sections(f, obj, 0x2c24, 0x2c21, 0x14);
    case kMSHSEC: return read_sections(f, obj, 0x2c28, 0x2c20, 0x15);
    default: return 2;
    }
}

// @src 1402a3880  SYMSUF writer (3D; status ignored by the object writer)
int write_symsuf(DB_STEP_DATA&, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n)
{
    if (f == nullptr) return 3;
    Cls_140257340List& l = symsuf_list(obj);
    for (std::int32_t i = 0; i < l.size(); ++i) {  // 140158e70
        Cls_140257340* e = l.nth(i + 1);            // 140258090
        if (e == nullptr) continue;
        rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", kw(kSYMSUF), n, e->id, 0);
        rt::crt::fprintf(f, " %20.10E %20.10E %20.10E\n", e->point[0], e->point[1], e->point[2]);
        rt::crt::fprintf(f, " %20.10E %20.10E %20.10E\n", e->direction[0], e->direction[1], e->direction[2]);
    }
    return 0;
}

// @src 1402a39e0  WPAXIS writer
int write_wpaxis(DB_STEP_DATA&, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n)
{
    if (f == nullptr) return 3;
    Cls_140258580List& l = wpaxis_list(obj);
    for (std::int32_t i = 0; i < gk::wpaxis_count(l); ++i) {
        Cls_140258580* e = gk::wpaxis_nth(l, i + 1);
        if (e == nullptr) continue;
        const std::int32_t id = rec<std::int32_t>(e, 0);
        const std::int32_t type = rec<std::int32_t>(e, 4);
        auto d = [e](std::size_t o) { return rec<double>(e, o); };
        const char* name = kw(kWPAXIS);
        switch (type) {
        case 0:
        case 1:
        case 2:
            rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", name, n, id, type, rec<std::int32_t>(e, 0x38));
            rt::crt::fprintf(f, " %20.10E %20.10E %20.10E\n", d(0x08), d(0x10), d(0x18));
            rt::crt::fprintf(f, " %20.10E %20.10E %20.10E\n", d(0x20), d(0x28), d(0x30));
            rt::crt::fprintf(f, " %20.10E %20.10E %20.10E %20.10E\n", d(0x40), d(0x48), d(0x50), d(0x58));
            break;
        case 3:
            rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", name, n, id, type, rec<std::int32_t>(e, 0x60));
            rt::crt::fprintf(f, " %20.10E %20.10E %20.10E\n", d(0x08), d(0x10), d(0x18));
            rt::crt::fprintf(f, " %20.10E %20.10E %20.10E\n", d(0x20), d(0x28), d(0x30));
            break;
        case 4:
            rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", name, n, id, type, rec<std::int32_t>(e, 0xb0));
            rt::crt::fprintf(f, " %20.10E %20.10E %20.10E\n", d(0x68), d(0x70), d(0x78));
            rt::crt::fprintf(f, " %20.10E %20.10E %20.10E\n", d(0x80), d(0x88), d(0x90));
            rt::crt::fprintf(f, " %20.10E %20.10E %20.10E\n", d(0x98), d(0xa0), d(0xa8));
            rt::crt::fprintf(f, " %20.10E\n", d(0xb8));
            break;
        case 5:
            rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", name, n, id, type);
            rt::crt::fprintf(f, " %20.10E %20.10E %20.10E\n", d(0x20), d(0x28), d(0x30));
            rt::crt::fprintf(f, " %20.10E\n", d(0xb8));
            break;
        case 6:
            rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", name, n, id, type);
            break;
        case 7:
            rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", name, n, id, type);
            rt::crt::fprintf(f, " %20.10E %20.10E %20.10E\n", d(0x20), d(0x28), d(0x30));
            rt::crt::fprintf(f, " %20.10E %20.10E %20.10E\n", d(0x08), d(0x10), d(0x18));
            rt::crt::fprintf(f, " %20.10E %7i %7i %20.10E %20.10E\n", d(0x40), rec<std::int32_t>(e, 0xc8),
                             rec<std::int32_t>(e, 0xcc), d(0xd0), d(0xd8));
            break;
        default:
            break;
        }
    }
    return 0;
}

}  // namespace def::key::g2geo::misc
