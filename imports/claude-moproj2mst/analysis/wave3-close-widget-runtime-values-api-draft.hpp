// Design artifact only. Not included by production or any build target.
// Values/aliases must be populated from the accepted probe003 reconciliation.
#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace m2m::engine {
class MainViewState;
class ViewManagerState;
class WidgetRuntimeState;

// Observation-local aliases are NOT original pointer values or global IDs.
// A receipt maps each positive alias to a native shared-data identity. Runtime
// interning creates one actual C++ data owner per alias/type/runtime.
using WidgetFontAlias = std::uint32_t;
using WidgetPaletteAlias = std::uint32_t;

struct WidgetFontValueObservation {
    WidgetFontAlias dataAlias = 0;
    std::uint32_t resolveMask = 0;
};
struct WidgetPaletteValueObservation {
    WidgetPaletteAlias dataAlias = 0;
    std::uint32_t groupResolveWord = 0;
};
struct WidgetLocaleValue {
    std::uint16_t index = 0;
    std::uint16_t numberOptions = 0;
};
struct WidgetFontDataObservation {
    WidgetFontAlias alias = 0;
    std::optional<std::uint32_t> comparisonField38;
};
struct WidgetPaletteDataObservation { WidgetPaletteAlias alias = 0; };

struct WidgetFontClassObservation {
    std::string classKeyBytes;
    WidgetFontValueObservation value;
};
struct WidgetPaletteClassObservation {
    std::string classKeyBytes;
    WidgetPaletteValueObservation value;
};
struct WidgetApplicationFontObservation {
    std::optional<WidgetFontValueObservation> defaultValue;
    std::optional<bool> mutexPresent;
    std::optional<bool> classCachePresent;
    // nullopt means incomplete/unqualified population, not an empty cache.
    // Present entries retain native firstNode/nextNode iteration order.
    std::optional<std::vector<WidgetFontClassObservation>> classes;
};
struct WidgetApplicationPaletteObservation {
    std::optional<WidgetPaletteValueObservation> defaultValue;
    std::optional<bool> classCachePresent;
    std::optional<std::vector<WidgetPaletteClassObservation>> classes;
};

struct WidgetInitialValueObservation {
    std::optional<WidgetFontValueObservation> font;
    std::optional<WidgetPaletteValueObservation> palette;
    std::optional<WidgetLocaleValue> locale;
    std::optional<std::uint32_t> inheritedFontMask;
    std::optional<std::uint32_t> inheritedPaletteMask;
    // Bind these to the existing actual WidgetState fields; later helpers read
    // the live state rather than an independent copy of the parent flags.
    std::optional<std::uint32_t> attributesLow;
    std::optional<std::array<std::uint32_t, 3>> extendedAttributes;
    std::optional<std::uint32_t> windowFlags;
    std::optional<bool> opaqueChildrenDirty; // private+1d8 bit16
    std::optional<bool> opaque;              // private+1d8 bit17
    std::optional<bool> graphicsEffectPresent; // private+d8; true remains guarded
    std::optional<bool> autoFillBackground;    // extra+5c bit4
    std::optional<bool> stylePresent;          // extra+20
    std::optional<std::int32_t> stylesheetLength;
    std::optional<bool> proxyPresent;          // extra+10, distinct from topExtra
};

struct WidgetRuntimeValueObservation {
    std::weak_ptr<MainViewState> mainView;
    std::weak_ptr<ViewManagerState> manager;
    std::string qualification; // exact accepted receipt/role/state binding
    std::vector<WidgetFontDataObservation> fontData;
    std::vector<WidgetPaletteDataObservation> paletteData;
    WidgetApplicationFontObservation applicationFont;
    WidgetApplicationPaletteObservation applicationPalette;
    WidgetInitialValueObservation mainViewValues;
    WidgetInitialValueObservation managerValues;
    // This is the QLocale value observed in the fresh QWidgetPrivate constructor
    // role; it is not inferred from the current MainView locale.
    std::optional<WidgetLocaleValue> freshPrivateLocale;
    std::optional<bool> applicationModalState;
    std::optional<std::int32_t> applicationStylesheetLength;
};

// Proposed public addition to the EXISTING WidgetRuntimeState:
// void observeStartupValues(const WidgetRuntimeValueObservation&);
// Must be called after binding the actual MainView and its actual manager to
// this runtime, and before pendingSplitView() exists. Validate everything into
// temporary owned state before committing. Empty qualifications, missing/live
// owner mismatches, conflicting duplicate aliases, missing referenced aliases,
// contradictory absent-cache/nonempty-entry states and replacement observations
// are misuse. Unknown optional fields are valid and retain an exact later guard.
// A repeat before allocation may be idempotent only if the complete supplied
// observation is identical; it must never reset subsequent native mutations.

// Runtime-owned state (private constructors; no detached public data factories):
class WidgetFontDataState;     // immutable identity + optional comparisonField38
class WidgetPaletteDataState;  // immutable identity; color/brush payload unqualified
struct WidgetFontValue {
    std::shared_ptr<const WidgetFontDataState> data;
    std::uint32_t resolveMask = 0;
};
struct WidgetPaletteValue {
    std::shared_ptr<const WidgetPaletteDataState> data;
    std::uint32_t groupResolveWord = 0;
};
class ApplicationFontCacheState;
class ApplicationPaletteCacheState;

// Proposed read-only accessors on existing WidgetState:
// const std::optional<WidgetFontValue>& font() const noexcept;
// const std::optional<WidgetPaletteValue>& palette() const noexcept;
// const std::optional<WidgetLocaleValue>& locale() const noexcept;
// std::optional<bool> opaqueChildrenDirty() const noexcept;
// std::optional<bool> opaque() const noexcept;
// Application caches and data objects are owned only by the same runtime;
// widgets share data objects but own their value/mask/group fields independently.
}
