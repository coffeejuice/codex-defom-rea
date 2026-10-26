// ctest-labels: p4-07
// Tier-1 tests of the GENDB step-header writer and the op-4 address tables (packet p4-07):
// 1401c5020, 1401c56a0, 1401e1f30, 1401e2180, 1403b3bcc .. 1403b9ad6. Expected values come from
// the decompiled stores (docs/re/dbmap/stepdriver.md §2.3) and are worked out by hand here.
#define _CRT_SECURE_NO_WARNINGS
#include <bit>
#include <cstdint>
#include <cstring>
#include <new>
#include <string>
#include <vector>

#include <defdbio/dbcore/globals_p1_12.hpp>
#include <defdbio/gendb/globals_p4_07.hpp>
#include <defdbio/gendb/step_header.hpp>
#include <defmodel/hdrstats/ext_p4_07.hpp>
#include <defmodel/hdrstats/globals_p4_07.hpp>
#include <defmodel/layout/cls_14017a6c0.hpp>
#include <defmodel/layout/cls_1401dbfe0.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/step/db_step.hpp>
#include <testkit/check.hpp>

using def::model::Cls_14017a6c0;
using def::model::Cls_1401dbfe0;
using def::model::Cls_1401e7b10;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;
namespace gendb = def::dbio::gendb;
namespace p407 = def::model::p407;
using def::dbio::dbcore::g_db;

namespace {

template <class T>
T& at(void* base, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<std::byte*>(base) + off);
}

struct Raw {
    explicit Raw(std::size_t n) : p(static_cast<std::byte*>(::operator new(n, std::align_val_t{16})))
    {
        std::memset(p, 0, n);
    }
    ~Raw() { ::operator delete(p, std::align_val_t{16}); }
    Raw(const Raw&) = delete;
    Raw& operator=(const Raw&) = delete;
    std::byte* p;
};

std::int32_t* nads1() { return g_db.words() + (0x140b48ec0 - 0x140b45b80) / 4; }

// 8 characters of a header word
std::string chars(const double* h, int i, int n)
{
    return std::string(reinterpret_cast<const char*>(h + i), static_cast<std::size_t>(n) * 8);
}

// A model with objects that have no mesh (no statistics), and the sim-control block.
struct Setup {
    Raw sim{sizeof(Cls_1401e7b10)};
    Raw db{sizeof(Cls_1401dbfe0)};
    Raw list{sizeof(Cls_14017a6c0)};
    Raw model{sizeof(DB_STEP_DATA)};
    std::vector<std::unique_ptr<Raw>> objs;

    Setup(int nobj, int dim)
    {
        at<std::int32_t>(model.p, 0x1c) = dim;
        at<std::int32_t>(sim.p, 0x168) = 2;          // PDIE
        at<double>(sim.p, 0x640) = 1.25;             // TNOW
        at<std::int32_t>(sim.p, 0xe4) = 3;           // CURSIM 1..3
        at<std::int32_t>(sim.p, 0xe8) = 4;
        at<std::int32_t>(sim.p, 0xec) = 5;
        std::strcpy(&at<char>(sim.p, 0x51), "OPERATION A");
        std::strcpy(&at<char>(sim.p, 0x92), "SIM NAME");
        dbo()->uae0 = -1;
        for (int k = 0; k < nobj; ++k) {
            objs.push_back(std::make_unique<Raw>(sizeof(LLOBJ_DATA)));
            LLOBJ_DATA* o = obj(k);
            o->owner = reinterpret_cast<DB_STEP_DATA*>(model.p);
            at<std::int32_t>(o, 0xc) = 1;
            std::strcpy(o->name, ("OBJ" + std::to_string(k + 1)).c_str());
            for (int i = 0; i < 9; ++i) at<double>(o, 0x22d0 + 8 * static_cast<std::size_t>(i)) = 10.0 * (k + 1) + i;
            if (k > 0) obj(k - 1)->next = o;
        }
        objects()->head = nobj > 0 ? obj(0) : nullptr;
        objects()->count = nobj;
        p407::ext::fun_140160f10 = [](const LLOBJ_DATA* o) { return at<std::int32_t>(const_cast<LLOBJ_DATA*>(o), 0xc) != 7; };
        p407::ext::fun_140324000 = []() { return 2; };
    }
    ~Setup()
    {
        p407::reset_ext_p4_07();
        p407::reset_globals_p4_07();
    }
    LLOBJ_DATA* obj(int k) { return reinterpret_cast<LLOBJ_DATA*>(objs[static_cast<std::size_t>(k)]->p); }
    Cls_1401dbfe0* dbo() { return reinterpret_cast<Cls_1401dbfe0*>(db.p); }
    Cls_14017a6c0* objects() { return reinterpret_cast<Cls_14017a6c0*>(list.p); }
    Cls_1401e7b10* simc() { return reinterpret_cast<Cls_1401e7b10*>(sim.p); }
};

} // namespace

TEST_CASE(block_sizes_and_version)
{
    std::int32_t dim = 2, f = 0, s = 0;
    gendb::header_block_sizes(&dim, &f, &s);
    CHECK_EQ(f, 100);
    CHECK_EQ(s, 400);
    Raw model(sizeof(DB_STEP_DATA));
    reinterpret_cast<DB_STEP_DATA*>(model.p)->version = 1100;
    CHECK_EQ(gendb::write_version(reinterpret_cast<DB_STEP_DATA*>(model.p)), 11000);
}

TEST_CASE(header_global_part_3d)
{
    Setup s(2, 3);
    std::vector<double> h(900, -1.0);
    std::int32_t hsize = 0, nobj = 2, fold = 1, dim = 3, ver = 11000;
    gendb::fill_step_header(s.dbo(), nullptr, &hsize, h.data(), 0, s.simc(), &nobj, &fold, s.objects(), &dim, &ver);
    CHECK_EQ(hsize, 900);             // 100 + 400 * 2
    CHECK_BITS(h[0], 2.0);            // NOBJ
    CHECK_BITS(h[1], 2.0);            // PDIE
    CHECK_BITS(h[2], 1.25);           // TNOW, twice
    CHECK_BITS(h[3], 1.25);
    CHECK_BITS(h[4], 3.0);            // CURSIM 1
    CHECK_BITS(h[5], 2.0);            // platform code (3D slot 5)
    CHECK_BITS(h[8], 1.0);            // fold flag, 3D only
    CHECK_BITS(h[9], 400.0);
    CHECK_BITS(h[10], 4.0);
    CHECK_BITS(h[11], 5.0);
    CHECK_BITS(h[12], 100.0);
    CHECK_BITS(h[6], 0.0);            // MAR / A lengths: written by op 4, zero here
    CHECK_BITS(h[13], 0.0);
    CHECK_EQ(chars(h.data(), 16, 8), std::string("OPERATION A") + std::string(64 - 11, ' '));
    CHECK_EQ(chars(h.data(), 24, 10), std::string("SIM NAME") + std::string(80 - 8, ' '));
    // object 2's block at 500: copies of STROKE (3..5), FORCE (0..2), speed (6..8) and the name
    const double* b = h.data() + 500;
    CHECK_BITS(b[3], 20.0);           // obj+0x22d0
    CHECK_BITS(b[5], 22.0);           // obj+0x22e0 (z, 3D)
    CHECK_BITS(b[0], 26.0);           // obj+0x2300
    CHECK_BITS(b[2], 28.0);           // obj+0x2310
    CHECK_BITS(b[6], 23.0);           // obj+0x22e8
    CHECK_BITS(b[8], 25.0);           // obj+0x22f8
    CHECK_EQ(chars(b, 106, 8), std::string("OBJ2") + std::string(60, ' '));
    CHECK_BITS(b[88], 0.0);           // no mesh: no counts, no statistics
    CHECK_BITS(b[115], 0.0);
}

TEST_CASE(header_2d_skips_z_and_type_7)
{
    Setup s(2, 2);
    at<std::int32_t>(s.obj(1), 0xc) = 7; // 140160f10: type 7 objects are skipped
    std::vector<double> h(900, -1.0);
    std::int32_t hsize = 0, nobj = 2, fold = 1, dim = 2, ver = 11000;
    gendb::fill_step_header(s.dbo(), nullptr, &hsize, h.data(), 0, s.simc(), &nobj, &fold, s.objects(), &dim, &ver);
    CHECK_BITS(h[8], 2.0);            // 2D: platform code at 8, no fold flag
    CHECK_BITS(h[5], 0.0);
    const double* b = h.data() + 100;
    CHECK_BITS(b[2], 10.0);           // STROKE x, y at 2, 3
    CHECK_BITS(b[3], 11.0);
    CHECK_BITS(b[0], 16.0);           // FORCE x, y at 0, 1
    CHECK_BITS(b[1], 17.0);
    CHECK_BITS(b[4], 13.0);           // speed x, y at 4, 5
    CHECK_BITS(b[5], 14.0);
    CHECK_EQ(chars(b, 34, 8), std::string("OBJ1") + std::string(60, ' '));
    for (int i = 0; i < 400; ++i) CHECK_BITS(h[500 + static_cast<std::size_t>(i)], 0.0); // type 7: untouched zeros
}

TEST_CASE(header_carries_previous_blocks_of_the_same_simulation)
{
    Setup s(2, 3);
    // header cache entry 0 (0xe0 bytes): NOBJ 1, object blocks of 0xcd0 bytes at +0x20
    std::vector<std::byte> cache(0xe0, std::byte{0});
    std::vector<double> blocks(0xcd0 / 8, 0.0);
    blocks[9] = 77.0;   // torque, a slot the writer does not compute
    blocks[3] = -1.0;   // STROKE x: overwritten by the copy
    at<std::int32_t>(cache.data(), 0) = 1;
    at<double*>(cache.data(), 0x20) = blocks.data();
    // directory entries (0x28 bytes), SimNo at +0x14
    std::vector<std::byte> dir(0x28, std::byte{0});
    at<std::int32_t>(dir.data(), 0x14) = 3;
    s.dbo()->uae0 = 0;
    s.dbo()->uac8 = 1;
    s.dbo()->uad8 = dir.data();
    s.dbo()->uae8 = cache.data();
    std::vector<double> h(900, -1.0);
    std::int32_t hsize = 0, nobj = 2, fold = 0, dim = 3, ver = 11000;
    gendb::fill_step_header(s.dbo(), nullptr, &hsize, h.data(), 0, s.simc(), &nobj, &fold, s.objects(), &dim, &ver);
    CHECK_BITS(h[100 + 9], 77.0);     // carried over
    CHECK_BITS(h[100 + 3], 10.0);     // recomputed copy wins
    CHECK_BITS(h[500 + 9], 0.0);      // object 2 >= cached NOBJ: no carry
    // another simulation number: nothing carried
    at<std::int32_t>(dir.data(), 0x14) = 9;
    gendb::fill_step_header(s.dbo(), nullptr, &hsize, h.data(), 0, s.simc(), &nobj, &fold, s.objects(), &dim, &ver);
    CHECK_BITS(h[100 + 9], 0.0);
}

TEST_CASE(address_tables_2d)
{
    def::dbio::dbcore::reset_globals();
    gendb::reset_globals_p4_07();
    const std::int32_t mar[5] = {1, 141, 421, 457, 607};
    const std::int32_t a[3] = {1, 5, 9};
    std::int32_t ver = 11000, n1 = 4, n2 = 3;
    gendb::set_mar_table_2d(&ver, mar, &n1);
    gendb::set_a_table_2d(&ver, a, &n2);
    CHECK_EQ(nads1()[0], 1);
    CHECK_EQ(nads1()[3], 457);
    CHECK_EQ(nads1()[4], 0); // only *n1 words
    CHECK_EQ(gendb::g_nads2[2], 9);
    CHECK_EQ(gendb::g_nads2[3], 0);
}

TEST_CASE(region_tables_from_nads)
{
    def::dbio::dbcore::reset_globals();
    gendb::reset_globals_p4_07();
    for (int k = 0; k < 59; ++k) nads1()[k] = 1 + k * k; // region k (1-based) length (k+1)^2 - k^2 = 2k + 1... from 0
    gendb::build_mar_regions();
    std::int32_t* t = g_db.mar_regions;
    CHECK_EQ(t[0], 0x3a);
    CHECK_EQ(t[2], 1);        // region 1: length nads(2) - nads(1) = 2 - 1
    CHECK_EQ(t[3], 1);        //           start nads(1)
    CHECK_EQ(t[6], 3);        // region 2: 5 - 2
    CHECK_EQ(t[7], 2);
    CHECK_EQ(t[4 * 58 - 2], 58 * 58 - 57 * 57); // region 58
    CHECK_EQ(t[1], 115);      // longest: region 58, 2 * 57 + 1
    for (int k = 0; k < 127; ++k) gendb::g_nads2[k] = 10 * k;
    gendb::build_a_regions();
    CHECK_EQ(g_db.a_regions[0], 0x7e);
    CHECK_EQ(g_db.a_regions[1], 10);
    CHECK_EQ(g_db.a_regions[4 * 126 - 1], 1250); // start of region 126
}

TEST_CASE(old_3d_tables_below_version_3000)
{
    def::dbio::dbcore::reset_globals();
    gendb::reset_globals_p4_07();
    std::vector<std::int32_t> a(64);
    for (int k = 0; k < 64; ++k) a[static_cast<std::size_t>(k)] = 100 + k;
    std::int32_t ver = 0x8fc, n = 7;
    gendb::set_mar_table_3d(&ver, a.data(), &n);
    CHECK_EQ(gendb::g_old_tables[0], 0x14);
    CHECK_EQ(gendb::g_old_tables[1], 0x32);
    CHECK_EQ(nads1()[6], 106);
    gendb::set_a_table_3d(&ver, a.data(), &n);
    CHECK_EQ(gendb::g_nads2[0], 100);
    CHECK_EQ(gendb::g_nads2[1], 101);
    CHECK_EQ(gendb::g_nads2[2], 0);   // only 2 words below 3000
    CHECK_EQ(gendb::g_old_tables[2], 102);  // 0x140707318 <- a[2] (then words 3.. overwritten)
    CHECK_EQ(gendb::g_old_tables[3], 108);  // 0x14070731c <- a[8..37], later words overwritten
    CHECK_EQ(gendb::g_old_tables[4], 138);  // 0x140707320 = a[38]
    CHECK_EQ(gendb::g_old_tables[5], 141);  // 0x140707324 <- a[41], a[42]
    CHECK_EQ(gendb::g_old_tables[6], 2);
    CHECK_EQ(gendb::g_old_tables[12], 200); // version 0x8fc
    CHECK_EQ(gendb::g_old_tables[20], 0);
    CHECK_EQ(gendb::g_old_tables[38], 2);
    ver = 3000;
    gendb::reset_globals_p4_07();
    gendb::set_a_table_3d(&ver, a.data(), &n);
    CHECK_EQ(gendb::g_nads2[6], 106);
    CHECK_EQ(gendb::g_old_tables[6], 0);
}

TEST_CASE(write_step_header_loads_the_tables)
{
    def::dbio::dbcore::reset_globals();
    gendb::reset_globals_p4_07();
    Setup s(1, 3);
    DB_STEP_DATA* m = reinterpret_cast<DB_STEP_DATA*>(s.model.p);
    m->version = 1100;
    m->u074[2] = 1;  // NOBJ (m+0x7c)
    std::vector<double> h(500, 0.0);
    m->u060 = h.data();
    // sim control and object list live inside the model here
    std::memcpy(&m->sim_ctrl, s.sim.p, sizeof(Cls_1401e7b10));
    m->objects.head = s.obj(0);
    m->objects.count = 1;
    for (int k = 0; k < 0x8c; ++k) m->u1b90[k] = 1 + 10 * k;
    for (int k = 0; k < 0x118; ++k) m->u1dc0[k] = 5 + k;
    p407::ext::fun_1401cc180 = [](DB_STEP_DATA*) { return 9999; };
    p407::ext::fun_14019a2f0 = [](DB_STEP_DATA*) { return 0; };
    std::int32_t pos = 1, hsize = 0;
    const char use_version = 1;
    gendb::write_step_header(m, nullptr, &pos, &hsize, s.dbo(), &use_version);
    CHECK_EQ(hsize, 500);
    CHECK_BITS(h[0], 1.0);
    // 3D index tables: MAR slots g_mar_index[0..2]; I1 = u1b90[i1] - u1b90[i0]
    using def::model::g_mar_index;
    const std::int32_t n1 = m->u1b90[g_mar_index[1]] - m->u1b90[g_mar_index[0]];
    CHECK(n1 > 0);
    CHECK_EQ(nads1()[0], m->u1b90[0]);
    CHECK_EQ(nads1()[n1 - 1], m->u1b90[n1 - 1]);
    CHECK_EQ(gendb::g_nads2[0], 5);
    CHECK_EQ(g_db.mar_regions[0], 0x3a); // 3D: region tables built
    CHECK_EQ(g_db.a_regions[0], 0x7e);
}

int main() { return testkit::run_all(); }
