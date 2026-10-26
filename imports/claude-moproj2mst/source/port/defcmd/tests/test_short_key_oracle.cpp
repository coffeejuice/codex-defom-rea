// ctest-labels: p5-shortkey oracle
// The short KEY writer 1401c5d20 / 1402a1df0 (KFWRIT core 1401c5810 with flags bit 31) checked
// against the original's output.
//
// No session of DEF_PRE_64 reaches the short writer: the three callers of 1401c5810 (menu 8 save
// 14000ac60, KFWRIT 14037c636, CNVT3D 140147a17) all pass flags 0 (docs/re/status/p5-shortkey.tsv).
// So there is no oracle run of it to compare with. What the original does write is the long KEY
// file of the same model, and every line of the short file is written there by the same code
// (GEOTYP by the group 1 writer, the object banner, OBJTYP 1402a25a0, OBJNAM 1402a2530, DIEGEO,
// RZ 1402a2db0, ELMCON 1402a30c0). The test builds the expected short file from the original's
// KFWRIT_OUT.KEY of the cached kfrw/<path> cases (harness/corpus; %USERPROFILE%/.deform-oracle or
// $DEFORM_ORACLE_CACHE) and compares it byte for byte with the port's short file of the same input.
//
// Parts the long file cannot give are taken from the port's own output and counted as unchecked:
// objects of type 5 / 7 (the long writer stops after OBJTYP or writes the movement block), and the
// DIEGEO records of an object where the two writers' geometry conditions differ (2D: curve 0 type
// 0 or object dimension 3; 3D: the object has curves, which the long writer writes with 1402aaa70
// and the short one sends to 1402a26c0).
//
// SHORTKEY_MAX_BYTES (default 2000000), SHORTKEY_MAX_FILES, SHORTKEY_ONLY (substring of the
// source path), SHORTKEY_VERBOSE=1. Without the cache the test passes with a note.
#define _CRT_SECURE_NO_WARNINGS
#include "../../defkey/tests/key_test_util.hpp"
#include "../../defkey/tests/crash_trace.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <process.h>

#include <defcmd/dispatch.hpp>
#include <defkey/key_reader.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defkey/line_io.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/keyword_registry.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/step/db_step.hpp>
#include <testkit/check.hpp>

namespace fs = std::filesystem;
namespace gk = def::model::geo_kw;
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

struct Record {
    std::string kw, text;
};

// Group 2 records per object number, in file order (same splitter as test_kw_g2geo_oracle.cpp):
// a line whose first token is a group 2 name starts one; lines starting with a blank continue it,
// OBJNAM takes the next line whatever it is, TPLOGY keeps its "* ... Data" lines.
std::map<int, std::vector<Record>> records_of(const std::string& text)
{
    std::map<int, std::vector<Record>> r;
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
        // TPLOGY (1402a9f90) has "* Vertex Data: " style section lines inside its record
        auto continues = [&](const std::string& x) {
            if (x.empty()) return false;
            if (x[0] == ' ') return true;
            return rec.kw == "TPLOGY" && x.rfind("* ", 0) == 0 && x.rfind("*  Data for Object", 0) != 0;
        };
        while (j < ls.size() && continues(ls[j])) rec.text += ls[j++] + "\n";
        r[obj].push_back(rec);
        i = j - 1;
    }
    return r;
}

std::string join(const std::vector<Record>& recs, const char* kw)
{
    std::string s;
    for (const Record& r : recs)
        if (r.kw == kw) s += r.text;
    return s;
}

// The geometry block, in file order: 2D the inline DIEGEO curves only; 3D what 1402a26c0 ->
// 1402a27a0 writes (curve group 1402ab160: DIEGEO, TPLOGY, CSPMTX; surface: DIEGEO, POLYTP,
// DIEPNT, DIEEDG).
std::string geometry_block(const std::vector<Record>& recs, bool two_d)
{
    std::string s;
    for (const Record& r : recs) {
        if (r.kw == "DIEGEO" || (!two_d && (r.kw == "TPLOGY" || r.kw == "CSPMTX" || r.kw == "POLYTP" ||
                                            r.kw == "DIEPNT" || r.kw == "DIEEDG")))
            s += r.text;
    }
    return s;
}

// The model side the short writer needs: group 2 and GEOTYP (+0x850) / UNIT (+0x84c) of group 1.
struct Hooks : def::key::KeyReaderHooks {
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
        if (group == 1 && id == geotyp_id) def::key::read_i(f, &at<std::int32_t>(m, 0x850));
        if (group == 1 && id == unit_id) def::key::read_i(f, &at<std::int32_t>(m, 0x84c));
        return 0;
    }
};

struct Totals {
    int files = 0, skipped = 0, identical = 0, differ = 0, objects = 0, unchecked_objects = 0,
        unchecked_geometry = 0;
};

// What the port's model says about an object before the short write.
struct ObjInfo {
    int no = 0;
    bool whole_unchecked = false, geometry_unchecked = false;
};

void check_case(const fs::path& src, const fs::path& oracle_out, Totals& tot, bool verbose)
{
    const std::string pid = std::to_string(_getpid());
    fs::path copy = fs::temp_directory_path() / ("p5sk_short_in_" + pid + ".key");
    fs::copy_file(src, copy, fs::copy_options::overwrite_existing);
    DB_STEP_DATA& m = *new DB_STEP_DATA(3);  // leaked with its objects (no dtor in this test)
    std::FILE* in = std::fopen(copy.string().c_str(), "r");
    std::FILE* sink = std::fopen("NUL", "w");
    Hooks hooks(m);
    def::key::read_key_stream(in, hooks, sink);
    std::fclose(in);
    std::fclose(sink);

    const bool two_d = at<std::int32_t>(m, 0x1c) == 2;
    std::vector<ObjInfo> objs;
    for (std::int32_t i = 0; i < m.u1378; ++i) {
        LLOBJ_DATA* o = m.objects.at(i + 1);
        if (o == nullptr) continue;
        ObjInfo inf;
        inf.no = at<std::int32_t>(*o, 8);
        const std::int32_t type = at<std::int32_t>(*o, 0xc);
        inf.whole_unchecked = type == 5 || type == 7;
        if (two_d)
            inf.geometry_unchecked = gk::geometry_has_edges(*o) &&
                                     (!gk::curve_type0_nonzero(*o) || gk::object_dim(*o) == 3);
        else
            inf.geometry_unchecked = gk::has_curve(*o, 0);
        objs.push_back(inf);
    }

    // the port: KFWRIT core with flags bit 31 (1401c5810 -> 1401c5d20 -> 1402a1df0)
    fs::path outp = fs::temp_directory_path() / ("p5sk_short_out_" + pid + ".key");
    const std::uint32_t flags = 0x80000000u;
    const std::int32_t st = def::cmd::write_key_file(&m, outp.string().c_str(), &flags);
    const std::string mine = slurp(outp);
    const std::string orig = slurp(oracle_out);
    ++tot.files;

    // the expected short file from the original's long file
    auto orig_recs = records_of(orig);
    auto mine_recs = records_of(mine);
    std::string exp;
    if (two_d) {
        for (const std::string& l : lines_of(orig))
            if (l.rfind("GEOTYP ", 0) == 0) {
                exp += l + "\n";
                break;
            }
    }
    for (const ObjInfo& inf : objs) {
        ++tot.objects;
        char banner[128];
        std::snprintf(banner, sizeof banner, "*\n*  Data for Object #     %i\n*\n", inf.no);
        exp += banner;
        const auto& src_recs = orig_recs[inf.no];
        const auto& own = mine_recs[inf.no];
        exp += join(src_recs, "OBJTYP");
        if (inf.whole_unchecked) {
            ++tot.unchecked_objects;
            exp += join(own, "OBJNAM") + geometry_block(own, two_d) + join(own, "RZ") + join(own, "ELMCON");
            continue;
        }
        exp += join(src_recs, "OBJNAM");
        if (inf.geometry_unchecked) {
            ++tot.unchecked_geometry;
            exp += geometry_block(own, two_d);
        } else {
            exp += geometry_block(src_recs, two_d);
        }
        exp += join(src_recs, "RZ");
        exp += join(src_recs, "ELMCON");
    }

    // both files are written in text mode ("wt"): the original's lines end in CR LF
    if (orig.find("\r\n") != std::string::npos) {
        std::string crlf;
        for (char c : exp) {
            if (c == '\n') crlf += '\r';
            crlf += c;
        }
        exp.swap(crlf);
    }
    const bool same = st == 0 && exp == mine;
    if (same) {
        ++tot.identical;
    } else {
        ++tot.differ;
        std::printf("DIFFER %s (status %d)\n", src.string().c_str(), st);
        if (verbose) {
            std::vector<std::string> a = lines_of(exp), b = lines_of(mine);
            for (std::size_t i = 0; i < a.size() || i < b.size(); ++i) {
                const std::string x = i < a.size() ? a[i] : "<eof>", y = i < b.size() ? b[i] : "<eof>";
                if (x != y) {
                    std::printf("  line %zu\n  orig: %s\n  port: %s\n", i + 1, x.c_str(), y.c_str());
                    break;
                }
            }
        }
    }
    fs::remove(copy);
    fs::remove(outp);
}

} // namespace

TEST_CASE(null_object_and_bad_type)
{
    keytest::legacy_printf();
    DB_STEP_DATA m(3);
    keytest::OutFile out("p5sk_null.key");
    CHECK_EQ(def::cmd::dispatch_hooks().write_object_short(&m, out.get(), nullptr, 0), 0);
    CHECK_EQ(out.text(), std::string());
}

TEST_CASE(short_writer_against_the_original_long_file)
{
    crash_trace::install();
    keytest::legacy_printf();
    fs::path cache = env("DEFORM_ORACLE_CACHE", "");
    if (cache.empty()) cache = fs::path(env("USERPROFILE", "")) / ".deform-oracle";
    if (!fs::exists(cache / "entries")) {
        std::printf("no oracle cache at %s: skipped\n", cache.string().c_str());
        return;
    }
    const std::uintmax_t max_bytes = std::strtoull(env("SHORTKEY_MAX_BYTES", "2000000").c_str(), nullptr, 10);
    const int max_files = std::atoi(env("SHORTKEY_MAX_FILES", "100000").c_str());
    const bool verbose = env("SHORTKEY_VERBOSE", "0") == "1";
    const std::string only = env("SHORTKEY_ONLY", "");
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
            if (!only.empty() && src.string().find(only) == std::string::npos) continue;
            if (fs::file_size(src) > max_bytes || tot.files >= max_files) {
                ++tot.skipped;
                continue;
            }
            check_case(src, out, tot, verbose);
        }
    }
    std::printf("kfrw cases: %d checked, %d skipped; identical %d, differ %d; objects %d "
                "(type 5/7 unchecked %d, geometry unchecked %d)\n",
                tot.files, tot.skipped, tot.identical, tot.differ, tot.objects, tot.unchecked_objects,
                tot.unchecked_geometry);
    CHECK_EQ(tot.differ, 0);
}

int main() { return testkit::run_all(); }
