// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include <algorithm>
#include <iterator>
#include <limits>
#include <set>
#include <stdexcept>
#include <string>
#include "m2m/engine/editor_setup.hpp"
#include "m2m/engine/simcontrol300_owner.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include <defmodel/layout/db_step_data.hpp>

using namespace m2m::engine::simcontrol300;

namespace {
struct HandoffIo final : Extra3DIo {
    HandoffIo() : Extra3DIo(1252) {}
    int reads = 0, removes = 0;
    bool readDoubles(const m2m::qt::String&, std::span<double> out) override
    { ++reads; std::fill(out.begin(), out.end(), 0.0); return false; }
    bool readInts(const m2m::qt::String&, std::span<int> out) override
    { ++reads; std::fill(out.begin(), out.end(), 0); return false; }
    bool readSprings(std::vector<SpringRecord>& out) override
    { ++reads; out.clear(); return false; }
    bool removeIfReadable(const m2m::qt::String&) override { ++removes; return true; }
    void nbcRemoveWarning() override {}
    std::unique_ptr<m2m::mst::OutputDevice> openWrite(const m2m::qt::String&) override
    { return nullptr; }
};
struct HandoffFixture {
    m2m::Project project;
    m2m::Operation operation;
    RemeshState remesh;
    HandoffFixture()
    {
        operation.project = &project;
        operation.engineModel = std::make_shared<m2m::engine::GuiModel>(nullptr, 3);
        auto& sc = *operation.engineModel->simCtl();
        sc.unit = 1; sc.nstart = 7; sc.nstep = 23;
        sc.smode = 99; sc.trans[0] = sc.trans[1] = 1;
    }
    Editor300Binding binding() { return {project, operation, *operation.engineModel, remesh}; }
    InitialEditorContext context(bool observations = false)
    {
        InitialEditorContext result{generalContextForMo(project, 3, {true, true}),
            {5, {}, MoStepCountContext{&operation}}, NumericLocale::KnownC, true,
            std::nullopt, StoppingLabelProfile::AsciiNoActiveTranslator};
        if (observations) {
            MoInitialObservations features;
            features.generalFills[0] = MoFeatureAvailability{1, std::nullopt, {1, 1, 1}};
            features.generalFills[1] = MoFeatureAvailability{1, std::nullopt, {1, 1, 1}};
            features.startupHeating = {0, 1};
            result.moObservations = features;
        }
        return result;
    }
};

// Omission codes bind the same real setup stages but leave one prerequisite
// unknown. No fixture field can synthesize a native setup-completion receipt.
void prepareHandoffSetup(EditorSetupState& state, int omit = -1)
{
    if (omit != 0)
        REQUIRE(state.bindComboModels(SetupComboModelProfile::FreshDefaultQtModel) == SetupAdvanceResult::Applied);
    if (omit != 1)
        REQUIRE(state.bindSpinControls(SetupSpinProfile::FreshNative) == SetupAdvanceResult::Applied);
    if (omit != 2)
        REQUIRE(state.bindStoppingControls(SetupStoppingProfile::FreshNativeTree) == SetupAdvanceResult::Applied);
    if (omit == 0 || omit == 3) return;
    REQUIRE(state.translateConsumedControls(SetupTranslationProfile::NoActiveTranslator) == SetupAdvanceResult::Applied);
    if (omit == 4) return;
    REQUIRE(state.installInternalConnectionProjection());
    REQUIRE(state.recordFinalIndexProjection() == SetupAdvanceResult::Applied);
    REQUIRE(state.recordAutomaticSlotProjection() == SetupAdvanceResult::Applied);
}
}

TEST_CASE(editor_setup_retains_native_slot_identity_without_inventing_widgets)
{
    EditorSetupState state;
    CHECK(editorSetupIdentities().size() == 681);
    std::set<SetupControlId> ids;
    for (const auto& identity : editorSetupIdentities()) {
        CHECK(ids.insert(identity.id).second);
        REQUIRE(state.control(identity.id));
        CHECK(state.control(identity.id)->identity.objectName == identity.objectName);
    }
    CHECK(state.control(0xa08)->identity.objectName == "m_rbtnInward");
    CHECK(state.control(0x1800)->identity.objectName == "m_tvPPVarList");
    CHECK(!state.control(0x1808));
    CHECK(!state.control(0x140)->checked);
    CHECK(!EditorSetupState::completeForNativeSetup);
    CHECK(!EditorSetupState::completeForPublicSave);
}

TEST_CASE(editor_setup_remesh_groups_retain_initial_checked_and_enable_state)
{
    EditorSetupState state;
    CHECK(state.control(0xa00)->checked == true);
    CHECK(state.control(0xa08)->checked == false);
    CHECK(state.control(0xa40)->checked == false);
    CHECK(state.control(0xa48)->checked == true);
    CHECK(!state.control(0xa08)->requestedChecked); // Qt constructor evidence
    CHECK(state.control(0x9e0)->requestedEnabled == true);
    CHECK(state.control(0x9f0)->requestedEnabled == true);
    CHECK(state.control(0xa10)->requestedEnabled == false);
    CHECK(!state.control(0xa40)->requestedEnabled); // no inferred Qt default
    CHECK(!state.control(0xa48)->requestedEnabled);
}

TEST_CASE(editor_setup_keeps_unresolved_defaults_separate_from_explicit_calls)
{
    EditorSetupState state;
    CHECK(state.control(0x148)->minimum == 1);
    CHECK(state.control(0x148)->maximum == 9999);
    CHECK(state.control(0x148)->value == 1);
    CHECK(state.control(0x158)->minimum == 1);
    CHECK(!state.control(0x158)->value); // range clamping needs Qt value state
    CHECK(state.control(0x1718)->minimum == -999999);
    CHECK(state.control(0x1718)->maximum == 999999);
    CHECK(state.control(0x1740)->singleStep == 10);
    CHECK(state.control(0x1298)->requestedChecked == true);
    CHECK(state.control(0x1298)->requestedEnabled == false);
    CHECK(!state.control(0x1298)->checked); // not a recovered full widget state
}

TEST_CASE(editor_setup_table_minimum_calls_do_not_become_guessed_counts)
{
    EditorSetupState state;
    CHECK(state.control(0xfa0)->columnCountLowerBound == 2);
    CHECK(!state.control(0xfa0)->columnCount);
    CHECK(state.control(0xfc0)->columnCountLowerBound == 1);
    CHECK(!state.control(0xfc0)->columnCount);
    CHECK(state.control(0xfe0)->columnCountLowerBound == 1);
    CHECK(state.control(0x1800)->columnCount == 3); // later unconditional call
    int tableCalls = 0;
    for (const auto& call : editorSetupPrefixCalls())
        if (call.control == 0x1800 && call.property == SetupProperty::ColumnCount)
            ++tableCalls;
    CHECK(tableCalls == 2);
}

TEST_CASE(editor_setup_connection_installation_does_not_emit_or_synchronize)
{
    EditorSetupState state;
    CHECK(state.receiveInternalToggled(0xa08, true).empty());
    CHECK(state.control(0xa10)->requestedEnabled == false);
    CHECK(!state.installInternalConnectionProjection());
    REQUIRE(state.bindComboModels(SetupComboModelProfile::FreshDefaultQtModel) == SetupAdvanceResult::Applied);
    REQUIRE(state.translateConsumedControls(SetupTranslationProfile::NoActiveTranslator) == SetupAdvanceResult::Applied);
    CHECK(state.installInternalConnectionProjection());
    CHECK(state.internalConnectionProjectionInstalled());
    CHECK(state.control(0xa10)->requestedEnabled == false);
    CHECK(!state.installInternalConnectionProjection());
    const auto writes = state.receiveInternalToggled(0xa08, true);
    REQUIRE(writes.size() == 1);
    CHECK(writes[0].receiver == 0xa10);
    CHECK(state.control(0xa10)->requestedEnabled == true);
    // Delivery observes a native signal, not a replacement for setChecked.
    CHECK(state.control(0xa08)->checked == false);
    CHECK(state.control(0xa00)->checked == true);
}

TEST_CASE(editor_setup_internal_duplicate_delivers_twice_without_coalescing)
{
    EditorSetupState state;
    REQUIRE(state.bindComboModels(SetupComboModelProfile::FreshDefaultQtModel) == SetupAdvanceResult::Applied);
    REQUIRE(state.translateConsumedControls(SetupTranslationProfile::NoActiveTranslator) == SetupAdvanceResult::Applied);
    REQUIRE(state.installInternalConnectionProjection());
    const auto writes = state.receiveInternalToggled(0x15f0, false);
    REQUIRE(writes.size() == 2);
    CHECK(writes[0].receiver == 0x1600);
    CHECK(writes[1].receiver == 0x1600);
    CHECK(!writes[0].requestedEnabled);
    CHECK(!writes[1].requestedEnabled);
    CHECK(state.control(0x1600)->requestedEnabled == false);
    CHECK(state.control(0x15f0)->requestedChecked == true);
    CHECK(editorSetupInternalConnections().size() == 14);
}

TEST_CASE(editor_setup_internal_routes_preserve_owned_receiver_independence)
{
    EditorSetupState first, second;
    REQUIRE(first.bindComboModels(SetupComboModelProfile::FreshDefaultQtModel) == SetupAdvanceResult::Applied);
    REQUIRE(first.translateConsumedControls(SetupTranslationProfile::NoActiveTranslator) == SetupAdvanceResult::Applied);
    REQUIRE(first.installInternalConnectionProjection());
    REQUIRE(second.bindComboModels(SetupComboModelProfile::FreshDefaultQtModel) == SetupAdvanceResult::Applied);
    REQUIRE(second.translateConsumedControls(SetupTranslationProfile::NoActiveTranslator) == SetupAdvanceResult::Applied);
    REQUIRE(second.installInternalConnectionProjection());
    const auto writes = first.receiveInternalToggled(0xa48, false);
    REQUIRE(writes.size() == 1);
    CHECK(writes[0].receiver == 0x9e0);
    CHECK(first.control(0x9e0)->requestedEnabled == false);
    CHECK(second.control(0x9e0)->requestedEnabled == true);
    CHECK(first.control(0xa10)->requestedEnabled == false);
    CHECK(first.receiveInternalToggled(0xffff, true).empty());
}

TEST_CASE(editor_setup_final_indices_stay_after_translation_and_connections)
{
    CHECK(editorSetupPrefixCalls().size() == 69);
    CHECK(std::none_of(editorSetupPrefixCalls().begin(), editorSetupPrefixCalls().end(),
        [](const auto& call) { return call.property == SetupProperty::CurrentIndex; }));
    const auto tail = editorSetupFinalIndexCalls();
    REQUIRE(tail.size() == 10);
    CHECK(tail.front().control == 0x10);
    CHECK(tail[2].control == 0x1d0);
    CHECK(tail[2].value == 1);
    CHECK(tail.back().control == 0x1700);
    CHECK(tail.back().value == 0);
    CHECK(tail.back().nativeReturnAddress == 0x140f21d5f);
}

TEST_CASE(editor_setup_automatic_slots_are_plans_not_synthetic_callbacks)
{
    EditorSetupState state;
    const auto slots = editorSetupAutomaticSlots();
    REQUIRE(slots.size() == 33);
    const auto found = std::find_if(slots.begin(), slots.end(),
        [](const auto& slot) { return slot.slot == "on_m_chbxDiffBnd_stateChanged(int)"; });
    REQUIRE(found != slots.end());
    CHECK(found->signal == "stateChanged(int)");
    CHECK(state.control(found->sender)->identity.objectName == "m_chbxDiffBnd");
    REQUIRE(state.bindComboModels(SetupComboModelProfile::FreshDefaultQtModel) == SetupAdvanceResult::Applied);
    REQUIRE(state.translateConsumedControls(SetupTranslationProfile::NoActiveTranslator) == SetupAdvanceResult::Applied);
    REQUIRE(state.installInternalConnectionProjection());
    CHECK(state.receiveInternalToggled(found->sender, true).empty());
    CHECK(!state.control(found->sender)->checked);
    CHECK(!EditorSetupState::completeForNativeSetup);
}

TEST_CASE(editor_setup_unknown_translation_preserves_prefix_and_blocks_later_stages)
{
    EditorSetupState state;
    CHECK(state.translateConsumedControls(SetupTranslationProfile::Unknown) == SetupAdvanceResult::UnknownTranslation);
    CHECK(state.progress() == SetupProgress::Prefix);
    CHECK(state.consumedTranslationActionsCompleted() == 0);
    CHECK(state.control(0xa00)->checked == true);
    CHECK(state.control(0xa10)->requestedEnabled == false);
    CHECK(!state.control(0xb8)->comboItems);
    CHECK(!state.control(0xb18)->text);
    CHECK(!state.installInternalConnectionProjection());
    CHECK(state.recordFinalIndexProjection() == SetupAdvanceResult::WrongStage);
    CHECK(state.recordAutomaticSlotProjection() == SetupAdvanceResult::WrongStage);
    CHECK(!state.control(0x1d0)->requestedCurrentIndex);
    REQUIRE(state.bindComboModels(SetupComboModelProfile::FreshDefaultQtModel) == SetupAdvanceResult::Applied);
    REQUIRE(state.translateConsumedControls(SetupTranslationProfile::NoActiveTranslator) == SetupAdvanceResult::Applied);
    CHECK(state.progress() == SetupProgress::ConsumedControlsTranslated);
    CHECK(state.consumedTranslationActionsCompleted() == 29);
}

TEST_CASE(editor_setup_explicit_untranslated_profile_keeps_native_text_and_list_order)
{
    EditorSetupState state;
    REQUIRE(state.bindComboModels(SetupComboModelProfile::FreshDefaultQtModel) == SetupAdvanceResult::Applied);
    REQUIRE(state.translateConsumedControls(SetupTranslationProfile::NoActiveTranslator) == SetupAdvanceResult::Applied);
    const auto& heating = state.control(0xb8)->comboItems;
    REQUIRE(heating);
    REQUIRE(heating->size() == 3);
    CHECK((*heating)[0] == m2m::qt::String(u"Resistance"));
    CHECK((*heating)[1] == m2m::qt::String(u"Induction"));
    CHECK((*heating)[2] == m2m::qt::String(u"Induction (BEM)"));
    const auto& pp = state.control(0x17b8)->comboItems;
    REQUIRE(pp);
    REQUIRE(pp->size() == 9);
    CHECK(pp->front() == m2m::qt::String(u"Time Between Temperatures"));
    CHECK(pp->back() == m2m::qt::String(u"Strain Above Strain Rate"));
    CHECK(state.control(0xb18)->text == m2m::qt::String(u"0.0"));
    CHECK(state.control(0xb28)->text == m2m::qt::String(u"1.0"));
    CHECK(state.control(0xb38)->text == m2m::qt::String(u"100"));
    CHECK(state.control(0xb48)->text == m2m::qt::String(u"0"));
    CHECK(state.control(0x6d0)->text == m2m::qt::String(u"0"));
    CHECK(!state.control(0xa10)->text); // empty Qt default still unmodeled
    CHECK(!state.control(0xb8)->requestedCurrentIndex); // no explicit setter argument
    CHECK(state.control(0xb8)->comboCurrentIndex == 0); // native model callback
    CHECK(!state.internalConnectionProjectionInstalled());
}

TEST_CASE(editor_setup_stage_order_never_replays_translations_or_fabricates_callbacks)
{
    EditorSetupState state;
    REQUIRE(state.bindComboModels(SetupComboModelProfile::FreshDefaultQtModel) == SetupAdvanceResult::Applied);
    REQUIRE(state.translateConsumedControls(SetupTranslationProfile::NoActiveTranslator) == SetupAdvanceResult::Applied);
    CHECK(state.recordFinalIndexProjection() == SetupAdvanceResult::WrongStage);
    REQUIRE(state.installInternalConnectionProjection());
    CHECK(state.recordAutomaticSlotProjection() == SetupAdvanceResult::WrongStage);
    REQUIRE(state.recordFinalIndexProjection() == SetupAdvanceResult::Applied);
    CHECK(state.control(0x1d0)->requestedCurrentIndex == 1);
    CHECK(state.control(0x1700)->requestedCurrentIndex == 0);
    CHECK(state.progress() == SetupProgress::FinalIndicesRecorded);
    REQUIRE(state.recordAutomaticSlotProjection() == SetupAdvanceResult::Applied);
    CHECK(state.progress() == SetupProgress::AutomaticSlotsRecorded);
    CHECK(state.recordAutomaticSlotProjection() == SetupAdvanceResult::WrongStage);
    CHECK(state.recordFinalIndexProjection() == SetupAdvanceResult::WrongStage);
    CHECK(state.translateConsumedControls(SetupTranslationProfile::NoActiveTranslator) == SetupAdvanceResult::WrongStage);
    CHECK(state.consumedTranslationActionsCompleted() == 29);
    CHECK(!state.control(0x1298)->checked);
    CHECK(!EditorSetupState::completeForNativeSetup);
    CHECK(!EditorSetupState::completeForPublicSave);
}

TEST_CASE(editor_setup_consumed_text_belongs_to_its_original_setup_owner)
{
    EditorSetupState first, second;
    REQUIRE(first.bindComboModels(SetupComboModelProfile::FreshDefaultQtModel) == SetupAdvanceResult::Applied);
    REQUIRE(first.translateConsumedControls(SetupTranslationProfile::NoActiveTranslator) == SetupAdvanceResult::Applied);
    CHECK(!second.control(0xb28)->text);
    CHECK(!second.control(0x1360)->comboItems);
    REQUIRE(first.installInternalConnectionProjection());
    REQUIRE(first.receiveInternalToggled(0xa48, false).size() == 1);
    CHECK(first.control(0x9e0)->requestedEnabled == false);
    CHECK(first.translateConsumedControls(SetupTranslationProfile::Unknown) == SetupAdvanceResult::WrongStage);
    CHECK(first.control(0x9e0)->requestedEnabled == false);
    CHECK(second.control(0x9e0)->requestedEnabled == true);
    CHECK(second.progress() == SetupProgress::Prefix);
}

TEST_CASE(editor_setup_unqualified_or_custom_combo_models_refuse_without_effects)
{
    EditorSetupState state;
    CHECK(state.comboModelProfile() == SetupComboModelProfile::Unknown);
    CHECK(!state.control(0xb8)->comboCurrentIndex);
    CHECK(state.bindComboModels(SetupComboModelProfile::Unknown) == SetupAdvanceResult::UnknownComboModel);
    CHECK(state.bindComboModels(SetupComboModelProfile::CustomModel) == SetupAdvanceResult::UnsupportedComboModel);
    CHECK(state.translateConsumedControls(SetupTranslationProfile::NoActiveTranslator) == SetupAdvanceResult::UnknownComboModel);
    CHECK(state.comboModelProfile() == SetupComboModelProfile::Unknown);
    CHECK(state.progress() == SetupProgress::Prefix);
    CHECK(state.consumedTranslationActionsCompleted() == 0);
    CHECK(!state.control(0xb8)->comboItems);
    CHECK(!state.control(0xb8)->comboCurrentIndex);
    CHECK(!state.control(0xb18)->text);
    CHECK(state.control(0xa00)->checked == true);
    CHECK(state.control(0xa10)->requestedEnabled == false);
    CHECK(!state.installInternalConnectionProjection());
    REQUIRE(state.bindComboModels(SetupComboModelProfile::FreshDefaultQtModel) == SetupAdvanceResult::Applied);
    CHECK(state.comboModelProfile() == SetupComboModelProfile::FreshDefaultQtModel);
    CHECK(state.control(0xb8)->comboCurrentIndex == -1);
}

TEST_CASE(editor_setup_fresh_native_combos_start_empty_with_invalid_selection)
{
    EditorSetupState state;
    REQUIRE(state.bindComboModels(SetupComboModelProfile::FreshDefaultQtModel) == SetupAdvanceResult::Applied);
    constexpr SetupControlId expected[] = {
        0xb8, 0x1f8, 0x250, 0x708, 0x7a0, 0x7c0, 0x838, 0x9b8,
        0xaa8, 0xad8, 0x10c0, 0x1360, 0x1388, 0x13b0, 0x13e0,
        0x1408, 0x1430, 0x1650, 0x1688, 0x16c8, 0x17b8
    };
    std::size_t resolved = 0;
    for (const auto& identity : editorSetupIdentities()) {
        const auto& control = *state.control(identity.id);
        const bool isCombo = std::find(std::begin(expected), std::end(expected), identity.id) != std::end(expected);
        CHECK(control.comboCurrentIndex.has_value() == isCombo);
        CHECK(control.comboItems.has_value() == isCombo);
        if (!isCombo) continue;
        ++resolved;
        CHECK(control.comboCurrentIndex == -1);
        REQUIRE(control.comboItems);
        CHECK(control.comboItems->empty());
        CHECK(!control.requestedCurrentIndex);
    }
    CHECK(resolved == 21);
    CHECK(state.translateConsumedControls(SetupTranslationProfile::Unknown) == SetupAdvanceResult::UnknownTranslation);
    CHECK(state.progress() == SetupProgress::Prefix);
    CHECK(state.consumedTranslationActionsCompleted() == 0);
    CHECK(state.control(0x17b8)->comboCurrentIndex == -1);
    REQUIRE(state.control(0x17b8)->comboItems);
    CHECK(state.control(0x17b8)->comboItems->empty());
}

TEST_CASE(editor_setup_insertion_selects_only_thirteen_populated_native_models)
{
    EditorSetupState state;
    REQUIRE(state.bindComboModels(SetupComboModelProfile::FreshDefaultQtModel) == SetupAdvanceResult::Applied);
    REQUIRE(state.translateConsumedControls(SetupTranslationProfile::NoActiveTranslator) == SetupAdvanceResult::Applied);
    constexpr SetupControlId populated[] = {
        0xb8, 0x1f8, 0x250, 0x7c0, 0xaa8, 0xad8, 0x1360,
        0x1388, 0x13b0, 0x13e0, 0x1408, 0x1430, 0x17b8
    };
    constexpr SetupControlId empty[] = { 0x708, 0x7a0, 0x838, 0x9b8, 0x10c0, 0x1650, 0x1688, 0x16c8 };
    for (const auto id : populated) {
        const auto& control = *state.control(id);
        REQUIRE(control.comboItems);
        CHECK(!control.comboItems->empty());
        CHECK(control.comboCurrentIndex == 0);
        CHECK(!control.requestedCurrentIndex);
    }
    for (const auto id : empty) {
        const auto& control = *state.control(id);
        REQUIRE(control.comboItems);
        CHECK(control.comboItems->empty());
        CHECK(control.comboCurrentIndex == -1);
        CHECK(!control.requestedCurrentIndex);
    }
    CHECK(!state.internalConnectionProjectionInstalled());
    CHECK(state.receiveInternalToggled(0xa48, false).empty());
    CHECK(state.control(0x9e0)->requestedEnabled == true);
    CHECK(!state.control(0x1d0)->requestedCurrentIndex);
    REQUIRE(state.installInternalConnectionProjection());
    REQUIRE(state.recordFinalIndexProjection() == SetupAdvanceResult::Applied);
    CHECK(state.control(0x1d0)->requestedCurrentIndex == 1);
    CHECK(!state.control(0x1d0)->comboCurrentIndex); // this is a stacked widget
    CHECK(state.control(0x17b8)->comboCurrentIndex == 0);
    CHECK(!state.control(0x17b8)->requestedCurrentIndex);
}

TEST_CASE(editor_setup_combo_binding_cannot_replay_or_replace_another_owner)
{
    EditorSetupState first, second;
    REQUIRE(first.bindComboModels(SetupComboModelProfile::FreshDefaultQtModel) == SetupAdvanceResult::Applied);
    CHECK(first.bindComboModels(SetupComboModelProfile::FreshDefaultQtModel) == SetupAdvanceResult::WrongStage);
    CHECK(first.bindComboModels(SetupComboModelProfile::CustomModel) == SetupAdvanceResult::WrongStage);
    REQUIRE(second.bindComboModels(SetupComboModelProfile::FreshDefaultQtModel) == SetupAdvanceResult::Applied);
    REQUIRE(first.translateConsumedControls(SetupTranslationProfile::NoActiveTranslator) == SetupAdvanceResult::Applied);
    CHECK(first.bindComboModels(SetupComboModelProfile::Unknown) == SetupAdvanceResult::WrongStage);
    CHECK(first.bindComboModels(SetupComboModelProfile::FreshDefaultQtModel) == SetupAdvanceResult::WrongStage);
    CHECK(first.control(0xb8)->comboCurrentIndex == 0);
    CHECK(second.control(0xb8)->comboCurrentIndex == -1);
    REQUIRE(first.control(0xb8)->comboItems);
    REQUIRE(second.control(0xb8)->comboItems);
    CHECK(first.control(0xb8)->comboItems->size() == 3);
    CHECK(second.control(0xb8)->comboItems->empty());
    CHECK(first.consumedTranslationActionsCompleted() == 29);
    CHECK(second.consumedTranslationActionsCompleted() == 0);
    CHECK(!EditorSetupState::completeForNativeSetup);
    CHECK(!EditorSetupState::completeForPublicSave);
}

TEST_CASE(editor_setup_spin_profile_refuses_unqualified_and_custom_controls)
{
    EditorSetupState state;
    CHECK(state.spinProfile() == SetupSpinProfile::Unknown);
    CHECK(!state.control(0x148)->spin);
    CHECK(state.bindSpinControls(SetupSpinProfile::Unknown) == SetupAdvanceResult::UnknownSpinControl);
    CHECK(state.bindSpinControls(SetupSpinProfile::CustomControl) == SetupAdvanceResult::UnsupportedSpinControl);
    CHECK(state.spinProfile() == SetupSpinProfile::Unknown);
    CHECK(state.progress() == SetupProgress::Prefix);
    CHECK(!state.control(0x158)->spin);
    CHECK(state.control(0x158)->minimum == 1); // explicit argument retained
    CHECK(!state.control(0x158)->value);
    CHECK(state.control(0x9e0)->requestedEnabled == true);
    CHECK(!state.internalConnectionProjectionInstalled());
    REQUIRE(state.bindSpinControls(SetupSpinProfile::FreshNative) == SetupAdvanceResult::Applied);
    CHECK(state.spinProfile() == SetupSpinProfile::FreshNative);
    CHECK(state.comboModelProfile() == SetupComboModelProfile::Unknown);
    CHECK(!state.control(0xb8)->comboItems);
}

TEST_CASE(editor_setup_native_spin_numeric_state_matches_all_eleven_controls)
{
    EditorSetupState state;
    REQUIRE(state.bindSpinControls(SetupSpinProfile::FreshNative) == SetupAdvanceResult::Applied);
    struct Expected { SetupControlId id; int minimum, maximum, value, step; };
    // Source-derived results of the11 native constructors and16 ordered calls.
    constexpr Expected expected[] = {
        {0x148, 1, 9999, 1, 1}, {0x158, 1, 9999, 1, 1},
        {0x6a8, 0, 999999, 0, 1}, {0x740, 0, 999999, 0, 1},
        {0x938, 0, 1000000000, 0, 1}, {0x11f8, 0, 99, 0, 1},
        {0x1200, 0, 99, 0, 1}, {0x1238, 0, 99, 0, 1},
        {0x1718, -999999, 999999, 0, 1}, {0x1740, 0, 999999, 0, 10},
        {0x1750, 1, 999999, 1, 10}
    };
    std::size_t resolved = 0;
    for (const auto& identity : editorSetupIdentities()) {
        const auto found = std::find_if(std::begin(expected), std::end(expected),
            [&](const Expected& item) { return item.id == identity.id; });
        const auto& spin = state.control(identity.id)->spin;
        CHECK(spin.has_value() == (found != std::end(expected)));
        if (found == std::end(expected)) continue;
        REQUIRE(spin);
        ++resolved;
        CHECK(spin->minimum == found->minimum);
        CHECK(spin->maximum == found->maximum);
        CHECK(spin->value == found->value);
        CHECK(spin->singleStep == found->step);
    }
    CHECK(resolved == 11);
    CHECK(!state.control(0x11f8)->minimum); // default is not an explicit setter
    CHECK(!state.control(0x11f8)->maximum);
    CHECK(!state.control(0x11f8)->value);
    CHECK(!state.control(0x11f8)->singleStep);
}

TEST_CASE(editor_setup_spin_minimum_clamps_without_an_explicit_value_setter)
{
    EditorSetupState state;
    REQUIRE(state.bindSpinControls(SetupSpinProfile::FreshNative) == SetupAdvanceResult::Applied);
    REQUIRE(state.control(0x158)->spin);
    REQUIRE(state.control(0x1750)->spin);
    CHECK(state.control(0x158)->spin->value == 1);
    CHECK(state.control(0x1750)->spin->value == 1);
    CHECK(!state.control(0x158)->value);
    CHECK(!state.control(0x1750)->value);
    CHECK(state.control(0x148)->value == 1);
    REQUIRE(state.control(0x1740)->spin);
    CHECK(state.control(0x1740)->spin->singleStep == 10);
    CHECK(state.control(0x1740)->spin->value == 0); // step is not a value change
    REQUIRE(state.control(0x1718)->spin);
    CHECK(state.control(0x1718)->spin->minimum == -999999);
    CHECK(state.control(0x1718)->spin->value == 0); // lowered minimum retains0
    CHECK(state.receiveInternalToggled(0xa48, false).empty());
    CHECK(state.control(0x9e0)->requestedEnabled == true);
    CHECK(state.consumedTranslationActionsCompleted() == 0);
    CHECK(!state.internalConnectionProjectionInstalled());
}

TEST_CASE(editor_setup_spin_binding_is_owned_and_cannot_replay_after_translation)
{
    EditorSetupState first, second;
    REQUIRE(first.bindSpinControls(SetupSpinProfile::FreshNative) == SetupAdvanceResult::Applied);
    CHECK(first.bindSpinControls(SetupSpinProfile::FreshNative) == SetupAdvanceResult::WrongStage);
    CHECK(first.bindSpinControls(SetupSpinProfile::CustomControl) == SetupAdvanceResult::WrongStage);
    CHECK(!second.control(0x158)->spin);
    REQUIRE(first.bindComboModels(SetupComboModelProfile::FreshDefaultQtModel) == SetupAdvanceResult::Applied);
    REQUIRE(second.bindComboModels(SetupComboModelProfile::FreshDefaultQtModel) == SetupAdvanceResult::Applied);
    REQUIRE(first.translateConsumedControls(SetupTranslationProfile::NoActiveTranslator) == SetupAdvanceResult::Applied);
    REQUIRE(second.translateConsumedControls(SetupTranslationProfile::NoActiveTranslator) == SetupAdvanceResult::Applied);
    CHECK(first.bindSpinControls(SetupSpinProfile::Unknown) == SetupAdvanceResult::WrongStage);
    CHECK(second.bindSpinControls(SetupSpinProfile::FreshNative) == SetupAdvanceResult::WrongStage);
    CHECK(!second.control(0x158)->spin);
    REQUIRE(first.control(0x158)->spin);
    CHECK(first.control(0x158)->spin->value == 1);
    CHECK(first.spinProfile() == SetupSpinProfile::FreshNative);
    CHECK(second.spinProfile() == SetupSpinProfile::Unknown);
    CHECK(!EditorSetupState::completeForNativeSetup);
    CHECK(!EditorSetupState::completeForPublicSave);
}

TEST_CASE(editor_setup_mds_parent_tree_requires_explicit_fresh_profile)
{
    EditorSetupState state;
    CHECK(!state.control(0x6a0)->checked);
    CHECK(!state.control(0x6a0)->enabled);
    CHECK(state.control(0x6b8)->requestedEnabled == false);
    CHECK(!state.control(0x6b8)->enabled);
    CHECK(state.bindStoppingControls(SetupStoppingProfile::Unknown) == SetupAdvanceResult::UnknownStoppingControl);
    CHECK(state.bindStoppingControls(SetupStoppingProfile::CustomControls) == SetupAdvanceResult::UnsupportedStoppingControl);
    CHECK(!state.control(0x6a0)->checked);
    REQUIRE(state.bindStoppingControls(SetupStoppingProfile::FreshNativeTree) == SetupAdvanceResult::Applied);
    for (const auto id : {0x6a0, 0x738}) {
        CHECK(state.control(id)->checked == true);
        CHECK(state.control(id)->enabled == true);
    }
    for (const auto id : {0x6b0, 0x748}) CHECK(state.control(id)->checked == false);
    for (const auto id : {0x6a8, 0x740}) CHECK(state.control(id)->enabled == true);
    for (const auto id : {0x6b8, 0x750}) CHECK(state.control(id)->enabled == false);
    CHECK(!state.control(0x6d0)->enabled); // no invented whole widget tree
    CHECK(state.bindStoppingControls(SetupStoppingProfile::FreshNativeTree) == SetupAdvanceResult::WrongStage);
}

TEST_CASE(editor_setup_factory_binding_preserves_allocation_order_and_exact_identity)
{
    HandoffFixture f;
    StagedEditor300 factory(f.binding());
    CHECK(!factory.setupWasBound()); CHECK(!factory.setupOwner());
    EditorSetupState setup, replacement;
    REQUIRE(factory.bindSetupOwner(setup));
    CHECK(factory.setupOwner() == &setup);
    CHECK(setup.factoryOwnerIdentity() == &factory);
    CHECK(!setup.pageOwnerIdentity()); CHECK(!setup.sealedForPages());
    CHECK(!factory.bindSetupOwner(setup));
    CHECK(factory.construction().setupBindIssue == Editor300SetupBindIssue::AlreadyBound);
    CHECK(!factory.bindSetupOwner(replacement));
    CHECK(factory.setupOwner() == &setup); CHECK(!replacement.factoryOwnerIdentity());
    StagedEditor300 other(f.binding());
    CHECK(!other.bindSetupOwner(setup));
    CHECK(other.construction().setupBindIssue == Editor300SetupBindIssue::SetupAlreadyOwned);
    CHECK(!other.setupWasBound()); CHECK(!other.setupOwner());
}

TEST_CASE(editor_setup_handoff_requires_exact_factory_and_retained_page_destination)
{
    HandoffFixture f;
    StagedEditor300 factory(f.binding()), other(f.binding());
    EditorSetupState setup;
    REQUIRE(factory.bindSetupOwner(setup)); prepareHandoffSetup(setup);
    InitialEditorSession unrelated;
    unrelated.pages.stopping.widgets.references[0].coordinates[0] = m2m::qt::L("retained");
    CHECK(setup.handoffToPages(other, unrelated, StoppingLabelProfile::AsciiNoActiveTranslator).issue ==
        SetupPageHandoffIssue::WrongFactory);
    CHECK(setup.handoffToPages(factory, unrelated, StoppingLabelProfile::AsciiNoActiveTranslator).issue ==
        SetupPageHandoffIssue::WrongDestination);
    CHECK(!setup.sealedForPages()); CHECK(!setup.pageOwnerIdentity());
    CHECK(unrelated.pages.stopping.widgets.references[0].coordinates[0] == m2m::qt::L("retained"));
    CHECK(!unrelated.constructionStarted);
}

TEST_CASE(editor_setup_missing_handoff_prerequisites_preserve_unsealed_setup_and_unstarted_pages)
{
    constexpr SetupPageHandoffIssue expected[]{
        SetupPageHandoffIssue::UnknownComboModel, SetupPageHandoffIssue::UnknownSpinControl,
        SetupPageHandoffIssue::UnknownStoppingControl, SetupPageHandoffIssue::UnknownTranslation,
        SetupPageHandoffIssue::WrongStage, SetupPageHandoffIssue::UnknownLabelProfile};
    for (int omit = 0; omit < 6; ++omit) {
        HandoffFixture f; StagedEditor300 factory(f.binding()); EditorSetupState setup; HandoffIo io;
        REQUIRE(factory.bindSetupOwner(setup)); prepareHandoffSetup(setup, omit);
        auto context = f.context();
        if (omit == 5) context.stoppingLabelProfile = StoppingLabelProfile::Unknown;
        const auto& result = factory.constructPageState(f.binding(), io, context);
        REQUIRE(result.setupHandoff);
        CHECK(result.setupHandoff->issue == expected[omit]);
        CHECK(result.bindingIssue == Editor300BindingIssue::SetupPrerequisite);
        CHECK(result.state == Editor300ConstructorState::PageRefused); CHECK(!result.pages);
        REQUIRE(factory.partialPageState()); CHECK(!factory.partialPageState()->constructionStarted);
        CHECK(!factory.partialPageState()->sourceModel); CHECK(!factory.partialPageState()->sourceRemesh);
        CHECK(factory.partialPageState()->pages.stopping.widgets.references[0].coordinates[0] == m2m::qt::L(""));
        CHECK(!setup.sealedForPages()); CHECK(!setup.pageOwnerIdentity());
        CHECK(io.reads == 0); CHECK(io.removes == 0);
        CHECK(f.operation.engineModel->simCtl()->smode == 99);
        factory.constructPageState(f.binding(), io, f.context());
        CHECK(result.repeatedConstructionRejected); CHECK(result.setupHandoff->issue == expected[omit]);
    }
}

TEST_CASE(editor_setup_handoff_retains_translated_controls_before_first_page_refusal)
{
    HandoffFixture f; StagedEditor300 factory(f.binding()); EditorSetupState setup; HandoffIo io;
    REQUIRE(factory.bindSetupOwner(setup)); prepareHandoffSetup(setup);
    const auto& result = factory.constructPageState(f.binding(), io, f.context());
    REQUIRE(result.setupHandoff); CHECK(result.setupHandoff->complete());
    REQUIRE(result.pages); CHECK(result.pages->issue == InitialOpenIssue::MissingMoObservation);
    CHECK(result.state == Editor300ConstructorState::PageRefused);
    const auto* pages = factory.partialPageState(); REQUIRE(pages);
    CHECK(setup.pageOwnerIdentity() == pages); CHECK(setup.sealedForPages());
    CHECK(pages->constructionStarted); CHECK(pages->sourceRemesh == &f.remesh);
    const auto& stopping = pages->pages.stopping;
    CHECK(stopping.widgets.labelProfile == StoppingLabelProfile::AsciiNoActiveTranslator);
    for (const auto& reference : stopping.widgets.references) {
        CHECK(reference.selectedObject == 0); CHECK(reference.node == 0);
        CHECK(reference.nodeChecked); CHECK(!reference.coordinateChecked);
        CHECK(reference.nodeEnabled); CHECK(reference.nodeSpinEnabled); CHECK(!reference.coordinatesEnabled);
        for (const auto& value : reference.coordinates) CHECK(value == m2m::qt::L("0"));
    }
    for (const auto& value : stopping.rotationVector) CHECK(value == m2m::qt::L("0"));
    for (const auto& value : stopping.rotationCenter) CHECK(value == m2m::qt::L("0"));
    CHECK(stopping.mdsModeCombo == 0); CHECK(stopping.referenceChoices.empty());
    CHECK(stopping.referenceFillOrder[0] == -1); CHECK(stopping.referenceFillOrder[1] == -1);
    CHECK(io.reads == 0); CHECK(f.operation.engineModel->simCtl()->smode == 99);
    CHECK(!factory.pageState()); CHECK(!StagedEditor300::completeForPublicSave);
}

TEST_CASE(editor_setup_handoff_preserves_reached_toggle_deliveries_without_synthesizing_radio_changes)
{
    HandoffFixture f; StagedEditor300 factory(f.binding()); EditorSetupState setup; HandoffIo io;
    REQUIRE(factory.bindSetupOwner(setup)); prepareHandoffSetup(setup);
    REQUIRE(setup.receiveInternalToggled(0x6a0, false).size() == 1);
    REQUIRE(setup.receiveInternalToggled(0x748, true).size() == 1);
    factory.constructPageState(f.binding(), io, f.context());
    REQUIRE(factory.construction().setupHandoff); REQUIRE(factory.construction().setupHandoff->complete());
    const auto& widgets = factory.partialPageState()->pages.stopping.widgets;
    CHECK(widgets.references[0].nodeChecked); CHECK(!widgets.references[0].nodeSpinEnabled);
    CHECK(!widgets.references[0].coordinatesEnabled);
    CHECK(widgets.references[1].nodeSpinEnabled); CHECK(widgets.references[1].coordinatesEnabled);
    CHECK(!widgets.references[1].coordinateChecked); // observed delivery is not setChecked
}

TEST_CASE(editor_setup_sealed_history_rejects_all_live_mutators)
{
    HandoffFixture f; StagedEditor300 factory(f.binding()); EditorSetupState setup; HandoffIo io;
    REQUIRE(factory.bindSetupOwner(setup)); prepareHandoffSetup(setup);
    factory.constructPageState(f.binding(), io, f.context()); REQUIRE(setup.sealedForPages());
    CHECK(setup.bindComboModels(SetupComboModelProfile::Unknown) == SetupAdvanceResult::HistoricalState);
    CHECK(setup.bindSpinControls(SetupSpinProfile::Unknown) == SetupAdvanceResult::HistoricalState);
    CHECK(setup.bindStoppingControls(SetupStoppingProfile::Unknown) == SetupAdvanceResult::HistoricalState);
    CHECK(setup.translateConsumedControls(SetupTranslationProfile::Unknown) == SetupAdvanceResult::HistoricalState);
    CHECK(!setup.installInternalConnectionProjection());
    CHECK(setup.recordFinalIndexProjection() == SetupAdvanceResult::HistoricalState);
    CHECK(setup.recordAutomaticSlotProjection() == SetupAdvanceResult::HistoricalState);
    bool rejected = false;
    try { setup.receiveInternalToggled(0x6a0, false); }
    catch (const std::logic_error& error) {
        rejected = std::string(error.what()).find("historical") != std::string::npos;
    }
    CHECK(rejected); CHECK(setup.constructorControl(0x6a8)->enabled == true);
    CHECK(setup.constructorControl(0x6d0)->text == m2m::qt::L("0"));
    InitialEditorSession other;
    CHECK(setup.handoffToPages(factory, other, StoppingLabelProfile::AsciiNoActiveTranslator).issue ==
        SetupPageHandoffIssue::AlreadySealed);
    CHECK(setup.pageOwnerIdentity() == factory.partialPageState());
    CHECK(!other.constructionStarted);
}

TEST_CASE(editor_setup_live_pages_keep_constructor_text_after_none_refills_and_outlive_history)
{
    HandoffFixture f; StagedEditor300 factory(f.binding()); HandoffIo io;
    auto setup = std::make_unique<EditorSetupState>();
    REQUIRE(factory.bindSetupOwner(*setup)); prepareHandoffSetup(*setup);
    const auto& result = factory.constructPageState(f.binding(), io, f.context(true));
    REQUIRE(result.pages); REQUIRE(result.pages->complete());
    REQUIRE(result.setupHandoff); REQUIRE(result.setupHandoff->complete());
    auto* pages = factory.pageState(); REQUIRE(pages);
    CHECK(setup->pageOwnerIdentity() == pages);
    for (const auto& reference : pages->pages.stopping.widgets.references)
        for (const auto& value : reference.coordinates) CHECK(value == m2m::qt::L("0"));
    pages->pages.stopping.widgets.references[0].coordinates[0] = m2m::qt::L("37.5");
    CHECK(setup->constructorControl(0x6d0)->text == m2m::qt::L("0"));
    CHECK(setup->control(0x6d0) == setup->constructorControl(0x6d0));
    const int previousReads = io.reads;
    factory.constructPageState(f.binding(), io, f.context(true));
    CHECK(result.repeatedConstructionRejected); CHECK(io.reads == previousReads);
    CHECK(pages->pages.stopping.widgets.references[0].coordinates[0] == m2m::qt::L("37.5"));
    setup.reset();
    CHECK(!factory.setupOwner()); CHECK(factory.setupWasBound());
    CHECK(factory.pageState() == pages); // transfer completed; no setup dereference
    CHECK(pages->pages.stopping.widgets.references[0].coordinates[0] == m2m::qt::L("37.5"));
    CHECK(!result.completeForPublicSave);
}

TEST_CASE(editor_setup_expired_owner_refuses_before_page_allocation_or_model_effects)
{
    HandoffFixture f; StagedEditor300 factory(f.binding()); HandoffIo io;
    EditorSetupGuard guard;
    {
        EditorSetupState setup; prepareHandoffSetup(setup);
        REQUIRE(factory.bindSetupOwner(setup)); guard.reset(&setup);
        CHECK(guard.get() == &setup);
    }
    CHECK(!guard.get()); CHECK(!factory.setupOwner()); CHECK(factory.setupWasBound());
    const auto& result = factory.constructPageState(f.binding(), io, f.context(true));
    CHECK(result.bindingIssue == Editor300BindingIssue::SetupOwnerExpired);
    CHECK(result.state == Editor300ConstructorState::PageRefused);
    CHECK(!result.pages); CHECK(!result.setupHandoff); CHECK(!factory.partialPageState());
    CHECK(io.reads == 0); CHECK(io.removes == 0);
    CHECK(f.operation.engineModel->simCtl()->smode == 99);
    EditorSetupState replacement;
    CHECK(!factory.bindSetupOwner(replacement)); CHECK(!replacement.factoryOwnerIdentity());
    CHECK(result.setupBindIssue == Editor300SetupBindIssue::AlreadyBound);
    CHECK(result.bindingIssue == Editor300BindingIssue::SetupOwnerExpired);
    guard.reset(); CHECK(!guard.get());
}

TEST_CASE(editor_setup_invalid_or_already_started_factory_does_not_claim_setup)
{
    HandoffFixture f; m2m::Project otherProject; HandoffIo io;
    StagedEditor300 invalid({otherProject, f.operation, *f.operation.engineModel, f.remesh});
    EditorSetupState first, second;
    CHECK(!invalid.bindSetupOwner(first)); CHECK(!first.factoryOwnerIdentity());
    CHECK(invalid.construction().setupBindIssue == Editor300SetupBindIssue::InvalidFactory);
    StagedEditor300 late(f.binding());
    late.constructPageState(f.binding(), io, f.context());
    CHECK(!late.bindSetupOwner(second)); CHECK(!second.factoryOwnerIdentity());
    CHECK(late.construction().setupBindIssue == Editor300SetupBindIssue::LateBinding);
    CHECK(late.construction().pages->issue == InitialOpenIssue::MissingMoObservation);
}

TEST_CASE(editor_setup_bound_factory_validates_current_context_before_handoff)
{
    HandoffFixture f; StagedEditor300 factory(f.binding()); EditorSetupState setup; HandoffIo io;
    REQUIRE(factory.bindSetupOwner(setup)); prepareHandoffSetup(setup);
    auto context = f.context(true); context.general.applicationMode = 1;
    const auto& result = factory.constructPageState(f.binding(), io, context);
    CHECK(result.bindingIssue == Editor300BindingIssue::ContextMismatch);
    CHECK(!result.setupHandoff); CHECK(!factory.partialPageState()); CHECK(!setup.sealedForPages());
    CHECK(setup.factoryOwnerIdentity() == &factory); CHECK(!setup.pageOwnerIdentity());
    CHECK(io.reads == 0); CHECK(f.operation.engineModel->simCtl()->smode == 99);
}

namespace {
NativeSetupObservation nativeSetupContext(const StagedEditor300& factory)
{
    NativeSetupObservation result;
    result.receiver = &factory;
    result.qualification = "Isolated exact-owner setup context; no production observation inferred";
    result.objectName = m2m::qt::String();
    result.declarativeHookDataPresent = false;
    result.widgetCreated = false;
    result.widgetExtra = NativeSetupWidgetExtra::Absent;
    result.geometryEndpoints = std::array<std::int32_t, 4>{10, -20, 649, 459};
    result.metaObject = NativeSetupMetaObject::NativeStatic;
    result.propertyTable = NativeSetupPropertyTable::AbsentExtraData;
    return result;
}
}

TEST_CASE(editor_setup_native_prefix_requires_exact_factory_binding_and_provenance)
{
    HandoffFixture f; StagedEditor300 factory(f.binding()), other(f.binding());
    EditorSetupState setup; REQUIRE(factory.bindSetupOwner(setup));
    auto context = nativeSetupContext(other);
    CHECK(factory.beginNativeSetupPrefix(f.binding(), context).issue == NativeSetupPrefixIssue::IdentityMismatch);
    context.receiver = &factory;
    m2m::Project otherProject;
    CHECK(factory.beginNativeSetupPrefix({otherProject, f.operation, *f.operation.engineModel, f.remesh}, context).issue ==
        NativeSetupPrefixIssue::IdentityMismatch);
    context.qualification.clear();
    CHECK(factory.beginNativeSetupPrefix(f.binding(), context).issue == NativeSetupPrefixIssue::MissingQualification);
    CHECK(!setup.nativePrefix().report.attempted); CHECK(setup.nativePrefix().effects.empty());
    CHECK(factory.nativeSetupPrefix() == &setup.nativePrefix());
    CHECK(!factory.pendingNativeSetupEvent(f.binding()));
}

TEST_CASE(editor_setup_native_prefix_unknown_name_is_first_runtime_guard)
{
    HandoffFixture f; StagedEditor300 factory(f.binding()); EditorSetupState setup;
    REQUIRE(factory.bindSetupOwner(setup));
    NativeSetupObservation context; context.receiver = &factory;
    context.qualification = "Actual owner exists; earlier native widget context is unavailable";
    const auto result = factory.beginNativeSetupPrefix(f.binding(), context);
    CHECK(result.attempted); CHECK(result.stage == NativeSetupPrefixStage::ObjectName);
    CHECK(result.issue == NativeSetupPrefixIssue::UnknownObjectName);
    CHECK(setup.nativePrefix().effects.empty()); CHECK(!setup.nativePrefix().widgetState()->objectName());
    CHECK(!setup.nativePrefix().eventState()); CHECK(!result.completeForNativeSetup);
}

TEST_CASE(editor_setup_native_prefix_retains_name_before_declarative_hook_refusals)
{
    for (const auto hook : {std::optional<bool>{}, std::optional<bool>{true}}) {
        HandoffFixture f; StagedEditor300 factory(f.binding()); EditorSetupState setup;
        REQUIRE(factory.bindSetupOwner(setup)); auto context = nativeSetupContext(factory);
        context.declarativeHookDataPresent = hook;
        const auto result = factory.beginNativeSetupPrefix(f.binding(), context);
        CHECK(result.stage == NativeSetupPrefixStage::ObjectNameHook);
        CHECK(result.issue == (hook ? NativeSetupPrefixIssue::DeclarativeNameHook : NativeSetupPrefixIssue::UnknownNameHook));
        REQUIRE(setup.nativePrefix().widgetState()->objectName());
        CHECK(*setup.nativePrefix().widgetState()->objectName() == m2m::qt::L("SfPreSimControl300"));
        CHECK(setup.nativePrefix().effects == std::vector<NativeSetupPrefixEffect>{NativeSetupPrefixEffect::ObjectNameAssigned});
        CHECK(!setup.nativePrefix().widgetState()->attributeBit(42)); CHECK(!setup.nativePrefix().widgetState()->geometryEndpoints());
    }
}

TEST_CASE(editor_setup_native_prefix_preserves_existing_name_and_skips_name_hook)
{
    HandoffFixture f; StagedEditor300 factory(f.binding()); EditorSetupState setup;
    REQUIRE(factory.bindSetupOwner(setup)); auto context = nativeSetupContext(factory);
    context.objectName = m2m::qt::L("kept editor name"); context.declarativeHookDataPresent.reset();
    const auto result = factory.beginNativeSetupPrefix(f.binding(), context);
    CHECK(result.stage == NativeSetupPrefixStage::PropertyNotification);
    CHECK(*setup.nativePrefix().widgetState()->objectName() == m2m::qt::L("kept editor name"));
    CHECK(setup.nativePrefix().effects.front() == NativeSetupPrefixEffect::ResizeAttribute42Set);
}

TEST_CASE(editor_setup_native_prefix_retains_resize_attribute_before_created_branch)
{
    for (const auto created : {std::optional<bool>{}, std::optional<bool>{true}}) {
        HandoffFixture f; StagedEditor300 factory(f.binding()); EditorSetupState setup;
        REQUIRE(factory.bindSetupOwner(setup)); auto context = nativeSetupContext(factory);
        context.widgetCreated = created;
        const auto result = factory.beginNativeSetupPrefix(f.binding(), context);
        CHECK(result.issue == (created ? NativeSetupPrefixIssue::CreatedWidget : NativeSetupPrefixIssue::UnknownCreatedState));
        CHECK(setup.nativePrefix().widgetState()->attributeBit(42) == true);
        CHECK(!setup.nativePrefix().widgetState()->attributeBit(35)); CHECK(!setup.nativePrefix().widgetState()->geometryEndpoints());
        CHECK(!setup.nativePrefix().falseVariantConstructed); CHECK(!setup.nativePrefix().widgetState()->dynamicBooleanProperties().names);
    }
}

TEST_CASE(editor_setup_native_prefix_resizes_then_stores_property_before_notification)
{
    HandoffFixture f; StagedEditor300 factory(f.binding()); EditorSetupState setup;
    REQUIRE(factory.bindSetupOwner(setup)); auto context = nativeSetupContext(factory);
    const auto result = factory.beginNativeSetupPrefix(f.binding(), context);
    const auto& state = setup.nativePrefix();
    CHECK(result.issue == NativeSetupPrefixIssue::NotificationContext);
    REQUIRE(state.widgetState()->geometryEndpoints());
    const std::array<std::int32_t, 4> rectangle{10, -20, 486, 369};
    CHECK(*state.widgetState()->geometryEndpoints() == rectangle);
    CHECK(state.widgetState()->attributeBit(42) == true); CHECK(state.widgetState()->attributeBit(35) == true);
    CHECK(state.falseVariantConstructed); CHECK(state.widgetState()->dynamicBooleanProperties().extraDataAllocated == true);
    CHECK(state.widgetState()->dynamicBooleanProperties().names == std::vector<std::string>{"sizeGripEnabled"});
    CHECK(state.widgetState()->dynamicBooleanProperties().values == std::vector<bool>{false});
    const std::vector<NativeSetupPrefixEffect> expected{
        NativeSetupPrefixEffect::ObjectNameAssigned, NativeSetupPrefixEffect::ResizeAttribute42Set,
        NativeSetupPrefixEffect::GeometrySizeAssigned, NativeSetupPrefixEffect::ResizeAttribute35Set,
        NativeSetupPrefixEffect::FalseVariantConstructed, NativeSetupPrefixEffect::DynamicExtraDataAllocated,
        NativeSetupPrefixEffect::PropertyNameAppended, NativeSetupPrefixEffect::PropertyValueAppended,
        NativeSetupPrefixEffect::PropertyEventConstructed};
    CHECK(state.effects == expected);
    const auto event = factory.pendingNativeSetupEvent(f.binding()); REQUIRE(event);
    CHECK(event->receiver == &factory); CHECK(event->owner == &setup);
    CHECK(event->type == 0xaa); CHECK(event->name == "sizeGripEnabled");
    CHECK(event->accepted); CHECK(!event->spontaneous); CHECK(event->notifyCallAddress == 0x6712e36f);
    CHECK(!event->posted); CHECK(event->privateDataNull);
    CHECK(!factory.partialPageState()); CHECK(!result.completeForNativeSetup);
}

TEST_CASE(editor_setup_native_prefix_applies_bounds_before_minimum_without_normalizing_them)
{
    HandoffFixture f; StagedEditor300 factory(f.binding()); EditorSetupState setup;
    REQUIRE(factory.bindSetupOwner(setup)); auto context = nativeSetupContext(factory);
    context.widgetExtra = NativeSetupWidgetExtra::Present;
    context.sizeLimits = NativeSetupSizeLimits{500, 600, 100, 200};
    context.propertyTable = NativeSetupPropertyTable::EmptyNativeTable;
    factory.beginNativeSetupPrefix(f.binding(), context);
    const std::array<std::int32_t, 4> rectangle{10, -20, 509, 579};
    CHECK(*setup.nativePrefix().widgetState()->geometryEndpoints() == rectangle);
    CHECK(setup.nativePrefix().widgetState()->dynamicBooleanProperties().extraDataAllocated == true);
    CHECK(std::find(setup.nativePrefix().effects.begin(), setup.nativePrefix().effects.end(),
        NativeSetupPrefixEffect::DynamicExtraDataAllocated) == setup.nativePrefix().effects.end());
    CHECK(setup.nativePrefix().widgetState()->dynamicBooleanProperties().names->size() == 1);
    CHECK(setup.nativePrefix().eventState() != nullptr);
}

TEST_CASE(editor_setup_native_prefix_preserves_native_rectangle_endpoint_wrap)
{
    HandoffFixture f; StagedEditor300 factory(f.binding()); EditorSetupState setup;
    REQUIRE(factory.bindSetupOwner(setup)); auto context = nativeSetupContext(factory);
    const auto maximum = std::numeric_limits<std::int32_t>::max();
    const auto minimum = std::numeric_limits<std::int32_t>::min();
    context.geometryEndpoints = std::array<std::int32_t, 4>{maximum, maximum, 0, 0};
    factory.beginNativeSetupPrefix(f.binding(), context);
    const std::array<std::int32_t, 4> rectangle{maximum, maximum, minimum + 475, minimum + 388};
    CHECK(*setup.nativePrefix().widgetState()->geometryEndpoints() == rectangle);
}

TEST_CASE(editor_setup_native_prefix_missing_constraints_and_geometry_preserve_earlier_writes)
{
    for (int missing = 0; missing != 3; ++missing) {
        HandoffFixture f; StagedEditor300 factory(f.binding()); EditorSetupState setup;
        REQUIRE(factory.bindSetupOwner(setup)); auto context = nativeSetupContext(factory);
        if (missing == 0) context.widgetExtra = NativeSetupWidgetExtra::Unknown;
        if (missing == 1) context.widgetExtra = NativeSetupWidgetExtra::Present;
        if (missing == 2) context.geometryEndpoints.reset();
        const auto result = factory.beginNativeSetupPrefix(f.binding(), context);
        CHECK(result.issue == (missing == 2 ? NativeSetupPrefixIssue::UnknownGeometry : NativeSetupPrefixIssue::UnknownConstraints));
        CHECK(setup.nativePrefix().widgetState()->attributeBit(42) == true);
        CHECK(!setup.nativePrefix().widgetState()->geometryEndpoints()); CHECK(!setup.nativePrefix().widgetState()->attributeBit(35));
        CHECK(!setup.nativePrefix().falseVariantConstructed); CHECK(!setup.nativePrefix().eventState());
    }
}

TEST_CASE(editor_setup_native_prefix_metadata_and_property_guards_retain_completed_resize)
{
    for (int missing = 0; missing != 4; ++missing) {
        HandoffFixture f; StagedEditor300 factory(f.binding()); EditorSetupState setup;
        REQUIRE(factory.bindSetupOwner(setup)); auto context = nativeSetupContext(factory);
        const NativeSetupPrefixIssue issues[]{NativeSetupPrefixIssue::UnknownMetaObject,
            NativeSetupPrefixIssue::DynamicMetaObject, NativeSetupPrefixIssue::UnknownPropertyTable,
            NativeSetupPrefixIssue::NonemptyPropertyTable};
        if (missing == 0) context.metaObject = NativeSetupMetaObject::Unknown;
        if (missing == 1) context.metaObject = NativeSetupMetaObject::DynamicOverride;
        if (missing == 2) context.propertyTable = NativeSetupPropertyTable::Unknown;
        if (missing == 3) context.propertyTable = NativeSetupPropertyTable::NonemptyNativeTable;
        CHECK(factory.beginNativeSetupPrefix(f.binding(), context).issue == issues[missing]);
        CHECK(setup.nativePrefix().widgetState()->geometryEndpoints().has_value());
        CHECK(setup.nativePrefix().widgetState()->attributeBit(35) == true); CHECK(setup.nativePrefix().falseVariantConstructed);
        CHECK(!setup.nativePrefix().widgetState()->dynamicBooleanProperties().extraDataAllocated);
        CHECK(!setup.nativePrefix().widgetState()->dynamicBooleanProperties().names); CHECK(!setup.nativePrefix().widgetState()->dynamicBooleanProperties().values);
        CHECK(!setup.nativePrefix().eventState());
    }
}

TEST_CASE(editor_setup_native_prefix_does_not_replay_or_bypass_pending_stage)
{
    HandoffFixture f; StagedEditor300 factory(f.binding()); EditorSetupState setup; HandoffIo io;
    REQUIRE(factory.bindSetupOwner(setup)); prepareHandoffSetup(setup);
    auto context = nativeSetupContext(factory);
    factory.beginNativeSetupPrefix(f.binding(), context);
    const auto effects = setup.nativePrefix().effects;
    context.objectName = m2m::qt::L("must not replace"); context.geometryEndpoints.reset();
    const auto repeated = factory.beginNativeSetupPrefix(f.binding(), context);
    CHECK(repeated.repeatedAttemptRejected); CHECK(repeated.issue == NativeSetupPrefixIssue::NotificationContext);
    CHECK(setup.nativePrefix().effects == effects);
    CHECK(*setup.nativePrefix().widgetState()->objectName() == m2m::qt::L("SfPreSimControl300"));
    CHECK(setup.nativePrefix().widgetState()->dynamicBooleanProperties().names->size() == 1);
    const auto& pages = factory.constructPageState(f.binding(), io, f.context(true));
    CHECK(pages.bindingIssue == Editor300BindingIssue::NativeSetupPrerequisite);
    CHECK(!factory.partialPageState()); CHECK(!setup.sealedForPages()); CHECK(io.reads == 0);
}

TEST_CASE(editor_setup_native_prefix_rejects_expired_owners_and_sealed_history)
{
    HandoffFixture f; StagedEditor300 factory(f.binding()); HandoffIo io;
    auto context = nativeSetupContext(factory);
    CHECK(factory.beginNativeSetupPrefix(f.binding(), context).issue == NativeSetupPrefixIssue::SetupOwnerUnavailable);
    auto setup = std::make_unique<EditorSetupState>(); REQUIRE(factory.bindSetupOwner(*setup));
    factory.beginNativeSetupPrefix(f.binding(), context); REQUIRE(factory.pendingNativeSetupEvent(f.binding()));
    setup.reset(); CHECK(!factory.nativeSetupPrefix()); CHECK(!factory.pendingNativeSetupEvent(f.binding()));
    CHECK(factory.beginNativeSetupPrefix(f.binding(), context).issue == NativeSetupPrefixIssue::SetupOwnerUnavailable);

    StagedEditor300 sealed(f.binding()); EditorSetupState history;
    REQUIRE(sealed.bindSetupOwner(history)); prepareHandoffSetup(history);
    sealed.constructPageState(f.binding(), io, f.context(true)); REQUIRE(history.sealedForPages());
    CHECK(sealed.beginNativeSetupPrefix(f.binding(), nativeSetupContext(sealed)).issue == NativeSetupPrefixIssue::HistoricalState);
    CHECK(!history.nativePrefix().report.attempted); CHECK(history.nativePrefix().effects.empty());
}

TEST_CASE(editor_setup_native_pending_event_requires_current_model_identity)
{
    HandoffFixture f; StagedEditor300 factory(f.binding()); EditorSetupState setup;
    REQUIRE(factory.bindSetupOwner(setup)); auto context = nativeSetupContext(factory);
    factory.beginNativeSetupPrefix(f.binding(), context); REQUIRE(factory.pendingNativeSetupEvent(f.binding()));
    auto wrong = std::make_shared<m2m::engine::GuiModel>(nullptr, 3);
    CHECK(!factory.pendingNativeSetupEvent({f.project, f.operation, *wrong, f.remesh}));
    f.operation.engineModel.reset(); // actual model guard expires
    CHECK(!factory.pendingNativeSetupEvent({f.project, f.operation, *wrong, f.remesh}));
    CHECK(factory.beginNativeSetupPrefix({f.project, f.operation, *wrong, f.remesh}, context).issue ==
        NativeSetupPrefixIssue::IdentityMismatch);
    CHECK(setup.nativePrefix().widgetState()->dynamicBooleanProperties().names->size() == 1); // retained evidence, no live dispatch claim
}

namespace {
using m2m::engine::WidgetApplicationEventObservation;
using m2m::engine::WidgetReceiverEventObservation;
using m2m::engine::WidgetEventReceiverKind;
using m2m::engine::WidgetDispatchStage;
using m2m::engine::WidgetRuntimeState;
using m2m::engine::WidgetState;

// Explicit isolated input matching the source-closed raw0xaa branch. These
// helpers do not import startup state or claim that base construction returned.
WidgetApplicationEventObservation propertyApplicationContext()
{
    WidgetApplicationEventObservation app;
    app.present = true; app.closing = false; app.internalCallbackCount = 0;
    app.filters = std::vector<WidgetApplicationEventObservation::Filter>{
        {WidgetApplicationEventObservation::Filter::Kind::WindowsVistaStyle, true}};
    return app; // raw0xaa bypasses gesture state, which remains unknown
}
WidgetReceiverEventObservation propertyReceiverContext()
{
    WidgetReceiverEventObservation receiver;
    receiver.sameApplicationThread = true; receiver.layoutPresent = false;
    receiver.objectFilterCount = 0;
    return receiver;
}
void qualifyPropertyDispatch(StagedEditor300& factory, WidgetRuntimeState& runtime)
{
    runtime.observeApplicationEvents(propertyApplicationContext(), "Isolated property application profile");
    factory.widgetState()->observeEventReceiver(propertyReceiverContext(), WidgetEventReceiverKind::QWidget,
        "Isolated same-editor receiver profile");
}
}

TEST_CASE(editor_setup_native_prefix_consumes_and_mutates_the_single_actual_widget)
{
    HandoffFixture f; StagedEditor300 factory(f.binding()); EditorSetupState setup;
    const auto widget = factory.widgetState();
    REQUIRE(widget); CHECK(widget->nativeIdentity() == &factory);
    CHECK(!widget->objectName()); CHECK(!widget->geometryEndpoints());
    CHECK(!widget->attributeBit(35)); CHECK(!widget->attributeBit(60));
    CHECK(!widget->widgetDataAssigned()); // allocation is not native QWidget initialization
    REQUIRE(factory.bindSetupOwner(setup));
    CHECK(setup.nativePrefix().widgetState().get() == widget.get());
    widget->observeObjectName(m2m::qt::L("actual retained name"));
    widget->observeAttributeBit(60, false);
    widget->observeAttributeBit(35, true); // captured true before the setter; no false seed
    widget->observeGeometryEndpoints({2, 3, 641, 482});
    widget->observeEmptyDynamicBooleanProperties(false);
    auto context = nativeSetupContext(factory);
    context.objectName.reset(); context.widgetCreated.reset(); context.geometryEndpoints.reset();
    context.propertyTable = NativeSetupPropertyTable::Unknown;
    context.declarativeHookDataPresent.reset(); // nonempty actual name skips that callback
    CHECK(factory.beginNativeSetupPrefix(f.binding(), context).stage == NativeSetupPrefixStage::PropertyNotification);
    const std::array<int, 4> rectangle{2, 3, 478, 392};
    CHECK(widget->geometryEndpoints() == rectangle);
    CHECK(widget->objectName() == m2m::qt::L("actual retained name"));
    CHECK(widget->attributeBit(35) == true); CHECK(widget->attributeBit(42) == true);
    CHECK(!widget->extendedAttributes()); CHECK(!widget->attributeBit(34));
    CHECK(widget->dynamicBooleanProperties().names == std::vector<std::string>{"sizeGripEnabled"});
    CHECK(widget->dynamicBooleanProperties().values == std::vector<bool>{false});
    widget->assignObjectName(m2m::qt::L("current component value"));
    CHECK(setup.nativePrefix().widgetState()->objectName() == m2m::qt::L("current component value"));
}

TEST_CASE(editor_setup_native_prefix_conflicting_observations_never_replace_actual_values)
{
    for (int conflict = 0; conflict != 4; ++conflict) {
        HandoffFixture f; StagedEditor300 factory(f.binding()); EditorSetupState setup;
        REQUIRE(factory.bindSetupOwner(setup)); auto context = nativeSetupContext(factory);
        auto widget = factory.widgetState();
        if (conflict == 0) widget->observeObjectName(m2m::qt::L("already owned"));
        if (conflict == 1) widget->observeAttributeBit(60, true);
        const std::array<int, 4> retainedGeometry{1, 2, 3, 4};
        if (conflict == 2) widget->observeGeometryEndpoints(retainedGeometry);
        if (conflict == 3) widget->observeEmptyDynamicBooleanProperties(true);
        const auto result = factory.beginNativeSetupPrefix(f.binding(), context);
        CHECK(result.issue == NativeSetupPrefixIssue::ObservationMismatch);
        CHECK(!setup.nativePrefix().eventState());
        if (conflict == 0) {
            CHECK(result.stage == NativeSetupPrefixStage::ObjectName);
            CHECK(widget->objectName() == m2m::qt::L("already owned"));
            CHECK(setup.nativePrefix().effects.empty()); CHECK(!widget->attributeBit(42));
        } else if (conflict == 1) {
            CHECK(result.stage == NativeSetupPrefixStage::ResizeCreatedState);
            CHECK(widget->attributeBit(60) == true); CHECK(widget->attributeBit(42) == true);
            CHECK(!widget->geometryEndpoints()); CHECK(!setup.nativePrefix().falseVariantConstructed);
        } else if (conflict == 2) {
            CHECK(result.stage == NativeSetupPrefixStage::ResizeGeometry);
            CHECK(widget->geometryEndpoints() == retainedGeometry); CHECK(widget->attributeBit(42) == true);
            CHECK(!widget->attributeBit(35)); CHECK(!setup.nativePrefix().falseVariantConstructed);
        } else {
            CHECK(result.stage == NativeSetupPrefixStage::PropertyTable);
            CHECK(widget->attributeBit(35) == true); CHECK(setup.nativePrefix().falseVariantConstructed);
            CHECK(widget->dynamicBooleanProperties().extraDataAllocated == true);
            REQUIRE(widget->dynamicBooleanProperties().names);
            CHECK(widget->dynamicBooleanProperties().names->empty());
            CHECK(widget->dynamicBooleanProperties().values->empty());
        }
    }
}

TEST_CASE(editor_setup_native_widget_lease_does_not_extend_native_factory_lifetime)
{
    HandoffFixture f; EditorSetupState setup;
    std::shared_ptr<WidgetState> escaped;
    const void* identity = nullptr;
    {
        StagedEditor300 factory(f.binding()); REQUIRE(factory.bindSetupOwner(setup));
        factory.beginNativeSetupPrefix(f.binding(), nativeSetupContext(factory));
        escaped = factory.widgetState(); identity = &factory;
        REQUIRE(escaped->nativeOwnerAlive());
        CHECK(setup.nativePrefix().widgetState().get() == escaped.get());
    }
    CHECK(escaped->nativeIdentity() == identity); CHECK(!escaped->nativeOwnerAlive());
    CHECK(setup.nativePrefix().widgetState().get() == escaped.get());
    CHECK(escaped->objectName() == m2m::qt::L("SfPreSimControl300")); // historical evidence
    bool refused = false;
    try { escaped->assignObjectName(m2m::qt::L("cannot revive")); }
    catch (const m2m::Error& error) { refused = error.code == M2M_E_UNSUPPORTED; }
    CHECK(refused); CHECK(escaped->objectName() == m2m::qt::L("SfPreSimControl300"));
}

TEST_CASE(editor_setup_runtime_binding_preserves_identity_without_registering_or_constructing)
{
    HandoffFixture f; StagedEditor300 factory(f.binding());
    auto runtime = std::make_shared<WidgetRuntimeState>(true, "Isolated actual runtime");
    auto wrongModel = std::make_shared<m2m::engine::GuiModel>(nullptr, 3);
    CHECK(!factory.bindWidgetRuntime({f.project, f.operation, *wrongModel, f.remesh}, runtime));
    CHECK(!factory.widgetState()->runtimeOwner());
    CHECK(!factory.bindWidgetRuntime(f.binding(), nullptr));
    REQUIRE(factory.bindWidgetRuntime(f.binding(), runtime));
    CHECK(factory.widgetState()->runtimeOwner() == runtime);
    CHECK(factory.widgetState()->threadState() == runtime->threadState());
    CHECK(!runtime->hasLiveRegistration(&factory)); CHECK(!runtime->populationComplete());
    CHECK(!factory.widgetState()->widgetDataAssigned()); CHECK(!factory.widgetState()->objectName());
    CHECK(factory.bindWidgetRuntime(f.binding(), runtime)); // same identity is harmless
    auto replacement = std::make_shared<WidgetRuntimeState>(true, "Different isolated runtime");
    bool refused = false;
    try { factory.bindWidgetRuntime(f.binding(), replacement); }
    catch (const m2m::Error& error) { refused = error.code == M2M_E_UNSUPPORTED; }
    CHECK(refused); CHECK(factory.widgetState()->runtimeOwner() == runtime);
    HandoffIo io;
    factory.constructPageState(f.binding(), io, f.context());
    CHECK(!factory.bindWidgetRuntime(f.binding(), runtime)); // no late constructor binding
}

TEST_CASE(editor_setup_property_continuation_without_runtime_retains_the_same_event_and_stores)
{
    HandoffFixture f; StagedEditor300 factory(f.binding()); EditorSetupState setup;
    REQUIRE(factory.bindSetupOwner(setup));
    CHECK(factory.continueNativeSetupPropertyNotification(f.binding()).issue == NativeSetupPrefixIssue::WrongStage);
    factory.beginNativeSetupPrefix(f.binding(), nativeSetupContext(factory));
    const auto* event = setup.nativePrefix().eventState(); REQUIRE(event);
    const auto effects = setup.nativePrefix().effects;
    for (int i = 0; i != 2; ++i) {
        CHECK(factory.continueNativeSetupPropertyNotification(f.binding()).issue ==
            NativeSetupPrefixIssue::RuntimeOwnerUnavailable);
        CHECK(setup.nativePrefix().eventState() == event); CHECK(event->nativeAlive());
        CHECK(setup.nativePrefix().notification().stage == WidgetDispatchStage::NotStarted);
        CHECK(setup.nativePrefix().effects == effects); CHECK(!setup.nativePrefix().originalSetterResult);
        CHECK(factory.widgetState()->dynamicBooleanProperties().names->size() == 1);
        CHECK(factory.widgetState()->dynamicBooleanProperties().values == std::vector<bool>{false});
    }
}

TEST_CASE(editor_setup_property_false_return_preserves_property_and_destroys_only_stack_event)
{
    HandoffFixture f; StagedEditor300 factory(f.binding()); EditorSetupState setup; HandoffIo io;
    auto runtime = std::make_shared<WidgetRuntimeState>(true, "Isolated actual event runtime");
    REQUIRE(factory.bindWidgetRuntime(f.binding(), runtime)); REQUIRE(factory.bindSetupOwner(setup));
    qualifyPropertyDispatch(factory, *runtime); prepareHandoffSetup(setup);
    factory.beginNativeSetupPrefix(f.binding(), nativeSetupContext(factory));
    const auto* event = setup.nativePrefix().eventState(); REQUIRE(event);
    const auto effectCount = setup.nativePrefix().effects.size();
    CHECK(event->nativeAlive()); CHECK(event->propertyName() == "sizeGripEnabled");
    const auto result = factory.continueNativeSetupPropertyNotification(f.binding());
    CHECK(result.stage == NativeSetupPrefixStage::PropertySetReturned);
    CHECK(result.issue == NativeSetupPrefixIssue::None); CHECK(!result.completeForNativeSetup);
    CHECK(setup.nativePrefix().notification().completed);
    CHECK(setup.nativePrefix().notification().originalResult == false);
    CHECK(setup.nativePrefix().originalSetterResult == false);
    CHECK(setup.nativePrefix().eventState() == event); CHECK(!event->nativeAlive());
    CHECK(event->propertyName().empty()); CHECK(!factory.pendingNativeSetupEvent(f.binding()));
    CHECK(event->accepted()); CHECK(!event->spontaneous()); CHECK(!event->posted());
    const auto& effects = setup.nativePrefix().effects;
    REQUIRE(effects.size() == effectCount + 3);
    CHECK(effects[effectCount] == NativeSetupPrefixEffect::PropertyNotificationReturned);
    CHECK(effects[effectCount + 1] == NativeSetupPrefixEffect::PropertyEventDestroyed);
    CHECK(effects[effectCount + 2] == NativeSetupPrefixEffect::DynamicPropertySetterReturned);
    CHECK(setup.nativePrefix().falseVariantConstructed); // caller temporary dtor is later
    CHECK(factory.widgetState()->dynamicBooleanProperties().names == std::vector<std::string>{"sizeGripEnabled"});
    CHECK(factory.widgetState()->dynamicBooleanProperties().values == std::vector<bool>{false});
    CHECK(!runtime->threadState()->notifyDepth()); // source +1/-1 did not invent a base
    CHECK(runtime->threadState()->retainedNotifyDelta() == 0);
    CHECK(!runtime->threadState()->postedEvents().populationKnown());
    CHECK(!runtime->hasLiveRegistration(&factory));
    const auto repeated = factory.continueNativeSetupPropertyNotification(f.binding());
    CHECK(repeated.repeatedNotificationRejected); CHECK(effects.size() == effectCount + 3);
    CHECK(runtime->threadState()->retainedNotifyDelta() == 0);
    const auto& pages = factory.constructPageState(f.binding(), io, f.context(true));
    CHECK(pages.bindingIssue == Editor300BindingIssue::NativeSetupPrerequisite);
    CHECK(!factory.partialPageState()); CHECK(!setup.sealedForPages()); CHECK(io.reads == 0);
    CHECK(!StagedEditor300::completeForPublicSave);
}

TEST_CASE(editor_setup_property_guard_resumes_same_stack_frame_without_replaying_writes)
{
    HandoffFixture f; StagedEditor300 factory(f.binding()); EditorSetupState setup;
    auto runtime = std::make_shared<WidgetRuntimeState>(true, "Isolated pending event runtime");
    REQUIRE(factory.bindWidgetRuntime(f.binding(), runtime)); REQUIRE(factory.bindSetupOwner(setup));
    runtime->observeApplicationEvents(propertyApplicationContext(), "Isolated application profile");
    auto receiver = propertyReceiverContext(); receiver.objectFilterCount.reset();
    factory.widgetState()->observeEventReceiver(receiver, WidgetEventReceiverKind::QWidget,
        "Complete receiver except its unobserved filter population");
    factory.beginNativeSetupPrefix(f.binding(), nativeSetupContext(factory));
    const auto* event = setup.nativePrefix().eventState(); REQUIRE(event);
    const auto effects = setup.nativePrefix().effects;
    for (int i = 0; i != 2; ++i) {
        const auto result = factory.continueNativeSetupPropertyNotification(f.binding());
        CHECK(result.issue == NativeSetupPrefixIssue::NotificationContext);
        CHECK(setup.nativePrefix().notification().stage == WidgetDispatchStage::ReceiverFilters);
        CHECK(!setup.nativePrefix().notification().completed);
        CHECK(runtime->threadState()->retainedNotifyDelta() == 1); CHECK(!runtime->threadState()->notifyDepth());
        CHECK(setup.nativePrefix().eventState() == event); CHECK(event->nativeAlive());
        CHECK(setup.nativePrefix().effects == effects); CHECK(!setup.nativePrefix().originalSetterResult);
        CHECK(factory.widgetState()->dynamicBooleanProperties().names->size() == 1);
    }
    factory.widgetState()->observeEventReceiver(propertyReceiverContext(), WidgetEventReceiverKind::QWidget,
        "The missing complete filter list is now qualified empty");
    CHECK(factory.continueNativeSetupPropertyNotification(f.binding()).stage == NativeSetupPrefixStage::PropertySetReturned);
    CHECK(runtime->threadState()->retainedNotifyDelta() == 0); CHECK(!runtime->threadState()->notifyDepth());
    CHECK(setup.nativePrefix().eventState() == event); CHECK(!event->nativeAlive());
    CHECK(setup.nativePrefix().effects.size() == effects.size() + 3);
    CHECK(factory.widgetState()->dynamicBooleanProperties().names->size() == 1);
}

TEST_CASE(editor_setup_property_continuation_keeps_actual_nonzero_thread_depth)
{
    HandoffFixture f; StagedEditor300 factory(f.binding()); EditorSetupState setup;
    auto runtime = std::make_shared<WidgetRuntimeState>(true, "Isolated nonzero depth runtime");
    REQUIRE(factory.bindWidgetRuntime(f.binding(), runtime)); REQUIRE(factory.bindSetupOwner(setup));
    qualifyPropertyDispatch(factory, *runtime);
    runtime->threadState()->observeNotifyDepth(7, "Explicit nonzero isolated thread depth");
    factory.beginNativeSetupPrefix(f.binding(), nativeSetupContext(factory));
    CHECK(factory.continueNativeSetupPropertyNotification(f.binding()).stage == NativeSetupPrefixStage::PropertySetReturned);
    CHECK(factory.widgetState()->threadState()->notifyDepth() == 7u);
    CHECK(runtime->threadState()->retainedNotifyDelta() == 0);
    CHECK(setup.nativePrefix().originalSetterResult == false);
}

TEST_CASE(editor_setup_property_continuation_revalidates_model_and_setup_liveness)
{
    HandoffFixture f; StagedEditor300 factory(f.binding());
    auto setup = std::make_unique<EditorSetupState>();
    auto runtime = std::make_shared<WidgetRuntimeState>(true, "Isolated lifetime runtime");
    REQUIRE(factory.bindWidgetRuntime(f.binding(), runtime)); REQUIRE(factory.bindSetupOwner(*setup));
    qualifyPropertyDispatch(factory, *runtime);
    factory.beginNativeSetupPrefix(f.binding(), nativeSetupContext(factory));
    auto wrong = std::make_shared<m2m::engine::GuiModel>(nullptr, 3);
    CHECK(factory.continueNativeSetupPropertyNotification({f.project, f.operation, *wrong, f.remesh}).issue ==
        NativeSetupPrefixIssue::IdentityMismatch);
    CHECK(setup->nativePrefix().notification().stage == WidgetDispatchStage::NotStarted);
    CHECK(runtime->threadState()->retainedNotifyDelta() == 0);
    setup.reset();
    CHECK(factory.continueNativeSetupPropertyNotification(f.binding()).issue == NativeSetupPrefixIssue::SetupOwnerUnavailable);
    CHECK(runtime->threadState()->retainedNotifyDelta() == 0);
    f.operation.engineModel.reset();
    CHECK(factory.continueNativeSetupPropertyNotification({f.project, f.operation, *wrong, f.remesh}).issue ==
        NativeSetupPrefixIssue::IdentityMismatch);
}

int main() { return testkit::run_all(); }

