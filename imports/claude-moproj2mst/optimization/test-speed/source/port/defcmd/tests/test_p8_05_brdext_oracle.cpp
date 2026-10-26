// BRDEXT oracle cases (tests/commands/BRDEXT/*.json) replayed through the port: the real AMG_DXT /
// AMG_DXT3 run from the DEFORM install (DEFORM_PC_PATH), in a per-process temporary directory.
// Compared with the cached oracle run: the BRDEXT message lines, every group 2 record of OUT.KEY,
// the files left in the directory (names and bytes: <prob>.GEO / .RES; the INI and DEF_BRD.OUT
// must be gone).
//
// DBREAD (P2, not on this branch) is replaced by a stand-in: the model comes from a seed KEY that
// the original wrote right after the same DBREAD (oracle "DEFFRZ unknown source object": DBREAD
// HAMMER_SI.DB, DEFFRZ fails before touching the model, KFWRIT; for COG_P1_B1.DB the failing
// "DEFINT obj1 3D (DEF_REM3_GUI)" run), then the DBREAD case's own
// fields are set as p5-01's case_dbread does: +0x1748 = the name, +0x744 = the last stored
// directory step, +0x340 = the name's stem. PROBID is the dispatcher case 0 (next line -> +0x340).
// Records that already differ after KFREAD + KFWRIT of the seed alone are baseline gaps.
// ctest-labels: p8-05 brdext oracle
#include "cmd_oracle_runner.hpp"

#include <defcmd/brdext.hpp>
#include <defcmd/cmd_p8_05.hpp>
#include <defcmd/meshgen/mg_globals.hpp>
#include <defdbio/step/db_info_read.hpp>
#include <defkey/line_io.hpp>
#include <defmodel/layout/cls_1401dbfe0.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/stdlib/str_helpers.hpp>
#include <testkit/check.hpp>

#include <direct.h>
#include <process.h>

namespace {

namespace fs = std::filesystem;
using cmdoracle::json_string;
using cmdoracle::lines_of;
using cmdoracle::slurp;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;

const char* kInstall = "C:\\Program Files\\SFTC\\DEFORM\\v11.0.2\\";

struct Entry {
    fs::path dir;
    std::string json;
};

std::map<std::string, Entry> scan_cache()
{
    std::map<std::string, Entry> m;
    fs::path cache = cmdoracle::env("DEFORM_ORACLE_CACHE", "");
    if (cache.empty()) cache = fs::path(cmdoracle::env("USERPROFILE", "")) / ".deform-oracle";
    if (!fs::exists(cache / "entries")) return m;
    for (auto& d1 : fs::directory_iterator(cache / "entries")) {
        if (!d1.is_directory()) continue;
        for (auto& d2 : fs::directory_iterator(d1.path())) {
            const fs::path rj = d2.path() / "result.json";
            if (!fs::exists(rj)) continue;
            std::string j = slurp(rj);
            const std::string label = json_string(j, "label");
            auto it = m.find(label);
            if (it != m.end() && json_string(j, "created") < json_string(it->second.json, "created")) continue;
            m[label] = Entry{d2.path(), std::move(j)};
        }
    }
    return m;
}


const std::map<std::string, Entry>& cache()
{
    static const std::map<std::string, Entry> c = scan_cache();
    return c;
}

// the raw "text" value with JSON escapes \n \" \\ undone
std::string json_text(const std::string& j, std::size_t from)
{
    const std::string pat = "\"text\": \"";
    std::size_t p = j.find(pat, from);
    p += pat.size();
    std::string out;
    while (p < j.size() && j[p] != '"') {
        if (j[p] == '\\' && p + 1 < j.size()) {
            ++p;
            out += j[p] == 'n' ? '\n' : j[p] == 't' ? '\t' : j[p];
            ++p;
            continue;
        }
        out += j[p++];
    }
    return out;
}

struct Hooks : def::key::KeyReaderHooks {
    DB_STEP_DATA* m;
    fs::path seed_key;   // KFREAD reads this
    fs::path dbread_seed; // DBREAD stand-in reads this
    Hooks(DB_STEP_DATA* model) : m(model) {}
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
    void read_seed(const fs::path& p)
    {
        std::FILE* s = std::fopen(p.string().c_str(), "r");
        std::FILE* sink = std::fopen("NUL", "w");
        def::key::read_key_stream(s, *this, sink);
        std::fclose(s);
        std::fclose(sink);
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
            read_seed(seed_key);
            return 0;
        }
        if (n == "DBREAD") {
            std::int32_t step = 0;
            (void)def::key::read_i(f, &step, false);
            char name[0x400];
            if (std::fgets(name, sizeof name, f) == nullptr) return 2;
            std::size_t l = std::strlen(name);
            while (l > 0 && (name[l - 1] == '\n' || name[l - 1] == '\r' || name[l - 1] == ' ')) name[--l] = 0;
            read_seed(dbread_seed);
            std::strcpy(m->u1748, name);
            def::model::Cls_1401dbfe0 db;
            if (def::dbio::load_db_info(db, name, false) != 0) return 2;
            const auto* rec = def::model::db_info_records(db);
            for (std::int32_t i = db.uac8 - 1; i >= 0; --i) {
                if (rec[i].stored != 0) {
                    step = rec[i].step;
                    break;
                }
            }
            m->u744 = step;
            const std::string stem = def::rt::Cls_1401b19c0(std::string(m->u1748)).stem();
            std::strncpy(m->u340, stem.c_str(), 0x400);
            return 0;
        }
        if (n == "PROBID") {
            char line[0x400];
            if (std::fgets(line, sizeof line, f) == nullptr) return 2; // rest of the keyword line
            if (std::fgets(line, sizeof line, f) == nullptr) return 2;
            std::size_t l = std::strlen(line);
            while (l > 0 && (line[l - 1] == '\n' || line[l - 1] == '\r' || line[l - 1] == ' ')) line[--l] = 0;
            std::strncpy(m->u340, line, 0x400);
            return 0;
        }
        if (n == "KFWRIT") {
            skip_line(f);
            char name[0x400];
            if (std::fgets(name, sizeof name, f) == nullptr) return 2;
            std::size_t l = std::strlen(name);
            while (l > 0 && (name[l - 1] == '\n' || name[l - 1] == '\r' || name[l - 1] == ' ')) name[--l] = 0;
            std::FILE* out = std::fopen(name, "wb");
            for (LLOBJ_DATA* ob = *reinterpret_cast<LLOBJ_DATA**>(reinterpret_cast<unsigned char*>(m) + 0x1380);
                 ob != nullptr; ob = *reinterpret_cast<LLOBJ_DATA**>(reinterpret_cast<unsigned char*>(ob) + 0x3940)) {
                def::key::g2obj::write_object(*m, out, ob, 0, false);
            }
            std::fclose(out);
            return 0;
        }
        if (n == "BRDEXT") return def::cmd::cmd_brdext(m, f);
        skip_line(f);
        return 0;
    }
};

bool is_message(const std::string& l)
{
    return l.rfind("INFO   : Extracting", 0) == 0 || l.rfind("ERROR  :", 0) == 0 ||
           l.rfind("         Success", 0) == 0 || l.rfind("Database [", 0) == 0 ||
           l.rfind("Generate a new", 0) == 0 || l.rfind("and perform border", 0) == 0;
}

std::vector<std::string> messages(const std::string& text)
{
    std::vector<std::string> v;
    for (auto& l : lines_of(text))
        if (is_message(l)) v.push_back(l);
    return v;
}

struct Run {
    std::string msgs;
    std::map<std::string, std::string> files; // name -> bytes (inputs excluded)
};

// replay CMD.KEY in dir (cwd = dir while it runs)
Run replay(const fs::path& dir, const std::string& cmd, const fs::path& seed_key, const fs::path& dbread_seed,
           const std::set<std::string>& input_names)
{
    def::cmd::reset_mg_globals();
    const fs::path cmdp = dir / "CMD.KEY";
    {
        std::ofstream o(cmdp, std::ios::binary);
        o << cmd;
    }
    const fs::path old = fs::current_path();
    fs::current_path(dir);
    const fs::path cap = fs::temp_directory_path() / keytest::tmp_name("p805_stdout.txt");
    DB_STEP_DATA* m = new DB_STEP_DATA(3); // leaked like the shared runner's
    std::fflush(stdout);
    const int saved = _dup(_fileno(stdout));
    std::FILE* t = std::fopen(cap.string().c_str(), "wb");
    _dup2(_fileno(t), _fileno(stdout));
    {
        Hooks h(m);
        h.seed_key = seed_key;
        h.dbread_seed = dbread_seed;
        std::FILE* f = std::fopen("CMD.KEY", "r");
        def::key::read_key_stream(f, h, stdout);
        std::fclose(f);
    }
    std::fflush(stdout);
    _dup2(saved, _fileno(stdout));
    _close(saved);
    std::fclose(t);
    fs::current_path(old);
    Run r;
    r.msgs = slurp(cap);
    std::error_code ec;
    fs::remove(cap, ec);
    for (auto& e : fs::directory_iterator(dir)) {
        const std::string n = e.path().filename().string();
        if (n == "CMD.KEY" || input_names.count(n) != 0) continue;
        r.files[n] = slurp(e.path());
    }
    return r;
}

std::string run_case(const std::string& file)
{
    const fs::path cj = cmdoracle::Runner::repo_root() / "tests" / "commands" / "BRDEXT" / file;
    const std::string label = json_string(slurp(cj), "label");
    auto it = cache().find(label);
    if (it == cache().end()) return "SKIP no oracle entry for '" + label + "'";
    const Entry& e = it->second;
    // inputs: CMD.KEY text and one data file
    const std::size_t ip = e.json.find("\"inputs\"");
    const std::string cmd = json_text(e.json, ip);
    std::string data_name, data_src;
    {
        std::size_t end = 0;
        std::size_t p = e.json.find("\"name\": \"", ip);
        (void)json_string(e.json, "name", p, &end); // CMD.KEY
        p = e.json.find("\"name\": \"", end);
        data_name = json_string(e.json, "name", p, &end);
        data_src = json_string(e.json, "source", end, &end);
    }
    const bool is_db = data_name.size() > 3 && data_name.substr(data_name.size() - 3) == ".DB";
    const fs::path dir = fs::temp_directory_path() / ("p805_brdext_" + std::to_string(_getpid()) + "_" + fs::path(file).stem().string());
    std::error_code ec;
    fs::remove_all(dir, ec);
    fs::create_directories(dir);
    fs::copy_file(data_src, dir / data_name, fs::copy_options::overwrite_existing);
    fs::path seed_key = dir / data_name;
    fs::path dbread_seed;
    if (is_db) {
        // 3D: DEFINT 3D fails on this PC in the original too (DEF_REM lacks mpich.dll) and leaves
        // the model as DBREAD read it
        const std::string seed_label = cmd.find("COG_P1_B1") != std::string::npos
                                           ? "DEFINT obj1 3D (DEF_REM3_GUI)"
                                           : "DEFFRZ unknown source object";
        auto s = cache().find(seed_label);
        if (s == cache().end()) return "SKIP no DBREAD seed (oracle '" + seed_label + "')";
        dbread_seed = fs::temp_directory_path() / keytest::tmp_name("p805_dbseed.key");
        fs::copy_file(s->second.dir / "files" / "OUT.KEY", dbread_seed, fs::copy_options::overwrite_existing);
    }
    const Run port = replay(dir, cmd, seed_key, dbread_seed, {data_name});
    std::string report;
    // messages
    const auto pm = messages(port.msgs);
    const auto om = messages(slurp(e.dir / "stdout.txt"));
    if (pm != om) {
        report += "  messages differ\n";
        for (auto& l : om) report += "    orig: " + l + "\n";
        for (auto& l : pm) report += "    port: " + l + "\n";
    }
    // files left (oracle: every file under files/)
    std::map<std::string, std::string> of;
    for (auto& f : fs::directory_iterator(e.dir / "files")) of[f.path().filename().string()] = slurp(f.path());
    for (auto& [n, b] : of)
        if (n != "OUT.KEY" && (port.files.count(n) == 0 || port.files.at(n) != b)) report += "  file " + n + (port.files.count(n) ? " differs\n" : " missing\n");
    for (auto& [n, b] : port.files)
        if (of.count(n) == 0) report += "  extra file " + n + "\n";
    // OUT.KEY records, with baseline gaps of the seed round trip
    std::set<std::pair<int, std::string>> gaps;
    {
        const fs::path bdir = dir / "base";
        fs::create_directories(bdir);
        const fs::path seed = is_db ? dbread_seed : seed_key;
        const std::string bcmd = "KFREAD\nSEED.KEY\nKFWRIT\nOUT.KEY\n";
        const Run base = replay(bdir, bcmd, seed, seed, {});
        // reference: the original's own KFREAD + KFWRIT of the seed (oracle "_baseline/<stem>");
        // the DBREAD seed is already such an output
        std::string ref = slurp(seed);
        if (!is_db) {
            auto bl = cache().find("_baseline/" + fs::path(data_name).stem().string());
            if (bl == cache().end()) return "SKIP no oracle baseline for " + data_name;
            ref = slurp(bl->second.dir / "files" / "OUT.KEY");
        }
        auto a = cmdoracle::object_records(ref);
        auto b = cmdoracle::object_records(base.files.count("OUT.KEY") ? base.files.at("OUT.KEY") : "");
        for (auto& [k, v] : a)
            if (b[k] != v) gaps.insert(k);
        for (auto& [k, v] : b)
            if (a[k] != v) gaps.insert(k);
    }
    auto a = cmdoracle::object_records(of.count("OUT.KEY") ? of["OUT.KEY"] : "");
    auto b = cmdoracle::object_records(port.files.count("OUT.KEY") ? port.files.at("OUT.KEY") : "");
    std::set<std::pair<int, std::string>> keys;
    for (auto& [k, v] : a) keys.insert(k);
    for (auto& [k, v] : b) keys.insert(k);
    int equal = 0, differ = 0, ngap = 0;
    for (auto& k : keys) {
        if (a[k] == b[k]) {
            ++equal;
        } else if (gaps.count(k) != 0) {
            ++ngap;
        } else {
            ++differ;
            report += "  record " + k.second + " object " + std::to_string(k.first) + " differs\n";
            if (cmdoracle::env("CMD_ORACLE_VERBOSE", "0") == "1") {
                report += "    orig:\n";
                for (auto& s : a[k]) report += s;
                report += "    port:\n";
                for (auto& s : b[k]) report += s;
            }
        }
    }
    if (cmdoracle::env("CMD_ORACLE_GAPS", "0") == "1") {
        std::string g = "  baseline gaps:";
        for (auto& k : gaps) g += " " + k.second + "/" + std::to_string(k.first);
        std::printf("%s\n", g.c_str());
    }
    if (dbread_seed != fs::path()) fs::remove(dbread_seed, ec);
    if (report.empty()) fs::remove_all(dir, ec);
    std::printf("%s: %d records equal, %d baseline gaps\n", file.c_str(), equal, ngap);
    return report;
}

void check_case(const char* file)
{
    _putenv_s("DEFORM_PC_PATH", kInstall);
    const std::string r = run_case(file);
    if (r.rfind("SKIP", 0) == 0) {
        std::printf("%s: %s\n", file, r.c_str());
        return;
    }
    if (!r.empty()) std::printf("%s FAILED:\n%s", file, r.c_str());
    CHECK(r.empty());
}

} // namespace

TEST_CASE(brdext_no_db) { check_case("brdext_no_db.json"); }
TEST_CASE(brdext_2d_workpiece) { check_case("brdext_2d_workpiece.json"); }
TEST_CASE(brdext_2d_die) { check_case("brdext_2d_die.json"); }
TEST_CASE(brdext_prefix_lookup) { check_case("brdext_prefix_lookup.json"); }
TEST_CASE(brdext_3d_cog) { check_case("brdext_3d_cog.json"); }

int main() { return testkit::run_all(); }
