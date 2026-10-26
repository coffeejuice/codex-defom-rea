// Isolated late Close false setter. No public Close or notifier integration.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>

#include "m2m/engine/close_busy_prefix.hpp"

namespace m2m::engine {
inline constexpr const char* closeBusyFalseSourceOccurrence = "0x140029c9b:0x1402ca9f0(false)";
inline constexpr const char* closeBusyFalseMainProfile = "0x1420fb108:virtual+300=0x14001daf0";

struct CloseBusyFalseMainField {
    std::int32_t value;
    std::uint64_t mainIdentity;
    std::string qualification;
};

// Supplied late-attempt observations, not native field owners/authentication.
// mainIdentity is an observation-local nonzero alias, not a native address.
// A binding receipt must identify the actual current global main window and its
// recovered vtable profile. Neither MainViewState nor CP58 entry facts imply it.
struct CloseBusyFalseMainObservations {
    const ConversionSession* sessionOwner = nullptr;
    std::string sourceOccurrence;
    std::optional<bool> globalMainPresent;
    std::string globalMainQualification;
    std::uint64_t mainIdentity = 0;
    std::string mainBindingQualification;
    std::string mainProfile;
    std::optional<CloseBusyFalseMainField> stackedCurrentIndex;
    std::optional<CloseBusyFalseMainField> mainField270;
};

enum class CloseBusyFalseStage {
    BeforeResetStores,
    AwaitMainBusyFacts,
    AwaitStackedCurrentIndex,
    AwaitMainField270,
    BeforeTreeEnableSelectionDispatch
};
enum class CloseBusyFalseStop { MainBusyFactsUnavailable, TreeEnableSelectionDispatchUnavailable };
struct CloseBusyFalseResult {
    CloseBusyFalseStop stop;
    bool resetCommittedThisAttempt;
    bool branchCommittedThisAttempt;
    std::optional<bool> selectionEnabledArgument;
    const char* nextSourceSite;
};

// This diagnostic projection is NOT another production global owner. Future
// integration must share actual mutable storage with the true setter instead
// of copying its fields. All initial values and this late occurrence are explicit.
class CloseBusyFalseGlobalOwner final {
public:
    CloseBusyFalseGlobalOwner(const ConversionSession&, CloseBusyNativeFields,
                              std::string sourceOccurrence, std::string qualification);
    CloseBusyFalseGlobalOwner(const CloseBusyFalseGlobalOwner&) = delete;
    CloseBusyFalseGlobalOwner& operator=(const CloseBusyFalseGlobalOwner&) = delete;
    CloseBusyFalseGlobalOwner(CloseBusyFalseGlobalOwner&&) = delete;
    CloseBusyFalseGlobalOwner& operator=(CloseBusyFalseGlobalOwner&&) = delete;

    const CloseBusyNativeFields& fields() const noexcept { return fields_; }
    CloseBusyFalseStage stage() const noexcept { return stage_; }
    const std::string& qualification() const noexcept { return qualification_; }
    std::optional<bool> requestedSelectionEnabled() const noexcept { return requestedEnable_; }
    std::uint64_t observedMainIdentity() const noexcept { return mainIdentity_; }
    std::optional<std::int32_t> observedStackedIndex() const noexcept { return stackedIndex_; }
private:
    friend CloseBusyFalseResult attemptCloseBusyFalsePrefix(const ConversionSession&,
        CloseBusyFalseGlobalOwner&, const CloseBusyFalseMainObservations&);
    const ConversionSession* session_;
    std::weak_ptr<ModelManager> manager_;
    CloseBusyNativeFields fields_;
    std::string qualification_;
    CloseBusyFalseStage stage_ = CloseBusyFalseStage::BeforeResetStores;
    std::uint64_t mainIdentity_ = 0;
    std::optional<std::int32_t> stackedIndex_;
    std::optional<bool> requestedEnable_;
};

// Validates the live Session/manager before stores. Then +9=0/counter=0 precede
// missing-main guards. Resolved main busy=(index==2 || main+270==1), in order;
// null main is unbusy. Commits +A then+B to !busy, and stops before notifier
// argument construction/population/Qt slot. No modal even when main stays busy.
// Retry retains partial progress and a resolved branch's latched Boolean.
CloseBusyFalseResult attemptCloseBusyFalsePrefix(const ConversionSession&,
    CloseBusyFalseGlobalOwner&, const CloseBusyFalseMainObservations&);
} // namespace m2m::engine
