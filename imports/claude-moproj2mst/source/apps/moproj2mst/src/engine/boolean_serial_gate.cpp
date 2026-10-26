#include "m2m/engine/boolean_serial_gate.hpp"
#include "m2m/model.hpp"
#include <utility>

namespace m2m::engine {
namespace {
bool ownerValid(const BooleanSerialNativeOwner& o)
{ return o.dispatcher && o.privateData && o.threadData; }
template<class T> void retain(std::optional<T>& current, const std::optional<T>& incoming)
{
    if (!incoming) return;
    if (current && current != incoming)
        throw Error(M2M_E_UNSUPPORTED, "Serial-gate frame cannot replace an already observed source-stage value");
    current = incoming;
}
}

std::shared_ptr<BooleanSerialGateState> BooleanSerialGateState::createQualified(
    const BooleanSerialNativeOwner& owner, std::optional<std::uint32_t> processed,
    const std::string& qualification)
{
    if (!ownerValid(owner) || qualification.empty())
        throw Error(M2M_E_UNSUPPORTED, "Serial gate needs qualified actual dispatcher/private/thread identities");
    auto state = std::shared_ptr<BooleanSerialGateState>(new BooleanSerialGateState(owner, processed));
    state->allocationOwner_ = state;
    return state;
}

void BooleanSerialGateState::observeProcessedSerial(std::uint32_t value, const std::string& qualification)
{
    if (!admitted_ || qualification.empty() || (processed_ && *processed_ != value))
        throw Error(M2M_E_UNSUPPORTED, "Processed-serial observation cannot replace live state or expired admission");
    processed_ = value;
}

const BooleanSerialGateReport& BooleanSerialGate::resume(
    const std::shared_ptr<BooleanSerialGateState>& state, const BooleanSerialMessageInput& input)
{
    if (!state || !state->admitted_ || input.qualification.empty() || !input.occurrence ||
        !ownerValid(input.owner) || input.owner != state->owner_)
        throw Error(M2M_E_UNSUPPORTED, "Serial-gate invocation lost qualified actual owner/occurrence admission");
    const auto canonical = state->allocationOwner_.lock();
    if (!canonical || canonical.get() != state.get() || canonical.owner_before(state) || state.owner_before(canonical))
        throw Error(M2M_E_UNSUPPORTED, "Serial gate cannot substitute a foreign state control block");
    if (bound_) {
        const auto actual = state_.lock();
        if (!actual || actual.get() != state.get() || actual.owner_before(state) || state.owner_before(actual) ||
            retained_.owner != input.owner || retained_.occurrence != input.occurrence)
            throw Error(M2M_E_UNSUPPORTED, "Pending serial-gate frame cannot replace its source owners or occurrence");
    } else {
        if (!state->admittedOccurrences_.insert(input.occurrence).second)
            throw Error(M2M_E_UNSUPPORTED, "A serial-gate occurrence already belongs to another retained frame");
        state_ = state; bound_ = true;
        retained_.owner = input.owner; retained_.occurrence = input.occurrence;
    }
    auto merged = retained_;
    retain(merged.message, input.message);
    retain(merged.wParam, input.wParam);
    retain(merged.fallbackTimerAtMessage, input.fallbackTimerAtMessage);
    retain(merged.serialAtGate, input.serialAtGate);
    retained_ = std::move(merged);
    auto pending = [&](const char* issue) -> const BooleanSerialGateReport& {
        report_.issue = issue; return report_;
    };
    if (report_.completed) return report_;
    if (report_.stage == BooleanSerialGateStage::Admission) {
        if (!input.prefixAdmitted)
            return pending("Actual application/filter/userdata prefix is not admitted");
        report_.stage = BooleanSerialGateStage::MessageRoute;
    }
    if (report_.stage == BooleanSerialGateStage::MessageRoute) {
        if (!retained_.message) return pending("Actual message kind is unknown");
        if (*retained_.message == 0x113u) {
            // 67142918 loads a dword into EAX: compare the zero-extended
            // fallback ID with the full native wParam, not a truncated ID.
            if (!retained_.fallbackTimerAtMessage)
                return pending("Fallback timer discriminator is unknown");
            if (*retained_.fallbackTimerAtMessage != 0 && !retained_.wParam)
                return pending("Actual WM_TIMER wParam is unknown");
            if (*retained_.fallbackTimerAtMessage == 0 ||
                *retained_.wParam != static_cast<std::uint64_t>(*retained_.fallbackTimerAtMessage))
                report_.stage = BooleanSerialGateStage::RegisteredTimerPending;
            else report_.stage = BooleanSerialGateStage::SerialRead;
        } else if (*retained_.message == 0x401u) {
            report_.stage = BooleanSerialGateStage::SerialRead;
        } else {
            report_.stage = BooleanSerialGateStage::OtherMessagePending;
        }
    }
    if (report_.stage == BooleanSerialGateStage::RegisteredTimerPending)
        return pending("Registered-timer handler67141ff0 is required; no Boolean readiness is inferred");
    if (report_.stage == BooleanSerialGateStage::OtherMessagePending)
        return pending("Other native message handling is outside this serial-gate prefix");
    if (report_.stage == BooleanSerialGateStage::SerialRead) {
        if (!retained_.serialAtGate) return pending("Actual source-stage serial read is unknown");
        report_.serialRead = retained_.serialAtGate;
        report_.stage = BooleanSerialGateStage::ProcessedSerialRead;
    }
    if (report_.stage == BooleanSerialGateStage::ProcessedSerialRead) {
        if (!state->processed_) return pending("Actual processed-serial field is unknown");
        report_.processedBefore = state->processed_;
        if (*report_.serialRead == *state->processed_) {
            report_.processedAfter = state->processed_;
            report_.stage = BooleanSerialGateStage::Complete;
            report_.completed = true; report_.issue.clear();
            return report_;
        }
        // 67142959 precedes the unclosed sendPostedEvents call67142968.
        state->processed_ = report_.serialRead;
        report_.processedAfter = state->processed_;
        report_.dispatchRequest = BooleanPostedDispatchRequest{state->owner_, retained_.occurrence, true, 0};
        report_.stage = BooleanSerialGateStage::PostedDispatchPending;
    }
    return pending("sendPostedEvents(null,0,sameThreadData) is required; mutation retained before unclosed effects");
}

} // namespace m2m::engine
