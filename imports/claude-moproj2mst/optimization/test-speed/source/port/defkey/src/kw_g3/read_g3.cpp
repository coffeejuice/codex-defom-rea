// Group 3 KEY reader FUN_140388fb0 (inter-object keywords). Packet kw-g3.
// Specs: docs/re/keywords/{CNTACT,FRCFAC,IHTCOF,SEPRES,INTRST,FRCROT,SEPDEN,FRCWIN,CNTMTH,
// WMODEL,RGDCNT}.md.
#include <defkey/kw_g3/kw_g3.hpp>

#include <cstdlib>

#include <defkey/crt_scan.hpp>
#include <defkey/line_io.hpp>
#include <defkey/table_io.hpp>
#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/db_step_data.hpp>

#include "g3_internal.hpp"

namespace def::key::g3 {

using detail::dbl;
using detail::trunc_i32;

namespace {

// part of @src 140388fb0: apply (type t, numbers d1, d2, d3) to friction k (0 or 1) of a friction
// block. The original repeats this code inline for FRCFAC (both frictions, and the single-friction
// line) and for the two frictions of an FRCWIN kind-4 window. Returns the table reader status when
// it runs, else `st` unchanged.
int apply_friction(std::FILE* f, Cls_1401fb020* fb, int k, std::int32_t t, double d1, double d2,
                   double d3, int st)
{
    std::uint8_t& fixed = k == 0 ? fb->u004 : fb->u005;
    std::int32_t& id = k == 0 ? fb->u008 : fb->u00c;
    std::int32_t& type = k == 0 ? fb->u010 : fb->u014;
    DYNARY* tb = k == 0 ? &fb->u018 : &fb->u090;
    std::uint64_t* par = fb->u108 + (k == 0 ? 0 : 6); // 0x108.. (friction 1), 0x138.. (friction 2)
    if (t < 0) {
        fixed = 1;
        id = -t;
        tb->ndim = 0;
        tb->value = d1;
        return st;
    }
    fixed = 0;
    def::model::dynary_reinit_keep(tb, 0x15); // 14012fc70
    type = t;
    switch (type) {
    case 0:
        tb->ndim = 0;
        tb->value = d1;
        break;
    case 1:
        tb->ndim = 1;
        tb->var[0] = 1;
        tb->n[0] = trunc_i32(d1);
        break;
    case 2:
        tb->ndim = 1;
        tb->var[0] = 4;
        tb->n[0] = trunc_i32(d1);
        break;
    case 3:
        tb->ndim = 1;
        tb->var[0] = 3;
        tb->n[0] = trunc_i32(d1);
        break;
    case 4:
        tb->ndim = 1;
        tb->var[0] = 0x15;
        tb->n[0] = trunc_i32(d1);
        break;
    case 5:
        tb->ndim = 3;
        tb->var[0] = 4;
        tb->var[1] = 3;
        tb->var[2] = 0x15;
        tb->n[0] = trunc_i32(d1);
        tb->n[1] = trunc_i32(d2);
        tb->n[2] = trunc_i32(d3);
        break;
    case 6:
        tb->ndim = 0;
        dbl(par[0]) = d1;
        dbl(par[1]) = d2;
        break;
    case 7:
        tb->ndim = 0;
        dbl(par[2]) = d1;
        dbl(par[3]) = d2;
        break;
    case 8:
        tb->ndim = 0;
        dbl(par[4]) = d1;
        dbl(par[5]) = d2;
        break;
    default:
        break;
    }
    if (tb->ndim != 0) st = def::key::read_table_body(f, tb); // 14041a800
    return st;
}

// part of @src 140388fb0: FRCFAC mode 4 / FRCWIN kind 4: two lines "%i %Flf %Flf %Flf", one per
// friction. Returns 2 on a short line (the caller returns it), else the last status: the first
// friction's table status is overwritten by the second line's helper, like the original.
int read_two_frictions(std::FILE* f, Cls_1401fb020* fb, int* st)
{
    std::int32_t t;
    double d1, d2, d3;
    *st = def::key::read_i_d3(f, &t, &d1, &d2, &d3); // 140415880
    if (*st != 0) return 2;
    *st = apply_friction(f, fb, 0, t, d1, d2, d3, *st);
    *st = def::key::read_i_d3(f, &t, &d1, &d2, &d3);
    if (*st != 0) return 2;
    *st = apply_friction(f, fb, 1, t, d1, d2, d3, *st);
    return 0;
}

} // namespace

// @src 140388fb0  group 3 parser: one inter-object keyword
int read_keyword(DB_STEP_DATA* m, std::FILE* f, int id)
{
    Cls_1401fa020* list = &m->inter_obj; // m + 0x13c8
    int st = 0;                         // local_60
    std::int32_t master = 0;            // local_108[0]
    std::int32_t slave = 0;             // local_a4
    std::int32_t a = 0;                 // local_64: mode / code / kind
    std::int32_t t = 0;                 // local_a8: type
    std::int32_t npts2d = 0;            // local_a0
    std::int32_t shape = 0;             // local_68
    std::int32_t ntok = 0;              // local_5c
    double d1 = 0.0;                    // local_58
    double d2 = 0.0;                    // local_f8
    double d3 = 0.0;                    // local_f0[0]
    Cls_1401f46e0* p = nullptr;         // local_70

    if (id == kCNTACT) {
        st = def::key::read_tokens(f, &ntok, 3); // 140413900
        if (ntok == 1) {
            const std::int32_t v = def::key::token_int(0);
            if (v == 0) list->clear(); // 1401fade0
            return 0;
        }
        if (ntok == 3) {
            master = def::key::token_int(0);
            slave = def::key::token_int(1);
            a = def::key::token_int(2);
            p = pair_find(list, master, slave);
            if (p == nullptr) {
                p = pair_append(list);
                p->u168 = master;
                p->u16c = slave;
            }
            if (a == 0) p->u170 = 0;
            if (!model_is_2d(m)) {
                if (a == 1) {
                    p->u170 = 1;
                    p->u174 = 1;
                } else if (a == 2) {
                    p->u170 = 2;
                    p->u174 = 1;
                } else if (a == 3) {
                    p->u170 = 1;
                    p->u174 = 0;
                } else if (a == 4) {
                    p->u170 = 2;
                    p->u174 = 0;
                }
            } else if (a == 1) {
                p->u170 = 1;
            } else if (a == 2) {
                p->u170 = 2;
            }
        }
    } else {
        if (def::key::crt::fscanf(f, "%i %i", &master, &slave) == -1) return 2;
        p = pair_find(list, master, slave);
        if (p == nullptr) {
            p = pair_append(list);
            p->u168 = master;
            p->u16c = slave;
        }
        Cls_1401fb020* fb = &p->u000;
        switch (id - 0x110) {
        case 0: { // FRCFAC
            st = def::key::read_ii_d3(f, &a, &t, &d1, &d2, &d3); // 140416100
            if (st != 0) return 2;
            if (a == 0)
                fb->u000 = 0;
            else if (a == 1)
                fb->u000 = 1;
            else if (a == 2)
                fb->u000 = 2;
            else if (a == 3)
                fb->u000 = 3;
            else if (a == 4)
                fb->u000 = 4;
            if (fb->u000 == 4) {
                if (read_two_frictions(f, fb, &st) != 0) return 2;
            } else {
                st = apply_friction(f, fb, 0, t, d1, d2, d3, st);
            }
            break;
        }
        case 1: // IHTCOF
            st = def::key::read_i_d(f, &t, &d1); // 1404154c0
            if (st != 0) return 2;
            def::model::dynary_reinit_keep(&p->u1b8, 0x17); // 14012fc70
            if (t == 0) {
                p->u1b8.ndim = 0;
                p->u1b8.value = d1;
            } else if (t == 1) {
                p->u1b8.ndim = 1;
                p->u1b8.var[0] = 1;
                p->u1b8.n[0] = trunc_i32(d1);
                st = def::key::read_table_body(f, &p->u1b8);
            } else if (t == 2) {
                p->u1b8.ndim = 1;
                p->u1b8.var[0] = 4;
                p->u1b8.n[0] = trunc_i32(d1);
                st = def::key::read_table_body(f, &p->u1b8);
            }
            if (t < 0) {
                p->u230 = -t;
                p->u234 = 1;
            } else {
                p->u230 = 0;
                p->u234 = 0;
            }
            break;
        case 2: // SEPRES
            if (def::key::read_i_d(f, &a, &d1) != 0) return 2;
            if (a == 0)
                p->u178 = 0;
            else if (a == 1)
                p->u178 = 1;
            else if (a == 2)
                p->u178 = 2;
            dbl(p->u180) = d1;
            st = 0;
            break;
        case 3: // INTRST
            st = def::key::read_i_d(f, &t, &d1);
            if (st != 0) return 2;
            def::model::dynary_reinit_keep(&p->u2b8, 0x4e);
            if (t == 0) {
                p->u2b8.ndim = 0;
                p->u2b8.value = d1;
            } else if (t == 1) {
                p->u2b8.ndim = 1;
                p->u2b8.var[0] = 3;
                p->u2b8.n[0] = trunc_i32(d1);
            } else if (t == 2) {
                p->u2b8.ndim = 1;
                p->u2b8.var[0] = 4;
                p->u2b8.n[0] = trunc_i32(d1);
            }
            st = def::key::read_table_body(f, &p->u2b8);
            break;
        case 4: // FRCROT
            st = def::key::read_i_d(f, &t, &d1);
            if (st != 0) return 2;
            def::model::dynary_reinit_keep(&p->u240, 0x16);
            if (t == 0) {
                p->u240.ndim = 0;
                p->u240.value = d1;
            } else if (t == 1) {
                p->u240.ndim = 1;
                p->u240.var[0] = 1;
                p->u240.n[0] = trunc_i32(d1);
            } else if (t == 2) {
                p->u240.ndim = 1;
                p->u240.var[0] = 4;
                p->u240.n[0] = trunc_i32(d1);
            }
            st = def::key::read_table_body(f, &p->u240);
            break;
        case 5: // SEPDEN: 2D only; in 3D the rest of the line is left for the main loop
            if (model_is_2d(m)) st = def::key::read_d(f, &dbl(p->u238)); // 140414990
            break;
        case 6: { // FRCWIN
            std::int32_t ntk = 0;   // local_38
            std::int32_t w = 0;     // local_3c
            std::int32_t q = 0;     // local_40
            if (!model_is_2d(m)) {
                shape = 0;
                st = def::key::read_tokens(f, &ntk, 6);
                w = def::key::token_int(0);
                shape = def::key::token_int(1);
                a = def::key::token_int(2);
                t = def::key::token_int(3);
                d2 = def::key::token_double(4);
                q = def::key::token_int(5);
            } else {
                st = def::key::read_tokens(f, &ntk, 8);
                w = def::key::token_int(0);
                npts2d = def::key::token_int(1);
                a = def::key::token_int(2);
                t = def::key::token_int(3);
                d2 = def::key::token_double(4);
                q = def::key::token_int(5);
            }
            if (st != 0) return 2;
            LLWINDEF_DATA* win = winlist_extend(&p->u188, w); // 140269bc0
            if (win == nullptr) detail::null_deref(0x14025d1a0); // window w < 1 on a pair without windows
            window_reset(win, 1); // 14025d1a0
            if (a == 1)
                win->fric.u000 = 1;
            else if (a == 2)
                win->fric.u000 = 2;
            else if (a == 3)
                win->fric.u000 = 3;
            else if (a == 4)
                win->fric.u000 = 4;
            if (!model_is_2d(m)) {
                st = def::key::read_d3(f, &win->u350[0], &win->u350[1], &win->u350[2]); // 140414c20
                if (st != 0) return 2;
                if (shape == 1) {
                    win->shape = 1;
                } else {
                    if (shape != 2) return 2;
                    win->shape = 2;
                }
                st = window_read_points_3d(f, win, true); // 14041b7d0
                window_build_faces(win);                  // 140267760
            } else {
                st = window_read_points_2d(f, win, npts2d, true); // 14041b6a0
                win->u350[0] = def::key::token_double(6);
                win->u350[1] = def::key::token_double(7);
            }
            if (q == 0) {
                win->orient = 2;
                if (p->u170 == 1)
                    win->orient_id = -p->u16c;
                else
                    win->orient_id = -p->u168;
            } else if (q == 1) {
                win->orient = 0;
            } else if (q == 2) {
                win->orient = 1;
            } else {
                if (-1 < q) return 2;
                win->orient = 2;
                win->orient_id = std::abs(q); // 140484ab8
            }
            Cls_1401fb020* wf = &win->fric;
            if (wf->u000 == 4) {
                if (read_two_frictions(f, wf, &st) != 0) return 2;
            } else if (t < 0) {
                wf->u004 = 1;
                wf->u008 = -t;
                wf->u018.ndim = 0;
                wf->u018.value = d2;
            } else {
                wf->u004 = 0;
                def::model::dynary_reinit_keep(&wf->u018, 0x15);
                wf->u010 = t;
                DYNARY* tb = &wf->u018;
                std::int32_t c1, c2, c3; // local_98, local_94, local_90[0]
                double p1, p2;           // local_f8, local_f0[0]
                switch (wf->u010) {
                case 0:
                    tb->ndim = 0;
                    tb->value = d2;
                    break;
                case 1:
                    tb->ndim = 1;
                    tb->var[0] = 1;
                    tb->n[0] = trunc_i32(d2);
                    break;
                case 2:
                    tb->ndim = 1;
                    tb->var[0] = 4;
                    tb->n[0] = trunc_i32(d2);
                    break;
                case 3:
                    tb->ndim = 1;
                    tb->var[0] = 3;
                    tb->n[0] = trunc_i32(d2);
                    break;
                case 4:
                    tb->ndim = 1;
                    tb->var[0] = 0x15;
                    tb->n[0] = trunc_i32(d2);
                    break;
                case 5:
                    st = def::key::read_iii(f, &c1, &c2, &c3, nullptr); // 140413fb0
                    tb->ndim = 3;
                    tb->var[0] = 4;
                    tb->var[1] = 3;
                    tb->var[2] = 0x15;
                    tb->n[0] = c1;
                    tb->n[1] = c2;
                    tb->n[2] = c3;
                    break;
                case 6:
                    st = def::key::read_dd(f, &p1, &p2); // 140414a40
                    tb->ndim = 0;
                    dbl(wf->u108[0]) = p1;
                    dbl(wf->u108[1]) = p2;
                    break;
                case 7:
                    st = def::key::read_dd(f, &p1, &p2);
                    tb->ndim = 0;
                    dbl(wf->u108[2]) = p1;
                    dbl(wf->u108[3]) = p2;
                    break;
                case 8:
                    st = def::key::read_dd(f, &p1, &p2);
                    tb->ndim = 0;
                    dbl(wf->u108[4]) = p1;
                    dbl(wf->u108[5]) = p2;
                    break;
                default:
                    break;
                }
                if (tb->ndim != 0) st = def::key::read_table_body(f, tb);
            }
            break;
        }
        case 7: { // CNTMTH
            std::int32_t b;
            st = def::key::read_ii_d(f, &a, &b, &d2); // 140415f00
            if (a == 0) {
                p->u330 = 0;
            } else {
                p->u330 = 1;
                p->u334 = b;
                dbl(p->u338) = d2;
            }
            break;
        }
        case 8: { // WMODEL
            std::int32_t x; // local_98, unused
            st = def::key::read_ii(f, &a, &x); // 140413ee0
            if (st != 0) return 2;
            if (a == 0) {
                p->u340 = 0;
                st = 0;
            } else if (a == 1) {
                p->u340 = 1;
                st = def::key::read_d4(f, &dbl(p->u348[0]), &dbl(p->u348[1]), &dbl(p->u348[2]),
                                   &dbl(p->u348[3])); // 140414e40
            } else if (a == 2) {
                p->u340 = 2;
                st = def::key::read_dd(f, &dbl(p->u348[4]), &dbl(p->u348[5])); // 140414a40
            } else {
                st = 0;
                if (a < 0) {
                    p->u340 = 3;
                    p->u378 = -a;
                }
            }
            break;
        }
        case 9: { // RGDCNT
            std::int32_t b, c;
            if (def::key::read_iii_d(f, &a, &b, &c, &d2) != 0) return 2; // 1404164e0
            if (-1 < a && a < 5) {
                p->u37c = a;
                p->u380 = b;
                p->u384 = c;
                dbl(p->u388) = d2;
            }
            st = 0;
            break;
        }
        default:
            return 2;
        }
    }
    // mirroring: every pair (m,s) with m != s is copied into (s,m)
    if (master != slave) {
        Cls_1401f46e0* q = pair_find(list, slave, master);
        if (q == nullptr) {
            q = pair_append(list);
            q->u168 = slave;
            q->u16c = master;
        }
        if (p->u170 == 1)
            q->u170 = 2;
        else if (p->u170 == 2)
            q->u170 = 1;
        else
            q->u170 = 0;
        pair_copy_keep_ids(q, p); // 1401f9e80
    }
    return st == 0 ? 0 : 2;
}

} // namespace def::key::g3
