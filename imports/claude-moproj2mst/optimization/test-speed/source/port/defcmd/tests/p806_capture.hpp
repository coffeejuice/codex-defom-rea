// p8-06 test support: replay of capture runs (tools/extprobe/make_wrappers.py) of the original.
// A capture case's oracle entry holds files/_ext_capture/<n>_3D_<PROG>/{before,after,args.txt}: the
// working directory before and after the n-th launch of PROG. system_hook (for
// def::cmd::g_launch_system_hook) checks the directory against "before" byte for byte, then makes
// it equal to "after", so a command runs without the external programs.
#pragma once

#include "cmd_oracle_runner.hpp"

namespace p806cap {

namespace fs = std::filesystem;

inline fs::path capture_cache()
{
    std::string c = cmdoracle::env("P806_CAPTURE_CACHE", "");
    if (c.empty()) c = "C:/Users/alext/Projects/deform-rea-work/out/p8mg/p8-06/capcache";
    return c;
}

// newest entry of `label` in the capture cache
inline fs::path find_entry(const std::string& label)
{
    fs::path best;
    std::string best_t;
    const fs::path e = capture_cache() / "entries";
    if (!fs::exists(e)) return best;
    for (auto& d1 : fs::directory_iterator(e)) {
        if (!d1.is_directory()) continue;
        for (auto& d2 : fs::directory_iterator(d1.path())) {
            const fs::path rj = d2.path() / "result.json";
            if (!fs::exists(rj)) continue;
            const std::string j = cmdoracle::slurp(rj);
            if (cmdoracle::json_string(j, "label") != label) continue;
            const std::string t = cmdoracle::json_string(j, "created");
            if (best.empty() || t > best_t) {
                best = d2.path();
                best_t = t;
            }
        }
    }
    return best;
}

inline std::set<std::string> listing(const fs::path& d)
{
    std::set<std::string> s;
    for (auto& e : fs::directory_iterator(d))
        if (e.is_regular_file()) s.insert(e.path().filename().string());
    return s;
}

struct Replay {
    fs::path capture;  // <entry>/files/_ext_capture
    std::map<std::string, int> count;
    std::vector<std::string> launched;
    std::string problems;
};
inline Replay* g_replay = nullptr;

inline void compare_dir(const fs::path& want, const std::string& what)
{
    const std::set<std::string> a = listing(want), b = listing(fs::current_path());
    // NEWEQ.MSG (empty) is opened by main's start-up 14001ce7b (p5-01), which the runner does not run
    for (auto& n : a)
        if (b.count(n) == 0 && n != "NEWEQ.MSG") g_replay->problems += "  " + what + ": missing " + n + "\n";
    for (auto& n : b)
        if (a.count(n) == 0) g_replay->problems += "  " + what + ": extra " + n + "\n";
    for (auto& n : a)
        if (b.count(n) != 0 && cmdoracle::slurp(want / n) != cmdoracle::slurp(n))
            g_replay->problems += "  " + what + ": " + n + " differs\n";
}

inline int system_hook(const char* command)
{
    std::string prog;
    for (const char* p : {"MESH3D_GUI", "BRKMSH", "CHKVOL", "DEF_AMG3_GUI"})
        if (std::strstr(command, p) != nullptr) {
            prog = p;
            break;
        }
    g_replay->launched.push_back(prog);
    const int k = ++g_replay->count[prog];
    const fs::path d = g_replay->capture / (std::to_string(k) + "_3D_" + prog);
    if (prog.empty() || !fs::exists(d)) {
        g_replay->problems += std::string("  unexpected launch: ") + command + "\n";
        return 0;
    }
    // cmd performs '> FILE' before the program starts: the capture's "before" copy has it, empty
    if (const char* gt = std::strchr(command, '>')) {
        std::string t = gt + 1;
        while (!t.empty() && t.front() == ' ') t.erase(0, 1);
        while (!t.empty() && (t.back() == '"' || t.back() == ' ')) t.pop_back();
        std::FILE* r = std::fopen(t.c_str(), "wb");
        if (r != nullptr) std::fclose(r);
    }
    compare_dir(d / "before", "before " + prog);
    for (auto& n : listing(fs::current_path()))
        if (!fs::exists(d / "after" / n)) fs::remove(n);
    for (auto& n : listing(d / "after")) fs::copy_file(d / "after" / n, n, fs::copy_options::overwrite_existing);
    return 0;
}


inline std::string records(const std::string& text, int obj, const char* kw)
{
    cmdoracle::RecordMap r = cmdoracle::object_records(text);
    std::string s;
    for (auto& t : r[{obj, kw}]) s += t;
    return s;
}

} // namespace p806cap
