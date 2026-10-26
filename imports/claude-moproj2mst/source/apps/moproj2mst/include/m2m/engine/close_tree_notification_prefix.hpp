// Actual shared notifier entry only; caller arguments/public Close stay guarded.
#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace m2m::doe { struct SessionNotificationState; }

namespace m2m::engine {
class ConversionSession;
class ModelManager;
class CloseBusyGlobalOwner;

inline constexpr const char* closeTreeNotificationSourceOccurrence =
    "0x140029a54:true setter ->0x1402cac94:0x1402cf3c0(false) direct callee entry";

enum class CloseTreeNotificationPrefixStage {
    AwaitSuppressionRead, SuppressedCalleeReturn, BeforeKeyQStringConstruction
};
enum class CloseTreeNotificationPrefixStop {
    SuppressedCalleeReturnReached, KeyQStringConstructionUnavailable
};
struct CloseTreeNotificationPrefixResult {
    CloseTreeNotificationPrefixStop stop;
    bool suppressionReadThisAttempt;
    bool flag90StoredThisAttempt;
    const char* nextSourceSite;
    static constexpr bool selectionEnabledArgument = false;
    static constexpr bool callerArgumentProductionCompleted = false;
    static constexpr bool busySetterComplete = false;
    static constexpr bool modelCleanupComplete = false;
    static constexpr bool completeForPublicClose = false;
};

// Explicit diagnostic callee-entry projection, not a native call/capture receipt.
// Uses the existing Session-owned notifier; labels cannot certify the missing
// 025080/QGenericArgument caller setup. The earlier busy prefix must already be
// reached. No reference to that prefix object survives this constructor.
class CloseTreeNotificationPrefixProgress final {
public:
    CloseTreeNotificationPrefixProgress(const ConversionSession&,
        CloseBusyGlobalOwner& alreadyReachedTrueBusyPrefix,
        std::string sourceOccurrence, std::string qualification);
    CloseTreeNotificationPrefixProgress(const CloseTreeNotificationPrefixProgress&) = delete;
    CloseTreeNotificationPrefixProgress& operator=(const CloseTreeNotificationPrefixProgress&) = delete;
    CloseTreeNotificationPrefixProgress(CloseTreeNotificationPrefixProgress&&) = delete;
    CloseTreeNotificationPrefixProgress& operator=(CloseTreeNotificationPrefixProgress&&) = delete;
    CloseTreeNotificationPrefixStage stage() const noexcept { return stage_; }
private:
    friend CloseTreeNotificationPrefixResult attemptCloseTreeNotificationPrefix(
        const ConversionSession&, CloseTreeNotificationPrefixProgress&);
    std::uintptr_t sessionIdentity_; // opaque comparison only; never dereferenced
    std::weak_ptr<ModelManager> manager_;
    std::weak_ptr<doe::SessionNotificationState> notifier_;
    std::string qualification_;
    CloseTreeNotificationPrefixStage stage_ = CloseTreeNotificationPrefixStage::AwaitSuppressionRead;
};

// 2cf3f1: suppressed skips directly to2cf72e with no store. Otherwise2cf408
// writes this actual notifier's flag90=false, then stops BEFORE2cf41f's key
// QString constructor. No map, snapshot, attempt log, callback or timer effect.
// Retry reports the consumed phase without rereading suppression/replaying the
// store. No public Close/busy callsite or method to bypass this guard exists.
CloseTreeNotificationPrefixResult attemptCloseTreeNotificationPrefix(
    const ConversionSession&, CloseTreeNotificationPrefixProgress&);
} // namespace m2m::engine
