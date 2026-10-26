// Group 4 (material) KEY writer, packet kw-g4b: the per-material writer FUN_1403cdca0 and the
// write / accessor handlers of this packet's keywords. Formats are the binary's strings; the
// stream is a text-mode FILE (CR LF). Specs: docs/re/keywords/<KW>.md, _common_g457.md 4 and 6.
#include <defkey/gendb_deps.hpp>
#include <defkey/kw_g4.hpp>

#include <defrt/stdlib/legacy_printf.hpp>

#include <cstdint>
#include <cstdio>
#include <initializer_list>
#include <cstdlib>
#include <cstring>

#include <defmodel/dynary_init.hpp>
#include <defmodel/keyword_registry.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/mesh2d/material_ops.hpp>

#include "key_io_deps.hpp"
#include <defkey/kw_g4a.hpp>

namespace def::key::g4 {

using namespace def::model;
using namespace def::key::g4dep;

namespace {

// The handlers look the material up again by number and use it without a null test, as the
// original does (the KEY writer only passes numbers of existing materials).
Material* mat_of(DB_STEP_DATA& m, const std::vector<int>& v)
{
    return material_list_find(&m.materials, v[0]); // 140159a70(v, 0), 14029c2c0
}

// The accessor pattern: *out = table of the material, if it exists; always 0.
template <class Pick>
int table_of(DB_STEP_DATA* m, const std::vector<int>& v, void** out, Pick pick)
{
    Material* mat = material_list_find(&m->materials, v[0]);
    if (mat != nullptr) {
        *out = pick(mat);
    }
    return 0;
}

} // namespace

// Public (defkey/gendb_deps.hpp): the GENDB counts frame calls it too.
// @src 14028aff0  1-based record i of a GRNDAT record list (i <= 1 gives the head)
Cls_14028a4c0* grndat_record_nth(const Cls_14028b640::RecordList* l, std::int32_t i)
{
    std::int32_t k = 0;
    Cls_14028a4c0* r = l->head;
    while (r != nullptr && (k = k + 1, k < i)) {
        r = r->next;
    }
    return r;
}

namespace {

// @src 1402f7310  GRNDAT record sub-function: "t n [n1]" line and body, or " 0 value"
int write_grndat_sub(std::FILE* f, const DYNARY* d)
{
    int rc = 0;
    std::int32_t t = -1;
    if (d->ndim == 0) {
        t = 0;
        rt::crt::fprintf(f, " %7i %20.10E\n", 0, d->value);
    }
    else if (d->ndim == 1) {
        if (d->var[0] == 3) {
            t = 1;
        }
        else if (d->var[0] == 0x17) {
            t = 2;
        }
        if (0 < t) {
            rt::crt::fprintf(f, " %7i %7i\n", t, d->n[0]);
            rc = write_table_body(f, d, 0);
        }
    }
    else if (d->ndim == 2) {
        if (d->var[0] == 3 && d->var[1] == 0x17) {
            t = 3;
        }
        if (0 < t) {
            rt::crt::fprintf(f, " %7i %7i %7i\n", t, d->n[0], d->n[1]);
            rc = write_table_body(f, d, 0);
        }
    }
    if (t < 0) {
        rt::crt::fprintf(f, " %7i %20.10E\n", 0, 0.0); // 1402f7454: R9 = XMM3 = 0.0
    }
    return rc;
}

// @src 1402f71f0  one GRNDAT type-2 record
int write_grndat_record(std::FILE* f, const Cls_14028a4c0* r)
{
    if (f == nullptr) {
        return 2;
    }
    rt::crt::fprintf(f, " %7i\n", r->n);           // 14029cd20
    write_values(f, r->n, r->values, 5, 0);     // 1402f7580
    rt::crt::fprintf(f, " %7i %20.10E\n", r->i, r->d);
    if (r->is_main == 0) {                      // 14029cd30
        write_grndat_sub(f, &r->sub1);
        write_grndat_sub(f, &r->sub2);
    }
    write_grndat_sub(f, &r->sub3);
    return 0;
}

// GRNDAT type 1: the code of a D table's "k n" line (k 0 = off for the tables with a flag).
std::int32_t grndat_k(const DYNARY& d, const std::int32_t* flag, bool has_k3)
{
    if (flag != nullptr && *flag == 0) {
        return 0;
    }
    if (d.var[0] == 3) {
        return 1;
    }
    if (!has_k3 || d.var[0] == 8) {
        return 2;
    }
    return 3;
}

} // namespace

// @src 1403cdca0  one material block of the KEY file
int write_material(DB_STEP_DATA& m, std::FILE* f, Material* mat, const char* name_override)
{
    if (mat == nullptr) {
        return 0;
    }
    if (mat->skip_key == 1) { // 1403d4970
        return 0;
    }
    char banner[1024];
    const std::int32_t number = mat->number;
    rt::crt::sprintf(banner, "Property Data of Material     %i", number);
    rt::crt::fprintf(f, "*\n*  %s\n*\n", banner);
    if (name_override == nullptr || std::strlen(name_override) == 0) {
        rt::crt::fprintf(f, "%-6s %7i\n%s\n", keyword_name(0x5c), number, mat->name);
    }
    else {
        rt::crt::fprintf(f, "%-6s %7i\n%s\n", keyword_name(0x5c), number, name_override);
    }
    const std::vector<int> v{number}; // 1402b7530; each handler gets its own copy
    int rc = g4a::write_frae2h(f, &m, v);
    if (rc != 0) return rc;
    rc = g4a::write_fperv(f, &m, v);
    if (rc != 0) return rc;
    rc = g4a::write_frcmod(f, &m, v);
    if (rc != 0) return rc;
    rc = g4a::write_fstres(f, &m, v);
    if (rc != 0) return rc;
    rc = write_young(f, m, v);
    if (rc != 0) return rc;
    rc = write_poison(f, m, v);
    if (rc != 0) return rc;
    rc = write_expand(f, m, v);
    if (rc != 0) return rc;
    rc = write_thrcnd(f, m, v);
    if (rc != 0) return rc;
    rc = write_heatcp(f, m, v);
    if (rc != 0) return rc;
    rc = write_masden(f, m, v);
    if (rc != 0) return rc;
    rc = write_coarse(f, m, v);
    if (rc != 0) return rc;
    rc = write_difbnd(f, m, v);
    if (rc != 0) return rc;
    rc = write_emsvty(f, m, v);
    if (rc != 0) return rc;
    rc = write_hdnpha(f, m, v);
    if (rc != 0) return rc;
    rc = write_hdntim(f, m, v);
    if (rc != 0) return rc;
    rc = write_jominy(f, m, v);
    if (rc != 0) return rc;
    // MSTMTR inline
    rt::crt::fprintf(f, "%-6s %7i %7i\n", keyword_name(0x74), number, mat->mstmtr_count);
    for (std::int32_t i = 0; i < mat->mstmtr_count; i = i + 1) {
        rt::crt::fprintf(f, " %7i\n", mat->mstmtr_list[i]);
    }
    rc = g4a::write_creep(f, &m, v);
    if (rc != 0) return rc;
    rc = write_difcoe(f, m, v);
    if (rc != 0) return rc;
    rc = write_elrst(f, m, v);
    if (rc != 0) return rc;
    rc = write_utsdat(f, m, v);
    if (rc != 0) return rc;
    // HDNRUL inline
    rt::crt::fprintf(f, "%-6s %7i %7i\n", keyword_name(0x78), number, mat->hdnrul);
    rc = write_pmeab(f, m, v);
    if (rc != 0) return rc;
    rc = write_pmitt(f, m, v);
    if (rc != 0) return rc;
    // MATDEN inline, 3D models only
    if (model_is_3d(&m)) {
        rt::crt::fprintf(f, "%-6s %7i %20.10E\n", keyword_name(0x7c), number, mat->matden);
    }
    rc = g4a::write_aniso(f, &m, v);
    if (rc != 0) return rc;
    rc = write_grndat(f, m, v);
    if (rc != 0) return rc;
    rc = g4a::write_burgrs(f, &m, v);
    if (rc != 0) return rc;
    rc = g4a::write_alpha(f, &m, v);
    if (rc != 0) return rc;
    rc = g4a::write_ndisfm(f, &m, v);
    if (rc != 0) return rc;
    rc = g4a::write_recvry(f, &m, v);
    if (rc != 0) return rc;
    // SIZEMD and TXTURE inline
    rt::crt::fprintf(f, "%-6s %7i %7i\n", keyword_name(0x7f), number, mat->sizemd);
    rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", keyword_name(0x80), number, mat->txture.a,
                 mat->txture.b, mat->txture.c);
    rc = write_sinter(f, m, v);
    if (rc != 0) return rc;
    rc = write_gbengy(f, m, v);
    if (rc != 0) return rc;
    rc = write_gbmobi(f, m, v);
    if (rc != 0) return rc;
    return write_nucsiz(f, m, v);
}

// ---- write handlers -----------------------------------------------------------------------------

// @src 1403cf050  YOUNG
int write_young(std::FILE* f, DB_STEP_DATA& m, std::vector<int> v)
{
    Material* mat = mat_of(m, v);
    write_table(f, keyword_name(0x5f), v[0], &mat->young, 0, nullptr);
    return 0;
}

// @src 1403cf180  POISON
int write_poison(std::FILE* f, DB_STEP_DATA& m, std::vector<int> v)
{
    Material* mat = mat_of(m, v);
    write_table(f, keyword_name(0x60), v[0], &mat->poison, 0, nullptr);
    return 0;
}

// @src 1403cf2b0  EXPAND (extra: reference value +0x7d8)
int write_expand(std::FILE* f, DB_STEP_DATA& m, std::vector<int> v)
{
    Material* mat = mat_of(m, v);
    write_table(f, keyword_name(0x61), v[0], &mat->expand, 1, &mat->expand_ref);
    return 0;
}

// @src 1403cf490  THRCND
int write_thrcnd(std::FILE* f, DB_STEP_DATA& m, std::vector<int> v)
{
    Material* mat = mat_of(m, v);
    write_table(f, keyword_name(0x62), v[0], &mat->thrcnd, 0, nullptr);
    return 0;
}

// @src 1403cf5c0  HEATCP (int extra +0x8d0, 1402f40f0)
int write_heatcp(std::FILE* f, DB_STEP_DATA& m, std::vector<int> v)
{
    Material* mat = mat_of(m, v);
    write_table_iextra(f, keyword_name(99), v[0], &mat->heatcp, 1, &mat->heatcp_extra);
    return 0;
}

// @src 1403cf700  EMSVTY
int write_emsvty(std::FILE* f, DB_STEP_DATA& m, std::vector<int> v)
{
    Material* mat = mat_of(m, v);
    write_table(f, keyword_name(100), v[0], &mat->emsvty, 0, nullptr);
    return 0;
}

// @src 1403cfca0  DIFCOE
int write_difcoe(std::FILE* f, DB_STEP_DATA& m, std::vector<int> v)
{
    const std::int32_t number = v[0];
    Material* mat = mat_of(m, v);
    const Cls_140289930& dc = mat->difcoe;
    const std::int32_t mode = dc.mode;
    if (mode == 0) {
        rt::crt::fprintf(f, "%-6s %7i %7i %20.10E\n", keyword_name(0x6b), number, 0, dc.d0.value);
    }
    else if (mode == 1) {
        rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", keyword_name(0x6b), number, 1, dc.d0.n[0],
                     dc.d0.n[1]);
        write_table_body(f, &dc.d0, 0);
    }
    else if (mode == 2) {
        rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", keyword_name(0x6b), number, 2, dc.d2.n[0]);
        write_table_body(f, &dc.d2, 0);
    }
    else if (mode == 3) {
        rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", keyword_name(0x6b), number, 3, dc.d3.n[0]);
        write_table_body(f, &dc.d3, 0);
    }
    else {
        if (mode != 4) {
            return 3;
        }
        const std::int32_t r = std::abs(dc.user_routine); // 140484ab8
        rt::crt::fprintf(f, "%-6s %7i %7i\n", keyword_name(0x6b), number, -r);
    }
    return 0;
}

// @src 1403cffe0  HDNPHA (var 12 -> 1, 3 -> 2, 9 -> 3; other shapes: 3)
int write_hdnpha(std::FILE* f, DB_STEP_DATA& m, std::vector<int> v)
{
    const std::int32_t number = v[0];
    Material* mat = mat_of(m, v);
    const DYNARY& h = mat->hdnpha;
    if (h.ndim == 0) {
        rt::crt::fprintf(f, "%-6s %7i %7i %20.10E\n", keyword_name(0x72), number, 0, h.value);
    }
    else if (h.ndim == 1) {
        std::int32_t t = 0;
        if (h.var[0] == 0xc) {
            t = 1;
        }
        else if (h.var[0] == 3) {
            t = 2;
        }
        else {
            if (h.var[0] != 9) {
                return 3;
            }
            t = 3;
        }
        rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", keyword_name(0x72), number, t, h.n[0]);
    }
    else {
        if (h.ndim != 2) {
            return 3;
        }
        rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", keyword_name(0x72), number, 4, h.n[0], h.n[1]);
    }
    write_table_body(f, &h, 0);
    return 0;
}

// @src 1403d0280  ELRST
int write_elrst(std::FILE* f, DB_STEP_DATA& m, std::vector<int> v)
{
    Material* mat = mat_of(m, v);
    write_table(f, keyword_name(0x73), v[0], &mat->elrst, 0, nullptr);
    return 0;
}

// @src 1403d03b0  HDNTIM (only a table with every n >= 1)
int write_hdntim(std::FILE* f, DB_STEP_DATA& m, std::vector<int> v)
{
    const std::int32_t number = v[0];
    Material* mat = mat_of(m, v);
    if (dynary_counts_positive(&mat->hdntim)) { // 14012f610
        rt::crt::fprintf(f, "%-6s %7i %7i\n", keyword_name(0x75), number, mat->hdntim.n[0]);
        write_table_body(f, &mat->hdntim, 0);
    }
    return 0;
}

// @src 1403d0510  JOMINY (only a table with every n >= 1)
int write_jominy(std::FILE* f, DB_STEP_DATA& m, std::vector<int> v)
{
    const std::int32_t number = v[0];
    Material* mat = mat_of(m, v);
    if (dynary_counts_positive(&mat->jominy)) {
        rt::crt::fprintf(f, "%-6s %7i %7i %7i %7i\n", keyword_name(0x76), number, 1,
                     mat->jominy.n[0], mat->jominy.n[1]);
        write_table_body(f, &mat->jominy, 0);
    }
    return 0;
}

// @src 1403d0680  UTSDAT (table in the FRCMOD block, +0x1b10)
int write_utsdat(std::FILE* f, DB_STEP_DATA& m, std::vector<int> v)
{
    Material* mat = mat_of(m, v);
    write_table(f, keyword_name(0x77), v[0], &mat->frcmod.uts, 0, nullptr);
    return 0;
}

// @src 1403d07b0  PMEAB
int write_pmeab(std::FILE* f, DB_STEP_DATA& m, std::vector<int> v)
{
    Material* mat = mat_of(m, v);
    write_table(f, keyword_name(0x79), v[0], &mat->pmeab, 0, nullptr);
    return 0;
}

// @src 1403d08e0  PMITT
int write_pmitt(std::FILE* f, DB_STEP_DATA& m, std::vector<int> v)
{
    Material* mat = mat_of(m, v);
    write_table(f, keyword_name(0x7a), v[0], &mat->pmitt, 0, nullptr);
    return 0;
}

// @src 1403d1eb0  MASDEN
int write_masden(std::FILE* f, DB_STEP_DATA& m, std::vector<int> v)
{
    Material* mat = mat_of(m, v);
    write_table(f, keyword_name(0x7d), v[0], &mat->masden, 0, nullptr);
    return 0;
}

// @src 1403d1fe0  COARSE (flag < 1: the flag only; else the COARSE case of 1402f3aa0)
int write_coarse(std::FILE* f, DB_STEP_DATA& m, std::vector<int> v)
{
    const std::int32_t number = v[0];
    Material* mat = mat_of(m, v);
    const double flag = static_cast<double>(mat->coarse_flag);
    if (mat->coarse_flag < 1) {
        rt::crt::fprintf(f, "%-6s %7i %7i\n", keyword_name(0x7e), number, mat->coarse_flag);
    }
    else {
        write_table(f, keyword_name(0x7e), number, &mat->coarse, 1, &flag);
    }
    return 0;
}

// @src 1403d2170  DIFBND
int write_difbnd(std::FILE* f, DB_STEP_DATA& m, std::vector<int> v)
{
    Material* mat = mat_of(m, v);
    write_table(f, keyword_name(0x81), v[0], &mat->difbnd, 0, nullptr);
    return 0;
}

// @src 1403d3a90  GRNDAT (types 1, 2, 4 as t 5, 5; others write nothing)
int write_grndat(std::FILE* f, DB_STEP_DATA& m, std::vector<int> v)
{
    const std::int32_t number = v[0];
    Material* mat = mat_of(m, v);
    const Cls_14028b850& g = mat->grndat;
    if (g.type == 1) {
        const Cls_14028a140& g1 = g.type1;
        rt::crt::fprintf(f, "%-6s %7i %7i\n", keyword_name(0x6c), number, 1);
        rt::crt::fprintf(f, " %7i %7i\n", grndat_k(g1.d[0], nullptr, false), g1.d[0].n[0]);
        rt::crt::fprintf(f, " %7i %7i\n", grndat_k(g1.d[1], nullptr, false), g1.d[1].n[0]);
        rt::crt::fprintf(f, " %7i %7i\n", grndat_k(g1.d[2], &g1.flag_d3, false), g1.d[2].n[0]);
        rt::crt::fprintf(f, " %7i %7i\n", grndat_k(g1.d[3], nullptr, true), g1.d[3].n[0]);
        rt::crt::fprintf(f, " %7i %7i\n", grndat_k(g1.d[4], &g1.flag_d5, false), g1.d[4].n[0]);
        rt::crt::fprintf(f, " %7i %7i\n", grndat_k(g1.d[5], nullptr, true), g1.d[5].n[0]);
        rt::crt::fprintf(f, " %7i %7i\n", grndat_k(g1.d[6], &g1.flag_d7, false), g1.d[6].n[0]);
        rt::crt::fprintf(f, " %7i %7i\n", grndat_k(g1.d[7], nullptr, true), g1.d[7].n[0]);
        rt::crt::fprintf(f, " %7i %7i\n", grndat_k(g1.d[8], &g1.flag_d9, true), g1.d[8].n[0]);
        for (const DYNARY& d : g1.d) {
            write_table_body(f, &d, 0);
        }
        rt::crt::fprintf(f, " %20.10E %20.10E\n", g1.g1, g1.g2); // +0x14c8, +0x14c0
    }
    else if (g.type == 2) {
        const Cls_14028b640& g2 = g.type2;
        rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", keyword_name(0x6c), number, 2, g2.c);
        write_grndat_record(f, &g2.main);
        for (const Cls_14028b640::RecordList* l : {&g2.l1, &g2.l2, &g2.l3, &g2.l4, &g2.l5}) {
            const std::int32_t n = grndat_record_list_count(l); // 14029cd50
            rt::crt::fprintf(f, " %7i\n", n);
            for (std::int32_t i = 0; i < n; i = i + 1) {
                write_grndat_record(f, grndat_record_nth(l, i + 1));
            }
        }
    }
    else if (g.type == 4) {
        rt::crt::fprintf(f, "%-6s %7i %7i\n", keyword_name(0x6c), number, 4);
        rt::crt::fprintf(f, " %7i %7i %7i\n", 5, g.table4.n[0], g.table4.n[1]);
        write_table_body(f, &g.table4, 0);
    }
    else if (g.type == 5) {
        rt::crt::fprintf(f, "%-6s %7i %7i\n", keyword_name(0x6c), number, 5);
        rt::crt::fprintf(f, " %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E\n", g.p[0], g.p[1],
                     g.p[2], g.p[3], g.p[4], g.p[5]);
    }
    return 0;
}

// @src 1403d4400  GBENGY
int write_gbengy(std::FILE* f, DB_STEP_DATA& m, std::vector<int> v)
{
    Material* mat = mat_of(m, v);
    write_table(f, keyword_name(0x83), v[0], &mat->gbengy, 0, nullptr);
    return 0;
}

// @src 1403d44a0  GBMOBI
int write_gbmobi(std::FILE* f, DB_STEP_DATA& m, std::vector<int> v)
{
    Material* mat = mat_of(m, v);
    rt::crt::fprintf(f, "%-6s %7i %20.10E %20.10E\n", keyword_name(0x84), v[0], mat->gbmobi_p,
                 mat->gbmobi_q);
    return 0;
}

// @src 1403d4550  NUCSIZ
int write_nucsiz(std::FILE* f, DB_STEP_DATA& m, std::vector<int> v)
{
    Material* mat = mat_of(m, v);
    write_table(f, keyword_name(0x85), v[0], &mat->nucsiz, 0, nullptr);
    return 0;
}

// @src 1403d45f0  SINTER (models 1..6; the count field is fixed per model; others: nothing)
int write_sinter(std::FILE* f, DB_STEP_DATA& m, std::vector<int> v)
{
    const std::int32_t number = v[0];
    Material* mat = mat_of(m, v);
    const std::int32_t model = mat->sinter_model;
    const double* p = mat->sinter_p;
    switch (model) {
    case 1:
        rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", keyword_name(0x82), number, model, 0);
        break;
    case 2:
    case 5:
        rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", keyword_name(0x82), number, model, 2);
        rt::crt::fprintf(f, " %20.10E %20.10E\n", p[0], p[1]);
        break;
    case 3:
        rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", keyword_name(0x82), number, model, 4);
        rt::crt::fprintf(f, " %20.10E %20.10E %20.10E %20.10E\n", p[0], p[1], p[2], p[3]);
        break;
    case 4:
    case 6:
        rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", keyword_name(0x82), number, model, 3);
        rt::crt::fprintf(f, " %20.10E %20.10E %20.10E\n", p[0], p[1], p[2]);
        break;
    default:
        break;
    }
    return 0;
}

// ---- accessors ----------------------------------------------------------------------------------

// @src 1403cf0f0  YOUNG
int table_young(DB_STEP_DATA* m, const char*, std::vector<int> v, void** out)
{
    return table_of(m, v, out, [](Material* p) { return static_cast<void*>(&p->young); });
}

// @src 1403cf220  POISON
int table_poison(DB_STEP_DATA* m, const char*, std::vector<int> v, void** out)
{
    return table_of(m, v, out, [](Material* p) { return static_cast<void*>(&p->poison); });
}

// @src 1403cf360  EXPAND
int table_expand(DB_STEP_DATA* m, const char*, std::vector<int> v, void** out)
{
    return table_of(m, v, out, [](Material* p) { return static_cast<void*>(&p->expand); });
}

// @src 1403cf530  THRCND
int table_thrcnd(DB_STEP_DATA* m, const char*, std::vector<int> v, void** out)
{
    return table_of(m, v, out, [](Material* p) { return static_cast<void*>(&p->thrcnd); });
}

// @src 1403cf670  HEATCP
int table_heatcp(DB_STEP_DATA* m, const char*, std::vector<int> v, void** out)
{
    return table_of(m, v, out, [](Material* p) { return static_cast<void*>(&p->heatcp); });
}

// @src 1403cf7a0  EMSVTY
int table_emsvty(DB_STEP_DATA* m, const char*, std::vector<int> v, void** out)
{
    return table_of(m, v, out, [](Material* p) { return static_cast<void*>(&p->emsvty); });
}

// @src 1403cff00  DIFCOE (mode 1, 2, 3 only)
int table_difcoe(DB_STEP_DATA* m, const char*, std::vector<int> v, void** out)
{
    Material* mat = material_list_find(&m->materials, v[0]);
    if (mat != nullptr) {
        if (mat->difcoe.mode == 1) {
            *out = &mat->difcoe.d0;
        }
        else if (mat->difcoe.mode == 2) {
            *out = &mat->difcoe.d2;
        }
        else if (mat->difcoe.mode == 3) {
            *out = &mat->difcoe.d3;
        }
    }
    return 0;
}

// @src 1403d01f0  HDNPHA
int table_hdnpha(DB_STEP_DATA* m, const char*, std::vector<int> v, void** out)
{
    return table_of(m, v, out, [](Material* p) { return static_cast<void*>(&p->hdnpha); });
}

// @src 1403d0320  ELRST
int table_elrst(DB_STEP_DATA* m, const char*, std::vector<int> v, void** out)
{
    return table_of(m, v, out, [](Material* p) { return static_cast<void*>(&p->elrst); });
}

// @src 1403d0480  HDNTIM
int table_hdntim(DB_STEP_DATA* m, const char*, std::vector<int> v, void** out)
{
    return table_of(m, v, out, [](Material* p) { return static_cast<void*>(&p->hdntim); });
}

// @src 1403d05f0  JOMINY
int table_jominy(DB_STEP_DATA* m, const char*, std::vector<int> v, void** out)
{
    return table_of(m, v, out, [](Material* p) { return static_cast<void*>(&p->jominy); });
}

// @src 1403d0720  UTSDAT
int table_utsdat(DB_STEP_DATA* m, const char*, std::vector<int> v, void** out)
{
    return table_of(m, v, out, [](Material* p) { return static_cast<void*>(&p->frcmod.uts); });
}

// @src 1403d0850  PMEAB
int table_pmeab(DB_STEP_DATA* m, const char*, std::vector<int> v, void** out)
{
    return table_of(m, v, out, [](Material* p) { return static_cast<void*>(&p->pmeab); });
}

// @src 1403d0980  PMITT
int table_pmitt(DB_STEP_DATA* m, const char*, std::vector<int> v, void** out)
{
    return table_of(m, v, out, [](Material* p) { return static_cast<void*>(&p->pmitt); });
}

// @src 1403d1f50  MASDEN
int table_masden(DB_STEP_DATA* m, const char*, std::vector<int> v, void** out)
{
    return table_of(m, v, out, [](Material* p) { return static_cast<void*>(&p->masden); });
}

// @src 1403d20e0  COARSE
int table_coarse(DB_STEP_DATA* m, const char*, std::vector<int> v, void** out)
{
    return table_of(m, v, out, [](Material* p) { return static_cast<void*>(&p->coarse); });
}

// @src 1403d2210  DIFBND
int table_difbnd(DB_STEP_DATA* m, const char*, std::vector<int> v, void** out)
{
    return table_of(m, v, out, [](Material* p) { return static_cast<void*>(&p->difbnd); });
}

} // namespace def::key::g4
