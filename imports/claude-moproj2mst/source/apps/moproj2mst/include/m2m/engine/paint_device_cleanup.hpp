#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace m2m::engine {

class PaintDeviceCleanupGlobals;
class PaintDeviceCleanupCounterState final {
public:
    std::uint32_t observationAlias() const noexcept { return alias_; }
    std::optional<std::int32_t> value() const noexcept { return value_; }
private:
    friend class PaintDeviceCleanupGlobals;
    explicit PaintDeviceCleanupCounterState(std::uint32_t alias) : alias_(alias) {}
    std::uint32_t alias_;
    std::optional<std::int32_t> value_;
};

struct PaintDeviceCleanupCounterObservation {
    std::uint32_t alias = 0;
    std::optional<std::int32_t> value; // Actual QAtomicInt cell, NOT an image/Qt ref count.
};
struct PaintDeviceCleanupGlobalObservation {
    std::weak_ptr<PaintDeviceCleanupGlobals> receiver;
    std::string qualification;
    std::string moduleSha256;
    std::optional<std::uint32_t> pointerRva;
    std::vector<PaintDeviceCleanupCounterObservation> counters;
    // Disengaged: unknown pointer;0: qualified null; positive: actual counter alias.
    std::optional<std::uint32_t> currentCounterAlias;
};

// One module/global owner for a qualified Qt runtime. Integration must bind this
// actual owner; neither a file initializer nor an image-data ref count supplies
// its current pointer/value. Observations are monotonic snapshot refinements.
class PaintDeviceCleanupGlobals final : public std::enable_shared_from_this<PaintDeviceCleanupGlobals> {
public:
    explicit PaintDeviceCleanupGlobals(std::string qualification);
    ~PaintDeviceCleanupGlobals();
    PaintDeviceCleanupGlobals(const PaintDeviceCleanupGlobals&) = delete;
    PaintDeviceCleanupGlobals& operator=(const PaintDeviceCleanupGlobals&) = delete;
    bool observe(const PaintDeviceCleanupGlobalObservation&);
    void releaseNativeOwner() noexcept { alive_ = false; }
    bool nativeOwnerAlive() const noexcept { return alive_; }
    bool qualified() const noexcept { return qualified_; }
    const std::string& qualification() const noexcept { return qualification_; }
    std::shared_ptr<const PaintDeviceCleanupCounterState> counterFor(std::uint32_t) const noexcept;
private:
    friend class PaintDeviceCleanupCall;
    std::string qualification_;
    bool alive_ = true, qualified_ = false;
    std::map<std::uint32_t, std::shared_ptr<PaintDeviceCleanupCounterState>> counters_;
    std::optional<std::shared_ptr<PaintDeviceCleanupCounterState>> currentCounter_;
};

enum class PaintDeviceCleanupStage { FirstGetter, SecondGetter, CounterComparison, NonzeroContinuation, Returned };
enum class PaintDeviceCleanupIssue {
    None, GlobalsUnavailable, GlobalQualificationUnavailable, CounterPointerUnavailable,
    LazyGlobalUnrecovered, GetterOwnerChanged, CounterOwnerUnavailable,
    CounterValueUnavailable, NonzeroCleanupUnrecovered
};
enum class PaintDeviceCleanupEffect { FirstGetterReturned, SecondGetterReturned, CounterValueRead };
struct PaintDeviceCleanupReport {
    PaintDeviceCleanupStage stage = PaintDeviceCleanupStage::FirstGetter;
    PaintDeviceCleanupIssue issue = PaintDeviceCleanupIssue::None;
    static constexpr bool completeForPublicSave = false;
};

// Only QPaintDevice cleanup-tail65148840's existing-counter==0 normal branch.
// Caller must separately recover destructor prefix/vtable/painting-warning
// effects. The selected fast path does not dereference its paint-device argument.
// Null/unknown/lazy globals and every nonzero comparison stay guarded. No mutex,
// list traversal, lazy allocation, callback registration, warning or Qt ref op
// is invented here. This helper does not bind the image/editor/public lifecycle.
class PaintDeviceCleanupCall final {
public:
    explicit PaintDeviceCleanupCall(const std::shared_ptr<PaintDeviceCleanupGlobals>&);
    PaintDeviceCleanupCall(const PaintDeviceCleanupCall&) = delete;
    PaintDeviceCleanupCall& operator=(const PaintDeviceCleanupCall&) = delete;
    PaintDeviceCleanupReport resume();
    const PaintDeviceCleanupReport& report() const noexcept { return report_; }
    const std::vector<PaintDeviceCleanupEffect>& effects() const noexcept { return effects_; }
    // Diagnostic lease only; the native getter borrows its pointer without ref/deref.
    std::shared_ptr<const PaintDeviceCleanupCounterState> counterOwner() const noexcept { return counter_.lock(); }
private:
    std::weak_ptr<PaintDeviceCleanupGlobals> globals_;
    std::weak_ptr<PaintDeviceCleanupCounterState> counter_;
    PaintDeviceCleanupReport report_;
    std::vector<PaintDeviceCleanupEffect> effects_;
};

} // namespace m2m::engine
