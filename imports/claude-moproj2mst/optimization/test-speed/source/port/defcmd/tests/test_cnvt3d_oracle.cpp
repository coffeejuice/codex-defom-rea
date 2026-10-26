// ctest-labels: p9-01 cnvt3d oracle
// CNVT3D (packet p9-01) against the original DEF_PRE: the cases of tests/commands/CNVT3D owned by
// p9-01 are replayed through the port (CMD.KEY read by the KEY loop: KFREAD reads a copy of the
// seed, KFWRIT writes every object with the group 2 writer, ACTOUT and CNVT3D go to the port's
// cases) in a per-process temp directory, and compared with the cached oracle run:
//   * the conversion messages on stdout (the lines CNVT3D prints),
//   * CONV_STATUS.DAT, " .LOG", DEFORM.LOG and DEF_CNV23.DAT byte for byte,
//   * the group 2 records of OUT.KEY (2D model after the command) and of CONV3D.KEY (the 3D model),
//     except the seed's baseline round-trip gaps (oracle label "_baseline/RING_SI").
// Cases whose path needs packets not on this branch are listed as blocked (with the owner) and
// not run, since their callees stop in DEF_NOT_PORTED. no_actout runs with stand-ins for those
// callees whose effect is not observable in that case (the save fails, so the 3D model is never
// written): the p8-08 simulation-control/material/phase-pair copies are no-ops, and the p9-03
// generator only logs its "Creating geometry..." line (what 14013c340 writes for n = 1).
// CNVT3D_ORACLE_VERBOSE=1 prints the differing records.
#define _CRT_SECURE_NO_WARNINGS
#include "cmd_oracle_runner.hpp"

#include <cstdlib>
#include <defcmd/cmd_p9_01.hpp>
#include <defcmd/cnvt3d.hpp>
#include <defcmd/dispatch.hpp>
#include <defkey/line_io.hpp>
#include <testkit/check.hpp>

#include <process.h>

namespace {

namespace fs = std::filesystem;
using cmdoracle::slurp;

// JSON string value with \n \t \" \\ \/ undone
std::string jstr(const std::string& j, const char* key, std::size_t from = 0, std::size_t* end = nullptr)
{
    std::string pat = std::string("\"") + key + "\": \"";
    std::size_t p = j.find(pat, from);
    if (p == std::string::npos) return {};
    p += pat.size();
    std::string out;
    while (p < j.size() && j[p] != '"') {
        char c = j[p];
        if (c == '\\' && p + 1 < j.size()) {
            ++p;
            c = j[p];
            if (c == 'n') c = '\n';
            else if (c == 't') c = '\t';
            else if (c == 'r') c = '\r';
        }
        out += c;
        ++p;
    }
    if (end != nullptr) *end = p;
    return out;
}

fs::path cache_dir()
{
    std::string c = cmdoracle::env("DEFORM_ORACLE_CACHE", "");
    if (c.empty()) return fs::path(cmdoracle::env("USERPROFILE", "")) / ".deform-oracle";
    return c;
}

// newest cached run of `label`
fs::path find_entry(const std::string& label)
{
    fs::path best;
    std::string best_created;
    const fs::path e = cache_dir() / "entries";
    if (!fs::exists(e)) return best;
    for (auto& d1 : fs::directory_iterator(e)) {
        if (!d1.is_directory()) continue;
        for (auto& d2 : fs::directory_iterator(d1.path())) {
            const fs::path rj = d2.path() / "result.json";
            if (!fs::exists(rj)) continue;
            const std::string j = slurp(rj);
            if (jstr(j, "label") != label) continue;
            const std::string cr = jstr(j, "created");
            if (best.empty() || cr > best_created) {
                best = d2.path();
                best_created = cr;
            }
        }
    }
    return best;
}

struct Hooks : def::key::KeyReaderHooks {
    def::model::DB_STEP_DATA* m;
    fs::path seed;
    explicit Hooks(def::model::DB_STEP_DATA* mm) : m(mm) {}
    bool has_sub_model() override { return false; }
    void set_model_dimension(int dim) override
    {
        std::int32_t d = dim;
        m->set_dimension(&d);
    }
    void set_sub_model_dimension(int) override {}
    static void skip_line(std::FILE* f)
    {
        char buf[0x400];
        (void)std::fgets(buf, sizeof buf, f);
    }
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
        std::string n = def::key::keyword_label(id);
        while (!n.empty() && n.back() == ' ') n.pop_back();
        if (n == "KFREAD") {
            skip_line(f);
            skip_line(f);
            std::FILE* s = std::fopen(seed.string().c_str(), "r");
            std::FILE* sink = std::fopen("NUL", "w");
            def::key::read_key_stream(s, *this, sink);
            std::fclose(s);
            std::fclose(sink);
            return 0;
        }
        if (n == "KFWRIT") {
            skip_line(f);
            char name[0x400] = {0};
            (void)std::fgets(name, sizeof name, f);
            std::FILE* out = std::fopen("OUT.KEY", "wb");
            for (def::model::LLOBJ_DATA* ob = *reinterpret_cast<def::model::LLOBJ_DATA**>(reinterpret_cast<unsigned char*>(m) + 0x1380);
                 ob != nullptr; ob = *reinterpret_cast<def::model::LLOBJ_DATA**>(reinterpret_cast<unsigned char*>(ob) + 0x3940))
                def::key::g2obj::write_object(*m, out, ob, 0, false);
            std::fclose(out);
            return 0;
        }
        if (n == "ACTOUT") {  // part of 140378e00 (as p5-01's case_actout)
            std::int32_t ignored = 0;
            char line[0x400];
            if (def::key::read_i(f, &ignored, false) != 0) return 2;
            if (std::fgets(line, 0x400, f) == nullptr) return 2;
            for (unsigned i = 0; i < std::strlen(line); ++i)
                if (line[i] == '\n') line[i] = '\0';
            std::strcpy(static_cast<def::model::ACTION_DB_STEP*>(m)->text, line);
            return 0;
        }
        if (n == "CNVT3D") return def::cmd::cmd_cnvt3d(m, f);
        skip_line(f);
        return 0;
    }
};

struct Run {
    std::string stdout_text;
    std::map<std::string, std::string> files;  // name -> content
};

// replay `cmd` with `seed` in a fresh directory; returns the files named in `want`
Run replay(const std::string& tag, const std::string& cmd, const std::string& seed_src, const std::vector<std::string>& want)
{
    Run r;
    const fs::path old = fs::current_path();
    const fs::path dir = fs::temp_directory_path() / ("p901_or_" + tag + "_" + std::to_string(_getpid()));
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    fs::current_path(dir);
    fs::copy_file(seed_src, dir / "SEED.KEY", fs::copy_options::overwrite_existing);
    {
        std::ofstream o("CMD.KEY", std::ios::binary);
        o << cmd;
    }
    auto* m = new def::model::DB_STEP_DATA(3);  // leaked as in cmd_oracle_runner
    std::fflush(stdout);
    const int saved = _dup(_fileno(stdout));
    std::FILE* cap = std::fopen("stdout.cap", "wb");
    _dup2(_fileno(cap), _fileno(stdout));
    {
        Hooks h(m);
        h.seed = dir / "SEED.KEY";
        std::FILE* f = std::fopen("CMD.KEY", "r");
        def::key::read_key_stream(f, h, stdout);
        std::fclose(f);
    }
    std::fflush(stdout);
    _dup2(saved, _fileno(stdout));
    _close(saved);
    std::fclose(cap);
    r.stdout_text = slurp(dir / "stdout.cap");
    for (const std::string& w : want)
        if (fs::exists(dir / w)) r.files[w] = slurp(dir / w);
    fs::current_path(old);
    fs::remove_all(dir, ec);
    return r;
}

bool is_message(const std::string& l)
{
    static const char* kPre[] = {"$BGN$", "$END$", "Conversion ", "Saving data", "No. of Objects", "Object = ",
                                 "Revolve", "Extrude", "Generate ", "NoGeometrySections", "OutputMesh",
                                 "Creating geometry", "Generating ", "Assigning ", "Interpolate ", "Load interpolated",
                                 "2D DB is not correct", "[", "Starting "};
    for (const char* p : kPre)
        if (l.rfind(p, 0) == 0) return true;
    return false;
}

std::vector<std::string> messages(const std::string& s)
{
    std::vector<std::string> v;
    for (auto& l : cmdoracle::lines_of(s))
        if (is_message(l)) v.push_back(l);
    return v;
}

struct Case {
    const char* file;     // tests/commands/CNVT3D/<file>.json
    const char* blocked;  // nullptr: run; else why not
    bool stand_ins;
};

// With p9-03 and the p8-07 / p8-08 owners in the build (p9/p10 integration) every case runs
// without stand-ins; CNVT3D_STAND_INS=1 restores the packet-branch behaviour.
#if __has_include(<defcmd/cnvt3d_build.hpp>) && __has_include(<defcmd/interp.hpp>) &&     __has_include(<defcmd/meshgen/mg_globals.hpp>)
constexpr bool kOwnersInBuild = true;
#else
constexpr bool kOwnersInBuild = false;
#endif

const Case kCases[] = {
    {"cnvt3d_n0", nullptr, false},
    {"cnvt3d_no_actout", nullptr, true},
    {"cnvt3d_no_geo23", "tet mesh path: generator 140147b70 (p9-03), surface/solid mesh (p8-06/p8-07), DEF_INT (p8-08), "
                        "sim-control/material copies 1401e8040/1401e84f0/1401ea900/14029b630/140339d30 (p8-08)", false},
    {"cnvt3d_two_objects", "geometry + tet mesh: 140147b70 (p9-03), GEOPRIM revolve (p9-05), p8-06/p8-07/p8-08", false},
    {"cnvt3d_geometry_die_obj1", "geometry path: 140147b70 / 140153710 (p9-03), GEOPRIM revolve 1402ce190 (p9-05), "
                                 "copies of p8-08", false},
};

void install_stand_ins()
{
    using namespace def::cmd::cnvt3d;
    ext().exchange_cleanup = [](def::cmd::MeshGenBase&, bool) {};  // fresh directory: nothing to delete
    ext().sim_ctrl_copy_a = [](def::model::Cls_1401e7b10*, def::model::Cls_1401e7b10*) {};
    ext().sim_ctrl_copy_b = [](def::model::Cls_1401e7b10*, def::model::Cls_1401e7b10*) {};
    ext().sim_ctrl_copy_c = [](def::model::Cls_1401e7b10*, def::model::Cls_1401e7b10*) {};
    ext().material_list_copy = [](void*, void*) {};
    ext().phase_pair_copy = [](void*, void*) {};
    ext().generate = [](def::cmd::MeshGenBase& c, std::int32_t, std::int32_t, std::int32_t, double, double, double,
                        std::int32_t kind) {
        if (kind == 5) log_message("Creating geometry...", c.u4360);
    };
}

} // namespace

TEST_CASE(cnvt3d_cases_match_the_oracle)
{
    keytest::legacy_printf();
    // FIXGEO.EXE / DEF_AMG* run from the DEFORM install through the launcher (140224100), as in the
    // p8-07 tests (p9/p10 integration: the owners make the real calls)
    if (std::getenv("DEFORM_PC_PATH") == nullptr) _putenv_s("DEFORM_PC_PATH", "C:\\Program Files\\SFTC\\DEFORM\\v11.0.2\\");
    if (!fs::exists(cache_dir() / "entries")) {
        std::printf("no oracle cache: skipped\n");
        return;
    }
    const bool verbose = cmdoracle::env("CNVT3D_ORACLE_VERBOSE", "0") == "1";
    const std::vector<std::string> text_files = {"CONV_STATUS.DAT", " .LOG", "DEFORM.LOG", "DEF_CNV23.DAT"};
    int ok = 0, bad = 0, blocked = 0, missing = 0;
    for (const Case& c : kCases) {
        const fs::path cf = cmdoracle::Runner::repo_root() / "tests" / "commands" / "CNVT3D" / (std::string(c.file) + ".json");
        const std::string j = slurp(cf);
        const std::string label = jstr(j, "label");
        const bool owners = kOwnersInBuild && cmdoracle::env("CNVT3D_STAND_INS", "0") != "1";
        if (c.blocked != nullptr && !owners) {
            std::printf("%-28s BLOCKED: %s\n", c.file, c.blocked);
            ++blocked;
            continue;
        }
        const fs::path e = find_entry(label);
        if (e.empty()) {
            std::printf("%-28s no oracle entry: skipped\n", c.file);
            ++missing;
            continue;
        }
        std::size_t pos = j.find("\"inputs\"");
        std::string cmd, seed;
        for (int k = 0; k < 2; ++k) {
            std::size_t end = 0;
            const std::string name = jstr(j, "name", pos, &end);
            std::size_t vend = 0;
            if (name == "CMD.KEY") cmd = jstr(j, "text", end, &vend);
            else seed = jstr(j, "source", end, &vend);
            pos = vend;
        }
        def::cmd::cnvt3d::reset_ext();
        if (c.stand_ins && !owners) install_stand_ins();
        std::vector<std::string> want = text_files;
        want.push_back("OUT.KEY");
        want.push_back("CONV3D.KEY");
        const Run port = replay(c.file, cmd, seed, want);
        def::cmd::cnvt3d::reset_ext();
        std::string rep;
        // messages
        const std::vector<std::string> pm = messages(port.stdout_text), om = messages(slurp(e / "stdout.txt"));
        if (pm != om) {
            rep += "  messages differ\n";
            for (auto& l : om) rep += "    orig: " + l + "\n";
            for (auto& l : pm) rep += "    port: " + l + "\n";
        }
        // status and log files
        for (const std::string& t : text_files) {
            const bool oe = fs::exists(e / "files" / t);
            const bool pe = port.files.count(t) != 0;
            if (oe != pe) rep += "  file '" + t + "' " + (oe ? "missing in the port" : "only in the port") + "\n";
            else if (oe && slurp(e / "files" / t) != port.files.at(t)) rep += "  file '" + t + "' differs\n";
        }
        // KEY records (baseline gaps of the seed excluded for OUT.KEY)
        std::set<std::pair<int, std::string>> gaps;
        const fs::path base = find_entry("_baseline/RING_SI");
        if (!base.empty()) {
            const Run pb = replay(std::string(c.file) + "_base", "KFREAD 0\nRING_SI.KEY\nKFWRIT\nOUT.KEY\n", seed, {"OUT.KEY"});
            cmdoracle::RecordMap a = cmdoracle::object_records(slurp(base / "files" / "OUT.KEY"));
            cmdoracle::RecordMap b = cmdoracle::object_records(pb.files.count("OUT.KEY") ? pb.files.at("OUT.KEY") : "");
            for (auto& [k, v] : a)
                if (b[k] != v) gaps.insert(k);
            for (auto& [k, v] : b)
                if (a[k] != v) gaps.insert(k);
        }
        int eq = 0, df = 0, gp = 0;
        for (const char* kf : {"OUT.KEY", "CONV3D.KEY"}) {
            const bool oe = fs::exists(e / "files" / kf);
            const bool pe = port.files.count(kf) != 0;
            if (oe != pe) {
                rep += std::string("  ") + kf + (oe ? " missing in the port\n" : " only in the port\n");
                ++df;
                continue;
            }
            if (!oe) continue;
            cmdoracle::RecordMap a = cmdoracle::object_records(slurp(e / "files" / kf));
            cmdoracle::RecordMap b = cmdoracle::object_records(port.files.at(kf));
            std::set<std::pair<int, std::string>> keys;
            for (auto& [k, v] : a) keys.insert(k);
            for (auto& [k, v] : b) keys.insert(k);
            for (auto& k : keys) {
                if (a[k] == b[k]) {
                    ++eq;
                } else if (std::string(kf) == "OUT.KEY" && gaps.count(k) != 0) {
                    ++gp;
                } else {
                    ++df;
                    rep += std::string("  ") + kf + " record " + k.second + " object " + std::to_string(k.first) + " differs\n";
                    if (verbose) {
                        for (auto& s : a[k]) rep += "    orig: " + s;
                        for (auto& s : b[k]) rep += "    port: " + s;
                    }
                }
            }
        }
        const bool pass = rep.empty();
        std::printf("%-28s %s  records: %d equal, %d differ, %d baseline gaps\n%s", c.file, pass ? "PASS" : "FAIL", eq, df,
                    gp, rep.c_str());
        if (pass) ++ok;
        else ++bad;
    }
    std::printf("CNVT3D oracle: %d pass, %d fail, %d blocked, %d without entry\n", ok, bad, blocked, missing);
    CHECK_EQ(bad, 0);
}

int main() { return testkit::run_all(); }
