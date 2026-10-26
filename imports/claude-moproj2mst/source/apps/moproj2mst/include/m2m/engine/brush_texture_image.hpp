#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

namespace m2m::engine {

// Internal style24 payloads. The actual WidgetRuntimeState must intern these
// owners and qualify observations before binding them to its shared brush data.
// This helper neither supplies startup observations nor admits public mode300.
struct BrushTextureImageDataObservation {
    std::optional<std::int32_t> format; // native QImageData+30
    std::optional<bool> colorTableAlphaBit; // native QImageData+58 mask4
};

class BrushTextureImageOpacityCall;
class BrushTextureImageDataState final {
public:
    explicit BrushTextureImageDataState(std::uint32_t alias) : alias_(alias) {}
    std::uint32_t observationAlias() const noexcept { return alias_; }
    bool canObserve(const BrushTextureImageDataObservation&) const noexcept;
    // Conflict rejection is atomic for this one data observation.
    bool observe(const BrushTextureImageDataObservation&) noexcept;
    std::size_t temporaryValueReferences() const noexcept { return temporaryReferences_; }
private:
    friend class BrushTextureImageOpacityCall;
    std::uint32_t alias_;
    std::optional<std::int32_t> format_;
    std::optional<bool> colorTableAlphaBit_;
    // Only refs introduced by modeled QImage return values. Host shared_ptr
    // references and unobserved absolute native ref counts are not substituted.
    std::size_t temporaryReferences_ = 0;
};

struct BrushTextureImageObservation {
    std::optional<bool> pixmapSelected; // brushData+90 !=0
    std::optional<bool> cachedPixmapPresent; // brushData+70 !=nullptr
    // Disengaged: unknown. Engaged with nullptr: qualified null QImageData.
    std::optional<std::shared_ptr<BrushTextureImageDataState>> imageData;
    std::optional<std::uint16_t> imagePaintingActiveWord; // embedded QImage+8
};

// The source image value embedded at brushData+78. Multiple call frames may
// reference this same state; no editor-local copy of its cache is constructed.
class BrushTextureImageState final {
public:
    bool canObserve(const BrushTextureImageObservation&) const noexcept;
    bool observe(const BrushTextureImageObservation&) noexcept;
    // Canonical embedded-image fields. A bound copy-source view delegates here;
    // it must not maintain a second observation/snapshot of this same value.
    const std::optional<std::shared_ptr<BrushTextureImageDataState>>& imageDataOwner() const noexcept { return imageData_; }
    std::optional<std::uint16_t> imagePaintingActiveWord() const noexcept { return imagePaintingActiveWord_; }
private:
    friend class BrushTextureImageOpacityCall;
    std::optional<bool> pixmapSelected_, cachedPixmapPresent_;
    std::optional<std::shared_ptr<BrushTextureImageDataState>> imageData_;
    std::optional<std::uint16_t> imagePaintingActiveWord_;
};

enum class BrushTextureImageStage {
    Selection, ImageGetter, ImagePixmapCache, ImageCopy, ImageFormat, ImageColorTableAlpha, Returned
};
enum class BrushTextureImageIssue {
    None, SourceUnavailable, SelectionUnavailable, PixmapPathUnrecovered,
    ImageDataUnavailable, ImageOwnerUnavailable, PixmapCachePresenceUnavailable,
    ImageConversionUnrecovered, PaintingActiveUnavailable, ActiveImageCopyUnrecovered,
    ImageFormatUnavailable, ImageColorTableAlphaUnavailable
};
enum class BrushTextureImageEffect {
    ImageNullnessRead, ImageValueCopied, ImageDataRetained, ImageFormatRead,
    ImageColorTableAlphaRead, ImageDataReleased, ImageValueDestroyed
};
struct BrushTextureImageReport {
    BrushTextureImageStage stage = BrushTextureImageStage::Selection;
    BrushTextureImageIssue issue = BrushTextureImageIssue::None;
    std::optional<bool> opaque;
};

// Begins only after the caller's source-qualified style24 dispatch and consumed
// brush alpha. The image copy remains alive across a metadata guard. Ownership
// effects are resumed once; completion destroys it before exposing the result.
class BrushTextureImageOpacityCall final {
public:
    explicit BrushTextureImageOpacityCall(std::shared_ptr<const BrushTextureImageState>);
    ~BrushTextureImageOpacityCall();
    BrushTextureImageOpacityCall(const BrushTextureImageOpacityCall&) = delete;
    BrushTextureImageOpacityCall& operator=(const BrushTextureImageOpacityCall&) = delete;
    BrushTextureImageReport resume();
    const BrushTextureImageReport& report() const noexcept { return report_; }
    const std::vector<BrushTextureImageEffect>& effects() const noexcept { return effects_; }
private:
    void complete(bool opaque);
    void dropTemporary() noexcept;
    std::shared_ptr<const BrushTextureImageState> source_;
    std::shared_ptr<BrushTextureImageDataState> temporaryData_;
    bool retained_ = false;
    BrushTextureImageReport report_;
    std::vector<BrushTextureImageEffect> effects_;
};

} // namespace m2m::engine
