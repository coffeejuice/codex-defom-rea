// ctest-labels: p67-5 commands
// GENBCC (dispatcher high case 8): every case of tests/commands/GENBCC/cases.json run through the
// port's KEY loop and compared with the original's results in the oracle cache:
//   * the KEY records the port's writer emits for the edge BCs (ECCTMP, ECTMFN, ECCDEF, ECDEFN,
//     LOCTMP), byte for byte;
//   * the nodal BC arrays BCCDEF, BCCDFN, BCCTMP, BCCTFN and URZ of every object, compared on
//     values against the oracle OUT.KEY records (the port's writer of those records belongs to
//     kw-g2sv, not merged yet: "compared on values until kw-g2sv lands");
//   * the stdout lines of the keyword phase that name GENBCC (the port's seed reading still
//     prints messages for keywords of unmerged packets, which are not this command's), and the
//     whole stderr.
// GEOTYP 3 (torsion) widens BCCDEF / BCCDFN / URZ from 2 to 3 components by realloc (1401d0fc0 ->
// 140180150): the entries past the old rows x 2 are uninitialised heap in both programs (the
// original happened to see zeros; GENBCC keeps a negative one), so they are not compared.
#include "p67_5_support.hpp"

#include <defcmd/dispatch.hpp>
#include <defcmd/p67_5.hpp>
#include <defmodel/bc/node_arrays.hpp>

#include <defkey/kw_g1a.hpp>
#include <defmodel/material/state_remap.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include <testkit/check.hpp>

namespace {

using p675::at;

// Redirects fd 2 into a temp file while alive (as p675::StdoutCapture for fd 1).
class StderrCapture {
public:
    StderrCapture() : name_(p675::tmp_name("p675_stderr.txt"))
    {
        std::fflush(stderr);
        saved_ = _dup(2);
        int fd = _open(name_.c_str(), _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, 0666);
        _dup2(fd, 2);
        _close(fd);
    }
    ~StderrCapture()
    {
        restore();
        std::remove(name_.c_str());
    }
    std::string text()
    {
        restore();
        return p675::slurp(name_);
    }

private:
    void restore()
    {
        if (saved_ < 0) return;
        std::fflush(stderr);
        _dup2(saved_, 2);
        _close(saved_);
        saved_ = -1;
    }
    std::string name_;
    int saved_ = -1;
};

std::string genbcc_lines(const std::string& s)
{
    std::string out;
    for (const std::string& l : p675::lines_of(s))
        if (l.find("GENBCC") != std::string::npos) out += l + "\n";
    return out;
}

struct Arr {
    const char* kw;
    std::size_t off;  // object offset of the array pointer
    int dim;          // index into the model's width table m+0x15f8
    bool dbl;
};
const Arr kArrs[] = {{"BCCDEF", 0x990, 0x12, false}, {"BCCDFN", 0x998, 0x13, false},
                     {"BCCTMP", 0x9a0, 0x14, false}, {"BCCTFN", 0x9a8, 0x15, false},
                     {"URZ", 0x8b8, 2, true}};

// "<obj> <kw>" -> number of leading entries that are defined after a widening realloc
std::map<std::string, std::size_t> g_defined;


// One nodal record of the original's OUT.KEY ("KW obj count default", then count rows
// "node v0 v1 ..."), expanded to rows x width tokens; rows not listed take the default.
struct Expanded {
    bool found = false;
    std::vector<std::string> tok;  // row-major, rows x width
};

Expanded expand(const std::string& key_text, const std::string& kw, int obj, int rows, int width)
{
    Expanded e;
    auto recs = p675::records_of(key_text);
    auto it = recs.find(kw + " " + std::to_string(obj));
    if (it == recs.end() || it->second.empty()) return e;
    std::vector<std::string> ls = p675::lines_of(it->second.front());
    char name[64], def[64];
    int o = 0, count = 0;
    if (std::sscanf(ls[0].c_str(), "%63s %d %d %63s", name, &o, &count, def) != 4) return e;
    e.found = true;
    e.tok.assign(static_cast<std::size_t>(rows) * width, def);
    for (int i = 1; i <= count && i < static_cast<int>(ls.size()); ++i) {
        std::istringstream in(ls[i]);
        int node = 0;
        in >> node;
        for (int c = 0; c < width; ++c) {
            std::string t;
            in >> t;
            if (node >= 1 && node <= rows) e.tok[static_cast<std::size_t>(node - 1) * width + c] = t;
        }
    }
    return e;
}

std::string fmt_int(std::int32_t v) { return std::to_string(v); }
std::string fmt_dbl(double v)
{
    char b[64];
    def::rt::crt::snprintf(b, sizeof b, "%.10E", v);
    return b;
}

// Compares one nodal array of every object with the oracle. Returns the number of mismatches.
int compare_nodal(p675::DB_STEP_DATA& m, const std::string& orig, const std::string& label)
{
    int bad = 0;
    const std::int32_t* dims = &at<std::int32_t>(m, 0x15f8);
    for (p675::LLOBJ_DATA* o = at<p675::LLOBJ_DATA*>(m, 0x1380); o != nullptr;
         o = at<p675::LLOBJ_DATA*>(*o, 0x3940)) {
        const int no = at<std::int32_t>(*o, 8);
        const int rows = at<std::int32_t>(*o, 0x880);
        for (const Arr& a : kArrs) {
            const int w = dims[a.dim];
            Expanded e = expand(orig, a.kw, no, rows, w);
            void* p = at<void*>(*o, a.off);
            if (!e.found) {
                if (p != nullptr && rows > 0) {
                    std::printf("MISMATCH %s %s %d: no record in the original\n", label.c_str(), a.kw, no);
                    ++bad;
                }
                continue;
            }
            int diff = 0;
            auto lim = g_defined.find(std::to_string(no) + " " + a.kw);
            for (int r = 0; r < rows; ++r) {
                for (int c = 0; c < w; ++c) {
                    const std::size_t k = static_cast<std::size_t>(r) * w + c;
                    if (lim != g_defined.end() && k >= lim->second) continue;
                    std::string mine;
                    if (p == nullptr) {
                        mine = a.dbl ? fmt_dbl(0.0) : "0";
                    } else if (a.dbl) {
                        mine = fmt_dbl(static_cast<double*>(p)[k]);
                    } else {
                        mine = fmt_int(static_cast<std::int32_t*>(p)[k]);
                    }
                    if (mine != e.tok[k]) {
                        if (diff < 5)
                            std::printf("MISMATCH %s %s %d node %d comp %d: orig %s port %s\n", label.c_str(), a.kw,
                                        no, r + 1, c, e.tok[k].c_str(), mine.c_str());
                        ++diff;
                    }
                }
            }
            if (diff != 0) ++bad;
        }
    }
    return bad;
}

void legacy_printf_options()
{
    unsigned long long& o = *__local_stdio_printf_options();
    o = (o | _CRT_INTERNAL_PRINTF_LEGACY_THREE_DIGIT_EXPONENTS | _CRT_INTERNAL_PRINTF_LEGACY_MSVCRT_COMPATIBILITY) &
        ~static_cast<unsigned long long>(_CRT_INTERNAL_PRINTF_STANDARD_ROUNDING);
}

} // namespace

TEST_CASE(genbcc_cases_match_oracle)
{
    legacy_printf_options();
    if (p675::repo_root().empty()) {
        std::printf("no repository root: skipped\n");
        return;
    }
    // GEOTYP (and STNOUT / ELMNOD) call FUN_1401d0fc0(M, 0) in the original (kw-g4b's port).
    // The hook also records which array entries a widening leaves uninitialised.
    def::key::g1a::model_hooks().rebuild_state_layout = [](def::model::DB_STEP_DATA* m, int arg) {
        const std::int32_t* dims = &at<std::int32_t>(*m, 0x15f8);
        std::map<std::string, std::pair<int, int>> before;  // key -> (rows, width)
        for (p675::LLOBJ_DATA* o = at<p675::LLOBJ_DATA*>(*m, 0x1380); o != nullptr;
             o = at<p675::LLOBJ_DATA*>(*o, 0x3940))
            for (const Arr& a : kArrs)
                before[std::to_string(at<std::int32_t>(*o, 8)) + " " + a.kw] = {at<std::int32_t>(*o, 0x880), dims[a.dim]};
        def::model::step_remap_material_state(*m, arg != 0);
        for (p675::LLOBJ_DATA* o = at<p675::LLOBJ_DATA*>(*m, 0x1380); o != nullptr;
             o = at<p675::LLOBJ_DATA*>(*o, 0x3940))
            for (const Arr& a : kArrs) {
                const std::string k = std::to_string(at<std::int32_t>(*o, 8)) + " " + a.kw;
                auto [rows, w0] = before[k];
                if (dims[a.dim] > w0) {
                    const std::size_t n = static_cast<std::size_t>(rows) * w0;
                    auto it = g_defined.find(k);
                    if (it == g_defined.end() || n < it->second) g_defined[k] = n;
                }
            }
    };
    const std::set<std::string> kws = {"ECCTMP", "ECTMFN", "ECCDEF", "ECDEFN", "LOCTMP"};
    const int genbcc = p675::kw_id("GENBCC");
    int checked = 0;
    for (const p675::Case& c : p675::load_cases("GENBCC")) {
        p675::fs::path e = p675::oracle_entry(c.label);
        if (e.empty()) {
            std::printf("%s: not in the oracle cache, skipped\n", c.label.c_str());
            continue;
        }
        g_defined.clear();
        auto& m = *new p675::DB_STEP_DATA(3);
        p675::CmdRunner run(m, c);
        run.on_command = [&](int id, std::FILE* f) -> int {
            if (id == genbcc) return def::cmd::dispatch(&m, f, id);  // FUN_140378e00
            return -1;
        };
        std::string out, err;
        {
            StderrCapture ecap;
            p675::StdoutCapture cap;
            run.run();
            out = cap.text();
            err = ecap.text();
        }
        for (auto& u : run.unsupported) std::printf("%s: unsupported command %s\n", c.label.c_str(), u.c_str());
        const std::string orig = p675::slurp(e / "files" / "OUT.KEY");
        int bad = p675::compare_records(orig, p675::write_model_key(m), kws, true, c.label);
        int bad_nodal = compare_nodal(m, orig, c.label);
        const std::string want_out = genbcc_lines(p675::keyword_phase_stdout(p675::slurp(e / "stdout.txt")));
        const std::string got_out = genbcc_lines(out);
        for (auto& [k, n] : g_defined)
            std::printf("%s: %s compared on its first %zu entries (widened by realloc)\n", c.label.c_str(), k.c_str(), n);
        const std::string want_err = p675::slurp(e / "stderr.txt");
        if (want_out != got_out) std::printf("STDOUT %s\n  orig:\n%s  port:\n%s", c.label.c_str(), want_out.c_str(), got_out.c_str());
        if (want_err != err) std::printf("STDERR %s\n  orig:\n%s  port:\n%s", c.label.c_str(), want_err.c_str(), err.c_str());
        std::printf("%s: %d record mismatches, %d nodal array mismatches, stdout %s, stderr %s\n", c.label.c_str(), bad,
                    bad_nodal, want_out == got_out ? "same" : "DIFFERENT", want_err == err ? "same" : "DIFFERENT");
        CHECK_EQ(bad, 0);
        CHECK_EQ(bad_nodal, 0);
        CHECK(want_out == got_out);
        CHECK(want_err == err);
        ++checked;
    }
    def::key::g1a::model_hooks().rebuild_state_layout = nullptr;
    std::printf("%d GENBCC cases checked\n", checked);
}

int main() { return testkit::run_all(); }
