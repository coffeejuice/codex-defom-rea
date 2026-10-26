// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include "m2m/engine/editor_setup.hpp"

using namespace m2m::engine::simcontrol300;

TEST_CASE(editor_setup_heat_mode_toggle_updates_only_heat_combo_requested_enable)
{
    EditorSetupState state;
    REQUIRE(state.bindComboModels(SetupComboModelProfile::FreshDefaultQtModel) ==
        SetupAdvanceResult::Applied);
    REQUIRE(state.translateConsumedControls(SetupTranslationProfile::NoActiveTranslator) ==
        SetupAdvanceResult::Applied);

    REQUIRE(state.control(0xa8)->identity.objectName == "modeHeatChckBx");
    REQUIRE(state.control(0xb8)->identity.objectName == "heatCmBx");
    REQUIRE(state.control(0xb8)->requestedEnabled == false);
    CHECK(!state.control(0xb8)->enabled); // effective parent-tree state is unknown
    CHECK(!state.control(0xa8)->checked); // no explicit checked setter is qualified
    REQUIRE(state.control(0xb8)->comboItems);
    const auto items = *state.control(0xb8)->comboItems;
    REQUIRE(items.size() == 3);
    REQUIRE(state.control(0xb8)->comboCurrentIndex == 0);

    CHECK(state.receiveInternalToggled(0xa8, true).empty());
    REQUIRE(state.installInternalConnectionProjection());
    CHECK(state.control(0xb8)->requestedEnabled == false); // connect does not synchronize
    CHECK(!state.control(0xb8)->enabled);

    const auto enabled = state.receiveInternalToggled(0xa8, true);
    REQUIRE(enabled.size() == 1);
    CHECK(enabled[0].sender == 0xa8);
    CHECK(enabled[0].receiver == 0xb8);
    CHECK(enabled[0].requestedEnabled);
    CHECK(state.control(0xb8)->requestedEnabled == true);
    CHECK(!state.control(0xb8)->enabled);
    CHECK(state.control(0xa8)->checked == std::nullopt);
    CHECK(*state.control(0xb8)->comboItems == items);
    CHECK(state.control(0xb8)->comboCurrentIndex == 0);

    const auto disabled = state.receiveInternalToggled(0xa8, false);
    REQUIRE(disabled.size() == 1);
    CHECK(disabled[0].sender == 0xa8);
    CHECK(disabled[0].receiver == 0xb8);
    CHECK(!disabled[0].requestedEnabled);
    CHECK(state.control(0xb8)->requestedEnabled == false);
    CHECK(!state.control(0xb8)->enabled);
    CHECK(state.control(0xa8)->checked == std::nullopt);
    CHECK(*state.control(0xb8)->comboItems == items);
    CHECK(state.control(0xb8)->comboCurrentIndex == 0);
}

int main() { return testkit::run_all(); }
