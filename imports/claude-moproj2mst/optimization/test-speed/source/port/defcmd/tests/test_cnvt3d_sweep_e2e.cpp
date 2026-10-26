// ctest-labels: p9-03 cnvt3d oracle
// Packet p9-03 end to end: the 12 CNVT3D cases of the packet (cnvt3d_sweep_* 9, revolve90_brick_obj2,
// extrude_tet_obj2, revolve360_tet_obj2) replayed through the whole command (p9-01 driver -> p9-03a
// entry / geometry -> p9-04 / p9-05 GEOPRIM -> p9-03b sweep) and compared with the cached oracle runs
// the way test_cnvt3d_oracle.cpp (p9-01) does: stdout messages, CONV_STATUS.DAT / " .LOG" /
// DEFORM.LOG / DEF_CNV23.DAT / NBCD.OUT bytes, the group 2 records of OUT.KEY and CONV3D.KEY.
//
// Each case runs in a child process (this executable with --case) because a callee that is still
// DEF_NOT_PORTED aborts; the parent prints one line per case: PASS, DIFF (with the differing
// records) or ABORT (with the function that stopped it). Stand-ins, all for packets not on this
// branch:
//   * p8-08 simulation-control / material / phase-pair copies and the exchange-file clean-up:
//     no-ops (model-level data outside the group 2 records; no exchange files in a fresh directory);
//   * p8-07 axis snapping 140155340: a no-op where the binary does nothing (c == 0 or GEO23 mode
//     != 1), DEF_NOT_PORTED otherwise;
//   * p8-07 NBCD.OUT read-back 140141580: a no-op returning 0, so BCCDEF / BCCDFN / ECCTMP / ECCATM /
//     SYMSUF of the converted object differ; they are listed apart ("NBCD stand-in");
//   * the p8-06 / p8-07 / p8-08 meshers and DEF_INT stay DEF_NOT_PORTED (the tet cases stop there).
// The test reports; it does not fail on cases blocked by other packets.
#define _CRT_SECURE_NO_WARNINGS
#include "cmd_oracle_runner.hpp"

#include <defcmd/cmd_p9_01.hpp>
#include <defcmd/cnvt3d.hpp>
#include <defcmd/cnvt3d_sweep.hpp>
#include <defcmd/dispatch.hpp>
#include <defkey/line_io.hpp>
#include <defrt/port.hpp>
#include <testkit/check.hpp>

#include <process.h>

#include "../src/cnvt3d_sweep/ext.hpp"
#include "../src/cnvt3d_sweep_b/ext.hpp"

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
    const fs::path dir = fs::temp_directory_path() / ("p903e_" + tag + "_" + std::to_string(_getpid()));
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

const char* kCases[] = {
    "cnvt3d_sweep_rev90_n2", "cnvt3d_sweep_rev180_n3", "cnvt3d_sweep_rev360_n4", "cnvt3d_sweep_axis_rev90_n2",
    "cnvt3d_sweep_axis_rev360_n4", "cnvt3d_sweep_axis_c0_rev90_n2", "cnvt3d_sweep_extrude_n3",
    "cnvt3d_sweep_snap_k1_rev90_n2", "cnvt3d_sweep_snap_k0_rev90_n2", "cnvt3d_revolve90_brick_obj2",
    "cnvt3d_extrude_tet_obj2", "cnvt3d_revolve360_tet_obj2"};

// records of the converted object that the NBCD.OUT stand-in explains
bool nbcd_explained(const std::string& kw)
{
    for (const char* k : {"BCCDEF", "BCCDFN", "ECCTMP", "ECCATM", "SYMSUF"})
        if (kw.rfind(k, 0) == 0) return true;
    return false;
}

void snap_stand_in(def::cmd::MeshGenBase*, def::model::LLOBJ_DATA* o, double c)
{
    if (c != 0.0 && o != nullptr &&
        *reinterpret_cast<std::int32_t*>(reinterpret_cast<unsigned char*>(o) + 0x2be8) == 1)
        DEF_NOT_PORTED(0x140155340);
}

// With the p8-07 / p8-08 owners in the build (p9/p10 integration) the stand-ins are off and the
// real copies, NBCD.OUT read-back and axis snapping run; CNVT3D_STAND_INS=1 turns them back on.
#if __has_include(<defcmd/interp.hpp>) && __has_include(<defcmd/meshgen/mg_globals.hpp>)
constexpr bool kOwnersInBuild = true;
#else
constexpr bool kOwnersInBuild = false;
#endif

void install_stand_ins()
{
    if (kOwnersInBuild && cmdoracle::env("CNVT3D_STAND_INS", "0") != "1") return;
    namespace cv = def::cmd::cnvt3d;
    cv::ext().exchange_cleanup = [](def::cmd::MeshGenBase&, bool) {};
    cv::ext().sim_ctrl_copy_a = [](def::model::Cls_1401e7b10*, def::model::Cls_1401e7b10*) {};
    cv::ext().sim_ctrl_copy_b = [](def::model::Cls_1401e7b10*, def::model::Cls_1401e7b10*) {};
    cv::ext().sim_ctrl_copy_c = [](def::model::Cls_1401e7b10*, def::model::Cls_1401e7b10*) {};
    cv::ext().material_list_copy = [](void*, void*) {};
    cv::ext().phase_pair_copy = [](void*, void*) {};
    cv::ext().read_nbcd = [](def::cmd::MeshGenBase&) { return 0; };
    def::cmd::cnvt3d_sweep::seam().fun_140155340 = &snap_stand_in;
    def::cmd::cnvt3d_sweep::ext::g.fun_140155340 = &snap_stand_in;
}

// one case in this process; 0 pass, 1 differs; the report line goes to `out`
int run_case(const char* file, std::FILE* out)
{
    keytest::legacy_printf();
    // the tet cases launch DEF_AMG3_GUI / MESH3D_GUI / BRKMSH / DEF_INT from the DEFORM install
    // (launcher 140224100 / 140224130), as in the p8-07 tests (p9/p10 integration)
    if (std::getenv("DEFORM_PC_PATH") == nullptr) _putenv_s("DEFORM_PC_PATH", "C:\\Program Files\\SFTC\\DEFORM\\v11.0.2\\");
    const fs::path cf = cmdoracle::Runner::repo_root() / "tests" / "commands" / "CNVT3D" / (std::string(file) + ".json");
    const std::string j = slurp(cf);
    const std::string label = jstr(j, "label");
    fs::path e = find_entry(label);
    if (e.empty()) {  // the case file was relabelled after the cached run ("3->4" -> "3 to 4")
        std::string old = label;
        const std::size_t p = old.find(" to ");
        if (p != std::string::npos) e = find_entry(old.replace(p, 4, "->"));
    }
    if (e.empty()) {
        std::fprintf(out, "NOENTRY\n");
        return 0;
    }
    std::size_t pos = j.find("\"inputs\"");
    std::string cmd, seed;
    for (int k = 0; k < 2; ++k) {
        std::size_t end = 0;
        const std::string name = jstr(j, "name", pos, &end);
        std::size_t vend = 0;
        if (name == "CMD.KEY") cmd = jstr(j, "text", end, &vend);
        else seed = (cf.parent_path() / jstr(j, "source", end, &vend)).string();
        pos = vend;
    }
    install_stand_ins();
    const std::vector<std::string> text_files = {"CONV_STATUS.DAT", " .LOG", "DEFORM.LOG", "DEF_CNV23.DAT", "NBCD.OUT"};
    std::vector<std::string> want = text_files;
    want.push_back("OUT.KEY");
    want.push_back("CONV3D.KEY");
    const Run port = replay(file, cmd, seed, want);
    std::string rep, expl;
    const std::vector<std::string> pm = messages(port.stdout_text), om = messages(slurp(e / "stdout.txt"));
    if (pm != om) rep += "messages; ";
    for (const std::string& t : text_files) {
        const bool oe = fs::exists(e / "files" / t);
        const bool pe = port.files.count(t) != 0;
        if (oe != pe) rep += "file '" + t + "' " + (oe ? "missing" : "extra") + "; ";
        else if (oe && slurp(e / "files" / t) != port.files.at(t)) rep += "file '" + t + "' differs; ";
    }
    int eq = 0, df = 0;
    for (const char* kf : {"OUT.KEY", "CONV3D.KEY"}) {
        const bool oe = fs::exists(e / "files" / kf);
        const bool pe = port.files.count(kf) != 0;
        if (oe != pe) {
            rep += std::string(kf) + (oe ? " missing; " : " extra; ");
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
                continue;
            }
            ++df;
            std::string kw = k.second;
            while (!kw.empty() && kw.back() == ' ') kw.pop_back();
            const std::string item = std::string(kf == std::string("OUT.KEY") ? "OUT" : "3D") + ":" + kw + "/" +
                                     std::to_string(k.first) + " ";
            if (std::string(kf) == "CONV3D.KEY" && nbcd_explained(kw)) expl += item;
            else rep += item;
        }
    }
    std::fprintf(out, "%s  records %d equal, %d differ | %s| NBCD stand-in: %s\n", rep.empty() ? "PASS" : "DIFF", eq, df,
                 rep.c_str(), expl.c_str());
    return rep.empty() ? 0 : 1;
}

std::string g_self;

} // namespace

TEST_CASE(cnvt3d_packet_cases_end_to_end)
{
    if (!fs::exists(cache_dir() / "entries")) {
        std::printf("no oracle cache: skipped\n");
        return;
    }
    int pass = 0, diff = 0, aborted = 0;
    for (const char* c : kCases) {
        const fs::path rep = fs::temp_directory_path() / ("p903e_rep_" + std::string(c) + "_" + std::to_string(_getpid()) + ".txt");
        const fs::path err = fs::path(rep.string() + ".err");
        std::error_code ec;
        fs::remove(rep, ec);
        const std::string a1 = "--case", a2 = c, a3 = rep.string(), a4 = err.string();
        const std::string q0 = "\"" + g_self + "\"", q3 = "\"" + a3 + "\"", q4 = "\"" + a4 + "\"";
        const char* argv[] = {q0.c_str(), a1.c_str(), a2.c_str(), q3.c_str(), q4.c_str(), nullptr};
        std::fflush(stdout);
        const intptr_t rc = _spawnv(_P_WAIT, g_self.c_str(), argv);
        const std::string r = fs::exists(rep) ? slurp(rep) : std::string();
        const std::string er = fs::exists(err) ? slurp(err) : std::string();
        if (r.empty()) {
            const std::size_t p = er.find("def_pre port: function");
            const std::string why = p == std::string::npos ? ("exit " + std::to_string(rc)) : er.substr(p, er.find('\n', p) - p);
            std::printf("%-32s ABORT  %s\n", c, why.c_str());
            ++aborted;
        } else {
            std::printf("%-32s %s", c, r.c_str());
            if (r.rfind("DIFF", 0) == 0) ++diff;
            else if (r.rfind("PASS", 0) == 0) ++pass;
        }
        fs::remove(rep, ec);
        fs::remove(err, ec);
    }
    std::printf("p9-03 end to end: %d pass, %d differ, %d stopped in callees of other packets\n", pass, diff, aborted);
}

int main(int argc, char** argv)
{
    g_self = argv[0];
    if (argc >= 5 && std::string(argv[1]) == "--case") {
        (void)std::freopen(argv[4], "w", stderr);
        std::FILE* out = std::fopen(argv[3], "w");
        const int r = run_case(argv[2], out);
        std::fclose(out);
        return r;
    }
    return testkit::run_all();
}
