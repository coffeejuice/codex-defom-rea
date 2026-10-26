// ctest-labels: kw-g2geo group2 oracle
// Oracle check of the 2D rigid curves that GENDB writes as LLGRY rows, over the cached
// "KFREAD <key> / GENDB 2 2 / OUT.DB" runs of the original DEF_PRE (cases
// data/kw_g2geo/gendb_2d_cases.tsv, cache %USERPROFILE%/.deform-oracle or $DEFORM_ORACLE_CACHE).
//
// Each 2D KEY (a copy) is read through the KEY loop with the group 2 parser. For every curve of
// every rigid object the rows are made the way GENDB's 140276d70 makes them from the model
// (curve type 2: the rows as stored; type 1: drop duplicate points 1402765e0, point extent
// 140345f5b, points -> entities 14032b34b) and must appear bit for bit, as one contiguous run of
// rows, in the oracle's LLGRY region (KEYs whose oracle LLGRY is empty are counted, not compared).140345f5b writes the scale of the entity tolerances
// (COMMON 140b84720..30) that 14032b34b reads, so a wrong merge of short segments shows up here.
//
// KW_G2GEO_LLGRY_ONLY (substring of the KEY path) and KW_G2GEO_LLGRY_VERBOSE=1 narrow the run.
// Without the cache or the corpus the test passes with a note.
#define _CRT_SECURE_NO_WARNINGS
#include "key_test_util.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <span>
#include <fstream>
#include <string>
#include <vector>

#include <defkey/key_reader.hpp>
#include <defkey/kw_g2obj.hpp>
#include <process.h>

#include "crash_trace.hpp"
#include <defkey/line_io.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/globals_p1_09.hpp>
#include <defmodel/globals_p1_10.hpp>
#include <defmodel/keyword_registry.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/members/ftn_geom.hpp>
#include <defmodel/statevar/geo2d_kernels.hpp>
#include <defmodel/step/db_step.hpp>
#include <deformdb/database.hpp>
#include <testkit/check.hpp>

namespace fs = std::filesystem;
using namespace def::key;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;
using def::model::OBJ_GEO2D;

namespace {

template <class T, class C>
T& at(C& base, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<unsigned char*>(&base) + off);
}

std::string env(const char* name, const char* def)
{
    const char* v = std::getenv(name);
    return v != nullptr && *v != '\0' ? v : def;
}

struct Hooks : KeyReaderHooks {
    DB_STEP_DATA& m;
    int geotyp_id, unit_id;
    explicit Hooks(DB_STEP_DATA& model)
        : m(model), geotyp_id(def::model::keyword_id("GEOTYP")), unit_id(def::model::keyword_id("UNIT"))
    {
    }
    bool has_sub_model() override { return false; }
    void set_model_dimension(int dim) override
    {
        at<std::int32_t>(m, 0x1c) = dim;
        def::model::step_setup_array_sizes(m, dim);
    }
    void set_sub_model_dimension(int) override {}
    int parse_keyword(int group, std::FILE* f, int id) override
    {
        if (group == 2) return def::key::g2obj::read_object_keyword(m, f, id);
        if (group == 1 && id == geotyp_id) read_i(f, &at<std::int32_t>(m, 0x850));
        if (group == 1 && id == unit_id) read_i(f, &at<std::int32_t>(m, 0x84c));
        return 0;
    }
};

// The rows of 140276d70 (GENDB, p4-06 write_curve_rows) built from the model's curve
std::vector<double> curve_rows(OBJ_GEO2D* geo)
{
    std::vector<double> out;
    if (geo->curve_type == 2) {
        out.assign(geo->points, geo->points + static_cast<std::size_t>(geo->n_points) * 6);
        return out;
    }
    std::int32_t n = geo->n_points;
    if (geo->fun_1402765e0()) n = geo->n_points;
    def::model::fun_140345f5b(geo->points, &n);
    std::vector<double> buf(static_cast<std::size_t>(n > 0 ? n : 1) * 60 + 60, 0.0);
    std::int32_t nent = 0;
    def::model::geo2d::points_to_entities(&n, geo->points, &nent, buf.data());
    out.assign(buf.begin(), buf.begin() + static_cast<std::ptrdiff_t>(nent) * 6);
    return out;
}

bool contains_run(std::span<const double> hay, const std::vector<double>& rows)
{
    if (rows.empty()) return true;
    for (std::size_t s = 0; s + rows.size() <= hay.size(); s += 6) {
        if (std::memcmp(hay.data() + s, rows.data(), rows.size() * sizeof(double)) == 0) return true;
    }
    return false;
}

struct Totals {
    int files = 0, skipped = 0, no_llgry = 0, curves = 0, bad_curves = 0, bad_files = 0;
};

void check_case(const fs::path& src, const fs::path& oracle_db, Totals& tot, bool verbose)
{
    def::model::reset_globals_p1_09();  // a fresh DEF_PRE process per case
    def::model::reset_globals_p1_10();
    fs::path copy = fs::temp_directory_path() / ("kw_g2geo_llgry_input_" + std::to_string(_getpid()) + ".key");
    fs::copy_file(src, copy, fs::copy_options::overwrite_existing);
    DB_STEP_DATA& m = *new DB_STEP_DATA(3);
    std::FILE* in = std::fopen(copy.string().c_str(), "r");
    std::FILE* sink = std::fopen("NUL", "w");
    Hooks hooks(m);
    read_key_stream(in, hooks, sink);
    std::fclose(in);
    std::fclose(sink);
    fs::remove(copy);

    auto db = deformdb::Database::open(oracle_db);
    const deformdb::StepData step = db->read_step(db->entries().back());
    // LLGRY words: its start (1-based) in the A start table MAR[141..] up to the next region's start
    std::span<const double> llgry;
    {
        const auto names = deformdb::a_region_names(db->entries().back().dim, db->entries().back().version);
        for (std::size_t k = 0; k + 1 < names.size(); ++k) {
            if (names[k] == nullptr || std::strcmp(names[k], "LLGRY") != 0) continue;
            const std::int32_t s0 = step.mar[140 + k], s1 = step.mar[140 + k + 1];
            if (0 < s0 && s0 <= s1 && static_cast<std::size_t>(s1 - 1) <= step.a.size())
                llgry = std::span<const double>(step.a.data() + (s0 - 1), static_cast<std::size_t>(s1 - s0));
            break;
        }
    }
    if (llgry.empty()) {
        // the original wrote no rigid curve rows for this KEY (MO previews / cut objects whose
        // objects GENDB leaves out): nothing to compare
        ++tot.no_llgry;
        return;
    }
    ++tot.files;
    bool bad = false;
    int obj_no = 0;
    for (LLOBJ_DATA* o = at<LLOBJ_DATA*>(m, 0x1380); o != nullptr; o = at<LLOBJ_DATA*>(*o, 0x3940)) {
        ++obj_no;
        if (at<std::int32_t>(*o, 0xc) != 0) continue;  // rigid objects only
        const std::int32_t nc = def::model::geo_kw::curve_count(*o);
        for (std::int32_t c = 0; c < nc; ++c) {
            OBJ_GEO2D* geo = def::model::geo_kw::curve(*o, c);
            if (geo == nullptr) continue;
            const std::vector<double> rows = curve_rows(geo);
            ++tot.curves;
            if (contains_run(llgry, rows)) continue;
            ++tot.bad_curves;
            bad = true;
            if (verbose) {
                std::printf("MISMATCH %s object %d curve %d: %zu rows not in LLGRY (%zu words)\n",
                            src.string().c_str(), obj_no, c, rows.size() / 6, llgry.size());
                for (std::size_t r = 0; r < rows.size(); r += 6)
                    std::printf("  %.17g %.17g %.17g %.17g %.17g %.17g\n", rows[r], rows[r + 1], rows[r + 2],
                                rows[r + 3], rows[r + 4], rows[r + 5]);
            }
        }
    }
    if (bad) {
        ++tot.bad_files;
        std::printf("bad: %s\n", src.string().c_str());
    }
    // the model is leaked on purpose with its objects (as in test_kw_g2geo_oracle.cpp)
}

} // namespace

TEST_CASE(gendb_llgry_rows_of_2d_rigid_curves_match_def_pre)
{
    crash_trace::install();
    keytest::legacy_printf();
    fs::path cache = env("DEFORM_ORACLE_CACHE", "");
    if (cache.empty()) cache = fs::path(env("USERPROFILE", "")) / ".deform-oracle";
    const fs::path corpus = env("DEFORM_CORPUS", "C:\\Users\\alext\\Projects\\DEFORM_corpus");
    const fs::path cases = testkit::repo_root() / "port" / "defkey" / "tests" / "data" / "kw_g2geo" / "gendb_2d_cases.tsv";
    if (!fs::exists(cache / "entries") || !fs::exists(corpus) || !fs::exists(cases)) {
        std::printf("note: oracle cache, corpus or case list missing, skipped\n");
        return;
    }
    const bool verbose = env("KW_G2GEO_LLGRY_VERBOSE", "0") == "1";
    const std::string only = env("KW_G2GEO_LLGRY_ONLY", "");
    Totals tot;
    std::ifstream in(cases);
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty() || line[0] == '#') continue;
        const std::size_t tab = line.find('\t');
        if (tab == std::string::npos) continue;
        const std::string rel = line.substr(0, tab), key = line.substr(tab + 1);
        if (!only.empty() && rel.find(only) == std::string::npos) continue;
        const fs::path db = cache / "entries" / key.substr(0, 2) / key / "files" / "OUT.DB";
        const fs::path kf = corpus / rel;
        if (!fs::exists(db) || !fs::exists(kf)) {
            ++tot.skipped;
            continue;
        }
        check_case(kf, db, tot, verbose);
    }
    std::printf("2D GENDB cases: %d checked, %d without LLGRY rows, %d skipped; rigid curves %d, not found in "
                "LLGRY %d (%d files)\n",
                tot.files, tot.no_llgry, tot.skipped, tot.curves, tot.bad_curves, tot.bad_files);
    CHECK_EQ(tot.bad_curves, 0);
}

int main() { return testkit::run_all(); }
