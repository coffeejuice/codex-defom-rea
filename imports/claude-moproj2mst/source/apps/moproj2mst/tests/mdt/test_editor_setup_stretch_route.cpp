// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include "m2m/engine/editor_setup.hpp"

using namespace m2m::engine::simcontrol300;

TEST_CASE(editor_setup_stretch_toggle_updates_only_group_requested_enable)
{
    EditorSetupState state;
    REQUIRE(state.bindComboModels(SetupComboModelProfile::FreshDefaultQtModel) ==
        SetupAdvanceResult::Applied);
    REQUIRE(state.translateConsumedControls(SetupTranslationProfile::NoActiveTranslator) ==
        SetupAdvanceResult::Applied);

    REQUIRE(state.control(0x1538)->identity.objectName == "m_chkbxSTRETCH");
    REQUIRE(state.control(0x1540)->identity.objectName == "groupBox72_2_2_2");
    REQUIRE(state.control(0x1540)->requestedEnabled == false); // setupUi setEnabled(false)
    CHECK(!state.control(0x1540)->enabled); // effective parent-tree state is unknown
    CHECK(state.control(0x1538)->checked == std::nullopt); // no explicit checked setter

    CHECK(state.receiveInternalToggled(0x1538, true).empty());
    REQUIRE(state.installInternalConnectionProjection());
    CHECK(state.control(0x1540)->requestedEnabled == false); // connection does not synchronize
    CHECK(!state.control(0x1540)->enabled);

    const auto enabled = state.receiveInternalToggled(0x1538, true);
    REQUIRE(enabled.size() == 1);
    CHECK(enabled[0].sender == 0x1538);
    CHECK(enabled[0].receiver == 0x1540);
    CHECK(enabled[0].requestedEnabled);
    CHECK(state.control(0x1540)->requestedEnabled == true);
    CHECK(!state.control(0x1540)->enabled);
    CHECK(state.control(0x1538)->checked == std::nullopt);

    const auto disabled = state.receiveInternalToggled(0x1538, false);
    REQUIRE(disabled.size() == 1);
    CHECK(disabled[0].sender == 0x1538);
    CHECK(disabled[0].receiver == 0x1540);
    CHECK(!disabled[0].requestedEnabled);
    CHECK(state.control(0x1540)->requestedEnabled == false);
    CHECK(!state.control(0x1540)->enabled);
    CHECK(state.control(0x1538)->checked == std::nullopt);
}

int main() { return testkit::run_all(); }
