// DBREAD helper W2 (packet p-dbread): ReadMatlInfo 1401cff20 and the LLMPR record reader
// 14028f300 (LLMAT_DATA::ReadInfoFromDBArray) with its GRNDAT / FRCMOD helpers.
// Word map: docs/re/dbmap/material.md (branch claude/project-thread-bz5k5e); the GENDB mirror is
// port/defdbio/src/gendb/p4_06/material.cpp (140294ae0). Reference: decompile
// out/dbread/w2/ann_14028f300.c and the listing out/dbread/w2/listing.txt (deform-rea-work).
//
// In the decompile of 14028f300 param_1 is an int*: param_1[k] is the int at byte 4k and
// param_1 + k the object at byte 4k. The helpers I / D / F below take that index k, so each line
// compares directly with the decompile (byte offset = 4k). w[k] is record word k (real64);
// (int)w[k] is CVTTSD2SI (truncation).
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS // strncpy as in the original
#endif
#include <defdbio/dbread/w2.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <defdbio/gendb/gate_ports.hpp>
#include <defdbio/gendb/gendb.hpp>
#include <defdbio/gendb/pre_steps.hpp>
#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/cls_14028a4c0.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/material/material_ctor.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/ftnstr/ftnstr.hpp>
#include <defrt/stdlib/refcount.hpp>

#include "dbread/pending_w2.hpp"

namespace def::dbio::dbread {

using model::DB_STEP_DATA;
using model::DYNARY;
using model::Material;

namespace {

// int / double / DYNARY at int index k of the material (byte 4k), as param_1[k] in the decompile
inline std::int32_t& I(Material* m, std::size_t k) { return model::field<std::int32_t>(m, k * 4); }
inline double& D(Material* m, std::size_t k) { return model::field<double>(m, k * 4); }
inline DYNARY* F(Material* m, std::size_t k) { return &model::field<DYNARY>(m, k * 4); }

// *(param_1 + 0x882): the model back pointer (Material+0x2208)
inline DB_STEP_DATA* owner(Material* m) { return model::field<DB_STEP_DATA*>(m, 0x2208); }

// 1401309b0 with a local flag byte
inline void rd(DYNARY* f, const std::int32_t* iifnc, const double* llfnc, std::int32_t fid, char flag)
{
    fun_1401309b0(f, iifnc, llfnc, fid, &flag);
}

inline std::int32_t ti(double x) { return static_cast<std::int32_t>(x); } // CVTTSD2SI
// NEG of a CVTTSD2SI result (32-bit wraparound)
inline std::int32_t neg_ti(double x) { return static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(ti(x))); }
// 140484ab8: CRT abs (abs(INT_MIN) == INT_MIN)
inline std::int32_t crt_abs(std::int32_t v)
{
    return v < 0 ? static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(v)) : v;
}

// part of @src 14028f300: the common property pattern P(fn, const, type, n1, n2) of
// YOUNG / POISON / EXPAND / THRCND / HEATCP: type 0 var 3, 1 var 9, 2 var 0xc, 3 2-D (3, 0xc)
void read_property(Material* p, const double* w, const std::int32_t* iifnc, const double* llfnc, std::size_t f,
                   std::size_t w_fn, std::size_t w_const, std::size_t w_type, std::size_t w_n1, std::size_t w_n2)
{
    DYNARY* d = F(p, f);
    if (ti(w[w_fn]) == 0) {
        d->ndim = 0;
        d->value = w[w_const];
    } else {
        const std::int32_t t = ti(w[w_type]);
        if (t == 0) {
            d->ndim = 1;
            d->var[0] = 3;
        } else if (t == 1) {
            d->ndim = 1;
            d->var[0] = 9;
        } else if (t == 2) {
            d->ndim = 1;
            d->var[0] = 0xc;
        } else if (t == 3) {
            d->ndim = 2;
            d->var[0] = 3;
            d->var[1] = 0xc;
            d->n[0] = ti(w[w_n1]);
            d->n[1] = ti(w[w_n2]);
        }
        rd(d, iifnc, llfnc, ti(w[w_fn]), 0);
    }
}

// part of @src 14028f300: FSTRES 6 / 12 sub-table: code 0 const, 1 var 3, 2 var `v2`, 3 2-D (3, v2)
void read_fstres_table(DYNARY* d, std::int32_t code, double c_or_n1, double n2, std::int32_t v2)
{
    if (code == 0) {
        d->ndim = 0;
        d->value = c_or_n1;
    } else if (code == 1) {
        d->ndim = 1;
        d->var[0] = 3;
        d->n[0] = ti(c_or_n1);
    } else if (code == 2) {
        d->ndim = 1;
        d->var[0] = v2;
        d->n[0] = ti(c_or_n1);
    } else if (code == 3) {
        d->ndim = 2;
        d->var[0] = 3;
        d->var[1] = v2;
        d->n[0] = ti(c_or_n1);
        d->n[1] = ti(n2);
    }
}

// part of @src 14028f300: BURGRS / ALPHA / NDISFM / RECVRY function read: flag 1 for 2-D/3-D
// (the sizes come from the n1, n2(, n3) prefix in LLFNC)
void read_disloc(DYNARY* d, const std::int32_t* iifnc, const double* llfnc, double fid)
{
    if (d->ndim == 1) {
        rd(d, iifnc, llfnc, ti(fid), 0);
    } else if (d->ndim == 2) {
        rd(d, iifnc, llfnc, ti(fid), 1);
    } else if (d->ndim == 3) {
        rd(d, iifnc, llfnc, ti(fid), 1);
    }
}

// part of @src 14028f300: ALPHA / NDISFM / RECVRY code: 0 const, 1 var 3, 2 var 8, 3 2-D (3, 8)
void disloc_code(DYNARY* d, std::int32_t code, double c)
{
    if (code == 0) {
        d->ndim = 0;
        d->value = c;
    } else if (code == 1) {
        d->ndim = 1;
        d->var[0] = 3;
    } else if (code == 2) {
        d->ndim = 1;
        d->var[0] = 8;
    } else if (code == 3) {
        d->ndim = 2;
        d->var[0] = 3;
        d->var[1] = 8;
    } else {
        std::fprintf(stdout, "Error in LLMAT_DATA::ReadInfoFromDBArray()\n");
    }
}

// part of @src 14028f300: GRNDAT mode-1 table type: 1 var 3, 2 var 8, 3 var 0x21 (`with21`)
inline void grn_var(DYNARY* d, std::int32_t t, bool with21)
{
    if (t == 1) {
        d->var[0] = 3;
    } else if (t == 2) {
        d->var[0] = 8;
    } else if (with21 && t == 3) {
        d->var[0] = 0x21;
    }
}

// part of @src 14028f300: GRNDAT mode-1 optional table: type 0 off (flag 0), 1/2(/3) on
inline void grn_opt(Material* p, std::size_t flag, DYNARY* d, std::int32_t t, bool with21)
{
    if (t == 0) {
        I(p, flag) = 0;
    } else if (t == 1) {
        I(p, flag) = 1;
        d->var[0] = 3;
    } else if (t == 2) {
        I(p, flag) = 1;
        d->var[0] = 8;
    } else if (with21 && t == 3) {
        I(p, flag) = 1;
        d->var[0] = 0x21;
    }
}

} // namespace

// @src 1401cff20  _DB_STEP_DATA::ReadMatlInfo
void read_materials(DB_STEP_DATA* m)
{
    std::int32_t dim = m->dimension; // 140158e90
    model::select_index_tables(&dim);
    const std::int32_t start = m->u1dc0[model::g_a_index[(0x1406e82d4ull - 0x1406e82d0ull) / 4]]; // LLMPR
    const double* a = static_cast<const double*>(m->u1b78);
    // 1401cffa0: operator new(4 * n) (size_t overflow -> SIZE_MAX); freed with free at 1401d0154
    const std::uint64_t n64 = static_cast<std::uint64_t>(static_cast<std::int64_t>(m->n_materials));
    const std::size_t bytes = n64 > (SIZE_MAX / 4) ? SIZE_MAX : static_cast<std::size_t>(n64 * 4);
    auto* num = static_cast<std::int32_t*>(std::malloc(bytes));
    for (std::int32_t i = 0; i < m->n_materials; i = i + 1) {
        Material* mat = model::material_list_add(&m->materials); // 14029c060
        const std::int32_t w = m->u1dc0[model::g_a_index[(0x1406e83c0ull - 0x1406e82d0ull) / 4]]; // width LLMPR
        const std::int32_t v = gendb::stored_version(*m);                                          // 1401cc180
        material_read_from_db(mat, a + (start - 1) + static_cast<std::int32_t>(w * i),
                              static_cast<const std::int32_t*>(m->u2220), static_cast<const double*>(m->u2228), v);
        num[i] = mat->number;
    }
    for (std::int32_t i = 0; i < m->n_materials; i = i + 1) {
        Material* mat = model::material_list_nth(&m->materials, i + 1); // 14029c260
        if (mat != nullptr && mat->mstmtr_flag == 1) {
            for (std::int32_t k = 0; k < mat->mstmtr_count; k = k + 1) {
                mat->mstmtr_list[k] = num[mat->mstmtr_list[k] - 1]; // DB slot -> material number
            }
        }
    }
    std::free(num);
    gendb::mixture_material_cleanup(m); // 1401d6250
}

// @src 14028f300  LLMAT_DATA::ReadInfoFromDBArray
void material_read_from_db(Material* p, const double* w, const std::int32_t* iifnc, const double* llfnc,
                           std::int32_t version)
{
    bool is2d = true;
    if (owner(p) != nullptr) {
        is2d = def::rt::dimension_is_2(owner(p)); // 1400015e0
    }
    I(p, 0) = ti(w[0]);
    char* name = reinterpret_cast<char*>(&I(p, 3));
    std::strncpy(name, reinterpret_cast<const char*>(w + 2), 0x40);
    fun_140324020(name, owner(p)->u748, 8, 0);
    def::rt::trim_fortran(name, 0x40);

    // ---- FSTRES (w11) ----
    switch (ti(w[0xb])) {
    case 0:
        I(p, 0x14) = -1;
        break;
    case 1:
        I(p, 0x14) = 0;
        D(p, 0x16) = w[0xc];
        D(p, 0x18) = w[0xd];
        D(p, 0x1a) = w[0xe];
        D(p, 0x1c) = w[0xf];
        break;
    case 2:
    case 3: {
        I(p, 0x14) = 1;
        const double fn = w[0xf];
        I(p, 0x25) = ti(w[0xc]);
        I(p, 0x26) = ti(w[0xd]);
        I(p, 0x27) = ti(w[0xe]);
        rd(F(p, 0x1e), iifnc, llfnc, ti(fn), 0);
        I(p, 0x32) = ti(w[0xb]) == 2 ? 1 : 0;
        break;
    }
    case 4:
        I(p, 0x14) = 2;
        D(p, 0x3c) = w[0xc];
        D(p, 0x3e) = w[0xd];
        D(p, 0x40) = w[0xe];
        D(p, 0x42) = w[0xf];
        D(p, 0x44) = w[0x10];
        break;
    case 5:
        I(p, 0x14) = 3;
        D(p, 0x46) = w[0xc];
        D(p, 0x48) = w[0xd];
        D(p, 0x4a) = w[0xe];
        D(p, 0x4c) = w[0xf];
        break;
    case 6:
        I(p, 0x14) = 4;
        read_fstres_table(F(p, 0x6c), ti(w[0xc]), w[0xe], w[0xf], 0xc);
        if (I(p, 0x6d) != 0) {
            rd(F(p, 0x6c), iifnc, llfnc, ti(w[0x12]), 0);
        }
        read_fstres_table(F(p, 0x4e), ti(w[0xd]), w[0x10], w[0x11], 0xc);
        if (I(p, 0x4f) != 0) {
            rd(F(p, 0x4e), iifnc, llfnc, ti(w[0x13]), 0);
        }
        break;
    case 7:
    case 8: {
        I(p, 0x14) = 5;
        const double fn = w[0xf];
        I(p, 0x91) = ti(w[0xc]);
        I(p, 0x92) = ti(w[0xd]);
        I(p, 0x93) = ti(w[0xe]);
        rd(F(p, 0x8a), iifnc, llfnc, ti(fn), 0);
        I(p, 0x9e) = ti(w[0xb]) == 7 ? 1 : 0;
        break;
    }
    case 9:
        I(p, 0x14) = 6;
        D(p, 0xa8) = w[0xc];
        D(p, 0xaa) = w[0xd];
        D(p, 0xac) = w[0xe];
        D(p, 0xae) = w[0xf];
        D(p, 0xb0) = w[0x10];
        D(p, 0xb2) = w[0x11];
        D(p, 0xb4) = w[0x12];
        D(p, 0xb6) = w[0x13];
        D(p, 0xb8) = w[0x14];
        D(p, 0xba) = w[0x15];
        D(p, 0xbc) = w[0x16];
        D(p, 0xbe) = w[0x4e];
        D(p, 0xc0) = w[0x4f];
        D(p, 0xc2) = w[0x50];
        break;
    case 10:
        I(p, 0x14) = 7;
        D(p, 0xc4) = w[0xc];
        D(p, 0xc6) = w[0xd];
        D(p, 0xc8) = w[0xe];
        D(p, 0xca) = w[0xf];
        D(p, 0xcc) = w[0x10];
        D(p, 0xce) = w[0x11];
        break;
    case 0xb:
        I(p, 0x14) = 8;
        D(p, 0xd0) = w[0xc];
        D(p, 0xd2) = w[0xd];
        D(p, 0xd4) = w[0xe];
        D(p, 0xd6) = w[0xf];
        D(p, 0xd8) = w[0x10];
        break;
    case 0xc:
        I(p, 0x14) = 9;
        read_fstres_table(F(p, 0x100), ti(w[0xc]), w[0xe], w[0xf], 8);
        if (I(p, 0x101) != 0) {
            rd(F(p, 0x100), iifnc, llfnc, ti(w[0x12]), 0);
        }
        break;
    case 0xd:
    case 0xf:
        I(p, 0x14) = ti(w[0xb]) == 0xd ? 10 : 0xc;
        I(p, 0x11e) = ti(w[0xc]);
        I(p, 0x127) = ti(w[0xc]);
        rd(F(p, 0x120), iifnc, llfnc, ti(w[0xf]), 0);
        I(p, 0x14e) = ti(w[0xd]);
        rd(F(p, 0x13e), iifnc, llfnc, ti(w[0x10]), 0);
        I(p, 0x16c) = ti(w[0xe]);
        rd(F(p, 0x15c), iifnc, llfnc, ti(w[0x11]), 0);
        break;
    case 0xe:
        I(p, 0x14) = 0xb;
        D(p, 0xda) = w[0xc];
        D(p, 0xdc) = w[0xd];
        D(p, 0xde) = w[0xe];
        D(p, 0xe0) = w[0xf];
        if (ti(w[0x10]) == 0) {
            I(p, 0xe3) = 0;
            D(p, 0xe4) = w[0x11];
        } else {
            dynary_read_selfdesc(F(p, 0xe2), iifnc, llfnc, ti(w[0x10]));
        }
        break;
    case 0x10: {
        I(p, 0x14) = 0xd;
        const double fn = w[0x10];
        I(p, 0x181) = ti(w[0xc]);
        I(p, 0x182) = ti(w[0xd]);
        I(p, 0x183) = ti(w[0xe]);
        I(p, 0x18a) = ti(w[0xf]);
        rd(F(p, 0x17a), iifnc, llfnc, ti(fn), 0);
        I(p, 0x18e) = 0;
        break;
    }
    default:
        I(p, 0x14) = 1;
        I(p, 0x32) = 0;
        break;
    }
    if (0.0 > w[0xb]) { // 1402906df: COMISD 0.0, w11; JBE skips (also on unordered)
        I(p, 0x14) = 0xe;
        I(p, 0x198) = neg_ti(w[0xb]);
    }
    // HDNRUL (w104)
    if (ti(w[0x68]) == 0) {
        I(p, 0x19a) = 0;
    } else {
        I(p, 0x19a) = 1;
    }
    read_property(p, w, iifnc, llfnc, 0x19c, 0x21, 0x20, 0x17, 0x18, 0x19); // YOUNG
    read_property(p, w, iifnc, llfnc, 0x1ba, 0x23, 0x22, 0x1a, 0x1b, 0x1c); // POISON
    read_property(p, w, iifnc, llfnc, 0x1d8, 0x25, 0x24, 0x1d, 0x1e, 0x1f); // EXPAND
    if (!is2d) {
        if (4999 < version) {
            D(p, 0x1f6) = w[0x58]; // EXPAND reference temperature
        }
    } else if (7999 < version) {
        D(p, 0x1f6) = w[0x58];
    }
    read_property(p, w, iifnc, llfnc, 0x1f8, 0x27, 0x26, 0x2f, 0x54, 0x55); // THRCND
    read_property(p, w, iifnc, llfnc, 0x216, 0x29, 0x28, 0x30, 0x56, 0x57); // HEATCP
    I(p, 0x234) = ti(w[99]);                                                 // HEATCP extra
    // MASDEN (w82 fn, w81 const, w83 type 0/1/2)
    if (ti(w[0x52]) == 0) {
        I(p, 0x7f9) = 0;
        D(p, 0x7fa) = w[0x51];
    } else {
        const std::int32_t t = ti(w[0x53]);
        if (t == 0) {
            I(p, 0x7f9) = 1;
            I(p, 0x7fc) = 3;
        } else if (t == 1) {
            I(p, 0x7f9) = 1;
            I(p, 0x7fc) = 9;
        } else if (t == 2) {
            I(p, 0x7f9) = 1;
            I(p, 0x7fc) = 0xc;
        }
        rd(F(p, 0x7f8), iifnc, llfnc, ti(w[0x52]), 0);
    }
    // COARSE (w65): a 20-word header function
    if (ti(w[0x41]) < 1) {
        I(p, 0x834) = 0;
    } else {
        DYNARY hdr; // 14012d050
        hdr.ncomp = 0x14;
        rd(&hdr, iifnc, llfnc, ti(w[0x41]), 0);
        const double* h = hdr.values;
        if (h[2] == 0.0) { // 1402912a0: UCOMISD [14051bed8] = 0.0; JP / JNZ
            I(p, 0x834) = ti(h[0]);
            I(p, 0x837) = 0;
            D(p, 0x838) = h[3];
        } else if (h[2] == 5.0) { // 140291307: UCOMISD [140522f48] = 5.0
            I(p, 0x834) = ti(h[0]);
            const double fn = h[1];
            I(p, 0x837) = 2;
            I(p, 0x83a) = 3;
            I(p, 0x83b) = 8;
            I(p, 0x83d) = ti(h[3]);
            I(p, 0x83e) = ti(h[4]);
            rd(F(p, 0x836), iifnc, llfnc, ti(fn), 0);
        }
    } // 14012f410
    // EMSVTY (w46 fn, w45 const)
    if (ti(w[0x2e]) == 0) {
        I(p, 0x237) = 0;
        D(p, 0x238) = w[0x2d];
    } else {
        I(p, 0x237) = 1;
        I(p, 0x23a) = 3;
        rd(F(p, 0x236), iifnc, llfnc, ti(w[0x2e]), 0);
    }
    // DIFCOE (w53..w56)
    if (ti(w[0x35]) < 0) {
        I(p, 0x254) = 4;
        I(p, 0x2b0) = crt_abs(ti(w[0x35]));
    } else {
        const std::int32_t t = ti(w[0x35]);
        if (t == 0) {
            I(p, 0x254) = 0;
            I(p, 599) = 0;
            D(p, 600) = w[0x38];
        } else if (t == 1) {
            I(p, 0x254) = 1;
            const double fn = w[0x38];
            I(p, 599) = 2;
            I(p, 0x25d) = ti(w[0x36]);
            I(p, 0x25e) = ti(w[0x37]);
            rd(F(p, 0x256), iifnc, llfnc, ti(fn), 0);
        } else if (t == 2) {
            I(p, 0x254) = 2;
            const double fn = w[0x38];
            I(p, 0x27b) = ti(w[0x36]);
            rd(F(p, 0x274), iifnc, llfnc, ti(fn), 0);
        } else if (t == 3) {
            I(p, 0x254) = 3;
            const double fn = w[0x38];
            I(p, 0x299) = ti(w[0x36]);
            rd(F(p, 0x292), iifnc, llfnc, ti(fn), 0);
        }
    }
    // CREEP (w57..w64)
    if (ti(w[0x39]) < 0) {
        I(p, 0x2b2) = 9;
        I(p, 0x368) = crt_abs(ti(w[0x39]));
    } else {
        switch (ti(w[0x39])) {
        case 0:
            I(p, 0x2b2) = 0;
            break;
        case 1: {
            I(p, 0x2b2) = 1;
            const double fn = w[0x40];
            I(p, 0x2f7) = ti(w[0x3a]);
            rd(F(p, 0x2f0), iifnc, llfnc, ti(fn), 0);
            break;
        }
        case 2: {
            I(p, 0x2b2) = 2;
            const double fn = w[0x40];
            I(p, 0x315) = ti(w[0x3a]);
            rd(F(p, 0x30e), iifnc, llfnc, ti(fn), 0);
            break;
        }
        case 3: {
            I(p, 0x2b2) = 3;
            const double fn = w[0x40];
            I(p, 0x333) = ti(w[0x3a]);
            rd(F(p, 0x32c), iifnc, llfnc, ti(fn), 0);
            break;
        }
        case 4: {
            I(p, 0x2b2) = 4;
            const double fn = w[0x40];
            I(p, 0x351) = ti(w[0x3a]);
            rd(F(p, 0x34a), iifnc, llfnc, ti(fn), 0);
            break;
        }
        case 5: {
            I(p, 0x2b2) = 5;
            const double fn = w[0x40];
            I(p, 0x2d9) = ti(w[0x3a]);
            I(p, 0x2da) = ti(w[0x3b]);
            I(p, 0x2db) = ti(w[0x3c]);
            rd(F(p, 0x2d2), iifnc, llfnc, ti(fn), 0);
            break;
        }
        case 6: {
            I(p, 0x2b2) = 6;
            const double fn = w[0x40];
            I(p, 699) = ti(w[0x3a]);
            I(p, 700) = ti(w[0x3b]);
            I(p, 0x2bd) = ti(w[0x3c]);
            rd(F(p, 0x2b4), iifnc, llfnc, ti(fn), 0);
            break;
        }
        case 7:
        case 8:
            I(p, 0x2b2) = ti(w[0x39]);
            I(p, 0x369) = ti(w[0x3a]);
            I(p, 0x371) = ti(w[0x3a]);
            rd(F(p, 0x36a), iifnc, llfnc, ti(w[0x3d]), 0);
            I(p, 0x398) = ti(w[0x3b]);
            rd(F(p, 0x388), iifnc, llfnc, ti(w[0x3e]), 0);
            I(p, 0x3b6) = ti(w[0x3c]);
            rd(F(p, 0x3a6), iifnc, llfnc, ti(w[0x3f]), 0);
            break;
        default:
            break;
        }
    }
    // HDNPHA (w89..w92)
    {
        const std::int32_t t = ti(w[0x59]);
        if (t == 0) {
            I(p, 0x3c5) = 0;
            D(p, 0x3c6) = w[0x5a];
        } else if (t == 1) {
            I(p, 0x3c5) = 1;
            I(p, 0x3c8) = 0xc;
        } else if (t == 2) {
            I(p, 0x3c5) = 1;
            I(p, 0x3c8) = 3;
        } else if (t == 3) {
            I(p, 0x3c5) = 1;
            I(p, 0x3c8) = 9;
        } else if (t == 4) {
            I(p, 0x3c5) = 2;
            I(p, 0x3c8) = 3;
            I(p, 0x3c9) = 0xc;
            I(p, 0x3cb) = ti(w[0x5b]);
            I(p, 0x3cc) = ti(w[0x5c]);
        }
        if (ti(w[0x59]) != 0) {
            rd(F(p, 0x3c4), iifnc, llfnc, ti(w[0x5a]), 0);
        }
    }
    // HDNTIM (w95), JOMINY (w96, w97, w98)
    rd(F(p, 0x3e2), iifnc, llfnc, ti(w[0x5f]), 0);
    {
        const double fn = w[0x62];
        I(p, 0x407) = ti(w[0x60]);
        I(p, 0x408) = ti(w[0x61]);
        rd(F(p, 0x400), iifnc, llfnc, ti(fn), 0);
    }
    // GRNDAT (w109 mode)
    if (ti(w[0x6d]) == 0) {
        I(p, 0x5fa) = 0;
    } else if (ti(w[0x6d]) == 1) {
        I(p, 0x5fa) = 1;
        const double* q = llfnc + (iifnc[ti(w[0x82]) * 2 - 2] - 1);
        grn_var(F(p, 0x422), ti(w[0x6e]), false);
        I(p, 0x429) = ti(w[0x6f]);
        fun_140131ea0(F(p, 0x422), q);
        q = q + static_cast<std::int32_t>(I(p, 0x429) * (I(p, 0x432) + 1));
        grn_var(F(p, 0x440), ti(w[0x70]), false);
        I(p, 0x447) = ti(w[0x71]);
        fun_140131ea0(F(p, 0x440), q);
        q = q + static_cast<std::int32_t>(I(p, 0x447) * (I(p, 0x450) + 1));
        grn_opt(p, 0x41e, F(p, 0x45e), ti(w[0x72]), false);
        I(p, 0x465) = ti(w[0x73]);
        fun_140131ea0(F(p, 0x45e), q);
        q = q + static_cast<std::int32_t>(I(p, 0x465) * (I(p, 0x46e) + 1));
        grn_var(F(p, 0x47c), ti(w[0x74]), true);
        I(p, 0x483) = ti(w[0x75]);
        fun_140131ea0(F(p, 0x47c), q);
        q = q + static_cast<std::int32_t>(I(p, 0x483) * (I(p, 0x48c) + 1));
        grn_opt(p, 0x41f, F(p, 0x49a), ti(w[0x76]), false);
        I(p, 0x4a1) = ti(w[0x77]);
        fun_140131ea0(F(p, 0x49a), q);
        q = q + static_cast<std::int32_t>(I(p, 0x4a1) * (I(p, 0x4aa) + 1));
        grn_var(F(p, 0x4b8), ti(w[0x78]), true);
        I(p, 0x4bf) = ti(w[0x79]);
        fun_140131ea0(F(p, 0x4b8), q);
        q = q + static_cast<std::int32_t>(I(p, 0x4bf) * (I(p, 0x4c8) + 1));
        grn_opt(p, 0x420, F(p, 0x4d6), ti(w[0x7a]), false);
        I(p, 0x4dd) = ti(w[0x7b]);
        fun_140131ea0(F(p, 0x4d6), q);
        q = q + static_cast<std::int32_t>(I(p, 0x4dd) * (I(p, 0x4e6) + 1));
        grn_var(F(p, 0x4f4), ti(w[0x7c]), true);
        I(p, 0x4fb) = ti(w[0x7d]);
        fun_140131ea0(F(p, 0x4f4), q);
        grn_opt(p, 0x421, F(p, 0x512), ti(w[0x7e]), true);
        I(p, 0x519) = ti(w[0x7f]);
        fun_140131ea0(F(p, 0x512), q + static_cast<std::int32_t>(I(p, 0x4fb) * (I(p, 0x504) + 1)));
        D(p, 0x532) = w[0x80];
        D(p, 0x530) = w[0x81];
    } else if (ti(w[0x6d]) == 2) {
        I(p, 0x5fa) = 2;
        I(p, 0x534) = ti(w[0x6e]);
        const std::int32_t check = ti(w[0x6f]);
        grain_record_read(&model::field<model::Cls_14028a4c0>(p, 0x536 * 4),
                          llfnc + (iifnc[ti(w[0x70]) * 2 - 2] - 1));
        auto* g = &model::field<model::Cls_14028b640>(p, 0x534 * 4);
        const std::int32_t n1 = ti(w[0x71]);
        const std::int32_t f1 = ti(w[0x72]);
        grain_record_list_read(&g->l1, iifnc, llfnc, n1, f1); // +0x16f8
        const std::int32_t n2 = ti(w[0x73]);
        const std::int32_t f2 = ti(w[0x74]);
        grain_record_list_read(&g->l2, iifnc, llfnc, n2, f2); // +0x16e0
        const std::int32_t n3 = ti(w[0x75]);
        const std::int32_t f3 = ti(w[0x76]);
        grain_record_list_read(&g->l3, iifnc, llfnc, n3, f3); // +0x16c8
        const std::int32_t n4 = ti(w[0x77]);
        const std::int32_t f4 = ti(w[0x78]);
        grain_record_list_read(&g->l4, iifnc, llfnc, n4, f4); // +0x1710
        const std::int32_t n5 = ti(w[0x79]);
        const std::int32_t f5 = ti(w[0x7a]);
        grain_record_list_read(&g->l5, iifnc, llfnc, n5, f5); // +0x1728
        if (model::grndat_record_count(g) != check) {         // 14028b780
            std::fprintf(stdout, "Inconsistency is detected in MESOSCALE Grain model.\n");
        }
    } else if (ti(w[0x6d]) == 4) {
        I(p, 0x5fa) = 4;
        I(p, 0x5d1) = 2;
        I(p, 0x5d4) = 3;
        I(p, 0x5d5) = 8;
        I(p, 0x5d7) = ti(w[0x6e]);
        I(p, 0x5d8) = ti(w[0x6f]);
        rd(F(p, 0x5d0), iifnc, llfnc, ti(w[0x70]), 0);
    } else if (ti(w[0x6d]) == 5) {
        I(p, 0x5fa) = 5;
        D(p, 0x5ee) = w[0x6e];
        D(p, 0x5f0) = w[0x6f];
        D(p, 0x5f2) = w[0x70];
        D(p, 0x5f4) = w[0x71];
        D(p, 0x5f6) = w[0x72];
        D(p, 0x5f8) = w[0x73];
    } else if (ti(w[0x6d]) < 0) {
        I(p, 0x5fa) = -1;
        I(p, 0x5fb) = crt_abs(ti(w[0x6d]));
    }
    // BURGRS (w139 code, w140)
    DYNARY* burgrs = F(p, 0x63c);
    switch (ti(w[0x8b])) {
    case 0:
        I(p, 0x63d) = 0;
        D(p, 0x63e) = w[0x8c];
        break;
    case 1:
        I(p, 0x63d) = 1;
        I(p, 0x640) = 3;
        break;
    case 2:
        I(p, 0x63d) = 1;
        I(p, 0x640) = 6;
        break;
    case 3:
        I(p, 0x63d) = 1;
        I(p, 0x640) = 0x18;
        break;
    case 4:
        I(p, 0x63d) = 2;
        I(p, 0x640) = 3;
        I(p, 0x641) = 6;
        break;
    case 5:
        I(p, 0x63d) = 2;
        I(p, 0x640) = 3;
        I(p, 0x641) = 0x18;
        break;
    case 6:
        I(p, 0x63d) = 2;
        I(p, 0x640) = 6;
        I(p, 0x641) = 0x18;
        break;
    case 7:
        I(p, 0x63d) = 3;
        I(p, 0x640) = 3;
        I(p, 0x641) = 6;
        I(p, 0x642) = 0x18;
        break;
    default:
        std::fprintf(stdout, "Error in LLMAT_DATA::ReadInfoFromDBArray()\n");
        break;
    }
    read_disloc(burgrs, iifnc, llfnc, w[0x8c]);
    // ALPHA (w141, w142), NDISFM (w143, w144), RECVRY (w145, w146)
    disloc_code(F(p, 0x65a), ti(w[0x8d]), w[0x8e]);
    if (I(p, 0x65b) == 1) {
        rd(F(p, 0x65a), iifnc, llfnc, ti(w[0x8e]), 0);
    } else if (I(p, 0x65b) == 2) {
        rd(F(p, 0x65a), iifnc, llfnc, ti(w[0x8e]), 1);
    }
    disloc_code(F(p, 0x678), ti(w[0x8f]), w[0x90]);
    if (I(p, 0x679) == 1) {
        rd(F(p, 0x678), iifnc, llfnc, ti(w[0x90]), 0);
    } else if (I(p, 0x679) == 2) {
        rd(F(p, 0x678), iifnc, llfnc, ti(w[0x90]), 1);
    }
    disloc_code(F(p, 0x696), ti(w[0x91]), w[0x92]);
    if (I(p, 0x697) == 1) {
        rd(F(p, 0x696), iifnc, llfnc, ti(w[0x92]), 0);
    } else if (I(p, 0x697) == 2) {
        rd(F(p, 0x696), iifnc, llfnc, ti(w[0x92]), 1);
    }
    // MSTMTR (w93 count, w94 function of DB slots)
    if (ti(w[0x5d]) != 0) {
        I(p, 0x732) = 1;
        I(p, 0x733) = ti(w[0x5d]);
        auto* list = static_cast<std::int32_t*>(
            std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(I(p, 0x733))), 4));
        model::field<std::int32_t*>(p, 0x734 * 4) = list;
        const std::int32_t s = iifnc[ti(w[0x5e]) * 2 - 2];
        for (std::int32_t k = 0; k < I(p, 0x733); k = k + 1) {
            model::field<std::int32_t*>(p, 0x734 * 4)[k] = ti(llfnc[(s - 1) + k]);
        }
    }
    // FRCMOD (w70 code, w71, w72; w74, w149)
    if (ti(w[0x46]) < 0) {
        I(p, 0x6b4) = 0xb;
        I(p, 0x6c2) = ti(w[0x46]);
    } else {
        switch (ti(w[0x46])) {
        case 0:
            I(p, 0x6b4) = 0;
            break;
        case 1:
            I(p, 0x6b4) = 1;
            break;
        case 2:
            I(p, 0x6b4) = 2;
            D(p, 0x6b6) = w[0x47];
            break;
        case 3:
            I(p, 0x6b4) = 3;
            break;
        case 4:
            I(p, 0x6b4) = 4;
            D(p, 0x6b8) = w[0x47];
            break;
        case 5:
            I(p, 0x6b4) = 5;
            D(p, 0x6ba) = w[0x47];
            break;
        case 6:
            I(p, 0x6b4) = 6;
            break;
        case 7:
            I(p, 0x6b4) = 7;
            D(p, 0x6bc) = w[0x47];
            D(p, 0x6be) = w[0x48];
            break;
        case 8:
            I(p, 0x6b4) = 8;
            break;
        case 9:
            I(p, 0x6b4) = 9;
            break;
        case 10:
            I(p, 0x6b4) = 10;
            break;
        case 0xb: {
            I(p, 0x6b4) = 0xc;
            const std::int32_t fn = ti(w[0x48]);
            if (0 < fn) {
                const std::int32_t s = iifnc[fn * 2 - 2] - 1;
                frcmod_code11_read(&I(p, 0x6b4), llfnc + s);
            }
            break;
        }
        case 0xc:
            I(p, 0x6b4) = 0xd;
            I(p, 0x6e5) = 1;
            I(p, 0x6e8) = 0x1f;
            rd(F(p, 0x6e4), iifnc, llfnc, ti(w[0x47]), 0);
            break;
        case 0xf:
            I(p, 0x6b4) = 0xf;
            D(p, 0x6ba) = w[0x47];
            break;
        case 0x10:
            I(p, 0x6b4) = 0x10;
            break;
        case 0x11: {
            I(p, 0x6b4) = 0x11;
            const double fn = w[0x48];
            I(p, 0x709) = ti(w[0x47]);
            rd(F(p, 0x702), iifnc, llfnc, ti(fn), 0);
            break;
        }
        default:
            break;
        }
    }
    D(p, 0x6c0) = w[0x4a];
    D(p, 0x6e2) = w[0x95];
    // UTSDAT (w107 flag, w108)
    if (ti(w[0x6b]) == 0) {
        I(p, 0x6c5) = 0;
        D(p, 0x6c6) = w[0x6c];
    } else {
        I(p, 0x6c5) = 1;
        I(p, 0x6c8) = 3;
        rd(F(p, 0x6c4), iifnc, llfnc, ti(w[0x6c]), 0);
    }
    // ANISO (B = 131 in 3D, 133 in 2D)
    std::int32_t b = 0;
    if (!def::rt::dimension_is_3(owner(p))) { // 1400015f0
        b = 0x85;
    } else {
        b = 0x83;
    }
    if (ti(w[b]) == 0) {
        I(p, 0x790) = 0;
    } else if (ti(w[b]) == 1) {
        I(p, 0x790) = 1;
        model::dynary_reinit_keep(F(p, 0x792), 0x68); // 14012fc70
    } else if (ti(w[b]) == 2) {
        I(p, 0x790) = 2;
        model::dynary_reinit_keep(F(p, 0x792), 0x69);
    } else if (ti(w[b]) == 3) {
        I(p, 0x790) = 3;
        model::dynary_reinit_keep(F(p, 0x792), 0x67);
    } else if (ti(w[b]) == 4) {
        I(p, 0x790) = 4;
        model::dynary_reinit_keep(F(p, 0x792), 0x65);
    } else {
        I(p, 0x790) = 5;
        I(p, 0x7b0) = neg_ti(w[b]);
    }
    if (0 < I(p, 0x790)) {
        I(p, 0x7a2) = ti(w[b + 2]);
        const std::int32_t sub = ti(w[b + 1]);
        if (sub == 1) {
            I(p, 0x793) = 1;
            I(p, 0x796) = 3;
            I(p, 0x799) = ti(w[b + 4]);
        } else if (sub == 2) {
            I(p, 0x793) = 1;
            I(p, 0x796) = 7;
            I(p, 0x799) = ti(w[b + 3]);
        } else if (sub == 3) {
            I(p, 0x793) = 2;
            I(p, 0x796) = 7;
            I(p, 0x797) = 3;
            I(p, 0x799) = ti(w[b + 3]);
            I(p, 0x79a) = ti(w[b + 4]);
        } else if (sub == 4) {
            I(p, 0x793) = 2;
            I(p, 0x796) = 3;
            I(p, 0x797) = 0x1e;
            I(p, 0x799) = ti(w[b + 3]);
            I(p, 0x79a) = ti(w[b + 4]);
        }
        rd(F(p, 0x792), iifnc, llfnc, ti(w[b + 5]), 0);
    }
    D(p, 0x7b2) = w[10];   // FRAE2H
    D(p, 0x7b4) = w[0x2c]; // +0x1ed0 (no keyword)
    D(p, 0x7b8) = w[0x2b]; // TMPLIQ
    D(p, 0x7b6) = w[0x2a]; // TMPSOL
    D(p, 0x7ba) = w[1];    // MATDEN
    // FPERV A (w50 flag, w49) and B (w148 flag, w147)
    if (ti(w[0x32]) == 0) {
        I(p, 0x7bd) = 0;
        D(p, 0x7be) = w[0x31];
    } else {
        if (ti(w[0x32]) == 1) {
            I(p, 0x7bd) = 1;
            I(p, 0x7c0) = 1;
        }
        rd(F(p, 0x7bc), iifnc, llfnc, ti(w[0x31]), 0);
    }
    if (ti(w[0x94]) == 0) {
        I(p, 0x7db) = 0;
        D(p, 0x7dc) = w[0x93];
    } else {
        if (ti(w[0x94]) == 1) {
            I(p, 0x7db) = 1;
            I(p, 0x7de) = 1;
        }
        rd(F(p, 0x7da), iifnc, llfnc, ti(w[0x93]), 0);
    }
    // ELRST (w51, w52)
    if (ti(w[0x33]) == 0) {
        I(p, 0x737) = 0;
        D(p, 0x738) = w[0x34];
    } else {
        const std::int32_t t = ti(w[0x33]);
        if (t == 1) {
            I(p, 0x737) = 1;
            I(p, 0x73a) = 3;
        } else if (t == 2) {
            I(p, 0x737) = 1;
            I(p, 0x73a) = 9;
        } else {
            I(p, 0x737) = 1;
            I(p, 0x73a) = 3;
        }
        rd(F(p, 0x736), iifnc, llfnc, ti(w[0x34]), 0);
    }
    // PMEAB (w100 type, w101; n1, n2 at w137, w138 (3D) / w131, w132 (2D))
    if (ti(w[100]) == 0) {
        I(p, 0x755) = 0;
        D(p, 0x756) = w[0x65];
    } else {
        const std::int32_t t = ti(w[100]);
        if (t == 1) {
            I(p, 0x755) = 1;
            I(p, 0x758) = 3;
        } else if (t == 2) {
            I(p, 0x755) = 1;
            I(p, 0x758) = 9;
        } else if (t == 4) {
            I(p, 0x755) = 2;
            I(p, 0x758) = 3;
            I(p, 0x759) = 0x14;
            if (!is2d) {
                I(p, 0x75b) = ti(w[0x89]);
                I(p, 0x75c) = ti(w[0x8a]);
            } else {
                I(p, 0x75b) = ti(w[0x83]);
                I(p, 0x75c) = ti(w[0x84]);
            }
        }
        rd(F(p, 0x754), iifnc, llfnc, ti(w[0x65]), 0);
    }
    // PMITT (w102, w103)
    if (ti(w[0x66]) == 0) {
        I(p, 0x773) = 0;
        D(p, 0x774) = w[0x67];
    } else {
        const std::int32_t t = ti(w[0x66]);
        if (t == 1) {
            I(p, 0x773) = 1;
            I(p, 0x776) = 3;
        } else if (t == 2) {
            I(p, 0x773) = 1;
            I(p, 0x776) = 9;
        }
        rd(F(p, 0x772), iifnc, llfnc, ti(w[0x67]), 0);
    }
    if (10999 < version) {
        I(p, 0x854) = ti(w[0x42]); // SIZEMD
        I(p, 0x855) = ti(w[0x96]); // TXTURE
        I(p, 0x856) = ti(w[0x97]);
        I(p, 0x857) = ti(w[0x98]);
        dynary_read_selfdesc(F(p, 0x858), iifnc, llfnc, ti(w[0x99])); // DIFBND
        I(p, 0x876) = ti(w[0x9a]);                                    // SINTER
        D(p, 0x878) = w[0x9b];
        D(p, 0x87a) = w[0x9c];
        D(p, 0x87c) = w[0x9d];
        D(p, 0x87e) = w[0x9e];
        dynary_read_selfdesc(F(p, 0x5fc), iifnc, llfnc, ti(w[0x9f])); // GBENGY
        D(p, 0x61a) = w[0xa0];                                        // GBMOBI
        D(p, 0x61c) = w[0xa1];
        dynary_read_selfdesc(F(p, 0x61e), iifnc, llfnc, ti(w[0xa2])); // NUCSIZ
    }
}

// @src 14028a740  grain record: [0] n, [1..n] values, [23] real, [24] int, [25..35] sub-table
// descriptors, data from [37]
void grain_record_read(model::Cls_14028a4c0* r, const double* in)
{
    std::int32_t n = ti(in[0]);
    model::grndat_record_set_n(r, &n); // 14029ccd0
    for (std::int32_t i = 0; i < r->n; i = i + 1) { // 14029cd20
        r->values[i] = in[i + 1];
    }
    r->d = in[0x17];
    r->i = ti(in[0x18]);
    const double* data = in + 0x25;
    if (r->is_main == 0) { // 14029cd30
        grain_subtable_read(r, in + 0x19, in + 0x1c, in + 0x1d, data, &r->sub1);
        data = data + gendb::dynary_words(&r->sub1); // 140130290
        grain_subtable_read(r, in + 0x1a, in + 0x1f, in + 0x20, data, &r->sub2);
        data = data + gendb::dynary_words(&r->sub2);
    }
    grain_subtable_read(r, in + 0x1b, in + 0x22, in + 0x23, data, &r->sub3);
}

// @src 14028a950  code 0 const n1; 1 var 3; 2 var 0x17; 3 2-D (3, 0x17); then 140131e30
void grain_subtable_read(model::Cls_14028a4c0* /*r*/, const double* code, const double* n1, const double* n2,
                         const double* data, DYNARY* f)
{
    const std::int32_t c = ti(*code);
    if (c == 0) {
        f->ndim = 0;
        f->value = *n1;
    } else if (c == 1) {
        f->ndim = 1;
        f->var[0] = 3;
        f->n[0] = ti(*n1);
    } else if (c == 2) {
        f->ndim = 1;
        f->var[0] = 0x17;
        f->n[0] = ti(*n1);
    } else if (c == 3) {
        f->ndim = 2;
        f->var[0] = 3;
        f->var[1] = 0x17;
        f->n[0] = ti(*n1);
        f->n[1] = ti(*n2);
    }
    dynary_read_data(f, data);
}

// @src 14028b3b0  clear the list, then one record per entry of function `fid` (each entry is the
// function number of a grain record)
void grain_record_list_read(model::Cls_14028b640::RecordList* l, const std::int32_t* iifnc, const double* llfnc,
                            std::int32_t count, std::int32_t fid)
{
    const std::int32_t s = iifnc[fid * 2 - 2];
    model::grndat_record_list_clear(l); // 14028b240
    for (std::int32_t i = 0; i < count; i = i + 1) {
        model::Cls_14028a4c0* r = model::grndat_record_list_add(l); // 14028b050
        if (r != nullptr) {
            const std::int32_t k = ti(llfnc[(s - 1) + i]);
            grain_record_read(r, llfnc + (iifnc[k * 2 - 2] - 1));
        }
    }
}

// @src 14028c420
void frcmod_code11_read(void* frc, const double* in)
{
    model::field<double>(frc, 0x1b0) = in[0];
    model::field<double>(frc, 0x1b8) = in[1];
    model::field<double>(frc, 0x1d0) = in[2];
    model::field<double>(frc, 0x1d8) = in[3];
    model::field<double>(frc, 0x1e0) = in[4];
    model::field<double>(frc, 0x1e8) = in[5];
    model::field<double>(frc, 0x1f0) = in[6];
    model::field<double>(frc, 0x1c0) = in[7];
    model::field<double>(frc, 0x1c8) = in[8];
}

} // namespace def::dbio::dbread
