// SfPreSimControl300 project-local sidecar boundary. The methods correspond to
// original small-file helpers, not the public m2m ABI. Native read/parser and
// delete bindings remain explicit dependencies; no ambient working directory.
#pragma once

#include <span>
#include <functional>
#include "m2m/engine/simcontrol300.hpp"
#include "m2m/mst/stream.hpp"

namespace m2m { struct RecoveryEntry; }
namespace m2m::engine::simcontrol300 {

struct SpringRecord {
    int firstObject;
    double firstValue, secondValue;
    int secondObject;
    std::array<double, 3> direction;
    int flag;
};

class Extra3DIo {
public:
    explicit Extra3DIo(unsigned codepage) : codepage_(codepage) {}
    virtual ~Extra3DIo() = default;
    // 1410da510/690: zero all outputs, exists then ReadOnly open, read up to
    // count values through original QTextStream. Return OPEN success, including
    // an empty file or a later parse/read failure. count0 still opens the file.
    virtual bool readDoubles(const qt::String& path, std::span<double> output) = 0;
    virtual bool readInts(const qt::String& path, std::span<int> output) = 0;
    // 1410dab40: SPRING.INI and optional SPRING2.INI. The native reader admits
    // bounded canonical ASCII positive rows with a missing, unreadable or
    // bounded canonical ASCII companion; the selected editor still requires
    // qualified combo-label context.
    virtual bool readSprings(std::vector<SpringRecord>& output) = 0;
    // 1407fbc10: fopen("rt"), close, remove; open failure returns true. This
    // is deliberately distinct from a filesystem exists+remove operation.
    virtual bool removeIfReadable(const qt::String& path) = 0;
    virtual void nbcRemoveWarning() = 0;
    // QFile WriteOnly (without Text); the supplied device retains the recovered
    // buffering/failure/close behavior. Null or !opened means open failure.
    virtual std::unique_ptr<mst::OutputDevice> openWrite(const qt::String& path) = 0;
    // 1410da9e0, implemented here. Returns open success; later failures ignored.
    bool writeStringList(const qt::String& path, const std::vector<qt::String>& items);
private:
    unsigned codepage_;
};

// Bounded QListWidget item payloads and selected-form projection. The native
// combo searches the visible item text's signed decimal prefix, first match.
// This is not an InitialEditorSession or native QWidget owner.
struct SpringComboLabels {
    std::vector<qt::String> first, second;
    int firstCurrentIndex = 0, secondCurrentIndex = 0;
};

struct SpringSelectionState {
    bool pageChecked = true;
    std::vector<SpringRecord> items;
    int currentRow = 0;
    int listCurrentRow = 0;
    std::optional<SpringComboLabels> comboLabels;
    int firstSelectedId = 0;
    int secondSelectedId = 0;
    std::array<qt::String, 5> visibleNumbers;
    bool recordFlagChecked = false;
};

enum class SpringItemEvent { InitialLoad, ItemClicked, SaveCurrentItem };
enum class SpringTransitionStage {
    Preflight, OldItemWritten, FirstComboApplied, SecondComboApplied,
    VisibleLoaded, Complete
};
struct SpringTransitionReport {
    ApplyResult outcome = ApplyResult::UnsupportedSnapshot;
    SpringTransitionStage stage = SpringTransitionStage::Preflight;
    bool oldItemWritten = false, sameRow = false, firstMatch = false, secondMatch = false;
};
SpringTransitionReport transitionSpringItem(SpringSelectionState&,
    std::optional<int> requestedRow, SpringItemEvent);
bool setSpringListCurrentRow(SpringSelectionState&, int);

struct Extra3DPage {
    bool nbcEnabled;
    std::array<qt::String, 2> nbc;
    bool switchToSparseEnabled;
    qt::String switchToSparse;
    bool stretchEnabled;
    qt::String stretchKind;
    std::array<qt::String, 2> stretch;
    bool weightEnabled;
    qt::String weight;
    bool strainDistributionEnabled;
    std::optional<SpringSelectionState> spring;
};

// The supplied reader must already establish the original numeric profile.
// Existing two-argument callers retain the empty-spring profile. Numeric ID
// overloads are synthetic parsed-label projections for legacy callers. The
// actual-label overload admits bounded finite edits and first-match selection;
// this is still a private page model, not full Session palette admission.
std::optional<Extra3DPage> fillExtra3D(Extra3DIo&, NumericLocale);
std::optional<Extra3DPage> fillExtra3D(Extra3DIo&, NumericLocale,
    std::span<const int> firstComboIds, std::span<const int> secondComboIds);
// The native constructor populates combo item text after reading the scalar
// sidecars. The lifecycle supplies this deferred projection so an object-sync
// failure cannot move ahead of those reads.
std::optional<Extra3DPage> fillExtra3D(Extra3DIo&, NumericLocale,
    std::function<std::optional<std::vector<int>>()> comboIdsAfterScalarReads);
// Actual native combo context. Labels are populated only after scalar reads.
std::optional<Extra3DPage> fillExtra3D(Extra3DIo&, NumericLocale, SpringComboLabels);
std::optional<Extra3DPage> fillExtra3D(Extra3DIo&, NumericLocale,
    std::function<std::optional<SpringComboLabels>()> comboLabelsAfterScalarReads);
// Constructor140ecb340 overwrites these texts AFTER the initial sidecar fill,
// including when NBC.DAT opened successfully. It leaves its checkbox unchanged.
void constructorRefreshExtra3D(Extra3DPage&);
ApplyResult applyExtra3D(Extra3DIo&, const Extra3DPage&);

// Bounded native profile: KnownC ASCII real input, signed decimal int32 input
// without ambiguous leading zeros, and positive-count SPRING files with
// canonical ASCII rows up to
// the explicit candidate resource cap with bounded companion flags. Other
// decoder/integer branches explicitly refuse. The QFile read/write directory is
// absolute, ordinary MAX_PATH with room for every fixed sidecar and may contain
// Unicode. Readable-delete retains its separate narrow ASCII path guard. This
// explicit directory replaces the original process current-directory dependency;
// original ASCII relative names under Unicode CWD remain a separate dependency.
std::unique_ptr<Extra3DIo> makeNativeExtra3DIo(const qt::String& directory,
    unsigned codepage, std::function<void()> nbcWarning);

// 140ed5dd0's first supported branch: both actual recovery lists exist and are
// empty. NRECID is cleared; ERECID remains. Advanced reapply and output-array
// resize calls still run after an Advanced validation stop.
ApplyResult applyEmptyRecoveryTail(SimControl&, GuiModel&, const AdvancedPage&,
    const std::vector<RecoveryEntry>* rootList,
    const std::vector<RecoveryEntry>* operationList);

// Owner-bound private Save tail. The implementation resolves the actual A/B
// holders at the reached tail after preceding page and Extra3D effects.
ApplyResult applyOwnedRecoveryTail3D(SimControl&, GuiModel&, const AdvancedPage&,
    Operation&);

} // namespace m2m::engine::simcontrol300
