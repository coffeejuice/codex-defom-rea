// m2m/engine_scene.hpp: the model links of the main window's 3D display SfGLDisplay (packet V,
// added by MDT L4 for SfOprShapeOptim; docs/re/moproj2mst/mdt/L4/spec.md section 9). The display
// itself, its main model +0x1a0 and its camera are packet moproj's (src/engine/viewer_camera.cpp,
// projectDisplay / displayShow); this header adds what engine code reads besides the main model:
// the second (+0x1b8) and third (+0x1c8) models, the named models (+0x1d8, 0x140cfa0e0) and the
// view mode (+0x204 / +0x20c). Decompiles: gui_mo/s1/mdt/L4/vs/a.c, b.c.
#pragma once

#include <array>
#include <memory>
#include <optional>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/engine.hpp"

namespace m2m::doe { struct ViewStepDataState; struct SliceCoefficients; struct SliceCollectionState; struct SliceConstructorRuntime; class PropertyTableState; }

namespace m2m::engine {

struct GuiModel;
struct Display;

// d7c260: snapshot the actual View list, refresh the current object, then visit
// existing Settings and table cells in source order. No table or View creation.
void finalizeStepDataPropertyTable(GuiModel&, doe::PropertyTableState&);
// c90640/141200360: snapshot actual View pointers after StateVar serialization;
// append each Object only after its complete writer returns.
void writeViewerStepDataObjects(GuiModel&, xml::Node& stepData);

// Actual existing zero-based View; no list growth, object allocation or
// Settings lookup. StepData Object parsing must retain this view's own state.
doe::ViewStepDataState& viewerStepDataState(GuiModel&, int zeroBasedIndex);

// 141200e50 SfDispOpt branch: existing View/Settings only, then object-derived
// reset, ordered XML fields, and cached mesh/geometry flags. Defaults retain
// viewer.cpp's explicitly qualified fresh CRT preference values (1/1).
void readViewerDisplayOptions(GuiModel&, int zeroBasedIndex, const xml::Node&);

enum class ViewerDisplayPreferenceProfile { FreshCrtDefaults };
// Read-only diagnostic snapshot of the actual Settings, never a second owner.
struct ViewerDisplayOptionSnapshot {
    std::int32_t mode = 0;
    std::array<std::uint8_t, 46> flags{};
    std::uint8_t cachedMesh = 0, cachedGeometry = 0;
    ViewerDisplayPreferenceProfile preferenceProfile = ViewerDisplayPreferenceProfile::FreshCrtDefaults;
};
std::optional<ViewerDisplayOptionSnapshot> viewerDisplayOptionSnapshot(const GuiModel&, int zeroBasedIndex);

// Ordered Slice import on the existing View. Coefficients can remain unavailable
// until their first native use; the collection allocation precedes that use.
void readViewerSlice(GuiModel&, int zeroBasedIndex, const doe::SliceCoefficients&,
                     doe::SliceConstructorRuntime* runtime = nullptr);
const doe::SliceCollectionState* viewerSliceState(const GuiModel&, int zeroBasedIndex) noexcept;
// Read-only diagnostic of a pointer already extracted before a cleanup refusal.
const doe::SliceCollectionState* viewerPendingSliceCleanup(const GuiModel&) noexcept;

struct DisplayModels {
    // QPointer<SfStepData> members expire independently of the model's backlink.
    GuiModelGuard second;          // +0x1b8 (0x140cf9960; 0x140cf9990 makes it, 0x140cf9930 sets it)
    // owner of a second model the display made (0x140cf9990) or was given (0x140cf9930); 0x140cf9bd0
    // deletes +0x1b8 (vt+0x18) when keepSecond is 0 (MDT L5S)
    std::unique_ptr<GuiModel> secondOwned;
    bool secondShown = false;      // +0x1c0; cf9b90 initializes, cf9c70 sets
    bool keepSecond = false;       // +0x1c1 (0x140cf9cd0)
    GuiModelGuard third;           // +0x1c8 (0x140cf9d20)
    bool thirdShown = false;       // +0x1d0; cf9e80 initializes, cf9f50 sets
    // +0x1d8 QMap<QString, holder*>; the holder (0x140d12250, 0x10 bytes) is a QPointer to the
    // model and a flag +8 = 0. The display made each model (0x140cfa0e0) and never deletes it.
    struct Named {
        String name;
        std::unique_ptr<GuiModel> model;
        bool flag = false;
    };
    std::vector<Named> named;      // in key order (QMap)
    int viewMode = 0;              // +0x204: 1 by 0x140c80a20, 2 by 0x140c80b00
    int viewArg = 0;               // +0x20c
};

// viewer_camera.cpp: the display's main model (+0x1a0) and the links above
GuiModel* displayMain(const Display& d);
// d0d610: guarded +1e0 override, otherwise +1a0 main; independent of shown flags.
GuiModel* displaySelectedModel(const Display& d);
// d0d3d0: clear override, then first match in named/third/main/second order.
// Sets only the matched shown flag; null equality participates in selection.
GuiModel* selectDisplayModel(Display& d, GuiModel* requested);
bool displayMainShown(const Display& d); // +0x1a8, cfac50 active-model selector
DisplayModels& displayModels(Display& d);

// @src 0x140cfa0e0  the named model, made on first use: new SfStepData(dim of the main model,
// 0x140c7a6b0, no database); "ROIs" step +0x3400 = 1, "Coupons" step +0x3400 = 2 (name, icon and
// tool tip: UI); model+0x10 = the display; step +0x744 and the title +0x340 from the main model's
// step, the sim control's GEOTYP (+0x100) from the main model's when both have one
GuiModel* displayNamedModel(Display& d, const String& name);

// @src 0x140c7ae00 + 0x140d0d670  the "ROIs" model of m's display (its +0x25c = 2), or m itself
// when no display shows m; null for null
GuiModel* displayRoiModel(GuiModel* m);

// @src 0x1402c81b0 -> 0x140cf9990  the second model, made on first use from the main model's
// dimension (model+0x10 = the display, the sim control's GEOTYP from the main model's); null without
// a main model (MDT L5S, src/engine/scene.cpp)
GuiModel* displaySecond(Display& d);
// @src 0x140cf9930  the second model set to m (the display owns it)
void displaySetSecond(Display& d, std::unique_ptr<GuiModel> m);
// @src 0x140cf9bd0  the second model deleted unless +0x1c1 is set
void displayDropSecond(Display& d);

// QPointer clearing: the display's links to m become null (called by GuiModel's dtor)
void displayForget(Display& d, const GuiModel* m);

}  // namespace m2m::engine
