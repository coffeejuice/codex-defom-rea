// ctest-labels: kw-g2bc nodal
// Tier-1 tests of the nodal BC family of kw-g2bc.
//
// 1. The per-node/per-element value readers (node_elem_var.hpp: 140416c90 1404199a0 140419b90
//    140419d40 140419ea0): expectations hand-derived from the decompiles
//    (deform-rea-work/out/g2/D_lw.c, B_w6.c, parser2.c).
// 2. The parser cases (nodal.cpp) against the original program: KEY files are read through the
//    KEY loop with the group 2 parser (g2obj::read_object_keyword -> g2bc::read_case), and the
//    arrays are printed in the formats of the state-variable writers 1402f4df0 / 1402f5c10
//    (kw-g2sv, not landed; INTERIM test-side renderer from the specs) and compared with the
//    records the original wrote: the crafted inputs tests/data/kw_g2bc/nodal/*.key (expected text
//    extracted from the oracle runs of oracle_cases.json) and, when the oracle cache is present,
//    every cached kfrw/ corpus round trip.
//
// The model and objects come from the real ctors; RZ and ELMCON are kw-g2geo's readers; the
// component table is p1-03's step_setup_array_sizes (1401d09c0).
#define _CRT_SECURE_NO_WARNINGS
#include "key_test_util.hpp"

#include <cstdint>
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
#include <defkey/node_elem_var.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/bc/node_arrays.hpp>
#include <defmodel/keyword_registry.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/ext.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <testkit/check.hpp>


using namespace def::key;
using keytest::InFile;

// ---- 1404199a0 ------------------------------------------------------------------------------

TEST_CASE(value_records_store_in_range_and_drop_others)
{
    // free format: records split and joined across lines; index 0 and 4 (> n) are read and dropped
    InFile in("t_nev_rec.txt", "2 1.5 2.5\n0 9 9 4 8 8\n1\n-3.25 7e1 tail\nNEXT\n");
    double dst[6] = {-1, -1, -1, -1, -1, -1};
    CHECK_EQ(read_value_records(in.get(), dst, 4, 3, 2), 0);
    CHECK_BITS(dst[0], -3.25);
    CHECK_BITS(dst[1], 70.0);
    CHECK_BITS(dst[2], 1.5);
    CHECK_BITS(dst[3], 2.5);
    CHECK_BITS(dst[4], -1.0);
    CHECK_BITS(dst[5], -1.0);
    // the final fgets swallowed " tail\n"
    CHECK_EQ(in.rest(), std::string("NEXT\n"));
}

TEST_CASE(value_records_count_below_one_reads_nothing)
{
    InFile in("t_nev_rec0.txt", "1 5.0\n");
    double dst[1] = {-1};
    CHECK_EQ(read_value_records(in.get(), dst, 0, 1, 1), 0);
    CHECK_EQ(read_value_records(in.get(), dst, -2, 1, 1), 0);
    CHECK_BITS(dst[0], -1.0);
    CHECK_EQ(in.rest(), std::string("1 5.0\n"));
}

TEST_CASE(value_records_eof)
{
    {
        // EOF inside the values: 1, the first record stays stored
        InFile in("t_nev_eof1.txt", "1 4.0\n2");
        double dst[2] = {0, 0};
        CHECK_EQ(read_value_records(in.get(), dst, 2, 2, 1), 1);
        CHECK_BITS(dst[0], 4.0);
        CHECK_BITS(dst[1], 0.0);
    }
    {
        // EOF on the final fgets (no newline after the last value is not EOF: fgets gets "")
        InFile in("t_nev_eof2.txt", "1 4.0\n");
        double dst[1] = {0};
        CHECK_EQ(read_value_records(in.get(), dst, 1, 1, 1), 0);  // fgets reads "\n"
        CHECK_BITS(dst[0], 4.0);
    }
    {
        InFile in("t_nev_eof3.txt", "1 4.0");
        double dst[1] = {0};
        CHECK_EQ(read_value_records(in.get(), dst, 1, 1, 1), 1);  // fgets at EOF
        CHECK_BITS(dst[0], 4.0);
    }
}

TEST_CASE(value_records_bad_token_reuses_buffer)
{
    // "x" does not convert: fscanf returns 0, the buffer keeps the previous record's value and
    // the token stays in the stream for the final fgets
    InFile in("t_nev_bad.txt", "1 3.0\n2 x\n");
    double dst[2] = {0, 0};
    CHECK_EQ(read_value_records(in.get(), dst, 2, 2, 1), 0);
    CHECK_BITS(dst[0], 3.0);
    CHECK_BITS(dst[1], 3.0);
    CHECK_EQ(in.rest(), std::string(""));  // the final fgets read "x\n"
}

// ---- 140419b90 ------------------------------------------------------------------------------

TEST_CASE(value_records_k_min_and_no_fgets)
{
    InFile in("t_nev_k.txt", "1 1 2 3\n2 4 5 6 rest\n");
    double dst[4] = {0, 0, 0, 0};
    CHECK_EQ(read_value_records_k(in.get(), dst, 2, 2, 2, 3), 0);
    CHECK_BITS(dst[0], 1.0);
    CHECK_BITS(dst[1], 2.0);
    CHECK_BITS(dst[2], 4.0);
    CHECK_BITS(dst[3], 5.0);
    CHECK_EQ(in.rest(), std::string(" rest\n"));
}

// ---- 140416c90 ------------------------------------------------------------------------------

TEST_CASE(node_values_mode0_default_and_records)
{
    InFile in("t_nev_m0.txt", " 2 7.5 junk\n3 1.0 2.0\n2 3.0 4.0\nK\n");
    double dst[6] = {0, 0, 0, 0, 0, 0};
    std::int32_t nc = 2;
    CHECK_EQ(read_node_values(in.get(), dst, 3, &nc, 0), 0);
    CHECK_BITS(dst[0], 7.5);
    CHECK_BITS(dst[1], 7.5);
    CHECK_BITS(dst[2], 3.0);
    CHECK_BITS(dst[3], 4.0);
    CHECK_BITS(dst[4], 1.0);
    CHECK_BITS(dst[5], 2.0);
    CHECK_EQ(in.rest(), std::string("K\n"));
}

TEST_CASE(node_values_header_errors)
{
    std::int32_t nc = 1;
    double dst[1] = {5};
    {
        InFile in("t_nev_h1.txt", "\n");
        CHECK_EQ(read_node_values(in.get(), dst, 1, &nc, 0), 1);
    }
    {
        InFile in("t_nev_h2.txt", "abc\n");
        CHECK_EQ(read_node_values(in.get(), dst, 1, &nc, 0), 1);
    }
    {
        InFile in("t_nev_h3.txt", "");
        CHECK_EQ(read_node_values(in.get(), dst, 1, &nc, 0), 1);
    }
    {
        InFile in("t_nev_h4.txt", "0 1\n");
        CHECK_EQ(read_node_values(in.get(), dst, 1, &nc, 5), 1);  // unknown mode after fgets
    }
    std::int32_t big = 11;
    {
        InFile in("t_nev_h5.txt", "0\n");
        CHECK_EQ(read_node_values(in.get(), dst, 1, &big, 2), 1);
        CHECK_EQ(in.rest(), std::string("0\n"));  // before the fgets
    }
    CHECK_BITS(dst[0], 5.0);
}

TEST_CASE(node_values_mode2_sets_ncomp_and_vector_default)
{
    InFile in("t_nev_m2.txt", "1 2 1.0 2.0\n2 8 9\n");
    double dst[4] = {0, 0, 0, 0};
    std::int32_t nc = 3;
    CHECK_EQ(read_node_values(in.get(), dst, 2, &nc, 2), 0);
    CHECK_EQ(nc, 2);
    CHECK_BITS(dst[0], 1.0);
    CHECK_BITS(dst[1], 2.0);
    CHECK_BITS(dst[2], 8.0);
    CHECK_BITS(dst[3], 9.0);
}

// ---- 140419d40 / 140419ea0 ------------------------------------------------------------------

TEST_CASE(int_values_default_records_and_hex)
{
    InFile in("t_nev_i.txt", "3 0x10\n3 -1 010\n9 1 1 1 1 2\nX\n");
    std::int32_t dst[6] = {7, 7, 7, 7, 7, 7};
    CHECK_EQ(read_int_values(in.get(), dst, 3, 2), 0);
    CHECK_EQ(dst[0], 1);
    CHECK_EQ(dst[1], 2);
    CHECK_EQ(dst[2], 16);
    CHECK_EQ(dst[3], 16);
    CHECK_EQ(dst[4], -1);
    CHECK_EQ(dst[5], 8);
    // records 3:(-1, 010 = 8), 9:(1, 1) dropped, 1:(1, 2)
    CHECK_EQ(in.rest(), std::string("X\n"));
}

TEST_CASE(int_values_no_default_keeps_values)
{
    InFile in("t_nev_i2.txt", "1\n2 5\n");
    std::int32_t dst[2] = {3, 3};
    CHECK_EQ(read_int_values(in.get(), dst, 2, 1), 0);
    CHECK_EQ(dst[0], 3);
    CHECK_EQ(dst[1], 5);
    InFile in2("t_nev_i3.txt", "  \n");
    CHECK_EQ(read_int_values(in2.get(), dst, 2, 1), 1);
    InFile in3("t_nev_i4.txt", "0 4\nK\n");
    CHECK_EQ(read_int_values(in3.get(), dst, 2, 1), 0);  // count 0: default only, no fgets
    CHECK_EQ(dst[0], 4);
    CHECK_EQ(dst[1], 4);
    CHECK_EQ(in3.rest(), std::string("K\n"));
}

// ---- parser cases vs the original program ---------------------------------------------------

namespace {

namespace fs = std::filesystem;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;

template <class T, class C>
T& at(C& base, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<unsigned char*>(&base) + off);
}

const char* const kMine[] = {"BCCDEF", "BCCDFN", "BCCANG", "BCCTMP", "BCCTFN",
                             "HDNOBJ", "BCCCRB", "BCCRHT", "CRBFLX", "RHTFLX"};

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

std::string env(const char* name, const char* def)
{
    const char* v = std::getenv(name);
    return v != nullptr && *v != '\0' ? v : def;
}

// ---- INTERIM stand-ins for the state variables (p1-09) ----
// set pointer -> true for a node set (LLOBJ_DATA+0x8a0), false for an element set (+0x9e0)
std::map<std::uintptr_t, bool> g_sets;
struct Slot {
    std::uint32_t off;
    int id;
};
const std::vector<Slot> kNodeSlots = {{0x40, 0x08}, {0x70, 0x0e}, {0x80, 0x10}};
const std::vector<Slot> kElemSlots = {{0x50, 0x2c}, {0x58, 0x2d}, {0xb0, 0x38}};

void free_slots(unsigned char* set, bool node)
{
    for (const Slot& s : node ? kNodeSlots : kElemSlots) {
        double*& p = *reinterpret_cast<double**>(set + s.off);
        std::free(p);
        p = nullptr;
    }
}

std::uint64_t statevar_hook(std::uint64_t addr, const std::uint64_t* a, int)
{
    auto it = g_sets.find(static_cast<std::uintptr_t>(a[0]));
    if (it == g_sets.end()) return 0;
    auto* set = reinterpret_cast<unsigned char*>(a[0]);
    if (addr == 0x140329dc0) {
        free_slots(set, it->second);
    } else if (addr == 0x140329e60) {
        const auto n = static_cast<std::int32_t>(a[1]);
        const auto* dims = reinterpret_cast<const std::int32_t*>(a[2]);
        free_slots(set, it->second);
        for (const Slot& s : it->second ? kNodeSlots : kElemSlots) {
            const std::int32_t cnt = n * dims[s.id];
            *reinterpret_cast<double**>(set + s.off) =
                cnt > 0 ? static_cast<double*>(std::calloc(cnt, 8)) : nullptr;
        }
    }
    return 0;
}

// The component-table entries of 1401d09c0 (p1-03) these keywords read.
void set_dims(DB_STEP_DATA& m)
{
    const std::int32_t d = at<std::int32_t>(m, 0x1c);
    std::int32_t* t = &at<std::int32_t>(m, 0x15f8);
    for (int i = 0; i < 0x4f; ++i) t[i] = 1;
    t[0] = d;
    t[0x12] = d;  // 0x1640
    t[0x13] = d;  // 0x1644
    t[0x38] = 2;  // 0x16d8
    if (d == 2 && at<std::int32_t>(m, 0x850) == 3) t[0x12] = t[0x13] = 3;
}

// Skip `n` data lines (the RZ / ELMCON records).
void skip_lines(std::FILE* f, std::int32_t n)
{
    char buf[0x400];
    for (std::int32_t i = 0; i < n; ++i)
        if (std::fgets(buf, sizeof buf, f) == nullptr) return;
}

struct Hooks : KeyReaderHooks {
    DB_STEP_DATA& m;
    int rz, elmcon, geotyp;
    std::set<int> mine;
    explicit Hooks(DB_STEP_DATA& model)
        : m(model), rz(def::model::keyword_id("RZ")), elmcon(def::model::keyword_id("ELMCON")),
          geotyp(def::model::keyword_id("GEOTYP"))
    {
        for (const char* k : kMine) mine.insert(def::model::keyword_id(k));
    }
    bool has_sub_model() override { return false; }
    void set_model_dimension(int dim) override
    {
        at<std::int32_t>(m, 0x1c) = dim;
        def::model::step_setup_array_sizes(m, dim);  // 1401d09c0 (p1-03)
    }
    void set_sub_model_dimension(int) override {}
    LLOBJ_DATA* object(std::FILE* f)
    {
        std::int32_t no = 0;
        if (std::fscanf(f, "%i", &no) == -1) return nullptr;
        LLOBJ_DATA* o = def::model::llobj_ctor::get_or_create(&m, no);
        if (o != nullptr) {
            g_sets[reinterpret_cast<std::uintptr_t>(&at<unsigned char>(*o, 0x8a0))] = true;
            g_sets[reinterpret_cast<std::uintptr_t>(&at<unsigned char>(*o, 0x9e0))] = false;
        }
        return o;
    }
    int parse_keyword(int group, std::FILE* f, int id) override
    {
        if (group == 1 && id == geotyp) {
            read_i(f, &at<std::int32_t>(m, 0x850));
            def::model::step_setup_array_sizes(m, at<std::int32_t>(m, 0x1c));  // as GEOTYP's 1401d0fc0 refresh
            return 0;
        }
        if (group != 2) return 0;
        if (mine.count(id) != 0) return def::key::g2obj::read_object_keyword(m, f, id);
        // RZ / ELMCON: kw-g2geo's readers (through the group 2 shell)
        if (id == rz || id == elmcon) return def::key::g2obj::read_object_keyword(m, f, id);
        return 0;
    }
};

// INTERIM renderers of the state-variable writers (kw-g2sv): 1402f4df0 (ints) and 1402f5c10
// format 0 (doubles), as the specs give them.
template <class T>
std::string render(const char* kw, std::int32_t no, const T* v, std::int32_t n, std::int32_t mm,
                   const char* hdr, const char* val)
{
    if (n == 0 || mm == 0 || v == nullptr) return {};
    char b[256];
    bool uniform = true;
    for (std::int32_t i = 0; i < n * mm; ++i) uniform = uniform && v[i] == v[0];
    if (uniform) {
        def::rt::crt::sprintf(b, hdr, kw, no, 0, v[0]);
        return b;
    }
    auto nz = [&](std::int32_t i) {
        for (std::int32_t j = 0; j < mm; ++j)
            if (v[i * mm + j] != T(0)) return true;
        return false;
    };
    std::int32_t k = 0;
    for (std::int32_t i = 0; i < n; ++i) k += nz(i) ? 1 : 0;
    def::rt::crt::sprintf(b, hdr, kw, no, k, T(0));
    std::string s = b;
    for (std::int32_t i = 0; i < n; ++i) {
        if (!nz(i)) continue;
        def::rt::crt::sprintf(b, " %7i", i + 1);
        s += b;
        for (std::int32_t j = 0; j < mm; ++j) {
            def::rt::crt::sprintf(b, val, v[i * mm + j]);
            s += b;
        }
        s += "\n";
    }
    return s;
}

// The records of this family for one object, in the object writer's order.
std::vector<std::pair<std::string, std::string>> render_object(DB_STEP_DATA& m, LLOBJ_DATA& o)
{
    std::vector<std::pair<std::string, std::string>> r;
    const std::int32_t no = at<std::int32_t>(o, 8);
    const std::int32_t n = at<std::int32_t>(o, 0x880), ne = at<std::int32_t>(o, 0x884);
    auto ints = [&](const char* kw, std::size_t off, std::size_t moff) {
        r.emplace_back(kw, render(kw, no, at<std::int32_t*>(o, off), n, at<std::int32_t>(m, moff),
                                  "%-6s %7i %7i %7i\n", " %7i"));
    };
    auto dbls = [&](const char* kw, std::size_t off, std::int32_t cnt, std::size_t moff) {
        r.emplace_back(kw, render(kw, no, at<double*>(o, off), cnt, at<std::int32_t>(m, moff),
                                  "%-6s %7i %7i %20.10E\n", " %20.10E"));
    };
    ints("BCCDEF", 0x990, 0x1640);
    ints("BCCDFN", 0x998, 0x1644);
    ints("BCCTMP", 0x9a0, 0x1648);
    ints("BCCTFN", 0x9a8, 0x164c);
    dbls("HDNOBJ", 0xa90, ne, 0x16d8);
    ints("BCCCRB", 0x9b0, 0x1650);
    if (at<std::int32_t>(m, 0x1c) == 3) dbls("CRBFLX", 0x910, n, 0x1630);
    ints("BCCRHT", 0x9b8, 0x1654);
    dbls("RHTFLX", 0x920, n, 0x1638);
    return r;
}

DB_STEP_DATA& read_model(const fs::path& src)
{
    // the real state-variable sets (p1-10 Cls_140329c70, p1-09 StateVar*) allocate the arrays
    DB_STEP_DATA& m = *new DB_STEP_DATA(3);  // the real model ctor (p1-03)
    std::FILE* in = std::fopen(src.string().c_str(), "r");
    std::FILE* sink = std::fopen("NUL", "w");
    Hooks hooks(m);
    read_key_stream(in, hooks, sink);
    std::fclose(in);
    std::fclose(sink);
    return m;  // objects and model leaked on purpose (their dtors are not ported)
}

fs::path data_dir() { return testkit::repo_root() / "port" / "defkey" / "tests" / "data" / "kw_g2bc" / "nodal"; }

// keyword -> object -> record texts, from a KEY text
using Recs = std::map<std::string, std::map<int, std::vector<std::string>>>;
Recs records_of(const std::string& text)
{
    Recs r;
    std::set<std::string> mine(std::begin(kMine), std::end(kMine));
    std::istringstream in(text);
    std::string l;
    std::vector<std::string> ls;
    while (std::getline(in, l)) ls.push_back(l);
    for (std::size_t i = 0; i < ls.size(); ++i) {
        if (ls[i].empty() || ls[i][0] == ' ') continue;
        char name[64] = {0};
        int obj = 0;
        if (std::sscanf(ls[i].c_str(), "%63s %i", name, &obj) < 2 || mine.count(name) == 0) continue;
        std::string t = ls[i] + "\n";
        std::size_t j = i + 1;
        while (j < ls.size() && !ls[j].empty() && ls[j][0] == ' ') t += ls[j++] + "\n";
        r[name][obj].push_back(t);
        i = j - 1;
    }
    return r;
}

void crafted_case(const char* name)
{
    keytest::legacy_printf();
    fs::path src = fs::temp_directory_path() / keytest::tmp_name(std::string("kw_g2bc_nodal_") + name + ".key");
    fs::copy_file(data_dir() / (std::string(name) + ".key"), src, fs::copy_options::overwrite_existing);
    DB_STEP_DATA& m = read_model(src);
    std::string mine;
    for (LLOBJ_DATA* o = at<LLOBJ_DATA*>(m, 0x1380); o != nullptr; o = at<LLOBJ_DATA*>(*o, 0x3940))
        for (auto& kt : render_object(m, *o)) mine += kt.second;
    const std::string expected = slurp(data_dir() / (std::string(name) + ".expected"));
    CHECK_EQ(mine, expected);
    fs::remove(src);
}

} // namespace

TEST_CASE(crafted_2d_against_oracle) { crafted_case("nodal_2d"); }
TEST_CASE(crafted_3d_against_oracle) { crafted_case("nodal_3d"); }

TEST_CASE(oracle_kfrw_corpus_records)
{
    keytest::legacy_printf();
    fs::path cache = env("DEFORM_ORACLE_CACHE", "");
    if (cache.empty()) cache = fs::path(env("USERPROFILE", "")) / ".deform-oracle";
    if (!fs::exists(cache / "entries")) {
        std::printf("no oracle cache at %s: skipped\n", cache.string().c_str());
        return;
    }
    const std::uintmax_t max_bytes = std::strtoull(env("KW_G2BC_MAX_BYTES", "4000000").c_str(), nullptr, 10);
    const bool verbose = env("KW_G2BC_VERBOSE", "0") == "1";
    int files = 0, skipped = 0, geo_skipped = 0, compared = 0, bad = 0;
    std::map<std::string, std::pair<int, int>> by_kw;  // ok, bad
    for (auto& d1 : fs::directory_iterator(cache / "entries")) {
        if (!d1.is_directory()) continue;
        for (auto& d2 : fs::directory_iterator(d1.path())) {
            fs::path rj = d2.path() / "result.json";
            if (!fs::exists(rj)) continue;
            std::string j = slurp(rj);
            if (j.find("\"label\": \"kfrw/") == std::string::npos) continue;
            std::size_t p = j.find("\"source\": \"");
            fs::path out = d2.path() / "files" / "KFWRIT_OUT.KEY";
            std::string srcs;
            if (p != std::string::npos) {
                for (p += 11; p < j.size() && j[p] != '"'; ++p) {
                    if (j[p] == '\\') ++p;
                    srcs += j[p];
                }
            }
            fs::path src = srcs;
            if (srcs.empty() || !fs::exists(out) || !fs::exists(src) || fs::file_size(src) > max_bytes) {
                ++skipped;
                continue;
            }
            // RZ / ELMCON failed in the original (e.g. 2D object files without a DEFORM-2D comment,
            // read as 3D): the geometry path (kw-g2geo) is not modelled by the fixture
            const std::string so = slurp(d2.path() / "stdout.txt");
            if (so.find("Keyword ELMCON Reading Error") != std::string::npos ||
                so.find("Keyword RZ Reading Error") != std::string::npos) {
                ++geo_skipped;
                continue;
            }
            fs::path copy = fs::temp_directory_path() / keytest::tmp_name("kw_g2bc_nodal_oracle.key");
            fs::copy_file(src, copy, fs::copy_options::overwrite_existing);
            Recs given = records_of(slurp(copy));
            Recs orig = records_of(slurp(out));
            DB_STEP_DATA& m = read_model(copy);
            ++files;
            for (LLOBJ_DATA* o = at<LLOBJ_DATA*>(m, 0x1380); o != nullptr; o = at<LLOBJ_DATA*>(*o, 0x3940)) {
                const int no = at<std::int32_t>(*o, 8);
                for (auto& kt : render_object(m, *o)) {
                    const std::string& kw = kt.first;
                    if (given[kw].count(no) == 0) continue;  // compare only what the input gives
                    std::vector<std::string> a = orig[kw][no], b;
                    if (!kt.second.empty()) b.push_back(kt.second);
                    ++compared;
                    if (a == b) {
                        ++by_kw[kw].first;
                        continue;
                    }
                    ++bad;
                    ++by_kw[kw].second;
                    if (verbose) {
                        std::printf("MISMATCH %s obj %d %s\n  orig:\n", src.string().c_str(), no, kw.c_str());
                        for (auto& s : a) std::printf("%.300s", s.c_str());
                        std::printf("  port:\n%.300s", kt.second.c_str());
                    }
                }
            }
            fs::remove(copy);
        }
    }
    std::printf("kfrw cases: %d checked, %d skipped (size/missing), %d skipped (RZ/ELMCON error in the "
                "original); records compared %d, mismatched %d\n",
                files, skipped, geo_skipped, compared, bad);
    for (auto& [kw, c] : by_kw) std::printf("  %-6s ok %5d  bad %5d\n", kw.c_str(), c.first, c.second);
    CHECK_EQ(bad, 0);
}

int main() { return testkit::run_all(); }
