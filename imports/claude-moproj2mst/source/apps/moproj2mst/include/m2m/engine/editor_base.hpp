#pragma once
#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "m2m/engine/view_manager.hpp"
#include "m2m/engine/brush_texture_image.hpp"
#include "m2m/engine/paint_device_cleanup.hpp"
#include "m2m/engine/paint_device_active_copy.hpp"

namespace m2m::engine::simcontrol300 {

class StagedEditor300;

struct EditorBaseContext {
    const StagedEditor300* receiver = nullptr;
    std::string qualification;
    std::shared_ptr<WidgetThreadState> currentThread;
};

enum class EditorBaseStage {
    NotStarted, PrivatePalette, PrivateFont, PrivateLocale, PrivateApplication,
    ThreadSelection, QObjectCounter, InitApplication, InitRegistry,
    InitData, Direction, DirectionPropagation, PaletteGroup, BrushOpacity, BrushStyleDispatch,
    RadialFocalRadius, RadialGeometry, RadialNonfinite,
    GradientStops, GradientScan, GradientValueDestruction, TextureImage,
    TextureImageActiveSource, TextureImageActiveCopy, TextureImageActiveFinish,
    TextureImagePaintDevicePrefix, TextureImageCleanup, OpacityDecision,
    OpacityEffect, OpacityWindowFlags, OpacityPlatform, SharedTail, QWidgetReturned
};
enum class EditorBaseIssue {
    None, IdentityMismatch, MissingQualification, WidgetOwnerUnavailable,
    RuntimeOwnerUnavailable, RuntimeOwnerChanged, WrongStage,
    PrivatePaletteUnavailable, PrivateFontUnavailable, PrivateLocaleUnavailable,
    ApplicationPresenceUnavailable, ApplicationMissing,
    CurrentThreadUnavailable, CurrentThreadMismatch, CurrentThreadChanged,
    ApplicationTypeUnavailable, NonGuiApplication, RegistryPresenceUnavailable,
    DirectionUnavailable, DirectionChangeUnrecovered,
    ModalStateUnavailable, ModalPaletteUnrecovered, ActivePaletteUnrecovered,
    PaletteBrushUnrecovered, PaletteBrushAlphaUnavailable, PaletteBrushStyleUnrecovered,
    GradientSourceUnavailable, GradientStopsPending, GradientReturnedValueUnrecovered,
    GradientSymbolOwnerChanged,
    RadialFocalRadiusUnavailable, RadialGeometryUnrecovered, RadialFloatingPointUnrecovered,
    TextureImageSourceUnavailable, TextureImagePending,
    TextureImageCleanupGlobalsUnavailable, TextureImageCleanupPending, TextureImageCleanupOwnerChanged,
    TextureImageActiveSourceUnavailable, TextureImageActiveCopyPending,
    TextureImageActiveValueUnavailable, TextureImageActiveFinishUnrecovered,
    TextureImageActiveFinishPending,
    NativeOpacityUnrecovered, ConstructorTailPending, ApplicationWrappersUnrecovered
};
enum class EditorBaseEffect {
    PrivateAllocation, QObjectPrivateConstructed, WidgetPrivatePrefix,
    PaletteConstructed, FontConstructed, LocaleConstructed,
    WidgetPrivateTail, ExtendedAttributesInitialized, QObjectThreadRetained,
    QObjectCountIncremented, PaintDeviceConstructed, QWidgetClassInstalled,
    RegistryStagePassed, WidgetDataAssigned, InitialAttributesAssigned,
    ParentlessGeometryAssigned, SelfFocusAssigned, WindowFlagsAdjusted,
    DirectionAttributeAssigned, DirectionResolved, PaletteGroupAssigned,
    BrushAlphaConsumed, RadialFocalRadiusRead, RadialExtendedResolved,
    GradientStopsReturned, GradientStopAlphaRead,
    GradientReturnedValueDestroyed, TextureImageDataBoundaryReached,
    TextureImagePaintingWordRead, TextureImagePaintDeviceClassInstalled,
    TextureImageCleanupReturned, TextureImageOpacityResolved,
    TextureImageActiveSourceBound, TextureImageActiveCopyReturned, TextureImageActiveFinishReturned,
    PaletteOpacityResolved, OpacityAssigned, NativeOpacityRaw120Read,
    NativeOpacityFlag11Read, NativeOpacityHelperReturned
};
struct EditorBaseReport {
    EditorBaseStage stage = EditorBaseStage::NotStarted;
    EditorBaseIssue issue = EditorBaseIssue::None;
    bool attempted = false;
    bool qwidgetReturned = false;
    // Application wrappers and their model/operation signal connections follow
    // QWidget. Even a returned QWidget would not authorize native setup/Save.
    static constexpr bool applicationWrappersReturned = false;
    static constexpr bool completeForPublicSave = false;
};

// One original constructor call frame on the factory's actual WidgetState.
// It holds no second set of mutable widget, application, font or thread values.
class EditorBaseConstruction final {
public:
    ~EditorBaseConstruction();
    EditorBaseConstruction(const EditorBaseConstruction&) = delete;
    EditorBaseConstruction& operator=(const EditorBaseConstruction&) = delete;
    const EditorBaseReport& report() const noexcept { return report_; }
    const std::vector<EditorBaseEffect>& effects() const noexcept { return effects_; }
    const std::string& qualification() const noexcept { return qualification_; }
    const WidgetConstructorTail* constructorTail() const noexcept { return tail_.get(); }
    const BrushGradientStopsCall* gradientStopsCall() const noexcept { return gradientStops_.get(); }
    const BrushTextureImageOpacityCall* textureImageCall() const noexcept { return textureImage_.get(); }
    const PaintDeviceCleanupCall* textureImageCleanupCall() const noexcept { return textureImageCleanup_.get(); }
    const PaintDeviceActiveCopyCall* textureImageActiveCopyCall() const noexcept { return textureImageActiveCopy_.get(); }
    const PaintDeviceActiveCopyValue* textureImageActiveReturnedValue() const noexcept { return textureImageActiveValue_.get(); }
    const PaintDeviceNullImageFinishCall* textureImageActiveFinishCall() const noexcept { return textureImageActiveFinish_.get(); }
    std::optional<std::uint16_t> textureImageReturnedPaintingWord() const noexcept { return textureImagePaintingWord_; }
    std::optional<bool> nativeOpacityRaw120Read() const noexcept { return nativeOpacityRaw120_; }
    std::optional<bool> nativeOpacityFlag11Read() const noexcept { return nativeOpacityFlag11_; }
private:
    friend class StagedEditor300;
    EditorBaseConstruction(const StagedEditor300*, const std::shared_ptr<WidgetState>&);
    EditorBaseReport resume(const EditorBaseContext&);
    const StagedEditor300* factory_;
    std::weak_ptr<WidgetState> widget_;
    std::weak_ptr<WidgetRuntimeState> runtime_;
    std::weak_ptr<WidgetThreadState> currentThread_;
    // Retained isOpaque call-frame state, referencing the actual shared brush.
    // An absent alpha comparison means QWidget bypassed isOpaque.
    std::weak_ptr<const WidgetBrushDataState> opacityBrush_;
    std::optional<bool> opacityAlphaIsFull_;
    std::unique_ptr<BrushGradientStopsCall> gradientStops_;
    std::unique_ptr<BrushTextureImageOpacityCall> textureImage_;
    std::weak_ptr<const BrushTextureImageState> textureImageSource_;
    std::weak_ptr<PaintDeviceActiveCopySourceState> textureImageActiveSource_;
    std::unique_ptr<PaintDeviceActiveCopyCall> textureImageActiveCopy_;
    std::unique_ptr<PaintDeviceActiveCopyValue> textureImageActiveValue_;
    std::unique_ptr<PaintDeviceNullImageFinishCall> textureImageActiveFinish_;
    std::optional<bool> textureImagePredicate_;
    std::optional<std::uint16_t> textureImagePaintingWord_;
    std::unique_ptr<PaintDeviceCleanupCall> textureImageCleanup_;
    std::weak_ptr<PaintDeviceCleanupGlobals> textureImageCleanupGlobals_;
    std::size_t gradientScanIndex_ = 0;
    bool gradientOpaque_ = true;
    bool opacityDesired_ = false;
    std::optional<bool> nativeOpacityRaw120_, nativeOpacityFlag11_;
    std::unique_ptr<WidgetConstructorTail> tail_;
    EditorBaseReport report_;
    std::vector<EditorBaseEffect> effects_;
    std::string qualification_;
};

} // namespace m2m::engine::simcontrol300
