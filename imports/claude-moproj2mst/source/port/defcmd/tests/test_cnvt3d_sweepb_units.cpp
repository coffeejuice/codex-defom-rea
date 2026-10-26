// ctest-labels: p9-03 p9-03b cnvt3d
// Tier-1 checks of the p9-03b helpers that the sweep oracle replay (test_cnvt3d_sweepb_oracle)
// does not reach: the DEF_INT step files and messages (140146050, CNVT3D.md 6.3), the extrude
// layers before the driver's rotation (CNVT3D_SWEEP.md 4: z = start + dz*(s-1)), the SV
// dispatcher's nodal flags and gaps (1401627e0), 1401d72e0, 140161fd0, and the tet-path copy
// 140151400 (scalars per node, element vectors rotated to the centroid direction).
// Expected values are from the specs and the decompile, computed by hand below.
#define _CRT_SECURE_NO_WARNINGS
#include "../../defkey/tests/key_test_util.hpp"

#include <direct.h>

#include <cmath>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <defcmd/cnvt3d_sweep_b.hpp>
#include <defcmd/launch.hpp>
#include <defcmd/meshgen/meshgen_base.hpp>
#include <defmodel/bc/node_arrays.hpp>
#include <defmodel/geo_kw/misc_model.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defmodel/material/state_remap.hpp>
#include <defrt/stdlib/libm.hpp>
#include <testkit/check.hpp>

#include "../src/cnvt3d_sweep_b/common.hpp"
#include "../src/cnvt3d_sweep_b/ext.hpp"

namespace fs = std::filesystem;
namespace sw = def::cmd::cnvt3d_sweep;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;
using sw::detail::at;

namespace {

std::string slurp(const fs::path& p)
{
    std::ifstream in(p, std::ios::binary);
    std::ostringstream s;
    s << in.rdbuf();
    return s.str();
}

struct TempDir {
    fs::path dir;
    char old[1024];
    explicit TempDir(const char* tag)
    {
        dir = fs::temp_directory_path() / keytest::tmp_name(tag);
        fs::create_directories(dir);
        (void)_getcwd(old, sizeof old);
        (void)_chdir(dir.string().c_str());
    }
    ~TempDir()
    {
        (void)_chdir(old);
        std::error_code ec;
        fs::remove_all(dir, ec);
    }
};

// a model of dimension dim with its SV width table, and object `id` with nn nodes / ne elements
DB_STEP_DATA* new_model(std::int32_t dim)
{
    auto* m = new DB_STEP_DATA(3);
    m->set_dimension(&dim);
    def::model::step_material_state_sizes(*m);
    return m;
}

LLOBJ_DATA* new_object(DB_STEP_DATA* m, std::int32_t id, std::int32_t nn, std::int32_t ne)
{
    LLOBJ_DATA* o = def::model::llobj_ctor::get_or_create(m, id);
    def::model::bc::set_node_count(*o, nn, m->u15f8);
    def::model::bc::set_element_count(*o, ne, m->u15f8);
    return o;
}

def::cmd::MeshGenBase* new_cvt(DB_STEP_DATA* m2, DB_STEP_DATA* m3, LLOBJ_DATA* o)
{
    auto* c = new def::cmd::MeshGenBase();
    c->u008 = o;
    c->u010 = o;
    c->u3f48 = m2;
    at<DB_STEP_DATA*>(c, 0x3f50) = m3;
    std::strcpy(c->u4360, "RUN/job.KEY");
    return c;
}

std::vector<std::string> g_log;
std::vector<std::string> g_cmds;
std::string g_db;

} // namespace

TEST_CASE(defint_step_files_and_messages)
{
    TempDir t("p903b_defint");
    sw::ext::reset_ext();
    g_log.clear();
    g_cmds.clear();
    sw::ext::g.fun_14013c6b0 = [](const char* msg, const char* stem) { g_log.push_back(std::string(msg) + "|" + stem); };
    sw::ext::g.fun_1401563c0 = [](def::cmd::MeshGenBase*, const char*) { return true; };
    sw::ext::g.fun_1402ac8a0 = [](DB_STEP_DATA*, const char* db, LLOBJ_DATA*) { g_db = db; };
    sw::ext::g.fun_1401b1520 = [](std::string s, int n) {
        // 1401b1520: s truncated to n characters when 0 < n < length
        if (n < static_cast<int>(s.size()) && 0 < n) s.resize(static_cast<std::size_t>(n));
        return s;
    };
    def::cmd::g_launch_system_hook = [](const char* c) { g_cmds.push_back(c); return 0; };
    DB_STEP_DATA* m2 = new_model(2);
    at<std::int32_t>(m2, 0x744) = 7;
    LLOBJ_DATA* o = new_object(m2, 3, 4, 1);
    auto* cvt = new_cvt(m2, m2, o);
    std::ofstream("NEW.MSH") << "x";  // removed after the launch
    sw::fun_140146050(cvt);
    def::cmd::g_launch_system_hook = nullptr;
    CHECK_EQ(cvt->u3f40, 1);
    CHECK(slurp("DEF_INT.ACT") == "\r\n5\r\nY\r\n1\r\nNEW.MSH\r\n\r\n6\r\n\r\n");
    CHECK(slurp("DEF_REM.INI") == "RUN/job.KEY\r\n7\r\n2\r\n3\r\n");
    // stem before the last '.', then from the last '/' on (140006760 substr(pos)), "_REM.KEY"
    CHECK(g_db == "RUN/job.DB");
    CHECK_EQ(g_cmds.size(), 1u);
    if (!g_cmds.empty()) CHECK(g_cmds[0].find("DEF_INT\" < DEF_INT.ACT > DEF_INT.OUT") != std::string::npos);
    CHECK(!fs::exists("NEW.MSH"));
    // the interpolated KEY "/job_REM.KEY" does not exist -> failed
    CHECK_EQ(g_log.size(), 2u);
    if (g_log.size() == 2) {
        CHECK(g_log[0] == "Interpolate SV On 2D Quad Mesh ...|RUN/job.KEY");
        CHECK(g_log[1] == "Interpolate failed ...|RUN/job.KEY");
    }
    sw::ext::reset_ext();
}

TEST_CASE(defint_step_writer_failure_removes_inputs)
{
    TempDir t("p903b_defint2");
    sw::ext::reset_ext();
    sw::ext::g.fun_1401563c0 = [](def::cmd::MeshGenBase*, const char*) { return false; };
    sw::ext::g.fun_1401b1520 = [](std::string s, int) { return s; };
    DB_STEP_DATA* m2 = new_model(2);
    LLOBJ_DATA* o = new_object(m2, 2, 4, 1);
    auto* cvt = new_cvt(m2, m2, o);
    sw::fun_140146050(cvt);
    CHECK(!fs::exists("DEF_INT.ACT"));
    CHECK(!fs::exists("DEF_REM.INI"));
    sw::ext::reset_ext();
}

TEST_CASE(sv_dispatcher_and_small_helpers)
{
    DB_STEP_DATA* m2 = new_model(2);
    LLOBJ_DATA* o = new_object(m2, 1, 3, 1);
    bool nodal = false;
    CHECK(sw::fun_1401627e0(o, 8, &nodal) != nullptr);  // NDTMP
    CHECK(nodal);
    nodal = true;
    CHECK(sw::fun_1401627e0(o, 0x2b, &nodal) != nullptr);  // STRAIN
    CHECK(!nodal);
    nodal = true;
    CHECK(sw::fun_1401627e0(o, 7, &nodal) == nullptr);  // no case 6
    CHECK(nodal);  // untouched
    CHECK(sw::fun_1401627e0(o, 0x40, &nodal) == nullptr);
    CHECK_EQ(sw::fun_140161fd0(o), 0x2e);
    at<std::int32_t>(m2, 0x86c + 6 * 4) = 5;
    CHECK_EQ(sw::fun_1401d72e0(m2, 6), 5);
    CHECK_EQ(sw::fun_1401d72e0(m2, 7), 0);
    CHECK_EQ(sw::fun_1401d72e0(m2, -1), 0);
}

TEST_CASE(extrude_layers_before_rotation)
{
    TempDir t("p903b_extrude");
    sw::ext::reset_ext();
    sw::ext::g.fun_140212460 = [](LLOBJ_DATA*, std::int32_t) { return 0; };
    sw::ext::g.fun_140180a40 = [](void* s) { return sw::detail::node_sv(s, 3, 0x20); };
    DB_STEP_DATA* m2 = new_model(2);
    DB_STEP_DATA* m3 = new_model(3);
    // one quad (1 2 3 4) over r 10..20, z 0..10
    LLOBJ_DATA* o = new_object(m2, 1, 4, 1);
    double* xy = sw::detail::coords(o);
    const double p[8] = {10, 0, 20, 0, 20, 10, 10, 10};
    for (int i = 0; i < 8; ++i) xy[i] = p[i];
    std::int32_t* c = sw::detail::conn(o);
    for (int i = 0; i < 4; ++i) c[i] = i + 1;
    double* tmp = nullptr;
    bool nd = false;
    tmp = sw::fun_1401627e0(o, 8, &nd);
    for (int i = 0; i < 4; ++i) tmp[i] = 100.0 + i;
    auto* cvt = new_cvt(m2, m3, o);
    sw::fun_14014c0a0(cvt, 3, 5.0, 50.0, o);
    LLOBJ_DATA* o3 = def::model::llobj_ctor::get_or_create(m3, 1);
    CHECK_EQ(sw::detail::obj_nn(o3), 12);
    CHECK_EQ(sw::detail::obj_ne(o3), 2);
    const double* X = sw::detail::coords(o3);
    for (int s = 0; s < 3; ++s)
        for (int q = 0; q < 4; ++q) {
            CHECK_BITS(X[(s * 4 + q) * 3], p[q * 2]);
            CHECK_BITS(X[(s * 4 + q) * 3 + 1], p[q * 2 + 1]);
            CHECK_BITS(X[(s * 4 + q) * 3 + 2], 5.0 + 25.0 * s);
        }
    const std::int32_t* C = sw::detail::conn(o3);
    const std::int32_t want[16] = {1, 2, 3, 4, 5, 6, 7, 8, 5, 6, 7, 8, 9, 10, 11, 12};
    for (int i = 0; i < 16; ++i) CHECK_EQ(C[i], want[i]);
    double* t3 = sw::fun_1401627e0(o3, 8, &nd);
    for (int i = 0; i < 12; ++i) CHECK_BITS(t3[i], 100.0 + (i % 4));  // row i % NN
    sw::ext::reset_ext();
}

TEST_CASE(tet_copy_rotates_to_centroid_direction)
{
    sw::ext::reset_ext();
    sw::ext::g.fun_140212460 = [](LLOBJ_DATA*, std::int32_t) { return 0; };
    sw::ext::g.fun_140180a40 = [](void* s) { return sw::detail::node_sv(s, 3, 0x20); };
    DB_STEP_DATA* m2 = new_model(2);
    DB_STEP_DATA* m3 = new_model(3);
    const std::int32_t kd = def::model::geo_kw::simctl_geo_kind(&m2->sim_ctrl);
    // the 2D object after DEF_INT: 4 nodes, 1 element (the projected tet)
    LLOBJ_DATA* src = new_object(m2, 1, 4, 1);
    // the tet object of the 3D model: one tetrahedron around (3, 4, z)
    LLOBJ_DATA* tet = new_object(m3, 1, 4, 1);
    double* X = sw::detail::coords(tet);
    const double p[12] = {3, 4, 0, 3, 4, 1, 6, 8, 0, 0, 0, 1};
    for (int i = 0; i < 12; ++i) X[i] = p[i];
    for (int i = 0; i < 4; ++i) sw::detail::conn(tet)[i] = i + 1;
    bool nd = false;
    double* t2 = sw::fun_1401627e0(src, 8, &nd);
    for (int i = 0; i < 4; ++i) t2[i] = 50.0 + i;
    auto* cvt = new_cvt(m2, m3, src);
    sw::fun_140151400(cvt, tet);
    double* t3 = sw::fun_1401627e0(tet, 8, &nd);
    for (int i = 0; i < 4; ++i) CHECK_BITS(t3[i], 50.0 + i);
    // centroid (3, 4) -> (c, s) = (0.6, 0.8): checked through the node DRZ of node 1 (x = 3,
    // y = 4, |.| = 5) when the 2D model's DRZ (id 1) is a 2-vector and the 3D one a 3-vector
    if (m2->u15f8[1] == 2 && m3->u15f8[1] == 3) {
        double* d2 = sw::fun_1401627e0(src, 1, &nd);
        d2[0] = 2.0;
        d2[1] = 7.0;
        sw::fun_140151400(cvt, tet);
        double* d3 = sw::fun_1401627e0(tet, 1, &nd);
        if (kd == 1) {
            CHECK_BITS(d3[0], 2.0 * (3.0 / 5.0));
            CHECK_BITS(d3[1], 2.0 * (4.0 / 5.0));
            CHECK_BITS(d3[2], 7.0);
        } else {
            CHECK_BITS(d3[0], 2.0);
            CHECK_BITS(d3[1], 7.0);
            CHECK_BITS(d3[2], 0.0);
        }
    }
    sw::ext::reset_ext();
}

int main() { return testkit::run_all(); }
