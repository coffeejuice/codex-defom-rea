// Session storage lifetime, guarded dispatch, and real Boolean editor leaf tests.
// Shell-order test drivers below do not implement original Qt fit/notifications.
// ctest-labels: m2m-mdt
#include "../support/test_paths.hpp"
#include <testkit/check.hpp>
#include <array>
#include <process.h>
#include <cstring>
#include <filesystem>
#include <functional>
#include <fstream>
#include "m2m/qt4path.hpp"
#include "m2m/convert.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_session.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/doe_task_runtime.hpp"
#include "m2m/doe_task_factory.hpp"
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include "m2m/data/boolean.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/boolean_trace_due.hpp"
#include "m2m/engine/model_manager.hpp"
#include "m2m/engine/simcontrol300_owner.hpp"
#include "m2m/engine/editor_setup.hpp"
#include "m2m/engine/editor_base.hpp"
#include "m2m/engine/view_manager.hpp"
#include "m2m/engine/widget_events.hpp"
#include "m2m/engine/display_owner.hpp"
#include "m2m/engine/close_effects.hpp"
#include "m2m/engine_scene.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"

namespace m2m::mst { void installLoaderSlots(); }
using namespace m2m;
using Session = engine::ConversionSession;
using Events = engine::DiagramLifecycleEvents;

namespace {
struct ShellDriver : engine::DiagramEffectDriver {
    std::vector<std::string> order;
    std::function<void(Session&)> duringFit;
    std::function<void(Session&, const FlowItem&, engine::FileEffects&)> duringClick;
    void fitAndNotify(Session& session) override {
        order.push_back("fit-boundary");
        if (duringFit) duringFit(session);
    }
    void clickAndUpdateMainView(Session& session, const FlowItem& item,
                                engine::FileEffects& effects) override {
        order.push_back("click:" + std::to_string(item.opId));
        if (duringClick) duringClick(session, item, effects);
    }
};

template<class F> void expectUnsupported(F&& body)
{
    bool refused = false;
    try { body(); }
    catch (const Error& error) { refused = error.code == M2M_E_UNSUPPORTED; }
    CHECK(refused);
}

engine::DueObservation due(const engine::PumpPoint& point, std::vector<Events::TimerId> timers)
{
    return {point, engine::DueObservation::Origin::DeclaredAdapterSchedule,
            "isolated-session-shell-test; not an original timer trace", std::move(timers)};
}

TEST_CASE(session_open_model_registration_precedes_project_read_and_selection)
{
    ShellDriver driver;
    Session session(driver);
    auto manager = session.modelManagerOwner();
    REQUIRE(manager);
    CHECK(manager->size() == 0);
    session.prepareProjectModel(qt::L("C:/PROBLEM/ModelOpen/recovery.DB"));
    auto& project = session.project();
    REQUIRE(project.dataDb);
    CHECK(project.dataDb->kind == engine::DbHandle::Kind::Mo);
    CHECK(project.dataDb->modelId == 1);
    CHECK(project.dataDb->moProject == nullptr);
    CHECK(manager->currentModelId() == -1);
    REQUIRE(manager->connections().size() == 2);
    CHECK(manager->connections()[0].sender.lock() == project.dataDb);
    CHECK(manager->connections()[0].receiver == &session);
    CHECK(manager->connections()[0].rawConnectionType == 3);
    initProject(project, qt::L("C:/PROBLEM/ModelOpen"), qt::L("recovery"));
    session.commitProjectModelConstruction();
    CHECK(project.dataDb->moProject == &project);
    auto io = doe::makeRecordingStudyIo();
    io->bindModelManager(manager);
    CHECK(io->modelManager()->currentModelId() == -1); // project-read phase sees no current
    session.commitProjectModelSelection();
    CHECK(io->modelManager()->currentModelId() == 1);
    CHECK(manager->currentModel() == project.dataDb.get());
    expectUnsupported([&] { manager->emitModelSignal(*project.dataDb, "2signalModelUpdated()"); });
}

TEST_CASE(outer_loader_candidate_mo_projection_uses_kind_and_follows_signal_registration)
{
    Project unrelatedProject;
    engine::DbHandle base(engine::DbHandle::Kind::Base);
    base.moProject = &unrelatedProject;
    CHECK(engine::detail::outerLoaderMoChild(nullptr) == nullptr);
    CHECK(engine::detail::outerLoaderMoChild(&base) == nullptr);

    engine::DbHandle mo(engine::DbHandle::Kind::Mo);
    CHECK(mo.moProject == nullptr && !mo.loaded && !mo.pathSet);
    CHECK(engine::detail::outerLoaderMoChild(&mo) == &mo);

    ShellDriver driver;
    Session session(driver);
    session.prepareProjectModel(qt::L("C:/PROBLEM/ModelOpen/unread.DB"));
    auto& project = session.project();
    REQUIRE(project.dataDb);
    CHECK(project.dataDb->kind == engine::DbHandle::Kind::Mo);
    CHECK(project.dataDb->moProject == nullptr);
    REQUIRE(session.modelManagerOwner()->connections().size() == 2);
    CHECK(session.modelManagerOwner()->connections()[0].sender.lock() == project.dataDb);
    CHECK(session.modelManagerOwner()->currentModel() == nullptr);
    // initProject and the construction/backlink commit occur only after prepare.
    session.commitProjectModelConstruction();
    CHECK(project.dataDb->moProject == &project);
}

TEST_CASE(session_model_selection_resolves_pointer_ordinal_and_clears_project_backlink_after_release)
{
    ShellDriver driver;
    Session session(driver);
    session.prepareProjectModel(qt::L("C:/PROBLEM/ModelOpen/recovery.DB"));
    initProject(session.project(), qt::L("C:/PROBLEM/ModelOpen"), qt::L("recovery"));
    session.commitProjectModelConstruction();
    auto model = session.project().dataDb;
    auto manager = session.modelManagerOwner();
    model->modelId = 73; // declared fixture proves ID must not be used as ordinal
    session.commitProjectModelSelection();
    CHECK(manager->currentModelId() == 73);
    session.releaseProjectAfterCloseEffects(); // qualified storage release, not normal Close replay
    CHECK(model->moProject == nullptr);
    CHECK(manager->currentModel() == model.get()); // outer owner survives Project storage release
    CHECK(manager->size() == 1);
}

TEST_CASE(session_open_model_binding_rejects_replacement_and_selection_before_project_construction)
{
    ShellDriver driver;
    Session session(driver);
    expectUnsupported([&] { session.commitProjectModelSelection(); });
    session.prepareProjectModel(qt::L("C:/PROBLEM/ModelOpen/first.DB"));
    auto model = session.project().dataDb;
    expectUnsupported([&] { session.commitProjectModelSelection(); });
    expectUnsupported([&] { session.prepareProjectModel(qt::L("C:/PROBLEM/ModelOpen/second.DB")); });
    CHECK(session.project().dataDb == model);
    CHECK(model->path == qt::L("C:/PROBLEM/ModelOpen/first.DB"));
    CHECK(session.modelManagerOwner()->size() == 1);
}

TEST_CASE(current_subproject_resolver_uses_live_manager_and_actual_child_ownership)
{
    ShellDriver driver;
    Session session(driver);
    auto manager = session.modelManagerOwner();
    CHECK(!engine::currentSubProjectDbOwner(*manager));
    session.prepareProjectModel(qt::L("C:/PROBLEM/ModelOwner/recovery.DB"));
    initProject(session.project(), qt::L("C:/PROBLEM/ModelOwner"), qt::L("recovery"));
    session.commitProjectModelConstruction();
    CHECK(!engine::currentSubProjectDbOwner(*manager)); // registered but not yet current
    session.commitProjectModelSelection();
    auto& project = session.project();
    CHECK(engine::currentSubProjectDbOwner(*manager) == project.dataDb);
    project.subs.push_back(makeSubProject(project, 4, 1));
    project.activeSub = 1;
    expectUnsupported([&] { engine::currentSubProjectDbOwner(*manager); });
    CHECK(!project.subs[1]->ownDb); // no invented type4 allocation
    project.subs[1]->ownDb = std::make_shared<engine::DbHandle>();
    CHECK(engine::currentSubProjectDbOwner(*manager) == project.subs[1]->ownDb);
    project.activeSub = 0;
    CHECK(engine::currentSubProjectDbOwner(*manager) == project.dataDb);
    project.activeSub = 99;
    CHECK(!engine::currentSubProjectDbOwner(*manager));
    project.activeSub = 0;
    auto base = std::make_shared<engine::DbHandle>();
    manager->appendCreatedModel(base);
    manager->selectModel(base.get());
    CHECK(!engine::currentSubProjectDbOwner(*manager)); // failed native Mo cast
    manager->selectModel(project.dataDb.get());
    CHECK(engine::currentSubProjectDbOwner(*manager) == project.dataDb);
}

TEST_CASE(current_project_resolver_returns_owner_before_active_subproject_resolution)
{
    ShellDriver driver;
    Session session(driver);
    auto manager = session.modelManagerOwner();
    CHECK(engine::currentProjectForManager(*manager) == nullptr);
    session.prepareProjectModel(qt::L("C:/PROBLEM/ModelOwner/recovery.DB"));
    initProject(session.project(), qt::L("C:/PROBLEM/ModelOwner"), qt::L("recovery"));
    session.commitProjectModelConstruction();
    CHECK(engine::currentProjectForManager(*manager) == nullptr); // registered, not selected
    session.commitProjectModelSelection();
    auto& project = session.project();
    project.activeSub = 99;
    CHECK(engine::currentProjectForManager(*manager) == &project);
    CHECK(!engine::currentSubProjectDbOwner(*manager));

    auto base = std::make_shared<engine::DbHandle>();
    manager->appendCreatedModel(base);
    manager->selectModel(base.get());
    CHECK(engine::currentProjectForManager(*manager) == nullptr); // failed Mo projection
    manager->selectModel(project.dataDb.get());
    CHECK(engine::currentProjectForManager(*manager) == &project);
}

TEST_CASE(current_project_resolver_guards_missing_project_and_inconsistent_backlink)
{
    ShellDriver driver;
    Session session(driver);
    auto manager = session.modelManagerOwner();
    session.prepareProjectModel(qt::L("C:/PROBLEM/ModelOwner/recovery.DB"));
    initProject(session.project(), qt::L("C:/PROBLEM/ModelOwner"), qt::L("recovery"));
    session.commitProjectModelConstruction();
    session.commitProjectModelSelection();
    auto& project = session.project();
    auto model = project.dataDb;
    project.dataDb = std::make_shared<engine::DbHandle>();
    expectUnsupported([&] { engine::currentProjectForManager(*manager); });
    expectUnsupported([&] { engine::currentSubProjectDbOwner(*manager); });
    project.dataDb = model;
    model->moProject = nullptr;
    expectUnsupported([&] { engine::currentProjectForManager(*manager); });
    expectUnsupported([&] { engine::currentSubProjectDbOwner(*manager); });
    CHECK(model->moProject == nullptr); // no invented lazy allocation or null success
    model->moProject = &project;
    CHECK(engine::currentProjectForManager(*manager) == &project);
}

TEST_CASE(current_subproject_resolver_guards_missing_project_after_storage_release)
{
    ShellDriver driver;
    Session session(driver);
    session.prepareProjectModel(qt::L("C:/PROBLEM/ModelOwner/recovery.DB"));
    initProject(session.project(), qt::L("C:/PROBLEM/ModelOwner"), qt::L("recovery"));
    session.commitProjectModelConstruction();
    session.commitProjectModelSelection();
    auto manager = session.modelManagerOwner();
    session.releaseProjectAfterCloseEffects();
    expectUnsupported([&] { engine::currentSubProjectDbOwner(*manager); });
    CHECK(manager->currentModel()->moProject == nullptr);
}

SubProject& addSub(Project& p, int operationId)
{
    auto sub = std::make_unique<SubProject>();
    sub->project = &p;
    sub->id = static_cast<int>(p.subs.size());
    sub->loaded = true;
    auto item = std::make_unique<FlowItem>();
    item->opId = operationId;
    sub->flow.items.push_back(std::move(item));
    sub->flow.activeItem = 0;
    p.subs.push_back(std::move(sub));
    return *p.subs.back();
}

// Install actual GuiModels, held only by their Operations. The deleter observes
// Display lifetime at the exact destructor boundary, then runs the real dtor.
void showTwoModels(Session& session, std::weak_ptr<engine::Display>& display,
                   std::vector<bool>& displayAliveAtModelDelete)
{
    auto& p = session.project();
    engine::GuiModel* first = nullptr;
    for (int index = 0; index < 2; ++index) {
        auto operation = std::make_unique<Operation>();
        operation->project = &p;
        operation->engineModel = std::shared_ptr<engine::GuiModel>(
            new engine::GuiModel(nullptr, 3), [&](engine::GuiModel* model) {
                displayAliveAtModelDelete.push_back(!display.expired());
                delete model;
            });
        auto* model = operation->engineModel.get();
        if (!first) first = model;
        p.operations.push_back(std::move(operation));
        engine::displayShow(p, model);
    }
    display = p.display;
    REQUIRE(first != nullptr);
    CHECK(first->display == p.display.get()); // old main backlink still exists
}
}

TEST_CASE(session_storage_keeps_actual_display_alive_for_both_operation_model_destructors)
{
    std::weak_ptr<engine::Display> display;
    std::vector<bool> alive;
    auto* previousProject = currentProject();
    {
        ShellDriver driver;
        Session session(driver);
        CHECK(currentProject() == &session.project());
        showTwoModels(session, display, alive);
        session.commitNullDiagramBinding();
        session.releaseProjectAfterCloseEffects(); // shell storage only, no Close parity claim
        CHECK(session.liveProject() == nullptr);
        CHECK(currentProject() == previousProject);
        CHECK(!display.expired());
        CHECK(session.displayIfCreated() != nullptr);
        CHECK(alive == std::vector<bool>({true, true}));
    }
    CHECK(display.expired());
    CHECK(currentProject() == previousProject);
}

TEST_CASE(session_exception_cleanup_retains_display_through_real_model_teardown)
{
    std::weak_ptr<engine::Display> display;
    std::vector<bool> alive;
    auto* previousProject = currentProject();
    bool caught = false;
    try {
        ShellDriver driver;
        Session session(driver);
        showTwoModels(session, display, alive);
        throw std::runtime_error("session-test-expected");
    } catch (const std::runtime_error& error) {
        caught = std::string(error.what()) == "session-test-expected";
    }
    CHECK(caught);
    CHECK(alive == std::vector<bool>({true, true}));
    CHECK(display.expired());
    CHECK(currentProject() == previousProject);
}

TEST_CASE(session_shell_null_bound_delivery_fits_after_project_release_without_selection)
{
    ShellDriver driver;
    Session session(driver);
    auto& sub = addSub(session.project(), 11);
    const auto first = session.commitNonNullDiagramBinding(sub);
    const auto second = session.commitNonNullDiagramBinding(sub);
    CHECK(first != second);
    session.commitNullDiagramBinding();
    session.releaseProjectAfterCloseEffects();
    REQUIRE(session.pending().size() == 2u);
    driver.duringFit = [](Session& current) { CHECK(current.liveProject() == nullptr); };
    engine::FileEffects effects;
    const engine::PumpPoint point{"test:post-close-observed-pump", 0};
    const auto result = session.dispatchAt(point, due(point, {first, second}), effects);
    REQUIRE(result.size() == 2u);
    CHECK(result[0].second == Events::Delivery::Delivered);
    CHECK(result[1].second == Events::Delivery::Delivered);
    CHECK(session.pending().empty());
    CHECK(driver.order == std::vector<std::string>({"fit-boundary", "fit-boundary"}));
}

TEST_CASE(session_shell_consumes_before_fit_and_reads_rebound_model_after_fit)
{
    ShellDriver driver;
    Session session(driver);
    auto& oldSub = addSub(session.project(), 11);
    auto& newSub = addSub(session.project(), 22);
    const auto first = session.commitNonNullDiagramBinding(oldSub);
    const auto second = session.commitNonNullDiagramBinding(oldSub);
    engine::FileEffects effects;
    const engine::PumpPoint point{"test:outer-pump", 0};
    Events::TimerId rebound = 0;
    driver.duringFit = [&](Session& current) {
        const auto nested = current.dispatchAt(point, due(point, {first}), effects);
        REQUIRE(nested.size() == 1u);
        CHECK(nested[0].second == Events::Delivery::NotPending);
        current.project().activeSub = 1;
        rebound = current.commitNonNullDiagramBinding(newSub);
        newSub.flow.items[0]->opId = 23;
    };
    session.dispatchAt(point, due(point, {first}), effects);
    CHECK(driver.order == std::vector<std::string>({"fit-boundary", "click:23"}));
    REQUIRE(session.pending().size() == 2u);
    CHECK_EQ(session.pending()[0].id, second);
    CHECK_EQ(session.pending()[1].id, rebound);
}

TEST_CASE(session_missing_observation_does_not_consume_but_missing_fit_consumes_only_due_timer)
{
    engine::DiagramEffectDriver unavailable;
    Session session(unavailable);
    auto& sub = addSub(session.project(), 11);
    const auto first = session.commitNonNullDiagramBinding(sub);
    const auto second = session.commitNonNullDiagramBinding(sub);
    engine::FileEffects effects;
    const engine::PumpPoint point{"140098129", 0};
    expectUnsupported([&] { session.dispatchAt(point, std::nullopt, effects); });
    CHECK_EQ(session.pending().size(), 2u);
    auto wrong = due(point, {first});
    wrong.point.occurrence = 1;
    expectUnsupported([&] { session.dispatchAt(point, wrong, effects); });
    CHECK_EQ(session.pending().size(), 2u);
    CHECK(session.dispatchAt(point, due(point, {}), effects).empty());
    CHECK_EQ(session.pending().size(), 2u);
    expectUnsupported([&] { session.dispatchAt(point, due(point, {first}), effects); });
    REQUIRE(session.pending().size() == 1u);
    CHECK_EQ(session.pending()[0].id, second);
    const auto replay = session.dispatchAt(point, due(point, {first}), effects);
    REQUIRE(replay.size() == 1u);
    CHECK(replay[0].second == Events::Delivery::NotPending);
}

TEST_CASE(session_requires_null_binding_before_release_and_guards_missing_click_after_fit)
{
    struct FitOnly : engine::DiagramEffectDriver {
        bool reached = false;
        void fitAndNotify(Session&) override { reached = true; }
    } driver;
    Session session(driver);
    auto& sub = addSub(session.project(), 11);
    const auto timer = session.commitNonNullDiagramBinding(sub);
    expectUnsupported([&] { session.releaseProjectAfterCloseEffects(); });
    CHECK(session.liveProject() != nullptr);
    REQUIRE(session.pending().size() == 1u);
    engine::FileEffects effects;
    const engine::PumpPoint point{"test:missing-click", 0};
    expectUnsupported([&] { session.dispatchAt(point, due(point, {timer}), effects); });
    CHECK(driver.reached);
    CHECK(session.pending().empty());
}

TEST_CASE(session_real_boolean_leaf_creates_one_cutter_and_refills_between_two_deliveries)
{
    for (bool twoDimensional : {true, false}) {
        ShellDriver driver; // fit/notifications are explicitly an isolated driver fixture
        Session session(driver);
        auto& p = session.project();
        const auto dir = m2m_test::scratchOutput(m2m_test::scratchRoot() /
            ("m2m_session_" + std::to_string(_getpid())) / (twoDimensional ? "2d" : "3d"));
        std::filesystem::create_directories(dir);
        initProject(p, String(dir.u16string()), qt::L("Boolean"));
        makeRegistry(p);
        mst::installLoaderSlots();
        auto* op = createOperation(p, qt::L("Boolean"),
            twoDimensional ? qt::L("SfOprBoolean2D") : qt::L("SfOprBoolean3D"), {}, 1);
        REQUIRE(op != nullptr);
        op->taskDir = qt::L("Operations/Task00001");
        opBase(*op)->current = op;
        op->engineModel = std::make_shared<engine::GuiModel>(nullptr, twoDimensional ? 2 : 3);
        auto* model = op->engineModel.get();
        op->objIds = {1, 5};
        for (int index = 0; index < 2; ++index) {
            auto entry = std::make_unique<ObjEntry>();
            entry->number = index + 1;
            op->objs.push_back(std::move(entry));
            auto item = std::make_unique<Item>();
            item->kind = ItemKind::Object;
            item->id = index ? 5 : 1;
            p.objects.rows.push_back(std::move(item));
            auto* object = model->objectWrap(index + 1, false, true);
            REQUIRE(object != nullptr);
            auto* bytes = reinterpret_cast<unsigned char*>(object);
            *reinterpret_cast<std::int32_t*>(bytes + 0xc) = index ? 0 : 2;
            std::strcpy(reinterpret_cast<char*>(bytes + 0x828), index ? "ExistingCutter" : "Workpiece");
        }
        auto* sub = activeSubProject(p);
        REQUIRE(sub != nullptr);
        sub->flow.items.clear();
        auto item = std::make_unique<FlowItem>();
        item->opId = op->id;
        sub->flow.items.push_back(std::move(item));
        sub->flow.activeItem = 0;
        engine::FileEffects effects;
        REQUIRE(opBase(*op)->objectTreeFill(p, *op, effects));
        auto* props = dynamic_cast<BooleanProps*>(op->props.get());
        REQUIRE(props != nullptr && props->data != nullptr);
        auto* initialStep = model->step.get();
        driver.duringClick = [](Session& current, const FlowItem& selected, engine::FileEffects& fx) {
            current.applyBooleanSelection(selected, fx); // actual replacement/refill, not a counter
        };
        const auto first = session.commitNonNullDiagramBinding(*sub);
        const auto second = session.commitNonNullDiagramBinding(*sub);
        const engine::PumpPoint point{"test:boolean-leaf", 0};
        session.dispatchAt(point, due(point, {first}), effects);
        CHECK_EQ(model->objectCount(), 3);
        CHECK_EQ(props->data->cutter, 3);
        session.dispatchAt(point, due(point, {second}), effects);
        CHECK_EQ(model->objectCount(), 3);
        CHECK_EQ(props->data->cutter, 3);
        CHECK(model->step.get() == initialStep);
        CHECK_EQ(model->step->u1b4c, 0);
        REQUIRE(p.objects.find(2) != nullptr);
        CHECK(p.objects.find(2)->name == qt::L("Cutter"));
        CHECK_EQ(objId(*op, 2), 2);
        const auto* original = reinterpret_cast<const char*>(model->object(2));
        CHECK(std::strcmp(original + 0x828, "ExistingCutter") == 0);
    }
}

namespace {
struct OpenScratch {
    std::filesystem::path directory, project;
    qt::CurrentDirScope keepDir{qt::currentPath()};
    OpenScratch() {
        static unsigned sequence = 0;
        directory = m2m_test::scratchOutput(m2m_test::scratchRoot() /
            ("session-open-" + std::to_string(_getpid()) + "-" + std::to_string(++sequence)));
        std::filesystem::create_directories(directory);
        project = directory / "project.moproj";
        std::ofstream(project, std::ios::binary) << "<Project><Unit Name=\"SI\"/></Project>";
    }
    void remesh(const std::string& bytes) {
        std::ofstream(directory / "LOC_DEN_CTRL.DAT", std::ios::binary) << bytes;
    }
    bool open(Session& session) {
        auto& p = session.project();
        initProject(p, qt::String(directory.u16string()), qt::L("project"));
        Ctx ctx; ctx.session = &session;
        return openProject(p, qt::String(project.u16string()), ctx);
    }
};
}
TEST_CASE(session_actual_open_loads_one_remesh_owner_that_survives_project_storage_release)
{
    OpenScratch disk; disk.remesh("FILTER_DISTANCE 7\nDONE\n");
    engine::DiagramEffectDriver driver; Session session(driver);
    auto* owner = &session.remeshState();
    CHECK(disk.open(session));
    CHECK(session.project().openedOk);
    CHECK(owner->enabled && owner->filterDistance == 7.0);
    session.releaseProjectAfterCloseEffects(); // storage boundary only; no native Close claim
    CHECK(&session.remeshState() == owner);
    CHECK(owner->enabled && owner->filterDistance == 7.0);
}
TEST_CASE(session_actual_open_keeps_remesh_partial_state_before_unsupported_chunk)
{
    OpenScratch disk; disk.remesh("INTERNAL_ELEMENT_SIZE_CONTROL_METHOD 4\n\x80\n");
    engine::DiagramEffectDriver driver; Session session(driver);
    expectUnsupported([&] { (void)disk.open(session); });
    CHECK(session.project().openedOk); // XML result precedes remesh load
    CHECK_EQ(session.remeshState().method, 4);
    CHECK(!session.remeshState().enabled); // no completed return to enabled setter
    auto result = convert(disk.project.u16string(), (disk.directory / "out.MST").u16string(),
                           M2M_FLAG_WRITE_SAVE, 1252);
    CHECK_EQ(result.code, M2M_E_UNSUPPORTED); // public driver installed same Open binding
    CHECK(!std::filesystem::exists(disk.directory / "out.MST"));
}
TEST_CASE(session_open_accepts_failed_xml_read_and_keeps_registered_model_until_save_decision)
{
    OpenScratch disk;
    std::ofstream(disk.project, std::ios::binary | std::ios::trunc) << "<Project><unclosed>";
    disk.remesh("FILTER_DISTANCE 9\nDONE\n");
    engine::DiagramEffectDriver driver; Session session(driver);
    auto& p = session.project();
    session.prepareProjectModel(qt::String((disk.directory / "project.DB").u16string()));
    initProject(p, qt::String(disk.directory.u16string()), qt::L("project"));
    session.commitProjectModelConstruction();
    Ctx ctx; ctx.session = &session;
    CHECK(openProject(p, qt::String(disk.project.u16string()), ctx));
    CHECK(!p.openedOk);
    CHECK_EQ(p.f39, 1);
    CHECK_EQ(p.f3a, 1);
    CHECK(session.remeshState().enabled && session.remeshState().filterDistance == 9.0);
    CHECK(qt::currentPath() == qt::cleanPath(qt::String(disk.directory.u16string())));
    CHECK(session.modelManagerOwner()->currentModel() == nullptr);
    session.commitProjectModelSelection();
    CHECK(session.modelManagerOwner()->currentModel() == p.dataDb.get());
    CHECK(p.dataDb->moProject == &p);
    activateSubProject(p, 0, ctx);
    CHECK(activeSubProject(p)->loaded);
    engine::SaveEffects saved;
    CHECK(!engine::saveCurrent(p, ctx, saved));
    CHECK(!saved.call.opened && !saved.call.validationComplete);
    CHECK(saved.current == nullptr && !saved.call.projectWriteAttempted);
    CHECK(p.dataDb->moProject == &p); // rejected Save is not outer Open failure cleanup
}
TEST_CASE(session_open_missing_file_returns_accepted_but_rejected_suffix_skips_tail)
{
    OpenScratch disk; disk.remesh("FILTER_DISTANCE 11\nDONE\n");
    engine::DiagramEffectDriver driver; Session session(driver);
    auto& p = session.project();
    initProject(p, qt::String(disk.directory.u16string()), qt::L("absent"));
    Ctx ctx; ctx.session = &session;
    const auto missing = qt::cleanPath(qt::String((disk.directory / "absent.moproj").u16string()));
    CHECK(openProject(p, missing, ctx));
    CHECK(!p.openedOk && p.path == missing);
    CHECK(session.remeshState().enabled && session.remeshState().filterDistance == 11.0);
    p.f39 = 0; p.f3a = 0;
    session.remeshState().filterDistance = 17.0;
    CHECK(!openProject(p, qt::String((disk.directory / "invalid.txt").u16string()), ctx));
    CHECK(p.path == missing);
    CHECK_EQ(p.f39, 0);
    CHECK_EQ(p.f3a, 0);
    CHECK_EQ(session.remeshState().filterDistance, 17.0);
}
TEST_CASE(session_forming_factory_retains_actual_parentless_owner_before_ui_setup_guard)
{
    OpenScratch disk;
    engine::DiagramEffectDriver driver; Session session(driver);
    auto& p = session.project();
    session.prepareProjectModel(qt::String((disk.directory / "project.DB").u16string()));
    initProject(p, qt::String(disk.directory.u16string()), qt::L("project"));
    session.commitProjectModelConstruction();
    session.commitProjectModelSelection();
    p.doeIo = doe::makeRecordingStudyIo();
    p.doeIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
    p.doeIo->bindModelManager(session.modelManagerOwner());
    p.originalSettings.moInputMode = 1;
    makeRegistry(p);
    mst::installLoaderSlots();
    auto* op = createOperation(p, qt::L("Forming"), qt::L("SfOprForming"), {}, 1);
    REQUIRE(op != nullptr);
    op->engineModel = std::make_shared<engine::GuiModel>(nullptr, 3);
    auto& model = *op->engineModel;
    auto* sc = model.simCtl(); REQUIRE(sc != nullptr);
    sc->solmtd = 2; sc->u2e8 = 4;
    for (auto& v : sc->solmtd_p) v = 0.0;
    opBase(*op)->current = op;
    engine::FileEffects effects;
    bool guarded = false;
    try { opBase(*op)->objectTreeFill(p, *op, effects); }
    catch (const NotPorted& e) { guarded = e.src == 0x6504abb0; }
    CHECK(guarded);
    auto* attempt = session.editor300FactoryAttempt(); REQUIRE(attempt != nullptr);
    CHECK(attempt->identityMatches({p, *op, model, session.remeshState()}));
    CHECK(attempt->construction().state == engine::simcontrol300::Editor300ConstructorState::AwaitingContext);
    CHECK(attempt->partialPageState() == nullptr);
    CHECK(session.editor300SetupProjection() == nullptr);
    CHECK(attempt->setupOwner() == nullptr);
    CHECK(attempt->nativeSetupPrefix() == nullptr);
    const auto* base = attempt->nativeBaseConstruction(); REQUIRE(base != nullptr);
    CHECK(base->report().attempted);
    CHECK(base->report().stage == engine::simcontrol300::EditorBaseStage::PrivatePalette);
    CHECK(base->report().issue == engine::simcontrol300::EditorBaseIssue::PrivatePaletteUnavailable);
    CHECK(!base->report().qwidgetReturned && !base->report().applicationWrappersReturned);
    const auto widget = attempt->widgetState(); REQUIRE(widget != nullptr);
    CHECK(widget->nativeOwnerAlive());
    CHECK(widget->runtimeOwner() == session.widgetRuntimeOwner());
    REQUIRE(widget->objectName().has_value());
    CHECK(widget->objectName()->isEmpty());
    CHECK(!widget->threadState());
    CHECK(!widget->palette() && !widget->font());
    CHECK(!session.widgetRuntimeOwner()->hasLiveRegistration(attempt));
    for (double v : sc->solmtd_p) CHECK_EQ(v, 0.0); // later CG fill was not moved before setup
    CHECK(session.modelManagerOwner()->ownerSession() == &session);
    expectUnsupported([&] { opBase(*op)->objectTreeFill(p, *op, effects); });
    CHECK(session.editor300FactoryAttempt() == attempt); // no replacement or replay
    CHECK(session.editor300SetupProjection() == nullptr);
}

TEST_CASE(session_editor_factory_rejects_unselected_or_substituted_owner_before_allocation)
{
    OpenScratch disk;
    engine::DiagramEffectDriver driver; Session session(driver);
    auto& p = session.project();
    session.prepareProjectModel(qt::String((disk.directory / "project.DB").u16string()));
    initProject(p, qt::String(disk.directory.u16string()), qt::L("project"));
    session.commitProjectModelConstruction();
    Operation op; op.project = &p;
    op.engineModel = std::make_shared<engine::GuiModel>(nullptr, 3);
    expectUnsupported([&] { session.beginEditor300Factory(p, op, *op.engineModel); });
    CHECK(session.editor300FactoryAttempt() == nullptr);
    CHECK(session.editor300SetupProjection() == nullptr);
    session.commitProjectModelSelection();
    engine::GuiModel different(nullptr, 3);
    expectUnsupported([&] { session.beginEditor300Factory(p, op, different); });
    CHECK(session.editor300FactoryAttempt() == nullptr);
}

TEST_CASE(session_notification_owner_survives_project_and_study_io_release)
{
    std::weak_ptr<doe::SessionNotificationState> retained;
    {
        engine::DiagramEffectDriver driver; Session session(driver);
        auto& p = session.project();
        p.doeIo = doe::makeRecordingStudyIo();
        p.doeIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
        std::weak_ptr<doe::StudyIo> io = p.doeIo;
        retained = session.sessionNotificationStateOwner();
        auto& state = p.doeIo->sessionNotificationState();
        CHECK(&state == session.sessionNotificationStateOwner().get());
        state.flag90 = true;
        state.emptyKeys.insert("2signalChangeStepRange()");
        session.releaseProjectAfterCloseEffects(); // storage release, not completed native Close
        CHECK(io.expired());
        REQUIRE(!retained.expired());
        CHECK(retained.lock()->flag90);
        CHECK(retained.lock()->emptyKeys.count("2signalChangeStepRange()") == 1);
    }
    CHECK(retained.expired());
}

TEST_CASE(session_task_allocation_owner_survives_project_release_but_io_does_not_own_it)
{
    std::weak_ptr<doe::TaskAllocationStore> observed;
    std::shared_ptr<doe::StudyIo> escapedIo;
    {
        engine::DiagramEffectDriver driver; Session session(driver);
        auto& p = session.project();
        p.doeIo = doe::makeRecordingStudyIo();
        escapedIo = p.doeIo;
        p.doeIo->bindTaskAllocationStore(session.taskAllocationStoreOwner());
        observed = session.taskAllocationStoreOwner();
        CHECK(p.doeIo->taskAllocationStoreOwner() == session.taskAllocationStoreOwner());
        session.releaseProjectAfterCloseEffects(); // adapter storage release only
        CHECK(!observed.expired());
        CHECK(escapedIo->taskAllocationStoreOwner() == session.taskAllocationStoreOwner());
    }
    CHECK(observed.expired());
    CHECK(escapedIo->taskAllocationStoreOwner() == nullptr);
    auto replacement = std::make_shared<doe::TaskAllocationStore>();
    expectUnsupported([&] { escapedIo->bindTaskAllocationStore(replacement); });
    CHECK(escapedIo->taskAllocationStoreOwner() == nullptr);
}

TEST_CASE(session_task_allocation_binding_is_idempotent_and_rejects_foreign_ownership)
{
    engine::DiagramEffectDriver driver; Session session(driver);
    auto io = doe::makeRecordingStudyIo();
    const auto owner = session.taskAllocationStoreOwner();
    REQUIRE(owner != nullptr);
    expectUnsupported([&] { io->bindTaskAllocationStore({}); });
    io->bindTaskAllocationStore(owner);
    io->bindTaskAllocationStore(owner);
    auto replacement = std::make_shared<doe::TaskAllocationStore>();
    expectUnsupported([&] { io->bindTaskAllocationStore(replacement); });
    // Same raw address with a separate control block is still a foreign owner.
    auto alias = std::shared_ptr<doe::TaskAllocationStore>(owner.get(), [](auto*) {});
    expectUnsupported([&] { io->bindTaskAllocationStore(alias); });
    CHECK(io->taskAllocationStoreOwner() == owner);
}

TEST_CASE(session_native_task_factory_retains_report_prefix_through_project_release)
{
    std::shared_ptr<doe::TaskFactoryRuntime> escapedFactory;
    std::shared_ptr<doe::TaskAllocationStore> escapedStore;
    std::shared_ptr<doe::StudyIo> escapedIo;
    {
        engine::DiagramEffectDriver driver; Session session(driver);
        escapedFactory = session.taskFactoryRuntimeOwner();
        escapedStore = session.taskAllocationStoreOwner();
        REQUIRE(escapedFactory && escapedStore);
        CHECK(escapedFactory->profile() == doe::TaskFactoryProfile::Native);
        CHECK(escapedFactory->widgetRuntimeOwner() == session.widgetRuntimeOwner());
        escapedIo = doe::makeRecordingStudyIo();
        escapedIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
        escapedIo->bindTaskAllocationStore(escapedStore);
        escapedIo->bindTaskFactoryRuntime(escapedFactory);
        session.project().doeIo = escapedIo;
        expectUnsupported([&] { doe::dispatchCreateTaskBase(*escapedIo, qt::L("SfTaskDoe")); });
        const auto state = escapedFactory->construction();
        CHECK(state.factoryCommitted && state.dataCommitted && state.dataFactoryBacklinkMatches);
        REQUIRE(state.pendingReportAllocation.has_value());
        CHECK_EQ(*state.pendingReportAllocation, 0u);
        CHECK_EQ(escapedStore->retainedPrototypeCount(), 1u);
        CHECK_EQ(escapedStore->retainedTaskCount(), 0u);
        CHECK(!escapedStore->retainedPrototypeAt(0)->qobjectBaseCompleted);
        session.releaseProjectAfterCloseEffects();
        CHECK(escapedFactory->nativeReceiverAlive());
        CHECK_EQ(escapedStore->retainedPrototypeCount(), 1u);
        CHECK(escapedIo->taskFactoryRuntimeOwner() == escapedFactory);
    }
    CHECK(!escapedFactory->nativeReceiverAlive());
    CHECK_EQ(escapedStore->retainedPrototypeCount(), 0u);
    CHECK(escapedIo->taskFactoryRuntimeOwner() == escapedFactory); // weak binding still sees escaped lease
    expectUnsupported([&] { escapedFactory->createTaskBase(qt::L("SfTaskDoe")); });
}

TEST_CASE(session_native_factory_binding_rejects_same_address_foreign_control_block)
{
    engine::DiagramEffectDriver driver; Session session(driver);
    auto io = doe::makeRecordingStudyIo();
    const auto owner = session.taskFactoryRuntimeOwner();
    REQUIRE(owner != nullptr);
    io->bindTaskFactoryRuntime(owner);
    auto foreign = std::shared_ptr<doe::TaskFactoryRuntime>(owner.get(), [](auto*) {});
    expectUnsupported([&] { io->bindTaskFactoryRuntime(foreign); });
    CHECK(io->taskFactoryRuntimeOwner() == owner);
}

TEST_CASE(session_task_observers_expire_at_adapter_end_even_with_an_escaped_store_lease)
{
    std::shared_ptr<doe::TaskAllocationStore> escaped;
    doe::TaskObserver task;
    {
        engine::DiagramEffectDriver driver; Session session(driver);
        escaped = session.taskAllocationStoreOwner();
        task = escaped->createParsedDoeTask();
        REQUIRE(task != nullptr);
        session.releaseProjectAfterCloseEffects();
        CHECK(task != nullptr); // Type2 Project release does not own these Tasks
    }
    CHECK(task == nullptr);
    expectUnsupported([&] { escaped->createParsedDoeTask(); });
}

TEST_CASE(session_record_allocation_survives_document_and_project_storage_release)
{
    std::shared_ptr<doe::TaskAllocationStore> escaped;
    std::weak_ptr<doe::SessionRecordIdentity> identity;
    doe::TaskObserver task;
    {
        engine::DiagramEffectDriver driver; Session session(driver);
        escaped = session.taskAllocationStoreOwner();
        auto& record = escaped->createSessionRecord();
        identity = record.identity();
        task = escaped->createParsedDoeTask();
        doe::bindSessionTask(record, task);
        {
            doe::SessionDocument document;
            document.parts.emplace_back();
            document.parts.back().sessions.push_back(&record);
            CHECK(document.parts.back().sessions.front() == task->sessionBacklink());
        } // native Type2 Part/inner-list storage does not own these records
        CHECK(!identity.expired());
        CHECK(task->sessionBacklink() == &record);
        session.releaseProjectAfterCloseEffects(); // adapter boundary, not a native Close proof
        CHECK(task->sessionBacklink() == &record);
        CHECK_EQ(escaped->retainedSessionCount(), std::size_t{1});
        CHECK(escaped->retainedSessionAt(0) == &record);
    }
    CHECK(identity.expired());
    CHECK(task == nullptr);
    CHECK(!escaped->adapterScopeLive());
    expectUnsupported([&] { escaped->createSessionRecord(); });
}
TEST_CASE(fresh_session_does_not_reuse_previous_session_notification_state)
{
    engine::DiagramEffectDriver driver;
    Session first(driver);
    auto firstState = first.sessionNotificationStateOwner();
    firstState->suppressed = true; firstState->flag90 = true;
    firstState->emptyKeys.insert("2signalOpenSession()");
    first.releaseProjectAfterCloseEffects();
    Session next(driver);
    auto nextState = next.sessionNotificationStateOwner();
    CHECK(nextState != firstState);
    CHECK(!nextState->suppressed && !nextState->flag90);
    CHECK(nextState->emptyKeys.empty());
}
TEST_CASE(session_owns_actual_view_manager_constructor_prefix_before_project_use)
{
    engine::DiagramEffectDriver driver; Session session(driver);
    auto view = session.mainViewOwner(); REQUIRE(view != nullptr);
    auto manager = view->managerChild(); REQUIRE(manager != nullptr);
    CHECK(manager->nativeReceiverAlive());
    CHECK(manager->parentMainView() == view);
    auto grid = manager->cachedGridLayout(); REQUIRE(grid != nullptr);
    CHECK(grid == manager->installedWidgetLayout());
    CHECK(grid->parentViewManager() == manager && grid->nativeOwnerAlive());
    CHECK(manager->gridInstallStage() == engine::GridInstallStage::Cached);
    CHECK(grid->layoutFlags() == 3 && grid->dirtyFlags() == 4);
    CHECK(grid->cachedSizeValue() == -1 && grid->items().empty());
    CHECK((grid->geometryEndpoints() == std::array<int, 4>({0, 0, -1, -1})));
    CHECK(manager->ownedGridLayouts().size() == 1);
    CHECK(!manager->nativeLayoutResolved());
    CHECK(!view->privateState().manager());
    expectUnsupported([&] { session.currentViewManagerOwner(); });
    const auto& registrations = session.sessionNotificationStateOwner()->viewManagerRegistrations;
    REQUIRE(registrations.at("2signalGetSceneInViewManager()").size() == 1);
    CHECK(registrations.at("2signalGetSceneInViewManager()")[0].receiver.lock() == manager);
    session.releaseProjectAfterCloseEffects(); // Project release is not main-window release
    CHECK(manager->nativeReceiverAlive());
    CHECK(view->managerChild() == manager);
    CHECK(manager->cachedGridLayout() == grid && grid->nativeOwnerAlive());
}

TEST_CASE(session_teardown_invalidates_escaped_mainview_and_manager_owners)
{
    std::shared_ptr<engine::MainViewState> view;
    std::shared_ptr<engine::ViewManagerState> manager;
    std::shared_ptr<engine::GridLayoutState> grid;
    std::shared_ptr<doe::SessionNotificationState> notifications;
    {
        engine::DiagramEffectDriver driver; Session session(driver);
        view = session.mainViewOwner(); manager = view->managerChild();
        grid = manager->cachedGridLayout(); REQUIRE(grid != nullptr);
        notifications = session.sessionNotificationStateOwner();
    }
    CHECK(!view->nativeOwnerAlive());
    CHECK(!manager->nativeReceiverAlive());
    CHECK(!grid->nativeOwnerAlive() && !grid->parentViewManager());
    CHECK(!manager->cachedGridLayout() && !manager->installedWidgetLayout());
    CHECK(notifications->viewManagerRegistrations.at("2signalGetSceneInViewManager()").size() == 1);
}

TEST_CASE(session_primary_secondary_display_owners_predate_project_open_and_keep_unknown_geometry)
{
    engine::DiagramEffectDriver driver; Session session(driver);
    auto primary = session.primarySceneOwner(), secondary = session.secondarySceneOwner();
    REQUIRE(primary != nullptr && secondary != nullptr);
    CHECK(primary != secondary);
    auto first = primary->displayOwner(), second = secondary->displayOwner();
    REQUIRE(first != nullptr && second != nullptr);
    CHECK(first != second);
    auto& p = session.project();
    CHECK(p.display == first);
    CHECK(session.displayIfCreated() == first.get());
    CHECK(session.modelManagerOwner()->currentModel() == nullptr);
    CHECK(engine::displayMain(*first) == nullptr && engine::displayMain(*second) == nullptr);
    CHECK(!engine::displayGeometryProfile(*first).pixels);
    CHECK(!engine::displayGeometryProfile(*second).pixels);
    CHECK(primary->itemPopulationQualification().empty());
    CHECK(secondary->itemPopulationQualification().empty());
    // Existing historical camera adapter binds its own qualified geometry;
    // neither startup size nor secondary geometry is inferred from that profile.
    p.diagramViewHeight = 175;
    CHECK(&engine::projectDisplay(p) == first.get());
    const auto geometry = engine::displayGeometryProfile(*first);
    CHECK(geometry.source == engine::DisplayGeometryProfile::Source::HistoricalOracleB);
    REQUIRE(geometry.pixels.has_value());
    CHECK((*geometry.pixels)[0] == 3363 && (*geometry.pixels)[1] == 1657);
    CHECK(!engine::displayGeometryProfile(*second).pixels);
    session.releaseProjectAfterCloseEffects();
    CHECK(session.primarySceneOwner() == primary && session.secondarySceneOwner() == secondary);
    CHECK(session.displayIfCreated() == first.get());
}

TEST_CASE(session_scene_displays_release_after_project_models_and_are_fresh_per_session)
{
    std::weak_ptr<engine::Display> primary, secondary;
    engine::DiagramEffectDriver driver;
    {
        Session first(driver);
        primary = first.primarySceneOwner()->displayOwner();
        secondary = first.secondarySceneOwner()->displayOwner();
        first.releaseProjectAfterCloseEffects();
        CHECK(!primary.expired() && !secondary.expired());
        Session next(driver);
        CHECK(next.project().display != primary.lock());
        CHECK(next.secondarySceneOwner()->displayOwner() != secondary.lock());
        CHECK(!engine::displayGeometryProfile(*next.project().display).pixels);
    }
    CHECK(primary.expired() && secondary.expired());
}

TEST_CASE(session_default_splitview_retains_value_aliases_and_parent_effects_before_post)
{
    engine::DiagramEffectDriver driver; Session session(driver);
    const auto mainView = session.mainViewOwner();
    const auto manager = mainView->managerChild();
    const auto runtime = session.widgetRuntimeOwner();
    REQUIRE(runtime && manager);
    CHECK(runtime->allWidgetsRegistryPresent() == true && !runtime->populationComplete());
    const auto split = manager->pendingSplitView();
    REQUIRE(split != nullptr);
    const auto report = session.defaultSplitViewConstructionReport();
    REQUIRE(report != nullptr);
    CHECK(!report->completed && !report->issue.empty());
    CHECK(report->stage == engine::SplitViewConstructStage::WidgetPostPending);
    CHECK(split->constructionStage() == report->stage);
    CHECK_EQ(runtime->currentWidgetCount().value(), 174u);
    CHECK_EQ(runtime->maximumWidgetCount().value(), 174u);
    REQUIRE(runtime->threadState());
    CHECK_EQ(runtime->threadState()->notifyDepth().value(), 0u);
    CHECK(!runtime->threadState()->postedEvents().populationKnown());
    const auto* tail = split->constructorTail(); REQUIRE(tail != nullptr);
    const auto* create = tail->createEvent(); REQUIRE(create != nullptr);
    CHECK_EQ(create->rawType(), 0x0fu);
    CHECK(create->nativeAlive() && !create->posted());
    CHECK(split->widgetState()->nativeIdentity() == split.get());
    CHECK(runtime->hasLiveRegistration(split.get()));
    CHECK(split->widgetState()->widgetDataAssigned());
    CHECK(split->widgetState()->parentWidget() == mainView->widgetState());
    CHECK(split->widgetState()->parentOwnsChild());
    REQUIRE(mainView->ownedSplitViews().size() == 1);
    CHECK(mainView->ownedSplitViews()[0] == split);
    CHECK(split->widgetState()->focusNextKnown() && split->widgetState()->focusPreviousKnown());
    CHECK(split->widgetState()->focusNextIdentity() == mainView.get());
    CHECK(split->widgetState()->focusPreviousIdentity() == manager.get());
    CHECK(mainView->widgetState()->focusPreviousIdentity() == split.get());
    CHECK(manager->widgetState()->focusNextIdentity() == split.get());
    CHECK(!split->widgetState()->inSetParent());
    REQUIRE(runtime->applicationFont() && runtime->applicationPalette());
    REQUIRE(split->widgetState()->font() && split->widgetState()->palette());
    REQUIRE(mainView->widgetState()->font() && mainView->widgetState()->palette());
    CHECK(split->widgetState()->font()->data == mainView->widgetState()->font()->data);
    CHECK(split->widgetState()->font()->data == runtime->applicationFont()->data);
    CHECK_EQ(split->widgetState()->font()->resolveMask, 0u);
    CHECK_EQ(runtime->applicationFont()->resolveMask, 51u);
    CHECK(split->widgetState()->palette()->data == runtime->applicationPalette()->data);
    CHECK_EQ(split->widgetState()->palette()->groupResolveWord, 0u);
    CHECK_EQ(mainView->widgetState()->palette()->groupResolveWord, 0u);
    CHECK_EQ(runtime->applicationPalette()->groupResolveWord, 13893616u);
    REQUIRE(split->widgetState()->locale());
    CHECK_EQ(split->widgetState()->locale()->index, 391);
    CHECK_EQ(split->widgetState()->locale()->numberOptions, 0);
    CHECK(split->widgetState()->opaqueChildrenDirty() == true);
    CHECK(split->widgetState()->opaque() == false);
    const auto& pending = mainView->widgetState()->pendingChildren();
    REQUIRE(pending && pending->size() == 7u);
    for (std::size_t i = 0; i < 5; ++i) {
        CHECK_EQ((*pending)[i].opaqueAlias, i + 1);
        CHECK((*pending)[i].knownWidget.expired());
    }
    CHECK((*pending)[5].knownWidget.lock() == manager->widgetState());
    CHECK((*pending)[6].knownWidget.lock() == split->widgetState());
    CHECK(!split->orientation().has_value());
    CHECK(!manager->nativeLayoutResolved());
    expectUnsupported([&] { session.currentViewManagerOwner(); });
    session.releaseProjectAfterCloseEffects();
    CHECK(manager->pendingSplitView() == split);
    CHECK(runtime->hasLiveRegistration(split.get()));
}

TEST_CASE(session_teardown_invalidates_pending_widget_registration_even_with_diagnostic_leases)
{
    std::shared_ptr<engine::WidgetRuntimeState> runtime;
    std::shared_ptr<engine::SplitViewState> split;
    {
        engine::DiagramEffectDriver driver; Session session(driver);
        runtime = session.widgetRuntimeOwner();
        split = session.mainViewOwner()->managerChild()->pendingSplitView();
        REQUIRE(split != nullptr);
        CHECK(runtime->hasLiveRegistration(split.get()));
    }
    CHECK(!split->nativeOwnerAlive());
    CHECK(!runtime->hasLiveRegistration(split.get()));
    CHECK(!split->widgetState()->parentWidget());
}


namespace {
// Candidate-only admission probes. These counters do not implement native fit
// or assert any original timer readiness; accepted dispatch still hits a guard.
struct TraceFitGuard : engine::DiagramEffectDriver {
    unsigned fitAttempts = 0, clickAttempts = 0;
    bool requireReleasedProject = false;
    void fitAndNotify(Session& session) override {
        ++fitAttempts;
        if (requireReleasedProject) CHECK(session.liveProject() == nullptr);
        throw Error(M2M_E_UNSUPPORTED, "candidate-only probe: native fit is unavailable");
    }
    void clickAndUpdateMainView(Session&, const FlowItem&, engine::FileEffects&) override {
        ++clickAttempts;
        throw Error(M2M_E_UNSUPPORTED, "candidate-only probe: native click is unavailable");
    }
};
struct SessionTraceRequest {
    engine::BooleanTraceBindingContext request;
    engine::BooleanPumpTrace trace;
    explicit SessionTraceRequest(const Session& session) {
        REQUIRE(session.pending().size() == 2);
        request.expectedIdentity = {std::string(64, 'a'), std::string(64, 'b'), std::string(64, 'c'),
            std::string(64, 'd'), std::string(64, 'e'), "synthetic session admission", "synthetic session trace"};
        request.expectedPoint = {"test:typed-session-trace", 0}; request.expectedNativeThread = 88;
        for (std::size_t i = 0; i < 2; ++i) {
            const auto& pending = session.pending()[i];
            engine::BooleanTraceRegistration registration;
            registration.native = {0x1000, 1, 0x2000 + i * 0x100, 1, 1 + i};
            registration.nativeTimerId = 11 + static_cast<int>(i);
            registration.requestedIntervalMs = 1000; registration.nativeThread = 88;
            registration.logicalReceiver = pending.receiver; registration.logicalTimer = pending.id;
            registration.evidence = "synthetic registration contract, not native readiness";
            request.registrations.push_back(registration);
        }
        trace.identity = request.expectedIdentity; trace.point = request.expectedPoint;
        trace.nativeThread = 88; trace.enterOrdinal = 100; trace.returnOrdinal = 200;
        trace.complete = trace.balanced = trace.registrationsComplete = true;
        trace.evidence = "candidate-only complete flat trace contract; no original execution";
        for (std::size_t i = 0; i < 2; ++i) {
            const auto& registered = request.registrations[1 - i]; // deliberately opposite pending order
            engine::BooleanTraceDelivery row;
            row.native = registered.native; row.nativeTimerId = registered.nativeTimerId;
            row.nativeThread = 88; row.enterOrdinal = 110 + i * 30; row.returnOrdinal = 120 + i * 30;
            row.timerEventObserved = row.modelUpdatedObserved = row.matchedCallbackReceiver = row.balanced = true;
            row.evidence = "synthetic balanced timerEvent/modelUpdated contract";
            trace.deliveries.push_back(row);
        }
    }
};
}

TEST_CASE(session_typed_trace_rejects_colliding_foreign_owner_before_fit_or_consumption)
{
    TraceFitGuard driverA, driverB;
    Session a(driverA), b(driverB);
    auto& subA = addSub(a.project(), 11); auto& subB = addSub(b.project(), 22);
    const auto firstA = a.commitNonNullDiagramBinding(subA), secondA = a.commitNonNullDiagramBinding(subA);
    const auto firstB = b.commitNonNullDiagramBinding(subB), secondB = b.commitNonNullDiagramBinding(subB);
    CHECK(firstA == firstB && secondA == secondB);
    CHECK(a.pending()[0].receiver == b.pending()[0].receiver);
    SessionTraceRequest input(a);
    auto bound = a.bindBooleanTrace(input.request, input.trace); REQUIRE(bound.binding);
    CHECK(!input.request.logicalOwner && !input.request.ownerScope); // caller remains owner-neutral
    engine::FileEffects effects;
    expectUnsupported([&] { b.dispatchBooleanTraceAt(input.trace.point, *bound.binding, effects); });
    CHECK(driverA.fitAttempts == 0 && driverB.fitAttempts == 0);
    CHECK(driverA.clickAttempts == 0 && driverB.clickAttempts == 0);
    REQUIRE(a.pending().size() == 2 && b.pending().size() == 2);
    CHECK(a.pending()[0].id == firstA && a.pending()[1].id == secondA);
    CHECK(b.pending()[0].id == firstB && b.pending()[1].id == secondB);
    auto wrong = input.trace.point; ++wrong.occurrence;
    expectUnsupported([&] { a.dispatchBooleanTraceAt(wrong, *bound.binding, effects); });
    CHECK(a.pending().size() == 2 && driverA.fitAttempts == 0);
    expectUnsupported([&] { a.dispatchBooleanTraceAt(input.trace.point, *bound.binding, effects); });
    CHECK(driverA.fitAttempts == 1 && driverA.clickAttempts == 0);
    REQUIRE(a.pending().size() == 1);
    CHECK(a.pending()[0].id == firstA); // secondA was first in the observed trace order
    expectUnsupported([&] { a.dispatchBooleanTraceAt(input.trace.point, *bound.binding, effects); });
    CHECK(driverA.fitAttempts == 1 && a.pending().size() == 1 && b.pending().size() == 2);
}

TEST_CASE(session_typed_trace_rejects_owner_selection_incomplete_and_nested_input)
{
    TraceFitGuard driver; Session session(driver);
    auto& sub = addSub(session.project(), 11);
    session.commitNonNullDiagramBinding(sub); session.commitNonNullDiagramBinding(sub);
    SessionTraceRequest input(session);
    Events foreign; engine::BooleanTraceOwnerScope foreignScope(foreign);
    input.request.logicalOwner = &foreign;
    CHECK(!session.bindBooleanTrace(input.request, input.trace).binding);
    input.request.logicalOwner = nullptr; input.request.ownerScope = &foreignScope;
    CHECK(!session.bindBooleanTrace(input.request, input.trace).binding);
    input.request.ownerScope = nullptr;
    input.trace.complete = false;
    CHECK(!session.bindBooleanTrace(input.request, input.trace).binding);
    input.trace.complete = true;
    input.trace.deliveries[0].returnOrdinal = 170; // second frame nests: cannot flatten its effects
    CHECK(!session.bindBooleanTrace(input.request, input.trace).binding);
    CHECK(session.pending().size() == 2 && driver.fitAttempts == 0 && driver.clickAttempts == 0);
}

TEST_CASE(session_typed_trace_scope_survives_null_binding_and_project_storage_release)
{
    TraceFitGuard driver; Session session(driver);
    auto& sub = addSub(session.project(), 11);
    const auto first = session.commitNonNullDiagramBinding(sub);
    session.commitNonNullDiagramBinding(sub);
    SessionTraceRequest input(session);
    auto bound = session.bindBooleanTrace(input.request, input.trace); REQUIRE(bound.binding);
    session.commitNullDiagramBinding(); session.releaseProjectAfterCloseEffects();
    CHECK(session.liveProject() == nullptr && session.pending().size() == 2);
    driver.requireReleasedProject = true;
    engine::FileEffects effects;
    expectUnsupported([&] { session.dispatchBooleanTraceAt(input.trace.point, *bound.binding, effects); });
    CHECK(driver.fitAttempts == 1 && driver.clickAttempts == 0);
    REQUIRE(session.pending().size() == 1); CHECK(session.pending()[0].id == first);
}

TEST_CASE(session_typed_trace_escaped_receipt_cannot_rebind_to_new_session_with_colliding_ids)
{
    std::optional<engine::BooleanBoundTraceDue> escaped;
    engine::PumpPoint point;
    Events::TimerId oldFirst = 0, oldSecond = 0;
    {
        TraceFitGuard oldDriver; Session old(oldDriver);
        auto& sub = addSub(old.project(), 11);
        oldFirst = old.commitNonNullDiagramBinding(sub); oldSecond = old.commitNonNullDiagramBinding(sub);
        SessionTraceRequest input(old); point = input.trace.point;
        auto bound = old.bindBooleanTrace(input.request, input.trace); REQUIRE(bound.binding);
        escaped = *bound.binding;
    }
    TraceFitGuard freshDriver; Session fresh(freshDriver);
    auto& sub = addSub(fresh.project(), 22);
    const auto first = fresh.commitNonNullDiagramBinding(sub), second = fresh.commitNonNullDiagramBinding(sub);
    CHECK(first == oldFirst && second == oldSecond);
    engine::FileEffects effects;
    expectUnsupported([&] { fresh.dispatchBooleanTraceAt(point, *escaped, effects); });
    CHECK(freshDriver.fitAttempts == 0 && freshDriver.clickAttempts == 0);
    REQUIRE(fresh.pending().size() == 2);
    CHECK(fresh.pending()[0].id == first && fresh.pending()[1].id == second);
}

int main() { return testkit::run_all(); }
