// Group 4 (material) KEY parser FUN_140384a50, packet kw-g4b.
// One function in the original; the ten kw-g4a cases call that packet's functions. Field names
// are the Material layout's (defmodel/layout/material.hpp); the comments give the absolute
// material offsets the decompile uses. Specs: docs/re/keywords/<KW>.md.
#include <defkey/kw_g4.hpp>

#include <cstdint>
#include <initializer_list>
#include <cstdlib>
#include <cstring>

#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/material/material_ctor.hpp>
#include <defmodel/material/state_remap.hpp>
#include <defmodel/mesh2d/material_ops.hpp>

#include "key_io_deps.hpp"
#include <defkey/kw_g4a.hpp>

namespace def::key::g4 {

using namespace def::model;
using namespace def::key::g4dep;

namespace {

// part of @src 140384a50  case 0x10 (GRNDAT type 1): one "k n" line of a D table. `flag` is the
// on/off int of tables 3, 5, 7, 9 (null for the others); `k3_var` is the var for k = 3 (0x21 for
// tables 4, 6, 8, 9; 0 = k 3 not accepted); k 0 clears the flag (tables with a flag only).
int grndat_d_line(std::FILE* f, DYNARY* d, int type, std::int32_t* flag, std::int32_t k3_var)
{
    std::int32_t k = 0;
    std::int32_t n = 0;
    if (line_ii(f, &k, &n) != 0) {
        return 2;
    }
    dynary_reinit_keep(d, type);
    if (flag != nullptr) {
        if (k == 0) {
            *flag = 0;
        }
        else if (k == 1) {
            *flag = 1;
            d->var[0] = 3;
        }
        else if (k == 2) {
            *flag = 1;
            d->var[0] = 8;
        }
        else if (k == 3 && k3_var != 0) {
            *flag = 1;
            d->var[0] = k3_var;
        }
    }
    else {
        if (k == 1) {
            d->var[0] = 3;
        }
        else if (k == 2) {
            d->var[0] = 8;
        }
        else if (k == 3 && k3_var != 0) {
            d->var[0] = k3_var;
        }
    }
    d->n[0] = n;
    return 0;
}

// @src 14041c070  GRNDAT record sub-function line: "t a b c" (t pre-set to -1), no re-init
int read_grndat_sub(std::FILE* f, DYNARY* d)
{
    char line[1024];
    std::int32_t t = -1;
    double a = 0.0;
    double b = 0.0;
    double c = 0.0;
    int rc = 0;
    if (std::fgets(line, 0x400, f) == nullptr) {
        rc = 1;
    }
    else if (std::strlen(line) == 1 && line[0] == '\n' && std::fgets(line, 0x400, f) == nullptr) {
        rc = 1;
    }
    else {
        replace_char(line, ',', ' '); // 140323970
        key_sscanf(line, "%i %Flf %Flf %Flf", &t, &a, &b, &c); // count unused
        if (t == 0) {
            d->ndim = 0;
            d->value = a;
        }
        else if (t == 1) {
            d->ndim = 1;
            d->var[0] = 3;
            d->n[0] = static_cast<std::int32_t>(a);
        }
        else if (t == 2) {
            d->ndim = 1;
            d->var[0] = 0x17;
            d->n[0] = static_cast<std::int32_t>(a);
        }
        else if (t == 3) {
            d->ndim = 2;
            d->var[0] = 3;
            d->var[1] = 0x17;
            d->n[0] = static_cast<std::int32_t>(a);
            d->n[1] = static_cast<std::int32_t>(b);
        }
        if (0 < t) {
            rc = read_table_body(f, d);
        }
    }
    return rc;
}

// @src 14041bed0  one GRNDAT type-2 record
int read_grndat_record(std::FILE* f, Cls_14028a4c0* r)
{
    std::int32_t n = 0;
    if (f == nullptr) {
        return 1;
    }
    if (key_fscanf(f, "%i", &n) == -1) {
        return 1;
    }
    grndat_record_set_n(r, &n); // 14029ccd0 (clamps n to 12)
    double* v = r->values;      // 1402f7580
    for (std::int32_t i = 0; i < n; i = i + 1) {
        if (key_fscanf(f, "%Flf", &v[i]) == -1) {
            return 1;
        }
    }
    if (key_fscanf(f, "%i %Flf", &r->i, &r->d) == -1) {
        return 1;
    }
    if (r->is_main == 0) { // 14029cd30
        if (read_grndat_sub(f, &r->sub1) != 0) {
            return 1;
        }
        if (read_grndat_sub(f, &r->sub2) != 0) {
            return 1;
        }
    }
    return read_grndat_sub(f, &r->sub3) == 0 ? 0 : 1;
}

// part of @src 140384a50  case 0x10 GRNDAT
int read_grndat(DB_STEP_DATA& md, std::FILE* f, Material* mat)
{
    Cls_14028b850& g = mat->grndat;
    std::int32_t t = 0;
    std::int32_t c = 0;
    int b8 = line_ii(f, &t, &c);
    if (b8 != 0) {
        return 2;
    }
    g.type = t; // +0x17e8
    if (t == 1) {
        Cls_14028a140& g1 = g.type1;
        // D1..D9 header lines: +0x1088 0x55, +0x1100 0x54, +0x1178 0x56 (flag +0x1078),
        // +0x11f0 0x57, +0x1268 0x58 (flag +0x107c), +0x12e0 0x59, +0x1358 0x5a (flag +0x1080),
        // +0x13d0 0x5b, +0x1448 0x5c (flag +0x1084).
        if (grndat_d_line(f, &g1.d[0], 0x55, nullptr, 0) != 0) return 2;
        if (grndat_d_line(f, &g1.d[1], 0x54, nullptr, 0) != 0) return 2;
        if (grndat_d_line(f, &g1.d[2], 0x56, &g1.flag_d3, 0) != 0) return 2;
        if (grndat_d_line(f, &g1.d[3], 0x57, nullptr, 0x21) != 0) return 2;
        if (grndat_d_line(f, &g1.d[4], 0x58, &g1.flag_d5, 0) != 0) return 2;
        if (grndat_d_line(f, &g1.d[5], 0x59, nullptr, 0x21) != 0) return 2;
        if (grndat_d_line(f, &g1.d[6], 0x5a, &g1.flag_d7, 0) != 0) return 2;
        if (grndat_d_line(f, &g1.d[7], 0x5b, nullptr, 0x21) != 0) return 2;
        if (grndat_d_line(f, &g1.d[8], 0x5c, &g1.flag_d9, 0x21) != 0) return 2;
        for (DYNARY& d : g1.d) {
            if (read_table_body(f, &d) != 0) {
                return 2;
            }
        }
        b8 = line_dd(f, &g1.g1, &g1.g2); // +0x14c8, +0x14c0
    }
    else if (t == 2) {
        Cls_14028b640& g2 = g.type2;
        g2.c = c; // +0x14d0
        b8 = read_grndat_record(f, &g2.main);
        if (b8 != 0) {
            return 2;
        }
        // lists in file order: +0x16f8, +0x16e0, +0x16c8, +0x1710, +0x1728
        for (Cls_14028b640::RecordList* l : {&g2.l1, &g2.l2, &g2.l3, &g2.l4, &g2.l5}) {
            std::int32_t count = 0;
            grndat_record_list_clear(l); // 14028b240
            line_i(f, &count, 1);        // result unused; skips one empty line
            for (std::int32_t i = 0; i < count; i = i + 1) {
                Cls_14028a4c0* r = grndat_record_list_add(l); // 14028b050
                if (read_grndat_record(f, r) != 0) {
                    return 2;
                }
                b8 = 0;
            }
        }
    }
    else if (t == 4) {
        b8 = read_table(f, &g.table4, 0, nullptr); // +0x1740
    }
    else if (t == 5) {
        b8 = line_d6(f, &g.p[0], &g.p[1], &g.p[2], &g.p[3], &g.p[4], &g.p[5]); // +0x17b8
    }
    else {
        b8 = 0;
        if (t < 0) {
            g.ref = -t;   // +0x17ec
            g.type = -1;
        }
    }
    step_remap_material_state(md, false); // FUN_1401d0fc0(model, 0)
    return b8;
}

} // namespace

// @src 140384a50  group 4 parser
int read_keyword(DB_STEP_DATA& md, std::FILE* f, int id)
{
    // 140384a61: fscanf("%i") into an uninitialised local; on a 0 result (not a number) the
    // original keeps whatever the stack held, which cannot be reproduced: the port uses 0.
    std::int32_t number = 0;
    int b8 = 0;
    if (key_fscanf(f, "%i", &number) == -1) {
        return 2;
    }
    MaterialList* list = &md.materials; // +0x1328
    Material* mat = material_list_find(list, number);
    if (mat == nullptr) {
        material_list_rebuild_index(list, false);
        mat = material_list_add(list);
        mat->number = number;
        md.n_materials = list->count; // 140199c50
        step_remap_material_state(md, true);
        material_list_rebuild_index(list, true);
    }
    double a = 0.0;
    double b = 0.0;
    double c = 0.0;
    double d = 0.0;
    std::int32_t t = 0;
    switch (id - 0x5c) {
    case 0x0: // MTNAME
        b8 = line_string(f, mat->name, 0x41);
        break;
    case 0x1:
        return g4a::read_frae2h(f, *mat);
    case 0x2:
        return g4a::read_fstres(f, *mat);
    case 0x3: // YOUNG +0x670
        b8 = read_table(f, &mat->young, 0, nullptr);
        break;
    case 0x4: // POISON +0x6e8
        b8 = read_table(f, &mat->poison, 0, nullptr);
        break;
    case 0x5: // EXPAND +0x760, extra +0x7d8
        b8 = read_table(f, &mat->expand, 1, &mat->expand_ref);
        break;
    case 0x6: // THRCND +0x7e0
        b8 = read_table(f, &mat->thrcnd, 0, nullptr);
        break;
    case 0x7: { // HEATCP +0x858, extra (int) +0x8d0
        double extra = 0.0;
        b8 = read_table(f, &mat->heatcp, 1, &extra);
        mat->heatcp_extra = static_cast<std::int32_t>(extra);
        break;
    }
    case 0x8: // EMSVTY +0x8d8
        b8 = read_table(f, &mat->emsvty, 0, nullptr);
        break;
    case 0x9: { // LTHEAT: one %i, discarded
        std::int32_t discarded = 0;
        b8 = line_i(f, &discarded, 0);
        break;
    }
    case 0xa: // TMPSOL +0x1ed8
        b8 = line_d(f, &mat->tmpsol);
        break;
    case 0xb: // TMPLIQ +0x1ee0
        b8 = line_d(f, &mat->tmpliq);
        break;
    case 0xc:
        return g4a::read_fperv(f, *mat);
    case 0xd:
        return g4a::read_frcmod(f, *mat);
    case 0xe:
        return g4a::read_creep(f, *mat);
    case 0xf: { // DIFCOE +0x950
        Cls_140289930& dc = mat->difcoe;
        b8 = line_id4(f, &t, &a, &b, &c, &d);
        if (b8 != 0) {
            return 2;
        }
        if (t == 0) {
            dc.mode = 0;
            dynary_init(&dc.d0, 0); // 14012d2a0, not the keeping variant
            dc.d0.value = a;        // +0x960
        }
        else if (t == 1) {
            dc.mode = 1;
            dynary_reinit_keep(&dc.d0, 0x29);
            dc.d0.n[0] = static_cast<std::int32_t>(a); // +0x974
            dc.d0.n[1] = static_cast<std::int32_t>(b); // +0x978
            b8 = read_table_body(f, &dc.d0);
        }
        else if (t == 2) {
            dc.mode = 2;
            dynary_reinit_keep(&dc.d2, 0x27);
            dc.d2.n[0] = static_cast<std::int32_t>(a); // +0x9ec
            b8 = read_table_body(f, &dc.d2);
        }
        else if (t == 3) {
            dc.mode = 3;
            dynary_reinit_keep(&dc.d3, 0x28);
            dc.d3.n[0] = static_cast<std::int32_t>(a); // +0xa64
            b8 = read_table_body(f, &dc.d3);
        }
        else if (t < 0) {
            dc.mode = 4;
            dc.user_routine = -t; // +0xac0
        }
        break;
    }
    case 0x10: // GRNDAT
        b8 = read_grndat(md, f, mat);
        if (b8 == 2) {
            return 2; // the early returns of the case skip FUN_1401d0fc0, as in the original
        }
        break;
    case 0x11: { // GRNMDL: one %i, discarded
        std::int32_t discarded = 0;
        b8 = line_i(f, &discarded, 0);
        break;
    }
    case 0x12:
        return g4a::read_burgrs(f, *mat);
    case 0x13:
        return g4a::read_alpha(f, *mat);
    case 0x14:
        return g4a::read_ndisfm(f, *mat);
    case 0x15:
        return g4a::read_recvry(f, *mat);
    case 0x16: { // HDNPHA +0xf10
        DYNARY& h = mat->hdnpha;
        b8 = line_idd(f, &t, &a, &b);
        if (b8 != 0) {
            return 2;
        }
        dynary_reinit_keep(&h, 0x23);
        if (t == 0) {
            h.ndim = 0;
            h.value = a;
        }
        else if (t == 1) {
            h.ndim = 1;
            h.var[0] = 0xc;
            h.n[0] = static_cast<std::int32_t>(a);
        }
        else if (t == 2) {
            h.ndim = 1;
            h.var[0] = 3;
            h.n[0] = static_cast<std::int32_t>(a);
        }
        else if (t == 3) {
            h.ndim = 1;
            h.var[0] = 9;
            h.n[0] = static_cast<std::int32_t>(a);
        }
        else {
            if (t != 4) {
                return 3;
            }
            h.ndim = 2;
            h.var[0] = 3;
            h.var[1] = 0xc;
            h.n[0] = static_cast<std::int32_t>(a);
            h.n[1] = static_cast<std::int32_t>(b);
        }
        b8 = read_table_body(f, &h);
        break;
    }
    case 0x17: // ELRST +0x1cd8
        b8 = read_table(f, &mat->elrst, 0, nullptr);
        break;
    case 0x18: { // MSTMTR +0x1cc8 flag, +0x1ccc n, +0x1cd0 list
        const std::int32_t old_n = mat->mstmtr_count;
        b8 = line_i(f, &mat->mstmtr_count, 0);
        if (b8 != 0) {
            return 2;
        }
        if (mat->mstmtr_count < 1) {
            if (mat->mstmtr_flag == 1 && mat->mstmtr_list != nullptr && 0 < old_n) {
                for (std::int32_t i = 0; i < old_n; i = i + 1) {
                    Material* part = material_list_find(&md.materials, mat->mstmtr_list[i]);
                    if (part != nullptr) {
                        part->mstmtr_flag = 0;
                    }
                }
                mat->mstmtr_flag = 0;
            }
        }
        else {
            mat->mstmtr_flag = 1;
            if (mat->mstmtr_list != nullptr) {
                std::free(mat->mstmtr_list);
            }
            mat->mstmtr_list = nullptr;
            mat->mstmtr_list = static_cast<std::int32_t*>(std::calloc(mat->mstmtr_count, 4));
            for (std::int32_t i = 0; i < mat->mstmtr_count; i = i + 1) {
                double v = 0.0;
                if (key_fscanf(f, "%Flf", &v) == -1) {
                    return 2;
                }
                mat->mstmtr_list[i] = static_cast<std::int32_t>(v);
            }
        }
        break;
    }
    case 0x19: { // HDNTIM +0xf88
        std::int32_t n = 0;
        b8 = line_i(f, &n, 0);
        if (b8 != 0) {
            return 2;
        }
        dynary_reinit_keep(&mat->hdntim, 0x53);
        mat->hdntim.n[0] = n; // +0xfa4
        b8 = read_table_body(f, &mat->hdntim);
        break;
    }
    case 0x1a: { // JOMINY +0x1000
        std::int32_t n0 = 0;
        std::int32_t n1 = 0;
        b8 = line_iii(f, &t, &n0, &n1, nullptr);
        if (b8 != 0) {
            return 2;
        }
        dynary_reinit_keep(&mat->jominy, 0x52);
        if (t == 1) {
            mat->jominy.n[0] = n0; // +0x101c
            mat->jominy.n[1] = n1; // +0x1020
            b8 = read_table_body(f, &mat->jominy);
        }
        break;
    }
    case 0x1b: { // UTSDAT +0x1b10 (in the FRCMOD block)
        DYNARY& u = mat->frcmod.uts;
        b8 = line_id(f, &t, &a);
        if (b8 != 0) {
            return 2;
        }
        dynary_reinit_keep(&u, 0x1b);
        if (t == 0) {
            u.ndim = 0;
            u.value = a;
        }
        else if (t == 1) {
            u.ndim = 1;
            u.var[0] = 3;
            u.n[0] = static_cast<std::int32_t>(a);
            b8 = read_table_body(f, &u);
        }
        break;
    }
    case 0x1c: // HDNRUL +0x668
        b8 = line_i(f, &mat->hdnrul, 0);
        break;
    case 0x1d: // PMEAB +0x1d50; a 2-D table gets var[1] = 20
        b8 = read_table(f, &mat->pmeab, 0, nullptr);
        if (mat->pmeab.ndim == 2) {
            mat->pmeab.var[1] = 0x14; // +0x1d64
        }
        break;
    case 0x1e: // PMITT +0x1dc8
        b8 = read_table(f, &mat->pmitt, 0, nullptr);
        break;
    case 0x1f:
        return g4a::read_aniso(f, *mat);
    case 0x20: // MATDEN +0x1ee8
        b8 = line_d(f, &mat->matden);
        break;
    case 0x21: // MASDEN +0x1fe0
        b8 = read_table(f, &mat->masden, 0, nullptr);
        break;
    case 0x22: // COARSE +0x20d0 flag, +0x20d8 table
        b8 = line_i(f, &mat->coarse_flag, 0);
        if (0 < mat->coarse_flag) {
            b8 = read_table(f, &mat->coarse, 0, nullptr);
        }
        break;
    case 0x23: // SIZEMD +0x2150
        b8 = line_i(f, &mat->sizemd, 0);
        break;
    case 0x24: // TXTURE +0x2154
        if (line_iii(f, &mat->txture.a, &mat->txture.b, &mat->txture.c, nullptr) != 0) {
            return 2;
        }
        b8 = 0;
        break;
    case 0x25: { // DIFBND +0x2160 (no re-init: type 0xaf, ncomp 2 stay)
        DYNARY& db = mat->difbnd;
        b8 = line_idd(f, &t, &a, &b);
        if (b8 == 0 && t == 7) {
            db.ndim = 2;
            db.var[0] = 3;
            db.var[1] = 4;
            db.n[0] = static_cast<std::int32_t>(a);
            db.n[1] = static_cast<std::int32_t>(b);
            b8 = read_table_body(f, &db);
        }
        else {
            if (b8 != 0 || t != 0) {
                return 2;
            }
            db.n[0] = 0;
            db.n[1] = 0;
            db.value = a; // +0x2168; ndim is left as it was
            b8 = 0;
        }
        break;
    }
    case 0x26: { // SINTER +0x21d8 model, +0x21e0 p1..p4
        std::int32_t count = 0;
        b8 = line_ii(f, &mat->sinter_model, &count);
        if (mat->sinter_model != 0 && 0 < count) {
            b8 = line_d4(f, &mat->sinter_p[0], &mat->sinter_p[1], &mat->sinter_p[2],
                         &mat->sinter_p[3]);
        }
        break;
    }
    case 0x27: // GBENGY +0x17f0
        b8 = read_table(f, &mat->gbengy, 0, nullptr);
        break;
    case 0x28: // GBMOBI +0x1868, +0x1870
        b8 = line_dd(f, &mat->gbmobi_p, &mat->gbmobi_q);
        break;
    case 0x29: // NUCSIZ +0x1878
        b8 = read_table(f, &mat->nucsiz, 0, nullptr);
        break;
    default:
        return 2;
    }
    return b8 == 0 ? 0 : 2;
}

} // namespace def::key::g4
