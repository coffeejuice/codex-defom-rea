#pragma once

#include <cstddef>
#include <memory>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

namespace m2m::engine { class PostGraphManagerState; class ViewManagerState; }

namespace m2m::doe {
class StudyIo;
class TaskFactoryRuntime;

struct TaskFactoryRegistration {
    std::weak_ptr<TaskFactoryRuntime> receiver;
    std::string bareMethod = "slotCreateTaskBase";
};

struct TaskFactoryRegistrationList {
    std::vector<TaskFactoryRegistration> entries;
    std::string qualification;
    bool mapKeyPresent = false;
};

struct SessionNotificationAttempt {
    std::string signal; //exact original key, including Qt marker/signature
    bool suppressed = false;
    bool emptyKeyInserted = false;
    std::optional<std::size_t> currentPart;
    std::optional<int> currentSession;
    std::optional<int> argument;
};

struct ViewManagerRegistration {
    // A retained diagnostic shared lease can outlive its QObject parent;
    // delivery must also require receiver->nativeReceiverAlive().
    std::weak_ptr<engine::ViewManagerState> receiver;
    std::string bareMethod;
};

// Shared single-project notifier state. Four final session keys and default
// task GetCurrentStepViewInfo and equipment-manager change have source-qualified empty bindings; graph
// registrations require their own explicit qualification.
struct SessionNotificationState {
    bool suppressed = false; //fresh global+141: PE zero-fill, normal scopes end0
    bool flag90 = false;
    std::set<std::string> emptyKeys;
    std::vector<SessionNotificationAttempt> attempts;
    // Actual shared map key2signalCreateTaskBase(). Unavailable is distinct
    // from an observed empty list. Registration order and weak lifetimes stay
    // on this owner, not a factory-local notifier copy.
    std::optional<TaskFactoryRegistrationList> createTaskBaseRegistrations;
    std::optional<std::vector<std::weak_ptr<engine::PostGraphManagerState>>> removeGraphItemsRegistrations;
    std::string removeGraphItemsQualification;
    // Same native notifier map, typed projection for the four d34870 keys.
    // Expired weak receivers remain in order; no invented automatic pruning.
    std::map<std::string, std::vector<ViewManagerRegistration>> viewManagerRegistrations;
};

//2cf3c0 for ChangeStepRange/OpenSession/ChapterChanged/NumChaptersChanged and
// default-task GetCurrentStepViewInfo and equipment-manager change only.
// Their full fixed registration-API call set has no bindings. Preserve the
// suppression branch, flag reset and empty-map insertion, not a fake callback.
void observeEmptySessionNotification(StudyIo& io, const std::string& signal,
    std::optional<std::size_t> currentPart, std::optional<int> currentSession,
    std::optional<int> argument = {});
} // namespace m2m::doe
