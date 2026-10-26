// ctest-labels: m2m-doe
#include <testkit/check.hpp>

#include "m2m/common.hpp"
#include "../../src/ops/type3_region_value.hpp"

using namespace m2m;
using namespace m2m::mst::detail;

TEST_CASE(type3_region_value_without_delimiters_is_unchanged)
{
    const qt::String input = qt::L("region");
    const auto result = deriveType3RegionValue(input);
    CHECK(result == input);
    CHECK_EQ(result.isNull(), input.isNull());
}

TEST_CASE(type3_region_value_removes_through_first_hash)
{
    CHECK(deriveType3RegionValue(qt::L("comment#region#tail")) == qt::L("region#tail"));
}

TEST_CASE(type3_region_value_selects_last_forward_slash)
{
    CHECK(deriveType3RegionValue(qt::L("root/branch/name.ext")) == qt::L("name"));
    CHECK(deriveType3RegionValue(qt::L("root/branch/name")) == qt::L("name"));
}

TEST_CASE(type3_region_value_selects_last_backslash)
{
    CHECK(deriveType3RegionValue(qt::L("root\\branch\\name.ext")) == qt::L("name"));
}

TEST_CASE(type3_region_value_applies_hash_path_and_extension_steps_in_order)
{
    CHECK(deriveType3RegionValue(qt::L("discard#folder/child\\name.part.ext")) ==
          qt::L("name.part"));
}

TEST_CASE(type3_region_value_preserves_utf16_surrogate_units_around_ascii_delimiters)
{
    const qt::String input(std::u16string{u'r', u'/', 0xd83d, 0xdca9, u'n', u'a', u'm', u'e', u'.', u'o', u'p', u't'});
    const qt::String expected(std::u16string{0xd83d, 0xdca9, u'n', u'a', u'm', u'e'});
    CHECK(deriveType3RegionValue(input) == expected);
}

TEST_CASE(type3_region_value_preserves_null_on_noop_and_returns_nonnull_empty_after_removal)
{
    const qt::String nullValue;
    const auto unchanged = deriveType3RegionValue(nullValue);
    CHECK(unchanged.isNull());

    const auto emptied = deriveType3RegionValue(qt::L("#"));
    CHECK(emptied.isEmpty());
    CHECK(!emptied.isNull());
}

int main() { return testkit::run_all(); }
