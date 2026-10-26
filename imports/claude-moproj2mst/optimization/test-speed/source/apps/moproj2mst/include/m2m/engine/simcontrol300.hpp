// Recovered initial SfPreSimControl300 page state. These helpers are not a complete
// editor and are deliberately not connected to the public Save dispatcher.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include "m2m/qt4.hpp"

namespace m2m { struct Project; struct Operation; }
namespace m2m::engine { struct GuiModel; }
namespace m2m::engine::simcontrol300 {

using SimControl = def::model::Cls_1401e7b10;

enum class Page { General, StepCount, StepSize, Stopping, Meshing, Iteration,
                  Environment, Advanced, Extra3D };

// 0x140eb80c0 then constructor vt+1b8 (0x140eb50f0). The second stopping
// and meshing fills are observable; a future complete snapshot must retain them.
inline constexpr std::array<Page, 9> initialFillOrder{
    Page::General, Page::StepCount, Page::StepSize, Page::Stopping, Page::Meshing,
    Page::Iteration, Page::Environment, Page::Advanced, Page::Extra3D};
inline constexpr std::array<Page, 2> constructorRepeatFillOrder{
    Page::Stopping, Page::Meshing};
// 0x140ecb340(0), called after the two immediate repeat fills, backs up SC,
// copies its curves into temporary editor state, refills these pages, then
// restores SC. This refresh still determines the final remembered widgets.
inline constexpr std::array<Page, 6> constructorBackedUpRefreshOrder{
    Page::General, Page::StepCount, Page::StepSize, Page::Iteration,
    Page::Environment, Page::Advanced};
inline constexpr auto saveApplyOrder = initialFillOrder;

// Recovered later-page obligations, not implemented page state. In particular,
// Save 0x140eb71e0 re-runs 0x140eb6310: first object with any nonzero rotation
// component, otherwise object 1. It does not use a frozen combo selection or 0.
struct LifecycleContract {
    static constexpr bool stoppingAndMeshingFilledTwice = true;
    static constexpr bool rotationReselectedAtSave = true;
    static constexpr int rotationFallbackObject = 1;
    static constexpr bool completeForPublicSave = false;
};

// External widget enable/override state must be supplied explicitly. There is
// no license singleton or inferred default here. procType1 is the isolated
// PRE-style profile; procType5 is MO, with explicit project/control state.
struct GeneralContext {
    int applicationMode;
    int dimension;
    std::array<bool, 6> processEnabled;
    bool preserveProcessFlags; // 0x140ebaf66 external operation override
    bool meshNumberEnabled;
    const Project* moProject = nullptr;
};
struct GeneralPage {
    qt::String title, operationName;
    int meshNumber;
    int unit;
    int simulationType;
    std::array<bool, 6> processChecked;
    std::array<bool, 6> processEnabled;
    bool heatMethod2;
    int heatMethodCombo;
    bool preserveProcessFlags;
    bool meshNumberEnabled;
    const Project* moProject = nullptr; // General reads active subproject at Save.
};

// Final original General enable dependencies, not guessed license answers.
// TRANS4's earlier1412fae90 assignment is overwritten by1412faeb0 at fill end.
struct GeneralCapabilityState {
    bool trans3Enabled; //1412fae10(dimension) OR1412fab10(dimension)
    bool trans4Trans5HeatEnabled; //1412faeb0(dimension)
};
GeneralContext generalContextForMo(const Project&, int dimension, GeneralCapabilityState);

// Observed original DEFORM-3D query returns for one General fill invocation.
// No observation has a default license answer: <=0 means unavailable, >0
// available, nullopt means unknown. The caller supplies evidence/state; this
// adapter neither contacts the original license service nor reads live settings.
struct MoFeatureAvailability {
    std::optional<std::int32_t> microstructure3D; //3D_DEF_GUI_MIC3
    std::optional<std::int32_t> wizardHeatTreat3D; //3D_DEF_WZD_HEAT_TREAT3, short-circuited
    // Three separate 3D_DEF_HEATING calls in source order: HTMTHD, TRANS5,
    // final TRANS4. Preserve differing observations; do not assume stability.
    std::array<std::optional<std::int32_t>, 3> heating;
};
std::optional<GeneralContext> generalContextForMoAvailability(const Project&,
    int dimension, const MoFeatureAvailability&);

// Startup feature-record bytes, distinct from General's later availability
// transactions. Original1412e73d0 combines both product dimensions into+6.
struct StartupHeatingFeatures {
    std::optional<std::uint8_t> twoD; //record+1c:2D_DEF_HEATING
    std::optional<std::uint8_t> threeD; //record+1d:3D_DEF_HEATING
};
std::optional<bool> pressureFieldsForStartupHeating(const StartupHeatingFeatures&);

// Numbered objects passing original 0x140834fa0, in combo insertion order.
// The caller owns recovery of that predicate; an unqualified list must not be
// manufactured from every object in the model.
struct MoStepCountContext {
    Operation* operation; // Null is the original no-operation branch.
};
struct StepCountContext {
    int applicationMode;
    std::vector<int> primaryDieObjects;
    std::optional<MoStepCountContext> mo; // Mandatory explicit state for procType5.
};
struct StepCountPage {
    int start, steps, increment, primaryDie;
    std::optional<MoStepCountContext> mo;
    bool nstartChecked = false;
};
enum class ValidatorState { Invalid = 0, Intermediate = 1, Acceptable = 2 };
enum class NumericLocale { KnownC, Unrecovered };
// The validator result is explicit widget state, not a replacement validator.
// Initial fill produces only finite numeric text / int32 text (Acceptable).
// Edited text requires the caller to supply the original validator's result.
struct ValidatedText {
    qt::String text;
    ValidatorState state;
};
struct IterationPage {
    int solver, method, thermalSolver;
    int cgFirstCombo, cgFourthCombo;
    qt::String cgSecond, cgThird;
    std::array<ValidatedText, 2> convergence;
    bool deformationBandwidth, thermalBandwidth;
    ValidatedText deformationIterations, thermalIterations;
};
enum class ApplyResult { Completed, ValidationStopped, UnsupportedSnapshot };

struct StepSizePage {
    int modeRadio; // 0/1/2; radio2 is stored as DTTYPE mode1
    int definitionRadio; // 1 or 2; radio2 above overrides STPDEF to3
    ValidatedText timeMaximum, displacementMaximum, strainMaximum;
    ValidatedText penetration, substeps;
    std::array<ValidatedText, 3> maximumChanges;
    ValidatedText variableTimeMaximum;
    std::array<ValidatedText, 3> variableMaximumChanges;
    ValidatedText slidingError;
    // Original temporary curve editor+e8, copied by0x140ecb340 before its
    // step-size refill. Const shared ownership prevents shallow native copies.
    std::shared_ptr<const def::model::DYNARY> curve;
};

struct StoppingReferenceControls {
    int selectedObject = 0;
    // Fresh setup checks the node radio; both spins are 0 in [0,999999].
    // These values persist when a subsequent fill selects None.
    bool nodeChecked = true, coordinateChecked = false;
    bool nodeEnabled = true, nodeSpinEnabled = true;
    bool coordinatesEnabled = false;
    int node = 0;
    // QLineEdit::text returns nonnull empty even for its initial null storage.
    std::array<qt::String, 3> coordinates{qt::L(""), qt::L(""), qt::L("")};
};
enum class StoppingLabelProfile { Unknown, AsciiNoActiveTranslator };
struct StoppingWidgetState {
    std::array<StoppingReferenceControls, 2> references;
    // Actual setup must supply its translator qualification. NumericLocale
    // alone does not establish bounds for the translated None label.
    StoppingLabelProfile labelProfile = StoppingLabelProfile::Unknown;
};
struct StoppingPage {
    ValidatedText timeMaximum;
    std::array<ValidatedText, 3> strokeMaximum, velocityMinimum, loadMaximum;
    ValidatedText strainMaximum, mdsThreshold;
    int mdsModeCombo = 0;
    std::array<qt::String, 3> rotationVector, rotationCenter;
    StoppingWidgetState widgets;
    std::vector<int> referenceChoices; // None(0), then eligible model order.
    // ebe100(0) falls through to reference 1 when the first selection is None.
    std::array<int, 2> referenceFillOrder{-1, -1};
    // Save looks up remembered reference numbers in the current model, and
    // independently re-runs the rotation-object scan.
};

enum class StoppingFillStage { Preflight, Scalars, ReferenceChoices,
    ReferenceAvailability, ReferenceFill, NodeCallback, Threshold, Rotation, Complete };
enum class StoppingFillIssue { None, InvalidModel, UnknownNumericLocale,
    InvalidRetainedState, NonfiniteValue, UnknownLabelProfile, UnsupportedObjectLabel,
    MissingCurrentObject, UnsupportedMdsMode, CoordinateOwner, CoordinateExtent,
    CoordinateSync };
struct StoppingFillReport {
    StoppingFillStage stage = StoppingFillStage::Preflight;
    StoppingFillIssue issue = StoppingFillIssue::None;
    int scalar = -1, invocation = -1, reference = -1, axis = -1;
    std::size_t nodeCallbacks = 0, coordinateGetterCalls = 0;
    std::size_t coordinateAllocationAttempts = 0, stateVariableDispatches = 0;
    std::size_t referenceVectorZeroings = 0;
    bool complete() const noexcept { return stage == StoppingFillStage::Complete; }
};

// Original process singleton143d91238. These are constructor1402cc4e0 values,
// not inferred settings. Project Open subsequently reads LOCAL_REMESH.DAT or
// LOC_DEN_CTRL.DAT and assigns enabled from that reader's boolean result.
// ConversionSession owns this fresh-process state; Open, editor and Save borrow
// that same owner. Raw TokenReader4 writes require byte flags and explicit bytes
// between fields, not native C++ padding or invalid bool representations.
struct RemeshState {
    int method = 1;                    // +0
    bool goodInternalChangeable = true; // +4
    double filterDistance = 0.0;       // +8
    double uniformInternalEdge = 0.0;  // +10
    std::uint8_t improveContact = 0;   // +18; Save prints the raw byte as %d
    std::array<std::byte, 7> contactToScalingBytes{}; // +19..1f, fresh PE zero-fill
    double scalingFactor = 1.2;        // +20
    std::uint8_t goodSurfaceChangeable = 1; // +28; Save prints the raw byte
    std::uint8_t enabled = 0;          // +29; Open assigns reader result later
};
struct MeshObjectSnapshot {
    int number;
    // Only these four doubles are copied by original14085e050. Shape lists,
    // dimensions and owning arrays are not part of this editor's copy.
    double interference, sizeRatio, tolerance, nodeCount;
};
struct MeshingPage {
    std::vector<MeshObjectSnapshot> objects; // sorted displayed object labels
    qt::String firstInterference, firstSizeRatio, firstTolerance;
    bool firstInterferenceNegative;
    int firstNodeCount;
    bool remeshEnabled;
    int remeshMethodRadio; // initial radio1, optionally changed to3 by fill
    qt::String remeshScalingFactor;
    bool goodInternalChangeableChecked; // inverse of state+4
};

struct EnvironmentTimeIdState {
    // Fresh Qt checkbox is unchecked. Fill only checks it when ID1 is found;
    // an absent ID1 retains the prior widget state across constructor refills.
    bool differenceBoundaryChecked = false;
    std::vector<std::array<qt::String, 2>> betweenTemperatureRows;
    std::vector<qt::String> belowTemperatureRows;
    std::vector<qt::String> aboveTemperatureRows;
};
struct EnvironmentCurveControls {
    bool constantChecked = true;
    bool tableChecked = false;
    bool constantInputEnabled = true;
    bool constantUnitEnabled = true;
    bool definitionEnabled = false;
};
struct EnvironmentAcvcofControls {
    int selection = 0; // constant, temperature(var3), var12, time(var1)
    bool constantInputEnabled = true;
    bool constantUnitEnabled = true;
    bool definitionEnabled = false;
};
struct EnvironmentCurveTemporaries {
    EnvironmentCurveTemporaries();
    // Actual editor+c8,d0,d8,e0 owners, constructed before the initial fill.
    // EnvironmentPage.curves aliases these objects; fill must not replace them.
    std::array<std::shared_ptr<def::model::DYNARY>, 4> curves;
    std::optional<int> currentCurveIndex; // populated by the reached callback
};
struct EnvironmentPage {
    // ENVTMP,CNVCOF,ENVATM,ACVCOF in original copy/apply order.
    std::array<std::shared_ptr<const def::model::DYNARY>, 4> curves;
    std::array<ValidatedText, 4> constantValues;
    // ENVTMP, CNVCOF, ENVATM only. ACVCOF has a separate four-radio callback.
    std::array<EnvironmentCurveControls, 3> curveControls;
    EnvironmentAcvcofControls acvcofControls;
    // ENVMPR,ENVMPT,EHRATE,PENINF,UNTE2H,TINTGF,BLZMAN.
    std::array<ValidatedText, 7> scalars;
    // This is one shared widget also present in GeneralPage, not a separate
    // environment model value. The composer must propagate this filled index.
    std::optional<int> heatMethodComboOverwrite;
    bool viewFactorChecked = false;
    bool viewFactorDefinitionEnabled = false;
    EnvironmentTimeIdState timeId;
};
struct EnvironmentViewFactorState {
    // Borrow the editor's existing +f0 temporary and +138 definition. Dimension
    // is queried from the current model at Apply, not remembered by the widget.
    int dimension;
    const def::model::DYNARY* curve;
    int definition;
};

struct AdvancedPage {
    std::array<ValidatedText, 2> time;
    int timeRadio;
    std::array<ValidatedText, 4> geometryErrorPercent;
    std::array<qt::String, 10> userStrings;
    std::array<bool, 7> outputFlags;
    std::array<int, 3> elementNodeOptions;
    int secondaryDieNumber;
    // The initial fill changes combo selection; Advanced Save does not read
    // these controls. Unpopulated line edits retain their setupUi state.
    std::array<int, 6> frequencyCombo;
    std::array<std::optional<qt::String>, 6> populatedFrequencyText;
};

// The guard is part of each fill contract. A rejected fill does not mutate SC.
// General supports a normal 3D editor with valid initial radios and printable ASCII text;
// StepCount supports procType5 with actual MO operation context (DOE is a
// separate subproject type); Iteration excludes 2D and the unrecovered explicit
// solver toggled callback (SOLMTD=6), invalid radio values and nonfinite CVGERR.
// The numeric locale must be positively supplied as KnownC; ambient original
// locale / validator state is not inferred from a finite model value.
std::optional<GeneralPage> fillGeneral(SimControl&, const GeneralContext&);
bool applyGeneral(SimControl&, const GeneralPage&);
std::optional<StepCountPage> fillStepCount(const SimControl&, const StepCountContext&);
void applyStepCount(SimControl&, const StepCountPage&);
std::optional<IterationPage> fillIteration3D(SimControl&, int dimension, NumericLocale);
ApplyResult applyIteration3D(SimControl&, const IterationPage&);
// First supported curve profile: DTTYPE property0x80, ndim0, no table/owned
// object, mode0/1, STPDEF1/2/3, 3D/KnownC and finite filled scalar values.
// Fill performs the original DTTYPE Open mutation; Apply copies the remembered
// temporary curve only after every preceding validation succeeds.
std::optional<StepSizePage> fillStepSize3D(SimControl&, int dimension, NumericLocale);
ApplyResult applyStepSize3D(SimControl&, const StepSizePage&);
// Dimension3/KnownC, finite filled scalars/rotation vectors. Selected MDS
// objects require finite coordinates and positive unique ASCII-bounded combo
// labels. Mode2 uses the original allocating coordinate getter and live GUI
// no-sync predicate. Reached reload/foreign-owner/unsafe-extent dependencies
// remain explicit. The legacy no-selection profile is retained. This isolated
// overload discards an incomplete local page and uses the fixed
// fresh ASCII/no-active-translator profile; actual owned construction must pass
// explicit retained state through the overload below.
std::optional<StoppingPage> fillStopping3D(const SimControl&, GuiModel&, NumericLocale);
// Constructor refills pass the same retained controls, including values that
// a None selection leaves untouched. This does not complete native setup.
std::optional<StoppingPage> fillStopping3D(const SimControl&, GuiModel&, NumericLocale,
                                        const StoppingWidgetState& previous);
// Actual owned composition retains this destination in original fill order,
// including reached widget/allocation/COW effects before a refused dependency.
// A refusal is a reconstruction boundary, not an original application error.
StoppingFillReport fillStopping3DInto(StoppingPage&, const SimControl&, GuiModel&, NumericLocale);
// The recovered PDIE warning explicitly defaults to No: skip VMIN/LMAX and
// continue the rest of apply, retaining preceding TMAX/SMAX stores.
ApplyResult applyStopping3D(SimControl&, GuiModel&, const StoppingPage&);
// 3D, positive unique object numbers, finite selected-first form scalars and
// finite enabled remesh scaling. Unvisited objects are copied without rounding.
std::optional<MeshingPage> fillMeshing3D(GuiModel&, const RemeshState&);
ApplyResult applyMeshing3D(GuiModel&, RemeshState&, const MeshingPage&);
// ENVTMP/CNVCOF/ENVATM accept constants or known owned tables (ndim1..3),
// with qualified axis/value extents and no signed32 count overflow. Expanded
// ACVCOF paths require the actual editor temporary owners; fallback collapse
// preserves component values and count history. 3D/KnownC. TIMEID accepts values in a known
// one-dimensional allocation without signed32 index overflow. Ignored IDs do
// not require value storage. Furnace flag/state are not fields of this page.
// pressure switch is original global feature byte1446ee586 supplied explicitly;
// its license-record binding is not yet installed in public conversion.
std::optional<EnvironmentPage> fillEnvironment3D(SimControl&, int dimension,
    NumericLocale, bool pressureFieldsPopulated,
    bool previousDifferenceBoundaryChecked = false,
    EnvironmentCurveTemporaries* = nullptr);
void fillEnvironmentSharedControls(GeneralPage&, const EnvironmentPage&);
// The checked VFACTR tail needs the explicit editor state. Missing state refuses
// at that reached boundary after the original preceding stores, never by using
// a new snapshot of the current SC or a default definition.
// Composed Apply supplies the same four mutable owners used by Fill and writes
// their ndim/value before each SC copy. Without owners this remains a qualified
// isolated snapshot helper; it does not claim original editor-state mutation.
ApplyResult applyEnvironment3D(SimControl&, const EnvironmentPage&,
    std::optional<EnvironmentViewFactorState> = std::nullopt,
    EnvironmentCurveTemporaries* = nullptr);
// 3D/KnownC, valid radios, ASCII user strings and existing6x1 integer FRQNCY.
// Valid movement-kind6 objects are excluded until their time-dependent curve
// evaluation/rotation chain is recovered. Apply rechecks this live-model guard.
std::optional<AdvancedPage> fillAdvanced3D(const SimControl&, GuiModel&, NumericLocale);
ApplyResult applyAdvanced3D(SimControl&, GuiModel&, const AdvancedPage&);

// Original CG toggled callback, including CVTTSD2SI's indefinite INT_MIN result.
// This is also the already-supported constructor/Open normalization hook.
void normalizeCgAtOpen(SimControl&);

// Only the eight recovered pages are represented. Later pages are deliberately
// absent, rather than default-constructed as if their widgets had been filled.
struct InitialEditorSnapshot {
    GeneralPage general;
    StepCountPage stepCount;
    StepSizePage stepSize;
    StoppingPage stopping;
    MeshingPage meshing;
    IterationPage iteration;
    EnvironmentPage environment;
    AdvancedPage advanced;
    static constexpr bool completeForPublicSave = false;
};

} // namespace m2m::engine::simcontrol300
