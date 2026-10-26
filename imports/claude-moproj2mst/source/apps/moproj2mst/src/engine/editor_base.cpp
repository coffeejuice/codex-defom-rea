#include "m2m/engine/editor_base.hpp"
#include "m2m/engine/widget_events.hpp"
#include "m2m/common.hpp"
#include <bit>
#include <limits>
#include <utility>

namespace m2m::engine::simcontrol300 {
namespace {
template<class T> bool sameOwner(const std::shared_ptr<T>& left,
    const std::shared_ptr<T>& right) noexcept
{
    return left && right && left.get() == right.get() &&
        !left.owner_before(right) && !right.owner_before(left);
}
}

EditorBaseConstruction::EditorBaseConstruction(const StagedEditor300* factory,
    const std::shared_ptr<WidgetState>& widget) : factory_(factory), widget_(widget) {}
EditorBaseConstruction::~EditorBaseConstruction() = default;

EditorBaseReport EditorBaseConstruction::resume(const EditorBaseContext& context)
{
    if (context.receiver != factory_)
        return {report_.stage, EditorBaseIssue::IdentityMismatch};
    if (context.qualification.empty())
        return {report_.stage, EditorBaseIssue::MissingQualification};
    auto widget = widget_.lock();
    if (!widget || !widget->nativeOwnerAlive() || widget->nativeIdentity() != factory_)
        return {report_.stage, EditorBaseIssue::WidgetOwnerUnavailable};
    auto runtime = widget->runtimeOwner();
    if (!runtime) return {report_.stage, EditorBaseIssue::RuntimeOwnerUnavailable};
    const auto pending = [&](EditorBaseIssue issue) {
        report_.issue = issue;
        return report_;
    };
    if (report_.attempted && !sameOwner(runtime, runtime_.lock()))
        return pending(EditorBaseIssue::RuntimeOwnerChanged);
    if (auto thread = currentThread_.lock(); thread && !sameOwner(thread, context.currentThread))
        return pending(EditorBaseIssue::CurrentThreadChanged);
    if (!report_.attempted) {
        qualification_ = context.qualification;
        runtime_ = runtime;
        report_.attempted = true;
        effects_.push_back(EditorBaseEffect::PrivateAllocation);
        // QObjectPrivate67131160: the native thread field is null until the
        // later QObject ctor. Runtime affinity is already bound separately.
        widget->thread_.reset();
        widget->parent_.reset();
        widget->parentOwnsChild_ = false;
        widget->objectName_ = qt::String();
        widget->childSuffix_.clear();
        widget->childPrefixUnrecovered_ = false;
        widget->pendingChildren_.emplace();
        widget->events_.objectFilterCount = 0;
        widget->sendChildEvents_ = true;
        widget->receivesChildEvents_ = true;
        widget->observeEmptyDynamicBooleanProperties(false);
        effects_.push_back(EditorBaseEffect::QObjectPrivateConstructed);
        // QWidgetPrivate+88..e8 are cleared before the first palette/font
        // constructors. Later extended-attribute stores must not move here.
        widget->graphicsEffectPresent_ = false;
        widget->fieldD0Present_ = false;
        widget->installedLayout_.reset();
        widget->layoutAliasAssigned_ = false;
        widget->events_.layoutPresent = false;
        widget->autoFillBackground_ = false;
        widget->stylePresent_ = false;
        widget->stylesheetLength_ = 0;
        widget->proxyPresent_ = false;
        effects_.push_back(EditorBaseEffect::WidgetPrivatePrefix);
        report_.stage = EditorBaseStage::PrivatePalette;
    }
    const auto* inputs = runtime->privateConstructionInputs(*widget);
    if (report_.stage == EditorBaseStage::PrivatePalette) {
        if (!inputs || !inputs->defaultPalette)
            return pending(EditorBaseIssue::PrivatePaletteUnavailable);
        widget->palette_ = WidgetPaletteValue{inputs->defaultPalette->data, 0};
        effects_.push_back(EditorBaseEffect::PaletteConstructed);
        report_.stage = EditorBaseStage::PrivateFont;
    }
    if (report_.stage == EditorBaseStage::PrivateFont) {
        if (!inputs || !inputs->defaultFont)
            return pending(EditorBaseIssue::PrivateFontUnavailable);
        widget->font_ = WidgetFontValue{inputs->defaultFont->data, 0};
        effects_.push_back(EditorBaseEffect::FontConstructed);
        report_.stage = EditorBaseStage::PrivateLocale;
    }
    if (report_.stage == EditorBaseStage::PrivateLocale) {
        if (!inputs || !inputs->defaultLocale)
            return pending(EditorBaseIssue::PrivateLocaleUnavailable);
        widget->locale_ = inputs->defaultLocale;
        effects_.push_back(EditorBaseEffect::LocaleConstructed);
        // Source known bits at6505e94f..96c, not a zero-filled private block.
        widget->opaqueChildrenDirty_ = true;
        widget->opaque_ = false;
        effects_.push_back(EditorBaseEffect::WidgetPrivateTail);
        report_.stage = EditorBaseStage::PrivateApplication;
    }
    if (report_.stage == EditorBaseStage::PrivateApplication) {
        const auto present = runtime->applicationEvents().present;
        if (!present) return pending(EditorBaseIssue::ApplicationPresenceUnavailable);
        if (!*present) return pending(EditorBaseIssue::ApplicationMissing);
        // Native caller passes exactly40804. Source isWidget and extended
        // attribute stores follow its app/version checks at6505e9a5..bd.
        widget->extendedAttributes_ = std::array<std::uint32_t, 3>{{0, 0, 0}};
        effects_.push_back(EditorBaseEffect::ExtendedAttributesInitialized);
        report_.stage = EditorBaseStage::ThreadSelection;
    }
    if (report_.stage == EditorBaseStage::ThreadSelection) {
        if (!context.currentThread) return pending(EditorBaseIssue::CurrentThreadUnavailable);
        if (!sameOwner(context.currentThread, runtime->threadState()))
            return pending(EditorBaseIssue::CurrentThreadMismatch);
        widget->thread_ = context.currentThread;
        currentThread_ = context.currentThread;
        effects_.push_back(EditorBaseEffect::QObjectThreadRetained);
        report_.stage = EditorBaseStage::QObjectCounter;
    }
    if (report_.stage == EditorBaseStage::QObjectCounter) {
        runtime->commitQObjectConstruction(*widget);
        effects_.push_back(EditorBaseEffect::QObjectCountIncremented);
        // QPaintDevice and QWidget native class install occur after QObject
        // returns, before privateinit; no extra receiver object is allocated.
        effects_.push_back(EditorBaseEffect::PaintDeviceConstructed);
        widget->paintDeviceKind_ = WidgetPaintDeviceKind::QWidget;
        widget->eventReceiverKind_ = WidgetEventReceiverKind::QWidget;
        effects_.push_back(EditorBaseEffect::QWidgetClassInstalled);
        report_.stage = EditorBaseStage::InitApplication;
    }
    if (report_.stage == EditorBaseStage::InitApplication) {
        const auto type = runtime->applicationType();
        if (!type) return pending(EditorBaseIssue::ApplicationTypeUnavailable);
        if (*type == 0) return pending(EditorBaseIssue::NonGuiApplication);
        report_.stage = EditorBaseStage::InitRegistry;
    }
    if (report_.stage == EditorBaseStage::InitRegistry) {
        const auto registry = runtime->allWidgetsRegistryPresent();
        if (!registry) return pending(EditorBaseIssue::RegistryPresenceUnavailable);
        if (*registry) runtime->registerWidget(widget);
        effects_.push_back(EditorBaseEffect::RegistryStagePassed);
        report_.stage = EditorBaseStage::InitData;
    }
    if (report_.stage == EditorBaseStage::InitData) {
        widget->widgetDataAssigned_ = true;
        widget->windowFlags_ = 0;
        widget->attributesLow_ = 0; // explicit6506e56c store
        effects_.push_back(EditorBaseEffect::WidgetDataAssigned);
        // Fixed flags0 does not take raw100 path. raw76 stays set for initial
        // type0; source setters below have no reached callback for these bits.
        widget->assignAttributeBit(76, true);
        widget->assignAttributeBit(16, true);
        effects_.push_back(EditorBaseEffect::InitialAttributesAssigned);
        widget->geometry_ = std::array<int, 4>{{0, 0, 639, 479}};
        effects_.push_back(EditorBaseEffect::ParentlessGeometryAssigned);
        widget->focusNext_ = widget->focusPrevious_ = factory_;
        widget->focusNextKnown_ = widget->focusPreviousKnown_ = true;
        effects_.push_back(EditorBaseEffect::SelfFocusAssigned);
        widget->windowFlags_ = 0x0800f001u;
        effects_.push_back(EditorBaseEffect::WindowFlagsAdjusted);
        report_.stage = EditorBaseStage::Direction;
    }
    if (report_.stage == EditorBaseStage::Direction) {
        const auto direction = runtime->applicationLayoutDirection();
        if (!direction) return pending(EditorBaseIssue::DirectionUnavailable);
        const bool desired = *direction == 1;
        const auto current = widget->attributeBit(56);
        if (!current) return pending(EditorBaseIssue::DirectionUnavailable);
        if (desired != *current) {
            //6506a55d writes raw56 before child propagation and raw5a. Keep
            // the reached store. Attribute56 maps directly to setter return;
            // the outer helper's following event/children tail stays pending.
            widget->assignAttributeBit(56, desired);
            effects_.push_back(EditorBaseEffect::DirectionAttributeAssigned);
            report_.stage = EditorBaseStage::DirectionPropagation;
            return pending(EditorBaseIssue::DirectionChangeUnrecovered);
        }
        effects_.push_back(EditorBaseEffect::DirectionResolved);
        report_.stage = EditorBaseStage::PaletteGroup;
    }
    if (report_.stage == EditorBaseStage::DirectionPropagation)
        return pending(EditorBaseIssue::DirectionChangeUnrecovered);
    if (report_.stage == EditorBaseStage::PaletteGroup) {
        auto& group = widget->palette_->groupResolveWord;
        if (widget->attributeBit(0) == true) {
            group = (group & 0xfffffff1u) | 1u;
        } else {
            if (widget->attributeBit(15) == true)
                return pending(EditorBaseIssue::ActivePaletteUnrecovered);
            const auto modal = runtime->applicationModalState();
            if (!modal) return pending(EditorBaseIssue::ModalStateUnavailable);
            if (*modal) return pending(EditorBaseIssue::ModalPaletteUnrecovered);
            group &= 0xfffffff0u;
        }
        effects_.push_back(EditorBaseEffect::PaletteGroupAssigned);
        report_.stage = EditorBaseStage::BrushOpacity;
    }
    if (report_.stage == EditorBaseStage::BrushOpacity) {
        // 6506e6f2..721: QPalette::brush(Current, Window) selects the live
        // palette's current group. Its QBrush value shares the runtime's data;
        // no observed success boolean or duplicate editor palette is consumed.
        const auto group = widget->palette_->groupResolveWord & 0x0fu;
        const auto brush = widget->palette_->data->brushAt(group, 10);
        if (!brush || !brush->style()) return pending(EditorBaseIssue::PaletteBrushUnrecovered);
        // Style0 and nonwindows bypass isOpaque entirely. Every entered
        // isOpaque call reads QColor::alpha at 6511c0ad..c0be BEFORE dispatching
        // style at c0c4, including still-unrecovered gradient/texture paths.
        if ((*widget->windowFlags_ & 1u) && *brush->style() != 0) {
            const auto alpha = brush->alphaHighByte();
            if (!alpha) return pending(EditorBaseIssue::PaletteBrushAlphaUnavailable);
            opacityBrush_ = brush;
            opacityAlphaIsFull_ = *alpha == 255u;
            effects_.push_back(EditorBaseEffect::BrushAlphaConsumed);
        }
        report_.stage = EditorBaseStage::BrushStyleDispatch;
    }
    if (report_.stage == EditorBaseStage::BrushStyleDispatch) {
        if (opacityAlphaIsFull_.has_value()) {
            const auto brush = opacityBrush_.lock();
            if (!brush || !brush->style()) return pending(EditorBaseIssue::PaletteBrushUnrecovered);
            if (*brush->style() == 1) {
                opacityDesired_ = *opacityAlphaIsFull_;
            } else if (*brush->style() == 15 || *brush->style() == 17) {
                // isExtendedRadialGradient returns false for these two styles;
                //6511c201 then calls stops on this same brush's embedded value.
                report_.stage = EditorBaseStage::GradientStops;
            } else if (*brush->style() == 16) {
                report_.stage = EditorBaseStage::RadialFocalRadius;
            } else if (*brush->style() == 24) {
                report_.stage = EditorBaseStage::TextureImage;
            }
            // All remaining raw styles: 6511aba0 returns false at acd7 for
            // style!=16. The equality checks at c0e4..c106 then reach the
            // constant false return at c1e3 without reading any payload.
        }
        if (report_.stage == EditorBaseStage::BrushStyleDispatch)
            report_.stage = EditorBaseStage::OpacityDecision;
    }
    if (report_.stage == EditorBaseStage::RadialFocalRadius) {
        const auto brush = opacityBrush_.lock();
        if (!brush) return pending(EditorBaseIssue::PaletteBrushUnrecovered);
        const auto word = brush->radialFocalRadiusWord();
        if (!word) return pending(EditorBaseIssue::RadialFocalRadiusUnavailable);
        effects_.push_back(EditorBaseEffect::RadialFocalRadiusRead);
        const auto masked = *word & 0xfffffff8u; //6511abd0..abd3, also focalRadius6511a370.
        if ((masked & 0x7f800000u) == 0x7f800000u) {
            // Original COMISD unordered/exception behavior is not qualified.
            // Classify bits before doing any host floating-point operation.
            report_.stage = EditorBaseStage::RadialNonfinite;
            return pending(EditorBaseIssue::RadialFloatingPointUnrecovered);
        }
        static_assert(sizeof(float) == sizeof(std::uint32_t) && std::numeric_limits<float>::is_iec559);
        const auto magnitude = static_cast<double>(std::bit_cast<float>(masked & 0x7fffffffu));
        if (magnitude <= 1e-12) {
            //6511ac07 takes geometry continuation, not the false return.
            report_.stage = EditorBaseStage::RadialGeometry;
            return pending(EditorBaseIssue::RadialGeometryUnrecovered);
        }
        //6511ac09 true ->isOpaque6511c0da false. No coordinate, radius or
        //gradient-stop value is consumed on this finite early-return branch.
        effects_.push_back(EditorBaseEffect::RadialExtendedResolved);
        opacityDesired_ = false;
        report_.stage = EditorBaseStage::OpacityDecision;
    }
    if (report_.stage == EditorBaseStage::RadialGeometry)
        return pending(EditorBaseIssue::RadialGeometryUnrecovered);
    if (report_.stage == EditorBaseStage::RadialNonfinite)
        return pending(EditorBaseIssue::RadialFloatingPointUnrecovered);
    if (report_.stage == EditorBaseStage::GradientStops) {
        if (!gradientStops_) {
            const auto brush = opacityBrush_.lock();
            if (!brush || !brush->gradientStopsSource())
                return pending(EditorBaseIssue::GradientSourceUnavailable);
            gradientStops_ = std::make_unique<BrushGradientStopsCall>(
                brush->gradientStopsSource(), runtime->gradientSharedNull(*widget));
        }
        if (auto symbol = runtime->gradientSharedNull(*widget);
            symbol && !gradientStops_->bindSharedNull(symbol))
            return pending(EditorBaseIssue::GradientSymbolOwnerChanged);
        const auto stops = gradientStops_->resume();
        if (stops.stage != BrushGradientStopsStage::Returned)
            return pending(EditorBaseIssue::GradientStopsPending);
        effects_.push_back(EditorBaseEffect::GradientStopsReturned);
        report_.stage = EditorBaseStage::GradientScan;
    }
    if (report_.stage == EditorBaseStage::GradientScan) {
        const auto data = gradientStops_->returnedData();
        if (!data || !data->nativeDataAlive() || !data->count() || *data->count() < 0 ||
            !data->payload() || static_cast<std::size_t>(*data->count()) != data->payload()->size())
            return pending(EditorBaseIssue::GradientReturnedValueUnrecovered);
        //6511c21b..c246 scans the actual returned value in order. The consumed
        //brush alpha does not decide gradient opacity. No RGB/position read.
        while (gradientScanIndex_ < data->payload()->size()) {
            const auto alpha = data->payload()->at(gradientScanIndex_).alphaHighByte();
            effects_.push_back(EditorBaseEffect::GradientStopAlphaRead);
            if (alpha != 255u) { gradientOpaque_ = false; break; }
            ++gradientScanIndex_;
        }
        report_.stage = EditorBaseStage::GradientValueDestruction;
    }
    if (report_.stage == EditorBaseStage::GradientValueDestruction) {
        //6511c24a..c295 releases the getter's returned local vector BEFORE
        //6506e72b compares/assigns the widget opaque bit. Keep this cursor past
        //destruction so a later native-opacity guard cannot reconstruct it.
        if (!gradientStops_->destroyReturnedValue())
            return pending(EditorBaseIssue::GradientReturnedValueUnrecovered);
        effects_.push_back(EditorBaseEffect::GradientReturnedValueDestroyed);
        opacityDesired_ = gradientOpaque_;
        report_.stage = EditorBaseStage::OpacityDecision;
    }
    if (report_.stage == EditorBaseStage::TextureImage) {
        if (!textureImage_) {
            const auto brush = opacityBrush_.lock();
            if (!brush || !brush->textureImageSource())
                return pending(EditorBaseIssue::TextureImageSourceUnavailable);
            textureImageSource_ = brush->textureImageSource();
            textureImage_ = std::make_unique<BrushTextureImageOpacityCall>(brush->textureImageSource());
        }
        const auto image = textureImage_->resume();
        if (image.stage == BrushTextureImageStage::ImageCopy &&
            image.issue == BrushTextureImageIssue::ActiveImageCopyUnrecovered) {
            // Selection/getter/cache dependencies have reached the actual
            // active-copy edge. That helper has retained/copied no image data.
            report_.stage = EditorBaseStage::TextureImageActiveSource;
        } else {
            if (image.stage != BrushTextureImageStage::Returned || !image.opaque.has_value())
                return pending(EditorBaseIssue::TextureImagePending);
            // Existing ordinary helper Returned is only its predicate/data
            // boundary, not a completed native PaintDevice destructor.
            textureImagePredicate_ = *image.opaque;
            effects_.push_back(EditorBaseEffect::TextureImageDataBoundaryReached);
            report_.stage = EditorBaseStage::TextureImagePaintDevicePrefix;
        }
    }
    if (report_.stage == EditorBaseStage::TextureImageActiveSource) {
        const auto brush = opacityBrush_.lock();
        if (!brush || brush->style() != 24 ||
            !sameOwner(brush->textureImageSource(), textureImageSource_.lock()))
            return pending(EditorBaseIssue::TextureImageActiveSourceUnavailable);
        std::shared_ptr<PaintDeviceActiveCopySourceState> source;
        try { source = runtime->bindTextureImageCopySource(widget, brush->observationAlias()); }
        catch (const Error& e) {
            if (e.code != M2M_E_UNSUPPORTED) throw;
            return pending(EditorBaseIssue::TextureImageActiveSourceUnavailable);
        }
        const auto globals = runtime->paintDeviceCleanupGlobals(*widget);
        if (!source || !source->nativeOwnerAlive() ||
            !sameOwner(source->textureImageSource(), textureImageSource_.lock()) ||
            !sameOwner(source->cleanupGlobalsOwner().lock(), globals))
            return pending(EditorBaseIssue::TextureImageActiveSourceUnavailable);
        textureImageActiveSource_ = source;
        textureImageActiveCopy_ = std::make_unique<PaintDeviceActiveCopyCall>(source, qualification_ + "/active-texture-copy");
        // Host provenance receipt only, not an extra native field/ref effect.
        effects_.push_back(EditorBaseEffect::TextureImageActiveSourceBound);
        report_.stage = EditorBaseStage::TextureImageActiveCopy;
    }
    if (report_.stage == EditorBaseStage::TextureImageActiveCopy) {
        const auto source = textureImageActiveSource_.lock();
        const auto globals = runtime->paintDeviceCleanupGlobals(*widget);
        if (!source || !source->nativeOwnerAlive() ||
            !sameOwner(source->textureImageSource(), textureImageSource_.lock()) ||
            !sameOwner(source->cleanupGlobalsOwner().lock(), globals))
            return pending(EditorBaseIssue::TextureImageActiveSourceUnavailable);
        if (textureImageActiveCopy_->resume().stage != PaintDeviceActiveCopyStage::Returned)
            return pending(EditorBaseIssue::TextureImageActiveCopyPending);
        textureImageActiveValue_ = textureImageActiveCopy_->takeReturnedImage();
        if (!textureImageActiveValue_) return pending(EditorBaseIssue::TextureImageActiveValueUnavailable);
        effects_.push_back(EditorBaseEffect::TextureImageActiveCopyReturned);
        report_.stage = EditorBaseStage::TextureImageActiveFinish;
    }
    if (report_.stage == EditorBaseStage::TextureImageActiveFinish) {
        const auto globals = runtime->paintDeviceCleanupGlobals(*widget);
        if (!textureImageActiveFinish_) {
            if (!textureImageActiveValue_ || !textureImageActiveValue_->prefixState()->nativeOwnerAlive())
                return pending(EditorBaseIssue::TextureImageActiveValueUnavailable);
            if (!sameOwner(textureImageActiveValue_->cleanupGlobalsOwner().lock(), globals))
                return pending(EditorBaseIssue::TextureImageActiveSourceUnavailable);
            textureImageActiveFinish_ = std::make_unique<PaintDeviceNullImageFinishCall>(std::move(textureImageActiveValue_));
        }
        if (!sameOwner(textureImageActiveFinish_->cleanupGlobalsOwner().lock(), globals))
            return pending(EditorBaseIssue::TextureImageActiveSourceUnavailable);
        // I1 owns its lifetime independently of the cached source now. Keep
        // this same consumer across guards; a moved holder is not a lost value.
        const auto finished = textureImageActiveFinish_->resume();
        if (finished.stage != PaintDeviceNullImageFinishStage::Returned || !finished.opaque.has_value())
            return pending(EditorBaseIssue::TextureImageActiveFinishPending);
        effects_.push_back(EditorBaseEffect::TextureImageActiveFinishReturned);
        opacityDesired_ = *finished.opaque;
        effects_.push_back(EditorBaseEffect::TextureImageOpacityResolved);
        //6511c1db returns only after I1 destruction;6506e72b then compares bit17.
        // Skip the ordinary image summary/projection and inner cleanup receipt.
        report_.stage = EditorBaseStage::OpacityDecision;
    }
    if (report_.stage == EditorBaseStage::TextureImagePaintDevicePrefix) {
        //650b9a93 calls QPaintDevice ctor65138140, which writes the RETURNED
        //image word+8=0 at6513814c. The admitted metadata path never paints it.
        //65138196 compares that word BEFORE installing the base class at81a5.
        //This is not an observation of the cached source image's painting word.
        textureImagePaintingWord_ = 0;
        effects_.push_back(EditorBaseEffect::TextureImagePaintingWordRead);
        effects_.push_back(EditorBaseEffect::TextureImagePaintDeviceClassInstalled);
        report_.stage = EditorBaseStage::TextureImageCleanup;
    }
    if (report_.stage == EditorBaseStage::TextureImageCleanup) {
        const auto globals = runtime->paintDeviceCleanupGlobals(*widget);
        if (!textureImageCleanup_) {
            if (!globals) return pending(EditorBaseIssue::TextureImageCleanupGlobalsUnavailable);
            textureImageCleanupGlobals_ = globals;
            textureImageCleanup_ = std::make_unique<PaintDeviceCleanupCall>(globals);
        } else if (globals && !sameOwner(globals, textureImageCleanupGlobals_.lock())) {
            return pending(EditorBaseIssue::TextureImageCleanupOwnerChanged);
        }
        const auto cleanup = textureImageCleanup_->resume();
        if (cleanup.stage != PaintDeviceCleanupStage::Returned)
            return pending(EditorBaseIssue::TextureImageCleanupPending);
        // Image-data release and the two getter/equality effects belong to
        // retained, separate frames. Neither is replayed by a later raw120 guard.
        effects_.push_back(EditorBaseEffect::TextureImageCleanupReturned);
        opacityDesired_ = *textureImagePredicate_;
        effects_.push_back(EditorBaseEffect::TextureImageOpacityResolved);
        report_.stage = EditorBaseStage::OpacityDecision;
    }
    if (report_.stage == EditorBaseStage::OpacityDecision) {
        effects_.push_back(EditorBaseEffect::PaletteOpacityResolved);
        if (*widget->opaque_ != opacityDesired_) {
            // 6506e72b..74d changes only private+1d8 bit17 BEFORE the helper.
            widget->opaque_ = opacityDesired_;
            effects_.push_back(EditorBaseEffect::OpacityAssigned);
            report_.stage = EditorBaseStage::OpacityEffect;
        } else {
            report_.stage = EditorBaseStage::SharedTail;
        }
    }
    if (report_.stage == EditorBaseStage::OpacityEffect) {
        // winUpdateIsOpaque: same QWidgetPrivate receiver; q_ptr+20 points to
        // private+148, hence data+0c/private+154 is the live windowFlags_ word.
        //65097955 tests window status BEFORE the reached raw120 read.
        if (!widget->windowFlags_) return pending(EditorBaseIssue::NativeOpacityUnrecovered);
        if (*widget->windowFlags_ & 1u) {
            const auto raw120 = widget->attributeBit(120);
            if (!raw120.has_value()) return pending(EditorBaseIssue::NativeOpacityUnrecovered);
            nativeOpacityRaw120_ = *raw120;
            effects_.push_back(EditorBaseEffect::NativeOpacityRaw120Read);
            if (*raw120) report_.stage = EditorBaseStage::OpacityWindowFlags;
        }
        if (report_.stage == EditorBaseStage::OpacityEffect) {
            effects_.push_back(EditorBaseEffect::NativeOpacityHelperReturned);
            report_.stage = EditorBaseStage::SharedTail;
        }
    }
    if (report_.stage == EditorBaseStage::OpacityWindowFlags) {
        if (!widget->windowFlags_) return pending(EditorBaseIssue::NativeOpacityUnrecovered);
        nativeOpacityFlag11_ = (*widget->windowFlags_ & 0x800u) != 0;
        effects_.push_back(EditorBaseEffect::NativeOpacityFlag11Read); //6509796c
        if (*nativeOpacityFlag11_) {
            //65097976 and subsequent native-window style operations are not
            // recovered. Preserve this consumed path; no earlier read replay.
            report_.stage = EditorBaseStage::OpacityPlatform;
            return pending(EditorBaseIssue::NativeOpacityUnrecovered);
        }
        //65097974 ->650979cf: clear bit11 returns even when raw120 is true.
        effects_.push_back(EditorBaseEffect::NativeOpacityHelperReturned);
        report_.stage = EditorBaseStage::SharedTail;
    }
    if (report_.stage == EditorBaseStage::OpacityPlatform)
        return pending(EditorBaseIssue::NativeOpacityUnrecovered);
    if (report_.stage == EditorBaseStage::SharedTail) {
        if (!tail_) tail_ = std::make_unique<WidgetConstructorTail>();
        const auto& tail = tail_->resume(*runtime, widget);
        if (!tail.completed) return pending(EditorBaseIssue::ConstructorTailPending);
        report_.qwidgetReturned = true;
        report_.stage = EditorBaseStage::QWidgetReturned;
    }
    // SfWidget/operation wrappers follow a returned QWidget. Native QPointer
    // assignment and active-step notification remain separate source effects.
    return pending(EditorBaseIssue::ApplicationWrappersUnrecovered);
}

} // namespace m2m::engine::simcontrol300
