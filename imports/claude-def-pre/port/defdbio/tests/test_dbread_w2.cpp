// ctest-labels: p-dbread dbread
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
// Tier 1 for helper W2 of packet p-dbread: ReadSimCtrl 1401cf4a0 (with 1401eb870, the name blocks
// 1401cf930 / 1401cfc30, the self-describing reader 140130dc0) and the material record reader
// 14028f300 / ReadMatlInfo 1401cff20. The DB arrays are built by hand; every expected value is the
// word the decompile copies (docs/re/dbmap/simctrl.md, material.md give the same word indices).
#include <defdbio/dbread/w2.hpp>
#include <testkit/check.hpp>

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <memory>
#include <vector>

#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/material/material_ctor.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defmodel/step/db_step.hpp>

using def::model::DB_STEP_DATA;
using def::model::DYNARY;
using def::model::Material;
namespace dr = def::dbio::dbread;
namespace dm = def::model;

namespace {

// writes `text` blank padded to n bytes at p
void put_text(void* p, const char* text, std::size_t n)
{
    std::memset(p, ' ', n);
    std::memcpy(p, text, std::strlen(text));
}

// A 3D model whose MAR / A arrays are vectors owned by the test (the pointers are cleared before
// the model goes away, so its dtor does not free them).
struct Model3D {
    std::unique_ptr<DB_STEP_DATA> m = std::make_unique<DB_STEP_DATA>(3);
    std::vector<std::int32_t> mar = std::vector<std::int32_t>(2000, 0);
    std::vector<double> a = std::vector<double>(3000, 0.0);
    Model3D()
    {
        std::int32_t dim = 3;
        dm::select_index_tables(&dim);
        m->u1b88 = mar.data();
        m->u1b78 = a.data();
        m->u748 = 1; // platform code 201: text is not byte swapped (140324020)
    }
    ~Model3D()
    {
        m->u1b88 = nullptr;
        m->u1b78 = nullptr;
    }
    void mar_start(int g, std::int32_t s) { m->u1b90[dm::g_mar_index[g]] = s; }
    void a_start(int g, std::int32_t s) { m->u1dc0[dm::g_a_index[g]] = s; }
    void a_width(int g, std::int32_t w) { m->u1dc0[dm::g_a_index[g]] = w; }
};

} // namespace

TEST_CASE(selfdesc_constant_and_table)
{
    // function 1: constant, ncomp 3, value 7.5; function 2: 2-D table 2 x 1 points, ncomp 1
    std::int32_t iifnc[6] = {1, 0, 15, 0, 15 + 10 + 2 + 1 + 2, 0};
    std::vector<double> llfnc(40, 0.0);
    llfnc[0] = 10.0;
    llfnc[1] = 0.0;
    llfnc[2] = 3.0;
    llfnc[3] = 7.5;
    double* t = llfnc.data() + 14; // function 2 starts at word 15
    t[0] = 10.0;
    t[1] = 2.0;
    t[2] = 3.0;  // var 1
    t[3] = 12.0; // var 2
    t[4] = 2.0;  // n1
    t[5] = 1.0;  // n2
    t[6] = 1.0;  // ncomp
    t[10] = 100.0; // axis 1
    t[11] = 200.0;
    t[12] = 0.25;  // axis 2
    t[13] = 5.0;   // values
    t[14] = 6.0;
    DYNARY c;
    dr::dynary_read_selfdesc(&c, iifnc, llfnc.data(), 1);
    CHECK_EQ(c.ndim, 0);
    CHECK_EQ(c.ncomp, 3);
    CHECK_BITS(c.value, 7.5);
    DYNARY d;
    dr::dynary_read_selfdesc(&d, iifnc, llfnc.data(), 2);
    CHECK_EQ(d.ndim, 2);
    CHECK_EQ(d.var[0], 3);
    CHECK_EQ(d.var[1], 12);
    CHECK_EQ(d.n[0], 2);
    CHECK_EQ(d.n[1], 1);
    CHECK_BITS(d.axis[0][0], 100.0);
    CHECK_BITS(d.axis[0][1], 200.0);
    CHECK_BITS(d.axis[1][0], 0.25);
    CHECK_BITS(d.values[0], 5.0);
    CHECK_BITS(d.values[1], 6.0);
    // a record without the 10.0 tag leaves the table alone
    llfnc[0] = 9.0;
    DYNARY e;
    e.ndim = 0;
    dr::dynary_read_selfdesc(&e, iifnc, llfnc.data(), 1);
    CHECK_EQ(e.ncomp, 1);
}

TEST_CASE(read_sim_ctrl_3d)
{
    Model3D w;
    w.mar_start(2, 1);    // IIFNC
    w.mar_start(6, 101);  // IISIM
    w.mar_start(11, 301); // IIUVL
    w.a_start(0, 1);      // LLFNC
    w.a_start(9, 201);    // LLWIN
    w.a_start(10, 301);   // LLWRZ
    w.a_start(3, 401);    // LLSIM
    w.a_start(22, 601);   // LLND1 start
    w.a_width(81, 2);     // LLND1 width: 2 node names
    w.a_start(36, 801);   // LLET1 start
    w.a_width(95, 1);     // LLET1 width: 1 element name
    std::int32_t* I = w.mar.data() + 100;
    double* L = w.a.data() + 400;
    I[0] = 3;     // SMODE
    I[1] = 5;     // MESHNO
    I[0x31] = 0;  // STYPE < 1 -> 1
    I[0x13] = 3;  // NUMNP
    I[0x14] = 2;  // NUMEL
    I[0x16] = 0;  // materials
    I[0x17] = 1;  // objects
    I[0x18] = 6;  // boundaries
    I[0x1a] = 1;  // NFN
    I[0x21] = 8;  // relations
    I[0x22] = 77; // 3D writer's windows word: not read
    I[0x27] = 4;  // windows (read from I[39])
    I[0x47] = 1;  // TIMEID = function 1
    I[0x50] = 1;  // VFACTR mode 1, count 0: not read
    L[0] = 12.5;
    L[0x33] = 42.9; // TNOW 3 -> 42
    L[0x1d] = 20.0; // ENVTMP constant (L[57] = 0, L[28] = 0)
    L[0x2b] = 0.0;
    L[0x2c] = 3.5;  // ACVCOF constant
    L[0x21] = 0.75; // DTSUB copied as is
    L[0x22] = -2.7; // MDSOBJ 1 -> -2
    // IIFNC / LLFNC: function 1 is a self-describing constant (TIMEID)
    w.mar[0] = 1;
    w.mar[1] = 0;
    w.mar[2] = 5;
    w.a[0] = 10.0;
    w.a[1] = 0.0;
    w.a[2] = 2.0;
    w.a[3] = 0.125;
    // IIUVL: USRDEF 1 = "HELLO", USRDEF 2 = blanks -> ""
    char* uvl = reinterpret_cast<char*>(w.mar.data() + 300);
    std::memset(uvl, ' ', 800);
    std::memcpy(uvl, "HELLO", 5);
    // UNNAME block: A[ASTART(LLND1)-1 + NUMNP*W] = A[600 + 6], two 64-character names
    put_text(w.a.data() + 606, "N1", 64);
    put_text(w.a.data() + 614, "N2", 64);
    // UENAME block: A[800 + NUMEL*1]
    put_text(w.a.data() + 802, "E1", 64);

    dr::read_sim_ctrl(w.m.get());
    DB_STEP_DATA* m = w.m.get();
    CHECK(m->u2220 == w.mar.data());
    CHECK(m->u2228 == w.a.data());
    CHECK(m->u2230 == w.a.data() + 200);
    CHECK(m->u2238 == w.a.data() + 300);
    CHECK_EQ(m->u1378, 1);
    CHECK_EQ(m->n_materials, 0);
    CHECK_EQ(m->u13f0, 6);
    CHECK_EQ(m->n_phase_on, 8);
    CHECK_EQ(m->u13e8, 3);
    CHECK_EQ(m->u13ec, 2);
    CHECK_EQ(m->u13fc, 1);
    CHECK_EQ(m->u13f8, 4);
    const auto& s = m->sim_ctrl;
    CHECK_EQ(s.smode, 3);
    CHECK_EQ(s.meshno, 5);
    CHECK_EQ(s.stype, 1);
    CHECK_BITS(s.tnow1, 12.5);
    CHECK_EQ(s.tnow3, 42);
    CHECK_EQ(s.mdsobj[0], -2);
    CHECK_BITS(s.dtsub, 0.75);
    CHECK_EQ(s.envtmp.ndim, 0);
    CHECK_BITS(s.envtmp.value, 20.0);
    CHECK_EQ(s.furnace_on, 0);
    CHECK_EQ(s.acvcof.ndim, 0);
    CHECK_BITS(s.acvcof.value, 3.5);
    CHECK_EQ(s.dttype.ndim, 0);
    CHECK_EQ(s.dttype.u58, 1);    // 1401ee510: no DTTYPE curve and DSMAX <= 0
    CHECK_EQ(s.frqncy_rows, 6);   // FRQNCY default 6 x 1 (1401e9850)
    CHECK_EQ(s.frqncy_cols, 1);
    CHECK_EQ(s.stnout[4], 1);     // all STNOUT 0 -> STNOUT 5 = 1
    CHECK_EQ(s.timeid.ndim, 0);
    CHECK_EQ(s.timeid.ncomp, 2);
    CHECK_BITS(s.timeid.value, 0.125);
    CHECK(std::strcmp(s.usrdef[0], "HELLO") == 0);
    CHECK(std::strcmp(s.usrdef[1], "") == 0);
    CHECK_EQ(m->u3398, 2);
    CHECK(std::strcmp(m->u33a0[0], "N1") == 0);
    CHECK(std::strcmp(m->u33a0[1], "N2") == 0);
    CHECK_EQ(m->u33a8, 1);
    CHECK(std::strcmp(m->u33b0[0], "E1") == 0);
}

TEST_CASE(usrdef_int_word_swap_platform_0)
{
    // u748 = 0: 140324020 sees code 200 and reverses the bytes of each 4-byte word (ints only)
    Model3D w;
    w.m->u748 = 0;
    std::vector<std::int32_t> iisim(100, 0);
    std::vector<double> llsim(100, 0.0);
    std::vector<char> uvl(800, ' ');
    std::memcpy(uvl.data(), "DCBAHGFE", 8);
    std::int32_t iifnc[2] = {1, 0};
    double llfnc[2] = {0.0, 0.0};
    dr::sim_ctrl_read_from_db(&w.m->sim_ctrl, iisim.data(), llsim.data(), uvl.data(), iifnc, llfnc);
    CHECK(std::strcmp(w.m->sim_ctrl.usrdef[0], "ABCDEFGH") == 0);
}

TEST_CASE(material_record)
{
    Model3D w;
    Material* mat = dm::material_list_add(&w.m->materials);
    std::vector<double> r(500, 0.0);
    r[0] = 7.0;
    put_text(r.data() + 2, "STEEL", 64);
    r[1] = 7.85;      // MATDEN
    r[10] = 0.85;     // FRAE2H
    r[0xb] = -5.0;    // FSTRES user routine 5
    r[0x20] = 2.1e5;  // YOUNG constant (w33 = 0)
    r[0x35] = -3.0;   // DIFCOE type 4, routine 3
    r[0x46] = 7.0;    // FRCMOD 7
    r[0x47] = 1.5;
    r[0x48] = 2.5;
    r[0x6d] = 5.0;    // GRNDAT 5
    for (int k = 0; k < 6; ++k) r[0x6e + k] = 1.0 + k;
    r[0x41] = 1.0;    // COARSE: header function 1
    r[0x42] = 3.0;    // SIZEMD (v >= 11000 only)
    std::int32_t iifnc[4] = {1, 0, 21, 0};
    std::vector<double> llfnc(20, 0.0);
    llfnc[0] = 4.0; // count
    llfnc[2] = 0.0; // constant form
    llfnc[3] = 9.5;
    dr::material_read_from_db(mat, r.data(), iifnc, llfnc.data(), 10000);
    CHECK_EQ(mat->number, 7);
    CHECK(std::strcmp(mat->name, "STEEL") == 0);
    CHECK_EQ(dm::field<std::int32_t>(mat, 0x50), 0xe);
    CHECK_EQ(dm::field<std::int32_t>(mat, 0x660), 5);
    CHECK_EQ(mat->young.ndim, 0);
    CHECK_BITS(mat->young.value, 2.1e5);
    CHECK_EQ(dm::field<std::int32_t>(mat, 0x950), 4);
    CHECK_EQ(dm::field<std::int32_t>(mat, 0xac0), 3);
    CHECK_EQ(dm::field<std::int32_t>(mat, 0x1ad0), 7);
    CHECK_BITS(dm::field<double>(mat, 0x1af0), 1.5);
    CHECK_BITS(dm::field<double>(mat, 0x1af8), 2.5);
    CHECK_EQ(dm::field<std::int32_t>(mat, 0x17e8), 5);
    CHECK_BITS(dm::field<double>(mat, 0x17b8), 1.0);
    CHECK_BITS(dm::field<double>(mat, 0x17e0), 6.0);
    CHECK_EQ(mat->coarse_flag, 4);
    CHECK_EQ(mat->coarse.ndim, 0);
    CHECK_BITS(mat->coarse.value, 9.5);
    CHECK_BITS(mat->frae2h, 0.85);
    CHECK_BITS(mat->matden, 7.85);
    CHECK_EQ(mat->sizemd, 0); // version 10000: not read
}

TEST_CASE(read_materials_maps_mixture_slots)
{
    Model3D w;
    w.a_start(1, 101);  // LLMPR start
    w.a_width(60, 200); // LLMPR width (stored)
    w.m->n_materials = 2;
    std::int32_t iifnc[4] = {1, 0, 2, 0};
    double llfnc[2] = {2.0, 0.0}; // MSTMTR list of function 1: DB slot 2
    w.m->u2220 = iifnc;
    w.m->u2228 = llfnc;
    w.m->step_version = 10000;
    double* r1 = w.a.data() + 100;
    double* r2 = r1 + 200;
    r1[0] = 11.0;
    r1[0x5d] = 1.0; // MSTMTR count
    r1[0x5e] = 1.0; // MSTMTR function
    r2[0] = 12.0;
    dr::read_materials(w.m.get());
    Material* m1 = dm::material_list_nth(&w.m->materials, 1);
    Material* m2 = dm::material_list_nth(&w.m->materials, 2);
    CHECK(m1 != nullptr && m2 != nullptr);
    CHECK_EQ(m1->number, 11);
    CHECK_EQ(m2->number, 12);
    CHECK_EQ(m1->mstmtr_flag, 1);
    CHECK_EQ(m1->mstmtr_count, 1);
    CHECK_EQ(m1->mstmtr_list[0], 12); // slot 2 -> material 12
    CHECK_EQ(m2->mstmtr_flag, 2);     // 1401d6250 marks the components
    w.m->u2220 = nullptr;
    w.m->u2228 = nullptr;
}

int main()
{
    std::setvbuf(stdout, nullptr, _IONBF, 0);
    return testkit::run_all();
}
