// Isolated status-entry null-main skip. No public Close completion.
#pragma once

#include <memory>
#include <optional>
#include <string>

namespace m2m::engine {
class ConversionSession;
class ModelManager;

inline constexpr const char* closeStatusSourceOccurrence =
    "0x140029ce4:0x1402d0bd0(normal-close-status)";

// Fresh observations for this status invocation, AFTER the false setter and its
// synchronous receivers. Earlier Close busy facts/MainView existence are not a
// substitute. Qualification labels supplied evidence; it does not authenticate it.
struct CloseStatusMainObservation {
    const ConversionSession* sessionOwner = nullptr;
    std::string sourceOccurrence;
    std::optional<bool> globalMainPresent;
    std::string qualification;
};

enum class CloseStatusPrefixStage {
    AwaitMainLookup,
    NonNullStatusBarGuard,
    NullSkipBeforeCallerQStringRelease
};
enum class CloseStatusPrefixStop {
    MainLookupUnavailable,
    NonNullStatusBarEffectsUnavailable,
    NullMainSkipBeforeCallerQStringRelease
};
struct CloseStatusPrefixResult {
    CloseStatusPrefixStop stop;
    bool lookupCommittedThisAttempt;
    std::optional<bool> observedMainPresent;
    const char* nextSourceSite;
};

// Session/manager-bound isolated continuation only. Creating this progress does
// not prove earlier Close/teardown/false-notification phases completed. There is
// no production global-main storage binding or caller QString owner added here.
class CloseStatusPrefixProgress final {
public:
    CloseStatusPrefixProgress(const ConversionSession&, std::string sourceOccurrence,
                              std::string qualification);
    CloseStatusPrefixProgress(const CloseStatusPrefixProgress&) = delete;
    CloseStatusPrefixProgress& operator=(const CloseStatusPrefixProgress&) = delete;
    CloseStatusPrefixProgress(CloseStatusPrefixProgress&&) = delete;
    CloseStatusPrefixProgress& operator=(CloseStatusPrefixProgress&&) = delete;

    CloseStatusPrefixStage stage() const noexcept { return stage_; }
    std::optional<bool> observedMainPresent() const noexcept { return mainPresent_; }
    const std::string& qualification() const noexcept { return qualification_; }
private:
    friend CloseStatusPrefixResult attemptCloseStatusPrefix(const ConversionSession&,
        CloseStatusPrefixProgress&, const CloseStatusMainObservation&);
    const ConversionSession* session_;
    std::weak_ptr<ModelManager> manager_;
    std::string qualification_;
    CloseStatusPrefixStage stage_ = CloseStatusPrefixStage::AwaitMainLookup;
    std::optional<bool> mainPresent_;
};

// Fresh qualified null global main skips2d0bd0's status/color/ShowMsgInArea work,
// then stops BEFORE caller029cea/029cf2 QString release and029cf8 public true.
// Nonnull stops before QMainWindow::statusBar at2d0c08, including when notifier
// suppression is true. Both lookup outcomes are latched; retry never re-queries.
// No native lookup, notification, tree/status/global mutation, timer destruction,
// QObject teardown, QString construction/release or public Close is executed.
CloseStatusPrefixResult attemptCloseStatusPrefix(const ConversionSession&,
    CloseStatusPrefixProgress&, const CloseStatusMainObservation&);
} // namespace m2m::engine
