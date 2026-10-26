// ctest-labels: m2m-mdt
// Actual typed owners/private page composition; no physical curve dialog claim.
#include <testkit/check.hpp>
#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include "m2m/engine/simcontrol300_owner.hpp"
#include "m2m/mst/writer.hpp"
#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/window/inter_object_ops.hpp>

using namespace m2m;
using namespace m2m::engine::simcontrol300;
namespace {
constexpr std::uint64_t nanBits = 0x7ff8000000000042ULL;
void makeTable(def::model::DYNARY& curve, int variable, int points = 2)
{
    def::model::dynary_free_arrays(&curve);
    curve.type = 0x80; curve.ndim = 1; curve.ncomp = 1;
    curve.var[0] = variable; curve.var[1] = 37; curve.var[2] = 41;
    curve.n[0] = points; curve.n[1] = curve.n[2] = 0;
    curve.u50 = 17; curve.u54 = 19;
    curve.value = std::bit_cast<double>(std::uint64_t{0x7ff8000000000077});
    def::model::dynary_alloc(&curve); REQUIRE(curve.axis[0]); REQUIRE(curve.values);
    for (int i = 0; i < points; ++i) { curve.axis[0][i] = i + 0.125; curve.values[i] = i + 0.25; }
    curve.values[0] = std::bit_cast<double>(nanBits); curve.values[1] = -0.0;
}
void scalars(SimControl& sc)
{
    sc.dttype.u58 = 0; sc.stpdef = 1;
    sc.dtmax1 = 1.23456789; sc.dsmax = 2.34567891; sc.demax = 0.25;
    sc.dplen = 0.5; sc.dtsub = 3; sc.dtpmax1 = 4; sc.dtpmax2_5[0] = 5;
    sc.dtpmax2_5[1] = 6; sc.slderr = 7;
}
void sameTable(const def::model::DYNARY& a, const def::model::DYNARY& b)
{
    CHECK_EQ(a.type, b.type); CHECK_EQ(a.ndim, b.ndim); CHECK_EQ(a.ncomp, b.ncomp);
    CHECK_EQ(a.u50, b.u50); CHECK_EQ(a.u54, b.u54);
    CHECK_EQ(std::bit_cast<std::uint64_t>(a.value), std::bit_cast<std::uint64_t>(b.value));
    REQUIRE(a.n[0] == b.n[0]); REQUIRE(a.axis[0]); REQUIRE(b.axis[0]); REQUIRE(a.values); REQUIRE(b.values);
    for (int i = 0; i < 3; ++i) { CHECK_EQ(a.var[i], b.var[i]); CHECK_EQ(a.n[i], b.n[i]); }
    for (int i = 0; i < a.n[0]; ++i) {
        CHECK_EQ(std::bit_cast<std::uint64_t>(a.axis[0][i]), std::bit_cast<std::uint64_t>(b.axis[0][i]));
        CHECK_EQ(std::bit_cast<std::uint64_t>(a.values[i]), std::bit_cast<std::uint64_t>(b.values[i]));
    }
}
struct Sink final : mst::OutputDevice {
    bool open = true;
    std::int64_t write(std::string_view value) override { return static_cast<std::int64_t>(value.size()); }
    bool flush() override { return true; }
    void close() override { notifyAboutToClose(); open = false; }
    bool opened() const override { return open; }
};
struct Io final : Extra3DIo {
    Io() : Extra3DIo(1252) {}
    std::function<void()> lastInitialRead;
    int removes = 0, writes = 0;
    bool readDoubles(const qt::String& path, std::span<double> values) override
    {
        std::fill(values.begin(), values.end(), 0.0);
        if (path == qt::L("NBC.DAT") && lastInitialRead) {
            auto call = std::move(lastInitialRead); lastInitialRead = {}; call();
        }
        return false;
    }
    bool readInts(const qt::String&, std::span<int> values) override
    { std::fill(values.begin(), values.end(), 0); return false; }
    bool readSprings(std::vector<SpringRecord>& values) override { values.clear(); return false; }
    bool removeIfReadable(const qt::String&) override { ++removes; return true; }
    void nbcRemoveWarning() override {}
    std::unique_ptr<mst::OutputDevice> openWrite(const qt::String&) override
    { ++writes; return std::make_unique<Sink>(); }
};
struct Fixture {
    Project project; Operation operation; RemeshState remesh; Io io;
    std::unique_ptr<StagedEditor300> owner;
    Fixture(int variable = 1)
    {
        auto sub = std::make_unique<SubProject>(); sub->project = &project;
        project.subs.push_back(std::move(sub));
        operation.project = &project; operation.engineModel = std::make_shared<engine::GuiModel>(nullptr, 3);
        auto& sc = control(); sc.unit = 1; sc.nstart = 7; sc.nstep = 23;
        sc.trans[0] = sc.trans[1] = 1; scalars(sc); makeTable(sc.dttype, variable);
        owner = std::make_unique<StagedEditor300>(binding());
    }
    engine::GuiModel& model() { return *operation.engineModel; }
    SimControl& control() { return *model().simCtl(); }
    Editor300Binding binding() { return {project, operation, model(), remesh}; }
    InitialEditorContext context()
    {
        MoInitialObservations observed;
        observed.generalFills[0] = MoFeatureAvailability{1, std::nullopt, {1, 1, 1}};
        observed.generalFills[1] = MoFeatureAvailability{1, std::nullopt, {1, 1, 1}};
        observed.startupHeating = {0, 1};
        return {generalContextForMo(project, 3, {true, true}),
            {5, {}, MoStepCountContext{&operation}}, NumericLocale::KnownC, true, observed};
    }
    InitialEditorSession& open()
    {
        owner->constructPageState(binding(), io, context());
        auto* session = owner->pageState(); REQUIRE(session); return *session;
    }
};
}

TEST_CASE(stepsize_table_fill_keeps_bounded_definition_and_raw_samples_for_all_modes)
{
    for (int variable : {1, 2}) for (int mode : {0, 1}) for (int stpdef : {1, 2, 3}) {
        engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); scalars(sc);
        makeTable(sc.dttype, variable); sc.dttype.u58 = mode; sc.stpdef = stpdef;
        const auto* axis = sc.dttype.axis[0]; const auto* values = sc.dttype.values;
        const auto form = fillStepSize3D(sc, 3, NumericLocale::KnownC); REQUIRE(form); REQUIRE(form->curve);
        CHECK_EQ(form->modeRadio, stpdef == 3 ? 2 : mode); CHECK_EQ(form->definitionRadio, stpdef == 1 ? 1 : 2);
        CHECK_EQ(sc.dttype.u58, stpdef == 3 ? 1 : mode);
        CHECK(sc.dttype.axis[0] == axis && sc.dttype.values == values);
        CHECK(form->curve.get() != &sc.dttype && form->curve->axis[0] != axis && form->curve->values != values);
        sameTable(*form->curve, sc.dttype);
        CHECK(form->timeMaximum.text == qt::L("1.23457"));
        CHECK(form->displacementMaximum.text == qt::L("2.34568"));
    }
}

TEST_CASE(stepsize_table_actual_constructor_retains_late_table_before_backed_refill_and_restore)
{
    Fixture f(2); auto& sc = f.control(); sc.stpdef = 3; sc.dttype.u58 = 0;
    f.io.lastInitialRead = [&] { sc.dttype.values[0] = 21.25; sc.dttype.axis[0][1] = 9.5; };
    auto& session = f.open(); const auto& report = f.owner->construction(); REQUIRE(report.pages);
    CHECK(report.pages->complete()); CHECK(report.pages->temporariesCopied);
    CHECK(report.pages->backupCreated && report.pages->backupRestored);
    CHECK(std::all_of(report.pages->initialPages.begin(), report.pages->initialPages.end(), [](bool v) { return v; }));
    CHECK(std::all_of(report.pages->refreshedPages.begin(), report.pages->refreshedPages.end(), [](bool v) { return v; }));
    REQUIRE(session.pages.stepSize.curve); sameTable(*session.pages.stepSize.curve, sc.dttype);
    CHECK_EQ(sc.dttype.u58, 1); CHECK_EQ(session.pages.stepSize.modeRadio, 2);
    CHECK(session.pages.stepSize.curve->values[0] == 21.25 && sc.dttype.axis[0][1] == 9.5);
    CHECK(session.pages.stepSize.curve->axis[0] != sc.dttype.axis[0]);
    CHECK(session.pages.stepSize.curve->values != sc.dttype.values);
    CHECK(!StagedEditor300::completeForPublicSave && !InitialEditorSession::completeForPublicSave);
}

TEST_CASE(stepsize_table_owner_bound_save_restores_remembered_table_and_selected_mode)
{
    Fixture f; auto& session = f.open(); auto& sc = f.control(); auto& form = session.pages.stepSize;
    const auto remembered = form.curve; REQUIRE(remembered); CHECK_EQ(remembered->u58, 0);
    sc.dttype.values[0] = 99; sc.dttype.axis[0][1] = 100;
    form.modeRadio = 1; form.timeMaximum = {qt::L("8.25"), ValidatorState::Acceptable};
    const auto report = applyInitialEditorPageState(session, f.model(), f.remesh, f.io, f.operation);
    CHECK(report.outcome == ApplyResult::Completed);
    REQUIRE(report.pages[static_cast<std::size_t>(Page::StepSize)]);
    CHECK(*report.pages[static_cast<std::size_t>(Page::StepSize)] == ApplyResult::Completed);
    REQUIRE(report.pages[static_cast<std::size_t>(Page::Extra3D)]); REQUIRE(report.recoveryTail);
    CHECK(*report.recoveryTail == ApplyResult::Completed);
    sameTable(sc.dttype, *remembered); CHECK_EQ(sc.dttype.u58, 1); CHECK_EQ(remembered->u58, 0);
    CHECK(sc.dtmax1 == 8.25 && sc.dsmax == 0); CHECK(sc.dttype.values != remembered->values);
    CHECK(form.curve == remembered); CHECK(!InitialApplyReport::completeForPublicSave);
}

TEST_CASE(stepsize_table_partial_scalar_validation_defers_copy_without_rolling_back_prefix)
{
    for (bool late : {false, true}) {
        engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); scalars(sc); makeTable(sc.dttype, 1);
        auto form = fillStepSize3D(sc, 3, NumericLocale::KnownC); REQUIRE(form);
        sc.dttype.values[0] = 91; const auto* axis = sc.dttype.axis[0]; const auto* values = sc.dttype.values;
        sc.demax = 19; sc.slderr = 29;
        form->modeRadio = 1; form->timeMaximum = {qt::L("8.25"), ValidatorState::Acceptable};
        (late ? form->slidingError : form->strainMaximum) = {qt::L("bad"), ValidatorState::Invalid};
        CHECK(applyStepSize3D(sc, *form) == ApplyResult::ValidationStopped);
        CHECK_EQ(sc.dttype.u58, 1); CHECK(sc.dtmax1 == 8.25 && sc.dsmax == 0); CHECK_EQ(sc.stpdef, 1);
        CHECK(sc.dttype.axis[0] == axis && sc.dttype.values == values && sc.dttype.values[0] == 91);
        CHECK_EQ(std::bit_cast<std::uint64_t>(form->curve->values[0]), nanBits);
        if (!late) CHECK(sc.demax == 19); else CHECK(sc.slderr == 29);
    }
}

TEST_CASE(stepsize_table_validation_stop_still_allows_later_owned_pages_and_recovery_tail)
{
    Fixture f; auto& session = f.open(); auto& sc = f.control();
    sc.dttype.values[0] = 91; const auto* values = sc.dttype.values; const int removes = f.io.removes;
    session.pages.stepSize.displacementMaximum = {qt::L("bad"), ValidatorState::Invalid};
    const auto report = applyInitialEditorPageState(session, f.model(), f.remesh, f.io, f.operation);
    CHECK(report.outcome == ApplyResult::ValidationStopped);
    REQUIRE(report.pages[static_cast<std::size_t>(Page::StepSize)]);
    CHECK(*report.pages[static_cast<std::size_t>(Page::StepSize)] == ApplyResult::ValidationStopped);
    REQUIRE(report.pages[static_cast<std::size_t>(Page::Environment)]);
    REQUIRE(report.pages[static_cast<std::size_t>(Page::Extra3D)]); REQUIRE(report.recoveryTail);
    CHECK(*report.recoveryTail == ApplyResult::Completed); CHECK(f.io.removes > removes);
    CHECK(sc.dttype.values == values && sc.dttype.values[0] == 91);
}

TEST_CASE(stepsize_table_alias_guards_precede_mode_and_scalar_effects)
{
    for (int mode = 0; mode < 7; ++mode) {
        engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); scalars(sc); makeTable(sc.dttype, 1, 3);
        auto filled = fillStepSize3D(sc, 3, NumericLocale::KnownC); REQUIRE(filled); auto form = *filled;
        auto temporary = std::make_shared<def::model::DYNARY>();
        def::model::dynary_copy(temporary.get(), form.curve.get());
        const auto* liveAxis = sc.dttype.axis[0]; const auto* liveValues = sc.dttype.values;
        auto* axis = temporary->axis[0]; auto* values = temporary->values;
        if (mode == 0) form.curve = std::shared_ptr<const def::model::DYNARY>(&sc.dttype, [](const auto*) {});
        else {
            form.curve = temporary;
            if (mode == 1) temporary->axis[0] = sc.dttype.axis[0];
            if (mode == 2) temporary->values = sc.dttype.values;
            if (mode == 3) temporary->axis[0] = sc.dttype.values;
            if (mode == 4) temporary->values = sc.dttype.axis[0];
            if (mode == 5) { temporary->axis[0] = sc.dttype.axis[0] + 1; temporary->n[0] = 2; }
            if (mode == 6) temporary->values = temporary->axis[0];
        }
        form.modeRadio = 1; form.timeMaximum = {qt::L("8.25"), ValidatorState::Acceptable};
        const double oldTime = sc.dtmax1, oldDistance = sc.dsmax;
        CHECK(applyStepSize3D(sc, form) == ApplyResult::UnsupportedSnapshot);
        CHECK_EQ(sc.dttype.u58, 0); CHECK(sc.dtmax1 == oldTime && sc.dsmax == oldDistance);
        CHECK(sc.dttype.axis[0] == liveAxis && sc.dttype.values == liveValues);
        CHECK_EQ(std::bit_cast<std::uint64_t>(sc.dttype.values[0]), nanBits);
        temporary->axis[0] = axis; temporary->values = values; temporary->n[0] = 3;
    }
}

TEST_CASE(stepsize_table_profile_and_destination_guards_refuse_before_normalization)
{
    for (int mode = 0; mode < 9; ++mode) {
        engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); scalars(sc); makeTable(sc.dttype, 1);
        const auto form = fillStepSize3D(sc, 3, NumericLocale::KnownC); REQUIRE(form);
        auto* axis = sc.dttype.axis[0]; auto* values = sc.dttype.values;
        std::unique_ptr<def::model::DYNARY> nested;
        if (mode == 0) sc.dttype.ndim = 2;
        if (mode == 1) sc.dttype.ncomp = 2;
        if (mode == 2) sc.dttype.var[0] = 3;
        if (mode == 3) sc.dttype.n[0] = 0;
        if (mode == 4) sc.dttype.n[0] = 4097;
        if (mode == 5) sc.dttype.type = 0x81;
        if (mode == 6) sc.dttype.axis[0] = nullptr;
        if (mode == 7) sc.dttype.values = nullptr;
        if (mode == 8) { nested = std::make_unique<def::model::DYNARY>(); sc.dttype.u68 = nested.get(); }
        sc.stpdef = 3;
        CHECK(!fillStepSize3D(sc, 3, NumericLocale::KnownC)); CHECK_EQ(sc.dttype.u58, 0);
        CHECK(applyStepSize3D(sc, *form) == ApplyResult::UnsupportedSnapshot); CHECK_EQ(sc.dttype.u58, 0);
        sc.dttype.type = 0x80; sc.dttype.ndim = 1; sc.dttype.ncomp = 1; sc.dttype.var[0] = 1;
        sc.dttype.n[0] = 2; sc.dttype.axis[0] = axis; sc.dttype.values = values; sc.dttype.u68 = nullptr;
    }
}

TEST_CASE(stepsize_table_backed_refresh_refusal_keeps_common_backup_restore)
{
    Fixture f; auto& sc = f.control(); sc.stpdef = 3;
    auto context = f.context(); context.moObservations->generalFills[1].reset();
    const auto& report = f.owner->constructPageState(f.binding(), f.io, context); REQUIRE(report.pages);
    CHECK(report.state == Editor300ConstructorState::PageRefused);
    CHECK(report.pages->stage == InitialOpenStage::BackedRefresh);
    CHECK(report.pages->backupCreated && report.pages->backupRestored); CHECK_EQ(sc.dttype.u58, 1);
    const auto* partial = f.owner->partialPageState(); REQUIRE(partial); REQUIRE(partial->pages.stepSize.curve);
    sameTable(*partial->pages.stepSize.curve, sc.dttype);
    CHECK(!partial->pageStateComplete); CHECK(!StagedEditor300::completeForPublicSave);
}

int main() { return testkit::run_all(); }
