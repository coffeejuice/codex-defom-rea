// ctest-labels: kw-g2bc ec
// kw-g2bc edge/face BCs (ECCDEF .. ECRFLX): the model slot functions (defmodel/bc/edge_face.hpp),
// the readers 14041a090 / 14041a440, the line writers, handlers and the block writer 1402abf20.
//
// Expected text comes from the original DEF_PRE (KFREAD + KFWRIT):
//   * tests/data/kw_g2bc/ec/c2_ec.key, c3_ec.key: crafted inputs, run with oracle_cases.json; the
//     .expected files hold the EC records of OUT.KEY, the .stdout files the ERROR lines of stdout;
//   * the cached kfrw/ round trips of the corpus (%USERPROFILE%\.deform-oracle or
//     $DEFORM_ORACLE_CACHE): every EC record of every object is compared.
//
// INTERIM stand-ins (test only, no @src): the boundary is built by ELMCON (FUN_1401fc290 with the
// Fortran finders FUN_14032d65e / FUN_140354ed0), which belongs to the mesh packet. Here:
//   * RZ only stores the node count obj+0x880 and skips the coordinate lines;
//   * ELMCON stores the connectivity in the test and builds the boundary:
//       2D: transcription of FUN_14032da62 (edge list, removal of shared edges, chaining into
//           closed loops) and of the 2D part of FUN_1401fc290;
//       3D: boundary faces of tets (faces 132 124 143 234, stored a b c a) and bricks (1432 1265
//           2376 3487 4158 5678), shared faces removed, stable-sorted by node sum, then the
//           (first, second node) exchange pass of FUN_140354ed0. The face order of the Fortran
//           finders FUN_140355cbf / FUN_14035672f is not known, so a 3D mismatch is also
//           re-checked as a set (records without their number) and reported separately.
//   * the loop records' model pointer (+0x28, set by 14015fbf0) and the state-variable widths
//     model+0x16fc..0x1728 (1, DB_STEP_DATA ctor) are set by the test.
// KW_G2BC_EC_MAX_BYTES (default 3000000) caps the corpus input size, KW_G2BC_EC_VERBOSE=1 prints
// each mismatch.
#define _CRT_SECURE_NO_WARNINGS
#include "key_test_util.hpp"

#include <io.h>

#include <algorithm>
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

#include <defkey/crt_scan.hpp>
#include <defkey/key_reader.hpp>
#include <defkey/kw_g2bc.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defkey/line_io.hpp>
#include <defkey/node_elem_var.hpp>
#include <defmodel/bc/edge_face.hpp>
#include <defmodel/keyword_registry.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/cls_14017e740.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <testkit/check.hpp>


namespace fs = std::filesystem;
using namespace def::key;
using def::model::Cls_14017e740;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;
namespace bc = def::model::bc;

namespace {

template <class T, class C>
T& at(C& base, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<unsigned char*>(&base) + off);
}

Cls_14017e740& bdry(LLOBJ_DATA& obj, int k) { return reinterpret_cast<Cls_14017e740*>(&at<unsigned char>(obj, 0xbe0))[k]; }

// ---- model setup ------------------------------------------------------------------------------

DB_STEP_DATA* new_model(int dim)
{
    auto* m = new DB_STEP_DATA(3);  // the real model ctor (p1-03) sets the widths W[0x41..0x4c]
    at<std::int32_t>(*m, 0x1c) = dim;
    return m;
}

LLOBJ_DATA* new_object(DB_STEP_DATA& m, int no)
{
    LLOBJ_DATA* o = def::model::llobj_ctor::get_or_create(&m, no);
    for (int k = 0; k < 100; ++k) bdry(*o, k).u028 = &m;
    return o;
}

struct Mesh {
    int npe = 4;
    std::vector<std::int32_t> conn;  // npe per element (4 in 2D)
};

// INTERIM: FUN_14032da62 (2D boundary chains) + the 2D part of FUN_1401fc290
void build_boundary_2d(LLOBJ_DATA& obj, const Mesh& mesh)
{
    const int nelem = at<std::int32_t>(obj, 0x884);
    std::vector<std::int32_t> e1(static_cast<std::size_t>(nelem) * 4 + 2), e2(e1.size());
    int ne = 0;
    for (int e = 1; e <= nelem; ++e) {
        const std::int32_t* c = &mesh.conn[static_cast<std::size_t>(e - 1) * 4];
        for (int k = 1; k < 5; ++k) {
            if (k == 4 && c[0] == c[3]) break;
            ++ne;
            e1[ne] = c[k - 1];
            e2[ne] = c[k % 4];
        }
    }
    int cnt = 0;
    for (int i = 1; i <= ne; ++i) {
        int a = e1[i];
        if (a == 0) continue;
        bool shared = false;
        if (i != ne) {
            for (int j = i + 1; j <= ne; ++j) {
                if (e1[j] == e2[i] && e2[j] == a) {
                    e1[j] = 0;
                    shared = true;
                    break;
                }
            }
        }
        if (!shared) {
            ++cnt;
            e1[cnt] = a;
            e2[cnt] = e2[i];
        }
    }
    std::vector<std::int32_t> list(std::max(2 * nelem + 0x3ea, 10000) + 2);
    std::vector<std::int32_t> start(1), end(1);
    int pos = 0;
    for (int i = 1; i <= cnt; ++i) {
        int a = e1[i];
        if (a == 0) continue;
        int cur = i;
        ++pos;
        start.push_back(pos);
        end.push_back(0);
        list[pos] = a;
        for (;;) {
            ++pos;
            int b = e2[cur];
            e1[cur] = 0;
            list[pos] = b;
            if (cnt < i + 1) break;
            int j = i + 1;
            while (j <= cnt && e1[j] != b) ++j;
            if (cnt < j) break;
            cur = j;
        }
        end.back() = pos;
        ++pos;
        list[pos] = 0;
    }
    int nloops = static_cast<int>(start.size()) - 1;
    at<std::int32_t>(obj, 0xbd8) = nloops < 0x65 ? nloops : 100;
    for (int k = 0; k < at<std::int32_t>(obj, 0xbd8) && k < 100; ++k) {
        Cls_14017e740& b = bdry(obj, k);
        b.u000 = end[k + 1] - start[k + 1] + 1;
        bc::alloc_boundary_arrays(b);
        std::memcpy(b.u008, &list[start[k + 1]], static_cast<std::size_t>(b.u000) * 4);
    }
}

// INTERIM: boundary faces of a 3D mesh (see the file comment)
void build_boundary_3d(LLOBJ_DATA& obj, const Mesh& mesh)
{
    static const int tet[4][3] = {{0, 2, 1}, {0, 1, 3}, {0, 3, 2}, {1, 2, 3}};
    static const int hex[6][4] = {{0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5}, {2, 3, 7, 6}, {3, 0, 4, 7}, {4, 5, 6, 7}};
    const int nelem = at<std::int32_t>(obj, 0x884);
    struct Face {
        std::int32_t n[4];
        std::int64_t sum;
    };
    std::vector<Face> all;
    std::map<std::vector<std::int32_t>, int> uses;
    auto key = [&](const Face& f) {
        std::vector<std::int32_t> k(f.n, f.n + (mesh.npe == 8 ? 4 : 3));
        std::sort(k.begin(), k.end());
        return k;
    };
    for (int e = 0; e < nelem; ++e) {
        const std::int32_t* c = &mesh.conn[static_cast<std::size_t>(e) * mesh.npe];
        if (mesh.npe == 8) {
            for (auto& d : hex) {
                Face f{{c[d[0]], c[d[1]], c[d[2]], c[d[3]]}, 0};
                f.sum = static_cast<std::int64_t>(f.n[0]) + f.n[1] + f.n[2] + f.n[3];
                all.push_back(f);
            }
        } else {
            for (auto& d : tet) {
                Face f{{c[d[0]], c[d[1]], c[d[2]], c[d[0]]}, 0};
                f.sum = static_cast<std::int64_t>(f.n[0]) + f.n[1] + f.n[2];
                all.push_back(f);
            }
        }
    }
    for (auto& f : all) ++uses[key(f)];
    std::vector<Face> bf;
    for (auto& f : all)
        if (uses[key(f)] == 1) bf.push_back(f);
    std::stable_sort(bf.begin(), bf.end(), [](const Face& a, const Face& b) { return a.sum < b.sum; });
    for (std::size_t i = 0; i < bf.size(); ++i) {
        for (std::size_t j = i + 1; j < bf.size() && bf[j].sum == bf[i].sum; ++j) {
            if (bf[j].n[0] < bf[i].n[0] || (bf[j].n[0] == bf[i].n[0] && bf[j].n[1] < bf[i].n[1])) std::swap(bf[i], bf[j]);
        }
    }
    bc::set_boundary_count(obj, 1);
    bc::set_boundary_face_count(obj, static_cast<std::int32_t>(bf.size()), 0);
    auto* F = static_cast<std::int32_t*>(bdry(obj, 0).u018);
    for (std::size_t i = 0; i < bf.size(); ++i) std::memcpy(F + 4 * i, bf[i].n, 16);
}

// INTERIM: the boundary part of ELMCON (FUN_1401fc290), then 140165d30 and 1401631e0 (ported)
void build_boundary(LLOBJ_DATA& obj, const Mesh& mesh)
{
    if (at<std::int32_t>(obj, 0x87c) == 0 || at<std::int32_t>(obj, 0x884) <= 0) return;
    if (at<std::int32_t>(*obj.get_owner(), 0x1c) == 2)
        build_boundary_2d(obj, mesh);
    else
        build_boundary_3d(obj, mesh);
    bc::recount_boundary_segments(obj);
    bc::realloc_edge_face_arrays(obj);
}

// ---- KEY loop hooks ---------------------------------------------------------------------------

struct Hooks : KeyReaderHooks {
    DB_STEP_DATA& m;
    std::map<LLOBJ_DATA*, Mesh> meshes;
    int rz = def::model::keyword_id("RZ");
    int elmcon = def::model::keyword_id("ELMCON");
    int objtyp = def::model::keyword_id("OBJTYP");
    explicit Hooks(DB_STEP_DATA& model) : m(model) {}
    bool has_sub_model() override { return false; }
    void set_model_dimension(int dim) override { at<std::int32_t>(m, 0x1c) = dim; }
    void set_sub_model_dimension(int) override {}
    LLOBJ_DATA* object(std::FILE* f)
    {
        std::int32_t no = 0;
        if (crt::fscanf(f, "%i", &no) == -1) return nullptr;
        LLOBJ_DATA* o = def::model::llobj_ctor::get_or_create(&m, no);
        if (o != nullptr)
            for (int k = 0; k < 100; ++k) bdry(*o, k).u028 = &m;
        return o;
    }
    int parse_keyword(int group, std::FILE* f, int id) override
    {
        if (group != 2) return 0;
        if (id == rz) {
            LLOBJ_DATA* o = object(f);
            if (o == nullptr) return 2;
            std::int32_t nn = 0;
            if (read_i(f, &nn) != 0) return 2;
            char line[0x400];
            for (int i = 0; i < nn; ++i)
                if (std::fgets(line, 0x400, f) == nullptr) return 2;
            at<std::int32_t>(*o, 0x880) = nn;
            return 0;
        }
        if (id == elmcon) {
            LLOBJ_DATA* o = object(f);
            if (o == nullptr) return 2;
            const bool d3 = at<std::int32_t>(m, 0x1c) != 2;
            std::int32_t nelem = 0, npe = 4;
            if (d3) {
                if (read_ii(f, &nelem, &npe) != 0) return 2;
                if (npe != 4 && npe != 8) return 2;
            } else if (read_i(f, &nelem) != 0) {
                return 2;
            }
            at<std::int32_t>(*o, 0x884) = nelem;
            Mesh mesh;
            mesh.npe = npe;
            for (int e = 0; e < nelem; ++e) {
                std::int32_t v[9] = {0};
                if (npe == 8) {
                    std::int32_t* p[9];
                    for (int i = 0; i < 9; ++i) p[i] = &v[i];
                    if (read_i9(f, p) != 0) return 2;
                } else if (read_i5(f, &v[0], &v[1], &v[2], &v[3], &v[4]) != 0) {
                    return 2;
                }
                for (int i = 0; i < npe; ++i) mesh.conn.push_back(v[1 + i]);
            }
            at<std::int32_t>(*o, 0x87c) = 1;
            meshes[o] = mesh;
            build_boundary(*o, meshes[o]);
            return 0;
        }
        // only OBJTYP and the EC keywords go to the group 2 parser: the other packets' readers
        // need node/element arrays this fixture does not build
        if (id == objtyp || (241 <= id && id <= 252)) return g2obj::read_object_keyword(m, f, id);
        return 0;
    }
};

// ---- record comparison ------------------------------------------------------------------------

std::string slurp(const fs::path& p)
{
    std::ifstream in(p, std::ios::binary);
    std::ostringstream s;
    s << in.rdbuf();
    return s.str();
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

bool is_ec_name(const std::string& n) { return n.size() == 6 && n.rfind("EC", 0) == 0 && def::model::keyword_id(n.c_str()) >= 241 && def::model::keyword_id(n.c_str()) <= 252; }

// (object, keyword) -> header line + record lines
using Blocks = std::map<std::pair<int, std::string>, std::vector<std::string>>;

Blocks ec_blocks(const std::string& text)
{
    Blocks r;
    std::vector<std::string> ls = lines_of(text);
    for (std::size_t i = 0; i < ls.size(); ++i) {
        char name[64] = {0};
        int obj = 0;
        if (ls[i].empty() || ls[i][0] == ' ') continue;
        if (std::sscanf(ls[i].c_str(), "%63s %i", name, &obj) < 2 || !is_ec_name(name)) continue;
        std::vector<std::string> b{ls[i]};
        std::size_t j = i + 1;
        while (j < ls.size() && !ls[j].empty() && ls[j][0] == ' ') b.push_back(ls[j++]);
        r[{obj, name}] = b;
        i = j - 1;
    }
    return r;
}

// the block without record numbers, as a sorted multiset of node sets and values
std::vector<std::string> as_set(const std::vector<std::string>& b)
{
    std::vector<std::string> out;
    if (b.empty()) return out;
    out.push_back(b[0]);
    for (std::size_t i = 1; i < b.size(); ++i) {
        std::istringstream s(b[i]);
        std::vector<std::string> t;
        std::string w;
        while (s >> w) t.push_back(w);
        if (t.size() < 2) continue;
        std::vector<long> nodes;
        for (std::size_t k = 1; k + 1 < t.size(); ++k) nodes.push_back(std::strtol(t[k].c_str(), nullptr, 10));
        std::sort(nodes.begin(), nodes.end());
        nodes.erase(std::unique(nodes.begin(), nodes.end()), nodes.end());
        std::string e;
        for (long n : nodes) e += std::to_string(n) + ",";
        out.push_back(e + "=" + t.back());
    }
    std::sort(out.begin() + 1, out.end());
    return out;
}

struct Result {
    int blocks = 0, exact = 0, set_only = 0, bad = 0, missing = 0;
};

// Reads `input` through the KEY loop, writes step 54 of every object, compares with `expected`
// (KFWRIT text or extracted EC records).
void run_case(const fs::path& input, const std::string& expected, Result& res, bool verbose,
              int* edge_errors = nullptr)
{
    fs::path copy = fs::temp_directory_path() / keytest::tmp_name("kw_g2bc_ec_input.key");
    fs::copy_file(input, copy, fs::copy_options::overwrite_existing);
    DB_STEP_DATA* m = new_model(3);
    std::FILE* in = std::fopen(copy.string().c_str(), "r");
    std::FILE* sink = std::fopen("NUL", "w");
    Hooks hooks(*m);
    // stdout (the readers' "ERROR in readEdgeFaceVarInt") goes to a file while reading
    fs::path so = fs::temp_directory_path() / keytest::tmp_name("kw_g2bc_ec_stdout.txt");
    std::fflush(stdout);
    int saved = _dup(1);
    std::FILE* sof = std::fopen(so.string().c_str(), "w");
    _dup2(_fileno(sof), 1);
    read_key_stream(in, hooks, sink);
    std::fflush(stdout);
    _dup2(saved, 1);
    _close(saved);
    std::fclose(sof);
    std::fclose(in);
    std::fclose(sink);
    if (edge_errors != nullptr) {
        *edge_errors = 0;
        for (auto& l : lines_of(slurp(so)))
            if (l == "ERROR in readEdgeFaceVarInt, iECCArrayAdr") ++*edge_errors;
    }

    fs::path outp = fs::temp_directory_path() / keytest::tmp_name("kw_g2bc_ec_out.key");
    std::FILE* out = std::fopen(outp.string().c_str(), "wb");
    for (LLOBJ_DATA* o = at<LLOBJ_DATA*>(*m, 0x1380); o != nullptr; o = at<LLOBJ_DATA*>(*o, 0x3940))
        g2bc::write_edge_face_bcs(*m, out, *o, 0);
    std::fclose(out);
    Blocks mine = ec_blocks(slurp(outp));
    Blocks orig = ec_blocks(expected);
    const bool d3 = at<std::int32_t>(*m, 0x1c) != 2;
    std::set<std::pair<int, std::string>> keys;
    for (auto& [k, v] : orig) keys.insert(k);
    for (auto& [k, v] : mine) keys.insert(k);
    for (auto& k : keys) {
        ++res.blocks;
        auto a = orig.find(k);
        auto b = mine.find(k);
        if (a == orig.end() || b == mine.end()) {
            ++res.missing;
            if (verbose)
                std::printf("MISSING %s obj %d %s (%s)\n", input.string().c_str(), k.first, k.second.c_str(),
                            a == orig.end() ? "not in original" : "not in port");
            continue;
        }
        if (a->second == b->second) {
            ++res.exact;
        } else if (d3 && as_set(a->second) == as_set(b->second)) {
            ++res.set_only;
        } else {
            ++res.bad;
            if (verbose) {
                std::printf("MISMATCH %s obj %d %s\n  orig:", input.string().c_str(), k.first, k.second.c_str());
                for (std::size_t i = 0; i < a->second.size() && i < 12; ++i) std::printf("\n%s", a->second[i].c_str());
                std::printf("\n  port:");
                for (std::size_t i = 0; i < b->second.size() && i < 12; ++i) std::printf("\n%s", b->second[i].c_str());
                std::printf("\n");
            }
        }
    }
    // objects and arrays are leaked on purpose (their real dtor is not in this packet)
    fs::remove(copy);
    fs::remove(outp);
    fs::remove(so);
}

std::string env(const char* name, const char* def)
{
    const char* v = std::getenv(name);
    return v != nullptr && *v != '\0' ? v : def;
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

fs::path data(const char* name)
{
    return testkit::repo_root() / "port" / "defkey" / "tests" / "data" / "kw_g2bc" / "ec" / name;
}

// ---- hand-built boundaries --------------------------------------------------------------------

// 2D object: two closed loops 1-2-3-4-1 (n = 5) and 5-6-7-5 (n = 4), 7 nodes, 2 elements
LLOBJ_DATA* object_2d(DB_STEP_DATA& m)
{
    LLOBJ_DATA* o = new_object(m, 1);
    at<std::int32_t>(*o, 0x87c) = 1;
    at<std::int32_t>(*o, 0x880) = 7;
    at<std::int32_t>(*o, 0x884) = 2;
    at<std::int32_t>(*o, 0xbd8) = 2;
    const std::int32_t l0[5] = {1, 2, 3, 4, 1}, l1[4] = {5, 6, 7, 5};
    bdry(*o, 0).u000 = 5;
    bc::alloc_boundary_arrays(bdry(*o, 0));
    std::memcpy(bdry(*o, 0).u008, l0, sizeof l0);
    bdry(*o, 1).u000 = 4;
    bc::alloc_boundary_arrays(bdry(*o, 1));
    std::memcpy(bdry(*o, 1).u008, l1, sizeof l1);
    bc::realloc_edge_face_arrays(*o);
    return o;
}

std::string read_ints(LLOBJ_DATA& o, const std::string& text, int k, int* st)
{
    keytest::InFile in("kw_g2bc_ec_in.txt", text);
    *st = read_edge_face_ints(in.get(), static_cast<std::int32_t*>(bc::edge_face_array(o, k)), &o, 1);
    return in.rest();
}

} // namespace

TEST_CASE(slot_count_and_2d_lookup)
{
    DB_STEP_DATA* m = new_model(2);
    LLOBJ_DATA* o = object_2d(*m);
    // segments 4 + 3, NS = 7 - 2 + 2 * 2
    CHECK_EQ(bc::edge_face_slot_count(*o), 9);
    CHECK_EQ(at<std::int32_t>(*o, 0x88c), 7);
    CHECK_EQ(bc::edge_slot(*o, 1, 2), 0);
    CHECK_EQ(bc::edge_slot(*o, 2, 1), 0);
    CHECK_EQ(bc::edge_slot(*o, 4, 1), 3);
    CHECK_EQ(bc::edge_slot(*o, 5, 6), 6);  // loop 1 starts at 5 + 1
    CHECK_EQ(bc::edge_slot(*o, 7, 5), 8);
    CHECK_EQ(bc::edge_slot(*o, 1, 3), -1);
    std::int32_t p = -1, q = -1;
    bc::edge_slot_nodes(*o, 3, &p, &q);
    CHECK(p == 4 && q == 1);
    bc::edge_slot_nodes(*o, 4, &p, &q);  // spare slot: nodes[4], nodes[5] (past the list: 0)
    CHECK(p == 1 && q == 0);
    bc::edge_slot_nodes(*o, 5, &p, &q);  // between the loops
    CHECK(p == 0 && q == 0);
    bc::edge_slot_nodes(*o, 7, &p, &q);
    CHECK(p == 6 && q == 7);
    // no mesh: nothing
    at<std::int32_t>(*o, 0x87c) = 0;
    CHECK_EQ(bc::edge_face_slot_count(*o), 0);
    CHECK_EQ(bc::edge_slot(*o, 1, 2), -1);
}

TEST_CASE(face_lookup_modes)
{
    DB_STEP_DATA* m = new_model(3);
    LLOBJ_DATA* o = new_object(*m, 1);
    at<std::int32_t>(*o, 0x87c) = 1;
    at<std::int32_t>(*o, 0x880) = 8;
    at<std::int32_t>(*o, 0x884) = 1;
    bc::set_boundary_count(*o, 1);
    bc::set_boundary_face_count(*o, 2, 0);
    auto* F = static_cast<std::int32_t*>(bdry(*o, 0).u018);
    const std::int32_t quads[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    std::memcpy(F, quads, sizeof quads);
    CHECK_EQ(bc::edge_face_slot_count(*o), 2);
    CHECK_EQ(bc::face_slot(*o, 4, 3, 2, 1), 0);
    CHECK_EQ(bc::face_slot(*o, 8, 6, 7, 5), 1);
    CHECK_EQ(bc::face_slot(*o, 1, 2, 3, 5), -1);
    CHECK_EQ(bc::face_slot(*o, 1, 2, 3, 4), 0);
    // triangle mode from face 0 (a b c a): only n1..n3 are tested
    const std::int32_t tris[8] = {1, 2, 3, 1, 2, 3, 4, 2};
    std::memcpy(F, tris, sizeof tris);
    CHECK_EQ(bc::face_slot(*o, 3, 1, 2, 99), 0);
    CHECK_EQ(bc::face_slot(*o, 4, 3, 2, 0), 1);
    CHECK_EQ(bc::face_slot(*o, 4, 3, 9, 2), -1);
    std::int32_t a, b, c, d;
    bc::face_nodes(*o, 1, &a, &b, &c, &d);
    CHECK(a == 2 && b == 3 && c == 4 && d == 2);
    // more than 100 boundaries are cut, an out-of-range face count only prints
    bc::set_boundary_count(*o, 150);
    CHECK_EQ(at<std::int32_t>(*o, 0xbd8), 100);
}

TEST_CASE(int_reader_paths)
{
    DB_STEP_DATA* m = new_model(2);
    LLOBJ_DATA* o = object_2d(*m);
    auto* a = static_cast<std::int32_t*>(bc::edge_face_array(*o, 0));
    int st = -1;
    // default fill of all NS slots, records by edge in either order, rest of the line consumed
    std::string rest = read_ints(*o, " 2 7\n 1 2 1 3\n 9 6 5 4 junk\nNEXT\n", 0, &st);
    CHECK_EQ(st, 0);
    CHECK_EQ(rest, std::string("NEXT\n"));
    CHECK_EQ(a[0], 3);
    CHECK_EQ(a[1], 7);
    CHECK_EQ(a[4], 7);
    CHECK_EQ(a[6], 4);
    CHECK_EQ(a[8], 7);
    // one conversion: no fill; count < 1: nothing more is read
    rest = read_ints(*o, " 0\nNEXT\n", 0, &st);
    CHECK_EQ(st, 0);
    CHECK_EQ(rest, std::string("NEXT\n"));
    CHECK_EQ(a[1], 7);
    rest = read_ints(*o, " -1 5\nNEXT\n", 0, &st);
    CHECK_EQ(a[1], 5);
    CHECK_EQ(rest, std::string("NEXT\n"));
    // blank header, EOF in a record, EOF on the final fgets -> 1
    read_ints(*o, "\n", 0, &st);
    CHECK_EQ(st, 1);
    read_ints(*o, " 2\n 1 1 2 3\n", 0, &st);
    CHECK_EQ(st, 1);
    read_ints(*o, " 1\n 1 1 2 3", 0, &st);
    CHECK_EQ(st, 1);
    // unmatched edge: message on stdout, record dropped, status 0
    read_ints(*o, " 1\n 1 1 3 9\n", 0, &st);
    CHECK_EQ(st, 0);
    // the parser maps a nonzero status to 2
    keytest::InFile in("kw_g2bc_ec_case.txt", "\n");
    CHECK_EQ(g2bc::read_case(*m, in.get(), *o, 241), 2);
    CHECK(g2bc::handles(241) && g2bc::handles(252) && !g2bc::handles(240) && !g2bc::handles(253));
}

TEST_CASE(double_reader_and_free_format)
{
    DB_STEP_DATA* m = new_model(2);
    LLOBJ_DATA* o = object_2d(*m);
    auto* a = static_cast<double*>(bc::edge_face_array(*o, 2));
    keytest::InFile in("kw_g2bc_ec_d.txt", " 2 1.5\n 1 3\n 2 2.25E+1 7 7 6 -1\nNEXT\n");
    CHECK_EQ(read_edge_face_values(in.get(), a, o, 1), 0);
    CHECK_EQ(in.rest(), std::string("NEXT\n"));
    CHECK_BITS(a[0], 1.5);
    CHECK_BITS(a[1], 22.5);
    CHECK_BITS(a[7], -1.0);
    CHECK_BITS(a[8], 1.5);
}

TEST_CASE(writer_formats)
{
    keytest::legacy_printf();
    DB_STEP_DATA* m = new_model(2);
    LLOBJ_DATA* o = object_2d(*m);
    auto* a = static_cast<std::int32_t*>(bc::edge_face_array(*o, 0));
    auto* d = static_cast<double*>(bc::edge_face_array(*o, 2));
    g2bc::KeyRef ref{1};
    {
        keytest::OutFile out("kw_g2bc_ec_w1.txt");
        g2bc::write_keyword(241, out.get(), *m, ref);
        CHECK_EQ(out.text(), std::string("ECCDEF       1       0       0\n"));
    }
    // spare slots 4 and 5 hold a value but are not considered and not written; k counts them
    for (int i = 0; i < 9; ++i) a[i] = 2;
    a[1] = 0;
    d[4] = 1.0;
    d[7] = -2.5;
    {
        keytest::OutFile out("kw_g2bc_ec_w2.txt");
        g2bc::write_keyword(241, out.get(), *m, ref);
        g2bc::write_keyword(247, out.get(), *m, ref);
        CHECK_EQ(out.text(), std::string("ECCDEF       1       6       0\n"
                                         "       1       1       2       2\n"
                                         "       2       3       4       2\n"
                                         "       3       4       1       2\n"
                                         "       6       5       6       2\n"
                                         "       7       6       7       2\n"
                                         "       8       7       5       2\n"
                                         "ECPRES       1       1    0.0000000000E+000\n"
                                         "       2       6       7   -2.5000000000E+000\n"));
    }
    // obj+0x3f24 > 0 is the printed number; component count >= 2 writes nothing
    at<std::int32_t>(*o, 0x3f24) = 12;
    at<std::int32_t>(*m, 0x1700) = 2;
    {
        keytest::OutFile out("kw_g2bc_ec_w3.txt");
        g2bc::write_edge_face_bcs(*m, out.get(), *o, 99);
        std::string t = out.text();
        CHECK(t.rfind("ECCDEF      12       6       0\n", 0) == 0);
        CHECK(t.find("ECDEFN") == std::string::npos);
        CHECK(t.find("ECRFLX      12       0    0.0000000000E+000\n") != std::string::npos);
    }
    // no boundary: the block writes nothing
    at<std::int32_t>(*o, 0xbd8) = 0;
    {
        keytest::OutFile out("kw_g2bc_ec_w4.txt");
        g2bc::write_edge_face_bcs(*m, out.get(), *o, 1);
        CHECK_EQ(out.text(), std::string());
    }
}

TEST_CASE(crafted_oracle_cases)
{
    keytest::legacy_printf();
    for (const char* name : {"c2_ec", "c3_ec"}) {
        Result r;
        int errors = -1;
        std::string expected = slurp(data((std::string(name) + ".expected").c_str()));
        REQUIRE(!expected.empty());
        run_case(data((std::string(name) + ".key").c_str()), expected, r, true, &errors);
        int want = 0;
        for (auto& l : lines_of(slurp(data((std::string(name) + ".stdout").c_str()))))
            if (l == "ERROR in readEdgeFaceVarInt, iECCArrayAdr") ++want;
        std::printf("%s: %d blocks, %d exact, %d as set, %d bad, %d missing; edge errors %d (original %d)\n", name,
                    r.blocks, r.exact, r.set_only, r.bad, r.missing, errors, want);
        CHECK_EQ(r.bad, 0);
        CHECK_EQ(r.missing, 0);
        CHECK_EQ(errors, want);
        if (std::string(name) == "c2_ec") CHECK_EQ(r.exact, r.blocks);
    }
}

TEST_CASE(corpus_oracle_round_trips)
{
    keytest::legacy_printf();
    fs::path cache = env("DEFORM_ORACLE_CACHE", "");
    if (cache.empty()) cache = fs::path(env("USERPROFILE", "")) / ".deform-oracle";
    if (!fs::exists(cache / "entries")) {
        std::printf("no oracle cache at %s: skipped\n", cache.string().c_str());
        return;
    }
    const std::uintmax_t max_bytes = std::strtoull(env("KW_G2BC_EC_MAX_BYTES", "3000000").c_str(), nullptr, 10);
    const bool verbose = env("KW_G2BC_EC_VERBOSE", "0") == "1";
    Result r2, r3;
    int files = 0, skipped = 0, files_ok = 0;
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
            std::string text = slurp(src);
            bool has_ec = false;
            for (std::size_t p = text.find("\nEC"); p != std::string::npos; p = text.find("\nEC", p + 1)) {
                char name[8] = {0};
                std::memcpy(name, text.c_str() + p + 1, std::min<std::size_t>(6, text.size() - p - 1));
                if (is_ec_name(name)) {
                    has_ec = true;
                    break;
                }
            }
            if (!has_ec) continue;
            if (fs::file_size(src) > max_bytes) {
                ++skipped;
                continue;
            }
            const bool d3 = text.find("DEFORM-3D") != std::string::npos && text.find("DEFORM-3D") < 400;
            Result& r = d3 ? r3 : r2;
            Result before = r;
            if (verbose) {
                std::printf("CASE %s\n", src.string().c_str());
                std::fflush(stdout);
            }
            run_case(src, slurp(out), r, verbose);
            ++files;
            if (r.bad == before.bad && r.missing == before.missing) ++files_ok;
        }
    }
    std::printf("kfrw cases with EC keywords: %d checked (%d without mismatch), %d skipped (size)\n", files,
                files_ok, skipped);
    std::printf("  2D blocks %d: exact %d, bad %d, missing %d\n", r2.blocks, r2.exact, r2.bad, r2.missing);
    std::printf("  3D blocks %d: exact %d, same set %d, bad %d, missing %d\n", r3.blocks, r3.exact, r3.set_only,
                r3.bad, r3.missing);
    CHECK_EQ(r2.bad, 0);
    CHECK_EQ(r3.bad, 0);
}

int main() { return testkit::run_all(); }
