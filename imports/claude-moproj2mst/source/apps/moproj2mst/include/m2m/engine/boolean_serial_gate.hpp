#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <set>
#include <string>

namespace m2m::engine {

// Observation-local identities, never default runtime addresses/handles.
struct BooleanSerialNativeOwner {
    std::uint64_t dispatcher = 0, privateData = 0, threadData = 0;
    bool operator==(const BooleanSerialNativeOwner&) const = default;
};

// One canonical diagnostic projection of the actual private+ac store. It does
// not allocate a QObject, dispatcher, timer or queue. Native lifetime/identity
// admission remains the capture/integration layer's responsibility.
class BooleanSerialGateState final {
public:
    static std::shared_ptr<BooleanSerialGateState> createQualified(
        const BooleanSerialNativeOwner&, std::optional<std::uint32_t> processedSerial,
        const std::string& qualification);
    BooleanSerialGateState(const BooleanSerialGateState&) = delete;
    BooleanSerialGateState& operator=(const BooleanSerialGateState&) = delete;
    const BooleanSerialNativeOwner& owner() const noexcept { return owner_; }
    std::optional<std::uint32_t> processedSerial() const noexcept { return processed_; }
    void observeProcessedSerial(std::uint32_t, const std::string& qualification);
    // Diagnostic admission expiry; does not emulate native destruction/reset.
    void releaseAdmissionScope() noexcept { admitted_ = false; }
private:
    friend class BooleanSerialGate;
    BooleanSerialGateState(const BooleanSerialNativeOwner& o, std::optional<std::uint32_t> p)
        : owner_(o), processed_(p) {}
    BooleanSerialNativeOwner owner_;
    std::optional<std::uint32_t> processed_;
    bool admitted_ = true;
    std::weak_ptr<BooleanSerialGateState> allocationOwner_;
    std::set<std::uint64_t> admittedOccurrences_;
};

struct BooleanSerialMessageInput {
    BooleanSerialNativeOwner owner;
    std::uint64_t occurrence = 0;
    // The routine is entered only after its application/native-filter and
    // window-userdata path has been qualified, not assumed empty/successful.
    bool prefixAdmitted = false;
    std::optional<std::uint32_t> message;
    std::optional<std::uint64_t> wParam;
    std::optional<std::uint32_t> fallbackTimerAtMessage; // private+b0, not a Boolean timer
    std::optional<std::uint32_t> serialAtGate; // actual private+a8 read; never inferred
    std::string qualification;
};

struct BooleanPostedDispatchRequest {
    BooleanSerialNativeOwner owner;
    std::uint64_t occurrence;
    bool nullReceiver = true;
    std::int32_t requestedType = 0;
};

enum class BooleanSerialGateStage {
    Admission, MessageRoute, SerialRead, ProcessedSerialRead,
    PostedDispatchPending, RegisteredTimerPending, OtherMessagePending, Complete
};
struct BooleanSerialGateReport {
    BooleanSerialGateStage stage = BooleanSerialGateStage::Admission;
    bool completed = false;
    std::optional<std::uint32_t> serialRead, processedBefore, processedAfter;
    std::optional<BooleanPostedDispatchRequest> dispatchRequest;
    std::string issue;
};

// One retained native prefix frame. No sendPostedEvents/Qt/Win32 callback is
// called here. A request remains pending; repeated resume is not a second call.
// Distinct occurrences require distinct frames. No native rollback is simulated.
class BooleanSerialGate final {
public:
    BooleanSerialGate() = default;
    BooleanSerialGate(const BooleanSerialGate&) = delete;
    BooleanSerialGate& operator=(const BooleanSerialGate&) = delete;
    const BooleanSerialGateReport& resume(const std::shared_ptr<BooleanSerialGateState>&,
                                         const BooleanSerialMessageInput&);
    const BooleanSerialGateReport& report() const noexcept { return report_; }
private:
    std::weak_ptr<BooleanSerialGateState> state_;
    bool bound_ = false;
    BooleanSerialMessageInput retained_;
    BooleanSerialGateReport report_;
};

} // namespace m2m::engine
