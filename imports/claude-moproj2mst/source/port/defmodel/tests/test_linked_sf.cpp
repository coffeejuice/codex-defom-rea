// Tier-1 tests of the SfLinkedList family: SfLinkedData, SfLinkedList, OPSTOP_DATA,
// OpStopLinkedList, DYNARY_DATA, DynAryLinkedList and the reference-counted array handle.
// ctest-labels: p1-06 linked
//
// Expected values come from the decompile: ctor stores (14025a5e0, 14025ac90, 14025b0d0), the
// slot 2/3 record layout of OPSTOP_DATA (14025ad20 / 14025ad80), the copy rules (14025acc0,
// 14025b150) and the clear loop 14025ab20.
#include <defmodel/layout/dyn_ary_linked_list.hpp>
#include <defmodel/layout/dynary_data.hpp>
#include <defmodel/layout/op_stop_linked_list.hpp>
#include <defmodel/layout/opstop_data.hpp>
#include <defmodel/sdctl/cow.hpp>
#include <testkit/check.hpp>

#include <cstdint>

using namespace def::model;

namespace {

// Links nodes made by create_item the way the list's own add (14025a7d0, not Phase 1) would.
void append(SfLinkedList& l, SfLinkedData* n)
{
    if (l.count == 0) {
        l.head = n;
    } else {
        l.tail->next = n;
    }
    l.tail = n;
    l.count = l.count + 1;
}

} // namespace

TEST_CASE(opstop_ctor_fields)
{
    OPSTOP_DATA d;
    CHECK_EQ(static_cast<int>(d.u08), 1);
    CHECK_EQ(d.u0c, 0);
    CHECK(d.next == nullptr);
    CHECK_BITS(d.value, 0.0);
}

TEST_CASE(opstop_record_round_trip)
{
    OPSTOP_DATA d;
    const double in[3] = {99.0, 1.9, 0.25}; // rec[1] = 1.9 truncates to 1, so value is taken
    d.from_values(in);
    CHECK_EQ(d.u0c, 1);
    CHECK_BITS(d.value, 0.25);
    double out[3] = {-1.0, -1.0, -1.0};
    d.to_values(out);
    CHECK_BITS(out[0], -1.0); // rec[0] is not touched
    CHECK_BITS(out[1], 1.0);
    CHECK_BITS(out[2], 0.25);
}

TEST_CASE(opstop_record_flag_off_keeps_value)
{
    OPSTOP_DATA d;
    d.value = 7.0;
    const double in[3] = {0.0, 2.0, 5.0};
    d.from_values(in);
    CHECK_EQ(d.u0c, 2);
    CHECK_BITS(d.value, 7.0);
    double out[3] = {0.0, 0.0, -3.0};
    d.to_values(out);
    CHECK_BITS(out[1], 2.0);
    CHECK_BITS(out[2], -3.0);
}

TEST_CASE(opstop_copy_from)
{
    OPSTOP_DATA a;
    OPSTOP_DATA b;
    b.u0c = 1;
    b.value = 3.5;
    a.copy_from(&b);
    CHECK_EQ(a.u0c, 1);
    CHECK_BITS(a.value, 3.5);

    // from a DYNARY_DATA node only the base field is copied (the cast gives null)
    DYNARY_DATA y;
    y.u0c = 9;
    a.copy_from(&y);
    CHECK_EQ(a.u0c, 9);
    CHECK_BITS(a.value, 3.5);
}

TEST_CASE(casts)
{
    OPSTOP_DATA o;
    DYNARY_DATA y;
    CHECK(OPSTOP_DATA::cast(static_cast<SfLinkedData*>(&o)) == &o);
    CHECK(OPSTOP_DATA::cast(static_cast<SfLinkedData*>(&y)) == nullptr);
    CHECK(DYNARY_DATA::cast(static_cast<SfLinkedData*>(&y)) == &y);
    CHECK(DYNARY_DATA::cast(static_cast<SfLinkedData*>(&o)) == nullptr);
    CHECK(OPSTOP_DATA::cast(static_cast<SfLinkedData*>(nullptr)) == nullptr);
}

TEST_CASE(dynary_data_ctor_fields)
{
    DYNARY_DATA d;
    CHECK_EQ(static_cast<int>(d.u08), 1);
    CHECK_EQ(d.u0c, 0);
    CHECK_EQ(d.u18, 0);
    CHECK_EQ(static_cast<int>(d.u1c), 0);
    CHECK_EQ(d.u20, 0);
    CHECK_EQ(d.u24, 0);
    CHECK_EQ(d.u28, 0);
    CHECK_EQ(d.u30.use_count(), 1);
    CHECK_EQ(d.u38.use_count(), 1);
    CHECK(d.u30.body()->first == nullptr && d.u30.body()->last == nullptr && d.u30.body()->end == nullptr);
    CHECK(d.u38.body()->first == nullptr);
}

TEST_CASE(dynary_data_copy_and_empty_slots)
{
    DYNARY_DATA a;
    DYNARY_DATA b;
    b.u0c = 4;
    b.u18 = 5;
    a.copy_from(&b);
    CHECK_EQ(a.u0c, 4);
    CHECK_EQ(a.u18, 0); // 14025b150 copies only the base field
    double rec[3] = {1.0, 2.0, 3.0};
    a.from_values(rec);
    a.to_values(rec);
    CHECK_BITS(rec[1], 2.0);
}

TEST_CASE(dynary_data_reset_detaches_shared_array)
{
    DYNARY_DATA d;
    d.u30.resize(2, 11);
    d.u30.mutable_at(1) = 22;
    def::model::CowVector<std::uint64_t> other(d.u30); // share d's 8-byte array
    CHECK_EQ(other.use_count(), 2);
    d.reset(); // detach: other keeps the data with refs 1, d's own copy is emptied
    CHECK(d.u30.body() != other.body());
    CHECK_EQ(other.use_count(), 1);
    CHECK_EQ(static_cast<int>(other[1]), 22);
    CHECK(d.u30.body()->first == nullptr);
    CHECK_EQ(d.u30.use_count(), 1);
}

TEST_CASE(lists_create_and_clear)
{
    OpStopLinkedList ol;
    CHECK(ol.head == nullptr && ol.tail == nullptr && ol.count == 0);
    for (int i = 0; i < 3; ++i) {
        SfLinkedData* n = ol.create_item();
        REQUIRE(OPSTOP_DATA::cast(n) != nullptr);
        append(ol, n);
    }
    CHECK_EQ(ol.count, 3);
    ol.clear();
    CHECK(ol.head == nullptr && ol.tail == nullptr && ol.count == 0);

    DynAryLinkedList dl;
    for (int i = 0; i < 2; ++i) {
        SfLinkedData* n = dl.create_item();
        REQUIRE(DYNARY_DATA::cast(n) != nullptr);
        append(dl, n);
    }
    CHECK_EQ(dl.count, 2);
    // the dtor clears (14025a6c0 -> 14025ab20); leak checkers would see a miss
}

int main() { return testkit::run_all(); }
