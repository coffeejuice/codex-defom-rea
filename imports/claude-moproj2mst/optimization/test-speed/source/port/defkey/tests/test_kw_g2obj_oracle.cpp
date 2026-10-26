// ctest-labels: kw-g2obj group2 oracle
// Oracle round-trip check of this packet's keywords over the cached KFREAD/KFWRIT runs of the
// original DEF_PRE (kfrw/<path> cases of harness/corpus, cache %USERPROFILE%\.deform-oracle or
// $DEFORM_ORACLE_CACHE).
//
// For each cached case: a copy of the input KEY file is read through the KEY loop
// (read_key_stream) with the group 2 parser of this packet (other groups are skipped; GEOTYP is
// stored because ANGMVY's writer reads it), every object is written back with write_object (plus
// the RM* handlers, which FUN_1402a9180 of kw-g2mg emits), and for every object and every
// keyword of this packet, the records written must equal the original's KFWRIT_OUT.KEY records
// byte for byte. That includes keywords the input does not give (their values are the object
// defaults), except RMDPTH/RMSTRK/RMTIME (written by kw-g2mg's block) and ANGMVY (its condition
// reads the WPAXIS list of kw-g2geo), which are compared only where the input gives them.
//
// Without the cache the test passes with a note. KW_G2OBJ_MAX_BYTES (default 2000000) caps the
// input size, KW_G2OBJ_MAX_FILES (default all) the number of cases, KW_G2OBJ_VERBOSE=1 prints each
// mismatch.
#define _CRT_SECURE_NO_WARNINGS
#include "key_test_util.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <defkey/key_reader.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defkey/line_io.hpp>
#include <defmodel/keyword_registry.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <testkit/check.hpp>

namespace fs = std::filesystem;
using namespace def::key;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;

namespace {

template <class T, class C>
T& at(C& base, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<unsigned char*>(&base) + off);
}

const std::set<std::string>& my_keywords()
{
    static const std::set<std::string> k = {
        "OBJNAM", "OBJTYP", "AVGSTR", "LMTSTR", "ZSTR",   "PENVOL", "REFTMP", "TMPLMT",
        "MOVCTL", "STROKE", "SPDLMT", "ANGMOV", "CNTRAX", "ANGMVY", "ANGMO2", "CNTRA2",
        "FORCE",  "PRSNAM", "PRSTRE", "OBJUPD", "OBJSTP", "RMDPTH", "RMSTRK", "RMTIME",
        "FRCSTP", "FRCNEL", "FRCMTH", "CRPTIM", "ELPSOL", "USRSUB", "REFPOS", "TRGVOL",
        "ROTSYM", "OTPRNG", "CSFREQ", "VOLCRG"};
    return k;
}

std::string env(const char* name, const char* def)
{
    const char* v = std::getenv(name);
    return v != nullptr && *v != '\0' ? v : def;
}

std::string slurp(const fs::path& p)
{
    std::ifstream in(p, std::ios::binary);
    std::ostringstream s;
    s << in.rdbuf();
    return s.str();
}

// "key": "value" of the first occurrence after `from`, JSON backslash escapes undone
std::string json_string(const std::string& j, const char* key, std::size_t from = 0)
{
    std::string pat = std::string("\"") + key + "\": \"";
    std::size_t p = j.find(pat, from);
    if (p == std::string::npos) return {};
    p += pat.size();
    std::string out;
    while (p < j.size() && j[p] != '"') {
        if (j[p] == '\\' && p + 1 < j.size()) ++p;
        out += j[p++];
    }
    return out;
}

struct Record {
    std::string kw;
    std::string text;
};
// object number -> records in order
using Records = std::map<int, std::vector<Record>>;

std::vector<std::string> lines_of(const std::string& s)
{
    std::vector<std::string> v;
    std::size_t a = 0;
    while (a < s.size()) {
        std::size_t b = s.find('\n', a);
        if (b == std::string::npos) b = s.size();
        std::string l = s.substr(a, b - a);
        if (!l.empty() && l.back() == '\r') l.pop_back();
        v.push_back(l);
        a = b + 1;
    }
    return v;
}

// Records of the group 2 keywords: a line whose first token is a registry name of group 2
// starts one (object = second token); following lines that start with a blank belong to it, and
// OBJNAM / PRSNAM take the next line whatever it is.
Records records_of(const std::string& text)
{
    Records r;
    std::vector<std::string> ls = lines_of(text);
    for (std::size_t i = 0; i < ls.size(); ++i) {
        const std::string& l = ls[i];
        if (l.empty() || l[0] == ' ' || l[0] == '*' || l[0] == '\t') continue;
        char name[64] = {0};
        int obj = 0;
        if (std::sscanf(l.c_str(), "%63s %i", name, &obj) < 2) continue;
        const def::model::KeywordRecord* k = def::model::find_keyword(name);
        if (k == nullptr || k->group != def::model::KeywordGroup::Object) continue;
        Record rec{name, l + "\n"};
        std::size_t j = i + 1;
        if ((rec.kw == "OBJNAM" || rec.kw == "PRSNAM") && j < ls.size()) rec.text += ls[j++] + "\n";
        while (j < ls.size() && !ls[j].empty() && ls[j][0] == ' ') rec.text += ls[j++] + "\n";
        r[obj].push_back(rec);
        i = j - 1;
    }
    return r;
}

struct Hooks : KeyReaderHooks {
    DB_STEP_DATA& m;
    int geotyp_id, unit_id;
    explicit Hooks(DB_STEP_DATA& model)
        : m(model), geotyp_id(def::model::keyword_id("GEOTYP")), unit_id(def::model::keyword_id("UNIT"))
    {
    }
    bool has_sub_model() override { return false; }
    void set_model_dimension(int dim) override { at<std::int32_t>(m, 0x1c) = dim; }
    void set_sub_model_dimension(int) override {}
    int parse_keyword(int group, std::FILE* f, int id) override
    {
        if (group == 2) return def::key::g2obj::read_object_keyword(m, f, id);
        if (group == 1 && id == geotyp_id) {
            // GEOTYP: S+0x100 = model+0x850 (simctrl.md); read_i on the rest of the line
            read_i(f, &at<std::int32_t>(m, 0x850));
        }
        if (group == 1 && id == unit_id) {
            // UNIT: S+0xfc = model+0x84c (the object REFTMP default reads it, 14015f9a0)
            read_i(f, &at<std::int32_t>(m, 0x84c));
        }
        return 0;
    }
};

struct Totals {
    int files = 0, skipped = 0, compared = 0, mismatched = 0, missing = 0;
    std::map<std::string, int> bad_by_kw, ok_by_kw;
};

void check_case(const fs::path& src, const fs::path& oracle_out, Totals& tot, bool verbose)
{
    // the input is read from a copy (corpus files stay untouched)
    fs::path copy = fs::temp_directory_path() / keytest::tmp_name("kw_g2obj_oracle_input.key");
    fs::copy_file(src, copy, fs::copy_options::overwrite_existing);
    // the real model ctor (p1-03), dimension 3 like the callers' DB_STEP_DATA(3)
    DB_STEP_DATA& m = *new DB_STEP_DATA(3);
    std::FILE* in = std::fopen(copy.string().c_str(), "r");
    std::FILE* sink = std::fopen("NUL", "w");
    Hooks hooks(m);
    read_key_stream(in, hooks, sink);
    std::fclose(in);
    std::fclose(sink);

    fs::path outp = fs::temp_directory_path() / keytest::tmp_name("kw_g2obj_oracle_out.key");
    std::FILE* out = std::fopen(outp.string().c_str(), "wb");
    const std::string input = slurp(copy);
    Records in_recs = records_of(input);
    for (LLOBJ_DATA* o = at<LLOBJ_DATA*>(m, 0x1380); o != nullptr; o = at<LLOBJ_DATA*>(*o, 0x3940)) {
        def::key::g2obj::write_object(m, out, o, 0, false);
#if !__has_include(<defkey/kw_g2mg.hpp>)  // with kw-g2mg, write_object prints them in the mesh block
        def::key::g2obj::KeyRef ref{at<std::int32_t>(*o, 8)};
        for (auto& rec : in_recs[at<std::int32_t>(*o, 8)]) {
            if (rec.kw == "RMDPTH") def::key::g2obj::write_rmdpth(out, m, ref);
            if (rec.kw == "RMSTRK") def::key::g2obj::write_rmstrk(out, m, ref);
            if (rec.kw == "RMTIME") def::key::g2obj::write_rmtime(out, m, ref);
        }
#endif
    }
    std::fclose(out);
    Records mine = records_of(slurp(outp));
    Records orig = records_of(slurp(oracle_out));
    ++tot.files;

    for (auto& [obj, recs] : orig) {
        std::set<std::string> given, cmp;
        for (auto& r : in_recs[obj]) given.insert(r.kw);
        for (auto& r : recs) cmp.insert(r.kw);
        for (auto& r : mine[obj]) cmp.insert(r.kw);
        for (const std::string& kw : given) cmp.insert(kw);
        for (const std::string& kw : cmp) {
            if (my_keywords().count(kw) == 0) continue;
            if ((kw == "RMDPTH" || kw == "RMSTRK" || kw == "RMTIME" || kw == "ANGMVY") && given.count(kw) == 0)
                continue;
            std::vector<std::string> a, b;
            for (auto& r : orig[obj])
                if (r.kw == kw) a.push_back(r.text);
            for (auto& r : mine[obj])
                if (r.kw == kw) b.push_back(r.text);
            ++tot.compared;
            if (a == b) {
                ++tot.ok_by_kw[kw];
                continue;
            }
            ++tot.mismatched;
            ++tot.bad_by_kw[kw];
            if (verbose) {
                std::printf("MISMATCH %s obj %d %s\n  orig:", src.string().c_str(), obj, kw.c_str());
                for (auto& s : a) std::printf("\n%s", s.c_str());
                std::printf("\n  port:");
                for (auto& s : b) std::printf("\n%s", s.c_str());
                std::printf("\n");
            }
        }
    }
    // the model is leaked on purpose (its dtor frees every object; not what this test checks)
    std::error_code ec;  // a scanner may still hold the scratch file; leaving it is harmless
    fs::remove(copy, ec);
    fs::remove(outp, ec);
}

} // namespace

TEST_CASE(oracle_round_trip_of_kw_g2obj_keywords)
{
    keytest::legacy_printf();
    fs::path cache = env("DEFORM_ORACLE_CACHE", "");
    if (cache.empty()) cache = fs::path(env("USERPROFILE", "")) / ".deform-oracle";
    if (!fs::exists(cache / "entries")) {
        std::printf("no oracle cache at %s: skipped\n", cache.string().c_str());
        return;
    }
    const std::uintmax_t max_bytes = std::strtoull(env("KW_G2OBJ_MAX_BYTES", "2000000").c_str(), nullptr, 10);
    const int max_files = std::atoi(env("KW_G2OBJ_MAX_FILES", "100000").c_str());
    const bool verbose = env("KW_G2OBJ_VERBOSE", "0") == "1";
    Totals tot;
    for (auto& d1 : fs::directory_iterator(cache / "entries")) {
        if (!d1.is_directory()) continue;
        for (auto& d2 : fs::directory_iterator(d1.path())) {
            fs::path rj = d2.path() / "result.json";
            if (!fs::exists(rj)) continue;
            std::string j = slurp(rj);
            if (json_string(j, "label").rfind("kfrw/", 0) != 0) continue;
            fs::path out = d2.path() / "files" / "KFWRIT_OUT.KEY";
            fs::path src = json_string(j, "source");
            if (!fs::exists(out) || src.empty() || !fs::exists(src)) {
                ++tot.skipped;
                continue;
            }
            if (fs::file_size(src) > max_bytes || tot.files >= max_files) {
                ++tot.skipped;
                continue;
            }
            check_case(src, out, tot, verbose);
        }
    }
    std::printf("kfrw cases: %d checked, %d skipped; records compared %d, mismatched %d\n",
                tot.files, tot.skipped, tot.compared, tot.mismatched);
    for (auto& [kw, n] : tot.ok_by_kw) {
        int bad = tot.bad_by_kw.count(kw) ? tot.bad_by_kw[kw] : 0;
        std::printf("  %-6s ok %5d  bad %5d\n", kw.c_str(), n, bad);
    }
    for (auto& [kw, n] : tot.bad_by_kw)
        if (tot.ok_by_kw.count(kw) == 0) std::printf("  %-6s ok %5d  bad %5d\n", kw.c_str(), 0, n);
    CHECK_EQ(tot.mismatched, 0);
}

int main() { return testkit::run_all(); }
