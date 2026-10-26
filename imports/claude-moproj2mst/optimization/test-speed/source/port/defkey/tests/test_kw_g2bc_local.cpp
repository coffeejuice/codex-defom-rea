// ctest-labels: kw-g2bc local
// kw-g2bc local family: BCCFNC, LOCTMP, LOCATM, LOCDEF, RSEDEF (parser cases 0x11, 0x56..0x58,
// 0x9e; writers 1402b4050, 1402ac560 with 1402ae590 / 1402aeb40 / 1402aef30, 1402b7140; accessors
// 1402b4180, 1402ae950, 1402aeda0, 1402af1f0).
//
// 1. Crafted oracle cases (tests/data/kw_g2bc/local, oracle_cases.json): each input is read with
//    the group 2 parser through the KEY loop, then every object's records of these keywords are
//    written as FUN_14029e250 orders them (BCCFNC list, LOC* block, RSEDEF) and compared with
//    the records of the original's KFWRIT output (*.expected) and its messages (*.messages).
// 2. Corpus: every cached kfrw/<path> run whose input gives one of these keywords; for each
//    object and keyword the input gives, the records written must equal the original's.
//    Without the cache this part passes with a note. KW_G2BC_LOCAL_MAX_BYTES (default 3000000)
//    caps the input size; KW_G2BC_LOCAL_VERBOSE=1 prints mismatches.
// 3. Handler and accessor details from the code (expected values derived by hand in comments).
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
#include <defkey/kw_g2bc.hpp>
#include <defkey/kw_g2bc_tables.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defmodel/layout/cls_1401364a0.hpp>
#include <defmodel/layout/cls_140269250.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/window/llwindef_data.hpp>
#include <defmodel/window/window_list.hpp>
#include <testkit/check.hpp>

namespace fs = std::filesystem;
using def::model::DB_STEP_DATA;
using def::model::DYNARY;
using def::model::LLOBJ_DATA;
using def::model::LLWINDEF_DATA;

namespace {

template <class T, class C>
T& at(C& base, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<unsigned char*>(&base) + off);
}

const std::set<std::string>& my_keywords()
{
    static const std::set<std::string> k = {"BCCFNC", "LOCTMP", "LOCATM", "LOCDEF", "RSEDEF"};
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
    std::string t = s.str();
    std::string out;
    for (char c : t)
        if (c != '\r') out += c;
    return out;
}

// A zero-filled model of dimension `dim` (DB_WRITE_VAR ctor default 3); leaked with its objects.
DB_STEP_DATA& new_model(int dim)
{
    DB_STEP_DATA& m = *new DB_STEP_DATA(3);  // the real model ctor (p1-03)
    at<std::int32_t>(m, 0x1c) = dim;
    def::model::step_setup_array_sizes(m, dim);
    return m;
}

struct Hooks : def::key::KeyReaderHooks {
    DB_STEP_DATA& m;
    explicit Hooks(DB_STEP_DATA& model) : m(model) {}
    bool has_sub_model() override { return false; }
    void set_model_dimension(int dim) override
    {
        at<std::int32_t>(m, 0x1c) = dim;
    def::model::step_setup_array_sizes(m, dim);
        def::model::step_setup_array_sizes(m, dim);  // 1401d09c0 (the model reset on a dimension change)
    }
    void set_sub_model_dimension(int) override {}
    int parse_keyword(int group, std::FILE* f, int id) override
    {
        if (group == 2) return def::key::g2obj::read_object_keyword(m, f, id);
        return 0;
    }
};

// Reads `path` (a copy is made) into a new model; the loop's messages go to `messages`.
DB_STEP_DATA& read_model(const fs::path& path, std::string* messages)
{
    fs::path copy = fs::temp_directory_path() / keytest::tmp_name("kw_g2bc_local_input.key");
    fs::copy_file(path, copy, fs::copy_options::overwrite_existing);
    DB_STEP_DATA& m = new_model(3);
    fs::path msg = fs::temp_directory_path() / keytest::tmp_name("kw_g2bc_local_messages.txt");
    std::FILE* in = std::fopen(copy.string().c_str(), "r");
    std::FILE* out = std::fopen(msg.string().c_str(), "w");
    Hooks hooks(m);
    def::key::read_key_stream(in, hooks, out);
    std::fclose(in);
    std::fclose(out);
    if (messages != nullptr) *messages = slurp(msg);
    fs::remove(copy);
    fs::remove(msg);
    return m;
}

// The records of this family for every object, in FUN_14029e250's order.
std::string write_family(DB_STEP_DATA& m)
{
    fs::path outp = fs::temp_directory_path() / keytest::tmp_name("kw_g2bc_local_out.key");
    std::FILE* out = std::fopen(outp.string().c_str(), "wb");
    for (LLOBJ_DATA* o = at<LLOBJ_DATA*>(m, 0x1380); o != nullptr; o = at<LLOBJ_DATA*>(*o, 0x3940)) {
        const std::int32_t no = at<std::int32_t>(*o, 8);
        for (std::int32_t i = 0; i < at<std::int32_t>(*o, 0x2e68 + 0x10); ++i)  // writer-map step 26
            def::key::g2bc::write_keyword(161, out, m, def::key::g2bc::KeyRef{no, i + 1});
        def::key::g2bc::write_local_bcs(m, out, *o, 0);  // step 53
        def::key::g2bc::write_rsedef(out, m, def::key::g2bc::KeyRef{no});  // step 60
    }
    std::fclose(out);
    std::string s = slurp(outp);
    fs::remove(outp);
    return s;
}

std::vector<std::string> lines_of(const std::string& s)
{
    std::vector<std::string> v;
    std::size_t a = 0;
    while (a < s.size()) {
        std::size_t b = s.find('\n', a);
        if (b == std::string::npos) b = s.size();
        v.push_back(s.substr(a, b - a));
        a = b + 1;
    }
    return v;
}

struct Record {
    std::string kw;
    std::string text;
};
using Records = std::map<int, std::vector<Record>>;  // object number -> records in order

// Records of this family: a line starting with one of its names starts one (object = second
// token); following lines that start with a blank belong to it.
Records records_of(const std::string& text)
{
    Records r;
    std::vector<std::string> ls = lines_of(text);
    for (std::size_t i = 0; i < ls.size(); ++i) {
        char name[64] = {0};
        int obj = 0;
        if (ls[i].empty() || ls[i][0] == ' ') continue;
        if (std::sscanf(ls[i].c_str(), "%63s %i", name, &obj) < 2) continue;
        if (my_keywords().count(name) == 0) continue;
        Record rec{name, ls[i] + "\n"};
        std::size_t j = i + 1;
        while (j < ls.size() && !ls[j].empty() && ls[j][0] == ' ') rec.text += ls[j++] + "\n";
        r[obj].push_back(rec);
        i = j - 1;
    }
    return r;
}

fs::path data_dir() { return testkit::repo_root() / "port/defkey/tests/data/kw_g2bc/local"; }

void check_crafted(const char* name)
{
    keytest::legacy_printf();
    std::string messages;
    DB_STEP_DATA& m = read_model(data_dir() / (std::string(name) + ".key"), &messages);
    const std::string mine = write_family(m);
    const std::string expected = slurp(data_dir() / (std::string(name) + ".expected"));
    CHECK_EQ(mine, expected);
    // the loop's "ERROR: Keyword" / "WARNING: Keyword" lines (the original's stdout, without the
    // KFREAD line of the command file)
    std::string got;
    for (const std::string& l : lines_of(messages))
        if (l.rfind("ERROR: Keyword", 0) == 0 || l.rfind("WARNING: Keyword", 0) == 0) got += l + "\n";
    fs::path mp = data_dir() / (std::string(name) + ".messages");
    CHECK_EQ(got, fs::exists(mp) ? slurp(mp) : std::string());
}

// "key": "value" of the first occurrence, JSON backslash escapes undone
std::string json_string(const std::string& j, const char* key)
{
    std::string pat = std::string("\"") + key + "\": \"";
    std::size_t p = j.find(pat);
    if (p == std::string::npos) return {};
    p += pat.size();
    std::string out;
    while (p < j.size() && j[p] != '"') {
        if (j[p] == '\\' && p + 1 < j.size()) ++p;
        out += j[p++];
    }
    return out;
}

bool gives_my_keyword(const std::string& text)
{
    for (const std::string& k : my_keywords())
        if (text.compare(0, k.size(), k) == 0 || text.find("\n" + k) != std::string::npos) return true;
    return false;
}

// The LLOBJ_DATA list of `m`, first object.
LLOBJ_DATA* first_object(DB_STEP_DATA& m) { return at<LLOBJ_DATA*>(m, 0x1380); }

// A model read from text (helper for part 3).
DB_STEP_DATA& model_from(const std::string& text)
{
    fs::path p = fs::temp_directory_path() / keytest::tmp_name("kw_g2bc_local_text.key");
    {
        std::ofstream o(p, std::ios::binary);
        o << text;
    }
    DB_STEP_DATA& m = read_model(p, nullptr);
    fs::remove(p);
    return m;
}

std::string capture(int (*fn)(std::FILE*, DB_STEP_DATA&, const def::key::g2bc::KeyRef&), DB_STEP_DATA& m,
                    const def::key::g2bc::KeyRef& ref, int* status)
{
    keytest::OutFile out("kw_g2bc_local_capture.txt");
    *status = fn(out.get(), m, ref);
    return out.text();
}

} // namespace

// ---- 1. crafted oracle cases --------------------------------------------------------------------

TEST_CASE(oracle_local_2d) { check_crafted("local_2d"); }
TEST_CASE(oracle_local_3d) { check_crafted("local_3d"); }
TEST_CASE(oracle_local_quirks_2d) { check_crafted("local_quirks_2d"); }

// ---- 2. corpus ------------------------------------------------------------------------------------

TEST_CASE(oracle_corpus_round_trip)
{
    keytest::legacy_printf();
    fs::path cache = env("DEFORM_ORACLE_CACHE", "");
    if (cache.empty()) cache = fs::path(env("USERPROFILE", "")) / ".deform-oracle";
    if (!fs::exists(cache / "entries")) {
        std::printf("no oracle cache at %s: skipped\n", cache.string().c_str());
        return;
    }
    const std::uintmax_t max_bytes =
        std::strtoull(env("KW_G2BC_LOCAL_MAX_BYTES", "3000000").c_str(), nullptr, 10);
    const bool verbose = env("KW_G2BC_LOCAL_VERBOSE", "0") == "1";
    int files = 0, skipped = 0, compared = 0, bad = 0;
    std::map<std::string, int> ok_by_kw, bad_by_kw;
    for (auto& d1 : fs::directory_iterator(cache / "entries")) {
        if (!d1.is_directory()) continue;
        for (auto& d2 : fs::directory_iterator(d1.path())) {
            fs::path rj = d2.path() / "result.json";
            if (!fs::exists(rj)) continue;
            const std::string j = slurp(rj);
            if (json_string(j, "label").rfind("kfrw/", 0) != 0) continue;
            fs::path out = d2.path() / "files" / "KFWRIT_OUT.KEY";
            fs::path src = json_string(j, "source");
            if (!fs::exists(out) || src.empty() || !fs::exists(src)) continue;
            const std::string input = slurp(src);
            if (!gives_my_keyword(input)) continue;
            if (fs::file_size(src) > max_bytes) {
                ++skipped;
                continue;
            }
            ++files;
            DB_STEP_DATA& m = read_model(src, nullptr);
            Records mine = records_of(write_family(m));
            Records orig = records_of(slurp(out));
            for (auto& [obj, recs] : records_of(input)) {
                std::set<std::string> given;
                for (auto& r : recs) given.insert(r.kw);
                for (const std::string& kw : given) {
                    std::vector<std::string> a, b;
                    for (auto& r : orig[obj])
                        if (r.kw == kw) a.push_back(r.text);
                    for (auto& r : mine[obj])
                        if (r.kw == kw) b.push_back(r.text);
                    ++compared;
                    if (a == b) {
                        ++ok_by_kw[kw];
                        continue;
                    }
                    ++bad;
                    ++bad_by_kw[kw];
                    if (verbose) {
                        std::printf("MISMATCH %s obj %d %s\n  orig:", src.string().c_str(), obj, kw.c_str());
                        for (auto& s : a) std::printf("\n%s", s.c_str());
                        std::printf("\n  port:");
                        for (auto& s : b) std::printf("\n%s", s.c_str());
                        std::printf("\n");
                    }
                }
            }
        }
    }
    std::printf("kfrw cases with BCCFNC/LOC*/RSEDEF: %d checked, %d skipped (size); records compared %d, "
                "mismatched %d\n",
                files, skipped, compared, bad);
    for (auto& [kw, n] : ok_by_kw)
        std::printf("  %-6s ok %4d  bad %4d\n", kw.c_str(), n, bad_by_kw.count(kw) ? bad_by_kw[kw] : 0);
    for (auto& [kw, n] : bad_by_kw)
        if (ok_by_kw.count(kw) == 0) std::printf("  %-6s ok %4d  bad %4d\n", kw.c_str(), 0, n);
    CHECK_EQ(bad, 0);
}

// ---- 3. handlers and accessors --------------------------------------------------------------------

TEST_CASE(bccfnc_accessor_is_off_by_one)
{
    // 1402b4180 passes ref[1] - 1 to FUN_140136860, whose n < 2 gives the head: 1 and 2 both give
    // the first table, 3 the second; 0 and count + 1 leave *out unchanged (status 0).
    DB_STEP_DATA& m = model_from("BCCFNC 1 7 1\n 0 1\nBCCFNC 1 8 1\n 0 2\nBCCFNC 1 9 1\n 0 3\n");
    LLOBJ_DATA* o = first_object(m);
    REQUIRE(o != nullptr);
    auto& list = at<def::model::Cls_1401364a0>(*o, 0x2e68);
    REQUIRE(list.size() == 3);
    using def::key::g2bc::table_bccfnc;
    void* out = nullptr;
    CHECK_EQ(table_bccfnc(&m, "", {1, 1}, &out), 0);
    CHECK(out == list.at(1));
    CHECK_EQ(static_cast<DYNARY*>(out)->u54, 7);
    out = nullptr;
    CHECK_EQ(table_bccfnc(&m, "", {1, 2}, &out), 0);
    CHECK_EQ(static_cast<DYNARY*>(out)->u54, 7);
    CHECK_EQ(table_bccfnc(&m, "", {1, 3}, &out), 0);
    CHECK_EQ(static_cast<DYNARY*>(out)->u54, 8);
    out = nullptr;
    CHECK_EQ(table_bccfnc(&m, "", {1, 0}, &out), 0);
    CHECK(out == nullptr);
    CHECK_EQ(table_bccfnc(&m, "", {1, 4}, &out), 0);
    CHECK(out == nullptr);
    CHECK_EQ(table_bccfnc(&m, "", {2, 1}, &out), 3);  // no object 2
}

TEST_CASE(loc_accessors_by_name)
{
    DB_STEP_DATA& m = model_from("*\n*  DEFORM-2D\n*\n"
                                 "LOCTMP 1 1 0 0 0 0 0 1\n 1\n 2\n 3\n 4\n 5\n"
                                 "LOCATM 1 1 0 0 0 0\n 1\n 2\n 3\n"
                                 "LOCDEF 1 1 0 0 1 0\n 1\n 2\n");
    LLOBJ_DATA* o = first_object(m);
    REQUIRE(o != nullptr);
    LLWINDEF_DATA* t = def::model::winlist_nth(&at<def::model::Cls_140269250>(*o, 0x2f40), 1);
    LLWINDEF_DATA* a = def::model::winlist_nth(&at<def::model::Cls_140269250>(*o, 0x2f70), 1);
    LLWINDEF_DATA* d = def::model::winlist_nth(&at<def::model::Cls_140269250>(*o, 0x2fa0), 1);
    REQUIRE(t != nullptr && a != nullptr && d != nullptr);
    auto addr = [](LLWINDEF_DATA* w, std::size_t off) { return reinterpret_cast<unsigned char*>(w) + off; };
    using namespace def::key::g2bc;
    void* out = nullptr;
    const struct {
        int (*fn)(DB_STEP_DATA*, const char*, const KeyRef&, void**);
        LLWINDEF_DATA* w;
        const char* name;
        std::size_t off;
    } cases[] = {
        {table_loctmp, t, "Env Tempreature", 0x380}, {table_loctmp, t, "Conv. Coef.", 0x3f8},
        {table_loctmp, t, "Emissivity", 0x470},      {table_loctmp, t, "Heat Flux", 0x4e8},
        {table_loctmp, t, "Heat Trans. Coef.", 0x568}, {table_locatm, a, "Env. Atom", 0x5e0},
        {table_locatm, a, "React. Coef.", 0x658},    {table_locatm, a, "Atom Flux", 0x6d0},
        {table_locdef, d, "Pressure", 0x748},        {table_locdef, d, "Friction", 0x20},
    };
    for (const auto& c : cases) {
        out = nullptr;
        CHECK_EQ(c.fn(&m, c.name, {1, 1}, &out), 0);
        CHECK(out == addr(c.w, c.off));
    }
    // the constant read into each table (sanity of the parser path)
    CHECK_BITS(static_cast<DYNARY*>(static_cast<void*>(addr(t, 0x568)))->value, 5.0);
    CHECK_BITS(static_cast<DYNARY*>(static_cast<void*>(addr(d, 0x20)))->value, 2.0);
    // unknown name: 0 and *out unchanged; short ref, null name, no object, no entry: 3
    out = nullptr;
    CHECK_EQ(table_loctmp(&m, "Env Temperature", {1, 1}, &out), 0);
    CHECK(out == nullptr);
    CHECK_EQ(table_loctmp(&m, "Heat Flux", {1}, &out), 3);
    CHECK_EQ(table_locatm(&m, nullptr, {1, 1}, &out), 3);
    CHECK_EQ(table_locdef(&m, "Pressure", {5, 1}, &out), 3);
    CHECK(out == nullptr);
    // FUN_140269900 with n < 1 gives the head, so entry 0 is found; an empty list gives null (3)
    CHECK_EQ(table_locdef(&m, "Pressure", {1, 0}, &out), 0);
    CHECK(out == addr(d, 0x748));
}

TEST_CASE(locdef_bad_direction_stops_the_block)
{
    // 1402aef30 returns 3 without output when bc+8 is not 1..3; FUN_1402ac560 returns that status
    // and writes no later entry. LOCTMP / LOCATM come first and are written.
    keytest::legacy_printf();
    DB_STEP_DATA& m = model_from("*\n*  DEFORM-2D\n*\n"
                                 "LOCATM 1 1 0 0 0 0\n 1\n 2\n 3\n"
                                 "LOCDEF 1 2 0 0 2 0\n 1\n 2\n");
    LLOBJ_DATA* o = first_object(m);
    REQUIRE(o != nullptr);
    LLWINDEF_DATA* d1 = def::model::winlist_nth(&at<def::model::Cls_140269250>(*o, 0x2fa0), 1);
    REQUIRE(d1 != nullptr);
    d1->fric.u000 = 4;
    keytest::OutFile out("kw_g2bc_local_block.txt");
    CHECK_EQ(def::key::g2bc::write_local_bcs(m, out.get(), *o, 0), 3);
    CHECK_EQ(out.text(), std::string("LOCATM       1       1       0       0       0       0\n"
                                     "    1.0000000000E+000\n"
                                     "    2.0000000000E+000\n"
                                     "    3.0000000000E+000\n"));
    int st = 0;
    CHECK_EQ(capture(def::key::g2bc::write_locdef, m, {1, 1}, &st), std::string());
    CHECK_EQ(st, 3);
    // a reference with fewer than 2 slots: 3 from each LOC* handler
    CHECK_EQ(capture(def::key::g2bc::write_loctmp, m, {1}, &st), std::string());
    CHECK_EQ(st, 3);
    CHECK_EQ(capture(def::key::g2bc::write_locatm, m, {1}, &st), std::string());
    CHECK_EQ(st, 3);
}

TEST_CASE(renumbered_object_prints_3f24)
{
    // every handler prints obj+0x3f24 when it is > 0
    keytest::legacy_printf();
    DB_STEP_DATA& m = model_from("BCCFNC 2 1 1\n 0 1\nRSEDEF 2 8\n 1 2 3 4 5 6 7 8\n");
    LLOBJ_DATA* o = at<LLOBJ_DATA*>(*first_object(m), 0x3940);  // object 2
    REQUIRE(o != nullptr && at<std::int32_t>(*o, 8) == 2);
    at<std::int32_t>(*o, 0x3f24) = 9;
    int st = 0;
    CHECK_EQ(capture(def::key::g2bc::write_bccfnc, m, {2, 1}, &st),
             std::string("BCCFNC       9       1       1\n"
                         "    0.0000000000E+000    1.0000000000E+000\n"));
    CHECK_EQ(capture(def::key::g2bc::write_rsedef, m, {2}, &st),
             std::string("RSEDEF       9       8\n"
                         "       1    2.0000000000E+000    3.0000000000E+000    4.0000000000E+000    "
                         "5.0000000000E+000    6.0000000000E+000    7.0000000000E+000    8.0000000000E+000\n"));
    CHECK_EQ(st, 0);
    // BCCFNC tables changed elsewhere to another shape are skipped
    at<def::model::Cls_1401364a0>(*o, 0x2e68).at(1)->var[0] = 3;
    CHECK_EQ(capture(def::key::g2bc::write_bccfnc, m, {2, 1}, &st), std::string());
    CHECK_EQ(st, 0);
}

int main() { return testkit::run_all(); }
