// ctest-labels: m2m-mdt doe-default-session
#include <testkit/check.hpp>

#include <memory>
#include <string>

#include <defmodel/layout/cls_1401e7b10.hpp>
#include "m2m/common.hpp"
#include "m2m/doe_session.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/doe_task_runtime.hpp"
#include "m2m/doe_task_factory.hpp"
#include "m2m/doe_statevar.hpp"
#include "m2m/doe_stepdata.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine_scene.hpp"
#include "m2m/engine/model_manager.hpp"
#include "m2m/mst/writer.hpp" //complete OpClass for local Project destruction

using namespace m2m;
using qt::L;
namespace {
template<class F> bool refuses(F&& action, const char* detail)
{
    try { action(); }
    catch (const Error& error) {
        return error.code == M2M_E_UNSUPPORTED && std::string(error.what()).find(detail) != std::string::npos;
    }
    return false;
}
struct Fixture {
    std::shared_ptr<doe::TaskAllocationStore> allocations = std::make_shared<doe::TaskAllocationStore>();
    Project project;
    std::shared_ptr<engine::DbHandle> child = std::make_shared<engine::DbHandle>(engine::DbHandle::Kind::Mo);
    engine::GuiModel main{child.get(), 3};
    doe::SessionDocument document;
    std::shared_ptr<doe::StudyIo> io = doe::makeRecordingStudyIo();
    std::shared_ptr<doe::TaskFactoryRuntime> taskProjection;
    std::shared_ptr<engine::ModelManager> manager = std::make_shared<engine::ModelManager>();
    Fixture()
    {
        io->bindTaskAllocationStore(allocations);
        taskProjection = doe::bindBoundedParsedDoeProjection(*io, allocations);
        project.path = L("C:/PROBLEM/DOEUnit/P.moproj"); //recording-only declared fixture
        document.dom.root = xml::createElement(L("PartRoot"));
        document.partRoot = document.dom.root;
        document.parts.emplace_back();
        auto& part = document.parts.back();
        part.root = xml::createElement(L("SessionRoot"));
        part.root->children.push_back(xml::createElement(L("Keep")));
        document.partRoot->children.push_back(part.root);
        part.operationStart = 1;
        part.operationEnd = 2;
        document.currentPart = 0;
        engine::projectDisplay(project);
    }
    void bind() { io->bindModelManager(manager); }
    void addActiveOperation(int id, const char* key, const char* name)
    {
        if (project.subs.empty()) project.subs.push_back(makeSubProject(project, 0, 0));
        const auto classKey = L(key);
        if (!project.registry.contains(classKey))
            project.registry[classKey] = std::make_unique<mst::OpClass>(classKey);
        auto operation = std::make_unique<Operation>();
        operation->project = &project;
        operation->id = id;
        operation->name = L(name);
        operation->classKey = classKey;
        operation->opClass = project.registry[classKey].get();
        project.operations.push_back(std::move(operation));
        auto& flow = project.subs[0]->flow;
        auto item = std::make_unique<FlowItem>();
        item->opId = id;
        item->className = classKey;
        item->col = static_cast<int>(flow.items.size());
        if (!flow.items.empty()) {
            item->prev = flow.items.back().get();
            flow.items.back()->next = item.get();
        }
        flow.items.push_back(std::move(item));
    }
    void construct() { doe::constructDefaultDoeSession(project, document, *io); }
    doe::AdapterDefaultSessionAllocation& staged()
    {
        REQUIRE(document.adapterDefaultAllocations.size() == 1u);
        REQUIRE(document.adapterDefaultAllocations.front()->record != nullptr);
        return *document.adapterDefaultAllocations.front();
    }
    void unchangedAppend()
    {
        CHECK(document.parts[0].sessions.empty());
        CHECK_EQ(document.parts[0].currentSession, -1);
        REQUIRE(document.parts[0].root->children.size() == 1u);
        CHECK(document.parts[0].root->children[0]->tag == L("Keep"));
        CHECK(io->directories().empty());
        CHECK(io->reads().empty());
        CHECK(io->textWrites().empty());
    }
};
}

TEST_CASE(doe_default_session_unavailable_manager_guards_before_allocation)
{
    Fixture f;
    CHECK(refuses([&] { f.construct(); }, "live model manager"));
    CHECK(f.document.adapterDefaultAllocations.empty());
    CHECK(f.io->observedSessionNotificationState() == &f.io->sessionNotificationState());
    CHECK(f.io->sessionNotificationState().attempts.empty());
    f.unchangedAppend();
}

TEST_CASE(doe_default_session_unselected_live_owner_null_title_and_null_image_metadata)
{
    Fixture f;
    f.bind();
    f.manager->appendCreatedModel(f.child); //actual registration does not select
    CHECK(refuses([&] { f.construct(); }, "main-window data-view snapshot hook"));
    auto& staged = f.staged();
    CHECK_EQ(staged.record->databaseId, -1);
    CHECK(staged.record->title.isNull());
    CHECK_EQ(staged.record->type, 0);
    CHECK_EQ(staged.record->subtype, 0);
    CHECK_EQ(staged.record->stepNumber, -1);
    CHECK(staged.factoryInvoked);
    REQUIRE(staged.record->task != nullptr);
    CHECK(staged.record->task->lists.operations.empty());
    REQUIRE(staged.record->task->lists.objects.size() == 1u);
    CHECK_EQ(staged.record->task->lists.objects[0], 0);
    REQUIRE(staged.detachedSession != nullptr);
    CHECK(!staged.detachedSession->hasAttribute(L("StepNumber")));
    CHECK(staged.detachedSession->attribute(L("DB_ID")) == L("-1"));
    CHECK(staged.detachedSession->attribute(L("TaskType")) == L("SfTaskDoe"));
    const auto images = staged.detachedSession->childElements(L("Image"));
    REQUIRE(images.size() == 1u);
    CHECK(images[0]->text() == L("00000000"));
    CHECK(staged.detachedStepData == nullptr);
    f.unchangedAppend();
}

TEST_CASE(doe_default_session_live_selected_id_short_wrap_and_main_nstart_before_snapshot_guard)
{
    Fixture f;
    f.bind();
    f.manager->appendCreatedModel(f.child);
    f.manager->selectOrdinal(0);
    f.child->modelId = 40000; //explicit fixture state: native int→short, not QString::toShort
    f.main.simCtl()->nstart = -123;
    engine::displayShow(f.project, &f.main);
    CHECK(refuses([&] { f.construct(); }, "constructor needs explicit resolved profile/global inputs"));
    auto& staged = f.staged();
    CHECK_EQ(staged.record->databaseId, -25536);
    CHECK_EQ(staged.record->stepNumber, -123);
    CHECK(staged.detachedSession->attribute(L("StepNumber")) == L("-123"));
    REQUIRE(staged.detachedStepData != nullptr);
    CHECK(staged.detachedStepData->tag == L("StepData"));
    CHECK(staged.detachedSession->childElements(L("StepData")).empty()); //not appended before writer
    REQUIRE(staged.detachedSession->children.size() == 1u);
    CHECK(staged.detachedSession->children[0]->tag == L("Image"));
    f.unchangedAppend();
}

TEST_CASE(doe_default_session_suppressed_return_notifier_keeps_task_null_and_flag90)
{
    Fixture f;
    f.bind();
    auto& state = f.io->sessionNotificationState();
    state.suppressed = true;
    state.flag90 = true;
    CHECK(refuses([&] { f.construct(); }, "main-window data-view snapshot hook"));
    CHECK(!f.staged().factoryInvoked);
    CHECK(f.staged().record->task == nullptr);
    CHECK(state.flag90);
    CHECK(state.emptyKeys.empty());
    REQUIRE(state.attempts.size() == 1u);
    CHECK(state.attempts[0].suppressed);
    CHECK(state.attempts[0].signal == "2signalGetCurrentStepViewInfo()");
    f.unchangedAppend();
}

TEST_CASE(doe_default_session_zero_range_null_current_queries_live_receivers_then_preserves_zero_bounds)
{
    Fixture f;
    f.bind();
    f.child->moProject = &f.project; //registered pending Project is deliberately NOT current
    f.manager->appendCreatedModel(f.child);
    f.document.parts[0].operationStart = 0;
    f.document.parts[0].operationEnd = 0;
    auto& state = f.io->sessionNotificationState();
    state.flag90 = true;
    CHECK(refuses([&] { f.construct(); }, "main-window data-view snapshot hook"));
    CHECK(f.staged().factoryInvoked);
    REQUIRE(f.staged().record->task != nullptr);
    CHECK(f.staged().detachedSession != nullptr);
    CHECK(!state.flag90);
    CHECK(state.emptyKeys.contains("2signalGetCurrentStepViewInfo()"));
    CHECK(!state.emptyKeys.contains("2signalCreateTaskBase()")); //known registered return receiver
    CHECK(!state.emptyKeys.contains("2signalGetOprStringList()"));
    CHECK(!state.emptyKeys.contains("2signalGetOprNoList()"));
    REQUIRE(state.attempts.size() == 3u);
    CHECK(state.attempts[1].signal == "2signalGetOprStringList()");
    CHECK(state.attempts[2].signal == "2signalGetOprNoList()");
    CHECK(!state.attempts[1].suppressed && !state.attempts[2].suppressed);
    CHECK_EQ(f.document.parts[0].operationStart, 0);
    CHECK_EQ(f.document.parts[0].operationEnd, 0);
    f.unchangedAppend();
}

TEST_CASE(doe_default_session_zero_range_guards_before_missing_live_project_lazy_construction)
{
    Fixture f;
    f.bind();
    f.manager->appendCreatedModel(f.child);
    f.manager->selectOrdinal(0);
    f.child->moProject = nullptr; //native044a90 would construct; not a fake empty Project
    f.document.parts[0].operationStart = f.document.parts[0].operationEnd = 0;
    CHECK(refuses([&] { f.construct(); }, "lazy Project construction"));
    CHECK(f.staged().factoryInvoked);
    REQUIRE(f.staged().record->task != nullptr);
    CHECK(f.staged().detachedSession == nullptr);
    const auto& state = f.io->sessionNotificationState();
    REQUIRE(state.attempts.size() == 2u);
    CHECK(state.attempts[0].signal == "2signalGetCurrentStepViewInfo()");
    CHECK(state.attempts[1].signal == "2signalGetOprStringList()");
    CHECK(!state.flag90);
    CHECK(!state.emptyKeys.contains("2signalGetOprStringList()"));
    CHECK_EQ(f.document.parts[0].operationStart, 0);
    CHECK_EQ(f.document.parts[0].operationEnd, 0);
    f.unchangedAppend();
}

TEST_CASE(doe_default_session_live_operation_range_uses_flow_numbers_and_case_sensitive_name_filter)
{
    Fixture f;
    f.bind();
    f.manager->appendCreatedModel(f.child);
    f.manager->selectOrdinal(0);
    f.child->moProject = &f.project;
    f.addActiveOperation(101, "SfOprReport", "Report");                 //flow1 excluded by class
    f.addActiveOperation(202, "SfOprForming", "skip 2D to 3D suffix");  //flow2 excluded by NAME
    f.addActiveOperation(303, "SfOprConvert2Dto3D", "renamed conversion"); //flow3 retained
    f.addActiveOperation(404, "SfOprForming", "2d to 3d lowercase");    //flow4 retained
    f.addActiveOperation(505, "SfOprShapeOptim", "Optimization");       //flow5 excluded by class
    f.addActiveOperation(606, "SfOprForming", "last retained");         //flow6 retained
    f.addActiveOperation(707, "SfOprForming", "trailing 2D to 3D");     //flow7 excluded by NAME
    f.document.parts[0].operationStart = f.document.parts[0].operationEnd = 0;
    CHECK(refuses([&] { f.construct(); }, "main-window data-view snapshot hook"));
    CHECK_EQ(f.document.parts[0].operationStart, 3);
    CHECK_EQ(f.document.parts[0].operationEnd, 6);
    CHECK_EQ(f.staged().record->databaseId, 1);
    REQUIRE(f.io->sessionNotificationState().attempts.size() == 3u);
    f.unchangedAppend();
}

TEST_CASE(doe_default_session_live_operation_range_all_filtered_leaves_zero_bounds)
{
    Fixture f;
    f.bind();
    f.manager->appendCreatedModel(f.child);
    f.manager->selectOrdinal(0);
    f.child->moProject = &f.project;
    f.addActiveOperation(99, "SfOprForming", "2D to 3D");
    f.document.parts[0].operationStart = f.document.parts[0].operationEnd = 0;
    CHECK(refuses([&] { f.construct(); }, "main-window data-view snapshot hook"));
    CHECK_EQ(f.document.parts[0].operationStart, 0);
    CHECK_EQ(f.document.parts[0].operationEnd, 0);
    f.unchangedAppend();
}

TEST_CASE(doe_default_session_live_project_without_active_subproject_returns_empty_lists)
{
    Fixture f;
    f.bind();
    f.manager->appendCreatedModel(f.child);
    f.manager->selectOrdinal(0);
    f.child->moProject = &f.project;
    CHECK(activeSubProject(f.project) == nullptr);
    f.document.parts[0].operationStart = f.document.parts[0].operationEnd = 0;
    CHECK(refuses([&] { f.construct(); }, "main-window data-view snapshot hook"));
    CHECK_EQ(f.document.parts[0].operationStart, 0);
    CHECK_EQ(f.document.parts[0].operationEnd, 0);
    REQUIRE(f.io->sessionNotificationState().attempts.size() == 3u);
    f.unchangedAppend();
}

TEST_CASE(doe_default_session_zero_range_suppression_keeps_local_lists_empty_with_live_current)
{
    Fixture f;
    f.bind();
    f.manager->appendCreatedModel(f.child);
    f.manager->selectOrdinal(0);
    f.child->moProject = &f.project;
    f.document.parts[0].operationStart = f.document.parts[0].operationEnd = 0;
    auto& state = f.io->sessionNotificationState();
    state.suppressed = true;
    state.flag90 = true;
    CHECK(refuses([&] { f.construct(); }, "main-window data-view snapshot hook"));
    CHECK(!f.staged().factoryInvoked);
    CHECK(f.staged().record->task == nullptr);
    CHECK(f.staged().detachedSession != nullptr);
    CHECK(state.flag90);
    CHECK(state.emptyKeys.empty());
    REQUIRE(state.attempts.size() == 3u);
    CHECK(state.attempts[1].suppressed && state.attempts[2].suppressed);
    CHECK(state.attempts[2].signal == "2signalGetOprNoList()");
    CHECK_EQ(f.document.parts[0].operationStart, 0);
    CHECK_EQ(f.document.parts[0].operationEnd, 0);
    f.unchangedAppend();
}

TEST_CASE(doe_default_session_one_zero_range_skips_collection_then_missing_display_retains_metadata)
{
    Fixture f;
    f.bind();
    f.document.parts[0].operationStart = 0; //only BOTH zero enter64c280
    f.project.display.reset();
    CHECK(refuses([&] { f.construct(); }, "actual display-main binding"));
    auto& staged = f.staged();
    REQUIRE(staged.detachedSession != nullptr);
    CHECK_EQ(staged.detachedSession->attrs.size(), 6u);
    CHECK(staged.detachedSession->attribute(L("DB_ID")) == L("-1"));
    CHECK(!staged.detachedSession->hasAttribute(L("TaskType")));
    CHECK(staged.detachedSession->children.empty());
    CHECK_EQ(f.document.parts[0].operationStart, 0);
    CHECK_EQ(f.document.parts[0].operationEnd, 2);
    f.unchangedAppend();
}

TEST_CASE(doe_default_session_completed_empty_snapshot_appends_stepdata_before_later_hook_guard)
{
    Fixture f;
    f.bind();
    f.main.doeStepData = std::make_shared<doe::StepDataState>();
    auto state = std::make_shared<doe::StateVarState>(); //explicit existing-state fixture
    f.main.doeStepData->stateVar = state;
    state->plotVar = 13; //serialization does not apply/evaluate the selection
    state->postparsePending = true;
    state->histogram = std::make_shared<doe::HistogramState>();
    state->histogram->graphOn = true;
    state->histogram->binWidth = 7;
    f.main.simCtl()->nstart = 12;
    engine::displayShow(f.project, &f.main);
    CHECK(refuses([&] { f.construct(); }, "main-window data-view snapshot hook"));
    auto& staged = f.staged();
    REQUIRE(staged.detachedStepData != nullptr);
    REQUIRE(staged.detachedSession->children.size() == 2u);
    CHECK(staged.detachedSession->children[0]->tag == L("Image"));
    CHECK(staged.detachedSession->children[1] == staged.detachedStepData);
    const auto nodes = staged.detachedStepData->childElements(L("StateVar"));
    REQUIRE(nodes.size() == 1u);
    const auto variables = nodes[0]->childElements(L("SVPlotVar"));
    REQUIRE(variables.size() == 1u);
    CHECK(variables[0]->text() == L("13"));
    const auto histograms = nodes[0]->childElements(L("SVHistoGraph"));
    REQUIRE(histograms.size() == 1u);
    const auto widths = histograms[0]->childElements(L("BinWidth"));
    REQUIRE(widths.size() == 1u);
    CHECK(widths[0]->text() == L("7"));
    CHECK(f.main.doeStepData->stateVar == state);
    CHECK(state->postparsePending);
    CHECK(state->histogram->graphOn);
    CHECK_EQ(state->histogram->binWidth, 7);
    CHECK(!f.main.doeStepData->propertyTableCreated());
    f.unchangedAppend();
}

int main() { return testkit::run_all(); }
