#include "m2m/engine/paint_device_active_copy.hpp"

#include <utility>

namespace m2m::engine {
namespace {
template<class T> bool sameOwner(const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) noexcept
{ return a.get() == b.get() && !a.owner_before(b) && !b.owner_before(a); }
}

PaintDeviceActiveCopySourceState::PaintDeviceActiveCopySourceState(std::string qualification)
    : qualification_(std::move(qualification)) {}
PaintDeviceActiveCopySourceState::~PaintDeviceActiveCopySourceState() { releaseNativeOwner(); }

std::shared_ptr<PaintDeviceActiveCopySourceState> PaintDeviceActiveCopySourceState::fromTextureImageSource(
    std::shared_ptr<const BrushTextureImageState> texture, const std::shared_ptr<PaintDeviceCleanupGlobals>& globals,
    std::string qualification)
{
    if (!texture || qualification.empty() || !globals || !globals->nativeOwnerAlive() || !globals->qualified() ||
        !sameOwner(globals, globals->weak_from_this().lock())) return {};
    auto source = std::make_shared<PaintDeviceActiveCopySourceState>(std::move(qualification));
    source->textureImage_ = std::move(texture); source->globals_ = globals; source->qualified_ = true;
    return source;
}

bool PaintDeviceActiveCopySourceState::nativeOwnerAlive() const noexcept
{
    if (!alive_) return false;
    if (!textureImage_) return true;
    const auto globals = globals_.lock();
    return globals && globals->nativeOwnerAlive() && globals->qualified() &&
        sameOwner(globals, globals->weak_from_this().lock());
}

bool PaintDeviceActiveCopySourceState::observe(const PaintDeviceActiveCopySourceObservation& o) noexcept
{
    if (textureImage_) return false; // No duplicate observations of the same native image.
    const auto self = weak_from_this().lock(); const auto receiver = o.receiver.lock();
    if (!alive_ || !self || !receiver || !sameOwner(self, receiver) ||
        qualification_.empty() || o.qualification != qualification_) return false;
    if (paintingWord_ && o.paintingWord && *paintingWord_ != *o.paintingWord) return false;
    if (o.imageData && *o.imageData && !(*o.imageData)->observationAlias()) return false;
    if (imageData_ && o.imageData && !sameOwner(*imageData_, *o.imageData)) return false;
    if (!paintingWord_ && o.paintingWord) paintingWord_ = o.paintingWord;
    if (!imageData_ && o.imageData) imageData_ = o.imageData;
    qualified_ = true;
    return true;
}

PaintDeviceActiveCopyValue::PaintDeviceActiveCopyValue(std::shared_ptr<PaintDevicePrefixState> prefix)
    : prefix_(std::move(prefix)) {}
PaintDeviceActiveCopyValue::~PaintDeviceActiveCopyValue() { prefix_->releaseNativeOwner(); }

PaintDeviceActiveCopyCall::PaintDeviceActiveCopyCall(
    const std::shared_ptr<PaintDeviceActiveCopySourceState>& source, std::string qualification)
    : source_(source), qualification_(std::move(qualification))
{
    if (source && source->textureImageBound()) {
        globals_ = source->cleanupGlobalsOwner(); globalsBound_ = true;
    }
}

bool PaintDeviceActiveCopyCall::bindCleanupGlobals(const std::shared_ptr<PaintDeviceCleanupGlobals>& globals) noexcept
{
    if (!globals || !globals->nativeOwnerAlive() || !globals->qualified() ||
        !sameOwner(globals, globals->weak_from_this().lock())) return false;
    const auto source = source_.lock();
    if (source && source->textureImageBound() && (!source->nativeOwnerAlive() ||
        !sameOwner(globals, source->cleanupGlobalsOwner().lock()))) return false;
    if (globalsBound_) return sameOwner(globals, globals_.lock());
    globals_ = globals; globalsBound_ = true;
    return true;
}

std::unique_ptr<PaintDeviceActiveCopyValue> PaintDeviceActiveCopyCall::takeReturnedImage() noexcept
{
    return report_.stage == PaintDeviceActiveCopyStage::Returned ? std::move(outer_) : nullptr;
}

PaintDeviceActiveCopyReport PaintDeviceActiveCopyCall::resume()
{
    using Stage = PaintDeviceActiveCopyStage;
    using Issue = PaintDeviceActiveCopyIssue;
    using Effect = PaintDeviceActiveCopyEffect;
    const auto pending = [&](Issue issue) { report_.issue = issue; return report_; };
    if (report_.stage == Stage::Returned) return report_; // Historical receipt only.
    const auto source = source_.lock();
    if (!source || !source->nativeOwnerAlive() || !sameOwner(source, source->weak_from_this().lock()))
        return pending(Issue::SourceUnavailable);
    if (!source->qualified()) return pending(Issue::SourceQualificationUnavailable);
    if (qualification_.empty()) return pending(Issue::CallQualificationUnavailable);
    if ((outer_ && !outer_->prefixState()->nativeOwnerAlive()) ||
        (inner_ && !inner_->prefixState()->nativeOwnerAlive())) return pending(Issue::TemporaryOwnerUnavailable);
    if (report_.stage == Stage::SourceQualification) report_.stage = Stage::OuterConstruction;
    if (report_.stage == Stage::OuterConstruction) {
        auto state = PaintDevicePrefixState::fromReturnedImageConstruction(qualification_ + "/outer");
        outerPrefix_ = state;
        outer_.reset(new PaintDeviceActiveCopyValue(std::move(state)));
        effects_.push_back(Effect::OuterImageConstructed); //650b9a93..aa0, data+10 not set yet.
        report_.stage = Stage::SourcePaintingWord;
    }
    if (report_.stage == Stage::SourcePaintingWord) {
        if (!source->paintingWord()) return pending(Issue::SourcePaintingWordUnavailable);
        report_.readSourcePaintingWord = *source->paintingWord();
        effects_.push_back(Effect::SourcePaintingWordRead); //650b9aa3, even for null source data.
        if (*report_.readSourcePaintingWord == 0) {
            report_.stage = Stage::InactiveSourceContinuation;
            return pending(Issue::InactiveSourceUnrecovered);
        }
        report_.stage = Stage::OuterNullInitialization;
    }
    if (report_.stage == Stage::InactiveSourceContinuation) return pending(Issue::InactiveSourceUnrecovered);
    if (report_.stage == Stage::OuterNullInitialization) {
        outer_->imageData_ = std::shared_ptr<BrushTextureImageDataState>{};
        effects_.push_back(Effect::OuterNullInitialized); //650b9aaa
        report_.stage = Stage::RectangleConstruction;
    }
    if (report_.stage == Stage::RectangleConstruction) {
        std::array<std::int32_t, 4> r;
        r[3] = -1; r[2] = -1; r[1] = 0; r[0] = 0; //6700b232..b243
        rectangle_ = r;
        effects_.push_back(Effect::DefaultRectangleConstructed);
        report_.stage = Stage::CopySourceData;
    }
    if (report_.stage == Stage::CopySourceData) {
        if (!source->imageData()) return pending(Issue::SourceImageDataUnavailable);
        effects_.push_back(Effect::CopySourceDataRead); //650b5df7, before rectangle/pixel reads.
        if (*source->imageData()) {
            report_.stage = Stage::NonNullSourceContinuation;
            return pending(Issue::NonNullActiveCopyUnrecovered);
        }
        report_.stage = Stage::InnerConstruction;
    }
    if (report_.stage == Stage::NonNullSourceContinuation) return pending(Issue::NonNullActiveCopyUnrecovered);
    if (report_.stage == Stage::InnerConstruction) {
        inner_.reset(new PaintDeviceActiveCopyValue(
            PaintDevicePrefixState::fromReturnedImageConstruction(qualification_ + "/inner")));
        inner_->imageData_ = std::shared_ptr<BrushTextureImageDataState>{}; //650b5e00..e10
        effects_.push_back(Effect::InnerImageConstructed);
        report_.stage = Stage::NullAssignment;
    }
    if (report_.stage == Stage::NullAssignment) {
        // Both values' null pointers are constructor/copy-prefix stores. There
        // is no pointer-equality shortcut:650b8ff3 rereads inner before the store.
        effects_.push_back(Effect::AssignmentInnerWordRead);
        effects_.push_back(Effect::AssignmentInnerDataRead);
        effects_.push_back(Effect::AssignmentOuterDataRead);
        effects_.push_back(Effect::AssignmentInnerDataReread);
        outer_->imageData_ = *inner_->imageData_;
        effects_.push_back(Effect::OuterNullAssigned); //650b8ff7, second outer-null store.
        report_.stage = Stage::InnerImageDestruction;
    }
    if (report_.stage == Stage::InnerImageDestruction) {
        //650b9ada repeats QImage class store, thenadf reads null data. There
        // is no image-data ref/deref/free on this selected path.
        effects_.push_back(Effect::InnerQImageClassInstalled);
        effects_.push_back(Effect::InnerImageDataRead);
        innerPrefix_ = std::make_unique<PaintDevicePrefixCall>(inner_->prefixState());
        report_.stage = Stage::InnerPrefix;
    }
    if (report_.stage == Stage::InnerPrefix) {
        if (innerPrefix_->resume().stage != PaintDevicePrefixStage::TailReady)
            return pending(Issue::InnerPrefixPending);
        effects_.push_back(Effect::InnerPrefixTailReached);
        report_.stage = Stage::InnerCleanup;
    }
    if (report_.stage == Stage::InnerCleanup) {
        const auto globals = globals_.lock();
        if (!globals || !globals->nativeOwnerAlive()) return pending(Issue::CleanupGlobalsUnavailable);
        if (!innerCleanup_) innerCleanup_ = std::make_unique<PaintDeviceCleanupCall>(globals);
        if (innerCleanup_->resume().stage != PaintDeviceCleanupStage::Returned)
            return pending(Issue::InnerCleanupPending);
        effects_.push_back(Effect::InnerCleanupReturned);
        inner_->prefixState()->releaseNativeOwner();
        effects_.push_back(Effect::InnerImageDestroyed);
        // Both destructors use the SAME QtGui module slot (RVA9cb858). Carry
        // that bound origin with I1; this is host provenance, not a Qt store/ref.
        outer_->cleanupGlobals_ = globals;
        effects_.push_back(Effect::ActiveCopyReturned); //650b9b19, outer still live and null.
        report_.stage = Stage::Returned; report_.issue = Issue::None;
    }
    return report_;
}

PaintDeviceNullImageFinishCall::PaintDeviceNullImageFinishCall(
    std::unique_ptr<PaintDeviceActiveCopyValue> value)
    : value_(std::move(value)), globals_(value_ ? value_->cleanupGlobalsOwner() : std::weak_ptr<PaintDeviceCleanupGlobals>{}) {}

PaintDeviceNullImageFinishReport PaintDeviceNullImageFinishCall::resume()
{
    using Stage = PaintDeviceNullImageFinishStage;
    using Issue = PaintDeviceNullImageFinishIssue;
    using Effect = PaintDeviceNullImageFinishEffect;
    const auto pending = [&](Issue issue) { report_.issue = issue; return report_; };
    if (report_.stage == Stage::Returned) return report_; // Historical receipt only.
    const auto state = value_ ? value_->prefixState() : nullptr;
    if (!state || !state->nativeOwnerAlive()) return pending(Issue::ValueUnavailable);
    const auto globals = globals_.lock();
    if (!globals || !globals->nativeOwnerAlive() || !globals->qualified() ||
        !sameOwner(globals, globals->weak_from_this().lock())) return pending(Issue::ModuleOriginUnavailable);
    if (report_.stage == Stage::ValueQualification) {
        // Origin/lifetime checks, not extra native field reads. A claimed base
        // prefix is not a fresh returned QImage and cannot certify this call.
        if (state->classIdentity() != PaintDevicePrefixClass::QImage)
            return pending(Issue::ValueClassUnrecovered);
        if (!value_->imageData()) return pending(Issue::ImageDataUnavailable);
        if (*value_->imageData()) return pending(Issue::NonNullImageUnrecovered);
        report_.stage = Stage::AlphaPredicate;
    }
    if (report_.stage == Stage::AlphaPredicate) {
        effects_.push_back(Effect::PredicateImageDataRead); //650aa340; null skips format/flag/pixels.
        effects_.push_back(Effect::AlphaPredicateReturnedFalse); //650aa382..384
        privateOpaque_ = true; //6511c194 retains !hasAlpha in DIL, not returned AL yet.
        effects_.push_back(Effect::OpacityNegated);
        report_.stage = Stage::QImageDestruction;
    }
    if (report_.stage == Stage::QImageDestruction) {
        //650b3322 stores QImage class even though already equal;3325 separately
        //reads the same null data. No image-data release/free event is reached.
        effects_.push_back(Effect::QImageClassInstalled);
        effects_.push_back(Effect::QImageDataRead);
        prefix_ = std::make_unique<PaintDevicePrefixCall>(state);
        report_.stage = Stage::PaintDevicePrefix;
    }
    if (report_.stage == Stage::PaintDevicePrefix) {
        if (prefix_->resume().stage != PaintDevicePrefixStage::TailReady)
            return pending(Issue::PrefixPending);
        effects_.push_back(Effect::PaintDevicePrefixTailReached);
        report_.stage = Stage::Cleanup;
    }
    if (report_.stage == Stage::Cleanup) {
        if (!cleanup_) cleanup_ = std::make_unique<PaintDeviceCleanupCall>(globals);
        if (cleanup_->resume().stage != PaintDeviceCleanupStage::Returned)
            return pending(Issue::CleanupPending);
        effects_.push_back(Effect::CleanupReturned);
        state->releaseNativeOwner();
        effects_.push_back(Effect::OuterImageDestroyed);
        report_.opaque = privateOpaque_; //6511c1db, only after the outer destructor returns.
        effects_.push_back(Effect::OpacityReturned);
        report_.stage = Stage::Returned; report_.issue = Issue::None;
    }
    return report_;
}

} // namespace m2m::engine
