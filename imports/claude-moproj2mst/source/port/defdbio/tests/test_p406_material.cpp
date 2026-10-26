// Tier-1 tests of the p4-06 material and inter-material GENDB writers
// (14029bdf0, 140294ae0, 14029b0d0, 14028e030, 14028c340, 14028aa70, 14028acb0, 14028b4a0,
// 14033a5a0, 140337f50).
// ctest-labels: p4-06 gendb
//
// Models, materials and pairs are zeroed raw storage with the fields the writers read set by
// hand. Every expected value is derived by hand from the decompile (p406.c) in the comments:
// FNCLOC (1401bb3d0) gives function k the LLFNC start IIFNC[2k-2], owner IIFNC[2k-1] and next
// start IIFNC[2k] = start + length; IIFNC[0] = 1. "Fn k at s" below means start s (1-based), so
// its data is llfnc[s-1 ..].
#include <testkit/check.hpp>

#include <cstring>
#include <deque>
#include <string>
#include <vector>

#include <defdbio/gendb/p4_06_material.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/step/db_step.hpp>

using def::model::Cls_14028a4c0;
using def::model::Cls_14028b640;
using def::model::DB_STEP_DATA;
using def::model::DYNARY;
using def::model::field;
using def::model::Material;
using def::model::MaterialList;
using def::model::PhasePair;
using def::model::PhasePairList;
namespace g = def::dbio::gendb;

namespace {

constexpr double kU = -99.0; // sentinel: word not written

// zeroed, 8-byte aligned storage for a layout class (never destroyed: no DYNARY dtor runs)
template <class T>
struct Raw {
    std::vector<std::uint64_t> buf = std::vector<std::uint64_t>(sizeof(T) / 8 + 1, 0);
    T* p = reinterpret_cast<T*>(buf.data());
};

std::deque<std::vector<double>> g_keep;
double* keep(std::vector<double> v)
{
    g_keep.push_back(std::move(v));
    return g_keep.back().data();
}

void seti(void* o, std::size_t off, std::int32_t v) { field<std::int32_t>(o, off) = v; }
void setd(void* o, std::size_t off, double v) { field<double>(o, off) = v; }
DYNARY* dy(void* o, std::size_t off) { return &field<DYNARY>(o, off); }

void t0(DYNARY* f, double v, std::int32_t ncomp = 1)
{
    f->ndim = 0;
    f->value = v;
    f->ncomp = ncomp;
}
void t1(DYNARY* f, std::int32_t var, std::vector<double> x, std::vector<double> y, std::int32_t ncomp = 1)
{
    f->ndim = 1;
    f->var[0] = var;
    f->n[0] = static_cast<std::int32_t>(x.size());
    f->ncomp = ncomp;
    f->axis[0] = keep(std::move(x));
    f->values = keep(std::move(y));
}
void t2(DYNARY* f, std::int32_t v1, std::int32_t v2, std::vector<double> x1, std::vector<double> x2,
        std::vector<double> y, std::int32_t ncomp = 1)
{
    f->ndim = 2;
    f->var[0] = v1;
    f->var[1] = v2;
    f->n[0] = static_cast<std::int32_t>(x1.size());
    f->n[1] = static_cast<std::int32_t>(x2.size());
    f->ncomp = ncomp;
    f->axis[0] = keep(std::move(x1));
    f->axis[1] = keep(std::move(x2));
    f->values = keep(std::move(y));
}
void t3(DYNARY* f, std::vector<double> x1, std::vector<double> x2, std::vector<double> x3, std::vector<double> y)
{
    f->ndim = 3;
    f->var[0] = 3;
    f->var[1] = 8;
    f->var[2] = 7;
    f->n[0] = static_cast<std::int32_t>(x1.size());
    f->n[1] = static_cast<std::int32_t>(x2.size());
    f->n[2] = static_cast<std::int32_t>(x3.size());
    f->ncomp = 1;
    f->axis[0] = keep(std::move(x1));
    f->axis[1] = keep(std::move(x2));
    f->axis[2] = keep(std::move(x3));
    f->values = keep(std::move(y));
}

DB_STEP_DATA* make_model(Raw<DB_STEP_DATA>& r, std::int32_t dim)
{
    DB_STEP_DATA* m = r.p;
    seti(m, 0x1c, dim);   // dimension word read by 1400015e0 / 1400015f0
    seti(m, 0x88, 100);   // allocated function count (FNCLOC warning limit)
    seti(m, 0x8c, 10000); // allocated LLFNC size
    return m;
}

Material* make_material(Raw<Material>& r, DB_STEP_DATA* m, std::int32_t number, std::int32_t slot)
{
    Material* mat = r.p;
    seti(mat, 0, number);
    seti(mat, 4, slot);
    field<DB_STEP_DATA*>(mat, 0x2208) = m;
    return mat;
}

// IIFNC directory check: starts[k-1] and owners[k-1] of functions 1..n, and the end word
void check_iifnc(const std::vector<std::int32_t>& iifnc, const std::vector<std::int32_t>& starts,
                 const std::vector<std::int32_t>& owners, std::int32_t end)
{
    const std::size_t n = starts.size();
    for (std::size_t k = 1; k <= n; ++k) {
        CHECK_EQ(iifnc[2 * k - 2], starts[k - 1]);
        CHECK_EQ(iifnc[2 * k - 1], owners[k - 1]);
    }
    CHECK_EQ(iifnc[2 * n], end);
}

void check_words(const double* p, const std::vector<double>& want)
{
    for (std::size_t i = 0; i < want.size(); ++i) {
        if (!testkit::same_bits(p[i], want[i])) {
            testkit::fail(__FILE__, __LINE__,
                          "word " + std::to_string(i) + ": " + std::to_string(p[i]) + " vs " + std::to_string(want[i]));
        }
    }
}

std::string blanks(const std::string& s, std::size_t n) { return s + std::string(n - s.size(), ' '); }

} // namespace

// ---------------------------------------------------------------------------------------------
// 3D list: materials 3, 2 (unused), 1 in list order; dense slots 1 (number 1) and 2 (number 3).
TEST_CASE(material_list_3d)
{
    Raw<DB_STEP_DATA> rm;
    DB_STEP_DATA* m = make_model(rm, 3);
    Raw<Material> r1, r2, r3;
    Material* m1 = make_material(r1, m, 1, 1);
    Material* m2 = make_material(r2, m, 2, 0); // unused: not written, no slot
    Material* m3 = make_material(r3, m, 3, 2);
    auto* list = &field<MaterialList>(m, 0x1328);
    list->head = m3;
    field<Material*>(m3, 0x2200) = m2;
    field<Material*>(m2, 0x2200) = m1;
    list->tail = m1;
    list->count = 3;

    // ---- material 1 ----
    std::strcpy(reinterpret_cast<char*>(m1) + 0xc, "STEEL");
    setd(m1, 0x1ee8, 7.8);    // MATDEN -> w1
    setd(m1, 0x1ec8, 0.9);    // FRAE2H -> w10
    setd(m1, 0x1ed0, 0.25);   // -> w44
    setd(m1, 0x1ee0, 1500.0); // TMPLIQ -> w43
    setd(m1, 0x1ed8, 1400.0); // TMPSOL -> w42
    t0(dy(m1, 0x1ef0), 0.3);  // FPERV 1 const: w50 = 0, w49 = 0.3
    t1(dy(m1, 0x1f68), 1, {0.0, 1.0}, {0.5, 0.6}); // FPERV 2, var 1: w148 = 1; fn 1 (len 2n = 4)
    seti(m1, 0x1ad0, 0xc);    // FRCMOD internal 12 -> code 11 (14028e140)
    setd(m1, 0x1b00, 0.2);    // p0 -> w74
    setd(m1, 0x1b88, 0.7);    // p4 -> w149
    for (int k = 0; k < 9; ++k) setd(m1, 0x1c80 + 8 * k, 100.0 + k); // FRCMOD block +0x1b0..+0x1f0
    t0(dy(m1, 0x1b10), 33.0); // UTSDAT const: w107 = 0, w108 = 33
    seti(m1, 0x668, 1);       // HDNRUL -> w104 = 1
    seti(m1, 0x50, 1);        // FSTRES internal 1, switch 2
    seti(m1, 0xc8, 1);        //   flag 1 -> code 2
    t3(dy(m1, 0x78), {10.0, 20.0}, {0.1}, {500.0}, {1.0, 2.0}); // len 2+1+1+2 = 6 (14012fff0)
    seti(m1, 0x20d0, 3);      // COARSE count 3
    t2(dy(m1, 0x20d8), 3, 8, {1.0, 2.0}, {3.0, 4.0}, {5.0, 6.0, 7.0, 8.0}); // P: len 2+2+4 = 8
    t0(dy(m1, 0x2160), 0.125, 2);        // DIFBND self-describing: len 10
    t1(dy(m1, 0x670), 9, {100.0, 200.0}, {2e5, 1.9e5}); // YOUNG var 9: w23 = 1; len 4
    t0(dy(m1, 0x6e8), 0.3);              // POISON const
    t2(dy(m1, 0x760), 3, 0xc, {20.0, 500.0}, {1.0}, {1.1e-5, 1.3e-5}); // EXPAND: len 2+1+2 = 5
    setd(m1, 0x7d8, 20.0);               // EXPAND reference -> w88
    t0(dy(m1, 0x7e0), 40.0);             // THRCND const
    t1(dy(m1, 0x858), 0xc, {0.5}, {4.5}); // HEATCP var 0xc: w48 = 2; len 2
    seti(m1, 0x8d0, 1);                  // HEATCP extra -> w99
    t0(dy(m1, 0x1fe0), 0.5);             // MASDEN const: w82 = 0, w81 = 0.5
    t0(dy(m1, 0x8d8), 0.6);              // EMSVTY const: w46 = 0, w45 = 0.6
    t0(dy(m1, 0xf10), 0.0);              // HDNPHA const: w89 = 0, w90 = 0
    seti(m1, 0xac8, 1);                  // CREEP 1: table +0xbc0
    t1(dy(m1, 0xbc0), 3, {1.0, 2.0}, {11.0, 12.0, 21.0, 22.0}, 2); // len n*(nc+1) = 6
    setd(m1, 0x960, 0.001);              // DIFCOE type 0: w56 = const
    // GRNDAT mode 1: nine 1-D tables (n 1, ncomp 1) at +0x1088 + 0x78 k; x = k+1, y = 10(k+1)
    seti(m1, 0x17e8, 1);
    const std::int32_t gvar[9] = {3, 8, 3, 0x21, 3, 8, 8, 8, 0x21};
    for (int k = 0; k < 9; ++k) t1(dy(m1, 0x1088 + 0x78 * k), gvar[k], {k + 1.0}, {10.0 * (k + 1)});
    seti(m1, 0x1078, 1);                 // flag of table 3
    seti(m1, 0x107c, 0);                 // flag of table 5 -> code 0
    seti(m1, 0x1080, 1);                 // flag of table 7
    seti(m1, 0x1084, 1);                 // flag of table 9
    setd(m1, 0x14c8, 0.65);              // -> w128
    setd(m1, 0x14c0, 0.35);              // -> w129
    t2(dy(m1, 0x18f0), 3, 6, {300.0}, {0.1, 0.2}, {7.0, 8.0}); // BURGRS (3,6): code 4; len 5+2
    t0(dy(m1, 0x1968), 0.5);             // ALPHA const
    t1(dy(m1, 0x19e0), 8, {1.0}, {3.0}); // NDISFM var 8: code 2, len 2
    t0(dy(m1, 0x1a58), 0.0);             // RECVRY const
    t1(dy(m1, 0x1cd8), 9, {2.0}, {4.0}); // ELRST var 9: w51 = 2; len 2
    t2(dy(m1, 0x1d50), 3, 0x14, {5.0}, {6.0}, {7.0}); // PMEAB 2-D: w100 = 4, 3D: w137/w138; len 3
    t0(dy(m1, 0x1dc8), 0.2);             // PMITT const
    seti(m1, 0x1e40, 2);                 // ANISO model 2 -> w131 = 2 (3D base 131)
    t1(dy(m1, 0x1e48), 7, {0.0, 1.0}, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0}, 3); // sub 1, var 7; len 2*3+2
    seti(m1, 0x2150, 2);                 // SIZEMD -> w66
    seti(m1, 0x2154, 1); seti(m1, 0x2158, 1); seti(m1, 0x215c, 2); // TXTURE -> w150..152
    seti(m1, 0x21d8, 1);                 // SINTER model -> w154
    setd(m1, 0x21e0, 0.1); setd(m1, 0x21e8, 0.2); setd(m1, 0x21f0, 0.3); setd(m1, 0x21f8, 0.4);
    t1(dy(m1, 0x17f0), 3, {900.0}, {0.8}); // GBENGY self-describing 1-D: len 2 + 10
    setd(m1, 0x1868, 1.5);               // GBMOBI -> w160
    setd(m1, 0x1870, 2.5);               //        -> w161
    t0(dy(m1, 0x1878), 3.5);             // NUCSIZ self-describing const: len 10

    // ---- material 3 ----
    seti(m3, 0x1ad0, 0xd);               // FRCMOD internal 13 -> code 12, p1 = (double)+0x1bac
    setd(m3, 0x1b00, 0.15);
    t1(dy(m3, 0x1b90), 0x1f, {1.0}, {2.0}); // its table: n (+0x1bac) = 1; len 2n = 2
    seti(m3, 0x50, 4);                   // FSTRES internal 4 -> code 6
    t1(dy(m3, 0x1b0), 0xc, {0.5}, {150.0}); // first table var 0xc: w12 = 2, w14 = n; len 2
    t0(dy(m3, 0x138), 50.0);             // second table const: w13 = 0, w16 = 50
    seti(m3, 0x1cc8, 1);                 // MSTMTR mixture of materials 3 and 1
    seti(m3, 0x1ccc, 2);
    std::int32_t comps[2] = {3, 1};
    field<std::int32_t*>(m3, 0x1cd0) = comps;
    seti(m3, 0x950, 2);                  // DIFCOE type 2: table +0x9d0
    t1(dy(m3, 0x9d0), 3, {800.0}, {1e-6}); // w54 = n 1, w55 = ncomp+1 = 2; len 2
    seti(m3, 0x17e8, 4);                 // GRNDAT mode 4
    t2(dy(m3, 0x1740), 3, 8, {1000.0}, {0.1}, {50.0}); // w110 = 1, w111 = 1; len 3

    std::vector<double> llmpr(2 * 500, kU);
    std::vector<std::int32_t> iifnc(200, 0);
    std::vector<double> llfnc(400, kU);
    const std::int32_t width = 500;
    g::write_material_list(list, llmpr.data(), iifnc.data(), llfnc.data(), &width);

    // Function order and lengths (owner = slot):
    //  mat 1: 1 FPERV2 4, 2 FRCMOD11 9, 3 FSTRES 6, 4 COARSE data 8, 5 COARSE header 20,
    //         6 DIFBND 10, 7 YOUNG 4, 8 EXPAND 5, 9 HEATCP 2, 10 CREEP 6, 11 GRNDAT 18,
    //         12 BURGRS 7, 13 NDISFM 2, 14 ELRST 2, 15 PMEAB 3, 16 ANISO 8, 17 GBENGY 12, 18 NUCSIZ 10
    //  mat 3: 19 FRCMOD12 2, 20 FSTRES 2, 21 DIFBND 10, 22 MSTMTR 2, 23 DIFCOE 2, 24 GRNDAT4 3,
    //         25 GBENGY 10, 26 NUCSIZ 10
    check_iifnc(iifnc,
                {1, 5, 14, 20, 28, 48, 58, 62, 67, 69, 75, 93, 100, 102, 104, 107, 115, 127,
                 137, 139, 141, 151, 153, 155, 158, 168},
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2}, 178);
    CHECK_EQ(field<std::int32_t>(m, 0x10c), 26); // FNCLOC counter

    const double* w = llmpr.data();
    // MTNAME: w2..w9 = "STEEL" blank-padded to 64 bytes (140323bf0)
    CHECK(std::memcmp(reinterpret_cast<const char*>(w + 2), blanks("STEEL", 64).data(), 64) == 0);
    check_words(w, {1.0, 7.8});
    check_words(w + 10, {0.9, 2.0, 2.0, 1.0, 1.0, 3.0, kU}); // w10 FRAE2H, w11..15 FSTRES, w16 unset
    check_words(w + 23, {1.0, kU, kU, kU, kU, kU, 3.0, 2.0, 1.0}); // w23..31 types / n1 / n2 of P
    check_words(w + 32, {kU, 7.0, 0.3, 0.0, kU, 8.0, 40.0, 0.0, kU, 9.0}); // w32..41 const / fn of P
    check_words(w + 42, {1400.0, 1500.0, 0.25, 0.6, 0.0, kU, 2.0, 0.3, 0.0, 2.0, 14.0, 0.0, kU, kU, 0.001});
    check_words(w + 57, {1.0, 2.0, kU, kU, kU, kU, kU, 10.0, 5.0, 2.0}); // CREEP w57..64, COARSE w65, SIZEMD
    check_words(w + 70, {11.0, 9.0, 2.0, 0.0, 0.2});                    // FRCMOD 11
    check_words(w + 81, {0.5, 0.0, kU, kU, kU, kU, kU, 20.0, 0.0, 0.0, kU, kU, 0.0, kU, kU, kU, kU, kU, 1.0});
    check_words(w + 100, {4.0, 15.0, 0.0, 0.2, 1.0, kU, kU, 0.0, 33.0});
    // GRNDAT w109..130: codes 1, 2, 1, 3, 0, 2, 2, 2 and the 9th = 2 from the 8th table's var
    // (the 9th's own var 0x21 would give 3), n = 1 each, w128/w129, fn 11
    check_words(w + 109, {1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, 3.0, 1.0, 0.0, 1.0, 2.0, 1.0, 2.0, 1.0, 2.0, 1.0,
                          2.0, 1.0, 0.65, 0.35, 11.0});
    // ANISO (3D base 131): model 2, sub 1 var 7 -> 2, ncomp 3, w134 = n, fn 16; PMEAB n1/n2 w137/138
    check_words(w + 131, {2.0, 2.0, 3.0, 2.0, kU, 16.0, 1.0, 1.0});
    check_words(w + 139, {4.0, 12.0, 0.0, 0.5, 2.0, 13.0, 0.0, 0.0, 1.0, 1.0, 0.7}); // BURGRS..RECVRY, FPERV2, w149
    check_words(w + 150, {1.0, 1.0, 2.0, 6.0, 1.0, 0.1, 0.2, 0.3, 0.4, 17.0, 1.5, 2.5, 18.0, kU});

    // LLFNC data of material 1
    check_words(llfnc.data() + 0, {0.0, 0.5, 1.0, 0.6});                                    // fn 1
    check_words(llfnc.data() + 4, {100.0, 101.0, 104.0, 105.0, 106.0, 107.0, 108.0, 102.0, 103.0}); // fn 2
    check_words(llfnc.data() + 13, {10.0, 20.0, 0.1, 500.0, 1.0, 2.0});                      // fn 3
    check_words(llfnc.data() + 19, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0});                // fn 4
    check_words(llfnc.data() + 27, {3.0, 4.0, 5.0, 2.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                    0.0, 0.0, 0.0, 0.0, 0.0});                               // fn 5 header
    check_words(llfnc.data() + 47, {10.0, 0.0, 2.0, 0.125, kU, kU, kU, kU, kU, kU});          // fn 6
    check_words(llfnc.data() + 57, {100.0, 2e5, 200.0, 1.9e5});                              // fn 7
    check_words(llfnc.data() + 61, {20.0, 500.0, 1.0, 1.1e-5, 1.3e-5});                      // fn 8
    check_words(llfnc.data() + 66, {0.5, 4.5});                                              // fn 9
    check_words(llfnc.data() + 68, {1.0, 11.0, 12.0, 2.0, 21.0, 22.0});                      // fn 10
    check_words(llfnc.data() + 74, {1.0, 10.0, 2.0, 20.0, 3.0, 30.0, 4.0, 40.0, 5.0, 50.0, 6.0, 60.0, 7.0, 70.0,
                                    8.0, 80.0, 9.0, 90.0});                                  // fn 11
    check_words(llfnc.data() + 92, {1.0, 2.0, 300.0, 0.1, 0.2, 7.0, 8.0});                   // fn 12
    check_words(llfnc.data() + 99, {1.0, 3.0, 2.0, 4.0, 5.0, 6.0, 7.0});                     // fns 13-15
    check_words(llfnc.data() + 106, {0.0, 1.0, 2.0, 3.0, 1.0, 4.0, 5.0, 6.0});               // fn 16
    check_words(llfnc.data() + 114, {10.0, 1.0, 3.0, 1.0, 1.0, kU, kU, kU, kU, kU, 900.0, 0.8}); // fn 17
    check_words(llfnc.data() + 126, {10.0, 0.0, 1.0, 3.5, kU});                              // fn 18

    // material 3 at slot 2
    const double* v = llmpr.data() + 500;
    check_words(v, {3.0, 0.0});
    check_words(v + 11, {6.0, 2.0, 0.0, 1.0, kU, 50.0, kU, 20.0, kU});    // FSTRES code 6
    check_words(v + 53, {2.0, 1.0, 2.0, 23.0});                           // DIFCOE 2
    check_words(v + 65, {0.0});                                           // COARSE none
    check_words(v + 70, {12.0, 19.0, 0.0, 0.0, 0.15});                    // FRCMOD 12: w71 = fn
    check_words(v + 93, {2.0, 22.0});                                     // MSTMTR
    check_words(v + 104, {0.0});
    check_words(v + 109, {4.0, 1.0, 1.0, 24.0, kU});                      // GRNDAT 4
    check_words(v + 131, {0.0, 0.0, kU});                                 // ANISO none
    check_words(v + 153, {21.0});
    check_words(v + 159, {25.0, 0.0, 0.0, 26.0});
    check_words(llfnc.data() + 136, {1.0, 2.0, 0.5, 150.0, 10.0, 0.0, 0.0, 0.0});
    check_words(llfnc.data() + 150, {2.0, 1.0, 800.0, 1e-6, 1000.0, 0.1, 50.0, 10.0}); // MSTMTR slots 2, 1
    CHECK(llfnc[177] == kU);
}

// ---------------------------------------------------------------------------------------------
// 2D material: MASDEN 2-D (overwrites w1), PMEAB n1/n2 at w131/w132, ANISO base 133,
// FRCMOD 17, FSTRES code 15, CREEP 7 (both copy n into the int before their first table).
TEST_CASE(material_record_2d)
{
    Raw<DB_STEP_DATA> rm;
    DB_STEP_DATA* m = make_model(rm, 2);
    Raw<Material> r;
    Material* mat = make_material(r, m, 5, 1);
    setd(mat, 0x1ee8, 7.0);                                          // MATDEN -> w1 first
    t2(dy(mat, 0x1fe0), 3, 0xc, {1.0, 2.0}, {3.0, 4.0, 5.0}, {6.0, 7.0, 8.0, 9.0, 10.0, 11.0}); // len 11
    seti(mat, 0x1ad0, 0x11);                                         // FRCMOD 17, p1 = (double)+0x1c24
    t1(dy(mat, 0x1c08), 3, {0.0, 1.0}, {0.1, 0.2});                  // n at +0x1c24 = 2; len 4
    seti(mat, 0x50, 12);                                             // FSTRES internal 12 -> code 15
    t1(dy(mat, 0x480), 3, {1.0, 2.0}, {3.0, 4.0, 5.0, 6.0}, 2);      // 14012fd30: 2*3 = 6
    t1(dy(mat, 0x570), 3, {7.0}, {8.0});                             // len 2; w14 = ncomp 1
    seti(mat, 0xac8, 7);                                             // CREEP 7
    t1(dy(mat, 0xda8), 3, {600.0}, {1e-3});                          // len 2
    t2(dy(mat, 0x1d50), 3, 0x14, {1.0, 2.0}, {3.0}, {4.0, 5.0});     // PMEAB: len 5
    seti(mat, 0x1e40, 5);                                            // ANISO model 5: -(+0x1ec0)
    seti(mat, 0x1ec0, 7);
    t2(dy(mat, 0x1e48), 3, 0x1e, {0.5}, {1.0, 2.0}, {3.0, 4.0});     // sub 2 var 3 -> 4; len 1*2+1+2

    const std::int32_t slot = 1;
    const std::int32_t table[5] = {0, 0, 0, 0, 1};
    std::vector<double> rec(500, kU);
    std::vector<std::int32_t> iifnc(100, 0);
    std::vector<double> llfnc(200, kU);
    g::write_material_record(mat, rec.data(), iifnc.data(), llfnc.data(), &slot, table);

    // 1 FRCMOD17 4, 2 FSTRES 6, 3 FSTRES 2, 4 DIFBND 10, 5 MASDEN 11, 6 CREEP 2, 7 PMEAB 5,
    // 8 ANISO 5, 9 GBENGY 10, 10 NUCSIZ 10
    check_iifnc(iifnc, {1, 5, 11, 13, 23, 34, 36, 41, 46, 56}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 66);

    const double* w = rec.data();
    check_words(w, {5.0, 3.0}); // w1: 7.0 (MATDEN), then MASDEN n1 = 2, then n2 = 3
    check_words(w + 11, {15.0, 2.0, kU, 1.0, 2.0, kU, 3.0});
    check_words(w + 57, {7.0, 1.0, kU, kU, 6.0, kU, kU});
    check_words(w + 70, {17.0, 2.0, 1.0, 0.0, 0.0});
    check_words(w + 81, {kU, 5.0, 3.0});              // MASDEN: const word untouched, fn 5, type 3
    check_words(w + 100, {4.0, 7.0});
    check_words(w + 131, {2.0, 1.0, -7.0, 4.0, 1.0, 1.0, 2.0, 8.0, 0.0}); // PMEAB n1 n2, ANISO 133..138
    check_words(w + 153, {4.0});
    check_words(w + 159, {9.0, 0.0, 0.0, 10.0});
    CHECK_EQ(field<std::int32_t>(mat, 0x478), 2); // FSTRES: +0x478 = n of +0x480
    CHECK_EQ(field<std::int32_t>(mat, 0xda4), 1); // CREEP: +0xda4 = n of +0xda8

    check_words(llfnc.data(), {0.0, 0.1, 1.0, 0.2, 1.0, 3.0, 4.0, 2.0, 5.0, 6.0, 7.0, 8.0});
    check_words(llfnc.data() + 12, {10.0, 0.0, 0.0, 0.0});
    check_words(llfnc.data() + 22, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0});
    check_words(llfnc.data() + 33, {600.0, 1e-3, 1.0, 2.0, 3.0, 4.0, 5.0, 0.5, 1.0, 2.0, 3.0, 4.0, 10.0});
}

// ---------------------------------------------------------------------------------------------
TEST_CASE(material_frcmod_unknown_type)
{
    Raw<Material> r;
    seti(r.p, 0x1ad0, 0xe); // no case in 14028e140 -> false
    double out[6] = {1, 1, 1, 1, 1, 1};
    CHECK(!g::material_frcmod_words(r.p, out));
    check_words(out, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0});
    seti(r.p, 0x1ad0, 7); // code 7: p1 = +0x1af0, p2 = +0x1af8
    setd(r.p, 0x1af0, 1.5);
    setd(r.p, 0x1af8, 2.5);
    setd(r.p, 0x1b00, 0.5);
    CHECK(g::material_frcmod_words(r.p, out));
    check_words(out, {7.0, 0.5, 1.5, 2.5, 0.0, 0.0});
}

// ---------------------------------------------------------------------------------------------
// GRNDAT mode 2 pieces: grain records and a record list.
TEST_CASE(grain_records)
{
    Raw<DB_STEP_DATA> rm;
    DB_STEP_DATA* m = make_model(rm, 3);
    Raw<Cls_14028a4c0> ra, rb;
    Cls_14028a4c0* a = ra.p;
    seti(a, 0, 2);
    setd(a, 8, 1.5);
    setd(a, 0x10, 2.5);
    setd(a, 0x68, 0.75);
    seti(a, 0x70, 4);
    t0(dy(a, 0x78), 9.0);                           // sub 1 const: code 0, n1 word = 9; length 0
    t1(dy(a, 0xf0), 0x17, {1.0}, {2.0});            // sub 2 var 0x17: code 2, n 1; length 2
    t2(dy(a, 0x168), 3, 0x17, {3.0}, {4.0}, {5.0}); // sub 3 (3, 0x17): code 3; length 3
    Cls_14028a4c0* b = rb.p;                        // main-type record: sub 1/2 skipped
    field<std::uint8_t>(b, 0x1e0) = 1;
    t0(dy(b, 0x168), 6.0);
    field<Cls_14028a4c0*>(a, 0x1e8) = b;

    // single record: words 0..36 header, data from 37 (sub 2 at 37, sub 3 at 39)
    std::vector<double> out(50, kU);
    g::write_grain_record(a, out.data());
    check_words(out.data(), {2.0, 1.5, 2.5, kU});
    check_words(out.data() + 23, {0.75, 4.0, 0.0, 2.0, 3.0, 9.0, kU, kU, 1.0, kU, kU, 1.0, 1.0, kU, 1.0, 2.0,
                                  3.0, 4.0, 5.0, kU});

    // list: fn 1 = list (len 2, two record numbers), fn 2 = record a (37 + 0 + 2 + 3 = 42),
    // fn 3 = record b (37 + 0 for a non-list constant)
    Cls_14028b640::RecordList list{};
    list.head = a;
    list.tail = b;
    list.count = 2;
    std::vector<std::int32_t> iifnc(20, 0);
    std::vector<double> llfnc(120, kU);
    const std::int32_t slot = 4;
    double count = kU, fid = kU;
    g::write_grain_record_list(&list, m, &slot, iifnc.data(), llfnc.data(), &count, &fid);
    check_iifnc(iifnc, {1, 3, 45}, {4, 4, 4}, 82);
    CHECK(count == 2.0);
    CHECK(fid == 1.0);
    check_words(llfnc.data(), {2.0, 3.0, 2.0, 1.5, 2.5});
    check_words(llfnc.data() + 44, {0.0, kU});                  // record b: n = 0
    check_words(llfnc.data() + 44 + 25, {-1.0, -1.0, 0.0, kU}); // b: subs 1/2 -1, sub 3 const
    check_words(llfnc.data() + 44 + 34, {6.0});
}

// ---------------------------------------------------------------------------------------------
// LLTFM: materials 1 (slot 1), 2 (unused), 3 (slot 2), NMAT 2. Record (from, to) at
// 50 * ((slot_to - 1) * 2 - 1 + slot_from): (1,3) -> 100, (3,1) -> 50, (3,3) -> 150.
TEST_CASE(phase_pair_list)
{
    Raw<DB_STEP_DATA> rm;
    DB_STEP_DATA* m = make_model(rm, 3);
    seti(m, 0x78, 2); // NMAT
    Raw<Material> r1, r2, r3;
    Material* m1 = make_material(r1, m, 1, 1);
    Material* m2 = make_material(r2, m, 2, 0);
    Material* m3 = make_material(r3, m, 3, 2);
    auto* mats = &field<MaterialList>(m, 0x1328);
    mats->head = m1;
    field<Material*>(m1, 0x2200) = m2;
    field<Material*>(m2, 0x2200) = m3;
    mats->count = 3;

    Raw<PhasePair> p13, p31, p12, p33;
    auto mk = [m](PhasePair* p, std::int32_t from, std::int32_t to) {
        seti(p, 0, from);
        seti(p, 4, to);
        field<DB_STEP_DATA*>(p, 0x778) = m;
    };
    mk(p13.p, 1, 3);
    mk(p31.p, 3, 1);
    mk(p12.p, 1, 2);
    mk(p33.p, 3, 3);
    auto* pl = &field<PhasePairList>(m, 0x1358);
    field<DB_STEP_DATA*>(pl, 0x18) = m;
    pl->head = p12.p;
    field<PhasePair*>(p12.p, 0x770) = p33.p;
    field<PhasePair*>(p33.p, 0x770) = p31.p;
    field<PhasePair*>(p31.p, 0x770) = p13.p;
    pl->count = 4;

    // pair (1, 3): PHASTF 2, TTTD code 1
    PhasePair* a = p13.p;
    seti(a, 8, 2);
    seti(a, 0xc, 1);
    seti(a, 0x10, 1);                                  // w1 = 1
    std::strcpy(reinterpret_cast<char*>(a) + 0x18, "Austenite to Pearlite");
    t1(dy(a, 0x478), 3, {700.0}, {80.0});              // PHASLH var 3: w2 = 1; fn 1 len 2
    t0(dy(a, 0x4f0), 2.5);                             // PHASVL const
    seti(a, 0x5e4, 1);                                 // TRNSFP on
    t2(dy(a, 0x5e8), 3, 0xc, {1.0}, {2.0}, {3.0});     // w15 = 3, w31/w32 = 1; fn 2 len 3
    seti(a, 0x5e0, 3);                                 // PHEQVF code -> w33
    t0(dy(a, 0x568), 0.4);                             //   const -> w34
    seti(a, 0x14, 1);                                  // TTTD internal 1: w6 = 1, w13 = 1
    DYNARY* t = dy(a, 0x78);                           // 3-D 1x1x1, ncomp 2
    t3(t, {10.0}, {20.0}, {30.0}, {0.25, 0.75});
    t->ncomp = 2;                                      // +0xb8 -> w8; 140336300: 2 + 3 + 2 = 7
    setd(a, 0x170, 0.01);                              // last two words of fn 3
    setd(a, 0x178, 0.99);
    seti(a, 0xf0, 4);                                  // -> w49
    seti(a, 0x660, 1);                                 // COARSN on
    t1(dy(a, 0x678), 3, {1.0}, {2.0});                 // var 3: w38 = 1; fn 4 len 2
    setd(a, 0x668, 0.5);
    setd(a, 0x670, 0.6);
    t0(dy(a, 0x6f0), 1.25);                            // IPSNRG const
    seti(a, 0x768, 2);                                 // VARSEL
    // pair (3, 1): PHASTF 1, TTTD code 5 (internal 7)
    PhasePair* b = p31.p;
    seti(b, 8, 1);
    seti(b, 0xc, 1);
    seti(b, 0x14, 7);                                  // w6 = 5, w8 = 6; fn 5 len 6
    for (int i = 0; i < 6; ++i) setd(b, 0x360 + 8 * i, 1.0 + i);
    t2(dy(b, 0x6f0), 3, 0x17, {1.0}, {2.0}, {3.0});    // IPSNRG 2-D: w44 = 3; fn 6 len 3
    // pair (1, 2): material 2 unused -> not written; pair (3, 3): inactive -> only w0
    seti(p12.p, 8, 1);
    seti(p12.p, 0xc, 1);

    std::vector<double> lltfm(4 * 50, kU);
    std::vector<std::int32_t> iifnc(40, 0);
    std::vector<double> llfnc(60, kU);
    const std::int32_t width = 50;
    g::write_phase_pair_list(pl, lltfm.data(), iifnc.data(), llfnc.data(), &width);

    // functions in loop order: (1,3) first (from 1), then (3,1); owner 0
    check_iifnc(iifnc, {1, 3, 6, 13, 15, 21}, {0, 0, 0, 0, 0, 0}, 24);

    CHECK(lltfm[0] == kU); // (1,1): no pair object
    const double* w = lltfm.data() + 100;
    check_words(w, {2.0, 1.0, 1.0, 1.0, 0.0, 2.5, 1.0, 3.0, 2.0, 1.0, 1.0, 1.0, kU, 1.0, 1.0, 3.0, 2.0, kU});
    CHECK(std::memcmp(reinterpret_cast<const char*>(w + 18), blanks("Austenite to Pearlite", 80).data(), 80) == 0);
    check_words(w + 28, {kU, kU, 1.0, 1.0, 1.0, 3.0, 0.4, kU, kU, 1.0, 1.0, kU, kU, 4.0, 0.5, 0.6, 0.0, kU, kU,
                         1.25, 2.0, 4.0});
    check_words(llfnc.data(), {700.0, 80.0, 1.0, 2.0, 3.0, 10.0, 20.0, 30.0, 0.25, 0.75, 0.01, 0.99, 1.0, 2.0});

    const double* v = lltfm.data() + 50;
    check_words(v, {1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 5.0, 5.0, 6.0, kU, kU, kU, kU, kU, 0.0, kU});
    CHECK(std::memcmp(reinterpret_cast<const char*>(v + 18), blanks("", 80).data(), 80) == 0);
    check_words(v + 28, {kU, kU, 2.0, kU, kU, kU, kU, kU, kU, 0.0, kU, kU, kU, kU, kU, kU, 3.0, 1.0, 1.0, 6.0, 0.0,
                         kU});
    check_words(llfnc.data() + 14, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 1.0, 2.0, 3.0, kU});

    check_words(lltfm.data() + 150, {0.0, kU}); // (3,3) inactive
}

int main() { return testkit::run_all(); }
