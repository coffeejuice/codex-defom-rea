#pragma once
#include <memory>
#include <string>
#include <vector>
#include "m2m/engine/view_manager.hpp"

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
    InitData, Direction, DirectionPropagation, PaletteGroup, BrushOpacity, QWidgetReturned
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
    PaletteBrushUnrecovered
};
enum class EditorBaseEffect {
    PrivateAllocation, QObjectPrivateConstructed, WidgetPrivatePrefix,
    PaletteConstructed, FontConstructed, LocaleConstructed,
    WidgetPrivateTail, ExtendedAttributesInitialized, QObjectThreadRetained,
    QObjectCountIncremented, PaintDeviceConstructed, QWidgetClassInstalled,
    RegistryStagePassed, WidgetDataAssigned, InitialAttributesAssigned,
    ParentlessGeometryAssigned, SelfFocusAssigned, WindowFlagsAdjusted,
    DirectionAttributeAssigned, DirectionResolved, PaletteGroupAssigned
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
    EditorBaseConstruction(const EditorBaseConstruction&) = delete;
    EditorBaseConstruction& operator=(const EditorBaseConstruction&) = delete;
    const EditorBaseReport& report() const noexcept { return report_; }
    const std::vector<EditorBaseEffect>& effects() const noexcept { return effects_; }
    const std::string& qualification() const noexcept { return qualification_; }
private:
    friend class StagedEditor300;
    EditorBaseConstruction(const StagedEditor300*, const std::shared_ptr<WidgetState>&);
    EditorBaseReport resume(const EditorBaseContext&);
    const StagedEditor300* factory_;
    std::weak_ptr<WidgetState> widget_;
    std::weak_ptr<WidgetRuntimeState> runtime_;
    std::weak_ptr<WidgetThreadState> currentThread_;
    EditorBaseReport report_;
    std::vector<EditorBaseEffect> effects_;
    std::string qualification_;
};

} // namespace m2m::engine::simcontrol300
