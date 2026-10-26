// REVIEW DRAFT ONLY. Source integration/build remain parent-owned.
#include "conversion_session.hpp"

#include <algorithm>
#include "m2m/engine.hpp"
#include "m2m/engine/boolean_lifecycle.hpp"

namespace m2m::engine {
namespace {
[[noreturn]] void unsupported(const char* text)
{
    throw Error(M2M_E_UNSUPPORTED, text);
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
      project_(std::make_unique<Project>()), scope_(std::make_unique<ProjectScope>(*project_))
{}

ConversionSession::~ConversionSession()
{
    // Adapter exception/final cleanup, NOT an original normal-Close replay.
    // No throwing effect driver or original dialog is invoked from a destructor.
    releaseProjectStorage();
    events_.destroyReceiver(receiver_);
    events_.dispatcherDestroyed();
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
    project_.reset();
}

} // namespace m2m::engine
