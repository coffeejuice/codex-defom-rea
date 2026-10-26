// ctest-labels: p67-5 commands
// GENSUB against the original: every case of tests/commands/GENSUB/cases.json runs through the
// port's KEY loop in its own scratch directory (GENSUB uses the fixed names AX_BIL.KEY, MESH.DAT,
// DEF_SUB.INI, NEWBRICK.KEY, DEF_CTRL.OUT), and the files the original left in its run directory
// (MESH.DAT, DEF_SUB.INI, DEF_CTRL.OUT, AX_BIL.KEY) are compared byte for byte, presence and
// content, plus the GENSUB lines of the keyword-phase stdout ("ERROR: Keyword GENSUB Reading
// Error. ").
//
// AX_BIL.KEY is the object block of the ported writer FUN_14029e250 (kw-g2obj). Until kw-g2sv's
// state-variable writer is linked (<defkey/kw_g2sv.hpp>) that block lacks the state-variable
// records (DRZ, NDTMP, STRAIN, BCCDEF, ...), so AX_BIL.KEY and the MESH.DAT made from it differ
// end to end. The converter is then checked on the original's own input instead:
//   * the original's AX_BIL.KEY where it was left (brick2, tet3d), and
//   * a reconstruction elsewhere: the object's block of the original's OUT.KEY (KFWRIT writes the
//     same block with the same writer; GENSUB does not change the model). The reconstruction is
//     verified to be byte-identical to the original's AX_BIL.KEY in the cases that kept it.
// Such a case is reported "partial (needs kw-g2sv)" when every other output matches, the port's
// block differs from the original's only by records the port does not write at all, and the
// converter reproduces MESH.DAT / DEF_CTRL.OUT from the original's block. With kw-g2sv linked
// every case must match end to end.
//
// Also: the scanf("%f") conversion of the VS2008 CRT (ties rounded down) on hand-computed values.
#include "p67_5_support.hpp"

#include <defcmd/dispatch.hpp>
#include <cstring>

#include <defcmd/p67_5.hpp>
#include <defcmd/p67_5_gensub.hpp>
#include <defrt/stdlib/crt_emulation.hpp>
#include <testkit/check.hpp>

#if __has_include(<defkey/kw_g2sv.hpp>)
constexpr bool kHaveStateVarWriter = true;
#else
constexpr bool kHaveStateVarWriter = false;
#endif

using namespace p675;
namespace gs = def::cmd::gensub;

namespace {

std::uint32_t bits_of(float f)
{
    std::uint32_t u;
    std::memcpy(&u, &f, 4);
    return u;
}

struct AbnormalExit {
    std::uint32_t status;
};
void throw_exit(std::uint32_t status) { throw AbnormalExit{status}; }

std::string gensub_lines(const std::string& text)
{
    std::string out;
    for (const std::string& l : lines_of(text))
        if (l.find("GENSUB") != std::string::npos) out += l + "\n";
    return out;
}

// Present-and-equal check of one file; prints the first differing line when verbose.
bool same_file(const fs::path& a, const fs::path& b, const std::string& what, bool verbose)
{
    const bool ea = fs::exists(a), eb = fs::exists(b);
    if (ea != eb) {
        std::printf("  %s: left by the original: %s, by the port: %s\n", what.c_str(), ea ? "yes" : "no", eb ? "yes" : "no");
        return false;
    }
    if (!ea) return true;
    const std::string x = slurp(a), y = slurp(b);
    if (x == y) return true;
    std::printf("  %s differs (%zu vs %zu bytes)\n", what.c_str(), x.size(), y.size());
    if (verbose) {
        auto la = lines_of(x), lb = lines_of(y);
        for (std::size_t i = 0; i < std::max(la.size(), lb.size()); ++i) {
            const std::string u = i < la.size() ? la[i] : "<none>", v = i < lb.size() ? lb[i] : "<none>";
            if (u != v) {
                std::printf("    line %zu\n     orig: %s\n     port: %s\n", i + 1, u.c_str(), v.c_str());
                break;
            }
        }
    }
    return false;
}

// The record keys that differ between the two texts; `only_missing` is cleared when one of them
// is present in the port's text (i.e. it is not just a record the port does not write).
std::string record_diff(const std::string& orig, const std::string& port, bool* only_missing)
{
    auto ra = records_of(orig), rb = records_of(port);
    std::set<std::string> keys;
    for (auto& [k, v] : ra) keys.insert(k);
    for (auto& [k, v] : rb) keys.insert(k);
    std::string diff;
    for (auto& k : keys) {
        if (ra[k] == rb[k]) continue;
        diff += " " + k.substr(0, k.find(' '));
        if (!rb[k].empty()) *only_missing = false;
    }
    if (diff.empty()) {
        diff = " (text outside records)";
        *only_missing = false;
    }
    return diff;
}

// The block of object `no` in a KEY text as FUN_14029e250 writes it ("*" line, "*  Data for
// Object #", ..., up to the "*" line that opens the next section), bytes unchanged.
std::string object_block(const std::string& key, int no)
{
    std::vector<std::string> ls;  // lines with their line ends
    for (std::size_t a = 0; a < key.size();) {
        std::size_t b = key.find('\n', a);
        b = b == std::string::npos ? key.size() : b + 1;
        ls.push_back(key.substr(a, b - a));
        a = b;
    }
    auto bare = [&](std::size_t i) {
        std::string s = ls[i];
        while (!s.empty() && (s.back() == '\n' || s.back() == '\r')) s.pop_back();
        return s;
    };
    const std::string tag = "*  Data for Object #";
    for (std::size_t i = 1; i < ls.size(); ++i) {
        const std::string l = bare(i);
        if (l.rfind(tag, 0) != 0 || std::atoi(l.c_str() + tag.size()) != no || bare(i - 1) != "*") continue;
        std::size_t j = i + 1;
        while (j < ls.size() && !(bare(j) == "*" && j + 1 < ls.size() && bare(j + 1).rfind("*  ", 0) == 0)) ++j;
        std::string out;
        for (std::size_t k = i - 1; k < j; ++k) out += ls[k];
        return out;
    }
    return {};
}

// The object number of the first "GENSUB n" with n != 0 in the command file.
int gensub_object(const fs::path& cmd)
{
    for (const std::string& l : lines_of(slurp(cmd))) {
        int n = 0;
        if (std::sscanf(l.c_str(), "GENSUB %d", &n) == 1 && n != 0) return n;
    }
    return 0;
}

// Runs the converter on `key` in `dir` as the dispatcher does (converter, delete AX_BIL.KEY) and
// compares MESH.DAT and DEF_CTRL.OUT with the original's files.
bool converter_matches(const std::string& key, const fs::path& dir, const fs::path& orig, DB_STEP_DATA& m,
                       const std::string& what, bool verbose)
{
    const fs::path home = fs::current_path();
    fs::create_directories(dir);
    fs::current_path(dir);
    spit(dir / "AX_BIL.KEY", key);
    std::int32_t o1 = 0, o2 = 0, o3 = 0, mesh = 1;
    bool ok = true;
    int st = -1;
    try {
        st = gs::convert_key_to_mesh(m, "AX_BIL.KEY", "MESH.DAT", &o1, &o2, &o3, &mesh);
    } catch (const gs::InputHang& h) {
        std::printf("  %s: %s\n", what.c_str(), h.what());
        ok = false;
    }
    gs::close_leaked_files();
    gs::delete_if_exists("AX_BIL.KEY");
    fs::current_path(home);
    for (const char* name : {"MESH.DAT", "DEF_CTRL.OUT"})
        if (!same_file(orig / name, dir / name, what + " " + name, verbose)) ok = false;
    std::printf("  %s: converter status %d, %s\n", what.c_str(), st, ok ? "MESH.DAT / DEF_CTRL.OUT match" : "MISMATCH");
    return ok;
}

} // namespace

TEST_CASE(gensub_crt_float_scan)
{
    CHECK_EQ(bits_of(gs::crt_atoflt("1.0000000000E+001")), bits_of(10.0f));
    CHECK_EQ(bits_of(gs::crt_atoflt("-2.5")), bits_of(-2.5f));
    CHECK_EQ(bits_of(gs::crt_atoflt("0.0000000000E+000")), bits_of(0.0f));
    CHECK_EQ(bits_of(gs::crt_atoflt("1.2000000000E+003")), bits_of(1200.0f));
    // 0.1 = 0x3dcccccd (above the tie, rounds up in any conversion)
    CHECK_EQ(bits_of(gs::crt_atoflt("1.0000000000E-001")), 0x3dcccccdu);
    // exact ties: 16777217 = 2^24 + 1 and 16777219 = 2^24 + 3 lie halfway between two floats.
    // _RoundMan rounds a tie down (16777216 / 16777218); a correctly rounded strtof gives
    // 16777216 / 16777220.
    CHECK_EQ(bits_of(gs::crt_atoflt("16777217")), bits_of(16777216.0f));
    CHECK_EQ(bits_of(gs::crt_atoflt("16777219")), bits_of(16777218.0f));
    CHECK_EQ(bits_of(gs::crt_atoflt("1.6777219000E+007")), bits_of(16777218.0f));
    CHECK_EQ(bits_of(gs::crt_atoflt("1e")), bits_of(1.0f));  // exponent without digits

    int i1 = 0, i2 = 0, i3 = 7;
    float f1 = 0;
    char tok[64];
    {
        void* const a[] = {tok, &i1, &i2, &i3};
        CHECK_EQ(gs::crt_sscanf("ELMCON 1 289\n", "%s %d %d %d", a), 3);  // 2D header: npe untouched
        CHECK_EQ(i3, 7);
    }
    {
        void* const a[] = {&f1};
        CHECK_EQ(gs::crt_sscanf("", "%f", a), -1);
        CHECK_EQ(gs::crt_sscanf("abc", "%f", a), 0);
    }
}

TEST_CASE(gensub_cases_against_the_oracle)
{
    unsigned long long& opt = *__local_stdio_printf_options();
    opt = (opt | _CRT_INTERNAL_PRINTF_LEGACY_THREE_DIGIT_EXPONENTS | _CRT_INTERNAL_PRINTF_LEGACY_MSVCRT_COMPATIBILITY) &
          ~static_cast<unsigned long long>(_CRT_INTERNAL_PRINTF_STANDARD_ROUNDING);
    if (repo_root().empty()) {
        std::printf("no repository root: skipped\n");
        return;
    }
    def::rt::set_abnormal_exit_hook(throw_exit);
    const bool verbose = env("P675_VERBOSE", "0") == "1";
    const int id = kw_id("GENSUB");
    const std::vector<Case> cases = load_cases("GENSUB");  // absolute input paths
    const fs::path home = fs::current_path();
    const fs::path scratch = fs::temp_directory_path() / ("p675_gensub_" + std::to_string(_getpid()));
    std::error_code ec;
    fs::remove_all(scratch, ec);
    fs::create_directories(scratch);
    int pass = 0, partial = 0, fail = 0, missing = 0;
    for (const Case& c : cases) {
        const fs::path e = oracle_entry(c.label);
        if (e.empty()) {
            std::printf("%-16s not in the oracle cache\n", c.label.c_str());
            ++missing;
            continue;
        }
        const fs::path of = e / "files";
        const std::string option = c.label.substr(c.label.find('/') + 1);
        const fs::path dir = scratch / option;
        fs::create_directories(dir);
        fs::current_path(dir);
        gs::oob_reads() = 0;
        auto& m = *new DB_STEP_DATA(3);
        std::string out, problem;
        {
            CmdRunner run(m, c);
            run.on_command = [&](int kid, std::FILE* f) -> int {
                if (kid != id) return -1;
                return def::cmd::dispatch(&m, f, kid);  // FUN_140378e00
            };
            StdoutCapture cap;
            try {
                run.run();
            } catch (const gs::InputHang& h) {
                problem = h.what();
            } catch (const AbnormalExit& x) {
                char b[64];
                std::snprintf(b, sizeof b, "abnormal exit 0x%08X", x.status);
                problem = b;
            }
            out = cap.text();
        }
        gs::close_leaked_files();  // where the original's process ends
        fs::current_path(home);

        bool hard = false;  // a difference not explained by the missing state-variable writer
        if (!problem.empty()) {
            std::printf("  %s: %s\n", c.label.c_str(), problem.c_str());
            hard = true;
        }
        const std::string so = gensub_lines(keyword_phase_stdout(slurp(e / "stdout.txt")));
        const std::string sp = gensub_lines(out);
        if (so != sp) {
            std::printf("  %s: stdout differs\n   orig: %s   port: %s\n", c.label.c_str(), so.c_str(), sp.c_str());
            hard = true;
        }
        for (const char* name : {"DEF_SUB.INI", "DEF_CTRL.OUT"})
            if (!same_file(of / name, dir / name, c.label + " " + name, verbose)) hard = true;
        // presence of MESH.DAT and AX_BIL.KEY is decided by the code paths, not by the values
        for (const char* name : {"MESH.DAT", "AX_BIL.KEY"})
            if (fs::exists(of / name) != fs::exists(dir / name)) {
                std::printf("  %s: %s left by the original: %d, by the port: %d\n", c.label.c_str(), name,
                            int(fs::exists(of / name)), int(fs::exists(dir / name)));
                hard = true;
            }
        const bool mesh_eq = same_file(of / "MESH.DAT", dir / "MESH.DAT", c.label + " MESH.DAT (end to end)", verbose);
        const bool ax_eq = !fs::exists(of / "AX_BIL.KEY") || slurp(of / "AX_BIL.KEY") == slurp(dir / "AX_BIL.KEY");

        bool soft_ok = true;  // the checks that replace the end-to-end ones without kw-g2sv
        const int no = gensub_object(c.inputs.at("CMD.KEY"));
        if (no != 0 && fs::exists(of / "OUT.KEY")) {
            const std::string block = object_block(slurp(of / "OUT.KEY"), no);
            if (fs::exists(of / "AX_BIL.KEY") && block != slurp(of / "AX_BIL.KEY")) {
                std::printf("  %s: reconstruction from OUT.KEY != the original's AX_BIL.KEY\n", c.label.c_str());
                soft_ok = false;
            }
            // the port's block of the same object, from the model it just read
            LLOBJ_DATA* o = def::key::g2obj::find_object(m, no);
            if (!block.empty() && o != nullptr) {
                const fs::path p = scratch / (option + "_port_block.key");
                std::FILE* f = std::fopen(p.string().c_str(), "wt");
                def::key::g2obj::write_object(m, f, o, 0, false);
                std::fclose(f);
                bool only_missing = true;
                const std::string mine = slurp(p);
                if (mine != block) {
                    const std::string d = record_diff(block, mine, &only_missing);
                    std::printf("  %s: AX_BIL.KEY records the port writes differently:%s\n", c.label.c_str(), d.c_str());
                    if (!only_missing) soft_ok = false;
                }
                if (fs::exists(of / "MESH.DAT") || fs::exists(of / "DEF_CTRL.OUT") || fs::exists(of / "AX_BIL.KEY"))
                    if (!converter_matches(block, scratch / (option + "_conv"), of, m, c.label + " converter on the original's block",
                                           verbose))
                        soft_ok = false;
            }
        }
        if (gs::oob_reads() != 0) std::printf("  %s: %d out-of-bounds reads\n", c.label.c_str(), gs::oob_reads());

        const char* verdict;
        if (!hard && mesh_eq && ax_eq && soft_ok) {
            verdict = "pass";
            ++pass;
        } else if (!hard && soft_ok && !kHaveStateVarWriter) {
            verdict = "partial (needs kw-g2sv: AX_BIL.KEY / MESH.DAT end to end)";
            ++partial;
        } else {
            verdict = "MISMATCH";
            ++fail;
        }
        std::printf("%-16s %s\n", c.label.c_str(), verdict);
    }
    fs::current_path(home);
    std::printf("GENSUB: %d pass, %d partial, %d mismatch, %d not in the oracle cache\n", pass, partial, fail, missing);
    if (env("P675_KEEP", "0") != "1") fs::remove_all(scratch, ec);
    CHECK_EQ(fail, 0);
}

int main() { return testkit::run_all(); }
