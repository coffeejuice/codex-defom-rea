// ctest-labels: p8-09 geolib
// Packet p8-09 (geometry support library): golden vectors derived from the listings
// (deform-rea-work/out/p809/all_l.txt) for the token reader 140411ad0, the float point list
// (140410fd0 .. 140411620), the surface checker's setup and node links (14040e620, 14040fde0,
// 14040e810), the _DEF_AMGGEO readers (1402484c0, 1402478f0 and their pre-checks) on the GFREAD
// oracle inputs (tests/commands/GFREAD/data), and the small helpers. The defCELL lists are p67-2's
// (not on this branch): the test installs a minimal list with the same layout through the seam's
// hooks. The store paths into a model are checked by test_p8_09_gfread_oracle.cpp.
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include <process.h>

#include <new>

#include <defcmd/geolib.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/def_edge.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <testkit/check.hpp>

#include "../src/geolib/ext_p67.hpp"

namespace gl = def::cmd::geolib;
namespace fs = std::filesystem;

namespace {

fs::path tmp_file(const std::string& stem, const std::string& text)
{
    const fs::path p = fs::temp_directory_path() / ("p809_" + stem + "_" + std::to_string(_getpid()) + ".txt");
    std::ofstream f(p, std::ios::binary);
    f << text;
    return p;
}

fs::path data(const char* name)
{
    return testkit::repo_root() / "tests" / "commands" / "GFREAD" / "data" / name;
}

// ---- minimal defCELL list for the hooks (layout of gl::CellList; 1-based get) --------------
void t_init(gl::CellList* c, std::int32_t cap, std::int32_t inc, std::int32_t width)
{
    c->count = 0;
    c->cap = cap;
    c->width = width;
    c->width2 = width;
    c->inc = inc;
    c->data = cap > 0 ? static_cast<std::int32_t*>(std::calloc(static_cast<std::size_t>(cap * width), 4)) : nullptr;
}
void t_free(gl::CellList* c)
{
    std::free(c->data);
    c->data = nullptr;
    c->count = 0;
    c->cap = 0;
}
std::int32_t t_append(gl::CellList* c, std::int32_t, const std::int32_t* rec)
{
    if (c->count >= c->cap) {
        c->cap += c->inc;
        c->data = static_cast<std::int32_t*>(std::realloc(c->data, static_cast<std::size_t>(c->cap * c->width) * 4));
    }
    std::memcpy(c->data + static_cast<std::ptrdiff_t>(c->count) * c->width, rec, static_cast<std::size_t>(c->width) * 4);
    return ++c->count;
}
std::int32_t t_count(const gl::CellList* c) { return c->count; }
std::int32_t t_width(const gl::CellList* c) { return c->width; }
std::int32_t* t_get(gl::CellList* c, std::int32_t k) { return c->data + static_cast<std::ptrdiff_t>(k - 1) * c->width; }
gl::CellList* t_array(std::int32_t n) { return static_cast<gl::CellList*>(std::calloc(static_cast<std::size_t>(n), sizeof(gl::CellList))); }

void install_cell_hooks()
{
    auto& h = gl::ext::g_hooks;
    h.cell_init = t_init;
    h.cell_free = t_free;
    h.cell_append = t_append;
    h.cell_count = t_count;
    h.cell_width = t_width;
    h.cell_get = t_get;
    h.cell_array = t_array;
}

} // namespace

// ---------------------------------------------------------------------------------------------
TEST_CASE(token_reader_lines_comments_and_types)
{
    gl::reset_token_reader();
    const fs::path p = tmp_file("tok", "* skipped\n\n  7, 8 : 9 = 10 // gone\nabc 1.5D2 2.5d-1\n-3 4294967295\n");
    std::FILE* f = std::fopen(p.string().c_str(), "r");
    REQUIRE(f != nullptr);
    gl::TokenReader r;
    gl::fun_140411920(&r);
    CHECK_EQ(r.fixed63, 0);
    std::int32_t i = 0;
    // new line: the '*' line and the blank line are skipped; separators " ,:=\n\t\r"; "//" cuts
    CHECK_EQ(gl::fun_140411ad0(&r, f, gl::tok_int, &i, gl::tok_new_line), 4);
    CHECK_EQ(i, 7);
    CHECK_EQ(gl::fun_140411ad0(&r, f, gl::tok_int, &i, 0), 0);
    CHECK_EQ(i, 8);
    CHECK_EQ(gl::fun_140411ad0(&r, f, gl::tok_int6, &i, 4), 0);  // token k of the current line
    CHECK_EQ(i, 10);
    CHECK_EQ(gl::fun_140411ad0(&r, f, gl::tok_int, &i, 5), -1);  // no token 5
    CHECK_EQ(gl::fun_140411ad0(&r, f, gl::tok_int, &i, 0), 0);   // next token is still 3 (9)
    CHECK_EQ(i, 9);
    CHECK_EQ(gl::fun_140411ad0(&r, f, gl::tok_int, &i, 0), 0);
    CHECK_EQ(i, 10);
    char s[64] = {0};
    // line used up: pos 0 reads the next line
    CHECK_EQ(gl::fun_140411ad0(&r, f, gl::tok_upper, s, 0), 0);
    CHECK(std::strcmp(s, "ABC") == 0);
    double d = 0;
    CHECK_EQ(gl::fun_140411ad0(&r, f, gl::tok_double, &d, 0), 0);
    CHECK_BITS(d, 150.0);  // D -> E
    float fl = 0;
    CHECK_EQ(gl::fun_140411ad0(&r, f, gl::tok_float, &fl, 0), 0);
    CHECK(fl == static_cast<float>(0.25));
    CHECK_EQ(gl::fun_140411ad0(&r, f, gl::tok_int, &i, gl::tok_new_line), 2);
    CHECK_EQ(i, -3);
    std::uint32_t u = 0;
    CHECK_EQ(gl::fun_140411ad0(&r, f, gl::tok_uint, &u, 0), 0);
    CHECK_EQ(u, 4294967295u);
    // EOF: -2; type 2 stores "DONE"
    std::strcpy(s, "x");
    CHECK_EQ(gl::fun_140411ad0(&r, f, gl::tok_upper, s, gl::tok_new_line), -2);
    CHECK(std::strcmp(s, "DONE") == 0);
    std::strcpy(s, "x");
    CHECK_EQ(gl::fun_140411ad0(&r, f, gl::tok_string, s, gl::tok_new_line), -2);
    CHECK(std::strcmp(s, "x") == 0);  // other types leave out untouched
    std::fclose(f);
    gl::fun_140411940(&r);
    fs::remove(p);
}

TEST_CASE(token_reader_whole_line_and_raw)
{
    gl::reset_token_reader();
    const fs::path p = tmp_file("tokline", "a b // c\r\n* star line\nlast\n");
    std::FILE* f = std::fopen(p.string().c_str(), "rb");
    REQUIRE(f != nullptr);
    gl::TokenReader r;
    gl::fun_140411920(&r);
    char s[128] = {0};
    // type 12: the next physical line, cut at \r \n only (no comment, no '*' skip)
    CHECK_EQ(gl::fun_140411ad0(&r, f, gl::tok_line, s, gl::tok_new_line), 1);
    CHECK(std::strcmp(s, "a b // c") == 0);
    CHECK_EQ(gl::fun_140411ad0(&r, f, gl::tok_line, s, gl::tok_new_line), 1);
    CHECK(std::strcmp(s, "* star line") == 0);
    // type 4 on a new line: strtok "\n\r" only
    CHECK_EQ(gl::fun_140411ad0(&r, f, gl::tok_raw, s, gl::tok_new_line), 1);
    CHECK(std::strcmp(s, "last") == 0);
    std::strcpy(s, "x");
    CHECK_EQ(gl::fun_140411ad0(&r, f, gl::tok_line, s, gl::tok_new_line), -2);
    CHECK(std::strcmp(s, "DONE") == 0);
    std::fclose(f);
    fs::remove(p);
}

TEST_CASE(token_helpers)
{
    gl::TokenReader r;
    char a[] = "1.0D0 2d3 D";
    gl::fun_140411970(&r, a, 'D', 'E', 0);  // both cases
    CHECK(std::strcmp(a, "1.0E0 2E3 E") == 0);
    char b[] = "dDd";
    gl::fun_140411970(&r, b, 'D', 'X', 1);  // exact case only
    CHECK(std::strcmp(b, "dXd") == 0);
    char c[] = "abC1";
    gl::fun_140411a60(&r, c);
    CHECK(std::strcmp(c, "ABC1") == 0);
}

// ---------------------------------------------------------------------------------------------
TEST_CASE(point_list_growth_and_ids)
{
    gl::FltPointList l;
    std::memset(&l, 0x5a, sizeof l);
    gl::fun_140410fd0(&l, 3, 2);
    CHECK_EQ(l.count, 0);
    CHECK_EQ(l.cap, 3);
    CHECK_EQ(l.grow, 2);
    CHECK_EQ(l.dirty, 1);
    for (float b : l.bounds) CHECK(b == 0.0f);
    REQUIRE(l.data != nullptr);
    // append grows when count >= cap - 1: ids 1, 2 fit cap 3; id 3 grows to 5; id 5 grows to 7
    const float p4[3] = {4.0f, 4.5f, 5.0f};
    CHECK_EQ(gl::fun_1404113b0(&l, 1.0f, 2.0f, 3.0f), 1);
    CHECK_EQ(l.cap, 3);
    CHECK_EQ(gl::fun_1404113b0(&l, 2.0f, 2.0f, 2.0f), 2);
    CHECK_EQ(l.cap, 3);
    CHECK_EQ(gl::fun_1404113b0(&l, 3.0f, 3.0f, 3.0f), 3);
    CHECK_EQ(l.cap, 5);
    CHECK_EQ(gl::fun_1404115d0(&l, p4), 4);
    CHECK_EQ(l.cap, 5);
    CHECK_EQ(gl::fun_1404113b0(&l, 5.0f, 6.0f, 7.0f), 5);
    CHECK_EQ(l.cap, 7);
    CHECK_EQ(gl::fun_140411620(&l), 5);
    CHECK(l.data[9] == 4.0f && l.data[10] == 4.5f && l.data[11] == 5.0f);
    CHECK(l.data[12] == 5.0f && l.data[13] == 6.0f && l.data[14] == 7.0f);
    l.dirty = 0;
    gl::fun_140411080(&l);
    CHECK_EQ(l.count, 0);
    CHECK_EQ(l.cap, 0);
    CHECK_EQ(l.grow, 100);
    CHECK_EQ(l.dirty, 1);
    CHECK(l.data == nullptr);
    // cap 0: no buffer until the first append (grow by `grow`)
    gl::fun_140410fd0(&l, 0, 100);
    CHECK(l.data == nullptr);
    CHECK_EQ(gl::fun_1404113b0(&l, 1.0f, 1.0f, 1.0f), 1);
    CHECK_EQ(l.cap, 100);
    gl::fun_140411080(&l);
}

TEST_CASE(float_allocs)
{
    CHECK(gl::fun_140257080(0) == nullptr);
    float* p = gl::fun_140257080(4);
    REQUIRE(p != nullptr);
    for (int i = 0; i < 4; ++i) CHECK(p[i] == 0.0f);
    float* q = gl::fun_1402570e0(p, 8);
    REQUIRE(q != nullptr);
    std::free(q);
    CHECK(gl::fun_1402570e0(nullptr, 0) == nullptr);
    double* d = gl::fun_1402571b0(nullptr, 3);
    REQUIRE(d != nullptr);
    std::free(d);
    CHECK(gl::fun_1402571b0(nullptr, 0) == nullptr);
}

TEST_CASE(surface_checker_setup_and_links)
{
    install_cell_hooks();
    gl::FltPointList pts;
    gl::fun_140410fd0(&pts, 10, 10);
    for (int i = 0; i < 5; ++i) gl::fun_1404113b0(&pts, static_cast<float>(i), 0.0f, 0.0f);
    gl::CellList cells;
    t_init(&cells, 4, 4, 3);
    const std::int32_t c1[3] = {1, 2, 3}, c2[3] = {1, 3, 4}, c3[3] = {4, 3, 5};
    t_append(&cells, 3, c1);
    t_append(&cells, 3, c2);
    t_append(&cells, 3, c3);
    gl::SurfCheck s;
    std::memset(&s, 0x77, sizeof s);
    gl::fun_14040e620(&s, &pts, &cells, 1.0E-9f);
    CHECK(s.pts == &pts);
    CHECK(s.cells == &cells);
    CHECK(s.links == nullptr && s.patches == nullptr && s.ua0 == nullptr);
    CHECK_EQ(s.nlinks, 0);
    CHECK_EQ(s.npatch, 0);
    CHECK_EQ(s.depth_limit, 1000);
    CHECK(s.small_area == 1.0E-9f);
    CHECK_EQ(s.list20.inc, 10);
    CHECK_EQ(s.list40.inc, 10);
    CHECK_EQ(s.list60.inc, 5);
    CHECK_EQ(s.list80.inc, 10);
    CHECK_EQ(s.list20.width, 1);
    for (int i = 0; i < 5; ++i) {
        CHECK_EQ(s.ue0[i], 0);
        CHECK_EQ(s.u108[i], 0);
    }
    CHECK_EQ(s.ud0, 0x77777777);  // not written by the ctor
    gl::fun_14040fde0(&s);
    REQUIRE(s.links != nullptr);
    CHECK_EQ(s.nlinks, 5);
    const std::vector<std::vector<std::int32_t>> want = {{1, 2}, {1}, {1, 2, 3}, {2, 3}, {3}};
    for (int v = 0; v < 5; ++v) {
        CHECK_EQ(s.links[v].count, static_cast<std::int32_t>(want[v].size()));
        CHECK_EQ(s.links[v].inc, 2);
        for (std::size_t k = 0; k < want[v].size(); ++k) CHECK_EQ(s.links[v].data[k], want[v][k]);
    }
    gl::fun_14040e810(&s);
    CHECK(s.links == nullptr && s.pts == nullptr && s.cells == nullptr);
    t_free(&cells);
    gl::fun_140411080(&pts);
    gl::ext::g_hooks = {};
}

// ---------------------------------------------------------------------------------------------
TEST_CASE(amggeo_ctor_and_prechecks)
{
    alignas(8) unsigned char buf[sizeof(gl::DEF_AMGGEO)];
    std::memset(buf, 0xcc, sizeof buf);
    gl::DEF_AMGGEO* g = gl::fun_140246d00(reinterpret_cast<gl::DEF_AMGGEO*>(buf));
    CHECK_EQ(g->curve_type, 1);
    CHECK_EQ(g->curve_count, 0);
    CHECK(g->curve_start == nullptr && g->curve_code == nullptr && g->points == nullptr && g->facets == nullptr);
    CHECK_EQ(g->npoint, 0);
    CHECK_EQ(g->nfacet, 0);
    gl::fun_140246d40(g);
    CHECK(gl::fun_1402488a0(data("geo3d_small.geo").string().c_str()));
    CHECK(!gl::fun_1402488a0(data("geo3d_badcheck.geo").string().c_str()));  // 3 tokens on line 2
    CHECK(!gl::fun_1402488a0(data("geo2d_multi.geo").string().c_str()));
    CHECK(gl::fun_140248390(data("geo2d_multi.geo").string().c_str()));
    CHECK(!gl::fun_140248390(data("geo3d_small.geo").string().c_str()));
    CHECK(!gl::fun_140248390("p809_no_such_file.geo"));
}

TEST_CASE(amggeo_3d_reader)
{
    gl::DEF_AMGGEO g;
    // mode 3 (the GFREAD 3D table and the first 2D pass): the 3D reader
    CHECK(gl::fun_140246f70(&g, data("geo3d_small.geo").string().c_str(), "", 3, nullptr));
    CHECK_EQ(g.npoint, 6);
    CHECK_EQ(g.nfacet, 5);
    const double nodes[18] = {0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0.1, 0, 1};
    for (int i = 0; i < 18; ++i) CHECK_BITS(g.points[i], nodes[i]);
    const std::int32_t facets[20] = {1, 2, 3, 4, 1, 3, 2, 0, 1, 4, 3, 0, 2, 6, 5, 0, 1, 2, 6, 1};
    for (int i = 0; i < 20; ++i) CHECK_EQ(g.facets[i], facets[i]);
    // the 2D store needs the curve starts, which only the 2D reader allocates
    CHECK(!gl::fun_14024bac0(&g, nullptr, 2));
    gl::DEF_AMGGEO h;
    CHECK(!gl::fun_1402484c0(&h, data("geo3d_badcheck.geo").string().c_str()));
    CHECK(!gl::fun_1402484c0(&h, data("geo3d_noelem.geo").string().c_str()));
    CHECK_EQ(h.npoint, 3);  // nodes read, ne = 0 -> fail, no facets
    CHECK(h.facets == nullptr);
}

TEST_CASE(amggeo_2d_reader_multi)
{
    gl::reset_token_reader();
    gl::DEF_AMGGEO g;
    const char close = 0;
    // obj null: the store fails (returns 0), the curve fields stay as read
    CHECK(!gl::fun_140246f70(&g, data("geo2d_multi.geo").string().c_str(), &close, 4, nullptr));
    CHECK_EQ(g.curve_count, 2);
    CHECK_EQ(g.curve_type, 1);  // file type 2 -> curve type 1
    CHECK_EQ(g.curve_start[0], 1);
    CHECK_EQ(g.curve_start[1], 4);
    CHECK_EQ(g.curve_code[0], 7);
    CHECK_EQ(g.curve_code[1], 4);  // the code -1 curve is dropped, the type 1 record skipped
    CHECK_EQ(g.npoint, 5);
    const double pts[15] = {3, 5, 0, 2, 5, 0.1, 2, 1, 0.2, 1.5, 2.5, 3.5, 1, 2, 3};
    for (int i = 0; i < 15; ++i) CHECK_BITS(g.points[i], pts[i]);
    // a 3D-format file fails the 2D pre-check
    gl::DEF_AMGGEO h;
    CHECK(!gl::fun_1402478f0(&h, data("geo3d_small.geo").string().c_str(), &close, 4, nullptr));
    CHECK(h.curve_start == nullptr);
}

TEST_CASE(amggeo_2d_reader_topo_and_type3)
{
    gl::reset_token_reader();
    gl::DEF_AMGGEO g;
    const char close = 0;
    CHECK(!gl::fun_1402478f0(&g, data("geo2d_topo.geo").string().c_str(), &close, 4, nullptr));
    CHECK_EQ(g.curve_count, 2);
    CHECK_EQ(g.curve_start[0], 1);
    CHECK_EQ(g.curve_start[1], 3);
    CHECK_EQ(g.curve_code[0], 5);
    CHECK_EQ(g.curve_code[1], 6);
    CHECK_EQ(g.npoint, 4);
    CHECK_BITS(g.points[9], 3.0);
    CHECK_BITS(g.points[10], 5.0);

    // file type 3: 6 values per point, the last in degrees -> radians; curve type 2
    gl::reset_token_reader();
    const fs::path p = tmp_file("t3", "3\n2 1\n1 1 2 3 4 5 90\n2 6 5 4 3 2 180\n");
    gl::DEF_AMGGEO h;
    CHECK(!gl::fun_1402478f0(&h, p.string().c_str(), &close, 4, nullptr));
    CHECK_EQ(h.curve_type, 2);
    CHECK_EQ(h.npoint, 2);
    CHECK_BITS(h.points[4], 5.0);
    CHECK_BITS(h.points[5], 90.0 * 0.017453292519943295);
    CHECK_BITS(h.points[11], 180.0 * 0.017453292519943295);
    fs::remove(p);

    // file type 1: x, y and z = 0; the close flag adds a copy of the first point
    gl::reset_token_reader();
    const fs::path q = tmp_file("t1", "1\n3 0\n1 1 2\n2 3 4\n3 5 6\n");
    gl::DEF_AMGGEO k;
    const char closed = 1;
    CHECK(!gl::fun_1402478f0(&k, q.string().c_str(), &closed, 4, nullptr));
    CHECK_EQ(k.curve_type, 1);
    CHECK_EQ(k.npoint, 4);
    CHECK_BITS(k.points[9], 1.0);
    CHECK_BITS(k.points[10], 2.0);
    CHECK_BITS(k.points[11], 0.0);
    fs::remove(q);
}

TEST_CASE(small_helpers)
{
    const double a[3] = {1.5, -2.0, 0.25}, b[3] = {2.0, 3.0, 8.0};
    CHECK_BITS(gl::fun_14023a380(a, b), (1.5 * 2.0 + -2.0 * 3.0) + 0.25 * 8.0);
    std::int32_t curve[8] = {0};
    gl::fun_1402567e0(curve, 42);
    CHECK_EQ(curve[3], 42);
}

TEST_CASE(model_helpers_geometry_mesh_groups)
{
    using def::model::DB_STEP_DATA;
    using def::model::LLOBJ_DATA;
    namespace gk = def::model::geo_kw;
    auto at32 = [](LLOBJ_DATA* o, std::size_t off) -> std::int32_t& {
        return *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::byte*>(o) + off);
    };
    DB_STEP_DATA* m = new DB_STEP_DATA(2);
    LLOBJ_DATA* o = def::model::llobj_ctor::get_or_create(m, 1);
    REQUIRE(o != nullptr);
    // 140198270: geometry 0 (created when missing) gets one more curve; 140198230 sets the count
    gl::fun_140198270(o);
    def::model::GEO2D_AND_MESH2D* g = gk::geometry(*o, 0);
    REQUIRE(g != nullptr);
    const std::int32_t n1 = static_cast<std::int32_t>(g->edges.size());
    gl::fun_140198270(o);
    CHECK_EQ(static_cast<std::int32_t>(g->edges.size()), n1 + 1);
    gl::fun_140198230(o, n1 + 3);
    CHECK_EQ(static_cast<std::int32_t>(g->edges.size()), n1 + 3);
    CHECK_EQ(static_cast<int>(g->ub8), 1);
    // 140202700: dim 0 -> the model's; counts; no material group yet
    gl::fun_1402029e0(o, 0);
    gl::fun_140202700(o, 6, 3, 1, m->u15f8, 0);
    CHECK_EQ(at32(o, 0x39c0), 2);
    CHECK_EQ(at32(o, 0x87c), 1);
    CHECK_EQ(at32(o, 0x880), 6);
    CHECK_EQ(at32(o, 0x884), 3);
    std::int32_t* grp = *reinterpret_cast<std::int32_t**>(reinterpret_cast<std::byte*>(o) + 0x9f8);
    REQUIRE(grp != nullptr);
    // 1402027e0: all element groups, then only those equal to the old group
    gl::fun_1402027e0(o, 7, false, true);
    CHECK_EQ(at32(o, 0x3980), 7);
    for (int i = 0; i < 3; ++i) CHECK_EQ(grp[i], 7);
    grp[1] = 3;
    gl::fun_1402027e0(o, 9, true, true);
    CHECK_EQ(grp[0], 9);
    CHECK_EQ(grp[1], 3);
    CHECK_EQ(grp[2], 9);
    gl::fun_1402027e0(o, 4, false, false);  // apply false: the object's group only
    CHECK_EQ(at32(o, 0x3980), 4);
    CHECK_EQ(grp[0], 9);
    // 1401ffd80: the mesh is gone
    gl::fun_1401ffd80(o);
    CHECK_EQ(at32(o, 0x87c), 0);
    CHECK_EQ(at32(o, 0x880), 0);
    CHECK_EQ(at32(o, 0x884), 0);
    CHECK_EQ(at32(o, 0x88c), 0);
    // 140158fc0 with flags 1: dtor, then free
    CHECK(gl::fun_140158fc0(m, 1) == m);
}

TEST_CASE(edge_count_shrink)
{
    using def::model::DB_STEP_DATA;
    using def::model::LLOBJ_DATA;
    namespace gk = def::model::geo_kw;
    DB_STEP_DATA* m = new DB_STEP_DATA(2);
    LLOBJ_DATA* o = def::model::llobj_ctor::get_or_create(m, 1);
    REQUIRE(o != nullptr);
    gl::fun_140198270(o);
    def::model::GEO2D_AND_MESH2D* g = gk::geometry(*o, 0);
    REQUIRE(g != nullptr);
    gl::fun_140198230(o, 5);
    REQUIRE(static_cast<int>(g->edges.size()) == 5);
    def::model::DEF_EDGE* e0 = g->edge_at(0);
    def::model::DEF_EDGE* e1 = g->edge_at(1);
    def::model::OBJ_GEO2D* c0 = g->edge_curve(0);
    def::model::OBJ_GEO2D* c1 = g->edge_curve(1);
    // 14027fe20 -> 140280290 with indexes 2, 3, 4: the first two edges and their curves stay
    gl::fun_140198230(o, 2);
    CHECK_EQ(static_cast<int>(g->edges.size()), 2);
    CHECK(g->edge_at(0) == e0);
    CHECK(g->edge_at(1) == e1);
    CHECK(g->edge_curve(0) == c0);
    CHECK(g->edge_curve(1) == c1);
    CHECK_EQ(g->edge_at(0)->get_id(), 1);
    CHECK_EQ(g->edge_at(1)->get_id(), 2);
    CHECK_EQ(static_cast<int>(g->ub8), 1);
    // remove the first edge directly: the former second edge is renumbered 1
    const std::int32_t idx[1] = {0};
    const std::int32_t one = 1;
    CHECK(gl::fun_140280290(g, idx, &one));
    CHECK_EQ(static_cast<int>(g->edges.size()), 1);
    CHECK(g->edge_at(0) == e1);
    CHECK_EQ(g->edge_at(0)->get_id(), 1);
    // the GFREAD call: n = 1 keeps one edge
    gl::fun_140198230(o, 4);
    gl::fun_140198230(o, 1);
    CHECK_EQ(static_cast<int>(g->edges.size()), 1);
    CHECK(g->edge_at(0) == e1);
}

TEST_CASE(amggeo_deleting_dtor_and_patch_free)
{
    install_cell_hooks();
    auto* g = static_cast<gl::DEF_AMGGEO*>(::operator new(sizeof(gl::DEF_AMGGEO)));
    gl::fun_140246d00(g);
    g->points = static_cast<double*>(std::calloc(3, 8));
    CHECK(gl::fun_1402566d0(g, 1) == g);  // dtor frees the arrays, then the object
    gl::DEF_AMGGEO h;
    CHECK(gl::fun_1402566d0(&h, 0) == &h);  // flags 0: dtor only
    new (&h) gl::DEF_AMGGEO();
    alignas(8) std::byte patch[0x88] = {};
    auto* lists = reinterpret_cast<gl::CellList*>(patch);
    for (int i = 0; i < 4; ++i) {
        t_init(&lists[i], 2, 2, 1);
        const std::int32_t v = i;
        t_append(&lists[i], 1, &v);
    }
    gl::fun_140459d00(patch);
    for (int i = 0; i < 4; ++i) {
        CHECK(lists[i].data == nullptr);
        CHECK_EQ(lists[i].count, 0);
    }
    gl::ext::g_hooks = {};
}

int main() { return testkit::run_all(); }
