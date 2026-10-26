// ctest-labels: p4-06 gendb oracle
// Oracle byte check of the p4-06 sim-control and material writers over DEF_PRE's own GENDB runs.
//
// Cases (data/p406/gendb_cases.tsv, made by data/p406/make_gendb_cases.py with the oracle runner):
// a corpus KEY file read by KFREAD, then "GENDB 2 2" (new DB, checks skipped) to OUT.DB.
// For each case the KEY file is read into a model through the KEY loop with the ported group
// parsers, the port's GENDB steps 1401c4950 (sim control) and 1401c49b0 (materials) write into
// fresh buffers laid out by the oracle step's own address tables (IIAD1 / IIAD2), and the result is
// compared word for word with the oracle step: IISIM, LLSIM, IIUVL, LLMPR and the IIFNC / LLFNC
// entries of the functions these two steps allocate (they run first, so their function numbers
// start at 1).
//
// Inputs that other packets compute are taken from the oracle step, not checked here: the GENDB
// counts in DB_WRITE_VAR (p4-02a/b: IISIM count words) and the materials' used flags / dense slots
// (p4-02b: LLMPR word 0 order), and in 2D S+0x680 (LLSIM 14, 1401bdaf0). Words masked because a later GENDB step or the driver writes them:
// IISIM NSTART (sign rule of 1401bb550 / 1401d7f40), in 3D IISIM 7 (PDIE first LLBND record,
// 14017b370) and, in 2D, IISIM 9 and 35 (PDIE principal
// record, 14017cb60).
//
// Without the cache or the corpus the test passes with a note. P406_MAX_CASES caps the number of
// cases, P406_VERBOSE=1 prints every differing word.
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <map>
#include <memory>
#include <new>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <deformdb/database.hpp>
#include <defdbio/gendb/p4_06_frame.hpp>
#include <defkey/group1.hpp>
#include <defkey/key_reader.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defkey/kw_g3/kw_g3.hpp>
#include <defkey/kw_g4.hpp>
#include <defkey/kw_g57.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defmodel/phase/phase_ops.hpp>
#include <defmodel/step/db_step.hpp>
#include <testkit/check.hpp>

#if defined(_WIN32)
#include <process.h>
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

struct Tally {
    int cases = 0, skipped = 0, failed = 0;
    std::map<std::string, int> diffs; // "region word" -> count of cases
};

std::string pid_name(const std::string& base)
{
#if defined(_WIN32)
    return std::to_string(_getpid()) + "_" + base;
#else
    return base;
#endif
}

// start (1-based) of a region by name from the oracle step's address tables
int start_of(const std::vector<std::int32_t>& mar, std::span<const char* const> names, const char* name, bool a_side)
{
    for (std::size_t k = 0; k < names.size(); ++k) {
        if (std::strcmp(names[k], name) == 0) {
            return a_side ? mar[140 + k] : mar[k];
        }
    }
    return -1;
}

void run_case(const fs::path& key_file, const fs::path& db_file, Tally& t, bool verbose)
{
    auto db = deformdb::Database::open(db_file);
    if (!db || db->entries().empty()) {
        ++t.skipped;
        return;
    }
    const deformdb::StepEntry& e = db->entries().back();
    deformdb::StepData s = db->read_step(e);
    if (!s.has_arrays() || s.mar.size() < 420) {
        ++t.skipped;
        return;
    }
    const int dim = e.dim == 2 ? 2 : 3;

    // ---- model from the KEY file ----
    // DEF_PRE's models live in zero-initialised static storage (14001c770 in main's object), and
    // the ctor leaves some fields (e.g. UNIT, S+0xfc) as they are: construct in zeroed memory.
    auto* primary = new (zeroed(sizeof(DB_STEP_DATA))) DB_STEP_DATA(3);
    auto* companion = new (zeroed(sizeof(DB_STEP_DATA))) DB_STEP_DATA(2);
    primary->companion = companion;
    {
        const std::string tmp = pid_name("p406_in.KEY");
        fs::copy_file(key_file, tmp, fs::copy_options::overwrite_existing);
        std::FILE* f = std::fopen(tmp.c_str(), "r");
        std::FILE* log = std::fopen(pid_name("p406_log.txt").c_str(), "w");
        Hooks h(primary);
        def::key::read_key_stream(f, h, log);
        std::fclose(f);
        std::fclose(log);
        std::remove(tmp.c_str());
        std::remove(pid_name("p406_log.txt").c_str());
    }
    DB_STEP_DATA* m = def::model::keyword_target_model(primary);
    if (m->dimension != dim) {
        ++t.skipped;
        return; // models leaked on purpose (their dtors need packets not merged here)
    }

    // ---- inputs computed by other packets, taken from the oracle ----
    auto mnames = deformdb::mar_region_names(dim, e.version);
    auto anames = deformdb::a_region_names(dim, e.version);
    const std::vector<std::int32_t>& omar = s.mar;
    const std::vector<double>& oa = s.a;
    const int iisim = start_of(omar, mnames, "IISIM", false) - 1;
    const int iifnc = start_of(omar, mnames, "IIFNC", false) - 1;
    const int iiuvl = start_of(omar, mnames, "IIUVL", false) - 1;
    const int llsim = start_of(omar, anames, "LLSIM", true) - 1;
    const int llcpu = start_of(omar, anames, "LLCPU", true) - 1;
    const int llfnc = start_of(omar, anames, "LLFNC", true) - 1;
    const int llmpr = start_of(omar, anames, "LLMPR", true) - 1;
    if (iisim < 0 || iifnc < 0 || iiuvl < 0 || llsim < 0 || llfnc < 0 || llmpr < 0) {
        ++t.skipped;
        return;
    }
    std::memcpy(m->u1b90, omar.data(), 140 * 4);
    std::memcpy(m->u1dc0, omar.data() + 140, 280 * 4);
    static const int k3d[][2] = {{3, 0x68}, {19, 0x6c}, {20, 0x70}, {21, 0x74}, {22, 0x78}, {23, 0x7c},
                                 {24, 0x80}, {25, 0x84}, {26, 0x88}, {27, 0x8c}, {28, 0x90}, {29, 0x94},
                                 {30, 0x98}, {31, 0x9c}, {33, 0xbc}, {34, 0xc0}};
    static const int k2d[][2] = {{5, 0x68},  {26, 0x6c}, {27, 0x70}, {28, 0x74}, {29, 0x78}, {30, 0x7c},
                                 {31, 0x80}, {32, 0x84}, {33, 0x88}, {34, 0x8c}, {37, 0xbc}, {39, 0xc0}};
    if (dim == 3) {
        for (auto& k : k3d) field<std::int32_t>(m, k[1]) = omar[iisim + k[0]];
    } else {
        for (auto& k : k2d) field<std::int32_t>(m, k[1]) = omar[iisim + k[0]];
    }
    if (dim == 2) {
        // S+0x680 (2D LLSIM 14): set by the 2D counts step 1401bdaf0 (p4-02b) from the geometry
        field<double>(m, 0x750 + 0x680) = oa[llsim + 14];
    }
    field<std::int32_t>(m, 0x10c) = 0; // function counter (1401baf30)
    const int nmat = field<std::int32_t>(m, 0x78);
    const int wmpr = m->u1dc0[140 + 1];
    for (def::model::Material* mat = m->materials.head; mat != nullptr; mat = field<def::model::Material*>(mat, 0x2200)) {
        field<std::int32_t>(mat, 4) = 0;
    }
    for (int k = 0; k < nmat; ++k) {
        const int no = static_cast<int>(oa[llmpr + k * wmpr]);
        def::model::Material* mat = def::model::material_list_find(&m->materials, no);
        if (mat != nullptr) field<std::int32_t>(mat, 4) = k + 1;
    }
    // GENDB's pre-steps on the model (1401d7f40): 1401ee510
    def::model::sim_ctrl_update_u240(m->sim_ctrl);

    // ---- the port's writers ----
    std::vector<std::int32_t> mar(omar.size(), 0);
    std::vector<double> a(oa.size() + 1000, 0.0);
    m->u050 = mar.data();
    m->u058 = a.data();
    def::dbio::gendb::write_step_sim_ctrl(m, mar.data() + iisim, a.data() + llsim, a.data() + llcpu,
                                          a.data() + llfnc, mar.data() + iifnc, mar.data() + iiuvl);
    def::dbio::gendb::write_step_materials(m, a.data() + llmpr, a.data() + llfnc, mar.data() + iifnc);
    const int nfn = field<std::int32_t>(m, 0x10c);

    // ---- compare ----
    std::set<std::string> bad;
    auto cmp_i = [&](const char* rg, int base, int n, std::set<int> mask) {
        for (int w = 0; w < n; ++w) {
            if (mask.count(w) != 0) continue;
            if (mar[base + w] != omar[base + w]) {
                bad.insert(std::string(rg) + " " + std::to_string(w));
                if (verbose) std::printf("  %s[%d] port %d oracle %d\n", rg, w, mar[base + w], omar[base + w]);
            }
        }
    };
    auto cmp_d = [&](const char* rg, int base, int n) {
        for (int w = 0; w < n; ++w) {
            if (std::memcmp(&a[base + w], &oa[base + w], 8) != 0) {
                bad.insert(std::string(rg) + " " + std::to_string(w));
                if (verbose) std::printf("  %s[%d] port %.17g oracle %.17g\n", rg, w, a[base + w], oa[base + w]);
            }
        }
    };
    if (dim == 3) {
        cmp_i("IISIM", iisim, 100, {2, 7});
    } else {
        cmp_i("IISIM", iisim, 100, {4, 9, 35});
    }
    cmp_d("LLSIM", llsim, 100);
    cmp_i("IIUVL", iiuvl, 200, {});
    cmp_d("LLMPR", llmpr, nmat * wmpr);
    cmp_i("IIFNC", iifnc, 2 * nfn + 1, {});
    if (nfn > 0) cmp_d("LLFNC", llfnc, mar[iifnc + 2 * nfn] - 1);

    ++t.cases;
    if (!bad.empty()) {
        ++t.failed;
        for (const std::string& b : bad) ++t.diffs[b];
        if (verbose) std::printf("DIFF %s (%zu words)\n", key_file.string().c_str(), bad.size());
    }
}

} // namespace

TEST_CASE(simctrl_and_materials_match_def_pre_gendb)
{
    const fs::path cases = testkit::repo_root() / "port" / "defcmd" / "tests" / "data" / "p406" / "gendb_cases.tsv";
    const fs::path corpus = env("DEFORM_CORPUS", "C:\\Users\\alext\\Projects\\DEFORM_corpus");
    const fs::path cache = oracle_root() / "entries";
    if (!fs::exists(cases) || !fs::exists(cache) || !fs::exists(corpus)) {
        std::printf("note: oracle cache or corpus missing, skipped\n");
        return;
    }
    const int max_cases = std::atoi(env("P406_MAX_CASES", "100000").c_str());
    const bool verbose = env("P406_VERBOSE", "0") == "1";
    std::ifstream in(cases);
    std::string line;
    Tally t;
    int n = 0;
    while (std::getline(in, line) && n < max_cases) {
        if (line.empty() || line[0] == '#') continue;
        const std::size_t tab = line.find('\t');
        const std::string rel = line.substr(0, tab), key = line.substr(tab + 1);
        const fs::path db = cache / key.substr(0, 2) / key / "files" / "OUT.DB";
        const fs::path kf = corpus / rel;
        if (!fs::exists(db) || !fs::exists(kf)) {
            ++t.skipped;
            continue;
        }
        ++n;
        if (verbose) std::printf("case %s\n", rel.c_str());
        run_case(kf, db, t, verbose);
    }
    std::printf("cases %d, skipped %d, with differences %d\n", t.cases, t.skipped, t.failed);
    for (const auto& [w, c] : t.diffs) std::printf("  %-12s in %d cases\n", w.c_str(), c);
    CHECK(t.cases > 0);
    CHECK_EQ(t.failed, 0);
}

int main()
{
    std::setvbuf(stdout, nullptr, _IONBF, 0);
    return testkit::run_all();
}
