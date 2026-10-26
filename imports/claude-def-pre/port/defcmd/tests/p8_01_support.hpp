// defcmd/tests/p8_01_support.hpp: test support of packet p8-01 (GFREAD driver and the small
// commands EXECMD, GENINT, IMPLUB, IMPEQP, ARMCTL, MORP).
//
// Oracle replays go through cmdoracle::Runner (cmd_oracle_runner.hpp): it places the case's inputs
// in a fresh working directory, reads CMD.KEY and compares the messages and the group 2 records.
// The command keywords are routed through the dispatcher def::cmd::dispatch (p5-01), which calls
// the p8-01 entry points; the routing callback also keeps, after each command, the model pointer
// and the files the command left in the working directory, so that the test can compare what the
// runner does not: the inter-object (group 3) records of OUT.KEY, BCCDEF (its writer is not merged
// yet; formatted here from the array as BCCDEF.md "KEY writer" prints it) and the files the
// commands write (DEF_MORP2.INI, DEF_ARM_CTL.DAT, ...), byte for byte.
//
// Seams to other packets get test hooks: the KEY file loader 1401b5f00 (p8-07, not published) is a
// stand-in with its steps (GENINT.md section 4) on p5-01's ModelKeyHooks; the launcher (p8-07)
// and system() record their calls; the GFREAD 3D table (p8-02) answers the no-op types and flags
// the reader types as blocked.
#pragma once

#include "cmd_oracle_runner.hpp"

#include <defcmd/cmd_p8_01.hpp>
#include <defcmd/dispatch.hpp>
#include <defcmd/gfread.hpp>
#include <defkey/kw_g3/kw_g3.hpp>
#include <defmodel/keyword_registry.hpp>
#include <defmodel/step/db_step.hpp>

#include "../src/p8_01/ext_p8_02.hpp"
#include "../src/p8_01/ext_p8_07.hpp"

#include <tuple>

namespace p801test {

namespace fs = std::filesystem;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;

// ---- calls recorded by the hooks ----------------------------------------------------------------
struct Calls {
    std::vector<std::tuple<int, std::string, int>> launches;  // mode, command line, dirKind
    std::vector<std::string> systems;                        // EXECMD mode 1
    std::vector<int> blocked_3d;                             // 3D table types that need p8-02
    bool run_echo = false;                                   // let system("echo ...") run
    DB_STEP_DATA* model = nullptr;                           // model of the last command
    std::map<std::string, std::string> files;                // working-directory files after it
};
inline Calls g_calls;

inline void hook_launch(int mode, const char* cmd, int dir) { g_calls.launches.emplace_back(mode, cmd, dir); }
inline int hook_system(const char* cmd)
{
    g_calls.systems.emplace_back(cmd);
    if (g_calls.run_echo && std::strncmp(cmd, "echo ", 5) == 0) return std::system(cmd);
    return 0;
}

// p8-02's 3D table, for the types it does nothing on (GFREAD.md section 3: 0, 1, 6, 7, other ->
// success); the readers (2 GEO, 3 STL, 4 PDA, 5 UNV, 10 NAS) are p8-02's / p8-09's.
inline bool hook_load_3d(def::cmd::gfread::Loader*, const char*, LLOBJ_DATA*, std::int32_t type)
{
    if (type == 2 || type == 3 || type == 4 || type == 5 || type == 10) {
        g_calls.blocked_3d.push_back(type);
        return false;
    }
    return true;
}

// Stand-in for FUN_1401b5f00(model, name, &flag, &mode) (p8-07's; GENINT.md section 4): flag ->
// 1401d7f10 before the read, name into model+0x2f8a, fopen "r" (failure: message, 1), mode into
// model+0x1740 for the read, the KEY interpreter, 0 again, " " into model+0x2f8a, flag ->
// 1401d7f40 (p5-01's finish_key_read) after it.
inline int hook_read_key_file(DB_STEP_DATA* m, const char* name, const char* flag, const std::int32_t* mode)
{
    if (*flag != 0) def::model::step_clear_u2f88(*m);
    char* store = reinterpret_cast<char*>(m) + 0x2f8a;
    std::strcpy(store, name);
    std::FILE* f = std::fopen(name, "r");
    if (f == nullptr) {
        std::fprintf(stdout, "ERROR: Keyword file %s open error!\n", name);
        return 1;
    }
    m->u1740 = *mode;
    def::cmd::ModelKeyHooks h(m);
    const int rc = def::key::read_key_stream(f, h, stdout);
    m->u1740 = 0;
    std::fclose(f);
    std::strcpy(store, " ");
    if (*flag != 0) def::cmd::finish_key_read(m);
    return rc;
}

inline void install_hooks()
{
    // P801_REAL_LAUNCH=1 (with DEFORM_PC_PATH = the DEFORM install): the owner's launcher runs the
    // programs (an integration build with p8-07); the launch-call checks then see no calls
    def::cmd::p801ext::g_launch_hook = cmdoracle::env("P801_REAL_LAUNCH", "0") == "1" ? nullptr : hook_launch;
    def::cmd::p801ext::g_system_hook = hook_system;
    def::cmd::p801ext::g_read_key_file_hook = hook_read_key_file;
#ifndef DEF_P801_HAVE_P8_02
    def::cmd::gfread::ext::g_load_3d_hook = hook_load_3d;
#endif
}

// The runner's command callback: the dispatcher, then the bookkeeping described at the top.
inline int route_command(DB_STEP_DATA& m, std::FILE* f, const std::string& name)
{
    const def::model::KeywordRecord* k = def::model::find_keyword(name.c_str());
    if (k == nullptr) return -1;
    const int rc = def::cmd::dispatch(&m, f, k->id);
    g_calls.model = &m;
    g_calls.files.clear();
    for (auto& e : fs::directory_iterator(fs::current_path())) {
        if (!e.is_regular_file()) continue;
        g_calls.files[e.path().filename().string()] = cmdoracle::slurp(e.path());
    }
    return rc;
}

// ---- comparisons the runner does not make -------------------------------------------------------

// The cache entry directory of a label (the newest run wins, as in the runner).
class EntryDirs {
public:
    EntryDirs()
    {
        fs::path c = cmdoracle::env("DEFORM_ORACLE_CACHE", "");
        if (c.empty()) c = fs::path(cmdoracle::env("USERPROFILE", "")) / ".deform-oracle";
        if (!fs::exists(c / "entries")) return;
        for (auto& d1 : fs::directory_iterator(c / "entries")) {
            if (!d1.is_directory()) continue;
            for (auto& d2 : fs::directory_iterator(d1.path())) {
                const fs::path rj = d2.path() / "result.json";
                if (!fs::exists(rj)) continue;
                const std::string j = cmdoracle::slurp(rj);
                const std::string label = cmdoracle::json_string(j, "label");
                const std::string created = cmdoracle::json_string(j, "created");
                auto it = e_.find(label);
                if (it != e_.end() && created < it->second.second) continue;
                e_[label] = {d2.path(), created};
            }
        }
    }
    fs::path find(const std::string& label) const
    {
        auto it = e_.find(label);
        return it == e_.end() ? fs::path() : it->second.first;
    }

private:
    std::map<std::string, std::pair<fs::path, std::string>> e_;
};

// group 3 records in order (first token a registry group 3 name, continuation lines start blank)
inline std::vector<std::string> inter_records(const std::string& text)
{
    std::vector<std::string> r;
    std::vector<std::string> ls = cmdoracle::lines_of(text);
    for (std::size_t i = 0; i < ls.size(); ++i) {
        const std::string& l = ls[i];
        if (l.empty() || l[0] == ' ' || l[0] == '*' || l[0] == '\t') continue;
        char name[64] = {0};
        if (std::sscanf(l.c_str(), "%63s", name) < 1) continue;
        const def::model::KeywordRecord* k = def::model::find_keyword(name);
        if (k == nullptr || k->group != def::model::KeywordGroup::InterObject) continue;
        std::string t = l + "\n";
        std::size_t j = i + 1;
        while (j < ls.size() && !ls[j].empty() && ls[j][0] == ' ') t += ls[j++] + "\n";
        r.push_back(t);
        i = j - 1;
    }
    return r;
}

// BCCDEF of one object as the KEY writer prints it (BCCDEF.md "KEY writer": FUN_1402f4df0 with
// default 0); "" when the writer writes nothing.
inline std::string bccdef_record(DB_STEP_DATA* m, LLOBJ_DATA* obj)
{
    auto at = [](void* p, std::size_t off) { return static_cast<unsigned char*>(p) + off; };
    const std::int32_t type = *reinterpret_cast<std::int32_t*>(at(obj, 0xc));
    if (type == 7 || type == 5) return {};
    const std::int32_t n = *reinterpret_cast<std::int32_t*>(at(obj, 0x880));
    const std::int32_t w = *reinterpret_cast<std::int32_t*>(at(m, 0x1640));
    const std::int32_t* a = *reinterpret_cast<std::int32_t**>(at(obj, 0x990));
    if (n == 0 || w == 0 || a == nullptr) return {};
    std::int32_t no = *reinterpret_cast<std::int32_t*>(at(obj, 0x3f24));
    if (no <= 0) no = *reinterpret_cast<std::int32_t*>(at(obj, 8));
    char b[128];
    bool uniform = true;
    std::int32_t k = 0;
    for (std::int32_t i = 0; i < n; ++i) {
        bool nz = false;
        for (std::int32_t j = 0; j < w; ++j) {
            if (a[i * w + j] != a[0]) uniform = false;
            if (a[i * w + j] != 0) nz = true;
        }
        if (nz) ++k;
    }
    if (uniform) {
        std::snprintf(b, sizeof b, "%-6s %7i %7i %7i\n", "BCCDEF", no, 0, a[0]);
        return b;
    }
    std::snprintf(b, sizeof b, "%-6s %7i %7i %7i\n", "BCCDEF", no, k, 0);
    std::string r = b;
    for (std::int32_t i = 0; i < n; ++i) {
        bool nz = false;
        for (std::int32_t j = 0; j < w; ++j) nz = nz || a[i * w + j] != 0;
        if (!nz) continue;
        std::snprintf(b, sizeof b, " %7i", i + 1);
        r += b;
        for (std::int32_t j = 0; j < w; ++j) {
            std::snprintf(b, sizeof b, " %7i", a[i * w + j]);
            r += b;
        }
        r += "\n";
    }
    return r;
}

struct Extra {
    bool pass = true;
    int equal = 0, differ = 0;
    std::string report;
};

// After a Runner::run of the case in `dir`: inter-object records, BCCDEF and the files the
// commands wrote. `inputs`: names placed by the runner (not compared); `external`: files the
// launched programs write in the original (the launch itself is checked through the calls).
inline Extra compare_extra(const fs::path& dir, const std::set<std::string>& inputs, const std::set<std::string>& external)
{
    Extra x;
    const bool verbose = cmdoracle::env("CMD_ORACLE_VERBOSE", "0") == "1";
    const std::string oout = cmdoracle::slurp(dir / "files" / "OUT.KEY");
    DB_STEP_DATA* m = g_calls.model;
    if (!oout.empty() && m != nullptr) {
        // inter-object pairs in order
        const fs::path tmp = fs::temp_directory_path() / keytest::tmp_name("p801_inter.key");
        std::FILE* f = std::fopen(tmp.string().c_str(), "wb");
        def::key::g3::write_inter_objects(m, f);
        std::fclose(f);
        const std::vector<std::string> ia = inter_records(oout), ib = inter_records(cmdoracle::slurp(tmp));
        std::error_code ec;
        fs::remove(tmp, ec);
        const std::size_t n = std::max(ia.size(), ib.size());
        for (std::size_t i = 0; i < n; ++i) {
            const std::string a = i < ia.size() ? ia[i] : "(none)\n", b = i < ib.size() ? ib[i] : "(none)\n";
            if (a == b) {
                ++x.equal;
                continue;
            }
            ++x.differ;
            x.report += "  inter-object record " + std::to_string(i) + " differs\n    orig: " + a + "    port: " + b;
        }
        // BCCDEF of every object
        const cmdoracle::RecordMap recs = cmdoracle::object_records(oout);
        for (LLOBJ_DATA* ob = *reinterpret_cast<LLOBJ_DATA**>(reinterpret_cast<unsigned char*>(m) + 0x1380); ob != nullptr;
             ob = *reinterpret_cast<LLOBJ_DATA**>(reinterpret_cast<unsigned char*>(ob) + 0x3940)) {
            const std::string mine = bccdef_record(m, ob);
            if (mine.empty()) continue;
            int no = 0;
            std::sscanf(mine.c_str() + 6, "%i", &no);
            auto it = recs.find({no, "BCCDEF"});
            const std::string theirs = it == recs.end() || it->second.empty() ? std::string() : it->second[0];
            if (mine == theirs) {
                ++x.equal;
                continue;
            }
            ++x.differ;
            x.report += "  BCCDEF (formatted from the array) object " + std::to_string(no) + " differs\n";
            if (verbose) x.report += "    orig:\n" + theirs + "    port:\n" + mine;
        }
    }
    // files the commands wrote
    std::map<std::string, std::string> of;
    if (fs::exists(dir / "files"))
        for (auto& e : fs::recursive_directory_iterator(dir / "files")) {
            if (!e.is_regular_file()) continue;
            const std::string name = fs::relative(e.path(), dir / "files").generic_string();
            if (name != "OUT.KEY") of[name] = cmdoracle::slurp(e.path());
        }
    for (auto& [n, bytes] : of) {
        if (external.count(n) != 0) continue;
        auto it = g_calls.files.find(n);
        if (it == g_calls.files.end()) {
            x.pass = false;
            x.report += "  file " + n + " not written by the port\n";
        } else if (it->second != bytes) {
            x.pass = false;
            x.report += "  file " + n + " differs\n";
        }
    }
    for (auto& [n, bytes] : g_calls.files)
        if (of.count(n) == 0 && external.count(n) == 0 && inputs.count(n) == 0) {
            x.pass = false;
            x.report += "  file " + n + " written by the port only\n";
        }
    x.pass = x.pass && x.differ == 0;
    return x;
}

} // namespace p801test
