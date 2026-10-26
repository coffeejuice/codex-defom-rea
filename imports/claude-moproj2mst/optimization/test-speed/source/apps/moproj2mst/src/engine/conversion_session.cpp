// Project/display lifetime and diagram receiver binding; not original Close replay.
#include "m2m/engine/conversion_session.hpp"

#include <algorithm>
#include "m2m/engine.hpp"
#include "m2m/mst/writer.hpp" // complete Project::registry's unique_ptr<OpClass>
#include "m2m/engine/boolean_lifecycle.hpp"
#include "m2m/engine/model_manager.hpp"
#include "m2m/engine/simcontrol300_owner.hpp"
#include "m2m/engine/editor_setup.hpp"
#include "m2m/engine/editor_base.hpp"
#include "m2m/engine/view_manager.hpp"
#include "m2m/engine/widget_events.hpp"
#include "m2m/engine/display_owner.hpp"
#include "m2m/engine/close_effects.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/doe_slice.hpp"
#include "m2m/doe_task_runtime.hpp"
#include "m2m/doe_task_factory.hpp"

namespace m2m::engine {
namespace {
[[noreturn]] void unsupported(const char* text)
{
    throw Error(M2M_E_UNSUPPORTED, text);
}

WidgetRuntimeValueObservation freshConverterWidgetValues(
    const std::shared_ptr<MainViewState>& mainView,
    const std::shared_ptr<ViewManagerState>& manager)
{
    // Probe003 first SplitView parent context, after manager/grid construction.
    // Aliases identify shared data within this runtime; they are not addresses.
    WidgetRuntimeValueObservation values;
    values.mainView = mainView;
    values.manager = manager;
    values.qualification = "SFTC11.0.2 probe003 first SplitView parent context; source-bound cache/value/receiver contract";
    values.fontData = {{1, 168}};
    values.paletteData = {{1}, {2}, {3}};
    values.applicationFont.defaultValue = WidgetFontValueObservation{1, 51};
    values.applicationFont.mutexPresent = true;
    values.applicationFont.classCachePresent = true;
    values.applicationFont.classes = std::vector<WidgetFontClassObservation>{};
    values.applicationPalette.defaultValue = WidgetPaletteValueObservation{1, 13893616};
    values.applicationPalette.classCachePresent = true;
    values.applicationPalette.classes = std::vector<WidgetPaletteClassObservation>{
        {"QMenuBar", {2, 13893616}}, {"QMenu", {3, 13893616}}};
    auto& main = values.mainViewValues;
    main.font = WidgetFontValueObservation{1, 0};
    main.palette = WidgetPaletteValueObservation{1, 0};
    main.locale = WidgetLocaleValue{391, 0};
    main.inheritedFontMask = main.inheritedPaletteMask = 0;
    main.attributesLow = 0x10000;
    main.extendedAttributes = std::array<std::uint32_t, 3>{{12, 4112, 0}};
    main.windowFlags = 0x0800f001u;
    main.opaqueChildrenDirty = true;
    main.opaque = true;
    main.graphicsEffectPresent = false;
    main.autoFillBackground = false;
    main.stylePresent = false;
    main.stylesheetLength = 0;
    main.proxyPresent = false;
    main.events.objectFilterCount = 0;
    main.events.sameApplicationThread = true;
    main.events.layoutPresent = false;
    main.events.gestureContextCount = 0;
    // Derived from unchanged map identity plus allocate-before-release/no-
    // restore source ordering; not a directly observed refcount.
    main.events.gestureMapExclusive = true;
    main.pendingChildren = std::vector<WidgetPendingChildObservation>{
        {1, {}}, {2, {}}, {3, {}}, {4, {}}, {5, {}}, {0, manager->widgetState()}};
    // Manager value payloads are not consumed by this first-parent prefix.
    values.freshPrivateLocale = WidgetLocaleValue{391, 0};
    values.applicationModalState = false;
    values.applicationStylesheetLength = 0;
    values.applicationEvents.present = true;
    values.applicationEvents.closing = false;
    values.applicationEvents.internalCallbackCount = 0;
    values.applicationEvents.filters = std::vector<WidgetApplicationEventObservation::Filter>{
        {WidgetApplicationEventObservation::Filter::Kind::WindowsVistaStyle, true}};
    values.applicationEvents.gestureManagerPresent = true;
    values.freshWidgetEvents.objectFilterCount = 0;
    values.freshWidgetEvents.sameApplicationThread = true;
    values.freshWidgetEvents.layoutPresent = false;
    values.freshWidgetEvents.gestureContextCount = 0;
    values.freshSendChildEvents = true;
    values.freshFieldD0Present = false;
    return values;
}

SplitViewStartupObservation freshConverterSplitProfile(const std::shared_ptr<ViewManagerState>& manager)
{
    // Probe002 first parent_enter: actual MainView state1 and globals3. This
    // owned fresh-launch profile does not establish every user environment.
    SplitViewStartupObservation profile;
    profile.receiver = manager;
    profile.qualification = "SFTC11.0.2 fresh startup: wave3-original-splitter-probe-002-receipt, first parent_enter";
    profile.parentAttributesLow = 0x00010000u;
    profile.parentExtendedAttributes = std::array<std::uint32_t, 3>{12, 4112, 0};
    profile.parentWindowFlags = 0x0800f001u;
    profile.parentTopExtraPresent = false;
    profile.parentThreadMatches = true;
    profile.childHookDataPresent = false;
    profile.parentExtraNativeChildren = false; //actual extra+5c bit5 in probe002
    profile.applicationCompileVersion = 0x40804;
    profile.applicationAttributes = 0;
    return profile;
}

GridStartupObservation freshConverterGridProfile(const std::shared_ptr<ViewManagerState>& manager)
{
    // The converter's fixed fresh SFTC11.0.2 startup profile, observed in
    // original-startup-probe003 and source-bound to the shipped Qt bodies.
    // This is not a claim about arbitrary host Qt applications or user filters.
    GridStartupObservation profile;
    profile.receiver = manager;
    profile.qualification = "SFTC11.0.2 fresh startup: wave3-original-grid-affinity-probe-receipt";
    profile.applicationPresent = true;
    profile.parentThreadMatches = true;
    profile.internalCallbackCount = 0;
    profile.applicationFilters = std::vector<GridApplicationFilter>{
        {GridApplicationFilter::Kind::WindowsVistaStyle, true}};
    profile.objectFilterCount = 0;
    profile.declarativeChildEventsEnabled = false;
    profile.childHookDataPresent = false;
    profile.currentLayoutFlags = 3;
    profile.currentWindowFlags = 0x0800f000u;
    return profile;
}
}

void DiagramEffectDriver::fitAndNotify(ConversionSession&)
{
    unsupported("diagram timer: original fit/view state and zoom notifications are not supplied");
}

void DiagramEffectDriver::clickAndUpdateMainView(ConversionSession&, const FlowItem&, FileEffects&)
{
    unsupported("diagram timer: complete click and main-view notification effects are not supplied");
}

ConversionSession::ConversionSession(DiagramEffectDriver& driver)
    : driver_(driver), receiver_(events_.createReceiver()),
      notificationState_(std::make_shared<doe::SessionNotificationState>()),
      primaryScene_(std::make_shared<PostGraphSceneState>(createSceneDisplayOwner())),
      secondaryScene_(std::make_shared<PostGraphSceneState>(createSceneDisplayOwner())),
      slicePlaneHeap_(std::make_shared<doe::SlicePlaneHeap>()),
      sliceConstructorRuntime_(std::make_shared<doe::SliceConstructorRuntime>()),
      taskAllocationStore_(std::make_shared<doe::TaskAllocationStore>()),
      widgetRuntime_(std::make_shared<WidgetRuntimeState>(true,
          "SFTC11.0.2 owned fresh startup: observed nonnull allWidgets registry; population incomplete")),
      taskFactoryRuntime_(std::make_shared<doe::TaskFactoryRuntime>(taskAllocationStore_, widgetRuntime_)),
      modelManager_(std::make_shared<ModelManager>(this)),
      mainView_(std::make_shared<MainViewState>())
{
    // Fixed shipped registration inventory:0111c0 has exactly one matching
    // 2cfbe0 call at011fcb. This qualifies the fresh fixed-startup profile;
    // arbitrary dynamic registrations are not implied by this binding.
    notificationState_->createTaskBaseRegistrations = doe::TaskFactoryRegistrationList{
        {{taskFactoryRuntime_, "slotCreateTaskBase"}},
        "SFTC11.0.2 fixed startup:348 direct registration calls; sole CreateTaskBase at140011fcb", true};
    const auto manager = mainView_->constructManagerChild(*notificationState_);
    mainView_->bindWidgetRuntime(widgetRuntime_);
    manager->ensureGridLayout(freshConverterGridProfile(manager));
    mainView_->observeInitialManagerFocusTail(
        "Probe002 first focus entry: MainView.previous is this actual manager; manager.next is MainView");
    widgetRuntime_->observeStartupValues(freshConverterWidgetValues(mainView_, manager));
    splitViewConstruction_ = std::make_unique<SplitViewConstructionReport>(
        manager->ensureDefaultSplitView(freshConverterSplitProfile(manager)));
    if (const auto split = manager->pendingSplitView()) {
        // Bind observed inputs to this SAME pending native component at its
        // source font/device boundary, never to a guessed future allocation.
        WidgetConstructorTailObservation tail;
        tail.qualification = "SFTC11.0.2 owned splitter004 first constructor tail; startup only";
        tail.expectedFontAlias = 1;
        tail.fontScreenField3c = 0;
        tail.displayDcAlias = 1;
        tail.displayLogicalDpiY = 168;
        tail.currentQThreadIsApplicationQThread = true;
        tail.metricInputs = {{split->widgetState(), WidgetPaintDeviceKind::QWidget, 0, false},
            {mainView_->widgetState(), WidgetPaintDeviceKind::QWidget, 0, true}};
        tail.currentWidgets = tail.maximumWidgets = 173;
        tail.notifyDepth = 0;
        tail.applicationCompileVersion = 0x40804;
        tail.applicationAttributes = 0;
        widgetRuntime_->observeConstructorTailValues(split->widgetState(), tail);
        *splitViewConstruction_ = manager->ensureDefaultSplitView(freshConverterSplitProfile(manager));
    }
    // The native aliases at0e9ce5/0e9cfc follow View_Default/Primary setup.
    // Keep them uncommitted while that constructor dependency is unresolved.
    // Project is the existing adapter scope; no full Qt startup claim is made.
    project_ = std::make_unique<Project>();
    project_->display = primaryScene_->displayOwner();
    scope_ = std::make_unique<ProjectScope>(*project_);
}

ConversionSession::~ConversionSession()
{
    // Adapter exception/final cleanup, NOT an original normal-Close replay.
    // No throwing effect driver or original dialog is invoked from a destructor.
    releaseProjectStorage();
    events_.destroyReceiver(receiver_);
    events_.dispatcherDestroyed();
    mainView_->releaseNativeOwner(); // also invalidates escaped diagnostic leases
    taskFactoryRuntime_->releaseAdapterScope(); // main-window adapter boundary, not normal Close
    taskAllocationStore_->releaseAdapterScope(); // fresh-process adapter boundary, not native Close
    // displayLease_ releases only after project-owned models have been freed.
}

Project& ConversionSession::project()
{
    if (!project_) unsupported("session project has already been released");
    return *project_;
}

Display* ConversionSession::displayIfCreated() noexcept
{
    return project_ ? project_->display.get() : displayLease_.get();
}

std::shared_ptr<ViewManagerState> ConversionSession::currentViewManagerOwner() const
{
    if (!viewManagerAliasesCommitted_)
        unsupported("Main-window view-manager aliases follow unrecovered View_Default/Primary construction");
    auto manager = viewManager_.lock();
    return manager && manager->nativeReceiverAlive() ? manager : nullptr;
}

void ConversionSession::beginEditor300Factory(Project& p, Operation& op, GuiModel& model)
{
    if (&p != project_.get() || op.project != &p || op.engineModel.get() != &model ||
        !p.dataDb || modelManager_->currentModel() != p.dataDb.get())
        unsupported("Editor300 factory needs actual selected Project, Operation and GuiModel ownership");
    if (editor300Factory_)
        unsupported("Editor300 replacement requires original widget-area leave/deferred-deletion ordering");
    editor300Factory_ = std::make_unique<simcontrol300::StagedEditor300>(
        simcontrol300::Editor300Binding{p, op, model, remesh_});
    // Bind the actual component to this process/session runtime. This stores
    // ownership only; it does not replay or certify QWidget construction.
    if (!editor300Factory_->bindWidgetRuntime({p, op, model, remesh_}, widgetRuntime_))
        unsupported("Editor300 could not bind the actual Session widget runtime");
    simcontrol300::EditorBaseContext base;
    base.receiver = editor300Factory_.get();
    base.qualification = "Actual Session editor owner; post-Open private-value aliases not bridged from startup";
    base.currentThread = widgetRuntime_->threadState();
    const auto report = editor300Factory_->continueNativeBase({p, op, model, remesh_}, base);
    if (report.issue != simcontrol300::EditorBaseIssue::PrivatePaletteUnavailable)
        unsupported("Editor300 base did not retain its expected private-palette dependency");
    // Native UI allocation at140eb27c5 follows returned QWidget AND application
    // wrappers. Retain their unresolved prefix; no setup allocation is made.
}

void ConversionSession::prepareProjectModel(const qt::String& databasePath)
{
    auto& p = project();
    if (modelManager_->size() || p.dataDb)
        unsupported("fresh Open model registration requires an empty manager and unbound Project");
    auto model = std::make_shared<DbHandle>(DbHandle::Kind::Mo);
    dbSetPath(*model, databasePath, false); //c79390 before native ID/list append
    modelManager_->appendCreatedModel(model);
    modelManager_->registerModelSignals(model);
    p.dataDb = std::move(model); //same native child passed into Project constructor
}

void ConversionSession::commitProjectModelConstruction()
{
    auto& p = project();
    if (!p.dataDb || p.dataDb->kind != DbHandle::Kind::Mo ||
        modelManager_->modelWithId(p.dataDb->modelId) != p.dataDb.get())
        unsupported("Project construction requires its actual registered Mo child");
    if (p.dataDb->moProject && p.dataDb->moProject != &p)
        unsupported("Mo child already owns a different Project");
    p.dataDb->moProject = &p;
}

void ConversionSession::commitProjectModelSelection()
{
    auto& p = project();
    if (!p.dataDb || p.dataDb->moProject != &p)
        unsupported("postOpen selection requires the completed actual Project binding");
    //c79ee0 resolves ordinal by identity. Model ID is not the ordinal.
    for (std::size_t ordinal = 0; ordinal < modelManager_->size(); ++ordinal)
        if (modelManager_->modelAt(static_cast<int>(ordinal)) == p.dataDb.get()) {
            modelManager_->selectOrdinal(static_cast<int>(ordinal));
            return;
        }
    unsupported("postOpen Project model is absent from the owning manager");
}

void ConversionSession::loadProjectRemesh(simcontrol300::RemeshIo& io)
{
    const auto report = simcontrol300::loadRemeshAtProjectOpen(
        remesh_, io, simcontrol300::NumericLocale::KnownC);
    if (report.outcome != simcontrol300::ApplyResult::Completed)
        unsupported("Open remesh input reaches an unrecovered reader profile");
}

ConversionSession::TimerId ConversionSession::commitNonNullDiagramBinding(SubProject& sub)
{
    auto& p = project();
    const bool owned = std::any_of(p.subs.begin(), p.subs.end(),
                                 [&sub](const auto& entry) { return entry.get() == &sub; });
    if (!owned || sub.project != &p)
        unsupported("diagram binding does not belong to the live session project");
    boundDiagramModel_ = &sub;
    return events_.modelAttached(receiver_, true);
}

void ConversionSession::commitNullDiagramBinding() noexcept
{
    boundDiagramModel_ = nullptr;
    // Null setModel does not register, cancel or coalesce pending timers.
}

ConversionSession::DispatchResult ConversionSession::dispatchAt(
    const PumpPoint& point, const std::optional<DueObservation>& observation, FileEffects& effects)
{
    if (point.sourceSite.empty() || !observation || observation->qualification.empty())
        unsupported("diagram pump: due-event observation is unavailable");
    if (point.sourceSite != observation->point.sourceSite ||
        point.occurrence != observation->point.occurrence)
        unsupported("diagram pump: observation belongs to a different source occurrence");
    DispatchResult result;
    for (const auto timer : observation->dueInOrder) {
        // deliver removes the timer BEFORE fit/click. If either effect is
        // unsupported or throws, the consumed timer remains consumed, and the
        // adapter error propagates. Do not retry it as though it were pending.
        const auto outcome = events_.deliver(timer, [&](Events::ReceiverId receiver) {
            if (receiver != receiver_) unsupported("unexpected diagram receiver identity");
            modelUpdated(effects);
        });
        result.emplace_back(timer, outcome);
    }
    return result;
}

void ConversionSession::modelUpdated(FileEffects& effects)
{
    driver_.fitAndNotify(*this);
    // The driver may rebind or release the project during a zoom signal. Read
    // current binding only after it returns. No registration-time item is kept.
    auto* sub = boundDiagramModel_;
    if (!sub) return;
    auto& p = project();
    const bool owned = std::any_of(p.subs.begin(), p.subs.end(),
                                 [sub](const auto& entry) { return entry.get() == sub; });
    if (!owned) unsupported("diagram model was removed without its null-binding transition");
    const auto& flow = sub->flow;
    if (flow.activeItem < 0 || static_cast<std::size_t>(flow.activeItem) >= flow.items.size()) return;
    const auto* item = flow.items[static_cast<std::size_t>(flow.activeItem)].get();
    if (!item) return;
    driver_.clickAndUpdateMainView(*this, *item, effects);
}

void ConversionSession::applyBooleanSelection(const FlowItem& item, FileEffects& effects)
{
    auto& p = project();
    auto* sub = boundDiagramModel_;
    if (!sub || activeSubProject(p) != sub || p.loadingSub)
        unsupported("Boolean timer selection is outside the recovered active binding");
    const auto& flow = sub->flow;
    if (flow.activeItem < 0 || static_cast<std::size_t>(flow.activeItem) >= flow.items.size() ||
        flow.items[static_cast<std::size_t>(flow.activeItem)].get() != &item)
        unsupported("Boolean timer selection changed before its recovered editor leaf");
    auto* operation = operationById(p, item.opId);
    if (!operation) unsupported("diagram item has no recovered operation");
    // Existing helper checks same-current, loaded Type0 Boolean and shown Object
    // editor, then performs real apply/replacement/refill. No synthetic counter.
    reselectBooleanObjectEditor(p, *operation, effects);
}

void ConversionSession::releaseProjectAfterCloseEffects()
{
    if (boundDiagramModel_)
        unsupported("Close cleanup reached model destruction before diagram null binding");
    releaseProjectStorage();
    // Receiver/dispatcher and pending timers remain live. A later observed due
    // callback still performs fit, then sees the null model.
}

void ConversionSession::releaseProjectStorage() noexcept
{
    boundDiagramModel_ = nullptr;
    if (!project_) return;
    // Captures lazily-created actual Display even if Open/Save threw before an
    // explicit sync. This additional shared owner prevents Project's reverse
    // member destruction from freeing Display before older operation models.
    displayLease_ = project_->display;
    scope_.reset(); // remove the adapter's thread-local Project pointer
    const auto model = project_->dataDb;
    const auto* releasedProject = project_.get();
    project_.reset();
    if (model && model->moProject == releasedProject) model->moProject = nullptr;
}

} // namespace m2m::engine
