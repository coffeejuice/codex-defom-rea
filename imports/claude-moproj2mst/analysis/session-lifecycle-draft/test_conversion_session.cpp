// REVIEW DRAFT ONLY. Intended landing: tests/mdt/test_conversion_session.cpp.
// Shell-order test drivers below do not implement original Qt fit/notifications.
// ctest-labels: m2m-mdt
#include "../support/test_paths.hpp"
#include <testkit/check.hpp>
#include <process.h>
#include <cstring>
#include <filesystem>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <defmodel/layout/db_step_data.hpp>
#include "m2m/data/boolean.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/conversion_session.hpp"
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

engine::DueObservation due(const engine::PumpPoint& point, std::vector<Events::TimerId> timers)
{
    return {point, engine::DueObservation::Origin::DeclaredAdapterSchedule,
            "isolated-session-shell-test; not an original timer trace", std::move(timers)};
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

template<class F> void expectUnsupported(F&& body)
{
    bool refused = false;
    try { body(); }
    catch (const Error& error) { refused = error.code == M2M_E_UNSUPPORTED; }
    CHECK(refused);
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

int main() { return testkit::run_all(); }
