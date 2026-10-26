// ctest-labels: p8-02
// p8-02 unit tests that run without the owners' code: the seams to the defCELL list (p67-2), the
// float point list and the token reader (p8-09) are replaced by small fakes with the owners'
// semantics (spec GFREAD_STL_GEO_NAS.md 2.2), so the STL reader, the locator and the NASTRAN
// readers of this packet are exercised on the case inputs of tests/commands/GFREAD/data.
// The full oracle replays are in test_p8_02_oracle.cpp (they need the owners merged).
#define _CRT_SECURE_NO_WARNINGS
#include <testkit/check.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>

#include "../src/p8_02/ext_p67.hpp"
#include "../src/p8_02/ext_p8_09.hpp"
#include "../src/p8_02/stl_nas_internal.hpp"

using namespace def::cmd::stlnas;

namespace {

struct Fatal : std::runtime_error {
    using std::runtime_error::runtime_error;
};

void c_init(CellList* c, i32 cap, i32 inc, i32 w)
{
    c->count = 0;
    c->cap = cap;
    c->width = w;
    c->width2 = w;
    c->inc = inc;
    c->data = cap > 0 ? static_cast<i32*>(std::calloc(static_cast<std::size_t>(cap * w), 4)) : nullptr;
}
void c_free(CellList* c)
{
    c->count = 0;
    c->cap = 0;
    std::free(c->data);
    c->data = nullptr;
}
i32 c_append(CellList* c, i32 n, const i32* r)
{
    if (c->count >= c->cap) {
        c->cap += c->inc;
        c->data = static_cast<i32*>(std::realloc(c->data, static_cast<std::size_t>(c->cap * c->width) * 4));
    }
    for (i32 i = 0; i < n; ++i) c->data[c->count * c->width + i] = r[i];
    ++c->count;
    return 0;
}
i32 c_count(CellList* c) { return c->count; }
i32* c_get(CellList* c, i32 k)
{
    if (k < 1 || c->count < k) throw Fatal("defCELL_GetCellData");
    return c->data + (k - 1) * c->width;
}
CellList* c_array(i32 n) { return n == 0 ? nullptr : static_cast<CellList*>(std::calloc(static_cast<std::size_t>(n), 0x20)); }
i32* c_realloc(i32* p, i32 n) { return n == 0 ? nullptr : static_cast<i32*>(std::realloc(p, static_cast<std::size_t>(n) * 4)); }
void c_fatal(const char* fmt, i32, i32) { throw Fatal(fmt); }

void p_init(PointList* l, i32 cap, i32 grow)
{
    std::memset(l, 0, sizeof *l);
    l->cap = cap;
    l->grow = grow;
    l->dirty = 1;
    if (cap > 0) l->data = static_cast<float*>(std::calloc(static_cast<std::size_t>(cap * 3), 4));
}
void p_free(PointList* l)
{
    std::free(l->data);
    l->data = nullptr;
    l->count = 0;
    l->cap = 0;
    l->dirty = 1;
}
i32 p_append(PointList* l, const float* p)
{
    if (l->count >= l->cap - 1) {
        l->cap += l->grow;
        l->data = static_cast<float*>(std::realloc(l->data, static_cast<std::size_t>(l->cap * 3) * 4));
    }
    std::memcpy(l->data + l->count * 3, p, 12);
    l->dirty = 1;
    return ++l->count;
}
i32 p_count(PointList* l) { return l->count; }
float* p_realloc(float* p, i32 n) { return n == 0 ? nullptr : static_cast<float*>(std::realloc(p, static_cast<std::size_t>(n) * 4)); }
void d_push(def::model::CowVector<double>* v, const double* x) { v->resize(v->size() + 1, *x); }

void install()
{
    namespace e = def::cmd::stlnas::ext;
    e::fun_1404050b0 = &c_init;
    e::fun_140405150 = &c_free;
    e::fun_140405540 = &c_append;
    e::fun_1404056c0 = &c_count;
    e::fun_140405720 = &c_get;
    e::fun_1404057a0 = &c_array;
    e::fun_140256e20 = &c_realloc;
    e::fun_14039d450 = &c_fatal;
    e::fun_140410fd0 = &p_init;
    e::fun_140411080 = &p_free;
    e::fun_1404115d0 = &p_append;
    e::fun_140411620 = &p_count;
    e::fun_1402570e0 = &p_realloc;
    e::fun_140256800 = &d_push;
}

void uninstall()
{
    def::cmd::stlnas::ext::reset_ext_p67();
    def::cmd::stlnas::ext::reset_ext_p8_09();
}

std::string data(const char* name)
{
    return (testkit::repo_root() / "tests" / "commands" / "GFREAD" / "data" / name).string();
}

}  // namespace

TEST_CASE(distance_is_float_left_to_right)
{
    const float a[3] = {1.0f, 2.0f, 3.0f};
    const float b[3] = {0.5f, 0.25f, 0.125f};
    float s = (a[0] - b[0]) * (a[0] - b[0]);
    s = s + (a[1] - b[1]) * (a[1] - b[1]);
    s = s + (a[2] - b[2]) * (a[2] - b[2]);
    CHECK_EQ(fun_140411880(a, b), s);
}

TEST_CASE(binary_stl_merge)
{
    install();
    StlObj s;
    fun_140244ea0(&s);
    const float tol = 1.0E-5f;
    const char check = 0;  // no DEF_STL.OUT (its checker is p8-09's)
    CHECK(fun_140244fa0(&s, data("stl_binary_merge.stl").c_str(), &tol, &check));
    CHECK_EQ(s.binary, 1);
    CHECK_EQ(s.nn, 10);  // the case's DIEGEO: 10 nodes, 6 facets
    CHECK_EQ(s.ne, 6);
    fun_140244f00(&s);
    uninstall();
}

TEST_CASE(ascii_detection)
{
    StlObj s;
    fun_140244ea0(&s);
    CHECK(fun_140245b90(&s, data("stl_ascii_merge.stl").c_str()));
    CHECK_EQ(s.binary, 0);
    CHECK(fun_140245b90(&s, data("stl_binary_merge.stl").c_str()));
    CHECK_EQ(s.binary, 1);
}

TEST_CASE(all_degenerate_is_fatal)
{
    install();
    StlObj s;
    fun_140244ea0(&s);
    const float tol = 1.0E-5f;
    const char check = 0;
    bool fatal = false;
    try {
        (void)fun_140244fa0(&s, data("stl_alldegen.stl").c_str(), &tol, &check);
    } catch (const Fatal& e) {
        fatal = std::string(e.what()).rfind("defCELL_GetCellData", 0) == 0;
    }
    CHECK(fatal);
    uninstall();
}

TEST_CASE(locator_neighbour_shell)
{
    install();
    Locator l;
    fun_14040ce80(&l, 1.0E-5f);
    const i32 idx[3] = {0, 5, 99};
    fun_14040dfd0(&l, idx, l.dims, 1, 0);
    // x in 0..1, y in 4..6, z in 98..99: 2*3*2 = 12 cells, the centre (0,5,99) is not on the shell
    CHECK_EQ(l.neigh.count, 11);
    fun_14040dfd0(&l, idx, l.dims, 0, 0);
    CHECK_EQ(l.neigh.count, 1);
    c_free(&l.neigh);
    uninstall();
}

TEST_CASE(nastran_fixed_tetra)
{
    install();
    alignas(NasObj) unsigned char raw[sizeof(NasObj)];
    NasObj* n = fun_14024f0a0(reinterpret_cast<NasObj*>(raw));
    const std::string f = data("nas_tet_fixed.nas");
    CHECK(!fun_14024fa50(n, f.c_str()));  // the first GRID line has no comma
    CHECK(fun_14024f6a0(n, f.c_str()));
    CHECK_EQ(n->nn, 5);
    CHECK_EQ(n->ne, 2);
    CHECK(testkit::same_bits(n->nodes[3], static_cast<double>(0.1f)));  // (float)atof, widened
    CHECK(testkit::same_bits(n->nodes[12], 1.0));                       // "1.0-3" is 1.0 for atof
    CHECK_EQ(n->conn[4], 2);
    CHECK_EQ(n->conn[7], 5);
    fun_14024f110(n);
    uninstall();
}

int main() { return testkit::run_all(); }
