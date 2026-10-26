// GENDB step header and the Fortran address tables of op 4 (packet p4-07).
// Declarations: defdbio/gendb/step_header.hpp.
#include <defdbio/gendb/step_header.hpp>

#include <cstddef>
#include <cstring>

#include <defdbio/dbcore/globals_p1_12.hpp>
#include <defdbio/gendb/globals_p4_07.hpp>
#include <defmodel/hdrstats/ext_p4_07.hpp>
#include <defmodel/hdrstats/globals_p4_07.hpp>
#include <defmodel/hdrstats/object_stats.hpp>
#include <defmodel/layout/cls_14017a6c0.hpp>
#include <defmodel/layout/cls_1401dbfe0.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/dynary_table.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/ftn/helpers.hpp>
#include <defrt/ftnstr/ftnstr.hpp>
#include <defrt/stdlib/refcount.hpp>

namespace def::dbio::gendb {

void wire_p4_07_externals(); // p4_07_wiring.cpp (Phase 4 gate)
namespace {
const bool g_p4_07_wired = (wire_p4_07_externals(), true);
} // namespace

namespace {

using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;
namespace p407 = def::model::p407;
namespace ext = def::model::p407::ext;

template <class T>
T& at(void* base, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<std::byte*>(base) + off);
}

std::int32_t slot(std::uint64_t addr) { return p407::hdr_slot(addr); }

// NADS1, 0x140b48ec0: 140 words inside p1-12's DbCommon; nads1()[k - 1] is NADS1(k)
std::int32_t* nads1()
{
    return def::dbio::dbcore::g_db.words() + (0x140b48ec0 - 0x140b45b80) / 4;
}

// min / max calls of 1401e2180: (code, comp, init) into blk[lo], blk[hi]
void minmax(LLOBJ_DATA* obj, std::int32_t code, std::int32_t comp, bool init, double* lo, double* hi)
{
    p407::obj_min_max(obj, &code, lo, hi, &comp, &init);
}

void stress(LLOBJ_DATA* obj, std::int32_t kind, double* lo, double* hi)
{
    const std::int32_t code = 500;
    const std::int32_t comp = 0;
    const bool init = true;
    p407::obj_stress_min_max(obj, &code, &kind, lo, hi, &comp, &init);
}

} // namespace

// @src 1401c56a0
std::int32_t write_version(const DB_STEP_DATA* m) { return m->version * 10; }

// @src 1401e1f30
void header_block_sizes(const std::int32_t* /*dim*/, std::int32_t* first, std::int32_t* stride)
{
    *first = 100;
    *stride = 400;
}

// @src 1401e2180
void fill_step_header(def::model::Cls_1401dbfe0* db, void* /*unused2*/, std::int32_t* hsize,
                      double* h, std::int32_t /*unused5*/, def::model::Cls_1401e7b10* sim,
                      const std::int32_t* nobj, const std::int32_t* fold,
                      def::model::Cls_14017a6c0* objects, const std::int32_t* dim,
                      const std::int32_t* /*version*/)
{
    const bool is2 = *dim == 2;
    const bool is3 = *dim == 3;
    p407::load_hdr_slots(dim); // 1401c7f80
    std::int32_t first = 0;
    std::int32_t stride = 0;
    header_block_sizes(dim, &first, &stride);
    *hsize = first + stride * *nobj;
    for (std::int32_t i = 0; i < *hsize; i = i + 1) h[i] = 0.0;
    h[0] = static_cast<double>(*nobj);
    h[1] = static_cast<double>(at<std::int32_t>(sim, 0x168));
    h[2] = at<double>(sim, 0x640);
    h[3] = at<double>(sim, 0x640);
    h[4] = static_cast<double>(at<std::int32_t>(sim, 0xe4));
    bool carry = true;
    if (-1 < db->uae0 && db->uad8 != nullptr && db->uae0 < db->uac8 &&
        at<std::int32_t>(db->uad8, 0x14 + static_cast<std::size_t>(db->uae0) * 0x28) !=
            at<std::int32_t>(sim, 0xe4)) {
        carry = false;
    }
    h[slot(0x1406e8524)] = static_cast<double>(ext::fun_140324000());
    if (is3) h[8] = static_cast<double>(*fold);
    h[0xc] = static_cast<double>(first);
    h[9] = static_cast<double>(stride);
    h[10] = static_cast<double>(at<std::int32_t>(sim, 0xe8));
    h[0xb] = static_cast<double>(at<std::int32_t>(sim, 0xec));
    def::rt::to_fortran(reinterpret_cast<char*>(h + 0x10), &at<char>(sim, 0x51), 0x40);
    def::rt::to_fortran(reinterpret_cast<char*>(h + 0x18), &at<char>(sim, 0x92), 0x50);
    for (std::int32_t i = 0; i < *nobj; i = i + 1) {
        LLOBJ_DATA* obj = objects->at(i + 1); // 14017aa80
        if (obj == nullptr || !ext::fun_140160f10(obj)) continue;
        double* blk = h + (first + stride * i);
        if (-1 < db->uae0 && db->uae8 != nullptr && carry &&
            i < at<std::int32_t>(db->uae8, static_cast<std::size_t>(db->uae0) * 0xe0)) {
            const std::int32_t n = stride < 400 ? stride : 400;
            std::byte* cached = at<std::byte*>(db->uae8, 0x20 + static_cast<std::size_t>(db->uae0) * 0xe0);
            std::memcpy(blk, cached + static_cast<std::ptrdiff_t>(i) * 0xcd0,
                        static_cast<std::size_t>(static_cast<std::int64_t>(n) << 3));
        }
        blk[slot(0x1406e852c)] = at<double>(obj, 0x22d0);
        blk[slot(0x1406e8530)] = at<double>(obj, 0x22d8);
        if (is3) blk[slot(0x1406e8534)] = at<double>(obj, 0x22e0);
        blk[slot(0x1406e85d0)] = at<double>(obj, 0x2300);
        blk[slot(0x1406e85d4)] = at<double>(obj, 0x2308);
        if (is3) blk[slot(0x1406e85d8)] = at<double>(obj, 0x2310);
        blk[slot(0x1406e85dc)] = at<double>(obj, 0x22e8);
        blk[slot(0x1406e85e0)] = at<double>(obj, 0x22f0);
        if (is3) blk[slot(0x1406e85e4)] = at<double>(obj, 0x22f8);
        def::rt::to_fortran(reinterpret_cast<char*>(blk + slot(0x1406e8528)), obj->name, 0x40);
        if (obj->u87c[0] == 0 || ext::fun_140160c20(obj)) continue;
        blk[slot(0x1406e8538)] = static_cast<double>(obj->u87c[1]); // +0x880 nodes
        blk[slot(0x1406e853c)] = static_cast<double>(obj->u87c[2]); // +0x884 elements
        if (is2) minmax(obj, 0x640, 0, true, blk + slot(0x1406e8608), blk + slot(0x1406e860c));
        minmax(obj, 700, 0, true, blk + slot(0x1406e8540), blk + slot(0x1406e8544));
        minmax(obj, 800, 0, true, blk + slot(0x1406e8548), blk + slot(0x1406e854c));
        minmax(obj, 600, 0, true, blk + slot(0x1406e8550), blk + slot(0x1406e8554));
        minmax(obj, 200, 0, true, blk + slot(0x1406e8560), blk + slot(0x1406e8564));
        double scratch; // local_170: the unused max of kind 4 and the unused min of kind 2
        stress(obj, 0, blk + slot(0x1406e8568), blk + slot(0x1406e856c));
        stress(obj, 1, blk + slot(0x1406e861c), blk + slot(0x1406e8620));
        stress(obj, 4, blk + slot(0x1406e8614), &scratch);
        stress(obj, 2, &scratch, blk + slot(0x1406e8618));
        std::int32_t code = 500;
        p407::obj_component_min_max(obj, &code, blk + slot(0x1406e8680));
        minmax(obj, 300, 0, true, blk + slot(0x1406e8570), blk + slot(0x1406e8574));
        if (at<std::int32_t>(sim, 0x13c) == 1 || at<std::int32_t>(sim, 0x13c) == 2)
            minmax(obj, 0x136, 0, false, blk + slot(0x1406e8570), blk + slot(0x1406e8574));
        code = 300;
        p407::obj_component_min_max(obj, &code, blk + slot(0x1406e867c));
        minmax(obj, 400, 0, true, blk + slot(0x1406e8578), blk + slot(0x1406e857c));
        code = 400;
        p407::obj_component_min_max(obj, &code, blk + slot(0x1406e8684));
        minmax(obj, 900, 1, true, blk + slot(0x1406e8580), blk + slot(0x1406e8584));
        minmax(obj, 900, 2, true, blk + slot(0x1406e8588), blk + slot(0x1406e858c));
        minmax(obj, 1000, 1, true, blk + slot(0x1406e8590), blk + slot(0x1406e8594));
        minmax(obj, 1000, 2, true, blk + slot(0x1406e8598), blk + slot(0x1406e859c));
        minmax(obj, 100, 0, true, blk + slot(0x1406e85a0), blk + slot(0x1406e85a4));
        if (at<std::int32_t>(sim, 0x138) == 1 || at<std::int32_t>(sim, 0x138) == 2)
            minmax(obj, 0x6e, 0, false, blk + slot(0x1406e85a0), blk + slot(0x1406e85a4));
        minmax(obj, 0xa28, 0, true, blk + slot(0x1406e85a8), blk + slot(0x1406e85ac));
        minmax(obj, 0xaf0, 1, true, blk + slot(0x1406e85b0), blk + slot(0x1406e85b4));
        minmax(obj, 0xaf0, 2, true, blk + slot(0x1406e85b8), blk + slot(0x1406e85bc));
        minmax(obj, 5000, 0, true, blk + slot(0x1406e85c0), blk + slot(0x1406e85c4));
        // In 2D DAT_1406e85cc is 0: the area (0.0 there) overwrites FORCE x (slot 0), as in DEF_PRE.
        blk[slot(0x1406e85cc)] = p407::obj_surface_area(obj, 1);
        blk[slot(0x1406e85c8)] = ext::fun_140163540(obj, 1);
    }
}

// @src 1401c5020
void write_step_header(DB_STEP_DATA* m, void* unused2, const std::int32_t* pos, std::int32_t* hsize,
                       def::model::Cls_1401dbfe0* db, const char* use_version)
{
    std::int32_t d = def::model::llobj::int_at_1c(m); // 140158e90
    def::model::select_index_tables(&d);             // 1401c7d40
    std::int32_t dim = def::model::llobj::int_at_1c(m);
    std::int32_t version = ext::fun_1401cc180(m);
    if (*use_version != '\0') version = write_version(m);
    std::int32_t fold = ext::fun_14019a2f0(m);
    fill_step_header(db, unused2, hsize, static_cast<double*>(m->u060), *pos, &m->sim_ctrl,
                     &m->u074[2], &fold, &m->objects, &dim, &version);
    std::int32_t wv = write_version(m);
    using def::model::g_mar_index;
    std::int32_t n1 = m->u1b90[g_mar_index[1]] - m->u1b90[g_mar_index[0]];
    std::int32_t n2 = m->u1b90[g_mar_index[2]] - m->u1b90[g_mar_index[1]];
    if (!def::rt::dimension_is_3(m)) {
        set_mar_table_2d(&wv, m->u1b90, &n1);
        set_a_table_2d(&wv, m->u1dc0, &n2);
    } else {
        set_mar_table_3d(&wv, m->u1b90, &n1);
        set_a_table_3d(&wv, m->u1dc0, &n2);
        build_mar_regions();
        build_a_regions();
    }
}

// @src 1403b3bcc
void set_mar_table_2d(const std::int32_t* /*version*/, const std::int32_t* mar, const std::int32_t* n1)
{
    def::rt::ftn::copy_words4(mar, nads1(), n1); // 140238025
}

// @src 1403b3bfd
void set_a_table_2d(const std::int32_t* /*version*/, const std::int32_t* a, const std::int32_t* n2)
{
    def::rt::ftn::copy_words4(a, g_nads2, n2);
}

// @src 1403b3c2e
void set_mar_table_3d(const std::int32_t* version, const std::int32_t* mar, const std::int32_t* n1)
{
    if (3000 <= *version) {
        def::rt::ftn::copy_words4(mar, nads1(), n1);
    } else {
        g_old_tables[0] = 0x14; // 0x140707310
        g_old_tables[1] = 0x32; // 0x140707314
        def::rt::ftn::copy_words4(mar, nads1(), n1);
    }
}

// @src 1403b3c9b
void set_a_table_3d(const std::int32_t* version, const std::int32_t* a, const std::int32_t* n2)
{
    if (3000 <= *version) {
        def::rt::ftn::copy_words4(a, g_nads2, n2);
        return;
    }
    // word w of the block is 0x140707310 + 4 w
    std::int32_t n = 2;
    def::rt::ftn::copy_words4(a, g_nads2, &n);
    n = 6;
    def::rt::ftn::copy_words4(a + 2, &g_old_tables[2], &n);    // 0x140707318
    n = 0x1e;
    def::rt::ftn::copy_words4(a + 8, &g_old_tables[3], &n);    // 0x14070731c
    g_old_tables[4] = a[0x98 / 4];                              // 0x140707320
    n = 2;
    def::rt::ftn::copy_words4(a + 0xa4 / 4, &g_old_tables[5], &n); // 0x140707324
    g_old_tables[6] = 2;     // 0x140707328
    g_old_tables[7] = 0x32;  // 0x14070732c
    g_old_tables[8] = 0x32;
    g_old_tables[9] = 0x32;
    g_old_tables[10] = 0x32;
    g_old_tables[11] = 0x32; // 0x14070733c
    if (*version < 0x8fc) {
        g_old_tables[12] = 0x32; // 0x140707340
    } else if (*version == 0x8fc) {
        g_old_tables[12] = 200;
    }
    g_old_tables[13] = 0x14; // 0x140707344
    g_old_tables[14] = 3;
    g_old_tables[15] = 0x14;
    g_old_tables[16] = 3;
    g_old_tables[17] = 3;
    g_old_tables[18] = 3;
    g_old_tables[19] = 3;    // 0x14070735c
    g_old_tables[20] = 0;    // 0x140707360
    g_old_tables[21] = 3;
    g_old_tables[22] = 3;
    g_old_tables[23] = 3;
    g_old_tables[24] = 3;
    g_old_tables[25] = 3;    // 0x140707374
    g_old_tables[26] = 1;    // 0x140707378
    g_old_tables[27] = 2;
    g_old_tables[28] = 6;
    g_old_tables[29] = 6;
    g_old_tables[30] = 6;    // 0x140707388
    g_old_tables[31] = 1;
    g_old_tables[32] = 1;
    g_old_tables[33] = 1;
    g_old_tables[34] = 1;
    g_old_tables[35] = 1;    // 0x14070739c
    g_old_tables[36] = 2;    // 0x1407073a0
    g_old_tables[37] = 6;
    g_old_tables[38] = 2;    // 0x1407073a8
}

namespace {

// The body shared by 1403b973a and 1403b9ad6 (entry 1403b99c9 / 1403b9ae2): table(1) = count,
// table(2) = longest region, region k: table(4k - 1) = nads(k + 1) - nads(k), table(4k) = nads(k).
// The loop counters 0x1407074d8 / 0x1407074dc (0x1407074e0 / e4) are locals.
void build_regions(const std::int32_t* nads, std::int32_t* table, std::int32_t count)
{
    table[0] = count;
    table[1] = 0;
    for (std::int32_t k = 1; k <= count; k = k + 1) {
        const std::int32_t start = nads[k - 1];
        table[4 * k - 2] = nads[k] - nads[k - 1];
        table[4 * k - 1] = start;
        if (table[4 * k - 2] > table[1]) table[1] = table[4 * k - 2];
    }
}

} // namespace

// @src 1403b973a
void build_mar_regions() { build_regions(nads1(), def::dbio::dbcore::g_db.mar_regions, 0x3a); }

// @src 1403b9ad6
void build_a_regions() { build_regions(g_nads2, def::dbio::dbcore::g_db.a_regions, 0x7e); }

void reset_globals_p4_07()
{
    for (std::int32_t& w : g_nads2) w = 0;
    for (std::int32_t& w : g_old_tables) w = 0;
}

} // namespace def::dbio::gendb
