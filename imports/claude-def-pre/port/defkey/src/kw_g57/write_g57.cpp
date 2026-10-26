// Group 5 KEY writer (packet kw-g57): FUN_1403cbf00 per phase pair, the column-2 write handlers,
// the column-3 accessors and the group 5 header writers. Specs: docs/re/keywords/<KW>.md
// "KEY grammar (write)", _common_g457.md section 6.
//
// Output goes through def::rt::crt::fprintf (the old CRT's float formatting).
#include <defkey/kw_g57.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <defkey/key_writer.hpp>
#include <defkey/table_io.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/phase/phase_ops.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

namespace def::key::g57 {



namespace {

// 1402f7720 (keyword registry packet): keyword name by id
const char* kw(int id) { return def::key::keyword_label(id); }

PhasePair* pair_at(DB_STEP_DATA* m, const std::vector<int>& v)
{
    return def::model::phase_pair_nth(&m->phase_pairs, v[0]); // 140159a70(v, 0), 140339fe0
}

} // namespace

// @src 1402f43a0  "name from to t ..." header from the table shape (t: 0 const, 1 var 3, 2 other
// var, 3 for ndim 2), then the body (mode 0)
int write_header_pair(std::FILE* f, const char* name, std::int32_t from, std::int32_t to,
                      const DYNARY* t)
{
    if (f == nullptr) return 2;
    if (t->ndim == 0) {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E\n", name, from, to, 0, t->value);
    } else if (t->ndim == 1) {
        const std::int32_t code = t->var[0] == 3 ? 1 : 2;
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", name, from, to, code, t->n[0]);
    } else if (t->ndim == 2) {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i\n", name, from, to, 3, t->n[0], t->n[1]);
    }
    const int r = write_table_body(f, t, 0); // 1402f3130
    return r;
}

// @src 1402f4530  the same with the stored type code instead of one derived from the shape
int write_header_pair_t(std::FILE* f, const char* name, std::int32_t from, std::int32_t to,
                        std::int32_t t_code, const DYNARY* t)
{
    if (f == nullptr) return 2;
    if (t->ndim == 0) {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %20.10E\n", name, from, to, t_code, t->value);
    } else if (t->ndim == 1) {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", name, from, to, t_code, t->n[0]);
    } else if (t->ndim == 2) {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i\n", name, from, to, t_code, t->n[0], t->n[1]);
    }
    return write_table_body(f, t, 0);
}

// @src 1402f4690  COARSN form: two doubles before the type code
int write_header_pair_dd(std::FILE* f, const char* name, std::int32_t from, std::int32_t to,
                         double c1, double c2, const DYNARY* t)
{
    if (f == nullptr) return 2;
    if (t->ndim == 0) {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E %7i %20.10E\n", name, from, to, c1, c2, 0, t->value);
    } else if (t->ndim == 1) {
        const std::int32_t code = t->var[0] == 3 ? 1 : 2;
        def::rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E %7i %7i\n", name, from, to, c1, c2, code, t->n[0]);
    } else if (t->ndim == 2) {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E %7i %7i %7i\n", name, from, to, c1, c2, 3, t->n[0],
                t->n[1]);
    }
    return write_table_body(f, t, 0);
}

// @src 1403cbf00  one phase pair: banner, TTTN, PHASTF, the handlers, VARSEL
int write_phase_pair(DB_STEP_DATA* m, std::FILE* f, PhasePair* pair, std::int32_t index)
{
    if (pair == nullptr) return 0;
    if (pair->phastf_on == 0) return 0;
    const std::int32_t from = pair->from;
    const std::int32_t to = pair->to;
    std::vector<int> v; // 140159860, 1402b7530: {index}
    v.push_back(index);
    char text[1120];
    std::snprintf(text, sizeof text, "Inter-Material Data for     %i and %i", pair->from, pair->to);
    def::rt::crt::fprintf(f, "*\n*  %s\n*\n", text);
    def::rt::crt::fprintf(f, "%-6s %7i %7i\n", kw(kTTTN), from, to);
    std::snprintf(text, sizeof text, "Transformation from %d -> %d", from, to);
    def::rt::crt::fprintf(f, "%s\n", text);
    def::rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", kw(kPHASTF), from, to, pair->phastf);
    int r = write_tttd(f, m, v);
    if (r == 0) r = write_phaslh(f, m, v);
    if (r == 0) r = write_phasvl(f, m, v);
    if (r == 0) r = write_trnsfp(f, m, v);
    if (r == 0) r = write_pheqvf(f, m, v);
    if (r == 0) r = write_coarsn(f, m, v);
    if (r == 0) r = write_ipsnrg(f, m, v);
    if (r != 0) return r;
    def::rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", kw(kVARSEL), from, to, pair->varsel);
    return 0;
}

// @src 1403ccd00  TTTD record of the pair's current kind (+0x14)
int write_tttd(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v)
{
    PhasePair* p = pair_at(m, v);
    const std::int32_t from = p->from;
    const std::int32_t to = p->to;
    const char* name = kw(kTTTD);
    switch (p->tttd_type) {
    case 0:
    case 1:
    case 2:
    case 3:
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", name, from, to, 1, p->tttd_kind);
        def::rt::crt::fprintf(f, " %7i %7i %7i\n", p->tttd_type == 1 ? 1 : p->tttd_type, p->tttd_nc, p->tttd_x);
        if (p->tttd_nc == 1)
            def::rt::crt::fprintf(f, " %20.10E %20.10E\n", p->tttd_f1, p->tttd_f1b);
        else
            def::rt::crt::fprintf(f, " %20.10E %20.10E\n", p->tttd_f1b, p->tttd_f2); // nc 2 and other values
        write_table_body(f, &p->tttd_t1, 1);
        break;
    case 4:
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", name, from, to, 2, p->tttd_kind);
        def::rt::crt::fprintf(f, " %7i %7i %7i %7i\n", p->tttd_t2.ncomp, p->tttd_t2.n[0], p->tttd_t2.n[1],
                p->tttd_t2.n[2]);
        write_table_body(f, &p->tttd_t2, 0);
        break;
    case 5: {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", name, from, to, 3, p->tttd_kind);
        double c[11]; // copied to the stack first (1403cd0b5)
        for (int i = 0; i < 11; i = i + 1) c[i] = p->tttd_c3[i];
        write_doubles(f, 11, c, 5, false); // 1402f2e80
        break;
    }
    case 6:
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", name, from, to, 4, p->tttd_kind);
        def::rt::crt::fprintf(f, " %20.10E %20.10E %20.10E %20.10E\n", p->tttd_c4[0], p->tttd_c4[1], p->tttd_c4[2],
                p->tttd_c4[3]);
        write_table_body(f, &p->tttd_t4, 1);
        break;
    case 7: {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", name, from, to, 5, p->tttd_kind);
        double c[6];
        for (int i = 0; i < 6; i = i + 1) c[i] = p->tttd_c5[i];
        write_doubles(f, 6, c, 5, false);
        break;
    }
    case 8:
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", name, from, to, 6, p->tttd_kind);
        def::rt::crt::fprintf(f, " %20.10E %20.10E %20.10E %20.10E\n", p->tttd_c6[0], p->tttd_c6[1], p->tttd_c6[2],
                p->tttd_c6[3]);
        break;
    case 9: {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", name, from, to, 7, p->tttd_kind);
        double c[7];
        for (int i = 0; i < 7; i = i + 1) c[i] = p->tttd_c7[i];
        write_doubles(f, 7, c, 5, false);
        break;
    }
    case 10:
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", name, from, to, 8, p->tttd_kind);
        def::rt::crt::fprintf(f, " %7i\n", p->tttd_t8.n[0]);
        write_table_body(f, &p->tttd_t8, 0);
        break;
    case 0xb:
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", name, from, to, 9, p->tttd_kind);
        def::rt::crt::fprintf(f, " %7i %7i\n", 2, p->tttd_nc);
        if (p->tttd_nc == 1)
            def::rt::crt::fprintf(f, " %20.10E %20.10E\n", p->tttd_f1, p->tttd_f1b);
        else if (p->tttd_nc == 2)
            def::rt::crt::fprintf(f, " %20.10E %20.10E\n", p->tttd_f9[0], p->tttd_f9[1]);
        else
            def::rt::crt::fprintf(f, " %20.10E %20.10E\n", p->tttd_f1b, p->tttd_f2);
        write_table_body(f, &p->tttd_t9, 1);
        break;
    case 0xc:
    case 0xd:
    case 0xe:
    case 0xf:
        if (p->tttd_type == 0xc)
            def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", name, from, to, 10, p->tttd_kind);
        else if (p->tttd_type == 0xd)
            def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", name, from, to, 0xb, p->tttd_kind);
        else if (p->tttd_type == 0xe)
            def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", name, from, to, 0xc, p->tttd_kind);
        if (p->tttd_type == 0xf)
            def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", name, from, to, 0xd, p->tttd_kind);
        def::rt::crt::fprintf(f, " %7i\n", p->tttd_vec.ncomp);
        write_table_body(f, &p->tttd_vec, 6);
        break;
    case 0x10: {
        const std::int32_t r = std::abs(p->tttd_ref); // 140484ab8
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", name, from, to, -r, p->tttd_kind);
        break;
    }
    default:
        return 3;
    }
    return 0;
}

// @src 1403cc4a0  PHASLH (header writer result ignored)
int write_phaslh(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v)
{
    PhasePair* p = pair_at(m, v);
    write_header_pair(f, kw(kPHASLH), p->from, p->to, &p->phaslh);
    return 0;
}

// @src 1403cc5e0  PHASVL
int write_phasvl(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v)
{
    PhasePair* p = pair_at(m, v);
    write_header_pair(f, kw(kPHASVL), p->from, p->to, &p->phasvl);
    return 0;
}

// @src 1403cc720  TRNSFP
int write_trnsfp(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v)
{
    PhasePair* p = pair_at(m, v);
    const std::int32_t from = p->from;
    const std::int32_t to = p->to;
    if (p->trnsfp_on == 1) {
        const DYNARY* t = &p->trnsfp;
        if (t->ndim == 0) {
            def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %20.10E\n", kw(kTRNSFP), from, to, 1, 0, t->value);
        } else if (t->ndim == 1) {
            const std::int32_t code = t->var[0] == 3 ? 1 : 2;
            def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i\n", kw(kTRNSFP), from, to, 1, code, t->n[0]);
        } else if (t->ndim == 2) {
            def::rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i %7i %7i\n", kw(kTRNSFP), from, to, 1, 3, t->n[0], t->n[1]);
        }
        write_table_body(f, t, 0);
    } else {
        def::rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", kw(kTRNSFP), from, to, 0);
    }
    return 0;
}

// @src 1403cca00  PHEQVF (only for PHASTF value 2)
int write_pheqvf(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v)
{
    PhasePair* p = pair_at(m, v);
    if (p->phastf == 2) write_header_pair_t(f, kw(kPHEQVF), p->from, p->to, p->pheqvf_t, &p->pheqvf);
    return 0;
}

// @src 1403ccb60  COARSN (only when present)
int write_coarsn(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v)
{
    PhasePair* p = pair_at(m, v);
    if (p->coarsn_on == 1)
        write_header_pair_dd(f, kw(kCOARSN), p->from, p->to, p->coarsn_c1, p->coarsn_c2, &p->coarsn);
    return 0;
}

// @src 1403ccc50  IPSNRG
int write_ipsnrg(std::FILE* f, DB_STEP_DATA* m, std::vector<int> v)
{
    PhasePair* p = pair_at(m, v);
    write_header_pair(f, kw(kIPSNRG), p->from, p->to, &p->ipsnrg);
    return 0;
}

// @src 1403cdb20  TTTD tables by label; 3 for another label or no pair
int access_tttd(DB_STEP_DATA* m, const char* label, std::vector<int> v, DYNARY** out)
{
    PhasePair* p = pair_at(m, v);
    if (p != nullptr) {
        if (std::strcmp(label, "Log(Time)") == 0) {
            *out = &p->tttd_t1;
            return 0;
        }
        if (std::strcmp(label, "Start and 50% Temperatur table") == 0) {
            *out = &p->tttd_t2;
            return 0;
        }
        if (std::strcmp(label, "Coefficient f(T)") == 0) {
            *out = &p->tttd_t4;
            return 0;
        }
        if (std::strcmp(label, "Phase Compositions") == 0) {
            *out = &p->tttd_t9;
            return 0;
        }
    }
    return 3;
}

// @src 1403cc550  PHASLH table
int access_phaslh(DB_STEP_DATA* m, const char*, std::vector<int> v, DYNARY** out)
{
    PhasePair* p = pair_at(m, v);
    if (p != nullptr) *out = &p->phaslh;
    return 0;
}

// @src 1403cc690  PHASVL table
int access_phasvl(DB_STEP_DATA* m, const char*, std::vector<int> v, DYNARY** out)
{
    PhasePair* p = pair_at(m, v);
    if (p != nullptr) *out = &p->phasvl;
    return 0;
}

// @src 1403cc970  TRNSFP table
int access_trnsfp(DB_STEP_DATA* m, const char*, std::vector<int> v, DYNARY** out)
{
    PhasePair* p = pair_at(m, v);
    if (p != nullptr) *out = &p->trnsfp;
    return 0;
}

// @src 1403ccad0  PHEQVF table
int access_pheqvf(DB_STEP_DATA* m, const char*, std::vector<int> v, DYNARY** out)
{
    PhasePair* p = pair_at(m, v);
    if (p != nullptr) *out = &p->pheqvf;
    return 0;
}

} // namespace def::key::g57
