// p8_08_oracle_util.hpp: replay of the DEFINT / DEFFRZ / REMESH oracle cases (tests/commands/<CMD>)
// through the shared runner cmd_oracle_runner.hpp, which it wraps without changes.
//
// The cases of these commands keep CMD.KEY inline ("text") and list it before the seed, while the
// runner expects the seed first and CMD.KEY as a file ("source"). For each case this helper
// builds a private cache (temp folder with the PID) holding a copy of the oracle entry whose
// result.json names the seed first and CMD.KEY as a file, plus a copy of the seed's baseline
// entry, and runs the runner on it. Cases whose CMD.KEY starts with DBREAD need the DB step reader
// (1401cc2a0, defdbio/dbread/dbread.hpp) and are skipped where that is not in the tree.
#pragma once

#include "cmd_oracle_runner.hpp"

#include <process.h>

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include <defcmd/cmd_p8_08.hpp>

namespace p808oracle {

namespace fs = std::filesystem;
using cmdoracle::json_string;
using cmdoracle::slurp;

struct Entry {
    fs::path dir;
    std::string json;
};

inline fs::path real_cache()
{
    std::string c = cmdoracle::env("DEFORM_ORACLE_CACHE", "");
    return c.empty() ? fs::path(cmdoracle::env("USERPROFILE", "")) / ".deform-oracle" : fs::path(c);
}

// newest entry of `label` in the real cache (empty dir when missing)
inline Entry find_entry(const std::string& label)
{
    Entry best;
    const fs::path root = real_cache() / "entries";
    if (!fs::exists(root)) return best;
    for (auto& d1 : fs::directory_iterator(root)) {
        if (!d1.is_directory()) continue;
        for (auto& d2 : fs::directory_iterator(d1.path())) {
            const fs::path rj = d2.path() / "result.json";
            if (!fs::exists(rj)) continue;
            std::string j = slurp(rj);
            if (json_string(j, "label") != label) continue;
            if (!best.dir.empty() && json_string(j, "created") < json_string(best.json, "created")) continue;
            best = Entry{d2.path(), j};
        }
    }
    return best;
}

inline std::string json_escape(const std::string& s)
{
    std::string o;
    for (char c : s) {
        if (c == '\\' || c == '"') o += '\\';
        o += c;
    }
    return o;
}

struct CaseResult {
    std::string label;
    bool skipped = false;
    std::string why;
    cmdoracle::Result res;
};

inline bool is_message(const std::string& l)
{
    // GENDB at the end of the DB cases is skipped by the shared runner; its check messages
    // (14018f0b0 and the step header) are not compared
    for (const char* g : {"INFO   : Checking ", "INFO   : Simulation Type", "INFO   : Input data has",
                          "INFO   : DEFORM Database Generated"})
        if (l.rfind(g, 0) == 0) return false;
    return l.rfind("INFO   :", 0) == 0 || l.rfind("         Skip", 0) == 0 || l.rfind("         Data", 0) == 0 ||
           l.rfind("         Force", 0) == 0 || l.rfind("         Success", 0) == 0 ||
           l.rfind("         Failure", 0) == 0 || l.rfind("ERROR: Keyword", 0) == 0 ||
           l.rfind("Error in _objectInterpolation", 0) == 0;
}

inline int command(def::model::DB_STEP_DATA& m, std::FILE* f, const std::string& name)
{
    if (name == "DEFINT") return def::cmd::cmd_defint(&m, f);
    if (name == "DEFFRZ") return def::cmd::cmd_deffrz(&m, f);
    if (name == "REMESH") return def::cmd::cmd_remesh(&m, f);
    return -1;
}

// Replays the oracle case `label` (the "label" of its JSON in tests/commands/<CMD>/).
inline CaseResult run_case(const std::string& label)
{
    CaseResult r;
    r.label = label;
    const Entry e = find_entry(label);
    if (e.dir.empty()) {
        r.skipped = true;
        r.why = "no oracle run in the cache";
        return r;
    }
    // inputs: CMD.KEY (text), the seed (the first source) and any other inline-text inputs (e.g. an
    // empty 64bit.DAT, which makes the 3D wrappers run their 64-bit programs)
    std::string cmd_text, seed_name, seed_src;
    std::vector<cmdoracle::CaseInput> extra;
    for (const cmdoracle::CaseInput& in : cmdoracle::case_inputs(e.json)) {
        if (in.name == "CMD.KEY") continue;
        if (!in.inline_text && seed_src.empty()) {
            seed_src = in.source;
            // the runner finds the baseline by the seed's name: use the source's file name
            // (the cases copy e.g. NAFEMS_T2_EN.KEY to IN.KEY; KFREAD reads the copy either way)
            seed_name = fs::path(seed_src).filename().string();
        } else {
            extra.push_back(in);
        }
    }
    // json_string turns "\n" into "n": re-read the raw text and decode \n, \\ and \" here
    {
        std::size_t p = e.json.find("\"text\": \"");
        std::string t;
        if (p != std::string::npos) {
            p += 9;
            while (p < e.json.size() && e.json[p] != '"') {
                if (e.json[p] == '\\' && p + 1 < e.json.size()) {
                    ++p;
                    t += e.json[p] == 'n' ? '\n' : e.json[p];
                } else {
                    t += e.json[p];
                }
                ++p;
            }
        }
        cmd_text = t;
    }
#if !__has_include(<defdbio/dbread/dbread.hpp>)
    if (cmd_text.find("DBREAD") != std::string::npos) {
        r.skipped = true;
        r.why = "needs DBREAD (DB step reader 1401cc2a0, not on this branch) and the DB copy; integration case";
        return r;
    }
#endif
#if !__has_include(<defcmd/cmd_p8_07.hpp>)
    // REMESH on an existing object runs DEFAMG (p8-07) after BRDEXT (p8-05)
    if (cmd_text.find("REMESH       1") != std::string::npos) {
        r.skipped = true;
        r.why = "needs p8-07 defamg_object 1401a9be0 (pending stub on this branch)";
        return r;
    }
#endif
    // the external programs come from the DEFORM install (common.md, launcher 140223790)
    _putenv_s("DEFORM_PC_PATH", "C:\\Program Files\\SFTC\\DEFORM\\v11.0.2\\");
    // KFREAD reads the seed under the source's file name, so that the runner finds the seed's
    // baseline ("_baseline/<stem>"); the cases copy e.g. NAFEMS_T2_EN.KEY to IN.KEY
    for (const char* kf : {"KFREAD\n", "KFREAD 0\n"}) {
        const std::size_t k = cmd_text.find(kf);
        if (k == std::string::npos) continue;
        const std::size_t a = k + std::strlen(kf);
        const std::size_t b = cmd_text.find('\n', a);
        if (b != std::string::npos) cmd_text.replace(a, b - a, seed_name);
        break;
    }
    const fs::path cache = fs::temp_directory_path() / ("p808_cache_" + std::to_string(_getpid()));
    std::error_code ec;
    fs::remove_all(cache, ec);
    const fs::path ent = cache / "entries" / "00" / "case";
    fs::create_directories(ent / "files");
    const fs::path cmd = cache / "CMD.KEY";
    {
        std::ofstream o(cmd, std::ios::binary);
        o << cmd_text;
    }
    {
        std::ofstream o(ent / "result.json", std::ios::binary);
        o << "{\n \"label\": \"" << json_escape(label) << "\",\n \"created\": \"" << json_string(e.json, "created")
          << "\",\n \"inputs\": [\n  {\n   \"name\": \"" << json_escape(seed_name) << "\",\n   \"source\": \""
          << json_escape(seed_src) << "\"\n  },\n  {\n   \"name\": \"CMD.KEY\",\n   \"source\": \""
          << json_escape(cmd.string()) << "\"\n  }";
        for (const cmdoracle::CaseInput& in : extra)
            o << ",\n  {\n   \"name\": \"" << json_escape(in.name) << "\",\n   \"text\": \"" << json_escape(in.text) << "\"\n  }";
        o << "\n ]\n}\n";
    }
    fs::copy_file(e.dir / "stdout.txt", ent / "stdout.txt");
    if (fs::exists(e.dir / "files" / "OUT.KEY")) fs::copy_file(e.dir / "files" / "OUT.KEY", ent / "files" / "OUT.KEY");
    const Entry b = find_entry("_baseline/" + fs::path(seed_name).stem().string());
    if (!b.dir.empty()) fs::copy(b.dir, cache / "entries" / "00" / "base", fs::copy_options::recursive);
    _putenv_s("DEFORM_ORACLE_CACHE", cache.string().c_str());
    const fs::path old = fs::current_path();
    fs::create_directories(cache / "cwd");
    fs::current_path(cache / "cwd");  // the commands create and delete exchange files in the cwd
    {
        cmdoracle::Runner runner;
        cmdoracle::Options o;
        // DBREAD cases go through the dispatcher (p5-01), which also reaches DEFINT / DEFFRZ / REMESH
        if (cmd_text.find("DBREAD") == std::string::npos) o.command = command;
        o.message = is_message;
        r.res = runner.run(label, o);
    }
    fs::current_path(old);
    _putenv_s("DEFORM_ORACLE_CACHE", "");
    fs::remove_all(cache, ec);
    return r;
}

} // namespace p808oracle
