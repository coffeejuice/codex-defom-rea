#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>

namespace m2m::engine {

enum class PaletteNativeValueKind { GetterTemporary, PrivateDestination };
enum class PaletteNativeValueStage { AwaitingRetain, Held, Released };
enum class PaletteNativeRefIssue {
    None, MissingQualification, StateUnavailable, StateOwnerMismatch,
    ValueNotRetained, ValueAlreadyReleased, RefCountUnavailable, DataDestructionPending
};
struct PaletteNativeAtomicReceipt {
    std::uint64_t sequence = 0;
    std::optional<std::uint32_t> beforeBits, afterBits;
    std::optional<bool> nonzero;
};
struct PaletteNativeValueReport {
    PaletteNativeValueStage stage = PaletteNativeValueStage::AwaitingRetain;
    PaletteNativeRefIssue issue = PaletteNativeRefIssue::None;
};

class PaletteNativeValueRef;

// Isolated physical Qt atomic state for ONE palette data owner. Actual runtime
// interning/module/data association is a separate required integration seam.
// C++ diagnostic reference counts do not determine this native dword.
class PaletteNativeRefState final : public std::enable_shared_from_this<PaletteNativeRefState> {
public:
    explicit PaletteNativeRefState(std::string qualification);
    PaletteNativeRefState(const PaletteNativeRefState&) = delete;
    PaletteNativeRefState& operator=(const PaletteNativeRefState&) = delete;
    bool observeCurrent(std::uint32_t bits, std::uint64_t currentSequence,
        const std::string& qualification) noexcept;
    void releaseNativeOwner() noexcept { alive_ = false; }
    bool nativeOwnerAlive() const noexcept { return alive_; }
    bool destructionPending() const noexcept { return destructionPending_; }
    std::optional<std::uint32_t> currentBits() const noexcept { return bits_; }
    std::uint64_t mutationSequence() const noexcept { return sequence_; }
    std::uint64_t refMutations() const noexcept { return refs_; }
    std::uint64_t derefMutations() const noexcept { return derefs_; }
    // Counts only explicit modeled native retain/release operations. Dropping
    // a host token does not close the original's unmodeled unwind/destruction.
    std::uint64_t nativeValueBalance() const noexcept { return valueBalance_; }
    const std::string& qualification() const noexcept { return qualification_; }
private:
    friend class PaletteNativeValueRef;
    PaletteNativeAtomicReceipt retain() noexcept;
    PaletteNativeAtomicReceipt release() noexcept;
    std::string qualification_;
    std::optional<std::uint32_t> bits_;
    std::uint64_t sequence_ = 0, refs_ = 0, derefs_ = 0, valueBalance_ = 0;
    bool alive_ = true, destructionPending_ = false;
};

// One modeled native value, not a host copy. Share this noncopyable token when
// copying diagnostic reports. Constructor/inspection performs no native ref.
class PaletteNativeValueRef final {
public:
    PaletteNativeValueRef(const std::shared_ptr<PaletteNativeRefState>&, PaletteNativeValueKind);
    ~PaletteNativeValueRef() = default; // Never synthesize native deref on host teardown.
    PaletteNativeValueRef(const PaletteNativeValueRef&) = delete;
    PaletteNativeValueRef& operator=(const PaletteNativeValueRef&) = delete;
    const PaletteNativeValueReport& retain();
    const PaletteNativeValueReport& release();
    const PaletteNativeValueReport& report() const noexcept { return report_; }
    PaletteNativeValueKind kind() const noexcept { return kind_; }
    std::weak_ptr<PaletteNativeRefState> stateOwner() const noexcept { return state_; }
    const std::optional<PaletteNativeAtomicReceipt>& retainReceipt() const noexcept { return retained_; }
    const std::optional<PaletteNativeAtomicReceipt>& releaseReceipt() const noexcept { return released_; }
private:
    PaletteNativeRefIssue validate(std::shared_ptr<PaletteNativeRefState>&) const noexcept;
    std::weak_ptr<PaletteNativeRefState> state_;
    PaletteNativeRefIssue bindingIssue_ = PaletteNativeRefIssue::None;
    PaletteNativeValueKind kind_;
    PaletteNativeValueReport report_;
    std::optional<PaletteNativeAtomicReceipt> retained_, released_;
};

} // namespace m2m::engine
