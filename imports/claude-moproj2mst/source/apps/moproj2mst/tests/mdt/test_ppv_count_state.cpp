// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include "m2m/model.hpp"

using namespace m2m;

TEST_CASE(ppv_count_aliases_the_existing_key_nrecvl_slot_field)
{
    Operation operation;
    operation.objs.push_back(std::make_unique<ObjEntry>());
    ObjEntry& slot = *operation.objs.front();

    CHECK_EQ(slot.sv.ppvCount(), 0);
    CHECK_EQ(slot.sv.nrecvlCount, 0);

    slot.sv.setPpvCount(3);
    CHECK_EQ(slot.sv.ppvCount(), 3);
    CHECK_EQ(slot.sv.nrecvlCount, 3); // KEY NRECVL emission sees the same +0xf0.

    slot.sv.nrecvlCount = 2;
    CHECK_EQ(slot.sv.ppvCount(), 2); // No duplicate PPV count storage exists.
}

TEST_CASE(ppv_count_state_is_per_slot)
{
    Operation operation;
    operation.objs.push_back(std::make_unique<ObjEntry>());
    operation.objs.push_back(std::make_unique<ObjEntry>());

    operation.objs[0]->sv.setPpvCount(4);
    operation.objs[1]->sv.setPpvCount(7);
    CHECK_EQ(operation.objs[0]->sv.ppvCount(), 4);
    CHECK_EQ(operation.objs[1]->sv.ppvCount(), 7);
}

int main() { return testkit::run_all(); }
