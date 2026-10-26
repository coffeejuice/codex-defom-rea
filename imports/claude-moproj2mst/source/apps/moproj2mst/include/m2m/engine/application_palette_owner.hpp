#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "m2m/engine/view_manager.hpp"

namespace m2m::engine {
class PaletteNativeValueRef;

enum class QApplicationPaletteSlotKnowledge { Unknown, KnownNull, KnownPresent };
enum class QApplicationPaletteReadIssue {
    None, QualificationUnavailable, SlotStateUnknown, InitializerUnavailable,
    InitializedPaletteUnavailable
};
enum class QApplicationPaletteEffect {
    WrapperAllocated, GlobalColorPaletteConstructed, SlotCached, OutputCopied
};

// Model of QApplicationPrivate::app_pal only. This is deliberately not wired
// into WidgetRuntimeState's class palette cache or the SplitView callsite.
struct QApplicationPaletteWrapper final {
    WidgetPaletteValue value;
};

struct QApplicationPaletteRead final {
    QApplicationPaletteReadIssue issue = QApplicationPaletteReadIssue::None;
    std::optional<WidgetPaletteValue> output;
    std::vector<QApplicationPaletteEffect> effects;
    std::shared_ptr<const PaletteNativeValueRef> nativeReference() const noexcept { return nativeReference_; }
private:
    friend class WidgetRuntimeState;
    friend class SplitViewPlaceholderCall;
    // Host report copies share this actual getter token, never a second ref.
    std::shared_ptr<PaletteNativeValueRef> nativeReference_;
};

class QApplicationPaletteOwnerState final {
public:
    static constexpr std::int32_t nativeDefaultGlobalColorArgument = 2;
    using DefaultPaletteConstructor =
        std::function<std::optional<WidgetPaletteValue>(std::int32_t globalColor)>;

    explicit QApplicationPaletteOwnerState(std::string qualification);
    bool observeNullSlot(const std::string& qualification);
    bool observePresentSlot(const std::string& qualification, WidgetPaletteValue);
    QApplicationPaletteRead read(const DefaultPaletteConstructor& = {});

    QApplicationPaletteSlotKnowledge slotKnowledge() const noexcept { return knowledge_; }
    std::shared_ptr<const QApplicationPaletteWrapper> wrapperOwner() const noexcept { return wrapper_; }
    const std::string& qualification() const noexcept { return qualification_; }

private:
    bool matchesQualification(const std::string&) const noexcept;
    std::string qualification_;
    QApplicationPaletteSlotKnowledge knowledge_ = QApplicationPaletteSlotKnowledge::Unknown;
    std::shared_ptr<const QApplicationPaletteWrapper> wrapper_;
};

} // namespace m2m::engine
