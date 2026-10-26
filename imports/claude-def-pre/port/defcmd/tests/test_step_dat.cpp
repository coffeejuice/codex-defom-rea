// ctest-labels: p9-ac8a0 cnvt3d oracle
// STEP.DAT writer 1402ac8a0 (defcmd/step_dat.hpp) on real models read through the KEY reader
// (groups 1-5 and 7, as KFREAD does), with the real accessors of the object's node and element
// sets and the kw-g2sv bare-matrix writers:
//   * golden_layout: port/defkey/tests/data/kw_g2sv/sv_2d.key (NAFEMS_T2_SI with hand-written
//     TIMERC / NRECVL / SIZESH / ERECVL / ... records); the block structure of STEP.DAT is walked
//     and checked against the model (counts, widths, values of the matrices it gives) and
//     DEF_INT_NO_DB.DAT against the model widths; a null object writes nothing;
//   * oracle_ring_si / oracle_ht_carb1_phases: the cached oracle runs "CNVT3D two objects 1,2"
//     and "CNVT3D revolve 90 brick obj1 with phases (HT_CARB1_SI)" (tests/commands/CNVT3D) left
//     the STEP.DAT 1402ac8a0 wrote (CNVT3D -> 140147b70 -> 140148960 -> 140146050); the seed is
//     read, the writer runs for the run's objects in order and the file must equal the cached
//     one byte for byte. Without the cache or the seed they pass with a note.
// Files are written in a per-process temp directory.
#define _CRT_SECURE_NO_WARNINGS
#include "../../defkey/tests/key_test_util.hpp"

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <defcmd/step_dat.hpp>
#include <defkey/group1.hpp>
#include <defkey/key_reader.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defkey/kw_g3/kw_g3.hpp>
#include <defkey/kw_g4.hpp>
#include <defkey/kw_g57.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defrt/ftn/unit_system.hpp>
#include <defrt/ftn/units.hpp>
#include <testkit/check.hpp>

namespace fs = std::filesystem;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;

namespace {

template <class T, class B>
T& at(B* base, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<unsigned char*>(base) + off);
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

// "key": "value" after `from`, backslash escapes undone
std::string json_string(const std::string& j, const char* key, std::size_t from = 0)
{
    const std::string pat = std::string("\"") + key + "\": \"";
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

// Per-process working directory for the files the writer opens by name.
struct TempCwd {
    fs::path old, dir;
    TempCwd()
    {
        old = fs::current_path();
        dir = fs::temp_directory_path() / keytest::tmp_name("p9ac8a0");
        std::error_code ec;
        fs::remove_all(dir, ec);
        fs::create_directories(dir);
        fs::current_path(dir);
    }
    ~TempCwd()
    {
        fs::current_path(old);
        std::error_code ec;
        fs::remove_all(dir, ec);
    }
};

struct Hooks : def::key::KeyReaderHooks {
    DB_STEP_DATA* m;
    explicit Hooks(DB_STEP_DATA* model) : m(model) {}
    bool has_sub_model() override { return false; }
    void set_model_dimension(int dim) override
    {
        std::int32_t d = dim;
        m->set_dimension(&d);  // 1401c8ba0
    }
    void set_sub_model_dimension(int) override {}
    int parse_keyword(int group, std::FILE* f, int id) override
    {
        switch (group) {
        case 1: return def::key::read_group1(m, f, id);
        case 2: return def::key::g2obj::read_object_keyword(*m, f, id);
        case 3: return def::key::g3::read_keyword(m, f, id);
        case 4: return def::key::g4::read_keyword(*m, f, id);
        case 5: return def::key::g57::read_phase_keyword(m, f, id);
        case 7: return def::key::g57::read_dim_keyword(m, f, id);
        default: return 0;
        }
    }
};

// KFREAD of a copy of `src` into a fresh model (leaked, as the other oracle tests do)
DB_STEP_DATA* read_model(const fs::path& src)
{
    keytest::legacy_printf();
    def::rt::ftn::reset_unit_globals();  // each original run is a fresh process (SI)
    def::rt::ftn::g_conv.english = 0;
    const fs::path copy = fs::current_path() / "seed.key";
    fs::copy_file(src, copy, fs::copy_options::overwrite_existing);
    DB_STEP_DATA* m = new DB_STEP_DATA(3);
    std::FILE* in = std::fopen(copy.string().c_str(), "r");
    std::FILE* sink = std::fopen("NUL", "w");
    Hooks h(m);
    def::key::read_key_stream(in, h, sink);
    std::fclose(in);
    std::fclose(sink);
    return m;
}

LLOBJ_DATA* object(DB_STEP_DATA* m, std::int32_t no)
{
    for (LLOBJ_DATA* o = at<LLOBJ_DATA*>(m, 0x1380); o != nullptr; o = at<LLOBJ_DATA*>(o, 0x3940))
        if (at<std::int32_t>(o, 8) == no) return o;
    return nullptr;
}

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

std::vector<double> nums(const std::string& l)
{
    std::vector<double> v;
    std::istringstream s(l);
    double x;
    while (s >> x) v.push_back(x);
    return v;
}

} // namespace

TEST_CASE(null_object_writes_nothing)
{
    TempCwd tmp;
    def::cmd::step_dat::write_step_dat(nullptr, "X.DB", nullptr);
    CHECK(!fs::exists("STEP.DAT"));
    CHECK(!fs::exists("DEF_INT_NO_DB.DAT"));
}

TEST_CASE(golden_layout)
{
    const fs::path key = testkit::repo_root() / "port" / "defkey" / "tests" / "data" / "kw_g2sv" / "sv_2d.key";
    REQUIRE(fs::exists(key));
    TempCwd tmp;
    DB_STEP_DATA* m = read_model(key);
    LLOBJ_DATA* o = object(m, 1);
    REQUIRE(o != nullptr);
    def::cmd::step_dat::write_step_dat(m, "SV_2D.DB", o);
    const std::string text = slurp("STEP.DAT");
    REQUIRE(!text.empty());
    CHECK(text.find("\r\n") != std::string::npos);  // "wt": text mode
    const std::vector<std::string> L = lines_of(text);
    const std::int32_t nn = at<std::int32_t>(o, 0x880), ne = at<std::int32_t>(o, 0x884);
    REQUIRE(nn > 0 && ne > 0);
    std::size_t i = 0;
    CHECK_EQ(L[i++], std::string("SV_2D.DB"));
    CHECK_EQ(std::atoi(L[i++].c_str()), at<std::int32_t>(m, 0x840));
    CHECK_EQ(std::atoi(L[i++].c_str()), at<std::int32_t>(m, 0x744));
    CHECK_EQ(std::atoi(L[i++].c_str()), 1);
    CHECK_EQ(std::atoi(L[i++].c_str()), nn);
    const double* xy = at<double*>(o, 0x8a0 + 8);
    for (std::int32_t k = 0; k < nn; ++k) {
        char b[64];
        std::snprintf(b, sizeof b, "%15.7E  %15.7E", xy[2 * k], xy[2 * k + 1]);
        if (k == 0) CHECK_EQ(L[i], std::string(b));
        ++i;
    }
    CHECK_EQ(std::atoi(L[i++].c_str()), ne);
    CHECK_EQ(nums(L[i]).size(), 4u);  // "%d  " x 4
    i += static_cast<std::size_t>(ne);
    CHECK_EQ(std::atoi(L[i++].c_str()), ne);
    i += static_cast<std::size_t>(ne);  // material groups
    // the option blocks of this 2D model are the plain element forms (options 0)
    REQUIRE(at<std::int32_t>(m, 0x88c) == 0 && at<std::int32_t>(m, 0x890) == 0 && at<std::int32_t>(m, 0x888) == 0);
    auto skip_rows = [&](std::int32_t n, std::int32_t w) {
        for (std::int32_t k = 0; k < n; ++k) {
            CHECK_EQ(nums(L[i]).size(), static_cast<std::size_t>(w));
            ++i;
        }
    };
    CHECK_EQ(std::atoi(L[i++].c_str()), ne);
    skip_rows(ne, 1);  // effective strain
    CHECK_EQ(std::atoi(L[i++].c_str()), ne);
    skip_rows(ne, at<std::int32_t>(m, 0x1698));
    CHECK_EQ(std::atoi(L[i++].c_str()), ne);
    skip_rows(ne, at<std::int32_t>(m, 0x1690));
    CHECK_EQ(std::atoi(L[i++].c_str()), ne);
    skip_rows(ne, at<std::int32_t>(m, 0x16dc));
    CHECK_EQ(std::atoi(L[i++].c_str()), nn);
    skip_rows(nn, 2);  // T - DTMP, T
    CHECK_EQ(std::atoi(L[i++].c_str()), ne);
    skip_rows(ne, 1);  // density
    CHECK_EQ(std::atoi(L[i++].c_str()), ne);
    skip_rows(ne, at<std::int32_t>(m, 0x16ac));
    // bare matrices: "rows  cols" headers with the model widths
    auto matrix = [&](std::int32_t rows, std::size_t woff, const double* expect) {
        const std::vector<double> h = nums(L[i++]);
        REQUIRE(h.size() == 2u);
        CHECK_EQ(static_cast<std::int32_t>(h[0]), rows);
        const std::int32_t w = at<std::int32_t>(m, woff);
        CHECK_EQ(static_cast<std::int32_t>(h[1]), w);
        if (rows < 1 || w < 1) return;
        for (std::int32_t r = 0; r < rows; ++r) {
            const std::vector<double> v = nums(L[i++]);
            CHECK_EQ(v.size(), static_cast<std::size_t>(w));
            if (expect != nullptr && r == rows - 1) {
                char a[32], b[32];
                std::snprintf(a, sizeof a, "%15.7E", v[static_cast<std::size_t>(w - 1)]);
                std::snprintf(b, sizeof b, "%15.7E", expect[r * w + w - 1]);
                CHECK_EQ(std::string(a), std::string(b));
            }
        }
    };
    matrix(nn, 0x1600, nullptr);  // velocity
    matrix(nn, 0x15fc, nullptr);  // displacement
    matrix(nn, 0x162c, nullptr);
    matrix(ne, 0x16c4, nullptr);  // DVOLF
    matrix(ne, 0x16d8, nullptr);
    matrix(ne, 0x16c8, nullptr);  // TICF
    // VOLFC / VOLFS: no phase functions in this model
    REQUIRE(at<std::int32_t>(o, 0x3590) == 0);
    CHECK_EQ(L[i++], std::to_string(ne) + "  0");
    CHECK_EQ(L[i++], std::to_string(ne) + "  0");
    matrix(ne, 0x16cc, nullptr);                          // GRAIN
    matrix(nn, 0x1680, at<double*>(o, 0x8a0 + 0xb8));    // TIMERC
    matrix(ne, 0x16d0, at<double*>(o, 0x9e0 + 0xa0));    // SIZESH
    matrix(ne, 0x16f4, at<double*>(o, 0x9e0 + 0xd8));    // ERECVL
    matrix(nn, 0x1684, at<double*>(o, 0x8a0 + 0xc0));    // NRECVL
    matrix(ne, 0x16b0, nullptr);                          // MATAXI
    matrix(ne, 0x16e8, nullptr);                          // USRELM (resized to 2 when 0)
    matrix(nn, 0x163c, nullptr);                          // USRNOD
    CHECK(at<std::int32_t>(m, 0x16e8) != 0);
    CHECK(at<std::int32_t>(m, 0x163c) != 0);
    // zero blocks, boundary header and its zero rows
    const std::int32_t nb = at<std::int32_t>(o, 0xbe0) - 1;
    const std::size_t rest = static_cast<std::size_t>(nn) * 4 + 1 + 2 + static_cast<std::size_t>(nb) * 3 +
                             static_cast<std::size_t>(nn) * 2;
    CHECK_EQ(L.size() - i, rest);
    CHECK_EQ(L[i + static_cast<std::size_t>(nn) * 4 + 1], std::string("1  1"));
    CHECK_EQ(L[i + static_cast<std::size_t>(nn) * 4 + 2],
             "1  2  " + std::to_string(nb) + "  1  " + std::to_string(nb));
    // DEF_INT_NO_DB.DAT: the six widths (options 0: no x4)
    std::ostringstream want;
    for (const int off : {0x1698, 0x1690, 0x16dc, 0x1600, 0x15fc, 0x16ac})
        want << at<std::int32_t>(m, off) << "\r\n";
    CHECK_EQ(slurp("DEF_INT_NO_DB.DAT"), want.str());
}

// One cached CNVT3D run: the seed `seed_name` is read, the writer runs for `objs` in order (the
// run's objects; STEP.DAT keeps the last) and the file must equal the cached one. Returns false
// (with a note) when the run or its seed is not available.
bool oracle_case(const std::string& label, const std::string& seed_name, const std::vector<std::int32_t>& objs)
{
    fs::path cache = env("DEFORM_ORACLE_CACHE", "");
    if (cache.empty()) cache = fs::path(env("USERPROFILE", "")) / ".deform-oracle";
    fs::path entry;
    std::string json;
    std::error_code ec;
    if (fs::exists(cache / "entries", ec)) {
        for (auto& d1 : fs::directory_iterator(cache / "entries", ec)) {
            if (!d1.is_directory()) continue;
            for (auto& d2 : fs::directory_iterator(d1.path(), ec)) {
                const fs::path rj = d2.path() / "result.json";
                if (!fs::exists(rj)) continue;
                const std::string j = slurp(rj);
                if (json_string(j, "label") != label) continue;
                if (!entry.empty() && json_string(j, "created") < json_string(json, "created")) continue;
                entry = d2.path();
                json = j;
            }
        }
    }
    if (entry.empty() || !fs::exists(entry / "files" / "STEP.DAT")) {
        std::printf("note: oracle run \"%s\" not cached, skipped\n", label.c_str());
        return false;
    }
    const std::size_t at_seed = json.find("\"name\": \"" + seed_name + "\"");
    const fs::path seed = at_seed == std::string::npos ? fs::path() : fs::path(json_string(json, "source", at_seed));
    if (seed.empty() || !fs::exists(seed)) {
        std::printf("note: seed %s not found, skipped\n", seed_name.c_str());
        return false;
    }
    const std::string want = slurp(entry / "files" / "STEP.DAT");
    const std::string db = lines_of(want)[0];
    TempCwd tmp;
    DB_STEP_DATA* m = read_model(seed);
    for (std::int32_t no : objs) {
        LLOBJ_DATA* o = object(m, no);
        REQUIRE(o != nullptr);
        def::cmd::step_dat::write_step_dat(m, db.c_str(), o);
    }
    const std::string got = slurp("STEP.DAT");
    CHECK_EQ(got.size(), want.size());
    CHECK(got == want);
    if (got != want) {
        std::size_t i = 0;
        while (i < got.size() && i < want.size() && got[i] == want[i]) ++i;
        std::printf("first difference at byte %zu:\n  port: ...%s...\n  orig: ...%s...\n", i,
                    got.substr(i > 60 ? i - 60 : 0, 120).c_str(), want.substr(i > 60 ? i - 60 : 0, 120).c_str());
    } else {
        std::printf("%s: STEP.DAT %zu bytes, identical\n", label.c_str(), got.size());
    }
    std::ostringstream nodb;
    for (const int off : {0x1698, 0x1690, 0x16dc, 0x1600, 0x15fc, 0x16ac})
        nodb << at<std::int32_t>(m, off) << "\r\n";
    CHECK_EQ(slurp("DEF_INT_NO_DB.DAT"), nodb.str());
    return true;
}

// RING_SI object 2 (no phases: VOLFC / VOLFS give "NE  0"); the run wrote object 1 first
TEST_CASE(oracle_ring_si)
{
    oracle_case("CNVT3D two objects 1,2", "RING_SI.KEY", {1, 2});
}

// HT_CARB1_SI object 1 (mixture material: VOLFC / VOLFS through 1402a7b60, 3 phase columns)
TEST_CASE(oracle_ht_carb1_phases)
{
    oracle_case("CNVT3D revolve 90 brick obj1 with phases (HT_CARB1_SI)", "HT_CARB1_SI.KEY", {1});
}

int main() { return testkit::run_all(); }
