// DBREAD helper W2 (packet p-dbread): per-object node / element arrays from the decoded A array
// (14016dfb0, 140170ed0), ReadSingleAArrayFromDB (1401d4400, 1401cecc0), the step-directory
// sizes (1401cc1a0) and the DB file-set lookups of Cls_1401dbfe0 (1401e5d60..1401e5f00).
// Reference: raw decompile out/dbread/w2/raw_<addr>.c and the listing out/dbread/w2/listing.txt
// (deform-rea-work). Region names: docs/re/dbmap/region_globals.tsv (branch bz5k5e).
//
// The region globals DAT_1406e8xxx are 0-based indices into the A address table (140 starts,
// then 140 widths) that DBREAD passes as a_tab (DB_STEP_DATA+0x1dc0); A(addr) reads the global
// at `addr` through the index tables (1401c7d40 fills them per dimension). Index arithmetic is
// 32-bit as in the binary: start - 1 + (first - 1) * width is an int, then sign-extended; the
// memcpy sizes are (int64)(count * width) << 3 with the product in int.
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS // strcpy as in the original
#endif
#include <defdbio/dbread/w2.hpp>

#include <cstdio>
#include <cstring>

#include <defdbio/gendb/gendb.hpp>
#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/bc/node_arrays.hpp>
#include <defmodel/layout/cls_1401dbfe0.hpp>
#include <defmodel/llobj/cls_140182880.hpp>
#include <defmodel/llobj/dynary_table.hpp>
#include <defmodel/sdctl/step_timer.hpp>
#include <defmodel/statevar_kw/statevar_kw.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/step/name_arrays.hpp>
#include <defrt/ftnstr/ftnstr.hpp>
#include <defrt/stdlib/refcount.hpp>

namespace def::dbio::dbread {

namespace {

using model::DB_STEP_DATA;
using model::LLOBJ_DATA;

template <class T> inline T& fld(void* base, std::ptrdiff_t off)
{
    return *reinterpret_cast<T*>(static_cast<char*>(base) + off);
}
template <class T> inline const T& fld(const void* base, std::ptrdiff_t off)
{
    return *reinterpret_cast<const T*>(static_cast<const char*>(base) + off);
}

// value of the A-table index global DAT_<addr> (DAT_1406e82d0.. = g_a_index)
inline std::int32_t A(std::uint64_t addr) { return model::g_a_index[(addr - 0x1406e82d0ull) / 4]; }
// value of the MAR-table index global DAT_<addr> (DAT_1406e81fc.. = g_mar_index)
inline std::int32_t M(std::uint64_t addr) { return model::g_mar_index[(addr - 0x1406e81fcull) / 4]; }

inline void copy_d(void* dst, const double* src, std::int32_t count)
{
    std::memcpy(dst, src, static_cast<std::size_t>(static_cast<std::int64_t>(count) << 3));
}

// *(obj + 15000): the owning DB_STEP_DATA
inline DB_STEP_DATA* owner(LLOBJ_DATA* obj) { return fld<DB_STEP_DATA*>(obj, 0x3a98); }

// 140158e90 + 1401c7d40: index tables of the owner's dimension
inline void select_tables(LLOBJ_DATA* obj)
{
    std::int32_t dim = model::llobj::int_at_1c(owner(obj));
    model::select_index_tables(&dim);
}

// The test every region block opens with: `*region == -1 || *region == R`; with -1 the start
// offset becomes start(R) - 1 (otherwise it keeps its value: 0 for a single region).
inline bool take(const std::int32_t* region, const std::int32_t* a_tab, std::uint64_t r, std::int32_t& s)
{
    if (*region == -1 || *region == A(r)) {
        if (*region == -1) {
            s = a_tab[A(r)] - 1;
        }
        return true;
    }
    return false;
}

// node block of width w: a[s + (first - 1) * w], nn * w words
inline void node_rows(LLOBJ_DATA* obj, std::ptrdiff_t member, const double* a, std::int32_t s, std::int32_t w)
{
    copy_d(fld<void*>(obj, member), a + (s + (fld<std::int32_t>(obj, 0x3984) - 1) * w),
           fld<std::int32_t>(obj, 0x880) * w);
}
// scalar node block: a[s - 1 + first], nn words
inline void node_scalar(LLOBJ_DATA* obj, std::ptrdiff_t member, const double* a, std::int32_t s)
{
    copy_d(fld<void*>(obj, member), a + (s + -1 + fld<std::int32_t>(obj, 0x3984)), fld<std::int32_t>(obj, 0x880));
}
// element block of width w: a[s + (first - 1) * w], ne * w words
inline void elem_rows(LLOBJ_DATA* obj, std::ptrdiff_t member, const double* a, std::int32_t s, std::int32_t w)
{
    copy_d(fld<void*>(obj, member), a + (s + (fld<std::int32_t>(obj, 0x3988) - 1) * w),
           fld<std::int32_t>(obj, 0x884) * w);
}

// part of @src 140170ed0: LLSTS / LLTEP / LLDMG element block with `k` state values of width d
// per element (k = 140183c20(i)); when the stored width W differs, k == 1 and W / d == 8 copy the
// first d values of each W-word row.
void elem_state_block(LLOBJ_DATA* obj, std::ptrdiff_t member, const double* a, std::int32_t s, std::int32_t k,
                      std::int32_t d, std::int32_t W)
{
    if (0 < W) {
        if (W == k * d) {
            copy_d(fld<void*>(obj, member), a + (s + (fld<std::int32_t>(obj, 0x3988) - 1) * W),
                   k * fld<std::int32_t>(obj, 0x884) * d);
        } else {
            const std::int32_t first = fld<std::int32_t>(obj, 0x3988);
            if (k == 1 && W / d == 8) {
                for (std::int32_t i = 0; i < fld<std::int32_t>(obj, 0x884); i = i + 1) {
                    for (std::int32_t j = 0; j < d; j = j + 1) {
                        fld<double*>(obj, member)[i * d + j] = (a + (s + (first + -1) * W))[i * W + j];
                    }
                }
            }
        }
    }
}

} // namespace

// @src 14016dfb0  LLOBJ_DATA::ReadNodeInfoFromDB: node arrays (obj + 0x8a8..0x968) from the A
// array. widths = DB_STEP_DATA + 0x15f8 (D[]); limit is compared with start(LLDTM) (2D only).
// mar and mar_tab are not used.
void obj_read_node_arrays(model::LLOBJ_DATA* obj, std::int32_t* /*mar*/, double* a, std::int32_t* /*mar_tab*/,
                          std::int32_t* a_tab, std::int32_t* D, std::int32_t numnp, const std::int32_t* region,
                          std::int32_t limit)
{
    select_tables(obj);
    if (fld<std::int32_t>(obj, 0x87c) == 0) {
        return;
    }
    if (fld<void*>(obj, 0x8a8) == nullptr) {
        model::bc::alloc_node_arrays(*obj, D); // 140162ff0
    }
    std::int32_t s = 0;
    if (take(region, a_tab, 0x1406e82fc, s) && D[0] == a_tab[A(0x1406e83e8)]) {
        node_rows(obj, 0x8a8, a, s, D[0]); // LLCRZ
    }
    if (take(region, a_tab, 0x1406e8300, s) && D[1] == a_tab[A(0x1406e83ec)]) {
        node_rows(obj, 0x8b0, a, s, D[1]); // LLDRZ
    }
    if (take(region, a_tab, 0x1406e8304, s) && D[2] == a_tab[A(0x1406e83f0)]) {
        node_rows(obj, 0x8b8, a, s, D[2]); // LLURZ
    }
    if (take(region, a_tab, 0x1406e8308, s) && D[3] == a_tab[A(0x1406e83f4)]) {
        node_rows(obj, 0x8c0, a, s, D[3]); // LLPFN
    }
    if (take(region, a_tab, 0x1406e830c, s) && D[4] == a_tab[A(0x1406e83f8)]) {
        node_rows(obj, 0x8c8, a, s, D[4]); // LLCFN
    }
    if (take(region, a_tab, 0x1406e8310, s)) { // LLPPR, 3D; a stored width 0 is accepted
        if (rt::dimension_is_3(owner(obj)) && (D[5] == a_tab[A(0x1406e83fc)] || a_tab[A(0x1406e83fc)] == 0)) {
            node_rows(obj, 0x8d0, a, s, D[5]);
        }
    }
    if (take(region, a_tab, 0x1406e8314, s) && D[6] == a_tab[A(0x1406e8400)]) {
        node_rows(obj, 0x8d8, a, s, D[6]); // LLCPR
    }
    if (take(region, a_tab, 0x1406e8318, s) && D[8] == a_tab[A(0x1406e8404)]) {
        node_scalar(obj, 0x8e0, a, s); // LLTMP
    }
    if (rt::dimension_is_2(owner(obj)) && take(region, a_tab, 0x1406e839c, s)) { // LLDTM, 2D
        if (a_tab[A(0x1406e839c)] < limit && (D[0xc] == a_tab[A(0x1406e8488)] || a_tab[A(0x1406e8488)] == 0)) {
            node_scalar(obj, 0x900, a, s);
        }
    }
    if (take(region, a_tab, 0x1406e831c, s) && D[9] == a_tab[A(0x1406e8408)]) {
        node_scalar(obj, 0x8e8, a, s); // LLPHT
    }
    if (take(region, a_tab, 0x1406e8320, s) && D[10] == a_tab[A(0x1406e840c)]) {
        node_scalar(obj, 0x8f0, a, s); // LLHIT
    }
    if (take(region, a_tab, 0x1406e8324, s)) { // LLFLX: width D[11], 0 or 3 (three NUMNP blocks)
        if (D[0xb] == a_tab[A(0x1406e8410)] || a_tab[A(0x1406e8410)] == 0 || a_tab[A(0x1406e8410)] == 3) {
            node_scalar(obj, 0x8f8, a, s);
        }
        if (a_tab[A(0x1406e8410)] == 3) {
            copy_d(fld<void*>(obj, 0x920), a + (s + numnp + -1 + fld<std::int32_t>(obj, 0x3984)),
                   fld<std::int32_t>(obj, 0x880));
        }
        if (a_tab[A(0x1406e8410)] == 3) {
            copy_d(fld<void*>(obj, 0x910), a + (s + numnp * 2 + -1 + fld<std::int32_t>(obj, 0x3984)),
                   fld<std::int32_t>(obj, 0x880));
        }
    }
    if (take(region, a_tab, 0x1406e8328, s) && D[0x11] == a_tab[A(0x1406e8414)]) {
        node_rows(obj, 0x968, a, s, D[0x11]); // LLND1 (USRNOD)
    }
    if (take(region, a_tab, 0x1406e8330, s) && D[0xd] == a_tab[A(0x1406e841c)]) {
        node_scalar(obj, 0x908, a, s); // LLATM
    }
    if (take(region, a_tab, 0x1406e832c, s) && D[0xf] == a_tab[A(0x1406e8418)]) {
        node_scalar(obj, 0x918, a, s); // LLVLT
    }
    if (take(region, a_tab, 0x1406e8334, s) && D[0x18] == a_tab[A(0x1406e8420)]) {
        // LLWER: the row offset uses the stored width (equal to D[0x18] here)
        copy_d(fld<void*>(obj, 0x928), a + (s + (fld<std::int32_t>(obj, 0x3984) - 1) * a_tab[A(0x1406e8420)]),
               fld<std::int32_t>(obj, 0x880) * D[0x18]);
    }
    // DRMESH, 3D: literal A slot 0x13 (a_tab + 0x4c) and its width slot 0x9f (a_tab + 0x27c)
    if (rt::dimension_is_3(owner(obj)) && (*region == -1 || *region == 0x13)) {
        if (*region == -1) {
            s = a_tab[0x13] - 1;
        }
        if (D[0x19] == a_tab[0x9f]) {
            node_rows(obj, 0x930, a, s, D[0x19]);
        }
    }
    // LLSTS / LLTEP / LLDMG node blocks follow the NUMEL element rows (owner + 0x13ec)
    if (take(region, a_tab, 0x1406e833c, s) && D[0x1a] == a_tab[A(0x1406e8428)] && owner(obj) != nullptr) {
        copy_d(fld<void*>(obj, 0x948),
               a + (s + (owner(obj)->u13ec + -1 + fld<std::int32_t>(obj, 0x3984)) * D[0x1a]),
               fld<std::int32_t>(obj, 0x880) * D[0x1a]);
    }
    if (take(region, a_tab, 0x1406e8350, s) && D[0x1b] == a_tab[A(0x1406e843c)] && owner(obj) != nullptr) {
        copy_d(fld<void*>(obj, 0x938),
               a + (s + (owner(obj)->u13ec + -1 + fld<std::int32_t>(obj, 0x3984)) * D[0x1b]),
               fld<std::int32_t>(obj, 0x880) * D[0x1b]);
    }
    if (take(region, a_tab, 0x1406e835c, s)) { // LLDMG
        if (D[0x1c] == a_tab[A(0x1406e8448)]) {
            if (owner(obj) != nullptr) {
                copy_d(fld<void*>(obj, 0x940),
                       a + (s + (owner(obj)->u13ec + -1 + fld<std::int32_t>(obj, 0x3984)) * D[0x1c]),
                       fld<std::int32_t>(obj, 0x880) * D[0x1c]);
            }
        } else if (fld<std::int32_t>(owner(obj), 0x888) == 2 && D[0x1c] < a_tab[A(0x1406e8448)]) {
            // ELMNOD 1 == 2 and a wider stored DAMAGE: widen the node variable first
            D[0x1c] = a_tab[A(0x1406e8448)];
            if (owner(obj) != nullptr) {
                const std::int32_t numel = owner(obj)->u13ec;
                model::svkw::set8a0_resize(&fld<std::byte>(obj, 0x8a0), 0x1c, D[0x1c]); // 140180310
                copy_d(fld<void*>(obj, 0x940), a + (s + (numel + -1 + fld<std::int32_t>(obj, 0x3984)) * D[0x1c]),
                       fld<std::int32_t>(obj, 0x880) * D[0x1c]);
            }
        }
    }
    if (take(region, a_tab, 0x1406e8378, s)) { // LLGSZ, node grain size when ELMNOD 5 == 1
        if (fld<std::int32_t>(owner(obj), 0x898) == 1) {
            double* g = nullptr;
            if (D[0x1d] != a_tab[A(0x1406e8464)]) {
                D[0x1d] = a_tab[A(0x1406e8464)];
            }
            const bool one = true;
            g = model::svkw::grain2_values(&fld<std::byte>(obj, 0x8a0), &one); // 140181420
            if (a_tab[A(0x1406e8464)] != 0 && g != nullptr && owner(obj) != nullptr) {
                node_rows(obj, 0x950, a, s, D[0x1d]);
            }
        }
    }
    if (10999 < gendb::stored_version(*owner(obj))) { // 1401cc180
        if (take(region, a_tab, 0x1406e83ac, s)) { // LLTRC
            if (D[0x22] != a_tab[A(0x1406e84c4)]) {
                D[0x22] = a_tab[A(0x1406e84c4)];
                model::svkw::set8a0_resize(&fld<std::byte>(obj, 0x8a0), 0x22, D[0x22]);
            }
            if (0 < D[0x22]) {
                node_rows(obj, 0x958, a, s, D[0x22]);
            }
        }
        if (take(region, a_tab, 0x1406e83b4, s)) { // LLNRV
            if (D[0x23] != a_tab[A(0x1406e84cc)]) {
                D[0x23] = a_tab[A(0x1406e84cc)];
                model::svkw::set8a0_resize(&fld<std::byte>(obj, 0x8a0), 0x23, D[0x23]);
            }
            if (0 < D[0x23]) {
                node_rows(obj, 0x960, a, s, D[0x23]);
            }
        }
    }
}

// @src 140170ed0  LLOBJ_DATA::ReadElemInfoFromDB: element arrays (obj + 0xa00..0xab8) from the A
// array. mar, mar_tab and iiobj_rec are not used.
void obj_read_element_arrays(model::LLOBJ_DATA* obj, std::int32_t* /*mar*/, double* a, std::int32_t* /*mar_tab*/,
                             std::int32_t* a_tab, std::int32_t* D, std::int32_t* /*iiobj_rec*/,
                             const std::int32_t* region)
{
    select_tables(obj);
    if (fld<std::int32_t>(obj, 0x87c) == 0) {
        return;
    }
    if (fld<void*>(obj, 0x9e8) == nullptr) {
        model::bc::alloc_element_arrays(*obj, D); // 140163090
    }
    auto* set = reinterpret_cast<model::Cls_140182880*>(&fld<std::byte>(obj, 0x9e0));
    std::int32_t s = 0;
    std::int32_t k = set->fun_140183c20(2);
    if (take(region, a_tab, 0x1406e833c, s)) { // LLSTS, D[0x26] per stress point
        elem_state_block(obj, 0xa00, a, s, k, D[0x26], a_tab[A(0x1406e8428)]);
    }
    if (take(region, a_tab, 0x1406e8340, s) && D[0x27] == a_tab[A(0x1406e842c)]) {
        elem_rows(obj, 0xa08, a, s, D[0x27]); // LLEPS
    }
    if (take(region, a_tab, 0x1406e8344, s) && D[0x28] == a_tab[A(0x1406e8430)]) {
        elem_rows(obj, 0xa10, a, s, D[0x28]); // LLTSR
    }
    if (take(region, a_tab, 0x1406e8348, s) && D[0x29] == a_tab[A(0x1406e8434)]) {
        elem_rows(obj, 0xa18, a, s, D[0x29]); // LLFTS
    }
    if (take(region, a_tab, 0x1406e834c, s) && D[0x2a] == a_tab[A(0x1406e8438)]) {
        elem_rows(obj, 0xa20, a, s, D[0x2a]); // LLFEP
    }
    k = set->fun_140183c20(1);
    if (take(region, a_tab, 0x1406e8350, s)) { // LLTEP
        elem_state_block(obj, 0xa28, a, s, k, D[0x2b], a_tab[A(0x1406e843c)]);
    }
    if (take(region, a_tab, 0x1406e8354, s) && D[0x2c] == a_tab[A(0x1406e8440)]) {
        elem_rows(obj, 0xa30, a, s, D[0x2c]); // LLRDT
    }
    if (take(region, a_tab, 0x1406e8358, s) && D[0x30] == a_tab[A(0x1406e8444)]) {
        elem_rows(obj, 0xa50, a, s, D[0x30]); // LLHGN
    }
    k = set->fun_140183c20(0);
    if (take(region, a_tab, 0x1406e835c, s)) { // LLDMG
        elem_state_block(obj, 0xa38, a, s, k, D[0x2d], a_tab[A(0x1406e8448)]);
    }
    if (take(region, a_tab, 0x1406e8338, s) && D[0x2e] == a_tab[A(0x1406e8424)]) {
        elem_rows(obj, 0xa40, a, s, D[0x2e]); // LLMTA
    }
    if (take(region, a_tab, 0x1406e8360, s) && D[0x3c] == a_tab[A(0x1406e844c)]) { // LLET1 (USRELM)
        const std::int32_t first = fld<std::int32_t>(obj, 0x3988);
        const std::int32_t w1 = D[0x3c];
        const std::int32_t ne = fld<std::int32_t>(obj, 0x884);
        const std::int32_t w2 = D[0x3c];
        double* dst = model::svkw::usrelm_values(set); // 140184a30
        copy_d(dst, a + (s + (first + -1) * w1), ne * w2);
    }
    if (take(region, a_tab, 0x1406e8364, s) && D[0x31] == a_tab[A(0x1406e8450)]) {
        elem_rows(obj, 0xa58, a, s, D[0x31]); // LLVLF
    }
    if (take(region, a_tab, 0x1406e836c, s) && D[0x32] == a_tab[A(0x1406e8458)]) {
        elem_rows(obj, 0xa60, a, s, D[0x32]); // LLVLN
    }
    if (take(region, a_tab, 0x1406e8368, s) && D[0x33] == a_tab[A(0x1406e8454)]) {
        elem_rows(obj, 0xa68, a, s, D[0x33]); // LLDVF
    }
    if (take(region, a_tab, 0x1406e8374, s) && D[0x34] == a_tab[A(0x1406e8460)]) {
        elem_rows(obj, 0xa70, a, s, D[0x34]); // LLICB
    }
    if (take(region, a_tab, 0x1406e8378, s)) { // LLGSZ, element grain size when ELMNOD 5 == 0
        if (fld<std::int32_t>(owner(obj), 0x898) == 0 && D[0x35] == a_tab[A(0x1406e8464)]) {
            elem_rows(obj, 0xa78, a, s, D[0x35]);
        }
    }
    if (take(region, a_tab, 0x1406e8370, s) && D[0x38] == a_tab[A(0x1406e845c)]) {
        elem_rows(obj, 0xa90, a, s, D[0x38]); // LLHDN
    }
    if (take(region, a_tab, 0x1406e8384, s) && D[0x39] == a_tab[A(0x1406e8470)]) {
        elem_rows(obj, 0xa98, a, s, D[0x39]); // LLYLS
    }
    if (take(region, a_tab, 0x1406e8388, s) && D[0x3a] == a_tab[A(0x1406e8474)]) {
        elem_rows(obj, 0xaa0, a, s, D[0x3a]); // LLZEI
    }
    // LLCUR in 2D: always from its own start, whatever *region is
    if (rt::dimension_is_2(owner(obj)) && D[0x3e] == a_tab[A(0x1406e8468)]) {
        elem_rows(obj, 0xab0, a, a_tab[A(0x1406e837c)] + -1, D[0x3e]);
    }
    if (10999 < gendb::stored_version(*owner(obj))) { // 1401cc180
        if (rt::dimension_is_3(owner(obj)) && take(region, a_tab, 0x1406e837c, s)) { // LLCUR, 3D
            if (D[0x3e] == a_tab[A(0x1406e8468)]) {
                elem_rows(obj, 0xab0, a, s, D[0x3e]);
            }
        }
        if (take(region, a_tab, 0x1406e83a0, s)) { // LLSIZ (SIZESH)
            if (D[0x36] != a_tab[A(0x1406e84b8)]) {
                D[0x36] = a_tab[A(0x1406e84b8)];
                model::svkw::set9e0_resize(set, 0x36, D[0x36]); // 140183840
            }
            if (0 < D[0x36]) {
                elem_rows(obj, 0xa80, a, s, D[0x36]);
            }
        }
        if (take(region, a_tab, 0x1406e83a4, s) && D[0x3b] == a_tab[A(0x1406e84bc)]) {
            elem_rows(obj, 0xaa8, a, s, D[0x3b]); // LLZMI
        }
        if (take(region, a_tab, 0x1406e83a8, s)) { // LLODF (TXTODF)
            if (D[0x37] != a_tab[A(0x1406e84c0)]) {
                D[0x37] = a_tab[A(0x1406e84c0)];
                model::svkw::set9e0_resize(set, 0x37, D[0x37]);
            }
            if (0 < D[0x37]) {
                elem_rows(obj, 0xa88, a, s, D[0x37]);
            }
        }
        if (take(region, a_tab, 0x1406e83b0, s)) { // LLERV (ERECVL)
            if (D[0x3f] != a_tab[A(0x1406e84c8)]) {
                D[0x3f] = a_tab[A(0x1406e84c8)];
                model::svkw::set9e0_resize(set, 0x3f, D[0x3f]);
            }
            if (0 < D[0x3f]) {
                elem_rows(obj, 0xab8, a, s, D[0x3f]);
            }
        }
    }
}

// @src 140161060
std::uint8_t obj_byte_3610(const model::LLOBJ_DATA* obj) { return fld<std::uint8_t>(obj, 0x3610); }

// @src 1401d4400  _DB_STEP_DATA::ReadSingleAArrayFromDB
std::uint64_t read_single_a_array(model::DB_STEP_DATA* m, const std::int32_t* region, const char* elem)
{
    std::int32_t dim = m->dimension; // 140158e90
    model::select_index_tables(&dim);
    const std::int32_t iiobj = m->u1b90[M(0x1406e8208)]; // IIOBJ start
    std::int32_t* mar = static_cast<std::int32_t*>(m->u1b88);
    if (*region < 0 || A(0x1406e84d0) <= *region) {
        return 0;
    }
    if (m->u26c4[*region] != 0) {
        return 0;
    }
    model::g_timer_read_obj_node_state_variable.start("Start");
    model::g_timer_read_obj_node_state_variable.checkpoint("ReadSingleAArrayFromDB", nullptr);
    read_single_a_array_from_file(m, region); // 1401cecc0
    model::g_timer_read_arrays.checkpoint("Assign the variable to the object data", nullptr);
    bool state = false;
    if (*region == A(0x1406e833c)) { // LLSTS
        state = 0 < m->u15f8[0x1a];
    } else if (*region == A(0x1406e8350)) { // LLTEP
        state = 0 < m->u15f8[0x1b];
    } else if (*region == A(0x1406e835c)) { // LLDMG
        state = 0 < m->u15f8[0x1c];
    }
    for (std::int32_t i = 0; i < m->u1378; i = i + 1) {
        auto* obj = static_cast<model::LLOBJ_DATA*>(model::step_object_at(*m, i + 1)); // 1401d6ed0
        std::int32_t* rec = mar + (iiobj + -1) + m->u1b90[M(0x1406e8268)] * i; // IIOBJ record i
        if (obj != nullptr && fld<std::int32_t>(obj, 0xc) != 7 && obj_byte_3610(obj) == 0) {
            double* a = static_cast<double*>(m->u1b78);
            if (state) {
                obj_read_node_arrays(obj, mar, a, m->u1b90, m->u1dc0, m->u15f8, m->u13e8, region, 0);
                obj_read_element_arrays(obj, mar, a, m->u1b90, m->u1dc0, m->u15f8, rec, region);
            } else if (*elem == '\0') {
                obj_read_element_arrays(obj, mar, a, m->u1b90, m->u1dc0, m->u15f8, rec, region);
            } else {
                obj_read_node_arrays(obj, mar, a, m->u1b90, m->u1dc0, m->u15f8, m->u13e8, region, 0);
            }
        }
    }
    const char report = 1;
    model::g_timer_read_obj_node_state_variable.stop(&report);
    return 1;
}

// @src 1401cecc0  _DB_STEP_DATA::ReadSingleAArrayFromDB (file part). Past the directory lookup the
// body needs 1401d2b70 == 0, which never happens (it always returns 1).
void read_single_a_array_from_file(model::DB_STEP_DATA* m, const std::int32_t* region)
{
    model::Cls_1401dbfe0* db = m->db;
    if (db == nullptr) {
        return;
    }
    std::int32_t nchar = 0;
    std::int32_t slot = m->u740 + 1;
    std::int32_t ierr = 0;    // [rsp+0x8b4]
    std::int32_t iprint = 0;  // [rsp+0x78]
    std::int32_t hsize = 0;   // [rsp+0x4a0]
    std::int32_t mar_size = 0; // [rsp+0x8b0]
    std::int32_t a_size = 0;  // [rsp+0x74]
    std::int32_t version = 0; // [rsp+0x7c]
    if (step_dir_sizes(m, db, m->u740, &hsize, &mar_size, &a_size, &version) == 0) {
        return;
    }
    if (model::names_up_to_date(m)) { // 1401d2b70
        return;
    }
    if (*region < 0 || 0x117 < *region) {
        return;
    }
    if (m->u1b68 == 0 || m->u1b6c == 0) {
        std::fprintf(stdout, "ERROR: _DB_STEP_DATA::ReadSingleAArrayFromDB() was called before\n"
                             "the step header is read. This is illegal.\n"); // str 14052a6e0
        return;
    }
    if (m->u26c4[*region] != 0) {
        return;
    }
    if (m->u1b78 == nullptr) {
        m->u1b78 = model::bc::calloc_doubles(m->u2b24 + 2); // 140257150
    }
    if (m->u2b28[*region] == 0) {
        m->u26c4[*region] = 1;
        return;
    }
    std::int32_t first = 0; // [rsp+0x70]
    char name[0x400];       // [rsp+0x4b0]
    char file[0x400];       // [rsp+0x90]
    db_file_for_step(db, name, &first, m->u740);
    nchar = static_cast<std::int32_t>(rt::to_fortran(file, name, 0x400)); // 140323bf0
    slot = (m->u740 - first) + 1;
    read_a_region_from_file(file, &nchar, &slot, &hsize, m->u1b80, &mar_size, static_cast<std::int32_t*>(m->u1b88),
                            &a_size, static_cast<double*>(m->u1b78), &m->u1dc0[*region], &version, &ierr,
                            &iprint); // 1403dc957
    m->u26c4[*region] = 1;
}

// @src 1401cc160
void set_step_version(model::DB_STEP_DATA* m, std::int32_t v) { m->step_version = v; }

// @src 1401cc1a0  the entry is that of m + 0x740 (the step argument is not used by the body)
std::uint64_t step_dir_sizes(model::DB_STEP_DATA* m, model::Cls_1401dbfe0* db, std::int32_t /*step*/,
                             std::int32_t* hsize, std::int32_t* mar_size, std::int32_t* a_size,
                             std::int32_t* version)
{
    if (db == nullptr) {
        return 0;
    }
    const auto* rec = static_cast<const model::DbDirRecord*>(db->uad8) + m->u740;
    *hsize = rec->hsize;       // +0x08
    *mar_size = rec->mar_size; // +0x0c
    *a_size = rec->a_size;     // +0x10
    *version = rec->version;   // +0x20
    if (gendb::stored_version(*m) != *version) { // 1401cc180
        set_step_version(m, *version);
    }
    return 1;
}

// @src 1401e5d60
void db_file_name(model::Cls_1401dbfe0* db, char* out, std::int32_t i)
{
    if (db->u008 < 2 || i < 0 || db->u008 <= i) {
        std::strcpy(out, db->u650); // 140484f90
    } else {
        std::strcpy(out, static_cast<const char*>(db->u330[i]));
    }
}

// @src 1401e5de0
std::int32_t db_file_index(model::Cls_1401dbfe0* db, std::int32_t step)
{
    if (1 < db->u008) {
        for (std::int32_t i = 0; i < db->u008; i = i + 1) {
            if (db->u00c[i].a <= step && step <= db->u00c[i].b) {
                return i;
            }
        }
    }
    return 0;
}

// @src 1401e5eb0
std::int32_t db_file_first(model::Cls_1401dbfe0* db, std::int32_t i)
{
    std::int32_t r = 0;
    if (db->u008 < 2 || i < 0 || db->u008 <= i) {
        r = 0;
    } else {
        r = db->u00c[i].a;
    }
    return r;
}

// @src 1401e5f00
void db_file_for_step(model::Cls_1401dbfe0* db, char* out, std::int32_t* first, std::int32_t step)
{
    if (db->u008 < 2) {
        std::strcpy(out, db->u650);
        *first = 0;
    } else {
        const std::int32_t i = db_file_index(db, step);
        db_file_name(db, out, i);
        *first = db_file_first(db, i);
    }
}

} // namespace def::dbio::dbread
