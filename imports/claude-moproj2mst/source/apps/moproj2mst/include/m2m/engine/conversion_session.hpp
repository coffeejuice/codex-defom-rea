// Session ownership and explicit diagram event projection.
// Original Close replay and real fit/notification effects have separate guards.
#pragma once

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "m2m/engine/lifecycle_events.hpp"
#include "m2m/engine/simcontrol300_remesh.hpp"
#include "m2m/qt4.hpp"

namespace m2m {
namespace doe { struct SessionNotificationState; class SlicePlaneHeap; struct SliceConstructorRuntime; class TaskAllocationStore; class TaskFactoryRuntime; class FlowReceiverWitnessRegistry; class DoeTableOwnerState; }
struct Project;
struct ProjectScope;
struct Operation;
struct SubProject;
struct FlowItem;
namespace engine {
struct Display;
struct FileEffects;
struct GuiModel;
class ModelManager;
class SessionModelManagerReadReceipt;
class OperationClassRegistry;
class MainViewState;
class ViewManagerState;
class WidgetRuntimeState;
struct SplitViewConstructionReport;
class PostGraphSceneState;
class BooleanTraceOwnerScope;
class ProcessCodecOwner;
class BooleanBoundTraceDue;
struct BooleanTraceBindingContext;
struct BooleanPumpTrace;
struct BooleanTraceDueResult;
namespace simcontrol300 { class StagedEditor300; class EditorSetupState; }

class ConversionSession;

// Each hook represents actual recovered effects. A trace entry or counter alone
// does not implement either boundary. The default implementation refuses.
struct DiagramEffectDriver {
    virtual ~DiagramEffectDriver() = default;
    // 183160 -> 174120: fit, invalidate/update, zoom state and both zoom signals.
    // Callbacks may change session binding. The caller resolves it AFTER return.
    virtual void fitAndNotify(ConversionSession&);
    // Complete signalDiagItemClicked delivery followed by UpdateMainView.
    // A qualified driver invokes session.applyBooleanSelection at its recovered
    // editor leaf. It must preserve all preceding/following recipients/effects.
    virtual void clickAndUpdateMainView(ConversionSession&, const FlowItem&, FileEffects&);
};

struct PumpPoint {
    // Use the precise source site/occurrence, e.g. 140098129, rather than "Save".
    std::string sourceSite;
    unsigned occurrence = 0;
};

struct DueObservation {
    enum class Origin { OriginalTrace, DeclaredAdapterSchedule };
    PumpPoint point;
    Origin origin;
    std::string qualification; // trace receipt or explicit adapter-profile identity
    std::vector<DiagramLifecycleEvents::TimerId> dueInOrder;
};

class ConversionSession final {
public:
    using Events = DiagramLifecycleEvents;
    using TimerId = Events::TimerId;
    using DispatchResult = std::vector<std::pair<TimerId, Events::Delivery>>;

    explicit ConversionSession(DiagramEffectDriver& driver);
    ConversionSession(DiagramEffectDriver& driver, ProcessCodecOwner& processCodecOwner);
    ~ConversionSession();
    ConversionSession(const ConversionSession&) = delete;
    ConversionSession& operator=(const ConversionSession&) = delete;

    Project& project();
    Project* liveProject() noexcept { return project_.get(); }
    // Borrowed explicit process identity; legacy packet Sessions remain unbound.
    // This owner must outlive Session, Project release and synchronous contexts.
    const ProcessCodecOwner* processCodecOwner() const noexcept { return processCodecOwner_; }
    Display* displayIfCreated() noexcept;
    // Distinct SfScene identities with their own embedded Display states.
    // These partial bindings do not certify full native scene construction or
    // current item population, visibility, geometry or Qt event effects.
    std::shared_ptr<PostGraphSceneState> primarySceneOwner() const noexcept { return primaryScene_; }
    std::shared_ptr<PostGraphSceneState> secondarySceneOwner() const noexcept { return secondaryScene_; }
    std::shared_ptr<ModelManager> modelManagerOwner() const noexcept { return modelManager_; }
    // Candidate-only existing owner read at fresh Open, not native factory,
    // class/cast or main-window readiness. Absent before that source edge.
    std::shared_ptr<const SessionModelManagerReadReceipt> modelManagerOpenReadReceipt() const noexcept {
        return modelManagerOpenRead_;
    }
    // One main-window-lifetime owner for the lazy DOE table cache. Its native
    // fields start unavailable; this binding alone delivers no Qt signal.
    doe::DoeTableOwnerState* doeTableOwner() const noexcept { return doeTableOwner_.get(); }
    // Adapter allocation retention for one original fresh-process boundary.
    // DbHandle plane lists borrow records and never own or release this heap.
    std::shared_ptr<doe::SlicePlaneHeap> slicePlaneHeapOwner() const noexcept { return slicePlaneHeap_; }
    // Original shared color scratch, independent of any DB/View lifetime.
    std::shared_ptr<doe::SliceConstructorRuntime> sliceConstructorRuntimeOwner() const noexcept { return sliceConstructorRuntime_; }
    // Retain original raw Task/Session allocations across Type2 list clears.
    // Adapter lifetime only; native factory initialization remains guarded.
    std::shared_ptr<doe::TaskAllocationStore> taskAllocationStoreOwner() const noexcept { return taskAllocationStore_; }
    // Main-window lifetime holder for the actual lazy native factory field.
    // Creating the holder does not construct its factory or prototype objects.
    std::shared_ptr<doe::TaskFactoryRuntime> taskFactoryRuntimeOwner() const noexcept { return taskFactoryRuntime_; }
    std::shared_ptr<MainViewState> mainViewOwner() const noexcept { return mainView_; }
    std::shared_ptr<WidgetRuntimeState> widgetRuntimeOwner() const noexcept { return widgetRuntime_; }
    // Explicit partial startup result; native manager aliases remain guarded.
    const SplitViewConstructionReport* defaultSplitViewConstructionReport() const noexcept {
        return splitViewConstruction_.get();
    }
    // Native mainwindow+148 is committed only AFTER View_Default/Primary setup.
    // Refuse while those constructor effects are unresolved, never treat the
    // uncommitted pointer as a source-proven null current manager.
    std::shared_ptr<ViewManagerState> currentViewManagerOwner() const;
    // Fresh01ca30: construct/register Mo child BEFORE project init/read. These
    // commits do not replace surrounding dialogs, receiver effects or failures.
    void prepareProjectModel(const qt::String& databasePath);
    void commitProjectModelConstruction(); //044a90 stores child+19c0 after ctor
    void commitProjectModelSelection(); //02a0e0/02a9b0 selects actual list ordinal
    // Retain the parentless factory prefix under the actual mainwindow's future
    // widget-area lifetime. This is not attachment or completed UI construction.
    void beginEditor300Factory(Project&, Operation&, GuiModel&);
    const simcontrol300::StagedEditor300* editor300FactoryAttempt() const noexcept {
        return editor300Factory_.get();
    }
    // Bounded consumed-control projection retained at the setupUi guard. It
    // does not certify full setup, native widget allocation or later callbacks.
    const simcontrol300::EditorSetupState* editor300SetupProjection() const noexcept {
        return editor300Setup_.get();
    }
    // Original143d91238 process storage survives normal Project destruction.
    simcontrol300::RemeshState& remeshState() noexcept { return remesh_; }
    void loadProjectRemesh(simcontrol300::RemeshIo&);
    std::shared_ptr<doe::SessionNotificationState> sessionNotificationStateOwner() const noexcept {
        return notificationState_;
    }
    std::shared_ptr<doe::FlowReceiverWitnessRegistry> flowReceiverWitnessRegistryOwner() const noexcept {
        return flowReceiverWitnessRegistry_;
    }
    const auto& pending() const noexcept { return events_.pending(); }

    // Invoke only at the respective recovered setModel commit point. This
    // commits the binding/timer portion, not the other 170430 view mutations.
    // Every nonnull call, including the same model twice, registers a timer.
    TimerId commitNonNullDiagramBinding(SubProject&);
    void commitNullDiagramBinding() noexcept;

    // nullopt means unavailable evidence, never an empty due list. An explicit
    // empty list means zero delivered events in this declared profile only.
    DispatchResult dispatchAt(const PumpPoint&, const std::optional<DueObservation>&,
                              FileEffects&);

    // Bind only an owner-neutral request to this actual session's event owner.
    // Typed admission validates lifetime/pending state immediately at dispatch.
    BooleanTraceDueResult bindBooleanTrace(const BooleanTraceBindingContext&,
                                           const BooleanPumpTrace&) const;
    DispatchResult dispatchBooleanTraceAt(const PumpPoint&, const BooleanBoundTraceDue&,
                                         FileEffects&);

    // Actual existing Boolean engine leaf; a complete effect driver can call it
    // at the source-bound point. This is not the full diagram click dispatcher.
    void applyBooleanSelection(const FlowItem&, FileEffects&);

    // Parent Close logic must have completed its accepted-answer callbacks and
    // real diagram null-rebind effects first. This method does NOT decide Close,
    // call Save, emit notifications or assert original Close completion.
    void releaseProjectAfterCloseEffects();

private:
    ConversionSession(DiagramEffectDriver&, ProcessCodecOwner*);
    void modelUpdated(FileEffects&);
    void releaseProjectStorage() noexcept;

    // Session lifetime, separate from Project. All are also alive during the
    // explicit project_.reset() in releaseProjectStorage.
    DiagramEffectDriver& driver_; // must outlive session, including callbacks
    ProcessCodecOwner* processCodecOwner_ = nullptr; // Borrowed, never reset/revoked by Session.
    Events events_;
    std::unique_ptr<BooleanTraceOwnerScope> booleanTraceScope_;
    Events::ReceiverId receiver_;
    std::shared_ptr<Display> displayLease_;
    simcontrol300::RemeshState remesh_;
    std::shared_ptr<doe::SessionNotificationState> notificationState_;
    std::shared_ptr<doe::FlowReceiverWitnessRegistry> flowReceiverWitnessRegistry_;
    std::shared_ptr<PostGraphSceneState> primaryScene_, secondaryScene_;
    std::shared_ptr<doe::SlicePlaneHeap> slicePlaneHeap_; // outlives model children
    std::shared_ptr<doe::SliceConstructorRuntime> sliceConstructorRuntime_;
    std::shared_ptr<doe::TaskAllocationStore> taskAllocationStore_;
    std::shared_ptr<WidgetRuntimeState> widgetRuntime_;
    std::shared_ptr<doe::TaskFactoryRuntime> taskFactoryRuntime_;
    // The native main window owns the class registry across Project and Mo
    // model teardown. Keep this adapter lease alive through ModelManager.
    std::shared_ptr<OperationClassRegistry> registryLease_;
    std::shared_ptr<ModelManager> modelManager_;
    std::shared_ptr<SessionModelManagerReadReceipt> modelManagerOpenRead_;
    std::unique_ptr<doe::DoeTableOwnerState> doeTableOwner_;
    std::shared_ptr<MainViewState> mainView_;
    std::weak_ptr<ViewManagerState> viewManager_;
    bool viewManagerAliasesCommitted_ = false;
    std::unique_ptr<SplitViewConstructionReport> splitViewConstruction_;
    std::unique_ptr<simcontrol300::StagedEditor300> editor300Factory_;
    std::unique_ptr<simcontrol300::EditorSetupState> editor300Setup_;
    std::unique_ptr<Project> project_;
    std::unique_ptr<ProjectScope> scope_;
    SubProject* boundDiagramModel_ = nullptr; // NOT Operation::engineModel
};

} // namespace engine
} // namespace m2m
