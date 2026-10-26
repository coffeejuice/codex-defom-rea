// ctest-labels: kw-g2geo group2 curve
// Tier-1 tests of the kw-g2geo curve family: DIEGEO curve / 3D curve / type-3 forms, TPLOGY,
// CSPMTX and their writers (1402aaa70, 1402a9f90, 1402aaf40, 1402ab160), including the automatic
// topology build (140281bf0) and degenerate-curve removal (140304e40).
//
// Expected text: the corpus round trips are copied from the original's KFWRIT_OUT.KEY in the
// oracle cache (labels kfrw/key/<path>, quoted per test); the others are worked out by hand from
// the format strings (" %7i %20.10E": ints right-aligned in 7, reals in 20 columns with the old
// CRT's three-digit exponent) and the specs (DIEGEO.md, TPLOGY.md, CSPMTX.md).
#define _CRT_SECURE_NO_WARNINGS
#include "key_test_util.hpp"

#include <cstring>
#include <sstream>
#include <string>

#include "../src/kw_g2geo/internal.hpp"

#include <defkey/kw_g2obj.hpp>
#include <defmodel/geo_kw/curve_model.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <testkit/check.hpp>

namespace gk = def::model::geo_kw;
namespace cv = def::key::g2geo::curve;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;
using keytest::InFile;
using keytest::OutFile;

namespace {

template <class T, class C>
T& at(C& base, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<unsigned char*>(&base) + off);
}

struct Model {
    void* raw;
    explicit Model(int dim)
    {
        raw = new DB_STEP_DATA(dim);  // the real model ctor (p1-03); leaked with its objects
    }
    DB_STEP_DATA& m() { return *static_cast<DB_STEP_DATA*>(raw); }
};

constexpr int kDIEGEO = 158, kTPLOGY = 269, kCSPMTX = 270;

int id_of(const std::string& name)
{
    if (name == "DIEGEO") return kDIEGEO;
    if (name == "TPLOGY") return kTPLOGY;
    if (name == "CSPMTX") return kCSPMTX;
    return -1;
}

// Reads `text` the way the KEY loop would: each keyword line's name is consumed by the loop and
// the parser gets the rest of the line plus the record lines up to the next keyword line.
// Returns the status of the last keyword (or the first nonzero one).
int read_key(Model& md, const std::string& text)
{
    std::istringstream in(text);
    std::string line, body, name;
    int st = 0;
    auto flush = [&]() {
        if (name.empty()) return;
        InFile f("kw_g2geo_curve_in.tmp", body);
        int r = def::key::g2obj::read_object_keyword(md.m(), f.get(), id_of(name));
        if (st == 0) st = r;
    };
    while (std::getline(in, line)) {
        if (line.size() >= 6 && id_of(line.substr(0, 6)) > 0) {
            flush();
            name = line.substr(0, 6);
            body = line.substr(6) + "\n";
        } else {
            body += line + "\n";
        }
    }
    flush();
    return st;
}

LLOBJ_DATA& object(Model& md, int no) { return *def::key::g2obj::find_object(md.m(), no); }

// ---- corpus text --------------------------------------------------------------------------------

// key/3D/EN/Cogging/Cogging_Multiple_Ops_Lab2/Objects/Object00005.KEY lines 34-80; the original's
// KFWRIT_OUT.KEY (oracle kfrw case) has exactly these lines at 135-181 (writer-map step 19, then
// 1402ab160 from step 20).
const char* kCogging5 =
    "DIEGEO       5       1      13\n"
    "       1   -2.1213203436E+001    0.0000000000E+000    0.0000000000E+000\n"
    "       2   -1.0606601718E+001   -1.0606601718E+001    4.0000000000E-001\n"
    "       3   -1.0323759005E+001   -1.0323759005E+001    0.0000000000E+000\n"
    "       4   -9.1923881554E+000   -9.1923881554E+000    4.0000000000E-001\n"
    "       5   -9.1621636534E+000   -9.1236603164E+000    0.0000000000E+000\n"
    "       6   -3.5768493775E+000    3.5768493775E+000    8.0000000000E-001\n"
    "       7   -3.2918987913E+000    3.7021623465E+000    0.0000000000E+000\n"
    "       8    9.1923881554E+000    9.1923881554E+000    4.0000000000E-001\n"
    "       9    9.2454778737E+000    9.2454778737E+000    0.0000000000E+000\n"
    "      10    1.0606601718E+001    1.0606601718E+001    4.0000000000E-001\n"
    "      11    1.0323759005E+001    1.0889444430E+001    0.0000000000E+000\n"
    "      12    1.7763568394E-015    2.1213203436E+001    0.0000000000E+000\n"
    "      13   -2.1213203436E+001    1.7763568394E-015    0.0000000000E+000\n";
const char* kCogging5Topo =
    "TPLOGY       5       1       1       1       1\n"
    "* Vertex Data: \n"
    "       1       1    0.0000000000E+000    0.0000000000E+000\n"
    "       1\n"
    "* Edge Data:\n"
    "       1       1       1       1\n"
    "       1\n"
    "* Loop Data:\n"
    "       1       1       1\n"
    "       1\n"
    "       1\n"
    "* Face Data:\n"
    "       1       1       0\n"
    "       1\n"
    "CSPMTX       5       1\n"
    "       1    6.1232339957E-017   -1.0000000000E+000    1.2246467991E-016    0.0000000000E+000\n"
    "       2   -6.1232339957E-017   -1.2246467991E-016   -1.0000000000E+000    0.0000000000E+000\n"
    "       3    1.0000000000E+000    6.1232339957E-017   -6.1232339957E-017    0.0000000000E+000\n"
    "       4   -2.0000000000E+001    3.9553394516E+000   -3.9565037501E+000    1.0000000000E+000\n";

}  // namespace

// The first point line above is line 35 of the corpus file (the DIEGEO header is line 34).
TEST_CASE(cogging_3d_curve_tplogy_cspmtx_round_trip)
{
    Model md(3);
    const std::string in = std::string(kCogging5) + kCogging5 + kCogging5Topo;
    CHECK_EQ(read_key(md, in), 0);
    LLOBJ_DATA& o = object(md, 5);
    CHECK_EQ(gk::curve_count(o), 1);
    keytest::legacy_printf();
    OutFile out("kw_g2geo_curve_out.tmp");
    cv::write_curves(md.m(), out.get(), o, 5);        // step 19
    CHECK_EQ(cv::write_curve_group(md.m(), out.get(), o, 5), 0);  // 1402ab160
    CHECK_EQ(out.text(), in);
}

// key/3D/SI/FlowForming/Implicit_ALE/Operations/Task00001/Flowform2DGeo.KEY objects 1 and 4
// (type-3 form, nhdr 4: the identity matrix, then the points); KFWRIT_OUT.KEY lines 118-123 and
// 283-290 (step 19 only: no surface flag).
TEST_CASE(flowform_type3)
{
    Model md(3);
    const std::string in =
        "DIEGEO       1       3       9       4\n"
        "       1    1.0000000000E+000    0.0000000000E+000    0.0000000000E+000    0.0000000000E+000\n"
        "       2    0.0000000000E+000    1.0000000000E+000    0.0000000000E+000    0.0000000000E+000\n"
        "       3    0.0000000000E+000    0.0000000000E+000    1.0000000000E+000    0.0000000000E+000\n"
        "       4    0.0000000000E+000    0.0000000000E+000    0.0000000000E+000    1.0000000000E+000\n"
        "       5    0.0000000000E+000    1.0000000000E+001    0.0000000000E+000\n"
        "       6    5.0000000000E+000    1.0000000000E+001    0.0000000000E+000\n"
        "       7    5.0000000000E+000    3.5000000000E+001    0.0000000000E+000\n"
        "       8    0.0000000000E+000    3.5000000000E+001    0.0000000000E+000\n"
        "       9    0.0000000000E+000    1.0000000000E+001    0.0000000000E+000\n"
        "DIEGEO       4       3      11       4\n"
        "       1    1.0000000000E+000    0.0000000000E+000    0.0000000000E+000    0.0000000000E+000\n"
        "       2    0.0000000000E+000    1.0000000000E+000    0.0000000000E+000    0.0000000000E+000\n"
        "       3    0.0000000000E+000    0.0000000000E+000    1.0000000000E+000    0.0000000000E+000\n"
        "       4    2.0000000000E+000    3.0000000000E+000    0.0000000000E+000    1.0000000000E+000\n"
        "       5    0.0000000000E+000    1.6000000000E+001    0.0000000000E+000\n"
        "       6    1.8000000000E+001    1.6000000000E+001    0.0000000000E+000\n"
        "       7    1.8000000000E+001    3.6795000000E+001    0.0000000000E+000\n"
        "       8    1.0376089695E+001    5.0000000000E+001    2.3834572681E+000\n"
        "       9    7.6239103047E+000    5.0000000000E+001    2.3834572681E+000\n"
        "      10    0.0000000000E+000    3.6795000000E+001    0.0000000000E+000\n"
        "      11    0.0000000000E+000    1.6000000000E+001    0.0000000000E+000\n";
    CHECK_EQ(read_key(md, in), 0);
    keytest::legacy_printf();
    {
        OutFile out("kw_g2geo_curve_out.tmp");
        cv::write_curves(md.m(), out.get(), object(md, 1), 1);
        CHECK_EQ(out.text(), std::string("DIEGEO       1       1       5\n"
                                         "       1    0.0000000000E+000    1.0000000000E+001    0.0000000000E+000\n"
                                         "       2    5.0000000000E+000    1.0000000000E+001    0.0000000000E+000\n"
                                         "       3    5.0000000000E+000    3.5000000000E+001    0.0000000000E+000\n"
                                         "       4    0.0000000000E+000    3.5000000000E+001    0.0000000000E+000\n"
                                         "       5    0.0000000000E+000    1.0000000000E+001    0.0000000000E+000\n"));
    }
    {
        // the matrix row 4 of object 4 is changed from the corpus (0 0 0 1) to show where it goes
        OutFile out("kw_g2geo_curve_out.tmp");
        cv::write_curves(md.m(), out.get(), object(md, 4), 4);
        cv::write_cspmtx(md.m(), out.get(), object(md, 4), 4);
        CHECK_EQ(out.text(), std::string("DIEGEO       4       1       7\n"
                                         "       1    0.0000000000E+000    1.6000000000E+001    0.0000000000E+000\n"
                                         "       2    1.8000000000E+001    1.6000000000E+001    0.0000000000E+000\n"
                                         "       3    1.8000000000E+001    3.6795000000E+001    0.0000000000E+000\n"
                                         "       4    1.0376089695E+001    5.0000000000E+001    2.3834572681E+000\n"
                                         "       5    7.6239103047E+000    5.0000000000E+001    2.3834572681E+000\n"
                                         "       6    0.0000000000E+000    3.6795000000E+001    0.0000000000E+000\n"
                                         "       7    0.0000000000E+000    1.6000000000E+001    0.0000000000E+000\n"
                                         "CSPMTX       4       1\n"
                                         "       1    1.0000000000E+000    0.0000000000E+000    0.0000000000E+000    0.0000000000E+000\n"
                                         "       2    0.0000000000E+000    1.0000000000E+000    0.0000000000E+000    0.0000000000E+000\n"
                                         "       3    0.0000000000E+000    0.0000000000E+000    1.0000000000E+000    0.0000000000E+000\n"
                                         "       4    2.0000000000E+000    3.0000000000E+000    0.0000000000E+000    1.0000000000E+000\n"));
    }
}

// key/2D/SI/MultipleOperations/MO2_Boolean_Oprations/Operations/Task00002/STEP.KEY object 2: a
// closed type-1 curve without TPLOGY; the original builds the topology (one vertex left at 0, 0,
// one closed edge, one loop, one face). Expected: its KFWRIT_OUT.KEY.
TEST_CASE(built_topology_closed_curve_2d)
{
    Model md(2);
    CHECK_EQ(read_key(md, "DIEGEO       2       1       5\n"
                          "       1  0.0000000E+000 -2.0000000E+001  0.0000000E+000\n"
                          "       2  7.5000000E+001 -2.0000000E+001  0.0000000E+000\n"
                          "       3  7.5000000E+001  0.0000000E+000  0.0000000E+000\n"
                          "       4  0.0000000E+000  0.0000000E+000  0.0000000E+000\n"
                          "       5  0.0000000E+000 -2.0000000E+001  0.0000000E+000\n"),
             0);
    LLOBJ_DATA& o = object(md, 2);
    keytest::legacy_printf();
    OutFile out("kw_g2geo_curve_out.tmp");
    cv::write_curves(md.m(), out.get(), o, 2);
    CHECK_EQ(cv::write_tplogy(md.m(), out.get(), o, 2), 0);
    CHECK_EQ(out.text(), std::string("DIEGEO       2       1       5\n"
                                     "       1    0.0000000000E+000   -2.0000000000E+001    0.0000000000E+000\n"
                                     "       2    7.5000000000E+001   -2.0000000000E+001    0.0000000000E+000\n"
                                     "       3    7.5000000000E+001    0.0000000000E+000    0.0000000000E+000\n"
                                     "       4    0.0000000000E+000    0.0000000000E+000    0.0000000000E+000\n"
                                     "       5    0.0000000000E+000   -2.0000000000E+001    0.0000000000E+000\n"
                                     "TPLOGY       2       1       1       1       1\n"
                                     "* Vertex Data: \n"
                                     "       1       1    0.0000000000E+000    0.0000000000E+000\n"
                                     "       1\n"
                                     "* Edge Data:\n"
                                     "       1       1       1       1\n"
                                     "       1\n"
                                     "* Loop Data:\n"
                                     "       1       1       1\n"
                                     "       1\n"
                                     "       1\n"
                                     "* Face Data:\n"
                                     "       1       1       0\n"
                                     "       1\n"));
    // built once: +0xb8 is 0 afterwards
    CHECK_EQ(static_cast<int>(gk::geometry(o, 0)->ub8), 0);
}

// key/2D/SI/HeatTreat/HT_QUENCH1_SI.KEY object 2: one open type-2 segment (angle 0) without
// TPLOGY; the original builds two vertices at the segment ends. Expected: its KFWRIT_OUT.KEY.
TEST_CASE(built_topology_open_segment_2d)
{
    Model md(2);
    CHECK_EQ(read_key(md, "DIEGEO       2       2       1\n"
                          "       1       1 -1.0000000E+001 -1.7700000E+000  5.0000000E+001  8.8250000E+000  0.0000000E+000\n"),
             0);
    LLOBJ_DATA& o = object(md, 2);
    keytest::legacy_printf();
    OutFile out("kw_g2geo_curve_out.tmp");
    cv::write_curves(md.m(), out.get(), o, 2);
    cv::write_tplogy(md.m(), out.get(), o, 2);
    CHECK_EQ(out.text(),
             std::string("DIEGEO       2       2       1\n"
                         "       1       1   -1.0000000000E+001   -1.7700000000E+000    5.0000000000E+001    8.8250000000E+000    0.0000000000E+000\n"
                         "TPLOGY       2       2       1       1       1\n"
                         "* Vertex Data: \n"
                         "       1       1   -1.0000000000E+001   -1.7700000000E+000\n"
                         "       1\n"
                         "       2       1    5.0000000000E+001    8.8250000000E+000\n"
                         "       1\n"
                         "* Edge Data:\n"
                         "       1       1       1       2\n"
                         "       1\n"
                         "* Loop Data:\n"
                         "       1       1       1\n"
                         "       1\n"
                         "       1\n"
                         "* Face Data:\n"
                         "       1       1       0\n"
                         "       1\n"));
}

// key/2D/EN/HeatTreat/HT_CARB1_EN.KEY object 2: DIEGEO + TPLOGY read and written back unchanged
// (KFWRIT_OUT.KEY lines 2842-2859).
TEST_CASE(tplogy_round_trip_2d)
{
    Model md(2);
    const std::string in =
        "DIEGEO       2       2       1\n"
        "       1       1   -3.9370100000E-001   -6.9685077000E-002    1.9685050000E+000    3.4744113250E-001    0.0000000000E+000\n"
        "TPLOGY       2       2       1       1       1\n"
        "* Vertex Data: \n"
        "       1       1   -3.9370100000E-001   -6.9685077000E-002\n"
        "       1\n"
        "       2       1    1.9685050000E+000    3.4744113250E-001\n"
        "       1\n"
        "* Edge Data:\n"
        "       1       1       1       2\n"
        "       1\n"
        "* Loop Data:\n"
        "       1       1       1\n"
        "       1\n"
        "       1\n"
        "* Face Data:\n"
        "       1       1       0\n"
        "       1\n";
    CHECK_EQ(read_key(md, in), 0);
    LLOBJ_DATA& o = object(md, 2);
    keytest::legacy_printf();
    OutFile out("kw_g2geo_curve_out.tmp");
    cv::write_curves(md.m(), out.get(), o, 2);
    cv::write_tplogy(md.m(), out.get(), o, 2);
    CHECK_EQ(out.text(), in);
}

// Separators (-1 rows) split curves; the type-2 angle is converted deg -> rad on reading and back
// on writing; the header count is the sum of real points. Hand-worked from DIEGEO.md.
TEST_CASE(separators_and_angles_2d)
{
    Model md(2);
    CHECK_EQ(read_key(md, "DIEGEO       1       2       3\n"
                          "  1 1 0.0 0.0 1.0 0.0 0.0\n"
                          "  -1 0 0 0 0 0 0\n"
                          "  2 2 1.0 0.0 0.0 1.0 90.0\n"
                          "  3 1 0.0 1.0 0.0 0.0 0.0\n"),
             0);
    LLOBJ_DATA& o = object(md, 1);
    CHECK_EQ(gk::curve_count(o), 2);
    CHECK_EQ(gk::curve(o, 0)->n_points, 1);
    CHECK_EQ(gk::curve(o, 1)->n_points, 2);
    CHECK_EQ(gk::curve(o, 1)->points[5], 90.0 * 0.017453292519943295);
    keytest::legacy_printf();
    OutFile out("kw_g2geo_curve_out.tmp");
    cv::write_curves(md.m(), out.get(), o, 1);
    CHECK_EQ(out.text(),
             std::string("DIEGEO       1       2       3\n"
                         "       1       1    0.0000000000E+000    0.0000000000E+000    1.0000000000E+000    0.0000000000E+000    0.0000000000E+000\n"
                         "      -1       0    0.0000000000E+000    0.0000000000E+000    0.0000000000E+000    0.0000000000E+000    0.0000000000E+000\n"
                         "       2       2    1.0000000000E+000    0.0000000000E+000    0.0000000000E+000    1.0000000000E+000    9.0000000000E+001\n"
                         "       3       1    0.0000000000E+000    1.0000000000E+000    0.0000000000E+000    0.0000000000E+000    0.0000000000E+000\n"));
    // two curves and no TPLOGY read: the TPLOGY writer returns 3 and writes nothing
    OutFile out2("kw_g2geo_curve_out2.tmp");
    CHECK_EQ(cv::write_tplogy(md.m(), out2.get(), o, 1), 3);
    CHECK_EQ(out2.text(), std::string(""));
}

// 140304e40: a type-1 curve with a single point is dropped before writing (the header total is
// taken after the removal).
TEST_CASE(degenerate_curve_removed)
{
    Model md(2);
    CHECK_EQ(read_key(md, "DIEGEO       1       1       3\n"
                          " 1 5.0 6.0 0.0\n"
                          " -1 0 0 0\n"
                          " 2 1.0 2.0 0.0\n"
                          " 3 3.0 4.0 0.0\n"),
             0);
    LLOBJ_DATA& o = object(md, 1);
    CHECK_EQ(gk::curve_count(o), 2);
    keytest::legacy_printf();
    OutFile out("kw_g2geo_curve_out.tmp");
    cv::write_curves(md.m(), out.get(), o, 1);
    CHECK_EQ(gk::curve_count(o), 1);
    CHECK_EQ(out.text(), std::string("DIEGEO       1       1       2\n"
                                     "       1    1.0000000000E+000    2.0000000000E+000    0.0000000000E+000\n"
                                     "       2    3.0000000000E+000    4.0000000000E+000    0.0000000000E+000\n"));
}

// Return codes of the cases (parser maps every failure to 2).
TEST_CASE(read_errors)
{
    Model md2(2);
    CHECK_EQ(read_key(md2, "DIEGEO       1       4       1\n 1 0 0 0\n"), 2);  // type not 1/2
    CHECK_EQ(read_key(md2, "DIEGEO       1       1       2\n 1 0 0 0\n"), 2);  // EOF in the points
    CHECK_EQ(read_key(md2, "DIEGEO       1       1       1\n 1 x 0 0\n"), 2);  // no conversion (3)
    Model md3(3);
    CHECK_EQ(read_key(md3, "DIEGEO       1       3       4       4\n"), 2);    // nhdr >= total
    CHECK_EQ(read_key(md3, "DIEGEO       1       5       2\n"), 2);            // curve form, bad type
    // 3D curve form: the point reader's status is ignored
    CHECK_EQ(read_key(md3, "DIEGEO       1       1       2\n 1 0 0 0\n"), 0);
    CHECK_EQ(read_key(md3, "CSPMTX       1       1\n 1 1 0 0 0\n 2 0 1 0 0\n"), 2);  // EOF in the rows
    CHECK_EQ(read_key(md3, "TPLOGY       1       1       1       1       1\n"), 2);   // EOF at the comment
}

int main() { return testkit::run_all(); }
