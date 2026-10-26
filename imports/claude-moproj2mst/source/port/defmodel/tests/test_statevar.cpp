// StateVarBase and its derived classes (p1-09). Expected values follow the decompiled code by
// hand (docs/re/status/p1-09.tsv lists the functions).
// ctest-labels: p1-09 statevar
#include <defmodel/layout/state_var_base.hpp>
#include <defmodel/layout/state_var_elem.hpp>
#include <defmodel/layout/state_var_elem_currnt.hpp>
#include <defmodel/layout/state_var_elem_damag.hpp>
#include <defmodel/layout/state_var_elem_mataxi.hpp>
#include <defmodel/layout/state_var_elem_sts.hpp>
#include <defmodel/layout/state_var_elem_teps.hpp>
#include <defmodel/layout/state_var_node.hpp>
#include <defmodel/layout/state_var_node_post.hpp>
#include <testkit/check.hpp>

#include <cstring>
#include <memory>

using namespace def::model;

static_assert(sizeof(StateVarNode) == 0x38);
static_assert(sizeof(StateVarNodePost) == 0x38);
static_assert(sizeof(StateVarElem) == 0x38);
static_assert(sizeof(StateVarElemMATAXI) == 0x38);
static_assert(sizeof(StateVarElemSTS) == 0x40);
static_assert(sizeof(StateVarElemTEPS) == 0x40);
static_assert(sizeof(StateVarElemDAMAG) == 0x40);
static_assert(sizeof(StateVarElemCURRNT) == 0x40);

namespace {

void check_dims(const StateVarBase& s, int a, int b, int c, int pa, int pb, int pc)
{
    CHECK_EQ(s.dim0(false), a);
    CHECK_EQ(s.dim1(false), b);
    CHECK_EQ(s.dim2(false), c);
    CHECK_EQ(s.dim0(true), pa);
    CHECK_EQ(s.dim1(true), pb);
    CHECK_EQ(s.dim2(true), pc);
}

// Fake element container for the STS/TEPS/DAMAG point count (140183c20 reads +0xec, +0xf0,
// +0xf8 of the owner and +0x888 + 4k of the object at +0xf0).
struct FakeOwner {
    alignas(8) unsigned char owner[0x108] = {};
    alignas(8) unsigned char sub[0x900] = {};
    void set(int type_k, int k, int f8, int ec)
    {
        void* p = sub;
        std::memcpy(owner + 0xf0, &p, sizeof p);
        std::memcpy(sub + 0x888 + 4 * k, &type_k, 4);
        std::memcpy(owner + 0xf8, &f8, 4);
        std::memcpy(owner + 0xec, &ec, 4);
    }
};

} // namespace

TEST_CASE(base_ctor_state)
{
    double* slot = reinterpret_cast<double*>(0x1234);
    StateVarBase s(7, &slot);
    CHECK_EQ(s.get_id(), 7);
    CHECK_EQ(s.id, 7);
    CHECK_EQ(s.u0c, 0);
    // ctor 0/1/1 for cur and prev, then reset() sets 0/1/1 again (prev = old cur)
    check_dims(s, 0, 1, 1, 0, 1, 1);
    CHECK_EQ(s.value_count(), 0);
    CHECK(s.value_data() == nullptr);
    CHECK(slot == nullptr);   // reset publishes null
    CHECK(s.u30 == &slot);
}

TEST_CASE(default_ctor_matches_llobj_member)
{
    StateVarBase s;   // LLOBJ_DATA + 0xb70: StateVarBase(-1, 0)
    CHECK_EQ(s.get_id(), -1);
    CHECK(s.u30 == nullptr);
    check_dims(s, 0, 1, 1, 0, 1, 1);
}

TEST_CASE(kinds_of_derived_classes)
{
    double* slot = nullptr;
    StateVarNode n(1, &slot);
    StateVarNodePost np(0x1e, &slot);
    StateVarElem e(0x27, &slot);
    StateVarElemMATAXI m(0x2e, &slot);
    FakeOwner o;
    StateVarElemSTS sts(0x26, &slot, o.owner);
    StateVarElemCURRNT cur(0x40, &slot, o.owner);
    CHECK_EQ(n.u0c, 1);
    CHECK_EQ(np.u0c, 1);
    CHECK_EQ(e.u0c, 2);
    CHECK_EQ(m.u0c, 2);
    CHECK_EQ(sts.u0c, 2);
    CHECK(sts.owner == o.owner);
    CHECK(cur.owner == o.owner);
}

TEST_CASE(slot1_sets_dims_and_resizes)
{
    double* slot = nullptr;
    StateVarNode s(3, &slot);
    s.vslot1_140301710(3, 2, 2);
    check_dims(s, 3, 2, 2, 0, 1, 1);
    CHECK_EQ(s.value_count(), 12);
    REQUIRE(s.value_data() != nullptr);
    CHECK(slot == s.value_data());
    for (int i = 0; i < 12; ++i) CHECK_BITS(s.value_data()[i], 0.0);
    // same size again: no resize, slot untouched
    slot = nullptr;
    s.vslot1_140301710(2, 3, 2);
    CHECK_EQ(s.value_count(), 12);
    CHECK(slot == nullptr);
    // shrink keeps the first values
    s.value_data()[0] = 5.0;
    s.vslot1_140301710(1, 1, 1);
    CHECK_EQ(s.value_count(), 1);
    CHECK_BITS(s.value_data()[0], 5.0);
    CHECK(slot == s.value_data());
}

TEST_CASE(resize_without_slot_does_not_publish)
{
    StateVarBase s(0, nullptr);
    s.vslot1_140301710(2, 2, 1);
    CHECK_EQ(s.value_count(), 4);
    CHECK(s.resize_values(false) == false);
    s.set_dim0(3);
    CHECK(s.resize_values(true) == true);   // remap is skipped without a slot
    CHECK_EQ(s.value_count(), 6);
}

TEST_CASE(resize_remap_moves_rows)
{
    double* slot = nullptr;
    StateVarBase s(0, &slot);
    s.vslot1_140301710(2, 2, 1);   // values r0 = {1, 2}, r1 = {3, 4}
    for (int i = 0; i < 4; ++i) s.value_data()[i] = i + 1.0;
    // to 2 x 3 x 1 with remap: old row r at r*2, new at r*3. The old dims come from prev, so
    // dim0 is set again (prev0 = 2). Cells not copied keep the resized data.
    s.set_dim0(2);
    s.set_dim1(3);
    CHECK(s.resize_values(true));
    const double* d = s.value_data();
    CHECK_EQ(s.value_count(), 6);
    // resize appends zeros: {1,2,3,4,0,0}; row 1 then gets {3,4} at 3..4, row 0 {1,2} at 0..1
    CHECK_BITS(d[0], 1.0);
    CHECK_BITS(d[1], 2.0);
    CHECK_BITS(d[2], 3.0);
    CHECK_BITS(d[3], 3.0);
    CHECK_BITS(d[4], 4.0);
    CHECK_BITS(d[5], 0.0);
}

TEST_CASE(slot2_base_and_node_post)
{
    double* slot = nullptr;
    const int per_id[8] = {0, 0, 0, 6, 0, 0, 0, 0};
    StateVarNode a(3, &slot);
    a.vslot2_140301690(5, per_id, -1);
    check_dims(a, 5, 6, 1, 0, 1, 1);
    CHECK_EQ(a.value_count(), 30);
    a.vslot2_140301690(9, per_id, 4);   // other id: nothing
    CHECK_EQ(a.dim0(false), 5);

    double* slot2 = nullptr;
    StateVarNodePost p(3, &slot2);
    p.vslot2_140301690(5, per_id, -1);   // -1 does not match NodePost
    CHECK_EQ(p.dim0(false), 0);
    CHECK_EQ(p.value_count(), 0);
    p.vslot2_140301690(5, per_id, 3);
    CHECK_EQ(p.value_count(), 30);
    CHECK(slot2 == p.value_data());

    // CURRNT: a copy of the base slot 2
    StateVarElemCURRNT c(3, &slot, nullptr);
    c.vslot2_140301690(2, per_id, -1);
    CHECK_EQ(c.value_count(), 12);
}

TEST_CASE(slot2_point_count_from_owner)
{
    double* slot = nullptr;
    int per_id[0x40] = {};
    per_id[0x26] = 6;
    per_id[0x2b] = 1;
    per_id[0x2d] = 2;
    FakeOwner o;
    {
        StateVarElemSTS s(0x26, &slot, o.owner);
        o.set(3, 2, 3, 8);   // type 3, f8 == 3, ec == 8 -> 8
        s.vslot2_140301690(2, per_id, -1);
        check_dims(s, 2, 6, 8, 0, 1, 1);
        CHECK_EQ(s.value_count(), 96);
        o.set(3, 2, 3, 4);   // ec != 8 -> 1
        s.vslot2_140301690(2, per_id, 0x26);
        CHECK_EQ(s.dim2(false), 1);
        o.set(3, 2, 1, 8);   // f8 != 3 -> 4
        s.vslot2_140301690(2, per_id, 0x26);
        CHECK_EQ(s.dim2(false), 4);
        o.set(1, 2, 3, 8);   // type != 3 -> 1
        s.vslot2_140301690(2, per_id, 0x26);
        CHECK_EQ(s.dim2(false), 1);
    }
    {
        FakeOwner o1;
        o1.set(3, 1, 0, 0);   // TEPS reads index 1 -> 4
        StateVarElemTEPS t(0x2b, &slot, o1.owner);
        t.vslot2_140301690(3, per_id, -1);
        check_dims(t, 3, 1, 4, 0, 1, 1);
    }
    {
        FakeOwner o0;
        o0.set(3, 0, 0, 0);   // DAMAG reads index 0 -> 4
        StateVarElemDAMAG d(0x2d, &slot, o0.owner);
        d.vslot2_140301690(3, per_id, 0x2d);
        CHECK_EQ(d.dim2(false), 4);
        CHECK_EQ(d.value_count(), 24);
    }
    {
        FakeOwner none;   // null at +0xf0 -> 1
        StateVarElemSTS s(0x26, &slot, none.owner);
        s.vslot2_140301690(1, per_id, -1);
        CHECK_EQ(s.dim2(false), 1);
    }
}

TEST_CASE(mataxi_identity_rows)
{
    double* slot = nullptr;
    int per_id[0x30] = {};
    per_id[0x2e] = 9;
    StateVarElemMATAXI m(0x2e, &slot);
    m.vslot2_140301690(2, per_id, -1);
    REQUIRE(m.value_count() == 18);
    const double* d = m.value_data();
    for (int i = 0; i < 18; ++i) {
        const int j = i % 9;
        CHECK_BITS(d[i], (j == 0 || j == 4 || j == 8) ? 1.0 : 0.0);
    }
    // no resize -> no re-initialisation
    m.value_data()[0] = 7.0;
    m.vslot2_140301690(2, per_id, -1);
    CHECK_BITS(m.value_data()[0], 7.0);
    // per_id != 9 -> plain resize
    per_id[0x2e] = 3;
    m.vslot2_140301690(2, per_id, -1);
    CHECK_EQ(m.value_count(), 6);
    CHECK_BITS(m.value_data()[4], 1.0);   // kept from the old identity row
}

TEST_CASE(copy_from_and_row_copy)
{
    double* sa = nullptr;
    double* sb = nullptr;
    StateVarElem a(1, &sa);
    StateVarElem b(2, &sb);
    b.vslot1_140301710(2, 3, 1);
    for (int i = 0; i < 6; ++i) b.value_data()[i] = 10.0 + i;
    CHECK(a.copy_from(&b));
    check_dims(a, 2, 3, 1, 0, 1, 1);
    REQUIRE(a.value_count() == 6);
    for (int i = 0; i < 6; ++i) CHECK_BITS(a.value_data()[i], 10.0 + i);
    // copy_from an empty source: dims 0 -> no values -> true
    StateVarElem empty(3, nullptr);
    CHECK(a.copy_from(&empty));
    CHECK_EQ(a.value_count(), 0);

    // row copy: a 1 x 3 source into row 1 of a 2 x 3 target
    StateVarElem t(4, nullptr);
    t.vslot1_140301710(2, 3, 1);
    StateVarElem r(5, nullptr);
    r.vslot1_140301710(1, 3, 1);
    for (int i = 0; i < 3; ++i) r.value_data()[i] = 1.5 * (i + 1);
    CHECK(t.vslot4_140301850(1, &r));
    CHECK_BITS(t.value_data()[2], 0.0);
    CHECK_BITS(t.value_data()[3], 1.5);
    CHECK_BITS(t.value_data()[5], 4.5);
    CHECK(!t.vslot4_140301850(2, &r));   // row out of range
    CHECK(!t.vslot4_140301850(-1, &r));
}

TEST_CASE(offsets_and_bounds)
{
    StateVarBase s(0, nullptr);
    s.vslot1_140301710(4, 3, 2);
    CHECK_EQ(s.row_offset(0), 0);
    CHECK_EQ(s.row_offset(3), 18);
    CHECK_EQ(s.row_offset(4), -1);
    CHECK_EQ(s.row_offset(-1), -1);
    CHECK(s.value_at(23) == s.value_data() + 23);
    CHECK(s.value_at(24) == nullptr);
    CHECK(s.value_at(-1) == nullptr);
}

TEST_CASE(clear_and_reset)
{
    double* slot = nullptr;
    StateVarNode s(2, &slot);
    s.vslot1_140301710(3, 2, 1);
    REQUIRE(slot != nullptr);
    s.clear();
    check_dims(s, 0, 2, 1, 3, 1, 1);   // only dim0 changes; prev0 = 3
    CHECK_EQ(s.value_count(), 0);
    CHECK(s.value_data() == nullptr);
    CHECK(slot == nullptr);
    s.vslot1_140301710(1, 2, 1);
    s.reset();
    CHECK_EQ(s.u0c, 0);
    check_dims(s, 0, 1, 1, 1, 2, 1);
    CHECK_EQ(s.value_count(), 2);   // reset leaves the values
    CHECK(slot == nullptr);
}

TEST_CASE(delete_through_base)
{
    double* slot = nullptr;
    std::unique_ptr<StateVarBase> p(new StateVarElemMATAXI(0x2e, &slot));
    p->vslot1_140301710(2, 9, 1);
    CHECK(slot != nullptr);
    p.reset();   // virtual dtor: clear() publishes null
    CHECK(slot == nullptr);
}

int main() { return testkit::run_all(); }
