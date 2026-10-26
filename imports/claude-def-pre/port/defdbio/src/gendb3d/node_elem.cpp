// gendb3d/node_elem.cpp: per-object node, element and element-face arrays of GENDB (packet p4-05).
// Field tables: docs/re/dbmap/objarrays.md sections 2.1-2.5 (branch claude/project-thread-bz5k5e).
//
// All index arithmetic is 32-bit as in the binary: start(R) - 1 + offset * width is an int, then
// sign-extended; memcpy sizes are (int64)(count * width) << 3 (or << 2) with the product in int.
#define _CRT_SECURE_NO_WARNINGS
#include <defdbio/gendb3d/obj3d.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/cls_140182880.hpp>
#include <defmodel/llobj/dynary_table.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/stdlib/refcount.hpp>

#include "gendb3d/access.hpp"
#include "gendb3d/ext_obj3d.hpp"

namespace def::dbio::g3d {

namespace {

using model::Cls_140182880;

// W[id]: DB_STEP_DATA + 0x15f8 + 4 * id (state-variable width table, 1401d09c0)
inline std::int32_t w(const DB_STEP_DATA* m, std::int32_t id) { return m->u15f8[id]; }

inline void copy_d(double* dst, const void* src, std::int32_t count)
{
    std::memcpy(dst, src, static_cast<std::size_t>(static_cast<std::int64_t>(count) << 3));
}
inline void copy_i(std::int32_t* dst, const void* src, std::int32_t count)
{
    std::memcpy(dst, src, static_cast<std::size_t>(static_cast<std::int64_t>(count) << 2));
}

// 140158e90 + 1401c7d40: select the index tables of the owner's dimension
inline void select_tables(LLOBJ_DATA* obj)
{
    std::int32_t dim = model::llobj::int_at_1c(owner_of(obj));
    model::select_index_tables(&dim);
}

inline Cls_140182880* elem_set(LLOBJ_DATA* obj) { return reinterpret_cast<Cls_140182880*>(at(obj, 0x9e0)); }

// fopen("DEF_PRE.ERR", "at") (14047b934 = _fsopen(.., 0x40)); mode strings 14051fd44 / 14051fe04 / 14051fe64
void report_width(const char* fmt, std::int32_t a, std::int32_t b)
{
    std::FILE* f = std::fopen("DEF_PRE.ERR", "at");
    rt::crt::fprintf(f, fmt, a, b);
    std::fclose(f);
}

} // namespace

// 140183cb0 (owner: defcmd element_set_conn, contact_cmds.cpp, used by p67-2): the same read here,
// because defdbio cannot call into defcmd; region sync 1 through the owner (1401d5820), return +8
static std::int32_t* elem_connectivity(Cls_140182880* s)
{
    if (s->u0f0 != nullptr) {
        std::int32_t what = 1;
        ext::f1401d5820(s->u0f0, &what);
    }
    return s->u008;
}

// @src 140183d00  element set: region sync 2, return +0x18 (material group per element). The one
// owner body: defcmd (objpos 2D surface, p8-08, meshgen, CNVT3D sweep, STEP.DAT) forwards here
std::int32_t* element_material_groups(void* elems)
{
    auto* s = static_cast<Cls_140182880*>(elems);
    if (s->u0f0 != nullptr) {
        std::int32_t what = 2;
        ext::f1401d5820(s->u0f0, &what);
    }
    return s->u018;
}

// @src 140165230  max(NBF 140165170, nodes +0x880) + 1000
std::int32_t bc_function_capacity(LLOBJ_DATA* obj)
{
    std::int32_t n = ext::f140165170(obj);
    if (n < fld<std::int32_t>(obj, 0x880)) {
        n = fld<std::int32_t>(obj, 0x880);
    }
    return n + 1000;
}

// @src 14016f8e0  WriteNodeInfoToDB: node fields (obj + 0x8a8..0x968) at rows node_off..
void write_node_info(LLOBJ_DATA* obj, std::int32_t* /*mar*/, double* a, const std::int32_t* /*mar_tab*/,
                     const std::int32_t* a_tab, const std::int32_t* node_off)
{
    select_tables(obj);
    DB_STEP_DATA* m = owner_of(obj);
    const std::int32_t nnp = numnp(owner_of(obj));
    const std::int32_t nel = numel(owner_of(obj));
    const std::int32_t nn = fld<std::int32_t>(obj, 0x880);
    const std::int32_t off = *node_off;
    auto put = [&](std::uint64_t dat, std::int32_t id, std::ptrdiff_t member) {
        copy_d(a + (a_tab[a_ix(dat)] - 1 + off * w(m, id)), fld<double*>(obj, member), nn * w(m, id));
    };
    put(0x1406e82fc, 0x00, 0x8a8);                   // RZ    LLCRZ
    put(0x1406e8300, 0x01, 0x8b0);                   // DRZ   LLDRZ
    put(0x1406e8304, 0x02, 0x8b8);                   // URZ   LLURZ
    put(0x1406e8308, 0x03, 0x8c0);                   // FRZ   LLPFN
    if (rt::dimension_is_3(owner_of(obj))) {
        put(0x1406e8310, 0x05, 0x8d0);               // PRZ   LLPPR
    }
    put(0x1406e8318, 0x08, 0x8e0);                   // NDTMP LLTMP
    if (rt::dimension_is_2(owner_of(obj))) {
        put(0x1406e839c, 0x0c, 0x900);               // *DTMP LLDTM
    }
    put(0x1406e831c, 0x09, 0x8e8);                   // NDHEAT LLPHT
    put(0x1406e8324, 0x0b, 0x8f8);                   // NDFLUX LLFLX component 1
    // RHTFLX / CRBFLX: LLFLX components 2 and 3 (+NUMNP, +2 NUMNP), both with width W[0x0b]
    copy_d(a + (off * w(m, 0x0b) - 1 + nnp + a_tab[a_ix(0x1406e8324)]), fld<double*>(obj, 0x920),
           nn * w(m, 0x0b));
    copy_d(a + (a_tab[a_ix(0x1406e8324)] - 1 + off * w(m, 0x0b) + nnp * 2), fld<double*>(obj, 0x910),
           nn * w(m, 0x0b));
    if (fld<double*>(obj, 0x968) != nullptr) {
        put(0x1406e8328, 0x11, 0x968);               // USRNOD LLND1
    }
    put(0x1406e8330, 0x0d, 0x908);                   // DATOM  LLATM
    put(0x1406e832c, 0x0f, 0x918);                   // VOTAGE LLVLT
    if (fld<double*>(obj, 0x928) != nullptr && 0 < a_tab[a_ix(0x1406e8420)]) {
        put(0x1406e8334, 0x18, 0x928);               // NWEAR  LLWER (table width > 0)
    }
    if (rt::dimension_is_3(owner_of(obj))) {
        // DRMESH: literal A slot 0x13 (a_tab + 0x4c)
        copy_d(a + (a_tab[0x13] - 1 + off * w(m, 0x19)), fld<double*>(obj, 0x930), nn * w(m, 0x19));
    }
    // ELMNOD option 2: nodal blocks after the NUMEL element rows of the same region
    if (fld<std::int32_t>(owner_of(obj), 0x890) == 2) {
        copy_d(a + (a_tab[a_ix(0x1406e833c)] - 1 + nel * w(m, 0x26) + off * w(m, 0x1a)),
               fld<double*>(obj, 0x948), nn * w(m, 0x1a)); // STRESS nodes
    }
    if (fld<std::int32_t>(owner_of(obj), 0x88c) == 2) {
        copy_d(a + (a_tab[a_ix(0x1406e8350)] - 1 + nel * w(m, 0x2b) + off * w(m, 0x1b)),
               fld<double*>(obj, 0x938), nn * w(m, 0x1b)); // STRAIN nodes
    }
    if (fld<std::int32_t>(owner_of(obj), 0x888) == 2) {
        copy_d(a + (a_tab[a_ix(0x1406e835c)] - 1 + nel * w(m, 0x2d) + off * w(m, 0x1c)),
               fld<double*>(obj, 0x940), nn * w(m, 0x1c)); // DAMAGE nodes
    }
    if (fld<std::int32_t>(owner_of(obj), 0x898) == 1) {
        put(0x1406e8378, 0x1d, 0x950);               // GRAIN (nodal) LLGSZ
    }
    if (0 < w(m, 0x22) && fld<double*>(obj, 0x958) != nullptr) {
        put(0x1406e83ac, 0x22, 0x958);               // TIMERC LLTRC
    }
    if (0 < w(m, 0x23) && fld<double*>(obj, 0x960) != nullptr) {
        put(0x1406e83b4, 0x23, 0x960);               // NRECVL LLNRV
    }
}

// @src 1401707e0  WriteNodeIntegerInfoToDB: BCCDEF/BCCDFN/BCCTMP/BCCTFN/BCCRHT/BCCCRB; function
//                 references above the capacity are zeroed, used ones flagged (fn_used / sym_used)
void write_node_int_info(LLOBJ_DATA* obj, std::int32_t* mar, const std::int32_t* mar_tab,
                         const std::int32_t* node_off, std::int32_t* fn_used, std::int32_t* sym_used,
                         const std::int32_t* fn_cap, const std::int32_t* sym_cap,
                         std::uint32_t* fn_kinds, std::int32_t* sym_any)
{
    select_tables(obj);
    DB_STEP_DATA* m = owner_of(obj);
    const std::int32_t nnp = numnp(owner_of(obj));
    const std::int32_t nn = fld<std::int32_t>(obj, 0x880);
    copy_i(mar + (mar_tab[mar_ix(0x1406e8218)] - 1 + *node_off * w(m, 0x12)), fld<std::int32_t*>(obj, 0x990),
           nn * w(m, 0x12)); // BCCDEF IIBCD

    std::int32_t* bcdef = fld<std::int32_t*>(obj, 0x990);
    std::int32_t* bcdfn = fld<std::int32_t*>(obj, 0x998);
    std::int32_t cnt = nn * w(m, 0x13);
    std::int32_t start = mar_tab[mar_ix(0x1406e821c)];
    std::int32_t off = *node_off;
    std::int32_t wid = w(m, 0x13);
    if (!rt::dimension_is_3(owner_of(obj))) {
        if (rt::dimension_is_2(owner_of(obj))) {
            for (std::int32_t k = 0; k < cnt; k = k + 1) {
                if (0 < bcdfn[k]) {
                    if (*fn_cap < bcdfn[k]) {
                        bcdfn[k] = 0;
                    }
                    else {
                        fn_used[bcdfn[k] - 1] = 1;
                        *fn_kinds = *fn_kinds | 1;
                    }
                }
            }
        }
    }
    else {
        for (std::int32_t k = 0; k < cnt; k = k + 1) {
            if (bcdef[k] == 6 && 0 < bcdfn[k]) { // symmetry plane
                if (*sym_cap < bcdfn[k]) {
                    bcdfn[k] = 0;
                }
                else {
                    sym_used[bcdfn[k] - 1] = 1;
                    *sym_any = 1;
                }
            }
            else if (0 < bcdfn[k]) {
                if (*fn_cap < bcdfn[k]) {
                    bcdfn[k] = 0;
                }
                else {
                    fn_used[bcdfn[k] - 1] = 1;
                    *fn_kinds = *fn_kinds | 1;
                }
            }
        }
    }
    copy_i(mar + (start - 1 + off * wid), bcdfn, cnt); // BCCDFN IIBFD

    // BCCTMP: IIBCT component 1, negative -> 2
    cnt = nn * w(m, 0x14);
    std::int32_t* dst = mar + (mar_tab[mar_ix(0x1406e8220)] - 1 + *node_off * w(m, 0x14));
    copy_i(dst, fld<std::int32_t*>(obj, 0x9a0), cnt);
    for (std::int32_t k = 0; k < cnt; k = k + 1) {
        if (dst[k] < 0) dst[k] = 2;
    }

    // BCCTFN IIBFT
    cnt = nn * w(m, 0x15);
    start = mar_tab[mar_ix(0x1406e8224)];
    off = *node_off;
    wid = w(m, 0x15);
    std::int32_t* bctfn = fld<std::int32_t*>(obj, 0x9a8);
    for (std::int32_t k = 0; k < cnt; k = k + 1) {
        if (0 < bctfn[k]) {
            if (*fn_cap < bctfn[k]) {
                bctfn[k] = 0;
            }
            else {
                fn_used[bctfn[k] - 1] = 1;
                *fn_kinds = *fn_kinds | 0x10;
            }
        }
    }
    copy_i(mar + (start - 1 + off * wid), bctfn, cnt);

    // BCCRHT: IIBCT component 2 (+NUMNP), negative -> 2
    cnt = nn * w(m, 0x17);
    dst = mar + (*node_off * w(m, 0x17) - 1 + nnp + mar_tab[mar_ix(0x1406e8220)]);
    copy_i(dst, fld<std::int32_t*>(obj, 0x9b8), cnt);
    for (std::int32_t k = 0; k < cnt; k = k + 1) {
        if (dst[k] < 0) dst[k] = 2;
    }

    // BCCCRB: IIBCT component 3 (+2 NUMNP), negative -> 2
    cnt = nn * w(m, 0x16);
    dst = mar + (mar_tab[mar_ix(0x1406e8220)] - 1 + *node_off * w(m, 0x16) + nnp * 2);
    copy_i(dst, fld<std::int32_t*>(obj, 0x9b0), cnt);
    for (std::int32_t k = 0; k < cnt; k = k + 1) {
        if (dst[k] < 0) dst[k] = 2;
    }
}

// @src 140160640  VOLFC rows of the object (obj + 0xa58, width *wd): negatives -> 0, row / sum,
//                 or all 0 when the sum is not above 1e-8 (DAT_14051bed0; NaN -> 0)
void normalize_volume_fractions(LLOBJ_DATA* obj, const std::int32_t* wd)
{
    const std::int32_t n = elem_set(obj)->count(); // 1401854f0
    for (std::int32_t e = 0; e < n; e = e + 1) {
        double* row = fld<double*>(obj, 0xa58) + e * *wd;
        double sum = 0.0;
        for (std::int32_t k = 0; k < *wd; k = k + 1) {
            if (0.0 > row[k]) row[k] = 0.0;
            sum = sum + row[k];
        }
        if (sum > 1e-8) {
            for (std::int32_t k = 0; k < *wd; k = k + 1) row[k] = row[k] / sum;
        }
        else {
            for (std::int32_t k = 0; k < *wd; k = k + 1) row[k] = 0.0;
        }
    }
}

// @src 1401607f0  the same over rows[0..*wd) once per element: the binary never advances the row,
//                 so only the first row is normalised (repeatedly)
void normalize_volume_fraction_rows(LLOBJ_DATA* obj, double* rows, const std::int32_t* wd)
{
    const std::int32_t n = elem_set(obj)->count(); // 1401854f0
    for (std::int32_t e = 0; e < n; e = e + 1) {
        double sum = 0.0;
        for (std::int32_t k = 0; k < *wd; k = k + 1) {
            if (0.0 > rows[k]) rows[k] = 0.0;
            sum = sum + rows[k];
        }
        if (sum > 1e-8) {
            for (std::int32_t k = 0; k < *wd; k = k + 1) rows[k] = rows[k] / sum;
        }
        else {
            for (std::int32_t k = 0; k < *wd; k = k + 1) rows[k] = 0.0;
        }
    }
}

// @src 140160980  copy the columns of the used materials (src width *wd, per material 14029c870)
//                 into dst rows of width = number of used materials; 1 on success
std::uint64_t remap_volume_fractions(LLOBJ_DATA* obj, const double* src, double* dst, const std::int32_t* wd)
{
    DB_STEP_DATA* m = owner_of(obj);
    std::int32_t nmax = ext::f14029c410(&m->materials);
    if (nmax == 0) return 0;
    if (*wd != m->n_materials) return 0;
    auto* pos = static_cast<std::int32_t*>(std::malloc(static_cast<std::size_t>(nmax) * 4));
    const std::int32_t used = material_positions(&m->materials, pos, &nmax);
    if (used == ext::f140199c10(m)) {
        for (std::int32_t k = 0; k < nmax; k = k + 1) {
            const std::int32_t col = ext::f14029c870(&m->materials, k + 1);
            std::int32_t p = pos[k];
            if (p != 0 && (p = p - 1, p < used)) {
                for (std::int32_t e = 0; e < fld<std::int32_t>(obj, 0x884); e = e + 1) {
                    dst[e * used + p] = src[e * *wd + col];
                }
            }
        }
        std::free(pos);
        return 1;
    }
    std::free(pos);
    return 0;
}

// @src 140172c90  WriteElemInfoToDB: element fields (obj + 0xa00..0xab8) at rows elem_off..
void write_elem_info(LLOBJ_DATA* obj, std::int32_t* /*mar*/, double* a, const std::int32_t* /*mar_tab*/,
                     const std::int32_t* a_tab, const std::int32_t* elem_off,
                     const std::int32_t* /*node_off*/)
{
    select_tables(obj);
    DB_STEP_DATA* m = owner_of(obj);
    numel(owner_of(obj));
    const std::int32_t ne = fld<std::int32_t>(obj, 0x884);

    // STRESS (p integration points; 8 -> replicate a 1-point field to the 8 points)
    std::int32_t p = elem_set(obj)->fun_140183c20(2);
    if (a_tab[a_ix(0x1406e8428)] == p * w(m, 0x26)) {
        copy_d(a + (a_tab[a_ix(0x1406e833c)] - 1 + *elem_off * a_tab[a_ix(0x1406e8428)]),
               fld<double*>(obj, 0xa00), p * ne * w(m, 0x26));
    }
    else {
        const std::int32_t start = a_tab[a_ix(0x1406e833c)];
        const std::int32_t off = *elem_off;
        const std::int32_t tw = a_tab[a_ix(0x1406e8428)];
        if (p == 1 && a_tab[a_ix(0x1406e8428)] / w(m, 0x26) == 8) {
            for (std::int32_t e = 0; e < fld<std::int32_t>(obj, 0x884); e = e + 1) {
                const std::int32_t rw = a_tab[a_ix(0x1406e8428)];
                for (std::int32_t c = 0; c < w(m, 0x26); c = c + 1) {
                    const double v = fld<double*>(obj, 0xa00)[e * w(m, 0x26) + c];
                    for (std::int32_t g = 0; g < 8; g = g + 1) {
                        (a + (start - 1 + off * tw))[e * rw + g * w(m, 0x26) + c] = v;
                    }
                }
            }
        }
    }
    // STNCMP LLTSR
    copy_d(a + (a_tab[a_ix(0x1406e8344)] - 1 + *elem_off * w(m, 0x28)), fld<double*>(obj, 0xa10),
           ne * w(m, 0x28));

    // STRAIN
    p = elem_set(obj)->fun_140183c20(1);
    if (a_tab[a_ix(0x1406e843c)] == p * w(m, 0x2b)) {
        copy_d(a + (a_tab[a_ix(0x1406e8350)] - 1 + *elem_off * a_tab[a_ix(0x1406e843c)]),
               fld<double*>(obj, 0xa28), p * ne * w(m, 0x2b));
    }
    else {
        const std::int32_t start = a_tab[a_ix(0x1406e8350)];
        const std::int32_t off = *elem_off;
        const std::int32_t tw = a_tab[a_ix(0x1406e843c)];
        if (p == 1 && a_tab[a_ix(0x1406e843c)] / w(m, 0x2b) == 8) {
            for (std::int32_t k = 0; k < fld<std::int32_t>(obj, 0x884) * w(m, 0x2b); k = k + 1) {
                const double v = fld<double*>(obj, 0xa28)[k];
                for (std::int32_t g = 0; g < 8; g = g + 1) {
                    (a + (start - 1 + off * tw))[k * 8 + g] = v;
                }
            }
        }
    }
    // DENSTY LLRDT
    copy_d(a + (a_tab[a_ix(0x1406e8354)] - 1 + *elem_off * w(m, 0x2c)), fld<double*>(obj, 0xa30),
           ne * w(m, 0x2c));

    // DAMAGE
    p = elem_set(obj)->fun_140183c20(0);
    if (a_tab[a_ix(0x1406e8448)] == p * w(m, 0x2d)) {
        copy_d(a + (a_tab[a_ix(0x1406e835c)] - 1 + *elem_off * a_tab[a_ix(0x1406e8448)]),
               fld<double*>(obj, 0xa38), p * ne * w(m, 0x2d));
    }
    else {
        const std::int32_t start = a_tab[a_ix(0x1406e835c)];
        const std::int32_t off = *elem_off;
        const std::int32_t tw = a_tab[a_ix(0x1406e8448)];
        if (p == 1 && a_tab[a_ix(0x1406e8448)] / w(m, 0x2d) == 8) {
            for (std::int32_t k = 0; k < fld<std::int32_t>(obj, 0x884) * w(m, 0x2d); k = k + 1) {
                const double v = fld<double*>(obj, 0xa38)[k];
                for (std::int32_t g = 0; g < 8; g = g + 1) {
                    (a + (start - 1 + off * tw))[k * 8 + g] = v;
                }
            }
        }
    }
    // MATAXI LLMTA
    if (w(m, 0x2e) != 0 && fld<double*>(obj, 0xa40) != nullptr) {
        copy_d(a + (a_tab[a_ix(0x1406e8338)] - 1 + *elem_off * w(m, 0x2e)), fld<double*>(obj, 0xa40),
               ne * w(m, 0x2e));
    }
    // USRELM LLET1
    if (ext::f140184a30(elem_set(obj)) != nullptr) {
        const std::int32_t start = a_tab[a_ix(0x1406e8360)];
        const std::int32_t off = *elem_off;
        const std::int32_t wid = w(m, 0x3c);
        const std::int32_t n = fld<std::int32_t>(obj, 0x884);
        const std::int32_t wid2 = w(m, 0x3c);
        double* buf = ext::f140184a30(elem_set(obj));
        copy_d(a + (start - 1 + off * wid), buf, n * wid2);
    }
    // VOLFC / VOLFS / DVOLF / TICF (objects with phases, obj + 0x3590 > 0)
    if (0 < fld<std::int32_t>(obj, 0x3590)) {
        if (ext::f140199c10(owner_of(obj)) == owner_of(obj)->n_materials) {
            normalize_volume_fractions(obj, &m->u15f8[0x31]);
            copy_d(a + (a_tab[a_ix(0x1406e8364)] - 1 + *elem_off * w(m, 0x31)), fld<double*>(obj, 0xa58),
                   ne * w(m, 0x31));
            copy_d(a + (a_tab[a_ix(0x1406e836c)] - 1 + *elem_off * w(m, 0x32)), fld<double*>(obj, 0xa60),
                   ne * w(m, 0x32));
        }
        else {
            const std::int32_t nm = ext::f140199c10(owner_of(obj));
            auto* buf = static_cast<double*>(std::malloc(static_cast<std::size_t>(
                static_cast<std::int64_t>(fld<std::int32_t>(obj, 0x884) * nm)) * 8));
            remap_volume_fractions(obj, fld<double*>(obj, 0xa58), buf, &m->u15f8[0x31]);
            std::int32_t nm2 = ext::f140199c10(owner_of(obj));
            normalize_volume_fraction_rows(obj, buf, &nm2);
            {
                const std::int32_t start = a_tab[a_ix(0x1406e8364)];
                const std::int32_t wid = ext::f140199c10(owner_of(obj));
                const std::int32_t off = *elem_off;
                const std::int32_t wid2 = ext::f140199c10(owner_of(obj));
                copy_d(a + (start - 1 + off * wid), buf, fld<std::int32_t>(obj, 0x884) * wid2);
            }
            remap_volume_fractions(obj, fld<double*>(obj, 0xa60), buf, &m->u15f8[0x32]);
            {
                const std::int32_t start = a_tab[a_ix(0x1406e836c)];
                const std::int32_t wid = ext::f140199c10(owner_of(obj));
                const std::int32_t off = *elem_off;
                const std::int32_t wid2 = ext::f140199c10(owner_of(obj));
                copy_d(a + (start - 1 + off * wid), buf, fld<std::int32_t>(obj, 0x884) * wid2);
            }
            std::free(buf);
        }
        if (0 < a_tab[a_ix(0x1406e8454)]) {
            if (a_tab[a_ix(0x1406e8454)] != w(m, 0x33)) {
                ext::f1401d6450(owner_of(obj));
                ext::f1401d0fc0(owner_of(obj), 0);
            }
            if (a_tab[a_ix(0x1406e8454)] == w(m, 0x33)) {
                copy_d(a + (a_tab[a_ix(0x1406e8368)] - 1 + *elem_off * w(m, 0x33)), fld<double*>(obj, 0xa68),
                       ne * w(m, 0x33)); // DVOLF LLDVF
            }
            else {
                // str 1405220e0
                report_width("Field width of DVOLF doesn't match %d vs %d\n", a_tab[a_ix(0x1406e8454)],
                             w(m, 0x33));
            }
        }
        if (0 < a_tab[a_ix(0x1406e8460)]) {
            if (a_tab[a_ix(0x1406e8460)] == w(m, 0x34)) {
                copy_d(a + (a_tab[a_ix(0x1406e8374)] - 1 + *elem_off * w(m, 0x34)), fld<double*>(obj, 0xa70),
                       ne * w(m, 0x34)); // TICF LLICB
            }
            else {
                // str 140522120
                report_width("Field width of TICF doesn't match %d vs %d\n", a_tab[a_ix(0x1406e8460)],
                             w(m, 0x34));
            }
        }
    }
    // GRAIN (element rows) when ELMNOD[5] == 0
    if (fld<std::int32_t>(owner_of(obj), 0x898) == 0) {
        if (w(m, 0x35) == a_tab[a_ix(0x1406e8464)]) {
            copy_d(a + (a_tab[a_ix(0x1406e8378)] - 1 + *elem_off * w(m, 0x35)), fld<double*>(obj, 0xa78),
                   ne * w(m, 0x35));
        }
        else {
            // str 140522160
            report_width("Field width of GRAIN doesn't match %d vs %d\n", a_tab[a_ix(0x1406e8464)], w(m, 0x35));
        }
    }
    copy_d(a + (a_tab[a_ix(0x1406e8370)] - 1 + *elem_off * w(m, 0x38)), fld<double*>(obj, 0xa90),
           ne * w(m, 0x38)); // HDNOBJ LLHDN
    copy_d(a + (a_tab[a_ix(0x1406e8384)] - 1 + *elem_off * w(m, 0x39)), fld<double*>(obj, 0xa98),
           ne * w(m, 0x39)); // YLDS LLYLS
    copy_d(a + (a_tab[a_ix(0x1406e8388)] - 1 + *elem_off * w(m, 0x3a)), fld<double*>(obj, 0xaa0),
           ne * w(m, 0x3a)); // ZEFI LLZEI
    copy_d(a + (a_tab[a_ix(0x1406e837c)] - 1 + *elem_off * w(m, 0x3e)), fld<double*>(obj, 0xab0),
           ne * w(m, 0x3e)); // CURRNT LLCUR
    if (0 < w(m, 0x36) && fld<double*>(obj, 0xa80) != nullptr) {
        copy_d(a + (a_tab[a_ix(0x1406e83a0)] - 1 + *elem_off * w(m, 0x36)), fld<double*>(obj, 0xa80),
               ne * w(m, 0x36)); // SIZESH LLSIZ
    }
    copy_d(a + (a_tab[a_ix(0x1406e83a4)] - 1 + *elem_off * w(m, 0x3b)), fld<double*>(obj, 0xaa8),
           ne * w(m, 0x3b)); // ZMFI LLZMI
    if (0 < w(m, 0x37) && fld<double*>(obj, 0xa88) != nullptr) {
        copy_d(a + (a_tab[a_ix(0x1406e83a8)] - 1 + *elem_off * w(m, 0x37)), fld<double*>(obj, 0xa88),
               ne * w(m, 0x37)); // TXTODF LLODF
    }
    if (0 < w(m, 0x3f) && fld<double*>(obj, 0xab8) != nullptr) {
        copy_d(a + (a_tab[a_ix(0x1406e83b0)] - 1 + *elem_off * w(m, 0x3f)), fld<double*>(obj, 0xab8),
               ne * w(m, 0x3f)); // ERECVL LLERV
    }
}

// @src 140174cd0  WriteElemIntegerInfoToDB: connectivity at MAR[conn_off - 1..] (+ node offset),
//                 MTLGRP at IIMTR rows elem_off..
void write_elem_int_info(LLOBJ_DATA* obj, std::int32_t* mar, const std::int32_t* mar_tab,
                         const std::int32_t* conn_off, const std::int32_t* elem_off,
                         const std::int32_t* node_off)
{
    select_tables(obj);
    const std::int32_t npe = elem_set(obj)->width(); // 140185530
    std::int32_t* dst = mar + (*conn_off - 1);
    const std::int32_t ne = fld<std::int32_t>(obj, 0x884);
    copy_i(dst, elem_connectivity(elem_set(obj)), ne * npe);
    if (0 < *node_off) {
        for (std::int32_t k = 0; k < fld<std::int32_t>(obj, 0x884) * npe; k = k + 1) {
            dst[k] = dst[k] + *node_off;
        }
    }
    const std::int32_t start = mar_tab[mar_ix(0x1406e8210)];
    const std::int32_t off = *elem_off;
    const std::int32_t ne2 = fld<std::int32_t>(obj, 0x884);
    copy_i(mar + (start - 1 + off), element_material_groups(elem_set(obj)), ne2);
}

// @src 1401774f0  WriteEdgeFaceInfoToDB: the 12 element-face BC arrays at rows face_off..;
//                 used function references are flagged in fn_used, kinds in *kinds
void write_edge_face_info(LLOBJ_DATA* obj, std::int32_t* mar, double* a,
                          const std::int32_t* mar_tab, const std::int32_t* a_tab,
                          const std::int32_t* face_off, const std::int32_t* n_faces,
                          std::int32_t* fn_used, std::uint32_t* kinds)
{
    select_tables(obj);
    auto mi = [&](std::uint64_t dat) { return mar + (mar_tab[mar_ix(dat)] - 1 + *face_off); };
    auto ai = [&](std::uint64_t dat) { return a + (a_tab[a_ix(dat)] - 1 + *face_off); };
    auto flag = [&](std::ptrdiff_t code, std::ptrdiff_t fn, std::int32_t skip, std::uint32_t bit) {
        for (std::int32_t k = 0; k < *n_faces; k = k + 1) {
            const std::int32_t f = fld<std::int32_t*>(obj, fn)[k];
            if (0 < f && fld<std::int32_t*>(obj, code)[k] != skip) {
                fn_used[f - 1] = 1;
                *kinds = *kinds | bit;
            }
        }
    };
    copy_i(mi(0x1406e8234), fld<std::int32_t*>(obj, 0xae8), *n_faces); // ECCDEF IIEDE
    copy_i(mi(0x1406e8240), fld<std::int32_t*>(obj, 0xaf0), *n_faces); // ECDEFN IIEDF
    flag(0xae8, 0xaf0, 9, 1);
    copy_d(ai(0x1406e838c), fld<double*>(obj, 0xaf8), *n_faces);       // ECPRES LLPRE
    copy_i(mi(0x1406e8238), fld<std::int32_t*>(obj, 0xb00), *n_faces); // ECCTMP IIETM
    copy_i(mi(0x1406e8244), fld<std::int32_t*>(obj, 0xb08), *n_faces); // ECTMFN IIETF
    flag(0xb00, 0xb08, 4, 0x10);
    copy_d(ai(0x1406e8390), fld<double*>(obj, 0xb10), *n_faces);       // ECHFLX LLHFL
    copy_i(mi(0x1406e823c), fld<std::int32_t*>(obj, 0xb18), *n_faces); // ECCATM IIEAT
    copy_i(mi(0x1406e8248), fld<std::int32_t*>(obj, 0xb20), *n_faces); // ECATFN IIEAF
    flag(0xb18, 0xb20, 4, 0x100);
    copy_d(ai(0x1406e8394), fld<double*>(obj, 0xb28), *n_faces);       // ECAFLX LLAFL
    if (0 < ext::f1401f3cb0(&owner_of(obj)->sim_ctrl)) {
        copy_i(mi(0x1406e8250), fld<std::int32_t*>(obj, 0xb30), *n_faces); // ECCRHT IIERH
        copy_i(mi(0x1406e8254), fld<std::int32_t*>(obj, 0xb38), *n_faces); // ECRHFN IIERF
        flag(0xb30, 0xb38, 4, 0x1000);
        copy_d(ai(0x1406e8380), fld<double*>(obj, 0xb40), *n_faces);       // ECRFLX
    }
}

// @src 140257550  if (dst) memcpy(dst, plane + 8, 0x18)
static void sym_plane_point(void* plane, void* dst)
{
    if (dst != nullptr) std::memcpy(dst, at(plane, 8), 0x18);
}

// @src 1402574d0  symmetry plane to LLFNC: point (+8), normal (+0x20 via 1402575d0)
static void write_sym_plane(void* plane, double* out)
{
    sym_plane_point(plane, out);
    ext::f1402575d0(plane, out + 3);
}

// @src 140178460  WriteBdryCndFuncToDB: each flagged local BC function gets a global number
//                 (1401bb3d0) and its data in LLFNC (140131360); the function-number arrays are
//                 then rewritten with the global numbers; 3D: flagged symmetry planes (type 6)
void write_bc_functions(LLOBJ_DATA* obj, std::int32_t* mar, const std::int32_t* mar_tab,
                        const std::int32_t* node_off, const std::int32_t* face_off,
                        const std::int32_t* n_faces, std::int32_t* iifnc, double* llfnc,
                        std::int32_t* fn_used, std::int32_t* sym_used, std::uint32_t* fn_kinds,
                        std::int32_t* sym_any, std::uint32_t* face_kinds)
{
    std::int32_t dim = model::llobj::int_at_1c(owner_of(obj));
    model::select_index_tables(&dim);
    std::int32_t* dst = nullptr;
    DB_STEP_DATA* m = owner_of(obj);
    std::int32_t pos = 0;
    std::int32_t len = 0;
    double number = 0.0;
    void* fn_list = at(obj, 0x2e68);
    if (*fn_kinds != 0 || *face_kinds != 0) {
        const std::int32_t nfn = ext::f140136a10(fn_list);
        for (std::int32_t k = 0; k < nfn; k = k + 1) {
            void* f = ext::f1401368c0(fn_list, k + 1);
            if (f != nullptr) {
                const std::int32_t id = fld<std::int32_t>(f, 0x54);
                if (fn_used[id - 1] != 0) {
                    pos = 0;
                    len = ext::f140130290(f);
                    std::int32_t local = k + 1;
                    alloc_function(owner_of(obj), &len, iifnc, &local, &number, &pos);
                    write_dynary_to_db_array(static_cast<const DYNARY*>(f), llfnc + (pos - 1));
                    fn_used[id - 1] = static_cast<std::int32_t>(number);
                }
            }
        }
        const std::int32_t nn = fld<std::int32_t>(obj, 0x880);
        std::int32_t* bcdef = fld<std::int32_t*>(obj, 0x990);
        if ((*fn_kinds & 1) != 0) {
            dst = mar + (mar_tab[mar_ix(0x1406e821c)] - 1 + *node_off * w(m, 0x13));
            std::int32_t* fnv = fld<std::int32_t*>(obj, 0x998);
            if (!rt::dimension_is_3(owner_of(obj))) {
                if (rt::dimension_is_2(owner_of(obj))) {
                    for (std::int32_t k = 0; k < fld<std::int32_t>(obj, 0x880) * w(m, 0x13); k = k + 1) {
                        if (0 < fnv[k]) dst[k] = fn_used[fnv[k] - 1];
                    }
                }
            }
            else {
                for (std::int32_t k = 0; k < fld<std::int32_t>(obj, 0x880) * w(m, 0x13); k = k + 1) {
                    if (bcdef[k] != 6 && 0 < fnv[k]) dst[k] = fn_used[fnv[k] - 1];
                }
            }
        }
        (void)nn;
        if ((*fn_kinds & 0x10) != 0) {
            dst = mar + (mar_tab[mar_ix(0x1406e8224)] - 1 + *node_off * w(m, 0x15));
            std::int32_t* fnv = fld<std::int32_t*>(obj, 0x9a8);
            for (std::int32_t k = 0; k < fld<std::int32_t>(obj, 0x880) * w(m, 0x15); k = k + 1) {
                if (0 < fnv[k]) dst[k] = fn_used[fnv[k] - 1];
            }
        }
        auto face = [&](std::uint32_t bit, std::uint64_t dat, std::ptrdiff_t code, std::ptrdiff_t fn,
                        std::int32_t skip) {
            if ((*face_kinds & bit) != 0) {
                dst = mar + (mar_tab[mar_ix(dat)] - 1 + *face_off);
                for (std::int32_t k = 0; k < *n_faces; k = k + 1) {
                    const std::int32_t f = fld<std::int32_t*>(obj, fn)[k];
                    if (0 < f && fld<std::int32_t*>(obj, code)[k] != skip) dst[k] = fn_used[f - 1];
                }
            }
        };
        face(1, 0x1406e8240, 0xae8, 0xaf0, 9);      // ECDEFN IIEDF
        face(0x10, 0x1406e8244, 0xb00, 0xb08, 4);   // ECTMFN IIETF
        face(0x100, 0x1406e8248, 0xb18, 0xb20, 4);  // ECATFN IIEAF
        face(0x1000, 0x1406e8254, 0xb30, 0xb38, 4); // ECRHFN IIERF
    }
    if (rt::dimension_is_3(owner_of(obj)) && *sym_any != 0) {
        void* planes = at(obj, 0x2e80);
        const std::int32_t np = ext::f1402582b0(planes);
        for (std::int32_t k = 0; k < np; k = k + 1) {
            if (sym_used[k] != 0) {
                void* pl = ext::f1402580f0(planes, k + 1);
                if (pl != nullptr) {
                    pos = 0;
                    len = 6;
                    std::int32_t id = k + 1;
                    alloc_function(owner_of(obj), &len, iifnc, &id, &number, &pos);
                    write_sym_plane(pl, llfnc + (pos - 1));
                    sym_used[k] = static_cast<std::int32_t>(number);
                }
            }
        }
        const std::int32_t start = mar_tab[mar_ix(0x1406e821c)];
        const std::int32_t off = *node_off;
        const std::int32_t wid = w(m, 0x13);
        const std::int32_t nn = fld<std::int32_t>(obj, 0x880);
        const std::int32_t wid2 = w(m, 0x13);
        std::int32_t* bcdef = fld<std::int32_t*>(obj, 0x990);
        std::int32_t* fnv = fld<std::int32_t*>(obj, 0x998);
        for (std::int32_t k = 0; k < nn * wid2; k = k + 1) {
            if (bcdef[k] == 6 && 0 < fnv[k]) {
                (mar + (start - 1 + off * wid))[k] = sym_used[fnv[k] - 1];
            }
        }
    }
}

// 140183cb0 for callers outside this file (p4-06's 2D object writer)
std::int32_t* element_connectivity(void* elems) { return elem_connectivity(static_cast<Cls_140182880*>(elems)); }

} // namespace def::dbio::g3d
