// ctest-labels: kw-g2obj llobj
// Tier-1 test of the LLOBJ_DATA copy 1401613a0 with no llobj::ext test hook installed: every call
// into another packet goes to the owner's port through the default dispatch (src/llobj/ext.cpp),
// as in the program (FUN_1401d0fc0's object backup, OBJPOS). The objects are built with the real
// model and object ctors (DB_STEP_DATA(dim), 14019ccc0).
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/linked/axis_point_list.hpp>
#include <defmodel/llobj/ext.hpp>
#include <defmodel/llobj_ctor/llobj_copy.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defmodel/bc/window_bc.hpp>
#include <defmodel/window/llwindef_data.hpp>
#include <testkit/check.hpp>

#include <cstddef>
#include <cstdint>

using namespace def::model;

namespace {

void copy(DB_STEP_DATA& m, LLOBJ_DATA* dst, LLOBJ_DATA* src)
{
    const bool t = true, f = false;
    llobj_copy_from(dst, src, m.u15f8, &t, &t, &t, &t, &f, &t);  // flags as FUN_1401d0fc0 passes them
}

} // namespace

TEST_CASE(copy_2d_object_with_geometry_list_runs_the_owners)
{
    llobj::ext::g_hook = nullptr;
    DB_STEP_DATA m(2);
    LLOBJ_DATA* src = llobj_ctor::get_or_create(&m, 1);
    LLOBJ_DATA* dst = llobj_ctor::get_or_create(&m, 2);
    REQUIRE(src != nullptr);
    REQUIRE(dst != nullptr);
    geo_kw::ensure_geo_list(*src);
    geo_kw::ensure_geo_list(*dst);
    geo_kw::geo_list_append_default(geo_kw::geo_list(*src));
    geo_kw::geo_list_append_default(geo_kw::geo_list(*src));
    const std::int32_t n = geo_kw::geo_list_count(geo_kw::geo_list(*src));
    copy(m, dst, src);
    CHECK_EQ(geo_kw::geo_list_count(geo_kw::geo_list(*dst)), n);
    CHECK_EQ(geo_kw::geo_list_count(geo_kw::geo_list(*src)), n);
}

TEST_CASE(copy_3d_object_runs_the_geo3d_owners)
{
    llobj::ext::g_hook = nullptr;
    DB_STEP_DATA m(3);
    LLOBJ_DATA* src = llobj_ctor::get_or_create(&m, 1);
    LLOBJ_DATA* dst = llobj_ctor::get_or_create(&m, 2);
    REQUIRE(src != nullptr);
    REQUIRE(dst != nullptr);
    std::int32_t* id = reinterpret_cast<std::int32_t*>(reinterpret_cast<char*>(dst) + 0x8);
    const std::int32_t before = *id;
    copy(m, dst, src);
    CHECK_EQ(*id, before);  // p8 false: +0x8 is not copied
}

// A populated surface (nodes, elements, flags, ids, point list) in slot 0 and slot 7 of a 3D
// object goes through 1401618b0 -> slot 4 (1402f9dc0) -> 1402f9b60 into dst; dst's own old
// surface data is replaced, and src is left unchanged.
TEST_CASE(copy_3d_object_with_populated_surfaces)
{
    llobj::ext::g_hook = nullptr;
    DB_STEP_DATA m(3);
    LLOBJ_DATA* src = llobj_ctor::get_or_create(&m, 1);
    LLOBJ_DATA* dst = llobj_ctor::get_or_create(&m, 2);
    REQUIRE(src != nullptr);
    REQUIRE(dst != nullptr);
    auto surf = [](LLOBJ_DATA* o, int i) { return static_cast<OBJ_GEO3D*>(o->u3c00[i]); };
    REQUIRE(surf(src, 0) != nullptr);
    REQUIRE(surf(dst, 0) != nullptr);

    auto fill = [](OBJ_GEO3D* s, std::int32_t nn, std::int32_t ne, double base) {
        double* x = s->resize_nodes(&nn);
        for (std::int32_t k = 0; k < nn * 3; ++k) x[k] = base + 0.5 * k;
        std::int32_t* e = s->resize_elems(&ne);
        for (std::int32_t k = 0; k < ne * 4; ++k) e[k] = 1 + (k % nn);
        std::int32_t* f = s->alloc_elem_flags();
        for (std::int32_t k = 0; k < ne; ++k) f[k] = 10 + k;
        std::int32_t* ids = s->resize_elem_ids();
        for (std::int32_t k = 0; k < ne; ++k) ids[k] = 100 + k;
    };
    fill(surf(src, 0), 5, 3, 1.0);
    fill(surf(src, 7), 4, 2, -2.0);
    reinterpret_cast<linked::Cls_140257340List*>(surf(src, 0)->u28)->add(41);
    reinterpret_cast<linked::Cls_140257340List*>(surf(src, 0)->u28)->add(42);
    fill(surf(dst, 0), 9, 6, 50.0);  // stale data that the copy must replace
    src->u3990 = 1;
    llobj_copy::set_u3994(src, 2);

    copy(m, dst, src);

    CHECK_EQ(static_cast<int>(dst->u3990), 1);
    CHECK_EQ(llobj_copy::get_u3994(dst), 2);
    for (int i : {0, 7}) {
        OBJ_GEO3D* a = surf(src, i);
        OBJ_GEO3D* b = surf(dst, i);
        REQUIRE(a != b);
        CHECK_EQ(b->node_count(), a->node_count());
        CHECK_EQ(b->elem_count(), a->elem_count());
        for (std::int32_t k = 0; k < a->node_count() * 3; ++k)
            CHECK_EQ(b->coords.begin()[k], a->coords.begin()[k]);
        for (std::int32_t k = 0; k < a->elem_count() * 4; ++k)
            CHECK_EQ(b->elems.begin()[k], a->elems.begin()[k]);
        REQUIRE(b->elem_flags != nullptr);
        REQUIRE(b->elem_flags != a->elem_flags);
        for (std::int32_t k = 0; k < a->elem_count(); ++k) {
            CHECK_EQ(b->elem_flags[k], a->elem_flags[k]);
            CHECK_EQ(b->elem_ids.begin()[k], a->elem_ids.begin()[k]);
        }
    }
    CHECK_EQ(surf(dst, 0)->node_count(), 5);
    CHECK_EQ(surf(dst, 0)->coords.begin()[14], 1.0 + 0.5 * 14);
    CHECK_EQ(reinterpret_cast<linked::Cls_140257340List*>(surf(dst, 0)->u28)->size(), 2);
    CHECK_EQ(surf(dst, 1)->node_count(), 0);  // empty slots stay empty
    CHECK_EQ(surf(src, 0)->node_count(), 5);  // src unchanged

    // the direct entry point other packets use (140199c20 -> 1402f9b60)
    llobj_copy::geo3d_assign(surf(dst, 3), surf(src, 7));
    CHECK_EQ(surf(dst, 3)->node_count(), 4);
    CHECK_EQ(surf(dst, 3)->coords.begin()[0], -2.0);
}

// The +0x3998 list helpers as p10-01 (1401d0fc0's object remap) and 1403019d0 reach them through
// llobj::ext::call with no hook: 140302090 detaches a shared body, 1403020c0 builds an iterator,
// 140301e30 / 140302020 / 140302040 walk the nodes.
TEST_CASE(list_helpers_3998_run_the_owners)
{
    llobj::ext::g_hook = nullptr;
    geo_kw::GeoList* a = geo_kw::new_geo_list();   // one default entry
    geo_kw::geo_list_append_default(a);             // two entries
    geo_kw::GeoList* b = new geo_kw::GeoList(*a);   // shares a's body
    REQUIRE(a->body() == b->body());
    CHECK_EQ(a->use_count(), 2);
    const auto u = [](const void* p) { return static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(p)); };

    llobj::ext::call(0x140302090, u(b));           // detach: b gets its own clone
    CHECK(a->body() != b->body());
    CHECK_EQ(a->use_count(), 1);
    CHECK_EQ(b->use_count(), 1);
    CHECK_EQ(geo_kw::geo_list_count(b), 2);
    const void* body_b = b->body();
    llobj::ext::call(0x140302090, u(b));           // not shared: unchanged
    CHECK(b->body() == body_b);

    // iterator over b: 1403020c0(out, first node), then step to end() (140301e30)
    const std::uint64_t head = *reinterpret_cast<const std::uint64_t*>(reinterpret_cast<const std::byte*>(body_b) + 8);
    const std::uint64_t first = *reinterpret_cast<const std::uint64_t*>(static_cast<std::uintptr_t>(head));
    std::uint64_t it = 0, end = 0, old = 0;
    CHECK_EQ(llobj::ext::call(0x1403020c0, u(&it), first), u(&it));
    CHECK_EQ(it, first);
    CHECK_EQ(llobj::ext::call(0x140301e30, u(b), u(&end)), u(&end));
    CHECK_EQ(end, head);
    int n = 0;
    while (it != end) {
        const std::uint64_t v = llobj::ext::call(0x140302020, u(&it));
        CHECK_EQ(v, it + 0x10);
        CHECK(reinterpret_cast<GEO2D_AND_MESH2D*>(static_cast<std::uintptr_t>(v)) == geo_kw::geo_list_at(b, n));
        const std::uint64_t prev = it;
        CHECK_EQ(llobj::ext::call(0x140302040, u(&it), u(&old), 0), u(&old));
        CHECK_EQ(old, prev);
        ++n;
    }
    CHECK_EQ(n, 2);
    geo_kw::delete_geo_list(a);
    geo_kw::delete_geo_list(b);
}

// The _OBJ_GEO3D inside a contact window (LLWINDEF_DATA+0x220) is p1-07's class now: the window
// copy 14025c9b0 runs 1402f9b60 on it for a 3D window, and 1402632d0 reads its nodes (shape 3).
TEST_CASE(window_copy_runs_the_geo3d_assign)
{
    DB_STEP_DATA m(3);
    LLWINDEF_DATA src(&m);
    LLWINDEF_DATA dst(&m);
    REQUIRE(window_is_3d(&src));
    src.shape = 3;
    CHECK(!window_has_shape(&src));  // shape 3 without nodes
    std::int32_t nn = 3, ne = 1;
    double* x = src.geo3d.resize_nodes(&nn);
    for (int k = 0; k < 9; ++k) x[k] = 0.25 * k;
    std::int32_t* e = src.geo3d.resize_elems(&ne);
    e[0] = 1, e[1] = 2, e[2] = 3, e[3] = 3;
    CHECK(window_has_shape(&src));
    window_copy(&dst, &src);
    CHECK_EQ(dst.shape, 3);
    CHECK_EQ(dst.geo3d.node_count(), 3);
    CHECK_EQ(dst.geo3d.elem_count(), 1);
    CHECK_EQ(dst.geo3d.coords.begin()[8], 2.0);
    CHECK(window_has_shape(&dst));
    window_free(&dst);  // 1402f9340 on +0x220
    CHECK_EQ(dst.geo3d.node_count(), 0);
}

int main() { return testkit::run_all(); }
