// ctest-labels: p9-03 p9-03b cnvt3d
// Replay of the CNVT3D brick sweep oracle cases (tests/commands/CNVT3D/cnvt3d_sweep_*.json)
// through the p9-03b functions alone: the seed KEY is read into a 2D model with the group 1-5/7
// parsers, the converter is bound to object 2 (140148840) and the revolve / extrude sweep
// (140148f70 / 14014c0a0) builds object 2 of a new 3D model. Compared with the original's cached
// output (tools/oracle cache, %USERPROFILE%\.deform-oracle):
//   * NBCD.OUT byte for byte (partial revolves);
//   * the group 2 records RZ, DRZ, NDTMP, ELMCON, MTLGRP, STRAIN, STRESS of object 2 in CONV3D.KEY,
//     written with the group 2 object writer. The extrude case compares only the records the
//     driver's +90 degree x rotation (other half, 14021d2f0) leaves alone.
// Not exercised here (other owners): the axis snapping 140155340 (p8-07; a no-op hook - it only
// rewrites the 2D source radius of nodes that the sweep treats as axis nodes anyway), the
// STNCMP block-6 offset 140212460 (p9-03a; hook = its decompile, owner ? 1401d72e0 : 0), the
// node accessor 140180a40 (p8-08; hook = the accessor pattern with variable 3, slot +0x20).
// Without the cache the test passes with a note.
#define _CRT_SECURE_NO_WARNINGS
#include "cmd_oracle_runner.hpp"

#include <direct.h>

#include <defcmd/cnvt3d_sweep_b.hpp>
#include <defcmd/meshgen/meshgen_base.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <testkit/check.hpp>

#include "../src/cnvt3d_sweep_b/common.hpp"
#include "../src/cnvt3d_sweep_b/ext.hpp"

namespace fs = std::filesystem;
namespace sw = def::cmd::cnvt3d_sweep;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;

namespace {

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

struct Case {
    const char* label;
    const char* seed;
    bool revolve;
    std::int32_t nsec;
    double start, ang, c;
};

const Case kCases[] = {
    {"CNVT3D sweep revolve 90 nsec 2 off-axis", "sweep_offaxis.KEY", true, 2, 0.0, 90.0, 0.0},
    {"CNVT3D sweep revolve 180 start 30 nsec 3->4 off-axis", "sweep_offaxis.KEY", true, 3, 30.0, 180.0, 0.0},
    {"CNVT3D sweep revolve 360 nsec 4 off-axis", "sweep_offaxis.KEY", true, 4, 0.0, 360.0, 0.0},
    {"CNVT3D sweep revolve 90 nsec 2 nodes on axis", "sweep_axis.KEY", true, 2, 0.0, 90.0, 0.001},
    {"CNVT3D sweep revolve 360 nsec 4 nodes on axis", "sweep_axis.KEY", true, 4, 0.0, 360.0, 0.001},
    {"CNVT3D sweep revolve 90 nsec 2 nodes on axis, c=0", "sweep_axis.KEY", true, 2, 0.0, 90.0, 0.0},
    {"CNVT3D sweep revolve 90 nsec 2 near-axis nodes snapped (k=1)", "sweep_nearaxis.KEY", true, 2, 0.0, 90.0, 0.001},
    {"CNVT3D sweep revolve 90 nsec 2 near-axis nodes snapped (k=0)", "sweep_nearaxis.KEY", true, 2, 0.0, 90.0, 0.001},
    {"CNVT3D sweep extrude length 50 start 5 nsec 3", "sweep_offaxis.KEY", false, 3, 5.0, 50.0, 0.0},
};

// label -> entry directory (newest run wins)
std::map<std::string, fs::path> scan_cache()
{
    std::map<std::string, fs::path> out;
    std::map<std::string, std::string> created;
    std::string cache = cmdoracle::env("DEFORM_ORACLE_CACHE", "");
    if (cache.empty()) cache = (fs::path(cmdoracle::env("USERPROFILE", "")) / ".deform-oracle").string();
    const fs::path e = fs::path(cache) / "entries";
    if (!fs::exists(e)) return out;
    for (auto& d1 : fs::directory_iterator(e)) {
        if (!d1.is_directory()) continue;
        for (auto& d2 : fs::directory_iterator(d1.path())) {
            const fs::path rj = d2.path() / "result.json";
            if (!fs::exists(rj)) continue;
            const std::string j = cmdoracle::slurp(rj);
            const std::string label = cmdoracle::json_string(j, "label");
            if (label.rfind("CNVT3D sweep", 0) != 0) continue;
            const std::string cr = cmdoracle::json_string(j, "created");
            if (out.count(label) != 0 && cr < created[label]) continue;
            out[label] = d2.path();
            created[label] = cr;
        }
    }
    return out;
}

std::int32_t hook_212460(LLOBJ_DATA* o, std::int32_t i)
{
    return o->owner == nullptr ? 0 : sw::fun_1401d72e0(o->owner, i);
}

int g_run = 0, g_skipped = 0;

void run_case(const Case& c, const fs::path& dir)
{
    const fs::path repo = cmdoracle::Runner::repo_root();
    const fs::path work = fs::temp_directory_path() / keytest::tmp_name(std::string("p903b_") + std::to_string(&c - kCases));
    fs::create_directories(work);
    const fs::path seed = work / "IN.KEY";
    fs::copy_file(repo / "tests/commands/CNVT3D/data" / c.seed, seed, fs::copy_options::overwrite_existing);
    char old_cwd[1024];
    (void)_getcwd(old_cwd, sizeof old_cwd);
    (void)_chdir(work.string().c_str());

    sw::ext::reset_ext();
    sw::ext::g.fun_140155340 = [](def::cmd::MeshGenBase*, LLOBJ_DATA*, double) {};
    sw::ext::g.fun_140212460 = &hook_212460;
    // 140180a40 (p8-08): the node-set accessor of variable 3, same pattern as its neighbours
    sw::ext::g.fun_140180a40 = [](void* set) { return sw::detail::node_sv(set, 3, 0x20); };

    auto* m2 = new DB_STEP_DATA(3);
    {
        Hooks h(m2);
        std::FILE* f = std::fopen(seed.string().c_str(), "r");
        std::FILE* sink = std::fopen("NUL", "w");
        def::key::read_key_stream(f, h, sink);
        std::fclose(f);
        std::fclose(sink);
    }
    auto* m3 = new DB_STEP_DATA(3);
    LLOBJ_DATA* o2 = def::model::object_list_find(&m2->objects, 2);
    REQUIRE(o2 != nullptr);
    auto* cvt = new def::cmd::MeshGenBase();
    // 140148840 (meshgen_bind) would set u010 to a copy of o2; its list copies reach the
    // unported 140302090, so u010 points at o2 itself (only id, type and name are read from it)
    cvt->u008 = o2;
    cvt->u010 = o2;
    cvt->u3f48 = m2;
    *reinterpret_cast<DB_STEP_DATA**>(reinterpret_cast<unsigned char*>(cvt) + 0x3f50) = m3;
    *reinterpret_cast<double*>(reinterpret_cast<unsigned char*>(cvt) + 0x4770) = c.c;
    if (c.revolve) sw::fun_140148f70(cvt, c.nsec, c.start, c.ang, c.c, cvt->u008);
    else sw::fun_14014c0a0(cvt, c.nsec, c.start, c.ang, cvt->u008);

    // NBCD.OUT
    const bool want_nbcd = fs::exists(dir / "files" / "NBCD.OUT");
    CHECK_EQ(fs::exists("NBCD.OUT"), want_nbcd);
    if (want_nbcd && fs::exists("NBCD.OUT")) {
        const std::string a = cmdoracle::slurp(dir / "files" / "NBCD.OUT");
        const std::string b = cmdoracle::slurp("NBCD.OUT");
        if (a != b) std::printf("  %s: NBCD.OUT differs\n--- orig\n%s--- port\n%s", c.label, a.c_str(), b.c_str());
        CHECK(a == b);
    }
    // object 2 records
    LLOBJ_DATA* o3 = def::model::object_list_find(&m3->objects, 2);
    REQUIRE(o3 != nullptr);
    {
        std::FILE* out = std::fopen("PORT3D.KEY", "wb");
        def::key::g2obj::write_object(*m3, out, o3, 0, false);
        std::fclose(out);
    }
    if (std::getenv("P903B_DUMP")) {
        std::printf("%s", cmdoracle::slurp("PORT3D.KEY").c_str());
        bool nd = false;
        double* t = sw::fun_1401627e0(o3, 8, &nd);
        std::printf("w2[8]=%d w3[8]=%d ndtmp3=%p v=%g\n", m2->u15f8[8], m3->u15f8[8], (void*)t, t ? t[0] : -1.0);
        double* t2 = sw::fun_1401627e0(o2, 8, &nd);
        std::printf("src ndtmp=%p v=%g nn=%d dim=%d\n", (void*)t2, t2 ? t2[0] : -1.0, *reinterpret_cast<int*>(reinterpret_cast<char*>(o2) + 0x880), m2->dimension);
    }
    cmdoracle::RecordMap want = cmdoracle::object_records(cmdoracle::slurp(dir / "files" / "CONV3D.KEY"));
    cmdoracle::RecordMap got = cmdoracle::object_records(cmdoracle::slurp("PORT3D.KEY"));
    std::vector<std::string> kws = {"ELMCON", "MTLGRP", "NDTMP", "STRAIN"};
    if (c.revolve) {
        kws.push_back("RZ");
        kws.push_back("DRZ");
        kws.push_back("STRESS");
    }
    for (const std::string& k : kws) {
        const auto& a = want[{2, k}];
        const auto& b = got[{2, k}];
        if (a != b) {
            std::printf("  %s: %s differs\n--- orig\n", c.label, k.c_str());
            for (auto& s : a) std::printf("%s", s.c_str());
            std::printf("--- port\n");
            for (auto& s : b) std::printf("%s", s.c_str());
        }
        CHECK(!a.empty());
        CHECK(a == b);
    }
    (void)_chdir(old_cwd);
    std::error_code ec;
    fs::remove_all(work, ec);
    ++g_run;
}

} // namespace

TEST_CASE(cnvt3d_sweep_oracle_cases)
{
    keytest::legacy_printf();
    const auto cache = scan_cache();
    for (const Case& c : kCases) {
        auto it = cache.find(c.label);
        if (it == cache.end()) {
            ++g_skipped;
            continue;
        }
        run_case(c, it->second);
    }
    std::printf("cnvt3d sweep oracle: %d cases run, %d not in the cache\n", g_run, g_skipped);
}

int main() { return testkit::run_all(); }
