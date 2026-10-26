// Tier 1: OBJ_MESH2D (Cls_1403ddf20) ctor, reset, clear, copy_from, dtor and its array handles.
// ctest-labels: p1-13 resultset
//
// The p1-04 sub-object functions are replaced by recorders, so the test checks what
// OBJ_MESH2D itself does: the call order and arguments read from the decompiles of 1403ddf20,
// 1403de190, 1403de2a0, 1403de3d0, 1403de460 and the fields it writes.
#include <defmodel/layout/obj_mesh2d.hpp>
#include <testkit/check.hpp>

#include <cstdlib>
#include <cstring>
#include <new>
#include <string>
#include <vector>

using namespace def::model;
using namespace def::model::mesh2d;

namespace {

std::vector<std::string> g_log;
OBJ_MESH2D* g_obj = nullptr;
const OBJ_MESH2D* g_src = nullptr;

std::string where(const void* p)
{
    const auto* b = reinterpret_cast<const std::byte*>(g_obj);
    const auto* q = static_cast<const std::byte*>(p);
    if (g_src != nullptr) {
        const auto* s = reinterpret_cast<const std::byte*>(g_src);
        if (q >= s && q < s + sizeof(OBJ_MESH2D)) {
            return "src+" + std::to_string(q - s);
        }
    }
    return std::to_string(q - b);
}

void rec(const char* what, const void* p) { g_log.push_back(std::string(what) + "@" + where(p)); }

void install_recorders()
{
    MemberExt& e = member_ext();
    e.ctor_a = [](void* p) { rec("ctor_a", p); };
    e.dtor_a = [](void* p) { rec("dtor_a", p); };
    e.clear_a = [](void* p) { rec("clear_a", p); };
    e.copy_a = [](void* d, const void* s, std::int32_t n, std::int32_t* f, char b) {
        rec("copy_a", d);
        rec("copy_a.src", s);
        g_log.push_back("copy_a.n=" + std::to_string(n) + " b=" + std::to_string(b));
        rec("copy_a.flags", f);
    };
    e.ctor_b = [](void* p) { rec("ctor_b", p); };
    e.dtor_b = [](void* p) { rec("dtor_b", p); };
    e.clear_b = [](void* p) { rec("clear_b", p); };
    e.copy_b = [](void* d, const void* s, std::int32_t n1, std::int32_t n2, std::int32_t* f, char b6,
                  char b7, char b8) {
        rec("copy_b", d);
        rec("copy_b.src", s);
        g_log.push_back("copy_b.n=" + std::to_string(n1) + "," + std::to_string(n2) + " b=" +
                        std::to_string(b6) + std::to_string(b7) + std::to_string(b8));
        rec("copy_b.flags", f);
    };
    e.ctor_c = [](void* p) { rec("ctor_c", p); };
    e.dtor_c = [](void* p) { rec("dtor_c", p); };
    e.clear_c = [](void* p) { rec("clear_c", p); };
    e.ctor_e = [](void* p) { rec("ctor_e", p); };
    e.dtor_e = [](void* p) { rec("dtor_e", p); };
    e.clear_e = [](void* p) { rec("clear_e", p); };
}

// Construct in caller-owned storage so the recorders know the object's address up front.
OBJ_MESH2D* make(void* mem)
{
    g_obj = static_cast<OBJ_MESH2D*>(mem);
    return new (mem) OBJ_MESH2D();
}

std::vector<std::string> expected_reset_clear(std::int32_t count)
{
    std::vector<std::string> v = {"clear_a@8", "clear_b@328", "clear_c@592"};
    for (std::int32_t i = 0; i < count; ++i) {
        v.push_back("clear_e@" + std::to_string(0x2c8 + i * 0x30));
    }
    return v;
}

} // namespace

TEST_CASE(ctor_order_and_defaults)
{
    install_recorders();
    g_log.clear();
    alignas(16) static unsigned char mem[sizeof(OBJ_MESH2D)];
    std::memset(mem, 0xcd, sizeof mem);
    OBJ_MESH2D* m = make(mem);

    std::vector<std::string> want = {"ctor_a@8", "ctor_b@328", "ctor_c@592"};
    for (int i = 0; i < 100; ++i) {
        want.push_back("ctor_e@" + std::to_string(0x2c8 + i * 0x30));
    }
    for (const auto& s : expected_reset_clear(0)) { // reset -> clear with count 0
        want.push_back(s);
    }
    CHECK(g_log == want);
    CHECK_EQ(m->count, 0);
    for (int i = 0; i < 0x4f; ++i) {
        const bool two = i <= 6 || i == 18 || i == 19 || i == 25;
        CHECK_EQ(m->flags[i], two ? 2 : 1);
    }
    std::int32_t v;
    std::memcpy(&v, m->s008 + 0x138, 4);
    CHECK_EQ(v, 2);
    std::memcpy(&v, m->s148 + 0xf8, 4);
    CHECK_EQ(v, 2);
    std::memcpy(&v, m->s250 + 0x68, 4);
    CHECK_EQ(v, 2);
    REQUIRE(m->h1588 != nullptr);
    CHECK_EQ(m->h1588->refs, 1);
    CHECK(m->h1588->data == nullptr);
    CHECK(m->h15b8->u18 == nullptr);

    g_log.clear();
    m->~OBJ_MESH2D();
    std::vector<std::string> wd = expected_reset_clear(0);
    for (int i = 99; i >= 0; --i) {
        wd.push_back("dtor_e@" + std::to_string(0x2c8 + i * 0x30));
    }
    wd.push_back("dtor_c@592");
    wd.push_back("dtor_b@328");
    wd.push_back("dtor_a@8");
    CHECK(g_log == wd);
}

TEST_CASE(clear_uses_count)
{
    install_recorders();
    alignas(16) static unsigned char mem[sizeof(OBJ_MESH2D)];
    OBJ_MESH2D* m = make(mem);
    m->count = 3;
    g_log.clear();
    m->clear();
    CHECK(g_log == expected_reset_clear(3));
    m->count = 0;
    m->~OBJ_MESH2D();
}

TEST_CASE(copy_from_reads_source_counts)
{
    install_recorders();
    alignas(16) static unsigned char smem[sizeof(OBJ_MESH2D)];
    alignas(16) static unsigned char dmem[sizeof(OBJ_MESH2D)];
    OBJ_MESH2D* s = make(smem);
    const std::int32_t na = 17, nb1 = 23, nb2 = 29;
    std::memcpy(s->s008 + 300, &na, 4);  // read by FUN_140182840 (+0x12c)
    std::memcpy(s->s148 + 0xe8, &nb1, 4); // FUN_1401854f0
    std::memcpy(s->s148 + 0xec, &nb2, 4); // FUN_140185530
    OBJ_MESH2D* d = make(dmem);
    d->flags[40] = 7;
    g_src = s;
    g_log.clear();
    d->copy_from(s);
    std::vector<std::string> want = expected_reset_clear(0);
    for (const auto& x : expected_reset_clear(0)) {
        want.push_back(x);
    }
    want.insert(want.end(), {"copy_a@8", "copy_a.src@src+8", "copy_a.n=17 b=1", "copy_a.flags@5568",
                             "copy_b@328", "copy_b.src@src+328", "copy_b.n=23,29 b=111",
                             "copy_b.flags@5568"});
    CHECK(g_log == want);
    CHECK_EQ(d->flags[40], 1); // reset ran before the copy
    CHECK_EQ(s->mesh_b_count(), 29);
    CHECK(s->mesh_a() == s->s008);
    g_src = nullptr;
    d->~OBJ_MESH2D();
    g_obj = s;
    s->~OBJ_MESH2D();
}

TEST_CASE(handle_release_destroys_elements)
{
    // the element members are copy-on-write handles: sharing them with outside handles shows
    // when the array's elements are destroyed
    IntVector a, b;
    IntList l;
    auto* blk = new RcArray<Elem20_1403e25a0>();
    blk->data = new Elem20_1403e25a0[2];
    blk->u10 = blk->data + 2;
    blk->u18 = blk->data + 2;
    blk->data[0].regions = a;
    blk->data[1].loops = b;
    blk->data[1].nodes = l;
    CHECK_EQ(a.use_count(), 2);
    blk->refs = 2;
    release_rc_array(blk); // 2 -> 1: nothing destroyed
    CHECK_EQ(a.use_count(), 2);
    release_rc_array(blk); // 1 -> 0: the elements are destroyed
    CHECK_EQ(a.use_count(), 1);
    CHECK_EQ(b.use_count(), 1);
    CHECK_EQ(l.use_count(), 1);
}

TEST_CASE(handle_push_back_and_resize)
{
    RcArray<Elem18_1403e23f0>* h = new_rc_array<Elem18_1403e23f0>();
    RcHandle<Elem18_1403e23f0> r(h);
    Elem18_1403e23f0 e;
    for (int i = 0; i < 5; ++i) {
        e.mat = i;
        r.push_back(e);
    }
    CHECK_EQ(r.size(), 5);
    CHECK_EQ(r.at(3).mat, 3);
    CHECK_EQ(h->u18 - h->data, 7); // capacities 1, 2, 4, 7
    RcArray<Face18>* f = new_rc_array<Face18>();
    RcHandle<Face18> rf(f);
    const Face18 init = {-1, -1, {-1, -1, -1, -1}};
    rf.resize(4, init);
    CHECK_EQ(rf.size(), 4);
    CHECK_EQ(rf.at(2).edge[3], -1);
    rf.resize(1, init);
    CHECK_EQ(rf.size(), 1);
    // a shared block is detached before writing
    h->refs = 2;
    RcArray<Elem18_1403e23f0>* old = h;
    r.at(0).mat = 9;
    CHECK(h != old);
    CHECK_EQ(old->refs, 1);
    CHECK_EQ(old->data[0].mat, 0);
    release_rc_array(old);
    release_rc_array(h);
    release_rc_array(f);
}

TEST_CASE(loop_list_pop_front)
{
    TopoLoopList l;
    l.push_back(LoopItem{1, 7});
    l.push_back(LoopItem{0, 8});
    CHECK_EQ(l.size(), 2u);
    CHECK_EQ(l.at(1).curve, 8);
}

int main() { return testkit::run_all(); }
