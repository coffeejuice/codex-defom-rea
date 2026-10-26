#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "m2m/engine/view_manager.hpp"
#include "m2m/engine/application_palette_owner.hpp"

namespace m2m::engine {

enum class SplitViewPlaceholderStage { NotStarted, PrivatePalette, ApplicationPaletteGetter, PrivatePaletteCopy, PaletteTemporaryDeref };
enum class SplitViewPlaceholderIssue {
    None, MissingQualification, RuntimeUnavailable, RuntimeOwnerMismatch,
    RuntimeQualificationUnavailable, WidgetOwnerUnavailable, PrefixStateChanged,
    CurrentPaletteUnavailable, PrivateInputsQualificationMismatch, PaletteCopyUnrecovered,
    ApplicationPaletteOwnerUnavailable, ApplicationPaletteGetterPending,
    PaletteNativeReferenceUnavailable, PaletteRefUnavailable, PaletteTemporaryDerefUnrecovered
};
enum class SplitViewPlaceholderEffect {
    PlaceholderStorageAllocated, WidgetPrivateAllocated,
    QObjectPrivateConstructed, WidgetPrivatePrefix, ApplicationPaletteGetterReturned,
    PrivatePaletteDataStored
};
struct SplitViewPlaceholderReport {
    SplitViewPlaceholderStage stage = SplitViewPlaceholderStage::NotStarted;
    SplitViewPlaceholderIssue issue = SplitViewPlaceholderIssue::None;
    bool attempted = false;
    static constexpr bool qwidgetReturned = false;
    static constexpr bool completeForPublicSave = false;
};

// One normal nonnull dab94c destination, not an Editor300 or its call frame.
// Diagnostic shared WidgetState leases do not extend this native value lifetime.
class SplitViewPlaceholderValue final {
public:
    ~SplitViewPlaceholderValue();
    SplitViewPlaceholderValue(const SplitViewPlaceholderValue&) = delete;
    SplitViewPlaceholderValue& operator=(const SplitViewPlaceholderValue&) = delete;
    std::shared_ptr<WidgetState> widgetState() const noexcept { return widget_; }
    bool nativeOwnerAlive() const noexcept { return widget_->nativeOwnerAlive(); }
private:
    friend class SplitViewPlaceholderCall;
    SplitViewPlaceholderValue();
    std::shared_ptr<WidgetState> widget_;
};

// Isolated prefix through one getter and private data-pointer store only. No production
// SplitView callsite exists until its earlier QWidget/QFrame/QSplitter gates return.
// @src 0x140dab94c -> QWidget65070d70 -> QWidgetPrivate6505e780..6505e8ca
class SplitViewPlaceholderCall final {
public:
    SplitViewPlaceholderCall(const std::shared_ptr<WidgetRuntimeState>&, std::string qualification);
    ~SplitViewPlaceholderCall();
    SplitViewPlaceholderCall(const SplitViewPlaceholderCall&) = delete;
    SplitViewPlaceholderCall& operator=(const SplitViewPlaceholderCall&) = delete;
    const SplitViewPlaceholderReport& resume();
    const SplitViewPlaceholderReport& report() const noexcept { return report_; }
    const std::vector<SplitViewPlaceholderEffect>& effects() const noexcept { return effects_; }
    const SplitViewPlaceholderValue* value() const noexcept { return value_.get(); }
    const std::string& qualification() const noexcept { return qualification_; }
    const QApplicationPaletteRead* applicationPaletteRead() const noexcept
    { return paletteRead_ ? &*paletteRead_ : nullptr; }
    std::weak_ptr<const QApplicationPaletteOwnerState> applicationPaletteOwner() const noexcept { return paletteOwner_; }
private:
    std::weak_ptr<WidgetRuntimeState> runtime_;
    SplitViewPlaceholderIssue runtimeBindingIssue_ = SplitViewPlaceholderIssue::None;
    std::string qualification_;
    std::unique_ptr<SplitViewPlaceholderValue> value_;
    std::weak_ptr<const QApplicationPaletteOwnerState> paletteOwner_;
    std::optional<QApplicationPaletteRead> paletteRead_;
    SplitViewPlaceholderReport report_;
    std::vector<SplitViewPlaceholderEffect> effects_;
};

} // namespace m2m::engine
