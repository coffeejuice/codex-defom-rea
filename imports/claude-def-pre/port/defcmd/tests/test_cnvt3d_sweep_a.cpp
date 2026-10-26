// ctest-labels: p9-03 cnvt3d
// Packet p9-03a (CNVT3D entry and geometry build): unit tests through the seam (ext.hpp) and the
// oracle-backed checks this half can run alone:
//   * STEP.DAT: the cached oracle run "CNVT3D two objects 1,2" left the STEP.DAT that 1402ac8a0
//     wrote; it is parsed back into arrays and model widths and rewritten byte for byte;
//   * CONV_STATUS.DAT message lines of 14013c340 ("[i/n] " prefix) against the cached runs;
//   * T9-1: the +90 degree x rotation of the extrude case (node z = 5*c printed 3.0616005879E-016)
//     comes from the PathScale cos of fl(pi/2) that p1-10's 140325691 already calls.
// Temp files live in a per-process directory; the oracle checks pass with a note when the cache
// (%USERPROFILE%\.deform-oracle or DEFORM_ORACLE_CACHE) is absent.
#define _CRT_SECURE_NO_WARNINGS
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <process.h>

#include <defcmd/cnvt3d_sweep.hpp>
#include <defcmd/meshgen/meshgen_base.hpp>
#include <defmodel/geoprim/def_geoprim.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/layout/obj_geo2d_digitized.hpp>
#include <defmodel/llobj/cls_14017e740.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defmodel/members/ftn_geom.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/stdlib/libm.hpp>
#include <defrt/stdlib/stdio_compat_test.hpp>
#include <testkit/check.hpp>

#include "../src/cnvt3d_sweep/ext.hpp"

namespace fs = std::filesystem;
namespace cs = def::cmd::cnvt3d_sweep;
using def::cmd::MeshGenBase;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;

namespace {

template <class T, class B>
T& at(B* base, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<unsigned char*>(base) + off);
}

std::string slurp(const fs::path& p)
{
    std::ifstream in(p, std::ios::binary);
    std::ostringstream s;
    s << in.rdbuf();
    return s.str();
}

// Per-process working directory; every test runs inside it.
struct TempCwd {
    fs::path old, dir;
    TempCwd()
    {
        old = fs::current_path();
        dir = fs::temp_directory_path() / ("p903a_" + std::to_string(_getpid()));
        fs::create_directories(dir);
        fs::current_path(dir);
        for (auto& e : fs::directory_iterator(dir)) fs::remove_all(e.path());
    }
    ~TempCwd()
    {
        fs::current_path(old);
        std::error_code ec;
        fs::remove_all(dir, ec);
    }
};

fs::path oracle_entry(const std::string& label)
{
    const char* c = std::getenv("DEFORM_ORACLE_CACHE");
    fs::path root = c != nullptr && *c != '\0' ? fs::path(c) : fs::path(std::getenv("USERPROFILE") ? std::getenv("USERPROFILE") : "") / ".deform-oracle";
    root /= "entries";
    std::error_code ec;
    if (!fs::exists(root, ec)) return {};
    const std::string needle = "\"label\": \"" + label + "\"";
    for (auto& d1 : fs::directory_iterator(root, ec)) {
        if (!d1.is_directory()) continue;
        for (auto& d2 : fs::directory_iterator(d1.path(), ec)) {
            const fs::path r = d2.path() / "result.json";
            if (!fs::exists(r)) continue;
            if (slurp(r).find(needle) != std::string::npos) return d2.path();
        }
    }
    return {};
}

} // namespace

// ---- iostream pieces (NBCD.OUT text) ---------------------------------------------------------

TEST_CASE(ostream_scientific_precision_8)
{
    cs::IosState s;
    cs::fun_140159050(s, 0x40);
    CHECK_EQ(cs::fun_140159090(s, 8), 6);
    cs::fun_140159050(s, 0x1000);
    CHECK_EQ(cs::fun_14015a260(s, 952.5), std::string("9.52500000e+002"));
    CHECK_EQ(cs::fun_14015a260(s, 952.5 * std::cos(1.5707963267948966)), std::string("5.83238038e-014"));
    CHECK_EQ(cs::fun_14015a260(s, -8.660254037844387), std::string("-8.66025404e+000"));
    s.width = 18;
    CHECK_EQ(cs::fun_14015a260(s, 1.0), std::string("1.00000000e+000   "));
    CHECK_EQ(s.width, 0);
    cs::IosState g;  // default: %g, precision 6
    CHECK_EQ(cs::fun_14015a260(g, 0.5), std::string("0.5"));
}

// ---- small getters ------------------------------------------------------------------------------

TEST_CASE(floorf_and_digitized_getters)
{
    CHECK_EQ(cs::fun_140159030(10.0f), 10.0f);
    CHECK_EQ(static_cast<int>(cs::fun_140159030(static_cast<float>(40 / 4))), 10);
    def::model::OBJ_GEO2D g;
    CHECK(!cs::fun_140159110(&g));
    g.make_ext();
    CHECK(cs::fun_140159150(&g) == g.ext);
    CHECK(!cs::fun_140159110(&g));
    g.ext->n_points = 2;
    CHECK(cs::fun_140159110(&g));
    cs::fun_1401590d0(g.ext, 0.01);
    CHECK(g.ext->u18 == 0.01);
    CHECK_EQ(cs::fun_1401590f0(g.ext), 2);
    g.ext->n_points = 0;
}

// ---- quad split ---------------------------------------------------------------------------------

TEST_CASE(triangle_quality)
{
    const double a[3] = {0, 0, 0}, b[3] = {1, 0, 0}, c[3] = {0.5, std::sqrt(0.75), 0};
    double q = -1;
    const double area = cs::fun_14023cdb0(a, b, c, &q);
    // the same expressions in the binary's order
    const double ab = 1.0, bc = (0.5 - 1) * (0.5 - 1) + std::sqrt(0.75) * std::sqrt(0.75),
                 ca = 0.25 + std::sqrt(0.75) * std::sqrt(0.75);
    const double s = ((ab + bc) + ca) / 2.0;
    const double p = std::fabs(((s - ab) * (s - bc)) * (s - ca));
    CHECK(testkit::same_bits(area, std::sqrt(s * p)));
    CHECK(testkit::same_bits(q, std::sqrt(p / s) / (((ab * bc) * ca * 0.25) / std::sqrt(s * p))));
    const double z[3] = {0, 0, 0};
    cs::fun_14023cdb0(z, z, z, &q);
    CHECK(q == 0.0);
}

TEST_CASE(quad_split_diagonal_choice)
{
    // square: both splits equal -> (n0 n1 n2)(n3 n0 n2)
    const double sq[12] = {0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0};
    const std::int32_t f[8] = {9, 9, 9, 9, 1, 2, 3, 4};
    std::int32_t t1[3], t2[3];
    cs::fun_14023d410(sq, f, 2, 4, t1, t2);
    CHECK(t1[0] == 1 && t1[1] == 2 && t1[2] == 3);
    CHECK(t2[0] == 4 && t2[1] == 1 && t2[2] == 3);
    // a kite long along 2-4: the 2-4 diagonal gives the better pair -> (n0 n1 n3)(n2 n3 n1)
    const double kite[12] = {0, 0, 0, 1, -0.2, 0, 2, 0, 0, 1, 0.2, 0};
    const std::int32_t g[4] = {1, 2, 3, 4};
    cs::fun_14023d410(kite, g, 1, 4, t1, t2);
    const std::int32_t* n = g;
    double qa, qb, qc, qd;
    auto P = [&](int i) { return kite + (n[i] - 1) * 3; };
    cs::fun_14023cdb0(P(0), P(1), P(3), &qa);
    cs::fun_14023cdb0(P(2), P(3), P(1), &qb);
    double r1 = qb > 1e-9 ? qa / qb : 0.0;
    if (r1 > 1.0) r1 = 1.0 / r1;
    cs::fun_14023cdb0(P(0), P(1), P(2), &qc);
    cs::fun_14023cdb0(P(3), P(0), P(2), &qd);
    double r2 = qd > 1e-9 ? qc / qd : 0.0;
    if (r2 > 1.0) r2 = 1.0 / r2;
    if (r1 > r2) {
        CHECK(t1[0] == 1 && t1[1] == 2 && t1[2] == 4 && t2[0] == 3 && t2[1] == 4 && t2[2] == 2);
    } else {
        CHECK(t1[0] == 1 && t1[1] == 2 && t1[2] == 3 && t2[0] == 4 && t2[1] == 1 && t2[2] == 3);
    }
}

// ---- PNTGEO.GEO / FIXGEO profile clean-up -----------------------------------------------------

TEST_CASE(pntgeo_read_close_delete)
{
    TempCwd tmp;
    def::model::OBJ_GEO2D g;
    g.make_ext();
    def::model::OBJ_GEO2D_DIGITIZED* d = g.ext;
    d->u18 = 0.01;
    CHECK_EQ(cs::fun_14041e4d0(d), -10);  // no file
    auto put = [](const char* t) {
        std::FILE* f = std::fopen("PNTGEO.GEO", "w");
        std::fputs(t, f);
        std::fclose(f);
    };
    put("2\n");
    CHECK_EQ(cs::fun_14041e4d0(d), -13);
    put("1\n0 0\n");
    CHECK_EQ(cs::fun_14041e4d0(d), -16);
    put("1\n2 0\n1 0.5 1.5\nx\n");
    CHECK_EQ(cs::fun_14041e4d0(d), -18);
    put("1\n3 0\n1 0.5 1.5\n2 2.0 3.0\n3 4.0 1.5\n");
    CHECK_EQ(cs::fun_14041e4d0(d), 0);
    REQUIRE(d->n_points == 3);
    CHECK(d->points[0] == 0.5 && d->points[1] == 1.5 && d->points[4] == 4.0);
    CHECK_EQ(cs::fun_14041ea80(d), 0);  // 3.5 > 0.01: the first point is appended
    REQUIRE(d->n_points == 4);
    CHECK(d->points[6] == 0.5 && d->points[7] == 1.5);
    CHECK_EQ(cs::fun_14041ea80(d), 0);  // closed now: unchanged
    CHECK_EQ(d->n_points, 4);
    CHECK_EQ(cs::fun_14045a040("PNTGEO.GEO"), 1);
    CHECK(!fs::exists("PNTGEO.GEO"));
    CHECK_EQ(cs::fun_14045a040("PNTGEO.GEO"), 1);
}

TEST_CASE(fixgeo_cleanup_reads_pntgeo)
{
    TempCwd tmp;
    cs::reset_seam();
    static cs::FixgeoPar seen;
    cs::seam().fun_14027ece0 = [](def::model::OBJ_GEO2D*, const cs::FixgeoPar* p, std::int32_t*, std::int32_t del,
                                 char rc) {
        std::memcpy(static_cast<void*>(&seen), p, sizeof seen);
        CHECK(del == 0 && rc == 0);
        std::FILE* f = std::fopen("PNTGEO.GEO", "w");
        std::fputs("1\n2 0\n1 1.0 2.0\n2 3.0 4.0\n", f);
        std::fclose(f);
        return 1;
    };
    def::model::OBJ_GEO2D g;
    g.make_ext();
    def::model::OBJ_GEO2D_DIGITIZED* d = g.ext;
    d->u18 = 0.01;
    d->u30 = 5;
    cs::fun_1402778c0(&g);
    // tol_b keeps the ctor value (140220390: 0.001)
    CHECK(seen.tol_a == 0.01 && seen.tol_b == 0.001 && seen.angle_a == 20.0 && seen.angle_b == 300.0);
    CHECK(seen.b28 == 1 && seen.b29 == 0);
    CHECK_EQ(d->n_points, 3);  // read 2, closed with the first point
    CHECK(d->points[4] == 1.0 && d->points[5] == 2.0);
    CHECK(!fs::exists("PNTGEO.GEO"));
    cs::reset_seam();
}

// ---- messages and DEF_INT_3DTET.DAT ----------------------------------------------------------

TEST_CASE(log_prefix_and_stem)
{
    TempCwd tmp;
    cs::reset_seam();
    cs::seam().fun_1401b1520 = [](std::string s, std::int32_t n) {
        return (0 < n && n < static_cast<std::int32_t>(s.size())) ? s.substr(0, static_cast<std::size_t>(n)) : s;
    };
    *cs::seam().dat_1406e2d84() = 2;
    *cs::seam().dat_1406e2d88() = 3;
    cs::fun_14013c340("Creating geometry...", "X.DB");
    CHECK_EQ(slurp("CONV_STATUS.DAT"), std::string("[2/3] Creating geometry...\r\n"));
    CHECK_EQ(slurp("X.LOG"), std::string("[2/3] Creating geometry...\r\n"));
    *cs::seam().dat_1406e2d88() = 1;
    cs::fun_14013c340("Generating surface mesh...", " ");
    CHECK_EQ(slurp(" .LOG"), std::string("Generating surface mesh...\r\n"));
    CHECK_EQ(slurp("CONV_STATUS.DAT"),
             std::string("[2/3] Creating geometry...\r\nGenerating surface mesh...\r\n"));
    cs::reset_seam();
}

TEST_CASE(def_int_3dtet_file)
{
    TempCwd tmp;
    const std::int32_t v[3] = {1, 0, 12};
    cs::fun_14013d7e0(3, v);
    CHECK_EQ(slurp("DEF_INT_3DTET.DAT"), std::string("1\r\n0\r\n12\r\n"));
    cs::fun_14013d7c0();
    CHECK(!fs::exists("DEF_INT_3DTET.DAT"));
}

// ---- entry 140147b70 ----------------------------------------------------------------------------

namespace {
struct Calls {
    int remesh = -1, rev = 0, ext = 0, geo = 0, binds = 0, surf = 0, solid = 0, sv = 0;
    int nsec = 0;
    double start = 0, ang = 0, c = 0;
};
Calls g_calls;

struct EntryFixture {
    DB_STEP_DATA* m2 = new DB_STEP_DATA(2);  // leaked on purpose, as the other tests do
    DB_STEP_DATA* m3 = new DB_STEP_DATA(3);
    LLOBJ_DATA* obj = def::model::llobj_ctor::get_or_create(m2, 2);
    MeshGenBase* cvt = new MeshGenBase();
    EntryFixture()
    {
        cs::reset_seam();
        g_calls = Calls{};
        cvt->u3f48 = m2;
        at<DB_STEP_DATA*>(cvt, 0x3f50) = m3;
        std::strcpy(cvt->u4360, " ");
        cs::seam().fun_1401ee4c0 = [](const void*) { return 2; };
        cs::seam().fun_140148840 = [](MeshGenBase* mg, LLOBJ_DATA* o, DB_STEP_DATA*, const char*) {
            mg->u008 = o;
            mg->u010 = o;
            ++g_calls.binds;
        };
        cs::seam().fun_140148960 = [](MeshGenBase*, std::int32_t t) { g_calls.remesh = t; };
        cs::seam().fun_140148f70 = [](MeshGenBase*, std::int32_t n, double s, double a, double c, LLOBJ_DATA*) {
            ++g_calls.rev;
            g_calls.nsec = n;
            g_calls.start = s;
            g_calls.ang = a;
            g_calls.c = c;
        };
        cs::seam().fun_14014c0a0 = [](MeshGenBase*, std::int32_t n, double s, double a, LLOBJ_DATA*) {
            ++g_calls.ext;
            g_calls.nsec = n;
            g_calls.start = s;
            g_calls.ang = a;
        };
    }
};
} // namespace

TEST_CASE(entry_kind4_remesh_target_and_dispatch)
{
    TempCwd tmp;
    EntryFixture fx;
    at<std::int32_t>(fx.obj, 0x2be8) = 1;  // GEO23 revolve overrides the sim-control kind 2
    at<std::int32_t>(fx.obj, 0x2d80) = 5;  // != element count 0 -> remesh
    cs::fun_140147b70(fx.cvt, 2, 5, 43, 30.0, 90.0, 0.001, 4);
    CHECK_EQ(g_calls.remesh, 10);  // 43 / (5 - 1), truncated
    CHECK_EQ(g_calls.rev, 1);
    CHECK_EQ(g_calls.ext, 0);
    CHECK(g_calls.nsec == 5 && g_calls.start == 30.0 && g_calls.ang == 90.0 && g_calls.c == 0.001);
    CHECK(fx.cvt->u008 == fx.obj);

    g_calls = Calls{};
    at<std::int32_t>(fx.obj, 0x2be8) = 0;  // no override: kind 2 extrude
    at<std::int32_t>(fx.obj, 0x2d80) = at<std::int32_t>(fx.obj, 0x884);  // same count: no remesh
    cs::fun_140147b70(fx.cvt, 2, 3, 8, 5.0, 50.0, 0.0, 4);
    CHECK_EQ(g_calls.remesh, -1);
    CHECK_EQ(g_calls.ext, 1);
    CHECK(g_calls.nsec == 3 && g_calls.start == 5.0 && g_calls.ang == 50.0);

    g_calls = Calls{};
    at<std::int32_t>(fx.obj, 0x2be8) = 3;  // out of 1..2: no override
    cs::seam().fun_1401ee4c0 = [](const void*) { return 1; };
    cs::fun_140147b70(fx.cvt, 2, 3, 8, 5.0, 50.0, 0.0, 4);
    CHECK_EQ(g_calls.rev, 1);
    cs::reset_seam();
}

TEST_CASE(entry_kind5_and_kind2)
{
    TempCwd tmp;
    EntryFixture fx;
    // kind 5: the 3D object already has surface geometry (140160d90) -> only the bind of mg
    cs::seam().fun_140155340 = [](MeshGenBase*, LLOBJ_DATA*, double c) { CHECK(c == 0.25); };
    cs::seam().fun_14019ccc0 = [](DB_STEP_DATA*, std::int32_t no) {
        CHECK_EQ(no, 2);
        return reinterpret_cast<LLOBJ_DATA*>(0x1000);
    };
    cs::seam().fun_140160d90 = [](LLOBJ_DATA*) { return true; };
    cs::seam().fun_140189580 = [](void*) { return 0; };  // 140153910: the fake 3D object has no windows
    cs::fun_140147b70(fx.cvt, 2, 20, 0, 0.0, 360.0, 0.25, 5);
    CHECK_EQ(g_calls.binds, 2);  // the source object, then mg -> the 3D object
    CHECK_EQ(slurp("CONV_STATUS.DAT"), std::string("Creating geometry...\r\n"));

    // kind 2: the surface mesh fails (DAT_1406e2d74 stays 0) -> no solid mesh, no SV transfer
    cs::seam().fun_14013ee00 = [](MeshGenBase*, char f) {
        CHECK(f == 0);
        ++g_calls.surf;
        return 0;
    };
    cs::seam().fun_140140110 = [](MeshGenBase*, char) {
        ++g_calls.solid;
        return 0;
    };
    cs::seam().fun_140148a20 = [](MeshGenBase*, LLOBJ_DATA*) { ++g_calls.sv; };
    cs::fun_140147b70(fx.cvt, 2, 20, 0, 0.0, 360.0, 0.25, 2);
    CHECK(g_calls.surf == 1 && g_calls.solid == 0 && g_calls.sv == 0);
    *cs::seam().dat_1406e2d74() = 1;
    cs::fun_140147b70(fx.cvt, 2, 20, 0, 0.0, 360.0, 0.25, 2);
    CHECK(g_calls.surf == 2 && g_calls.solid == 1 && g_calls.sv == 1);
    CHECK_EQ(slurp("CONV_STATUS.DAT"),
             std::string("Creating geometry...\r\nGenerating surface mesh...\r\nGenerating surface mesh...\r\n"
                         "Generating solid mesh...\r\nAssigning state variables...\r\n"));
    cs::reset_seam();
}

// ---- 140153910 mesh-window z shift ----------------------------------------------------------------

namespace {
double g_win_xyz[9];
std::int32_t g_win_type = 3;
unsigned char g_win[0x400];
} // namespace

TEST_CASE(mesh_window_shift_extrude_only)
{
    cs::reset_seam();
    std::vector<unsigned char> obj(0x3000, 0);
    at<double>(obj.data(), 0x2d40) = 1.0;
    cs::seam().fun_140189580 = [](void*) { return 1; };
    cs::seam().fun_140269900 = [](void*, std::int32_t n) -> void* {
        CHECK_EQ(n, 1);
        at<std::int32_t>(g_win, 0x210) = g_win_type;
        return g_win;
    };
    cs::seam().fun_1400034d0 = [](void*) { return 3; };
    cs::seam().fun_140003500 = [](void*) { return 0; };
    cs::seam().fun_140006410 = [](void*) { return g_win_xyz; };
    cs::seam().fun_140005510 = [](void*) -> std::int32_t* { return nullptr; };
    const double z0[3] = {5.0 + 5e-5, 55.0 - 5e-5, 30.0};
    for (int i = 0; i < 3; ++i) g_win_xyz[i * 3 + 2] = z0[i];
    auto* o = reinterpret_cast<LLOBJ_DATA*>(obj.data());
    cs::fun_140153910(nullptr, 1, 5.0, 50.0, o);  // revolve: nothing
    CHECK(g_win_xyz[2] == z0[0]);
    cs::fun_140153910(nullptr, 2, 5.0, -50.0, o);  // the far end is start + len
    CHECK(g_win_xyz[2] == z0[0] - 50.0 * 0.001);
    CHECK(g_win_xyz[5] == z0[1]);
    for (int i = 0; i < 3; ++i) g_win_xyz[i * 3 + 2] = z0[i];
    cs::fun_140153910(nullptr, 2, 5.0, 50.0, o);
    CHECK(g_win_xyz[2] == z0[0] - std::fabs(50.0) * 0.001);
    CHECK(g_win_xyz[5] == z0[1] + std::fabs(50.0) * 0.001);
    CHECK(g_win_xyz[8] == 30.0);
    g_win_type = 2;
    for (int i = 0; i < 3; ++i) g_win_xyz[i * 3 + 2] = z0[i];
    cs::fun_140153910(nullptr, 2, 5.0, 50.0, o);
    CHECK(g_win_xyz[2] == z0[0]);
    g_win_type = 3;
    cs::reset_seam();
}

// ---- 140154270 face split (T9-3) ------------------------------------------------------------------

namespace {
double g_o3_xyz[12] = {0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0};
double g_xy2[8] = {0, 0, 1, 0, 1, 1, 0, 1};
std::int32_t g_conn2[4] = {1, 2, 3, 4};
double g_mesh_xy[64];
std::int32_t g_mesh_conn[64];
int g_rot = 0, g_fc = 0, g_del = 0;
} // namespace

TEST_CASE(mesh_boundary_face_split_uses_prim_2518)
{
    cs::reset_seam();
    namespace gp = def::model::geoprim;
    gp::Ext saved = gp::ext();
    gp::ext().mesh_alloc_nodes = [](def::model::OBJ_MESH2D*, const std::int32_t*) {};
    gp::ext().mesh_alloc_elems = [](def::model::OBJ_MESH2D*, const std::int32_t*, const std::int32_t*) {};
    gp::ext().mesh_coords = [](def::model::OBJ_MESH2D*) { return g_mesh_xy; };
    gp::ext().mesh_conn = [](def::model::OBJ_MESH2D*) { return g_mesh_conn; };
    auto* m2 = new DB_STEP_DATA(2);
    LLOBJ_DATA* obj = def::model::llobj_ctor::get_or_create(m2, 1);
    auto* m3 = new DB_STEP_DATA(3);
    LLOBJ_DATA* o3 = def::model::llobj_ctor::get_or_create(m3, 1);
    at<std::int32_t>(obj, 0x2be8) = 2;  // extrude: no -90 rotation
    at<std::int32_t>(obj, 0x2c28) = 3;
    at<std::int32_t>(obj, 0x2d80) = 77;
    cs::seam().fun_140183cb0 = [](void*) { return g_conn2; };
    cs::seam().fun_1401808d0 = [](void*) { return g_o3_xyz; };
    cs::seam().fun_1402ba500 = [](gp::DEF_GEOPRIM* p, LLOBJ_DATA* o, const char* f, DB_STEP_DATA*, void* ctx) {
        CHECK(*f == 0 && ctx != nullptr);
        CHECK(p->mode_geo == 0 && p->mode_mesh == 2 && p->kind == 1);
        p->u24fc = 1;
        p->u24fd = 1;
        // 1402ce310 stores the int flag in the low 4 bytes of the 8-byte list slot
        auto flag = [](std::int32_t v) {
            std::uint64_t bits = static_cast<std::uint32_t>(v);
            double d;
            std::memcpy(&d, &bits, 8);
            return d;
        };
        p->l2518.push_back(flag(0));
        p->l2518.push_back(flag(1));
        auto& b = at<def::model::Cls_14017e740>(o, 0xbe0);
        b.free_arrays();
        b.u000 = 4;
        b.u010 = 2;
        auto* faces = static_cast<std::int32_t*>(std::calloc(8, 4));
        const std::int32_t v[8] = {1, 2, 3, 1, 1, 2, 3, -4};
        std::memcpy(faces, v, sizeof v);
        b.u018 = faces;
        return true;
    };
    cs::seam().fun_14021d2f0 = [](LLOBJ_DATA*, const double*, const double*, double) { ++g_rot; };
    cs::seam().fun_1401fc730 = [](LLOBJ_DATA*, char* f) { CHECK(*f == 1); ++g_fc; };
    cs::seam().fun_1401ffd80 = [](LLOBJ_DATA*) { ++g_del; };
    CHECK(cs::fun_140154270(nullptr, obj, o3, m3));
    auto& b = at<def::model::Cls_14017e740>(o3, 0xbe0);
    REQUIRE(b.u010 == 3);
    CHECK_EQ(b.u000, 4);
    const auto* f = static_cast<const std::int32_t*>(b.u018);
    // face 1 kept; face 2 (4th entry -4: made positive, flagged) split into two triangles
    // whose 4th entry is minus their first node
    CHECK(f[0] == 1 && f[1] == 2 && f[2] == 3 && f[3] == 1);
    CHECK(f[4] == 1 && f[5] == 2 && f[6] == 3 && f[7] == -1);
    CHECK(f[8] == 4 && f[9] == 1 && f[10] == 3 && f[11] == -4);
    CHECK(g_rot == 0 && g_fc == 1 && g_del == 1);
    CHECK_EQ(at<std::int32_t>(o3, 0x2d90), 77);
    gp::ext() = saved;
    cs::reset_seam();
}

// STEP.DAT (1402ac8a0) is tested with its owner: test_step_dat.cpp (p9-ac8a0), oracle_ring_si

// ---- CONV_STATUS.DAT message lines of the cached runs ------------------------------------------

TEST_CASE(status_lines_oracle)
{
    const char* labels[] = {"CNVT3D two objects 1,2", "CNVT3D sweep extrude length 50 start 5 nsec 3",
                            "CNVT3D extrude tet obj2", "CNVT3D revolve 360 tet mesh obj2 (RING_SI)",
                            "CNVT3D revolve 90 brick obj2"};
    const char* mine[] = {"Creating geometry...", "Generating surface mesh...", "Generating solid mesh...",
                          "Assigning state variables..."};
    int checked = 0;
    for (const char* lab : labels) {
        const fs::path e = oracle_entry(lab);
        if (e.empty()) continue;
        std::istringstream in(slurp(e / "files" / "CONV_STATUS.DAT"));
        std::string want;
        std::string l;
        int n = 1;
        while (std::getline(in, l)) {
            if (l.rfind("No. of Objects = ", 0) == 0) n = std::atoi(l.c_str() + 17);
            std::string body = l;
            if (!body.empty() && body.back() == '\r') body.pop_back();
            const std::size_t p = body.find("] ");
            const std::string msg = (body.size() > 0 && body[0] == '[' && p != std::string::npos) ? body.substr(p + 2) : body;
            for (const char* m : mine)
                if (msg == m) want += l + "\n";
        }
        TempCwd tmp;
        cs::reset_seam();
        std::istringstream again(want);
        while (std::getline(again, l)) {
            if (!l.empty() && l.back() == '\r') l.pop_back();
            int i = 1;
            std::string msg = l;
            if (l[0] == '[') {
                i = std::atoi(l.c_str() + 1);
                msg = l.substr(l.find("] ") + 2);
            }
            *cs::seam().dat_1406e2d84() = i;
            *cs::seam().dat_1406e2d88() = n;
            cs::fun_14013c340(msg.c_str(), " ");
        }
        CHECK_EQ(slurp("CONV_STATUS.DAT"), want);
        ++checked;
    }
    std::printf("status lines checked in %d cached runs\n", checked);
    cs::reset_seam();
}

// ---- T9-1: the +90 degree rotation constant -------------------------------------------------------

TEST_CASE(t9_1_rotation_constant_is_pathscale_cos)
{
    double s = 0, c = 0;
    def::rt::libm::ps_sincos((90.0 / 45.0) * 0.7853981633974483, &s, &c);
    char buf[64];
    def::rt::crt::snprintf(buf, sizeof buf, "%.10E", 5.0 * c);
    CHECK_EQ(std::string(buf), std::string("3.0616005879E-016"));
    def::rt::crt::snprintf(buf, sizeof buf, "%.10E", 30.0 * c);
    CHECK_EQ(std::string(buf), std::string("1.8369603527E-015"));
    CHECK(c != std::cos(1.5707963267948966));  // the CRT cos gives 6.123233995736766e-17
    // the node path of 14021d2f0: 140325691 on (10, 0, 5) about x through 0
    double p[3] = {10.0, 0.0, 5.0};
    const double o[3] = {0, 0, 0}, d[3] = {1, 0, 0}, a = 90.0;
    def::model::fun_140325691(p, o, d, &a);
    def::rt::crt::snprintf(buf, sizeof buf, "%.10E %.10E %.10E", p[0], p[1], p[2]);
    CHECK_EQ(std::string(buf), std::string("1.0000000000E+001 -5.0000000000E+000 3.0616005879E-016"));
}

int main() { return testkit::run_all(); }
