// ctest-labels: kw-g2bc win
// Tier-1 tests of the window BCs WINPRS / WINTMP / WINATM / WINRSE (packet kw-g2bc, family win):
// parser cases 0x52..0x55 of FUN_14038b8a0, writer-map steps 48-52 of FUN_14029e250 and
// FUN_1402ab880.
//
// Golden files (tests/data/kw_g2bc/win): <case>.key are hand-written inputs, <case>.expected the
// WIN* records of the original DEF_PRE's KFWRIT after KFREAD of that input (oracle_cases.json,
// cache %USERPROFILE%\.deform-oracle; CR removed). err_* inputs contain read errors (the loop goes
// on after each "Reading Error"). The corpus test compares the WIN* records of every cached
// kfrw/<corpus file> case whose input uses them (WINTMP only in the corpus).
#define _CRT_SECURE_NO_WARNINGS
#include "key_test_util.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <defkey/key_reader.hpp>
#include <defkey/kw_g2bc.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defkey/line_io.hpp>
#include <defmodel/bc/window_bc.hpp>
#include <defmodel/layout/cls_140269250.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/window/llwindef_data.hpp>
#include <defmodel/window/window_list.hpp>
#include <testkit/check.hpp>

namespace fs = std::filesystem;
using def::model::Cls_140269250;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;
using def::model::LLWINDEF_DATA;

namespace {

template <class T, class C>
T& at(C& base, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<unsigned char*>(&base) + off);
}

std::string slurp(const fs::path& p)
{
    std::ifstream in(p, std::ios::binary);
    std::ostringstream s;
    s << in.rdbuf();
    std::string o;
    for (char c : s.str())
        if (c != '\r') o += c;
    return o;
}

std::string data_path(const char* name)
{
    return (testkit::repo_root() / "port" / "defkey" / "tests" / "data" / "kw_g2bc" / "win" / name).string();
}

std::string env(const char* name, const char* def)
{
    const char* v = std::getenv(name);
    return v != nullptr && *v != '\0' ? v : def;
}

bool is_win(const std::string& l)
{
    return l.rfind("WINPRS", 0) == 0 || l.rfind("WINTMP", 0) == 0 || l.rfind("WINATM", 0) == 0 ||
           l.rfind("WINRSE", 0) == 0;
}

// The WIN* records of a KEY text: a line starting with a WIN* name and the following lines that
// start with a blank. Returns object number -> concatenated records, in file order.
std::map<int, std::string> win_records(const std::string& text)
{
    std::map<int, std::string> r;
    std::istringstream in(text);
    std::string l;
    int obj = -1;
    while (std::getline(in, l)) {
        if (is_win(l)) {
            obj = std::atoi(l.c_str() + 6);
            r[obj] += l + "\n";
        } else if (obj >= 0 && !l.empty() && l[0] == ' ') {
            r[obj] += l + "\n";
        } else {
            obj = -1;
        }
    }
    return r;
}

std::string join(const std::map<int, std::string>& r)
{
    std::string o;
    for (auto& [k, v] : r) o += v;
    return o;
}

// Zeroed DB_STEP_DATA (its ctor belongs to p1-03) with the dimension the KEY comment sets.
struct Model {
    void* raw;
    DB_STEP_DATA& m;
    Model() : raw(new DB_STEP_DATA(3)), m(*static_cast<DB_STEP_DATA*>(raw)) {}  // real ctor (p1-03)
    // leaked on purpose: the model dtor frees the objects, which this test does not check
    LLOBJ_DATA* first() { return at<LLOBJ_DATA*>(m, 0x1380); }
    static LLOBJ_DATA* next(LLOBJ_DATA* o) { return at<LLOBJ_DATA*>(*o, 0x3940); }
};

struct Hooks : def::key::KeyReaderHooks {
    DB_STEP_DATA& m;
    std::vector<int> statuses;  // results of the WIN* cases
    explicit Hooks(DB_STEP_DATA& model) : m(model) {}
    bool has_sub_model() override { return false; }
    void set_model_dimension(int dim) override { at<std::int32_t>(m, 0x1c) = dim; }
    void set_sub_model_dimension(int) override {}
    int parse_keyword(int group, std::FILE* f, int id) override
    {
        if (group == 2) {
            int st = def::key::g2obj::read_object_keyword(m, f, id);
            if (id >= 0xe2 && id <= 0xe5) statuses.push_back(st);
            return st;
        }
        char rest[0x400];
        std::fgets(rest, sizeof rest, f);
        return 0;
    }
};

// Read `src` (a copy) through the KEY loop.
void load(Hooks& hooks, const fs::path& src, const char* tag)
{
    fs::path copy = fs::temp_directory_path() / keytest::tmp_name(std::string("kw_g2bc_win_") + tag + ".key");
    fs::copy_file(src, copy, fs::copy_options::overwrite_existing);
    std::FILE* in = std::fopen(copy.string().c_str(), "r");
    std::FILE* sink = std::fopen("NUL", "w");
    def::key::read_key_stream(in, hooks, sink);
    std::fclose(in);
    std::fclose(sink);
    fs::remove(copy);
}

// Write every object's window lists (writer-map steps 48-52) with N = obj+8.
std::string write_all(Model& md, const char* tag)
{
    keytest::OutFile out((std::string("kw_g2bc_win_out_") + tag + ".txt").c_str());
    for (LLOBJ_DATA* o = md.first(); o != nullptr; o = Model::next(o))
        def::key::g2bc::write_window_lists(md.m, out.get(), *o, static_cast<std::uint32_t>(at<std::int32_t>(*o, 8)));
    return out.text();
}

std::string round_trip(const char* name, std::vector<int>* statuses = nullptr)
{
    keytest::legacy_printf();
    Model md;
    Hooks hooks(md.m);
    load(hooks, data_path((std::string(name) + ".key").c_str()), name);
    if (statuses != nullptr) *statuses = hooks.statuses;
    return write_all(md, name);
}

}  // namespace

// ---- oracle round trips of crafted inputs ------------------------------------------------------

TEST_CASE(oracle_win_2d)
{
    std::vector<int> st;
    const std::string mine = round_trip("win_2d", &st);
    CHECK_EQ(join(win_records(mine)), slurp(data_path("win_2d.expected")));
    CHECK_EQ(st.size(), static_cast<std::size_t>(10));
    for (int s : st) CHECK_EQ(s, 0);
}

TEST_CASE(oracle_win_3d)
{
    std::vector<int> st;
    const std::string mine = round_trip("win_3d", &st);
    CHECK_EQ(join(win_records(mine)), slurp(data_path("win_3d.expected")));
    CHECK_EQ(st.size(), static_cast<std::size_t>(10));
    for (int s : st) CHECK_EQ(s, 0);
}

// WINPRS 3D with shape 3: "Reading Error" after the window was created, reset and its origin
// stored; WINRSE with a missing corner line (EOF) -> 2.
TEST_CASE(oracle_err_prs_3d)
{
    std::vector<int> st;
    const std::string mine = round_trip("err_prs_3d", &st);
    CHECK_EQ(join(win_records(mine)), slurp(data_path("err_prs_3d.expected")));
    CHECK_EQ(st.size(), static_cast<std::size_t>(3));
    if (st.size() == 3) {
        CHECK_EQ(st[0], 0);
        CHECK_EQ(st[1], 2);
        CHECK_EQ(st[2], 2);
    }
}

// WINATM: table 2's body hits EOF -> 2, but the mode (-4) is already stored.
TEST_CASE(oracle_err_atm_2d)
{
    std::vector<int> st;
    const std::string mine = round_trip("err_atm_2d", &st);
    CHECK_EQ(join(win_records(mine)), slurp(data_path("err_atm_2d.expected")));
    CHECK_EQ(st.size(), static_cast<std::size_t>(1));
    if (!st.empty()) CHECK_EQ(st[0], 2);
}

// WINTMP: table 4's body hits EOF -> 2 before the mode (-5) is stored (header code 1).
TEST_CASE(oracle_err_tmp_2d)
{
    std::vector<int> st;
    const std::string mine = round_trip("err_tmp_2d", &st);
    CHECK_EQ(join(win_records(mine)), slurp(data_path("err_tmp_2d.expected")));
    CHECK_EQ(st.size(), static_cast<std::size_t>(1));
    if (!st.empty()) CHECK_EQ(st[0], 2);
}

// ---- 1402ab880 ---------------------------------------------------------------------------------

// The type-5 writer has the same WINPRS code as step 48: its output equals the original's WINPRS
// records of the same model. N = obj+8 when the given number is < 1.
TEST_CASE(winprs_block_matches_step_48)
{
    keytest::legacy_printf();
    for (const char* name : {"win_2d", "win_3d"}) {
        Model md;
        Hooks hooks(md.m);
        load(hooks, data_path((std::string(name) + ".key").c_str()), name);
        keytest::OutFile out("kw_g2bc_win_block.txt");
        int rc = 0;
        for (LLOBJ_DATA* o = md.first(); o != nullptr; o = Model::next(o))
            rc |= def::key::g2bc::write_winprs_block(md.m, out.get(), *o, 0);
        CHECK_EQ(rc, 0);
        std::string want;
        std::istringstream in(slurp(data_path((std::string(name) + ".expected").c_str())));
        std::string l;
        bool on = false;
        while (std::getline(in, l)) {
            if (is_win(l)) on = l.rfind("WINPRS", 0) == 0;
            if (on) want += l + "\n";
        }
        CHECK_EQ(out.text(), want);
    }
}

// ---- reader details worked out from the decompile ----------------------------------------------

namespace {
struct Direct {
    Model md;
    LLOBJ_DATA* obj = nullptr;
    explicit Direct(int dim)
    {
        keytest::legacy_printf();
        at<std::int32_t>(md.m, 0x1c) = dim;
    }
    // "no rest-of-line" reads one keyword through the group 2 parser
    int read(int id, const std::string& text)
    {
        keytest::InFile in("kw_g2bc_win_direct.txt", text);
        int st = def::key::g2obj::read_object_keyword(md.m, in.get(), id);
        obj = md.first();
        return st;
    }
    Cls_140269250* list(std::size_t off) { return &at<Cls_140269250>(*obj, off); }
};
}  // namespace

TEST_CASE(winprs_unset_dimension_reads_nothing)
{
    Direct d(0);
    keytest::InFile in("kw_g2bc_win_unset.txt", " 1 1 2 0 0 1\n0 0\n");
    CHECK_EQ(def::key::g2obj::read_object_keyword(d.md.m, in.get(), 0xe2), 0);
    CHECK_EQ(in.rest(), std::string(" 1 2 0 0 1\n0 0\n"));  // the object number was read
}

TEST_CASE(winrse_3d_checks_k_before_creating)
{
    Direct d(3);
    CHECK_EQ(d.read(0xe5, " 1 2 2\n"), 2);
    CHECK_EQ(d.list(0x2fd0)->count, 0);
    Direct u(0);  // unset dimension: created and reset first
    CHECK_EQ(u.read(0xe5, " 1 1 2\n"), 2);
    CHECK_EQ(u.list(0x2fd0)->count, 1);
    Direct t(2);
    CHECK_EQ(t.read(0xe5, " 1 1 0\n"), 2);
    CHECK_EQ(t.list(0x2fd0)->count, 1);
    CHECK_EQ(winlist_nth(t.list(0x2fd0), 1)->u174, 10);
}

TEST_CASE(window_fields_after_read)
{
    Direct d(3);
    CHECK_EQ(d.read(0xe4, " 1 2 2 1 3 -7\n1 2 3\n0 0 0\n0 0 1\n0.25 0.5\n2\n0 1\n1 2\n2 2\n0 1\n5 6\n1 2 3 4\n"), 0);
    LLWINDEF_DATA* w = winlist_nth(d.list(0x2f10), 2);
    CHECK_EQ(d.list(0x2f10)->count, 2);
    CHECK_EQ(w->index, 2);
    CHECK_EQ(w->u174, 4);
    CHECK_EQ(w->dim, 3);
    CHECK_EQ(w->shape, 2);
    CHECK_EQ(w->nface, 4);  // r1 = 0.25 >= 1e-20: a fourth rim point
    CHECK_EQ(w->orient, 2);
    CHECK_EQ(w->orient_id, 7);
    CHECK_EQ(w->t568[2].ndim, 2);
    CHECK_EQ(w->t568[2].var[0], 1);
    CHECK_EQ(w->t568[2].var[1], 0xc);
    CHECK_EQ(w->t568[2].n[1], 2);
    CHECK_EQ(winlist_nth(d.list(0x2f10), 1)->u174, 0);  // placeholder: ctor kind 0
}

// ---- model helpers -----------------------------------------------------------------------------

TEST_CASE(winlist_remove_keeps_links)
{
    Direct d(2);
    CHECK_EQ(d.read(0xe5, " 1 3 1\n1 0 0\n"), 0);
    Cls_140269250* l = d.list(0x2fd0);
    CHECK_EQ(l->count, 3);
    LLWINDEF_DATA* w1 = winlist_nth(l, 1);
    LLWINDEF_DATA* w2 = winlist_nth(l, 2);
    LLWINDEF_DATA* w3 = winlist_nth(l, 3);
    CHECK(!def::model::window_has_shape(w1));
    CHECK(def::model::window_has_shape(w3));
    CHECK_EQ(def::model::winlist_remove(l, w2), 1);
    CHECK_EQ(l->count, 2);
    CHECK(l->head == w1 && w1->next == w3 && l->tail == w3);
    CHECK_EQ(def::model::winlist_remove(l, w3), 1);
    CHECK(l->tail == w1 && w1->next == nullptr);
    LLWINDEF_DATA other(nullptr);
    CHECK_EQ(def::model::winlist_remove(l, &other), 0);
    CHECK_EQ(def::model::winlist_remove(l, w1), 1);
    CHECK(l->head == nullptr);
    CHECK_EQ(l->count, 0);
}

// ---- corpus ------------------------------------------------------------------------------------

namespace {
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
}  // namespace

TEST_CASE(oracle_corpus_window_records)
{
    keytest::legacy_printf();
    fs::path cache = env("DEFORM_ORACLE_CACHE", "");
    if (cache.empty()) cache = fs::path(env("USERPROFILE", "")) / ".deform-oracle";
    if (!fs::exists(cache / "entries")) {
        std::printf("no oracle cache at %s: skipped\n", cache.string().c_str());
        return;
    }
    int files = 0, objects = 0, bad = 0;
    for (auto& d1 : fs::directory_iterator(cache / "entries")) {
        if (!d1.is_directory()) continue;
        for (auto& d2 : fs::directory_iterator(d1.path())) {
            fs::path rj = d2.path() / "result.json";
            if (!fs::exists(rj)) continue;
            std::string j = slurp(rj);
            if (json_string(j, "label").rfind("kfrw/", 0) != 0) continue;
            fs::path out = d2.path() / "files" / "KFWRIT_OUT.KEY";
            fs::path src = json_string(j, "source");
            if (!fs::exists(out) || src.empty() || !fs::exists(src)) continue;
            const std::string input = slurp(src);
            auto given = win_records(input);
            if (given.empty()) continue;
            Model md;
            Hooks hooks(md.m);
            load(hooks, src, "corpus");
            auto mine = win_records(write_all(md, "corpus"));
            auto orig = win_records(slurp(out));
            ++files;
            for (auto& [obj, text] : given) {
                ++objects;
                if (mine[obj] != orig[obj]) {
                    ++bad;
                    std::printf("MISMATCH %s obj %d\n--- orig\n%s--- port\n%s", src.string().c_str(), obj,
                                orig[obj].c_str(), mine[obj].c_str());
                }
            }
        }
    }
    std::printf("corpus kfrw cases with WIN*: %d files, %d objects compared, %d mismatched\n", files, objects, bad);
    CHECK_EQ(bad, 0);
}

int main() { return testkit::run_all(); }
