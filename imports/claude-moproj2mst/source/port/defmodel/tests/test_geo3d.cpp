// _OBJ_GEO3D (p1-07): construction, node/element arrays, transforms, bounding-box length,
// element reversal and the copy. Members of other packets are fakes that record their calls.
// ctest-labels: p1-07 geo
#include <cmath>
#include <cstring>
#include <cstdint>
#include <string>
#include <vector>

#include <defmodel/layout/obj_geo3d.hpp>
#include <testkit/check.hpp>

#include "geo_fakes.hpp"

using namespace def::model;

namespace {

std::vector<std::uint64_t> seq;

void* list_ctor(void* l) { seq.push_back(0x140257bf0); return l; }
void list_dtor(void*) { seq.push_back(0x140257c60); }
void list_clear(void*) { seq.push_back(0x1402581b0); }
void list_copy(void*, void*) { seq.push_back(0x140257c80); }
double list_rot_angle = 0.0;
void list_rotate(void*, const double*, const double*, double a) { seq.push_back(0x140258400); list_rot_angle = a; }
void** h50(void** h) { seq.push_back(0x1402fedd0); *h = nullptr; return h; }
void** h58(void** h) { seq.push_back(0x1402fef20); *h = nullptr; return h; }
void h50_dtor(void**) { seq.push_back(0x1402fee40); }
void h58_dtor(void**) { seq.push_back(0x1402fef90); }
void h50_clear(void**) { seq.push_back(0x1402ff680); }
void h58_clear(void**) { seq.push_back(0x1402ff790); }
void* tree_ctor(void* t) { seq.push_back(0x14042a7e0); return t; }
void tree_dtor(void*) { seq.push_back(0x14042a870); }
double mat_angle = 0.0;
void* rot_matrix(void* out, const double*, const double*, double* a)
{
    mat_angle = *a;
    // translation by (1, 0, 0) as the stand-in matrix
    double m[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1};
    std::memcpy(out, m, sizeof m);
    return out;
}
void mat_dtor(void*) { seq.push_back(0x140220560); }

void install()
{
    geofake::reset();
    seq.clear();
    pending::fun_140257bf0 = &list_ctor;
    pending::fun_140257c60 = &list_dtor;
    pending::fun_1402581b0 = &list_clear;
    pending::fun_140257c80 = &list_copy;
    pending::fun_140258400 = &list_rotate;
    pending::fun_1402fedd0 = &h50;
    pending::fun_1402fef20 = &h58;
    pending::fun_1402fee40 = &h50_dtor;
    pending::fun_1402fef90 = &h58_dtor;
    pending::fun_1402ff680 = &h50_clear;
    pending::fun_1402ff790 = &h58_clear;
    pending::fun_14042a7e0 = &tree_ctor;
    pending::fun_14042a870 = &tree_dtor;
    pending::fun_1402214c0 = &rot_matrix;
    pending::fun_140220560 = &mat_dtor;
}

void set_nodes(OBJ_GEO3D& g, std::vector<double> xyz)
{
    std::int32_t n = static_cast<std::int32_t>(xyz.size() / 3);
    double* p = g.resize_nodes(&n);
    for (std::size_t i = 0; i < xyz.size(); ++i) p[i] = xyz[i];
}

void set_elems(OBJ_GEO3D& g, std::vector<std::int32_t> e)
{
    std::int32_t n = static_cast<std::int32_t>(e.size() / 4);
    std::int32_t* p = g.resize_elems(&n);
    for (std::size_t i = 0; i < e.size(); ++i) p[i] = e[i];
}

std::vector<double> nodes(const OBJ_GEO3D& g) { return std::vector<double>(g.coords.begin(), g.coords.end()); }
std::vector<std::int32_t> elems(const OBJ_GEO3D& g) { return std::vector<std::int32_t>(g.elems.begin(), g.elems.end()); }

} // namespace

TEST_CASE(ctor_and_dtor_call_member_code_in_order)
{
    install();
    {
        OBJ_GEO3D g;
        CHECK(seq == (std::vector<std::uint64_t>{0x140257bf0, 0x1402fedd0, 0x1402fef20, 0x14042a7e0}));
        CHECK_EQ(g.node_count(), 0);
        CHECK_EQ(g.elem_count(), 0);
        CHECK(g.elem_flags == nullptr);
        CHECK_EQ(g.u48, 0);
        CHECK_EQ(g.u60, 0);
        CHECK(!g.u70);
        CHECK(g.u78 == nullptr);
        CHECK_EQ(g.is_extension(), 100);
        CHECK_EQ(std::string(g.class_name()), std::string("_OBJ_GEO3D"));
        CHECK_BITS(g.char_length(), 0.0); // no nodes
        seq.clear();
    }
    // clear_all (point list, h50, h58 clears), then tree, h58, h50, point list dtors
    CHECK(seq == (std::vector<std::uint64_t>{0x1402581b0, 0x1402ff680, 0x1402ff790, 0x14042a870,
                                              0x1402fef90, 0x1402fee40, 0x140257c60}));
}

TEST_CASE(resize_and_counts)
{
    install();
    OBJ_GEO3D g;
    std::int32_t zero = 0;
    CHECK(g.resize_nodes(&zero) == nullptr);
    CHECK(g.resize_elems(&zero) == nullptr);
    CHECK(g.alloc_elem_flags() == nullptr);
    CHECK(g.resize_elem_ids() == nullptr);
    set_nodes(g, {0, 0, 0, 1, 2, 2});
    set_elems(g, {1, 2, 2, 2});
    CHECK_EQ(g.node_count(), 2);
    CHECK_EQ(g.elem_count(), 1);
    std::int32_t* f = g.alloc_elem_flags();
    REQUIRE(f != nullptr);
    CHECK_EQ(f[0], 0);
    std::int32_t* ids = g.resize_elem_ids();
    REQUIRE(ids != nullptr);
    CHECK_EQ(ids[0], 0);
    CHECK_EQ(static_cast<int>(g.elem_ids.size()), 1);
}

TEST_CASE(char_length_scale_translate)
{
    install();
    OBJ_GEO3D g;
    set_nodes(g, {0, 0, 0, 1, 2, 2});
    CHECK_BITS(g.char_length(), 0.0); // no elements yet
    set_elems(g, {1, 2, 2, 2});
    CHECK_BITS(g.char_length(), 3.0); // sqrt(1 + 4 + 4)
    double f = 3.0;
    g.scale(&f);
    CHECK(nodes(g) == (std::vector<double>{0, 0, 0, 3, 6, 6}));
    double d[3] = {1, -1, 0.5};
    g.translate(d);
    CHECK(nodes(g) == (std::vector<double>{1, -1, 0.5, 4, 5, 6.5}));
}

TEST_CASE(reverse_elements)
{
    install();
    OBJ_GEO3D g;
    set_nodes(g, {0, 0, 0});
    // 1 2 3 4 -> 4 3 2 1; 5 6 7 -8 -> -8 7 6 5 -> |.| first, -|.| last: 8 7 6 -5
    set_elems(g, {1, 2, 3, 4, 5, 6, 7, -8});
    g.reverse();
    CHECK(elems(g) == (std::vector<std::int32_t>{4, 3, 2, 1, 8, 7, 6, -5}));
}

TEST_CASE(rotate_converts_degrees_for_the_matrix)
{
    install();
    OBJ_GEO3D g;
    set_nodes(g, {1, 2, 3, 4, 5, 6});
    double c[3] = {0, 0, 0};
    double axis[3] = {0, 0, 1};
    g.rotate(c, axis, 90.0);
    CHECK_BITS(list_rot_angle, 90.0);                          // the list gets degrees
    CHECK_BITS(mat_angle, (90.0 * 3.141592653589793) / 180.0); // (a * pi) / 180
    CHECK(nodes(g) == (std::vector<double>{2, 2, 3, 5, 5, 6}));  // stand-in matrix: +1 in x
    CHECK(seq.back() == 0x140220560);
}

TEST_CASE(assign_copies_arrays)
{
    install();
    OBJ_GEO3D a;
    set_nodes(a, {1, 2, 3, 4, 5, 6});
    set_elems(a, {1, 2, 2, 2, 2, 1, 1, 1});
    a.alloc_elem_flags()[1] = 7;
    a.resize_elem_ids()[0] = 42;
    OBJ_GEO3D b;
    seq.clear();
    b.copy_from(&a);
    CHECK(nodes(b) == nodes(a));
    CHECK(elems(b) == elems(a));
    REQUIRE(b.elem_flags != nullptr);
    CHECK_EQ(b.elem_flags[1], 7);
    CHECK_EQ(b.elem_ids[0], 42);
    CHECK(seq.back() == 0x140257c80);
    CHECK(b.coords.body() != a.coords.body()); // copied element by element, not shared
    b.reset();                                   // slot 2: arrays emptied, flags pointer dropped
    CHECK_EQ(b.node_count(), 0);
    CHECK(b.elem_flags == nullptr);
}

int main() { return testkit::run_all(); }
