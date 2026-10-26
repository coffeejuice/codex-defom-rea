// ctest-labels: p67-7 ctc3d
// Tier-1 tests of stages 1, 2 and 4 of the GENCTC 3D pipeline (14005c3a7 EI3, 140052f49 NI5,
// 14002cc15 EI2) on a hand-made slave of two quads sharing an edge, with hand-derived results.
//
// The cube attribute store (group "cube") is not ported yet on this branch, so this executable
// defines a test double of the five accessors the three stages use (140227e00, 1402280d0,
// 140228260, 140228690, 140228830) with the documented semantics: Init replaces a field's data
// and count, Get copies it and its count and leaves the buffer untouched (count 0) when the field
// is missing, 140227e00 returns the polygon's four node numbers and count 4. The linker takes
// these definitions from this object, so the library's stub object for the cube is not pulled.
// When the cube lands, replace the double by a real cube (140225640 + 140225ff0) and drop it.
#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/pipeline.hpp>

#include <testkit/check.hpp>

#include <array>
#include <map>
#include <tuple>
#include <vector>

namespace def::cmd::ctc3d {

namespace {

// key: (obj, inst, kind 0 = node / 1 = element, entity, field)
std::map<std::tuple<int, int, int, int, int>, std::vector<i32>> g_store;
std::vector<std::vector<i32>> g_faces; // 1-based polygons (index 0 unused)

void put(int kind, i32* obj, i32* inst, i32* ent, i32* field, void* buf, i32* count)
{
    const i32* b = static_cast<const i32*>(buf);
    g_store[{*obj, *inst, kind, *ent, *field}] = std::vector<i32>(b, b + *count);
}

void get(int kind, i32* obj, i32* inst, i32* ent, i32* field, void* buf, i32* count)
{
    auto it = g_store.find({*obj, *inst, kind, *ent, *field});
    if (it == g_store.end()) {
        *count = 0;
        return;
    }
    i32* b = static_cast<i32*>(buf);
    for (std::size_t i = 0; i < it->second.size(); ++i) {
        b[i] = it->second[i];
    }
    *count = static_cast<i32>(it->second.size());
}

} // namespace

void fun_140227e00(i32*, i32*, i32* poly, i32* conn, i32* count)
{
    for (int i = 0; i < 4; ++i) {
        conn[i] = g_faces[static_cast<std::size_t>(*poly)][static_cast<std::size_t>(i)];
    }
    *count = 4;
}
void fun_1402280d0(i32* o, i32* s, i32* e, i32* f, void* b, i32* c) { put(1, o, s, e, f, b, c); }
void fun_140228260(i32* o, i32* s, i32* e, i32* f, void* b, i32* c) { get(1, o, s, e, f, b, c); }
void fun_140228690(i32* o, i32* s, i32* e, i32* f, void* b, i32* c) { put(0, o, s, e, f, b, c); }
void fun_140228830(i32* o, i32* s, i32* e, i32* f, void* b, i32* c) { get(0, o, s, e, f, b, c); }

} // namespace def::cmd::ctc3d

using namespace def::cmd::ctc3d;

namespace {

i32 g_obj = 1, g_inst = 1;

// nodes 1..6:  4---5---6     quad A = (1, 2, 5, 4), quad B = (2, 3, 6, 5); edge k of a quad
//              |   |   |     runs from node k to node k+1; A's edge 2 (2-5) is B's edge 4 (5-2)
//              1---2---3
void setup(const std::vector<i32>& bcc_unused)
{
    (void)bcc_unused;
    reset_image();
    g_store.clear();
    g_faces = {{}, {1, 2, 5, 4}, {2, 3, 6, 5}};
    // EI1: neighbours per edge, then edge ids (unused here)
    std::vector<i32> ei1a = {0, 2, 0, 0, 1, 2, 3, 4};
    std::vector<i32> ei1b = {0, 0, 0, 1, 2, 5, 6, 7};
    i32 p1 = 1, p2 = 2, f1 = 1, f2 = 2, c8 = 8, c1 = 1;
    fun_1402280d0(&g_obj, &g_inst, &p1, &f1, ei1a.data(), &c8);
    fun_1402280d0(&g_obj, &g_inst, &p2, &f1, ei1b.data(), &c8);
    i32 one = 1;
    fun_1402280d0(&g_obj, &g_inst, &p1, &f2, &one, &c1);
    fun_1402280d0(&g_obj, &g_inst, &p2, &f2, &one, &c1);
    // polygon count of cube (1, 1)
    m<i32>(0x14076e588 + 4 * (1 * 3 + 1 * 300)) = 2;
}

std::vector<i32> field(int kind, i32 ent, i32 f)
{
    return g_store[{1, 1, kind, ent, f}];
}

// bcc(3, 6) from per-node triples
std::vector<i32> codes(std::initializer_list<std::array<i32, 3>> nodes)
{
    std::vector<i32> v;
    for (const auto& t : nodes) {
        v.insert(v.end(), t.begin(), t.end());
    }
    return v;
}

} // namespace

TEST_CASE(ei3_code_against_no_code)
{
    // A's nodes fixed in x, B's other nodes free: A gets code (1, 1), B none -> class 1
    auto bcc = codes({{1, 0, 0}, {1, 0, 0}, {0, 0, 0}, {1, 0, 0}, {1, 0, 0}, {0, 0, 0}});
    setup(bcc);
    const i32 nface = 2, ndof = 3;
    fun_14005c3a7(&g_obj, &g_inst, &nface, bcc.data(), &ndof);
    CHECK(field(1, 1, 3) == (std::vector<i32>{0, 1, 0, 0}));
    CHECK(field(1, 2, 3) == (std::vector<i32>{0, 0, 0, 1}));
}

TEST_CASE(ei3_same_code_same_direction)
{
    // every node fixed in x: same code and direction on both sides -> class 2
    auto bcc = codes({{1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0}});
    setup(bcc);
    const i32 nface = 2, ndof = 3;
    fun_14005c3a7(&g_obj, &g_inst, &nface, bcc.data(), &ndof);
    CHECK(field(1, 1, 3) == (std::vector<i32>{0, 2, 0, 0}));
    CHECK(field(1, 2, 3) == (std::vector<i32>{0, 0, 0, 2}));
}

TEST_CASE(ei3_fixed_in_other_direction_and_ni5)
{
    // A fixed in x, B fixed in y, the shared nodes 2 and 5 in both: code 1 on both sides but
    // directions 1 and 2 -> class 1; then NI5: the shared edge (EI3 == 1 on A's edge 2 and B's
    // edge 4) has the common codes (1, dir 1) and (1, dir 2), each adds w = 1 to both nodes, twice
    // over (once per polygon): 4, / 4 = 1
    auto bcc = codes({{1, 0, 0}, {1, 1, 0}, {0, 1, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}});
    setup(bcc);
    const i32 nface = 2, ndof = 3, n = 6;
    fun_14005c3a7(&g_obj, &g_inst, &nface, bcc.data(), &ndof);
    CHECK(field(1, 1, 3) == (std::vector<i32>{0, 1, 0, 0}));
    CHECK(field(1, 2, 3) == (std::vector<i32>{0, 0, 0, 1}));
    fun_140052f49(&g_obj, &g_inst, &n, bcc.data(), &ndof);
    const i32 want[7] = {0, 0, 1, 0, 0, 1, 0};
    for (i32 node = 1; node <= 6; ++node) {
        CHECK(field(0, node, 5) == (std::vector<i32>{want[node]}));
    }
}

TEST_CASE(ni5_class2_edges_copy_the_common_code)
{
    // every node fixed in x: EI3 == 2 on the shared edge; stage 2 then gives its two nodes the
    // common code's class 4 (positive code), all other nodes stay 0
    auto bcc = codes({{1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0}});
    setup(bcc);
    const i32 nface = 2, ndof = 3, n = 6;
    fun_14005c3a7(&g_obj, &g_inst, &nface, bcc.data(), &ndof);
    fun_140052f49(&g_obj, &g_inst, &n, bcc.data(), &ndof);
    const i32 want[7] = {0, 0, 4, 0, 0, 4, 0};
    for (i32 node = 1; node <= 6; ++node) {
        CHECK(field(0, node, 5) == (std::vector<i32>{want[node]}));
    }
}

TEST_CASE(ei2_common_positive_code_disables)
{
    auto bcc = codes({{1, 0, 0}, {1, 0, 0}, {0, 0, 0}, {1, 0, 0}, {1, 0, 0}, {0, 0, 0}});
    setup(bcc);
    const i32 nface = 2, ndof = 3;
    fun_14002cc15(&g_obj, &g_inst, &nface, bcc.data(), &ndof);
    CHECK(field(1, 1, 2) == (std::vector<i32>{0}));
    CHECK(field(1, 2, 2) == (std::vector<i32>{1}));
}

int main() { return testkit::run_all(); }
