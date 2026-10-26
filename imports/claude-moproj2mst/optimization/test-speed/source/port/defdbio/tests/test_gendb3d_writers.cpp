// ctest-labels: p4-05 gendb3d
// Tier 1 for packet p4-05: the per-object writers of the 3D GENDB object pass (node / element /
// face / BC-function arrays, general words, LLBND record, fold mesh, contact codes, function
// table helpers). Objects, DB_STEP_DATA and the address tables are zeroed byte buffers with the
// fields set by offset; region slots are the 3D ones of docs/re/dbmap/region_globals.tsv
// (A: LLCRZ 11, LLDRZ 12, LLURZ 13, LLPFN 15, LLPPR 17, LLTMP 20, LLPHT 21, LLFLX 23, LLND1 24,
// LLVLT 25, LLATM 26, LLSTS 29, LLTSR 31, LLTEP 34, LLRDT 35, LLDMG 38, LLPRE 51, LLHFL 52,
// LLAFL 53; widths LLSTS 169; MAR: IIMTR 5, IIBCD 7, IIBFD 8, IIBCT 9, IIBFT 10, IIEDE 17,
// IIETM 18, IIEAT 19, IIEDF 20, IIETF 21, IIEAF 22). Calls into other packets are answered by the
// ext hook. Expected values are worked out by hand from the decompile.
#include <defdbio/gendb3d/obj3d.hpp>
#include <testkit/check.hpp>

#include <cmath>
#include <cstdint>
#include <cstring>
#include <limits>
#include <map>
#include <vector>

#include <defmodel/layout/cls_14017a6c0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/step/db_step.hpp>

#include "../src/gendb3d/access.hpp"
#include "../src/gendb3d/ext.hpp"

using namespace def::dbio::g3d;
using def::model::DB_STEP_DATA;
using def::model::DYNARY;
using def::model::LLOBJ_DATA;

namespace {

struct Buf {
    std::vector<std::uint64_t> w;
    explicit Buf(std::size_t bytes) : w((bytes + 7) / 8, 0) {}
    template <class T> T* as() { return reinterpret_cast<T*>(w.data()); }
};

// zeroed model (dimension 3) and object owned by it
struct World {
    Buf mb{sizeof(DB_STEP_DATA)};
    Buf ob{sizeof(LLOBJ_DATA)};
    DB_STEP_DATA* m;
    LLOBJ_DATA* o;
    World()
    {
        m = mb.as<DB_STEP_DATA>();
        o = ob.as<LLOBJ_DATA>();
        fld<std::int32_t>(m, 0x1c) = 3;
        o->owner = m;
    }
    template <class T> T& of(std::ptrdiff_t off) { return fld<T>(o, off); }
};

// tables: start(slot) = slot * 1000 + 1 (1-based), widths given per slot
struct Tabs {
    std::int32_t mar_tab[140] = {};
    std::int32_t a_tab[280] = {};
    std::vector<std::int32_t> mar = std::vector<std::int32_t>(80 * 1000, 0);
    std::vector<double> a = std::vector<double>(80 * 1000, 0.0);
    Tabs()
    {
        for (int i = 0; i < 70; ++i) mar_tab[i] = i * 1000 + 1;
        for (int i = 0; i < 140; ++i) a_tab[i] = i * 1000 + 1;
    }
    double* A(int slot) { return a.data() + slot * 1000; }
    std::int32_t* M(int slot) { return mar.data() + slot * 1000; }
};

// hook: fixed answers per address; addresses not listed fall through to the default bodies
std::map<std::uint64_t, std::uint64_t> g_ans;
std::vector<std::uint64_t> g_calls;
bool hook(std::uint64_t addr, const std::uint64_t*, int, std::uint64_t* r)
{
    g_calls.push_back(addr);
    auto it = g_ans.find(addr);
    if (it == g_ans.end()) return false;
    *r = it->second;
    return true;
}
void reset_hook()
{
    g_ans.clear();
    g_calls.clear();
    ext::g_hook = hook;
}

} // namespace

// ---- function table -------------------------------------------------------------------------

TEST_CASE(alloc_function_pairs_and_positions)
{
    World w;
    w.m->u074[(0x88 - 0x74) / 4] = 10;   // functions allocated
    w.m->u074[(0x8c - 0x74) / 4] = 100;  // LLFNC length
    std::int32_t iifnc[8] = {};
    double num = 0;
    std::int32_t pos = 0;
    std::int32_t len = 5, id = 7;
    alloc_function(w.m, &len, iifnc, &id, &num, &pos);
    // n = 1: iifnc[1] = id, iifnc[0] = 1, iifnc[2] = 1 + 5, pos = iifnc[0]
    CHECK_EQ(w.m->u10c, 1);
    CHECK_BITS(num, 1.0);
    CHECK_EQ(iifnc[0], 1);
    CHECK_EQ(iifnc[1], 7);
    CHECK_EQ(iifnc[2], 6);
    CHECK_EQ(pos, 1);
    len = 3;
    id = 9;
    alloc_function(w.m, &len, iifnc, &id, &num, &pos);
    // n = 2: iifnc[3] = 9, iifnc[4] = iifnc[2] + 3 = 9, pos = iifnc[2] = 6
    CHECK_BITS(num, 2.0);
    CHECK_EQ(iifnc[3], 9);
    CHECK_EQ(iifnc[4], 9);
    CHECK_EQ(pos, 6);
}

TEST_CASE(dynary_to_db_array_shapes)
{
    DYNARY f;
    double vals[6] = {10, 11, 12, 13, 14, 15};
    double ax0[3] = {1, 2, 3};
    double ax1[2] = {7, 8};
    f.values = vals;
    f.axis[0] = ax0;
    f.axis[1] = ax1;
    // ndim 0, listed type 0xa7: the ncomp values, then again via the memcpy (count 14012ff20)
    f.type = 0xa7;
    f.ndim = 0;
    f.ncomp = 2;
    double out[16] = {};
    write_dynary_to_db_array(&f, out);
    CHECK_BITS(out[0], 10.0);
    CHECK_BITS(out[1], 11.0);
    CHECK_BITS(out[2], 0.0);
    // ndim 0, unlisted type: nothing
    double out2[4] = {-1, -1, -1, -1};
    f.type = 0x50;
    write_dynary_to_db_array(&f, out2);
    CHECK_BITS(out2[0], -1.0);
    // ndim 1: rows (x, v0, v1)
    f.type = 0x50;
    f.ndim = 1;
    f.n[0] = 3;
    f.ncomp = 2;
    double out3[16] = {};
    write_dynary_to_db_array(&f, out3);
    const double e3[9] = {1, 10, 11, 2, 12, 13, 3, 14, 15};
    for (int i = 0; i < 9; ++i) CHECK_BITS(out3[i], e3[i]);
    double out4[16] = {};
    write_table_1d_to_db_array(&f, out4);
    for (int i = 0; i < 9; ++i) CHECK_BITS(out4[i], e3[i]);
    // ndim 2: axes then values (count = ncomp * n0 * n1 = 1 * 3 * 2)
    f.ndim = 2;
    f.n[0] = 3;
    f.n[1] = 2;
    f.ncomp = 1;
    double out5[16] = {};
    write_dynary_to_db_array(&f, out5);
    const double e5[11] = {1, 2, 3, 7, 8, 10, 11, 12, 13, 14, 15};
    for (int i = 0; i < 11; ++i) CHECK_BITS(out5[i], e5[i]);
    f.values = nullptr;
    f.axis[0] = f.axis[1] = nullptr;
}

TEST_CASE(func_to_db_array_mapped)
{
    DYNARY f;
    double vals[3] = {2.9, 1.0, 3.0};
    f.values = vals;
    f.type = 0x7c;
    f.ncomp = 3;
    std::int32_t map[3] = {40, 50, 60};
    double out[3] = {};
    write_func_to_db_array(&f, out, map);
    // (int)2.9 = 2 -> map[1]
    CHECK_BITS(out[0], 50.0);
    CHECK_BITS(out[1], 40.0);
    CHECK_BITS(out[2], 60.0);
    write_func_to_db_array(&f, out, nullptr);
    CHECK_BITS(out[0], 2.9);
    f.values = nullptr;
}

// ---- node / element writers -------------------------------------------------------------------

TEST_CASE(node_info_places_fields)
{
    reset_hook();
    World w;
    Tabs t;
    w.m->u06c = 50;  // NUMNP
    w.m->u070 = 40;  // NUMEL
    std::int32_t* W = w.m->u15f8;
    W[0x00] = 3; W[0x01] = 3; W[0x02] = 3; W[0x03] = 3; W[0x05] = 3; W[0x08] = 1; W[0x09] = 1;
    W[0x0b] = 1; W[0x0d] = 1; W[0x0f] = 1; W[0x19] = 3; W[0x26] = 6; W[0x1a] = 6;
    const int nn = 2;
    w.of<std::int32_t>(0x880) = nn;
    std::vector<double> f[40];
    for (int k = 0; k < 40; ++k) f[k].assign(16, 100.0 * k);
    const std::ptrdiff_t offs[] = {0x8a8, 0x8b0, 0x8b8, 0x8c0, 0x8c8, 0x8d0, 0x8d8, 0x8e0, 0x8e8, 0x8f0,
                                   0x8f8, 0x900, 0x908, 0x910, 0x918, 0x920, 0x928, 0x930, 0x938, 0x940,
                                   0x948, 0x950, 0x958, 0x960};
    for (int k = 0; k < 24; ++k) {
        for (int i = 0; i < 16; ++i) f[k][i] = 100.0 * (k + 1) + i;
        w.of<double*>(offs[k]) = f[k].data();
    }
    w.of<double*>(0x968) = nullptr;  // no USRNOD
    w.of<double*>(0x928) = nullptr;  // no NWEAR
    fld<std::int32_t>(w.m, 0x890) = 2;  // ELMNOD stress option 2 -> nodal block
    std::int32_t off = 5;
    write_node_info(w.o, t.mar.data(), t.a.data(), t.mar_tab, t.a_tab, &off);
    // RZ: A[start(11) - 1 + 5 * 3 + i] = rz[i], i < 6
    for (int i = 0; i < 6; ++i) CHECK_BITS(t.A(11)[15 + i], f[0][i]);
    CHECK_BITS(t.A(11)[21], 0.0);
    for (int i = 0; i < 6; ++i) CHECK_BITS(t.A(17)[15 + i], f[5][i]);  // PRZ (3D)
    CHECK_BITS(t.A(20)[5], f[7][0]);                                   // NDTMP width 1
    // NDFLUX comp 1 at +5, RHTFLX (0x920) at +NUMNP, CRBFLX (0x910) at +2 NUMNP
    CHECK_BITS(t.A(23)[5], f[10][0]);
    CHECK_BITS(t.A(23)[50 + 5], f[15][0]);
    CHECK_BITS(t.A(23)[100 + 6], f[13][1]);
    // DRMESH: literal slot 0x13
    CHECK_BITS(t.A(19)[15], f[17][0]);
    // STRESS nodal block: start - 1 + NUMEL * W[0x26] + off * W[0x1a]
    CHECK_BITS(t.A(29)[240 + 30], f[20][0]);
    CHECK_BITS(t.A(29)[240 + 30 + 11], f[20][11]);
    // *DTMP only in 2D: slot of 0x1406e839c untouched
    CHECK_BITS(t.A(55)[5], 0.0);
}

TEST_CASE(node_int_info_functions_and_negatives)
{
    reset_hook();
    World w;
    Tabs t;
    w.m->u06c = 10;
    std::int32_t* W = w.m->u15f8;
    W[0x12] = 3; W[0x13] = 3; W[0x14] = 1; W[0x15] = 1; W[0x16] = 1; W[0x17] = 1;
    w.of<std::int32_t>(0x880) = 2;
    std::int32_t def[6] = {6, 0, 0, 1, 1, 1};
    std::int32_t dfn[6] = {2, 0, 0, 3, 99, 0};  // 99 > cap -> 0
    std::int32_t tmp[2] = {-1, 4};
    std::int32_t tfn[2] = {1, 0};
    std::int32_t crb[2] = {-5, 0};
    std::int32_t rht[2] = {0, -2};
    w.of<std::int32_t*>(0x990) = def;
    w.of<std::int32_t*>(0x998) = dfn;
    w.of<std::int32_t*>(0x9a0) = tmp;
    w.of<std::int32_t*>(0x9a8) = tfn;
    w.of<std::int32_t*>(0x9b0) = crb;
    w.of<std::int32_t*>(0x9b8) = rht;
    std::int32_t used[10] = {}, sym[10] = {};
    std::int32_t cap = 10, scap = 10;
    std::uint32_t kinds = 0;
    std::int32_t any = 0;
    std::int32_t off = 1;
    write_node_int_info(w.o, t.mar.data(), t.mar_tab, &off, used, sym, &cap, &scap, &kinds, &any);
    // BCCDEF copied at IIBCD + 1 * 3
    CHECK_EQ(t.M(7)[3], 6);
    // node 0 comp 0: code 6, fn 2 -> symmetry plane 2
    CHECK_EQ(sym[1], 1);
    CHECK_EQ(any, 1);
    CHECK_EQ(used[2], 1);       // fn 3 from comp 3
    CHECK_EQ(dfn[4], 0);        // 99 > cap zeroed in the object
    CHECK_EQ(t.M(8)[3 + 4], 0);
    CHECK_EQ(used[0], 1);       // BCCTFN 1
    CHECK_EQ(kinds, 0x11u);
    // BCCTMP negative -> 2 at IIBCT + 1
    CHECK_EQ(t.M(9)[1], 2);
    CHECK_EQ(t.M(9)[2], 4);
    // BCCRHT at + NUMNP (10), BCCCRB at + 2 NUMNP
    CHECK_EQ(t.M(9)[10 + 1], 0);
    CHECK_EQ(t.M(9)[10 + 2], 2);
    CHECK_EQ(t.M(9)[20 + 1], 2);
}

TEST_CASE(elem_info_replicates_one_point_stress)
{
    reset_hook();
    World w;
    Tabs t;
    std::int32_t* W = w.m->u15f8;
    W[0x26] = 2;
    // element set at obj+0x9e0: owner u0f0 = obj (reads obj+0x888 options); option 0 -> p = 1
    fld<LLOBJ_DATA*>(w.o, 0x9e0 + 0xf0) = w.o;
    fld<std::int32_t>(w.o, 0x9e0 + 0xf8) = 3;
    w.of<std::int32_t>(0x884) = 2;
    t.a_tab[169] = 16;  // LLSTS table width 16 = 8 points * 2
    double sts[4] = {1, 2, 3, 4};
    w.of<double*>(0xa00) = sts;
    std::vector<double> z(64, 0.0);
    for (std::ptrdiff_t off = 0xa08; off <= 0xab8; off += 8) w.of<double*>(off) = z.data();
    g_ans[0x140184a30] = 0;  // no USRELM buffer
    fld<std::int32_t>(w.m, 0x898) = 1;  // ELMNOD grain nodal: no element GRAIN
    std::int32_t eoff = 1, noff = 0;
    write_elem_info(w.o, t.mar.data(), t.a.data(), t.mar_tab, t.a_tab, &eoff, &noff);
    // rows of 16 at start - 1 + 1 * 16: element e, point g, comp c = sts[e * 2 + c]
    double* r = t.A(29) + 16;
    for (int e = 0; e < 2; ++e)
        for (int g = 0; g < 8; ++g)
            for (int c = 0; c < 2; ++c) CHECK_BITS(r[e * 16 + g * 2 + c], sts[e * 2 + c]);
}

TEST_CASE(edge_face_info_flags)
{
    reset_hook();
    World w;
    Tabs t;
    std::int32_t def[3] = {9, 1, 0}, dfn[3] = {1, 2, 0};
    std::int32_t tmp[3] = {4, 0, 0}, tfn[3] = {3, 4, 0};
    std::int32_t z[3] = {};
    double p[3] = {1.5, 2.5, 3.5}, zd[3] = {};
    w.of<std::int32_t*>(0xae8) = def;
    w.of<std::int32_t*>(0xaf0) = dfn;
    w.of<double*>(0xaf8) = p;
    w.of<std::int32_t*>(0xb00) = tmp;
    w.of<std::int32_t*>(0xb08) = tfn;
    w.of<double*>(0xb10) = zd;
    w.of<std::int32_t*>(0xb18) = z;
    w.of<std::int32_t*>(0xb20) = z;
    w.of<double*>(0xb28) = zd;
    fld<std::int32_t>(w.m, 0x750 + 0x118) = 0;  // no ECCRHT
    std::int32_t used[10] = {};
    std::uint32_t kinds = 0;
    std::int32_t off = 4, n = 3;
    write_edge_face_info(w.o, t.mar.data(), t.a.data(), t.mar_tab, t.a_tab, &off, &n, used, &kinds);
    CHECK_EQ(t.M(17)[4], 9);
    CHECK_EQ(t.M(20)[5], 2);
    CHECK_BITS(t.A(51)[6], 3.5);
    // ECDEFN: face 0 skipped (code 9), face 1 fn 2 flagged; ECTMFN: face 0 skipped (4), face 1 fn 4
    CHECK_EQ(used[0], 0);
    CHECK_EQ(used[1], 1);
    CHECK_EQ(used[2], 0);
    CHECK_EQ(used[3], 1);
    CHECK_EQ(kinds, 0x11u);
}

TEST_CASE(bc_functions_get_global_numbers)
{
    reset_hook();
    World w;
    Tabs t;
    w.m->u074[(0x88 - 0x74) / 4] = 100;
    w.m->u074[(0x8c - 0x74) / 4] = 1000;
    w.m->u10c = 4;  // four functions already allocated
    std::int32_t* W = w.m->u15f8;
    W[0x13] = 3; W[0x15] = 1;
    w.of<std::int32_t>(0x880) = 1;
    std::int32_t def[3] = {1, 0, 0}, dfn[3] = {2, 0, 0}, tfn[1] = {0};
    w.of<std::int32_t*>(0x990) = def;
    w.of<std::int32_t*>(0x998) = dfn;
    w.of<std::int32_t*>(0x9a8) = tfn;
    // one local function, list id 1 at +0x54, flagged as used fn 2 (id at +0x54 = 2)
    DYNARY f;
    f.type = 0xa7;
    f.ndim = 0;
    f.ncomp = 1;
    double v = 7.25;
    f.values = &v;
    fld<std::int32_t>(&f, 0x54) = 2;
    g_ans[0x140136a10] = 1;
    g_ans[0x1401368c0] = ext::u(&f);
    g_ans[0x140130290] = 1;
    std::int32_t iifnc[20] = {};
    iifnc[8] = 21;  // LLFNC end of function 4 -> function 5 starts at 21
    std::vector<double> llfnc(64, 0.0);
    std::int32_t used[4] = {0, 1, 0, 0}, sym[4] = {};
    std::uint32_t kinds = 1, fkinds = 0;
    std::int32_t any = 0, noff = 2, foff = 0, nf = 0;
    write_bc_functions(w.o, t.mar.data(), t.mar_tab, &noff, &foff, &nf, iifnc, llfnc.data(), used, sym,
                       &kinds, &any, &fkinds);
    // global number 5, id = list index 1, LLFNC start 21: value at llfnc[20] (twice: ndim 0)
    CHECK_EQ(w.m->u10c, 5);
    CHECK_EQ(iifnc[9], 1);
    CHECK_EQ(iifnc[10], 22);
    CHECK_BITS(llfnc[20], 7.25);
    CHECK_EQ(used[1], 5);
    // IIBFD + noff * 3: dfn 2 -> 5
    CHECK_EQ(t.M(8)[6], 5);
    f.values = nullptr;
}

TEST_CASE(general_info_words)
{
    reset_hook();
    World w;
    w.m->u114 = 10;
    w.m->u118 = 20;
    w.of<std::int32_t>(0xc) = 3;
    w.of<std::int32_t>(0x87c) = 1;
    w.of<std::int32_t>(0x880) = 5;
    w.of<std::int32_t>(0x884) = 4;
    fld<std::int32_t>(w.o, 0x9e0 + 0xec) = 4;  // tetrahedra
    fld<std::int32_t>(w.m, 0x854) = 1;
    std::strcpy(w.o->name, "BILLET");
    std::int32_t iiobj[20] = {};
    double llobj[64] = {};
    const char meshed = 1;
    write_general_info(w.o, iiobj, llobj, 33, 7, 1, nullptr, nullptr, nullptr, &meshed);
    CHECK_EQ(iiobj[0], 1);
    CHECK_EQ(iiobj[1], 2);  // type 3
    CHECK_EQ(iiobj[2], 0);
    CHECK_EQ(iiobj[3], 1);
    CHECK_EQ(iiobj[4], 11);
    CHECK_EQ(iiobj[5], 15);
    CHECK_EQ(iiobj[6], 21);
    CHECK_EQ(iiobj[7], 24);
    CHECK_EQ(w.m->u114, 15);
    CHECK_EQ(w.m->u118, 24);
    CHECK_EQ(iiobj[12], 4);
    CHECK_EQ(iiobj[13], 33);
    CHECK_EQ(iiobj[8], 7);
    CHECK_EQ(iiobj[9], 7);
    CHECK_EQ(w.of<std::int32_t>(0x3588), 7);
    // name as a 64-character blank-padded field from LLOBJ byte 0x20
    const char* nm = reinterpret_cast<const char*>(llobj) + 0x20;
    CHECK(std::memcmp(nm, "BILLET  ", 8) == 0);
    CHECK_EQ(nm[63], ' ');
    // type 7: only word 0
    w.of<std::int32_t>(0xc) = 7;
    std::int32_t ii2[20];
    for (auto& x : ii2) x = -1;
    write_general_info(w.o, ii2, llobj, 1, 1, 1, nullptr, nullptr, nullptr, &meshed);
    CHECK_EQ(ii2[0], 0);
    CHECK_EQ(ii2[1], -1);
}

TEST_CASE(volume_fraction_normalisation)
{
    World w;
    fld<std::int32_t>(w.o, 0x9e0 + 0xe8) = 2;  // elements (1401854f0)
    double vf[4] = {-1.0, 3.0, 1e-9, 0.0};
    w.of<double*>(0xa58) = vf;
    std::int32_t wd = 2;
    normalize_volume_fractions(w.o, &wd);
    CHECK_BITS(vf[0], 0.0);
    CHECK_BITS(vf[1], 1.0);
    CHECK_BITS(vf[2], 0.0);  // sum 1e-9 <= 1e-8 -> zeroed
    CHECK_BITS(vf[3], 0.0);
    // NaN sum: not above 1e-8 -> zeroed
    double nanrow[4] = {std::numeric_limits<double>::quiet_NaN(), 1.0, 5.0, 5.0};
    normalize_volume_fraction_rows(w.o, nanrow, &wd);
    CHECK_BITS(nanrow[0], 0.0);
    CHECK_BITS(nanrow[1], 0.0);
    // only the first row is touched
    CHECK_BITS(nanrow[2], 5.0);
}

TEST_CASE(nearest_loop_node_2d)
{
    World w;
    fld<std::int32_t>(w.m, 0x1c) = 2;
    w.of<std::int32_t>(0xbd8) = 2;
    w.of<std::int32_t>(0x880) = 3;
    double rz[6] = {0, 0, 10, 0, 10, 10};
    w.of<double*>(0x8a8) = rz;
    std::int32_t l0[2] = {1, 2}, l1[1] = {3};
    w.of<std::int32_t>(0xbe0) = 2;
    w.of<std::int32_t*>(0xbe8) = l0;
    w.of<std::int32_t>(0xbe0 + 0x30) = 1;
    w.of<std::int32_t*>(0xbe8 + 0x30) = l1;
    std::int32_t node, loop;
    double px, py;
    CHECK(nearest_loop_node(w.o, 9.0, 9.0, &node, &loop, &px, &py));
    CHECK_EQ(node, 3);
    CHECK_EQ(loop, 1);
    CHECK_BITS(px, 9.0);
    w.of<std::int32_t>(0xbd8) = 0;
    CHECK(!nearest_loop_node(w.o, 9.0, 9.0, &node, &loop, &px, &py));
    CHECK_EQ(node, -1);
}

TEST_CASE(contact_codes_3d)
{
    reset_hook();
    World w;
    w.m->u06c = 2;                       // NUMNP
    w.m->u074[(0x80 - 0x74) / 4] = 3;    // LLBND records
    // two objects in the model's list: #2 single record 1, #3 records 2..3 (type 3, then 2)
    Buf b2(sizeof(LLOBJ_DATA)), b3(sizeof(LLOBJ_DATA));
    auto* o2 = b2.as<LLOBJ_DATA>();
    auto* o3 = b3.as<LLOBJ_DATA>();
    fld<std::int32_t>(o2, 8) = 2;
    fld<std::int32_t>(o2, 0x3588) = 1;
    fld<std::int32_t>(o2, 0x358c) = 1;
    fld<std::int32_t>(o3, 8) = 3;
    fld<std::int32_t>(o3, 0x3588) = 2;
    fld<std::int32_t>(o3, 0x358c) = 3;
    o2->next = o3;
    w.m->objects.head = o2;
    w.m->objects.owner = w.m;
    std::int32_t rw = 100;
    std::vector<double> llbnd(300, 0.0);
    llbnd[100] = 3.0;  // record 2 type 3
    llbnd[200] = 2.0;  // record 3 type 2
    llbnd[0xf] = 5.0;  // record 1 attach word: object 3 -> its first record
    llbnd[0x3e] = 3.0;
    std::int32_t iibcd[6] = {-202, -2, 0, -3, 0, 7};
    convert_contact_codes(&w.m->objects, nullptr, iibcd, llbnd.data(), &rw);
    // -(200+2): other components equal to -2 zeroed, then -(1) - 200
    CHECK_EQ(iibcd[0], -201);
    CHECK_EQ(iibcd[1], 0);
    // -3: first record with type < 3.0 is 3
    CHECK_EQ(iibcd[3], -3);
    CHECK_EQ(iibcd[5], 7);
    CHECK_BITS(llbnd[0x3e], 2.0);
}

TEST_CASE(fold_info_ranges_and_faces)
{
    reset_hook();
    World w;
    w.m->u0d0 = 4;  // fold node offset
    w.m->u0d4 = 1;  // fold face offset
    g_ans[0x140160e50] = 1;  // FLDGEO only
    g_ans[0x140160eb0] = 0;
    g_ans[0x140167d70] = 2;  // nodes
    g_ans[0x140167e00] = 1;  // faces
    double xyz[6] = {1, 2, 3, 4, 5, 6};
    std::int32_t fc[4] = {1, 2, 2, -1};
    g_ans[0x140006410] = ext::u(xyz);
    g_ans[0x140005510] = ext::u(fc);
    std::int32_t iiobj[20] = {};
    double llfld[6] = {};
    std::int32_t iifld[4] = {};
    std::int32_t nn = 0, nf = 0;
    write_fold_info(w.o, iiobj, llfld, iifld, &nn, &nf);
    CHECK_EQ(iiobj[15], 5);
    CHECK_EQ(iiobj[16], 6);
    CHECK_EQ(iiobj[17], 2);
    CHECK_EQ(iiobj[18], 2);
    CHECK_EQ(nn, 2);
    CHECK_EQ(nf, 1);
    CHECK_BITS(llfld[5], 6.0);
    // node numbers + (first - 1) = +4; 4th word -1 (< 1) -> -1 - 4
    CHECK_EQ(iifld[0], 5);
    CHECK_EQ(iifld[2], 6);
    CHECK_EQ(iifld[3], -5);
}

TEST_CASE(counters_round_trip)
{
    World w;
    std::int32_t v = 17;
    set_record_counter(w.m, &v);
    CHECK_EQ(record_counter(w.m), 17);
    CHECK_EQ(w.m->u110, 17);
    set_face_offset(w.m, &v);
    CHECK_EQ(face_offset(w.m), 17);
    set_geo_facet_id_offset(w.m, &v);
    CHECK_EQ(w.m->u12c, 17);
    w.m->u074[(0x98 - 0x74) / 4] = 9;
    CHECK_EQ(n_objects_meshed(w.m), 9);
}

int main()
{
    std::int32_t dim = 3;
    def::model::select_index_tables(&dim);
    return testkit::run_all();
}
