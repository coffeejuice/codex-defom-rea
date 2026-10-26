#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "m2m/engine/brush_texture_image.hpp"
#include "m2m/engine/paint_device_cleanup.hpp"
#include "m2m/engine/paint_device_prefix.hpp"

namespace m2m::engine {

class PaintDeviceActiveCopySourceState;
struct PaintDeviceActiveCopySourceObservation {
    std::weak_ptr<PaintDeviceActiveCopySourceState> receiver;
    std::string qualification;
    std::optional<std::uint16_t> paintingWord;
    // Unknown versus qualified null versus the actual shared QImageData owner.
    std::optional<std::shared_ptr<BrushTextureImageDataState>> imageData;
};

// Standalone isolated owner, or a delegated VIEW of one canonical embedded image
// with its weak module origin. Runtime integration must intern that view once
// per actual brush payload; this factory alone does not prove registry membership.
// Copy calls borrow it; diagnostic metadata leases do not extend native life.
class PaintDeviceActiveCopySourceState final
    : public std::enable_shared_from_this<PaintDeviceActiveCopySourceState> {
public:
    explicit PaintDeviceActiveCopySourceState(std::string qualification);
    ~PaintDeviceActiveCopySourceState();
    PaintDeviceActiveCopySourceState(const PaintDeviceActiveCopySourceState&) = delete;
    PaintDeviceActiveCopySourceState& operator=(const PaintDeviceActiveCopySourceState&) = delete;
    static std::shared_ptr<PaintDeviceActiveCopySourceState> fromTextureImageSource(
        std::shared_ptr<const BrushTextureImageState>, const std::shared_ptr<PaintDeviceCleanupGlobals>&,
        std::string qualification);
    bool observe(const PaintDeviceActiveCopySourceObservation&) noexcept;
    void releaseNativeOwner() noexcept { alive_ = false; }
    bool nativeOwnerAlive() const noexcept;
    bool qualified() const noexcept { return qualified_; }
    const std::string& qualification() const noexcept { return qualification_; }
    bool textureImageBound() const noexcept { return !!textureImage_; }
    std::shared_ptr<const BrushTextureImageState> textureImageSource() const noexcept { return textureImage_; }
    std::weak_ptr<PaintDeviceCleanupGlobals> cleanupGlobalsOwner() const noexcept { return globals_; }
    std::optional<std::uint16_t> paintingWord() const noexcept {
        return textureImage_ ? textureImage_->imagePaintingActiveWord() : paintingWord_;
    }
    const std::optional<std::shared_ptr<BrushTextureImageDataState>>& imageData() const noexcept {
        return textureImage_ ? textureImage_->imageDataOwner() : imageData_;
    }
private:
    std::string qualification_;
    bool alive_ = true, qualified_ = false;
    std::optional<std::uint16_t> paintingWord_;
    std::optional<std::shared_ptr<BrushTextureImageDataState>> imageData_;
    // Strong host lease keeps metadata references stable, not the native module
    // alive. Bound observations go ONLY through the actual payload's observer.
    std::shared_ptr<const BrushTextureImageState> textureImage_;
    std::weak_ptr<PaintDeviceCleanupGlobals> globals_;
};

class PaintDeviceActiveCopyCall;
// Native lifetime responsibility is held by this unique value owner, not by its
// diagnostic prefix-state leases. Destruction marks host abandonment only; it
// does not invent a native QImage destructor/prefix/cleanup effect.
class PaintDeviceActiveCopyValue final {
public:
    ~PaintDeviceActiveCopyValue();
    PaintDeviceActiveCopyValue(const PaintDeviceActiveCopyValue&) = delete;
    PaintDeviceActiveCopyValue& operator=(const PaintDeviceActiveCopyValue&) = delete;
    std::shared_ptr<PaintDevicePrefixState> prefixState() const noexcept { return prefix_; }
    const std::optional<std::shared_ptr<BrushTextureImageDataState>>& imageData() const noexcept { return imageData_; }
    // Host provenance for the actual QtGui module instance, not a native QImage
    // field/ref operation. Set only by the producer after inner cleanup returns.
    std::weak_ptr<PaintDeviceCleanupGlobals> cleanupGlobalsOwner() const noexcept { return cleanupGlobals_; }
private:
    friend class PaintDeviceActiveCopyCall;
    explicit PaintDeviceActiveCopyValue(std::shared_ptr<PaintDevicePrefixState>);
    std::shared_ptr<PaintDevicePrefixState> prefix_;
    std::optional<std::shared_ptr<BrushTextureImageDataState>> imageData_;
    std::weak_ptr<PaintDeviceCleanupGlobals> cleanupGlobals_;
};

enum class PaintDeviceActiveCopyStage {
    SourceQualification, OuterConstruction, SourcePaintingWord, InactiveSourceContinuation,
    OuterNullInitialization, RectangleConstruction, CopySourceData, NonNullSourceContinuation,
    InnerConstruction, NullAssignment, InnerImageDestruction, InnerPrefix, InnerCleanup, Returned
};
enum class PaintDeviceActiveCopyIssue {
    None, SourceUnavailable, SourceQualificationUnavailable, CallQualificationUnavailable,
    SourcePaintingWordUnavailable, InactiveSourceUnrecovered, SourceImageDataUnavailable,
    NonNullActiveCopyUnrecovered, TemporaryOwnerUnavailable, InnerPrefixPending,
    CleanupGlobalsUnavailable, InnerCleanupPending
};
enum class PaintDeviceActiveCopyEffect {
    OuterImageConstructed, SourcePaintingWordRead, OuterNullInitialized, DefaultRectangleConstructed,
    CopySourceDataRead, InnerImageConstructed, AssignmentInnerWordRead, AssignmentInnerDataRead,
    AssignmentOuterDataRead, AssignmentInnerDataReread, OuterNullAssigned,
    InnerQImageClassInstalled, InnerImageDataRead, InnerPrefixTailReached,
    InnerCleanupReturned, InnerImageDestroyed, ActiveCopyReturned
};
struct PaintDeviceActiveCopyReport {
    PaintDeviceActiveCopyStage stage = PaintDeviceActiveCopyStage::SourceQualification;
    PaintDeviceActiveCopyIssue issue = PaintDeviceActiveCopyIssue::None;
    std::optional<std::uint16_t> readSourcePaintingWord;
    static constexpr bool completeForPublicSave = false;
};

//650b9a70 active-source/null-data normal copy only. The inner null image must
// finish its zero prefix and selected actual-global cleanup before outer copy
// return. No alpha/opacity or outer destruction is performed. Inactive, unknown,
// nonnull, warning, lazy/nonzero-global and public constructor paths stay guarded.
class PaintDeviceActiveCopyCall final {
public:
    PaintDeviceActiveCopyCall(const std::shared_ptr<PaintDeviceActiveCopySourceState>&, std::string qualification);
    PaintDeviceActiveCopyCall(const PaintDeviceActiveCopyCall&) = delete;
    PaintDeviceActiveCopyCall& operator=(const PaintDeviceActiveCopyCall&) = delete;
    bool bindCleanupGlobals(const std::shared_ptr<PaintDeviceCleanupGlobals>&) noexcept;
    PaintDeviceActiveCopyReport resume();
    const PaintDeviceActiveCopyReport& report() const noexcept { return report_; }
    const std::vector<PaintDeviceActiveCopyEffect>& effects() const noexcept { return effects_; }
    std::shared_ptr<PaintDevicePrefixState> returnedImagePrefixState() const noexcept { return outerPrefix_.lock(); }
    std::shared_ptr<PaintDevicePrefixState> innerImagePrefixState() const noexcept { return inner_ ? inner_->prefixState() : nullptr; }
    const PaintDevicePrefixCall* innerPrefixCall() const noexcept { return innerPrefix_.get(); }
    const PaintDeviceCleanupCall* innerCleanupCall() const noexcept { return innerCleanup_.get(); }
    const std::optional<std::array<std::int32_t, 4>>& defaultRectangle() const noexcept { return rectangle_; }
    // Explicit native-value ownership transfer only after copy return. Merely
    // taking a diagnostic prefix lease cannot keep an abandoned value callable.
    std::unique_ptr<PaintDeviceActiveCopyValue> takeReturnedImage() noexcept;
private:
    std::weak_ptr<PaintDeviceActiveCopySourceState> source_;
    std::string qualification_;
    bool globalsBound_ = false;
    std::weak_ptr<PaintDeviceCleanupGlobals> globals_;
    std::unique_ptr<PaintDeviceActiveCopyValue> outer_, inner_;
    std::weak_ptr<PaintDevicePrefixState> outerPrefix_;
    std::unique_ptr<PaintDevicePrefixCall> innerPrefix_;
    std::unique_ptr<PaintDeviceCleanupCall> innerCleanup_;
    std::optional<std::array<std::int32_t, 4>> rectangle_;
    PaintDeviceActiveCopyReport report_;
    std::vector<PaintDeviceActiveCopyEffect> effects_;
};

enum class PaintDeviceNullImageFinishStage {
    ValueQualification, AlphaPredicate, QImageDestruction, PaintDevicePrefix, Cleanup, Returned
};
enum class PaintDeviceNullImageFinishIssue {
    None, ValueUnavailable, ValueClassUnrecovered, ImageDataUnavailable,
    NonNullImageUnrecovered, ModuleOriginUnavailable, PrefixPending, CleanupPending
};
enum class PaintDeviceNullImageFinishEffect {
    PredicateImageDataRead, AlphaPredicateReturnedFalse, OpacityNegated,
    QImageClassInstalled, QImageDataRead, PaintDevicePrefixTailReached,
    CleanupReturned, OuterImageDestroyed, OpacityReturned
};
struct PaintDeviceNullImageFinishReport {
    PaintDeviceNullImageFinishStage stage = PaintDeviceNullImageFinishStage::ValueQualification;
    PaintDeviceNullImageFinishIssue issue = PaintDeviceNullImageFinishIssue::None;
    std::optional<bool> opaque; // Absent until the same outer value's destructor returns.
    static constexpr bool completeForPublicSave = false;
};

//6511c18d..c1db null returned-image branch only. Own the actual returned value,
// retain !hasAlpha privately, then finish its QImage prefix, QPaintDevice prefix
// and separate actual-module cleanup before publishing opacity. No independent
// module binding/replacement, copied value, source observation or GUI admission.
class PaintDeviceNullImageFinishCall final {
public:
    explicit PaintDeviceNullImageFinishCall(std::unique_ptr<PaintDeviceActiveCopyValue>);
    PaintDeviceNullImageFinishCall(const PaintDeviceNullImageFinishCall&) = delete;
    PaintDeviceNullImageFinishCall& operator=(const PaintDeviceNullImageFinishCall&) = delete;
    PaintDeviceNullImageFinishReport resume();
    const PaintDeviceNullImageFinishReport& report() const noexcept { return report_; }
    const std::vector<PaintDeviceNullImageFinishEffect>& effects() const noexcept { return effects_; }
    std::shared_ptr<PaintDevicePrefixState> valuePrefixState() const noexcept { return value_ ? value_->prefixState() : nullptr; }
    std::weak_ptr<PaintDeviceCleanupGlobals> cleanupGlobalsOwner() const noexcept { return globals_; }
    const PaintDevicePrefixCall* prefixCall() const noexcept { return prefix_.get(); }
    const PaintDeviceCleanupCall* cleanupCall() const noexcept { return cleanup_.get(); }
private:
    std::unique_ptr<PaintDeviceActiveCopyValue> value_;
    std::weak_ptr<PaintDeviceCleanupGlobals> globals_;
    bool privateOpaque_ = false;
    std::unique_ptr<PaintDevicePrefixCall> prefix_;
    std::unique_ptr<PaintDeviceCleanupCall> cleanup_;
    PaintDeviceNullImageFinishReport report_;
    std::vector<PaintDeviceNullImageFinishEffect> effects_;
};

} // namespace m2m::engine
