// ctest-labels: kw-g2mg group2
// Tier-1 tests of packet kw-g2mg on what the corpus does not contain (no DYNARY, no OPSTOP value
// entries, few MGUSER shapes) and on the read/write asymmetries the specs list
// (docs/re/keywords/DYNARY.md, OPSTOP.md, MGNELM.md, MGSIZM.md, MGUSER.md). Expected texts follow
// the write grammars of those specs.
#define _CRT_SECURE_NO_WARNINGS
#include "key_test_util.hpp"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <new>
#include <string>
#include <vector>

#include <defkey/key_reader.hpp>
#include <defkey/kw_g2mg.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dyn_ary_linked_list.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/op_stop_linked_list.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <testkit/check.hpp>

using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;
namespace g2mg = def::key::g2mg;

namespace {

std::string E(double v)
{
    char b[64];
    def::rt::crt::sprintf(b, " %20.10E", v);
    return b;
}
std::string I(int v)
{
    char b[32];
    std::snprintf(b, sizeof b, " %7i", v);
    return b;
}

template <class T>
T& at(void* p, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<unsigned char*>(p) + off);
}

struct Hooks : def::key::KeyReaderHooks {
    DB_STEP_DATA& m;
    explicit Hooks(DB_STEP_DATA& model) : m(model) {}
    bool has_sub_model() override { return false; }
    void set_model_dimension(int dim) override { m.dimension = dim; }
    void set_sub_model_dimension(int) override {}
    int parse_keyword(int group, std::FILE* f, int id) override
    {
        return group == 2 ? def::key::g2obj::read_object_keyword(m, f, id) : 3;
    }
};

// A model from the real ctor (dimension as given); objects are made by the group 2 parser.
struct Fixture {
    unsigned char* mem;
    std::string log;
    explicit Fixture(int dim = 3) : mem(static_cast<unsigned char*>(::operator new(sizeof(DB_STEP_DATA))))
    {
        new (mem) DB_STEP_DATA(3);  // the real model ctor (p1-03)
        m().dimension = dim;
        read("MGSIZM 1 0\n");  // object 1, built by the real LLOBJ_DATA ctor
    }
    DB_STEP_DATA& m() { return *reinterpret_cast<DB_STEP_DATA*>(mem); }
    int read(const std::string& key)
    {
        keytest::InFile in("kw_g2mg_in.KEY", key);
        std::FILE* lf = std::fopen(keytest::tmp_name("kw_g2mg_unit.log").c_str(), "w+");
        Hooks h(m());
        const int rc = def::key::read_key_stream(in.get(), h, lf);
        std::rewind(lf);
        log.clear();
        int c;
        while ((c = std::fgetc(lf)) != EOF) log += static_cast<char>(c);
        std::fclose(lf);
        std::remove(keytest::tmp_name("kw_g2mg_unit.log").c_str());
        return rc;
    }
    LLOBJ_DATA& obj(int n) { return *def::key::g2obj::find_object(m(), n); }
    template <class W>
    std::string write(W w, int n, int* rc = nullptr)
    {
        keytest::OutFile out("kw_g2mg_out.KEY");
        // N as the object writer passes it (the mesh block prints N as given; OPSTOP and DYNARY
        // would also take 0 for "use obj+8")
        const int r = w(m(), out.get(), obj(n), static_cast<std::uint32_t>(n));
        if (rc != nullptr) *rc = r;
        return out.text();
    }
};

} // namespace

TEST_CASE(opstop_entries_clear_and_gaps)
{
    keytest::legacy_printf();
    Fixture fx;
    CHECK_EQ(fx.read("OPSTOP 1 0 0\nOPSTOP 1 2 1\n2.5\nOPSTOP 1 4 7\n"), 0);
    CHECK_EQ(at<std::int32_t>(&fx.obj(1), 0x20b8 + 0x18), 4); // gap entries 1 and 3 created
    // only type-1 entries are written, at their positions
    CHECK_EQ(fx.write(g2mg::write_opstop, 1),
             "OPSTOP       1       0       0\nOPSTOP       1       2       1\n" + E(2.5) + "\n");
    CHECK_EQ(fx.read("OPSTOP 1 0 5\n"), 0);
    CHECK_EQ(at<std::int32_t>(&fx.obj(1), 0x20b8 + 0x18), 0);
    CHECK_EQ(fx.write(g2mg::write_opstop, 1), std::string("OPSTOP       1       0       0\n"));
    // type 1 with the value line missing: Reading Error
    CHECK_EQ(fx.read("OPSTOP 1 1 1\n"), 2);
}

TEST_CASE(dynary_double_and_int_tables)
{
    keytest::legacy_printf();
    Fixture fx;
    CHECK_EQ(fx.read("DYNARY 1 1 1 1 2 2\n1 1.5 2.5\n2 3.5 4.5\n"
                     "DYNARY 1 2 5 2 1 8\n1 1 2 3 4 5 6 7 8\n"
                     "DYNARY 1 3 1 2 1 4\n9 4 3 2 1\n"),
             0);
    const std::string want =
        "DYNARY       1       1       1       1       2       2\n" + I(1) + E(1.5) + E(2.5) + "\n" +
        I(2) + E(3.5) + E(4.5) + "\n" +
        "DYNARY       1       2       1       2       1       8\n" // kind 5 is read as 1
        "       1       1       2       3       4       5       6       7       8\n"
        "DYNARY       1       3       1       2       1       4\n"
        "       1       4       3       2       1\n";
    CHECK_EQ(fx.write(g2mg::write_dynary, 1), want);
}

TEST_CASE(dynary_sparse_records_and_uniform_arrays)
{
    keytest::legacy_printf();
    Fixture fx;
    // count 3, ncomp 2, one record, default 9.5; record 7 (out of range) is read and dropped
    CHECK_EQ(fx.read("DYNARY 1 1 1 3 3 2 2 9.5\n2 1.0 2.0\n7 5 5\nDYNARY 1 2 1 3 2 2 0 4.0\n"), 0);
    const std::string want =
        "DYNARY       1       1       1       3       3       2       3" + E(0) + "\n" +
        I(1) + E(9.5) + E(9.5) + "\n" + I(2) + E(1) + E(2) + "\n" + I(3) + E(9.5) + E(9.5) + "\n" +
        "DYNARY       1       2       1       3       2       2       0" + E(4) + "\n";
    CHECK_EQ(fx.write(g2mg::write_dynary, 1), want);
}

TEST_CASE(dynary_writer_stops_at_an_unwritable_record)
{
    keytest::legacy_printf();
    Fixture fx;
    // record 2 is a gap (class 0); records 3 is never reached
    CHECK_EQ(fx.read("DYNARY 1 1 1 1 1 2\n1 1 2\nDYNARY 1 3 1 1 1 2\n1 3 4\n"), 0);
    int rc = 0;
    const std::string out = fx.write(g2mg::write_dynary, 1, &rc);
    CHECK_EQ(rc, 3);
    CHECK_EQ(out, "DYNARY       1       1       1       1       1       2\n" + I(1) + E(1) + E(2) + "\n");
    // a double table with ncomp 4 has no row format: header, then 3
    Fixture fx2;
    CHECK_EQ(fx2.read("DYNARY 1 1 1 1 1 4\n"), 0); // no body lines are read for ncomp 4
    CHECK_EQ(fx2.write(g2mg::write_dynary, 1, &rc),
             std::string("DYNARY       1       1       1       1       1       4\n"));
    CHECK_EQ(rc, 3);
}

TEST_CASE(mgnelm_2d_tokens_and_3d_flag_clamp)
{
    Fixture fx(2);
    CHECK_EQ(fx.read("MGNELM 1 1625\n"), 0);
    CHECK_EQ(at<std::int32_t>(&fx.obj(1), 0x2d80), 1625);
    CHECK_EQ(fx.read("MGNELM 1 5 6\n"), 0); // two tokens: token 2 is missing, flag 0
    CHECK_EQ(at<std::int32_t>(&fx.obj(1), 0x2d8c), 6);
    CHECK_EQ(at<std::int32_t>(&fx.obj(1), 0x2d94), 0);
    Fixture fx3;
    CHECK_EQ(fx3.read("MGNELM 1 1 2 3 7\n"), 0);
    CHECK_EQ(at<std::int32_t>(&fx3.obj(1), 0x2d90), 3);
    CHECK_EQ(at<std::int32_t>(&fx3.obj(1), 0x2d94), 0); // outside 0..1
}

TEST_CASE(mgsizm_in_2d_leaves_the_value_for_the_key_loop)
{
    Fixture fx(2);
    fx.read("MGSIZM 1 2.5\n");
    CHECK(fx.log.find("WARNING: Keyword 2.5 not found") != std::string::npos);
    Fixture fx3;
    CHECK_EQ(fx3.read("MGSIZM 1 2.5\n"), 0);
    CHECK_EQ(at<double>(&fx3.obj(1), 0x2da8), 2.5);
}

TEST_CASE(mguser_3d_shapes_renumbering_and_errors)
{
    keytest::legacy_printf();
    Fixture fx;
    const std::string pts = "4\n1 0 0 0\n2 1 0 0\n3 1 1 0\n4 0 1 0\n";
    // window 1: size 0 (skipped), window 2: shape 12 -> written as 11 with the flag, opt -3
    CHECK_EQ(fx.read("MGUSER 1 1 0 0 0 0 11 1 0\n" + pts + "MGUSER 1 2 2.5 0 0 0 12 -3 1\n" + pts), 0);
    const std::string out = fx.write(g2mg::write_mesh_block, 1);
    const std::string head = "MGUSER       1       1" + E(2.5) + E(0) + E(0) + E(0) + I(11) + I(-3) + I(1) + "\n";
    if (out.find(head) == std::string::npos) std::printf("%s\n---\n%s", head.c_str(), out.c_str());
    CHECK(out.find(head) != std::string::npos);
    CHECK(out.find("MGUSER       1       2") == std::string::npos); // renumbered 1..n
    // an invalid shape returns 2 and leaves the body lines for the key loop
    Fixture fx2;
    CHECK_EQ(fx2.read("MGUSER 1 1 1 0 0 0 5 1\n" + pts), 2);
}

TEST_CASE(mguser_2d_keeps_window_numbers)
{
    keytest::legacy_printf();
    Fixture fx(2);
    CHECK_EQ(fx.read("MGUSER 1 2 2 6.5 0 0 1\n1 1.5 2.5\n2 3.5 4.5\n"), 0);
    const std::string out = fx.write(g2mg::write_mesh_block, 1);
    // window 1 (created as a gap, size 0) is skipped; window 2 keeps its number
    const std::string want = "MGUSER       1       2       2" + E(6.5) + E(0) + E(0) + I(1) + "\n" +
                             I(1) + E(1.5) + E(2.5) + "\n" + I(2) + E(3.5) + E(4.5) + "\n";
    CHECK(out.size() >= want.size() && out.compare(out.size() - want.size(), want.size(), want) == 0);
}

TEST_CASE(mesh_block_order)
{
    keytest::legacy_printf();
    for (int dim : {2, 3}) {
        Fixture fx(dim);
        CHECK_EQ(fx.read("MGTELM 1 4.9\nMGGRID 1 25.7 -3.2\n"), 0);
        const std::string out = fx.write(g2mg::write_mesh_block, 1);
        std::vector<std::string> names;
        std::size_t a = 0;
        while (a < out.size()) {
            const std::size_t b = out.find('\n', a);
            names.push_back(out.substr(a, 6));
            a = b + 1;
        }
        const std::vector<std::string> want = {"RMDPTH", "RMSTRK", "RMTIME", "RMSTEP", "MGSIZR",
                                               "MGWTMP", "MGWSTN", "MGWSTR", "MGNELM", "MGTELM",
                                               "MGWCUV", "MGGRID", "MGERR ", "MGWUSR"};
        CHECK(names == want);
        if (out.find("MGGRID       1      25      -3\n") == std::string::npos) std::printf("%s", out.c_str());
        CHECK(out.find("MGTELM       1       4\n") != std::string::npos);
        CHECK(out.find("MGGRID       1      25      -3\n") != std::string::npos);
    }
}

int main() { return testkit::run_all(); }
