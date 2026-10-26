#include "../support/test_paths.hpp"
// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include <process.h>
#include <cstring>
#include <filesystem>
#include <functional>
#include <string>
#include <vector>
#include <defmodel/layout/db_step_data.hpp>
#include "m2m/data/boolean.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/boolean_lifecycle.hpp"
#include "m2m/engine/lifecycle_events.hpp"
#include "m2m/mst/loader_l5.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4path.hpp"
#include "../../src/engine/engine_internal.hpp"

namespace m2m::mst { void installLoaderSlots(); }
namespace fs = std::filesystem;
using namespace m2m;

namespace {
struct Rig {
    Project p;
    Operation* op = nullptr;
    engine::GuiModel* model = nullptr;
    fs::path dir;
    explicit Rig(bool two = false)
    {
        static unsigned serial = 0;
        dir = m2m_test::scratchOutput(m2m_test::scratchRoot() /
            ("m2m_boolean_" + std::to_string(_getpid())) / std::to_string(++serial));
        fs::create_directories(dir);
        initProject(p, qt::String(dir.u16string()), qt::L("Boolean"));
        makeRegistry(p);
        mst::installLoaderSlots();
        op = createOperation(p, qt::L("Boolean"),
                             two ? qt::L("SfOprBoolean2D") : qt::L("SfOprBoolean3D"), {}, 1);
        REQUIRE(op != nullptr);
        op->taskDir = qt::L("Operations/Task00001");
        opBase(*op)->current = op;
        op->engineModel = std::make_shared<engine::GuiModel>(nullptr, two ? 2 : 3);
        model = op->engineModel.get();
        op->objIds = {1, 5};
        for (int i = 0; i < 2; ++i) {
            auto entry = std::make_unique<ObjEntry>();
            entry->number = i + 1;
            op->objs.push_back(std::move(entry));
            auto item = std::make_unique<Item>();
            item->kind = ItemKind::Object;
            item->id = i ? 5 : 1;
            p.objects.rows.push_back(std::move(item));
            auto* object = model->objectWrap(i + 1, false, true);
            REQUIRE(object != nullptr);
            *engine::at<std::int32_t>(object, 0xc) = i ? 0 : 2;
            std::strcpy(engine::at<char>(object, 0x828), i ? "ExistingCutter" : "Workpiece");
        }
    }
    data::BooleanData& data()
    {
        auto* props = dynamic_cast<BooleanProps*>(op->props.get());
        REQUIRE(props != nullptr);
        REQUIRE(props->data != nullptr);
        return *props->data;
    }
};

// Observe the checkpoints while continuing into both real Boolean hooks.
// Scoped restoration keeps the process-global loader table unchanged afterward.
struct HookObservation {
    static inline decltype(mst::g_l5Slots.booleanEditorApply) apply = nullptr;
    static inline decltype(mst::g_l5Slots.booleanEditorFill) fill = nullptr;
    static inline std::vector<int>* observed = nullptr;
    std::vector<int> currentObjects;
    HookObservation()
    {
        apply = mst::g_l5Slots.booleanEditorApply;
        fill = mst::g_l5Slots.booleanEditorFill;
        observed = &currentObjects;
        mst::g_l5Slots.booleanEditorApply = [](mst::BooleanObjectEditor& editor, Project& p,
                Operation& op, engine::FileEffects& fx) {
            observed->push_back(engine::opModel(p, op)->step->u1b4c);
            const bool result = apply(editor, p, op, fx);
            // Make the factory's independent zero checkpoint observable.
            engine::opModel(p, op)->step->u1b4c = 47;
            return result;
        };
        mst::g_l5Slots.booleanEditorFill = [](mst::BooleanObjectEditor& editor, Project& p,
                Operation& op) {
            observed->push_back(engine::opModel(p, op)->step->u1b4c);
            return fill(editor, p, op);
        };
    }
    ~HookObservation()
    {
        mst::g_l5Slots.booleanEditorApply = apply;
        mst::g_l5Slots.booleanEditorFill = fill;
        observed = nullptr;
    }
};

engine::BooleanUpdateDataEmission admittedUpdateData(
    const std::shared_ptr<const void>& receiver, const std::shared_ptr<const void>& editor,
    std::uint64_t occurrence = 1)
{
    engine::BooleanUpdateDataEmission input;
    input.occurrence = occurrence;
    input.emissionAdmitted = input.receiverRegistrationAdmitted = true;
    input.notifierSuppressed = false;
    input.methodResolved = input.currentWidgetMatched = true;
    input.receiverOwner = receiver;
    input.currentEditorOwner = editor;
    input.qualification = "synthetic admitted receiver suffix; no original runtime observation";
    return input;
}

struct ThrowAfterApply {
    static inline decltype(mst::g_l5Slots.booleanEditorApply) original = nullptr;
    ThrowAfterApply()
    {
        original = mst::g_l5Slots.booleanEditorApply;
        mst::g_l5Slots.booleanEditorApply = [](mst::BooleanObjectEditor& editor, Project& p,
                Operation& op, engine::FileEffects& fx) -> bool {
            original(editor, p, op, fx);
            throw Error(M2M_E_UNSUPPORTED, "synthetic interruption after real Apply effects");
        };
    }
    ~ThrowAfterApply() { mst::g_l5Slots.booleanEditorApply = original; }
};

struct ReenterApply {
    static inline decltype(mst::g_l5Slots.booleanEditorApply) original = nullptr;
    static inline std::function<void()> callback;
    explicit ReenterApply(std::function<void()> invoke)
    {
        original = mst::g_l5Slots.booleanEditorApply;
        callback = std::move(invoke);
        mst::g_l5Slots.booleanEditorApply = [](mst::BooleanObjectEditor& editor, Project& p,
                Operation& op, engine::FileEffects& fx) {
            callback();
            return original(editor, p, op, fx);
        };
    }
    ~ReenterApply() { mst::g_l5Slots.booleanEditorApply = original; callback = {}; }
};
} // namespace

// Original 0x1411e25b0 fills two combo selections; 0x1411e2870 applies
// the filled selection even when the underlying data changes in between.
TEST_CASE(boolean_editor_snapshot_creates_missing_cutter)
{
    Rig r;
    ProjectScope scope(r.p);
    engine::FileEffects fx;
    REQUIRE(opBase(*r.op)->objectTreeFill(r.p, *r.op, fx));
    CHECK_EQ(r.model->objectCount(), 2); // no inferred Open-time apply
    r.data().cutter = 2;
    REQUIRE(opBase(*r.op)->updateDataInWidgetArea(r.p, *r.op, fx));
    CHECK_EQ(r.data().workpiece, 1);
    CHECK_EQ(r.data().cutter, 3);
    CHECK_EQ(r.model->objectCount(), 3);
    REQUIRE(r.p.objects.find(2) != nullptr); // smallest unused manager id, not object number
    CHECK(r.p.objects.find(2)->name == qt::L("Cutter"));
    CHECK_EQ(objId(*r.op, 2), 2);
    CHECK(std::strcmp(engine::at<char>(r.model->object(3), 0x828), "Cutter") == 0);
    CHECK(r.op->objs[2]->name == qt::L("Cutter"));
}

TEST_CASE(boolean_editor_existing_cutter_is_retained)
{
    Rig r;
    ProjectScope scope(r.p);
    r.data().cutter = 2;
    engine::FileEffects fx;
    REQUIRE(opBase(*r.op)->objectTreeFill(r.p, *r.op, fx));
    REQUIRE(opBase(*r.op)->updateDataInWidgetArea(r.p, *r.op, fx));
    CHECK_EQ(r.model->objectCount(), 2);
    CHECK_EQ(r.data().cutter, 2);
    CHECK(std::strcmp(engine::at<char>(r.model->object(2), 0x828), "ExistingCutter") == 0);
}

TEST_CASE(boolean_sync_update_data_applies_before_return_without_timer_or_refill)
{
    Rig r;
    ProjectScope scope(r.p);
    engine::FileEffects fx;
    REQUIRE(opBase(*r.op)->objectTreeFill(r.p, *r.op, fx));
    const auto receiver = std::make_shared<const int>(1);
    const auto editor = std::make_shared<const int>(2);
    engine::BooleanSynchronousUpdateData direct(r.p, *r.op, receiver, editor,
        "synthetic actual-owner mapping; not a Session/native binding");
    CHECK_EQ(r.model->objectCount(), 2); // binding admits no first Open effect
    r.model->step->u1b4c = 9;
    HookObservation observation;
    const auto result = direct.dispatch(r.p, *r.op, fx, admittedUpdateData(receiver, editor));
    CHECK(result.completed && result.outcome == engine::BooleanUpdateDataOutcome::Invoked);
    CHECK_EQ(r.data().cutter, 3); // effect visible before any caller continuation
    CHECK_EQ(r.model->objectCount(), 3);
    CHECK(observation.currentObjects == std::vector<int>({9})); // no implicit Fill / current-object reset
    REQUIRE(opBase(*r.op)->objectTreeFill(r.p, *r.op, fx));
    direct.releaseAdmissionScope();
    const auto replacement = std::make_shared<const int>(3);
    engine::BooleanSynchronousUpdateData next(r.p, *r.op, receiver, replacement, "synthetic replacement owner");
    CHECK(next.dispatch(r.p, *r.op, fx, admittedUpdateData(receiver, replacement, 2)).completed);
    CHECK_EQ(r.model->objectCount(), 3); // explicit replacement Fill changes the selected snapshot
}

TEST_CASE(boolean_sync_update_data_requires_explicit_emission_and_suffix_observations)
{
    Rig r;
    ProjectScope scope(r.p);
    engine::FileEffects fx;
    REQUIRE(opBase(*r.op)->objectTreeFill(r.p, *r.op, fx));
    const auto receiver = std::make_shared<const int>(1);
    const auto editor = std::make_shared<const int>(2);
    engine::BooleanSynchronousUpdateData direct(r.p, *r.op, receiver, editor, "synthetic owners");
    auto input = admittedUpdateData(receiver, editor);
    input.emissionAdmitted = false;
    CHECK(!direct.dispatch(r.p, *r.op, fx, input).completed);
    input.emissionAdmitted = true;
    input.notifierSuppressed.reset();
    CHECK(!direct.dispatch(r.p, *r.op, fx, input).completed);
    input.notifierSuppressed = false;
    input.receiverRegistrationAdmitted = false;
    CHECK(!direct.dispatch(r.p, *r.op, fx, input).completed);
    input.receiverRegistrationAdmitted = true;
    input.methodResolved.reset();
    CHECK(!direct.dispatch(r.p, *r.op, fx, input).completed);
    input.methodResolved = true;
    input.currentWidgetMatched.reset();
    CHECK(!direct.dispatch(r.p, *r.op, fx, input).completed);
    CHECK_EQ(r.model->objectCount(), 2);
    input.currentWidgetMatched = true;
    CHECK(direct.dispatch(r.p, *r.op, fx, input).outcome == engine::BooleanUpdateDataOutcome::Invoked);
    CHECK_EQ(r.model->objectCount(), 3);
}

TEST_CASE(boolean_sync_update_data_suppression_unresolved_method_and_absent_current_skip_apply)
{
    Rig r;
    ProjectScope scope(r.p);
    engine::FileEffects fx;
    REQUIRE(opBase(*r.op)->objectTreeFill(r.p, *r.op, fx));
    const auto receiver = std::make_shared<const int>(1);
    const auto editor = std::make_shared<const int>(2);
    engine::BooleanSynchronousUpdateData direct(r.p, *r.op, receiver, editor, "synthetic owners");
    auto input = admittedUpdateData(receiver, editor);
    input.notifierSuppressed = true;
    CHECK(direct.dispatch(r.p, *r.op, fx, input).outcome == engine::BooleanUpdateDataOutcome::Suppressed);
    input = admittedUpdateData(receiver, editor, 2);
    input.methodResolved = false;
    CHECK(direct.dispatch(r.p, *r.op, fx, input).outcome == engine::BooleanUpdateDataOutcome::MethodNotResolved);
    input = admittedUpdateData(receiver, editor, 3);
    input.currentWidgetMatched = false;
    CHECK(direct.dispatch(r.p, *r.op, fx, input).outcome == engine::BooleanUpdateDataOutcome::NoCurrentEditor);
    CHECK_EQ(r.model->objectCount(), 2);
    CHECK_EQ(r.data().cutter, 0);
}

TEST_CASE(boolean_sync_update_data_rejects_colliding_owner_addresses_and_other_operations)
{
    Rig r;
    Rig other;
    ProjectScope scope(r.p);
    engine::FileEffects fx;
    REQUIRE(opBase(*r.op)->objectTreeFill(r.p, *r.op, fx));
    const auto receiver = std::make_shared<const int>(1);
    const auto editor = std::make_shared<const int>(2);
    engine::BooleanSynchronousUpdateData direct(r.p, *r.op, receiver, editor, "synthetic owners");
    const auto rejected = [&](Project& p, Operation& op, const engine::BooleanUpdateDataEmission& input) {
        bool caught = false;
        try { direct.dispatch(p, op, fx, input); }
        catch (const Error& error) { caught = error.code == M2M_E_UNSUPPORTED; }
        CHECK(caught);
    };
    auto input = admittedUpdateData(receiver, editor);
    const std::shared_ptr<const void> forgedReceiver(receiver.get(), [](const void*) {});
    input.receiverOwner = forgedReceiver;
    rejected(r.p, *r.op, input);
    input = admittedUpdateData(receiver, editor);
    const std::shared_ptr<const void> forgedEditor(editor.get(), [](const void*) {});
    input.currentEditorOwner = forgedEditor;
    rejected(r.p, *r.op, input);
    rejected(other.p, *other.op, admittedUpdateData(receiver, editor)); // same logical operation ID
    const auto model = r.op->engineModel;
    r.op->engineModel = std::make_shared<engine::GuiModel>(nullptr, 3);
    rejected(r.p, *r.op, admittedUpdateData(receiver, editor));
    r.op->engineModel = model;
    CHECK_EQ(r.model->objectCount(), 2);
    CHECK(direct.dispatch(r.p, *r.op, fx, admittedUpdateData(receiver, editor)).completed);
}

TEST_CASE(boolean_sync_update_data_expired_receiver_and_editor_never_apply)
{
    Rig r;
    ProjectScope scope(r.p);
    engine::FileEffects fx;
    REQUIRE(opBase(*r.op)->objectTreeFill(r.p, *r.op, fx));
    auto receiver = std::make_shared<const int>(1);
    auto editor = std::make_shared<const int>(2);
    engine::BooleanSynchronousUpdateData expiredReceiver(r.p, *r.op, receiver, editor, "synthetic owners");
    auto input = admittedUpdateData(receiver, editor);
    receiver.reset();
    CHECK(expiredReceiver.dispatch(r.p, *r.op, fx, input).outcome == engine::BooleanUpdateDataOutcome::ExpiredReceiver);
    receiver = std::make_shared<const int>(3);
    engine::BooleanSynchronousUpdateData expiredEditor(r.p, *r.op, receiver, editor, "synthetic owners");
    input = admittedUpdateData(receiver, editor);
    editor.reset();
    CHECK(expiredEditor.dispatch(r.p, *r.op, fx, input).outcome == engine::BooleanUpdateDataOutcome::NoCurrentEditor);
    CHECK_EQ(r.model->objectCount(), 2);
}

TEST_CASE(boolean_sync_update_data_consumes_before_effect_and_does_not_replay_after_throw)
{
    Rig r;
    ProjectScope scope(r.p);
    engine::FileEffects fx;
    REQUIRE(opBase(*r.op)->objectTreeFill(r.p, *r.op, fx));
    const auto receiver = std::make_shared<const int>(1);
    const auto editor = std::make_shared<const int>(2);
    engine::BooleanSynchronousUpdateData direct(r.p, *r.op, receiver, editor, "synthetic owners");
    const auto input = admittedUpdateData(receiver, editor);
    ThrowAfterApply interruption;
    for (int attempt = 0; attempt < 2; ++attempt) {
        bool caught = false;
        try { direct.dispatch(r.p, *r.op, fx, input); }
        catch (const Error& error) { caught = error.code == M2M_E_UNSUPPORTED; }
        CHECK(caught);
        CHECK_EQ(r.model->objectCount(), 3);
        CHECK_EQ(r.data().cutter, 3);
    }
}

TEST_CASE(boolean_sync_update_data_nested_dispatch_and_released_scope_remain_guarded)
{
    Rig r;
    ProjectScope scope(r.p);
    engine::FileEffects fx;
    REQUIRE(opBase(*r.op)->objectTreeFill(r.p, *r.op, fx));
    const auto receiver = std::make_shared<const int>(1);
    const auto editor = std::make_shared<const int>(2);
    engine::BooleanSynchronousUpdateData direct(r.p, *r.op, receiver, editor, "synthetic owners");
    bool nestedRefused = false;
    {
        ReenterApply nested([&] {
            try { direct.dispatch(r.p, *r.op, fx, admittedUpdateData(receiver, editor, 2)); }
            catch (const Error& error) { nestedRefused = error.code == M2M_E_UNSUPPORTED; }
        });
        CHECK(direct.dispatch(r.p, *r.op, fx, admittedUpdateData(receiver, editor)).completed);
    }
    CHECK(nestedRefused);
    CHECK_EQ(r.model->objectCount(), 3);
    direct.releaseAdmissionScope();
    bool releasedRefused = false;
    try { direct.dispatch(r.p, *r.op, fx, admittedUpdateData(receiver, editor, 2)); }
    catch (const Error& error) { releasedRefused = error.code == M2M_E_UNSUPPORTED; }
    CHECK(releasedRefused);
    CHECK_EQ(r.model->objectCount(), 3);
}

TEST_CASE(boolean_editor_workpiece_match_precedes_cutter_match)
{
    Rig r;
    ProjectScope scope(r.p);
    r.data().cutter = r.data().workpiece;
    engine::FileEffects fx;
    REQUIRE(opBase(*r.op)->objectTreeFill(r.p, *r.op, fx));
    REQUIRE(opBase(*r.op)->updateDataInWidgetArea(r.p, *r.op, fx));
    CHECK_EQ(r.data().workpiece, 1);
    CHECK_EQ(r.data().cutter, 3);
}

TEST_CASE(boolean_editor_missing_both_creates_workpiece_before_cutter)
{
    Rig r;
    ProjectScope scope(r.p);
    r.data().workpiece = 77;
    engine::FileEffects fx;
    REQUIRE(opBase(*r.op)->objectTreeFill(r.p, *r.op, fx));
    REQUIRE(opBase(*r.op)->updateDataInWidgetArea(r.p, *r.op, fx));
    CHECK_EQ(r.data().workpiece, 3);
    CHECK_EQ(r.data().cutter, 4);
    CHECK(std::strcmp(engine::at<char>(r.model->object(3), 0x828), "Workpiece") == 0);
    CHECK(std::strcmp(engine::at<char>(r.model->object(4), 0x828), "Cutter") == 0);
}

TEST_CASE(boolean_reselection_two_due_callbacks_apply_then_refill_actual_editor)
{
    for (bool two : {true, false}) {
        Rig r(two);
        ProjectScope scope(r.p);
        engine::FileEffects fx;
        REQUIRE(opBase(*r.op)->objectTreeFill(r.p, *r.op, fx));
        auto* initialStep = r.model->step.get();
        r.model->step->u1b4c = 9;
        HookObservation probe;
        engine::DiagramLifecycleEvents events;
        const auto receiver = events.createReceiver();
        const auto first = events.modelAttached(receiver, true);
        const auto second = events.modelAttached(receiver, true);
        const auto callback = [&](engine::DiagramLifecycleEvents::ReceiverId) {
            engine::reselectBooleanObjectEditor(r.p, *r.op, fx);
        };
        CHECK(events.deliver(first, callback) == engine::DiagramLifecycleEvents::Delivery::Delivered);
        CHECK_EQ(r.data().cutter, 3);
        CHECK_EQ(r.model->objectCount(), 3);
        CHECK(events.deliver(second, callback) == engine::DiagramLifecycleEvents::Delivery::Delivered);
        CHECK_EQ(r.data().cutter, 3); // Refilled text selects the existing Cutter.
        CHECK_EQ(r.model->objectCount(), 3);
        CHECK(r.model->step.get() == initialStep);
        CHECK(probe.currentObjects == std::vector<int>({0, 0, 0, 0}));
        CHECK_EQ(r.model->step->u1b4c, 0);
        CHECK(std::strcmp(engine::at<char>(r.model->object(2), 0x828), "ExistingCutter") == 0);
    }
}

TEST_CASE(boolean_reselection_scope_guards_precede_current_object_mutation)
{
    Rig r;
    ProjectScope scope(r.p);
    engine::FileEffects fx;
    REQUIRE(opBase(*r.op)->objectTreeFill(r.p, *r.op, fx));
    const auto refused = [&] {
        bool result = false;
        try { engine::reselectBooleanObjectEditor(r.p, *r.op, fx); }
        catch (const Error& error) { result = error.code == M2M_E_UNSUPPORTED; }
        CHECK(result);
        CHECK_EQ(r.model->step->u1b4c, 9);
        CHECK_EQ(r.model->objectCount(), 2);
    };
    r.model->step->u1b4c = 9;
    r.p.loadingSub = true;
    refused();
    r.p.loadingSub = false;
    activeSubProject(r.p)->type = 2;
    refused();
    activeSubProject(r.p)->type = 0;
    auto* cls = opBase(*r.op);
    cls->current = nullptr;
    refused();
    cls->current = r.op;
    auto originalData = dynamic_cast<BooleanProps*>(r.op->props.get())->data;
    dynamic_cast<BooleanProps*>(r.op->props.get())->data.reset();
    refused();
    dynamic_cast<BooleanProps*>(r.op->props.get())->data = originalData;
    const auto originalKey = r.op->classKey;
    r.op->classKey = qt::L("SfOprCycle");
    refused();
    r.op->classKey = originalKey;
    r.op->project = nullptr;
    refused();
    r.op->project = &r.p;
    auto originalModel = r.op->engineModel;
    r.op->engineModel.reset();
    refused();
    r.op->engineModel = originalModel;
}

TEST_CASE(boolean_reselection_unshown_editor_refusal_keeps_original_zero_prefix)
{
    Rig r; // No initial factory/fill, so old property-stack state is unqualified.
    ProjectScope scope(r.p);
    engine::FileEffects fx;
    r.model->step->u1b4c = 9;
    bool refused = false;
    try { engine::reselectBooleanObjectEditor(r.p, *r.op, fx); }
    catch (const Error& error) { refused = error.code == M2M_E_UNSUPPORTED; }
    CHECK(refused);
    CHECK_EQ(r.model->step->u1b4c, 0);
    CHECK_EQ(r.model->objectCount(), 2);
    CHECK_EQ(r.data().cutter, 0);
}

TEST_CASE(boolean_sidefiles_staged_directory_and_dimension)
{
    for (bool two : {true, false}) {
        Rig r(two);
        ProjectScope scope(r.p);
        r.data().cutter = 2;
        r.data().remesh = 1;
        Ctx ctx;
        engine::FileEffects fx;
        // A nonexistent task directory prevents QFile.open; the writer creates no directory.
        mst::g_l5Slots.booleanSideFiles(r.p, *r.op, r.data(), ctx, fx);
        CHECK(fx.writes.empty());
        const auto task = qt::cleanPath(taskDirAbs(*r.op));
        ctx.pendingDirectories.push_back(task.u16());
        mst::g_l5Slots.booleanSideFiles(r.p, *r.op, r.data(), ctx, fx);
        REQUIRE(fx.writes.size() == 2);
        CHECK(fx.mkdirs.empty());
        CHECK_EQ(ctx.pendingWrites.size(), std::size_t{2});
        CHECK(!fs::exists(fs::path(task.u16())));
        CHECK(fx.writes[0].bytes ==
            "WZD_MACHDIST_WORKPIECE_OBJECT_NO\n  * OBJECT # FOR WORKPIECE \n1\n\n"
            "WZD_BOOLEAN_CUTTER_OBJECT_NO\n  * OBJECT # FOR CUTTER \n2\n\n"
            "WZD_BOOLEAN_REMESH\n  * PERFORM REMESH AFTER BOOLEAN\n1\n\nDONE\n\n");
        const std::string& ini = fx.writes[1].bytes;
        CHECK(ini.find("KFREAD\nOperations/Task00001/Step.KEY\n\n") != std::string::npos);
        CHECK(ini.find("WZD_MACHDIST_PASS_OBJNO\n2\n\n") != std::string::npos);
        CHECK((ini.find("BEGIN_FREE_DIST_BCC_INFO\nBCC_INIT    \t1\n") != std::string::npos) == !two);
        CHECK(ini.find("WZD_BOOLEAN_OUTPUT_FILENAME\nOperations/Task00001/DEF_MACHDIST_OUT.KEY\n\n") != std::string::npos);
        CHECK(ini.find('\r') == std::string::npos);
    }
}

int main() { return testkit::run_all(); }
