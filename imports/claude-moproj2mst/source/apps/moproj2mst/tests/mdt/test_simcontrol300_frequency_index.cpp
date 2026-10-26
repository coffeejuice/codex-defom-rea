// ctest-labels: m2m-mdt
// Explicit bound private combo event; enabled values are setter projections.
#include <testkit/check.hpp>
#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>
#include <limits>
#include <memory>
#include <optional>
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
        auto* pages = owner->pageState(); REQUIRE(pages); return *pages;
    }
};
std::array<std::uint64_t, 6> bits(const SimControl& sc)
{
    REQUIRE(sc.frqncy.values);
    std::array<std::uint64_t, 6> result;
    for (int i = 0; i < 6; ++i) result[i] = std::bit_cast<std::uint64_t>(sc.frqncy.values[i]);
    return result;
}
}

TEST_CASE(model_frequency_index_command_admits_control_shape_and_single_cell_write)
{
    Fixture f; auto& model = *f.operation.engineModel;
    auto* const control = model.simCtl(); REQUIRE(control); REQUIRE(control->frqncy.values);
    for (int i = 0; i < 6; ++i) control->frqncy.values[i] = 20 + i;
    auto before = bits(*control);

    CHECK(!model.applyEditorFrequencyIndexIfControlMatches(nullptr, 0, 0));
    engine::GuiModel other(nullptr, 3);
    CHECK(!model.applyEditorFrequencyIndexIfControlMatches(other.simCtl(), 0, 0));
    auto heldStep = std::move(model.step);
    CHECK(!model.applyEditorFrequencyIndexIfControlMatches(nullptr, 0, 0));
    model.step = std::move(heldStep);
    CHECK(!model.applyEditorFrequencyIndexIfControlMatches(control, -1, 0));
    CHECK(!model.applyEditorFrequencyIndexIfControlMatches(control, 0, 3));
    CHECK(bits(*control) == before);

    const int oldStride = control->frqncy_cols;
    control->frqncy_cols = 2;
    CHECK(!model.applyEditorFrequencyIndexIfControlMatches(control, 0, 2));
    control->frqncy_cols = oldStride;
    CHECK(bits(*control) == before);

    const int oldRows = control->frqncy_rows;
    const int oldComponents = control->frqncy.ncomp;
    auto* const values = control->frqncy.values;
    control->frqncy_rows = 5;
    CHECK(model.applyEditorFrequencyIndexIfControlMatches(control, 0, 2));
    CHECK(!model.applyEditorFrequencyIndexIfControlMatches(control, 0, 0));
    control->frqncy_rows = oldRows;
    control->frqncy.ncomp = 5;
    CHECK(model.applyEditorFrequencyIndexIfControlMatches(control, 1, 1));
    CHECK(!model.applyEditorFrequencyIndexIfControlMatches(control, 1, 0));
    control->frqncy.ncomp = oldComponents;
    control->frqncy.values = nullptr;
    CHECK(model.applyEditorFrequencyIndexIfControlMatches(control, 1, 1));
    CHECK(!model.applyEditorFrequencyIndexIfControlMatches(control, 1, 0));
    control->frqncy.values = values;
    CHECK(bits(*control) == before);

    CHECK(model.applyEditorFrequencyIndexIfControlMatches(control, 0, 1));
    CHECK_EQ(std::bit_cast<std::uint64_t>(values[0]), std::uint64_t{0xbff0000000000000});
    for (int i = 1; i < 6; ++i) CHECK_EQ(std::bit_cast<std::uint64_t>(values[i]), before[i]);
    before = bits(*control);
    CHECK(model.applyEditorFrequencyIndexIfControlMatches(control, 4, 0));
    CHECK_EQ(std::bit_cast<std::uint64_t>(values[4]), std::uint64_t{0});
    for (int i = 0; i < 6; ++i) if (i != 4)
        CHECK_EQ(std::bit_cast<std::uint64_t>(values[i]), before[i]);
}

TEST_CASE(frequency_fill_projects_known_enabled_arguments_from_each_native_selection)
{
    for (int first : {0, -1, 5}) {
        Fixture f; auto& sc = *f.operation.engineModel->simCtl(); REQUIRE(sc.frqncy.values);
        const std::array<int, 6> values{first, 0, -2, (std::numeric_limits<std::int32_t>::min)(),
            (std::numeric_limits<std::int32_t>::max)(), 7};
        for (int i = 0; i < 6; ++i) sc.frqncy.values[i] = values[i];
        const auto& form = f.open().pages.advanced;
        for (int row = 0; row < 6; ++row) {
            REQUIRE(form.frequencyInputEnabled[row]);
            const bool expected = row == 0 ? first != 0 && first != -1 : values[row] != 0;
            CHECK(*form.frequencyInputEnabled[row] == expected);
        }
    }
}

TEST_CASE(frequency_automatic_zero_stores_only_addressed_double_and_disables_without_text_change)
{
    for (int row = 0; row < 6; ++row) {
        Fixture f; auto& session = f.open(); auto& form = session.pages.advanced;
        auto& sc = *f.operation.engineModel->simCtl();
        for (int i = 0; i < 6; ++i) sc.frqncy.values[i] = 20 + i;
        sc.frqncy.values[row] = std::bit_cast<double>(std::uint64_t{0x7ff8000000000042});
        const auto before = bits(sc); const int reads = f.io.reads;
        form.frequencyCombo[row] = 0; form.frequencyInputEnabled[row] = true;
        form.populatedFrequencyText[row] = qt::L("REMEMBERED INVALID TEXT");
        CHECK(f.owner->frequencyIndexChanged(f.binding(), row, 0) == ApplyResult::Completed);
        CHECK_EQ(std::bit_cast<std::uint64_t>(sc.frqncy.values[row]), std::uint64_t{0});
        REQUIRE(form.frequencyInputEnabled[row]); CHECK(!*form.frequencyInputEnabled[row]);
        const auto after = bits(sc);
        for (int i = 0; i < 6; ++i) if (i != row) CHECK_EQ(after[i], before[i]);
        CHECK(*form.populatedFrequencyText[row] == qt::L("REMEMBERED INVALID TEXT"));
        CHECK_EQ(form.frequencyCombo[row], 0); CHECK_EQ(session.pages.stepCount.steps, 23);
        CHECK_EQ(f.io.reads, reads); CHECK_EQ(f.io.writes, 0); CHECK_EQ(f.io.removes, 0);
        CHECK_EQ(f.operation.nstartGiven, 0);
    }
}

TEST_CASE(frequency_first_row_sentinel_then_custom_retains_text_until_explicit_edit)
{
    Fixture f; auto& form = f.open().pages.advanced; auto& sc = *f.operation.engineModel->simCtl();
    form.populatedFrequencyText[0] = qt::L("123"); form.frequencyCombo[0] = 1;
    CHECK(f.owner->frequencyIndexChanged(f.binding(), 0, 1) == ApplyResult::Completed);
    CHECK_EQ(std::bit_cast<std::uint64_t>(sc.frqncy.values[0]), std::uint64_t{0xbff0000000000000});
    REQUIRE(form.frequencyInputEnabled[0]); CHECK(!*form.frequencyInputEnabled[0]);
    form.frequencyCombo[0] = 2;
    CHECK(f.owner->frequencyIndexChanged(f.binding(), 0, 2) == ApplyResult::Completed);
    CHECK(*form.frequencyInputEnabled[0]); CHECK(sc.frqncy.values[0] == -1);
    CHECK(*form.populatedFrequencyText[0] == qt::L("123"));
    CHECK(f.owner->frequencyTextEdited(f.binding(), 0, *form.populatedFrequencyText[0],
        NumericLocale::KnownC) == ApplyResult::Completed);
    CHECK(sc.frqncy.values[0] == 123); CHECK(*form.frequencyInputEnabled[0]);
}

TEST_CASE(frequency_auto_custom_and_text_delivery_are_distinct_for_all_six_rows)
{
    for (int row = 0; row < 6; ++row) {
        Fixture f; auto& form = f.open().pages.advanced; auto& sc = *f.operation.engineModel->simCtl();
        sc.frqncy.values[row] = 87; form.populatedFrequencyText[row] = qt::L("55");
        form.frequencyCombo[row] = 0;
        CHECK(f.owner->frequencyIndexChanged(f.binding(), row, 0) == ApplyResult::Completed);
        CHECK(sc.frqncy.values[row] == 0); CHECK(!*form.frequencyInputEnabled[row]);
        const int custom = row == 0 ? 2 : 1;
        form.frequencyCombo[row] = custom; // cached change alone delivers nothing
        CHECK(sc.frqncy.values[row] == 0); CHECK(!*form.frequencyInputEnabled[row]);
        CHECK(f.owner->frequencyIndexChanged(f.binding(), row, custom) == ApplyResult::Completed);
        CHECK(sc.frqncy.values[row] == 0); CHECK(*form.frequencyInputEnabled[row]);
        CHECK(f.owner->frequencyTextEdited(f.binding(), row, *form.populatedFrequencyText[row],
            NumericLocale::KnownC) == ApplyResult::Completed);
        CHECK(sc.frqncy.values[row] == 55); CHECK(*form.frequencyInputEnabled[row]);
    }
}

TEST_CASE(frequency_explicit_repeated_same_index_reasserts_model_and_requested_enable)
{
    Fixture f; auto& form = f.open().pages.advanced; auto& sc = *f.operation.engineModel->simCtl();
    form.frequencyCombo[0] = 0;
    CHECK(f.owner->frequencyIndexChanged(f.binding(), 0, 0) == ApplyResult::Completed);
    sc.frqncy.values[0] = 9; form.frequencyInputEnabled[0] = true;
    CHECK(f.owner->frequencyIndexChanged(f.binding(), 0, 0) == ApplyResult::Completed);
    CHECK(sc.frqncy.values[0] == 0); CHECK(!*form.frequencyInputEnabled[0]);
}

TEST_CASE(frequency_every_choice_requires_current_sc_and_stride_before_enable_projection)
{
    for (int row = 0; row < 6; ++row) for (int selection = 0; selection <= (row == 0 ? 2 : 1); ++selection)
        for (int variant = 0; variant < 3; ++variant) {
            Fixture f; auto& form = f.open().pages.advanced; auto& model = *f.operation.engineModel;
            auto& original = *model.simCtl(); const auto before = bits(original);
            form.frequencyCombo[row] = selection; form.frequencyInputEnabled[row].reset();
            std::unique_ptr<def::model::DB_STEP_DATA> heldStep;
            if (variant < 2) heldStep = std::move(model.step);
            if (variant == 1) model.step = std::make_unique<def::model::DB_STEP_DATA>(3);
            if (variant == 2) original.frqncy_cols = 2;
            const auto result = f.owner->frequencyIndexChanged(f.binding(), row, selection);
            original.frqncy_cols = 1;
            CHECK(result == ApplyResult::UnsupportedSnapshot); CHECK(bits(original) == before);
            CHECK(!form.frequencyInputEnabled[row]); CHECK_EQ(form.frequencyCombo[row], selection);
        }
}

TEST_CASE(frequency_store_needs_values_shape_but_custom_does_not_consume_it)
{
    for (int row = 0; row < 6; ++row) for (int variant = 0; variant < 3; ++variant) {
        Fixture f; auto& form = f.open().pages.advanced; auto& sc = *f.operation.engineModel->simCtl();
        const auto before = bits(sc); auto* values = sc.frqncy.values;
        form.frequencyCombo[row] = 0; form.frequencyInputEnabled[row].reset();
        if (variant == 0) sc.frqncy_rows = 5;
        if (variant == 1) sc.frqncy.ncomp = 5;
        if (variant == 2) sc.frqncy.values = nullptr;
        const auto automatic = f.owner->frequencyIndexChanged(f.binding(), row, 0);
        const bool remainedUnknown = !form.frequencyInputEnabled[row];
        const int custom = row == 0 ? 2 : 1; form.frequencyCombo[row] = custom;
        const auto enabled = f.owner->frequencyIndexChanged(f.binding(), row, custom);
        sc.frqncy_rows = 6; sc.frqncy.ncomp = 6; sc.frqncy.values = values;
        CHECK(automatic == ApplyResult::UnsupportedSnapshot); CHECK(remainedUnknown);
        CHECK(enabled == ApplyResult::Completed); REQUIRE(form.frequencyInputEnabled[row]);
        CHECK(*form.frequencyInputEnabled[row]); CHECK(bits(sc) == before);
    }
}

TEST_CASE(frequency_invalid_delivery_cache_or_binding_refuses_before_any_effect)
{
    Fixture f; auto& form = f.open().pages.advanced; auto& sc = *f.operation.engineModel->simCtl();
    const auto before = bits(sc); const auto enabled = form.frequencyInputEnabled;
    for (const auto choice : {std::pair{-1, 0}, std::pair{6, 0}, std::pair{0, -1},
             std::pair{0, 3}, std::pair{1, 2}})
        CHECK(f.owner->frequencyIndexChanged(f.binding(), choice.first, choice.second) == ApplyResult::UnsupportedSnapshot);
    form.frequencyCombo[0] = 0;
    CHECK(f.owner->frequencyIndexChanged(f.binding(), 0, 2) == ApplyResult::UnsupportedSnapshot);
    Project other;
    CHECK(f.owner->frequencyIndexChanged({other, f.operation, *f.operation.engineModel, f.remesh}, 0, 0)
        == ApplyResult::UnsupportedSnapshot);
    CHECK(bits(sc) == before); CHECK(form.frequencyInputEnabled == enabled);
}

TEST_CASE(frequency_index_unconstructed_partial_and_expired_opi_remain_guarded)
{
    Fixture f;
    CHECK(f.owner->frequencyIndexChanged(f.binding(), 0, 0) == ApplyResult::UnsupportedSnapshot);
    CHECK(!f.owner->partialPageState());
    auto context = f.context(); context.moObservations.reset();
    f.owner->constructPageState(f.binding(), f.io, context);
    CHECK(f.owner->frequencyIndexChanged(f.binding(), 0, 0) == ApplyResult::UnsupportedSnapshot);
    CHECK(!f.owner->pageState());
    Fixture live; auto& form = live.open().pages.advanced; form.frequencyCombo[0] = 2;
    const auto enabled = form.frequencyInputEnabled;
    REQUIRE(live.operation.commitOperationInfoQObjectBoundary(
        "declared private frequency-index fixture OPI boundary; not native destruction"));
    CHECK(live.owner->frequencyIndexChanged(live.binding(), 0, 2) == ApplyResult::UnsupportedSnapshot);
    CHECK(form.frequencyInputEnabled == enabled); CHECK(!StagedEditor300::completeForPublicSave);
}

int main() { return testkit::run_all(); }
