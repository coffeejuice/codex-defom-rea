// REVIEW DRAFT ONLY. Not part of a CMake target or a claimed implementation.
#pragma once

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "m2m/engine/lifecycle_events.hpp"

namespace m2m {
struct Project;
struct ProjectScope;
struct SubProject;
struct FlowItem;
namespace engine {
struct Display;
struct FileEffects;

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
    ~ConversionSession();
    ConversionSession(const ConversionSession&) = delete;
    ConversionSession& operator=(const ConversionSession&) = delete;

    Project& project();
    Project* liveProject() noexcept { return project_.get(); }
    Display* displayIfCreated() noexcept;
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

    // Actual existing Boolean engine leaf; a complete effect driver can call it
    // at the source-bound point. This is not the full diagram click dispatcher.
    void applyBooleanSelection(const FlowItem&, FileEffects&);

    // Parent Close logic must have completed its accepted-answer callbacks and
    // real diagram null-rebind effects first. This method does NOT decide Close,
    // call Save, emit notifications or assert original Close completion.
    void releaseProjectAfterCloseEffects();

private:
    void modelUpdated(FileEffects&);
    void releaseProjectStorage() noexcept;

    // Session lifetime, separate from Project. All are also alive during the
    // explicit project_.reset() in releaseProjectStorage.
    DiagramEffectDriver& driver_; // must outlive session, including callbacks
    Events events_;
    Events::ReceiverId receiver_;
    std::shared_ptr<Display> displayLease_;
    std::unique_ptr<Project> project_;
    std::unique_ptr<ProjectScope> scope_;
    SubProject* boundDiagramModel_ = nullptr; // NOT Operation::engineModel
};

} // namespace engine
} // namespace m2m
