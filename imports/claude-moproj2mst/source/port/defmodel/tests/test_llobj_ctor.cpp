// ctest-labels: kw-g2obj llobj
// Tier-1 tests of the LLOBJ_DATA init 14015f0b0 / owner 14015ef60 and the emptying 140161260
// (defmodel/llobj_ctor). The ctor 14015ec80 itself needs the DYNARY, StateVarBase, list and
// _OBJ_GEO3D ctors of other packets, which are stubs on this branch, so the object is built in
// zero-filled storage with the COW handles constructed and the external calls recorded through
// llobj::ext::g_hook. Expected values are the stores of the decompile (out/g2obj/ctor_closure.c).
#include <cstring>
#include <new>
#include <vector>

#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/geo/pending.hpp>
#include <defmodel/llobj/ext.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defmodel/llobj_ctor/obj_defaults.hpp>
#include <defmodel/sdctl/cow.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <testkit/check.hpp>

using namespace def::model;

namespace {

template <class T>
T& at(void* p, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<unsigned char*>(p) + off);
}

std::vector<std::uint64_t> g_calls;
unsigned char g_geo[100][0x130];
int g_geo_next = 0;

int g_geo_ctor = 0;
int g_u3998_placeholder = 0;
// fakes for the calls OBJ_GEO3D's ctor (p1-07) still makes through defmodel/geo/pending.hpp
void* fake_list(void* p) { ++g_geo_ctor; return p; }
void** fake_handle(void** h) { return h; }
void* fake_tree(void* t) { return t; }
void fake_void(void*) {}

std::uint64_t recorder(std::uint64_t addr, const std::uint64_t*, int)
{
    g_calls.push_back(addr);
    return 0;
}

struct Fixture {
    void* mraw;
    void* oraw;
    explicit Fixture(int dim)
    {
        mraw = ::operator new(sizeof(DB_STEP_DATA));
        std::memset(mraw, 0, sizeof(DB_STEP_DATA));
        at<std::int32_t>(mraw, 0x1c) = dim;
        oraw = ::operator new(sizeof(LLOBJ_DATA));
        std::memset(oraw, 0, sizeof(LLOBJ_DATA));
        LLOBJ_DATA* o = obj();
        new (&o->u3950) CowVector<void*>();
        new (&o->u3958) CowVector<void*>();
        new (&o->u3960) CowVector<void*>();
        new (&o->u3968) CowVector<std::int32_t>();
        new (&o->u3970) CowVector<void*>();
        new (&o->u3978) CowVector<void*>();
        // init creates the +0x3998 list only when it is null; its element GEO2D_AND_MESH2D (p1-07)
        // still makes pending calls on this branch, so the test supplies a placeholder
        o->u3998 = &g_u3998_placeholder;
        llobj::ext::g_hook = recorder;
        pending::fun_140257bf0 = fake_list;
        pending::fun_1402fedd0 = fake_handle;
        pending::fun_1402fef20 = fake_handle;
        pending::fun_14042a7e0 = fake_tree;
        pending::fun_140220540 = fake_tree;
        pending::fun_140220560 = fake_void;
        pending::fun_140220570 = fake_void;
        g_geo_ctor = 0;
        g_calls.clear();
    }
    ~Fixture() { llobj::ext::g_hook = nullptr; }
    DB_STEP_DATA* m() { return static_cast<DB_STEP_DATA*>(mraw); }
    LLOBJ_DATA* obj() { return static_cast<LLOBJ_DATA*>(oraw); }
};

} // namespace

TEST_CASE(set_owner_and_init_3d)
{
    Fixture fx(3);
    LLOBJ_DATA* o = fx.obj();
    at<std::int32_t>(o, 8) = 9;
    at<std::int32_t>(o, 0xc) = 4;
    at<std::int32_t>(fx.m(), 0x750 + 0xfc) = 1;  // UNIT = 1: REFTMP default 20
    llobj_ctor::set_owner(o, fx.m());
    CHECK(o->owner == fx.m());
    CHECK_EQ(at<std::int32_t>(o, 8), 0);
    CHECK_EQ(at<std::int32_t>(o, 0xc), 0);
    CHECK_EQ(at<std::int32_t>(o, 0x39c0), 3);   // 140199af0(dimension)
    CHECK_EQ(at<std::int32_t>(o, 0x890), 2);    // 140165500(obj, 2) in 3D
    CHECK_EQ(at<std::int32_t>(o, 0x894), 4);
    CHECK_EQ(at<std::int32_t>(o, 0x898), 4);
    CHECK_EQ(at<std::int32_t>(o, 0x89c), 3);
    CHECK_EQ(at<std::int32_t>(o, 0xbc8), -1);
    CHECK_EQ(at<std::int32_t>(o, 0x39f0), 1);
    CHECK_EQ(at<double>(o, 0x1eb8), 20.0);      // 14015f9a0 with UNIT 1
    CHECK_EQ(at<double>(o, 0x1ec0), 1.0);
    CHECK_EQ(at<double>(o, 0x1ea0), 0.01);      // LMTSTR
    CHECK_EQ(at<double>(o, 0x1eb0), 1000000.0); // PENVOL
    CHECK_EQ(at<std::int32_t>(o, 0x2098), -1);  // TRGVOL mode
    CHECK_EQ(at<std::int32_t>(o, 0x1f18), 2);   // ELPSOL
    CHECK_EQ(at<double>(o, 0x21c0), -1.0);      // 3D direction: component dim-1
    CHECK_EQ(at<std::int32_t>(o, 0x21c8), 1);   // movement type
    CHECK_EQ(at<std::uint8_t>(o, 0x2bd8), 1);
    CHECK_EQ(at<double>(o, 0x2dc8), -0.7);      // RMDPTH, 3D
    CHECK_EQ(at<std::int32_t>(o, 0x3aa0), -1);
    CHECK_EQ(at<double>(o, 0x3ab0), 5.0);
    CHECK_EQ(at<double>(o, 0x3b68), 7.0);
    CHECK_EQ(at<std::uint8_t>(&o->u3a60, 8), 1);
    CHECK_EQ(o->u3598.type, 0x7c);
    // owners of the sub-objects
    CHECK(at<DB_STEP_DATA*>(o, 0x1ea0 + 0x2b8) == fx.m());
    CHECK(at<DB_STEP_DATA*>(o, 0x2160 + 0xa80) == fx.m());
    CHECK(at<DB_STEP_DATA*>(o, 0x2d10) == fx.m());
    CHECK(at<LLOBJ_DATA*>(o, 0xb58 + 0x10) == o);
    // 100 OBJ_GEO3D objects in the geometry slots (their ctor runs once each here; the two
    // embedded ones at 0x3328/0x3458 belong to the LLOBJ_DATA ctor, not run in this test)
    CHECK_EQ(g_geo_ctor, 100);
    for (int i = 0; i < 100; ++i) CHECK(o->u3c00[i] != nullptr);
    CHECK(o->u3998 == &g_u3998_placeholder);  // kept when set
}

TEST_CASE(init_2d_defaults)
{
    Fixture fx(2);
    LLOBJ_DATA* o = fx.obj();
    llobj_ctor::set_owner(o, fx.m());  // UNIT 0: REFTMP default 68
    CHECK_EQ(at<double>(o, 0x1eb8), 68.0);
    CHECK_EQ(at<std::int32_t>(o, 0x890), 0);
    CHECK_EQ(at<std::int32_t>(o, 0x894), 4);
    CHECK_EQ(at<std::int32_t>(o, 0x1f20), 1);  // PENVOL k (2D)
    CHECK_EQ(at<double>(o, 0x21b0), 1.0);      // 2D direction (1, 0)
    CHECK_EQ(at<double>(o, 0x21b8), 0.0);
    CHECK_EQ(at<double>(o, 0x2dc8), 0.0);      // RMDPTH, 2D
    CHECK_EQ(at<double>(o, 0x2d10 + 0x10), 3.0);
    CHECK_EQ(at<std::int32_t>(o, 0x2d10 + 0x80), 1000);
    CHECK_EQ(at<DYNARY>(o, 0x25a8).type, 0x79);  // movement type-6 table template in 2D
}

TEST_CASE(make_empty_stores)
{
    Fixture fx(2);
    LLOBJ_DATA* o = fx.obj();
    llobj_ctor::set_owner(o, fx.m());
    at<std::int32_t>(o, 0xc) = 3;
    o->name[0] = 'A';
    o->u87c[4] = 5;
    g_calls.clear();
    llobj_ctor::make_empty(o);
    CHECK_EQ(at<std::int32_t>(o, 0xc), 7);
    CHECK_EQ(o->name[0], '\0');
    CHECK_EQ(o->u87c[4], 0);
    for (int i = 0; i < 100; ++i) CHECK(o->u3c00[i] != nullptr);  // slots kept (reset only)
}

TEST_CASE(geo23_angle)
{
    unsigned char p[0x130] = {};
    llobj_ctor::geo23_defaults(reinterpret_cast<std::byte*>(p));
    CHECK_EQ(at<double>(p, 8), 210.0);
    CHECK_EQ(at<std::int32_t>(p, 0x3c), 70);   // 210 / 3
    CHECK_EQ(at<double>(p, 0x10), -15.0);      // (180 - 210) / 2
    double small = 21.0;
    llobj_ctor::geo23_set_angle(reinterpret_cast<std::byte*>(p), &small);
    CHECK_EQ(at<std::int32_t>(p, 0x3c), 10);   // 7 <= 10 -> 10
}

int main() { return testkit::run_all(); }
