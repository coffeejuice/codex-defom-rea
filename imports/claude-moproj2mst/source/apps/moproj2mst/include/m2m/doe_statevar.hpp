#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "m2m/qt4.hpp"
#include "m2m/xml.hpp"

namespace m2m::engine { struct GuiModel; struct Display; struct StateVarPlotEffects; }

namespace m2m::doe {
class StudyIo;

enum class SettingObservation { Unavailable, Absent, Value };

// Converted getter result from an explicit profile. Unavailable is not an absent
// QSettings key;808740/808420 return0/false only for an observed absent key.
template<class T> struct StateVarSetting {
    SettingObservation observation = SettingObservation::Unavailable;
    T converted{};
    std::optional<T> result() const {
        if (observation == SettingObservation::Unavailable) return {};
        return observation == SettingObservation::Absent ? T{} : converted;
    }
};

struct StateVarPalette {
    std::array<int, 25> codes{};                  //143d76584 ->1446b3d60
    std::array<int, 25> alternateCodes{};         //143d765e8 ->1446b3dd0
    std::array<std::array<float, 4>, 25> rgba{};  //143d763f4 ->1446b3bd0
    bool operator==(const StateVarPalette&) const = default;
};

struct StateVarGlobalValues {
    int colorbarNo = 0;                          //143d763c4 ->+40
    qt::String title;                           //143d763d8 ->+438
    std::array<float, 4> deflectionReferenceRgba{}; //143d77590+270 ->+490
    int paletteMode = 0;                         //143d763ec ->+528
    bool paletteFlag0 = false, paletteFlag1 = false; //143d763f0/f1 ->+52c/d
    StateVarPalette palette;
    int principalMode = 0;                       //143d7664c ->+530
    std::array<float, 12> principalRgba{};        //143d76650 ->+534
    std::array<int, 7> iso{};                     //143d76688..6a0 ->+564..57c
};

//0x20-byte allocation d33d80 copies only these fields; padding+0c/+1c is not
// copied. Preserve floating-point bits without inventing channel/value meanings.
struct StateVarColorbarPoint {
    std::array<std::uint32_t, 3> floatBits{}; //+0/+4/+8
    std::uint64_t doubleBits = 0;           //+10
    std::int32_t field18 = 0;
    bool operator==(const StateVarColorbarPoint&) const = default;
};

struct StateVarColorbarSnapshot {
    std::int32_t field00 = 0;
    qt::String title;                       //+8
    std::uint64_t field10Bits = 0;
    std::uint64_t field18Bits = 0x3ff0000000000000ULL; //double1 default
    bool flag20 = false;
    std::vector<StateVarColorbarPoint> first;  //+28
    std::vector<StateVarColorbarPoint> second; //+30: controls clone loop count
};

// External observations are explicit inputs, not reconstructed Qt/global defaults.
enum class StateVarPresence { Unavailable, Absent, Present };
enum class StateVarDisplayBackend { Unavailable, Scene, Alternative };
struct StateVarLegendGlobals {
    std::uint8_t fieldD8 = 0; //143d763c8
    int fieldDc = 0, fieldE0 = 0, fieldE4 = 0; //143d763b8/c0/bc
    std::uint8_t fieldE8 = 0; //143d763e8
    bool operator==(const StateVarLegendGlobals&) const = default;
};
struct StateVarDisplayContext {
    const engine::Display* boundDisplay = nullptr;
    std::optional<bool> mainWindowAvailable;
    StateVarDisplayBackend backend = StateVarDisplayBackend::Unavailable;
    StateVarPresence legendSlot = StateVarPresence::Unavailable;
    StateVarPresence propertyTable = StateVarPresence::Unavailable;
    std::optional<std::array<double, 2>> sceneSize; //QGraphicsScene width/height
    std::optional<StateVarLegendGlobals> legendGlobals;
};

// The adapter never reads the current user's settings. A nonempty identity plus
// explicitly present globals/settings is required before fresh construction.
struct StateVarConstructorContext {
    std::string profileIdentity;
    std::optional<StateVarGlobalValues> globals;
    StateVarSetting<int> deflectionScaleType;
    StateVarSetting<int> deflectionScale;
    StateVarSetting<int> deflectionLineThickness;
    StateVarSetting<bool> showDeflectionReference;
    StateVarSetting<bool> transparentDeflectionReference;
    // Global143d9a5a0+760 snapshot. Unavailable and observed empty are distinct.
    std::optional<std::vector<StateVarColorbarSnapshot>> colorbars;
    std::optional<qt::String> translatedNoneTitle; //QObject::tr(" None"), d267a0
    std::optional<StateVarDisplayContext> displayContext;
};

struct HistogramState {
    int binWidth = 10;          //4117b0: +10
    bool graphOn = false;      //+14
    bool flag30 = false;       //+30
    // Font/plot Qt objects remain opaque; their recovered fixed defaults are
    // retained in evidence, not asserted to be a complete QObject projection.
    bool qtPayloadRecovered = false;
};

struct HistogramRegistration {
    std::string signal, slot; // notifier bare method names after2cfbe0
    std::weak_ptr<HistogramState> receiver;
};

// Logical projection of the recovered constructor/visibility calls. Native Qt
// payload (QTime, font metrics, painting/event delivery) remains opaque.
struct StateVarLegendState {
    const engine::Display* display = nullptr;
    int width = 0, height = 0, x = 0, y = 0;
    double aspect = 0;
    std::array<double, 4> constraintFields{82, 200, 40, 512}; //+10/+18/+20/+28
    StateVarLegendGlobals globals;
    qt::String smallFontFamily = qt::L("Arial"), largeFontFamily = qt::L("Arial");
    int smallFontPoints = 8, largeFontPoints = 10, fontWeight = 50;
    bool italic = false;
    int smallColor = 2, largeColor = 2; //Qt::black
    int graphicsFlags = 7; //1|2|4; acceptHoverEvents=true
    bool acceptHoverEvents = true;
    bool visible = false, insertedIntoScene = false;
    int cacheMode = 1;
    std::size_t cacheInvalidations = 0;
    bool qtPayloadRecovered = false;
};

// Isolated single-project state/observations of original global palette copies
// and deferred registrations. This is not process-global dispatch/Close parity.
struct StateVarRuntime {
    std::optional<StateVarPalette> palette;
    std::vector<HistogramRegistration> histogramRegistrations;
    std::vector<std::string> events;
    std::vector<StateVarLegendState> legends;
    struct PointAllocation {
        StateVarColorbarPoint value;
        bool orphanedByRecoveredDelete = false;
    };
    // Original QList clear does not delete pointer payloads. Keep their data in
    // this conversion's arena, including recovered orphan events. Scope cleanup
    // is not a claim about original process-global Close or allocator behavior.
    std::vector<PointAllocation> colorbarPointAllocations;
};

struct StateVarColorbar {
    std::int32_t field00 = 0;
    qt::String title;
    std::uint64_t field10Bits = 0;
    std::uint64_t field18Bits = 0x3ff0000000000000ULL;
    bool flag20 = false;
    std::vector<std::size_t> firstAllocations, secondAllocations;
};

struct StateVarState {
    std::string constructorProfile;
    StateVarGlobalValues constructorGlobals;
    int deflectionScaleType = 0, deflectionScale = 0, deflectionLineThickness = 0;
    bool showDeflectionReference = false, transparentDeflectionReference = false;
    int colorbarNo = 0; //assigned from explicit globals before XML
    int plotType = 2, type = 1, plotVar = 0, plotCompNo = 0, rangeType = 0;
    double rangeMin = 0, rangeMax = 0, minimum = 0, maximum = 0;
    std::shared_ptr<HistogramState> histogram;
    std::unique_ptr<StateVarColorbar> auxiliaryColorbar; //+448
    //Original leaves+448 stale between delete and replacement. Represent this
    //without dereferencing a freed pointer if an unsupported prefix stops there.
    bool auxiliaryPointerDangling = false;
    bool auxiliaryColorbarPending = false;
    bool postparsePending = false;
    bool plotApplyInProgress = false; //+450, d25340/d25360
    bool cachedDataPresent = false; //+470, logical pointer presence only
    int cachedVariable = -1; //+478, d302c0
    qt::String plotTitle; //+430
    bool nestedQtPlotBlocksRecovered = false;
};

// d15b80 ->d16530 for an explicit selection. notify=false is the XML route;
// notify=true executes shared notifier gates then typed receiver effects at the
// exact two emit points. Missing receiver knowledge guards after prior effects.
void applyStateVarSelection(engine::GuiModel& model, int variable, int component,
    bool notify, const StateVarConstructorContext* context, StudyIo* io,
    engine::StateVarPlotEffects* effects = nullptr);

// c905b0/d15610/d15720, then d328c0. Persists separately from the property table
// through model reset. Changed colorbar uses an explicit optional global list;
// invalid/missing payloads guard after their established prefix. Stops
// at unresolved postparse/model/display branches. Empty-view variable0 can complete
// with explicit translation and, when consumed, display observations.
void readStateVar(engine::GuiModel& model, const xml::Node& node,
                  const StateVarConstructorContext* context, StudyIo* io);

//d318e0: append the ten scalar children, then lazily ensure Histogram and append
//its two persisted fields only when graphOn. Reuses actual owners, without XML
//read resets, selection application, colorbar replacement or legend effects.
void writeStateVar(engine::GuiModel& model, xml::Node& node,
                   const StateVarConstructorContext* context, StudyIo& io);

} // namespace m2m::doe
