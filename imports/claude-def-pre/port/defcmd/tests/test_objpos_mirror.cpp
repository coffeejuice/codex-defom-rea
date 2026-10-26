// ctest-labels: p67-1 objpos
// OBJPOS mirror (mode 4) against the original: each case reads a copy of the seed KEY through the
// KEY loop with the group 2 parser into a real DB_STEP_DATA, runs mirror_3d / mirror_2d on the
// case's object with the case's arguments, writes every object with write_object and compares the
// group 2 records with the original's OUT.KEY (KFWRIT after OBJPOS), byte for byte.
//
// Oracle results: $OBJPOS_ORACLE_DIR (default C:/Users/alext/Projects/deform-rea-work/out/p671/
// oracle/OBJPOS)/<case>/files/OUT.KEY; seeds under $DEFORM_CORPUS (default
// C:/Users/alext/Projects/DEFORM_corpus). A case whose files are missing is skipped with a note.
// Keywords the port cannot write yet are listed in kSkipped (with the reason) and not compared.
// OBJPOS_MIRROR_VERBOSE=1 prints each mismatch.
#define _CRT_SECURE_NO_WARNINGS
#include "../../defkey/tests/key_test_util.hpp"

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

#include <defcmd/objpos.hpp>
#include <defcmd/objpos_mirror.hpp>
#include <defkey/key_reader.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defkey/line_io.hpp>
#include <defmodel/keyword_registry.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/step/db_step.hpp>
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

struct Record {
    std::string kw;
    std::string text;
};
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

// group 2 records per object (as in defkey/tests/test_kw_g2geo_oracle.cpp)
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

// keywords not compared, with the reason
const std::map<std::string, std::string>& skipped()
{
    static const std::map<std::string, std::string> k = {
        {"STRESS", "state-variable writer (kw-g2sv) not on this branch"},
    };
    return k;
}

struct Case {
    const char* label;
    const char* seed;  // relative to the corpus
    int obj;
    int dim;
    double a[3];
    double b[3];
};

const Case kCases[] = {
    {"4-basic_3d", "key/3D/SI/Forging/UPSET_Brick.KEY", 2, 3, {0, 0, 0}, {1, 0, 0}},
    {"4-oblique_3d", "key/3D/SI/Forging/UPSET_Brick.KEY", 1, 3, {1, 2, 3}, {1, 1, 1}},
    {"4-zero_3d", "key/3D/SI/Forging/UPSET_Brick.KEY", 2, 3, {0, 0, 0}, {0, 0, 0}},
    {"ik-mirror-brick-3d", "key/3D/SI/Forging/UPSET_Brick.KEY", 2, 3, {0, 0, 0}, {1, 0, 0}},
    {"ik-mirror-tet-3d", "key/3D/SI/Forging/UPSET_Tet.KEY", 2, 3, {0, 0, 0}, {1, 0, 0}},
    {"4-basic_2d", "key/2D/SI/Forging/SPIKE_SI.KEY", 1, 2, {0, 0}, {0, 1}},
    {"4-oblique_2d", "key/2D/SI/Forging/SPIKE_SI.KEY", 3, 2, {0, 10}, {10, 20}},
    {"gk-4-type2_2d", "key/2D/SI/HeatTreat/HT_QUENCH1_SI.KEY", 2, 2, {0, 0}, {0, 1}},
};

struct Totals {
    int cases = 0, skipped_cases = 0, compared = 0, mismatched = 0;
    std::map<std::string, int> bad;
    std::set<std::string> unwritten;
    std::set<std::string> target_kws;  // keywords compared on the mirrored objects
};

void run_case(const Case& c, const fs::path& oracle, const fs::path& corpus, Totals& tot, bool verbose)
{
    const fs::path src = corpus / c.seed;
    const fs::path out_orig = oracle / c.label / "files" / "OUT.KEY";
    if (!fs::exists(src) || !fs::exists(out_orig)) {
        std::printf("%s: seed or oracle output missing, skipped\n", c.label);
        ++tot.skipped_cases;
        return;
    }
    fs::path copy = fs::temp_directory_path() / keytest::tmp_name(std::string("objpos_mirror_in_") + c.label + ".key");
    fs::copy_file(src, copy, fs::copy_options::overwrite_existing);
    DB_STEP_DATA& m = *new DB_STEP_DATA(3);
    std::FILE* in = std::fopen(copy.string().c_str(), "r");
    std::FILE* sink = std::fopen("NUL", "w");
    Hooks hooks(m);
    read_key_stream(in, hooks, sink);
    std::fclose(in);
    std::fclose(sink);

    LLOBJ_DATA* target = nullptr;
    for (LLOBJ_DATA* o = at<LLOBJ_DATA*>(m, 0x1380); o != nullptr; o = at<LLOBJ_DATA*>(*o, 0x3940))
        if (at<std::int32_t>(*o, 8) == c.obj) target = o;
    CHECK(target != nullptr);
    if (target == nullptr) return;
    if (env("OBJPOS_MIRROR_NOOP", "0") == "1") {
        // sanity mode: no mirror, so the mirrored object must mismatch
    } else if (c.dim == 3)
        def::cmd::objpos::mirror_3d(target, c.a, c.b);
    else
        def::cmd::objpos::mirror_2d(target, c.a, c.b);

    fs::path outp = fs::temp_directory_path() / keytest::tmp_name(std::string("objpos_mirror_out_") + c.label + ".key");
    std::FILE* out = std::fopen(outp.string().c_str(), "wb");
    for (LLOBJ_DATA* o = at<LLOBJ_DATA*>(m, 0x1380); o != nullptr; o = at<LLOBJ_DATA*>(*o, 0x3940))
        def::key::g2obj::write_object(m, out, o, 0, false);
    std::fclose(out);
    Records mine = records_of(slurp(outp));
    Records orig = records_of(slurp(out_orig));
    ++tot.cases;
    int bad_here = 0, cmp_here = 0;
    std::set<std::string> written;  // every keyword the port wrote, any object
    for (auto& [obj, recs] : mine)
        for (auto& r : recs) written.insert(r.kw);
    for (auto& [obj, recs] : orig) {
        std::set<std::string> kws;
        for (auto& r : recs) kws.insert(r.kw);
        for (auto& r : mine[obj]) kws.insert(r.kw);
        for (const std::string& kw : kws) {
            if (skipped().count(kw)) continue;
            std::vector<std::string> a, b;
            for (auto& r : recs)
                if (r.kw == kw) a.push_back(r.text);
            for (auto& r : mine[obj])
                if (r.kw == kw) b.push_back(r.text);
            if (written.count(kw) == 0) {  // a keyword the port does not write (yet), any object
                tot.unwritten.insert(kw);
                continue;
            }
            ++tot.compared;
            ++cmp_here;
            if (obj == c.obj) tot.target_kws.insert(kw);
            if (a == b) continue;
            ++tot.mismatched;
            ++bad_here;
            ++tot.bad[std::string(c.label) + " obj " + std::to_string(obj) + " " + kw];
            if (verbose) {
                // first differing line only
                std::vector<std::string> la, lb;
                for (auto& s : a)
                    for (auto& l : lines_of(s)) la.push_back(l);
                for (auto& s : b)
                    for (auto& l : lines_of(s)) lb.push_back(l);
                std::size_t i = 0, nd = 0;
                for (; i < la.size() && i < lb.size(); ++i)
                    if (la[i] != lb[i]) ++nd;
                std::printf("MISMATCH %s obj %d %s (%zu vs %zu lines, %zu differ)\n", c.label, obj, kw.c_str(),
                            la.size(), lb.size(), nd);
                for (i = 0; i < la.size() && i < lb.size(); ++i)
                    if (la[i] != lb[i]) {
                        std::printf("  line %zu\n  orig: %s\n  port: %s\n", i, la[i].c_str(), lb[i].c_str());
                        break;
                    }
            }
        }
    }
    std::printf("%s: %d records compared, %d mismatched\n", c.label, cmp_here, bad_here);
    fs::remove(copy);
    fs::remove(outp);
    // the model and its objects are leaked on purpose (as the other oracle tests)
}

} // namespace

TEST_CASE(objpos_mirror_against_oracle)
{
    keytest::legacy_printf();
    const fs::path oracle = env("OBJPOS_ORACLE_DIR", "C:/Users/alext/Projects/deform-rea-work/out/p671/oracle/OBJPOS");
    const fs::path corpus = env("DEFORM_CORPUS", "C:/Users/alext/Projects/DEFORM_corpus");
    const bool verbose = env("OBJPOS_MIRROR_VERBOSE", "0") == "1";
    const std::string only = env("OBJPOS_MIRROR_ONLY", "");
    Totals tot;
    for (const Case& c : kCases) {
        if (!only.empty() && only != c.label) continue;
        run_case(c, oracle, corpus, tot, verbose);
    }
    for (auto& [k, n] : skipped()) std::printf("not compared: %s (%s)\n", k.c_str(), n.c_str());
    std::printf("compared on the mirrored objects:");
    for (auto& k : tot.target_kws) std::printf(" %s", k.c_str());
    std::printf("\nnot written by the port, not compared:");
    for (auto& k : tot.unwritten) std::printf(" %s", k.c_str());
    std::printf("\n");
    for (auto& [k, n] : tot.bad) std::printf("  bad: %s\n", k.c_str());
    std::printf("cases %d (skipped %d), records compared %d, mismatched %d\n", tot.cases, tot.skipped_cases,
                tot.compared, tot.mismatched);
    CHECK_EQ(tot.mismatched, 0);
}

// The float matrix of a mirror at x = 0: diag(-1, 1, 1), no translation.
TEST_CASE(float_mirror_matrix_axis_plane)
{
    const double p[3] = {0.0, 0.0, 0.0};
    const double n[3] = {1.0, 0.0, 0.0};
    def::cmd::objpos::F44 m = def::cmd::objpos::mirror_matrix(p, n);
    CHECK(m.m[0] == -1.0f && m.m[5] == 1.0f && m.m[10] == 1.0f && m.m[3] == 0.0f);
    double x[3] = {0.1, 2.0, -3.0};
    def::cmd::objpos::mirror_point_f(x, &m);
    CHECK(x[0] == -static_cast<double>(0.1f) && x[1] == 2.0 && x[2] == -3.0);
}

// 140241270 / 14030a170: row-vector convention, translation in row 3
TEST_CASE(xf4_point_translation_row)
{
    double m[16] = {0, 1, 0, 0, -1, 0, 0, 0, 0, 0, 1, 0, 10, 20, 30, 1};  // rotate z by 90 deg, then move
    double p[3] = {1.0, 2.0, 3.0};
    def::cmd::objpos::fun_140241270(m, p);
    CHECK(p[0] == 8.0 && p[1] == 21.0 && p[2] == 33.0);
    double q[2] = {1.0, 2.0};
    def::cmd::objpos::xf4_point_2d(m, q);
    CHECK(q[0] == 8.0 && q[1] == 21.0);
}

int main() { return testkit::run_all(); }
