// ctest-labels: kw-g2geo group2 surface
// Tier-1 tests of the kw-g2geo surface family: the DIEGEO 3D surface form (read_surface), POLYTP,
// DIEPNT, DIEEDG, FLDGEO, FLWNET and their writers (1402a26c0, 1402a27a0, 1402a2b70). Expected
// text is worked out by hand from the decompiled format strings ("%-6s %7i ...": name padded to
// 6, ints right-aligned in 7, reals "%20.10E" with the old CRT's three-digit exponent) and from
// the specs (docs/re/keywords/<KW>.md). The last test reads object 2 of the corpus file
// 3D\SI\Extrusion\Lagrangian\Extrude_T_T_LAG_Cold.KEY and compares the writer's output with the
// original's KFWRIT_OUT.KEY from the oracle cache (skipped when either is missing).
#define _CRT_SECURE_NO_WARNINGS
#include "key_test_util.hpp"

#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include "../src/kw_g2geo/internal.hpp"

#include <defkey/kw_g2obj.hpp>
#include <defkey/line_io.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/surface_model.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <testkit/check.hpp>

namespace fs = std::filesystem;
namespace gk = def::model::geo_kw;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;
using def::model::OBJ_GEO3D;
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

constexpr int kObjtyp = 145, kPOLYTP = 299, kDIEPNT = 300, kDIEEDG = 301, kFLDGEO = 289,
              kFLWNET = 290;

int read(Model& md, int id, const std::string& text)
{
    InFile in("kw_g2geo_surface_in.tmp", text);
    return def::key::g2obj::read_object_keyword(md.m(), in.get(), id);
}

LLOBJ_DATA& object(Model& md, int no)
{
    char buf[32];
    std::snprintf(buf, sizeof buf, " %d 1 0\n", no);
    read(md, kObjtyp, buf);
    return *def::key::g2obj::find_object(md.m(), no);
}

int read_surf(Model& md, LLOBJ_DATA& obj, int a, int nn, int ne, const std::string& text)
{
    InFile in("kw_g2geo_surface_in.tmp", text);
    return def::key::g2geo::surface::read_surface(md.m(), in.get(), obj, a, nn, ne);
}

// node line " %7i %20.10E %20.10E %20.10E"
const char* kNodes4 =
    "       1    0.0000000000E+000    0.0000000000E+000    0.0000000000E+000\n"
    "       2    1.5000000000E+000    0.0000000000E+000    0.0000000000E+000\n"
    "       3    1.5000000000E+000   -2.0000000000E+000    0.0000000000E+000\n"
    "       4    0.0000000000E+000    0.0000000000E+000    3.0000000000E+000\n";

}  // namespace

TEST_CASE(surface_polytp_diepnt_dieedg_round_trip)
{
    Model md(3);
    LLOBJ_DATA& obj = object(md, 1);
    CHECK_EQ(read_surf(md, obj, 1, 4, 2,
                       " 1 0 0 0\n 2 1.5 0 0\n 3 1.5 -2 0\n 4 0 0 3\n 1 1 2 3 1\n 2 1 3 4 -1\n"),
             0);
    CHECK_EQ(gk::surface_count(obj), 1);
    CHECK(gk::has_surface_flag(obj));
    CHECK_EQ(gk::surface(obj, 0)->node_count(), 4);
    CHECK_EQ(gk::surface(obj, 0)->elem_count(), 2);

    // POLYTP: n = 2 lines, default 0; values 1013 / 1011 are patch ids 13 / 11
    CHECK_EQ(read(md, kPOLYTP, " 1 2 0\n       1    1013\n       2    1011\n"), 0);
    OBJ_GEO3D* s = gk::surface(obj, 0);
    CHECK_EQ(s->elem_ids[0], 1013);
    CHECK_EQ(s->elem_ids[1], 1011);
    CHECK_EQ(gk::geo3d_patch_count(s), 2);
    CHECK_EQ(gk::geo3d_patch_at(s, 0), 13);
    CHECK_EQ(gk::geo3d_patch_at(s, 1), 11);

    // DIEPNT: set 5 (2 points), set 2 (empty: not written), set 9 type 2 (extra written as 0)
    CHECK_EQ(read(md, kDIEPNT, " 1 5 1 2 3\n 1 1.0 2.0 3.0 1.0 0.3\n 2 -1.0 0.5 0 1 0.3\n"), 0);
    CHECK_EQ(read(md, kDIEPNT, " 1 2 1 0 7\n"), 0);
    CHECK_EQ(read(md, kDIEPNT, " 1 9 2 1 4\n 1 4 5 6 7 8\n"), 0);
    CHECK_EQ(read(md, kDIEPNT, " 1 9 2 1 4\n"), 2);  // point line missing: 2, the set is cleared
    CHECK_EQ(read(md, kDIEPNT, " 1 9 2 1 4\n 1 4 5 6 7 8\n"), 0);
    CHECK_EQ(static_cast<int>(gk::die_point_sets(s).size()), 3);

    // DIEEDG: type 2 (12 values over two lines), type 1 with extra, a set retyped to 5 (status 2,
    // its values stay: written as a DIEPNT line), a new type-3 set (2, empty: not written)
    CHECK_EQ(read(md, kDIEEDG, " 1 1 2 12\n 10 11 12 13 14 15 16 17 18 19\n 20 21\n"), 0);
    CHECK_EQ(read(md, kDIEEDG, " 1 4 1 2 77\n 5 6\n"), 0);
    CHECK_EQ(read(md, kDIEEDG, " 1 7 2 1\n 8\n"), 0);
    CHECK_EQ(read(md, kDIEEDG, " 1 7 5 0\n"), 2);
    CHECK_EQ(read(md, kDIEEDG, " 1 8 3 0\n"), 2);
    CHECK_EQ(read(md, kDIEEDG, " 1 9 2 3\n 1 2\n"), 2);  // EOF in the values
    CHECK_EQ(static_cast<int>(gk::die_edge_sets(s).size()), 5);

    keytest::legacy_printf();
    OutFile out("kw_g2geo_surface_out.tmp");
    CHECK_EQ(def::key::g2geo::surface::write_geometry_dispatch(md.m(), out.get(), obj, 1), 0);
    std::string exp = std::string("DIEGEO       1       1       4       2\n") + kNodes4 +
                      "       1       1       2       3       1\n"
                      "       2       1       3       4      -1\n"
                      "POLYTP       1       2       0\n"
                      "       1    1013\n"
                      "       2    1011\n"
                      "DIEPNT       1       1       1       2       3\n"
                      "       1    1.0000000000E+000    2.0000000000E+000    3.0000000000E+000    1.0000000000E+000    3.0000000000E-001\n"
                      "       2   -1.0000000000E+000    5.0000000000E-001    0.0000000000E+000    1.0000000000E+000    3.0000000000E-001\n"
                      "DIEPNT       1       2       2       1       0\n"
                      "       1    4.0000000000E+000    5.0000000000E+000    6.0000000000E+000    7.0000000000E+000    8.0000000000E+000\n"
                      "DIEEDG       1       1       2      12\n"
                      "      10      11      12      13      14      15      16      17      18      19\n"
                      "      20      21\n"
                      "DIEEDG       1       2       1       2      77\n"
                      "       5       6\n"
                      "DIEPNT       1       3       5       0\n"
                      "DIEEDG       1       4       2       2\n"
                      "       1       2\n";
    CHECK_EQ(out.text(), exp);
}

TEST_CASE(polytp_uniform_and_2d)
{
    Model md(3);
    LLOBJ_DATA& obj = object(md, 1);
    CHECK_EQ(read_surf(md, obj, 2, 4, 2, " 1 0 0 0\n 2 1.5 0 0\n 3 1.5 -2 0\n 4 0 0 3\n"), 0);
    CHECK_EQ(gk::surface(obj, 0)->elems[7], 0);  // a != 1: no element lines read
    CHECK_EQ(read(md, kPOLYTP, " 1 0 1005\n"), 0);
    CHECK_EQ(gk::geo3d_patch_count(gk::surface(obj, 0)), 1);
    keytest::legacy_printf();
    OutFile out("kw_g2geo_surface_out.tmp");
    CHECK_EQ(def::key::g2geo::surface::write_surface(md.m(), out.get(), obj, 3), 0);
    CHECK_EQ(out.text(), std::string("DIEGEO       3       1       4       2\n") + kNodes4 +
                             "       1       0       0       0       0\n"
                             "       2       0       0       0       0\n"
                             "POLYTP       3       0    1005\n");

    // 2D: the 3D-only cases return 0 and leave the rest of the line
    Model m2(2);
    object(m2, 1);
    for (int id : {kPOLYTP, kDIEPNT, kDIEEDG}) {
        InFile in("kw_g2geo_surface_in.tmp", " 1 2 0\n");
        CHECK_EQ(def::key::g2obj::read_object_keyword(m2.m(), in.get(), id), 0);
        CHECK_EQ(in.rest(), std::string(" 2 0\n"));
    }
}

TEST_CASE(fldgeo_flwnet)
{
    Model md(3);
    LLOBJ_DATA& obj = object(md, 1);
    const std::string body = " 1 0 0 0\n 2 1.5 0 0\n 3 1.5 -2 0\n 4 0 0 3\n 1 1 2 3 -3\n";
    CHECK_EQ(read(md, kFLDGEO, " 1 0 4 1\n" + body), 0);
    CHECK_EQ(read(md, kFLWNET, " 1 0 4 1\n" + body), 0);
    // a short node line gives 0s for the missing values; the next node line hits EOF -> 2
    CHECK_EQ(read(md, kFLWNET, " 1 0 4 1\n 1 0 0\n"), 2);
    CHECK_EQ(read(md, kFLDGEO, " 1 0 4 1\n 1 0 0 0\n"), 2);  // EOF in the nodes
    CHECK(gk::has_fldgeo(obj));
    CHECK(gk::has_flwnet(obj));
    keytest::legacy_printf();
    OutFile out("kw_g2geo_surface_out.tmp");
    CHECK_EQ(def::key::g2geo::surface::write_surface_mesh(md.m(), out.get(), "FLDGEO",
                                                          &at<std::byte>(obj, 0x3328), 2),
             0);
    // the failed FLDGEO read re-sized to the same sizes (values kept: 14019a740 inserts nothing)
    // and read node 1 again; node 2 was zeroed by the failing reader (outputs are set to 0 before
    // the fgets); the negative 4th node of the first read was made positive
    CHECK_EQ(out.text(), std::string("FLDGEO       2       1       4       1\n") +
                             "       1    0.0000000000E+000    0.0000000000E+000    0.0000000000E+000\n"
                             "       2    0.0000000000E+000    0.0000000000E+000    0.0000000000E+000\n"
                             "       3    1.5000000000E+000   -2.0000000000E+000    0.0000000000E+000\n"
                             "       4    0.0000000000E+000    0.0000000000E+000    3.0000000000E+000\n"
                             "       1       1       2       3       3\n");
    // no mesh -> 3
    Model m2(3);
    LLOBJ_DATA& o2 = object(m2, 1);
    OutFile out2("kw_g2geo_surface_out2.tmp");
    CHECK_EQ(def::key::g2geo::surface::write_surface_mesh(m2.m(), out2.get(), "FLDGEO",
                                                          &at<std::byte>(o2, 0x3328), 1),
             3);
    CHECK_EQ(out2.text(), std::string());
}

namespace {

std::string slurp(const fs::path& p)
{
    std::ifstream in(p, std::ios::binary);
    std::ostringstream s;
    s << in.rdbuf();
    return s.str();
}

std::string strip_cr(const std::string& s)
{
    std::string r;
    for (char c : s)
        if (c != '\r') r += c;
    return r;
}

// the text from the line starting with `from` up to (not including) the line starting with `to`
std::string section(const std::string& t, const std::string& from, const std::string& to)
{
    std::size_t a = t.find("\n" + from);
    if (a == std::string::npos) return {};
    std::size_t b = t.find("\n" + to, a + 1);
    if (b == std::string::npos) return {};
    return t.substr(a + 1, b - a);
}

fs::path oracle_file(const std::string& label)
{
    const char* up = std::getenv("USERPROFILE");
    if (up == nullptr) return {};
    fs::path root = fs::path(up) / ".deform-oracle" / "entries";
    std::error_code ec;
    if (!fs::exists(root, ec)) return {};
    for (auto& d1 : fs::directory_iterator(root, ec))
        for (auto& d2 : fs::directory_iterator(d1.path(), ec)) {
            fs::path r = d2.path() / "result.json";
            if (!fs::exists(r, ec)) continue;
            if (slurp(r).find("\"" + label + "\"") != std::string::npos) return d2.path() / "files" / "KFWRIT_OUT.KEY";
        }
    return {};
}

}  // namespace

TEST_CASE(corpus_extrude_object2_matches_oracle)
{
    const fs::path in_path =
        "C:/Users/alext/Projects/DEFORM_corpus/key/3D/SI/Extrusion/Lagrangian/Extrude_T_T_LAG_Cold.KEY";
    const fs::path or_path = oracle_file("kfrw/key/3D/SI/Extrusion/Lagrangian/Extrude_T_T_LAG_Cold.KEY");
    std::error_code ec;
    if (!fs::exists(in_path, ec) || or_path.empty() || !fs::exists(or_path, ec)) {
        std::printf("note: corpus file or oracle output missing, skipped\n");
        return;
    }
    const std::string in_sec = strip_cr(section(slurp(in_path), "DIEGEO       2", "RZ           2"));
    const std::string exp = strip_cr(section(slurp(or_path), "DIEGEO       2", "RZ           2"));
    REQUIRE(!in_sec.empty() && !exp.empty());

    Model md(3);
    LLOBJ_DATA& obj = object(md, 2);
    InFile in("kw_g2geo_surface_in.tmp", in_sec);
    std::FILE* f = in.get();
    char tok[40];
    int bad = 0;
    while (std::fscanf(f, "%39s", tok) == 1) {
        const std::string k = tok;
        int st;
        if (k == "DIEGEO") {
            std::int32_t o = 0, a = 0, nn = 0, ne = 0;
            st = def::key::read_i4(f, &o, &a, &nn, &ne);
            if (st == 0) st = def::key::g2geo::surface::read_surface(md.m(), f, obj, a, nn, ne);
        } else if (k == "POLYTP") {
            st = def::key::g2obj::read_object_keyword(md.m(), f, kPOLYTP);
        } else if (k == "DIEPNT") {
            st = def::key::g2obj::read_object_keyword(md.m(), f, kDIEPNT);
        } else if (k == "DIEEDG") {
            st = def::key::g2obj::read_object_keyword(md.m(), f, kDIEEDG);
        } else {
            st = 9;
        }
        if (st != 0) ++bad;
    }
    CHECK_EQ(bad, 0);
    keytest::legacy_printf();
    OutFile out("kw_g2geo_surface_out.tmp");
    def::key::g2geo::surface::write_surface(md.m(), out.get(), obj, 2);
    const std::string got = out.text();
    CHECK_EQ(got.size(), exp.size());
    CHECK(got == exp);
}

int main() { return testkit::run_all(); }
