#include "m2m/engine/close_empty_manager_tail.hpp"

#include <limits>
#include <utility>

#include "m2m/common.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"

namespace m2m::engine {
namespace {
[[noreturn]] void unavailable(const char* detail)
{
    throw Error(M2M_E_UNSUPPORTED, std::string("Close empty manager tail: ") + detail);
}
bool qualified(const std::string& text)
{
    return text.find_first_not_of(" \t\r\n") != std::string::npos;
}
bool sameManager(const std::shared_ptr<ModelManager>& left,
                 const std::shared_ptr<ModelManager>& right)
{
    return left && right && left.get() == right.get() &&
        !left.owner_before(right) && !right.owner_before(left);
}
std::vector<const DbHandle*> population(const ModelManager& manager)
{
    const auto count = manager.size();
    if (count > static_cast<std::size_t>(std::numeric_limits<int>::max()))
        unavailable("model population exceeds the represented ordinal range");
    std::vector<const DbHandle*> result;
    result.reserve(count);
    for (std::size_t ordinal = 0; ordinal < count; ++ordinal) {
        const auto* child = manager.modelAt(static_cast<int>(ordinal));
        if (!child) unavailable("actual model population is unavailable");
        result.push_back(child);
    }
    return result;
}
}

CloseEmptyManagerTailProgress::CloseEmptyManagerTailProgress(
    const ConversionSession& session, std::string sourceOccurrence, std::string qualification)
    : session_(&session), manager_(session.modelManagerOwner()),
      qualification_(std::move(qualification))
{
    const auto manager = manager_.lock();
    if (!manager || manager->ownerSession() != &session)
        unavailable("actual Session manager owner binding is unavailable");
    if (sourceOccurrence != closeEmptyManagerSourceOccurrence || !qualified(qualification_))
        unavailable("entry occurrence/qualification is unavailable");
}

CloseEmptyManagerTailResult attemptCloseEmptyManagerTail(
    const ConversionSession& session, CloseEmptyManagerTailProgress& progress)
{
    const auto manager = session.modelManagerOwner();
    if (progress.session_ != &session || !sameManager(manager, progress.manager_.lock()) ||
        manager->ownerSession() != &session)
        unavailable("progress does not bind this live Session manager identity");

    if (progress.stage_ == CloseEmptyManagerTailStage::InspectPopulation) {
        progress.models_ = population(*manager);
        progress.connections_ = manager->connections().size();
        if (!progress.models_.empty()) {
            progress.stage_ = CloseEmptyManagerTailStage::OwnedChildTeardownBlocked;
        } else if (progress.connections_ != 0) {
            // clear() also drops adapter connection metadata. An empty native
            // child loop is not authority to remove a separate existing entry.
            progress.stage_ = CloseEmptyManagerTailStage::ConnectionCleanupBlocked;
        } else {
            manager->clear(); // owning-list clear precedes explicit current reset
            progress.stage_ = CloseEmptyManagerTailStage::BeforePostClearProjectLookup;
            return {CloseEmptyManagerTailStop::BeforePostClearProjectLookup, true, 0,
                    "0x140029a83:post-clear Project lookup"};
        }
    } else if (population(*manager) != progress.models_ ||
               manager->connections().size() != progress.connections_) {
        // In particular, an external clear cannot bypass blocked child effects;
        // a later append cannot be cleared under an already consumed occurrence.
        unavailable("population changed after the consumed cleanup boundary");
    }

    if (progress.stage_ == CloseEmptyManagerTailStage::OwnedChildTeardownBlocked)
        return {CloseEmptyManagerTailStop::OwnedChildTeardownUnavailable, false,
                progress.models_.size(), "0x140c784f2:owned child deleting virtual+0x18"};
    if (progress.stage_ == CloseEmptyManagerTailStage::ConnectionCleanupBlocked)
        return {CloseEmptyManagerTailStop::ConnectionCleanupUnavailable, false, 0,
                "0x140c7852b:empty list with unresolved connection metadata"};
    return {CloseEmptyManagerTailStop::BeforePostClearProjectLookup, false, 0,
            "0x140029a83:post-clear Project lookup"};
}
} // namespace m2m::engine
