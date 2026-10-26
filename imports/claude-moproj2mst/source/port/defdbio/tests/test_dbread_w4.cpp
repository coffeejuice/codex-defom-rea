// ctest-labels: p-dbread dbread
// Tier 1 for helper W4 of packet p-dbread (DBREAD 1401cc420 subtrees: fold meshes 14016d300 /
// 1402fa100, dynamic arrays 1403bc272 / 14025bd10 / 14025bdc0, LLTFM records 140336740, LLBRT
// records 1401f58a0, node BC arrays 140170410, BC function tables 140177ba0). Models and objects
// are zeroed byte buffers with the fields set by offset (as test_gendb3d_writers.cpp does); the
// expected values are worked out by hand from the decompile (comments). The end-to-end DB round
// trip is the lead's.
//
// Function loads go through W1's read_func_from_db_array (1401309b0) / read_table_rows_1d
// (140131ea0); the checks look at the DYNARY they fill (u54 = the IIFNC row's second word).
#include <defdbio/dbread/w4.hpp>
#include <testkit/check.hpp>

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <new>
#include <vector>

#include <defdbio/dbcore/globals_p1_12.hpp>
#include <defmodel/layout/cls_1401364a0.hpp>
#include <defmodel/layout/cls_1401f46e0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dyn_ary_linked_list.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/dynary_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/layout/phase_pair.hpp>
#include <defmodel/linked/axis_point_list.hpp>
#include <defmodel/mesh_gen/sf_list_ops.hpp>
#include <defmodel/step/db_step.hpp>


using namespace def::dbio::dbread;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;
using def::model::OBJ_GEO3D;

namespace {

struct Buf {
    std::vector<std::uint64_t> w;
    explicit Buf(std::size_t bytes) : w((bytes + 7) / 8, 0) {}
    template <class T> T* as() { return reinterpret_cast<T*>(w.data()); }
};

template <class T> T& fld(void* p, std::size_t off) { return *reinterpret_cast<T*>(static_cast<char*>(p) + off); }

// zeroed model of dimension dim, object owned by it
struct World {
    Buf mb{sizeof(DB_STEP_DATA)};
    Buf ob{sizeof(LLOBJ_DATA)};
    DB_STEP_DATA* m;
    LLOBJ_DATA* o;
    explicit World(std::int32_t dim)
    {
        m = mb.as<DB_STEP_DATA>();
        o = ob.as<LLOBJ_DATA>();
        fld<std::int32_t>(m, 0x1c) = dim;
        o->owner = m;
    }
};

OBJ_GEO3D* geo_at(LLOBJ_DATA* o, std::size_t off) { return reinterpret_cast<OBJ_GEO3D*>(reinterpret_cast<char*>(o) + off); }

} // namespace

// 1403bc272: NI = dyn_hdr[0], max int size [1], NR [10], max real size [11]
TEST_CASE(dynamic_array_counts_read_the_descriptor)
{
    auto& h = def::dbio::dbcore::g_db.dyn_hdr;
    h[0] = 3;
    h[1] = 7;
    h[10] = 5;
    h[11] = 9;
    std::int32_t ni = 0, nimax = 0, nr = 0, nrmax = 0;
    dynamic_array_counts(&ni, &nimax, &nr, &nrmax);
    CHECK_EQ(ni, 3);
    CHECK_EQ(nimax, 7);
    CHECK_EQ(nr, 5);
    CHECK_EQ(nrmax, 9);
    std::memset(h, 0, sizeof h);
}

// 1402fa100: src 5 nodes / 3 faces, split at node 3 and face 2 -> geo 2 nodes / 1 face,
// net 3 nodes / 2 faces with node numbers - 2 (4th: abs(.) - 2 when negating)
TEST_CASE(split_fold_mesh_renumbers_the_second_part)
{
    OBJ_GEO3D src, geo, net;
    std::int32_t nn = 5, ne = 3;
    src.resize_nodes(&nn);
    src.resize_elems(&ne);
    for (int k = 0; k < 15; ++k) src.coords.mutable_at(k) = 10.0 + k;
    const std::int32_t el[12] = {1, 2, 2, 2, 3, 4, 5, -5, 4, 5, 3, 5};
    for (int k = 0; k < 12; ++k) src.elems.mutable_at(k) = el[k];
    const std::int32_t first_node = 3, first_face = 2;
    const char negate = 1;
    split_fold_mesh(&src, &geo, &net, &first_node, &first_face, &negate);
    CHECK_EQ(geo.node_count(), 2);
    CHECK_EQ(geo.elem_count(), 1);
    CHECK_EQ(net.node_count(), 3);
    CHECK_EQ(net.elem_count(), 2);
    CHECK_EQ(geo.coords[5], 15.0);
    CHECK_EQ(net.coords[0], 16.0);
    CHECK_EQ(net.coords[8], 24.0);
    CHECK_EQ(geo.elems[3], 2);
    // face 2 (3,4,5,-5): (1,2,3, abs(-5) - 2 = 3); face 3 (4,5,3,5): (2,3,1,3)
    const std::int32_t want[8] = {1, 2, 3, 3, 2, 3, 1, 3};
    for (int k = 0; k < 8; ++k) CHECK_EQ(net.elems[k], want[k]);
}

// 14016d300: fold nodes 3..7 (base 2), faces 2..3. LLFLD start 11 (a_off 10), IIFLD start 1:
// faces = MAR + (1 + 2 * 4 - 5) = MAR + 4. Face (3,4,5,5) -> (1,2,3,3) FLDGEO; face (6,7,6,-7)
// -> (4,5,4,-5) FLWNET; min node = min(5, 4, 5, 4) = 4 -> split at node 4, face 2:
// FLDGEO 3 nodes / 1 face, FLWNET 2 nodes / 1 face (4-3, 5-3, 4-3, 5-3) = (1,2,1,2)
TEST_CASE(read_fold_info_splits_fldgeo_and_flwnet)
{
    World w(3);
    new (reinterpret_cast<char*>(w.o) + 0x3328) OBJ_GEO3D();
    new (reinterpret_cast<char*>(w.o) + 0x3458) OBJ_GEO3D();
    std::int32_t iiobj[20] = {};
    iiobj[15] = 3;
    iiobj[16] = 7;
    iiobj[17] = 2;
    iiobj[18] = 3;
    std::int32_t mar_tab[140] = {};
    std::int32_t a_tab[280] = {};
    a_tab[14] = 11;
    a_tab[0x9a] = 3;
    mar_tab[23] = 1;
    mar_tab[0x5d] = 4;
    std::vector<double> a(100, 0.0);
    for (int k = 0; k < 30; ++k) a[10 + k] = static_cast<double>(k);
    std::vector<std::int32_t> mar(20, 0);
    const std::int32_t rows[8] = {3, 4, 5, 5, 6, 7, 6, -7};
    for (int k = 0; k < 8; ++k) mar[4 + k] = rows[k];
    read_fold_info(w.o, iiobj, mar.data(), a.data(), mar_tab, a_tab);
    OBJ_GEO3D* g = geo_at(w.o, 0x3328);
    OBJ_GEO3D* n = geo_at(w.o, 0x3458);
    CHECK_EQ(g->node_count(), 3);
    CHECK_EQ(g->elem_count(), 1);
    CHECK_EQ(n->node_count(), 2);
    CHECK_EQ(n->elem_count(), 1);
    // node 3 is LLFLD row 3: a[10 + 6] = 6
    CHECK_EQ(g->coords[0], 6.0);
    CHECK_EQ(n->coords[0], 15.0);
    const std::int32_t wg[4] = {1, 2, 3, 3};
    const std::int32_t wn[4] = {1, 2, 1, 2};
    for (int k = 0; k < 4; ++k) {
        CHECK_EQ(g->elems[k], wg[k]);
        CHECK_EQ(n->elems[k], wn[k]);
    }
    g->~OBJ_GEO3D();
    n->~OBJ_GEO3D();
}

// 14016d300: only positive 4th nodes -> the whole mesh is FLDGEO; a zero IIFLD width reads nothing
TEST_CASE(read_fold_info_fldgeo_only_and_width_gate)
{
    World w(3);
    new (reinterpret_cast<char*>(w.o) + 0x3328) OBJ_GEO3D();
    new (reinterpret_cast<char*>(w.o) + 0x3458) OBJ_GEO3D();
    std::int32_t iiobj[20] = {};
    iiobj[15] = 1;
    iiobj[16] = 3;
    iiobj[17] = 1;
    iiobj[18] = 1;
    std::int32_t mar_tab[140] = {};
    std::int32_t a_tab[280] = {};
    a_tab[14] = 1;
    a_tab[0x9a] = 3;
    mar_tab[23] = 1;
    mar_tab[0x5d] = 0;
    std::vector<double> a(20, 1.0);
    std::vector<std::int32_t> mar = {1, 2, 3, 3};
    read_fold_info(w.o, iiobj, mar.data(), a.data(), mar_tab, a_tab);
    CHECK_EQ(geo_at(w.o, 0x3328)->node_count(), 0);
    mar_tab[0x5d] = 4;
    read_fold_info(w.o, iiobj, mar.data(), a.data(), mar_tab, a_tab);
    CHECK_EQ(geo_at(w.o, 0x3328)->node_count(), 3);
    CHECK_EQ(geo_at(w.o, 0x3328)->elem_count(), 1);
    CHECK_EQ(geo_at(w.o, 0x3458)->node_count(), 0);
    geo_at(w.o, 0x3328)->~OBJ_GEO3D();
    geo_at(w.o, 0x3458)->~OBJ_GEO3D();
}

// 14025bdc0 / 14025bd10: header (kind, subtype, n / ncomp rows, ncomp); values copied only for the
// matching storage type
TEST_CASE(dynary_list_add_copies_values_by_type)
{
    def::model::DynAryLinkedList list;
    def::model::CowVector<double> dv;
    dv.resize(6, 0.0);
    for (int k = 0; k < 6; ++k) dv.mutable_at(k) = 0.5 * k;
    dynary_list_add_real(&list, 1, 1, 6, 2, &dv);
    auto* d = def::model::dynary_list_nth(&list, 1);
    REQUIRE(d != nullptr);
    CHECK_EQ(def::model::dynary_data_count(d), 3);
    CHECK_EQ(def::model::dynary_data_ncomp(d), 2);
    CHECK_EQ(def::model::dynary_data_is_int(d), 0);
    CHECK_EQ(def::model::dynary_data_doubles(d)[5], 2.5);
    def::model::CowVector<std::int32_t> iv;
    iv.resize(4, 0);
    for (int k = 0; k < 4; ++k) iv.mutable_at(k) = 10 + k;
    dynary_list_add_int(&list, 1, 2, 4, 4, &iv);
    auto* d2 = def::model::dynary_list_nth(&list, 2);
    REQUIRE(d2 != nullptr);
    CHECK_EQ(def::model::dynary_data_count(d2), 1);
    CHECK_EQ(def::model::dynary_data_is_int(d2), 1);
    CHECK_EQ(def::model::dynary_data_ints(d2)[3], 13);
    // ncomp 0: a node is appended, nothing else
    dynary_list_add_int(&list, 1, 2, 4, 0, &iv);
    CHECK(def::model::dynary_list_nth(&list, 3) != nullptr);
}

// 140336740: constants; TTTD type 3 copies 11 coefficients from the function named in word 7
// (IIFNC row 2 starts at 5 -> LLFNC[4..14]), type 4 4 of them and a table; type -3 -> 0x10 / abs 3
TEST_CASE(phase_pair_record_constants)
{
    Buf mb(sizeof(DB_STEP_DATA));
    Buf pb(sizeof(def::model::PhasePair));
    auto* pp = pb.as<def::model::PhasePair>();
    fld<DB_STEP_DATA*>(pp, 0x778) = mb.as<DB_STEP_DATA>();
    fld<std::int32_t>(mb.as<DB_STEP_DATA>(), 0x748) = 2; // 2 * 100 + 2: no byte swap (140324020)
    std::vector<double> rec(0x40, 0.0);
    rec[0] = 2.0;
    rec[1] = 2.0;
    std::memcpy(&rec[0x12], "AUSTENITE->MARTENSITE   ", 24);
    rec[3] = 1.5;
    rec[5] = 2.5;
    rec[0x21] = 4.0;
    rec[0x22] = 7.25;
    rec[6] = 3.0;
    rec[7] = 2.0;
    rec[0x2f] = 9.5;
    rec[0x30] = 6.9;
    rec[8] = 8.0;
    std::int32_t iifnc[6] = {1, 0, 5, 77, 20, 0};
    std::vector<double> llfnc(20);
    for (int k = 0; k < 20; ++k) llfnc[k] = 100.0 + k;
    read_phase_pair_record(pp, rec.data(), iifnc, llfnc.data());
    CHECK_EQ(fld<std::int32_t>(pp, 0x8), 2);
    CHECK_EQ(fld<std::int32_t>(pp, 0xc), 1);
    CHECK_EQ(fld<std::int32_t>(pp, 0x10), 2);
    CHECK(std::strcmp(reinterpret_cast<char*>(pp) + 0x18, "AUSTENITE->MARTENSITE") == 0);
    CHECK_EQ(fld<double>(pp, 0x480), 1.5);
    CHECK_EQ(fld<double>(pp, 0x4f8), 2.5);
    CHECK_EQ(fld<std::int32_t>(pp, 0x5e0), 4);
    CHECK_EQ(fld<std::int32_t>(pp, 0x56c), 0);
    CHECK_EQ(fld<double>(pp, 0x570), 7.25);
    CHECK_EQ(fld<std::int32_t>(pp, 0x14), 5);
    CHECK_EQ(fld<double>(pp, 0x270), 104.0);
    CHECK_EQ(fld<double>(pp, 0x2c0), 114.0);
    CHECK_EQ(fld<double>(pp, 0x6f8), 9.5);
    CHECK_EQ(fld<std::int32_t>(pp, 0x768), 6);
    // TTTD type 4: 4 coefficients from LLFNC[4..7] to +0x340, then the f(T) table (DYNARY +0x2c8):
    // n[0] = (8 - 4) / 2 = 2 rows (x, y) from LLFNC[8..11]
    rec[6] = 4.0;
    fld<std::int32_t>(pp, 0x2c8 + 4) = 1; // ndim 1, as the PhasePair ctor's type 0x32 init leaves it
    read_phase_pair_record(pp, rec.data(), iifnc, llfnc.data());
    CHECK_EQ(fld<std::int32_t>(pp, 0x14), 6);
    CHECK_EQ(fld<double>(pp, 0x358), 107.0);
    auto* t4 = reinterpret_cast<def::model::DYNARY*>(reinterpret_cast<char*>(pp) + 0x2c8);
    REQUIRE(t4->n[0] == 2);
    CHECK_EQ(t4->axis[0][1], 110.0);
    CHECK_EQ(t4->values[1], 111.0);
    rec[6] = -3.0;
    read_phase_pair_record(pp, rec.data(), iifnc, llfnc.data());
    CHECK_EQ(fld<std::int32_t>(pp, 0x14), 0x10);
    CHECK_EQ(fld<std::int32_t>(pp, 0x470), 3);
    // PHASLH by function 2 of kind 2: +0x47c = 1, +0x488 = 0xc, then 1401309b0(pp + 0x478, .., 2):
    // start 5, length 20 - 5 = 15 -> 7 points (x, y) from LLFNC[4..], u54 = 77
    rec[2] = 2.0;
    rec[3] = 2.0;
    read_phase_pair_record(pp, rec.data(), iifnc, llfnc.data());
    auto* lh = reinterpret_cast<def::model::DYNARY*>(reinterpret_cast<char*>(pp) + 0x478);
    CHECK_EQ(lh->ndim, 1);
    CHECK_EQ(lh->var[0], 0xc);
    CHECK_EQ(lh->u54, 77);
    CHECK_EQ(lh->n[0], 7);
    CHECK_EQ(lh->axis[0][0], 104.0);
    CHECK_EQ(lh->values[6], 117.0);
    // word 0 = 0: +0x8 = 0, +0xc = 0, nothing else
    rec[0] = 0.0;
    fld<std::int32_t>(pp, 0x768) = 77;
    read_phase_pair_record(pp, rec.data(), iifnc, llfnc.data());
    CHECK_EQ(fld<std::int32_t>(pp, 0xc), 0);
    CHECK_EQ(fld<std::int32_t>(pp, 0x768), 77);
}

// 1401f58a0, 3D layout: word 1 = 0 -> +0x174 = 1; friction code 5 (word 5) with dims words
// 0xe..0x10 and function word 4; IHTCOF constant word 6; word 8 = -2 -> +0x230 = 2, +0x234 = 1;
// CNTMTH words 0x1f..0x21; WMODEL 2 from IIFNC row 1 (start 3 -> LLFNC[2], [3]); RGDCNT 0x26..0x29
TEST_CASE(inter_obj_record_3d)
{
    Buf mb(sizeof(DB_STEP_DATA));
    fld<std::int32_t>(mb.as<DB_STEP_DATA>(), 0x1c) = 3;
    Buf eb(sizeof(def::model::Cls_1401f46e0));
    auto* e = eb.as<def::model::Cls_1401f46e0>();
    e->owner = mb.as<DB_STEP_DATA>();
    std::vector<double> rec(0x40, 0.0);
    rec[0] = 1.0;
    rec[0xc] = 2.0;
    rec[0xd] = 0.125;
    rec[2] = 1.0;
    rec[5] = 5.0;
    rec[4] = 3.0;
    rec[0xe] = 7.0;
    rec[0xf] = 8.0;
    rec[0x10] = 9.0;
    rec[6] = 4.5;
    rec[8] = -2.0;
    rec[0x13] = 0.0;
    rec[0x14] = 0.75;
    rec[0x1f] = 1.0;
    rec[0x20] = 12.0;
    rec[0x21] = 0.5;
    rec[0x22] = 2.0;
    rec[0x23] = 1.0;
    rec[0x26] = 4.0;
    rec[0x27] = 5.0;
    rec[0x28] = 6.0;
    rec[0x29] = 1e-3;
    std::int32_t iifnc[8] = {3, 0, 5, 0, 7, 55, 9, 0};
    std::vector<double> llfnc(1000, 0.0);
    llfnc[2] = 11.0;
    llfnc[3] = 22.0;
    llfnc[6] = -1.5;
    read_inter_obj_record(e, rec.data(), iifnc, llfnc.data(), nullptr, 0, nullptr);
    CHECK_EQ(fld<std::int32_t>(e, 0x170), 1);
    CHECK_EQ(fld<std::int32_t>(e, 0x174), 1);
    CHECK_EQ(fld<std::int32_t>(e, 0x178), 2);
    CHECK_EQ(fld<double>(e, 0x180), 0.125);
    CHECK_EQ(fld<std::int32_t>(e, 0x0), 1);
    CHECK_EQ(fld<std::uint8_t>(e, 0x4), 0);
    CHECK_EQ(fld<std::int32_t>(e, 0x10), 5);
    CHECK_EQ(fld<std::int32_t>(e, 0x1c), 3);
    CHECK_EQ(fld<std::int32_t>(e, 0x28), 4);
    CHECK_EQ(fld<std::int32_t>(e, 0x30), 0x15);
    CHECK_EQ(fld<std::int32_t>(e, 0x34), 7);
    CHECK_EQ(fld<std::int32_t>(e, 0x3c), 9);
    // 1401309b0(e + 0x18, .., 3): row 3 starts at 7 (u54 = 55); a 7 x 8 x 9 table, axes from LLFNC[6..]
    auto* fr = reinterpret_cast<def::model::DYNARY*>(reinterpret_cast<char*>(e) + 0x18);
    CHECK_EQ(fr->u54, 55);
    CHECK_EQ(fr->n[2], 9);
    CHECK_EQ(fr->axis[0][0], -1.5);
    CHECK_EQ(fld<std::int32_t>(e, 0x1bc), 0);
    CHECK_EQ(fld<double>(e, 0x1c0), 4.5);
    CHECK_EQ(fld<std::int32_t>(e, 0x230), 2);
    CHECK_EQ(fld<std::int32_t>(e, 0x234), 1);
    CHECK_EQ(fld<double>(e, 0x2c0), 0.75);
    CHECK_EQ(fld<std::int32_t>(e, 0x330), 1);
    CHECK_EQ(fld<std::int32_t>(e, 0x334), 12);
    CHECK_EQ(fld<double>(e, 0x338), 0.5);
    CHECK_EQ(fld<std::int32_t>(e, 0x340), 2);
    CHECK_EQ(fld<double>(e, 0x368), 11.0);
    CHECK_EQ(fld<double>(e, 0x370), 22.0);
    CHECK_EQ(fld<std::int32_t>(e, 0x37c), 4);
    CHECK_EQ(fld<std::int32_t>(e, 0x384), 6);
    CHECK_EQ(fld<double>(e, 0x388), 1e-3);
}

// 1401f58a0, 2D layout: friction code -3 -> flag byte 1, reference 3, value word 3; FRCROT
// constant word 0xe; SEPRES words 0x10..0x12
TEST_CASE(inter_obj_record_2d)
{
    Buf mb(sizeof(DB_STEP_DATA));
    fld<std::int32_t>(mb.as<DB_STEP_DATA>(), 0x1c) = 2;
    Buf eb(sizeof(def::model::Cls_1401f46e0));
    auto* e = eb.as<def::model::Cls_1401f46e0>();
    e->owner = mb.as<DB_STEP_DATA>();
    std::vector<double> rec(0x40, 0.0);
    rec[0] = 2.0;
    rec[0x10] = 1.0;
    rec[0x11] = 3.5;
    rec[0x12] = 0.25;
    rec[2] = 2.0;
    rec[5] = -3.0;
    rec[3] = 0.3;
    rec[0xe] = 0.9;
    std::int32_t iifnc[2] = {1, 0};
    double llfnc[2] = {};
    read_inter_obj_record(e, rec.data(), iifnc, llfnc, nullptr, 0, nullptr);
    CHECK_EQ(fld<std::int32_t>(e, 0x170), 2);
    CHECK_EQ(fld<std::int32_t>(e, 0x178), 1);
    CHECK_EQ(fld<double>(e, 0x180), 3.5);
    CHECK_EQ(fld<double>(e, 0x238), 0.25);
    CHECK_EQ(fld<std::int32_t>(e, 0x0), 2);
    CHECK_EQ(fld<std::uint8_t>(e, 0x4), 1);
    CHECK_EQ(fld<std::int32_t>(e, 0x8), 3);
    CHECK_EQ(fld<double>(e, 0x20), 0.3);
    CHECK_EQ(fld<double>(e, 0x248), 0.9);
    CHECK_EQ(fld<std::int32_t>(e, 0x1c), 0); // no function table was loaded
}

// 140177ba0 (3D): function 1 used by a node with NBCD 6 -> SYMSUF point (id = IIFNC word 2 of row 1,
// point / direction LLFNC[0..5]); function 2 (node BC ref) and 3 (+0x9a8) -> BCCFNC table entries
// loaded by 1401309b0 with k = 2, 3; references become the rows' second IIFNC words
TEST_CASE(bc_functions_rebuild_tables)
{
    World w(3);
    std::int32_t nbcd[2] = {6, 0};
    std::int32_t ref1[2] = {1, 2};
    std::int32_t ref3[2] = {3, 0};
    fld<std::int32_t>(w.o, 0x880) = 2;
    fld<std::int32_t*>(w.o, 0x990) = nbcd;
    fld<std::int32_t*>(w.o, 0x998) = ref1;
    fld<std::int32_t*>(w.o, 0x9a8) = ref3;
    std::int32_t wtab[0x4f] = {};
    wtab[0x13] = 1;
    wtab[0x15] = 1;
    const std::int32_t iifnc[8] = {1, 101, 7, 102, 9, 103, 11, 0};
    std::vector<double> llfnc = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 0};
    read_bc_functions(w.o, iifnc, llfnc.data(), 3, wtab);
    CHECK_EQ(ref1[0], 101);
    CHECK_EQ(ref1[1], 102);
    CHECK_EQ(ref3[0], 103);
    auto* pts = reinterpret_cast<def::model::linked::Cls_140257340List*>(reinterpret_cast<char*>(w.o) + 0x2e80);
    REQUIRE(pts->size() == 1);
    CHECK_EQ(pts->nth(1)->id, 101);
    CHECK_EQ(pts->nth(1)->point[2], 3.0);
    CHECK_EQ(pts->nth(1)->direction[0], 4.0);
    auto* table = reinterpret_cast<def::model::Cls_1401364a0*>(reinterpret_cast<char*>(w.o) + 0x2e68);
    CHECK_EQ(table->size(), 2);
    // functions 2 and 3: one point each (length 2), u54 = the rows' second words
    CHECK_EQ(table->at(1)->ndim, 1);
    CHECK_EQ(table->at(1)->var[0], 1);
    CHECK_EQ(table->at(1)->u54, 102);
    CHECK_EQ(table->at(1)->axis[0][0], 7.0);
    CHECK_EQ(table->at(2)->u54, 103);
    CHECK_EQ(table->at(2)->values[0], 10.0);
    table->clear();
    pts->clear();
}

// 140170410 (3D slots): NBCD rows of width W[0x12] = 3 from the node offset; the BC-ref array is
// skipped when its stored width (7) differs from W[0x13]; with stored NBCD width 3 the +0x9b8 /
// +0x9b0 parts follow the 0x9a0 part at + NUMNP and + 2 NUMNP
TEST_CASE(node_bc_info_width_gates)
{
    World w(3);
    std::int32_t dim = 3;
    def::model::select_index_tables(&dim);
    auto mi = [](std::uint64_t dat) { return def::model::g_mar_index[(dat - 0x1406e81fcull) / 4]; };
    std::int32_t mar_tab[140] = {};
    mar_tab[mi(0x1406e8218)] = 11;
    mar_tab[mi(0x1406e8278)] = 0;
    mar_tab[mi(0x1406e827c)] = 7;
    mar_tab[mi(0x1406e8220)] = 41;
    mar_tab[mi(0x1406e8280)] = 3;
    mar_tab[mi(0x1406e8224)] = 61;
    mar_tab[mi(0x1406e8284)] = 0;
    std::int32_t wtab[0x4f] = {};
    wtab[0x12] = 3;
    wtab[0x13] = 1;
    wtab[0x14] = 1;
    wtab[0x15] = 1;
    std::vector<std::int32_t> mar(100);
    for (int k = 0; k < 100; ++k) mar[k] = k;
    std::int32_t nb[6] = {}, r1[2] = {-1, -1}, r2[2] = {}, r3[2] = {}, r4[2] = {}, r5[2] = {};
    fld<std::int32_t>(w.o, 0x87c) = 1;
    fld<std::int32_t>(w.o, 0x880) = 2;
    fld<std::int32_t>(w.o, 0x3984) = 2;
    fld<void*>(w.o, 0x8a8) = nb; // allocated already
    fld<std::int32_t*>(w.o, 0x990) = nb;
    fld<std::int32_t*>(w.o, 0x998) = r1;
    fld<std::int32_t*>(w.o, 0x9a0) = r2;
    fld<std::int32_t*>(w.o, 0x9a8) = r3;
    fld<std::int32_t*>(w.o, 0x9b0) = r4;
    fld<std::int32_t*>(w.o, 0x9b8) = r5;
    read_node_bc_info(w.o, mar.data(), mar_tab, wtab, 5);
    // NBCD: MAR + (11 - 1 + (2 - 1) * 3) = MAR[13..18]
    CHECK_EQ(nb[0], 13);
    CHECK_EQ(nb[5], 18);
    CHECK_EQ(r1[0], -1);
    // 0x9a0: MAR + (41 - 2 + 2) = 41; 0x9b8: + 5; 0x9b0: + 10; 0x9a8: MAR + (61 - 2 + 2)
    CHECK_EQ(r2[0], 41);
    CHECK_EQ(r5[1], 47);
    CHECK_EQ(r4[0], 51);
    CHECK_EQ(r3[1], 62);
}

int main()
{
    std::setvbuf(stdout, nullptr, _IONBF, 0);
    return testkit::run_all();
}
