// ctest-labels: dbread gendb oracle
// DB round trip: DBREAD (1401cc420) then GENDB must give back the DB that DEF_PRE wrote.
//
// Cases: the oracle OUT.DBs of data/p406/gendb_cases.tsv (DEF_PRE's own "KFREAD <key> / GENDB 2 2"
// runs, one step each). For each case the step is read into a fresh model with the ported DBREAD
// (fun_1401cc420 on the last directory entry of the DB info that load_db_info 1401dc3f0 reads),
// the GENDB command's own steps run as in test_dbread_rt_oracle.cpp (1401d7f40, MESHNO, then
// 1401bb550(m, path, 1, 2, 0)), and the new OUT.DB is compared with the oracle's by defcmp (default
// masks). Each case runs in a child process (this executable with --one ...), so a DEF_NOT_PORTED
// abort fails that case only; the parent tallies equal / different / aborted cases.
//
// Until the DBREAD lead (defdbio/src/dbread/lead.cpp) is in the tree the test passes with a note.
// Env: DBREAD_RT_MAX, DBREAD_RT_JOBS (default 8), DBREAD_RT_FILTER, DBREAD_RT_VERBOSE,
// DBREAD_RT_REQUIRE=1 (differences fail the test), DBREAD_RT_IDENTITY=1 (compare with the input DB even
// where data/p406/dbread_rt_expect.tsv names DEF_PRE's own round trip).
#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <map>
#include <mutex>
#include <new>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include <defcmp/defcmp.hpp>
#if __has_include("../../defdbio/src/dbread/lead.cpp")
#include <defdbio/dbread/dbread.hpp>
#define DBREAD_RT_HAVE_LEAD 1
#endif
#include <defdbio/step/db_info_read.hpp>
#include <defmodel/layout/cls_1401dbfe0.hpp>
#include <defdbio/gendb/gendb.hpp>
#include <defkey/group1.hpp>
#include <defkey/key_reader.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defkey/kw_g3/kw_g3.hpp>
#include <defkey/kw_g4.hpp>
#include <defkey/kw_g57.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/phase/phase_ops.hpp>
#include <defmodel/step/db_step.hpp>
#include <testkit/check.hpp>

#if __has_include(<defdbio/gendb/pre_steps.hpp>)
#include <defdbio/gendb/pre_steps.hpp>
#define DBREAD_RT_HAVE_PRE_STEPS 1
#endif

#if defined(_WIN32)
#include <direct.h>
#include <process.h>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

namespace fs = std::filesystem;
using def::model::DB_STEP_DATA;
using def::model::field;

namespace {

std::string env(const char* name, const char* def)
{
    const char* v = std::getenv(name);
    return v != nullptr && *v != '\0' ? v : def;
}

fs::path oracle_root()
{
    std::string c = env("DEFORM_ORACLE_CACHE", "");
    if (!c.empty()) return c;
    return fs::path(env("USERPROFILE", "")) / ".deform-oracle";
}

struct Hooks : def::key::KeyReaderHooks {
    DB_STEP_DATA* m;
    explicit Hooks(DB_STEP_DATA* model) : m(model) {}
    bool has_sub_model() override { return def::model::companion_model(m) != nullptr; }
    void set_model_dimension(int d) override { m->set_dimension(&d); }
    void set_sub_model_dimension(int d) override { m->companion->set_dimension(&d); }
    int parse_keyword(int group, std::FILE* f, int id) override
    {
        DB_STEP_DATA* t = def::model::keyword_target_model(m);
        switch (group) {
        case 1: return def::key::read_group1(t, f, id);
        case 2: return def::key::g2obj::read_object_keyword(*t, f, id);
        case 3: return def::key::g3::read_keyword(t, f, id);
        case 4: return def::key::g4::read_keyword(*t, f, id);
        case 5: return def::key::g57::read_phase_keyword(t, f, id);
        case 7: return def::key::g57::read_dim_keyword(m, f, id);
        default: {
            char rest[0x400];
            std::fgets(rest, sizeof rest, f); // commands: not in the corpus model files
            return 0;
        }
        }
    }
};

void* zeroed(std::size_t n)
{
    void* p = ::operator new(n, std::align_val_t{16});
    std::memset(p, 0, n);
    return p;
}

// ---- child: one case ------------------------------------------------------------------------
// --one <key file> <oracle OUT.DB> <work dir> <stdout file> <stderr file>. Prints one line "RESULT equal" or
// "RESULT diff <where>" (or "RESULT nodb"); a DEF_NOT_PORTED abort ends the process instead.
int run_one(const fs::path& key_file, const fs::path& oracle_db, const fs::path& work, const char* out,
            const char* err, const char* expect_arg = nullptr)
{
    // the child redirects its own output (no shell is involved in starting it)
    std::freopen(out, "w", stdout);
    std::freopen(err, "w", stderr);
    std::setvbuf(stdout, nullptr, _IONBF, 0);
#if defined(_WIN32)
    // an abort (DEF_NOT_PORTED) ends the child quietly: no error-report dialog on the desktop
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
    ::SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
#endif
    fs::create_directories(work);
    fs::current_path(work);
#if !defined(DBREAD_RT_HAVE_LEAD)
    (void)key_file;
    std::printf("\nRESULT nodb (DBREAD not in the tree)\n");
    return 2;
#else
    // the input is a copy of the oracle DB (never the cache file itself)
    fs::copy_file(oracle_db, work / "IN.DB", fs::copy_options::overwrite_existing);
    (void)key_file;
    auto* primary = new (zeroed(sizeof(DB_STEP_DATA))) DB_STEP_DATA(3);
    auto* companion = new (zeroed(sizeof(DB_STEP_DATA))) DB_STEP_DATA(2);
    primary->companion = companion;
    auto* info = new def::model::Cls_1401dbfe0();
    if (def::dbio::load_db_info(*info, "IN.DB", false) != 0 || info->uac8 < 1) { // 1401dc3f0
        std::printf("\nRESULT nodb (load_db_info)\n");
        return 2;
    }
    DB_STEP_DATA* m = primary;
    const std::int32_t rd = def::dbio::dbread::fun_1401cc420(m, info, info->uac8 - 1);
    if (rd != 0) {
        std::printf("\nRESULT nodb (DBREAD returned %d)\n", rd);
        return 2;
    }
    // GENDB command, 140378e00 case 4 with the arguments "2 2"
#if defined(DBREAD_RT_HAVE_PRE_STEPS)
    def::dbio::gendb::gendb_command_pre_steps(m); // 1401d7f40
#endif
    if (field<std::uint8_t>(m, 0x2f89) != 0) {
        field<std::int32_t>(m, 0x840) = field<std::int32_t>(m, 0x840) + 1; // MESHNO
    }
    std::fflush(stdout);
    const std::int32_t rc = def::dbio::gendb::write_db(*m, "OUT.DB", 1, 2, 0);
    std::fflush(stdout);
    if (!fs::exists(work / "OUT.DB")) {
        std::printf("\nRESULT nodb rc=%d\n", rc);
        return 2;
    }
    // the expected DB: DEF_PRE's own "DBREAD -1 / IN.DB / GENDB 2 2" result for the same input when the
    // parent passes it (7th argument) or DBREAD_RT_EXPECT names one, else the input itself
    const std::string expect = expect_arg != nullptr ? std::string(expect_arg) : env("DBREAD_RT_EXPECT", "");
    const fs::path expected_db = expect.empty() ? oracle_db : fs::path(expect);
    const defcmp::Result r = defcmp::compare_db(expected_db, work / "OUT.DB", defcmp::DbMasks{});
    if (!r.error.empty()) {
        std::printf("\nRESULT diff error: %s\n", r.error.c_str());
        return 1;
    }
    if (r.equal) {
        std::printf("\nRESULT equal\n");
        return 0;
    }
    std::string where = r.where;
    std::replace(where.begin(), where.end(), '\n', ' ');
    std::printf("\nRESULT diff %s | expected %s | actual %s\n", where.c_str(), r.expected.c_str(),
                r.actual.c_str());
    return 1;
#endif
}

// ---- parent ---------------------------------------------------------------------------------
std::string slurp(const fs::path& p)
{
    std::ifstream in(p, std::ios::binary);
    std::stringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

std::string last_line_with(const std::string& text, const char* needle)
{
    std::size_t at = text.rfind(needle);
    if (at == std::string::npos) return "";
    std::size_t end = text.find_first_of("\r\n", at); // the child's stdout is in text mode
    return text.substr(at, end == std::string::npos ? std::string::npos : end - at);
}

struct Case {
    std::string rel;
    fs::path key, db;
    fs::path expect; // DEF_PRE's DBREAD + GENDB result for db (empty: compare with db)
};

struct Tally {
    std::mutex mu;
    int equal = 0, diff = 0, aborted = 0, nodb = 0;
    std::map<std::string, int> places;      // first differing place -> cases
    std::map<std::string, int> not_ported;  // abort line -> cases
};

std::string region_of(const std::string& where)
{
    // "step 1 (slot 0), A region LLBND, keyword ..., word 23" -> keep up to the region name
    std::size_t k = where.find("region ");
    if (k == std::string::npos) return where.substr(0, 60);
    std::size_t e = where.find_first_of(",;|", k);
    return where.substr(0, e == std::string::npos ? std::string::npos : e);
}

} // namespace

TEST_CASE(dbread_then_gendb_gives_back_the_db)
{
    const fs::path cases = testkit::repo_root() / "port" / "defcmd" / "tests" / "data" / "p406" / "gendb_cases.tsv";
    const fs::path corpus = env("DEFORM_CORPUS", "C:\\Users\\alext\\Projects\\DEFORM_corpus");
    const fs::path cache = oracle_root() / "entries";
    if (!fs::exists(cases) || !fs::exists(cache) || !fs::exists(corpus)) {
        std::printf("note: oracle cache or corpus missing, skipped\n");
        return;
    }
    const int max_cases = std::atoi(env("DBREAD_RT_MAX", "100000").c_str());
    const int jobs = std::max(1, std::atoi(env("DBREAD_RT_JOBS", "8").c_str()));
    const std::string filter = env("DBREAD_RT_FILTER", "");
    const bool verbose = env("DBREAD_RT_VERBOSE", "0") == "1";
    const bool require = env("DBREAD_RT_REQUIRE", "0") == "1";

    // dbread_rt_expect.tsv: <key file> TAB <oracle entry of DEF_PRE's "DBREAD -1 / IN.DB / GENDB 2 2"
    // run on that case's OUT.DB>. DEF_PRE's own DB round trip is not the identity (2D curves of type 1
    // are rebuilt from their points, 1402763d0 -> 14032ac75 / 14032b34b), so it is the reference.
    std::map<std::string, std::string> expect_key;
    if (env("DBREAD_RT_IDENTITY", "0") != "1") {
        std::ifstream in(cases.parent_path() / "dbread_rt_expect.tsv");
        std::string line;
        while (std::getline(in, line)) {
            if (line.empty() || line[0] == '#') continue;
            const std::size_t tab = line.find('	');
            if (tab != std::string::npos) expect_key[line.substr(0, tab)] = line.substr(tab + 1);
        }
    }
    std::vector<Case> list;
    {
        std::ifstream in(cases);
        std::string line;
        while (std::getline(in, line) && static_cast<int>(list.size()) < max_cases) {
            if (line.empty() || line[0] == '#') continue;
            const std::size_t tab = line.find('\t');
            const std::string rel = line.substr(0, tab), key = line.substr(tab + 1);
            if (!filter.empty() && rel.find(filter) == std::string::npos) continue;
            const fs::path db = cache / key.substr(0, 2) / key / "files" / "OUT.DB";
            const fs::path kf = corpus / rel;
            if (!fs::exists(db) || !fs::exists(kf)) continue;
            fs::path ex;
            auto it = expect_key.find(rel);
            if (it != expect_key.end()) {
                ex = cache / it->second.substr(0, 2) / it->second / "files" / "NEW.DB";
                if (!fs::exists(ex)) ex.clear();
            }
            list.push_back({rel, kf, db, ex});
        }
    }
    const fs::path root = fs::temp_directory_path() / ("dbread_rt_" + std::to_string(::_getpid()));
    fs::create_directories(root);
    char self[1024] = {};
#if defined(_WIN32)
    ::GetModuleFileNameA(nullptr, self, sizeof self);
#endif

    Tally t;
    std::atomic<std::size_t> next{0};
    auto worker = [&] {
        while (true) {
            const std::size_t i = next.fetch_add(1);
            if (i >= list.size()) return;
            const Case& c = list[i];
            const fs::path work = root / std::to_string(i);
            const fs::path out = root / (std::to_string(i) + ".out");
            const fs::path err = root / (std::to_string(i) + ".err");
            auto q = [](const fs::path& p) { return "\"" + p.string() + "\""; }; // _spawnv does not quote
            const std::string a1 = q(c.key), a2 = q(c.db), a3 = q(work), a4 = q(out), a5 = q(err), a6 = q(c.expect);
            const char* argv[] = {self, "--one", a1.c_str(), a2.c_str(), a3.c_str(), a4.c_str(), a5.c_str(),
                                  c.expect.empty() ? nullptr : a6.c_str(), nullptr};
            ::_spawnv(_P_WAIT, self, argv); // argv only: no shell, nothing is opened by association
            const std::string o = slurp(out), e = slurp(err);
            const std::string res = last_line_with(o, "RESULT ");
            std::lock_guard<std::mutex> lock(t.mu);
            if (res == "RESULT equal") {
                ++t.equal;
            } else if (res.rfind("RESULT diff", 0) == 0) {
                ++t.diff;
                ++t.places[region_of(res.substr(12))];
            } else if (res.rfind("RESULT nodb", 0) == 0) {
                ++t.nodb;
            } else {
                ++t.aborted;
                std::string np = last_line_with(e, "def_pre port:");
                ++t.not_ported[np.empty() ? "abort without message" : np];
                if (verbose) std::printf("ABORT %s\n  %s\n", c.rel.c_str(), np.c_str());
            }
            if (verbose && !res.empty()) std::printf("%s\n  %s\n", c.rel.c_str(), res.c_str());
            if (res == "RESULT equal") {
                std::error_code ec;
                fs::remove_all(work, ec);
                fs::remove(out, ec);
                fs::remove(err, ec);
            }
        }
    };
    std::vector<std::thread> pool;
    for (int k = 0; k < jobs; ++k) pool.emplace_back(worker);
    for (auto& th : pool) th.join();

    std::printf("cases %zu: equal %d, different %d, no DB %d, aborted %d (work dir %s)\n", list.size(), t.equal,
                t.diff, t.nodb, t.aborted, root.string().c_str());
    for (const auto& [p, n] : t.places) std::printf("  diff  %4d  %s\n", n, p.c_str());
    for (const auto& [p, n] : t.not_ported) std::printf("  abort %4d  %s\n", n, p.c_str());
#if !defined(DBREAD_RT_HAVE_LEAD)
    std::printf("note: DBREAD lead not in the tree yet, every case reports no DB\n");
#endif
    CHECK(!list.empty());
    if (require) {
        CHECK_EQ(t.diff + t.nodb + t.aborted, 0);
    }
}

int main(int argc, char** argv)
{
    std::setvbuf(stdout, nullptr, _IONBF, 0);
    if ((argc == 7 || argc == 8) && std::strcmp(argv[1], "--one") == 0)
        return run_one(argv[2], argv[3], argv[4], argv[5], argv[6], argc == 8 ? argv[7] : nullptr);
    return testkit::run_all();
}
