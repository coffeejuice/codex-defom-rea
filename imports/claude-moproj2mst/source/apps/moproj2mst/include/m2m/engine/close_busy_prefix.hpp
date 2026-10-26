// Isolated accepted-Close true busy prefix, not public Close integration.
#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace m2m::engine {
class ConversionSession;
class ModelManager;

inline constexpr const char* closeBusyTrueSourceOccurrence = "0x140029a54:0x1402ca9f0(true)";

// Every value is supplied explicitly; no fresh-zero native defaults are assumed.
struct CloseBusyNativeFields {
    constexpr CloseBusyNativeFields(std::uint8_t busy, std::uint8_t a,
                                   std::uint8_t b, std::uint32_t counter) noexcept
        : flag9(busy), fieldA(a), fieldB(b), counter143d91230(counter) {}
    std::uint8_t flag9;
    std::uint8_t fieldA;
    std::uint8_t fieldB;
    std::uint32_t counter143d91230;
};

enum class CloseBusyPrefixStage { BeforeTrueStore, BeforeTreeEnableSelectionDispatch };
enum class CloseBusyPrefixStop { TreeEnableSelectionDispatchUnavailable };

struct CloseBusyPrefixResult {
    CloseBusyPrefixStop stop;
    bool prefixCommittedThisAttempt;
    bool selectionEnabledArgument; // exact pending notifier argument, always false
    const char* nextSourceSite;
};

// A single isolated global-state projection associated with the actual Session
// and its manager identity. This is not a native observation authenticator or
// production Session field binding. Qualification labels caller-supplied evidence.
// The caller must establish that all actual can-close guards already allowed at
// this occurrence. Construction itself neither runs those guards nor mutates it.
class CloseBusyGlobalOwner final {
public:
    CloseBusyGlobalOwner(const ConversionSession&, CloseBusyNativeFields,
                         std::string sourceOccurrence, std::string qualification);
    CloseBusyGlobalOwner(const CloseBusyGlobalOwner&) = delete;
    CloseBusyGlobalOwner& operator=(const CloseBusyGlobalOwner&) = delete;
    CloseBusyGlobalOwner(CloseBusyGlobalOwner&&) = delete;
    CloseBusyGlobalOwner& operator=(CloseBusyGlobalOwner&&) = delete;

    const CloseBusyNativeFields& fields() const noexcept { return fields_; }
    CloseBusyPrefixStage stage() const noexcept { return stage_; }
    const std::string& qualification() const noexcept { return qualification_; }
private:
    friend CloseBusyPrefixResult attemptCloseBusyTruePrefix(
        const ConversionSession&, CloseBusyGlobalOwner&);
    const ConversionSession* session_;
    std::weak_ptr<ModelManager> manager_; // identity check, never keeps Session alive
    CloseBusyNativeFields fields_;
    std::string qualification_;
    CloseBusyPrefixStage stage_ = CloseBusyPrefixStage::BeforeTrueStore;
};

// Requires the same live Session on every attempt; never dereferences session_.
// Commits +9=1, +A=0, +B=0 in native order, leaves counter unchanged, then stops
// before generic-argument construction/TreeEnableSelection(false) dispatch.
// Retry reports the same boundary without replaying stores. No notifier, timer,
// model, Project, OPI, Save, modal, false-reset or destruction effects are run.
CloseBusyPrefixResult attemptCloseBusyTruePrefix(
    const ConversionSession&, CloseBusyGlobalOwner&);
} // namespace m2m::engine
