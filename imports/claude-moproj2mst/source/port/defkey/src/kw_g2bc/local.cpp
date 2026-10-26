// kw-g2bc local family: BCCFNC (case 0x11), LOCTMP / LOCATM / LOCDEF (cases 0x56..0x58), RSEDEF
// (case 0x9e); the LOC* block writer FUN_1402ac560, the RSEDEF writer FUN_1402b7140, the registry
// column-2 handlers (1402b4050, 1402ae590, 1402aeb40, 1402aef30) and column-3 accessors
// (1402b4180, 1402ae950, 1402aeda0, 1402af1f0).
// Specs: docs/re/keywords/BCCFNC.md, LOCTMP.md, LOCATM.md, LOCDEF.md, RSEDEF.md.
// Decompile: deform-rea-work/out/g2bc_local/w.c.
//
// Model storage:
//   BCCFNC  obj+0x2e68 Cls_1401364a0 (list of DYNARY, function number at DYNARY+0x54)
//   LOCTMP  obj+0x2f40 window list (LLWINDEF_DATA, kind 5), tables +0x380 +0x3f8 +0x470 +0x4e8,
//           optional +0x568 (flag +0x560)
//   LOCATM  obj+0x2f70 window list (kind 6), tables +0x5e0 +0x658 +0x6d0
//   LOCDEF  obj+0x2fa0 window list (kind 7), pressure +0x748, direction +8 and friction table
//           +0x20 (the window's Cls_1401fb020 part, not touched by the reset 14025d1a0)
//   RSEDEF  obj+0x20d8 DYNARY (ncomp at obj+0x2118, values at obj+0x2120)
#define _CRT_SECURE_NO_WARNINGS
#include "internal.hpp"

#include <climits>
#include <cstring>

#include <defkey/key_writer.hpp>
#include <defkey/kw_g2bc_tables.hpp>
#include <defkey/line_io.hpp>
#include <defkey/table_io.hpp>
#include <defmodel/bc/local_bc.hpp>
#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/cls_1401364a0.hpp>
#include <defmodel/layout/cls_140269250.hpp>
#include <defmodel/window/llwindef_data.hpp>
#include <defmodel/window/window_list.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

namespace def::key::g2bc {

using model::Cls_1401364a0;
using model::Cls_140269250;
using model::LLWINDEF_DATA;

namespace {

constexpr int kBccfnc = 161;  // 0xa1, case 0x11
constexpr int kLoctmp = 230;  // 0xe6, case 0x56
constexpr int kLocatm = 231;  // 0xe7, case 0x57
constexpr int kLocdef = 232;  // 0xe8, case 0x58
constexpr int kRsedef = 302;  // 0x12e, case 0x9e

constexpr std::size_t kFunctions = 0x2e68;  // Cls_1401364a0
constexpr std::size_t kLocTmp = 0x2f40;     // Cls_140269250
constexpr std::size_t kLocAtm = 0x2f70;
constexpr std::size_t kLocDef = 0x2fa0;
constexpr std::size_t kRseTable = 0x20d8;   // DYNARY
constexpr std::size_t kRseCount = 0x2118;   // = kRseTable + 0x40 (ncomp)

// (int)d as cvttsd2si: truncation toward zero, 0x80000000 when out of range or NaN
std::int32_t trunc_int(double d)
{
    if (!(d > -2147483649.0 && d < 2147483648.0)) return INT_MIN;
    return static_cast<std::int32_t>(d);
}

Cls_1401364a0& functions(LLOBJ_DATA& obj) { return at<Cls_1401364a0>(obj, kFunctions); }
Cls_140269250& winlist(LLOBJ_DATA& obj, std::size_t off) { return at<Cls_140269250>(obj, off); }

// table at byte offset `off` of a window (the decompile's lVar + off)
DYNARY& wtable(LLWINDEF_DATA& w, std::size_t off) { return at<DYNARY>(w, off); }

// The value-line shapes of the LOC* cases: constant v, or a 1-D table over `var` with (int)v
// points, or a 2-D table.
void set_constant(DYNARY& t, double v)
{
    t.ndim = 0;
    t.value = v;
}
void set_1d(DYNARY& t, std::int32_t var, double v)
{
    t.ndim = 1;
    t.var[0] = var;
    t.n[0] = trunc_int(v);
}
void set_2d(DYNARY& t, std::int32_t var0, std::int32_t var1, double v, double w)
{
    t.ndim = 2;
    t.var[0] = var0;
    t.var[1] = var1;
    t.n[0] = trunc_int(v);
    t.n[1] = trunc_int(w);
}

// Conv / Emis of LOCTMP and React of LOCATM: type 0 constant, 1 1-D var 1, 2 1-D var `var2`,
// 3 2-D vars (1, var2); other types store nothing.
void set_by_type_2(DYNARY& t, std::int32_t type, std::int32_t var2, double v, double w)
{
    if (type == 0) {
        set_constant(t, v);
    } else if (type == 1) {
        set_1d(t, 1, v);
    } else if (type == 2) {
        set_1d(t, var2, v);
    } else if (type == 3) {
        set_2d(t, 1, var2, v, w);
    }
}
// Env / Flux of LOCTMP, Atom / Flux of LOCATM, Pressure of LOCDEF: 0 constant, 1 1-D var 1.
void set_by_type_1(DYNARY& t, std::int32_t type, double v)
{
    if (type == 0) {
        set_constant(t, v);
    } else if (type == 1) {
        set_1d(t, 1, v);
    }
}

// The window an entry keyword works on: FUN_140269bc0(list, id) then FUN_14025d1a0(w, kind).
// id <= 0 is not extended and gives the head; with an empty list that is NULL, which the original
// dereferences (crash): the port returns null and the case reports 2.
LLWINDEF_DATA* reset_entry(Cls_140269250& l, std::int32_t id, std::int32_t kind)
{
    LLWINDEF_DATA* w = model::winlist_extend(&l, id);
    if (w == nullptr) return nullptr;
    model::window_reset(w, kind);
    return w;
}

// part of @src 14038b8a0  case 0x11 BCCFNC (p_main.c 1339-1355)
int read_bccfnc(std::FILE* f, LLOBJ_DATA& obj)
{
    std::int32_t no = 0;
    std::int32_t npts = 0;
    if (read_ii(f, &no, &npts) != 0) return 2;
    if (no < 1) return 2;
    DYNARY* t = model::dynary_list_find(&functions(obj), no);
    if (t == nullptr) {
        t = functions(obj).add();
        t->u54 = no;
    }
    model::dynary_reinit_keep(t, 0xd);
    t->n[0] = npts;
    return read_table_body(f, t, 0);
}

// part of @src 14038b8a0  case 0x56 LOCTMP (p_main.c 2760-2893)
int read_loctmp(std::FILE* f, LLOBJ_DATA& obj)
{
    std::int32_t id = 0, v838 = 0, t_env = 0, t_conv = 0, t_emis = 0, t_flux = 0, t_htc = 0;
    std::int32_t* const line0[7] = {&id, &v838, &t_env, &t_conv, &t_emis, &t_flux, &t_htc};
    if (read_i7(f, line0) != 0) return 2;
    LLWINDEF_DATA* w = reset_entry(winlist(obj, kLocTmp), id, 5);
    if (w == nullptr) return 2;
    w->u838 = v838;
    double v = 0.0, x = 0.0;

    if (read_d(f, &v) != 0) return 2;
    set_by_type_1(wtable(*w, 0x380), t_env, v);
    if (read_table_body(f, &wtable(*w, 0x380), 0) != 0) return 2;

    if (read_dd(f, &v, &x) != 0) return 2;
    set_by_type_2(wtable(*w, 0x3f8), t_conv, 3, v, x);
    if (read_table_body(f, &wtable(*w, 0x3f8), 0) != 0) return 2;

    if (read_dd(f, &v, &x) != 0) return 2;
    set_by_type_2(wtable(*w, 0x470), t_emis, 3, v, x);
    if (read_table_body(f, &wtable(*w, 0x470), 0) != 0) return 2;

    if (read_d(f, &v) != 0) return 2;
    set_by_type_1(wtable(*w, 0x4e8), t_flux, v);
    if (read_table_body(f, &wtable(*w, 0x4e8), 0) != 0) return 2;

    if (t_htc < 1) {
        w->u560 = 0;
        return 0;
    }
    w->u560 = 1;
    if (read_d(f, &v) != 0) return 2;
    DYNARY& htc = wtable(*w, 0x568);
    if (t_htc == 1) {
        set_constant(htc, v);
    } else if (t_htc == 2) {
        set_1d(htc, 1, v);
    } else if (t_htc == 3) {
        set_1d(htc, 4, v);
    }
    return read_table_body(f, &htc, 0) != 0 ? 2 : 0;
}

// part of @src 14038b8a0  case 0x57 LOCATM (p_main.c 2894-2969)
int read_locatm(std::FILE* f, LLOBJ_DATA& obj)
{
    std::int32_t id = 0, v838 = 0, t_atom = 0, t_react = 0, t_flux = 0;
    if (read_i5(f, &id, &v838, &t_atom, &t_react, &t_flux, false) != 0) return 2;
    LLWINDEF_DATA* w = reset_entry(winlist(obj, kLocAtm), id, 6);
    if (w == nullptr) return 2;
    w->u838 = v838;
    double v = 0.0, x = 0.0;

    if (read_d(f, &v) != 0) return 2;
    set_by_type_1(wtable(*w, 0x5e0), t_atom, v);
    if (read_table_body(f, &wtable(*w, 0x5e0), 0) != 0) return 2;

    if (read_dd(f, &v, &x) != 0) return 2;
    set_by_type_2(wtable(*w, 0x658), t_react, 0xc, v, x);
    if (read_table_body(f, &wtable(*w, 0x658), 0) != 0) return 2;

    if (read_d(f, &v) != 0) return 2;
    set_by_type_1(wtable(*w, 0x6d0), t_flux, v);
    return read_table_body(f, &wtable(*w, 0x6d0), 0) != 0 ? 2 : 0;
}

// part of @src 14038b8a0  case 0x58 LOCDEF (p_main.c 2970-3049)
int read_locdef(std::FILE* f, LLOBJ_DATA& obj)
{
    std::int32_t id = 0, v838 = 0, t_pres = 0, dir = 0, t_fric = 0;
    if (read_i5(f, &id, &v838, &t_pres, &dir, &t_fric, false) != 0) return 2;
    LLWINDEF_DATA* w = reset_entry(winlist(obj, kLocDef), id, 7);
    if (w == nullptr) return 2;
    w->u838 = v838;
    double v = 0.0, x = 0.0, y = 0.0;

    if (read_d(f, &v) != 0) return 2;
    set_by_type_1(wtable(*w, 0x748), t_pres, v);
    if (read_table_body(f, &wtable(*w, 0x748), 0) != 0) return 2;

    // bc+8: only 1, 2 and 3 are stored
    if (dir == 1) {
        w->fric.u000 = 1;
    } else if (dir == 2) {
        w->fric.u000 = 2;
    } else if (dir == 3) {
        w->fric.u000 = 3;
    }

    if (read_d3(f, &v, &x, &y) != 0) return 2;
    DYNARY& fr = wtable(*w, 0x20);  // = w->fric.u018
    if (t_fric == 0) {
        set_constant(fr, v);
    } else if (t_fric == 1) {
        set_1d(fr, 1, v);
    } else if (t_fric == 2) {
        set_1d(fr, 4, v);
    } else if (t_fric == 3) {
        set_1d(fr, 3, v);
    } else if (t_fric == 4) {
        set_1d(fr, 0x15, v);
    } else if (t_fric == 5) {
        fr.ndim = 3;
        fr.var[0] = 4;
        fr.var[1] = 3;
        fr.var[2] = 0x15;
        fr.n[0] = trunc_int(v);
        fr.n[1] = trunc_int(x);
        fr.n[2] = trunc_int(y);
    }
    return read_table_body(f, &fr, 0) != 0 ? 2 : 0;
}

// part of @src 14038b8a0  case 0x9e RSEDEF (p_main.c 3851-3860): the table read's status is
// ignored, the case returns the count line's
int read_rsedef(std::FILE* f, LLOBJ_DATA& obj)
{
    std::int32_t n = 0;
    const int st = read_i(f, &n, false);
    if (0 < n) {
        at<std::int32_t>(obj, kRseCount) = n;
        read_table_body(f, &at<DYNARY>(obj, kRseTable), 0);
    }
    return st;
}

// The common prologue of the LOC* handlers: object by ref[0] (not checked for null), entry
// ref[1] (positional), printed number obj+0x3f24 when > 0.
struct Entry {
    LLWINDEF_DATA* w;
    std::int32_t n;
};
Entry loc_entry(DB_STEP_DATA& m, const KeyRef& ref, std::size_t list)
{
    std::int32_t n = ref[0];
    LLOBJ_DATA* obj = find_object(m, n);
    LLWINDEF_DATA* w = model::winlist_nth(&winlist(*obj, list), ref[1]);
    if (0 < at<std::int32_t>(*obj, 0x3f24)) n = at<std::int32_t>(*obj, 0x3f24);
    return {w, n};
}

// header code of a 2-variable table: ndim 0 -> 0; ndim 1 -> 1 when var[0] == 1, else 2; else 3
std::int32_t code_by_type_2(const DYNARY& t)
{
    if (t.ndim == 0) return 0;
    if (t.ndim == 1) return t.var[0] == 1 ? 1 : 2;
    return 3;
}

// The LOC* accessor prologue (1402ae950 / 1402aeda0 / 1402af1f0): the entry, or null for 3.
LLWINDEF_DATA* loc_accessor_entry(DB_STEP_DATA* m, const char* name, const KeyRef& ref, std::size_t list)
{
    if (ref.size() < 2 || name == nullptr) return nullptr;
    LLOBJ_DATA* obj = find_object(*m, ref[0]);
    if (obj == nullptr) return nullptr;
    return model::winlist_nth(&winlist(*obj, list), ref[1]);
}

} // namespace

namespace local {

bool handles(int id)
{
    return id == kBccfnc || id == kLoctmp || id == kLocatm || id == kLocdef || id == kRsedef;
}

// part of @src 14038b8a0  the local family's cases; the parser returns 2 for a nonzero status
int read_case(DB_STEP_DATA&, std::FILE* f, LLOBJ_DATA& obj, int id)
{
    int st = 2;
    switch (id) {
    case kBccfnc: st = read_bccfnc(f, obj); break;
    case kLoctmp: st = read_loctmp(f, obj); break;
    case kLocatm: st = read_locatm(f, obj); break;
    case kLocdef: st = read_locdef(f, obj); break;
    case kRsedef: st = read_rsedef(f, obj); break;
    default: return 2;
    }
    return st == 0 ? 0 : 2;
}

bool handles_write(int id) { return id == kBccfnc || id == kLoctmp || id == kLocatm || id == kLocdef; }

int write_keyword(int id, std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    switch (id) {
    case kBccfnc: return write_bccfnc(f, m, ref);
    case kLoctmp: return write_loctmp(f, m, ref);
    case kLocatm: return write_locatm(f, m, ref);
    case kLocdef: return write_locdef(f, m, ref);
    default: return 0;
    }
}

} // namespace local

// @src 1402b4050  BCCFNC handler2: the ref[1]-th table of obj+0x2e68 (list order), written only
// when it is a 1-D table over variable 1 (time)
int write_bccfnc(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    std::int32_t n = ref[0];
    const std::int32_t k = ref[1];
    LLOBJ_DATA* obj = find_object(m, n);
    if (0 < at<std::int32_t>(*obj, 0x3f24)) n = at<std::int32_t>(*obj, 0x3f24);
    DYNARY* t = functions(*obj).at(k);
    if (t->ndim == 1 && t->var[0] == 1) {
        rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", keyword_label(kBccfnc), n, t->u54, t->n[0]);
        write_table_body(f, t, 0);  // status ignored
    }
    return 0;
}

// @src 1402ae590  LOCTMP handler2
int write_loctmp(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    if (ref.size() < 2) return 3;
    const Entry e = loc_entry(m, ref, kLocTmp);
    LLWINDEF_DATA& w = *e.w;
    const std::int32_t c_env = wtable(w, 0x380).ndim != 0 ? 1 : 0;
    const std::int32_t c_conv = code_by_type_2(wtable(w, 0x3f8));
    const std::int32_t c_emis = code_by_type_2(wtable(w, 0x470));
    const std::int32_t c_flux = wtable(w, 0x4e8).ndim != 0 ? 1 : 0;
    std::int32_t c_htc;
    if (w.u560 == 0) {
        c_htc = 0;
    } else {
        const DYNARY& h = wtable(w, 0x568);
        c_htc = 1;
        if (h.ndim == 0) {
            c_htc = 1;
        } else if (h.ndim == 1 && h.var[0] == 1) {
            c_htc = 2;
        } else if (h.ndim == 1 && h.var[0] == 4) {
            c_htc = 3;
        }
    }
    rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i %7i %7i %7i\n", keyword_label(kLoctmp), e.n, w.index, w.u838,
                     c_env, c_conv, c_emis, c_flux, c_htc);
    write_table_body(f, &wtable(w, 0x380), 1);
    write_table_body(f, &wtable(w, 0x3f8), 1);
    write_table_body(f, &wtable(w, 0x470), 1);
    write_table_body(f, &wtable(w, 0x4e8), 1);
    if (w.u560 == 1) write_table_body(f, &wtable(w, 0x568), 1);
    return 0;
}

// @src 1402aeb40  LOCATM handler2
int write_locatm(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    if (ref.size() < 2) return 3;
    const Entry e = loc_entry(m, ref, kLocAtm);
    LLWINDEF_DATA& w = *e.w;
    const std::int32_t c_atom = wtable(w, 0x5e0).ndim != 0 ? 1 : 0;
    const std::int32_t c_react = code_by_type_2(wtable(w, 0x658));
    const std::int32_t c_flux = wtable(w, 0x6d0).ndim != 0 ? 1 : 0;
    rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i %7i\n", keyword_label(kLocatm), e.n, w.index, w.u838, c_atom,
                     c_react, c_flux);
    write_table_body(f, &wtable(w, 0x5e0), 1);
    write_table_body(f, &wtable(w, 0x658), 1);
    write_table_body(f, &wtable(w, 0x6d0), 1);
    return 0;
}

// @src 1402aef30  LOCDEF handler2: 3 without output when the direction bc+8 is not 1..3
int write_locdef(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    if (ref.size() < 2) return 3;
    const Entry e = loc_entry(m, ref, kLocDef);
    LLWINDEF_DATA& w = *e.w;
    const std::int32_t c_pres = wtable(w, 0x748).ndim != 0 ? 1 : 0;
    std::int32_t dir;
    if (w.fric.u000 == 1) {
        dir = 1;
    } else if (w.fric.u000 == 2) {
        dir = 2;
    } else {
        if (w.fric.u000 != 3) return 3;
        dir = 3;
    }
    // local_40 is uninitialised in the original for ndim 1 with another variable and for ndim 2
    // (and > 3); not reachable from KEY input. The port prints 0 there.
    std::int32_t c_fric = 0;
    const DYNARY& fr = wtable(w, 0x20);
    if (fr.ndim == 0) {
        c_fric = 0;
    } else if (fr.ndim == 1) {
        if (fr.var[0] == 1) {
            c_fric = 1;
        } else if (fr.var[0] == 4) {
            c_fric = 2;
        } else if (fr.var[0] == 3) {
            c_fric = 3;
        } else if (fr.var[0] == 0x15) {
            c_fric = 4;
        }
    } else if (fr.ndim == 3) {
        c_fric = 5;
    }
    rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i %7i\n", keyword_label(kLocdef), e.n, w.index, w.u838, c_pres, dir,
                     c_fric);
    write_table_body(f, &wtable(w, 0x748), 1);
    write_table_body(f, &wtable(w, 0x20), 1);
    return 0;
}

// @src 1402b4180  BCCFNC accessor: table ref[1] - 1 of the list (see kw_g2bc_tables.hpp)
int table_bccfnc(DB_STEP_DATA* m, const char*, const KeyRef& ref, void** out)
{
    const std::int32_t no = ref[0];
    const std::int32_t k = ref[1];
    LLOBJ_DATA* obj = find_object(*m, no);
    if (obj == nullptr) return 3;
    if (0 < k && k <= functions(*obj).size()) *out = functions(*obj).at(k + -1);
    return 0;
}

// @src 1402ae950  LOCTMP accessor by table name
int table_loctmp(DB_STEP_DATA* m, const char* name, const KeyRef& ref, void** out)
{
    LLWINDEF_DATA* w = loc_accessor_entry(m, name, ref, kLocTmp);
    if (w == nullptr) return 3;
    if (std::strcmp(name, "Env Tempreature") == 0) {
        *out = &wtable(*w, 0x380);
    } else if (std::strcmp(name, "Conv. Coef.") == 0) {
        *out = &wtable(*w, 0x3f8);
    } else if (std::strcmp(name, "Emissivity") == 0) {
        *out = &wtable(*w, 0x470);
    } else if (std::strcmp(name, "Heat Flux") == 0) {
        *out = &wtable(*w, 0x4e8);
    } else if (std::strcmp(name, "Heat Trans. Coef.") == 0) {
        *out = &wtable(*w, 0x568);
    }
    return 0;
}

// @src 1402aeda0  LOCATM accessor by table name
int table_locatm(DB_STEP_DATA* m, const char* name, const KeyRef& ref, void** out)
{
    LLWINDEF_DATA* w = loc_accessor_entry(m, name, ref, kLocAtm);
    if (w == nullptr) return 3;
    if (std::strcmp(name, "Env. Atom") == 0) {
        *out = &wtable(*w, 0x5e0);
    } else if (std::strcmp(name, "React. Coef.") == 0) {
        *out = &wtable(*w, 0x658);
    } else if (std::strcmp(name, "Atom Flux") == 0) {
        *out = &wtable(*w, 0x6d0);
    }
    return 0;
}

// @src 1402af1f0  LOCDEF accessor by table name
int table_locdef(DB_STEP_DATA* m, const char* name, const KeyRef& ref, void** out)
{
    LLWINDEF_DATA* w = loc_accessor_entry(m, name, ref, kLocDef);
    if (w == nullptr) return 3;
    if (std::strcmp(name, "Pressure") == 0) {
        *out = &wtable(*w, 0x748);
    } else if (std::strcmp(name, "Friction") == 0) {
        *out = &wtable(*w, 0x20);
    }
    return 0;
}

// @src 1402ac560  LOCTMP, LOCATM, LOCDEF of one object, ref = (obj+8, entry 1..count) per entry;
// the first nonzero handler status stops the block and is returned. N is computed (N < 1 gives
// obj+8) but not used: each handler prints obj+0x3f24 or the number itself. (The original also
// returns 0 for a null object; the reference here cannot be null.)
int write_local_bcs(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n)
{
    std::int32_t no = static_cast<std::int32_t>(n);
    if (no < 1) no = at<std::int32_t>(obj, 8);
    (void)no;
    KeyRef ref;
    const struct {
        std::size_t list;
        int (*handler)(std::FILE*, DB_STEP_DATA&, const KeyRef&);
    } blocks[3] = {{kLocTmp, write_loctmp}, {kLocAtm, write_locatm}, {kLocDef, write_locdef}};
    for (const auto& b : blocks) {
        for (std::int32_t i = 0; i < model::winlist_count(&winlist(obj, b.list)); i = i + 1) {
            ref.clear();  // FUN_14015a620
            ref.push_back(at<std::int32_t>(obj, 8));
            ref.push_back(i + 1);
            const int st = b.handler(f, m, ref);
            if (st != 0) return st;
        }
    }
    return 0;
}

// @src 1402b7140  RSEDEF: only an 8-value vector is written (first value as an int)
int write_rsedef(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    std::int32_t n = ref[0];
    LLOBJ_DATA* obj = find_object(m, n);
    if (0 < at<std::int32_t>(*obj, 0x3f24)) n = at<std::int32_t>(*obj, 0x3f24);
    const std::int32_t cnt = at<std::int32_t>(*obj, kRseCount);
    if (cnt == 8) {
        const double* v = at<DYNARY>(*obj, kRseTable).values;
        rt::crt::fprintf(f, "%-6s %7i %7i\n", keyword_label(kRsedef), n, cnt);
        rt::crt::fprintf(f, " %7i %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E\n", trunc_int(v[0]), v[1],
                         v[2], v[3], v[4], v[5], v[6], v[7]);
    }
    return 0;
}

} // namespace def::key::g2bc
