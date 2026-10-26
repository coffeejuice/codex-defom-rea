// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include <array>
#include <memory>
#include <string>
#include <utility>
#include <limits>
#include "m2m/engine/view_manager.hpp"
#include "m2m/engine/widget_events.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/model.hpp"

using namespace m2m;
using namespace m2m::engine;
namespace {
constexpr std::array bindings{
    std::pair{"2signalMakeLayoutInViewManager()", "slotMakeLayout"},
    std::pair{"2signalSetWidgetInViewManager()", "slotSetWidget"},
    std::pair{"2signalRefreshSceneInViewManager()", "slotRefreshScene"},
    std::pair{"2signalGetSceneInViewManager()", "slotGetScene"}};
template<class F> void refused(F&& f) {
    bool caught = false;
    try { f(); } catch (const Error& e) { caught = e.code == M2M_E_UNSUPPORTED; }
    CHECK(caught);
}
GridStartupObservation gridFixture(const std::shared_ptr<ViewManagerState>& manager) {
    GridStartupObservation observed;
    observed.receiver = manager;
    observed.qualification = "isolated grid fixture, not an original runtime observation";
    observed.applicationPresent = true;
    observed.parentThreadMatches = true;
    observed.internalCallbackCount = 0;
    observed.applicationFilters = std::vector<GridApplicationFilter>{
        {GridApplicationFilter::Kind::WindowsVistaStyle, true}};
    observed.objectFilterCount = 0;
    observed.declarativeChildEventsEnabled = false;
    observed.childHookDataPresent = false;
    observed.currentLayoutFlags = 3;
    observed.currentWindowFlags = 0x0800f000;
    return observed;
}
}

TEST_CASE(view_manager_constructor_retains_separate_parent_private_and_receiver)
{
    doe::SessionNotificationState state;
    auto parent = std::make_shared<MainViewState>();
    CHECK(parent->nativeOwnerAlive());
    CHECK(!parent->managerChild());
    CHECK(!parent->privateState().manager());
    auto manager = parent->constructManagerChild(state);
    REQUIRE(manager);
    CHECK(parent->managerChild() == manager);
    CHECK(manager->parentMainView() == parent);
    CHECK(manager->nativeReceiverAlive());
    CHECK(manager->layoutName().isNull() && manager->layoutName().isEmpty());
    CHECK(!manager->nativeLayoutResolved());
    CHECK(!parent->privateState().manager()); // later alias is not moved ahead of layout
    refused([&] { parent->commitPrivateManagerLink(manager); });
    refused([&] { manager->requireLayoutForCurrentLookup(); });
    CHECK(state.viewManagerRegistrations.size() == bindings.size());
    for (const auto& [key, method] : bindings) {
        const auto& list = state.viewManagerRegistrations.at(key);
        REQUIRE(list.size() == 1);
        CHECK(list[0].receiver.lock() == manager);
        CHECK(list[0].bareMethod == method);
    }
    CHECK(!state.flag90 && state.emptyKeys.empty() && state.attempts.empty());
}

TEST_CASE(view_manager_registration_deduplicates_exact_pair_without_pruning)
{
    doe::SessionNotificationState state;
    const auto [key, method] = bindings[0];
    state.viewManagerRegistrations[key].push_back({{}, method});
    auto firstParent = std::make_shared<MainViewState>();
    auto first = firstParent->constructManagerChild(state);
    first->registerConstructorSignals(state);
    auto& list = state.viewManagerRegistrations.at(key);
    REQUIRE(list.size() == 2);
    CHECK(list[0].receiver.expired());
    CHECK(list[1].receiver.lock() == first);
    // Same method, different receiver appends; this is not a method-only set.
    auto secondParent = std::make_shared<MainViewState>();
    auto second = secondParent->constructManagerChild(state);
    REQUIRE(list.size() == 3);
    CHECK(list[1].receiver.lock() == first);
    CHECK(list[2].receiver.lock() == second);
    firstParent->releaseNativeOwner();
    CHECK(!first->nativeReceiverAlive());
    second->registerConstructorSignals(state);
    CHECK(list.size() == 3); // invalid receiver entry stays, even with diagnostic lease
    refused([&] { first->registerConstructorSignals(state); });
}

TEST_CASE(view_manager_parent_release_invalidates_escaped_native_receiver)
{
    doe::SessionNotificationState state;
    auto parent = std::make_shared<MainViewState>();
    auto manager = parent->constructManagerChild(state);
    parent->releaseNativeOwner();
    CHECK(!parent->nativeOwnerAlive());
    CHECK(!parent->managerChild());
    CHECK(!manager->nativeReceiverAlive());
    refused([&] { (void)parent->privateState(); });
    refused([&] { manager->requireLayoutForCurrentLookup(); });
    refused([&] { parent->constructManagerChild(state); });
    parent->releaseNativeOwner(); // adapter cleanup is idempotent, not Close replay
    for (const auto& [key, method] : bindings)
        CHECK(state.viewManagerRegistrations.at(key).size() == 1);
    manager.reset();
    for (const auto& [key, method] : bindings)
        CHECK(state.viewManagerRegistrations.at(key)[0].receiver.expired());
}

TEST_CASE(view_manager_missing_parent_replacement_and_substitution_are_refused)
{
    doe::SessionNotificationState state;
    MainViewState unowned;
    refused([&] { unowned.constructManagerChild(state); });
    CHECK(state.viewManagerRegistrations.empty());
    auto firstParent = std::make_shared<MainViewState>();
    auto first = firstParent->constructManagerChild(state);
    auto secondParent = std::make_shared<MainViewState>();
    auto second = secondParent->constructManagerChild(state);
    refused([&] { firstParent->constructManagerChild(state); });
    refused([&] { firstParent->commitPrivateManagerLink(second); });
    CHECK(firstParent->managerChild() == first);
    CHECK(!firstParent->privateState().manager());
}

TEST_CASE(view_manager_destroyed_parent_does_not_leave_dispatchable_diagnostic_lease)
{
    doe::SessionNotificationState state;
    std::shared_ptr<ViewManagerState> retained;
    {
        auto parent = std::make_shared<MainViewState>();
        retained = parent->constructManagerChild(state);
    }
    CHECK(!retained->parentMainView());
    CHECK(!retained->nativeReceiverAlive());
    CHECK(state.viewManagerRegistrations.at(bindings[0].first).size() == 1);
    refused([&] { retained->registerConstructorSignals(state); });
}

TEST_CASE(view_manager_grid_prefix_preserves_actual_owner_before_callback_guard)
{
    doe::SessionNotificationState state;
    auto parent = std::make_shared<MainViewState>();
    auto manager = parent->constructManagerChild(state);
    CHECK(!manager->installedWidgetLayout());
    CHECK(!manager->cachedGridLayout());
    CHECK(manager->ownedGridLayouts().empty());
    refused([&] { manager->ensureGridLayout(); });
    auto grid = manager->installedWidgetLayout();
    REQUIRE(grid);
    REQUIRE(manager->ownedGridLayouts().size() == 1);
    CHECK(manager->ownedGridLayouts()[0] == grid);
    CHECK(grid->parentViewManager() == manager);
    CHECK(grid->nativeOwnerAlive());
    CHECK(grid->isTopLevelLayout());
    const std::array<int, 4> zeroMargins{{0, 0, 0, 0}};
    CHECK(grid->contentsMargins() == zeroMargins);
    CHECK(grid->rowCount() == 1 && grid->columnCount() == 1);
    CHECK(grid->items().empty());
    CHECK(manager->gridParentTransferPending());
    CHECK(!manager->cachedGridLayout());
    CHECK(!manager->nativeLayoutResolved());
    CHECK(manager->layoutName().isNull());
    CHECK(!parent->privateState().manager());
}

TEST_CASE(view_manager_grid_boundary_cannot_commit_later_alias_or_repeat_allocation)
{
    doe::SessionNotificationState state;
    auto parent = std::make_shared<MainViewState>();
    auto manager = parent->constructManagerChild(state);
    refused([&] { manager->ensureGridLayout(); });
    auto grid = manager->installedWidgetLayout();
    refused([&] { parent->commitPrivateManagerLink(manager); });
    refused([&] { manager->requireLayoutForCurrentLookup(); });
    refused([&] { manager->ensureGridLayout(); });
    CHECK(manager->installedWidgetLayout() == grid);
    CHECK(manager->ownedGridLayouts().size() == 1);
    CHECK(!manager->cachedGridLayout());
    CHECK(!parent->privateState().manager());
}

TEST_CASE(view_manager_grid_owner_release_invalidates_escaped_grid_and_manager)
{
    doe::SessionNotificationState state;
    auto parent = std::make_shared<MainViewState>();
    auto manager = parent->constructManagerChild(state);
    refused([&] { manager->ensureGridLayout(); });
    auto grid = manager->installedWidgetLayout();
    REQUIRE(grid);
    parent->releaseNativeOwner();
    CHECK(!manager->nativeReceiverAlive());
    CHECK(!grid->nativeOwnerAlive());
    CHECK(!grid->parentViewManager());
    CHECK(manager->ownedGridLayouts().empty());
    CHECK(!manager->installedWidgetLayout());
    CHECK(!manager->cachedGridLayout());
    CHECK(!manager->gridParentTransferPending());
    refused([&] { manager->ensureGridLayout(); });
    CHECK(manager->ownedGridLayouts().empty());
}

TEST_CASE(view_manager_grid_exception_cleanup_releases_unleased_native_child)
{
    doe::SessionNotificationState state;
    std::weak_ptr<GridLayoutState> observed;
    {
        auto parent = std::make_shared<MainViewState>();
        auto manager = parent->constructManagerChild(state);
        refused([&] { manager->ensureGridLayout(); });
        observed = manager->installedWidgetLayout();
        CHECK(!observed.expired());
    }
    CHECK(observed.expired());
    for (const auto& [key, method] : bindings) {
        CHECK(state.viewManagerRegistrations.at(key).size() == 1);
        CHECK(state.viewManagerRegistrations.at(key)[0].receiver.expired());
    }
}

TEST_CASE(view_manager_grids_follow_separate_actual_native_parent_identities)
{
    doe::SessionNotificationState state;
    auto firstParent = std::make_shared<MainViewState>();
    auto secondParent = std::make_shared<MainViewState>();
    auto first = firstParent->constructManagerChild(state);
    auto second = secondParent->constructManagerChild(state);
    refused([&] { first->ensureGridLayout(); });
    refused([&] { second->ensureGridLayout(); });
    auto firstGrid = first->installedWidgetLayout();
    auto secondGrid = second->installedWidgetLayout();
    REQUIRE(firstGrid && secondGrid);
    CHECK(firstGrid != secondGrid);
    CHECK(firstGrid->parentViewManager() == first);
    CHECK(secondGrid->parentViewManager() == second);
    firstParent->releaseNativeOwner();
    CHECK(!firstGrid->nativeOwnerAlive());
    CHECK(secondGrid->nativeOwnerAlive());
    CHECK(second->installedWidgetLayout() == secondGrid);
    CHECK(second->gridParentTransferPending());
}

TEST_CASE(view_manager_grid_prefix_does_not_dispatch_constructor_notifier_keys)
{
    doe::SessionNotificationState state;
    auto parent = std::make_shared<MainViewState>();
    auto manager = parent->constructManagerChild(state);
    refused([&] { manager->ensureGridLayout(); });
    for (const auto& [key, method] : bindings) {
        const auto& list = state.viewManagerRegistrations.at(key);
        REQUIRE(list.size() == 1);
        CHECK(list[0].receiver.lock() == manager);
        CHECK(list[0].bareMethod == method);
    }
    CHECK(!state.flag90 && state.emptyKeys.empty() && state.attempts.empty());
}

TEST_CASE(view_manager_grid_observation_requires_actual_owner_and_parent_affinity)
{
    doe::SessionNotificationState state;
    auto parent = std::make_shared<MainViewState>();
    auto manager = parent->constructManagerChild(state);
    auto otherParent = std::make_shared<MainViewState>();
    auto other = otherParent->constructManagerChild(state);
    auto observed = gridFixture(other);
    refused([&] { manager->ensureGridLayout(observed); });
    observed = gridFixture(manager);
    observed.qualification.clear();
    refused([&] { manager->ensureGridLayout(observed); });
    observed = gridFixture(manager);
    observed.parentThreadMatches.reset();
    refused([&] { manager->ensureGridLayout(observed); });
    observed.parentThreadMatches = false;
    refused([&] { manager->ensureGridLayout(observed); });
    CHECK(manager->gridInstallStage() == GridInstallStage::Absent);
    CHECK(manager->ownedGridLayouts().empty());
    CHECK(!manager->installedWidgetLayout());
}

TEST_CASE(view_manager_grid_qualified_tail_commits_same_owned_cache_and_native_fields)
{
    doe::SessionNotificationState state;
    auto parent = std::make_shared<MainViewState>();
    auto manager = parent->constructManagerChild(state);
    const auto observed = gridFixture(manager);
    auto grid = manager->ensureGridLayout(observed);
    REQUIRE(grid);
    REQUIRE(manager->ownedGridLayouts().size() == 1);
    CHECK(manager->ownedGridLayouts()[0] == grid);
    CHECK(manager->installedWidgetLayout() == grid);
    CHECK(manager->cachedGridLayout() == grid);
    CHECK(grid->parentViewManager() == manager && grid->nativeOwnerAlive());
    CHECK(grid->layoutFlags() == 3 && grid->dirtyFlags() == 4);
    CHECK(grid->cachedSizeValue() == -1);
    const std::array<int, 4> invalidRect{{0, 0, -1, -1}};
    CHECK(grid->geometryEndpoints() == invalidRect);
    CHECK(grid->items().empty() && grid->isTopLevelLayout());
    CHECK(manager->gridInstallStage() == GridInstallStage::Cached);
    CHECK(!manager->gridParentTransferPending());
    CHECK(manager->gridContinuationQualification() == observed.qualification);
    CHECK(!state.flag90 && state.attempts.empty() && state.emptyKeys.empty());
}

TEST_CASE(view_manager_grid_qualified_retry_continues_existing_unqualified_prefix)
{
    doe::SessionNotificationState state;
    auto parent = std::make_shared<MainViewState>();
    auto manager = parent->constructManagerChild(state);
    refused([&] { manager->ensureGridLayout(); });
    auto originalGrid = manager->installedWidgetLayout();
    REQUIRE(originalGrid);
    CHECK(manager->gridInstallStage() == GridInstallStage::ChildAddedPending);
    CHECK(manager->ensureGridLayout(gridFixture(manager)) == originalGrid);
    CHECK(manager->ownedGridLayouts().size() == 1);
    CHECK(manager->cachedGridLayout() == originalGrid);
}

TEST_CASE(view_manager_grid_childadded_refuses_missing_or_changed_application_filters)
{
    doe::SessionNotificationState state;
    auto parent = std::make_shared<MainViewState>();
    auto manager = parent->constructManagerChild(state);
    auto observed = gridFixture(manager);
    observed.applicationFilters.reset();
    refused([&] { manager->ensureGridLayout(observed); });
    auto originalGrid = manager->installedWidgetLayout();
    REQUIRE(originalGrid);
    observed.applicationFilters = std::vector<GridApplicationFilter>{};
    refused([&] { manager->ensureGridLayout(observed); });
    observed.applicationFilters = std::vector<GridApplicationFilter>{{GridApplicationFilter::Kind::Unknown, true}};
    refused([&] { manager->ensureGridLayout(observed); });
    observed = gridFixture(manager);
    observed.applicationFilters->push_back({GridApplicationFilter::Kind::WindowsVistaStyle, true});
    refused([&] { manager->ensureGridLayout(observed); });
    CHECK(manager->gridInstallStage() == GridInstallStage::ChildAddedPending);
    CHECK(manager->installedWidgetLayout() == originalGrid);
    CHECK(!manager->cachedGridLayout());
    CHECK(manager->ownedGridLayouts().size() == 1);
}

TEST_CASE(view_manager_grid_childadded_preserves_unknown_callbacks_and_affinity_warning)
{
    doe::SessionNotificationState state;
    auto parent = std::make_shared<MainViewState>();
    auto manager = parent->constructManagerChild(state);
    auto observed = gridFixture(manager);
    observed.internalCallbackCount.reset();
    refused([&] { manager->ensureGridLayout(observed); });
    observed.internalCallbackCount = 1;
    refused([&] { manager->ensureGridLayout(observed); });
    observed = gridFixture(manager);
    observed.objectFilterCount.reset();
    refused([&] { manager->ensureGridLayout(observed); });
    observed.objectFilterCount = 1;
    refused([&] { manager->ensureGridLayout(observed); });
    observed = gridFixture(manager);
    observed.applicationPresent.reset();
    refused([&] { manager->ensureGridLayout(observed); });
    observed.applicationPresent = false;
    refused([&] { manager->ensureGridLayout(observed); });
    observed = gridFixture(manager);
    observed.applicationFilters->front().sameApplicationThread.reset();
    refused([&] { manager->ensureGridLayout(observed); });
    observed.applicationFilters->front().sameApplicationThread = false;
    refused([&] { manager->ensureGridLayout(observed); });
    CHECK(manager->gridInstallStage() == GridInstallStage::ChildAddedPending);
    CHECK(!manager->cachedGridLayout());
    CHECK(manager->ownedGridLayouts().size() == 1);
}

TEST_CASE(view_manager_grid_postparent_hook_guard_retains_completed_dispatch_stage)
{
    doe::SessionNotificationState state;
    auto parent = std::make_shared<MainViewState>();
    auto manager = parent->constructManagerChild(state);
    auto observed = gridFixture(manager);
    observed.declarativeChildEventsEnabled.reset();
    refused([&] { manager->ensureGridLayout(observed); });
    CHECK(manager->gridInstallStage() == GridInstallStage::PostParentHooksPending);
    auto grid = manager->installedWidgetLayout();
    observed.applicationFilters.reset(); // completed dispatch is not re-executed
    observed.declarativeChildEventsEnabled = true;
    refused([&] { manager->ensureGridLayout(observed); });
    observed.declarativeChildEventsEnabled = false;
    observed.childHookDataPresent.reset();
    refused([&] { manager->ensureGridLayout(observed); });
    observed.childHookDataPresent = true;
    refused([&] { manager->ensureGridLayout(observed); });
    CHECK(manager->gridInstallStage() == GridInstallStage::PostParentHooksPending);
    CHECK(!manager->cachedGridLayout());
    observed.childHookDataPresent = false;
    CHECK(manager->ensureGridLayout(observed) == grid);
}

TEST_CASE(view_manager_grid_update_guard_follows_real_invalidation_without_replay)
{
    doe::SessionNotificationState state;
    auto parent = std::make_shared<MainViewState>();
    auto manager = parent->constructManagerChild(state);
    auto observed = gridFixture(manager);
    observed.currentLayoutFlags.reset();
    refused([&] { manager->ensureGridLayout(observed); });
    CHECK(manager->gridInstallStage() == GridInstallStage::UpdatePending);
    auto grid = manager->installedWidgetLayout();
    REQUIRE(grid);
    CHECK(grid->dirtyFlags() == 4 && grid->cachedSizeValue() == -1);
    const std::array<int, 4> invalidRect{{0, 0, -1, -1}};
    CHECK(grid->geometryEndpoints() == invalidRect);
    observed.currentLayoutFlags = 7; // a different profile, not a native flag mutation
    refused([&] { manager->ensureGridLayout(observed); });
    CHECK(grid->layoutFlags() == 3);
    CHECK(manager->gridInstallStage() == GridInstallStage::UpdatePending);
    observed.currentLayoutFlags = 3;
    observed.applicationFilters.reset();
    observed.declarativeChildEventsEnabled.reset();
    CHECK(manager->ensureGridLayout(observed) == grid);
    CHECK(manager->ownedGridLayouts().size() == 1);
}

TEST_CASE(view_manager_grid_window_guard_retains_invalidation_and_cache_order)
{
    doe::SessionNotificationState state;
    auto parent = std::make_shared<MainViewState>();
    auto manager = parent->constructManagerChild(state);
    auto observed = gridFixture(manager);
    observed.currentWindowFlags.reset();
    refused([&] { manager->ensureGridLayout(observed); });
    CHECK(manager->gridInstallStage() == GridInstallStage::WindowTailPending);
    auto grid = manager->installedWidgetLayout();
    REQUIRE(grid);
    CHECK(grid->dirtyFlags() == 4 && grid->cachedSizeValue() == -1);
    CHECK(!manager->cachedGridLayout());
    observed.currentWindowFlags = 0x0800f001;
    refused([&] { manager->ensureGridLayout(observed); });
    refused([&] { manager->ensureGridLayout(); });
    CHECK(manager->gridInstallStage() == GridInstallStage::WindowTailPending);
    CHECK(manager->ownedGridLayouts().size() == 1);
    observed.currentWindowFlags = 0x0800f000;
    observed.currentLayoutFlags.reset();
    observed.applicationFilters.reset();
    observed.childHookDataPresent.reset();
    CHECK(manager->ensureGridLayout(observed) == grid);
    CHECK(manager->cachedGridLayout() == grid);
}

TEST_CASE(view_manager_grid_cached_retrieval_does_not_revalidate_constructor_profile)
{
    doe::SessionNotificationState state;
    auto parent = std::make_shared<MainViewState>();
    auto manager = parent->constructManagerChild(state);
    auto grid = manager->ensureGridLayout(gridFixture(manager));
    CHECK(manager->ensureGridLayout(GridStartupObservation{}) == grid);
    CHECK(manager->ensureGridLayout() == grid);
    CHECK(manager->ownedGridLayouts().size() == 1);
    CHECK(!manager->nativeLayoutResolved());
    CHECK(manager->layoutName().isNull());
    refused([&] { manager->requireLayoutForCurrentLookup(); });
    refused([&] { parent->commitPrivateManagerLink(manager); });
    CHECK(!parent->privateState().manager());
}

TEST_CASE(view_manager_grid_continuation_cannot_substitute_evidence_after_partial_tail)
{
    doe::SessionNotificationState state;
    auto parent = std::make_shared<MainViewState>();
    auto manager = parent->constructManagerChild(state);
    auto observed = gridFixture(manager);
    observed.currentWindowFlags.reset();
    refused([&] { manager->ensureGridLayout(observed); });
    auto grid = manager->installedWidgetLayout();
    const auto originalQualification = observed.qualification;
    observed.qualification = "different execution profile";
    observed.currentWindowFlags = 0x0800f000;
    refused([&] { manager->ensureGridLayout(observed); });
    CHECK(manager->gridInstallStage() == GridInstallStage::WindowTailPending);
    CHECK(manager->gridContinuationQualification() == originalQualification);
    CHECK(!manager->cachedGridLayout());
    observed.qualification = originalQualification;
    CHECK(manager->ensureGridLayout(observed) == grid);
}

TEST_CASE(view_manager_grid_completed_cache_does_not_keep_destroyed_native_owner_alive)
{
    doe::SessionNotificationState state;
    auto parent = std::make_shared<MainViewState>();
    auto manager = parent->constructManagerChild(state);
    auto grid = manager->ensureGridLayout(gridFixture(manager));
    parent->releaseNativeOwner();
    CHECK(!manager->nativeReceiverAlive() && !grid->nativeOwnerAlive());
    CHECK(!grid->parentViewManager());
    CHECK(!manager->cachedGridLayout() && !manager->installedWidgetLayout());
    CHECK(manager->ownedGridLayouts().empty());
    CHECK(manager->gridInstallStage() == GridInstallStage::Absent);
    CHECK(manager->gridContinuationQualification().empty());
    refused([&] { manager->ensureGridLayout(gridFixture(manager)); });
    CHECK(state.viewManagerRegistrations.at(bindings[0].first).size() == 1);
}

namespace {
struct SplitFixture {
    doe::SessionNotificationState notifications;
    std::shared_ptr<MainViewState> main = std::make_shared<MainViewState>();
    std::shared_ptr<ViewManagerState> manager = main->constructManagerChild(notifications);
    std::shared_ptr<WidgetRuntimeState> runtime;
    SplitViewStartupObservation observed;
    explicit SplitFixture(std::optional<bool> registryPresent = true)
        : runtime(std::make_shared<WidgetRuntimeState>(registryPresent, "isolated Qt runtime fixture"))
    {
        main->bindWidgetRuntime(runtime);
        manager->ensureGridLayout(gridFixture(manager));
        observed.receiver = manager;
        observed.qualification = "isolated splitter prefix fixture, not original startup";
        observed.parentWindowFlags = 0x0800f001;
        observed.parentAttributesLow = 0x10000;
        observed.parentExtendedAttributes = std::array<std::uint32_t, 3>{{12, 4112, 0}};
        observed.parentExtraNativeChildren = false;
        observed.parentThreadMatches = true;
        observed.childHookDataPresent = false;
        observed.parentTopExtraPresent = false;
    }
};
}

TEST_CASE(view_manager_split_registry_precedes_widget_data_and_parent_kind)
{
    SplitFixture f;
    f.observed.parentWindowFlags.reset();
    auto report = f.manager->ensureDefaultSplitView(f.observed);
    auto split = f.manager->pendingSplitView();
    REQUIRE(split);
    CHECK(!report.completed && report.stage == SplitViewConstructStage::WidgetParentKindPending);
    CHECK(f.runtime->hasLiveRegistration(split.get()));
    CHECK(f.runtime->hasLiveRegistration(f.main.get()));
    CHECK(f.runtime->hasLiveRegistration(f.manager.get()));
    CHECK(!f.runtime->populationComplete());
    CHECK(split->widgetState()->nativeIdentity() == split.get());
    CHECK(!split->widgetState()->widgetDataAssigned());
    CHECK(!split->widgetState()->attributesLow());
    CHECK(!split->widgetState()->parentWidget());
    CHECK(!split->orientation());
}

TEST_CASE(view_manager_split_unknown_registry_retains_same_pre_data_identity)
{
    SplitFixture f(std::nullopt);
    auto report = f.manager->ensureDefaultSplitView(f.observed);
    auto split = f.manager->pendingSplitView();
    CHECK(report.stage == SplitViewConstructStage::WidgetRegistryPending);
    CHECK(!f.runtime->hasLiveRegistration(split.get()));
    CHECK(!split->widgetState()->widgetDataAssigned());
    report = f.manager->ensureDefaultSplitView(f.observed);
    CHECK(f.manager->pendingSplitView() == split);
    CHECK(report.stage == SplitViewConstructStage::WidgetRegistryPending);
}

TEST_CASE(view_manager_split_absent_registry_skips_insertion_without_stopping_constructor)
{
    SplitFixture f(false);
    f.observed.parentExtraNativeChildren.reset();
    const auto report = f.manager->ensureDefaultSplitView(f.observed);
    auto split = f.manager->pendingSplitView();
    CHECK(report.stage == SplitViewConstructStage::ParentNativeAttributePending);
    CHECK(!f.runtime->hasLiveRegistration(split.get()));
    CHECK(split->widgetState()->widgetDataAssigned());
}

TEST_CASE(view_manager_split_uncaptured_parent_flag_preserves_initialized_unparented_prefix)
{
    SplitFixture f;
    const auto grid = f.manager->cachedGridLayout();
    f.observed.parentExtraNativeChildren.reset();
    const auto report = f.manager->ensureDefaultSplitView(f.observed);
    auto split = f.manager->pendingSplitView();
    const auto widget = split->widgetState();
    CHECK(report.stage == SplitViewConstructStage::ParentNativeAttributePending);
    CHECK(!report.completed && !report.issue.empty());
    CHECK(widget->attributesLow() == 0x10000u);
    REQUIRE(widget->extendedAttributes());
    CHECK((*widget->extendedAttributes())[1] == 4096);
    const std::array<int, 4> initialRect{{0, 0, 99, 29}};
    CHECK(widget->geometryEndpoints() == initialRect);
    CHECK(widget->focusNextIdentity() == split.get() && widget->focusPreviousIdentity() == split.get());
    CHECK(widget->inSetParent() && !widget->parentWidget());
    CHECK(!widget->parentOwnsChild() && f.main->ownedSplitViews().empty());
    CHECK(f.manager->cachedGridLayout() == grid);
    CHECK(f.manager->layoutName().isNull() && !split->splitViewName());
    CHECK(!f.manager->nativeLayoutResolved() && !f.main->privateState().manager());
}

TEST_CASE(view_manager_split_native_attribute_store_precedes_unresolved_native_window_effect)
{
    SplitFixture f;
    f.observed.parentExtraNativeChildren = true;
    f.observed.parentAttributesLow.reset(); // first predicate already decides
    auto report = f.manager->ensureDefaultSplitView(f.observed);
    auto split = f.manager->pendingSplitView();
    CHECK(report.stage == SplitViewConstructStage::ParentNativeAttributeEffectPending);
    REQUIRE(split->widgetState()->extendedAttributes());
    CHECK(((*split->widgetState()->extendedAttributes())[2] & 16u) != 0);
    CHECK(!split->widgetState()->parentWidget());
    f.observed.parentExtraNativeChildren = false;
    f.observed.parentAttributesLow = 0x10000;
    report = f.manager->ensureDefaultSplitView(f.observed);
    CHECK(report.stage == SplitViewConstructStage::ParentNativeAttributeEffectPending);
    CHECK(f.main->ownedSplitViews().empty());
}

TEST_CASE(view_manager_split_parent_pointer_precedes_thread_check_and_child_membership)
{
    SplitFixture f;
    f.observed.parentThreadMatches.reset();
    auto report = f.manager->ensureDefaultSplitView(f.observed);
    auto split = f.manager->pendingSplitView();
    CHECK(report.stage == SplitViewConstructStage::ParentThreadPending);
    CHECK(split->widgetState()->parentWidget() == f.main->widgetState());
    CHECK(!split->widgetState()->parentOwnsChild());
    CHECK(f.main->ownedSplitViews().empty());
    f.observed.parentThreadMatches = false;
    report = f.manager->ensureDefaultSplitView(f.observed);
    CHECK(report.stage == SplitViewConstructStage::ParentThreadPending); // before native warning/clear
    f.observed.parentThreadMatches = true;
    f.observed.childHookDataPresent.reset();
    report = f.manager->ensureDefaultSplitView(f.observed);
    CHECK(report.stage == SplitViewConstructStage::ParentHookPending);
    CHECK(f.main->ownedSplitViews().size() == 1 && f.main->ownedSplitViews()[0] == split);
    CHECK(split->widgetState()->parentOwnsChild());
}

TEST_CASE(view_manager_split_actual_parent_order_is_separate_from_grid_and_pending_construction)
{
    SplitFixture f;
    f.observed.childHookDataPresent.reset();
    auto report = f.manager->ensureDefaultSplitView(f.observed);
    auto split = f.manager->pendingSplitView();
    CHECK(report.stage == SplitViewConstructStage::ParentHookPending);
    const auto& children = f.main->widgetState()->knownChildSuffix();
    CHECK(f.main->widgetState()->childPrefixUnrecovered());
    REQUIRE(children.size() == 2);
    CHECK(children[0] == f.manager.get() && children[1] == split.get());
    const auto& managerChildren = f.manager->widgetState()->knownChildSuffix();
    REQUIRE(managerChildren.size() == 1);
    CHECK(managerChildren[0] == f.manager->cachedGridLayout().get());
    report = f.manager->ensureDefaultSplitView(f.observed);
    CHECK(f.main->ownedSplitViews().size() == 1 && children.size() == 2);
    CHECK(f.manager->pendingSplitView() == split);
    CHECK(!split->orientation() && !split->splitViewName());
}

TEST_CASE(view_manager_split_flags_commit_before_unknown_parent_native_creation)
{
    SplitFixture f;
    f.observed.parentExtendedAttributes.reset();
    auto report = f.manager->ensureDefaultSplitView(f.observed);
    auto widget = f.manager->pendingSplitView()->widgetState();
    CHECK(report.stage == SplitViewConstructStage::ParentNativeCreationPending);
    CHECK(widget->parentOwnsChild());
    CHECK(widget->windowFlags() == 0x0800f000u);
    CHECK(widget->attributesLow() == 0u);
    CHECK(widget->inSetParent());
}

TEST_CASE(view_manager_split_platform_visibility_retains_next_backing_store_and_compatibility_guards)
{
    SplitFixture f;
    f.observed.parentTopExtraPresent.reset();
    auto report = f.manager->ensureDefaultSplitView(f.observed);
    auto split = f.manager->pendingSplitView();
    CHECK(report.stage == SplitViewConstructStage::ParentBackingStorePending);
    CHECK(split->widgetState()->attributesLow() == 0u);
    f.observed.parentTopExtraPresent = false;
    report = f.manager->ensureDefaultSplitView(f.observed);
    CHECK(report.stage == SplitViewConstructStage::ParentCompatibilityPending);
    CHECK(!report.completed && !report.issue.empty());
    CHECK(f.manager->pendingSplitView() == split);
    CHECK(f.main->ownedSplitViews().size() == 1);
    CHECK(!f.manager->nativeLayoutResolved());
}

TEST_CASE(view_manager_split_partial_widget_and_registration_die_with_native_owner)
{
    for (bool parentCommitted : {false, true}) {
        SplitFixture f;
        if (!parentCommitted) f.observed.parentExtraNativeChildren.reset();
        f.manager->ensureDefaultSplitView(f.observed);
        auto split = f.manager->pendingSplitView();
        auto widget = split->widgetState();
        REQUIRE(f.runtime->hasLiveRegistration(split.get()));
        f.main->releaseNativeOwner();
        CHECK(!split->nativeOwnerAlive() && !widget->parentWidget());
        CHECK(!f.runtime->hasLiveRegistration(split.get()));
        CHECK(!f.runtime->hasLiveRegistration(f.main.get()));
        CHECK(!f.runtime->hasLiveRegistration(f.manager.get()));
        CHECK(f.main->ownedSplitViews().empty());
        CHECK(!f.manager->pendingSplitView());
    }
}

TEST_CASE(view_manager_split_cannot_substitute_runtime_profile_or_receiver)
{
    SplitFixture f;
    f.observed.parentExtraNativeChildren.reset();
    f.manager->ensureDefaultSplitView(f.observed);
    auto split = f.manager->pendingSplitView();
    auto other = std::make_shared<WidgetRuntimeState>(true, "other runtime");
    refused([&] { f.main->bindWidgetRuntime(other); });
    f.observed.qualification = "another construction";
    refused([&] { f.manager->ensureDefaultSplitView(f.observed); });
    CHECK(f.manager->pendingSplitView() == split);
    f.observed.receiver.reset();
    refused([&] { f.manager->ensureDefaultSplitView(f.observed); });
}

TEST_CASE(view_manager_focus_observation_binds_actual_tail_only)
{
    SplitFixture f;
    auto window = f.main->widgetState();
    auto manager = f.manager->widgetState();
    CHECK(!window->focusPreviousKnown() && !window->focusNextKnown());
    CHECK(!manager->focusPreviousKnown() && !manager->focusNextKnown());
    f.main->observeInitialManagerFocusTail("isolated initial tail");
    CHECK(window->focusPreviousKnown() && window->focusPreviousIdentity() == f.manager.get());
    CHECK(manager->focusNextKnown() && manager->focusNextIdentity() == f.main.get());
    CHECK(!window->focusNextKnown() && !manager->focusPreviousKnown());
    CHECK(!f.manager->pendingSplitView() && !f.main->privateState().manager());
    CHECK(!f.manager->nativeLayoutResolved());
}

TEST_CASE(view_manager_focus_observation_requires_live_actual_runtime_and_initial_stage)
{
    doe::SessionNotificationState signals;
    auto main = std::make_shared<MainViewState>();
    refused([&] { main->observeInitialManagerFocusTail("missing manager"); });
    auto manager = main->constructManagerChild(signals);
    refused([&] { main->observeInitialManagerFocusTail("missing runtime"); });
    auto runtime = std::make_shared<WidgetRuntimeState>(true, "isolated runtime");
    main->bindWidgetRuntime(runtime);
    refused([&] { main->observeInitialManagerFocusTail(""); });
    main->observeInitialManagerFocusTail("same observation");
    main->observeInitialManagerFocusTail("same observation");
    refused([&] { main->observeInitialManagerFocusTail("replacement observation"); });
    main->releaseNativeOwner();
    refused([&] { main->observeInitialManagerFocusTail("same observation"); });
}

TEST_CASE(view_manager_split_compatibility_missing_inputs_preserve_parent_and_focus_prefix)
{
    SplitFixture f;
    f.main->observeInitialManagerFocusTail("isolated tail");
    auto report = f.manager->ensureDefaultSplitView(f.observed);
    auto split = f.manager->pendingSplitView();
    CHECK(report.stage == SplitViewConstructStage::ParentCompatibilityPending);
    CHECK(split->widgetState()->parentOwnsChild());
    f.observed.applicationCompileVersion = 0x40804;
    report = f.manager->ensureDefaultSplitView(f.observed);
    CHECK(report.stage == SplitViewConstructStage::ParentCompatibilityPending);
    CHECK(f.main->widgetState()->focusPreviousIdentity() == f.manager.get());
    CHECK(f.manager->widgetState()->focusNextIdentity() == f.main.get());
    CHECK(split->widgetState()->focusNextIdentity() == split.get());
    CHECK(split->widgetState()->focusPreviousIdentity() == split.get());
}

TEST_CASE(view_manager_split_compatibility_creation_branch_stops_before_focus_writes)
{
    for (bool oldVersion : {false, true}) {
        SplitFixture f;
        f.main->observeInitialManagerFocusTail("isolated tail");
        f.observed.applicationCompileVersion = oldVersion ? 0x401ff : 0x40804;
        // Old-version short circuit does not require the later attribute read.
        if (!oldVersion) f.observed.applicationAttributes = 1;
        auto report = f.manager->ensureDefaultSplitView(f.observed);
        CHECK(report.stage == SplitViewConstructStage::ParentCompatibilityEffectPending);
        auto split = f.manager->pendingSplitView();
        CHECK(split->widgetState()->parentOwnsChild());
        CHECK(f.main->widgetState()->focusPreviousIdentity() == f.manager.get());
        CHECK(f.manager->widgetState()->focusNextIdentity() == f.main.get());
        CHECK(split->widgetState()->focusPreviousIdentity() == split.get());
        f.observed.applicationCompileVersion = 0x40804;
        f.observed.applicationAttributes = 0;
        report = f.manager->ensureDefaultSplitView(f.observed);
        CHECK(report.stage == SplitViewConstructStage::ParentCompatibilityEffectPending);
    }
}

TEST_CASE(view_manager_split_missing_focus_tail_keeps_actual_parent_and_fresh_ring)
{
    SplitFixture f;
    f.observed.applicationCompileVersion = 0x40804;
    f.observed.applicationAttributes = 0;
    const auto report = f.manager->ensureDefaultSplitView(f.observed);
    auto split = f.manager->pendingSplitView();
    CHECK(report.stage == SplitViewConstructStage::ParentFocusPending);
    CHECK(split->widgetState()->parentWidget() == f.main->widgetState());
    CHECK(split->widgetState()->focusNextKnown() && split->widgetState()->focusPreviousKnown());
    CHECK(split->widgetState()->focusNextIdentity() == split.get());
    CHECK(!f.main->widgetState()->focusPreviousKnown());
    refused([&] { f.main->observeInitialManagerFocusTail("too late"); });
    CHECK(f.manager->ensureDefaultSplitView(f.observed).stage == SplitViewConstructStage::ParentFocusPending);
}

TEST_CASE(view_manager_split_focus_splices_same_owners_then_retains_font_preparation_guard)
{
    SplitFixture f;
    f.main->observeInitialManagerFocusTail("isolated tail, not native evidence");
    f.observed.applicationCompileVersion = 0x40804;
    f.observed.applicationAttributes = 0;
    auto grid = f.manager->cachedGridLayout();
    auto report = f.manager->ensureDefaultSplitView(f.observed);
    auto split = f.manager->pendingSplitView();
    auto child = split->widgetState();
    CHECK(report.stage == SplitViewConstructStage::ParentFontPreparationPending);
    CHECK(!report.completed && !report.issue.empty());
    CHECK(f.main->widgetState()->focusPreviousIdentity() == split.get());
    CHECK(f.manager->widgetState()->focusNextIdentity() == split.get());
    CHECK(child->focusPreviousIdentity() == f.manager.get());
    CHECK(child->focusNextIdentity() == f.main.get());
    CHECK(!f.main->widgetState()->focusNextKnown());
    CHECK(!f.manager->widgetState()->focusPreviousKnown());
    REQUIRE(child->extendedAttributes());
    CHECK(((*child->extendedAttributes())[0] & (1u << 10)) == 0);
    CHECK(child->inSetParent() && child->parentOwnsChild());
    CHECK(!split->splitViewName() && !split->orientation());
    CHECK(f.manager->cachedGridLayout() == grid && f.manager->layoutName().isNull());
    CHECK(!f.manager->nativeLayoutResolved() && !f.main->privateState().manager());
}

TEST_CASE(view_manager_split_font_guard_does_not_replay_initial_focus_splice)
{
    SplitFixture f;
    f.main->observeInitialManagerFocusTail("isolated tail");
    f.observed.applicationCompileVersion = 0x40804;
    f.observed.applicationAttributes = 0;
    f.manager->ensureDefaultSplitView(f.observed);
    auto split = f.manager->pendingSplitView();
    f.observed.applicationCompileVersion.reset();
    f.observed.applicationAttributes.reset();
    const auto report = f.manager->ensureDefaultSplitView(f.observed);
    CHECK(report.stage == SplitViewConstructStage::ParentFontPreparationPending);
    CHECK(f.manager->pendingSplitView() == split && f.main->ownedSplitViews().size() == 1);
    CHECK(f.main->widgetState()->focusPreviousIdentity() == split.get());
    CHECK(f.manager->widgetState()->focusNextIdentity() == split.get());
    CHECK(split->widgetState()->focusNextIdentity() == f.main.get());
    refused([&] { f.main->observeInitialManagerFocusTail("isolated tail"); });
}

TEST_CASE(view_manager_split_spliced_owner_release_invalidates_all_live_registrations)
{
    SplitFixture f;
    f.main->observeInitialManagerFocusTail("isolated tail");
    f.observed.applicationCompileVersion = 0x40804;
    f.observed.applicationAttributes = 0;
    f.manager->ensureDefaultSplitView(f.observed);
    auto split = f.manager->pendingSplitView();
    f.main->releaseNativeOwner();
    CHECK(!split->nativeOwnerAlive() && !split->widgetState()->parentWidget());
    CHECK(!f.runtime->hasLiveRegistration(split.get()));
    CHECK(!f.runtime->hasLiveRegistration(f.manager.get()));
    CHECK(!f.runtime->hasLiveRegistration(f.main.get()));
}

namespace {
struct ValueFixture : SplitFixture {
    ValueFixture() {
        main->observeInitialManagerFocusTail("isolated focus tail");
        observed.applicationCompileVersion = 0x40804;
        observed.applicationAttributes = 0;
    }
    WidgetRuntimeValueObservation values() {
engine::WidgetRuntimeValueObservation widgetValues;
widgetValues.mainView = main;
widgetValues.manager = manager;
widgetValues.qualification = "isolated value/cache fixture, not runtime evidence";
widgetValues.fontData = {{1, 168}};
widgetValues.paletteData = {{1}, {2}, {3}};
widgetValues.applicationFont.defaultValue = engine::WidgetFontValueObservation{1, 51};
widgetValues.applicationFont.mutexPresent = true;
widgetValues.applicationFont.classCachePresent = true;
widgetValues.applicationFont.classes = std::vector<engine::WidgetFontClassObservation>{};
widgetValues.applicationPalette.defaultValue = engine::WidgetPaletteValueObservation{1, 13893616};
widgetValues.applicationPalette.classCachePresent = true;
widgetValues.applicationPalette.classes = std::vector<engine::WidgetPaletteClassObservation>{
    {"QMenuBar", {2, 13893616}}, {"QMenu", {3, 13893616}}};
auto& mainValues = widgetValues.mainViewValues;
mainValues.font = engine::WidgetFontValueObservation{1, 0};
mainValues.palette = engine::WidgetPaletteValueObservation{1, 0};
mainValues.locale = engine::WidgetLocaleValue{391, 0};
mainValues.inheritedFontMask = mainValues.inheritedPaletteMask = 0;
mainValues.attributesLow = 65536;
mainValues.extendedAttributes = std::array<std::uint32_t, 3>{{12, 4112, 0}};
mainValues.windowFlags = 0x0800f001u;
mainValues.opaqueChildrenDirty = true;
mainValues.opaque = true;
mainValues.graphicsEffectPresent = false;
mainValues.autoFillBackground = false;
mainValues.stylePresent = false;
mainValues.stylesheetLength = 0;
mainValues.proxyPresent = false;
mainValues.events.objectFilterCount = 0;
mainValues.events.sameApplicationThread = true;
mainValues.events.layoutPresent = false;
mainValues.events.gestureContextCount = 0;
// Inferred from unchanged actual map identity across the natural raw15 call,
// together with the source's allocate-before-release/no-restore detach body.
mainValues.events.gestureMapExclusive = true;
mainValues.pendingChildren = std::vector<engine::WidgetPendingChildObservation>{
    {1, {}}, {2, {}}, {3, {}}, {4, {}}, {5, {}}, {0, manager->widgetState()}};
// Manager values are unnecessary for first-parent consumption; leave unknown.
widgetValues.freshPrivateLocale = engine::WidgetLocaleValue{391, 0};
widgetValues.applicationModalState = false;
widgetValues.applicationStylesheetLength = 0;
widgetValues.applicationEvents.present = true;
widgetValues.applicationEvents.closing = false;
widgetValues.applicationEvents.internalCallbackCount = 0;
widgetValues.applicationEvents.filters = std::vector<engine::WidgetApplicationEventObservation::Filter>{
    {engine::WidgetApplicationEventObservation::Filter::Kind::WindowsVistaStyle, true}};
widgetValues.applicationEvents.gestureManagerPresent = true;
widgetValues.freshWidgetEvents.objectFilterCount = 0;
widgetValues.freshWidgetEvents.sameApplicationThread = true;
widgetValues.freshWidgetEvents.layoutPresent = false;
widgetValues.freshWidgetEvents.gestureContextCount = 0;
widgetValues.freshSendChildEvents = true;
widgetValues.freshFieldD0Present = false;
return widgetValues;

    }
    SplitViewConstructionReport start(const WidgetRuntimeValueObservation& v) {
        runtime->observeStartupValues(v);
        return manager->ensureDefaultSplitView(observed);
    }
};
}

TEST_CASE(view_manager_values_bind_shared_data_and_keep_value_words_local)
{
    ValueFixture f;
    auto v = f.values();
    f.runtime->observeStartupValues(v);
    REQUIRE(f.main->widgetState()->font());
    CHECK(f.main->widgetState()->font()->data == f.runtime->applicationFont()->data);
    CHECK(f.main->widgetState()->font()->resolveMask == 0);
    CHECK(f.runtime->applicationFont()->resolveMask == 51);
    CHECK(f.main->widgetState()->palette()->data == f.runtime->applicationPalette()->data);
    CHECK(f.main->widgetState()->palette()->groupResolveWord == 0);
    CHECK(f.runtime->applicationPalette()->groupResolveWord == 13893616);
    CHECK(!f.manager->widgetState()->font());
    CHECK(!f.manager->pendingSplitView());
}

TEST_CASE(view_manager_values_invalid_alias_binding_is_atomic_and_retryable)
{
    ValueFixture f;
    auto v = f.values(); v.mainViewValues.palette->dataAlias = 99;
    refused([&] { f.runtime->observeStartupValues(v); });
    CHECK(!f.main->widgetState()->font() && !f.runtime->applicationFont());
    v.mainViewValues.palette->dataAlias = 1;
    f.runtime->observeStartupValues(v);
    CHECK(f.main->widgetState()->font().has_value());
    refused([&] { f.runtime->observeStartupValues(v); });
}

TEST_CASE(view_manager_values_reject_foreign_owner_and_pending_native_identity)
{
    ValueFixture f, other;
    auto v = f.values(); v.manager = other.manager;
    refused([&] { f.runtime->observeStartupValues(v); });
    v = f.values(); v.mainViewValues.pendingChildren->back().knownWidget = other.manager->widgetState();
    refused([&] { f.runtime->observeStartupValues(v); });
    CHECK(!f.runtime->applicationFont());
}

TEST_CASE(view_manager_values_reject_duplicate_data_and_inconsistent_cache_population)
{
    ValueFixture f;
    auto v = f.values(); v.fontData.push_back({1, 168});
    refused([&] { f.runtime->observeStartupValues(v); });
    v = f.values(); v.applicationFont.classCachePresent = false;
    refused([&] { f.runtime->observeStartupValues(v); });
    CHECK(!f.main->widgetState()->font());
}

TEST_CASE(view_manager_values_missing_cache_retains_focus_and_fresh_default_words)
{
    ValueFixture f;
    auto v = f.values(); v.applicationFont.classes.reset();
    auto r = f.start(v);
    auto w = f.manager->pendingSplitView()->widgetState();
    CHECK(r.stage == SplitViewConstructStage::ParentFontPreparationPending);
    CHECK(w->font()->data == f.runtime->applicationFont()->data && w->font()->resolveMask == 0);
    CHECK(w->palette()->data == f.runtime->applicationPalette()->data && w->palette()->groupResolveWord == 0);
    CHECK(w->inSetParent() && w->parentOwnsChild());
    CHECK(f.main->widgetState()->focusPreviousIdentity() == f.manager->pendingSplitView().get());
}

TEST_CASE(view_manager_values_exact_class_key_precedes_inheritance_order)
{
    ValueFixture f;
    auto v = f.values(); v.fontData.push_back({2, 168});
    v.applicationFont.classes = std::vector<WidgetFontClassObservation>{{"QObject", {2, 0}}, {"QWidget", {1, 0}}};
    CHECK(f.start(v).stage == SplitViewConstructStage::WidgetFontDevicePending);
}

TEST_CASE(view_manager_values_first_inherited_class_keeps_different_data_guard)
{
    ValueFixture f;
    auto v = f.values(); v.fontData.push_back({2, 168});
    v.applicationFont.classes = std::vector<WidgetFontClassObservation>{{"QPaintDevice", {2, 0}}, {"QObject", {1, 0}}};
    CHECK(f.start(v).stage == SplitViewConstructStage::ParentFontPreparationPending);
}

TEST_CASE(view_manager_values_parent_palette_write_precedes_different_class_guard)
{
    ValueFixture f;
    auto v = f.values(); v.mainViewValues.palette->groupResolveWord = 9;
    v.applicationPalette.classes->push_back({"QWidget", {2, 0}});
    CHECK(f.start(v).stage == SplitViewConstructStage::ParentPaletteResolvePending);
    CHECK(f.main->widgetState()->palette()->groupResolveWord == 0);
    CHECK(f.runtime->applicationPalette()->groupResolveWord == 13893616);
    CHECK(f.main->widgetState()->pendingChildren()->size() == 6);
    CHECK(f.manager->ensureDefaultSplitView(f.observed).stage == SplitViewConstructStage::ParentPaletteResolvePending);
}

TEST_CASE(view_manager_values_palette_candidate_group_is_not_assigned_to_child)
{
    ValueFixture f;
    auto v = f.values(); v.mainViewValues.attributesLow = 65537;
    auto r = f.start(v);
    CHECK(r.stage == SplitViewConstructStage::ParentEnabledPending);
    CHECK(f.main->widgetState()->palette()->groupResolveWord == 1);
    CHECK(f.manager->pendingSplitView()->widgetState()->palette()->groupResolveWord == 0);
}

TEST_CASE(view_manager_values_palette_unknown_modal_stops_before_group_write)
{
    ValueFixture f;
    auto v = f.values(); v.applicationModalState.reset(); v.mainViewValues.palette->groupResolveWord = 9;
    CHECK(f.start(v).stage == SplitViewConstructStage::ParentPalettePreparationPending);
    CHECK(f.main->widgetState()->palette()->groupResolveWord == 9);
}

TEST_CASE(view_manager_values_unequal_locale_stops_after_real_parent_palette_effect)
{
    ValueFixture f;
    auto v = f.values(); v.mainViewValues.locale->numberOptions = 1;
    v.mainViewValues.palette->groupResolveWord = 9;
    CHECK(f.start(v).stage == SplitViewConstructStage::ParentLocalePending);
    CHECK(f.main->widgetState()->palette()->groupResolveWord == 0);
    CHECK(f.manager->pendingSplitView()->widgetState()->locale()->numberOptions == 0);
    CHECK(f.main->widgetState()->pendingChildren()->size() == 6);
}

TEST_CASE(view_manager_values_unknown_receiver_filter_preserves_pre_event_prefix)
{
    ValueFixture f;
    auto v = f.values(); v.mainViewValues.events.objectFilterCount.reset();
    v.mainViewValues.palette->groupResolveWord = 9;
    CHECK(f.start(v).stage == SplitViewConstructStage::ParentChildAddedPending);
    CHECK(f.main->widgetState()->palette()->groupResolveWord == 0);
    CHECK(f.main->widgetState()->pendingChildren()->size() == 6);
}

TEST_CASE(view_manager_values_empty_pending_population_retains_posted_event_boundary)
{
    ValueFixture f;
    auto v = f.values(); v.mainViewValues.pendingChildren->clear();
    CHECK(f.start(v).stage == SplitViewConstructStage::ParentPendingChildrenPending);
    CHECK(f.main->widgetState()->pendingChildren()->empty());
    CHECK(f.manager->pendingSplitView()->widgetState()->inSetParent());
}

TEST_CASE(view_manager_values_raw15_guard_retains_ordered_actual_weak_append_once)
{
    ValueFixture f;
    auto v = f.values(); v.mainViewValues.events.gestureMapExclusive.reset();
    CHECK(f.start(v).stage == SplitViewConstructStage::ParentChangeEventPending);
    auto split = f.manager->pendingSplitView();
    const auto& list = *f.main->widgetState()->pendingChildren();
    REQUIRE(list.size() == 7);
    CHECK(list.front().opaqueAlias == 1);
    CHECK(list[5].knownWidget.lock() == f.manager->widgetState());
    CHECK(list.back().knownWidget.lock() == split->widgetState());
    CHECK(f.manager->ensureDefaultSplitView(f.observed).stage == SplitViewConstructStage::ParentChangeEventPending);
    CHECK(list.size() == 7 && split->widgetState()->inSetParent());
}

TEST_CASE(view_manager_values_dirty_prefix_precedes_unknown_graphics_effect)
{
    ValueFixture f;
    auto v = f.values(); v.mainViewValues.graphicsEffectPresent.reset();
    CHECK(f.start(v).stage == SplitViewConstructStage::ParentDirtyEffectsPending);
    auto child = f.manager->pendingSplitView()->widgetState();
    CHECK(child->opaqueChildrenDirty() == true);
    CHECK(child->inSetParent() && f.main->widgetState()->pendingChildren()->size() == 7);
}

TEST_CASE(view_manager_values_dirty_propagation_updates_actual_ancestor_only)
{
    ValueFixture f;
    auto v = f.values(); v.mainViewValues.opaqueChildrenDirty = false;
    v.managerValues.opaqueChildrenDirty = false;
    CHECK(f.start(v).stage == SplitViewConstructStage::WidgetFontDevicePending);
    CHECK(f.main->widgetState()->opaqueChildrenDirty() == true);
    CHECK(f.manager->widgetState()->opaqueChildrenDirty() == false);
}

TEST_CASE(view_manager_values_supported_parent_return_keeps_next_constructor_boundary)
{
    ValueFixture f;
    auto r = f.start(f.values());
    auto split = f.manager->pendingSplitView(); auto child = split->widgetState();
    CHECK(r.stage == SplitViewConstructStage::WidgetFontDevicePending && !r.completed);
    CHECK(!child->inSetParent() && child->parentOwnsChild());
    CHECK(child->opaqueChildrenDirty() == true && child->opaque() == false);
    CHECK(child->palette()->groupResolveWord == 0 && child->font()->resolveMask == 0);
    CHECK(child->attributesLow() == 0);
    CHECK(!split->orientation() && !split->splitViewName());
    CHECK(!f.manager->nativeLayoutResolved() && !f.main->privateState().manager());
    CHECK(f.main->widgetState()->knownChildSuffix().size() == 2);
    CHECK(f.main->widgetState()->pendingChildren()->size() == 7);
    CHECK(f.manager->ensureDefaultSplitView(f.observed).stage == SplitViewConstructStage::WidgetFontDevicePending);
    CHECK(f.main->widgetState()->pendingChildren()->size() == 7);
}

TEST_CASE(view_manager_values_native_release_invalidates_weak_entries_and_value_owners)
{
    ValueFixture f;
    f.start(f.values());
    auto split = f.manager->pendingSplitView();
    auto widget = split->widgetState();
    auto data = widget->font()->data;
    auto pending = *f.main->widgetState()->pendingChildren();
    f.main->releaseNativeOwner();
    CHECK(!widget->nativeOwnerAlive() && !widget->font() && !widget->palette());
    CHECK(!f.runtime->hasLiveRegistration(split.get()));
    CHECK(data == f.runtime->applicationFont()->data);
    CHECK(!pending.back().knownWidget.lock()->nativeOwnerAlive());
    refused([&] { f.runtime->observeStartupValues(f.values()); });
}

TEST_CASE(widget_component_observations_do_not_replace_live_name_or_geometry)
{
    auto owner = std::make_shared<MainViewState>();
    auto widget = owner->widgetState();
    widget->observeObjectName(qt::String());
    widget->assignObjectName(qt::L("current"));
    refused([&] { widget->observeObjectName(qt::L("stale")); });
    CHECK(widget->objectName()->utf8() == "current");
    std::array<int, 4> initial{5, 7, 100, 200}, changed{5, 7, 481, 396};
    widget->observeGeometryEndpoints(initial);
    widget->assignGeometryEndpoints(changed);
    refused([&] { widget->observeGeometryEndpoints(initial); });
    CHECK(widget->geometryEndpoints() == changed);
}

TEST_CASE(widget_component_sparse_attributes_do_not_invent_other_bits)
{
    auto owner = std::make_shared<MainViewState>();
    auto widget = owner->widgetState();
    CHECK(!widget->extendedAttributes());
    widget->assignAttributeBit(42, true);
    widget->assignAttributeBit(35, true);
    CHECK(widget->attributeBit(42) == true && widget->attributeBit(35) == true);
    CHECK(!widget->attributeBit(60));
    CHECK(!widget->extendedAttributes());
    refused([&] { widget->observeAttributeBit(42, false); });
    widget->assignAttributeBit(42, false);
    CHECK(widget->attributeBit(42) == false && widget->attributeBit(35) == true);
}

TEST_CASE(widget_component_dynamic_property_lists_preserve_allocation_and_append_order)
{
    auto owner = std::make_shared<MainViewState>();
    auto widget = owner->widgetState();
    refused([&] { widget->appendDynamicPropertyName("sizeGripEnabled"); });
    widget->observeEmptyDynamicBooleanProperties(false);
    CHECK(widget->dynamicBooleanProperties().extraDataAllocated == false);
    widget->allocateDynamicBooleanProperties();
    CHECK(widget->dynamicBooleanProperties().extraDataAllocated == true);
    widget->appendDynamicPropertyName("sizeGripEnabled");
    CHECK(widget->dynamicBooleanProperties().names->size() == 1);
    CHECK(widget->dynamicBooleanProperties().values->empty());
    refused([&] { widget->appendDynamicPropertyName("outOfOrder"); });
    widget->appendDynamicPropertyBoolean(false);
    CHECK(widget->dynamicBooleanProperties().values->size() == 1);
    CHECK(!widget->dynamicBooleanProperties().values->front());
    refused([&] { widget->observeEmptyDynamicBooleanProperties(true); });
    CHECK(widget->dynamicBooleanProperties().names->front() == "sizeGripEnabled");
}

TEST_CASE(widget_component_native_release_prevents_all_live_storage_writes)
{
    auto owner = std::make_shared<MainViewState>();
    auto widget = owner->widgetState();
    widget->assignObjectName(qt::L("retained diagnostic"));
    owner.reset();
    CHECK(!widget->nativeOwnerAlive());
    refused([&] { widget->assignObjectName(qt::L("changed")); });
    refused([&] { widget->assignAttributeBit(35, true); });
    refused([&] { widget->assignGeometryEndpoints({0, 0, 1, 1}); });
    refused([&] { widget->observeEmptyDynamicBooleanProperties(false); });
    CHECK(widget->objectName()->utf8() == "retained diagnostic");
}

TEST_CASE(grid_parented_allocation_preserves_partial_constructor_words)
{
    auto owner = std::make_shared<MainViewState>();
    auto grid = GridLayoutState::beginParentedConstruction(owner->widgetState());
    CHECK(grid->constructionProfile() == GridConstructionProfile::ParentedPending);
    CHECK(grid->rowCount() == 0 && grid->columnCount() == 0);
    CHECK(grid->layoutFlags() == 6 && grid->layoutFlagsKnownMask() == 0x0fu);
    CHECK(grid->dirtyFlags() == 4 && grid->dirtyFlagsKnownMask() == 0x17u);
    const std::array<int, 4> margins{-1, -1, -1, -1}, rectangle{0, 0, -1, -1};
    CHECK(grid->contentsMargins() == margins && grid->geometryEndpoints() == rectangle);
    CHECK(grid->cachedSizeValue() == -1 && grid->items().empty());
    CHECK(!grid->isTopLevelLayout());
}

TEST_CASE(grid_proposed_parent_does_not_commit_thread_membership_or_layout)
{
    auto owner = std::make_shared<MainViewState>();
    auto runtime = std::make_shared<WidgetRuntimeState>(true, "pending grid fixture");
    owner->bindWidgetRuntime(runtime);
    auto widget = owner->widgetState();
    auto before = widget->knownChildSuffix().size();
    auto grid = GridLayoutState::beginParentedConstruction(widget);
    CHECK(!grid->parentWidget() && !grid->threadState() && !grid->parentViewManager());
    CHECK(!widget->installedLayout() && !widget->layoutPresence());
    CHECK(widget->knownChildSuffix().size() == before);
    owner->releaseNativeOwner();
    CHECK(grid->nativeOwnerAlive()); // cursor-owned allocation is not yet a child
    refused([&] { GridLayoutState::beginParentedConstruction(widget); });
    refused([&] { GridLayoutState::beginParentedConstruction(nullptr); });
}

TEST_CASE(grid_manager_installed_alias_is_the_actual_widget_alias)
{
    doe::SessionNotificationState state;
    auto owner = std::make_shared<MainViewState>();
    auto manager = owner->constructManagerChild(state);
    auto runtime = std::make_shared<WidgetRuntimeState>(true, "manager grid fixture");
    owner->bindWidgetRuntime(runtime);
    refused([&] { manager->ensureGridLayout(); });
    auto grid = manager->installedWidgetLayout();
    REQUIRE(grid);
    CHECK(grid == manager->widgetState()->installedLayout());
    CHECK(manager->widgetState()->layoutPresence() == true);
    CHECK(grid->parentWidget() == manager->widgetState());
    CHECK(grid->threadState() == runtime->threadState());
    CHECK(grid->constructionProfile() == GridConstructionProfile::ManagerReady);
    CHECK(grid->rowCount() == 1 && grid->layoutFlagsKnownMask() == 0xffffffffu);
    CHECK(!manager->cachedGridLayout());
    WidgetReceiverEventObservation stale;
    stale.layoutPresent = false;
    refused([&] { manager->widgetState()->observeEventReceiver(stale, WidgetEventReceiverKind::QWidget, "stale null layout"); });
    CHECK(manager->widgetState()->layoutPresence() == true);
}

TEST_CASE(grid_and_widget_keep_native_thread_reference_until_owner_release)
{
    doe::SessionNotificationState state;
    auto owner = std::make_shared<MainViewState>();
    auto manager = owner->constructManagerChild(state);
    auto runtime = std::make_shared<WidgetRuntimeState>(true, "native thread reference fixture");
    owner->bindWidgetRuntime(runtime);
    refused([&] { manager->ensureGridLayout(); });
    auto grid = manager->installedWidgetLayout();
    std::weak_ptr<WidgetThreadState> thread = runtime->threadState();
    runtime.reset();
    CHECK(!thread.expired());
    CHECK(grid->threadState() == manager->widgetState()->threadState());
    owner->releaseNativeOwner();
    CHECK(!grid->nativeOwnerAlive() && !grid->threadState());
    CHECK(!manager->widgetState()->threadState() && !manager->widgetState()->installedLayout());
    CHECK(thread.expired());
}

TEST_CASE(widget_child_event_flag_is_observed_on_actual_receiver_without_default)
{
    auto owner = std::make_shared<MainViewState>();
    auto widget = owner->widgetState();
    CHECK(!widget->receivesChildEvents());
    widget->observeReceivesChildEvents(true);
    widget->observeReceivesChildEvents(true);
    CHECK(widget->receivesChildEvents() == true);
    refused([&] { widget->observeReceivesChildEvents(false); });
    owner->releaseNativeOwner();
    refused([&] { widget->observeReceivesChildEvents(true); });
}

namespace {
WidgetConstructorTailObservation tailValues(ValueFixture& f) {
    WidgetConstructorTailObservation o;
    o.qualification = "isolated constructor tail fixture";
    o.expectedFontAlias = 1; o.fontScreenField3c = 0;
    o.displayDcAlias = 1; o.displayLogicalDpiY = 168;
    o.currentQThreadIsApplicationQThread = true;
    o.metricInputs = {{f.main->widgetState(), WidgetPaintDeviceKind::QWidget, 0, true}};
    o.currentWidgets = 173; o.maximumWidgets = 173; o.notifyDepth = 0;
    o.applicationCompileVersion = 0x40804; o.applicationAttributes = 0;
    return o;
}
WidgetPrivateConstructionObservation privateValues() {
    WidgetPrivateConstructionObservation o;
    o.qualification = "isolated exact private call inputs";
    o.fontData = {{1, 168, 0}}; o.paletteData = {{1}};
    o.brushData = {{23, 1, 255}}; o.paletteBrushes = {{1, 0, 10, 23}};
    o.defaultFont = WidgetFontValueObservation{1, 0};
    o.defaultPalette = WidgetPaletteValueObservation{1, 0};
    o.defaultLocale = WidgetLocaleValue{391, 0};
    o.applicationType = 1; o.applicationLayoutDirection = 0; o.applicationModalState = false;
    return o;
}
}

TEST_CASE(widget_private_inputs_share_data_without_committing_later_live_fields)
{
    auto a = std::make_shared<MainViewState>(), b = std::make_shared<MainViewState>();
    auto runtime = std::make_shared<WidgetRuntimeState>(true, "private input runtime");
    runtime->bindWidgetComponent(a->widgetState()); runtime->bindWidgetComponent(b->widgetState());
    runtime->observePrivateConstructionInputs(a->widgetState(), privateValues());
    runtime->observePrivateConstructionInputs(b->widgetState(), privateValues());
    auto* x = runtime->privateConstructionInputs(*a->widgetState());
    auto* y = runtime->privateConstructionInputs(*b->widgetState());
    REQUIRE(x && y);
    CHECK(x->defaultFont->data == y->defaultFont->data);
    CHECK(x->defaultPalette->data == y->defaultPalette->data);
    auto xBrush = x->defaultPalette->data->brushAt(0, 10);
    auto yBrush = y->defaultPalette->data->brushAt(0, 10);
    REQUIRE(xBrush && yBrush);
    CHECK(xBrush == yBrush && xBrush->observationAlias() == 23u);
    CHECK(xBrush->style() == 1 && xBrush->alphaHighByte() == 255u);
    CHECK(!x->defaultPalette->data->brushAt(3, 10));
    CHECK(!x->defaultPalette->data->brushAt(0, 20));
    CHECK(!a->widgetState()->font() && !a->widgetState()->palette() && !a->widgetState()->locale());
    CHECK(runtime->applicationType() == 1 && runtime->applicationLayoutDirection() == 0);
    CHECK(runtime->applicationModalState() == false);
}

TEST_CASE(widget_private_inputs_reject_foreign_control_block_and_stale_values)
{
    auto owner = std::make_shared<MainViewState>(); auto widget = owner->widgetState();
    auto runtime = std::make_shared<WidgetRuntimeState>(true, "private identity runtime");
    runtime->bindWidgetComponent(widget);
    auto foreign = std::shared_ptr<WidgetState>(widget.get(), [](WidgetState*) {});
    refused([&] { runtime->bindWidgetComponent(foreign); });
    refused([&] { runtime->observePrivateConstructionInputs(foreign, privateValues()); });
    CHECK(!runtime->privateConstructionInputs(*widget));
    runtime->observePrivateConstructionInputs(widget, privateValues());
    auto changed = privateValues(); changed.fontData[0].comparisonField38 = 96;
    refused([&] { runtime->observePrivateConstructionInputs(widget, changed); });
    CHECK(runtime->privateConstructionInputs(*widget)->defaultFont->data->comparisonField38() == 168u);
    owner->releaseNativeOwner();
    CHECK(!runtime->privateConstructionInputs(*widget));
}

TEST_CASE(widget_palette_brush_alias_and_slot_conflicts_preserve_shared_owner)
{
    auto owner = std::make_shared<MainViewState>(); auto widget = owner->widgetState();
    auto runtime = std::make_shared<WidgetRuntimeState>(true, "palette brush identity runtime");
    runtime->bindWidgetComponent(widget);
    auto original = privateValues();
    runtime->observePrivateConstructionInputs(widget, original);
    auto palette = runtime->privateConstructionInputs(*widget)->defaultPalette->data;
    auto brush = palette->brushAt(0, 10);
    REQUIRE(brush);

    auto conflictingValue = original;
    conflictingValue.brushData = {{23, 1, 254}};
    refused([&] { runtime->observePrivateConstructionInputs(widget, conflictingValue); });
    CHECK(palette->brushAt(0, 10) == brush);
    CHECK(brush->alphaHighByte() == 255u);

    auto conflictingSlot = original;
    conflictingSlot.brushData = {{24, 0, std::nullopt}};
    conflictingSlot.paletteBrushes = {{1, 0, 10, 24}};
    refused([&] { runtime->observePrivateConstructionInputs(widget, conflictingSlot); });
    CHECK(palette->brushAt(0, 10) == brush);
    CHECK(brush->observationAlias() == 23u);
}

TEST_CASE(widget_palette_brush_observations_reject_zero_aliases_and_out_of_range_slots)
{
    auto owner = std::make_shared<MainViewState>(); auto widget = owner->widgetState();
    auto runtime = std::make_shared<WidgetRuntimeState>(true, "palette brush bounds runtime");
    runtime->bindWidgetComponent(widget);
    auto zeroAlias = privateValues(); zeroAlias.brushData = {{0, 1, 255}};
    refused([&] { runtime->observePrivateConstructionInputs(widget, zeroAlias); });
    CHECK(!runtime->privateConstructionInputs(*widget));

    auto badGroup = privateValues(); badGroup.paletteBrushes = {{1, 3, 10, 23}};
    refused([&] { runtime->observePrivateConstructionInputs(widget, badGroup); });
    CHECK(!runtime->privateConstructionInputs(*widget));

    auto badRole = privateValues(); badRole.paletteBrushes = {{1, 0, 20, 23}};
    refused([&] { runtime->observePrivateConstructionInputs(widget, badRole); });
    CHECK(!runtime->privateConstructionInputs(*widget));
}

TEST_CASE(qstring_shared_null_identity_is_stable_within_one_root_and_separate_across_roots)
{
    auto first = std::make_shared<engine::WidgetRuntimeState>(true, "first process root");
    auto second = std::make_shared<engine::WidgetRuntimeState>(true, "second process root");
    const auto firstString = first->qstringSharedNull();
    const auto secondString = second->qstringSharedNull();
    CHECK(firstString && secondString && firstString != secondString);
    CHECK(first->qstringSharedNull() == firstString);
    CHECK(firstString->symbolRva == 0x2f0b30);
    CHECK(std::string(firstString->moduleSha256) ==
        "bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160");
    CHECK(firstString->referenceDelta == 0 && secondString->referenceDelta == 0);
    first->commitQStringSharedNullReference();
    first->commitQStringSharedNullReference();
    CHECK(firstString->referenceDelta == 2 && secondString->referenceDelta == 0);
    CHECK(first->qlistSharedNull()->referenceDelta == 0);
}

TEST_CASE(qobject_counter_unknown_base_retains_exact_per_object_delta)
{
    auto runtime = std::make_shared<WidgetRuntimeState>(true, "QObject counter fixture");
    int first, second;
    QObjectConstructionRecord a(&first), b(&second);
    runtime->commitQObjectConstruction(a);
    CHECK(a.committed() && !runtime->qobjectCount());
    CHECK(runtime->retainedQObjectConstructionDelta() == 1u);
    refused([&] { runtime->commitQObjectConstruction(a); });
    runtime->commitQObjectConstruction(b);
    runtime->observeQObjectCount(1893, "current observed value after two increments");
    CHECK(runtime->qobjectCount() == 1893u);
    CHECK(runtime->retainedQObjectConstructionDelta() == 2u);
    refused([&] { runtime->observeQObjectCount(1891, "stale earlier counter"); });
}

TEST_CASE(qobject_counter_wrap_and_actual_widget_binding_are_distinct_from_completion)
{
    auto owner = std::make_shared<MainViewState>(); auto widget = owner->widgetState();
    auto runtime = std::make_shared<WidgetRuntimeState>(true, "QObject wrap fixture");
    refused([&] { runtime->commitQObjectConstruction(*widget); });
    runtime->bindWidgetComponent(widget);
    runtime->observeQObjectCount(std::numeric_limits<std::uint32_t>::max(), "maximum bit pattern");
    runtime->commitQObjectConstruction(*widget);
    CHECK(runtime->qobjectCount() == 0u);
    CHECK(!runtime->hasLiveRegistration(owner.get()));
    refused([&] { runtime->commitQObjectConstruction(*widget); });
}

TEST_CASE(widget_tail_matching_font_preserves_actual_alias_and_retains_create_at_post)
{
    ValueFixture f; f.start(f.values()); auto split = f.manager->pendingSplitView();
    auto widget = split->widgetState(); auto data = widget->font()->data;
    f.runtime->observeConstructorTailValues(widget, tailValues(f));
    auto report = f.manager->ensureDefaultSplitView(f.observed);
    CHECK(report.stage == SplitViewConstructStage::WidgetPostPending && !report.completed);
    CHECK(widget->font()->data == data && widget->font()->resolveMask == 0);
    CHECK(widget->attributeBit(34) == true && widget->attributeBit(35) == true);
    CHECK(f.runtime->currentWidgetCount() == 174u && f.runtime->maximumWidgetCount() == 174u);
    REQUIRE(split->constructorTail()->createEvent());
    CHECK(split->constructorTail()->createEvent()->nativeAlive());
    CHECK(!split->constructorTail()->createEvent()->posted());
    CHECK(f.runtime->threadState()->notifyDepth() == 0u);
    f.manager->ensureDefaultSplitView(f.observed);
    CHECK(f.runtime->currentWidgetCount() == 174u);
    CHECK(!f.runtime->threadState()->postedEvents().populationKnown());
    CHECK(!split->orientation() && !split->splitViewName());
}

TEST_CASE(widget_tail_device_mismatch_guards_before_attributes_and_counts)
{
    ValueFixture f; f.start(f.values()); auto widget = f.manager->pendingSplitView()->widgetState();
    auto o = tailValues(f); o.displayLogicalDpiY = 96;
    f.runtime->observeConstructorTailValues(widget, o);
    f.manager->ensureDefaultSplitView(f.observed);
    CHECK(f.manager->pendingSplitView()->constructorTail()->report().stage == WidgetConstructorTailStage::FontDevice);
    CHECK(widget->attributeBit(34) == false && widget->attributeBit(35) == false);
    CHECK(f.runtime->currentWidgetCount() == 173u);
}

TEST_CASE(widget_tail_missing_count_preserves_prior_attributes_and_resumes_once)
{
    ValueFixture f; f.start(f.values()); auto split = f.manager->pendingSplitView(); auto widget = split->widgetState();
    auto o = tailValues(f); o.currentWidgets.reset(); o.maximumWidgets.reset();
    f.runtime->observeConstructorTailValues(widget, o);
    f.manager->ensureDefaultSplitView(f.observed);
    CHECK(split->constructorTail()->report().stage == WidgetConstructorTailStage::WidgetCounts);
    CHECK(widget->attributeBit(34) == true && !f.runtime->currentWidgetCount());
    f.runtime->observeConstructorTailValues(widget, tailValues(f));
    CHECK(f.manager->ensureDefaultSplitView(f.observed).stage == SplitViewConstructStage::WidgetPostPending);
    CHECK(f.runtime->currentWidgetCount() == 174u);
}

TEST_CASE(widget_tail_compatibility_guard_retains_increment_before_create)
{
    ValueFixture f; f.start(f.values()); auto split = f.manager->pendingSplitView(); auto widget = split->widgetState();
    auto o = tailValues(f); o.applicationAttributes = 1;
    f.runtime->observeConstructorTailValues(widget, o);
    f.manager->ensureDefaultSplitView(f.observed);
    CHECK(split->constructorTail()->report().stage == WidgetConstructorTailStage::Compatibility);
    CHECK(f.runtime->currentWidgetCount() == 174u && !split->constructorTail()->createEvent());
    f.manager->ensureDefaultSplitView(f.observed);
    CHECK(f.runtime->currentWidgetCount() == 174u);
}

TEST_CASE(widget_tail_counter_maximum_uses_native_signed_comparison)
{
    ValueFixture f; f.start(f.values()); auto widget = f.manager->pendingSplitView()->widgetState();
    auto o = tailValues(f); o.currentWidgets = 0x7fffffffu; o.maximumWidgets = 0x7fffffffu;
    f.runtime->observeConstructorTailValues(widget, o);
    f.manager->ensureDefaultSplitView(f.observed);
    CHECK(f.runtime->currentWidgetCount() == 0x80000000u);
    CHECK(f.runtime->maximumWidgetCount() == 0x7fffffffu);
}

TEST_CASE(widget_tail_rejects_foreign_component_and_stale_counter_observation)
{
    ValueFixture f; f.start(f.values()); auto widget = f.manager->pendingSplitView()->widgetState();
    auto foreign = std::shared_ptr<WidgetState>(widget.get(), [](WidgetState*) {});
    refused([&] { f.runtime->observeConstructorTailValues(foreign, tailValues(f)); });
    f.runtime->observeConstructorTailValues(widget, tailValues(f));
    f.manager->ensureDefaultSplitView(f.observed);
    refused([&] { f.runtime->observeConstructorTailValues(widget, tailValues(f)); });
    CHECK(f.runtime->currentWidgetCount() == 174u);
}

int main() { return testkit::run_all(); }
