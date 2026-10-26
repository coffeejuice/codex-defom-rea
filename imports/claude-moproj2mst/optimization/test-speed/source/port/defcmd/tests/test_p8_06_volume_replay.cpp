// ctest-labels: p8-06
// Replay of the 3D volume mesher run 140141d50 (packet p8-06) against capture runs of the original:
// tools/extprobe/make_wrappers.py wrappers around MESH3D_GUI / BRKMSH / CHKVOL / DEF_AMG3_GUI record
// the working directory before and after every launch (cache: P806_CAPTURE_CACHE, default
// deform-rea-work/out/p8mg/p8-06/capcache; cases tests/commands/DEFAMG/defamg_3d*.json and
// tests/commands/MENU5/menu5_die_generate_mesh_3d_chkvol.json run with --exe <wrapper DEF_PRE>).
//
// The test reads the seed KEY (IN.KEY) into a 3D model, puts the surface mesh that DEF_AMG3_GUI
// wrote (its MESH3D.INP, as the surface step's reader 14013f010 of p8-07 stores it: nodes, the
// polygons at obj+0xbf0 / +0xbf8, state 1) into the object, starts in a directory holding the
// files the original had before MESH3D_GUI (minus the four the step writes), and runs
// mesh3d_run_volume_mesher with the launcher's system() hook. At every launch the directory must
// equal the capture's "before" copy byte for byte (DEF_MGN.ACT, MESH3D.SCR, MESH3D.INP,
// DEFORM_UIRUNREMESH.DAT, BRKMSH.INI, CHK_VOL.INI ...); the hook then makes it equal to the
// "after" copy (mesh.out, SCRATCH.KEY ...). Afterwards the object's RZ and ELMCON records must
// equal the ones in the original's OUT.KEY, and the directory the original's final one.
// Skipped when the capture cache is missing.
#include "p806_capture.hpp"

#include <defcmd/geolib.hpp>
#include <defcmd/launch.hpp>
#include <defcmd/meshgen/mesh3d.hpp>
#include <defcmd/meshgen/mg_globals.hpp>
#include <defcmd/p8_06_ext.hpp>
#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/node_access.hpp>
#include <testkit/check.hpp>

#include <process.h>

namespace {

namespace fs = std::filesystem;
using namespace p806cap;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;

struct Hooks : def::key::KeyReaderHooks {
    DB_STEP_DATA* m;
    explicit Hooks(DB_STEP_DATA* model) : m(model) {}
    bool has_sub_model() override { return false; }
    void set_model_dimension(int dim) override
    {
        std::int32_t d = dim;
        m->set_dimension(&d);
    }
    void set_sub_model_dimension(int) override {}
    int parse_keyword(int group, std::FILE* f, int id) override
    {
        switch (group) {
        case 1: return def::key::read_group1(m, f, id);
        case 2: return def::key::g2obj::read_object_keyword(*m, f, id);
        case 3: return def::key::g3::read_keyword(m, f, id);
        case 4: return def::key::g4::read_keyword(*m, f, id);
        case 5: return def::key::g57::read_phase_keyword(m, f, id);
        case 7: return def::key::g57::read_dim_keyword(m, f, id);
        default: break;
        }
        char buf[0x400];
        (void)std::fgets(buf, sizeof buf, f);
        return 0;
    }
};

LLOBJ_DATA* object(DB_STEP_DATA* m, int id)
{
    for (LLOBJ_DATA* ob = *reinterpret_cast<LLOBJ_DATA**>(reinterpret_cast<unsigned char*>(m) + 0x1380); ob != nullptr;
         ob = *reinterpret_cast<LLOBJ_DATA**>(reinterpret_cast<unsigned char*>(ob) + 0x3940)) {
        if (*reinterpret_cast<std::int32_t*>(reinterpret_cast<unsigned char*>(ob) + 8) == id) return ob;
    }
    return nullptr;
}

template <class T>
T& fld(LLOBJ_DATA* o, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<unsigned char*>(o) + off);
}

// the surface mesh of DEF_AMG3_GUI's MESH3D.INP into obj, as 14013f010 stores it (test double)
bool load_surface(DB_STEP_DATA* m, LLOBJ_DATA* obj, const fs::path& inp)
{
    std::FILE* f = std::fopen(inp.string().c_str(), "rt");
    if (f == nullptr) return false;
    int n = 0, k = 0;
    if (std::fscanf(f, "%d\n", &n) != 1 || n < 1) return false;
    std::vector<double> xyz(static_cast<std::size_t>(n) * 3);
    for (int i = 0; i < n; ++i) (void)std::fscanf(f, "%*d %lf %lf %lf\n", &xyz[i * 3], &xyz[i * 3 + 1], &xyz[i * 3 + 2]);
    if (std::fscanf(f, "%d\n", &k) != 1 || k < 1) return false;
    auto* poly = static_cast<std::int32_t*>(std::calloc(static_cast<std::size_t>(k) * 4, 4));
    for (int i = 0; i < k; ++i)
        (void)std::fscanf(f, "%*d %d %d %d %d\n", &poly[i * 4], &poly[i * 4 + 1], &poly[i * 4 + 2], &poly[i * 4 + 3]);
    std::fclose(f);
    def::cmd::geolib::fun_140202700(obj, n, 0, 2, m->u15f8, 0);
    double* c = def::model::fun_1401808d0(reinterpret_cast<def::model::Cls_14017ef20*>(reinterpret_cast<unsigned char*>(obj) + 0x8a0));
    std::memcpy(c, xyz.data(), xyz.size() * 8);
    fld<std::int32_t>(obj, 0x87c) = 1;
    // as 14013f010: one boundary record, its face arrays (re)allocated for k faces by 140200a70
    // (+0xbf8 4k ints and +0xc00 k ints), the polygons copied in, segments recounted. Setting
    // only the count and +0xbf8 left +0xc00 at the ELMCON-sized array, which the layer copy of
    // meshgen_bind (14017ec20) then read k ints from: a heap overread that crashed now and then.
    def::model::bc::set_boundary_count(*obj, 1);         // 140163300
    def::model::bc::set_boundary_face_count(*obj, k, 0);  // 140200a70
    std::memcpy(fld<std::int32_t*>(obj, 0xbf8), poly, static_cast<std::size_t>(k) * 16);
    def::model::bc::recount_boundary_segments(*obj);     // 140165d30
    std::free(poly);
    return true;
}

// One capture case: label, object id, the launches expected.
void run_case(const char* label, int obj_id, const std::vector<std::string>& expect_launch)
{
    const fs::path entry = find_entry(label);
    if (entry.empty()) {
        std::printf("SKIP %s: no capture run in %s\n", label, capture_cache().string().c_str());
        return;
    }
    const fs::path cap = entry / "files" / "_ext_capture";
    const fs::path surf = cap / "1_3D_DEF_AMG3_GUI" / "after" / "MESH3D.INP";
    const fs::path first = cap / "1_3D_MESH3D_GUI" / "before";
    REQUIRE(fs::exists(surf));
    REQUIRE(fs::exists(first));

    const fs::path old = fs::current_path();
    const fs::path work = fs::temp_directory_path() / ("p806_replay_" + std::to_string(_getpid()) + "_" + std::to_string(obj_id));
    fs::remove_all(work);
    fs::create_directories(work);
    for (auto& n : listing(first))
        if (n != "DEF_MGN.ACT" && n != "MESH3D.SCR" && n != "MESH3D.INP" && n != "DEFORM_UIRUNREMESH.DAT")
            fs::copy_file(first / n, work / n);
    fs::current_path(work);

    auto* m = new DB_STEP_DATA(3);  // leaked, as in cmd_oracle_runner
    {
        Hooks h(m);
        std::FILE* f = std::fopen("IN.KEY", "r");
        REQUIRE(f != nullptr);
        std::FILE* sink = std::fopen("NUL", "w");
        def::key::read_key_stream(f, h, sink);
        std::fclose(f);
        std::fclose(sink);
    }
    LLOBJ_DATA* obj = object(m, obj_id);
    REQUIRE(obj != nullptr);
    REQUIRE(load_surface(m, obj, surf));

    def::cmd::reset_mg_globals();
    def::cmd::g_mesher_state = 1;  // the surface mesh was read (14013f010)
    _putenv("DEFORM_PC_PATH=");
    Replay rp;
    rp.capture = cap;
    g_replay = &rp;
    def::cmd::g_launch_system_hook = &system_hook;
    def::cmd::MeshGenBase mg;
    def::cmd::meshgen_bind(mg, obj, m, m->u340);
    const std::int32_t r = def::cmd::mesh3d_run_volume_mesher(mg);
    def::cmd::g_launch_system_hook = nullptr;
    g_replay = nullptr;

    CHECK_EQ(r, 0);
    CHECK_EQ(def::cmd::g_mesher_state, 2);
    CHECK(rp.launched == expect_launch);
    // the directory at the end of 140141d50: the last capture's "after" minus what the step deletes
    const fs::path last = cap / ("1_3D_" + expect_launch.back()) / "after";
    std::set<std::string> want = listing(last);
    for (const char* n : {"MESH3D.SCR", "MESH3D.INP", "MESH3D.MSG", "BRKMSH.INI", "SCRATCH.KEY", "SCRATCH.DAT",
                          "DEF_MGN.OUT", "BRKMSH.KEY", "BRKMSH.MSG", "CHK_VOL.MSG", "DEFORM.GEO", "DEFORM.DST",
                          "DEFORM.RES", "DEFORM_001.KEY", "DEFORM_002.KEY"})
        want.erase(n);
    const std::set<std::string> have = listing(fs::current_path());
    for (auto& n : want)
        if (have.count(n) == 0) rp.problems += "  end: missing " + n + "\n";
    for (auto& n : have)
        if (want.count(n) == 0) rp.problems += "  end: extra " + n + "\n";

    // mesh against the original's OUT.KEY
    const fs::path outk = fs::path("PORT_OUT.KEY");
    {
        std::FILE* out = std::fopen(outk.string().c_str(), "wb");
        def::key::g2obj::write_object(*m, out, obj, 0, false);
        std::fclose(out);
    }
    const std::string orig = cmdoracle::slurp(entry / "files" / "OUT.KEY");
    const std::string port = cmdoracle::slurp(outk);
    for (const char* kw : {"RZ", "ELMCON"}) {
        const std::string a = records(orig, obj_id, kw), b = records(port, obj_id, kw);
        if (a.empty() || a != b) rp.problems += std::string("  OUT.KEY record ") + kw + " differs\n";
    }
    fs::current_path(old);
    std::error_code ec;
    fs::remove_all(work, ec);
    if (!rp.problems.empty()) std::printf("%s:\n%s", label, rp.problems.c_str());
    CHECK(rp.problems.empty());
}

} // namespace

TEST_CASE(defamg_3d_volume_mesher)
{
    run_case("DEFAMG obj1 3D", 1, {"MESH3D_GUI", "BRKMSH"});
}

TEST_CASE(defamg_3d_block3_volume_mesher)
{
    run_case("DEFAMG obj1 3D with MGUSER window (BLOCK3.DAT)", 1, {"MESH3D_GUI", "BRKMSH"});
}

TEST_CASE(menu5_generate_mesh_chkvol)
{
    run_case("MENU5 obj2 die geometry 7 Generate Mesh 3D with CHK_VOL.INI > 0 (CHKVOL, T8-5, for p10-02)", 2,
             {"MESH3D_GUI", "BRKMSH", "CHKVOL"});
}

int main() { return testkit::run_all(); }
