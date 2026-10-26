// ctest-labels: m2m-doe m2m-dblist
#include <testkit/check.hpp>

#include <cstdint>
#include <vector>

#include "m2m/doe_dblist_warning_receipt.hpp"

using namespace m2m;

TEST_CASE(doe_dblist_warning_receipt_preserves_title_prefix_names_and_parent)
{
    const std::vector<qt::String> missing{
        qt::L("../../../first.DB"), qt::L("../second.DB")};
    constexpr std::uintptr_t mainWindowToken = 0x12345678u;

    const auto receipt = doe::makeDbListWarningReceipt(true, 2, missing, mainWindowToken);

    CHECK(receipt.shouldDisplay);
    CHECK(receipt.title == qt::L("Error in reading"));
    CHECK(receipt.messagePrefix == qt::L(
        "The following Database files referred to in the Session file could not be found:\n\n"));
    REQUIRE(receipt.missingDbNames.size() == 2);
    CHECK(receipt.missingDbNames[0] == qt::L("../../../first.DB"));
    CHECK(receipt.missingDbNames[1] == qt::L("../second.DB"));
    REQUIRE(receipt.parentWidgetToken.has_value());
    CHECK_EQ(*receipt.parentWidgetToken, mainWindowToken);
}

TEST_CASE(doe_dblist_warning_receipt_state_five_suppresses_only_warning_receipt)
{
    const std::vector<qt::String> missing{qt::L("missing.DB")};

    const auto receipt = doe::makeDbListWarningReceipt(true, 5, missing, std::nullopt);

    CHECK(!receipt.shouldDisplay);
    CHECK(receipt.title.isNull());
    CHECK(receipt.messagePrefix.isNull());
    CHECK(receipt.missingDbNames.empty());
    CHECK(!receipt.parentWidgetToken.has_value());
}

TEST_CASE(doe_dblist_warning_receipt_requires_missing_path_and_keeps_nonfive_states)
{
    const std::vector<qt::String> missing{qt::L("missing.DB")};

    const auto success = doe::makeDbListWarningReceipt(false, 0, missing, 0x55u);
    CHECK(!success.shouldDisplay);
    CHECK(success.title.isNull());
    CHECK(success.missingDbNames.empty());

    const auto otherMode = doe::makeDbListWarningReceipt(true, 4, missing, 0x66u);
    CHECK(otherMode.shouldDisplay);
    REQUIRE(otherMode.parentWidgetToken.has_value());
    CHECK_EQ(*otherMode.parentWidgetToken, static_cast<std::uintptr_t>(0x66u));
}

int main() { return testkit::run_all(); }
