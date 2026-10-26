// ctest-labels: key-common dynary
// Tier-1 tests of the DYNARY template family (14012d2a0, 14012fc70, 14012fad0, 14012ff20,
// 14012fc00, 14012f610, 14012f480). Expected values are read off the decompile of 14012d2a0's
// switch and the helpers (docs/re/keywords/_common.md section 3), not from running the port.
#include <defmodel/dynary_init.hpp>
#include <testkit/check.hpp>

#include <cstdlib>
#include <cstring>

using namespace def::model;

namespace {

// The ctor (p1-06) is not ported yet; it zeroes the object before calling 14012d2a0, so tests
// start from zeroed storage.
struct Zeroed {
    alignas(DYNARY) unsigned char buf[sizeof(DYNARY)];
    Zeroed() { std::memset(buf, 0, sizeof buf); }
    DYNARY* get() { return reinterpret_cast<DYNARY*>(buf); }
};

void release(DYNARY* t) { dynary_free_arrays(t); }

} // namespace

TEST_CASE(default_type_is_scalar_constant)
{
    Zeroed z;
    DYNARY* t = z.get();
    for (int type : {0, 1, 2, 0x15, 0x16, 0x17, 0x1d, 0x1e, 0x4e}) {
        dynary_init(t, type);
        CHECK_EQ(t->type, type);
        CHECK_EQ(t->ndim, 0);
        CHECK_EQ(t->ncomp, 1);
        CHECK_EQ(dynary_value_count(t), 0);
        CHECK(t->values == nullptr);
        CHECK(t->axis[0] == nullptr);
    }
}

TEST_CASE(type_80_and_b5_clamp_ncomp)
{
    Zeroed z;
    DYNARY* t = z.get();
    dynary_init(t, 0x80); // ncomp 0, clamped to 1 by 14012fc00 inside the allocation
    CHECK_EQ(t->ndim, 0);
    CHECK_EQ(t->ncomp, 1);
    CHECK_EQ(dynary_value_count(t), 0);
    CHECK(t->values == nullptr);

    dynary_init(t, 0xb5); // ndim 0 but type 0xb5 counts ncomp values even when ncomp < 2
    CHECK_EQ(t->ncomp, 1);
    CHECK_EQ(dynary_value_count(t), 1);
    REQUIRE(t->values != nullptr);
    CHECK_BITS(t->values[0], 0.0);
    release(t);
}

TEST_CASE(table_templates)
{
    Zeroed z;
    DYNARY* t = z.get();
    dynary_init(t, 0x27); // ndim 1, var temperature (3), ncomp 2, no points yet
    CHECK_EQ(t->ndim, 1);
    CHECK_EQ(t->var[0], 3);
    CHECK_EQ(t->ncomp, 2);
    CHECK_EQ(t->n[0], 0);
    CHECK_EQ(dynary_value_count(t), 0);
    CHECK(t->axis[0] == nullptr);

    dynary_init(t, 0x79); // ndim 1, n 1, ncomp 2, var 1
    CHECK_EQ(t->n[0], 1);
    CHECK_EQ(dynary_value_count(t), 2);
    CHECK(t->axis[0] != nullptr);
    CHECK(t->values != nullptr);
    release(t);

    dynary_init(t, 0x2c); // ndim 3: var 0xc, 6, 3; ncomp 2
    CHECK_EQ(t->ndim, 3);
    CHECK_EQ(t->var[0], 0xc);
    CHECK_EQ(t->var[1], 6);
    CHECK_EQ(t->var[2], 3);
    CHECK_EQ(t->ncomp, 2);

    dynary_init(t, 0xae); // constant 1.0 (_DAT_140509c90)
    CHECK_BITS(t->value, 1.0);
    dynary_init(t, 0xaf);
    CHECK_BITS(t->value, 0.0);
    CHECK_EQ(t->ncomp, 2);
    CHECK_EQ(dynary_value_count(t), 2);
    release(t);
}

TEST_CASE(unallocated_types)
{
    Zeroed z;
    DYNARY* t = z.get();
    t->ncomp = 0;
    dynary_init(t, 0x65); // ncomp 6, no allocation, so no clamp either
    CHECK_EQ(t->ncomp, 6);
    CHECK(t->values == nullptr);
    dynary_init(t, 0x51);
    CHECK_EQ(t->ndim, 3);
    CHECK(t->values == nullptr);
}

TEST_CASE(reinit_keeps_links)
{
    Zeroed z;
    DYNARY* t = z.get();
    dynary_init(t, 0);
    t->u50 = 5;
    t->u54 = 7;
    t->u58 = 9;
    t->u60 = 11;
    t->u64 = 13;
    dynary_reinit_keep(t, 0x70);
    CHECK_EQ(t->u50, 0);
    CHECK_EQ(t->u54, 7);
    CHECK_EQ(t->u58, 0);
    CHECK_EQ(t->u60, 11);
    CHECK_EQ(t->u64, 13);
    CHECK_EQ(t->ndim, 1);
    dynary_init(t, 0x70);
    CHECK_EQ(t->u54, 0);
    CHECK_EQ(t->u60, 0);
}

TEST_CASE(value_count_wraps_like_int32)
{
    Zeroed z;
    DYNARY* t = z.get();
    t->ndim = 2;
    t->ncomp = 1;
    t->n[0] = 65536;
    t->n[1] = 65536; // 2^32 wraps to 0
    CHECK_EQ(dynary_value_count(t), 0);
    t->n[0] = 46341;
    t->n[1] = 46341; // 2147488281 wraps negative -> 0
    CHECK_EQ(dynary_value_count(t), 0);
    t->n[0] = 3;
    t->n[1] = 4;
    t->ncomp = 2;
    CHECK_EQ(dynary_value_count(t), 24);
    t->n[1] = -1;
    CHECK_EQ(dynary_value_count(t), 0);
}

TEST_CASE(counts_positive_and_clamp)
{
    Zeroed z;
    DYNARY* t = z.get();
    CHECK(dynary_counts_positive(t)); // ndim 0
    t->ndim = 2;
    t->n[0] = 2;
    t->n[1] = 0;
    CHECK(!dynary_counts_positive(t));
    t->n[1] = 1;
    CHECK(dynary_counts_positive(t));
    t->n[2] = -4;
    t->ncomp = -2;
    dynary_clamp_shape(t);
    CHECK_EQ(t->n[2], 0);
    CHECK_EQ(t->ncomp, 1);
}

TEST_CASE(free_keeps_shared_arrays)
{
    Zeroed z;
    DYNARY* t = z.get();
    double* borrowed = static_cast<double*>(std::calloc(4, 8));
    double* vals = static_cast<double*>(std::calloc(4, 8));
    t->ndim = 1;
    t->n[0] = 0; // count 0 but pointer set: the arrays belong to someone else
    t->axis[0] = borrowed;
    t->values = vals;
    dynary_free_arrays(t);
    CHECK(t->axis[0] == nullptr);
    CHECK(t->values == nullptr);
    std::free(borrowed); // still valid: not freed by the table
    std::free(vals);
}

namespace {
// Stand-ins for p1-12's unit functions: factor(kind, mode) = kind + mode/10, temperature adds 100*mode.
double fake_factor(int kind, int mode) { return kind + mode / 10.0; }
double fake_temp(int mode, double t) { return t + 100.0 * mode; }
const UnitFunctions kFake{fake_factor, fake_temp};
} // namespace

TEST_CASE(unit_conversion)
{
    Zeroed z;
    DYNARY* t = z.get();
    dynary_init(t, 0x29); // ndim 2, var (3 -> kind 9, 0xc -> kind 16), ncomp 1
    t->n[0] = 1;
    t->n[1] = 2;
    dynary_alloc(t);
    t->axis[0][0] = 20.0;
    t->axis[1][0] = 1.0;
    t->axis[1][1] = 2.0;
    t->values[0] = 3.0;
    t->value = 5.0;
    dynary_convert_units(t, 2, 1, true, kFake); // factor(2, 1) = 2.1
    CHECK_BITS(t->axis[0][0], 120.0);           // temperature axis
    CHECK_BITS(t->axis[1][1], 2.0 * fake_factor(16, 1));      // kVarUnitKind[0xc] = 16
    CHECK_BITS(t->values[0], 3.0 * fake_factor(2, 1));
    CHECK_BITS(t->value, 5.0 * fake_factor(2, 1));
    CHECK_BITS(t->values[1], 0.0);

    dynary_convert_units_inverse(t, 9, 2, false, kFake); // 1 / temperature
    CHECK_BITS(t->value, 1.0 / (5.0 * fake_factor(2, 1) + 200.0));
    CHECK_BITS(t->axis[0][0], 120.0); // axes untouched
    release(t);

    dynary_init(t, 0x2a); // default shape (ndim 0, ncomp 1); type 0x2a inverts the factor
    t->value = 4.0;
    dynary_convert_units(t, 3, 1, false, kFake);
    CHECK_BITS(t->value, 4.0 * (1.0 / fake_factor(3, 1)));
    dynary_convert_units_inverse(t, 3, 1, false, kFake); // 1/f for every type
    CHECK_BITS(t->value, 4.0 * (1.0 / fake_factor(3, 1)) * (1.0 / fake_factor(3, 1)));
}

TEST_CASE(unit_conversion_guards)
{
    Zeroed z;
    DYNARY* t = z.get();
    dynary_init(t, 0);
    t->value = 4.0;
    dynary_convert_units(t, 3, 0, false, kFake); // mode 0
    CHECK_BITS(t->value, 4.0);
    t->ncomp = 2; // vectors are not converted
    dynary_convert_units(t, 3, 1, false, kFake);
    CHECK_BITS(t->value, 4.0);
    t->ncomp = 1;
    t->ndim = 1; // a dimension without points
    dynary_convert_units(t, 3, 1, false, kFake);
    CHECK_BITS(t->value, 4.0);
}

int main() { return testkit::run_all(); }
