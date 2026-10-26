#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <set>
#include <string>

namespace m2m {
struct Project;
struct Operation;
namespace engine {
struct FileEffects;

// Explicit observation of one custom-notifier emission, not a posted event or
// timer-readiness observation. This projects only the selected receiver suffix;
// notifier +90/map/list prefix effects stay with the admitting caller. Weak
// aliases name the actual receiver and current editor owners supplied when the
// scope was bound. Capture admission is
// the caller's responsibility; these flags never default to native readiness.
struct BooleanUpdateDataEmission {
    std::uint64_t occurrence = 0;
    bool emissionAdmitted = false, receiverRegistrationAdmitted = false;
    std::optional<bool> notifierSuppressed, methodResolved, currentWidgetMatched;
    std::weak_ptr<const void> receiverOwner, currentEditorOwner;
    std::string qualification;
};

enum class BooleanUpdateDataOutcome {
    Unqualified, Suppressed, ExpiredReceiver, MethodNotResolved,
    NoCurrentEditor, Invoked
};
struct BooleanUpdateDataResult {
    BooleanUpdateDataOutcome outcome = BooleanUpdateDataOutcome::Unqualified;
    bool completed = false;
    std::string issue;
};

// Borrowed Project/Operation must outlive this non-rebindable scope. No native
// QObject or event owner is allocated. The weak owners bind actual receiver /
// editor lifetime and identity; the operation/model binding is rechecked at the
// effect boundary. A replacement editor needs a new qualified scope; the owner
// must release the old scope's admission when changing the current editor.
class BooleanSynchronousUpdateData final {
public:
    BooleanSynchronousUpdateData(Project&, Operation&,
        const std::shared_ptr<const void>& receiverOwner,
        const std::shared_ptr<const void>& currentEditorOwner,
        const std::string& qualification);
    BooleanSynchronousUpdateData(const BooleanSynchronousUpdateData&) = delete;
    BooleanSynchronousUpdateData& operator=(const BooleanSynchronousUpdateData&) = delete;
    BooleanUpdateDataResult dispatch(Project&, Operation&, FileEffects&,
                                    const BooleanUpdateDataEmission&);
    void releaseAdmissionScope() noexcept { admitted_ = false; }
private:
    Project* project_;
    Operation* operation_;
    const void* class_;
    std::weak_ptr<const void> receiver_, editor_, model_;
    std::set<std::uint64_t> consumed_;
    bool inFlight_ = false, admitted_ = true;
};

// Bounded same-current, shown Object editor replacement for a Type0 Boolean
// operation: 038950 -> row0 -> 5f36d0 -> 11e0510. Uses the actual class hooks.
// This is not timer readiness, scene fitting, general tree selection, or the
// complete GUI notification chain. Other classes/states are explicitly refused.
// If the old editor is not shown, refusal follows the original current-object
// zero prefix; the unqualified index0 propsToEngine branch is not inferred.
void reselectBooleanObjectEditor(Project&, Operation&, FileEffects&);

}  // namespace engine
}  // namespace m2m
