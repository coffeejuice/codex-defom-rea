// ctest-labels: m2m-mdt
// Bounded private solver6 Fill, not native QWidget/public Session completion.
#include <testkit/check.hpp>
#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>
#include <limits>
#include <memory>
#include <optional>
#include "m2m/engine/simcontrol300_owner.hpp"
#include "m2m/mst/writer.hpp"
#include <defmodel/layout/db_step_data.hpp>

using namespace m2m;
using namespace m2m::engine::simcontrol300;
namespace {
void explicitSource(SimControl& sc)
{
    sc.solmtd = 6; sc.u2e8 = 77;
    sc.solmtd_p[0] = 1.234567891; sc.solmtd_p[1] = 12345678.9;
    sc.solmtd_p[2] = (std::numeric_limits<std::int32_t>::min)();
    sc.solmtd_p[3] = (std::numeric_limits<std::int32_t>::max)();
    sc.itrmth = 2; sc.solmtt = 4; sc.cvgerr[0] = 0.125; sc.cvgerr[1] = 0.5;
    sc.defbwd = 1; sc.tmpbwd = 0; sc.itrmxd = 312; sc.itrmxt = 13;
}
std::array<std::uint64_t, 4> bits(const SimControl& sc)
{
    std::array<std::uint64_t, 4> result;
    for (int i = 0; i < 4; ++i) result[i] = std::bit_cast<std::uint64_t>(sc.solmtd_p[i]);
    return result;
}
void text(const IterationPage& form, int index, const char* expected)
{
    REQUIRE(form.explicitParameterText[index]);
    CHECK(*form.explicitParameterText[index] == qt::L(expected));
}
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
        sc.trans[0] = sc.trans[1] = 1; explicitSource(sc);
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
        const auto& report = owner->constructPageState(binding(), io, context());
        CHECK(report.state == Editor300ConstructorState::AwaitingNativeBoundaries);
        auto* pages = owner->pageState(); REQUIRE(pages); return *pages;
    }
};
}

TEST_CASE(explicit_fill_populates_g6_then_integer_text_without_model_normalization)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); explicitSource(sc);
    const auto before = bits(sc);
    const auto form = fillIteration3D(sc, model.step->dimension, NumericLocale::KnownC); REQUIRE(form);
    CHECK_EQ(form->solver, 6); REQUIRE(form->explicitControlsVisible); CHECK(*form->explicitControlsVisible);
    text(*form, 0, "1.23457"); text(*form, 1, "1.23457e+07");
    text(*form, 2, "-2147483648"); text(*form, 3, "2147483647");
    CHECK_EQ(sc.u2e8, 77); CHECK_EQ(sc.solmtd, 6); CHECK(bits(sc) == before);
    CHECK_EQ(form->method, 2); CHECK_EQ(form->thermalSolver, 4);
    CHECK(form->deformationBandwidth); CHECK(!form->thermalBandwidth);
    CHECK(form->deformationIterations.text == qt::L("312"));
    CHECK(form->thermalIterations.text == qt::L("13"));
}

TEST_CASE(explicit_fill_integer_boundaries_signed_zero_and_ignored_count_are_preserved)
{
    const std::array<std::array<double, 2>, 4> pairs{{
        {0.0, -0.0}, {-1.0, 1.0}, {1073741824.0, -1073741824.0}, {2147483646.0, -2147483647.0}}};
    const std::array<std::array<const char*, 2>, 4> expected{{
        {"0", "0"}, {"-1", "1"}, {"1073741824", "-1073741824"}, {"2147483646", "-2147483647"}}};
    for (int count : {0, 4, 77}) for (std::size_t pair = 0; pair < pairs.size(); ++pair) {
        engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); explicitSource(sc);
        sc.u2e8 = count; sc.solmtd_p[0] = 1; sc.solmtd_p[1] = -2;
        sc.solmtd_p[2] = pairs[pair][0]; sc.solmtd_p[3] = pairs[pair][1];
        const auto before = bits(sc);
        const auto form = fillIteration3D(sc, 3, NumericLocale::KnownC); REQUIRE(form);
        text(*form, 0, "1"); text(*form, 1, "-2");
        text(*form, 2, expected[pair][0]); text(*form, 3, expected[pair][1]);
        CHECK_EQ(sc.u2e8, count); CHECK(bits(sc) == before);
    }
}

TEST_CASE(explicit_fill_guards_fractional_nonfinite_or_out_of_int32_last_parameters)
{
    const std::array<double, 10> excluded{0.5, -3.9, 2147483647.5, 2147483648.0, -2147483649.0,
        (std::numeric_limits<double>::infinity)(), -(std::numeric_limits<double>::infinity)(),
        std::bit_cast<double>(std::uint64_t{0x7ff8000000000042}),
        std::bit_cast<double>(std::uint64_t{0x7ff0000000000042}),
        (std::numeric_limits<double>::denorm_min)()};
    for (int index : {2, 3}) for (double value : excluded) {
        engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); explicitSource(sc);
        sc.solmtd_p[index] = value; const auto before = bits(sc);
        CHECK(!fillIteration3D(sc, 3, NumericLocale::KnownC));
        CHECK_EQ(sc.u2e8, 77); CHECK_EQ(sc.solmtd, 6); CHECK(bits(sc) == before);
    }
}

TEST_CASE(explicit_fill_retains_nonfinite_first_parameter_and_old_fill_guards)
{
    for (int index : {0, 1}) for (double value : {(std::numeric_limits<double>::infinity)(),
            (std::numeric_limits<double>::quiet_NaN)()}) {
        engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); explicitSource(sc);
        sc.solmtd_p[index] = value; const auto before = bits(sc);
        CHECK(!fillIteration3D(sc, 3, NumericLocale::KnownC)); CHECK(bits(sc) == before);
    }
    for (int guard = 0; guard < 6; ++guard) {
        engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); explicitSource(sc);
        if (guard == 2) sc.itrmth = 3;
        if (guard == 3) sc.solmtt = 6;
        if (guard == 4) sc.cvgerr[0] = (std::numeric_limits<double>::quiet_NaN)();
        if (guard == 5) sc.solmtd = 3;
        const auto before = bits(sc);
        CHECK(!fillIteration3D(sc, guard == 0 ? 2 : 3,
            guard == 1 ? NumericLocale::Unrecovered : NumericLocale::KnownC));
        CHECK_EQ(sc.u2e8, 77); CHECK(bits(sc) == before);
    }
}

TEST_CASE(explicit_fill_does_not_populate_hidden_text_or_restrict_old_cg_profile)
{
    for (int solver : {0, 1, 2, 4, 5}) {
        engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); explicitSource(sc);
        sc.solmtd = solver; sc.u2e8 = 4;
        sc.solmtd_p[0] = 1.9; sc.solmtd_p[1] = 2.9; sc.solmtd_p[2] = -3.9; sc.solmtd_p[3] = 5.9;
        const auto before = bits(sc);
        const auto form = fillIteration3D(sc, 3, NumericLocale::KnownC); REQUIRE(form);
        REQUIRE(form->explicitControlsVisible); CHECK(!*form->explicitControlsVisible);
        for (const auto& value : form->explicitParameterText) CHECK(!value);
        CHECK(bits(sc) == before);
        if (solver == 2) {
            CHECK_EQ(form->cgFirstCombo, 1); CHECK_EQ(form->cgFourthCombo, 1);
            CHECK(form->cgSecond == qt::L("2")); CHECK(form->cgThird == qt::L("-3"));
        }
    }
}

TEST_CASE(explicit_qualified_fill_reaches_actual_bound_private_pages_without_public_completion)
{
    Fixture f; auto& sc = *f.operation.engineModel->simCtl(); const auto before = bits(sc);
    auto& session = f.open(); const auto& form = session.pages.iteration;
    CHECK_EQ(form.solver, 6); REQUIRE(form.explicitControlsVisible); CHECK(*form.explicitControlsVisible);
    text(form, 0, "1.23457"); text(form, 1, "1.23457e+07");
    text(form, 2, "-2147483648"); text(form, 3, "2147483647");
    CHECK_EQ(sc.u2e8, 77); CHECK(bits(sc) == before); CHECK(session.pageStateComplete);
    CHECK_EQ(f.io.writes, 0); CHECK_EQ(f.io.removes, 0);
    CHECK(!StagedEditor300::completeForPublicSave); CHECK(!InitialEditorSession::completeForPublicSave);
}

TEST_CASE(explicit_filled_text_survives_nonexplicit_and_explicit_visibility_events)
{
    Fixture f; auto& form = f.open().pages.iteration;
    const auto remembered = form.explicitParameterText;
    CHECK(f.owner->explicitSolverToggled(f.binding(), true) == ApplyResult::Completed);
    form.solver = 0;
    CHECK(f.owner->explicitSolverToggled(f.binding(), false) == ApplyResult::Completed);
    CHECK(!*form.explicitControlsVisible); CHECK(form.explicitParameterText == remembered);
    form.solver = 6;
    CHECK(f.owner->explicitSolverToggled(f.binding(), true) == ApplyResult::Completed);
    CHECK(*form.explicitControlsVisible); CHECK(form.explicitParameterText == remembered);
}

TEST_CASE(explicit_fill_with_unknown_or_excluded_text_keeps_apply_and_public_guards)
{
    Fixture f; auto& session = f.open(); auto& sc = *f.operation.engineModel->simCtl();
    const auto before = bits(sc); const int count = sc.u2e8;
    auto form = session.pages.iteration;
    form.explicitParameterText[2].reset();
    CHECK(applyIteration3D(sc, form) == ApplyResult::UnsupportedSnapshot);
    CHECK_EQ(sc.u2e8, count); CHECK_EQ(sc.solmtd, 6); CHECK(bits(sc) == before);
    form = session.pages.iteration;
    form.explicitParameterText[3] = qt::L("1e33");
    CHECK(applyIteration3D(sc, form) == ApplyResult::UnsupportedSnapshot);
    CHECK_EQ(sc.u2e8, count); CHECK_EQ(sc.solmtd, 6); CHECK(bits(sc) == before);
    CHECK(f.owner->construction().state == Editor300ConstructorState::AwaitingNativeBoundaries);
    CHECK(!StagedEditor300::completeForPublicSave);
}

int main() { return testkit::run_all(); }
