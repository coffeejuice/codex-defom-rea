// ctest-labels: p4-07
// Tier-1 tests of packet p4-07's object statistics (defmodel/hdrstats). Expected values are hand
// calculations shown next to each check, or the immediates of 1401c7f80. The oracle comparison
// against DEF_PRE's GENDB output is defdbio.step_header_oracle.
#include <bit>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <new>
#include <vector>

#include <defmodel/hdrstats/ext_p4_07.hpp>
#include <defmodel/hdrstats/globals_p4_07.hpp>
#include <defmodel/hdrstats/object_stats.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/state_var_base.hpp>
#include <testkit/check.hpp>

using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;
using def::model::StateVarBase;
namespace p407 = def::model::p407;
using p407::Vec3;

namespace {

template <class T>
T& at(void* base, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<std::byte*>(base) + off);
}

// Zeroed raw object storage (the functions under test read raw fields only).
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

// The 3-vector helpers as decompiled (14023a1b0 a + b, 14023a4b0 a x b, 1402399b0 + sqrt).
void install_vector_fakes()
{
    using namespace p407::ext;
    fun_140239700 = [](Vec3* out, double x, double y, double z) {
        *out = Vec3{{x, y, z}};
        return out;
    };
    fun_14023a1b0 = [](Vec3* out, const Vec3* a, const Vec3* b) {
        *out = Vec3{{a->v[0] + b->v[0], a->v[1] + b->v[1], a->v[2] + b->v[2]}};
        return out;
    };
    fun_14023a4b0 = [](Vec3* out, const Vec3* a, const Vec3* b) {
        *out = Vec3{{a->v[1] * b->v[2] - a->v[2] * b->v[1], a->v[2] * b->v[0] - a->v[0] * b->v[2],
                     a->v[0] * b->v[1] - a->v[1] * b->v[0]}};
        return out;
    };
    fun_140239a30 = [](const Vec3* a) {
        return std::sqrt(a->v[0] * a->v[0] + a->v[1] * a->v[1] + a->v[2] * a->v[2]);
    };
}

} // namespace

TEST_CASE(slot_table_2d_and_3d)
{
    p407::reset_globals_p4_07();
    std::int32_t dim = 2;
    p407::load_hdr_slots(&dim);
    CHECK_EQ(p407::hdr_slot(0x1406e8524), 8);    // platform code at H[8]
    CHECK_EQ(p407::hdr_slot(0x1406e8528), 0x22); // object name 34..41
    CHECK_EQ(p407::hdr_slot(0x1406e8538), 0x2a); // nodes 42, elements 43
    CHECK_EQ(p407::hdr_slot(0x1406e853c), 0x2b);
    CHECK_EQ(p407::hdr_slot(0x1406e85cc), 0);    // surface area over FORCE x (the 2D defect)
    CHECK_EQ(p407::hdr_slot(0x1406e85d0), 0);
    CHECK_EQ(p407::hdr_slot(0x1406e85d4), 1);
    CHECK_EQ(p407::hdr_slot(0x1406e8608), 0x38);
    CHECK_EQ(p407::hdr_slot(0x1406e8708), 0x77);
    CHECK(p407::g_hdr_label_table == p407::HdrLabelTable::table_1405d4ac0);
    dim = 3;
    p407::load_hdr_slots(&dim);
    CHECK_EQ(p407::hdr_slot(0x1406e8524), 5);
    CHECK_EQ(p407::hdr_slot(0x1406e8528), 0x6a);  // name 106..113
    CHECK_EQ(p407::hdr_slot(0x1406e8538), 0x58);  // nodes 88
    CHECK_EQ(p407::hdr_slot(0x1406e85c8), 0x18);  // volume 24
    CHECK_EQ(p407::hdr_slot(0x1406e85cc), 0x73);  // surface area 115
    CHECK_EQ(p407::hdr_slot(0x1406e85d8), 2);     // FORCE z
    CHECK_EQ(p407::hdr_slot(0x1406e8680), 0x27);  // stress components 39..50
    CHECK_EQ(p407::hdr_slot(0x1406e8708), 0x80);
    CHECK(p407::g_hdr_label_table == p407::HdrLabelTable::table_1405d54c0);
    dim = 7; // anything but 2 is 3D
    p407::load_hdr_slots(&dim);
    CHECK_EQ(p407::hdr_slot(0x1406e8524), 5);
    p407::reset_globals_p4_07();
}

TEST_CASE(principal_2d_uniaxial)
{
    const double s[4] = {100.0, 50.0, 0.0, 0.0}; // sxx, syy, szz, sxy
    double eff = 0, pmax = 0, p1 = 0, p2 = 0, ang = -7.0;
    std::int32_t kind = 0;
    p407::principal_2d(&kind, s, &eff, &pmax, &p1, &p2, &ang);
    // c = 75, r = sqrt(50*50*0.25 + 0) = 25
    CHECK_BITS(p1, 100.0);
    CHECK_BITS(p2, 50.0);
    CHECK_BITS(pmax, 100.0); // max(p2, p1, szz)
    CHECK_BITS(ang, 0.0);    // |pmax - sxx| < 1e-5
    // ((0*0.5 + (0 + (2500 + 10000))) * 2) / 3
    CHECK_BITS(eff, std::sqrt(25000.0 / 3.0));
    kind = 1; // von Mises: mean 50, (50^2 + 50^2) * 3 * 0.5 = 7500 (dy = 0)
    p407::principal_2d(&kind, s, &eff, &pmax, &p1, &p2, &ang);
    CHECK_BITS(eff, std::sqrt(7500.0));
    kind = 2; // minimum branch: min(p2, p1, szz) = 0; |pmax - syy| test sets nothing here
    ang = -7.0;
    p407::principal_2d(&kind, s, &eff, &pmax, &p1, &p2, &ang);
    CHECK_BITS(pmax, 0.0);
    CHECK_BITS(ang, -7.0);
}

TEST_CASE(principal_2d_shear_angle)
{
    const double s[4] = {0.0, 0.0, 0.0, 10.0};
    double eff = 0, pmax = 0, p1 = 0, p2 = 0, ang = 0;
    std::int32_t kind = 1; // t = sxy (no halving for kinds 1, 2)
    p407::principal_2d(&kind, s, &eff, &pmax, &p1, &p2, &ang);
    CHECK_BITS(p1, 10.0);
    CHECK_BITS(p2, -10.0);
    // d = 0 -> 1e-6 (REAL*4 rounded); angle = atan(20 / d) * 0.5, + pi/2 (REAL*4 digits) since syy >= sxx
    const double tiny = std::bit_cast<double>(std::uint64_t{0x3eb0c6f7a0000000});
    const double half_pi_f = std::bit_cast<double>(std::uint64_t{0x3ff921fb00000000});
    const double a = std::atan(20.0 / tiny) * 0.5; // ps_atan and atan agree near pi/2 to the bit? checked loosely
    CHECK(std::fabs(ang - (a + half_pi_f)) < 1e-12);
}

TEST_CASE(cubic_roots_and_failures)
{
    // (x - 1)(x - 2)(x - 3) = x^3 - 6x^2 + 11x - 6
    const double a = -6.0, b = 11.0, c = -6.0;
    double rmax = 0, rmid = 0, rmin = 0;
    std::int32_t fail = -1;
    p407::cubic_roots(&a, &b, &c, &rmax, &rmid, &rmin, &fail);
    CHECK_EQ(fail, 0);
    CHECK(std::fabs(rmax - 3.0) < 1e-12);
    CHECK(std::fabs(rmid - 2.0) < 1e-12);
    CHECK(std::fabs(rmin - 1.0) < 1e-12);
    // q = (a^2 - 3b) / 9 <= 0 -> fail, outputs zeroed first
    const double z = 0.0;
    rmax = rmid = rmin = 5.0;
    p407::cubic_roots(&z, &z, &z, &rmax, &rmid, &rmin, &fail);
    CHECK_EQ(fail, 1);
    CHECK_BITS(rmax, 0.0);
    CHECK_BITS(rmid, 0.0);
    CHECK_BITS(rmin, 0.0);
}

TEST_CASE(principal_3d_uniaxial_and_zero)
{
    const double s[6] = {100.0, 50.0, 10.0, 0.0, 0.0, 0.0};
    double eff = 0, pmax = 0, pmid = 0, pmin = 0;
    std::int32_t kind = 1;
    p407::principal_3d(&kind, s, &eff, &pmax, &pmid, &pmin);
    // ((0 + (0 + 0)) * 6 + ((10-100)^2 + ((50-10)^2 + (100-50)^2))) * 0.5 = 6100
    CHECK_BITS(eff, std::sqrt(6100.0));
    CHECK(std::fabs(pmax - 100.0) < 1e-9);
    CHECK(std::fabs(pmid - 50.0) < 1e-9);
    CHECK(std::fabs(pmin - 10.0) < 1e-9);
    kind = 2; // 2/3, 4/3 weighted norm: sqrt(4/3*0 ... + 2/3*100 + ...) (weights are the binary's words)
    p407::principal_3d(&kind, s, &eff, &pmax, &pmid, &pmin);
    CHECK(std::fabs(eff - std::sqrt(2.0 / 3.0 * (10000.0 + 2500.0 + 100.0))) < 1e-9);
    double zero[6] = {0, 0, 0, 0, 0, 0};
    std::int32_t fail = 9;
    pmax = pmid = pmin = 1.0;
    p407::principal_3d_roots(zero, &pmax, &pmid, &pmin, &fail);
    CHECK_EQ(fail, 0);
    CHECK_BITS(pmax, 0.0);
    CHECK_BITS(pmid, 0.0);
    CHECK_BITS(pmin, 0.0);
}

TEST_CASE(jacobi_and_eigsrt)
{
    // diagonal: no off-diagonal sum, returns 0 rotations, d = diagonal, v = identity
    float a[9] = {3, 0, 0, 0, 1, 0, 0, 0, 2};
    float d[3], v[9];
    CHECK_EQ(p407::jacobi3(a, d, v), 0);
    CHECK(d[0] == 3.0f && d[1] == 1.0f && d[2] == 2.0f);
    CHECK(v[0] == 1.0f && v[4] == 1.0f && v[8] == 1.0f && v[1] == 0.0f);
    p407::eigsrt3(d, v);
    CHECK(d[0] == 3.0f && d[1] == 2.0f && d[2] == 1.0f);
    // the columns follow: column 1 now is old column 2 (z), column 2 old column 1 (y)
    CHECK(v[1 * 3 + 2] == 1.0f && v[2 * 3 + 1] == 1.0f);
    // [[2,1,0],[1,2,0],[0,0,3]]: eigenvalues 3, 1, 3
    float b[9] = {2, 1, 0, 1, 2, 0, 0, 0, 3};
    const std::int32_t nrot = p407::jacobi3(b, d, v);
    CHECK(nrot >= 1 && nrot < 50);
    p407::eigsrt3(d, v);
    CHECK(std::fabs(d[0] - 3.0f) < 1e-5f && std::fabs(d[1] - 3.0f) < 1e-5f && std::fabs(d[2] - 1.0f) < 1e-5f);
}

TEST_CASE(fabs_float_roundtrip)
{
    CHECK(p407::fabs_float(-2.5f) == 2.5f);
    CHECK(p407::fabs_float_b(-0.0f) == 0.0f);
}

TEST_CASE(polygon_area_unit_square)
{
    install_vector_fakes();
    const Vec3 sq[4] = {{{0, 0, 0}}, {{1, 0, 0}}, {{1, 1, 0}}, {{0, 1, 0}}};
    CHECK_BITS(p407::polygon_area(4, sq), 1.0);
    const Vec3 tri[4] = {{{0, 0, 0}}, {{2, 0, 0}}, {{0, 2, 0}}, {{0, 2, 0}}}; // triangle, last node repeated
    CHECK_BITS(p407::polygon_area(4, tri), 2.0);
    p407::reset_ext_p4_07();
}

TEST_CASE(surface_area_from_mesh_faces)
{
    install_vector_fakes();
    Raw model(sizeof(DB_STEP_DATA));
    Raw obj(sizeof(LLOBJ_DATA));
    at<std::int32_t>(model.p, 0x1c) = 3;
    auto* o = reinterpret_cast<LLOBJ_DATA*>(obj.p);
    o->owner = reinterpret_cast<DB_STEP_DATA*>(model.p);
    o->u87c[0] = 1;
    // two unit squares sharing an edge; node numbers may be negative (abs taken)
    static double xyz[] = {0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 2, 0, 0, 2, 1, 0};
    static std::int32_t faces[] = {1, 2, 3, 4, 2, -5, 6, 3};
    at<std::int32_t>(obj.p, 0xbe0 + 0x10) = 2;
    at<std::int32_t*>(obj.p, 0xbe0 + 0x18) = faces;
    p407::ext::fun_1401808d0 = [](void*) { return xyz; };
    CHECK_BITS(p407::obj_surface_area(o, 1), 2.0);
    CHECK_BITS(p407::obj_surface_area(o, 5), 0.0);   // other selectors: 0
    o->u87c[0] = 0;
    CHECK_BITS(p407::obj_surface_area(o, 1), 0.0);   // no mesh
    at<std::int32_t>(model.p, 0x1c) = 2;
    CHECK_BITS(p407::obj_surface_area(o, 1), 0.0);   // 2D: 0
    p407::reset_ext_p4_07();
}

TEST_CASE(principal_kind_table)
{
    CHECK(p407::sv_kind_is_principal(700, 0));
    CHECK(p407::sv_kind_is_principal(14000, 9));
    CHECK(p407::sv_kind_is_principal(0x14b4, 0));
    CHECK(!p407::sv_kind_is_principal(0x14b4, 2));
    CHECK(p407::sv_kind_is_principal(500, 5));
    CHECK(!p407::sv_kind_is_principal(500, 1));
    CHECK(p407::sv_kind_is_principal(0xd48, 3));
    CHECK(!p407::sv_kind_is_principal(600, 2));
}

TEST_CASE(strain_block_offsets)
{
    Raw model(sizeof(DB_STEP_DATA));
    Raw obj(sizeof(LLOBJ_DATA));
    auto* o = reinterpret_cast<LLOBJ_DATA*>(obj.p);
    o->owner = reinterpret_cast<DB_STEP_DATA*>(model.p);
    std::int32_t* cnt = &at<std::int32_t>(model.p, 0x86c); // STNOUT 1..5
    std::int32_t code = 3000;
    CHECK_EQ(p407::strain_block_offset(o, &code), 0);      // all counts 0
    cnt[0] = 1;
    CHECK_EQ(p407::strain_block_offset(o, &code), 0);
    code = 3100;
    CHECK_EQ(p407::strain_block_offset(o, &code), -1);     // STNOUT2 not 1
    cnt[1] = 1;
    CHECK_EQ(p407::strain_block_offset(o, &code), 1);      // 0 + STNOUT1
    code = 3500;
    cnt[4] = 2;
    CHECK_EQ(p407::strain_block_offset(o, &code), 4);      // 0 + 1 + 1 + 0 + 0 + 2
    code = 42;
    CHECK_EQ(p407::strain_block_offset(o, &code), -1);
}

TEST_CASE(state_var_value_index)
{
    StateVarBase sv(-1, nullptr);
    sv.vslot1_140301710(2, 3, 2); // 12 values
    p407::sv_set_id(&sv, 0x26);
    CHECK_EQ(sv.get_id(), 0x26);
    p407::sv_set_value(&sv, 1, 1, 2, 5.0); // row * 3 * 2 + col * 3 + k = 6 + 3 + 2
    CHECK_BITS(sv.value_data()[11], 5.0);
}

TEST_CASE(min_max_scan_rules)
{
    Raw model(sizeof(DB_STEP_DATA));
    Raw obj(sizeof(LLOBJ_DATA));
    auto* o = reinterpret_cast<LLOBJ_DATA*>(obj.p);
    o->owner = reinterpret_cast<DB_STEP_DATA*>(model.p);
    at<std::int32_t>(model.p, 0x1c) = 3;
    o->u87c[1] = 3; // nodes
    o->u87c[2] = 2; // elements
    static double vel[] = {3, 4, 0, 0, 0, 0, 1, 2, 2}; // lengths 5, 0, 3
    static double temp[] = {20, -5, 7};
    static std::int32_t dims[0x4f] = {};
    dims[2] = 3; // W[2] velocity
    dims[8] = 1; // W[8] temperature
    std::memcpy(&at<std::int32_t>(model.p, 0x15f8), dims, sizeof dims);
    at<double*>(obj.p, 0x8a0 + 0x18) = vel;
    at<double*>(obj.p, 0x8a0 + 0x40) = temp;
    using namespace p407::ext;
    fun_14017f020 = [](void*) -> std::int32_t* { return dims; };
    fun_14017ffe0 = [](void*, std::int32_t, std::int32_t*, std::int32_t) {};
    std::int32_t code = 700, comp = 0;
    bool init = true;
    double lo = 9, hi = 9;
    p407::obj_min_max(o, &code, &lo, &hi, &comp, &init);
    // init from vel[0] = 3 (the first value, not the first length), then lengths 5, 0, 3
    CHECK_BITS(lo, 0.0);
    CHECK_BITS(hi, 5.0);
    code = 600;
    init = false; // start from 0
    p407::obj_min_max(o, &code, &lo, &hi, &comp, &init);
    CHECK_BITS(lo, -5.0);
    CHECK_BITS(hi, 20.0);
    code = 4711; // unknown code: zeros
    p407::obj_min_max(o, &code, &lo, &hi, &comp, &init);
    CHECK_BITS(lo, 0.0);
    CHECK_BITS(hi, 0.0);
    o->owner = nullptr; // no owner: zeros, nothing read
    code = 600;
    lo = hi = 1.0;
    p407::obj_min_max(o, &code, &lo, &hi, &comp, &init);
    CHECK_BITS(lo, 0.0);
    CHECK_BITS(hi, 0.0);
    p407::reset_ext_p4_07();
}

int main() { return testkit::run_all(); }
