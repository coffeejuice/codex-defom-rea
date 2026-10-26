// ctest-labels: p1-10 members furnace
// Tier 1 for the furnace model (Cls_1403753f0, 140375470 .. 1403769f0).
//
// The curve list and curve primitives belong to other packets and are still stubs, so this test
// defines fakes for every one the furnace code calls (members/ext_deps.hpp and the
// Cls_1401364a0 ctor/dtor). Because the test object defines them, the linker never pulls the
// stub objects from the library. When those packets land, these fakes go and the test runs on
// the real functions. Expected values are hand-traced from the decompiles of 140375600,
// 140375e90 and 1403769f0 (constants: the @data addresses quoted in members/furnace.cpp).
#define _CRT_SECURE_NO_WARNINGS // strncpy / getenv are what the original and the test use
#include <defmodel/layout/cls_1403753f0.hpp>
#include <defmodel/members/ext_deps.hpp>
#include <defmodel/members/furnace.hpp>
#include <testkit/check.hpp>

#include <cstring>
#include <fstream>
#include <map>
#include <tuple>
#include <vector>

using namespace def::model;

// ---- fakes ------------------------------------------------------------------------------------

namespace {

struct FakeCurve {
    alignas(DYNARY) unsigned char raw[sizeof(DYNARY)];
    std::int32_t type = -1;
    bool allocated = false;
    DYNARY* c() { return reinterpret_cast<DYNARY*>(raw); }
};

std::vector<FakeCurve*> g_curves;
std::map<std::tuple<const void*, int, int, int, int>, double> g_values;
struct Conv {
    const void* c;
    int kind, mode;
    bool axes, inverse;
};
std::vector<Conv> g_conv;
int g_unit_system = 2;

FakeCurve* fake_of(DYNARY* c)
{
    for (FakeCurve* f : g_curves)
        if (f->c() == c)
            return f;
    return nullptr;
}

void reset_fakes()
{
    for (FakeCurve* f : g_curves)
        delete f;
    g_curves.clear();
    g_values.clear();
    g_conv.clear();
    g_unit_system = 2;
}

double val(DYNARY* c, int i, int j, int k, int l)
{
    auto it = g_values.find({c, i, j, k, l});
    return it == g_values.end() ? 0.0 : it->second;
}

} // namespace

namespace def::model {

Cls_1401364a0::Cls_1401364a0() : head(nullptr), tail(nullptr), count(0) {}
Cls_1401364a0::~Cls_1401364a0() {}

namespace ext {

DYNARY* fun_140136670(Cls_1401364a0* list)
{
    auto* f = new FakeCurve();
    std::memset(f->raw, 0, sizeof f->raw);
    g_curves.push_back(f);
    list->count = list->count + 1;
    return f->c();
}
DYNARY* fun_140136860(Cls_1401364a0*, std::int32_t i)
{
    // 1-based; i <= 1 gives the first curve (the walk starts at the head)
    if (g_curves.empty())
        return nullptr;
    if (i < 1)
        i = 1;
    return i <= static_cast<int>(g_curves.size()) ? g_curves[i - 1]->c() : nullptr;
}
std::int32_t fun_14013c1b0(Cls_1401364a0*) { return static_cast<std::int32_t>(g_curves.size()); }
void fun_140136910(Cls_1401364a0* list)
{
    for (FakeCurve* f : g_curves)
        delete f;
    g_curves.clear();
    list->count = 0;
}
void fun_14012d2a0(DYNARY* c, std::int32_t type) { fake_of(c)->type = type; }
void fun_14012fad0(DYNARY* c) { fake_of(c)->allocated = true; }
double fun_140130720(DYNARY* c, std::int32_t i, std::int32_t j, std::int32_t k, std::int32_t l)
{
    return val(c, i, j, k, l);
}
void fun_140130800(DYNARY* c, double v, std::int32_t i, std::int32_t j, std::int32_t k,
                   std::int32_t l)
{
    g_values[{c, i, j, k, l}] = v;
}
void fun_14012e400(DYNARY* c, std::int32_t kind, std::int32_t mode, bool axes)
{
    g_conv.push_back({c, kind, mode, axes, false});
}
void fun_14012e6d0(DYNARY* c, std::int32_t kind, std::int32_t mode, bool axes)
{
    g_conv.push_back({c, kind, mode, axes, true});
}
std::int32_t fun_140235c10() { return g_unit_system; }
// Distinct, exactly representable factors: kind + mode/4.
double fun_140235d00(std::int32_t kind, std::int32_t mode) { return kind + mode * 0.25; }
// Temperature: t * 2 + mode.
double fun_140235d80(std::int32_t mode, double t) { return t * 2.0 + mode; }

} // namespace ext
} // namespace def::model

// ---- tests ------------------------------------------------------------------------------------

TEST_CASE(ctor_names_and_zeroes_flags)
{
    reset_fakes();
    Cls_1403753f0 f; // 1403753f0 -> 140375470
    CHECK(std::strcmp(f.name, "Furnace model") == 0);
    for (std::uint8_t b : f.u58)
        CHECK_EQ(static_cast<int>(b), 0);
}

TEST_CASE(setup_builds_21_typed_curves_plus_extras)
{
    reset_fakes();
    Cls_1403753f0 f;
    std::int32_t n = 23;
    f.setup_curves(&n); // 140375600
    REQUIRE(g_curves.size() == 23u);
    for (int i = 0; i < 21; ++i)
        CHECK_EQ(g_curves[i]->type, 0x8e + i);
    CHECK_EQ(g_curves[21]->type, 0xa3);
    CHECK_EQ(g_curves[22]->type, 0xa3);
    n = 0x16; // what 1401ea6b0 passes: one extra curve
    f.setup_curves(&n);
    CHECK_EQ(f.curve_count(), 22);
}

TEST_CASE(defaults_shape_and_values_si)
{
    reset_fakes();
    Cls_1403753f0 f;
    std::int32_t n = 22;
    f.setup_curves(&n);
    f.set_defaults(); // SI (fake 140235c10 == 2): no conversion
    CHECK(g_conv.empty());
    std::int32_t i = 1;
    DYNARY* c1 = f.curve(&i);
    CHECK_EQ(c1->ndim, 1);
    CHECK_EQ(c1->n[0], 2);
    CHECK_EQ(c1->ncomp, 6);
    CHECK_EQ(val(c1, 0, 0, 0, 0), 2.0);
    i = 8;
    DYNARY* c8 = f.curve(&i);
    CHECK_EQ(c8->n[0], 12);
    CHECK_EQ(c8->ncomp, 5);
    CHECK_EQ(val(c8, 5, 0, 0, 3), 1.8);
    CHECK_EQ(val(c8, 0, 0, 0, 2), 1000000000000.0);
    CHECK_EQ(val(c8, 9, 0, 0, 3), 68.0);
    i = 5;
    DYNARY* c5 = f.curve(&i);
    CHECK_EQ(c5->ncomp, 6);
    CHECK_EQ(val(c5, 1, 0, 0, 0), 0.94);      // column 6 - u40
    CHECK_EQ(val(c5, 1, 0, 0, 5), 7.322e-09); // column 11 - u40
    i = 11;
    CHECK_EQ(f.curve(&i)->var[0], 1); // curve 11: only u10[0] = 1, no arrays
    CHECK(!g_curves[10]->allocated);
    CHECK(g_curves[13]->allocated);  // curve 14
    CHECK(!g_curves[11]->allocated); // curves 12, 13: shaped, not allocated
    i = 21;
    DYNARY* c21 = f.curve(&i); // beyond the table: ndim 1, zero shape
    CHECK_EQ(c21->ndim, 1);
    CHECK_EQ(c21->n[0], 0);
    CHECK_EQ(c21->ncomp, 0);
}

TEST_CASE(defaults_english_converts_with_mode_2)
{
    reset_fakes();
    g_unit_system = 1;
    Cls_1403753f0 f;
    std::int32_t n = 22;
    f.setup_curves(&n);
    f.set_defaults();
    std::int32_t i = 4;
    DYNARY* c4 = f.curve(&i);
    CHECK_EQ(val(c4, 0, 0, 0, 3), 2400.0 * 2.0 + 2); // 1403769f0 case 4: temperature
    i = 6;
    DYNARY* c6 = f.curve(&i);
    CHECK_EQ(val(c6, 0, 0, 0, 0), 0.0 * (3 + 0.5)); // unset (0) times factor(3, 2)
    i = 8;
    DYNARY* c8 = f.curve(&i);
    CHECK_EQ(val(c8, 2, 0, 0, 2), 0.4 * (0x14 + 0.5));
    CHECK_EQ(val(c8, 4, 0, 0, 3), 1002.0 * (0x27 + 0.5) / (4 + 0.5));
    CHECK_EQ(val(c8, 6, 0, 0, 1), 0.003661 / (9 + 0.5));
    CHECK_EQ(val(c8, 5, 0, 0, 1), 528.0 * 2.0 + 2);
}

TEST_CASE(convert_mode_0_does_nothing)
{
    reset_fakes();
    Cls_1403753f0 f;
    std::int32_t n = 22;
    f.setup_curves(&n);
    f.set_defaults();
    auto before = g_values;
    f.convert_units(0);
    CHECK(g_values == before);
    CHECK(g_conv.empty());
}

TEST_CASE(convert_curve6_writes_all_to_column_3_minus_u40)
{
    reset_fakes();
    Cls_1403753f0 f;
    std::int32_t n = 22;
    f.setup_curves(&n);
    std::int32_t i = 6;
    DYNARY* c6 = f.curve(&i);
    c6->ncomp = 1;
    g_values[{c6, 1, 0, 0, 2}] = 10.0; // 3 - u40
    g_values[{c6, 1, 0, 0, 3}] = 20.0; // 4 - u40
    g_values[{c6, 1, 0, 0, 4}] = 30.0; // 5 - u40
    f.convert_units(1);
    // last write wins: temperature of column 4 - u40 + 1 = 30 -> 61
    CHECK_EQ(val(c6, 1, 0, 0, 2), 30.0 * 2.0 + 1);
    CHECK_EQ(val(c6, 1, 0, 0, 3), 20.0); // untouched
    CHECK_EQ(val(c6, 1, 0, 0, 4), 30.0);
}

TEST_CASE(convert_curve7_converts_referenced_curves)
{
    reset_fakes();
    Cls_1403753f0 f;
    std::int32_t n = 22;
    f.setup_curves(&n);
    std::int32_t i = 7;
    DYNARY* c7 = f.curve(&i);
    c7->ncomp = 10;
    for (int row = 2; row <= 3; ++row)
        for (int col = 2; col <= 5; ++col)
            g_values[{c7, row, 0, 0, col}] = 16.0 + col + 0.75; // curves 18..21, truncated
    g_values[{c7, 4, 0, 0, 2}] = 22.0;
    g_values[{c7, 2, 0, 0, 1}] = 4.0; // column 0x15 - 2*u40 = 1
    f.convert_units(2);
    CHECK_EQ(val(c7, 2, 0, 0, 1), 4.0 * (0x29 + 0.5));
    // 8 conversions from rows 2 and 3, then curve 22: kind 5 and the inverse kind 3; curve 11
    // adds one more (kind 9) after them
    REQUIRE(g_conv.size() == 11u);
    std::vector<Conv> fromc(g_conv.begin(), g_conv.begin() + 10);
    std::int32_t c11 = 11;
    CHECK(g_conv[10].c == f.curve(&c11));
    CHECK_EQ(g_conv[10].kind, 9);
    const int kinds[8] = {0x17, 0x18, 0x19, 0x2a, 0x17, 0x18, 0x19, 0x2a};
    for (int k = 0; k < 8; ++k) {
        std::int32_t want = 18 + (k % 4);
        CHECK(fromc[k].c == f.curve(&want));
        CHECK_EQ(fromc[k].kind, kinds[k]);
        CHECK_EQ(fromc[k].mode, 2);
        CHECK(fromc[k].axes);
        CHECK(!fromc[k].inverse);
    }
    std::int32_t c22 = 22;
    CHECK(fromc[8].c == f.curve(&c22));
    CHECK_EQ(fromc[8].kind, 5);
    CHECK(fromc[8].axes);
    CHECK(fromc[9].c == f.curve(&c22));
    CHECK_EQ(fromc[9].kind, 3);
    CHECK(!fromc[9].axes);
    CHECK(fromc[9].inverse);
}

// The shape table must be the one in DEF_PRE_64.exe (read-only; skipped when it is absent).
TEST_CASE(shape_table_matches_binary)
{
    const char* exe = std::getenv("DEFORM_PRE_EXE");
    std::string path = exe ? exe : R"(C:\Program Files\SFTC\DEFORM\v11.0.2\DEF_PRE_64.exe)";
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        std::printf("skipped: %s not found\n", path.c_str());
        return;
    }
    std::vector<unsigned char> d((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    auto u16 = [&](std::size_t o) { return static_cast<std::uint32_t>(d[o] | d[o + 1] << 8); };
    auto u32 = [&](std::size_t o) { return u16(o) | (u16(o + 2) << 16); };
    std::uint32_t pe = u32(0x3c);
    std::uint32_t nsec = u16(pe + 6), opt = u16(pe + 20);
    std::uint64_t base = u32(pe + 24 + 24) | (static_cast<std::uint64_t>(u32(pe + 24 + 28)) << 32);
    std::uint64_t rva = 0x140550ec0ull - base;
    std::size_t off = 0;
    for (std::uint32_t s = 0; s < nsec; ++s) {
        std::size_t h = pe + 24 + opt + 40 * s;
        std::uint32_t vs = u32(h + 8), va = u32(h + 12), rs = u32(h + 16), rp = u32(h + 20);
        if (va <= rva && rva < va + (vs > rs ? vs : rs))
            off = rp + static_cast<std::size_t>(rva - va);
    }
    REQUIRE(off != 0);
    for (int r = 0; r < 20; ++r)
        for (int k = 0; k < 5; ++k)
            CHECK_EQ(static_cast<std::int32_t>(u32(off + 4 * (r * 5 + k))), kFurnaceCurveShape[r][k]);
}

int main()
{
    int rc = testkit::run_all();
    reset_fakes();
    return rc;
}
