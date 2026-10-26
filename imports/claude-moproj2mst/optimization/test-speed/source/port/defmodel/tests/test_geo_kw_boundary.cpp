// ctest-labels: kw-g2geo boundary
// Boundary build of ELMCON (def::model::geo_kw::build_boundary, 1401fc290) and its Fortran finders.
//
// 1. Hand-checked meshes: the expected chains / faces are worked out from the algorithm in the
//    comments (edge order of FL2_FNDBND, MSVC qsort / shortsort order of the face sums,
//    pair removal, the equal-sum reordering of FL3_FNDBND).
// 2. Oracle: for every cached KFREAD -> KFWRIT run of the original (%USERPROFILE%\.deform-oracle
//    or $DEFORM_ORACLE_CACHE) whose KFWRIT_OUT.KEY has non-uniform EC* records, the object's
//    ELMCON of that output is loaded into an object, the boundary is built, and every EC*
//    record must (a) be found by the slot lookup of kw-g2bc (140200c90 / 140200ff0), (b) come in
//    increasing slot order (the writer walks the slots in order), and (c) carry exactly the
//    nodes the port stores at that slot (edge orientation, face node order). This checks the
//    boundary order and node order against the original indirectly. BOUNDARY_ORACLE_MAX_BYTES
//    (default 3000000) caps the output size, BOUNDARY_ORACLE_VERBOSE=1 prints mismatches.
#define _CRT_SECURE_NO_WARNINGS
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/bc/edge_face.hpp>
#include <defmodel/geo_kw/boundary.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/cls_14017e740.hpp>
#include <testkit/check.hpp>

namespace fs = std::filesystem;
using namespace def::model;
using def::model::geo_kw::build_boundary;

namespace {

template <class T>
T& at(void* base, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<std::byte*>(base) + off);
}

// A zeroed model (only the dimension at +0x1c is read) and a zeroed object owned by it.
struct Fixture {
    std::vector<std::uint64_t> model_mem = std::vector<std::uint64_t>(0x1000, 0);
    std::vector<std::uint64_t> obj_mem = std::vector<std::uint64_t>(sizeof(LLOBJ_DATA) / 8 + 1, 0);
    std::vector<std::int32_t> conn;
    void* model() { return model_mem.data(); }
    LLOBJ_DATA& obj() { return *reinterpret_cast<LLOBJ_DATA*>(obj_mem.data()); }
    Cls_14017e740& rec(int i) { return reinterpret_cast<Cls_14017e740*>(&at<std::byte>(&obj(), 0xbe0))[i]; }

    // dim 2: nnpe 4 per element; dim 3: nnpe 4 (tet) or 8 (brick)
    Fixture(int dim, int nelem, int nnpe, std::vector<std::int32_t> c) : conn(std::move(c))
    {
        at<std::int32_t>(model(), 0x1c) = dim;
        void* o = &obj();
        at<void*>(o, 0x3a98) = model();
        for (int i = 0; i < 100; ++i) rec(i).u028 = model();
        at<std::int32_t>(o, 0x87c) = 1;
        at<std::int32_t>(o, 0x884) = nelem;
        if (dim == 3) {
            at<std::int32_t>(o, 0x894) = nnpe;
            at<std::int32_t>(o, 0x898) = nnpe == 8 ? 6 : 4;
        }
        at<std::int32_t*>(o, 0x9e8) = conn.data();
    }
    ~Fixture()
    {
        for (int i = 0; i < 100; ++i) rec(i).free_arrays();
        for (int k = 0; k < 12; ++k) std::free(bc::edge_face_array(obj(), k));
    }
    std::int32_t nbdry() { return at<std::int32_t>(&obj(), 0xbd8); }
    std::int32_t segments() { return at<std::int32_t>(&obj(), 0x88c); }
    std::vector<std::int32_t> chain(int i)
    {
        const auto* p = static_cast<const std::int32_t*>(rec(i).u008);
        return std::vector<std::int32_t>(p, p + rec(i).u000);
    }
    std::vector<std::int32_t> faces()
    {
        const auto* p = static_cast<const std::int32_t*>(rec(0).u018);
        return std::vector<std::int32_t>(p, p + 4 * rec(0).u010);
    }
    std::vector<std::int32_t> elems()
    {
        const auto* p = static_cast<const std::int32_t*>(rec(0).u020);
        return std::vector<std::int32_t>(p, p + rec(0).u010);
    }
};

using V = std::vector<std::int32_t>;

} // namespace

// 2x2 quads, nodes   7 8 9
//                    4 5 6
//                    1 2 3,  e1 (1 2 5 4), e2 (2 3 6 5), e3 (4 5 8 7), e4 (5 6 9 8).
// Edges in order: (1,2) (2,5) (5,4) (4,1) | (2,3) (3,6) (6,5) (5,2) | (4,5) (5,8) (8,7) (7,4)
// | (5,6) (6,9) (9,8) (8,5). Interior edges cancel with their later reverse; kept in order:
// (1,2) (4,1) (2,3) (3,6) (8,7) (7,4) (6,9) (9,8). Chain from (1,2), always searching from
// edge 2 for the edge starting at the current end: 1 2 3 6 9 8 7 4 1.
TEST_CASE(quad_2x2_one_closed_chain)
{
    Fixture fx(2, 4, 4, {1, 2, 5, 4, 2, 3, 6, 5, 4, 5, 8, 7, 5, 6, 9, 8});
    build_boundary(fx.obj());
    CHECK_EQ(fx.nbdry(), 1);
    CHECK(fx.chain(0) == V({1, 2, 3, 6, 9, 8, 7, 4, 1}));
    CHECK_EQ(fx.segments(), 8);
    // NS = 8 - 2 + 2 * 1; slots: edge (3,6) is slot 2 in either order
    CHECK_EQ(bc::edge_face_slot_count(fx.obj()), 8);
    CHECK_EQ(bc::edge_slot(fx.obj(), 3, 6), 2);
    CHECK_EQ(bc::edge_slot(fx.obj(), 1, 4), 7);
    CHECK(bc::edge_face_array(fx.obj(), 0) != nullptr);
}

// The raw finder output: nodes = chain, 0 terminator replaced by -1 after the last chain.
// Two separate elements: a quad (1 2 3 4) and a triangle (5 6 7 5) -> chains 1 2 3 4 1 and
// 5 6 7 5 at positions 1..5 and 7..10, a 0 at 6, -1 at 11.
TEST_CASE(fl2_two_chains_and_terminators)
{
    std::vector<std::int32_t> conn = {1, 2, 3, 4, 5, 6, 7, 5};
    std::int32_t nelem = 2, nchain = -5;
    std::vector<std::int32_t> ranges(16, 99), nodes(20, 99);
    CHECK_EQ(geo_kw::fl2_fndbnd(&nelem, conn.data(), &nchain, ranges.data(), nodes.data()), 0);
    CHECK_EQ(nchain, 2);
    CHECK(V(ranges.begin(), ranges.begin() + 4) == V({1, 5, 7, 10}));
    CHECK(V(nodes.begin(), nodes.begin() + 12) == V({1, 2, 3, 4, 1, 0, 5, 6, 7, 5, -1, 99}));

    Fixture fx(2, 2, 4, conn);
    build_boundary(fx.obj());
    CHECK_EQ(fx.nbdry(), 2);
    CHECK(fx.chain(0) == V({1, 2, 3, 4, 1}));
    CHECK(fx.chain(1) == V({5, 6, 7, 5}));
    CHECK_EQ(fx.segments(), 7);
    CHECK_EQ(bc::edge_face_slot_count(fx.obj()), 7 - 2 + 4);
    // second chain starts at slot 5 + 1 = 6
    CHECK_EQ(bc::edge_slot(fx.obj(), 6, 5), 6);
}

// No elements: no chain, terminator -1 in slot 1.
TEST_CASE(fl2_empty_mesh)
{
    std::int32_t nelem = 0, nchain = 7;
    std::vector<std::int32_t> ranges(4, 99), nodes(4, 99);
    geo_kw::fl2_fndbnd(&nelem, nullptr, &nchain, ranges.data(), nodes.data());
    CHECK_EQ(nchain, 0);
    CHECK_EQ(nodes[0], -1);
}

// One brick 1..8. Faces (table 0x140604fd0) and sums: s1 (1 4 3 2) 10, s2 (5 6 7 8) 26,
// s3 (1 2 6 5) 14, s4 (2 3 7 6) 18, s5 (3 4 8 7) 22, s6 (4 1 5 8) 18.
// shortsort of the 6 (sum, slot) pairs: pass 1 moves 26 (slot 2) to the end ->
// 10 18(6) 14 18(4) 22 26; pass 2 keeps 22; pass 3 swaps the first 18 (slot 6) with position 4
// -> 10 18(4) 14 18(6); pass 4 -> 10 14 18(4) 18(6). Output slots 1 3 4 6 5 2; the equal sums
// 18: (2 3 7 6) before (4 1 5 8) already (2 < 4).
TEST_CASE(single_brick)
{
    Fixture fx(3, 1, 8, {1, 2, 3, 4, 5, 6, 7, 8});
    build_boundary(fx.obj());
    CHECK_EQ(fx.nbdry(), 1);
    CHECK(fx.faces() == V({1, 4, 3, 2, 1, 2, 6, 5, 2, 3, 7, 6, 4, 1, 5, 8, 3, 4, 8, 7, 5, 6, 7, 8}));
    CHECK(fx.elems() == V({1, 1, 1, 1, 1, 1}));
    CHECK_EQ(fx.segments(), 6);
    CHECK_EQ(bc::edge_face_slot_count(fx.obj()), 6);
    CHECK_EQ(bc::face_slot(fx.obj(), 8, 7, 3, 4), 4);
}

// Two tets A (1 2 3 4), B (2 3 4 5), tet table (1 3 2) (1 2 4) (1 4 3) (2 3 4):
// A: s1 (1 3 2) 6, s2 (1 2 4) 7, s3 (1 4 3) 8, s4 (2 3 4) 9;
// B: s5 (2 4 3) 9, s6 (2 3 5) 10, s7 (2 5 4) 11, s8 (3 4 5) 12.
// shortsort of 8: passes 1-3 keep 12, 11, 10 in place; pass 4 swaps the first 9 (slot 4) with
// position 5 -> order 1 2 3 5 4 6 7 8. Slot 5 (2 4 3) finds all its nodes in slot 4 (same sum
// run) -> slot 4 zeroed and slot 5 not output. Triangles get 4th = 1st; element = (s-1)/4+1.
TEST_CASE(two_tets_share_a_face)
{
    Fixture fx(3, 2, 4, {1, 2, 3, 4, 2, 3, 4, 5});
    build_boundary(fx.obj());
    CHECK(fx.faces() == V({1, 3, 2, 1, 1, 2, 4, 1, 1, 4, 3, 1, 2, 3, 5, 2, 2, 5, 4, 2, 3, 4, 5, 3}));
    CHECK(fx.elems() == V({1, 1, 1, 2, 2, 2}));
    CHECK_EQ(bc::edge_face_slot_count(fx.obj()), 6);
    // triangle mode lookup (face 0 is "a b c a")
    CHECK_EQ(bc::face_slot(fx.obj(), 5, 2, 3, 0), 3);
    CHECK_EQ(bc::face_slot(fx.obj(), 2, 3, 4, 0), -1);
}

// A wedge stored as a brick (two zero nodes): faces from the wedge table, the 6th (3 3 6 6) is
// rejected (2 equal pairs). Sums s1 (1 3 2 1) 7, s2 (4 5 6 4) 19, s3 (1 2 5 4) 12,
// s4 (2 3 6 5) 16, s5 (3 1 4 6) 14. shortsort: 19 to the end (swap with slot 5), 16 stays,
// 14 swaps with 12 -> slots 1 3 5 4 2. Element = (s-1)/5+1 = 1.
TEST_CASE(wedge_as_brick)
{
    Fixture fx(3, 1, 8, {1, 2, 3, 4, 5, 6, 0, 0});
    build_boundary(fx.obj());
    CHECK(fx.faces() == V({1, 3, 2, 1, 1, 2, 5, 4, 3, 1, 4, 6, 2, 3, 6, 5, 4, 5, 6, 4}));
    CHECK(fx.elems() == V({1, 1, 1, 1, 1}));
}

// qsort is not stable: 9 equal keys go through the partition (size > 8).
TEST_CASE(crt_qsort_equal_keys_order)
{
    // keys: 5 x9, values 1..9. median of three: no swaps (all equal); partition: loguy runs to
    // mid (pos 5, 1-based) then past hi; higuy stops at... all comparisons are 0, so loguy goes
    // to hi + 1 and higuy to mid - 1 after the first scan: no swap, break. Then higuy walks down
    // over the equal run to lo. Both halves: lo..higuy empty, loguy > hi: nothing is moved.
    std::vector<std::int32_t> p;
    for (int i = 1; i <= 9; ++i) {
        p.push_back(5);
        p.push_back(i);
    }
    std::int32_t n = 9;
    geo_kw::sort_key_pairs(p.data(), &n);
    for (int i = 0; i < 9; ++i) CHECK_EQ(p[2 * i + 1], i + 1);
    // shortsort with equal maxima: (2,a) (1,b) (2,c): pass 1 takes the first 2 (a) to the end
    std::vector<std::int32_t> q = {2, 1, 1, 2, 2, 3};
    n = 3;
    geo_kw::sort_key_pairs(q.data(), &n);
    CHECK(q == V({1, 2, 2, 3, 2, 1}));
}

// ---------------------------------------------------------------------------------------------
// Oracle check over the cached KFWRIT_OUT.KEY files.

namespace {

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

std::vector<long long> ints_of(const std::string& l)
{
    std::vector<long long> v;
    std::istringstream in(l);
    std::string t;
    while (in >> t) {
        char* end = nullptr;
        long long x = std::strtoll(t.c_str(), &end, 10);
        if (end == t.c_str() || *end != '\0') break;  // stop at the double value
        v.push_back(x);
    }
    return v;
}

struct Totals {
    int cases = 0, objects = 0, records = 0, bad = 0;
    int records_by_nnpe[11] = {0};  // 3D records: [4] tet meshes, [8] brick meshes
    int records_3d = 0;
};

void check_output(const std::string& label, const std::string& text, int dim, Totals& tot, bool verbose)
{
    std::vector<std::string> ls = lines_of(text);
    // object -> connectivity
    struct Mesh {
        int nelem = 0, nnpe = 0;
        std::vector<std::int32_t> conn;
    };
    std::map<int, Mesh> meshes;
    for (std::size_t i = 0; i < ls.size(); ++i) {
        if (ls[i].rfind("ELMCON", 0) != 0) continue;
        std::vector<long long> h = ints_of(ls[i].substr(6));
        if (h.size() < 2) continue;
        Mesh m;
        m.nelem = static_cast<int>(h[1]);
        for (int e = 0; e < m.nelem && i + 1 + e < ls.size(); ++e) {
            std::vector<long long> r = ints_of(ls[i + 1 + e]);
            if (m.nnpe == 0) m.nnpe = static_cast<int>(r.size()) - 1;
            for (std::size_t k = 1; k < r.size(); ++k) m.conn.push_back(static_cast<std::int32_t>(r[k]));
        }
        if (dim == 2 && m.nnpe != 4) continue;
        meshes[static_cast<int>(h[0])] = std::move(m);
    }
    std::map<int, std::unique_ptr<Fixture>> built;
    for (std::size_t i = 0; i < ls.size(); ++i) {
        const std::string& l = ls[i];
        if (l.size() < 7 || l.rfind("EC", 0) != 0 || l[6] != ' ') continue;
        std::vector<long long> h = ints_of(l.substr(6));
        if (h.size() < 2 || h[1] <= 0) continue;
        const int objn = static_cast<int>(h[0]);
        if (meshes.count(objn) == 0) continue;
        if (built.count(objn) == 0) {
            Mesh& m = meshes[objn];
            auto fx = std::make_unique<Fixture>(dim, m.nelem, m.nnpe, m.conn);
            build_boundary(fx->obj());
            built[objn] = std::move(fx);
            ++tot.objects;
        }
        Fixture& fx = *built[objn];
        std::int32_t last = -1;
        for (long long r = 0; r < h[1] && i + 1 + r < ls.size(); ++r) {
            std::vector<long long> v = ints_of(ls[i + 1 + r]);
            ++tot.records;
            if (dim == 3) {
                ++tot.records_3d;
                ++tot.records_by_nnpe[meshes[objn].nnpe == 8 ? 8 : 4];
            }
            bool ok = false;
            std::int32_t s = -1;
            if (dim == 2 && v.size() >= 3) {
                s = bc::edge_slot(fx.obj(), static_cast<int>(v[1]), static_cast<int>(v[2]));
                std::int32_t p = 0, q = 0;
                if (s >= 0) bc::edge_slot_nodes(fx.obj(), s, &p, &q);
                ok = s > last && p == v[1] && q == v[2];
            } else if (dim == 3 && v.size() >= 5) {
                s = bc::face_slot(fx.obj(), static_cast<int>(v[1]), static_cast<int>(v[2]),
                                  static_cast<int>(v[3]), static_cast<int>(v[4]));
                std::int32_t a = 0, b = 0, c = 0, d = 0;
                if (s >= 0) bc::face_nodes(fx.obj(), s, &a, &b, &c, &d);
                ok = s > last && a == v[1] && b == v[2] && c == v[3] && d == v[4];
            }
            if (!ok) {
                ++tot.bad;
                if (verbose)
                    std::printf("  %s obj %d %.6s: record '%s' slot %d (previous %d)\n", label.c_str(),
                                objn, l.c_str(), ls[i + 1 + r].c_str(), s, last);
            }
            if (s > last) last = s;
        }
    }
}

} // namespace

TEST_CASE(oracle_ec_records_follow_the_boundary_order)
{
    fs::path cache = env("DEFORM_ORACLE_CACHE", "");
    if (cache.empty()) cache = fs::path(env("USERPROFILE", "")) / ".deform-oracle";
    if (!fs::exists(cache / "entries")) {
        std::printf("no oracle cache at %s: skipped\n", cache.string().c_str());
        return;
    }
    const std::uintmax_t max_bytes = std::strtoull(env("BOUNDARY_ORACLE_MAX_BYTES", "3000000").c_str(), nullptr, 10);
    const bool verbose = env("BOUNDARY_ORACLE_VERBOSE", "0") == "1";
    Totals tot;
    for (auto& d1 : fs::directory_iterator(cache / "entries")) {
        if (!d1.is_directory()) continue;
        for (auto& d2 : fs::directory_iterator(d1.path())) {
            fs::path rj = d2.path() / "result.json";
            fs::path out = d2.path() / "files" / "KFWRIT_OUT.KEY";
            if (!fs::exists(rj) || !fs::exists(out) || fs::file_size(out) > max_bytes) continue;
            std::string label = json_string(slurp(rj), "label");
            if (label.rfind("kfrw/", 0) != 0) continue;
            int dim = label.find("/2D/") != std::string::npos ? 2 : label.find("/3D/") != std::string::npos ? 3 : 0;
            if (dim == 0) continue;
            std::string text = slurp(out);
            if (text.find("\nEC") == std::string::npos) continue;
            ++tot.cases;
            check_output(label, text, dim, tot, verbose);
        }
    }
    std::printf("oracle: %d outputs, %d objects with EC records, %d records (3D: %d tet, %d brick), %d bad\n",
                tot.cases, tot.objects, tot.records, tot.records_by_nnpe[4], tot.records_by_nnpe[8], tot.bad);
    CHECK_EQ(tot.bad, 0);
}

int main() { return testkit::run_all(); }
