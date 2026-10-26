// Tier 1 for packet p4-06: the 2D object writer FUN_14017cb60, the 2D boundary-record writer
// FUN_140176830 and their helpers. The p4-05 array writers, movement and window writers and the P2
// geometry access are hooked (ext::g_hook): the test supplies geometry and records the calls.
// Expected values are worked out by hand from the decompile / listing in each case.
// ctest-labels: p4-06 gendb
#include <defdbio/gendb/ext_p4_06.hpp>
#include <defdbio/gendb/p4_06_obj2d.hpp>
#include <testkit/check.hpp>

#include <cstdint>
#include <cstring>
#include <map>
#include <memory>
#include <vector>

#include <defmodel/layout/obj_geo2d.hpp>

using namespace def::model;
using namespace def::dbio::gendb;

namespace {

template <class T>
struct Zeroed {
    std::unique_ptr<std::byte[]> buf{new std::byte[sizeof(T) + 16]()};
    T* get() { return reinterpret_cast<T*>((reinterpret_cast<std::uintptr_t>(buf.get()) + 15) & ~std::uintptr_t{15}); }
};

struct Call {
    std::uint64_t addr;
    std::vector<std::uint64_t> args;
    std::map<int, std::int32_t> at; // values behind int / char pointer arguments, read during the call
};

// pointer arguments whose target is read at call time: (address, index, is_char)
struct Capture {
    std::uint64_t addr;
    int idx;
    bool is_char;
};
const Capture kCapture[] = {
    {0x1401912a0, 1, false}, {0x1401912a0, 6, false}, {0x140174cd0, 3, false}, {0x140174cd0, 4, false},
    {0x140174cd0, 5, false}, {0x1401774f0, 5, false}, {0x1401774f0, 6, false}, {0x14016cf10, 9, true},
    {0x14018a760, 8, true},  {0x14018a760, 10, false}, {0x140196840, 8, true}, {0x140196840, 9, true},
    {0x140196840, 10, true}, {0x140196840, 12, false}, {0x14016d200, 9, true}, {0x140187a60, 6, false},
    {0x14017e190, 4, false},
};

struct World {
    std::vector<Call> calls;
    std::map<std::pair<std::uint64_t, std::int32_t>, OBJ_GEO2D*> curves; // (obj, idx) -> curve
    std::map<std::uint64_t, std::int32_t> ncurves;                        // obj -> count
    std::map<std::uint64_t, double*> coords;                              // node sub-object -> xy
    std::map<std::uint64_t, std::int32_t*> conn;                          // element sub-object -> rows
    std::int32_t scratch = 5;
};
World* g_world = nullptr;

std::uint64_t hook(std::uint64_t addr, const std::uint64_t* a, int n)
{
    World& w = *g_world;
    switch (addr) {
    case 0x1401982c0: {
        const std::int32_t idx = *reinterpret_cast<const std::int32_t*>(a[1]);
        auto it = w.curves.find({a[0], idx});
        return it == w.curves.end() ? 0 : reinterpret_cast<std::uint64_t>(it->second);
    }
    case 0x140198310: return static_cast<std::uint64_t>(w.ncurves[a[0]]);
    case 0x140160cc0: return w.ncurves[a[0]] > 0 ? 1 : 0;
    case 0x1401808d0: return reinterpret_cast<std::uint64_t>(w.coords[a[0]]);
    case 0x140183cb0: return reinterpret_cast<std::uint64_t>(w.conn[a[0]]);
    case 0x140165230: return static_cast<std::uint64_t>(w.scratch);
    default: break;
    }
    Call c{addr, std::vector<std::uint64_t>(a, a + n), {}};
    for (const Capture& k : kCapture) {
        if (k.addr == addr && k.idx < n) {
            c.at[k.idx] = k.is_char ? *reinterpret_cast<const char*>(a[k.idx])
                                    : *reinterpret_cast<const std::int32_t*>(a[k.idx]);
        }
    }
    w.calls.push_back(std::move(c));
    return 0;
}

std::uint64_t u(const void* p) { return reinterpret_cast<std::uintptr_t>(p); }

} // namespace

TEST_CASE(pow_int_binary_exponentiation)
{
    CHECK_BITS(pow_int(10.0, 0), 1.0);
    CHECK_BITS(pow_int(10.0, 1), 10.0);
    CHECK_BITS(pow_int(10.0, 3), 1000.0);
    CHECK_BITS(pow_int(2.0, -2), 0.25); // 1 / (2 * 2)
    CHECK_BITS(pow_int_impl(3.0, 5), 243.0);
}

TEST_CASE(bnd_row_cursor_get_set)
{
    Zeroed<DB_STEP_DATA> zm;
    DB_STEP_DATA* m = zm.get();
    std::int32_t v = 42;
    gendb_set_bnd_row_cursor(m, &v);
    CHECK_EQ(field<std::int32_t>(m, 0x120), 42);
    CHECK_EQ(gendb_bnd_row_cursor(m), 42);
}

TEST_CASE(principal_record_search)
{
    Zeroed<LLOBJ_DATA> zo;
    LLOBJ_DATA* o = zo.get();
    std::vector<double> llbnd(10 * 4, 0.0);
    std::int32_t w = 10;
    field<std::int32_t>(o, 0x3588) = 2;
    field<std::int32_t>(o, 0x358c) = 2;
    CHECK_EQ(find_principal_record(o, llbnd.data(), &w), 2); // single record: returned as is
    field<std::int32_t>(o, 0x358c) = 4;
    CHECK_EQ(find_principal_record(o, llbnd.data(), &w), 0); // none flagged
    llbnd[(3 - 1) * 10 + 1] = 1.0;
    llbnd[(4 - 1) * 10 + 1] = 1.0;
    CHECK_EQ(find_principal_record(o, llbnd.data(), &w), 3); // first flagged record
}

TEST_CASE(boundary_box_skips_repeated_nodes)
{
    std::int32_t nodes[5] = {1, 2, 2, 3, 1};
    std::byte bnd[0x30] = {};
    field<std::int32_t>(bnd, 0) = 5;
    field<std::int32_t*>(bnd, 8) = nodes;
    const double xy[6] = {0.0, 5.0, -2.0, 1.0, 4.0, 3.0}; // node 1 (0,5), 2 (-2,1), 3 (4,3)
    double mn[2] = {9, 9}, mx[2] = {9, 9};
    CHECK(boundary_bounding_box(bnd, mn, mx, xy));
    // pairs (1,2) (2,2 skipped) (2,3) (3,1): points 1, 2, 3 -> min (-2, 1) max (4, 5)
    CHECK_BITS(mn[0], -2.0);
    CHECK_BITS(mn[1], 1.0);
    CHECK_BITS(mx[0], 4.0);
    CHECK_BITS(mx[1], 5.0);
    field<std::int32_t>(bnd, 0) = 0;
    CHECK(!boundary_bounding_box(bnd, mn, mx, xy));
}

TEST_CASE(write_objects_2d_two_objects)
{
    World w;
    g_world = &w;
    def::dbio::gendb::ext::g_hook = &hook;

    Zeroed<DB_STEP_DATA> zm;
    DB_STEP_DATA* m = zm.get();
    field<std::int32_t>(m, 0x1c) = 2;        // 2D
    field<std::int32_t>(m, 0x114) = 10;      // node counter
    field<std::int32_t>(m, 0x118) = 20;      // element counter
    field<std::int32_t>(m, 0x8b8) = 2;       // PDIE = object 2
    field<std::int32_t>(m, 0x750 + 0x104) = 1; // TRANS 1 (so 1401f3d90 is false)
    field<std::int32_t>(m, 0x750 + 0x108) = 1; // TRANS 2 (LOCDEF flag)
    m->objects.owner = m;

    // materials 3 and 7, both used -> slots 1 and 2
    Zeroed<Material> zm3, zm7;
    field<std::int32_t>(zm3.get(), 0) = 3;
    field<std::int32_t>(zm3.get(), 4) = 1;
    field<std::int32_t>(zm7.get(), 0) = 7;
    field<std::int32_t>(zm7.get(), 4) = 1;
    field<Material*>(zm3.get(), 0x2200) = zm7.get();
    m->materials.head = zm3.get();
    m->materials.tail = zm7.get();
    m->materials.count = 2;

    // object 1: plastic, meshed, one closed boundary 1-2-3-1, elements [1 2 5 4] [2 3 6 5]
    Zeroed<LLOBJ_DATA> z1, z2;
    LLOBJ_DATA* o1 = z1.get();
    LLOBJ_DATA* o2 = z2.get();
    field<std::int32_t>(o1, 8) = 1;
    field<std::int32_t>(o1, 0xc) = 1;
    field<DB_STEP_DATA*>(o1, 0x3a98) = m;
    field<std::int32_t>(o1, 0x87c) = 1;
    field<std::int32_t>(o1, 0x880) = 6;
    field<std::int32_t>(o1, 0x884) = 2;
    field<std::int32_t>(o1, 0x88c) = 3;
    field<std::int32_t>(o1, 0x894) = 4;
    field<std::int32_t>(o1, 0x9e0 + 0xec) = 4; // Cls_140182880::width (140185530)
    field<std::int32_t>(o1, 0xbd8) = 1;
    std::int32_t bnodes[4] = {1, 2, 3, 1};
    field<std::int32_t>(o1, 0xbe0) = 4;
    field<std::int32_t*>(o1, 0xbe8) = bnodes;
    double xy1[12] = {0, 0, 1, 0, 2, 0, 0, 1, 1, 1, 2, 1};
    field<double*>(o1, 0x8a8) = xy1;
    std::int32_t conn1[8] = {1, 2, 5, 4, 2, 3, 6, 5};
    w.coords[u(reinterpret_cast<std::byte*>(o1) + 0x8a0)] = xy1;
    w.conn[u(reinterpret_cast<std::byte*>(o1) + 0x9e0)] = conn1;

    // object 2: rigid, no mesh, two segment curves
    field<std::int32_t>(o2, 8) = 2;
    field<std::int32_t>(o2, 0xc) = 0;
    field<DB_STEP_DATA*>(o2, 0x3a98) = m;
    field<std::int32_t>(o2, 0x3588) = 2; // LLBND records 2..3 (written by 14016cf10, hooked here)
    field<std::int32_t>(o2, 0x358c) = 3;
    OBJ_GEO2D ca, cb;
    double rows_a[6] = {1, 0, 0, 1, 1, 0};
    double rows_b[12] = {1, -1, -1, 2, 2, 0, 1, 2, 2, 3, 0, 0};
    ca.curve_type = 2;
    ca.n_points = 1;
    ca.points = rows_a;
    cb.curve_type = 2;
    cb.n_points = 2;
    cb.points = rows_b;
    w.ncurves[u(o2)] = 2;
    w.curves[{u(o2), 0}] = &ca;
    w.curves[{u(o2), 1}] = &cb;

    m->objects.head = o1;
    field<LLOBJ_DATA*>(o1, 0x3940) = o2;
    m->objects.tail = o2;
    m->objects.count = 2;

    std::vector<std::int32_t> mar(2000, 0);
    std::vector<double> a(6000, 0.0);
    std::vector<std::int32_t> mt(140, 0), at(280, 0);
    mt[3] = 101; // IIOBJ
    mt[4] = 301; // IIBDY
    mt[5] = 501; // IIBEL
    mt[6] = 701; // slot 6
    mt[7] = 801; // element materials
    mt[8] = 1;   // IISIM
    mt[9] = 901;
    mt[73] = 20; // IIOBJ width
    at[5] = 1;     // LLOBJ
    at[6] = 1001;  // LLBND
    at[8] = 3001;  // LLGRY
    at[11] = 4001;
    at[145] = 100; // LLOBJ width
    at[146] = 100; // LLBND width
    at[149] = 7;   // LLWIN width
    mar[800] = 7; // element 1 material 7 -> slot 2
    mar[801] = 3; // element 2 material 3 -> slot 1
    mar[802] = 0;
    std::int32_t iifnc[4] = {};
    double llfnc[4] = {};
    double llwin[4] = {};
    double llwrz[4] = {};

    write_objects_2d(&m->objects, mar.data(), a.data(), mt.data(), at.data(), iifnc, llfnc, llwin, llwrz);

    // ---- object 1, boundary record 1 (LLBND record at offset 0) ----
    const double* r1 = a.data() + 1000;
    CHECK_BITS(r1[0], 3.0); // deformable
    CHECK_BITS(r1[1], 1.0); // principal (the only boundary)
    CHECK_BITS(r1[2], 4.0); // boundary node count
    CHECK_BITS(r1[3], 1.0); // first IIBDY row
    // edges (1,2) -> element 1, (2,3) -> element 2, (3,1) -> none -> 1; offsets +10 / +20
    CHECK_EQ(mar[300], 11);
    CHECK_EQ(mar[301], 12);
    CHECK_EQ(mar[302], 13);
    CHECK_EQ(mar[303], 11);
    CHECK_EQ(mar[500], 21);
    CHECK_EQ(mar[501], 22);
    CHECK_EQ(mar[502], 21);
    CHECK_EQ(field<std::int32_t>(m, 0x11c), 4); // last record: row cursor = element-row cursor
    CHECK_EQ(field<std::int32_t>(m, 0x120), 4);

    // ---- object 2: two geometry records (LLBND offsets 100 and 200) ----
    const double* r2 = a.data() + 1000 + 100;
    const double* r3 = a.data() + 1000 + 200;
    CHECK_BITS(r2[0], 1.0);
    CHECK_BITS(r2[1], 0.0);
    CHECK_BITS(r2[4], 1.0); // one row of curve 1
    CHECK_BITS(r2[5], 1.0); // first LLGRY word
    CHECK_BITS(r2[6], 2.0); // curve 0 is a segment curve
    CHECK_BITS(r3[0], 1.0);
    CHECK_BITS(r3[1], 1.0); // curve 2 holds the overall min and max (score 1111)
    CHECK_BITS(r3[4], 2.0);
    CHECK_BITS(r3[5], 7.0); // 1 * 6 + 1
    CHECK(std::memcmp(a.data() + 3000, rows_a, sizeof rows_a) == 0);
    CHECK(std::memcmp(a.data() + 3006, rows_b, sizeof rows_b) == 0);
    CHECK_EQ(field<std::int32_t>(m, 0x124), 3);
    CHECK_EQ(field<std::int32_t>(m, 0x110), 3);

    // ---- PDIE: IISIM words 35 and 9 = principal record of object 2 ----
    CHECK_EQ(mar[35], 3);
    CHECK_EQ(mar[9], 3);
    // ---- element materials renumbered to slots ----
    CHECK_EQ(mar[800], 2);
    CHECK_EQ(mar[801], 1);
    CHECK_EQ(mar[802], 0);

    // ---- call order and arguments ----
    std::vector<std::uint64_t> order;
    for (const Call& c : w.calls) {
        order.push_back(c.addr);
    }
    const std::vector<std::uint64_t> expect = {
        0x1401912a0, 0x14016f8e0, 0x140174cd0, 0x1401774f0, 0x140172c90, 0x1401707e0, 0x140178460,
        0x14016cf10, 0x14018a760, 0x140196840, 0x14016d200, 0x140187a60,
        0x1401912a0, 0x1401912a0, 0x14016cf10, 0x14018a760, 0x140196840, 0x14016d200, 0x140187a60, 0x14017e190};
    CHECK(order == expect);
    REQUIRE(w.calls.size() == expect.size());
    // movement of object 1 gets LLBND cursor 1, of object 2 cursors 2 and 3
    CHECK_EQ(w.calls[0].at[6], 1);
    CHECK_EQ(w.calls[0].args[0], u(reinterpret_cast<std::byte*>(o1) + 0x2160));
    CHECK_EQ(w.calls[13].at[6], 3);
    // 140174cd0: connectivity offset before this object (slot 6 start), element and node counters
    CHECK_EQ(w.calls[2].at[3], 701);
    CHECK_EQ(w.calls[2].at[4], 20);
    // 1401774f0: face offset = (int)word 3 of the object's first record - 1 = 0; edge count 3 - 2 + 2
    CHECK_EQ(w.calls[3].at[5], 0);
    CHECK_EQ(w.calls[3].at[6], 3);
    // 14016cf10 of object 1: mode 2 - 701, cursor 1, one boundary, meshed
    CHECK_EQ(static_cast<std::int32_t>(w.calls[7].args[3]), -699);
    CHECK_EQ(static_cast<std::int32_t>(w.calls[7].args[4]), 1);
    CHECK_EQ(static_cast<std::int32_t>(w.calls[7].args[5]), 1);
    CHECK_EQ(w.calls[7].at[9], 1);
    CHECK_EQ(w.calls[7].args[1], u(mar.data() + 100));
    CHECK_EQ(w.calls[7].args[2], u(a.data()));
    // object 1 window flags: remesh 1, WIN* 1, LOCDEF 1, WINPRS 1
    CHECK_EQ(w.calls[8].at[8], 1);
    CHECK_EQ(w.calls[9].at[8], 1);
    CHECK_EQ(w.calls[9].at[9], 1);
    CHECK_EQ(w.calls[9].at[10], 1);
    CHECK_EQ(static_cast<std::int32_t>(w.calls[9].args[6]), 7);
    // object 2 (second IIOBJ / LLOBJ record): cursor 3, two curves, not meshed, all flags 0
    CHECK_EQ(w.calls[14].args[1], u(mar.data() + 100 + 20));
    CHECK_EQ(w.calls[14].args[2], u(a.data() + 100));
    CHECK_EQ(static_cast<std::int32_t>(w.calls[14].args[4]), 3);
    CHECK_EQ(static_cast<std::int32_t>(w.calls[14].args[5]), 2);
    CHECK_EQ(w.calls[14].at[9], 0);
    CHECK_EQ(w.calls[16].at[8], 0);
    CHECK_EQ(w.calls[16].at[9], 0);
    CHECK_EQ(w.calls[16].at[10], 0);
    CHECK_EQ(w.calls[18].at[6], 2); // object id passed to 140187a60
    // 14017e190: A slot 11, MAR slot 9, LLBND, width
    CHECK_EQ(w.calls[19].args[1], u(a.data() + 4000));
    CHECK_EQ(w.calls[19].args[2], u(mar.data() + 900));
    CHECK_EQ(w.calls[19].at[4], 100);

    ca.points = nullptr; // not owned
    cb.points = nullptr;
    def::dbio::gendb::ext::g_hook = nullptr;
}

int main() { return testkit::run_all(); }
