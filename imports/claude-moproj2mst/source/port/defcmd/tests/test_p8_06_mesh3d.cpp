// ctest-labels: p8-06 tier1
// Tier 1 for the p8-06 paths the capture replay (test_p8_06_volume_replay.cpp) does not reach:
// MESH3D.INP from the surface geometry (state 0, 1401427e0 with the quad split) and SCRATCH.KEY
// files without an ELMCON line or with an unsupported node count (140142f10). Expected files are
// written out by hand from the listing (formats 14051db00 / 14051db20 / 14051db4c).
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/meshgen/mesh3d.hpp>
#include <defcmd/meshgen/mg_globals.hpp>
#include <defcmd/p8_06_ext.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/surface_model.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <testkit/check.hpp>

#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include <process.h>

namespace fs = std::filesystem;
using def::model::LLOBJ_DATA;
using def::model::OBJ_GEO3D;

namespace {

std::string slurp(const char* p)
{
    std::ifstream in(p, std::ios::binary);
    std::ostringstream s;
    s << in.rdbuf();
    return s.str();
}

struct InTemp {
    fs::path old = fs::current_path();
    fs::path dir;
    explicit InTemp(const char* tag)
    {
        dir = fs::temp_directory_path() / (std::string("p806_t1_") + tag + "_" + std::to_string(_getpid()));
        fs::remove_all(dir);
        fs::create_directories(dir);
        fs::current_path(dir);
    }
    ~InTemp()
    {
        fs::current_path(old);
        std::error_code ec;
        fs::remove_all(dir, ec);  // may fail where the original leaves a file open; harmless
    }
};

// object with surface 0 = 4 nodes of the unit square and the given polygons
struct SurfObj {
    LLOBJ_DATA obj{nullptr};
    OBJ_GEO3D* s = new OBJ_GEO3D();
    SurfObj(const std::int32_t* poly, std::int32_t npoly)
    {
        obj.u3c00[0] = s;
        const std::int32_t n = 4;
        s->resize_nodes(&n);
        s->resize_elems(&npoly);
        const double xyz[12] = {0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0.5};
        std::memcpy(def::model::geo_kw::geo3d_coords(s), xyz, sizeof xyz);
        std::memcpy(def::model::geo_kw::geo3d_elems(s), poly, static_cast<std::size_t>(npoly) * 16);
    }
    ~SurfObj()
    {
        obj.u3c00[0] = nullptr;
        delete s;
    }
};

const char* kNodes =
    "         4\n"
    "         1       0.0000000       0.0000000       0.0000000 \n"
    "         2       1.0000000       0.0000000       0.0000000 \n"
    "         3       1.0000000       1.0000000       0.0000000 \n"
    "         4       0.0000000       1.0000000       0.5000000 \n";

std::string write_inp(const std::int32_t* poly, std::int32_t npoly, std::int32_t* rc)
{
    SurfObj so(poly, npoly);
    def::cmd::MeshGenBase mg;
    mg.u008 = &so.obj;
    def::cmd::g_mesher_state = 0;
    *rc = def::cmd::mesh3d_write_inp(mg);
    return slurp("MESH3D.INP");
}

// "wt": text mode writes CR LF
std::string crlf(const std::string& s)
{
    std::string r;
    for (char c : s) {
        if (c == '\n') r += '\r';
        r += c;
    }
    return r;
}

} // namespace

TEST_CASE(inp_state0_quad_split)
{
    InTemp t("quad");
    const std::int32_t poly[8] = {1, 2, 3, 4, 2, 3, 4, -1};
    std::int32_t rc = -1;
    const std::string got = write_inp(poly, 2, &rc);
    CHECK_EQ(rc, 0);
    // first polygon is a quad (1 != |4|): every polygon k becomes (a,b,c,a) or (a,b,c,-a) and
    // (a,c,|d|,a) / (a,c,|d|,-a) at k + n
    const std::string want = std::string(kNodes) +
                             "         4\n"
                             "         1          1          2          3          1\n"
                             "         2          2          3          4         -2\n"
                             "         3          1          3          4          1\n"
                             "         4          2          4          1         -2\n";
    CHECK(got == crlf(want));
}

TEST_CASE(inp_state0_first_triangle_no_split)
{
    InTemp t("tri");
    const std::int32_t poly[8] = {1, 2, 3, 1, 1, 3, 4, 2};  // only the first polygon is tested
    std::int32_t rc = -1;
    const std::string got = write_inp(poly, 2, &rc);
    CHECK_EQ(rc, 0);
    const std::string want = std::string(kNodes) +
                             "         2\n"
                             "         1          1          2          3          1\n"
                             "         2          1          3          4          2\n";
    CHECK(got == crlf(want));
}

namespace {


bool read_scratch(const char* text)
{
    {
        std::FILE* f = std::fopen("SCRATCH.KEY", "wb");
        std::fputs(text, f);
        std::fclose(f);
    }
    LLOBJ_DATA obj(nullptr);
    def::cmd::MeshGenBase mg;
    mg.u008 = &obj;
    const bool ok = def::cmd::mesh3d_read_scratch_key(mg);
    CHECK_EQ(obj.u87c[1], 0);  // obj+0x880: no mesh was allocated
    return ok;
}

} // namespace

TEST_CASE(scratch_without_rz_or_elmcon)
{
    InTemp t("scratch");
    CHECK(!read_scratch("NOTHING HERE\n"));
    CHECK(!read_scratch("  RZ       2       2\n1 0 0 0\n2 1 0 0\n"));
}

TEST_CASE(scratch_six_nodes_per_element_fails)
{
    InTemp t("scratch6");
    // ELMCON with 6 nodes per element: the element loop stops at once; the counts are >= 1, so
    // the original would still allocate the mesh (not checked here: that needs a model)
    CHECK(!read_scratch("RZ 2 0\nELMCON 2 1 6\n1 1 2 3 4 5 6\n"));
}

int main() { return testkit::run_all(); }
