// p4-02b: material counts (MSTMTR marking and renumbering, the property-function counts of one
// material) and the DYNARY DB lengths they use.
//
// A property function is a DYNARY; "c1" adds one function of length 140130200 (1D table), "c2"
// one of length 140130160 (2D table), usually after the table is prepared by 140134120. The
// field offsets are those of the decompile (Material layout: defmodel/layout/material.hpp).
#include <defdbio/gendb/counts_frame.hpp>

#include <cstdlib>
#include <intrin.h>

#include <defdbio/gendb/counts_ext.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defmodel/step/db_step.hpp>

namespace def::dbio::gendb {

using model::field;

namespace {

DYNARY* tab(void* base, std::size_t off) { return reinterpret_cast<DYNARY*>(static_cast<char*>(base) + off); }

// ndim 1: one function, 140130200
void c1(DYNARY* t, std::int32_t* n, std::int32_t* len)
{
    *n = *n + 1;
    *len = gendb_ext().fun_140130200(t) + *len;
}

// ndim 2: 140134120, then one function of 140130160 + extra
void c2(DYNARY* t, std::int32_t* n, std::int32_t* len, std::int32_t extra)
{
    gendb_ext().fun_140134120(t);
    std::int32_t v = gendb_ext().fun_140130160(t);
    *n = *n + 1;
    *len = *len + v + extra;
}

// the "ndim 1 or 2" pattern used by most material functions
void c12(DYNARY* t, std::int32_t* n, std::int32_t* len)
{
    if (t->ndim == 1) {
        c1(t, n, len);
    } else if (t->ndim == 2) {
        c2(t, n, len, 0);
    }
}

bool used(Material* m) { return field<std::int32_t>(m, 4) != 0; } // 1401b0620: +4 != 0

} // namespace

// @src 140130090  DB length of a 3D table with components: n0 + n1 + n2 + product of max(.,1)
std::int32_t dynary_length_3d_comp(const DYNARY* t)
{
    std::int32_t a = t->n[0] < 1 ? 1 : t->n[0];
    std::int32_t b = t->n[1] < 1 ? 1 : t->n[1];
    std::int32_t c = t->n[2] < 1 ? 1 : t->n[2];
    std::int32_t d = t->ncomp < 1 ? 1 : t->ncomp;
    return t->n[0] + t->n[1] + t->n[2] + a * b * c * d;
}

// @src 140130380  DB length of a table by ndim (0: 0; 1: 140130230; 2: 140130160; 3: 14012fff0) + 10
std::int32_t dynary_length_plus10(const DYNARY* t)
{
    std::int32_t r = 0;
    if (t->ndim == 0) {
        r = 0;
    } else if (t->ndim == 1) {
        r = gendb_ext().fun_140130230(t);
    } else if (t->ndim == 2) {
        r = gendb_ext().fun_140130160(t);
    } else if (t->ndim == 3) {
        r = gendb_ext().fun_14012fff0(t);
    }
    return r + 10;
}

// @src 1401304e0  one function of length 140130380
void dynary_count_plus10(const DYNARY* t, std::int32_t* n, std::int32_t* len)
{
    *n = *n + 1;
    *len = dynary_length_plus10(t) + *len;
}

// @src 14029b5c0  clear the used flag (+4) and the group flags (+8) of every material (14028d030)
void material_list_clear_flags(MaterialList* l)
{
    for (std::int32_t i = 0; i < l->count; i = i + 1) { // 140199c50: count at +0x10
        Material* m = model::material_list_nth(l, i + 1);
        if (m != nullptr) {
            field<std::int32_t>(m, 4) = 0;
            field<std::int32_t>(m, 8) = 0;
        }
    }
}

// @src 14029b8e0  function counts of the used materials (14028e4b0)
void material_list_function_counts(MaterialList* l, std::int32_t* n, std::int32_t* len,
                                   std::int32_t* err)
{
    for (std::int32_t i = 0; i < l->count; i = i + 1) {
        Material* m = model::material_list_nth(l, i + 1);
        if (m != nullptr && used(m)) {
            material_function_counts(m, n, len, err);
        }
    }
}

// @src 14029c410  largest material number
std::int32_t material_list_max_number(MaterialList* l)
{
    std::int32_t mx = 0;
    for (std::int32_t i = 0; i < l->count; i = i + 1) {
        Material* m = model::material_list_nth(l, i + 1);
        if (m != nullptr && mx < m->number) {
            mx = m->number;
        }
    }
    return mx;
}

// @src 14029c490  number of mixture materials (MSTMTR n > 0); each component material gets the
// mixture's used flag (as 0/1) and its group flags
std::int32_t material_list_mark_mixtures(MaterialList* l)
{
    std::int32_t cnt = 0;
    for (std::int32_t i = 0; i < l->count; i = i + 1) {
        Material* m = model::material_list_nth(l, i + 1);
        if (m == nullptr || m->mstmtr_count <= 0) {
            continue;
        }
        cnt = cnt + 1;
        for (std::int32_t k = 0; k < m->mstmtr_count; k = k + 1) {
            Material* p = model::material_list_find(l, m->mstmtr_list[k]);
            if (p != nullptr) {
                field<std::int32_t>(p, 4) = used(m) ? 1 : 0;
                field<std::int32_t>(p, 8) = field<std::int32_t>(m, 8);
            }
        }
    }
    return cnt;
}

// @src 14029c5c0  renumber the used materials 1..k in material-number order (into +4); returns k.
// The index array is never freed by the original (kept as is).
std::int32_t material_list_renumber(MaterialList* l)
{
    if (l->count < 1) {
        return 0;
    }
    std::int32_t nmax = material_list_max_number(l);
    unsigned __int64 hi = 0;
    unsigned __int64 sz = _umul128(static_cast<unsigned __int64>(static_cast<std::int64_t>(nmax)), 4, &hi);
    auto* idx = static_cast<std::int32_t*>(std::malloc(hi != 0 ? ~0ull : sz)); // thunk 1404797d8
    for (std::int32_t i = 0; i < nmax; i = i + 1) {
        idx[i] = 0;
    }
    for (std::int32_t i = 0; i < l->count; i = i + 1) {
        Material* m = model::material_list_nth(l, i + 1);
        if (m != nullptr && used(m)) {
            idx[m->number - 1] = 1;
        }
    }
    std::int32_t k = 0;
    for (std::int32_t i = 0; i < nmax; i = i + 1) {
        if (0 < idx[i]) {
            k = k + 1;
            idx[i] = k;
        }
    }
    for (std::int32_t i = 0; i < l->count; i = i + 1) {
        Material* m = model::material_list_nth(l, i + 1);
        if (m != nullptr && used(m)) {
            field<std::int32_t>(m, 4) = idx[m->number - 1];
        }
    }
    return k;
}

// @src 14028e4b0  property-function counts of one material
void material_function_counts(Material* mat, std::int32_t* n, std::int32_t* len, std::int32_t*)
{
    flow_stress_counts(reinterpret_cast<char*>(mat) + 0x50, n, len);
    c12(tab(mat, 0x670), n, len); // YOUNG
    c12(tab(mat, 0x6e8), n, len); // POISON
    c12(tab(mat, 0x760), n, len); // EXPAND
    c12(tab(mat, 0x7e0), n, len); // THRCND
    c12(tab(mat, 0x858), n, len); // HEATCP
    if (tab(mat, 0x1fe0)->ndim == 1) {
        c1(tab(mat, 0x1fe0), n, len);
    }
    if (0 < field<std::int32_t>(mat, 0x20d0)) {
        if (field<std::int32_t>(mat, 0x20dc) == 0) {
            *n = *n + 1;
            *len = *len + 0x14;
        } else {
            *n = *n + 2;
            *len = gendb_ext().fun_140130290(tab(mat, 0x20d8)) + *len;
            *len = *len + 0x14;
        }
    }
    if (tab(mat, 0x8d8)->ndim == 1) { // EMSVTY
        c1(tab(mat, 0x8d8), n, len);
    }
    *n = *n + 1;
    *len = *len + dynary_length_plus10(tab(mat, 0x2160));
    diffusion_counts(&field<std::int32_t>(mat, 0x950), n, len);
    creep_counts(&field<std::int32_t>(mat, 0xac8), n, len);
    c12(tab(mat, 0xf10), n, len); // HDNPHA
    if (field<std::int32_t>(mat, 0xfa4) != 0) { // HDNTIM n[0]
        *n = *n + 1;
        *len = gendb_ext().fun_140130200(tab(mat, 0xf88)) + *len;
    }
    if (0 < field<std::int32_t>(mat, 0x101c) && 0 < field<std::int32_t>(mat, 0x1020)) { // JOMINY
        c2(tab(mat, 0x1000), n, len, 0);
    }
    grain_counts(reinterpret_cast<char*>(mat) + 0x1078, n, len);
    elastic_counts(reinterpret_cast<char*>(mat) + 0x18f0, n, len);
    material_misc_counts(&field<std::int32_t>(mat, 0x1ad0), n, len);
    if (0 < mat->mstmtr_count) {
        *n = *n + 1;
        *len = *len + mat->mstmtr_count;
    }
    if (tab(mat, 0x1cd8)->ndim == 1) {
        c1(tab(mat, 0x1cd8), n, len);
    }
    c12(tab(mat, 0x1d50), n, len);
    if (tab(mat, 0x1dc8)->ndim == 1) {
        c1(tab(mat, 0x1dc8), n, len);
    }
    hardness_counts(&field<std::int32_t>(mat, 0x1e40), n, len);
    if (tab(mat, 0x1ef0)->ndim == 1) {
        c1(tab(mat, 0x1ef0), n, len);
    }
    if (tab(mat, 0x1f68)->ndim == 1) {
        c1(tab(mat, 0x1f68), n, len);
    }
    *n = *n + 1;
    *len = *len + dynary_length_plus10(tab(mat, 0x17f0));
    *n = *n + 1;
    *len = *len + dynary_length_plus10(tab(mat, 0x1878));
}

// @src 1402894b0  flow-stress functions (Material+0x50, by the FSTRES form in the first int)
void flow_stress_counts(void* fs, std::int32_t* n, std::int32_t* len)
{
    auto* p = static_cast<std::int32_t*>(fs);
    std::int32_t v;
    switch (p[0]) {
    case 1:
        gendb_ext().fun_140134120(tab(fs, 0x28));
        v = gendb_ext().fun_14012fff0(tab(fs, 0x28));
        *n = *n + 1;
        *len = *len + v;
        break;
    case 4:
        c12(tab(fs, 0x160), n, len);
        c12(tab(fs, 0xe8), n, len);
        break;
    case 5:
        gendb_ext().fun_140134120(tab(fs, 0x1d8));
        v = gendb_ext().fun_14012fff0(tab(fs, 0x1d8));
        *n = *n + 1;
        *len = *len + v;
        break;
    case 9:
        c12(tab(fs, 0x3b0), n, len);
        break;
    case 10:
    case 0xc:
        *n = *n + 3;
        *len = *len + gendb_ext().fun_140130230(tab(fs, 0x430));
        *len = *len + gendb_ext().fun_140130290(tab(fs, 0x4a8));
        *len = *len + gendb_ext().fun_140130290(tab(fs, 0x520));
        break;
    case 0xb:
        if (p[0xcf] != 0) {
            *n = *n + 1;
            *len = *len + dynary_length_plus10(tab(fs, 0x338));
        }
        break;
    case 0xd:
        v = dynary_length_3d_comp(tab(fs, 0x598));
        *n = *n + 1;
        *len = *len + v;
        break;
    default:
        break;
    }
}

// @src 140289a40  diffusion function (Material+0x950): table at +8 / +0x80 / +0xf8 for forms 1..3
void diffusion_counts(std::int32_t* d, std::int32_t* n, std::int32_t* len)
{
    std::int32_t* t = nullptr;
    std::int32_t v = 0;
    if (d[0] == 1) {
        t = d + 2;
    } else if (d[0] == 2) {
        t = d + 0x20;
    } else if (d[0] == 3) {
        t = d + 0x3e;
    }
    if (d[0] == 1) {
        gendb_ext().fun_140134120(reinterpret_cast<DYNARY*>(t));
        v = gendb_ext().fun_140130160(reinterpret_cast<DYNARY*>(t));
    } else if (1 < d[0] && d[0] < 4) {
        v = t[7] * (t[0x10] + 1); // n[0] * (ncomp + 1), even
        if (v % 2 == 1) {
            v = v + 1;
        }
    }
    if (d[0] != 0) {
        *n = *n + 1;
        *len = *len + v;
    }
}

// @src 140289d80  creep functions (Material+0xac8)
void creep_counts(std::int32_t* c, std::int32_t* n, std::int32_t* len)
{
    std::int32_t* t = nullptr;
    std::int32_t v = 0;
    switch (c[0]) {
    case 1: t = c + 0x3e; break;
    case 2: t = c + 0x5c; break;
    case 3: t = c + 0x7a; break;
    case 4: t = c + 0x98; break;
    case 5: t = c + 0x20; break;
    case 6: t = c + 2; break;
    default: break;
    }
    switch (c[0]) {
    case 1:
    case 2:
    case 3:
    case 4:
        v = gendb_ext().fun_140130230(reinterpret_cast<DYNARY*>(t));
        break;
    case 5:
    case 6:
        gendb_ext().fun_140134120(reinterpret_cast<DYNARY*>(t));
        v = gendb_ext().fun_14012fff0(reinterpret_cast<DYNARY*>(t));
        break;
    case 7:
    case 8:
        *n = *n + 3;
        *len = *len + gendb_ext().fun_140130230(reinterpret_cast<DYNARY*>(c + 0xb8));
        *len = *len + gendb_ext().fun_140130290(reinterpret_cast<DYNARY*>(c + 0xd6));
        v = gendb_ext().fun_140130290(reinterpret_cast<DYNARY*>(c + 0xf4));
        *len = *len + v;
        break;
    default:
        break;
    }
    if (c[0] != 0 && c[0] != 7 && c[0] != 8) {
        *n = *n + 1;
        *len = *len + v;
    }
}

// @src 14028a080  hardness function (Material+0x1e40)
void hardness_counts(std::int32_t* h, std::int32_t* n, std::int32_t* len)
{
    if (h[0] == 0) {
        return;
    }
    if (h[3] == 1) {
        *n = *n + 1;
        *len = h[9] * h[0x12] + h[9] + *len;
    } else if (h[3] == 2) {
        *n = *n + 1;
        *len = h[0x12] * h[9] * h[10] + h[9] + h[10] + *len;
    }
}

// @src 14028a650  DB length of one grain record: 0x25 (+ two tables unless +0x1e0 is set) + 1
std::int32_t grain_record_length(void* g)
{
    std::int32_t r = 0x25;
    if (field<std::uint8_t>(g, 0x1e0) == 0) { // 14029cd30
        std::int32_t a = gendb_ext().fun_140130290(tab(g, 0x78));
        r = gendb_ext().fun_140130290(tab(g, 0xf0));
        r = a + 0x25 + r;
    }
    return r + gendb_ext().fun_140130290(tab(g, 0x168));
}

// @src 14028a6f0  one function of length 14028a650
void grain_record_counts(void* g, std::int32_t* n, std::int32_t* len)
{
    std::int32_t v = grain_record_length(g);
    *n = *n + 1;
    *len = *len + v;
}

// @src 14028b2f0  each record of a grain list, then one function holding the record count
void grain_list_counts(void* l, std::int32_t* n, std::int32_t* len)
{
    std::int32_t cnt = field<std::int32_t>(l, 0x10); // 14029cd50
    for (std::int32_t i = 0; i < cnt; i = i + 1) {
        void* r = gendb_ext().fun_14028aff0(l, i + 1);
        if (r != nullptr) {
            grain_record_counts(r, n, len);
        }
    }
    if (0 < cnt) {
        *n = *n + 1;
        *len = *len + cnt;
    }
}

// @src 14028b990  DB length of the grain tables (mode 1: nine 1D tables, n0 * (ncomp + 1) each;
// mode 4: 140130290 of the table at +0x6c8)
std::int32_t grain_table_length(void* g)
{
    std::int32_t r = 0;
    std::int32_t mode = field<std::int32_t>(g, 0x770);
    if (mode == 0) {
        r = 0;
    } else if (mode == 1) {
        auto f = [g](std::size_t a, std::size_t b) {
            return field<std::int32_t>(g, a) * (field<std::int32_t>(g, b) + 1);
        };
        r = f(0x2c, 0x50) + f(0xa4, 0xc8) + f(0x11c, 0x140) + f(0x194, 0x1b8) + f(0x20c, 0x230) +
            f(0x284, 0x2a8) + f(0x2fc, 0x320) + f(0x374, 0x398) + f(0x3ec, 0x410);
    } else if (mode == 4) {
        r = gendb_ext().fun_140130290(tab(g, 0x6c8));
    }
    return r;
}

// @src 14028bb60  grain functions (Material+0x1078) by mode (+0x770)
void grain_counts(void* g, std::int32_t* n, std::int32_t* len)
{
    std::int32_t mode = field<std::int32_t>(g, 0x770);
    if (mode == 0) {
        return;
    }
    if (mode == 1) {
        std::int32_t v = grain_table_length(g);
        *n = *n + 1;
        *len = *len + v;
    } else if (mode == 2) {
        char* b = static_cast<char*>(g);
        grain_record_counts(b + 0x460, n, len);
        grain_list_counts(b + 0x680, n, len);
        grain_list_counts(b + 0x668, n, len);
        grain_list_counts(b + 0x650, n, len);
        grain_list_counts(b + 0x698, n, len);
        grain_list_counts(b + 0x6b0, n, len);
    } else if (mode == 4) {
        gendb_ext().fun_140130410(tab(g, 0x6c8), n, len);
    }
}

// @src 14028bd80  four tables at +0, +0x78, +0xf0, +0x168 (Material+0x18f0); the first also takes
// a 3D form, and the 2D/3D forms add 2/3
void elastic_counts(void* e, std::int32_t* n, std::int32_t* len)
{
    DYNARY* t0 = tab(e, 0);
    if (t0->ndim == 1) {
        c1(t0, n, len);
    } else if (t0->ndim == 2) {
        c2(t0, n, len, 2);
    } else if (t0->ndim == 3) {
        gendb_ext().fun_140134120(t0);
        std::int32_t v = gendb_ext().fun_14012fff0(t0);
        *n = *n + 1;
        *len = *len + v + 3;
    }
    for (std::size_t off : {std::size_t{0x78}, std::size_t{0xf0}, std::size_t{0x168}}) {
        DYNARY* t = tab(e, off);
        if (t->ndim == 1) {
            c1(t, n, len);
        } else if (t->ndim == 2) {
            c2(t, n, len, 2);
        }
    }
}

// @src 14028c240  table at +0x40 (1D), then by the kind in the first int: 0xc fixed 9, 0xd the 1D
// table at +0xc0, 0x11 the table at +0x138 (Material+0x1ad0)
void material_misc_counts(std::int32_t* p, std::int32_t* n, std::int32_t* len)
{
    if (p[0x11] == 1) {
        *n = *n + 1;
        *len = gendb_ext().fun_140130200(reinterpret_cast<DYNARY*>(p + 0x10)) + *len;
    }
    if (p[0] == 0xc) {
        *n = *n + 1;
        *len = *len + 9;
    } else if (p[0] == 0xd) {
        *n = *n + 1;
        *len = gendb_ext().fun_140130200(reinterpret_cast<DYNARY*>(p + 0x30)) + *len;
    } else if (p[0] == 0x11) {
        *n = *n + 1;
        *len = gendb_ext().fun_140130290(reinterpret_cast<DYNARY*>(p + 0x4e)) + *len;
    }
}

} // namespace def::dbio::gendb
