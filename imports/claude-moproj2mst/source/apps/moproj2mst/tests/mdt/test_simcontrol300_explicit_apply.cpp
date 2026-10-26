// ctest-labels: m2m-mdt
// Private numeric-value Apply; no native Qt/public Session completion claim.
#include <testkit/check.hpp>
#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include "m2m/engine/simcontrol300_owner.hpp"
#include "m2m/mst/writer.hpp"
#include <defmodel/layout/db_step_data.hpp>

using namespace m2m;
using namespace m2m::engine::simcontrol300;
namespace {
void source(SimControl& sc)
{
    sc.solmtd = 6; sc.u2e8 = 77;
    sc.solmtd_p[0] = 1.234567891; sc.solmtd_p[1] = 12345678.9;
    sc.solmtd_p[2] = -3; sc.solmtd_p[3] = 9;
    sc.itrmth = 2; sc.solmtt = 4;
    sc.cvgerr[0] = 0.125; sc.cvgerr[1] = 0.5;
    sc.defbwd = 1; sc.tmpbwd = 0; sc.itrmxd = 312; sc.itrmxt = 13;
}
std::array<std::uint64_t, 4> bits(const SimControl& sc)
{
    std::array<std::uint64_t, 4> result;
    for (int i = 0; i < 4; ++i) result[i] = std::bit_cast<std::uint64_t>(sc.solmtd_p[i]);
    return result;
}
IterationPage filled(SimControl& sc)
{
    source(sc); const auto form = fillIteration3D(sc, 3, NumericLocale::KnownC);
    REQUIRE(form); return *form;
}
void texts(IterationPage& form, const std::array<const char*, 4>& values)
{ for (int i = 0; i < 4; ++i) form.explicitParameterText[i] = qt::L(values[i]); }
void oldTail(SimControl& sc)
{
    sc.solmtd = 1; sc.u2e8 = 99; sc.itrmth = 0; sc.solmtt = 0;
    sc.cvgerr[0] = 7; sc.cvgerr[1] = 8;
    sc.defbwd = 42; sc.tmpbwd = 43; sc.itrmxd = 44; sc.itrmxt = 45;
}
void explicitPrefix(const SimControl& sc)
{
    CHECK_EQ(sc.u2e8, 4); CHECK_EQ(sc.solmtd, 6); CHECK_EQ(sc.itrmth, 2); CHECK_EQ(sc.solmtt, 4);
    CHECK(sc.solmtd_p[0] == 1.5); CHECK(sc.solmtd_p[1] == 2.5);
    CHECK(sc.solmtd_p[2] == 3.5); CHECK(sc.solmtd_p[3] == 4.5);
}
struct Io final : Extra3DIo {
    Io() : Extra3DIo(1252) {}
    bool readDoubles(const qt::String&, std::span<double> values) override
    { std::fill(values.begin(), values.end(), 0.0); return false; }
    bool readInts(const qt::String&, std::span<int> values) override
    { std::fill(values.begin(), values.end(), 0); return false; }
    bool readSprings(std::vector<SpringRecord>& values) override { values.clear(); return false; }
    bool removeIfReadable(const qt::String&) override { return true; }
    void nbcRemoveWarning() override {}
    std::unique_ptr<mst::OutputDevice> openWrite(const qt::String&) override { return nullptr; }
};
}

TEST_CASE(explicit_apply_consumes_actual_bound_filled_text_and_rounds_g6_values)
{
    for (int count : {0, 77}) {
        Project project; Operation operation; RemeshState remesh; Io io;
        operation.project = &project;
        operation.engineModel = std::make_shared<engine::GuiModel>(nullptr, 3);
        auto& sc = *operation.engineModel->simCtl(); source(sc); sc.u2e8 = count;
        sc.unit = 1; sc.nstart = 7; sc.nstep = 23; sc.trans[0] = sc.trans[1] = 1;
        Editor300Binding binding{project, operation, *operation.engineModel, remesh};
        StagedEditor300 owner(binding);
        MoInitialObservations observed;
        observed.generalFills[0] = MoFeatureAvailability{1, std::nullopt, {1, 1, 1}};
        observed.generalFills[1] = MoFeatureAvailability{1, std::nullopt, {1, 1, 1}};
        observed.startupHeating = {0, 1};
        const InitialEditorContext context{generalContextForMo(project, 3, {true, true}),
            {5, {}, MoStepCountContext{&operation}}, NumericLocale::KnownC, true, observed};
        owner.constructPageState(binding, io, context); auto* session = owner.pageState(); REQUIRE(session);
        auto& form = session->pages.iteration; const auto remembered = form.explicitParameterText;
        CHECK(applyIteration3D(sc, form) == ApplyResult::Completed);
        CHECK_EQ(sc.u2e8, 4); CHECK_EQ(sc.solmtd, 6);
        CHECK(sc.solmtd_p[0] == 1.23457); CHECK(sc.solmtd_p[1] == 12345700.0);
        CHECK(sc.solmtd_p[2] == -3); CHECK(sc.solmtd_p[3] == 9);
        CHECK(form.explicitParameterText == remembered);
        CHECK(owner.construction().state == Editor300ConstructorState::AwaitingNativeBoundaries);
        CHECK(!StagedEditor300::completeForPublicSave);
    }
}

TEST_CASE(explicit_apply_uses_double_for_last_two_and_has_no_visibility_predicate)
{
    for (bool known : {false, true}) {
        engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); auto form = filled(sc);
        texts(form, {"+1.5", ".25", "-3.25", "2.5e-1"});
        if (known) form.explicitControlsVisible = false; else form.explicitControlsVisible.reset();
        sc.latent = 91;
        CHECK(applyIteration3D(sc, form) == ApplyResult::Completed);
        CHECK(sc.solmtd_p[0] == 1.5); CHECK(sc.solmtd_p[1] == 0.25);
        CHECK(sc.solmtd_p[2] == -3.25); CHECK(sc.solmtd_p[3] == 0.25);
        CHECK_EQ(sc.u2e8, 4); CHECK_EQ(sc.latent, 91);
        CHECK(form.explicitControlsVisible.has_value() == known);
        if (known) CHECK(!*form.explicitControlsVisible);
    }
}

TEST_CASE(explicit_apply_accepts_bounded_decimal_forms_and_signed_zero)
{
    const std::array<const char*, 10> forms{"1.", ".5", "-.5", "1E+02", "1e00",
        "000000000000001", "123456789012345", "1e32", "1e-32", "9.99999999999999E32"};
    const std::array<double, 10> expected{1, 0.5, -0.5, 100, 1, 1, 123456789012345.0,
        1e32, 1e-32, 9.99999999999999e32};
    for (std::size_t i = 0; i < forms.size(); ++i) {
        engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); auto form = filled(sc);
        texts(form, {forms[i], forms[i], forms[i], forms[i]});
        CHECK(applyIteration3D(sc, form) == ApplyResult::Completed);
        for (double value : sc.solmtd_p) CHECK(value == expected[i]);
    }
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); auto form = filled(sc);
    texts(form, {"-0", "-0.0e+0", "0", "+0"});
    CHECK(applyIteration3D(sc, form) == ApplyResult::Completed);
    CHECK_EQ(std::bit_cast<std::uint64_t>(sc.solmtd_p[0]), std::uint64_t{0x8000000000000000});
    CHECK_EQ(std::bit_cast<std::uint64_t>(sc.solmtd_p[1]), std::uint64_t{0x8000000000000000});
    CHECK_EQ(std::bit_cast<std::uint64_t>(sc.solmtd_p[2]), std::uint64_t{0});
    CHECK_EQ(std::bit_cast<std::uint64_t>(sc.solmtd_p[3]), std::uint64_t{0});
}

TEST_CASE(explicit_apply_preflights_all_known_texts_before_count_or_parameter_stores)
{
    const std::array<qt::String, 18> excluded{qt::String(), qt::L(""), qt::L(" "), qt::L("1 "),
        qt::L("NaN"), qt::L("Inf"), qt::L("0x1p2"), qt::L("."), qt::L("+"), qt::L("1..2"),
        qt::L("1e"), qt::L("1e+"), qt::L("1e33"), qt::L("1e-33"), qt::L("1e000"),
        qt::L("1234567890123456"), qt::String(u"\u0661"), qt::String(std::u16string{u'1', u'\0', u'2'})};
    for (int index = 0; index < 4; ++index) {
        engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); auto initial = filled(sc);
        oldTail(sc); const auto before = bits(sc);
        auto missing = initial; missing.explicitParameterText[index].reset();
        CHECK(applyIteration3D(sc, missing) == ApplyResult::UnsupportedSnapshot);
        CHECK_EQ(sc.u2e8, 99); CHECK_EQ(sc.solmtd, 1); CHECK_EQ(sc.itrmth, 0); CHECK_EQ(sc.solmtt, 0);
        CHECK(bits(sc) == before);
        for (const auto& text : excluded) {
            auto form = initial; form.explicitParameterText[index] = text;
            CHECK(applyIteration3D(sc, form) == ApplyResult::UnsupportedSnapshot);
            CHECK_EQ(sc.u2e8, 99); CHECK_EQ(sc.solmtd, 1); CHECK_EQ(sc.itrmth, 0); CHECK_EQ(sc.solmtt, 0);
            CHECK(bits(sc) == before); CHECK(sc.cvgerr[0] == 7); CHECK_EQ(sc.itrmxd, 44);
        }
    }
}

TEST_CASE(explicit_apply_invalid_first_convergence_retains_complete_parameter_prefix)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); auto form = filled(sc);
    texts(form, {"1.5", "2.5", "3.5", "4.5"}); oldTail(sc);
    form.convergence[0] = {qt::L("bad"), ValidatorState::Invalid};
    CHECK(applyIteration3D(sc, form) == ApplyResult::ValidationStopped); explicitPrefix(sc);
    CHECK(sc.cvgerr[0] == 7); CHECK(sc.cvgerr[1] == 8);
    CHECK_EQ(sc.defbwd, 42); CHECK_EQ(sc.tmpbwd, 43); CHECK_EQ(sc.itrmxd, 44); CHECK_EQ(sc.itrmxt, 45);
}

TEST_CASE(explicit_apply_invalid_second_convergence_retains_additional_first_store)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); auto form = filled(sc);
    texts(form, {"1.5", "2.5", "3.5", "4.5"}); oldTail(sc);
    form.convergence[0] = {qt::L("0.25"), ValidatorState::Intermediate};
    form.convergence[1] = {qt::L("bad"), ValidatorState::Invalid};
    CHECK(applyIteration3D(sc, form) == ApplyResult::ValidationStopped); explicitPrefix(sc);
    CHECK(sc.cvgerr[0] == 0.25); CHECK(sc.cvgerr[1] == 8);
    CHECK_EQ(sc.defbwd, 42); CHECK_EQ(sc.tmpbwd, 43); CHECK_EQ(sc.itrmxd, 44); CHECK_EQ(sc.itrmxt, 45);
}

TEST_CASE(explicit_apply_intermediate_continues_and_invalid_integer_fields_default200)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); auto form = filled(sc);
    texts(form, {"1.5", "2.5", "3.5", "4.5"}); oldTail(sc);
    form.convergence[0] = {qt::L(""), ValidatorState::Intermediate};
    form.convergence[1] = {qt::L("0.75"), ValidatorState::Acceptable};
    form.deformationBandwidth = false; form.thermalBandwidth = true;
    form.deformationIterations = {qt::L("bad"), ValidatorState::Invalid};
    form.thermalIterations = {qt::L("23"), ValidatorState::Intermediate};
    CHECK(applyIteration3D(sc, form) == ApplyResult::Completed); explicitPrefix(sc);
    CHECK(sc.cvgerr[0] == 0); CHECK(sc.cvgerr[1] == 0.75);
    CHECK_EQ(sc.defbwd, 0); CHECK_EQ(sc.tmpbwd, 1); CHECK_EQ(sc.itrmxd, 200); CHECK_EQ(sc.itrmxt, 23);
}

TEST_CASE(explicit_apply_profile_does_not_restrict_nonexplicit_or_cg_text_state)
{
    for (int solver : {0, 1, 2, 4, 5}) {
        engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); auto form = filled(sc);
        form.solver = solver; form.explicitParameterText[0].reset();
        form.explicitParameterText[3] = qt::L("NaN");
        form.cgFirstCombo = 1; form.cgSecond = qt::L("3"); form.cgThird = qt::L("-2"); form.cgFourthCombo = 0;
        const auto before = bits(sc);
        CHECK(applyIteration3D(sc, form) == ApplyResult::Completed); CHECK_EQ(sc.solmtd, solver);
        if (solver == 2) {
            CHECK_EQ(sc.u2e8, 4); CHECK(sc.solmtd_p[0] == 1); CHECK(sc.solmtd_p[1] == 3);
            CHECK(sc.solmtd_p[2] == -2); CHECK(sc.solmtd_p[3] == 1);
        } else { CHECK_EQ(sc.u2e8, 0); CHECK(bits(sc) == before); }
    }
}

TEST_CASE(explicit_apply_keeps_old_method_thermal_and_unknown_solver_preflight)
{
    for (int variant = 0; variant < 3; ++variant) {
        engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); auto form = filled(sc);
        if (variant == 0) form.method = 3;
        if (variant == 1) form.thermalSolver = 6;
        if (variant == 2) form.solver = 3;
        oldTail(sc); const auto before = bits(sc);
        CHECK(applyIteration3D(sc, form) == ApplyResult::UnsupportedSnapshot);
        CHECK_EQ(sc.u2e8, 99); CHECK_EQ(sc.solmtd, 1); CHECK(bits(sc) == before);
    }
}

int main() { return testkit::run_all(); }
