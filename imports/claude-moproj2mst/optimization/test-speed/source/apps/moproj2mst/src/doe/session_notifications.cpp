#include "m2m/doe_session_notifications.hpp"

#include <memory>
#include <utility>

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_task_factory.hpp"

namespace m2m::doe {
TaskObserver dispatchCreateTaskBase(StudyIo& io, const qt::String& type)
{
    auto& state = io.sessionNotificationState();
    if (state.suppressed) return {}; //2cf771: before map and receiver lookup
    if (!state.createTaskBaseRegistrations || state.createTaskBaseRegistrations->qualification.empty())
        throw Error(M2M_E_UNSUPPORTED, "DOE Task return-notifier registration population is unavailable");
    auto& list = *state.createTaskBaseRegistrations;
    if (!list.mapKeyPresent && !list.entries.empty())
        throw Error(M2M_E_UNSUPPORTED, "DOE Task return-notifier nonempty list needs existing map key");
    if (!list.mapKeyPresent) {
        list.mapKeyPresent = true; //2d03f0 default insertion, without flag90 reset
        state.emptyKeys.insert("2signalCreateTaskBase()");
    }
    const auto snapshot = list.entries; //actual local QList snapshot, not pruning
    TaskObserver result; //Q_RETURN_ARG(SfTaskBase *) initially null
    for (const auto& registration : snapshot) {
        const auto receiver = registration.receiver.lock();
        if (!receiver || !receiver->nativeReceiverAlive()) continue;
        if (registration.bareMethod != "slotCreateTaskBase")
            throw Error(M2M_E_UNSUPPORTED, "DOE Task return-notifier receiver method remains unsupported");
        result = receiver->createTaskBase(type); //DirectConnection1, QString unchanged
    }
    return result;
}

void StudyIo::bindSessionNotificationState(std::shared_ptr<SessionNotificationState> owner)
{
    if (!owner || (sessionNotificationState_ && sessionNotificationState_ != owner))
        throw Error(M2M_E_UNSUPPORTED, "DOE notifier state must bind a nonnull shared owner before observations");
    sessionNotificationState_ = std::move(owner);
}

SessionNotificationState& StudyIo::sessionNotificationState()
{
    if (!sessionNotificationState_)
        sessionNotificationState_ = std::make_shared<SessionNotificationState>();
    return *sessionNotificationState_;
}

void observeEmptySessionNotification(StudyIo& io, const std::string& signal,
    std::optional<std::size_t> currentPart, std::optional<int> currentSession,
    std::optional<int> argument)
{
    if (signal != "2signalChangeStepRange()" && signal != "2signalOpenSession()" &&
        signal != "2signalChapterChanged()" && signal != "2signalNumChaptersChanged()" &&
        signal != "2signalGetCurrentStepViewInfo()" && signal != "2signalMoEquipManagerChanged()")
        throw Error(M2M_E_UNSUPPORTED, "DOE notifier key has no recovered empty binding contract");
    auto& state = io.sessionNotificationState();
    SessionNotificationAttempt attempt{signal, state.suppressed, false,
        currentPart, currentSession, argument};
    if (!state.suppressed) {
        state.flag90 = false; //before2d03f0 map lookup/default insertion
        attempt.emptyKeyInserted = state.emptyKeys.insert(signal).second;
        //The complete fixed registration inventory also has no equipment-key
        //receiver; its add/remove ID is an unconsumed diagnostic argument here.
    }
    state.attempts.push_back(std::move(attempt));
}
} // namespace m2m::doe
