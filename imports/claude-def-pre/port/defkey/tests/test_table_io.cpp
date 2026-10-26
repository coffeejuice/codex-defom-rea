// ctest-labels: key-common
// Tier-1 tests of the DYNARY KEY reader/writers (14041a800, 14041b270, 14041ace0, 14041b060,
// 1402f2e80, 1402f2fe0, 1402f3130, 1402f34f0, 1402f3840, 1402f3aa0). Expected text is worked out
// by hand from the decompiled format strings: " %20.10E" of 1.0 is " " + 3 pad + "1.0000000000E+000"
// with the old CRT's three-digit exponent.
#include "key_test_util.hpp"

#include <defkey/table_io.hpp>
#include <defmodel/dynary_init.hpp>
#include <testkit/check.hpp>

#include <cstring>
#include <string>

using namespace def::key;
using namespace def::model;
using keytest::InFile;
using keytest::OutFile;

namespace {

// The ctor (p1-06) zeroes the object and calls 14012d2a0(this, 0); same here on raw storage.
struct Table {
    alignas(DYNARY) unsigned char buf[sizeof(DYNARY)];
    explicit Table(int type)
    {
        std::memset(buf, 0, sizeof buf);
        dynary_init(get(), type);
    }
    ~Table() { dynary_free_arrays(get()); }
    DYNARY* get() { return reinterpret_cast<DYNARY*>(buf); }
    DYNARY* operator->() { return get(); }
};

const std::string E1 = "    1.0000000000E+000"; // " %20.10E" of 1.0

} // namespace

TEST_CASE(write_doubles_layout)
{
    keytest::legacy_printf();
    OutFile out("t_tb_wd.txt");
    const double v[7] = {1.0, -2.5, 0.0, 1.52587890625e-05, 1e100, 6.82e1, 1e-300};
    CHECK_EQ(write_doubles(out.get(), 7, v, 3), 0);
    CHECK_EQ(write_doubles(out.get(), 0, v, 3), 0); // nothing, not even "\n"
    CHECK_EQ(write_doubles(out.get(), 1, v, 0), 2);
    CHECK_EQ(write_doubles(nullptr, 1, v, 1), 2);
    CHECK_EQ(write_doubles(out.get(), 2, v, 5, true), 0);
    const std::string want = E1 + "   -2.5000000000E+000    0.0000000000E+000\n"
                                  "            1.5258789063E-005    1.0000000000E+100    6.8200000000E+001\n"
                                  "            1.0000000000E-300\n" +
                             E1 + ",   -2.5000000000E+000,\n";
    CHECK_EQ(out.text(), want);
}

TEST_CASE(write_ints_layout)
{
    OutFile out("t_tb_wi.txt");
    const std::int32_t v[3] = {1, -20, 300};
    CHECK_EQ(write_ints(out.get(), 3, v, 2), 0);
    CHECK_EQ(write_ints(out.get(), 2, v, 5, true), 0);
    CHECK_EQ(out.text(), std::string("       1     -20\n             300\n       1,     -20,\n"));
}

TEST_CASE(read_body_1d)
{
    Table t(0x70); // ndim 1, var 3, ncomp 1
    t->n[0] = 3;
    InFile in("t_tb_r1.txt", "1 10\n2\n20 3 30 tail\nNEXT\n");
    CHECK_EQ(read_table_body(in.get(), t.get()), 0);
    CHECK_BITS(t->axis[0][2], 3.0);
    CHECK_BITS(t->values[1], 20.0);
    CHECK_BITS(t->values[2], 30.0);
    CHECK_EQ(in.rest(), std::string("NEXT\n")); // fgets swallowed " tail\n"
}

TEST_CASE(read_body_short_is_error)
{
    Table t(0x70);
    t->n[0] = 2;
    InFile in("t_tb_r2.txt", "1 10\n2\n");
    CHECK_EQ(read_table_body(in.get(), t.get()), 1);
}

TEST_CASE(read_body_comma_stalls)
{
    // fscanf("%lf") returns 0 (not -1) at ',': nothing more converts, no error, values stay 0.
    Table t(0x70);
    t->n[0] = 2;
    InFile in("t_tb_r3.txt", "1, 10\n2 20\nX\n");
    CHECK_EQ(read_table_body(in.get(), t.get()), 0);
    CHECK_BITS(t->axis[0][0], 1.0);
    CHECK_BITS(t->values[0], 0.0);
    CHECK_BITS(t->axis[0][1], 0.0);
    CHECK_EQ(in.rest(), std::string("2 20\nX\n")); // the final fgets took ", 10\n"
}

TEST_CASE(read_body_constant_reads_nothing)
{
    Table t(0); // ndim 0, ncomp 1
    InFile in("t_tb_r4.txt", "5\n");
    CHECK_EQ(read_table_body(in.get(), t.get()), 0);
    CHECK_EQ(in.rest(), std::string("5\n"));
}

TEST_CASE(read_body_2d_order)
{
    Table t(0x29); // ndim 2, var (3, 0xc), ncomp 1
    t->n[0] = 2;
    t->n[1] = 3;
    InFile in("t_tb_r5.txt", "0 1\n10 20 30\n1 2 3\n4 5 6\nafter\n");
    CHECK_EQ(read_table_body(in.get(), t.get()), 0);
    CHECK_BITS(t->axis[0][1], 1.0);
    CHECK_BITS(t->axis[1][2], 30.0);
    CHECK_EQ(dynary_value_count(t.get()), 6);
    CHECK_BITS(t->values[5], 6.0);
    CHECK_EQ(in.rest(), std::string("after\n"));
}

TEST_CASE(write_body_modes)
{
    keytest::legacy_printf();
    Table t(0x70);
    t->n[0] = 2;
    dynary_alloc(t.get());
    t->axis[0][0] = 1.0;
    t->axis[0][1] = 2.0;
    t->values[0] = 10.0;
    t->values[1] = 20.0;
    OutFile out("t_tb_w1.txt");
    CHECK_EQ(write_table_body(out.get(), t.get(), 0), 0);
    CHECK_EQ(write_table_body(out.get(), nullptr, 0), 0);
    CHECK_EQ(write_table_body(nullptr, t.get(), 0), 2);
    CHECK_EQ(out.text(), E1 + "    1.0000000000E+001\n    2.0000000000E+000    2.0000000000E+001\n");

    Table e(0x70); // no points: mode 0 writes nothing, mode 1 writes the count
    OutFile out2("t_tb_w2.txt");
    CHECK_EQ(write_table_body(out2.get(), e.get(), 0), 0);
    CHECK_EQ(write_table_body(out2.get(), e.get(), 1), 0);
    Table c(0xae); // constant 1.0: mode 0 nothing, mode 1 and 6 the value
    CHECK_EQ(write_table_body(out2.get(), c.get(), 0), 0);
    CHECK_EQ(write_table_body(out2.get(), c.get(), 6), 0);
    CHECK_EQ(out2.text(), std::string("       0\n") + E1 + "\n");
}

TEST_CASE(write_body_2d_per_line_is_max_count)
{
    keytest::legacy_printf();
    Table t(0x29);
    t->n[0] = 2;
    t->n[1] = 3;
    dynary_alloc(t.get());
    for (int i = 0; i < 6; ++i) t->values[i] = 1.0;
    t->axis[0][0] = t->axis[0][1] = 1.0;
    t->axis[1][0] = t->axis[1][1] = t->axis[1][2] = 1.0;
    OutFile out("t_tb_w3.txt");
    CHECK_EQ(write_table_body(out.get(), t.get(), 0), 0);
    CHECK_EQ(out.text(), E1 + E1 + "\n" + E1 + E1 + E1 + "\n" + E1 + E1 + E1 + "\n        " + E1 + E1 + E1 + "\n");
}

TEST_CASE(header_round_trip)
{
    keytest::legacy_printf();
    Table t(0);
    double extra = -1.0;
    InFile in("t_tb_h1.txt", "1 2 0.5\n100 1\n200 2\nNEXT\n");
    CHECK_EQ(read_table_header(in.get(), t.get(), 0, &extra), 0);
    CHECK_EQ(t->ndim, 1);
    CHECK_EQ(t->var[0], 3);
    CHECK_EQ(t->n[0], 2);
    CHECK_BITS(extra, 0.5); // type < 4 and 3 fields: extra = field 3
    CHECK_BITS(t->values[1], 2.0);
    CHECK_EQ(in.rest(), std::string("NEXT\n"));
    OutFile out("t_tb_h2.txt");
    CHECK_EQ(write_table_header_d(out.get(), "YOUNG", 1, t.get(), 0, nullptr), 0);
    CHECK_EQ(write_table_header_d(out.get(), "YOUNG", 1, t.get(), 1, &extra), 0);
    const std::string body = "    1.0000000000E+002" + E1 + "\n    2.0000000000E+002    2.0000000000E+000\n";
    CHECK_EQ(out.text(), "YOUNG        1       1       2\n" + body +
                             "YOUNG        1       1       2    5.0000000000E-001\n" + body);
}

TEST_CASE(header_constant_and_int_extra)
{
    keytest::legacy_printf();
    Table t(0);
    InFile in("t_tb_h3.txt", "0 2.5\n");
    double extra = 7.0;
    CHECK_EQ(read_table_header(in.get(), t.get(), 0, &extra), 0);
    CHECK_BITS(extra, 7.0); // only 2 fields: extra untouched
    CHECK_BITS(t->value, 2.5);
    OutFile out("t_tb_h4.txt");
    const std::int32_t ie = 4;
    CHECK_EQ(write_table_header_i(out.get(), "HEATCP", 2, t.get(), 1, &ie), 0);
    CHECK_EQ(out.text(), std::string("HEATCP       2       0    2.5000000000E+000       4\n"));
}

TEST_CASE(region_table_round_trip)
{
    keytest::legacy_printf();
    Table t(0);
    InFile in("t_tb_g1.txt", " 1 1 3 2 1\n 100 5\n 200 6\n 2 0 1 7.5 0\nEND\n");
    CHECK_EQ(read_region_table(in.get(), t.get()), 0);
    CHECK_EQ(t->ndim, 1);
    CHECK_EQ(t->var[0], 3);
    CHECK_EQ(t->n[0], 2);
    CHECK_EQ(t->ncomp, 1);
    CHECK_BITS(t->values[1], 6.0);
    Table c(0);
    CHECK_EQ(read_region_table(in.get(), c.get()), 0);
    CHECK_BITS(c->value, 7.5);
    CHECK_EQ(in.rest(), std::string("END\n"));
    OutFile out("t_tb_g2.txt");
    CHECK_EQ(write_region_table(out.get(), t.get(), 1, 1), 0);
    CHECK_EQ(write_region_table(out.get(), c.get(), 1, 2), 0);
    CHECK_EQ(write_region_table(out.get(), c.get(), 0, 2), 0); // mode 0 writes nothing
    CHECK_EQ(out.text(), std::string("       1       1       3       2       1\n") +
                             "    1.0000000000E+002    5.0000000000E+000\n"
                             "    2.0000000000E+002    6.0000000000E+000\n"
                             "       2       0       1    7.5000000000E+000\n");
}

TEST_CASE(region_string_table_round_trip)
{
    Table t(0);
    InFile in("t_tb_s1.txt", " 1 1 0 2 8\nfirst\nsecond name  \n");
    CHECK_EQ(read_region_string_table(in.get(), t.get()), 0);
    const char* s0 = reinterpret_cast<const char*>(t->values);
    CHECK_EQ(std::string(s0, 6), std::string("first "));  // blank-padded to 64
    CHECK_EQ(s0[63], ' ');
    OutFile out("t_tb_s2.txt");
    CHECK_EQ(write_region_string_table(out.get(), t.get(), 1, 3), 0);
    CHECK_EQ(out.text(), std::string("       3       1       0       2       8\nfirst\nsecond name\n"));
}

int main() { return testkit::run_all(); }
