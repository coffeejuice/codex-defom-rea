// ctest-labels: kw-g3
// Tier-1 tests of the shape-3 branches of the window body reader FUN_14041b7d0 and writer
// FUN_1402f4890 (packet kw-g3): the window's _OBJ_GEO3D at LLWINDEF_DATA+0x220 is filled from
// "<nn>", nn x "<i> <x> <y> <z>", "<ne>", ne x "<i> <n1> <n2> <n3> <n4>" and written back as
// "%d\n", " %7i %20.10E %20.10E %20.10E\n", "%d\n", " %7i %7i %7i %7i %7i\n" (strings 1405423ec,
// 140542948, 140542968, 140542970). Expected values are worked out from the disassembly of the
// two branches (docs/re/keywords/MGUSER.md, "shape 3"). FRCWIN and the WIN* BCs reject shape 3
// before the body; the only KEY path to it is MGUSER in 3D (kw-g2mg reader case 0x32 and the
// mesh-settings block writer), which the last test drives end to end. The corpus has no shape-3
// window (3D MGUSER shapes: 1, 2, 11 only), so there is no oracle comparison here.
#define _CRT_SECURE_NO_WARNINGS
#include "kw_g3_test_util.hpp"

#include <cstdint>
#include <cstdio>
#include <new>
#include <string>

#include <defkey/key_reader.hpp>
#include <defkey/kw_g2mg.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defmodel/layout/cls_140269250.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/window/llwindef_data.hpp>
#include <defmodel/window/window_list.hpp>
#include <testkit/check.hpp>

using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;
using def::model::LLWINDEF_DATA;
using namespace def::key::g3;

namespace {

// The canonical text of a window with 4 nodes and 2 elements (the second a triangle that repeats
// its last node), as 1402f4890 writes it.
const char* const kBody =
    "4\n"
    "       1    0.0000000000E+000    0.0000000000E+000    0.0000000000E+000\n"
    "       2    1.5000000000E+000    0.0000000000E+000    0.0000000000E+000\n"
    "       3    1.5000000000E+000    2.2500000000E+000   -1.0000000000E-003\n"
    "       4    0.0000000000E+000    2.2500000000E+000    1.2345678901E+002\n"
    "2\n"
    "       1       1       2       3       4\n"
    "       2       3       4       1       1\n";

struct Window {
    g3test::FakeModel model{3};
    LLWINDEF_DATA w{model.get()};
    Window() { w.shape = 3; }
    int read(const std::string& text, bool indexed, const char* tag)
    {
        keytest::InFile in(tag, text);
        return window_read_points_3d(in.get(), &w, indexed);
    }
    std::string write(std::int32_t mode, const char* tag, int* st = nullptr)
    {
        keytest::OutFile out(tag);
        const int r = window_write_points(out.get(), &w, mode);
        if (st != nullptr) *st = r;
        return out.text();
    }
};

void check_body_model(LLWINDEF_DATA& w)
{
    CHECK_EQ(w.geo3d.node_count(), 4);
    CHECK_EQ(w.geo3d.elem_count(), 2);
    const double want_xyz[12] = {0, 0, 0, 1.5, 0, 0, 1.5, 2.25, -1e-3, 0, 2.25, 123.45678901};
    const double* xyz = w.geo3d.coords.begin();
    for (int i = 0; i < 12; ++i) CHECK_EQ(xyz[i], want_xyz[i]);
    const std::int32_t want_el[8] = {1, 2, 3, 4, 3, 4, 1, 1};
    const std::int32_t* el = w.geo3d.elems.begin();
    for (int i = 0; i < 8; ++i) CHECK_EQ(el[i], want_el[i]);
    CHECK_EQ(w.npts, 0); // shape 3 leaves the point buffer alone
    CHECK(w.pts == nullptr);
}

} // namespace

TEST_CASE(shape3_read_fills_geo3d_and_round_trips)
{
    keytest::legacy_printf();
    Window x;
    CHECK_EQ(x.read(kBody, true, "g3w3_in.txt"), 0);
    check_body_model(x.w);
    // the indices are written in every mode (no with_index test in this branch)
    int st = -1;
    CHECK_EQ(x.write(0, "g3w3_out0.txt", &st), std::string(kBody));
    CHECK_EQ(st, 0);
    CHECK_EQ(x.write(1, "g3w3_out1.txt", &st), std::string(kBody));
    CHECK_EQ(st, 0);
}

TEST_CASE(shape3_read_ignores_indices_and_uses_free_format)
{
    keytest::legacy_printf();
    // leading indices are read but never compared, also with indexed = true; commas are blanks
    // and trailing text is ignored; the same lines are read with indexed = false
    const std::string loose = "4\n"
                              "9 0 0 0\n"
                              "-1,1.5,0,0\n"
                              "0 1.5 2.25 -1e-3 extra\n"
                              "4 0 2.25 123.45678901\n"
                              "2\n"
                              "5 1 2 3 4\n"
                              "5 3 4 1 1 7\n";
    for (bool indexed : {true, false}) {
        Window x;
        CHECK_EQ(x.read(loose, indexed, "g3w3l_in.txt"), 0);
        check_body_model(x.w);
        CHECK_EQ(x.write(0, "g3w3l_out.txt"), std::string(kBody));
    }
}

TEST_CASE(shape3_empty_and_errors)
{
    keytest::legacy_printf();
    {
        // no nodes and no elements: nothing allocated, "0\n0\n" written
        Window x;
        CHECK_EQ(x.read("0\n0\n", true, "g3w3e_in.txt"), 0);
        CHECK_EQ(x.w.geo3d.node_count(), 0);
        CHECK_EQ(x.w.geo3d.elem_count(), 0);
        CHECK_EQ(x.write(0, "g3w3e_out.txt"), std::string("0\n0\n"));
    }
    {
        // EOF before the node count
        Window x;
        CHECK_EQ(x.read("", true, "g3w3e1_in.txt"), 1);
    }
    {
        // EOF in the node lines: the nodes already read stay
        Window x;
        CHECK_EQ(x.read("3\n1 1 2 3\n", true, "g3w3e2_in.txt"), 1);
        CHECK_EQ(x.w.geo3d.node_count(), 3);
        CHECK_EQ(x.w.geo3d.coords.begin()[2], 3.0);
        CHECK_EQ(x.w.geo3d.coords.begin()[3], 0.0);
    }
    {
        // EOF before the element count / in the element lines
        Window x;
        CHECK_EQ(x.read("1\n1 1 2 3\n", true, "g3w3e3_in.txt"), 1);
        CHECK_EQ(x.w.geo3d.elem_count(), 0);
        Window y;
        CHECK_EQ(y.read("1\n1 1 2 3\n2\n1 1 1 1 1\n", true, "g3w3e4_in.txt"), 1);
        CHECK_EQ(y.w.geo3d.elem_count(), 2);
        CHECK_EQ(y.w.geo3d.elems.begin()[4], 0);
    }
    {
        // a null file: 2 before anything is written
        Window x;
        CHECK_EQ(window_write_points(nullptr, &x.w, 0), 2);
    }
}

// ---- MGUSER shape 3 end to end (kw-g2mg reader case 0x32 -> 14041b7d0, writer -> 1402f4890) ----

namespace {

struct G2Hooks : def::key::KeyReaderHooks {
    DB_STEP_DATA& m;
    explicit G2Hooks(DB_STEP_DATA& model) : m(model) {}
    bool has_sub_model() override { return false; }
    void set_model_dimension(int dim) override { m.dimension = dim; }
    void set_sub_model_dimension(int) override {}
    int parse_keyword(int group, std::FILE* f, int id) override
    {
        return group == 2 ? def::key::g2obj::read_object_keyword(m, f, id) : 3;
    }
};

// A model from the real ctor (3D); object 1 made by the group 2 parser (as test_kw_g2mg.cpp).
struct Model {
    unsigned char* mem;
    Model() : mem(static_cast<unsigned char*>(::operator new(sizeof(DB_STEP_DATA))))
    {
        new (mem) DB_STEP_DATA(3);
        m().dimension = 3;
        read("MGSIZM 1 0\n");
    }
    DB_STEP_DATA& m() { return *reinterpret_cast<DB_STEP_DATA*>(mem); }
    int read(const std::string& key)
    {
        keytest::InFile in("g3w3mg_in.KEY", key);
        keytest::OutFile log("g3w3mg.log");
        G2Hooks h(m());
        return def::key::read_key_stream(in.get(), h, log.get());
    }
    LLOBJ_DATA& obj(int n) { return *def::key::g2obj::find_object(m(), n); }
};

} // namespace

TEST_CASE(mguser_shape3_key_round_trip)
{
    keytest::legacy_printf();
    Model md;
    // the MGUSER record as the mesh-settings writer prints it for a shape-3 window (no flag)
    const std::string rec = std::string("MGUSER       1       1    2.5000000000E+000    1.0000000000E-001"
                                        "    2.0000000000E-001    3.0000000000E-001       3       1\n") +
                            kBody;
    CHECK_EQ(md.read(rec), 0);
    auto* list = reinterpret_cast<def::model::Cls_140269250*>(reinterpret_cast<unsigned char*>(&md.obj(1)) +
                                                              0x2d50);
    CHECK_EQ(def::model::winlist_count(list), 1);
    LLWINDEF_DATA* w = def::model::winlist_nth(list, 1);
    CHECK(w != nullptr);
    CHECK_EQ(w->shape, 3);
    check_body_model(*w);
    CHECK(w->faces == nullptr); // 140267760 builds faces for shapes 1 and 2 only
    keytest::OutFile out("g3w3mg_out.KEY");
    def::key::g2mg::write_mesh_block(md.m(), out.get(), md.obj(1), 1);
    const std::string text = out.text();
    const std::size_t at = text.find("MGUSER");
    CHECK(at != std::string::npos);
    if (at != std::string::npos) {
        if (text.substr(at) != rec) std::printf("%s", text.c_str());
        CHECK_EQ(text.substr(at), rec); // MGUSER is the last line group of the block
    }
}

int main() { return testkit::run_all(); }
