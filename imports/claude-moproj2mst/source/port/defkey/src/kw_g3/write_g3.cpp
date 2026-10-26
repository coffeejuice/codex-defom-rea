// Group 3 KEY writer: FUN_1403c9ad0 (all pairs), FUN_1403c8560 (one pair with its windows) and
// the per-keyword record writers. Packet kw-g3. Specs: docs/re/keywords/<KW>.md "KEY writer".
//
// Output goes through the CRT fprintf with the original format strings; the three-digit exponent
// of the old CRT comes from def::rt::init_stdio_compat() (p1-01), set once at start.
#include <defkey/kw_g3/kw_g3.hpp>

#include <cstdlib>

#include <defkey/crt_scan.hpp>
#include <defkey/line_io.hpp>
#include <defkey/table_io.hpp>
#include <defkey/key_writer.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defmodel/layout/db_step_data.hpp>

#include "g3_internal.hpp"

namespace def::key::g3 {

using detail::dbl;

namespace {

// 1402f7720 (keyword registry packet): keyword name by id, one static buffer
const char* kw(int id) { return def::key::keyword_label(id); }

// part of @src 1403c9be0 / 1403c8560: one friction sub-record " %7i ..." (after a mode-4 header)
void write_friction_sub(std::FILE* f, const Cls_1401fb020* fb, int k)
{
    const std::uint8_t fixed = k == 0 ? fb->u004 : fb->u005;
    const std::int32_t id = k == 0 ? fb->u008 : fb->u00c;
    const std::int32_t type = k == 0 ? fb->u010 : fb->u014;
    const DYNARY* tb = k == 0 ? &fb->u018 : &fb->u090;
    const std::uint64_t* par = fb->u108 + (k == 0 ? 0 : 6);
    if (fixed == 0) {
        switch (type) {
        case 0: def::rt::crt::fprintf(f, " %7i %20.10E\n", 0, tb->value); break;
        case 1: def::rt::crt::fprintf(f, " %7i %7i\n", 1, tb->n[0]); break;
        case 2: def::rt::crt::fprintf(f, " %7i %7i\n", 2, tb->n[0]); break;
        case 3: def::rt::crt::fprintf(f, " %7i %7i\n", 3, tb->n[0]); break;
        case 4: def::rt::crt::fprintf(f, " %7i %7i\n", 4, tb->n[0]); break;
        case 5: def::rt::crt::fprintf(f, " %7i %7i %7i %7i\n", 5, tb->n[0], tb->n[1], tb->n[2]); break;
        case 6: def::rt::crt::fprintf(f, " %7i %20.10E %20.10E\n", 6, dbl(par[0]), dbl(par[1])); break;
        case 7: def::rt::crt::fprintf(f, " %7i %20.10E %20.10E\n", 7, dbl(par[2]), dbl(par[3])); break;
        case 8: def::rt::crt::fprintf(f, " %7i %20.10E %20.10E\n", 8, dbl(par[4]), dbl(par[5])); break;
        default: break;
        }
        if (tb->ndim != 0) def::key::write_table_body(f, tb, 0); // 1402f3130
    } else {
        def::rt::crt::fprintf(f, " %7i %20.10E\n", -id, tb->value);
    }
}

} // namespace

// @src 1403c9ad0  every pair in list order; the first non-zero result stops the write
int write_inter_objects(DB_STEP_DATA* m, std::FILE* f)
{
    Cls_1401fa020* list = &m->inter_obj;
    for (std::int32_t i = 0; i < pair_count(list); i = i + 1) {
        Cls_1401f46e0* p = pair_nth(list, i + 1);
        const int r = write_pair(m, f, p);
        if (r != 0) return r;
    }
    return 0;
}

// @src 1403c8560  CNTACT record of one pair, then its records and windows
int write_pair(DB_STEP_DATA* m, std::FILE* f, Cls_1401f46e0* p)
{
    std::int32_t code = 0;    // local_40
    std::int32_t ftype = 0;   // local_50 (kept across windows, like the original)
    std::int32_t wshape = 0;  // local_44
    if (p == nullptr) return 0;
    const std::int32_t ms = p->u168;
    const std::int32_t sl = p->u16c;
    if (ms == 0 || sl == 0) return 0;
    if (sl < ms) return 0;
    if (ms == sl && p->u170 == 1) return 0;
    if (p->u170 == 0) {
        code = 0;
    } else if (!model_is_2d(m)) {
        if (model_is_3d(m)) {
            if (p->u170 == 1 && p->u174 == 1)
                code = 1;
            else if (p->u170 == 2 && p->u174 == 1)
                code = 2;
            else if (p->u170 == 1 && p->u174 == 0)
                code = 3;
            else {
                if (p->u170 != 2 || p->u174 != 0) return 3;
                code = 4;
            }
        }
    } else if (p->u170 == 1) {
        code = 1;
    } else {
        if (p->u170 != 2) return 3;
        code = 2;
    }
    def::rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", kw(kCNTACT), ms, sl, code);
    if (p->u170 == 0) return 0;

    write_frcfac(m, f, p, 0, 0); // 1403c8800: result ignored
    if (model_is_2d(m)) {
        const int r = write_frcrot(f, p);
        if (r != 0) return r;
    }
    write_ihtcof(m, f, p, ms, sl); // 1403c8870: result ignored

    Cls_140269250* wl = &p->u188;
    for (std::int32_t i = 0; i < winlist_count(wl); i = i + 1) {
        std::int32_t orient = 0; // local_2c
        std::int32_t cnt = 0;    // local_30
        LLWINDEF_DATA* w = winlist_nth(wl, i + 1);
        if (model_is_3d(m)) {
            if (w->shape == 1) {
                wshape = 1;
            } else {
                if (w->shape != 2) return 3;
                wshape = 2;
            }
        }
        if (w->orient == 0)
            orient = 1;
        else if (w->orient == 1)
            orient = 2;
        else if (w->orient == 2)
            orient = -std::abs(w->orient_id); // 140484ab8
        const Cls_1401fb020* fb = &w->fric;
        if (fb->u000 == 1)
            code = 1;
        else if (fb->u000 == 2)
            code = 2;
        else if (fb->u000 == 3)
            code = 3;
        else {
            if (fb->u000 != 4) return 3;
            code = 4;
        }
        if (fb->u000 == 4) {
            if (!model_is_2d(m)) {
                def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i %7i %7i %7i\n", kw(kFRCWIN), ms, sl,
                             w->index, wshape, code, 0, 0, orient);
                def::rt::crt::fprintf(f, " %20.10E %20.10E %20.10E\n", w->u350[0], w->u350[1], w->u350[2]);
            } else {
                def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i %7i %7i %7i %15.7E %15.7E\n", kw(kFRCWIN),
                             ms, sl, w->index, w->npts, code, 0, 0, orient, w->u350[0], w->u350[1]);
            }
            window_write_points(f, w, 0); // 1402f4890
            if (fb->u004 == 0) {
                if (0 <= fb->u010 && fb->u010 <= 8) ftype = fb->u010;
            }
            if (fb->u005 == 0) {
                if (0 <= fb->u014 && fb->u014 <= 8) ftype = fb->u014;
            }
            write_friction_sub(f, fb, 0);
            write_friction_sub(f, fb, 1);
        } else {
            if (fb->u004 == 0) {
                if (fb->u010 == 0) {
                    ftype = 0;
                    if (!model_is_2d(m)) {
                        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i %7i %20.10E %7i\n", kw(kFRCWIN), ms,
                                     sl, w->index, wshape, code, 0, fb->u018.value, orient);
                    } else {
                        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i %7i %15.7E %7i %15.7E %15.7E\n",
                                     kw(kFRCWIN), ms, sl, w->index, w->npts, code, 0,
                                     fb->u018.value, orient, w->u350[0], w->u350[1]);
                    }
                } else {
                    switch (fb->u010) {
                    case 1: ftype = 1; cnt = fb->u018.n[0]; break;
                    case 2: ftype = 2; cnt = fb->u018.n[0]; break;
                    case 3: ftype = 3; cnt = fb->u018.n[0]; break;
                    case 4: ftype = 4; cnt = fb->u018.n[0]; break;
                    case 5: ftype = 5; cnt = 0; break;
                    case 6: ftype = 6; cnt = 0; break;
                    case 7: ftype = 7; cnt = 0; break;
                    case 8: ftype = 8; cnt = 0; break;
                    default: break; // ftype keeps the previous window's value
                    }
                    if (!model_is_2d(m)) {
                        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i %7i %7i %7i\n", kw(kFRCWIN), ms, sl,
                                     w->index, wshape, code, ftype, cnt, orient);
                    } else {
                        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i %7i %7i %7i %15.7E %15.7E\n",
                                     kw(kFRCWIN), ms, sl, w->index, w->npts, code, ftype, cnt, orient,
                                     w->u350[0], w->u350[1]);
                    }
                }
            } else {
                if (!model_is_2d(m)) {
                    def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i %7i %20.10E %7i\n", kw(kFRCWIN), ms, sl,
                                 w->index, wshape, code, -fb->u008, fb->u018.value, orient);
                } else {
                    def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i %7i %15.7E %7i %15.7E %15.7E\n",
                                 kw(kFRCWIN), ms, sl, w->index, w->npts, code, -fb->u008,
                                 fb->u018.value, orient, w->u350[0], w->u350[1]);
                }
            }
            if (model_is_3d(m)) {
                def::rt::crt::fprintf(f, " %20.10E %20.10E %20.10E\n", w->u350[0], w->u350[1], w->u350[2]);
            }
            window_write_points(f, w, 0);
            const std::int32_t ty = fb->u010;
            if (ty == 5) {
                def::rt::crt::fprintf(f, " %7i %7i %7i\n", fb->u018.n[0], fb->u018.n[1], fb->u018.n[2]);
            } else if (ty == 6) {
                def::rt::crt::fprintf(f, " %20.10E %20.10E\n", dbl(fb->u108[0]), dbl(fb->u108[1]));
            } else if (ty == 7) {
                def::rt::crt::fprintf(f, " %20.10E %20.10E\n", dbl(fb->u108[2]), dbl(fb->u108[3]));
            } else if (ty == 8) {
                def::rt::crt::fprintf(f, " %20.10E %20.10E\n", dbl(fb->u108[4]), dbl(fb->u108[5]));
            }
            if (fb->u018.ndim != 0) def::key::write_table_body(f, &fb->u018, 0);
        }
    }
    (void)ftype;
    int r = write_sepres(f, p);
    if (r == 0) r = write_sepden(f, m, p);
    if (r == 0) r = write_intrst(f, p);
    if (r == 0) r = write_cntmth(f, m, p);
    if (r == 0) r = write_wmodel(f, p);
    if (r == 0) r = write_rgdcnt(f, p);
    return r;
}

// @src 1403c9be0  FRCFAC record (master/slave 0 -> taken from the pair)
int write_frcfac(DB_STEP_DATA*, std::FILE* f, Cls_1401f46e0* p, std::int32_t master,
                 std::int32_t slave)
{
    if (master == 0) master = p->u168;
    if (slave == 0) slave = p->u16c;
    if (master == 0 || slave == 0) return 0;
    const Cls_1401fb020* fb = &p->u000;
    std::int32_t mode;
    if (fb->u000 == 0)
        mode = 0;
    else if (fb->u000 == 1)
        mode = 1;
    else if (fb->u000 == 2)
        mode = 2;
    else if (fb->u000 == 3)
        mode = 3;
    else {
        if (fb->u000 != 4) return 3;
        mode = 4;
    }
    if (fb->u000 == 4) {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", kw(kFRCFAC), master, slave, mode);
        write_friction_sub(f, fb, 0);
        write_friction_sub(f, fb, 1);
    } else if (fb->u004 == 0) {
        const DYNARY* tb = &fb->u018;
        switch (fb->u010) {
        case 0:
            def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %20.10E\n", kw(kFRCFAC), master, slave, mode, 0,
                         tb->value);
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i\n", kw(kFRCFAC), master, slave, mode, fb->u010,
                         tb->n[0]);
            break;
        case 5:
            def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i %7i %7i\n", kw(kFRCFAC), master, slave, mode, 5,
                         tb->n[0], tb->n[1], tb->n[2]);
            break;
        case 6:
            def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %20.10E %20.10E\n", kw(kFRCFAC), master, slave, mode,
                         6, dbl(fb->u108[0]), dbl(fb->u108[1]));
            break;
        case 7:
            def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %20.10E %20.10E\n", kw(kFRCFAC), master, slave, mode,
                         7, dbl(fb->u108[2]), dbl(fb->u108[3]));
            break;
        case 8:
            def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %20.10E %20.10E\n", kw(kFRCFAC), master, slave, mode,
                         8, dbl(fb->u108[4]), dbl(fb->u108[5]));
            break;
        default:
            break;
        }
        if (tb->ndim != 0) def::key::write_table_body(f, tb, 0);
    } else {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %20.10E\n", kw(kFRCFAC), master, slave, mode, -fb->u008,
                     fb->u018.value);
    }
    return 0;
}

// @src 1403caa40  FRCROT record (2D)
int write_frcrot(std::FILE* f, Cls_1401f46e0* p)
{
    const std::int32_t ms = p->u168;
    const std::int32_t sl = p->u16c;
    std::int32_t code;
    if (p->u240.ndim == 0) {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E\n", kw(kFRCROT), ms, sl, 0, p->u240.value);
    } else {
        if (p->u240.ndim != 1) return 3;
        if (p->u240.var[0] == 1)
            code = 1;
        else {
            if (p->u240.var[0] != 4) return 3;
            code = 2;
        }
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", kw(kFRCROT), ms, sl, code, p->u240.n[0]);
    }
    def::key::write_table_body(f, &p->u240, 0);
    return 0;
}

// @src 1403ca830  IHTCOF record (master/slave 0 -> taken from the pair)
int write_ihtcof(DB_STEP_DATA*, std::FILE* f, Cls_1401f46e0* p, std::int32_t master,
                 std::int32_t slave)
{
    std::int32_t code;
    if (master == 0) master = p->u168;
    if (slave == 0) slave = p->u16c;
    if (master == 0 || slave == 0) return 0;
    if (p->u234 == 1) {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E\n", kw(kIHTCOF), master, slave, -p->u230, 0.0);
    } else if (p->u1b8.ndim == 0) {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E\n", kw(kIHTCOF), master, slave, 0, p->u1b8.value);
    } else {
        if (p->u1b8.ndim != 1) return 3;
        if (p->u1b8.var[0] == 1)
            code = 1;
        else {
            if (p->u1b8.var[0] != 4) return 3;
            code = 2;
        }
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", kw(kIHTCOF), master, slave, code, p->u1b8.n[0]);
        def::key::write_table_body(f, &p->u1b8, 0);
    }
    return 0;
}

// @src 1403cad60  SEPRES record
int write_sepres(std::FILE* f, Cls_1401f46e0* p)
{
    std::int32_t c;
    if (p->u178 == 0)
        c = 0;
    else if (p->u178 == 1)
        c = 1;
    else {
        if (p->u178 != 2) return 3;
        c = 2;
    }
    def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E\n", kw(kSEPRES), p->u168, p->u16c, c, dbl(p->u180));
    return 0;
}

// @src 1403caf20  SEPDEN record (2D)
int write_sepden(std::FILE* f, DB_STEP_DATA* m, Cls_1401f46e0* p)
{
    if (model_is_2d(m)) {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %20.10E\n", kw(kSEPDEN), p->u168, p->u16c, dbl(p->u238));
    }
    return 0;
}

// @src 1403cb0a0  INTRST record
int write_intrst(std::FILE* f, Cls_1401f46e0* p)
{
    std::int32_t code;
    if (p->u2b8.ndim == 0) {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E\n", kw(kINTRST), p->u168, p->u16c, 0,
                     p->u2b8.value);
    } else {
        if (p->u2b8.ndim != 1) return 3;
        if (p->u2b8.var[0] == 3)
            code = 1;
        else {
            if (p->u2b8.var[0] != 4) return 3;
            code = 2;
        }
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", kw(kINTRST), p->u168, p->u16c, code, p->u2b8.n[0]);
    }
    def::key::write_table_body(f, &p->u2b8, 0);
    return 0;
}

// @src 1403cbce0  CNTMTH record (3D)
int write_cntmth(std::FILE* f, DB_STEP_DATA* m, Cls_1401f46e0* p)
{
    if (model_is_3d(m)) {
        if (p->u330 == 0) {
            def::rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", kw(kCNTMTH), p->u168, p->u16c, 0);
        } else {
            if (p->u330 != 1) return 3;
            def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %20.10E\n", kw(kCNTMTH), p->u168, p->u16c, 1,
                         p->u334, dbl(p->u338));
        }
    }
    return 0;
}

// @src 1403cb840  WMODEL record
int write_wmodel(std::FILE* f, Cls_1401f46e0* p)
{
    if (p->u340 == 1) {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", kw(kWMODEL), p->u168, p->u16c, 1, 4);
        def::rt::crt::fprintf(f, " %20.10E %20.10E %20.10E %20.10E\n", dbl(p->u348[0]), dbl(p->u348[1]),
                     dbl(p->u348[2]), dbl(p->u348[3]));
    } else if (p->u340 == 2) {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", kw(kWMODEL), p->u168, p->u16c, 2, 2);
        def::rt::crt::fprintf(f, " %20.10E %20.10E\n", dbl(p->u348[4]), dbl(p->u348[5]));
    } else if (p->u340 == 3) {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", kw(kWMODEL), p->u168, p->u16c, -p->u378);
    }
    return 0;
}

// @src 1403cbb10  RGDCNT record (only when +0x37c > 0)
int write_rgdcnt(std::FILE* f, Cls_1401f46e0* p)
{
    if (0 < p->u37c) {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i %20.10E\n", kw(kRGDCNT), p->u168, p->u16c, p->u37c,
                     p->u380, p->u384, dbl(p->u388));
    }
    return 0;
}

} // namespace def::key::g3
