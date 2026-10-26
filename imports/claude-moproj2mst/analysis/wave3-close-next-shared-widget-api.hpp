// Interface draft only. Not included by production. Root validates/release first.
// These declarations describe additions to the existing engine owners.
// No new application, font-cache, widget, thread or queue owner is introduced.

struct WidgetPrivateConstructionObservation {
    std::string qualification;
    std::vector<WidgetFontDataObservation> fontData;
    std::vector<WidgetPaletteDataObservation> paletteData;
    std::optional<WidgetFontValueObservation> defaultFont;
    std::optional<WidgetPaletteValueObservation> defaultPalette;
    std::optional<WidgetLocaleValue> defaultLocale;
};
struct WidgetPrivateConstructionInputs {
    std::string qualification;
    std::optional<WidgetFontValue> defaultFont;
    std::optional<WidgetPaletteValue> defaultPalette;
    std::optional<WidgetLocaleValue> defaultLocale;
};
// WidgetRuntimeState additions:
// void observePrivateConstructionInputs(const std::shared_ptr<WidgetState>&,
//                                       const WidgetPrivateConstructionObservation&);
// const WidgetPrivateConstructionInputs* privateConstructionInputs(const WidgetState&) const;
// Inputs are evidence tied to this actual live receiver, separate from live
// WidgetState fields. Data interns into the SAME runtime tables. Each source
// cursor commits font/palette/locale only at its real call site, preserving any
// earlier unknown-call guard. No all-at-once default-state initializer.
// Existing known runtime/default data cannot be overwritten as a new profile.
// Startup input does not qualify post-Open editor input; same-value later
// observation needs its own exact receiver/checkpoint qualification.

enum class WidgetPaintDeviceKind { Unknown, QWidget };
struct WidgetMetricInputObservation {
    std::weak_ptr<WidgetState> widget;
    WidgetPaintDeviceKind kind = WidgetPaintDeviceKind::Unknown;
    std::optional<std::uint16_t> dpiYOverride;
};
struct WidgetConstructorTailObservation {
    std::string qualification;
    WidgetFontAlias expectedFontAlias = 0;
    std::optional<std::uint32_t> fontScreenField3c;
    // 0 is observed null; positive aliases name the same runtime-owned DC.
    std::optional<std::uint32_t> displayDcAlias;
    std::optional<std::int32_t> displayLogicalDpiY;
    std::optional<bool> currentQThreadIsApplicationQThread;
    std::vector<WidgetMetricInputObservation> metricInputs;
    std::optional<std::uint32_t> currentWidgets, maximumWidgets;
    std::optional<std::uint32_t> notifyDepth;
    std::optional<std::int32_t> applicationCompileVersion;
    std::optional<std::uint32_t> applicationAttributes;
};
// WidgetRuntimeState additions:
// void observeConstructorTailValues(const std::shared_ptr<WidgetState>&,
//                                   const WidgetConstructorTailObservation&);
// Actual widget must already be bound and at the font/device boundary with
// its real font value. First SplitView attempt may stop there, root binds this
// observation to pendingSplitView()->widgetState(), then resumes SAME cursor.
// Thus no guessed future widget identity or reallocation is needed.
// Same runtime owns WidgetDisplayDeviceState (DC alias + DPI capability) and
// QWidget current/max values. No observed-success input or copied live values.

enum class WidgetConstructorTailStage {
    FontDevice, Attributes, WidgetCounts, Compatibility, Create,
    PostPending, Complete
};
struct WidgetConstructorTailReport {
    WidgetConstructorTailStage stage = WidgetConstructorTailStage::FontDevice;
    bool completed = false;
    std::string issue;
};
// New WidgetConstructorTail in widget_events.hpp/.cpp:
// const WidgetConstructorTailReport& resume(WidgetRuntimeState&,
//                                           const std::shared_ptr<WidgetState>&);
// const WidgetConstructorTailReport& report() const noexcept;
// It retains exact runtime/receiver lifetime, typed stack Create event and
// WidgetEventDispatch. SplitView and EditorBase own one tail each. No callback
// flags indicate success. Normal Create result is computed by shared dispatch.
// Create remains alive across PostPending: native destructor is6506e869 AFTER
// raw4a postreturn6506e857 and private+c8 write6506e85d.

// Shared QObject count on WidgetRuntimeState, separate from QWidget counters:
// void observeQObjectCount(std::uint32_t, const std::string& qualification);
// std::optional<std::uint32_t> qobjectCount() const noexcept;
// std::uint32_t retainedQObjectConstructionDelta() const noexcept;
// Private source-ordered commits, friends EditorBaseConstruction and
// EditorGridConstruction: commitQObjectConstruction(WidgetState&/GridLayoutState&).
// Each actual component records whether its ctor increment has committed;
// duplicate commit refuses. Native uint32 wrap semantics; an unknown base
// stays unknown with exact delta. Destructor zero-test/free remains guarded.

// WidgetEventState additions: create(), childAdded(actual GridLayoutState).
// Child payload is a borrowed lifetime-checked actual identity, never owner.
// Raw44 reports true after normal QWidget/QObject childEvent wrapper; Create
// and property report false. Child event remains alive across post-parent
// declarative bookkeeping until caller reaches6712d677 destruction.
