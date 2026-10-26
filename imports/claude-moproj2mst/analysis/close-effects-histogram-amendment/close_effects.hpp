// Isolated Close notification effects. Lifecycle invocation remains root-owned.
#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace m2m::doe { struct StateVarState; class StudyIo; }

namespace m2m::engine {
struct DbHandle;
struct GuiModel;

// d16530 notify=true invokes this after its variable/component stores and before
// rendering. Implementations execute receiver effects, not diagnostic counters.
struct StateVarPlotEffects {
    virtual ~StateVarPlotEffects() = default;
    virtual void changePtrkGraph(GuiModel&, doe::StateVarState&) = 0;
    // d17b90 emits after its type/title/range prefix, before d189b0/legend.
    // First-only implementations retain an exact refusal at this second hook.
    virtual void changeHistoGraph(GuiModel&, doe::StateVarState&);
};

// SfPtrkManager (309cc0), lazily owned by the original data root at +1978.
// This is its actual projected state, not a Close-time copy of its flags.
// A nonnull parent initializes +4d0=false before registering the receiver.
// Construction here does not claim the selected Open path created a manager.
class PtrkGraphManagerState final {
public:
    explicit PtrkGraphManagerState(const std::shared_ptr<DbHandle>& owner);
    const std::weak_ptr<DbHandle>& owner() const noexcept { return owner_; }
    bool graphOn = false; // +4d0; caller must preserve intervening native changes

private:
    std::weak_ptr<DbHandle> owner_;
};

// Explicit ordered receiver population. An observed empty list is different
// from unavailable knowledge. Qualification identifies its evidence/profile;
// supplying one does not establish that the original application was observed.
struct PtrkGraphRegistrations {
    std::string qualification;
    std::vector<std::weak_ptr<PtrkGraphManagerState>> receivers;
};

// Shared lease keeps the actual StudyIo/runtime registry alive across Project
// storage release. Receiver ownership remains StateVar's; registrations are weak.
struct HistogramRegistrations {
    std::shared_ptr<doe::StudyIo> owner;
    std::string qualification;
};

// These receiver methods require the shared notifier's +90/+141 gates to have
// been handled by lifecycle integration. No duplicate suppression flags here.
// Source-qualified subset of signalChangePtrkGraph delivery. Registration
// lifetime and live receiver state are real; graphOn=true remains guarded before
// its first unresolved lazy allocation/registration effect (321660 ->31fab0).
class CloseEffects final : public StateVarPlotEffects {
public:
    // nullopt restores unavailable knowledge. Neither this method nor dispatch
    // creates a manager, infers fresh absence, or changes any graph flag.
    void setPtrkGraphRegistrations(std::optional<PtrkGraphRegistrations>);
    const std::optional<PtrkGraphRegistrations>& ptrkGraphRegistrations() const noexcept {
        return ptrkGraphRegistrations_;
    }
    void changePtrkGraph(GuiModel&, doe::StateVarState&) override;
    void setHistogramRegistrations(std::optional<HistogramRegistrations>);
    const std::optional<HistogramRegistrations>& histogramRegistrations() const noexcept {
        return histogramRegistrations_;
    }
    void changeHistoGraph(GuiModel&, doe::StateVarState&) override;

private:
    std::optional<PtrkGraphRegistrations> ptrkGraphRegistrations_;
    std::optional<HistogramRegistrations> histogramRegistrations_;
};
} // namespace m2m::engine
