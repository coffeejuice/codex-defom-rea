#include <testkit/check.hpp>
#include "m2m/model.hpp"

using m2m::RecoveryEntry;

TEST_CASE(ppv_recovery_entry_preserves_existing_key_aggregate)
{
    const RecoveryEntry keyEntry{1, 12.5};
    CHECK_EQ(keyEntry.type, 1);
    CHECK_EQ(keyEntry.value, 12.5);
    CHECK_EQ(keyEntry.ppvSecondInt, 0);
    CHECK_EQ(keyEntry.ppvFirstDouble, 0.0);
    CHECK_EQ(keyEntry.ppvSecondDouble, 0.0);
}

TEST_CASE(ppv_recovery_entry_retains_four_field_point_and_zero_key_value)
{
    const RecoveryEntry ppvEntry{7, 0.0, 11, 2.25, -4.5};
    CHECK_EQ(ppvEntry.type, 7);
    CHECK_EQ(ppvEntry.value, 0.0);
    CHECK_EQ(ppvEntry.ppvSecondInt, 11);
    CHECK_EQ(ppvEntry.ppvFirstDouble, 2.25);
    CHECK_EQ(ppvEntry.ppvSecondDouble, -4.5);
}

TEST_CASE(ppv_recovery_entry_defaults_extra_fields)
{
    const RecoveryEntry defaultEntry{};
    CHECK_EQ(defaultEntry.type, 0);
    CHECK_EQ(defaultEntry.value, 0.0);
    CHECK_EQ(defaultEntry.ppvSecondInt, 0);
    CHECK_EQ(defaultEntry.ppvFirstDouble, 0.0);
    CHECK_EQ(defaultEntry.ppvSecondDouble, 0.0);
}

int main() { return testkit::run_all(); }
