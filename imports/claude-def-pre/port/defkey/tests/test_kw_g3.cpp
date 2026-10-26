// ctest-labels: kw-g3
// Tier-1 tests of the group 3 (inter-object) KEY reader FUN_140388fb0 and writer FUN_1403c9ad0.
//
// Golden files (tests/data/kw_g3): g3_*.KEY are hand-written inputs covering every keyword form;
// g3_*.expected is the "Inter-Object Data" section of the original DEF_PRE's KFWRIT after KFREAD
// of that input (oracle cache, CMD.KEY "KFREAD / in.KEY / KFWRIT / out.KEY"; CR removed), and
// g3_quirks_3d.messages the loop's messages from the same run. Other expected values are worked
// out from the decompiled code in the comments next to them.
#include "kw_g3_test_util.hpp"

#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/window/inter_object_ops.hpp>
#include <defmodel/window/window_list.hpp>
#include <testkit/check.hpp>

#include <cmath>
#include <string>
#include <vector>

using namespace def::key::g3;
using g3test::data_path;
using g3test::read_file;
using g3test::round_trip;
using g3test::strip_cr;

namespace {

std::string drop_blank_lines(const std::string& s)
{
    std::string o, line;
    for (char c : s) {
        if (c == '\n') {
            if (!line.empty()) o += line + "\n";
            line.clear();
        } else if (c != '\r') {
            line += c;
        }
    }
    if (!line.empty()) o += line + "\n";
    return o;
}

// Read `text` (no dimension comment: the model keeps `dim`) and return the model.
struct Loaded {
    g3test::FakeModel model;
    int status;
    std::string messages;
    Loaded(int dim, const std::string& text, const char* tag) : model(dim)
    {
        keytest::legacy_printf();
        keytest::InFile in((std::string(tag) + "_in.txt").c_str(), text);
        keytest::OutFile msg((std::string(tag) + "_msg.txt").c_str());
        g3test::Hooks hooks(model.get());
        status = def::key::read_key_stream(in.get(), hooks, msg.get());
        messages = msg.text();
    }
    Cls_1401fa020* list() { return &model.get()->inter_obj; }
    Cls_1401f46e0* pair(int m, int s) { return pair_find(list(), m, s); }
    std::string write(const char* tag)
    {
        keytest::OutFile out((std::string(tag) + "_out.txt").c_str());
        last_write = write_inter_objects(model.get(), out.get());
        return out.text();
    }
    int last_write = 0;
};

} // namespace

// ---- oracle round trips -------------------------------------------------------------------------

TEST_CASE(oracle_round_trip_3d)
{
    const auto r = round_trip(read_file(data_path("g3_3d.KEY")), "g3rt3");
    CHECK_EQ(r.read_status, 0);
    CHECK_EQ(r.write_status, 0);
    CHECK_EQ(r.written, strip_cr(read_file(data_path("g3_3d.expected"))));
    CHECK_EQ(r.messages, std::string(""));
}

TEST_CASE(oracle_round_trip_2d)
{
    const auto r = round_trip(read_file(data_path("g3_2d.KEY")), "g3rt2");
    CHECK_EQ(r.read_status, 0);
    CHECK_EQ(r.write_status, 0);
    CHECK_EQ(r.written, strip_cr(read_file(data_path("g3_2d.expected"))));
    CHECK_EQ(r.messages, std::string(""));
}

// Clearing with an empty CNTACT, unchanged values for out-of-range codes, SEPDEN in 3D, FRCWIN
// shape 3 and orientation 3 (Reading Error), the mirror copy overwriting a window list.
TEST_CASE(oracle_round_trip_quirks_3d)
{
    const auto r = round_trip(read_file(data_path("g3_quirks_3d.KEY")), "g3rtq");
    CHECK_EQ(r.read_status, 2); // the loop reports the last error status
    CHECK_EQ(r.write_status, 0);
    CHECK_EQ(r.written, strip_cr(read_file(data_path("g3_quirks_3d.expected"))));
    CHECK_EQ(drop_blank_lines(r.messages), strip_cr(read_file(data_path("g3_quirks_3d.messages"))));
}

// Friction type 9 (no FRCFAC record, no mode-4 sub-record), the FRCWIN type code kept from the
// previous window for type 9, FRCROT read in 3D but not written, 2D window types 5 and 7.
TEST_CASE(oracle_round_trip_quirks2)
{
    for (const char* d : {"3d", "2d"}) {
        const std::string base = std::string("g3_quirks2_") + d;
        const auto r = round_trip(read_file(data_path((base + ".KEY").c_str())), "g3rtq2");
        CHECK_EQ(r.read_status, 0);
        CHECK_EQ(r.write_status, 0);
        CHECK_EQ(r.written, strip_cr(read_file(data_path((base + ".expected").c_str()))));
        CHECK_EQ(r.messages, std::string(""));
    }
}

// ---- reader details (from the decompile of 140388fb0) ------------------------------------------

TEST_CASE(cntact_codes_and_mirror)
{
    Loaded l3(3, "CNTACT 2 5 3\nCNTACT 6 1 2\n", "g3c1");
    Cls_1401f46e0* p = l3.pair(2, 5);
    REQUIRE(p != nullptr);
    CHECK_EQ(p->u170, 1); // 3D code 3 -> (1, 0)
    CHECK_EQ(p->u174, 0);
    Cls_1401f46e0* q = l3.pair(5, 2); // mirror: direction 1 -> 2, sub-flag copied in 3D
    REQUIRE(q != nullptr);
    CHECK_EQ(q->u170, 2);
    CHECK_EQ(q->u174, 0);
    CHECK_EQ(l3.pair(1, 6)->u170, 1); // (6,1) has 2, its mirror (1,6) gets 1
    CHECK_EQ(pair_count(l3.list()), 4);
    // written once, from the lower object number
    CHECK_EQ(l3.write("g3c1"), std::string("CNTACT       2       5       3\n"
                                           "FRCFAC       2       5       1       0    0.0000000000E+000\n"
                                           "IHTCOF       2       5       0    0.0000000000E+000\n"
                                           "SEPRES       2       5       0    0.0000000000E+000\n"
                                           "INTRST       2       5       0    0.0000000000E+000\n"
                                           "CNTMTH       2       5       0\n"
                                           "CNTACT       1       6       1\n"
                                           "FRCFAC       1       6       1       0    0.0000000000E+000\n"
                                           "IHTCOF       1       6       0    0.0000000000E+000\n"
                                           "SEPRES       1       6       0    0.0000000000E+000\n"
                                           "INTRST       1       6       0    0.0000000000E+000\n"
                                           "CNTMTH       1       6       0\n"));
    Loaded l2(2, "CNTACT 1 2 3\nCNTACT 1 3 2\n", "g3c2");
    CHECK_EQ(l2.pair(1, 2)->u170, 0); // 2D: code 3 leaves the new pair's direction 0
    CHECK_EQ(l2.pair(1, 3)->u170, 2);
    CHECK_EQ(l2.pair(3, 1)->u170, 1);
}

TEST_CASE(cntact_clear_forms)
{
    Loaded a(3, "CNTACT 1 2 1\nCNTACT 0\nCNTACT 3 4 1\n", "g3cl1");
    CHECK_EQ(pair_count(a.list()), 2); // "CNTACT 0" deleted (1,2) and (2,1)
    CHECK(a.pair(1, 2) == nullptr);
    Loaded b(3, "CNTACT 1 2 1\nCNTACT 5\n", "g3cl2");
    CHECK_EQ(pair_count(b.list()), 2); // a non-zero single token keeps the pairs
    Loaded c(3, "CNTACT 1 2 1\nCNTACT 1 2\n", "g3cl3");
    CHECK_EQ(pair_count(c.list()), 2); // two tokens: nothing
    CHECK_EQ(c.status, 0);
}

TEST_CASE(frcfac_short_tables)
{
    // mode 4: friction 1 is a 3-point table and the file ends; its table status is overwritten
    // by the helper of the second friction line, which hits EOF: return 2 ("Reading Error")
    Loaded l(3, "FRCFAC 1 2 4 0 0\n 1 3 0 0\n", "g3fm4");
    CHECK_EQ(l.status, 2);
    Loaded k(3, "FRCFAC 1 2 1 1 2\n 0 0.5\n", "g3fm1");
    CHECK_EQ(k.status, 2); // single friction: the short table's status is kept -> 2
}

TEST_CASE(friction_types_store_fields)
{
    Loaded l(3, "FRCFAC 1 2 2 6 0.1 0.2 0.3\nFRCFAC 3 4 3 -9 1.5 0 0\n", "g3ft");
    const auto* fb = &l.pair(1, 2)->u000;
    CHECK_EQ(fb->u000, 2);
    CHECK_EQ(fb->u010, 6);
    CHECK_BITS(*reinterpret_cast<const double*>(&fb->u108[0]), 0.1);
    CHECK_BITS(*reinterpret_cast<const double*>(&fb->u108[1]), 0.2);
    CHECK_EQ(fb->u018.ndim, 0);
    const auto* fx = &l.pair(3, 4)->u000;
    CHECK_EQ(fx->u004, 1); // fixed
    CHECK_EQ(fx->u008, 9);
    CHECK_BITS(fx->u018.value, 1.5);
    // mirror copied the friction block and the parameter pairs
    CHECK_BITS(*reinterpret_cast<const double*>(&l.pair(2, 1)->u000.u108[1]), 0.2);
    CHECK_EQ(l.pair(4, 3)->u000.u008, 9);
}

TEST_CASE(sepden_3d_leaves_the_line)
{
    Loaded l(3, "SEPDEN 1 2 7.5\n", "g3sd");
    CHECK_BITS(*reinterpret_cast<const double*>(&l.pair(1, 2)->u238), 0.0);
    // the main loop reads "7.5" as an unknown keyword
    CHECK_EQ(drop_blank_lines(l.messages), std::string("WARNING: Keyword 7.5 not found. Skip \n"));
    Loaded m(2, "SEPDEN 1 2 7.5\n", "g3sd2");
    CHECK_BITS(*reinterpret_cast<const double*>(&m.pair(1, 2)->u238), 7.5);
    CHECK_BITS(*reinterpret_cast<const double*>(&m.pair(2, 1)->u238), 7.5); // 2D mirror copies it
}

TEST_CASE(wmodel_and_rgdcnt)
{
    Loaded l(3, "WMODEL 1 2 -4 0\nRGDCNT 1 2 4 7 8 0.5\nRGDCNT 1 2 5 1 1 1\n", "g3wr");
    Cls_1401f46e0* p = l.pair(1, 2);
    CHECK_EQ(p->u340, 3);
    CHECK_EQ(p->u378, 4);
    CHECK_EQ(p->u37c, 4); // 5 is outside 0..4: ignored
    CHECK_EQ(p->u380, 7);
    CHECK_EQ(p->u384, 8);
    CHECK_EQ(l.status, 0);
}

TEST_CASE(frcwin_3d_box_faces)
{
    Loaded l(3,
             "FRCWIN 1 2 2 1 1 0 0.1 1\n 0 0 0\n"
             " 1 0 0 0\n 2 2 0 0\n 3 2 4 0\n 4 0 4 0\n 5 0 0 6\n 6 2 0 6\n 7 2 4 6\n 8 0 4 6\n",
             "g3wb");
    CHECK_EQ(l.status, 0);
    Cls_1401f46e0* p = l.pair(1, 2);
    CHECK_EQ(winlist_count(&p->u188), 2); // window 2 requested: windows 1 and 2 exist
    LLWINDEF_DATA* w1 = winlist_nth(&p->u188, 1);
    LLWINDEF_DATA* w = winlist_nth(&p->u188, 2);
    CHECK_EQ(w1->index, 1);
    CHECK_EQ(w->index, 2);
    CHECK_EQ(w->shape, 1);
    REQUIRE(w->nface == 6); // 140263dc0: 8 points -> 6 faces
    // 140267760: face 0 = (p2 + p7) / 2 = ((2,0,0) + (2,4,6)) / 2
    CHECK_BITS(w->faces[0], 2.0);
    CHECK_BITS(w->faces[1], 2.0);
    CHECK_BITS(w->faces[2], 3.0);
    // face 5 = (p1 + p3) / 2 = ((0,0,0) + (2,4,0)) / 2
    CHECK_BITS(w->faces[15], 1.0);
    CHECK_BITS(w->faces[16], 2.0);
    CHECK_BITS(w->faces[17], 0.0);
    CHECK_EQ(w->orient, 0);
    // the mirror (2,1) got its own copy of both windows, without face centres (14025c9b0)
    LLWINDEF_DATA* m2 = winlist_nth(&l.pair(2, 1)->u188, 2);
    REQUIRE(m2 != nullptr);
    CHECK(m2 != w);
    CHECK(m2->faces == nullptr);
    CHECK_BITS(m2->pts[3 * 6 + 2], 6.0);
}

TEST_CASE(frcwin_3d_cylinder_rim_points)
{
    // axis along z from (0,0,0) to (0,0,10), radius 2, second radius 0.5 -> 4 faces
    Loaded l(3, "FRCWIN 1 2 1 2 1 0 0.1 1\n 0 0 0\n 0 0 0\n 0 0 10\n 0.5 2.0\n", "g3wc");
    LLWINDEF_DATA* w = winlist_nth(&l.pair(1, 2)->u188, 1);
    REQUIRE(w->nface == 4);
    // 140267560(w, 2): dir = (0,0,1) (largest |component| index 2); G = (1,1,0), dot = 0,
    // G[2] = -0/1 = -0.0; |G| = sqrt(2); G = (2/sqrt2, 2/sqrt2, -0.0) + face 1 (0,0,10)
    const double k = (1.0 / std::sqrt(2.0)) * 2.0;
    CHECK_BITS(w->faces[6], k);
    CHECK_BITS(w->faces[7], k);
    CHECK_BITS(w->faces[8], 10.0);
    const double k2 = (1.0 / std::sqrt(2.0)) * 0.5;
    CHECK_BITS(w->faces[9], k2);
    CHECK_BITS(w->faces[11], 10.0);
}

TEST_CASE(frcwin_window_zero_on_empty_list_is_null)
{
    // FRCWIN window 0 on a pair without windows: 140269900 returns NULL and the original crashes
    // in 14025d1a0. Not exercised (the port aborts on purpose); window 0 with windows present
    // resets the first window:
    Loaded l(3, "FRCWIN 1 2 1 1 1 0 0.1 1\n 0 0 0\n 1 0 0 0\n 2 1 0 0\n 3 1 1 0\n 4 0 1 0\n"
                " 5 0 0 1\n 6 1 0 1\n 7 1 1 1\n 8 0 1 1\n"
                "FRCWIN 1 2 0 1 2 0 0.7 1\n 0 0 0\n 1 0 0 0\n 2 1 0 0\n 3 1 1 0\n 4 0 1 0\n"
                " 5 0 0 1\n 6 1 0 1\n 7 1 1 1\n 8 0 1 1\n",
             "g3w0");
    LLWINDEF_DATA* w = winlist_nth(&l.pair(1, 2)->u188, 1);
    CHECK_EQ(winlist_count(&l.pair(1, 2)->u188), 1);
    CHECK_EQ(w->fric.u000, 2);
    CHECK_EQ(w->index, 1); // kept by 14025d1a0
    CHECK_BITS(w->fric.u018.value, 0.7);
}

// ---- writer details (from the decompile of 1403c8560 and the record writers) -------------------

TEST_CASE(write_aborts_on_bad_codes)
{
    Loaded l(3, "CNTACT 1 2 1\nCNTACT 3 4 1\n", "g3wa");
    l.pair(1, 2)->u178 = 7; // SEPRES code outside 0..2 -> 3, after the earlier records
    const std::string out = l.write("g3wa");
    CHECK_EQ(l.last_write, 3);
    CHECK_EQ(out, std::string("CNTACT       1       2       1\n"
                              "FRCFAC       1       2       1       0    0.0000000000E+000\n"
                              "IHTCOF       1       2       0    0.0000000000E+000\n"));
    // a 3D window with shape 3 (only from a DB) stops the write before its record
    Loaded w(3, "CNTACT 1 2 1\nFRCWIN 1 2 1 1 1 0 0.1 1\n 0 0 0\n 1 0 0 0\n 2 1 0 0\n 3 1 1 0\n"
                " 4 0 1 0\n 5 0 0 1\n 6 1 0 1\n 7 1 1 1\n 8 0 1 1\n",
             "g3wa2");
    winlist_nth(&w.pair(1, 2)->u188, 1)->shape = 3;
    const std::string o2 = w.write("g3wa2");
    CHECK_EQ(w.last_write, 3);
    CHECK_EQ(o2, std::string("CNTACT       1       2       1\n"
                             "FRCFAC       1       2       1       0    0.0000000000E+000\n"
                             "IHTCOF       1       2       0    0.0000000000E+000\n"));
    // FRCFAC and IHTCOF results are ignored: a bad friction mode only drops the FRCFAC record
    Loaded f(3, "CNTACT 1 2 1\n", "g3wa3");
    f.pair(1, 2)->u000.u000 = 9;
    const std::string o3 = f.write("g3wa3");
    CHECK_EQ(f.last_write, 0);
    CHECK_EQ(o3, std::string("CNTACT       1       2       1\n"
                             "IHTCOF       1       2       0    0.0000000000E+000\n"
                             "SEPRES       1       2       0    0.0000000000E+000\n"
                             "INTRST       1       2       0    0.0000000000E+000\n"
                             "CNTMTH       1       2       0\n"));
}

TEST_CASE(write_skips)
{
    Loaded l(3, "CNTACT 0 2 1\nCNTACT 3 3 1\nCNTACT 3 3 2\n", "g3ws");
    // (0,2): master 0 -> skipped; (2,0) has s < m; (3,3) direction 2 after the second line
    CHECK_EQ(l.write("g3ws"), std::string("CNTACT       3       3       2\n"
                                          "FRCFAC       3       3       1       0    0.0000000000E+000\n"
                                          "IHTCOF       3       3       0    0.0000000000E+000\n"
                                          "SEPRES       3       3       0    0.0000000000E+000\n"
                                          "INTRST       3       3       0    0.0000000000E+000\n"
                                          "CNTMTH       3       3       0\n"));
}

// ---- registry handler columns 2 and 3 (1403cab90 ... 1403cbdf0) -------------------------------

TEST_CASE(registry_handlers)
{
    Loaded l(3, "CNTACT 1 2 1\nFRCFAC 1 2 1 0 0.25\nSEPRES 1 2 1 0.5\n", "g3rh");
    keytest::OutFile out("g3rh_out.txt");
    CHECK_EQ(handler2_frcfac(out.get(), l.model.get(), {0, 1, 2}), 0);
    CHECK_EQ(handler2_frcfac(out.get(), l.model.get(), {0, 1}), 3); // needs 3 elements
    CHECK_EQ(handler2_sepres(out.get(), l.model.get(), {1, 0, 0}), 0); // pair #1 by position
    CHECK_EQ(handler2_sepres(out.get(), l.model.get(), {1}), 3);
    CHECK_EQ(out.text(), std::string("FRCFAC       1       2       1       0    2.5000000000E-001\n"
                                     "SEPRES       1       2       1    5.0000000000E-001\n"));
    DYNARY* t = nullptr;
    CHECK_EQ(handler3_frcfac(l.model.get(), "Fric Frac 2", {0, 1, 2}, &t), 0);
    CHECK(t == &l.pair(1, 2)->u000.u090);
    handler3_frcfac(l.model.get(), "x", {0, 1, 2}, &t);
    CHECK(t == &l.pair(1, 2)->u000.u018);
    handler3_intrst(l.model.get(), "", {0, 2, 1}, &t);
    CHECK(t == &l.pair(2, 1)->u2b8);
    DYNARY* none = nullptr;
    handler3_frcrot(l.model.get(), "", {0, 7, 8}, &none);
    CHECK(none == nullptr); // no pair: left unchanged
}

int main() { return testkit::run_all(); }
