// ctest-labels: p4-05 gendb3d oracle
// Oracle check of packet p4-05 (the 3D object pass of GENDB, FUN_14017b370): DEF_PRE_64 ran
// "KFREAD <key> / GENDB / NEW.DB" on copies of 3D example KEY files (oracle cache, labels
// p405/gendb/<key>, %USERPROFILE%/.deform-oracle or $DEFORM_ORACLE_CACHE). Here the same KEY (a
// copy) is read into a DB_STEP_DATA with the group 1, 2 and 4 parsers, the oracle step gives the
// address tables and the counts that the earlier GENDB stages leave behind (packets p4-01/p4-02),
// write_objects_3d runs into zeroed buffers of the oracle's sizes, and the object regions are
// compared word for word with the oracle's arrays.
//
// The calls into functions other packets own are answered by a hook that follows their
// decompiles (each case quotes the body). Without the cache the test passes with a note.
// P405_ORACLE_VERBOSE=1 prints the first mismatches per region; P405_ORACLE_ONLY=<substring>.
#define _CRT_SECURE_NO_WARNINGS
#include <defdbio/dbcore/util.hpp>
#include <defdbio/gendb3d/obj3d.hpp>

#include <algorithm>
#include <testkit/check.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <map>
#include <process.h>
#include <set>
#include <string>
#include <vector>

#include <deformdb/database.hpp>
#include <deformdb/model.hpp>
#include <defkey/group1.hpp>
#include <defkey/key_reader.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defkey/kw_g4.hpp>
#include <defmodel/keyword_registry.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/stdlib/refcount.hpp>

#include "../../defdbio/src/gendb3d/access.hpp"
#include "../../defdbio/src/gendb3d/ext.hpp"

namespace fs = std::filesystem;
using namespace def::dbio::g3d;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;
using def::model::OBJ_GEO3D;

namespace {

bool verbose() { const char* v = std::getenv("P405_ORACLE_VERBOSE"); return v && *v == '1'; }

fs::path cache_root()
{
    if (const char* c = std::getenv("DEFORM_ORACLE_CACHE")) return c;
    const char* h = std::getenv("USERPROFILE");
    return fs::path(h ? h : ".") / ".deform-oracle";
}

// result.json of an entry whose label is p405/gendb/<key>
struct Entry { fs::path dir, key_src; std::string label; };
std::vector<Entry> find_entries()
{
    std::vector<Entry> r;
    fs::path root = cache_root() / "entries";
    if (!fs::exists(root)) return r;
    for (auto& d1 : fs::directory_iterator(root)) {
        if (!d1.is_directory()) continue;
        for (auto& d2 : fs::directory_iterator(d1.path())) {
            fs::path rj = d2.path() / "result.json";
            if (!fs::exists(rj)) continue;
            std::ifstream f(rj);
            std::string s((std::istreambuf_iterator<char>(f)), {});
            auto p = s.find("\"p405/gendb/");
            if (p == std::string::npos) continue;
            auto q = s.find('"', p + 1);
            Entry e;
            e.label = s.substr(p + 1, q - p - 1);
            e.dir = d2.path();
            // the first input's "source" (JSON string, backslashes escaped)
            auto sp = s.find("\"source\": \"");
            if (sp != std::string::npos) {
                std::string src;
                for (std::size_t i = sp + 11; i < s.size() && s[i] != '"'; ++i) {
                    if (s[i] == '\\' && i + 1 < s.size()) ++i;
                    src += s[i];
                }
                e.key_src = src;
            }
            r.push_back(e);
        }
    }
    return r;
}

// ---------------------------------------------------------------------------------------------
// hook for the calls into other packets
// ---------------------------------------------------------------------------------------------
DB_STEP_DATA* g_m = nullptr;
std::set<std::uint64_t> g_unhandled;
std::uint64_t g_static_item[8] = {};  // default item of 1402fd3e0 / 1402fd740 (zeroed)

template <class T> T& F(std::uint64_t p, std::ptrdiff_t off) { return fld<T>(ext::as_p<void>(p), off); }

// walk a COW list body: **(body + 8), then n links
void* cow_walk(void* body, std::uint64_t n)
{
    void* x = *reinterpret_cast<void**>(*reinterpret_cast<void**>(static_cast<char*>(body) + 8));
    for (std::uint64_t i = 0; i < n; ++i) x = *reinterpret_cast<void**>(x);
    return x;
}
std::int32_t cow_count(void* handle_addr) { return fld<std::int32_t>(*reinterpret_cast<void**>(handle_addr), 0x10); }

std::int32_t dynary_len(std::uint64_t p)  // 140130290
{
    const std::int32_t* f = ext::as_p<std::int32_t>(p);
    if (f[1] == 0) {
        const int t = f[0];
        return (t == 0xa7 || t == 0xa8 || t == 0xaa || t == 0xab || t == 0xad || t == 0xb3 || t == 0xb5) ? f[0x10] : 0;
    }
    if (f[1] == 1) return f[0] == 0x7c ? f[0x10] : f[7] << 1;  // 140130200
    auto at1 = [&](int k) { return f[k] < 1 ? 1 : f[k]; };
    if (f[1] == 2) return f[7] + f[8] + at1(7) * at1(8) * at1(0x10);  // 140130160 (sum + product)
    if (f[1] == 3) return f[7] + f[8] + f[9] + at1(7) * at1(8) * at1(9);  // 14012fff0
    return 0;
}

std::int32_t item50_per(std::uint64_t it) { return F<std::int32_t>(it, 4) == 1 ? 5 : 0; }            // 1402f8760
std::int32_t item58_per(std::uint64_t it) { std::int32_t t = F<std::int32_t>(it, 4); return (0 < t && t < 3) ? 1 : 0; }  // 1402f8ce0
std::int32_t item_cnt(std::uint64_t it) { return cow_count(ext::as_p<char>(it) + 0x10); }          // 14019a000 / 14019a060

bool hook(std::uint64_t addr, const std::uint64_t* a, int, std::uint64_t* r)
{
    switch (addr) {
    // ---- one-line accessors with default bodies in the ext headers: use them
    case 0x140199c10: case 0x1401f3cb0: case 0x1401f3c30: case 0x1402575d0: case 0x14017aaf0:
    case 0x14019a280: case 0x140006410: case 0x140005510: case 0x140199fa0: case 0x140198620:
    case 0x1402b7470: case 0x1402b74d0: case 0x1402b7480: case 0x1402b74e0: case 0x1402b74b0:
    case 0x1402b7510: case 0x14001cd90: case 0x1401f3c50: case 0x14019a5c0: case 0x14019a600:
    case 0x140194d40:
        return false;
    case 0x14029c410: {  // largest material number: for i < count, nth(i+1)->number
        auto* l = ext::as_p<def::model::MaterialList>(a[0]);
        std::int32_t mx = 0;
        for (int i = 0; i < l->count; ++i) {
            auto* p = reinterpret_cast<std::int32_t*>(def::model::material_list_nth(l, i + 1));
            if (p && mx < *p) mx = *p;
        }
        *r = static_cast<std::uint32_t>(mx);
        return true;
    }
    case 0x1401f3d90: {  // sim+0x104 == 0 && +0x108 == 1 && +0x10c == 0 && +0x110 == 0 && +0x118 == 0
        std::uint64_t s = a[0];
        *r = (F<std::int32_t>(s, 0x104) == 0 && F<std::int32_t>(s, 0x108) == 1 && F<std::int32_t>(s, 0x10c) == 0 &&
              F<std::int32_t>(s, 0x110) == 0 && F<std::int32_t>(s, 0x118) == 0) ? 1 : 0;
        return true;
    }
    case 0x140136a10: {  // max(count +0x10, ids +0x54 along +0x60)
        std::int32_t mx = F<std::int32_t>(a[0], 0x10);
        for (std::uint64_t n = F<std::uint64_t>(a[0], 0); n; n = F<std::uint64_t>(n, 0x60))
            if (mx <= F<std::int32_t>(n, 0x54)) mx = F<std::int32_t>(n, 0x54);
        *r = static_cast<std::uint32_t>(mx);
        return true;
    }
    case 0x1401368c0: {  // node with id (+0x54) == n along +0x60
        std::uint64_t n = F<std::uint64_t>(a[0], 0);
        while (n && F<std::int32_t>(n, 0x54) != static_cast<std::int32_t>(a[1])) n = F<std::uint64_t>(n, 0x60);
        *r = n;
        return true;
    }
    case 0x1402582b0: {  // max(count +0x10, ids +0 along +0x38)
        std::int32_t mx = F<std::int32_t>(a[0], 0x10);
        for (std::uint64_t n = F<std::uint64_t>(a[0], 0); n; n = F<std::uint64_t>(n, 0x38))
            if (mx <= F<std::int32_t>(n, 0)) mx = F<std::int32_t>(n, 0);
        *r = static_cast<std::uint32_t>(mx);
        return true;
    }
    case 0x1402580f0: {
        std::uint64_t n = F<std::uint64_t>(a[0], 0);
        while (n && F<std::int32_t>(n, 0) != static_cast<std::int32_t>(a[1])) n = F<std::uint64_t>(n, 0x38);
        *r = n;
        return true;
    }
    case 0x140130290: *r = static_cast<std::uint32_t>(dynary_len(a[0])); return true;
    case 0x140130200: { const std::int32_t* f = ext::as_p<std::int32_t>(a[0]); *r = static_cast<std::uint32_t>(f[0] == 0x7c ? f[0x10] : f[7] << 1); return true; }
    case 0x140165170: {  // NBF: 0 unless mesh, elements and surfaces; 3D: +0x88c after 140165d30
        std::uint64_t o = a[0];
        std::int32_t n = 0;
        if (F<std::int32_t>(o, 0x87c) != 0 && F<std::int32_t>(o, 0x884) != 0 && F<std::int32_t>(o, 0xbd8) != 0) {
            F<std::int32_t>(o, 0x88c) = 0;
            for (int k = 0; k < F<std::int32_t>(o, 0xbd8); ++k) F<std::int32_t>(o, 0x88c) += F<std::int32_t>(o, 0xbe0 + k * 0x30 + 0x10);
            n = F<std::int32_t>(o, 0x88c);
            if (n < 0) n = 0;
        }
        *r = static_cast<std::uint32_t>(n);
        return true;
    }
    case 0x140184a30: *r = F<std::uint64_t>(a[0], 0xe0); return true;  // USRELM slot (no rebuild here)
    case 0x1401d5820: *r = 0; return true;                               // 1401d2bb0 is 1: nothing
    case 0x140203a20: {  // obj+0x3990 && surface 0 has nodes and elements
        std::uint64_t o = a[0];
        auto* g = static_cast<OBJ_GEO3D*>(ext::as_p<LLOBJ_DATA>(o)->u3c00[0]);
        *r = (F<std::uint8_t>(o, 0x3990) != 0 && g && g->node_count() > 0 && g->elem_count() > 0) ? 1 : 0;
        return true;
    }
    case 0x140160e50: case 0x140160eb0: {
        auto* g = reinterpret_cast<OBJ_GEO3D*>(ext::as_p<char>(a[0]) + (addr == 0x140160e50 ? 0x3328 : 0x3458));
        *r = (g->node_count() > 0 && g->elem_count() > 0) ? 1 : 0;
        return true;
    }
    case 0x140167d70: case 0x140167e00: {
        std::int32_t n = 0;
        for (std::ptrdiff_t off : {0x3328, 0x3458}) {
            auto* g = reinterpret_cast<OBJ_GEO3D*>(ext::as_p<char>(a[0]) + off);
            if (g->node_count() > 0 && g->elem_count() > 0) n += addr == 0x140167d70 ? g->node_count() : g->elem_count();
        }
        *r = static_cast<std::uint32_t>(n);
        return true;
    }
    case 0x1402fe9d0: *r = ext::u(ext::as_p<OBJ_GEO3D>(a[0])->elems.begin()); return true;
    case 0x1402fcfe0:  // surface-set rebuild 1402fced0 not modelled: count of the +0x40 list
        *r = static_cast<std::uint32_t>(cow_count(ext::as_p<char>(a[0]) + 0x40));
        g_unhandled.insert(addr);
        return true;
    case 0x14019a080: *r = static_cast<std::uint32_t>(cow_count(ext::as_p<char>(a[0]) + 0x50)); return true;
    case 0x14019a0a0: *r = static_cast<std::uint32_t>(cow_count(ext::as_p<char>(a[0]) + 0x58)); return true;
    case 0x1402fd3e0: case 0x1402fd740: {
        const std::ptrdiff_t off = addr == 0x1402fd3e0 ? 0x50 : 0x58;
        const std::int32_t n = *ext::as_p<std::int32_t>(a[1]);
        const std::int32_t c = cow_count(ext::as_p<char>(a[0]) + off);
        if (n < 1 || c < n) { *r = ext::u(g_static_item); return true; }
        *r = ext::u(static_cast<char*>(cow_walk(*reinterpret_cast<void**>(ext::as_p<char>(a[0]) + off), n - 1)) + 0x10);
        return true;
    }
    case 0x1402ffcd0: case 0x1402fffa0: *r = ext::u(cow_walk(ext::as_p<void>(a[0]), a[1])); return true;
    case 0x14019a000: case 0x14019a060: *r = static_cast<std::uint32_t>(item_cnt(a[0])); return true;
    case 0x1402f8760: *r = static_cast<std::uint32_t>(item50_per(a[0])); return true;
    case 0x1402f8ce0: *r = static_cast<std::uint32_t>(item58_per(a[0])); return true;
    case 0x1402f87a0: { std::int32_t c = item_cnt(a[0]), p = item50_per(a[0]); *r = static_cast<std::uint32_t>((c < 1 || p < 1) ? 0 : c * p + 10); return true; }
    case 0x1402f8d30: { std::int32_t c = item_cnt(a[0]), p = item58_per(a[0]); *r = static_cast<std::uint32_t>((c < 1 || p < 1) ? 0 : c * p + 10); return true; }
    case 0x1402b7560: case 0x140159a70:  // entry n of the list whose handle is at a[0]: walk + 0x10
        *r = ext::u(static_cast<char*>(cow_walk(*ext::as_p<void*>(a[0]), a[1])) + 0x10);
        return true;
    // ---- bclists
    case 0x140189580: *r = static_cast<std::uint32_t>(F<std::int32_t>(a[0], 0x40 + 0x14)); return true;
    case 0x140195b30: *r = static_cast<std::uint32_t>(F<std::int32_t>(a[0], 0x48 + 0x14)); return true;
    case 0x140195b60: *r = static_cast<std::uint32_t>(F<std::int32_t>(a[0], 0x78 + 0x14)); return true;
    case 0x140195b90: *r = static_cast<std::uint32_t>(F<std::int32_t>(a[0], 0xa8 + 0x14)); return true;
    case 0x140195bc0: *r = static_cast<std::uint32_t>(F<std::int32_t>(a[0], 0xd8 + 0x14)); return true;
    case 0x140195bf0: *r = static_cast<std::uint32_t>(F<std::int32_t>(a[0], 0x108 + 0x14)); return true;
    case 0x140195c20: *r = static_cast<std::uint32_t>(F<std::int32_t>(a[0], 0x138 + 0x14)); return true;
    case 0x140195c50: *r = static_cast<std::uint32_t>(F<std::int32_t>(a[0], 0x168 + 0x14)); return true;
    case 0x14019a530: *r = static_cast<std::uint32_t>(F<std::int32_t>(a[0], 0x200 + 0x10)); return true;
    case 0x14019a560: *r = static_cast<std::uint32_t>(F<std::int32_t>(a[0], 0x218 + 0x18)); return true;
    case 0x140269900: case 0x140259b60: {  // nth (1-based, head for n <= 1), links +0x840 / +0xe0
        const std::ptrdiff_t link = addr == 0x140269900 ? 0x840 : 0xe0;
        std::int32_t k = 0;
        std::uint64_t n = F<std::uint64_t>(a[0], 0);
        while (n && (++k, k < static_cast<std::int32_t>(a[1]))) n = F<std::uint64_t>(n, link);
        *r = n;
        return true;
    }
    case 0x14025a9e0: {
        std::int32_t k = 0;
        std::uint64_t n = F<std::uint64_t>(a[0], 8);
        while (n && (++k, k < static_cast<std::int32_t>(a[1]))) n = F<std::uint64_t>(n, 0x10);
        *r = n;
        return true;
    }
    case 0x140186d00: {  // entries of the +0x200 list with type (+4) 5 or 6
        std::int32_t c = F<std::int32_t>(a[0], 0x200 + 0x10), n = 0;
        for (std::int32_t i = 1; i <= c; ++i) {
            std::int32_t k = 0;
            std::uint64_t x = F<std::uint64_t>(a[0], 0x200);
            while (x && (++k, k < i)) x = F<std::uint64_t>(x, 0xe0);
            if (F<std::int32_t>(x, 4) == 5 || F<std::int32_t>(x, 4) == 6) ++n;
        }
        *r = static_cast<std::uint32_t>(n);
        return true;
    }
    case 0x14025c830: *r = F<std::uint64_t>(a[0], 0x888) ? (def::rt::dimension_is_2(ext::as_p<void>(F<std::uint64_t>(a[0], 0x888))) ? 1 : 0) : 0; return true;
    default:
        g_unhandled.insert(addr);
        *r = 0;
        return true;
    }
}

// ---------------------------------------------------------------------------------------------
struct Hooks : def::key::KeyReaderHooks {
    DB_STEP_DATA& m;
    explicit Hooks(DB_STEP_DATA& model) : m(model) {}
    bool has_sub_model() override { return false; }
    void set_model_dimension(int dim) override
    {
        fld<std::int32_t>(&m, 0x1c) = dim;
        def::model::step_setup_array_sizes(m, dim);
    }
    void set_sub_model_dimension(int) override {}
    int parse_keyword(int group, std::FILE* f, int id) override
    {
        if (group == 1) return def::key::read_group1(&m, f, id);
        if (group == 2) return def::key::g2obj::read_object_keyword(m, f, id);
        if (group == 4) return def::key::g4::read_keyword(m, f, id);
        char line[0x400];
        std::fgets(line, sizeof line, f);
        return 0;
    }
};

struct Region { const char* name; bool mar; int slot; };
// 3D slots (docs/re/dbmap/region_globals.tsv) the object pass writes
const Region kRegions[] = {
    {"IIOBJ", true, 3}, {"IINOD", true, 4}, {"IIMTR", true, 5}, {"IIBCD", true, 7}, {"IIBFD", true, 8},
    {"IIBCT", true, 9}, {"IIBFT", true, 10}, {"IIDOD", true, 13}, {"IIBDY", true, 14}, {"IIBEL", true, 15},
    {"IIBND", true, 16}, {"IIEDE", true, 17}, {"IIETM", true, 18}, {"IIEAT", true, 19}, {"IIEDF", true, 20},
    {"IIETF", true, 21}, {"IIEAF", true, 22}, {"IIFLD", true, 23}, {"IIERH", true, 24}, {"IIERF", true, 25},
    {"LLOBJ", false, 5}, {"LLBND", false, 6}, {"LLGRY", false, 8}, {"LLCRZ", false, 11}, {"LLDRZ", false, 12},
    {"LLURZ", false, 13}, {"LLFLD", false, 14}, {"LLPFN", false, 15}, {"LLPPR", false, 17}, {"LLDRM", false, 19},
    {"LLTMP", false, 20}, {"LLPHT", false, 21}, {"LLFLX", false, 23}, {"LLND1", false, 24}, {"LLVLT", false, 25},
    {"LLATM", false, 26}, {"LLWER", false, 27}, {"LLMTA", false, 28}, {"LLSTS", false, 29}, {"LLTSR", false, 31},
    {"LLTEP", false, 34}, {"LLRDT", false, 35}, {"LLDMG", false, 38}, {"LLET1", false, 40}, {"LLVLF", false, 41},
    {"LLDVF", false, 42}, {"LLVLN", false, 43}, {"LLHDN", false, 44}, {"LLICB", false, 45}, {"LLGSZ", false, 46},
    {"LLSIZ", false, 47}, {"LLRFL", false, 48}, {"LLYLS", false, 49}, {"LLZEI", false, 50}, {"LLPRE", false, 51},
    {"LLHFL", false, 52}, {"LLAFL", false, 53}, {"LLZMI", false, 56}, {"LLODF", false, 57}, {"LLTRC", false, 58},
    {"LLERV", false, 59}, {"LLCUR", false, 60}, {"LLNRV", false, 61},
};

// Words that come from GENDB stages before the object pass and are not modelled here: the
// boundary-surface node lists (obj+0xbe0 count/list, built by the object checks) feed IIBND and
// LLBND words 1/2; node temperatures from the object temperature feed LLTMP. Reported, not failed.
bool upstream(const char* region) { return std::strcmp(region, "IIBND") == 0 || std::strcmp(region, "LLTMP") == 0; }
bool upstream_llbnd_word(std::int64_t i, std::int32_t w) { return w > 0 && (i % w == 1 || i % w == 2); }

struct Totals { int cases = 0, regions = 0, bad_regions = 0; long long upstream_words = 0; std::map<std::string, int> bad; };

// One run of the object pass; fn0 = functions written by the earlier stages. Returns the
// function-number shift seen in the function-number regions (0 when they agree).
int check_case(const Entry& e, Totals& tot, std::int32_t fn0, bool report)
{
    const std::string keyname = e.label.substr(std::string("p405/gendb/").size());
    fs::path key = e.key_src;  // the corpus file the oracle ran on (read through a copy below)
    fs::path db = e.dir / "files" / "NEW.DB";
    if (key.empty() || !fs::exists(key) || !fs::exists(db)) {
        std::printf("  %s: inputs not found in the cache entry\n", e.label.c_str());
        return 0;
    }
    fs::path copy = fs::temp_directory_path() / ("p405_oracle_" + std::to_string(_getpid()) + ".key");
    fs::copy_file(key, copy, fs::copy_options::overwrite_existing);

    DB_STEP_DATA& m = *new DB_STEP_DATA(3);
    g_m = &m;
    {
        std::FILE* in = std::fopen(copy.string().c_str(), "r");
        std::FILE* sink = std::fopen("NUL", "w");
        Hooks hk(m);
        def::key::read_key_stream(in, hk, sink);
        std::fclose(in);
        std::fclose(sink);
    }
    fs::remove(copy);
    // GENDB's count stage (packet p4-02b, materials) marks the materials the elements use (+4);
    // the object pass renumbers MTLGRP through those marks (14029ba10)
    for (LLOBJ_DATA* o = m.objects.head; o; o = o->next) {
        const std::int32_t* mg = fld<std::int32_t*>(o, 0x9e0 + 0x18);
        for (std::int32_t k = 0; mg && k < fld<std::int32_t>(o, 0x884); ++k) {
            for (int i = 0; i < m.materials.count; ++i) {
                auto* p = reinterpret_cast<std::int32_t*>(def::model::material_list_nth(&m.materials, i + 1));
                if (p && p[0] == mg[k]) p[1] = 1;
            }
        }
    }
    if (verbose() && report) {
        for (LLOBJ_DATA* o = m.objects.head; o; o = o->next) {
            auto* g = static_cast<OBJ_GEO3D*>(o->u3c00[0]);
            std::printf("    obj %d type %d mesh %d nodes %d elems %d surf0 %d/%d geo %p ext %d geo nodes %d elems %d\n",
                        fld<std::int32_t>(o, 8), fld<std::int32_t>(o, 0xc), fld<std::int32_t>(o, 0x87c),
                        fld<std::int32_t>(o, 0x880), fld<std::int32_t>(o, 0x884), fld<std::int32_t>(o, 0xbe0),
                        fld<std::int32_t>(o, 0xbf0), static_cast<void*>(g), g ? g->is_extension() : -1,
                        g ? g->node_count() : -1, g ? g->elem_count() : -1);
            const std::int32_t* mg = fld<std::int32_t*>(o, 0x9e0 + 0x18);
            if (mg) std::printf("      mtlgrp[0] %d\n", mg[0]);
            char* mv = reinterpret_cast<char*>(o) + 0x2160;
            std::printf("      mv +68 %d +ec %d +f0 %.17g dir %.17g %.17g %.17g\n", fld<std::int32_t>(mv, 0x68),
                        fld<std::int32_t>(mv, 0xec), fld<double>(mv, 0xf0), fld<double>(mv, 0x50),
                        fld<double>(mv, 0x58), fld<double>(mv, 0x60));
        }
        for (int i = 0; i < m.materials.count; ++i) {
            auto* p = reinterpret_cast<std::int32_t*>(def::model::material_list_nth(&m.materials, i + 1));
            std::printf("    material %d: number %d flag(+4) %d\n", i + 1, p ? p[0] : -1, p ? p[1] : -1);
        }
    }

    auto dbh = deformdb::Database::open(db);
    const auto& ents = dbh->entries();
    deformdb::StepData st = dbh->read_step(ents.front());
    deformdb::StepModel sm(st);
    std::int32_t mar_tab[140] = {}, a_tab[280] = {};
    for (int i = 1; i <= 70; ++i) { mar_tab[i - 1] = sm.mar_start(i); mar_tab[70 + i - 1] = sm.mar_width(i); }
    for (int i = 1; i <= 140; ++i) { a_tab[i - 1] = sm.a_start(i); a_tab[140 + i - 1] = sm.a_width(i); }

    std::vector<std::int32_t> mar(st.mar.size(), 0);
    std::vector<double> a(st.a.size(), 0.0);
    // regions of the other stages come from the oracle: IIFNC/LLFNC (functions written before the
    // objects), IISIM, LLSIM ...; the object regions start zeroed
    mar = st.mar;
    a = st.a;
    auto region_len = [&](const Region& r) -> std::pair<std::int64_t, std::int64_t> {
        const std::int32_t s = r.mar ? mar_tab[r.slot] : a_tab[r.slot];
        // end = next larger start, else the array end
        std::int64_t e2 = r.mar ? static_cast<std::int64_t>(mar.size()) + 1 : static_cast<std::int64_t>(a.size()) + 1;
        const int n = r.mar ? 70 : 140;
        for (int i = 0; i < n; ++i) {
            const std::int32_t s2 = r.mar ? mar_tab[i] : a_tab[i];
            if (s2 > s && s2 < e2) e2 = s2;
        }
        return {s, e2 - s};
    };
    for (const Region& r : kRegions) {
        auto [s, n] = region_len(r);
        if (s < 1 || n <= 0) continue;
        // LLND1 / LLET1 end with the user variable names, written after the object pass (1401c4a30 /
        // 1401c4be0, packet p4-06): the region keeps the oracle's bytes and the pass overwrites the data
        if (std::strcmp(r.name, "LLND1") == 0 || std::strcmp(r.name, "LLET1") == 0) continue;
        for (std::int64_t i = 0; i < n; ++i) {
            if (r.mar) mar[s - 1 + i] = 0; else a[s - 1 + i] = 0.0;
        }
    }

    // counters left by the earlier stages (p4-01 / p4-02a/b): from the oracle step
    m.u06c = sm.numnp();
    m.u070 = sm.numel();
    {
        auto [s, n] = region_len({"LLBND", false, 6});
        const std::int32_t w = a_tab[146];
        m.u074[(0x80 - 0x74) / 4] = w > 0 ? static_cast<std::int32_t>(n / w) : 0;
    }
    {
        // +0x98: facets of all rigid geometries (IIDOD holds 4 words per facet, then 1 id each)
        auto [s, n] = region_len({"IIDOD", true, 13});
        m.u074[(0x98 - 0x74) / 4] = static_cast<std::int32_t>(n / 5);
    }
    m.u074[(0x88 - 0x74) / 4] = 1 << 20;
    m.u074[(0x8c - 0x74) / 4] = 1 << 28;
    m.u134 = 1;
    m.u138 = 1;
    // functions of the earlier stages: the IIFNC pairs (id, LLFNC start) are numbered in write
    // order; the object pass continues the count. Start = functions before the first object one,
    // taken as the count whose LLFNC start equals the smallest start referenced by the objects is
    // not known here, so the count of IIFNC entries written before stage 4 is read from the
    // sim-control / material words: approximated by the number of functions whose start is below
    // the first function the object pass writes (found by scanning for P405_FN0 or 0).
    m.u10c = fn0;
    std::int32_t* iifnc = mar.data() + (mar_tab[2] - 1);
    double* llfnc = a.data() + (a_tab[0] - 1);
    const std::int32_t* or_iifnc = st.mar.data() + (mar_tab[2] - 1);
    const std::int32_t ll0 = fn0 > 0 ? or_iifnc[2 * fn0] - 1 : 0;  // first LLFNC word of the object pass
    {
        auto [s, n] = region_len({"IIFNC", true, 2});
        for (std::int64_t i = 2 * fn0 + 1; i < n; ++i) iifnc[i] = 0;
        auto [s2, n2] = region_len({"LLFNC", false, 0});
        for (std::int64_t i = ll0; i < n2; ++i) llfnc[i] = 0.0;
    }
    double* llwin = a.data() + (a_tab[9] - 1);
    double* llwrz = a.data() + (a_tab[10] - 1);
    std::int32_t dim3 = 3;
    def::model::select_index_tables(&dim3);
    ext::g_hook = hook;
    g_unhandled.clear();
    write_objects_3d(&m.objects, mar.data(), a.data(), mar_tab, a_tab, iifnc, llfnc, llwin, llwrz);
    ext::g_hook = nullptr;

    // the shift between the global function numbers of the port and of the oracle
    int shift = 0;
    for (int slot : {8, 10, 20, 21, 22, 25}) {
        auto [s, n] = region_len({"", true, slot});
        for (std::int64_t i = 0; s > 0 && i < n && shift == 0; ++i) {
            if (mar[s - 1 + i] > 0 && st.mar[s - 1 + i] > 0 && mar[s - 1 + i] != st.mar[s - 1 + i])
                shift = st.mar[s - 1 + i] - mar[s - 1 + i];
        }
    }
    // IIOBJ words 19..22: first/last global numbers of the object's BC function lists
    {
        const std::int32_t w = mar_tab[73];
        auto [s, n] = region_len({"IIOBJ", true, 3});
        for (std::int64_t r = 0; s > 0 && w > 22 && (r + 1) * w <= n && shift == 0; ++r) {
            for (int k = 19; k <= 22 && shift == 0; ++k) {
                const std::int64_t i = s - 1 + r * w + k;
                if (mar[i] > 0 && st.mar[i] > 0 && mar[i] != st.mar[i]) shift = st.mar[i] - mar[i];
            }
        }
    }
    if (!report) return shift;

    // The DB stores each A region RLE-compressed (p1-12 write_a_regions -> 14044b0fa). Its run test
    // is COMISD equality, so a -0.0 inside a run of zeros comes back as the run's first value
    // (+0.0), e.g. LLBND word 23 = speed * dz = 0 * -1. Compare after the same round trip.
    if (st.compressed) {
        for (const deformdb::Region& rg : st.a_regions) {
            if (rg.stored == 0 || rg.length < 1 || rg.start < 1) continue;
            std::vector<double> buf(a.begin() + (rg.start - 1), a.begin() + (rg.start - 1 + rg.length));
            buf.resize(static_cast<std::size_t>(rg.length) + 2);
            std::int32_t len = rg.length, stored = 0, n2 = 0;
            def::dbio::dbcore::rle_encode_real(&len, buf.data(), &stored);
            buf.resize(static_cast<std::size_t>(std::max(stored, rg.length)) + 2);
            def::dbio::dbcore::rle_decode_real(&stored, buf.data(), &n2);
            if (n2 == rg.length) std::copy(buf.begin(), buf.begin() + n2, a.begin() + (rg.start - 1));
        }
    }

    ++tot.cases;
    int bad_here = 0;
    for (const Region& r : kRegions) {
        auto [s, n] = region_len(r);
        if (s < 1 || n <= 0) continue;
        ++tot.regions;
        std::int64_t diffs = 0, first = -1;
        for (std::int64_t i = 0; i < n; ++i) {
            bool eq = r.mar ? mar[s - 1 + i] == st.mar[s - 1 + i]
                            : std::memcmp(&a[s - 1 + i], &st.a[s - 1 + i], 8) == 0;
            if (!eq && std::strcmp(r.name, "LLBND") == 0 && upstream_llbnd_word(i, a_tab[146])) {
                ++tot.upstream_words;
                continue;
            }
            if (!eq) { if (first < 0) first = i; ++diffs; }
        }
        if (diffs && upstream(r.name)) {
            std::printf("  %s %s: %lld words differ (input from an earlier GENDB stage, not compared)\n", keyname.c_str(),
                        r.name, static_cast<long long>(diffs));
            continue;
        }
        if (diffs) {
            ++tot.bad_regions;
            ++bad_here;
            ++tot.bad[r.name];
            std::printf("  %s %s: %lld of %lld words differ (first at %lld)\n", keyname.c_str(), r.name,
                        static_cast<long long>(diffs), static_cast<long long>(n), static_cast<long long>(first));
            if (verbose()) {
                int shown = 0;
                for (std::int64_t i = first; i < n && shown < 8; ++i) {
                    if (r.mar) {
                        if (mar[s - 1 + i] != st.mar[s - 1 + i]) {
                            std::printf("      [%lld] port %d oracle %d\n", (long long)i, mar[s - 1 + i], st.mar[s - 1 + i]);
                            ++shown;
                        }
                    }
                    else if (std::memcmp(&a[s - 1 + i], &st.a[s - 1 + i], 8) != 0) {
                        std::printf("      [%lld] port %.17g oracle %.17g\n", (long long)i, a[s - 1 + i], st.a[s - 1 + i]);
                        ++shown;
                    }
                }
            }
        }
    }
    // functions written by the object pass: IIFNC pairs fn0+1..end and their LLFNC words
    {
        const std::int32_t end = m.u10c;
        std::int64_t d1 = 0, d2 = 0;
        for (std::int64_t i = 2 * fn0 + 1; i <= 2 * static_cast<std::int64_t>(end); ++i)
            if (iifnc[i] != or_iifnc[i]) ++d1;
        const std::int64_t ll1 = end > fn0 ? or_iifnc[2 * end] - 1 : ll0;
        const double* or_llfnc = st.a.data() + (a_tab[0] - 1);
        for (std::int64_t i = ll0; i < ll1; ++i)
            if (std::memcmp(&llfnc[i], &or_llfnc[i], 8) != 0) ++d2;
        tot.regions += 2;
        if (d1 || d2) {
            tot.bad_regions += (d1 ? 1 : 0) + (d2 ? 1 : 0);
            ++bad_here;
            std::printf("  %s functions %d..%d: IIFNC %lld words differ, LLFNC %lld of %lld words differ\n", keyname.c_str(),
                        fn0 + 1, end, static_cast<long long>(d1), static_cast<long long>(d2),
                        static_cast<long long>(ll1 - ll0));
        }
    }
    std::printf("  %s: %d region(s) differ; functions after the pass %d; unhandled calls:", keyname.c_str(), bad_here, m.u10c);
    for (auto x : g_unhandled) std::printf(" %llx", static_cast<unsigned long long>(x));
    std::printf("\n");
    return shift;
}

} // namespace

TEST_CASE(gendb3d_objects_match_oracle)
{
    auto entries = find_entries();
    if (entries.empty()) {
        std::printf("note: no p405/gendb oracle entries under %s; nothing compared\n", cache_root().string().c_str());
        return;
    }
    const char* only = std::getenv("P405_ORACLE_ONLY");
    Totals tot;
    for (const Entry& e : entries) {
        if (only && e.label.find(only) == std::string::npos) continue;
        // first run: the function numbers the earlier stages used (shift of the object references)
        Totals scratch;
        const int fn0 = check_case(e, scratch, 0, false);
        check_case(e, tot, fn0, true);
    }
    std::printf("cases %d, regions compared %d, differing %d; LLBND boundary words skipped %lld\n",
                tot.cases, tot.regions, tot.bad_regions, tot.upstream_words);
    CHECK_EQ(tot.bad_regions, 0);
}

int main() { return testkit::run_all(); }
