// ctest-labels: m2m-mdt close-status-owner-prefix
#include <testkit/check.hpp>

#include <memory>
#include <string>
#include <type_traits>

#include "m2m/common.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/close_status_owner_prefix.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"
#include "m2m/engine/view_manager.hpp"
#include "m2m/mst/writer.hpp"

using namespace m2m;
using namespace m2m::engine;
namespace {
CloseStatusMainObservation nonnull(const ConversionSession& session)
{
    return {&session, closeStatusSourceOccurrence, true, "declared fixture fresh actual global main lookup"};
}
auto mainOwner(const ConversionSession& session)
{
    return CloseStatusMainOwner::create(session, "declared fixture native global main binding");
}
template<class F> bool refuses(F&& body, const char* detail)
{
    try { body(); }
    catch (const Error& error) {
        return error.code == M2M_E_UNSUPPORTED && std::string(error.what()).find(detail) != std::string::npos;
    }
    return false;
}
}

static_assert(!std::is_copy_constructible_v<CloseStatusOwnerPrefixProgress>);
static_assert(!std::is_move_constructible_v<CloseStatusOwnerPrefixProgress>);
static_assert(!std::is_copy_constructible_v<CloseStatusMainOwner>);
static_assert(!std::is_move_constructible_v<CloseStatusMainOwner>);
static_assert(!std::is_copy_constructible_v<CloseStatusLayoutOwner>);
static_assert(!std::is_move_constructible_v<CloseStatusLayoutOwner>);
static_assert(!std::is_copy_constructible_v<CloseStatusItemOwner>);
static_assert(!std::is_move_constructible_v<CloseStatusItemOwner>);

TEST_CASE(status_owner_missing_actual_layout_is_not_inferred_from_session_mainview)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    auto main = mainOwner(session);
    REQUIRE(session.mainViewOwner());
    CloseStatusOwnerPrefixProgress progress(session, main, "declared fixture status occurrence");
    const auto result = attemptCloseStatusOwnerPrefix(session, main, progress, nonnull(session));
    CHECK(result.stop == CloseStatusOwnerPrefixStop::ActualLayoutOwnerUnavailable);
    CHECK(result.entryConsumedThisAttempt);
    CHECK(!result.slotConsumedThisAttempt);
    CHECK(!progress.consumedLayoutOwner());
}

TEST_CASE(status_owner_unknown_item_slot_is_not_known_null)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    auto main = mainOwner(session);
    auto layout = main->observeLayoutOwner("declared fixture actual native layout binding");
    REQUIRE(!layout->statusItemOwner());
    CHECK(layout->statusItemKnowledge() == CloseStatusItemKnowledge::Unknown);
    CloseStatusOwnerPrefixProgress progress(session, main, "declared fixture status occurrence");
    const auto result = attemptCloseStatusOwnerPrefix(session, main, progress, nonnull(session));
    CHECK(result.stop == CloseStatusOwnerPrefixStop::StatusItemKnowledgeUnavailable);
    CHECK(!result.slotConsumedThisAttempt);
    CHECK(progress.consumedLayoutOwner() == layout);
}

TEST_CASE(status_owner_known_null_slot_stops_before_statusbar_allocation)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    auto main = mainOwner(session);
    auto layout = main->observeLayoutOwner("declared fixture actual layout");
    CHECK(layout->observeNullStatusItem("declared fixture actual current layout+4f8 null"));
    CHECK(!layout->observeNullStatusItem("later duplicate null receipt"));
    CHECK(layout->slotQualification() == "declared fixture actual current layout+4f8 null");
    CloseStatusOwnerPrefixProgress progress(session, main, "declared fixture status occurrence");
    const auto result = attemptCloseStatusOwnerPrefix(session, main, progress, nonnull(session));
    CHECK(result.stop == CloseStatusOwnerPrefixStop::StatusBarAllocationUnavailable);
    CHECK(result.slotConsumedThisAttempt);
    CHECK(progress.stage() == CloseStatusOwnerPrefixStage::BeforeStatusBarAllocation);
    CHECK(!progress.consumedItemOwner());
    CHECK(std::string(result.nextSourceSite).find("0x653ca63f") == 0);
    layout->observeStatusItem("declared fixture later replacement item");
    const auto retry = attemptCloseStatusOwnerPrefix(session, main, progress, {});
    CHECK(retry.stop == CloseStatusOwnerPrefixStop::StatusBarAllocationUnavailable);
    CHECK(!retry.slotConsumedThisAttempt);
    CHECK(!retry.entryConsumedThisAttempt);
}

TEST_CASE(status_owner_known_item_stops_before_virtual_widget_and_preserves_consumed_identity)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    auto main = mainOwner(session);
    auto layout = main->observeLayoutOwner("declared fixture actual layout");
    auto item = layout->observeStatusItem("declared fixture actual current status-item identity");
    CHECK(item->layoutOwner() == layout);
    CloseStatusOwnerPrefixProgress progress(session, main, "declared fixture status occurrence");
    const auto result = attemptCloseStatusOwnerPrefix(session, main, progress, nonnull(session));
    CHECK(result.stop == CloseStatusOwnerPrefixStop::ItemVirtualWidgetUnavailable);
    CHECK(result.slotConsumedThisAttempt);
    CHECK(progress.consumedItemOwner() == item);
    CHECK(std::string(result.nextSourceSite).find("0x653ccd98") == 0);
    layout->observeNullStatusItem("declared fixture later current slot becomes null");
    CHECK(!layout->statusItemOwner());
    const auto retry = attemptCloseStatusOwnerPrefix(session, main, progress, {});
    CHECK(retry.stop == CloseStatusOwnerPrefixStop::ItemVirtualWidgetUnavailable);
    CHECK(!retry.slotConsumedThisAttempt);
    CHECK(progress.consumedItemOwner() == item); // metadata lease, not native Qt liveness
}

TEST_CASE(status_owner_can_qualify_unknown_slot_later_without_replaying_entry)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    auto main = mainOwner(session);
    CloseStatusOwnerPrefixProgress progress(session, main, "declared fixture status occurrence");
    attemptCloseStatusOwnerPrefix(session, main, progress, nonnull(session));
    auto layout = main->observeLayoutOwner("declared fixture actual layout becomes bound");
    CHECK(main->observeLayoutOwner("later receipt preserves same layout") == layout);
    CHECK(attemptCloseStatusOwnerPrefix(session, main, progress, {}).stop ==
          CloseStatusOwnerPrefixStop::StatusItemKnowledgeUnavailable);
    layout->observeNullStatusItem("declared fixture current slot null");
    const auto result = attemptCloseStatusOwnerPrefix(session, main, progress, {});
    CHECK(result.stop == CloseStatusOwnerPrefixStop::StatusBarAllocationUnavailable);
    CHECK(!result.entryConsumedThisAttempt);
    CHECK(result.slotConsumedThisAttempt);
    CHECK(progress.consumedLayoutOwner() == layout);
}

TEST_CASE(status_owner_requires_fresh_nonnull_entry_and_retains_null_entry_outcome)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    auto main = mainOwner(session);
    main->observeLayoutOwner("declared fixture actual layout")->observeNullStatusItem("declared null slot");
    CloseStatusOwnerPrefixProgress unknown(session, main, "declared fixture missing lookup");
    CHECK(attemptCloseStatusOwnerPrefix(session, main, unknown, {}).stop ==
          CloseStatusOwnerPrefixStop::StatusEntryUnavailable);
    CloseStatusOwnerPrefixProgress nullEntry(session, main, "declared fixture null lookup");
    auto observation = nonnull(session);
    observation.globalMainPresent = false;
    CHECK(attemptCloseStatusOwnerPrefix(session, main, nullEntry, observation).stop ==
          CloseStatusOwnerPrefixStop::NullMainOutsideNonNullPrefix);
    CHECK(attemptCloseStatusOwnerPrefix(session, main, nullEntry, nonnull(session)).stop ==
          CloseStatusOwnerPrefixStop::NullMainOutsideNonNullPrefix);
    CHECK(!nullEntry.consumedLayoutOwner());
}

TEST_CASE(status_owner_rejects_foreign_main_or_session_without_consuming_entry)
{
    DiagramEffectDriver effectsA, effectsB;
    ConversionSession sessionA(effectsA), sessionB(effectsB);
    auto main = mainOwner(sessionA);
    auto otherMain = mainOwner(sessionA);
    CloseStatusOwnerPrefixProgress progress(sessionA, main, "declared fixture status occurrence");
    CHECK(refuses([&] { attemptCloseStatusOwnerPrefix(sessionA, otherMain, progress, nonnull(sessionA)); }, "identity"));
    CHECK(refuses([&] { attemptCloseStatusOwnerPrefix(sessionB, main, progress, nonnull(sessionB)); }, "identity"));
    CHECK(progress.stage() == CloseStatusOwnerPrefixStage::AwaitNonNullEntry);
}

TEST_CASE(status_owner_requires_nonblank_main_layout_slot_and_progress_receipts)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    CHECK(refuses([&] { CloseStatusMainOwner::create(session, " \r\n"); }, "qualification"));
    auto main = mainOwner(session);
    CHECK(refuses([&] { main->observeLayoutOwner(" \t"); }, "qualification"));
    CHECK(!main->layoutOwner());
    auto layout = main->observeLayoutOwner("declared fixture actual layout");
    CHECK(refuses([&] { layout->observeNullStatusItem(""); }, "qualification"));
    CHECK(refuses([&] { layout->observeStatusItem(" "); }, "qualification"));
    CHECK(layout->statusItemKnowledge() == CloseStatusItemKnowledge::Unknown);
    CHECK(refuses([&] { CloseStatusOwnerPrefixProgress progress(session, main, " "); }, "qualification"));
}

TEST_CASE(status_owner_prefix_preserves_project_current_notifier_pending_and_opi)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    session.prepareProjectModel(qt::L("C:/PROBLEM/StatusOwner/fixture.DB"));
    auto manager = session.modelManagerOwner();
    auto model = session.project().dataDb;
    auto* project = session.liveProject();
    const auto pending = session.pending().size();
    const auto connections = manager->connections().size();
    auto notifications = session.sessionNotificationStateOwner();
    notifications->flag90 = true;
    notifications->suppressed = true;
    const auto keys = notifications->emptyKeys;
    const auto attempts = notifications->attempts.size();
    Operation operation;
    mst::OpClass cls(qt::L("StatusOwnerProbe"));
    cls.current = &operation;
    for (const bool present : {false, true}) {
        auto main = mainOwner(session);
        auto layout = main->observeLayoutOwner("declared fixture layout");
        if (present) layout->observeStatusItem("declared fixture item");
        else layout->observeNullStatusItem("declared fixture null item");
        CloseStatusOwnerPrefixProgress progress(session, main, "declared fixture status occurrence");
        attemptCloseStatusOwnerPrefix(session, main, progress, nonnull(session));
    }
    CHECK(manager->size() == 1);
    CHECK(manager->currentModel() == nullptr);
    CHECK(manager->connections().size() == connections);
    CHECK(session.project().dataDb == model);
    CHECK(session.liveProject() == project);
    CHECK(model->moProject == nullptr);
    CHECK(session.pending().size() == pending);
    CHECK(notifications->flag90);
    CHECK(notifications->suppressed);
    CHECK(notifications->emptyKeys == keys);
    CHECK(notifications->attempts.size() == attempts);
    CHECK(cls.current == &operation);
    CHECK(operation.operationInfoIdentity().guard().expiry() == OperationInfoExpiry::Live);
}

TEST_CASE(status_owner_diagnostic_leases_cannot_authorize_replacement_session)
{
    std::shared_ptr<CloseStatusMainOwner> main;
    std::shared_ptr<ModelManager> retainedManager;
    std::unique_ptr<CloseStatusOwnerPrefixProgress> progress;
    {
        DiagramEffectDriver effects;
        ConversionSession original(effects);
        retainedManager = original.modelManagerOwner();
        main = mainOwner(original);
        main->observeLayoutOwner("declared fixture original layout");
        progress = std::make_unique<CloseStatusOwnerPrefixProgress>(original, main,
            "declared fixture original status occurrence");
    }
    DiagramEffectDriver effects;
    ConversionSession replacement(effects);
    REQUIRE(retainedManager != replacement.modelManagerOwner());
    CHECK(refuses([&] { attemptCloseStatusOwnerPrefix(replacement, main, *progress,
        nonnull(replacement)); }, "identity"));
    CHECK(progress->stage() == CloseStatusOwnerPrefixStage::AwaitNonNullEntry);
}

TEST_CASE(status_owner_qualified_v2_accessor_still_requires_actual_widget_field_knowledge)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    auto main = mainOwner(session);
    auto item = main->observeLayoutOwner("declared actual layout")->observeStatusItem("declared actual item");
    CHECK(item->qualifyQWidgetItemV2Accessor("declared actual V2 table+60 target651a4ce0"));
    CHECK(!item->qualifyQWidgetItemV2Accessor("later same-profile receipt"));
    CHECK(item->widgetAccessorQualification() == "declared actual V2 table+60 target651a4ce0");
    CHECK(item->widgetFieldKnowledge() == CloseStatusWidgetFieldKnowledge::Unknown);
    CloseStatusOwnerPrefixProgress progress(session, main, "declared status occurrence");
    const auto result = attemptCloseStatusOwnerPrefix(session, main, progress, nonnull(session));
    CHECK(result.stop == CloseStatusOwnerPrefixStop::WidgetFieldKnowledgeUnavailable);
    CHECK(result.accessorConsumedThisAttempt);
    CHECK(!result.widgetFieldConsumedThisAttempt);
    CHECK(progress.stage() == CloseStatusOwnerPrefixStage::AwaitItemWidgetField);
}

TEST_CASE(status_owner_actual_v2_null_widget_cast_skips_metaobject_and_stops_at_allocation)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    auto main = mainOwner(session);
    auto item = main->observeLayoutOwner("declared actual layout")->observeStatusItem("declared actual item");
    item->qualifyQWidgetItemV2Accessor("declared actual V2 accessor");
    CHECK(item->observeNullWidgetField("declared current raw widget+10 null"));
    CHECK(!item->observeNullWidgetField("later same-null receipt"));
    CloseStatusOwnerPrefixProgress progress(session, main, "declared status occurrence");
    const auto result = attemptCloseStatusOwnerPrefix(session, main, progress, nonnull(session));
    CHECK(result.stop == CloseStatusOwnerPrefixStop::StatusBarAllocationUnavailable);
    CHECK(result.accessorConsumedThisAttempt);
    CHECK(result.widgetFieldConsumedThisAttempt);
    CHECK(!progress.consumedWidgetOwner());
    CHECK(progress.stage() == CloseStatusOwnerPrefixStage::BeforeStatusBarAllocation);
    CHECK(std::string(result.nextSourceSite).find("0x653ca63f") == 0);
}

TEST_CASE(status_owner_actual_v2_nonnull_widget_guards_before_qobject_metaobject)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    auto widget = session.mainViewOwner()->widgetState();
    REQUIRE(widget && widget->nativeOwnerAlive());
    REQUIRE(widget->runtimeOwner() == session.widgetRuntimeOwner());
    const auto qobjects = session.widgetRuntimeOwner()->qobjectCount();
    const auto widgets = session.widgetRuntimeOwner()->currentWidgetCount();
    const auto parent = widget->parentWidget();
    const auto flags = widget->windowFlags();
    auto main = mainOwner(session);
    auto item = main->observeLayoutOwner("declared actual layout")->observeStatusItem("declared actual item");
    item->qualifyQWidgetItemV2Accessor("declared actual V2 accessor");
    item->observeWidgetField(widget, "declared raw item+10 names this same actual widget");
    CloseStatusOwnerPrefixProgress progress(session, main, "declared status occurrence");
    const auto result = attemptCloseStatusOwnerPrefix(session, main, progress, nonnull(session));
    CHECK(result.stop == CloseStatusOwnerPrefixStop::WidgetMetaObjectUnavailable);
    CHECK(result.accessorConsumedThisAttempt);
    CHECK(result.widgetFieldConsumedThisAttempt);
    CHECK(progress.consumedWidgetOwner() == widget);
    CHECK(std::string(result.nextSourceSite).find("0x6711ddeb") == 0);
    CHECK(session.widgetRuntimeOwner()->qobjectCount() == qobjects);
    CHECK(session.widgetRuntimeOwner()->currentWidgetCount() == widgets);
    CHECK(widget->parentWidget() == parent);
    CHECK(widget->windowFlags() == flags);
    // An actual QWidget owner is not proof that it is a QStatusBar.
}

TEST_CASE(status_owner_consumed_v2_widget_cannot_be_replaced_by_later_null_field)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    auto widget = session.mainViewOwner()->widgetState();
    auto main = mainOwner(session);
    auto item = main->observeLayoutOwner("declared actual layout")->observeStatusItem("declared actual item");
    item->qualifyQWidgetItemV2Accessor("declared actual V2 accessor");
    item->observeWidgetField(widget, "declared raw item+10 widget");
    CloseStatusOwnerPrefixProgress progress(session, main, "declared status occurrence");
    attemptCloseStatusOwnerPrefix(session, main, progress, nonnull(session));
    item->observeNullWidgetField("declared later replacement of current widget field");
    const auto retry = attemptCloseStatusOwnerPrefix(session, main, progress, {});
    CHECK(retry.stop == CloseStatusOwnerPrefixStop::WidgetMetaObjectUnavailable);
    CHECK(!retry.accessorConsumedThisAttempt);
    CHECK(!retry.widgetFieldConsumedThisAttempt);
    CHECK(progress.consumedWidgetOwner() == widget);
}

TEST_CASE(status_owner_unknown_widget_field_can_be_qualified_without_replaying_virtual_dispatch)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    auto main = mainOwner(session);
    auto layout = main->observeLayoutOwner("declared actual layout");
    auto item = layout->observeStatusItem("declared actual original item");
    item->qualifyQWidgetItemV2Accessor("declared actual original V2 accessor");
    CloseStatusOwnerPrefixProgress progress(session, main, "declared status occurrence");
    attemptCloseStatusOwnerPrefix(session, main, progress, nonnull(session));
    layout->observeStatusItem("declared later layout slot replacement");
    item->observeNullWidgetField("declared consumed original item widget+10 null");
    const auto result = attemptCloseStatusOwnerPrefix(session, main, progress, {});
    CHECK(result.stop == CloseStatusOwnerPrefixStop::StatusBarAllocationUnavailable);
    CHECK(!result.entryConsumedThisAttempt);
    CHECK(!result.slotConsumedThisAttempt);
    CHECK(!result.accessorConsumedThisAttempt);
    CHECK(result.widgetFieldConsumedThisAttempt);
    CHECK(progress.consumedItemOwner() == item);
}

TEST_CASE(status_owner_foreign_runtime_widget_is_not_cast_or_treated_as_null)
{
    DiagramEffectDriver effectsA, effectsB;
    ConversionSession sessionA(effectsA), sessionB(effectsB);
    auto foreign = sessionB.mainViewOwner()->widgetState();
    auto main = mainOwner(sessionA);
    auto item = main->observeLayoutOwner("declared actual layout")->observeStatusItem("declared actual item");
    item->qualifyQWidgetItemV2Accessor("declared actual V2 accessor");
    item->observeWidgetField(foreign, "declared foreign-runtime field binding is not valid current process");
    CloseStatusOwnerPrefixProgress progress(sessionA, main, "declared status occurrence");
    CHECK(attemptCloseStatusOwnerPrefix(sessionA, main, progress, nonnull(sessionA)).stop ==
          CloseStatusOwnerPrefixStop::WidgetIdentityUnavailable);
    item->observeWidgetField(sessionA.mainViewOwner()->widgetState(), "declared later current field change");
    CHECK(attemptCloseStatusOwnerPrefix(sessionA, main, progress, {}).stop ==
          CloseStatusOwnerPrefixStop::WidgetIdentityUnavailable);
    CHECK(progress.consumedWidgetOwner() == foreign);
}

TEST_CASE(status_owner_consumed_widget_native_death_retains_nonnull_guard)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    auto widget = session.mainViewOwner()->widgetState();
    auto main = mainOwner(session);
    auto item = main->observeLayoutOwner("declared actual layout")->observeStatusItem("declared actual item");
    item->qualifyQWidgetItemV2Accessor("declared actual V2 accessor");
    item->observeWidgetField(widget, "declared raw item+10 widget");
    CloseStatusOwnerPrefixProgress progress(session, main, "declared status occurrence");
    attemptCloseStatusOwnerPrefix(session, main, progress, nonnull(session));
    session.mainViewOwner()->releaseNativeOwner(); // explicit candidate-owner fixture boundary
    REQUIRE(!widget->nativeOwnerAlive());
    CHECK(attemptCloseStatusOwnerPrefix(session, main, progress, {}).stop ==
          CloseStatusOwnerPrefixStop::WidgetIdentityUnavailable);
    CHECK(progress.stage() == CloseStatusOwnerPrefixStage::BeforeWidgetMetaObject);
}

TEST_CASE(status_owner_accessor_and_widget_field_receipts_require_explicit_identity)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    auto main = mainOwner(session);
    auto item = main->observeLayoutOwner("declared actual layout")->observeStatusItem("declared actual item");
    CHECK(refuses([&] { item->qualifyQWidgetItemV2Accessor(" \t"); }, "qualification"));
    CHECK(item->widgetAccessorQualification().empty());
    CHECK(refuses([&] { item->observeNullWidgetField(" "); }, "qualification"));
    CHECK(refuses([&] { item->observeWidgetField({}, "declared missing actual widget owner"); }, "owner"));
    CHECK(item->widgetFieldKnowledge() == CloseStatusWidgetFieldKnowledge::Unknown);
    CHECK(!item->widgetFieldOwner());
}

int main() { return testkit::run_all(); }
