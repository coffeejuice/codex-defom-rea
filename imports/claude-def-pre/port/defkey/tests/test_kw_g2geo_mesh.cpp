// ctest-labels: kw-g2geo group2 mesh
// Tier-1 tests of the kw-g2geo "mesh" family: RZ, ELMCON, MTLGRP, DRZ, URZ, FRZ, PRZ, DRMESH
// (parser cases of 14038b8a0), the RZ / ELMCON writers 1402a2db0 / 1402a30c0 and the material
// group recomputation 140202960.
//
// Expected texts: the KEY records of the original's KFWRIT round trips of
// 2D\EN\plane stress\Ela_PlaneStress_EN.KEY and 3D\EN\NAFEMS_Benchmarks\NAFEMS_T2.KEY (copied
// from the oracle outputs), and hand-derived values from the decompile (formats "%-6s %7i %7i",
// " %7i %20.10E ...", stores per RZ.md / ELMCON.md / MTLGRP.md). When the oracle cache and the
// corpus are present, the full RZ / ELMCON blocks of those two files are also compared.
//
// INTERIM fixture (no @src): objects come from kw-g2obj's zero-filling object shim, so the node /
// element state-variable sets at obj+0x8a0 / obj+0x9e0 are not constructed. The test stands in
// for the p1-09 state variables through llobj::ext::g_hook (140329e60: calloc of n x dims[id]
// doubles for the slots used here; 140329dc0: free), and fills the component table
// DB_STEP_DATA+0x15f8 entries these keywords read as 1401d09c0 does. The model byte +0x1b5c is set
// so that ELMCON skips the boundary build 1401fc290 (boundary family) except where noted.
#define _CRT_SECURE_NO_WARNINGS
#include "key_test_util.hpp"

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <defkey/kw_g2geo.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defmodel/geo_kw/mesh_model.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/cls_140182880.hpp>
#include <defmodel/llobj/cls_14017ef20.hpp>
#include <defmodel/llobj/ext.hpp>
#include <defrt/ftn/units.hpp>
#include <testkit/check.hpp>

namespace fs = std::filesystem;
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

constexpr int kRZ = 159, kDRZ = 160, kURZ = 165, kFRZ = 166, kPRZ = 167, kELMCON = 173,
              kMTLGRP = 174, kDRMESH = 263, kObjtyp = 145;

// ---- INTERIM state-variable stand-ins (p1-09) ----
struct Slot {
    std::uint32_t off;
    int id;
};
// node set obj+0x8a0: DRZ 0x10 (id 1), URZ 0x18 (2), FRZ 0x20 (3), PRZ 0x30 (5), temperature 0x40
// (8), DRMESH 0x90 (0x19); element set obj+0x9e0: 0x50 (0x2c), 0x58 (0x2d)
const std::vector<Slot> kNodeSlots = {{0x10, 1}, {0x18, 2}, {0x20, 3}, {0x30, 5}, {0x40, 8}, {0x90, 0x19}};
const std::vector<Slot> kElemSlots = {{0x50, 0x2c}, {0x58, 0x2d}};
std::map<std::uintptr_t, bool> g_sets;  // set address -> node set

void free_slots(unsigned char* set, bool node)
{
    for (const Slot& s : node ? kNodeSlots : kElemSlots) {
        double*& p = *reinterpret_cast<double**>(set + s.off);
        std::free(p);
        p = nullptr;
    }
}

std::uint64_t statevar_hook(std::uint64_t addr, const std::uint64_t* a, int)
{
    auto it = g_sets.find(static_cast<std::uintptr_t>(a[0]));
    if (it == g_sets.end()) return 0;
    auto* set = reinterpret_cast<unsigned char*>(a[0]);
    if (addr == 0x140329dc0) {
        free_slots(set, it->second);
    } else if (addr == 0x140329e60) {
        const auto n = static_cast<std::int32_t>(a[1]);
        const auto* dims = reinterpret_cast<const std::int32_t*>(a[2]);
        free_slots(set, it->second);
        for (const Slot& s : it->second ? kNodeSlots : kElemSlots) {
            const std::int32_t cnt = n * dims[s.id];
            *reinterpret_cast<double**>(set + s.off) = cnt > 0 ? static_cast<double*>(std::calloc(cnt, 8)) : nullptr;
        }
    }
    return 0;
}

// Zero-filled model with the dimension, the component table entries of 1401d09c0 these keywords
// read (all 1, entries 0..6, 0x19 = dim) and +0x1b5c set (no boundary build).
struct Model {
    void* raw;
    explicit Model(int dim)
    {
        raw = new DB_STEP_DATA(dim);  // the real model ctor (p1-03): component table, state variables
        at<std::uint8_t>(m(), 0x1b5c) = 1;
        // object 1 (OBJTYP 1 2 0: type 1)
        InFile in("kw_g2geo_mesh_obj.tmp", " 1 2 0\n");
        def::key::g2obj::read_object_keyword(m(), in.get(), kObjtyp);
        LLOBJ_DATA* o = obj();
        g_sets[reinterpret_cast<std::uintptr_t>(&at<unsigned char>(*o, 0x8a0))] = true;
        g_sets[reinterpret_cast<std::uintptr_t>(&at<unsigned char>(*o, 0x9e0))] = false;
    }
    // the model is leaked on purpose with its objects
    DB_STEP_DATA& m() { return *static_cast<DB_STEP_DATA*>(raw); }
    LLOBJ_DATA* obj() { return def::key::g2obj::find_object(m(), 1); }
};

int read(Model& md, int id, const std::string& text, std::string* rest = nullptr)
{
    InFile in("kw_g2geo_mesh_in.tmp", text);
    int st = def::key::g2obj::read_object_keyword(md.m(), in.get(), id);
    if (rest != nullptr) *rest = in.rest();
    return st;
}

std::string write_rz(Model& md)
{
    keytest::legacy_printf();
    OutFile out("kw_g2geo_mesh_out.tmp");
    def::key::g2geo::write_nodes(md.m(), out.get(), *md.obj(), 1);
    return out.text();
}

std::string write_elm(Model& md)
{
    keytest::legacy_printf();
    OutFile out("kw_g2geo_mesh_out.tmp");
    def::key::g2geo::write_elements(md.m(), out.get(), *md.obj(), 1);
    return out.text();
}

// Ela_PlaneStress_EN.KEY lines 140-149 / 170-174 (2D)
const char* kRz2d =
    " 1       9\n"
    "       1  0.0000000E+000  6.0000000E+000\n"
    "       2  0.0000000E+000  0.0000000E+000\n"
    "       3  0.0000000E+000  3.0000488E+000\n"
    "       4  5.0000000E+000  6.0000000E+000\n"
    "       5  2.5010449E+000  6.0000000E+000\n"
    "       6  2.5003933E+000  3.0001792E+000\n"
    "       7  2.5005283E+000  0.0000000E+000\n"
    "       8  5.0000000E+000  3.0006680E+000\n"
    "       9  5.0000000E+000  0.0000000E+000\n";
const char* kElm2d =
    " 1       4\n"
    "       1       6       3       2       7\n"
    "       2       5       1       3       6\n"
    "       3       8       6       7       9\n"
    "       4       4       5       6       8\n";

} // namespace

TEST_CASE(rz_elmcon_2d_round_trip)
{
    Model md(2);
    def::rt::ftn::g_units.unit_flag = 0;  // unit system 1: initial temperature 20.0
    CHECK_EQ(read(md, kRZ, kRz2d), 0);
    LLOBJ_DATA& o = *md.obj();
    CHECK_EQ(at<std::int32_t>(o, 0x880), 9);
    CHECK_EQ(at<std::int32_t>(o, 0x87c), 0);  // RZ does not set the mesh flag
    CHECK_BITS(at<double*>(o, 0x8a8)[4 * 2], 2.5010449);
    CHECK_BITS(at<double*>(o, 0x8e0)[8], 20.0);
    CHECK_EQ(write_rz(md), std::string());  // no mesh yet: nothing written
    CHECK_EQ(read(md, kELMCON, kElm2d), 0);
    CHECK_EQ(at<std::int32_t>(o, 0x87c), 1);
    CHECK_EQ(at<std::int32_t>(o, 0x884), 4);
    CHECK_EQ(at<std::int32_t*>(o, 0x9e8)[3 * 4 + 3], 8);
    CHECK_BITS(at<double*>(o, 0xa30)[3], 1.0);  // 1401653b0 defaults
    CHECK_EQ(write_rz(md),
             std::string("RZ           1       9\n"
                         "       1    0.0000000000E+000    6.0000000000E+000\n"
                         "       2    0.0000000000E+000    0.0000000000E+000\n"
                         "       3    0.0000000000E+000    3.0000488000E+000\n"
                         "       4    5.0000000000E+000    6.0000000000E+000\n"
                         "       5    2.5010449000E+000    6.0000000000E+000\n"
                         "       6    2.5003933000E+000    3.0001792000E+000\n"
                         "       7    2.5005283000E+000    0.0000000000E+000\n"
                         "       8    5.0000000000E+000    3.0006680000E+000\n"
                         "       9    5.0000000000E+000    0.0000000000E+000\n"));
    CHECK_EQ(write_elm(md),
             std::string("ELMCON       1       4\n"
                         "       1       6       3       2       7\n"
                         "       2       5       1       3       6\n"
                         "       3       8       6       7       9\n"
                         "       4       4       5       6       8\n"));
    // object dimension 3 in a 2D model: the 3D layouts. RZ reads the 2D array with stride 3 (the
    // node count is lowered to 6 here so that 6 x 3 stays inside the 9 x 2 array); ELMCON takes
    // the typed writer, which writes nothing for +0x890 = 0.
    at<std::int32_t>(o, 0x39c0) = 3;
    at<std::int32_t>(o, 0x880) = 6;
    const std::string rz3 = write_rz(md);
    CHECK_EQ(rz3.substr(0, rz3.find('\n', 23) + 1),
             std::string("RZ           1       6\n"
                         "       1    0.0000000000E+000    6.0000000000E+000    0.0000000000E+000\n"));
    at<std::int32_t>(o, 0x880) = 9;
    CHECK_EQ(write_elm(md), std::string());
}

TEST_CASE(initial_temperature_by_unit_system)
{
    Model md(2);
    def::rt::ftn::g_units.unit_flag = 1;  // unit system 2: 68.0
    CHECK_EQ(read(md, kRZ, " 1 2\n1 0 0\n2 1 1\n"), 0);
    CHECK_BITS(at<double*>(*md.obj(), 0x8e0)[0], 68.0);
    CHECK_BITS(at<double*>(*md.obj(), 0x8e0)[1], 68.0);
    def::rt::ftn::g_units.unit_flag = 0;
}

TEST_CASE(nodal_vectors_2d)
{
    Model md(2);
    CHECK_EQ(read(md, kRZ, kRz2d), 0);
    LLOBJ_DATA& o = *md.obj();
    // Ela_PlaneStress_EN.KEY lines 150, 160-163, 164
    CHECK_EQ(read(md, kDRZ, " 1       0  0.0000000E+000\n"), 0);
    CHECK_EQ(read(md, kURZ,
                  " 1       3  0.0000000E+000\n"
                  "       1  0.0000000E+000  1.0000000E-001\n"
                  "       4  0.0000000E+000  1.0000000E-001\n"
                  "       5  0.0000000E+000  1.0000000E-001\n"),
             0);
    CHECK_BITS(at<double*>(o, 0x8b8)[1], 0.1);
    CHECK_BITS(at<double*>(o, 0x8b8)[3 * 2 + 1], 0.1);
    CHECK_BITS(at<double*>(o, 0x8b8)[1 * 2 + 1], 0.0);
    CHECK_EQ(read(md, kFRZ, " 1 0 2.5\n"), 0);  // mode 0: the default fills every component
    CHECK_BITS(at<double*>(o, 0x8c0)[17], 2.5);
    CHECK_EQ(read(md, kPRZ, " 1 1\n 9 1.5 -2.0\n"), 0);
    CHECK_BITS(at<double*>(o, 0x8d0)[16], 1.5);
    CHECK_BITS(at<double*>(o, 0x8d0)[17], -2.0);
    CHECK_EQ(read(md, kDRZ, " 1\n"), 2);  // blank header
    // DRMESH in 2D: nothing consumed, status 0
    std::string rest;
    CHECK_EQ(read(md, kDRMESH, " 1 0 1.0\n", &rest), 0);
    CHECK_EQ(rest, std::string(" 0 1.0\n"));
}

TEST_CASE(rz_elmcon_3d_bricks_and_drmesh)
{
    Model md(3);
    // NAFEMS_T2.KEY: RZ line 139 + 3 nodes, ELMCON line 245 + 3 elements (counts reduced)
    CHECK_EQ(read(md, kRZ,
                  " 1       3\n"
                  "       1    0.0000000000E+000    0.0000000000E+000    0.0000000000E+000\n"
                  "       2   -2.4107104261E-017    1.4761264915E-033    3.9370100000E-001\n"
                  "       3    2.4107104261E-017    3.9370100000E-001    0.0000000000E+000\n"),
             0);
    CHECK_EQ(read(md, kELMCON,
                  " 1       3       8\n"
                  "       1       1       3       4       2       5       7       8       6\n"
                  "       2       5       7       8       6       9      11      12      10\n"
                  "       3       9      11      12      10      13      15      16      14\n"),
             0);
    LLOBJ_DATA& o = *md.obj();
    CHECK_EQ(at<std::int32_t>(o, 0x890), 4);
    CHECK_EQ(at<std::int32_t>(o, 0x894), 8);
    CHECK_EQ(at<std::int32_t>(o, 0x898), 6);
    CHECK_EQ(at<std::int32_t>(o, 0x89c), 4);
    CHECK_EQ(write_rz(md),
             std::string("RZ           1       3\n"
                         "       1    0.0000000000E+000    0.0000000000E+000    0.0000000000E+000\n"
                         "       2   -2.4107104261E-017    1.4761264915E-033    3.9370100000E-001\n"
                         "       3    2.4107104261E-017    3.9370100000E-001    0.0000000000E+000\n"));
    CHECK_EQ(write_elm(md),
             std::string("ELMCON       1       3       8\n"
                         "       1       1       3       4       2       5       7       8       6\n"
                         "       2       5       7       8       6       9      11      12      10\n"
                         "       3       9      11      12      10      13      15      16      14\n"));
    // DRMESH (NAFEMS_T2.KEY line 185) and a record: m = model+0x165c = 3
    CHECK_EQ(read(md, kDRMESH, " 1       1    0.0000000000E+000\n 2 1 2 3\n"), 0);
    CHECK_BITS(at<double*>(o, 0x930)[3], 1.0);
    CHECK_BITS(at<double*>(o, 0x930)[5], 3.0);
    CHECK_BITS(at<double*>(o, 0x930)[8], 0.0);
}

TEST_CASE(elmcon_3d_tets_and_errors)
{
    Model md(3);
    CHECK_EQ(read(md, kELMCON, " 1 1 6\n"), 2);  // npe neither 4 nor 8
    CHECK_EQ(at<std::int32_t>(*md.obj(), 0x884), 0);
    CHECK_EQ(read(md, kELMCON, " 1 2 4\n 1 1 2 3 4\n 2 2,3,4,5\n"), 0);
    LLOBJ_DATA& o = *md.obj();
    CHECK_EQ(at<std::int32_t>(o, 0x890), 2);
    CHECK_EQ(at<std::int32_t>(o, 0x894), 4);
    CHECK_EQ(at<std::int32_t>(o, 0x898), 4);
    CHECK_EQ(at<std::int32_t>(o, 0x89c), 3);
    CHECK_EQ(write_elm(md), std::string("ELMCON       1       2       4\n"
                                        "       1       1       2       3       4\n"
                                        "       2       2       3       4       5\n"));
    // RZ: no nodes yet -> nothing written even with the mesh flag
    CHECK_EQ(write_rz(md), std::string());
    // a blank element line is an error; the count stays, the mesh flag is not set again
    at<std::int32_t>(o, 0x87c) = 0;
    CHECK_EQ(read(md, kELMCON, " 1 2 4\n 1 1 2 3 4\n\n"), 2);
    CHECK_EQ(at<std::int32_t>(o, 0x884), 2);
    CHECK_EQ(at<std::int32_t>(o, 0x87c), 0);
    // RZ record EOF
    CHECK_EQ(read(md, kRZ, " 1 2\n 1 0 0 0\n"), 2);
    CHECK_EQ(at<std::int32_t>(o, 0x880), 2);
}

TEST_CASE(mtlgrp_most_frequent_group)
{
    Model md(2);
    CHECK_EQ(read(md, kRZ, " 1 4\n1 0 0\n2 1 0\n3 1 1\n4 0 1\n"), 0);
    CHECK_EQ(read(md, kELMCON, " 1 3\n1 1 2 3 4\n2 1 2 3 4\n3 1 2 3 4\n"), 0);
    LLOBJ_DATA& o = *md.obj();
    // uniform form (Ela_PlaneStress_EN.KEY line 175): group 1
    CHECK_EQ(read(md, kMTLGRP, " 1       0       1\n"), 0);
    CHECK_EQ(at<std::int32_t>(o, 0x3980), 1);
    // groups 3, 2, 3: the most frequent is 3
    CHECK_EQ(read(md, kMTLGRP, " 1 1 3\n 2 2\n"), 0);
    CHECK_EQ(at<std::int32_t*>(o, 0x9f8)[1], 2);
    CHECK_EQ(at<std::int32_t>(o, 0x3980), 3);
    // tie 2 / 5 (+ 5): first seen wins only on ties -> groups 2, 5, 5 -> 5
    CHECK_EQ(read(md, kMTLGRP, " 1 3\n1 2\n2 5\n3 5\n"), 0);
    CHECK_EQ(at<std::int32_t>(o, 0x3980), 5);
    // tie: 4, 6, 0 -> each once, first (4) wins
    CHECK_EQ(read(md, kMTLGRP, " 1 3\n1 4\n2 6\n3 0\n"), 0);
    CHECK_EQ(at<std::int32_t>(o, 0x3980), 4);
    // all 0: the most frequent group is 0, the old value stays
    CHECK_EQ(read(md, kMTLGRP, " 1 0 0\n"), 0);
    CHECK_EQ(at<std::int32_t>(o, 0x3980), 4);
    // a new ELMCON pre-fills the groups with obj+0x3980
    CHECK_EQ(read(md, kELMCON, " 1 2\n1 1 2 3 4\n2 1 2 3 4\n"), 0);
    CHECK_EQ(at<std::int32_t*>(o, 0x9f8)[1], 4);
    // 1401fb270 with obj+0x3980 >= 1: distinct count
    std::int32_t cnt = -1, grp = -1;
    at<std::int32_t*>(o, 0x9f8)[1] = 7;
    def::model::geo_kw::material_groups(o, &cnt, &grp);
    CHECK_EQ(cnt, 2);
    CHECK_EQ(grp, 4);
    // header EOF -> 2
    CHECK_EQ(read(md, kMTLGRP, ""), 2);
}

TEST_CASE(write_handlers_route_to_state_var_writer)
{
    Model md(2);
    CHECK(def::key::g2geo::handles_write(kDRZ) && def::key::g2geo::handles_write(kURZ) &&
          def::key::g2geo::handles_write(kFRZ) && def::key::g2geo::handles_write(kPRZ));
    CHECK(!def::key::g2geo::handles_write(kRZ) && !def::key::g2geo::handles_write(kDRMESH));
    OutFile out("kw_g2geo_mesh_out.tmp");
    def::key::g2geo::KeyRef ref{1};
    CHECK_EQ(def::key::g2geo::write_keyword(kDRZ, out.get(), md.m(), ref), 0);
    CHECK_EQ(def::key::g2geo::write_nodes(md.m(), nullptr, *md.obj(), 1), 3);
    CHECK_EQ(def::key::g2geo::write_elements(md.m(), nullptr, *md.obj(), 1), 3);
}

// The same reads with the real node / element state-variable sets (Cls_14017ef20 at +0x8a0,
// Cls_140182880 at +0x9e0, p1-04/p1-09, wired at the Phase 1 gate) constructed in the shim
// object, without the test hook: what the object shim needs before the KEY loop can route RZ /
// ELMCON here.
TEST_CASE(real_state_variable_sets)
{
    Model md(3);
    def::model::llobj::ext::g_hook = nullptr;
    LLOBJ_DATA& o = *md.obj();
    new (&at<unsigned char>(o, 0x8a0)) def::model::Cls_14017ef20();
    new (&at<unsigned char>(o, 0x9e0)) def::model::Cls_140182880();
    CHECK_EQ(read(md, kRZ, " 1 2\n1 1 2 3\n2 4 5 6\n"), 0);
    CHECK_EQ(read(md, kDRZ, " 1 1 0.0\n 2 7 8 9\n"), 0);
    CHECK_BITS(at<double*>(o, 0x8b0)[5], 9.0);
    CHECK_EQ(read(md, kELMCON, " 1 1 4\n1 1 2 1 2\n"), 0);
    CHECK_BITS(at<double*>(o, 0xa30)[0], 1.0);
    CHECK_EQ(write_elm(md), std::string("ELMCON       1       1       4\n       1       1       2       1       2\n"));
    def::model::llobj::ext::g_hook = nullptr;  // the real state-variable code (no test hook)
}

// ---- corpus / oracle: full RZ and ELMCON blocks ------------------------------------------------

namespace {

std::string slurp(const fs::path& p)
{
    std::ifstream in(p, std::ios::binary);
    std::ostringstream s;
    s << in.rdbuf();
    std::string t = s.str();
    std::string r;
    for (char c : t)
        if (c != '\r') r += c;
    return r;
}

// the record starting with "<kw> " plus its following lines that start with a blank
std::string block(const std::string& text, const std::string& kw)
{
    std::size_t p = text.rfind("\n" + kw + " ");
    if (p == std::string::npos) return {};
    ++p;
    std::size_t e = text.find('\n', p);
    while (e != std::string::npos && e + 1 < text.size() && text[e + 1] == ' ') e = text.find('\n', e + 1);
    return text.substr(p, e == std::string::npos ? std::string::npos : e + 1 - p);
}

fs::path oracle_output(const std::string& label)
{
    const char* up = std::getenv("USERPROFILE");
    const char* cache = std::getenv("DEFORM_ORACLE_CACHE");
    fs::path root = cache != nullptr ? fs::path(cache) : (up != nullptr ? fs::path(up) / ".deform-oracle" : fs::path());
    std::error_code ec;
    if (root.empty() || !fs::exists(root / "entries", ec)) return {};
    for (auto& d : fs::recursive_directory_iterator(root / "entries", ec)) {
        if (d.path().filename() != "result.json") continue;
        if (slurp(d.path()).find("\"label\": \"" + label + "\"") != std::string::npos)
            return d.path().parent_path() / "files" / "KFWRIT_OUT.KEY";
    }
    return {};
}

void corpus_case(const char* rel, int dim)
{
    fs::path src = fs::path("C:/Users/alext/Projects/DEFORM_corpus/key") / rel;
    if (const char* c = std::getenv("DEFORM_CORPUS")) src = fs::path(c) / "key" / rel;
    std::string label = std::string("kfrw/key/") + rel;
    for (char& ch : label)
        if (ch == '\\') ch = '/';
    fs::path out = oracle_output(label);
    std::error_code ec;
    if (out.empty() || !fs::exists(src, ec)) {
        std::printf("note: %s skipped (no corpus or oracle cache)\n", rel);
        return;
    }
    const std::string in = slurp(src);
    const std::string expect = slurp(out);
    Model md(dim);
    std::string rz = block(in, "RZ"), elm = block(in, "ELMCON");
    REQUIRE(!rz.empty() && !elm.empty());
    CHECK_EQ(read(md, kRZ, rz.substr(2)), 0);
    CHECK_EQ(read(md, kELMCON, elm.substr(6)), 0);
    CHECK_EQ(write_rz(md), block(expect, "RZ"));
    CHECK_EQ(write_elm(md), block(expect, "ELMCON"));
}

} // namespace

TEST_CASE(corpus_blocks_match_oracle)
{
    corpus_case("2D/EN/plane stress/Ela_PlaneStress_EN.KEY", 2);
    corpus_case("3D/EN/NAFEMS_Benchmarks/NAFEMS_T2.KEY", 3);
}

int main() { return testkit::run_all(); }
