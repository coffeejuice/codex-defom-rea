#include "m2m/engine/application_palette_owner.hpp"
#include <utility>

namespace m2m::engine {

QApplicationPaletteOwnerState::QApplicationPaletteOwnerState(std::string qualification)
    : qualification_(std::move(qualification))
{}

bool QApplicationPaletteOwnerState::matchesQualification(const std::string& qualification) const noexcept
{
    return !qualification_.empty() && qualification == qualification_;
}

bool QApplicationPaletteOwnerState::observeNullSlot(const std::string& qualification)
{
    if (!matchesQualification(qualification)) return false;
    if (knowledge_ == QApplicationPaletteSlotKnowledge::Unknown) {
        knowledge_ = QApplicationPaletteSlotKnowledge::KnownNull;
        return true;
    }
    return knowledge_ == QApplicationPaletteSlotKnowledge::KnownNull && !wrapper_;
}

bool QApplicationPaletteOwnerState::observePresentSlot(
    const std::string& qualification, WidgetPaletteValue value)
{
    if (!matchesQualification(qualification) || !value.data) return false;
    if (knowledge_ == QApplicationPaletteSlotKnowledge::Unknown) {
        auto wrapper = std::make_shared<QApplicationPaletteWrapper>();
        wrapper->value = std::move(value);
        wrapper_ = std::move(wrapper);
        knowledge_ = QApplicationPaletteSlotKnowledge::KnownPresent;
        return true;
    }
    return knowledge_ == QApplicationPaletteSlotKnowledge::KnownPresent && wrapper_ &&
        wrapper_->value.data == value.data &&
        wrapper_->value.groupResolveWord == value.groupResolveWord;
}

QApplicationPaletteRead QApplicationPaletteOwnerState::read(
    const DefaultPaletteConstructor& constructDefault)
{
    QApplicationPaletteRead result;
    if (qualification_.empty()) {
        result.issue = QApplicationPaletteReadIssue::QualificationUnavailable;
        return result;
    }
    if (knowledge_ == QApplicationPaletteSlotKnowledge::Unknown) {
        result.issue = QApplicationPaletteReadIssue::SlotStateUnknown;
        return result;
    }
    if (knowledge_ == QApplicationPaletteSlotKnowledge::KnownNull) {
        if (!constructDefault) {
            result.issue = QApplicationPaletteReadIssue::InitializerUnavailable;
            return result;
        }
        auto value = constructDefault(nativeDefaultGlobalColorArgument);
        if (!value || !value->data) {
            result.issue = QApplicationPaletteReadIssue::InitializedPaletteUnavailable;
            return result;
        }

        // Only the normal completed allocation/constructor/cache path is
        // represented. Constructor unwind and allocation failure are outside
        // this isolated state transition.
        auto wrapper = std::make_shared<QApplicationPaletteWrapper>();
        wrapper->value = std::move(*value);
        wrapper_ = std::move(wrapper);
        knowledge_ = QApplicationPaletteSlotKnowledge::KnownPresent;
        result.effects.push_back(QApplicationPaletteEffect::WrapperAllocated);
        result.effects.push_back(QApplicationPaletteEffect::GlobalColorPaletteConstructed);
        result.effects.push_back(QApplicationPaletteEffect::SlotCached);
    }
    if (!wrapper_ || !wrapper_->value.data) {
        result.issue = QApplicationPaletteReadIssue::SlotStateUnknown;
        result.effects.clear();
        return result;
    }
    result.output = wrapper_->value;
    result.effects.push_back(QApplicationPaletteEffect::OutputCopied);
    return result;
}

} // namespace m2m::engine
