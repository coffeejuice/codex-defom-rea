#include "m2m/engine/brush_texture_image.hpp"

#include <utility>

namespace m2m::engine {
namespace {
template<class T> bool conflict(const std::optional<T>& current,
    const std::optional<T>& next) noexcept
{ return current && next && *current != *next; }

template<class T> void merge(std::optional<T>& current, const std::optional<T>& next)
{ if (!current && next) current = next; }

bool sameDataOwner(const std::shared_ptr<BrushTextureImageDataState>& a,
    const std::shared_ptr<BrushTextureImageDataState>& b) noexcept
{ return a.get() == b.get() && !a.owner_before(b) && !b.owner_before(a); }
}

bool BrushTextureImageDataState::canObserve(const BrushTextureImageDataObservation& o) const noexcept
{
    return alias_ && !conflict(format_, o.format) && !conflict(colorTableAlphaBit_, o.colorTableAlphaBit);
}

bool BrushTextureImageDataState::observe(const BrushTextureImageDataObservation& o) noexcept
{
    if (!canObserve(o)) return false;
    merge(format_, o.format);
    merge(colorTableAlphaBit_, o.colorTableAlphaBit);
    return true;
}

bool BrushTextureImageState::canObserve(const BrushTextureImageObservation& o) const noexcept
{
    if (conflict(pixmapSelected_, o.pixmapSelected) ||
        conflict(cachedPixmapPresent_, o.cachedPixmapPresent) ||
        conflict(imagePaintingActiveWord_, o.imagePaintingActiveWord) ||
        (imageData_ && o.imageData && !sameDataOwner(*imageData_, *o.imageData)))
        return false;
    if (o.imageData && *o.imageData && !(*o.imageData)->observationAlias()) return false;
    return true;
}

bool BrushTextureImageState::observe(const BrushTextureImageObservation& o) noexcept
{
    if (!canObserve(o)) return false;
    merge(pixmapSelected_, o.pixmapSelected);
    merge(cachedPixmapPresent_, o.cachedPixmapPresent);
    merge(imageData_, o.imageData);
    merge(imagePaintingActiveWord_, o.imagePaintingActiveWord);
    return true;
}

BrushTextureImageOpacityCall::BrushTextureImageOpacityCall(
    std::shared_ptr<const BrushTextureImageState> source) : source_(std::move(source)) {}

BrushTextureImageOpacityCall::~BrushTextureImageOpacityCall() { dropTemporary(); }

void BrushTextureImageOpacityCall::dropTemporary() noexcept
{
    if (retained_) {
        --temporaryData_->temporaryReferences_;
        retained_ = false;
    }
    temporaryData_.reset();
}

void BrushTextureImageOpacityCall::complete(bool opaque)
{
    if (retained_) {
        dropTemporary();
        effects_.push_back(BrushTextureImageEffect::ImageDataReleased);
    }
    effects_.push_back(BrushTextureImageEffect::ImageValueDestroyed);
    report_.opaque = opaque;
    report_.stage = BrushTextureImageStage::Returned;
    report_.issue = BrushTextureImageIssue::None;
}

BrushTextureImageReport BrushTextureImageOpacityCall::resume()
{
    using Issue = BrushTextureImageIssue;
    using Stage = BrushTextureImageStage;
    using Effect = BrushTextureImageEffect;
    const auto pending = [&](Issue issue) {
        report_.issue = issue;
        return report_;
    };
    if (report_.stage == Stage::Returned) return report_;
    if (!source_) return pending(Issue::SourceUnavailable);
    if (report_.stage == Stage::Selection) {
        if (!source_->pixmapSelected_) return pending(Issue::SelectionUnavailable);
        //6511c112: pixmap path requires two distinct QPixmap return values and
        //platform virtual alpha/isQBitmap owners. It must not enter this helper.
        if (*source_->pixmapSelected_) return pending(Issue::PixmapPathUnrecovered);
        report_.stage = Stage::ImageGetter;
    }
    if (report_.stage == Stage::ImageGetter) {
        //6511a1b7..a1fb: isNull is evaluated before the cached pixmap pointer.
        if (!source_->imageData_) return pending(Issue::ImageDataUnavailable);
        effects_.push_back(Effect::ImageNullnessRead);
        report_.stage = *source_->imageData_ ? Stage::ImageCopy : Stage::ImagePixmapCache;
    }
    if (report_.stage == Stage::ImagePixmapCache) {
        if (!source_->cachedPixmapPresent_) return pending(Issue::PixmapCachePresenceUnavailable);
        if (*source_->cachedPixmapPresent_) return pending(Issue::ImageConversionUnrecovered);
        report_.stage = Stage::ImageCopy;
    }
    if (report_.stage == Stage::ImageCopy) {
        //650b9aa3 branches on the source QPaintDevice word even for null data.
        if (!source_->imagePaintingActiveWord_) return pending(Issue::PaintingActiveUnavailable);
        if (*source_->imagePaintingActiveWord_ != 0) return pending(Issue::ActiveImageCopyUnrecovered);
        temporaryData_ = *source_->imageData_;
        if (temporaryData_ && !temporaryData_->observationAlias()) return pending(Issue::ImageOwnerUnavailable);
        effects_.push_back(Effect::ImageValueCopied);
        if (temporaryData_) {
            ++temporaryData_->temporaryReferences_;
            retained_ = true;
            effects_.push_back(Effect::ImageDataRetained);
        }
        report_.stage = Stage::ImageFormat;
    }
    if (report_.stage == Stage::ImageFormat) {
        //650aa344..a347: null data returns hasAlpha=false, no format read.
        if (!temporaryData_) { complete(true); return report_; }
        if (!temporaryData_->format_) return pending(Issue::ImageFormatUnavailable);
        effects_.push_back(Effect::ImageFormatRead);
        const auto format = *temporaryData_->format_;
        if (format == 5 || format == 6 || format == 8 || format == 10 || format == 12 || format == 15) {
            complete(false);
            return report_;
        }
        //650aa36a reads bit4 before comparing formats1/2/3. Preserve this read
        //even for formats where its value cannot change the final result.
        report_.stage = Stage::ImageColorTableAlpha;
    }
    if (report_.stage == Stage::ImageColorTableAlpha) {
        if (!temporaryData_->colorTableAlphaBit_) return pending(Issue::ImageColorTableAlphaUnavailable);
        effects_.push_back(Effect::ImageColorTableAlphaRead);
        const auto format = *temporaryData_->format_;
        const bool hasAlpha = *temporaryData_->colorTableAlphaBit_ &&
            (format == 1 || format == 2 || format == 3);
        complete(!hasAlpha);
    }
    return report_;
}

} // namespace m2m::engine
