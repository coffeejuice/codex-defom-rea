// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include <algorithm>
#include <bit>
#include <cstdint>
#include <cstring>
#include <functional>
#include <initializer_list>
#include <string>
#include "m2m/engine/simcontrol300_lifecycle.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine.hpp"
#include "m2m/p17.hpp"
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/dynary_init.hpp>
#include <defmodel/llobj/cls_14017ef20.hpp>

using namespace m2m;
using namespace m2m::engine::simcontrol300;
namespace {
struct Device final : mst::OutputDevice {
    explicit Device(std::string& bytes) : bytes(bytes) {}
    std::string& bytes;
    bool open = true;
    std::int64_t write(std::string_view value) override
    {
        bytes.append(value); return static_cast<std::int64_t>(value.size());
    }
    bool flush() override { return true; }
    void close() override { notifyAboutToClose(); open = false; }
    bool opened() const override { return open; }
};
struct Io final : Extra3DIo {
    Io() : Extra3DIo(1252) {}
    bool nbcExists = true;
    int readCalls = 0;
    std::function<void()> duringInitialRead, duringFirstEffect;
    std::vector<std::string> effects;
    std::string nbcBytes;
    bool readDoubles(const qt::String& path, std::span<double> output) override
    {
        ++readCalls;
        std::fill(output.begin(), output.end(), 0.0);
        if (path != qt::L("NBC.DAT")) return false;
        if (duringInitialRead) {
            auto call = std::move(duringInitialRead); duringInitialRead = {}; call();
        }
        if (nbcExists && output.size() == 2) { output[0] = 17.0; output[1] = 29.0; }
        return nbcExists;
    }
    bool readInts(const qt::String&, std::span<int> output) override
    {
        ++readCalls;
        std::fill(output.begin(), output.end(), 0); return false;
    }
    bool readSprings(std::vector<SpringRecord>& output) override { ++readCalls; output.clear(); return false; }
    void firstEffect()
    {
        if (duringFirstEffect) {
            auto call = std::move(duringFirstEffect); duringFirstEffect = {}; call();
        }
    }
    bool removeIfReadable(const qt::String& path) override
    {
        effects.push_back("remove:" + path.utf8()); firstEffect(); return true;
    }
    void nbcRemoveWarning() override { effects.push_back("warning"); }
    std::unique_ptr<mst::OutputDevice> openWrite(const qt::String& path) override
    {
        effects.push_back("write:" + path.utf8()); firstEffect();
        return std::make_unique<Device>(nbcBytes);
    }
};
InitialEditorContext context(bool pressure = true)
{
    return {{1, 3, {true, true, true, true, true, true}, false, true},
        {1, {}}, NumericLocale::KnownC, pressure};
}
template <class T> T& field(def::model::LLOBJ_DATA* object, std::size_t offset)
{
    return *reinterpret_cast<T*>(reinterpret_cast<std::byte*>(object) + offset);
}
void initialize(SimControl& sc)
{
    std::strcpy(sc.title, "OPEN TITLE"); std::strcpy(sc.oprnam, "OPEN OPERATION");
    sc.meshno = 30000; sc.unit = 1; sc.stype = 1;
    sc.nstart = 7; sc.nstep = 23; sc.stpinc = 3;
    sc.solmtd = 2; sc.u2e8 = 4;
    sc.solmtd_p[0] = 1.9; sc.solmtd_p[1] = 2.9;
    sc.solmtd_p[2] = -3.9; sc.solmtd_p[3] = 5.9;
    sc.tmax = 1.2345678; sc.dsmax = 2.3456789;
    sc.dttype.u58 = 0; sc.stpdef = 1; // Explicit displacement-controlled test profile.
    sc.tnow1 = 3.4567891; sc.tnow2 = 4.5678912;
    sc.htmthd = 3; sc.envmpr = 5.6789123; sc.envmpt = 6.7891234;
    sc.envtmp.value = 7.8912345; sc.vfactr.value = 8.9123456;
}
constexpr std::size_t index(Page page) { return static_cast<std::size_t>(page); }
void timeIdTable(def::model::DYNARY& table, std::initializer_list<double> ids,
    int components, std::initializer_list<double> values)
{
    def::model::dynary_free_arrays(&table);
    table.ndim = 1; table.n[0] = static_cast<int>(ids.size());
    table.n[1] = table.n[2] = 0; table.ncomp = components;
    REQUIRE(values.size() == ids.size() * components);
    def::model::dynary_alloc(&table);
    REQUIRE(table.axis[0]); REQUIRE(table.values);
    std::copy(ids.begin(), ids.end(), table.axis[0]);
    std::copy(values.begin(), values.end(), table.values);
}
}

TEST_CASE(initial_editor_dimension_preflight_preserves_admission_and_locale_order)
{
    auto checkRefusal = [](int dimension, bool absentStep, InitialOpenIssue expected) {
        engine::GuiModel model(nullptr, 3);
        if (absentStep) model.step.reset();
        else model.step->dimension = dimension;
        if (absentStep) REQUIRE(!model.step);
        RemeshState remesh; Io io; InitialEditorSession session; InitialOpenReport report;
        auto invalidLocale = context(); invalidLocale.numericLocale = NumericLocale::Unrecovered;
        openInitialEditorInto(session, model, remesh, io, invalidLocale, report);
        CHECK(report.stage == InitialOpenStage::Preflight);
        CHECK(report.issue == expected);
        CHECK(session.constructionStarted); CHECK(!session.pageStateComplete);
        CHECK(!report.reachedPage);
        for (bool reached : report.initialPages) CHECK(!reached);
        for (bool reached : report.repeatPages) CHECK(!reached);
        for (bool reached : report.refreshedPages) CHECK(!reached);
        CHECK(!session.sourceModel); CHECK(!session.sourceControl); CHECK(!session.sourceRemesh);
        if (absentStep) CHECK(!model.step);
        else CHECK(model.step && model.dimension() == dimension);
        CHECK(io.readCalls == 0); CHECK(io.effects.empty());
    };
    checkRefusal(2, false, InitialOpenIssue::InvalidModel);
    checkRefusal(0, false, InitialOpenIssue::InvalidModel);
    checkRefusal(0, true, InitialOpenIssue::InvalidModel);

    engine::GuiModel validModel(nullptr, 3); RemeshState validRemesh; Io validIo;
    InitialEditorSession validSession; InitialOpenReport validReport;
    auto invalidLocale = context(); invalidLocale.numericLocale = NumericLocale::Unrecovered;
    openInitialEditorInto(validSession, validModel, validRemesh, validIo, invalidLocale, validReport);
    CHECK(validReport.stage == InitialOpenStage::Preflight);
    CHECK(validReport.issue == InitialOpenIssue::UnrecoveredNumericLocale);
    CHECK(!validReport.reachedPage); CHECK(!validSession.sourceModel);
    CHECK(validIo.readCalls == 0); CHECK(validIo.effects.empty());

    engine::GuiModel retryModel(nullptr, 3); retryModel.step->dimension = 2;
    RemeshState retryRemesh; Io retryIo; InitialEditorSession retrySession;
    InitialOpenReport retryReport; openInitialEditorInto(retrySession, retryModel,
        retryRemesh, retryIo, context(), retryReport);
    retryModel.step->dimension = 3;
    openInitialEditorInto(retrySession, retryModel, retryRemesh, retryIo, context(), retryReport);
    CHECK(retryReport.stage == InitialOpenStage::Preflight);
    CHECK(retryReport.issue == InitialOpenIssue::AlreadyStarted);
    CHECK(!retryReport.reachedPage); CHECK(!retrySession.sourceModel);
    CHECK(retryIo.readCalls == 0); CHECK(retryIo.effects.empty());
}

TEST_CASE(initial_editor_session_remembers_open_state_through_save_and_real_deep_temporaries)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); initialize(sc);
    RemeshState remesh; Io io;
    auto session = openInitialEditor(model, remesh, io, context()); REQUIRE(session);
    CHECK(!InitialEditorSession::completeForPublicSave);
    CHECK(sc.solmtd_p[0] == 1.9 && sc.solmtd_p[1] == 2.9); // Open is not Save's integer conversion.
    CHECK_EQ(sc.meshno, 30000); CHECK(sc.tmax == 1.2345678);
    REQUIRE(session->viewFactorCurve); CHECK(session->viewFactorCurve.get() != &sc.vfactr);
    CHECK(session->viewFactorCurve->value == 8.9123456);
    REQUIRE(session->pages.environment.curves[0]);
    CHECK(session->pages.environment.curves[0].get() != &sc.envtmp);
    CHECK_EQ(session->pages.general.heatMethodCombo, 2); // Final General refill then Environment.
    std::strcpy(sc.title, "LATEST TITLE"); sc.meshno = 42; sc.nstep = 99;
    sc.tmax = sc.tnow1 = sc.envmpr = sc.envtmp.value = 99.0;
    sc.vfactr.value = 99.0;
    for (double& value : sc.solmtd_p) value = 99.0;
    std::vector<RecoveryEntry> empty;
    const auto report = applyInitialEditorPageState(*session, model, remesh, io, &empty, &empty);
    CHECK(report.outcome == ApplyResult::Completed);
    for (const auto& page : report.pages) { REQUIRE(page); CHECK(*page == ApplyResult::Completed); }
    REQUIRE(report.recoveryTail); CHECK(*report.recoveryTail == ApplyResult::Completed);
    CHECK(std::strcmp(sc.title, "OPEN TITLE") == 0); CHECK_EQ(sc.meshno, 9999);
    CHECK_EQ(sc.nstart, -8); CHECK_EQ(sc.nstep, 23);
    CHECK(sc.solmtd_p[0] == 1.0 && sc.solmtd_p[1] == 2.0 &&
        sc.solmtd_p[2] == -3.0 && sc.solmtd_p[3] == 5.0);
    CHECK(sc.tmax == 1.23457); CHECK(sc.tnow1 == 3.45679); CHECK(sc.envmpr == 5.67891);
    CHECK(sc.envtmp.value == 7.89123); CHECK(sc.vfactr.value == 99.0); CHECK_EQ(sc.htmthd, 3);
    CHECK(io.nbcBytes == "75\t\t\t105\t\r\n");
}

TEST_CASE(initial_editor_name_limit_survives_backed_refresh_without_mutating_open_model)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); initialize(sc);
    const std::string original = "ABCDEFGHIJKLMNOPQRST" + std::string(44, 'X');
    REQUIRE(original.size() == 64);
    std::strcpy(sc.oprnam, original.c_str());
    RemeshState remesh; Io io;
    InitialEditorSession session;
    InitialOpenReport opened;
    openInitialEditorInto(session, model, remesh, io, context(), opened);
    REQUIRE(opened.complete());
    CHECK(opened.backupCreated);
    CHECK(opened.backupRestored);
    CHECK(opened.refreshedPages[0]); // General is the first backed-up refresh.
    CHECK(session.pages.general.operationName == qt::L("ABCDEFGHIJKLMNOPQRST"));
    CHECK(std::string(sc.oprnam) == original);
    std::strcpy(sc.oprnam, "LATER LIVE MODEL");
    const std::vector<RecoveryEntry> empty;
    const auto saved = applyInitialEditorPageState(session, model, remesh, io, &empty, &empty);
    REQUIRE(saved.outcome == ApplyResult::Completed);
    REQUIRE(saved.pages[index(Page::General)]);
    CHECK(*saved.pages[index(Page::General)] == ApplyResult::Completed);
    CHECK(std::string(sc.oprnam) == "ABCDEFGHIJKLMNOPQRST");
    CHECK(!InitialEditorSession::completeForPublicSave);
}

TEST_CASE(initial_editor_context_binds_mode_zero_profile_into_its_owned_session)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); initialize(sc);
    auto* object = model.objectWrap(1, true, false);
    field<int>(object, 0x1ec8) = 0; field<int>(object, 0x1ee8) = 73;
    field<int>(object, 0x87c) = 0;
    field<double>(object, 0x1ed0) = 1.2345678;
    field<double>(object, 0x1ed8) = 0.0; field<double>(object, 0x1ee0) = 0.0;
    sc.mdsobj[0] = 1; sc.mdsobj[1] = 0;
    RemeshState remesh; Io io; auto explicitContext = context();
    explicitContext.stoppingLabelProfile = StoppingLabelProfile::AsciiNoActiveTranslator;
    auto session = openInitialEditor(model, remesh, io, explicitContext); REQUIRE(session);
    CHECK(session->pages.stopping.widgets.labelProfile == StoppingLabelProfile::AsciiNoActiveTranslator);
    const auto& reference = session->pages.stopping.widgets.references[0];
    CHECK(reference.nodeChecked); CHECK(!reference.nodeEnabled); CHECK(reference.nodeSpinEnabled);
    CHECK_EQ(reference.node, 0); CHECK_EQ(field<int>(object, 0x1ec8), 0);
    CHECK_EQ(field<int>(object, 0x1ee8), 73);
    const std::vector<RecoveryEntry> empty;
    CHECK(applyInitialEditorPageState(*session, model, remesh, io, &empty, &empty).outcome ==
        ApplyResult::Completed);
    CHECK_EQ(field<int>(object, 0x1ec8), 2); CHECK_EQ(field<int>(object, 0x1ee8), 0);
    CHECK(field<double>(object, 0x1ed0) == 1.235);
    CHECK(!InitialEditorSession::completeForPublicSave);
}

TEST_CASE(initial_editor_mode_two_repeats_bounded_spin_and_saves_remembered_value)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); initialize(sc);
    auto* object = model.objectWrap(1, true, false);
    field<int>(object, 0x1ec8) = 2; field<int>(object, 0x1ee8) = 1000000;
    field<int>(object, 0x87c) = 0; field<int>(object, 0x880) = 0;
    field<double>(object, 0x1ed0) = 1.2345678;
    field<double>(object, 0x1ed8) = 0.0; field<double>(object, 0x1ee0) = 0.0;
    sc.mdsobj[0] = 0; sc.mdsobj[1] = 1;
    RemeshState remesh; Io io; auto explicitContext = context();
    explicitContext.stoppingLabelProfile = StoppingLabelProfile::AsciiNoActiveTranslator;
    io.duringInitialRead = [&] {
        CHECK_EQ(field<int>(object, 0x1ee8), 1000000);
        CHECK(field<double*>(object, 0x8a8) == nullptr);
        field<int>(object, 0x1ee8) = 999999;
        field<double>(object, 0x1ed0) = 7.8912345;
    };
    InitialEditorSession session; InitialOpenReport opened;
    openInitialEditorInto(session, model, remesh, io, explicitContext, opened);
    REQUIRE(opened.complete()); CHECK(opened.repeatPages[0]); CHECK(opened.backupRestored);
    const auto& stopping = session.pages.stopping;
    CHECK_EQ(stopping.referenceFillOrder[0], 1); CHECK_EQ(stopping.referenceFillOrder[1], 1);
    CHECK_EQ(stopping.widgets.references[1].node, 999999);
    CHECK(stopping.widgets.references[1].nodeChecked);
    CHECK(stopping.widgets.references[1].coordinates[0] == qt::L("7.891"));
    CHECK(field<double*>(object, 0x8a8) == nullptr);
    field<int>(object, 0x1ee8) = 12; field<double>(object, 0x1ed0) = 99.0;
    const std::vector<RecoveryEntry> empty;
    const auto saved = applyInitialEditorPageState(session, model, remesh, io, &empty, &empty);
    CHECK(saved.outcome == ApplyResult::Completed);
    CHECK_EQ(field<int>(object, 0x1ec8), 2); CHECK_EQ(field<int>(object, 0x1ee8), 999999);
    CHECK(field<double>(object, 0x1ed0) == 7.891);
    CHECK(field<double*>(object, 0x8a8) == nullptr);
    CHECK(!InitialEditorSession::completeForPublicSave);
}

TEST_CASE(initial_editor_coordinate_selection_requires_explicit_label_profile)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); initialize(sc);
    auto* object = model.objectWrap(1, true, false);
    field<int>(object, 0x1ec8) = 1; field<double>(object, 0x1ed0) = 1.2345678;
    field<double>(object, 0x1ed8) = 0.0; field<double>(object, 0x1ee0) = 0.0;
    sc.mdsobj[0] = 1; sc.mdsobj[1] = 0;
    RemeshState remesh; Io io; InitialEditorSession session; InitialOpenReport opened;
    openInitialEditorInto(session, model, remesh, io, context(), opened);
    CHECK(!opened.complete()); CHECK(opened.issue == InitialOpenIssue::UnsupportedPage);
    REQUIRE(opened.reachedPage); CHECK(*opened.reachedPage == Page::Stopping);
    CHECK(session.pages.stopping.widgets.labelProfile == StoppingLabelProfile::Unknown);
    CHECK(field<double>(object, 0x1ed0) == 1.2345678);
}

TEST_CASE(initial_editor_retains_stopping_allocation_and_current_page_on_sync_refusal)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); initialize(sc);
    auto* object = model.objectWrap(1, true, false);
    field<int>(object, 0x1ec8) = 2; field<int>(object, 0x1ee8) = 1;
    field<int>(object, 0x880) = 1; field<int>(object, 0x87c) = 1;
    auto& nodes = field<def::model::Cls_14017ef20>(object, 0x8a0);
    nodes.n = 1; model.step->u15f8[0] = 3;
    model.step->u1b58 = 0; model.step->step_version = 6000;
    sc.mdsobj[0] = 1; sc.mdsobj[1] = 0;
    RemeshState remesh; Io io; InitialEditorSession session; InitialOpenReport opened;
    session.pages.stopping.mdsThreshold.text = qt::L("unreached threshold");
    auto explicitContext = context();
    explicitContext.stoppingLabelProfile = StoppingLabelProfile::AsciiNoActiveTranslator;
    openInitialEditorInto(session, model, remesh, io, explicitContext, opened);
    CHECK(!opened.complete()); CHECK(opened.stage == InitialOpenStage::InitialFill);
    CHECK(opened.issue == InitialOpenIssue::UnsupportedPage); REQUIRE(opened.stoppingFill);
    CHECK(opened.stoppingFill->issue == StoppingFillIssue::CoordinateSync);
    CHECK_EQ(opened.stoppingFill->coordinateAllocationAttempts, 1u); REQUIRE(nodes.u008);
    CHECK(opened.initialPages[0]); CHECK(opened.initialPages[1]); CHECK(opened.initialPages[2]);
    CHECK(!opened.initialPages[3]); CHECK(!opened.temporariesCopied); CHECK(io.effects.empty());
    CHECK(session.pages.stopping.timeMaximum.text == qt::L("1.23457"));
    CHECK_EQ(session.pages.stopping.widgets.references[0].node, 1);
    CHECK(session.pages.stopping.mdsThreshold.text == qt::L("unreached threshold"));
    const std::vector<RecoveryEntry> empty;
    CHECK(applyInitialEditorPageState(session, model, remesh, io, &empty, &empty).outcome ==
        ApplyResult::UnsupportedSnapshot);
}

TEST_CASE(initial_editor_node_geometry_is_owned_across_repeated_fills_and_save)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); initialize(sc);
    auto* object = model.objectWrap(1, true, false);
    field<int>(object, 0x1ec8) = 2; field<int>(object, 0x1ee8) = 1;
    field<int>(object, 0x880) = 1; field<int>(object, 0x87c) = 1;
    field<double>(object, 0x1ed0) = 1.2345678;
    field<double>(object, 0x1ed8) = 2.3456789; field<double>(object, 0x1ee0) = 3.4567891;
    auto& nodes = field<def::model::Cls_14017ef20>(object, 0x8a0);
    nodes.n = 1; model.step->u15f8[0] = 3;
    sc.mdsobj[0] = 1; sc.mdsobj[1] = 0;
    RemeshState remesh; Io io; InitialEditorSession session; InitialOpenReport opened;
    auto explicitContext = context();
    explicitContext.stoppingLabelProfile = StoppingLabelProfile::AsciiNoActiveTranslator;
    openInitialEditorInto(session, model, remesh, io, explicitContext, opened);
    REQUIRE(opened.complete()); REQUIRE(nodes.u008); REQUIRE(opened.stoppingFill);
    CHECK_EQ(opened.stoppingFill->coordinateGetterCalls, 0u); // final repeated fill is same-value.
    CHECK(nodes.u008[0] == 0.0); CHECK(nodes.u130 == model.step.get());
    CHECK(session.pages.stopping.widgets.references[0].coordinates[0] == qt::L("1.235"));
    CHECK(field<double>(object, 0x1ed0) == 1.2345678);
    // Advanced Apply140ec7500 ->14082a360 ->14086d7f0 ->140854370
    // reallocates node arrays, even at an unchanged width. Preserve the owner
    // and existing coordinate bytes, not an allocator-dependent address.
    nodes.u008[0] = 17.25; nodes.u008[1] = -3.5; nodes.u008[2] = 9.75;
    bool sawFirstRemap = false;
    io.duringFirstEffect = [&] {
        sawFirstRemap = true; // Extra3D follows Advanced, before recovery tail.
        CHECK(model.object(1) == object); CHECK(nodes.u130 == model.step.get());
        CHECK_EQ(nodes.n, 1); CHECK_EQ(model.step->u15f8[0], 3);
        REQUIRE(nodes.u008);
        CHECK(nodes.u008[0] == 17.25); CHECK(nodes.u008[1] == -3.5);
        CHECK(nodes.u008[2] == 9.75);
    };
    field<double>(object, 0x1ed0) = 90.0; field<int>(object, 0x1ee8) = 99;
    const std::vector<RecoveryEntry> empty;
    const auto applied = applyInitialEditorPageState(session, model, remesh, io, &empty, &empty);
    CHECK(applied.outcome == ApplyResult::Completed); CHECK(sawFirstRemap);
    REQUIRE(applied.recoveryTail); CHECK(*applied.recoveryTail == ApplyResult::Completed);
    CHECK_EQ(field<int>(object, 0x1ee8), 1); CHECK(field<double>(object, 0x1ed0) == 1.235);
    // Recovery140ed5dd0 repeats Advanced and its array remap.
    CHECK(model.object(1) == object); CHECK(nodes.u130 == model.step.get());
    CHECK_EQ(nodes.n, 1); CHECK_EQ(model.step->u15f8[0], 3); REQUIRE(nodes.u008);
    CHECK(nodes.u008[0] == 17.25); CHECK(nodes.u008[1] == -3.5);
    CHECK(nodes.u008[2] == 9.75); CHECK(!InitialEditorSession::completeForPublicSave);
}

TEST_CASE(initial_editor_coordinate_references_use_repeat_fill_memory_at_save)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); initialize(sc);
    auto* object = model.objectWrap(1, true, false);
    field<int>(object, 0x1ec8) = 1; field<int>(object, 0x1ee8) = 73;
    field<int>(object, 0x87c) = 0;
    field<double>(object, 0x1ed0) = 1.2345678;
    field<double>(object, 0x1ed8) = 2.3456789;
    field<double>(object, 0x1ee0) = 3.4567891;
    sc.mdsobj[0] = 0; sc.mdsobj[1] = 1;
    RemeshState remesh; Io io;
    // Extra3D's initial read occurs after the first stopping fill, before the
    // constructor repeats stopping. The repeated fill must replace its text.
    io.duringInitialRead = [&] { field<double>(object, 0x1ed0) = 7.8912345; };
    InitialEditorSession session; InitialOpenReport opened;
    session.pages.stopping.widgets.labelProfile = StoppingLabelProfile::AsciiNoActiveTranslator;
    openInitialEditorInto(session, model, remesh, io, context(), opened);
    REQUIRE(opened.complete()); CHECK(opened.repeatPages[0]); CHECK(opened.backupRestored);
    const auto& stopping = session.pages.stopping;
    CHECK_EQ(stopping.referenceFillOrder[0], 1); CHECK_EQ(stopping.referenceFillOrder[1], 1);
    CHECK(stopping.widgets.references[1].coordinates[0] == qt::L("7.891"));
    CHECK_EQ(stopping.widgets.references[1].node, 0);
    CHECK(field<double>(object, 0x1ed0) == 7.8912345); CHECK_EQ(field<int>(object, 0x1ee8), 73);
    field<double>(object, 0x1ed0) = 99.0; sc.mdsobj[1] = 0;
    const std::vector<RecoveryEntry> empty;
    const auto applied = applyInitialEditorPageState(session, model, remesh, io, &empty, &empty);
    CHECK(applied.outcome == ApplyResult::Completed);
    CHECK_EQ(sc.mdsobj[0], 0); CHECK_EQ(sc.mdsobj[1], 1);
    CHECK(field<double>(object, 0x1ed0) == 7.891); CHECK_EQ(field<int>(object, 0x1ee8), 0);
    CHECK(!InitialEditorSession::completeForPublicSave);
    CHECK(!InitialApplyReport::completeForPublicSave);
}

TEST_CASE(initial_editor_coordinate_none_refill_retains_the_previous_widgets)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); initialize(sc);
    auto* object = model.objectWrap(1, true, false);
    field<int>(object, 0x1ec8) = 1; field<int>(object, 0x87c) = 0;
    field<double>(object, 0x1ed0) = 1.2345678;
    field<double>(object, 0x1ed8) = 0.0; field<double>(object, 0x1ee0) = 0.0;
    sc.mdsobj[0] = 1; sc.mdsobj[1] = 0;
    RemeshState remesh; Io io;
    io.duringInitialRead = [&] { sc.mdsobj[0] = 0; };
    InitialEditorSession session; InitialOpenReport opened;
    session.pages.stopping.widgets.labelProfile = StoppingLabelProfile::AsciiNoActiveTranslator;
    openInitialEditorInto(session, model, remesh, io, context(), opened); REQUIRE(opened.complete());
    const auto& reference = session.pages.stopping.widgets.references[0];
    CHECK_EQ(reference.selectedObject, 0);
    CHECK(reference.coordinateChecked); CHECK(!reference.nodeEnabled);
    CHECK(reference.coordinatesEnabled); CHECK(!reference.nodeSpinEnabled);
    CHECK(reference.coordinates[0] == qt::L("1.235"));
    const std::vector<RecoveryEntry> empty;
    CHECK(applyInitialEditorPageState(session, model, remesh, io, &empty, &empty).outcome ==
        ApplyResult::Completed);
    CHECK(field<double>(object, 0x1ed0) == 1.2345678); // None skips writes, retaining display memory.
}

TEST_CASE(initial_editor_timeid_final_refill_retains_checkbox_history_and_current_rows)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); initialize(sc);
    timeIdTable(sc.timeid, {1, 2}, 2, {0, 0, 11, 22});
    RemeshState remesh; Io io;
    io.duringInitialRead = [&] {
        timeIdTable(sc.timeid, {3, 4}, 1, {33.123456789, 44.123456789});
        sc.timeid.u54 = 37;
    };
    auto session = openInitialEditor(model, remesh, io, context()); REQUIRE(session);
    const auto& display = session->pages.environment.timeId;
    CHECK(display.differenceBoundaryChecked); CHECK(display.betweenTemperatureRows.empty());
    REQUIRE(display.belowTemperatureRows.size() == 1); REQUIRE(display.aboveTemperatureRows.size() == 1);
    CHECK(display.belowTemperatureRows[0] == qt::L("33.1235"));
    CHECK(display.aboveTemperatureRows[0] == qt::L("44.1235"));
    CHECK_EQ(sc.timeid.n[0], 2); CHECK_EQ(sc.timeid.ncomp, 1); CHECK_EQ(sc.timeid.u54, 37);
    REQUIRE(sc.timeid.axis[0]); REQUIRE(sc.timeid.values);
    CHECK(sc.timeid.axis[0][0] == 3.0); CHECK(sc.timeid.axis[0][1] == 4.0);
    CHECK(sc.timeid.values[0] == 33.123456789); CHECK(sc.timeid.values[1] == 44.123456789);
}

TEST_CASE(initial_editor_timeid_apply_preserves_later_live_model_over_display_snapshot)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); initialize(sc);
    timeIdTable(sc.timeid, {2}, 2, {11.123456789, 22.123456789});
    sc.vfactr_mode = 1; sc.vfactr_n = 7;
    RemeshState remesh; Io io;
    auto session = openInitialEditor(model, remesh, io, context()); REQUIRE(session);
    CHECK(!session->pages.environment.timeId.differenceBoundaryChecked);
    REQUIRE(session->pages.environment.timeId.betweenTemperatureRows.size() == 1);
    timeIdTable(sc.timeid, {1, 4}, 1, {77.123456789, 88.123456789});
    sc.timeid.u54 = 91; sc.timeid.var[0] = 99;
    const auto* axis = sc.timeid.axis[0]; const auto* values = sc.timeid.values;
    sc.envtmp.value = 99.0; sc.vfactr.value = 99.0; sc.vfactr_n = 99;
    std::vector<RecoveryEntry> empty;
    const auto report = applyInitialEditorPageState(*session, model, remesh, io, &empty, &empty);
    CHECK(report.outcome == ApplyResult::Completed);
    CHECK(sc.timeid.axis[0] == axis); CHECK(sc.timeid.values == values);
    CHECK_EQ(sc.timeid.n[0], 2); CHECK_EQ(sc.timeid.ncomp, 1); CHECK_EQ(sc.timeid.u54, 91);
    CHECK_EQ(sc.timeid.var[0], 99); CHECK(sc.timeid.axis[0][0] == 1.0); CHECK(sc.timeid.axis[0][1] == 4.0);
    CHECK(sc.timeid.values[0] == 77.123456789); CHECK(sc.timeid.values[1] == 88.123456789);
    CHECK(sc.envtmp.value == 7.89123); CHECK(sc.vfactr.value == 8.9123456); CHECK_EQ(sc.vfactr_n, 7);
}

TEST_CASE(initial_editor_environment_table_modes_follow_final_fill_and_remembered_owners)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); initialize(sc);
    timeIdTable(sc.cnvcof, {1, 2}, 1, {11, 22});
    RemeshState remesh; Io io;
    io.duringInitialRead = [&] {
        timeIdTable(sc.envtmp, {3, 4}, 2, {31.123456789, 32.123456789, 41.123456789, 42.123456789});
        sc.envtmp.value = 19.123456789; sc.envtmp.u54 = 37;
        def::model::dynary_free_arrays(&sc.cnvcof);
        sc.cnvcof.ndim = 0; sc.cnvcof.n[0] = 0; sc.cnvcof.value = 23.123456789;
        timeIdTable(sc.envatm, {5, 6}, 1, {51.123456789, 61.123456789});
        sc.envatm.value = 29.123456789;
    };
    auto session = openInitialEditor(model, remesh, io, context()); REQUIRE(session);
    auto& form = session->pages.environment;
    CHECK(form.curveControls[0].tableChecked); CHECK(form.curveControls[1].constantChecked);
    CHECK(form.curveControls[2].tableChecked);
    REQUIRE(form.curves[0]); REQUIRE(form.curves[0]->values);
    CHECK(form.curves[0]->values != sc.envtmp.values); CHECK(form.curves[0]->value == 19.123456789);
    CHECK(form.curves[0]->values[0] == 31.123456789); CHECK_EQ(form.curves[0]->u54, 37);
    sc.envtmp.value = sc.cnvcof.value = sc.envatm.value = 99.0;
    sc.envtmp.values[0] = sc.envatm.values[0] = 99.0; sc.envtmp.u54 = 99;
    std::vector<RecoveryEntry> empty;
    const auto report = applyInitialEditorPageState(*session, model, remesh, io, &empty, &empty);
    CHECK(report.outcome == ApplyResult::Completed);
    CHECK_EQ(sc.envtmp.ndim, 1); CHECK_EQ(sc.envtmp.ncomp, 2); CHECK_EQ(sc.envtmp.u54, 37);
    CHECK(sc.envtmp.value == 19.123456789); CHECK(sc.envtmp.values[0] == 31.123456789);
    CHECK(sc.envtmp.values[3] == 42.123456789); CHECK(sc.envtmp.axis[0][1] == 4.0);
    CHECK_EQ(sc.cnvcof.ndim, 0); CHECK(sc.cnvcof.value == 23.1235);
    CHECK_EQ(sc.envatm.ndim, 1); CHECK(sc.envatm.value == 29.123456789);
    CHECK(sc.envatm.values[0] == 51.123456789); CHECK(sc.envatm.values[1] == 61.123456789);
    CHECK(sc.envtmp.values != form.curves[0]->values);
    CHECK(sc.envatm.values != form.curves[2]->values);
}

TEST_CASE(initial_editor_acvcof_collapse_retains_count_history_and_owner_links)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); initialize(sc);
    timeIdTable(sc.acvcof, {10, 20}, 1, {11.123456789, 22.123456789});
    sc.acvcof.var[0] = 99; sc.acvcof.value = 7.123456789;
    auto* originalAxis = sc.acvcof.axis[0]; auto* originalValues = sc.acvcof.values;
    RemeshState remesh; Io io;
    io.duringInitialRead = [&] {
        CHECK_EQ(sc.acvcof.ndim, 0); CHECK_EQ(sc.acvcof.var[0], 0);
        CHECK_EQ(sc.acvcof.n[0], 2); CHECK(sc.acvcof.axis[0] == originalAxis);
        CHECK(sc.acvcof.values == originalValues); // callback did not free arrays
    };
    auto session = openInitialEditor(model, remesh, io, context()); REQUIRE(session);
    CHECK_EQ(session->pages.environment.acvcofControls.selection, 0);
    CHECK_EQ(sc.acvcof.ndim, 0); CHECK_EQ(sc.acvcof.var[0], 0); CHECK_EQ(sc.acvcof.n[0], 2);
    CHECK(sc.acvcof.axis[0] == nullptr); CHECK(sc.acvcof.values == nullptr); // backup copy shape
    REQUIRE(session->environmentCurves.currentCurveIndex);
    CHECK_EQ(*session->environmentCurves.currentCurveIndex, 3);
    for (int i = 0; i < 4; ++i)
        CHECK(session->pages.environment.curves[i].get() == session->environmentCurves.curves[i].get());
    const auto& temporary = *session->environmentCurves.curves[3];
    CHECK_EQ(temporary.n[0], 2); CHECK_EQ(temporary.ndim, 0); CHECK(temporary.axis[0] == nullptr);
    CHECK(temporary.values == nullptr); CHECK(temporary.value == 7.123456789);
    sc.acvcof.value = 99;
    std::vector<RecoveryEntry> empty;
    const auto report = applyInitialEditorPageState(*session, model, remesh, io, &empty, &empty);
    CHECK(report.outcome == ApplyResult::Completed);
    CHECK(sc.acvcof.value == 7.12346); CHECK_EQ(sc.acvcof.n[0], 2);
    CHECK(temporary.value == 7.12346); // Save writes the actual remembered owner.
    CHECK(sc.acvcof.values == nullptr); CHECK(sc.acvcof.axis[0] == nullptr);
}


TEST_CASE(initial_editor_acvcof_collapse_copies_component_prefix_and_empty_zeros)
{
    // Expected values derive from original destination-count copy, including
    // all five special ndim0 value types. These are not original-app captures.
    for (int type : {0, 0xb5, 0xa7, 0xa8, 0xaa, 0xab})
        for (int components : {1, 3}) for (int shape = 0; shape < 3; ++shape) {
            engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); initialize(sc);
            auto& curve = sc.acvcof;
            curve.type = type; curve.ndim = shape + 1; curve.ncomp = components;
            curve.n[0] = 2; curve.n[1] = shape == 2 ? 1 : 0; curve.n[2] = shape == 2 ? 1 : 0;
            curve.var[0] = 99; curve.var[1] = 7; curve.var[2] = 9;
            curve.value = 7.123456789; curve.u54 = 37;
            def::model::dynary_alloc(&curve);
            for (int d = 0; d < curve.ndim; ++d)
                for (int row = 0; row < curve.n[d]; ++row) curve.axis[d][row] = row + 10.0;
            const bool emptyTable = shape == 1;
            std::vector<std::uint64_t> prefix(static_cast<std::size_t>(components), 0);
            if (!emptyTable) {
                REQUIRE(curve.values);
                for (int i = 0; i < 2 * components; ++i) curve.values[i] = 11.123456789 + i;
                curve.values[0] = std::bit_cast<double>(std::uint64_t{0x7ff8000000000042ULL});
                for (int i = 0; i < components; ++i)
                    prefix[i] = std::bit_cast<std::uint64_t>(curve.values[i]);
            } else CHECK(curve.values == nullptr);
            RemeshState remesh; Io io;
            auto session = openInitialEditor(model, remesh, io, context()); REQUIRE(session);
            auto& owner = *session->environmentCurves.curves[3];
            const bool hasValues = components > 1 || type != 0;
            for (const auto* result : {&curve, &owner}) {
                CHECK_EQ(result->type, type); CHECK_EQ(result->ndim, 0); CHECK_EQ(result->ncomp, components);
                CHECK_EQ(result->n[0], 2); CHECK_EQ(result->n[1], shape == 2 ? 1 : 0);
                CHECK_EQ(result->n[2], shape == 2 ? 1 : 0);
                CHECK_EQ(result->var[0], 0); CHECK_EQ(result->var[1], 7); CHECK_EQ(result->var[2], 9);
                CHECK_EQ(result->u54, 37); CHECK(result->value == 7.123456789);
                for (const auto* axis : result->axis) CHECK(axis == nullptr);
                if (hasValues) {
                    REQUIRE(result->values);
                    for (int i = 0; i < components; ++i)
                        CHECK(std::bit_cast<std::uint64_t>(result->values[i]) == prefix[i]);
                } else CHECK(result->values == nullptr);
            }
            auto* ownerValues = owner.values;
            curve.value = 99;
            std::vector<RecoveryEntry> empty;
            const auto report = applyInitialEditorPageState(*session, model, remesh, io, &empty, &empty);
            CHECK(report.outcome == ApplyResult::Completed);
            CHECK(curve.value == 7.12346); CHECK(owner.value == 7.12346);
            CHECK(owner.values == ownerValues); CHECK_EQ(curve.n[0], 2);
            if (hasValues) {
                REQUIRE(curve.values); CHECK(curve.values != ownerValues);
                for (int i = 0; i < components; ++i)
                    CHECK(std::bit_cast<std::uint64_t>(curve.values[i]) == prefix[i]);
            } else CHECK(curve.values == nullptr);
        }
}

TEST_CASE(initial_editor_final_collapse_save_mutates_owner_before_later_page_effects)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); initialize(sc);
    timeIdTable(sc.acvcof, {10, 20}, 2, {11.125, 22.25, 33.5, 44.75}); sc.acvcof.var[0] = 3;
    sc.acvcof.value = 9.123456789;
    RemeshState remesh; Io io;
    io.duringInitialRead = [&] { sc.acvcof.var[0] = 99; };
    auto session = openInitialEditor(model, remesh, io, context()); REQUIRE(session);
    auto& owner = *session->environmentCurves.curves[3];
    CHECK_EQ(session->pages.environment.acvcofControls.selection, 0);
    CHECK_EQ(sc.acvcof.ndim, 1); CHECK_EQ(sc.acvcof.var[0], 99); // final backup restoration
    CHECK_EQ(owner.ndim, 1); CHECK_EQ(owner.var[0], 0); // callback only wrote owner var
    auto* oldAxis = owner.axis[0]; auto* oldValues = owner.values;
    REQUIRE(oldAxis); REQUIRE(oldValues);
    session->pages.environment.constantValues[3] = {qt::L("invalid"), ValidatorState::Invalid};
    session->pages.environment.scalars[0] = {qt::L("invalid"), ValidatorState::Invalid};
    sc.acvcof.value = 99; sc.envmpr = 88;
    bool observed = false;
    io.duringFirstEffect = [&] {
        observed = true;
        CHECK_EQ(owner.ndim, 0); CHECK(owner.value == 9.123456789);
        CHECK(owner.axis[0] == oldAxis); CHECK(owner.values == oldValues);
        CHECK(owner.values[3] == 44.75); CHECK_EQ(owner.n[0], 2);
        CHECK_EQ(sc.acvcof.ndim, 0); CHECK_EQ(sc.acvcof.var[0], 0);
        CHECK(sc.acvcof.axis[0] == nullptr); REQUIRE(sc.acvcof.values);
        CHECK(sc.acvcof.values[0] == 11.125); CHECK(sc.acvcof.values[1] == 22.25);
        CHECK(sc.acvcof.value == 9.123456789); CHECK(sc.envmpr == 88);
    };
    std::vector<RecoveryEntry> empty;
    const auto report = applyInitialEditorPageState(*session, model, remesh, io, &empty, &empty);
    CHECK(observed); CHECK(report.outcome == ApplyResult::ValidationStopped);
    REQUIRE(report.pages[index(Page::Environment)]);
    CHECK(*report.pages[index(Page::Environment)] == ApplyResult::ValidationStopped);
    REQUIRE(report.pages[index(Page::Extra3D)]); CHECK(!io.effects.empty());
    CHECK(session->environmentCurves.curves[3].get() == &owner);
}

TEST_CASE(initial_editor_acvcof_final_refill_mutates_owner_before_model_restore)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); initialize(sc);
    timeIdTable(sc.acvcof, {10, 20}, 1, {11.123456789, 22.123456789}); sc.acvcof.var[0] = 3;
    RemeshState remesh; Io io;
    io.duringInitialRead = [&] {
        sc.envatm.ndim = 1; sc.acvcof.var[0] = 99;
        sc.acvcof.value = 19.123456789; sc.acvcof.values[0] = 31.123456789;
    };
    auto session = openInitialEditor(model, remesh, io, context()); REQUIRE(session);
    CHECK_EQ(session->pages.environment.acvcofControls.selection, 1);
    CHECK_EQ(sc.acvcof.var[0], 99); // restored backup, unlike the owner below
    const auto* owner = session->environmentCurves.curves[3].get(); REQUIRE(owner);
    CHECK(session->pages.environment.curves[3].get() == owner);
    CHECK_EQ(owner->var[0], 3); CHECK_EQ(owner->ndim, 1);
    CHECK(owner->value == 19.123456789); REQUIRE(owner->values);
    CHECK(owner->values[0] == 31.123456789); CHECK(owner->values != sc.acvcof.values);
    sc.acvcof.value = 99; sc.acvcof.values[0] = 99;
    std::vector<RecoveryEntry> empty;
    const auto report = applyInitialEditorPageState(*session, model, remesh, io, &empty, &empty);
    CHECK(report.outcome == ApplyResult::Completed);
    CHECK_EQ(sc.acvcof.var[0], 3); CHECK(sc.acvcof.value == 19.123456789);
    CHECK(sc.acvcof.values[0] == 31.123456789);
    CHECK(session->environmentCurves.curves[3].get() == owner);
}

TEST_CASE(initial_editor_replaced_environment_owner_refuses_before_any_effect)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); initialize(sc);
    RemeshState remesh; Io io;
    auto session = openInitialEditor(model, remesh, io, context()); REQUIRE(session);
    sc.envtmp.value = 99; sc.meshno = 42;
    session->environmentCurves.curves[0] = std::make_shared<def::model::DYNARY>();
    std::vector<RecoveryEntry> empty;
    const auto report = applyInitialEditorPageState(*session, model, remesh, io, &empty, &empty);
    CHECK(report.outcome == ApplyResult::UnsupportedSnapshot);
    for (const auto& page : report.pages) CHECK(!page);
    CHECK(io.effects.empty()); CHECK(sc.envtmp.value == 99); CHECK_EQ(sc.meshno, 42);
}

TEST_CASE(initial_editor_checked_viewfactor_uses_same_remembered_temporary_and_definition)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); initialize(sc);
    sc.vfactr_mode = -3; sc.vfactr_n = 7;
    sc.vfactr.ndim = 1; sc.vfactr.n[0] = 2; sc.vfactr.ncomp = 1;
    sc.vfactr.var[0] = 1; sc.vfactr.u54 = 37;
    def::model::dynary_alloc(&sc.vfactr);
    REQUIRE(sc.vfactr.axis[0]); REQUIRE(sc.vfactr.values);
    sc.vfactr.axis[0][0] = 0.0; sc.vfactr.axis[0][1] = 2.0;
    sc.vfactr.values[0] = 0.123456789; sc.vfactr.values[1] = 0.987654321;
    RemeshState remesh; Io io;
    auto session = openInitialEditor(model, remesh, io, context()); REQUIRE(session);
    REQUIRE(session->viewFactorCurve); REQUIRE(session->viewFactorCurve->values);
    CHECK(session->pages.environment.viewFactorChecked);
    CHECK(session->pages.environment.viewFactorDefinitionEnabled);
    CHECK_EQ(session->viewFactorDefinition, 7); CHECK_EQ(sc.vfactr_mode, -3);
    CHECK(session->viewFactorCurve->values != sc.vfactr.values);
    sc.vfactr_mode = 0; sc.vfactr_n = 99; sc.vfactr.value = 99.0;
    sc.vfactr.values[0] = 99.0; sc.vfactr.axis[0][1] = 99.0;
    std::vector<RecoveryEntry> empty;
    const auto report = applyInitialEditorPageState(*session, model, remesh, io, &empty, &empty);
    CHECK(report.outcome == ApplyResult::Completed);
    CHECK_EQ(sc.vfactr_mode, 1); CHECK_EQ(sc.vfactr_n, 7); CHECK_EQ(sc.vfactr.u54, 37);
    CHECK_EQ(sc.vfactr.ndim, 1); CHECK(sc.vfactr.value == 8.9123456);
    REQUIRE(sc.vfactr.values); REQUIRE(sc.vfactr.axis[0]);
    CHECK(sc.vfactr.values != session->viewFactorCurve->values);
    CHECK(sc.vfactr.values[0] == 0.123456789); CHECK(sc.vfactr.values[1] == 0.987654321);
    CHECK(sc.vfactr.axis[0][1] == 2.0);
}

TEST_CASE(initial_editor_viewfactor_final_refill_controls_copy_and_unchecked_preserves_live_state)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); initialize(sc);
    sc.vfactr_mode = 1; sc.vfactr_n = 7;
    RemeshState remesh; Io io;
    io.duringInitialRead = [&] {
        sc.vfactr_mode = 0; sc.vfactr_n = 19; sc.vfactr.value = 17.123456789;
    };
    auto session = openInitialEditor(model, remesh, io, context()); REQUIRE(session);
    CHECK(!session->pages.environment.viewFactorChecked);
    CHECK(!session->pages.environment.viewFactorDefinitionEnabled);
    REQUIRE(session->viewFactorCurve); CHECK(session->viewFactorCurve->value == 17.123456789);
    CHECK_EQ(session->viewFactorDefinition, 19);
    sc.vfactr_mode = 9; sc.vfactr_n = 88; sc.vfactr.value = 77.0;
    std::vector<RecoveryEntry> empty;
    const auto report = applyInitialEditorPageState(*session, model, remesh, io, &empty, &empty);
    CHECK(report.outcome == ApplyResult::Completed); CHECK_EQ(sc.vfactr_mode, 0);
    CHECK_EQ(sc.vfactr_n, 88); CHECK(sc.vfactr.value == 77.0);
}

TEST_CASE(initial_editor_active_furnace_survives_backup_refresh_and_apply_retains_later_live_state)
{
    for (unsigned flag : {1u, 255u}) {
        engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); initialize(sc);
        sc.furnace_on = static_cast<std::uint8_t>(flag);
        std::strcpy(sc.furnace.name, "INITIAL FURNACE"); sc.furnace.u58[5] = 0x7b;
        const auto count = sc.furnace.curve_count(); REQUIRE(count >= 1);
        const std::int32_t firstIndex = 1;
        auto* first = sc.furnace.curve(&firstIndex); REQUIRE(first);
        first->value = 12.3456789; first->u54 = 37;
        RemeshState remesh; Io io;
        auto session = openInitialEditor(model, remesh, io, context()); REQUIRE(session);
        // The field-aware backup recreates owned curves; reacquire their owner.
        first = sc.furnace.curve(&firstIndex); REQUIRE(first);
        CHECK_EQ(sc.furnace.curve_count(), count); CHECK_EQ(sc.furnace_on, flag);
        CHECK(std::strcmp(sc.furnace.name, "INITIAL FURNACE") == 0);
        CHECK(first->value == 12.3456789); CHECK_EQ(first->u54, 37);
        CHECK_EQ(sc.furnace.u58[5], 0x7b); // Original copy omits these bytes.
        const auto liveFlag = static_cast<std::uint8_t>(flag == 255 ? 0 : 254);
        sc.furnace_on = liveFlag; std::strcpy(sc.furnace.name, "LIVE FURNACE");
        first->value = 77.123456789; first->u54 = 91; sc.furnace.u58[5] = 0x3a;
        sc.envtmp.value = 99.0;
        std::vector<RecoveryEntry> empty;
        const auto report = applyInitialEditorPageState(*session, model, remesh, io, &empty, &empty);
        CHECK(report.outcome == ApplyResult::Completed); CHECK(sc.envtmp.value == 7.89123);
        first = sc.furnace.curve(&firstIndex); REQUIRE(first);
        CHECK_EQ(sc.furnace.curve_count(), count); CHECK_EQ(sc.furnace_on, liveFlag);
        CHECK(std::strcmp(sc.furnace.name, "LIVE FURNACE") == 0);
        CHECK(first->value == 77.123456789); CHECK_EQ(first->u54, 91);
        CHECK_EQ(sc.furnace.u58[5], 0x3a);
    }
}

TEST_CASE(initial_editor_backup_initialization_preserves_both_loaded_unit_profiles)
{
    for (const int startup : {0, 1, 2}) for (const int unit : {1, 2}) {
        GuiStartupUnitScope startupUnit(startup);
        engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl();
        CHECK_EQ(sc.unit, startup); // Original GUI constructor's pre-owner seed.
        initialize(sc);
        sc.unit = unit;
        // Distinct loaded values must survive backup initialization rather than
        // being replaced by that unit's native environment defaults.
        sc.unte2h = 0.123456789; sc.blzman = 0.234567891;
        sc.cnvcof.value = 0.345678912; sc.acvcof.value = 0.456789123;
        RemeshState remesh; Io io;
        auto session = openInitialEditor(model, remesh, io, context()); REQUIRE(session);
        CHECK_EQ(sc.unit, unit); CHECK_EQ(session->pages.general.unit, unit);
        CHECK(sc.unte2h == 0.123456789); CHECK(sc.blzman == 0.234567891);
        CHECK(sc.cnvcof.value == 0.345678912); CHECK(sc.acvcof.value == 0.456789123);
        REQUIRE(session->pages.environment.curves[1]);
        REQUIRE(session->pages.environment.curves[3]);
        CHECK(session->pages.environment.curves[1]->value == 0.345678912);
        CHECK(session->pages.environment.curves[3]->value == 0.456789123);
        sc.unit = 3 - unit; sc.cnvcof.value = sc.acvcof.value = 99.0;
        std::vector<RecoveryEntry> empty;
        const auto report = applyInitialEditorPageState(*session, model, remesh, io, &empty, &empty);
        CHECK(report.outcome == ApplyResult::Completed); CHECK_EQ(sc.unit, unit);
        CHECK(sc.cnvcof.value == 0.345679); CHECK(sc.acvcof.value == 0.456789);
        CHECK(sc.unte2h == 0.123457); CHECK(sc.blzman == 0.234568);
        CHECK_EQ(guiStartupUnit(), startup); // Applying loaded UNIT is not a startup-global store.
    }
}

TEST_CASE(initial_editor_repeat_and_final_refills_use_their_actual_model_state)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); initialize(sc);
    auto* object = model.objectWrap(1, true, false); field<int>(object, 0x87c) = 1;
    field<double>(object, 0x2dc8) = 1.1111111;
    RemeshState remesh; Io io;
    // Instrument the last initial page's real I/O seam. Values seen by the
    // following repeat/refill stages must supersede the earlier page snapshot.
    io.duringInitialRead = [&] {
        sc.tmax = 22.2222222; field<double>(object, 0x2dc8) = -33.3333333;
        std::strcpy(sc.title, "LATE CONSTRUCTOR TITLE"); sc.nstep = 44; sc.dsmax = 55.5555555;
    };
    auto session = openInitialEditor(model, remesh, io, context()); REQUIRE(session);
    CHECK(session->pages.stopping.timeMaximum.text == qt::L("22.2222"));
    CHECK(session->pages.meshing.firstInterference == qt::L("33.3333"));
    CHECK(session->pages.meshing.firstInterferenceNegative);
    CHECK(session->pages.general.title == qt::L("LATE CONSTRUCTOR TITLE"));
    CHECK_EQ(session->pages.stepCount.steps, 44);
    CHECK(session->pages.stepSize.displacementMaximum.text == qt::L("55.5556"));
    CHECK(sc.tmax == 22.2222222); CHECK(sc.dsmax == 55.5555555);
    CHECK(field<double>(object, 0x2dc8) == -33.3333333); // Fill has not applied its rounded form.
    std::vector<RecoveryEntry> empty;
    CHECK(applyInitialEditorPageState(*session, model, remesh, io, &empty, &empty).outcome == ApplyResult::Completed);
    CHECK(sc.tmax == 22.2222); CHECK(sc.dsmax == 55.5556);
    CHECK(field<double>(object, 0x2dc8) == -33.3333);
    CHECK(session->pages.meshing.objects.empty()); // Original140eb52f0 releases the temporary array.
}

TEST_CASE(initial_editor_page_validation_stop_continues_later_pages_io_and_advanced_reapply)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); initialize(sc);
    RemeshState remesh; Io io;
    auto session = openInitialEditor(model, remesh, io, context(false)); REQUIRE(session);
    session->pages.stepSize.displacementMaximum = {qt::L("invalid"), ValidatorState::Invalid};
    session->pages.iteration.convergence[0] = {qt::L("invalid"), ValidatorState::Invalid};
    sc.dsmax = 81.0; sc.envmpr = 82.0; sc.stnout[4] = 0;
    bool observed = false;
    io.duringFirstEffect = [&] {
        observed = true;
        CHECK(sc.dsmax == 81.0); CHECK(sc.envmpr == 0.0); CHECK_EQ(sc.stnout[4], 1);
        CHECK(sc.tnow1 == 3.45679); // Advanced preceded Extra3D I/O.
        sc.tnow1 = 99.0; // The tail must really apply Advanced again.
    };
    std::vector<RecoveryEntry> empty;
    const auto report = applyInitialEditorPageState(*session, model, remesh, io, &empty, &empty);
    CHECK(observed); CHECK(report.outcome == ApplyResult::ValidationStopped);
    REQUIRE(report.pages[index(Page::StepSize)]);
    CHECK(*report.pages[index(Page::StepSize)] == ApplyResult::ValidationStopped);
    REQUIRE(report.pages[index(Page::Iteration)]);
    CHECK(*report.pages[index(Page::Iteration)] == ApplyResult::ValidationStopped);
    REQUIRE(report.pages[index(Page::Extra3D)]); REQUIRE(report.recoveryTail);
    CHECK(*report.recoveryTail == ApplyResult::Completed); CHECK(sc.tnow1 == 3.45679);
}

TEST_CASE(initial_editor_unrecovered_live_advanced_state_stops_after_earlier_page_stores)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); initialize(sc);
    RemeshState remesh; Io io;
    auto session = openInitialEditor(model, remesh, io, context()); REQUIRE(session);
    auto* object = model.objectWrap(1, true, false); REQUIRE(object);
    field<int>(object, 0x21c8) = 6;
    auto& path = field<def::model::DYNARY>(object, 0x25a8);
    REQUIRE(path.type == 0x7a); REQUIRE(path.ndim == 1);
    REQUIRE(path.n[0] == 1); REQUIRE(path.ncomp == 4);
    REQUIRE(path.axis[0]); REQUIRE(path.values);
    path.values[0] = 1.0; // nonzero path evaluation is still unrecovered
    sc.nstep = 99;
    std::vector<RecoveryEntry> empty;
    const auto report = applyInitialEditorPageState(*session, model, remesh, io, &empty, &empty);
    CHECK(report.outcome == ApplyResult::UnsupportedSnapshot); CHECK_EQ(sc.nstep, 23);
    REQUIRE(report.pages[index(Page::Advanced)]);
    CHECK(*report.pages[index(Page::Advanced)] == ApplyResult::UnsupportedSnapshot);
    CHECK(!report.pages[index(Page::Extra3D)]); CHECK(!report.recoveryTail); CHECK(io.effects.empty());
}

TEST_CASE(initial_editor_populated_recovery_refuses_at_tail_after_page_and_io_prefix)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); initialize(sc);
    RemeshState remesh; Io io;
    auto session = openInitialEditor(model, remesh, io, context()); REQUIRE(session);
    sc.nstep = 99; sc.nrecid.ncomp = 7;
    std::vector<RecoveryEntry> populated(1), empty;
    const auto report = applyInitialEditorPageState(*session, model, remesh, io, &populated, &empty);
    CHECK(report.outcome == ApplyResult::UnsupportedSnapshot); CHECK_EQ(sc.nstep, 23);
    REQUIRE(report.pages[index(Page::Extra3D)]); REQUIRE(report.recoveryTail);
    CHECK(*report.recoveryTail == ApplyResult::UnsupportedSnapshot);
    CHECK(!io.effects.empty()); CHECK_EQ(sc.nrecid.ncomp, 7);
}

TEST_CASE(initial_editor_actual_operation_routes_populated_recovery_after_extra3d)
{
    Project project;
    auto sub = std::make_unique<SubProject>(); sub->project = &project;
    project.subs.push_back(std::move(sub));
    Operation operation; operation.project = &project;
    operation.engineModel = std::make_shared<engine::GuiModel>(nullptr, 3);
    auto& model = *operation.engineModel;
    auto& sc = *model.simCtl(); initialize(sc);
    RemeshState remesh; Io io;
    auto session = openInitialEditor(model, remesh, io, context()); REQUIRE(session);
    project.recoveryA->push_back(RecoveryEntry{0, 0.0, 41, 2.5, 3.5});
    operation.recoveryB->push_back(RecoveryEntry{1, 0.0, 43, 4.5, 5.5});
    sc.nstep = 99;
    const auto report = applyInitialEditorPageState(*session, model, remesh, io, operation);
    CHECK(report.outcome == ApplyResult::Completed);
    REQUIRE(report.pages[index(Page::Extra3D)]); REQUIRE(report.recoveryTail);
    CHECK(*report.recoveryTail == ApplyResult::Completed);
    CHECK(!io.effects.empty()); CHECK_EQ(sc.nstep, 23);
    CHECK_EQ(sc.nrecid.n[0], 1); CHECK_EQ(sc.erecid.n[0], 1);
    REQUIRE(sc.nrecid.axis[0]); REQUIRE(sc.erecid.axis[0]);
    CHECK(sc.nrecid.axis[0][0] == 41.0); CHECK(sc.erecid.axis[0][0] == 43.0);
    CHECK(!InitialEditorSession::completeForPublicSave);
}

TEST_CASE(initial_editor_actual_operation_mismatch_refuses_before_pages_or_io)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); initialize(sc);
    RemeshState remesh; Io io;
    auto session = openInitialEditor(model, remesh, io, context()); REQUIRE(session);
    Project project; Operation operation;
    operation.project = &project;
    operation.engineModel = std::make_shared<engine::GuiModel>(nullptr, 3);
    sc.nstep = 99;
    const auto report = applyInitialEditorPageState(*session, model, remesh, io, operation);
    CHECK(report.outcome == ApplyResult::UnsupportedSnapshot);
    for (const auto& page : report.pages) CHECK(!page);
    CHECK(!report.recoveryTail); CHECK(io.effects.empty()); CHECK_EQ(sc.nstep, 99);
}

TEST_CASE(initial_editor_preflight_rejects_replaced_step_without_effects)
{
    engine::GuiModel model(nullptr, 3); auto& original = *model.simCtl(); initialize(original);
    RemeshState remesh; Io io;
    auto session = openInitialEditor(model, remesh, io, context()); REQUIRE(session);
    original.nstep = 99;
    std::unique_ptr<def::model::DB_STEP_DATA> retainedStep;
    retainedStep = std::move(model.step);
    REQUIRE(retainedStep);
    model.step = std::make_unique<def::model::DB_STEP_DATA>(3);
    auto* const replacementControl = model.simCtl(); REQUIRE(replacementControl);
    replacementControl->nstep = 177;
    const auto report = applyInitialEditorPageState(*session, model, remesh, io,
        static_cast<const std::vector<RecoveryEntry>*>(nullptr),
        static_cast<const std::vector<RecoveryEntry>*>(nullptr));
    CHECK(report.outcome == ApplyResult::UnsupportedSnapshot);
    for (const auto& page : report.pages) CHECK(!page);
    CHECK(!report.recoveryTail); CHECK(io.effects.empty());
    CHECK_EQ(original.nstep, 99); CHECK_EQ(replacementControl->nstep, 177);
}

TEST_CASE(initial_editor_actual_operation_rechecks_control_after_extra3d_callback)
{
    Project project;
    auto sub = std::make_unique<SubProject>(); sub->project = &project;
    project.subs.push_back(std::move(sub));
    Operation operation; operation.project = &project;
    operation.engineModel = std::make_shared<engine::GuiModel>(nullptr, 3);
    auto& model = *operation.engineModel;
    auto& sc = *model.simCtl(); initialize(sc);
    RemeshState remesh; Io io;
    auto session = openInitialEditor(model, remesh, io, context()); REQUIRE(session);
    std::unique_ptr<def::model::DB_STEP_DATA> retainedStep;
    sc.nstep = 99;
    session->pages.meshing.objects.push_back(MeshObjectSnapshot{1, 1.0, 2.0, 3.0, 4.0});
    project.recoveryA->push_back(RecoveryEntry{0, 0.0, 41, 2.5, 3.5});
    bool replaced = false;
    io.duringFirstEffect = [&] {
        retainedStep = std::move(model.step);
        model.step = std::make_unique<def::model::DB_STEP_DATA>(3);
        replaced = true;
    };
    const auto report = applyInitialEditorPageState(*session, model, remesh, io, operation);
    CHECK(replaced); CHECK(!io.effects.empty());
    REQUIRE(report.pages[index(Page::Extra3D)]); REQUIRE(report.recoveryTail);
    CHECK(*report.recoveryTail == ApplyResult::UnsupportedSnapshot);
    CHECK(report.outcome == ApplyResult::UnsupportedSnapshot);
    CHECK_EQ(sc.nstep, 23); // Apply overwrote the post-Open sentinel before tail refusal.
    CHECK(session->pages.meshing.objects.empty()); // The temporary list was cleared before identity refusal.
    REQUIRE(model.simCtl());
    CHECK_EQ(model.simCtl()->nrecid.n[0], 0);
}

TEST_CASE(initial_editor_context_and_model_identity_remain_explicit)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); initialize(sc);
    RemeshState remesh; Io io;
    auto unknown = context(); unknown.numericLocale = NumericLocale::Unrecovered;
    sc.smode = 99;
    CHECK(!openInitialEditor(model, remesh, io, unknown)); CHECK_EQ(sc.smode, 99);
    auto session = openInitialEditor(model, remesh, io, context()); REQUIRE(session);
    engine::GuiModel other(nullptr, 3); other.simCtl()->nstep = 123;
    std::vector<RecoveryEntry> empty;
    const auto report = applyInitialEditorPageState(*session, other, remesh, io, &empty, &empty);
    CHECK(report.outcome == ApplyResult::UnsupportedSnapshot);
    CHECK_EQ(other.simCtl()->nstep, 123); CHECK(io.effects.empty());
    for (const auto& page : report.pages) CHECK(!page);
}

TEST_CASE(initial_editor_requires_same_live_remesh_owner_before_any_apply_effect)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); initialize(sc);
    RemeshState owner; owner.enabled = true; owner.method = 3; owner.scalingFactor = 2.5;
    Io io; auto session = openInitialEditor(model, owner, io, context()); REQUIRE(session);
    CHECK(session->sourceRemesh == &owner);
    RemeshState copy = owner;
    sc.nstep = 777; std::strcpy(sc.title, "UNCHANGED");
    std::vector<RecoveryEntry> empty;
    auto report = applyInitialEditorPageState(*session, model, copy, io, &empty, &empty);
    CHECK(report.outcome == ApplyResult::UnsupportedSnapshot);
    for (const auto& page : report.pages) CHECK(!page);
    CHECK(!report.recoveryTail); CHECK_EQ(sc.nstep, 777); CHECK(std::strcmp(sc.title, "UNCHANGED") == 0);
    CHECK_EQ(copy.method, 3); CHECK(copy.scalingFactor == 2.5); CHECK(io.effects.empty());
    session->sourceRemesh = nullptr;
    report = applyInitialEditorPageState(*session, model, owner, io, &empty, &empty);
    CHECK(report.outcome == ApplyResult::UnsupportedSnapshot);
    for (const auto& page : report.pages) CHECK(!page);
    CHECK(!report.recoveryTail); CHECK_EQ(sc.nstep, 777); CHECK(io.effects.empty());
}

TEST_CASE(initial_editor_remesh_widgets_write_same_owner_without_restoring_other_global_fields)
{
    for (const bool enabledAtFill : {false, true}) {
        engine::DiagramEffectDriver driver;
        engine::ConversionSession conversion(driver);
        engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); initialize(sc);
        auto& owner = conversion.remeshState(); owner.enabled = enabledAtFill; owner.method = 3;
        owner.scalingFactor = 2.345678; owner.goodInternalChangeable = false;
        Io io; auto session = openInitialEditor(model, owner, io, context()); REQUIRE(session);
        CHECK(session->sourceRemesh == &owner);
        // Simulate later original-global changes after the widgets have filled.
        // The editor reads/writes the same owner, not a saved complete struct.
        owner.enabled = true; owner.method = 5; owner.scalingFactor = 8.0;
        owner.goodInternalChangeable = true; owner.filterDistance = 77.0;
        owner.uniformInternalEdge = 1.25; owner.improveContact = true; owner.goodSurfaceChangeable = false;
        std::vector<RecoveryEntry> empty;
        const auto report = applyInitialEditorPageState(*session, model, owner, io, &empty, &empty);
        CHECK(report.outcome == ApplyResult::Completed);
        CHECK_EQ(owner.enabled, enabledAtFill);
        if (enabledAtFill) {
            CHECK_EQ(owner.method, 3); CHECK(owner.scalingFactor == 2.34568);
            CHECK(!owner.goodInternalChangeable);
        } else {
            CHECK_EQ(owner.method, 5); CHECK(owner.scalingFactor == 8.0);
            CHECK(owner.goodInternalChangeable);
        }
        CHECK(owner.filterDistance == 77.0); CHECK(owner.uniformInternalEdge == 1.25);
        CHECK(owner.improveContact); CHECK(!owner.goodSurfaceChangeable);
    }
}

namespace {
SpringComboLabels springSentinel()
{
    return {{qt::L("prior-first"), qt::L("prior-tail")}, {qt::L("prior-second")}, 17, 29};
}
void checkSpringSentinel(const SpringComboLabels& labels)
{
    CHECK((labels.first == std::vector<qt::String>{qt::L("prior-first"), qt::L("prior-tail")}));
    CHECK((labels.second == std::vector<qt::String>{qt::L("prior-second")}));
    CHECK_EQ(labels.firstCurrentIndex, 17); CHECK_EQ(labels.secondCurrentIndex, 29);
}
void checkSpringCatalog(const SpringComboLabels& labels, const std::vector<qt::String>& expected)
{
    CHECK(labels.first == expected); CHECK(labels.second == expected);
    CHECK_EQ(labels.firstCurrentIndex, 0); CHECK_EQ(labels.secondCurrentIndex, 0);
}
}

TEST_CASE(editor_spring_labels_zero_count_and_absent_step_replace_owned_output)
{
    engine::GuiModel model(nullptr, 3);
    REQUIRE(model.objectCount() == 0);
    auto labels = springSentinel();
    CHECK(model.copyEditorSpringComboLabels(labels));
    checkSpringCatalog(labels, {qt::L("0   - None")});
    model.step.reset();
    labels = springSentinel();
    CHECK(model.copyEditorSpringComboLabels(labels));
    checkSpringCatalog(labels, {qt::L("0   - None")});
}

TEST_CASE(editor_spring_labels_dense_number_order_kind7_and_owned_strings)
{
    engine::GuiModel model(nullptr, 3);
    for (int i = 1; i <= 3; ++i) {
        auto* object = model.objectWrap(i, true, false); REQUIRE(object);
        field<std::int32_t>(object, 0xc) = i == 2 ? 7 : 1;
        std::strcpy(&field<char>(object, 0x828), i == 1 ? "First" : "Third");
    }
    // Kind 7 is skipped before its malformed name is examined.
    std::memset(&field<char>(model.object(2), 0x828), 0xff, 0x54);
    auto labels = springSentinel();
    CHECK(model.copyEditorSpringComboLabels(labels));
    const std::vector<qt::String> expected{qt::L("0   - None"), qt::L("1   - First"), qt::L("3   - Third")};
    checkSpringCatalog(labels, expected);
    std::strcpy(&field<char>(model.object(1), 0x828), "Changed");
    std::memset(&field<char>(model.object(2), 0x828), 0, 0x54);
    field<std::int32_t>(model.object(2), 0xc) = 1;
    model.step.reset();
    checkSpringCatalog(labels, expected);
}

TEST_CASE(editor_spring_labels_hole_and_count_caps_preserve_all_output)
{
    engine::GuiModel model(nullptr, 3);
    auto* object = model.objectWrap(1, true, false); REQUIRE(object);
    field<std::int32_t>(object, 0xc) = 1;
    std::strcpy(&field<char>(object, 0x828), "Admitted before hole");
    const auto actualCount = model.step->u1378;
    REQUIRE(actualCount == 1);
    for (int count : {2, -1, 4097}) {
        // Retain real allocation ownership; only the reported count is scoped.
        model.step->u1378 = count;
        auto labels = springSentinel();
        const bool copied = model.copyEditorSpringComboLabels(labels);
        model.step->u1378 = actualCount;
        CHECK(!copied); checkSpringSentinel(labels);
    }
}

TEST_CASE(editor_spring_labels_name_and_formatted_buffer_boundaries)
{
    engine::GuiModel model(nullptr, 3);
    auto* object = model.objectWrap(1, true, false); REQUIRE(object);
    field<std::int32_t>(object, 0xc) = 1;
    auto* name = &field<char>(object, 0x828);
    auto setName = [&](const std::string& value) {
        std::memset(name, 0, 0x54); std::memcpy(name, value.data(), value.size());
    };
    for (const auto& invalid : {std::string(0x54, 'N'), std::string("bad\x80", 4),
             std::string(58, 'L')}) {
        setName(invalid);
        auto labels = springSentinel();
        CHECK(!model.copyEditorSpringComboLabels(labels)); checkSpringSentinel(labels);
    }
    // Six prefix bytes plus 57 name bytes produces the last admitted length, 63.
    setName(std::string(57, 'L'));
    auto labels = springSentinel();
    CHECK(model.copyEditorSpringComboLabels(labels));
    checkSpringCatalog(labels, {qt::L("0   - None"), qt::String::latin1("1   - " + std::string(57, 'L'))});
    setName("");
    CHECK(model.copyEditorSpringComboLabels(labels));
    checkSpringCatalog(labels, {qt::L("0   - None"), qt::L("1   - ")});
    // A terminator at the last scanned index is found, but formatting still refuses.
    setName(std::string(0x53, 'N'));
    labels = springSentinel();
    CHECK(!model.copyEditorSpringComboLabels(labels)); checkSpringSentinel(labels);
    setName("Restored");
}

TEST_CASE(initial_editor_spring_combo_projection_observes_model_after_scalar_sidecar_reads)
{
    engine::GuiModel model(nullptr, 3); auto& sc = *model.simCtl(); initialize(sc);
    REQUIRE(model.objectCount() == 0);
    struct OrderedIo final : Extra3DIo {
        engine::GuiModel& model;
        std::vector<std::string> calls;
        bool objectsCreated = false;
        explicit OrderedIo(engine::GuiModel& value) : Extra3DIo(1252), model(value) {}
        bool readDoubles(const qt::String& path, std::span<double> output) override {
            calls.push_back("double:" + path.utf8() + ":" + std::to_string(output.size()));
            std::fill(output.begin(), output.end(), 0.0);
            if (path == qt::L("STRAIN_DST.DAT")) {
                REQUIRE(!objectsCreated); REQUIRE(model.objectCount() == 0);
                // A supplied IO observation changes the actual model only at
                // the last scalar read. An eager combo scan would retain {0}
                // and fail the later one-row IDs10/20 admission.
                for (int number = 1; number <= 20; ++number)
                    REQUIRE(model.objectWrap(number, true, false));
                objectsCreated = true;
                calls.push_back("objects-created-after-scalars");
            }
            return true;
        }
        bool readInts(const qt::String& path, std::span<int> output) override {
            calls.push_back("int:" + path.utf8() + ":" + std::to_string(output.size()));
            std::fill(output.begin(), output.end(), 1);
            return true; // Includes the conditional STRETCH double-read.
        }
        bool readSprings(std::vector<SpringRecord>& output) override {
            calls.push_back("springs");
            REQUIRE(objectsCreated); REQUIRE(model.objectCount() == 20);
            output = {{10, 0.5, 1.5, 20, {0.0, 1.0, 0.0}, 0}};
            return true;
        }
        bool removeIfReadable(const qt::String&) override { CHECK(false); return false; }
        void nbcRemoveWarning() override { CHECK(false); }
        std::unique_ptr<mst::OutputDevice> openWrite(const qt::String&) override { CHECK(false); return {}; }
    } io(model);
    RemeshState remesh; InitialEditorSession session; InitialOpenReport report;
    auto selectedContext = context();
    selectedContext.stoppingLabelProfile = StoppingLabelProfile::AsciiNoActiveTranslator;
    openInitialEditorInto(session, model, remesh, io, selectedContext, report);
    CHECK((io.calls == std::vector<std::string>{"double:NBC.DAT:2", "int:SW2SP.DAT:1",
        "int:STRETCH.DAT:1", "double:STRETCH.DAT:3", "double:B_WEIGHT.DAT:1",
        "double:STRAIN_DST.DAT:0", "objects-created-after-scalars", "springs"}));
    REQUIRE(report.complete()); CHECK(report.initialPages[index(Page::Extra3D)]);
    REQUIRE(session.extra3d.spring); REQUIRE(session.extra3d.spring->items.size() == 1);
    CHECK_EQ(session.extra3d.spring->firstSelectedId, 10);
    CHECK_EQ(session.extra3d.spring->secondSelectedId, 20);
    CHECK_EQ(session.extra3d.spring->currentRow, 0);
    CHECK(!session.extra3d.spring->recordFlagChecked);
    CHECK(session.sourceModel == &model); CHECK(session.sourceRemesh == &remesh);
    CHECK(!InitialEditorSession::completeForPublicSave);
}

int main() { return testkit::run_all(); }
