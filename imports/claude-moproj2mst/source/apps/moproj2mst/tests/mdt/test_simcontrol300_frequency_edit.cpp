// ctest-labels: m2m-mdt
// Explicit private textEdited event; no native widget/full Session claim.
#include <testkit/check.hpp>
#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>
#include <limits>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include "m2m/engine/simcontrol300_owner.hpp"
#include "m2m/mst/writer.hpp"
#include <defmodel/layout/db_step_data.hpp>

using namespace m2m;
using namespace m2m::engine::simcontrol300;
namespace {
struct Io final : Extra3DIo {
    Io() : Extra3DIo(1252) {}
    int reads = 0, writes = 0, removes = 0;
    bool readDoubles(const qt::String&, std::span<double> values) override
    { ++reads; std::fill(values.begin(), values.end(), 0.0); return false; }
    bool readInts(const qt::String&, std::span<int> values) override
    { ++reads; std::fill(values.begin(), values.end(), 0); return false; }
    bool readSprings(std::vector<SpringRecord>& values) override
    { ++reads; values.clear(); return false; }
    bool removeIfReadable(const qt::String&) override { ++removes; return true; }
    void nbcRemoveWarning() override {}
    std::unique_ptr<mst::OutputDevice> openWrite(const qt::String&) override
    { ++writes; return nullptr; }
};
struct Fixture {
    Project project; Operation operation; RemeshState remesh; Io io;
    std::unique_ptr<StagedEditor300> owner;
    Fixture()
    {
        operation.project = &project;
        operation.engineModel = std::make_shared<engine::GuiModel>(nullptr, 3);
        auto& sc = *operation.engineModel->simCtl();
        sc.unit = 1; sc.nstart = 7; sc.nstep = 23;
        sc.trans[0] = sc.trans[1] = 1;
        owner = std::make_unique<StagedEditor300>(binding());
    }
    Editor300Binding binding() { return {project, operation, *operation.engineModel, remesh}; }
    InitialEditorContext context()
    {
        MoInitialObservations features;
        features.generalFills[0] = MoFeatureAvailability{1, std::nullopt, {1, 1, 1}};
        features.generalFills[1] = MoFeatureAvailability{1, std::nullopt, {1, 1, 1}};
        features.startupHeating = {0, 1};
        return {generalContextForMo(project, 3, {true, true}),
            {5, {}, MoStepCountContext{&operation}}, NumericLocale::KnownC, true, features};
    }
    InitialEditorSession& open()
    {
        owner->constructPageState(binding(), io, context());
        auto* pages = owner->pageState(); REQUIRE(pages);
        auto& sc = *operation.engineModel->simCtl();
        REQUIRE(sc.frqncy.values); REQUIRE(sc.frqncy_rows == 6);
        REQUIRE(sc.frqncy_cols == 1); REQUIRE(sc.frqncy.ncomp == 6);
        return *pages;
    }
};
std::array<std::uint64_t, 6> bits(const SimControl& sc)
{
    std::array<std::uint64_t, 6> result;
    for (int i = 0; i < 6; ++i) result[i] = std::bit_cast<std::uint64_t>(sc.frqncy.values[i]);
    return result;
}
}

TEST_CASE(frequency_edit_updates_each_live_row_immediately_without_other_effects)
{
    for (int row = 0; row < 6; ++row) {
        Fixture f; auto& session = f.open(); auto& form = session.pages.advanced;
        auto& sc = *f.operation.engineModel->simCtl();
        for (int i = 0; i < 6; ++i) sc.frqncy.values[i] = 10 + i;
        const auto before = bits(sc); const auto combos = form.frequencyCombo;
        const int value = row == 5 ? (std::numeric_limits<std::int32_t>::max)() : 101 + row;
        form.populatedFrequencyText[row] = qt::number(value);
        const int reads = f.io.reads;
        sc.nstep = 51; f.operation.nstartGiven = 1; f.remesh.method = 5;
        CHECK(f.owner->frequencyTextEdited(f.binding(), row, *form.populatedFrequencyText[row],
            NumericLocale::KnownC) == ApplyResult::Completed);
        CHECK(sc.frqncy.values[row] == static_cast<double>(value));
        const auto after = bits(sc);
        for (int i = 0; i < 6; ++i) if (i != row) CHECK_EQ(after[i], before[i]);
        CHECK(form.frequencyCombo == combos); CHECK(*form.populatedFrequencyText[row] == qt::number(value));
        CHECK_EQ(sc.nstep, 51); CHECK_EQ(f.operation.nstartGiven, 1); CHECK_EQ(f.remesh.method, 5);
        CHECK_EQ(session.pages.stepCount.steps, 23);
        CHECK_EQ(f.io.reads, reads); CHECK_EQ(f.io.writes, 0); CHECK_EQ(f.io.removes, 0);
    }
}

TEST_CASE(frequency_empty_and_null_edit_write_positive_zero_and_preserve_other_raw_rows)
{
    for (bool null : {false, true}) {
        Fixture f; auto& form = f.open().pages.advanced; auto& sc = *f.operation.engineModel->simCtl();
        sc.frqncy.values[2] = -0.0;
        sc.frqncy.values[1] = std::bit_cast<double>(std::uint64_t{0x7ff8000000000042});
        sc.frqncy.values[4] = -987.5;
        const auto before = bits(sc);
        form.populatedFrequencyText[2] = null ? qt::String() : qt::L("");
        CHECK(f.owner->frequencyTextEdited(f.binding(), 2, *form.populatedFrequencyText[2],
            NumericLocale::KnownC) == ApplyResult::Completed);
        CHECK_EQ(std::bit_cast<std::uint64_t>(sc.frqncy.values[2]), std::uint64_t{0});
        const auto after = bits(sc);
        for (int i = 0; i < 6; ++i) if (i != 2) CHECK_EQ(after[i], before[i]);
        CHECK_EQ(form.populatedFrequencyText[2]->isNull(), null);
    }
}

TEST_CASE(frequency_cache_assignment_and_advanced_apply_do_not_synthesize_text_edit)
{
    Fixture f; auto& form = f.open().pages.advanced; auto& model = *f.operation.engineModel;
    auto& sc = *model.simCtl(); sc.frqncy.values[3] = 7;
    form.populatedFrequencyText[3] = qt::L("999");
    CHECK(sc.frqncy.values[3] == 7);
    CHECK(applyAdvanced3D(sc, model, form) == ApplyResult::Completed);
    CHECK(sc.frqncy.values[3] == 7);
    CHECK(f.owner->frequencyTextEdited(f.binding(), 3, *form.populatedFrequencyText[3],
        NumericLocale::KnownC) == ApplyResult::Completed);
    CHECK(sc.frqncy.values[3] == 999);
}

TEST_CASE(frequency_repeated_explicit_edit_has_no_combo_or_same_text_shortcut)
{
    Fixture f; auto& form = f.open().pages.advanced; auto& sc = *f.operation.engineModel->simCtl();
    form.frequencyCombo[0] = 0; form.populatedFrequencyText[0] = qt::L("61");
    CHECK(f.owner->frequencyTextEdited(f.binding(), 0, *form.populatedFrequencyText[0],
        NumericLocale::KnownC) == ApplyResult::Completed);
    sc.frqncy.values[0] = 85;
    CHECK(f.owner->frequencyTextEdited(f.binding(), 0, *form.populatedFrequencyText[0],
        NumericLocale::KnownC) == ApplyResult::Completed);
    CHECK(sc.frqncy.values[0] == 61); CHECK_EQ(form.frequencyCombo[0], 0);
}

TEST_CASE(frequency_excluded_numeric_spellings_refuse_without_any_store)
{
    Fixture f; auto& form = f.open().pages.advanced; auto& sc = *f.operation.engineModel->simCtl();
    const auto before = bits(sc);
    const std::array<qt::String, 11> excluded{qt::L("0"), qt::L("-1"), qt::L("+1"), qt::L("01"),
        qt::L(" 1"), qt::L("1 "), qt::L("1x"), qt::L("2147483648"),
        qt::L("99999999999999999999999999"), qt::String(u"\u0661"),
        qt::String(std::u16string{u'1', u'\0', u'2'})};
    for (const auto& text : excluded) {
        form.populatedFrequencyText[1] = text;
        CHECK(f.owner->frequencyTextEdited(f.binding(), 1, text, NumericLocale::KnownC)
            == ApplyResult::UnsupportedSnapshot);
        CHECK(bits(sc) == before); CHECK(*form.populatedFrequencyText[1] == text);
    }
}

TEST_CASE(frequency_unknown_cache_mismatched_event_row_locale_or_binding_refuses)
{
    Fixture f; auto& form = f.open().pages.advanced; auto& sc = *f.operation.engineModel->simCtl();
    const auto before = bits(sc); const auto text = qt::L("123");
    form.populatedFrequencyText[0].reset();
    CHECK(f.owner->frequencyTextEdited(f.binding(), 0, text, NumericLocale::KnownC) == ApplyResult::UnsupportedSnapshot);
    form.populatedFrequencyText[0] = text;
    CHECK(f.owner->frequencyTextEdited(f.binding(), 0, qt::L("456"), NumericLocale::KnownC) == ApplyResult::UnsupportedSnapshot);
    CHECK(f.owner->frequencyTextEdited(f.binding(), 0, text, NumericLocale::Unrecovered) == ApplyResult::UnsupportedSnapshot);
    for (int row : {-1, 6})
        CHECK(f.owner->frequencyTextEdited(f.binding(), row, text, NumericLocale::KnownC) == ApplyResult::UnsupportedSnapshot);
    Project other;
    CHECK(f.owner->frequencyTextEdited({other, f.operation, *f.operation.engineModel, f.remesh}, 0, text,
        NumericLocale::KnownC) == ApplyResult::UnsupportedSnapshot);
    CHECK(bits(sc) == before);
}

TEST_CASE(frequency_missing_replaced_sc_or_table_shape_refuses_without_repair)
{
    for (int variant = 0; variant < 6; ++variant) {
        Fixture f; auto& form = f.open().pages.advanced; auto& model = *f.operation.engineModel;
        auto& original = *model.simCtl(); const auto before = bits(original);
        const auto text = qt::L("555"); form.populatedFrequencyText[0] = text;
        std::unique_ptr<def::model::DB_STEP_DATA> heldStep;
        auto* values = original.frqncy.values;
        if (variant < 2) heldStep = std::move(model.step);
        if (variant == 1) model.step = std::make_unique<def::model::DB_STEP_DATA>(3);
        if (variant == 2) original.frqncy_rows = 5;
        if (variant == 3) original.frqncy_cols = 2;
        if (variant == 4) original.frqncy.ncomp = 5;
        if (variant == 5) original.frqncy.values = nullptr;
        auto* replacement = variant == 1 ? model.simCtl() : nullptr;
        if (replacement) { REQUIRE(replacement->frqncy.values); replacement->frqncy.values[0] = 44; }
        const auto result = f.owner->frequencyTextEdited(f.binding(), 0, text, NumericLocale::KnownC);
        // Restore owned array metadata before assertions/teardown.
        original.frqncy_rows = 6; original.frqncy_cols = 1;
        original.frqncy.ncomp = 6; original.frqncy.values = values;
        CHECK(result == ApplyResult::UnsupportedSnapshot); CHECK(bits(original) == before);
        if (replacement) CHECK(replacement->frqncy.values[0] == 44);
    }
}

TEST_CASE(frequency_unconstructed_partial_or_expired_opi_cannot_advance_public_owner)
{
    Fixture f; const auto text = qt::L("42");
    CHECK(f.owner->frequencyTextEdited(f.binding(), 0, text, NumericLocale::KnownC) == ApplyResult::UnsupportedSnapshot);
    CHECK(!f.owner->partialPageState());
    auto context = f.context(); context.moObservations.reset();
    f.owner->constructPageState(f.binding(), f.io, context);
    CHECK(f.owner->frequencyTextEdited(f.binding(), 0, text, NumericLocale::KnownC) == ApplyResult::UnsupportedSnapshot);
    CHECK(!f.owner->pageState());
    Fixture live; auto& form = live.open().pages.advanced; form.populatedFrequencyText[0] = text;
    auto& sc = *live.operation.engineModel->simCtl(); const auto before = bits(sc);
    REQUIRE(live.operation.commitOperationInfoQObjectBoundary(
        "declared private frequency fixture OPI boundary; not native destruction"));
    CHECK(live.owner->frequencyTextEdited(live.binding(), 0, text, NumericLocale::KnownC) == ApplyResult::UnsupportedSnapshot);
    CHECK(bits(sc) == before); CHECK(!StagedEditor300::completeForPublicSave);
}

TEST_CASE(frequency_text_model_command_admits_matching_control_and_only_writes_selected_row)
{
    engine::GuiModel model(nullptr, 3);
    auto* control = model.simCtl(); REQUIRE(control); REQUIRE(control->frqncy.values);
    for (int row = 0; row < 6; ++row) control->frqncy.values[row] = 20.0 + row;
    const auto before = bits(*control);
    CHECK(model.applyEditorFrequencyTextIfControlMatches(control, 4, qt::L("2147483647")));
    const auto after = bits(*control);
    for (int row = 0; row < 6; ++row) {
        if (row == 4) CHECK_EQ(control->frqncy.values[row], 2147483647.0);
        else CHECK_EQ(after[row], before[row]);
    }
    CHECK(model.applyEditorFrequencyTextIfControlMatches(control, 2, qt::L("")));
    CHECK_EQ(std::bit_cast<std::uint64_t>(control->frqncy.values[2]), std::uint64_t{0});
}

TEST_CASE(frequency_text_model_command_refuses_invalid_identity_row_text_and_table_without_writes)
{
    engine::GuiModel model(nullptr, 3);
    auto* control = model.simCtl(); REQUIRE(control); REQUIRE(control->frqncy.values);
    for (int row = 0; row < 6; ++row) control->frqncy.values[row] = 30.0 + row;
    const auto before = bits(*control);
    auto* otherIdentity = reinterpret_cast<const def::model::Cls_1401e7b10*>(
        reinterpret_cast<std::uintptr_t>(control) + 16);
    CHECK(!model.applyEditorFrequencyTextIfControlMatches(otherIdentity, 0, qt::L("8")));
    CHECK(!model.applyEditorFrequencyTextIfControlMatches(nullptr, 0, qt::L("8")));
    CHECK(!model.applyEditorFrequencyTextIfControlMatches(control, -1, qt::L("8")));
    CHECK(!model.applyEditorFrequencyTextIfControlMatches(control, 6, qt::L("8")));
    for (const auto& text : {qt::L("0"), qt::L("01"), qt::L("+1"), qt::L("2147483648"), qt::L("1x")})
        CHECK(!model.applyEditorFrequencyTextIfControlMatches(control, 0, text));
    CHECK(bits(*control) == before);
    const int rows = control->frqncy_rows, cols = control->frqncy_cols, ncomp = control->frqncy.ncomp;
    auto* values = control->frqncy.values;
    control->frqncy_cols = 2;
    CHECK(!model.applyEditorFrequencyTextIfControlMatches(control, 0, qt::L("8")));
    control->frqncy_cols = cols;
    control->frqncy_rows = 5;
    CHECK(!model.applyEditorFrequencyTextIfControlMatches(control, 0, qt::L("8")));
    control->frqncy_rows = rows;
    control->frqncy.ncomp = 5;
    CHECK(!model.applyEditorFrequencyTextIfControlMatches(control, 0, qt::L("8")));
    control->frqncy.ncomp = ncomp;
    control->frqncy.values = nullptr;
    CHECK(!model.applyEditorFrequencyTextIfControlMatches(control, 0, qt::L("8")));
    control->frqncy.values = values;
    CHECK(bits(*control) == before);
    model.step.reset();
    CHECK(!model.applyEditorFrequencyTextIfControlMatches(control, 0, qt::L("8")));
    CHECK(!model.applyEditorFrequencyTextIfControlMatches(nullptr, 0, qt::L("8")));
}

int main() { return testkit::run_all(); }
