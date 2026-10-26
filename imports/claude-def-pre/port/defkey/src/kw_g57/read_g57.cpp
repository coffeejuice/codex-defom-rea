// Group 5 KEY reader FUN_140383120 (phase transformation) and group 7 reader FUN_14037e530 (DIM).
// Packet kw-g57. Specs: docs/re/keywords/{PHASTF,TTTD,TTTN,PHASLH,PHASVL,TRNSFP,PHEQVF,COARSN,
// IPSNRG,VARSEL,DIM}.md.
#include <defkey/kw_g57.hpp>

#include <cstdlib>

#include <defkey/crt_scan.hpp>
#include <defkey/line_io.hpp>
#include <defkey/table_io.hpp>
#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/phase/phase_ops.hpp>
#include <defmodel/step/name_arrays.hpp>

#include "g57_internal.hpp"

namespace def::key::g57 {

using detail::trunc_i32;

namespace {

// part of @src 140383120: PHASLH / PHASVL / IPSNRG header "t a b" applied to a re-typed table;
// `var2` is the second variable (12 for PHASLH/PHASVL, 23 for IPSNRG)
void apply_tab3(DYNARY* t, std::int32_t code, double a, double b, std::int32_t var2)
{
    if (code == 0) {
        t->ndim = 0;
        t->value = a;
    } else if (code == 1) {
        t->ndim = 1;
        t->var[0] = 3;
        t->n[0] = trunc_i32(a);
    } else if (code == 2) {
        t->ndim = 1;
        t->var[0] = var2;
        t->n[0] = trunc_i32(a);
    } else if (code == 3) {
        t->ndim = 2;
        t->var[0] = 3;
        t->var[1] = var2;
        t->n[0] = trunc_i32(a);
        t->n[1] = trunc_i32(b);
    }
}

} // namespace

// @src 140383120  group 5 parser: one phase-transformation keyword
int read_phase_keyword(DB_STEP_DATA* m, std::FILE* f, int id)
{
    int st = 0; // local_60
    std::int32_t from = 0, to = 0;
    if (crt::fscanf(f, "%i %i", &from, &to) == -1) return 2;
    PhasePair* p = def::model::phase_pair_find(&m->phase_pairs, from, to); // 14033a040
    if (p == nullptr) {
        p = def::model::phase_pair_append(&m->phase_pairs); // 140339df0
        p->from = from;
        p->to = to;
    }
    std::int32_t a = 0;  // local_6c
    std::int32_t t = 0;  // local_a8
    double d1 = 0.0;     // local_f8
    double d2 = 0.0;     // local_f0
    switch (id - 0x86) {
    case 0: // PHASTF: the value is stored even when the helper fails
        st = read_i(f, &a, false); // 140413de0
        p->phastf = a;
        if (a == 0) {
            p->phastf_on = 0;
        } else {
            p->phastf_on = 1;
            def::model::count_active_phase_pairs(m); // 1401d6450
            def::model::call_1401d0fc0(m, 0);  // 1401d0fc0
        }
        break;
    case 1: { // TTTD
        std::int32_t kind = 0; // local_98
        st = read_ii(f, &a, &kind); // 140413ee0
        if (st != 0) return 2;
        if (kind == 0)
            p->tttd_kind = 0;
        else if (kind == 1)
            p->tttd_kind = 1;
        else if (kind == 2)
            p->tttd_kind = 2;
        if (a == 1) {
            std::int32_t sub = 0, nc = 0, x = 0; // local_5c, local_58, local_54
            st = read_iii(f, &sub, &nc, &x, nullptr); // 140413fb0
            if (st != 0) return 2;
            st = read_dd(f, &d1, &d2); // 140414a40
            if (st != 0) return 2;
            if (sub == 0) {
                p->tttd_type = 0;
                def::model::dynary_reinit_keep(&p->tttd_t1, 0x2c); // 14012fc70
            } else if (sub == 1) {
                p->tttd_type = 1;
                def::model::dynary_reinit_keep(&p->tttd_t1, 0x2d);
            } else if (sub == 2) {
                p->tttd_type = 2;
                def::model::dynary_reinit_keep(&p->tttd_t1, 0x2e);
            } else if (sub == 3) {
                p->tttd_type = 3;
                def::model::dynary_reinit_keep(&p->tttd_t1, 0x2f);
            }
            p->tttd_nc = nc;
            p->tttd_t1.ncomp = nc;
            p->tttd_x = x;
            if (p->tttd_nc == 1) {
                p->tttd_f1 = d1;
                p->tttd_f1b = d2;
            } else {
                p->tttd_f1b = d1;
                p->tttd_f2 = d2;
            }
            st = read_iii(f, &p->tttd_t1.n[0], &p->tttd_t1.n[1], &p->tttd_t1.n[2], nullptr);
            if (st != 0) return 2;
            st = read_table_body(f, &p->tttd_t1, 0); // 14041a800
        } else if (a == 2) {
            std::int32_t q = 0, n0 = 0, n1 = 0, n2 = 0; // local_98, local_94, local_90, local_8c
            p->tttd_type = 4;
            st = read_i4(f, &q, &n0, &n1, &n2); // 140414100
            if (st != 0) return 2;
            def::model::dynary_reinit_keep(&p->tttd_t2, 0x30);
            p->tttd_t2.n[0] = n0;
            p->tttd_t2.n[1] = n1;
            p->tttd_t2.n[2] = n2;
            if (q != 2) return 3;
            p->tttd_t2.ncomp = 2;
            st = read_table_body(f, &p->tttd_t2, 0);
        } else if (a == 3) {
            p->tttd_type = 5;
            double* c = p->tttd_c3;
            if (crt::fscanf(f, "%Flf %Flf %Flf %Flf %Flf %Flf %Flf %Flf %Flf %Flf %Flf", &c[0], &c[1],
                            &c[2], &c[3], &c[4], &c[5], &c[6], &c[7], &c[8], &c[9], &c[10]) == -1)
                return 2;
        } else if (a == 4) {
            std::int32_t n = 0; // local_70
            p->tttd_type = 6;
            st = read_d4(f, &p->tttd_c4[0], &p->tttd_c4[1], &p->tttd_c4[2], &p->tttd_c4[3]); // 140414e40
            if (st != 0) return 2;
            st = read_i(f, &n, false);
            if (st != 0) return 2;
            def::model::dynary_reinit_keep(&p->tttd_t4, 0x32);
            p->tttd_t4.n[0] = n;
            st = read_table_body(f, &p->tttd_t4, 0);
        } else if (a == 5) {
            p->tttd_type = 7;
            double* c = p->tttd_c5;
            if (crt::fscanf(f, "%Flf %Flf %Flf %Flf %Flf %Flf", &c[0], &c[1], &c[2], &c[3], &c[4],
                            &c[5]) == -1)
                return 2;
        } else if (a == 6) {
            p->tttd_type = 8;
            st = read_d4(f, &p->tttd_c6[0], &p->tttd_c6[1], &p->tttd_c6[2], &p->tttd_c6[3]);
        } else if (a == 7) {
            p->tttd_type = 9;
            double* c = p->tttd_c7;
            if (crt::fscanf(f, "%Flf %Flf %Flf %Flf %Flf %Flf %Flf", &c[0], &c[1], &c[2], &c[3], &c[4],
                            &c[5], &c[6]) == -1)
                return 2;
        } else if (a == 8) {
            std::int32_t n = 0;
            p->tttd_type = 10;
            st = read_i(f, &n, false);
            if (st != 0) return 2;
            def::model::dynary_reinit_keep(&p->tttd_t8, 0x36);
            p->tttd_t8.n[0] = n;
            st = read_table_body(f, &p->tttd_t8, 0);
        } else if (a == 9) {
            std::int32_t ignored = 0, nc = 0; // local_50, local_4c
            st = read_ii(f, &ignored, &nc);
            if (st != 0) return 2;
            st = read_dd(f, &d1, &d2);
            if (st != 0) return 2;
            p->tttd_type = 0xb;
            def::model::dynary_reinit_keep(&p->tttd_t9, 0x2c);
            p->tttd_nc = nc;
            p->tttd_t9.ncomp = nc;
            if (p->tttd_nc == 1) {
                p->tttd_f1 = d1;
                p->tttd_f1b = d2;
            } else {
                p->tttd_f9[0] = d1;
                p->tttd_f9[1] = d2;
            }
            st = read_iii(f, &p->tttd_t9.n[0], &p->tttd_t9.n[1], &p->tttd_t9.n[2], nullptr);
            if (st != 0) return 2;
            st = read_table_body(f, &p->tttd_t9, 0);
        } else if (a >= 10 && a <= 13) {
            std::int32_t nc = 0; // local_48 / local_44 / local_40 / local_3c
            p->tttd_type = a + 2; // 0xc .. 0xf
            st = read_i(f, &nc, false);
            if (st != 0) return 2;
            p->tttd_vec.ncomp = nc; // not re-initialised
            st = read_table_body(f, &p->tttd_vec, 0);
        } else if (a < 0) {
            p->tttd_type = 0x10;
            p->tttd_ref = std::abs(a); // 140484ab8
        }
        break;
    }
    case 2: // TTTN
        st = read_string_next_line(f, p->name, 0x51); // 140413ab0
        break;
    case 3: // PHASLH
        st = read_i_dd(f, &t, &d1, &d2); // 140415680
        if (st != 0) return 2;
        def::model::dynary_reinit_keep(&p->phaslh, 0x1f);
        apply_tab3(&p->phaslh, t, d1, d2, 0xc);
        st = read_table_body(f, &p->phaslh, 0);
        break;
    case 4: // PHASVL
        st = read_i_dd(f, &t, &d1, &d2);
        if (st != 0) return 2;
        def::model::dynary_reinit_keep(&p->phasvl, 0x20);
        apply_tab3(&p->phasvl, t, d1, d2, 0xc);
        st = read_table_body(f, &p->phasvl, 0);
        break;
    case 5: // TRNSFP: the table is not re-initialised
        st = read_ii_dd(f, &a, &t, &d1, &d2); // 140415ff0
        if (st != 0) return 2;
        if (a == 0) {
            p->trnsfp_on = 0;
            st = 0;
        } else {
            p->trnsfp_on = 1;
            apply_tab3(&p->trnsfp, t, d1, d2, 0xc);
            st = read_table_body(f, &p->trnsfp, 0);
        }
        break;
    case 6: { // PHEQVF: t 4..7 act like 0..3
        st = read_i_dd(f, &t, &d1, &d2);
        if (st != 0) return 2;
        def::model::dynary_reinit_keep(&p->pheqvf, 0x6c);
        p->pheqvf_t = t;
        std::int32_t code = -1;
        if (t == 0 || t == 4)
            code = 0;
        else if (t == 1 || t == 5)
            code = 1;
        else if (t == 2 || t == 6)
            code = 2;
        else if (t == 3 || t == 7)
            code = 3;
        apply_tab3(&p->pheqvf, code, d1, d2, 0xc);
        st = read_table_body(f, &p->pheqvf, 0);
        break;
    }
    case 7: { // COARSN: c1 c2 t v and a fifth number that is discarded
        double scratch[8];
        double* const out[5] = {&p->coarsn_c1, &p->coarsn_c2, &d1, &d2, scratch};
        st = read_d5(f, out); // 140414f50
        if (st != 0) return 2;
        p->coarsn_on = 1;
        t = trunc_i32(d1);
        def::model::dynary_reinit_keep(&p->coarsn, 0x6d);
        if (t == 0) {
            p->coarsn.ndim = 0;
            p->coarsn.value = d2;
        } else if (t == 1) {
            p->coarsn.ndim = 1;
            p->coarsn.var[0] = 3;
            p->coarsn.n[0] = trunc_i32(d2);
        }
        st = read_table_body(f, &p->coarsn, 0);
        break;
    }
    case 8: // IPSNRG
        st = read_i_dd(f, &t, &d1, &d2);
        if (st != 0) return 2;
        def::model::dynary_reinit_keep(&p->ipsnrg, 0x6e);
        apply_tab3(&p->ipsnrg, t, d1, d2, 0x17);
        st = read_table_body(f, &p->ipsnrg, 0);
        break;
    case 9: // VARSEL: stored even when the helper fails
        st = read_i(f, &a, false);
        p->varsel = a;
        break;
    default:
        return 2;
    }
    return st == 0 ? 0 : 2;
}

// @src 14037e530  group 7 parser: DIM d (2 or 3) routes the following keywords
int read_dim_keyword(DB_STEP_DATA* m, std::FILE* f, int id)
{
    if (id != kDIM) return 2;
    std::int32_t d = 0;
    const int st = read_i(f, &d, false); // 140413de0
    if (d == 2 || d == 3) def::model::set_dim_routing(m, &d); // 1401d8140
    return st == 0 ? 0 : 2;
}

} // namespace def::key::g57
