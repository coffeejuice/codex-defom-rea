// ctest-labels: m2m-mdt
// Explicit private page event; not a native QWidget or full Session fixture.
#include <testkit/check.hpp>
#include <algorithm>
#include <array>
#include <cstring>
#include <iterator>
#include <memory>
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
    Project project;
    Operation operation;
    RemeshState remesh;
    Io io;
    std::unique_ptr<StagedEditor300> owner;
    Fixture()
    {
        operation.project = &project;
        operation.engineModel = std::make_shared<engine::GuiModel>(nullptr, 3);
        auto& sc = *operation.engineModel->simCtl();
        sc.unit = 1; sc.nstart = 7; sc.nstep = 23;
        sc.smode = 99; sc.trans[0] = sc.trans[1] = 1;
        std::strcpy(sc.title, "ORIGINAL TITLE");
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
std::array<char, 81> titleBytes(const SimControl& sc)
{
    std::array<char, 81> result; static_assert(sizeof(sc.title) == 81);
    std::copy(std::begin(sc.title), std::end(sc.title), result.begin()); return result;
}
}

TEST_CASE(title_delivery_copies_immediately_without_general_apply_or_external_effects)
{
    Fixture f; auto& session = f.open(); auto& page = session.pages.general;
    auto& sc = *f.operation.engineModel->simCtl();
    const int reads = f.io.reads;
    page.title = qt::L("DELIVERED TITLE"); page.operationName = qt::L("UNSAVED NAME");
    page.unit = 1; page.simulationType = 1;
    sc.unit = 2; sc.stype = 77; sc.smode = 99; sc.trans[0] = 8;
    sc.nstart = 51; sc.nstep = 52; sc.pdie1 = 53;
    std::strcpy(sc.oprnam, "MODEL NAME");
    f.operation.name = qt::L("OPI NAME"); f.operation.nstartGiven = 1; f.operation.startStepNo = 91;
    f.remesh.method = 5; f.remesh.enabled = 1;
    CHECK(f.owner->titleChanged(f.binding(), page.title) == ApplyResult::Completed);
    CHECK_EQ(std::string(sc.title), std::string("DELIVERED TITLE"));
    CHECK(page.title == qt::L("DELIVERED TITLE")); CHECK(page.operationName == qt::L("UNSAVED NAME"));
    CHECK_EQ(std::string(sc.oprnam), std::string("MODEL NAME"));
    CHECK_EQ(sc.unit, 2); CHECK_EQ(sc.stype, 77); CHECK_EQ(sc.smode, 99); CHECK_EQ(sc.trans[0], 8);
    CHECK_EQ(sc.nstart, 51); CHECK_EQ(sc.nstep, 52); CHECK_EQ(sc.pdie1, 53);
    CHECK(f.operation.name == qt::L("OPI NAME")); CHECK_EQ(f.operation.nstartGiven, 1);
    CHECK_EQ(f.operation.startStepNo, 91); CHECK_EQ(f.remesh.method, 5); CHECK_EQ(f.remesh.enabled, 1);
    CHECK_EQ(f.io.reads, reads); CHECK_EQ(f.io.writes, 0); CHECK_EQ(f.io.removes, 0);
    CHECK_EQ(session.pages.stepCount.steps, 23);
    CHECK(f.owner->construction().state == Editor300ConstructorState::AwaitingNativeBoundaries);
    CHECK(!StagedEditor300::completeForPublicSave);
}

TEST_CASE(title_shorter_delivery_copies_zero_without_padding_the_remaining_buffer)
{
    Fixture f; auto& page = f.open().pages.general; auto& sc = *f.operation.engineModel->simCtl();
    std::fill(std::begin(sc.title), std::end(sc.title), 'Q'); sc.title[80] = 0;
    page.title = qt::L("AB");
    CHECK(f.owner->titleChanged(f.binding(), page.title) == ApplyResult::Completed);
    CHECK_EQ(sc.title[0], 'A'); CHECK_EQ(sc.title[1], 'B'); CHECK_EQ(sc.title[2], '\0');
    CHECK_EQ(sc.title[3], 'Q'); CHECK_EQ(sc.title[79], 'Q'); CHECK_EQ(sc.title[80], '\0');
}

TEST_CASE(title_eighty_ascii_bytes_fit_before_adjacent_operation_name)
{
    Fixture f; auto& page = f.open().pages.general; auto& sc = *f.operation.engineModel->simCtl();
    std::strcpy(sc.oprnam, "ADJACENT SENTINEL");
    page.title = qt::String(std::u16string(80, u'Z'));
    CHECK(f.owner->titleChanged(f.binding(), page.title) == ApplyResult::Completed);
    for (int i = 0; i < 80; ++i) CHECK_EQ(sc.title[i], 'Z');
    CHECK_EQ(sc.title[80], '\0'); CHECK_EQ(std::string(sc.oprnam), std::string("ADJACENT SENTINEL"));
}

TEST_CASE(title_null_and_empty_delivery_preserve_cached_representation_and_only_store_zero)
{
    for (bool null : {false, true}) {
        Fixture f; auto& page = f.open().pages.general; auto& sc = *f.operation.engineModel->simCtl();
        page.title = null ? qt::String() : qt::L("");
        CHECK_EQ(page.title.isNull(), null);
        const auto before = titleBytes(sc);
        CHECK(f.owner->titleChanged(f.binding(), page.title) == ApplyResult::Completed);
        CHECK_EQ(sc.title[0], '\0');
        for (int i = 1; i < 81; ++i) CHECK_EQ(sc.title[i], before[i]);
        CHECK_EQ(page.title.isNull(), null); CHECK(page.title.isEmpty());
    }
}

TEST_CASE(title_each_explicit_delivery_recopies_after_external_sc_change)
{
    Fixture f; auto& page = f.open().pages.general; auto& sc = *f.operation.engineModel->simCtl();
    page.title = qt::L("SAME DELIVERED TEXT");
    CHECK(f.owner->titleChanged(f.binding(), page.title) == ApplyResult::Completed);
    std::strcpy(sc.title, "EXTERNAL CHANGE");
    CHECK(f.owner->titleChanged(f.binding(), page.title) == ApplyResult::Completed);
    CHECK_EQ(std::string(sc.title), std::string("SAME DELIVERED TEXT"));
}

TEST_CASE(title_excluded_text_and_mismatched_event_refuse_without_model_store)
{
    const std::array<qt::String, 5> excluded{
        qt::String(std::u16string(81, u'X')), qt::String(u"NONASCII\u00e9"),
        qt::String(std::u16string{u'A', u'\0', u'B'}), qt::String(u"LINE\nBREAK"), qt::String(u"TAB\tTEXT")};
    Fixture f; auto& page = f.open().pages.general; auto& sc = *f.operation.engineModel->simCtl();
    const auto before = titleBytes(sc);
    for (const auto& text : excluded) {
        page.title = text;
        CHECK(f.owner->titleChanged(f.binding(), text) == ApplyResult::UnsupportedSnapshot);
        CHECK(titleBytes(sc) == before); CHECK(page.title == text);
    }
    page.title = qt::L("CACHED");
    CHECK(f.owner->titleChanged(f.binding(), qt::L("OTHER EVENT")) == ApplyResult::UnsupportedSnapshot);
    CHECK(titleBytes(sc) == before);
    Project otherProject;
    CHECK(f.owner->titleChanged({otherProject, f.operation, *f.operation.engineModel, f.remesh}, page.title)
        == ApplyResult::UnsupportedSnapshot);
    CHECK(titleBytes(sc) == before);
}

TEST_CASE(title_missing_and_replaced_current_control_do_not_use_stale_source)
{
    for (bool replace : {false, true}) {
        Fixture f; auto& session = f.open(); auto& page = session.pages.general;
        page.title = qt::L("NEW EVENT");
        auto oldStep = std::move(f.operation.engineModel->step);
        const auto before = titleBytes(oldStep->sim_ctrl);
        if (replace) f.operation.engineModel->step = std::make_unique<def::model::DB_STEP_DATA>(3);
        auto* replacement = f.operation.engineModel->simCtl();
        REQUIRE(replacement != session.sourceControl);
        if (replacement) std::strcpy(replacement->title, "REPLACEMENT");
        CHECK(f.owner->titleChanged(f.binding(), page.title) == ApplyResult::UnsupportedSnapshot);
        CHECK(titleBytes(oldStep->sim_ctrl) == before); CHECK(page.title == qt::L("NEW EVENT"));
        if (replacement) CHECK_EQ(std::string(replacement->title), std::string("REPLACEMENT"));
    }
}

TEST_CASE(title_unconstructed_partial_and_expired_opi_cannot_bypass_public_page_guard)
{
    Fixture f; auto& sc = *f.operation.engineModel->simCtl(); const auto before = titleBytes(sc);
    CHECK(f.owner->titleChanged(f.binding(), qt::L("EVENT")) == ApplyResult::UnsupportedSnapshot);
    CHECK(!f.owner->partialPageState()); CHECK(titleBytes(sc) == before);
    auto context = f.context(); context.moObservations.reset();
    f.owner->constructPageState(f.binding(), f.io, context);
    CHECK(f.owner->titleChanged(f.binding(), qt::L("EVENT")) == ApplyResult::UnsupportedSnapshot);
    CHECK(!f.owner->pageState()); CHECK(titleBytes(sc) == before);

    Fixture live; auto& page = live.open().pages.general; page.title = qt::L("DELIVERED");
    const auto model = live.operation.engineModel; const auto liveBefore = titleBytes(*model->simCtl());
    REQUIRE(live.operation.commitOperationInfoQObjectBoundary(
        "declared private title fixture OPI boundary; not native destruction"));
    CHECK(live.owner->titleChanged(live.binding(), page.title) == ApplyResult::UnsupportedSnapshot);
    CHECK(titleBytes(*model->simCtl()) == liveBefore); CHECK(!live.owner->pageState());
    CHECK(!StagedEditor300::completeForPublicSave);
}

int main() { return testkit::run_all(); }
