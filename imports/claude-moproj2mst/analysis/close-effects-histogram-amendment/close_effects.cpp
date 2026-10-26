#include "m2m/engine/close_effects.hpp"

#include <utility>

#include "m2m/common.hpp"
#include "m2m/doe_statevar.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_stepdata.hpp"
#include "m2m/engine.hpp"

namespace m2m::engine {
namespace {
[[noreturn]] void unsupported(const char* detail)
{
    throw Error(M2M_E_UNSUPPORTED, std::string("Close ChangePtrkGraph: ") + detail);
}
}

PtrkGraphManagerState::PtrkGraphManagerState(const std::shared_ptr<DbHandle>& owner)
    : owner_(owner)
{
    if (!owner) unsupported("SfPtrkManager requires its actual nonnull data-root owner");
}

void CloseEffects::setPtrkGraphRegistrations(std::optional<PtrkGraphRegistrations> registrations)
{
    ptrkGraphRegistrations_ = std::move(registrations);
}

void CloseEffects::changePtrkGraph(GuiModel& model, doe::StateVarState& state)
{
    // Adapter identity qualification, not an additional original branch.
    if (!model.doeStepData || model.doeStepData->stateVar.get() != &state)
        unsupported("application must pass the StateVar owned by the actual GuiModel");
    if (!ptrkGraphRegistrations_ || ptrkGraphRegistrations_->qualification.empty())
        unsupported("ordered receiver population is unavailable");

    // 2cf3c0 traverses a local QList copy. Expired QPointers are removed only
    // from that copy; it does not clean the original global registration list.
    const auto snapshot = ptrkGraphRegistrations_->receivers;
    for (const auto& weak : snapshot) {
        const auto receiver = weak.lock();
        if (!receiver) continue;
        // An externally retained receiver whose original parent has died is an
        // incomplete ownership projection, not a proof of a native live object.
        const auto owner = receiver->owner().lock();
        if (!owner) unsupported("live receiver has lost its original data-root owner");

        // 322420 ->3215c0 is a pure +4d0 load. False returns before model lookup,
        // graph creation, plot-variable reads or any graph mutation.
        if (!receiver->graphOn) continue;

        // Even variable0 must first execute 321660 ->31fab0. That getter can
        // allocate a graph and add it to a lazy model+250 list. Do not skip it
        // based on the caller's plotVar, or fabricate allocation via a counter.
        unsupported("enabled receiver needs native graph ownership and registration (321660/31fab0)");
    }
}
void StateVarPlotEffects::changeHistoGraph(GuiModel&, doe::StateVarState&)
{
    throw Error(M2M_E_UNSUPPORTED, "Close ChangeHistoGraph: receiver effect is unavailable");
}

void CloseEffects::setHistogramRegistrations(std::optional<HistogramRegistrations> registrations)
{
    histogramRegistrations_ = std::move(registrations);
}

void CloseEffects::changeHistoGraph(GuiModel& model, doe::StateVarState& state)
{
    const auto refuse = [](const char* detail) {
        throw Error(M2M_E_UNSUPPORTED, std::string("Close ChangeHistoGraph: ") + detail);
    };
    if (!model.doeStepData || model.doeStepData->stateVar.get() != &state)
        refuse("application must pass the StateVar owned by the actual GuiModel");
    if (!histogramRegistrations_ || !histogramRegistrations_->owner ||
        histogramRegistrations_->qualification.empty())
        refuse("actual runtime and ordered receiver coverage are unavailable");

    // Bind the actual runtime and take a new local snapshot at each emit. New
    // helpers registered since binding are included. Flags are never copied.
    const auto lease = histogramRegistrations_->owner;
    const auto snapshot = lease->stateVarRuntime().histogramRegistrations;
    for (const auto& registration : snapshot) {
        if (registration.signal != "signalChangeHistoGraph") continue;
        const auto receiver = registration.receiver.lock();
        if (!receiver) continue; // do not prune persistent runtime registrations
        if (registration.slot != "slotChangeHistoGraph")
            refuse("live receiver uses an unsupported slot");
        // 413f00 ->4117e0 reads the existing helper's actual +14 field.
        if (!receiver->graphOn) continue;
        // The next read is helper+38+7c, NOT HistogramState::binWidth at+10.
        refuse("enabled receiver needs nested Qt field +0xb4 and graph effects (4122f0)");
    }
}
} // namespace m2m::engine
