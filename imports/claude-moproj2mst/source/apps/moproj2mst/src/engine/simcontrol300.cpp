#include "m2m/engine/simcontrol300.hpp"
#include "m2m/engine/simcontrol300_io.hpp"
#include "m2m/engine.hpp"
#include "m2m/mst/writer.hpp" // Original class vt+200 at both MO fill and apply.

#include <algorithm>
#include <bit>
#include <cstdint>
#include <cmath>
#include <cstring>
#include <limits>
#include <string_view>
#include <typeinfo>
#include <emmintrin.h>
#include <defmodel/window/inter_object_ops.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/material/state_remap.hpp>
#include <defmodel/dynary_init.hpp>
#include <defmodel/statevar_kw/statevar_kw.hpp>
#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/llobj/cls_14017ef20.hpp>
#include <defmodel/llobj/ext.hpp>
#include <defmodel/members/ext_deps.hpp>
#include <defmodel/layout/state_var_node.hpp>
#include <defmodel/layout/state_var_node_post.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/step/db_directory_receipt.hpp>
#include <defdbio/dbread/w2.hpp>
#include <defmodel/sdctl/step_timer.hpp>

namespace m2m::engine::simcontrol300 {
namespace {
int clampSpin(int value, int low, int high)
{
    return (std::min)((std::max)(value, low), high);
}

template <std::size_t N> bool asciiTerminated(const char (&value)[N])
{
    for (unsigned char c : value) {
        if (c == 0) return true;
        if (c < 0x20 || c > 0x7e) return false;
    }
    return false;
}

int integer(double value) { return _mm_cvttsd_si32(_mm_set_sd(value)); }

// Admission for explicit Fill's last two CVTT conversions, without executing
// a potentially exceptional conversion just to test a rejected input.
bool exactSignedInt32(double value)
{
    const auto bits = std::bit_cast<std::uint64_t>(value);
    const auto magnitude = bits & 0x7fffffffffffffffULL;
    if (magnitude == 0) return true; // either signed zero
    const int exponent = static_cast<int>((magnitude >> 52) & 0x7ff) - 1023;
    if (exponent < 0 || exponent > 31) return false;
    if (exponent == 31)
        return (bits >> 63) != 0 && (magnitude & 0x000fffffffffffffULL) == 0;
    const auto fractionalMask = (std::uint64_t{1} << (52 - exponent)) - 1;
    return (magnitude & fractionalMask) == 0;
}

// Candidate lexical admission only, not the native validator. The bounded
// magnitudes avoid unqualified Qt numeric range paths without preconverting.
bool explicitApplyTextProfile(const qt::String& value)
{
    const auto& text = value.u16();
    if (text.empty() || text.size() > 21) return false;
    std::size_t at = 0;
    if (text[at] == u'+' || text[at] == u'-') ++at;
    int digits = 0;
    bool point = false;
    while (at < text.size() && text[at] != u'e' && text[at] != u'E') {
        const char16_t unit = text[at++];
        if (unit == u'.') {
            if (point) return false;
            point = true;
        } else {
            if (unit < u'0' || unit > u'9' || ++digits > 15) return false;
        }
    }
    if (digits == 0) return false;
    if (at == text.size()) return true;
    ++at; // exponent marker
    if (at < text.size() && (text[at] == u'+' || text[at] == u'-')) ++at;
    int exponentDigits = 0, exponent = 0;
    while (at < text.size()) {
        const char16_t unit = text[at++];
        if (unit < u'0' || unit > u'9' || ++exponentDigits > 2) return false;
        exponent = exponent * 10 + (unit - u'0');
    }
    return exponentDigits != 0 && exponent <= 32;
}

bool writesOperationNstart(const Operation& operation)
{
    const auto* cls = opBase(operation); //140054e90
    return cls && cls->writesDimNstart();
}

void updateMode(SimControl& sc) // 0x1408e00d0
{
    sc.smode = sc.trans[0] == 0 ? 1 : (sc.trans[1] == 0 ? 2 : 3);
}

// QString::sprintf("%.6g", value), not a rounding operation on the model.
qt::String convergenceText(double value)
{
    return qt::number(value, 'g', 6);
}

ValidatedText numericText(double value)
{
    return {qt::number(value, 'g', 6), ValidatorState::Acceptable};
}

bool assignValidated(double& destination, const ValidatedText& input)
{
    if (input.state == ValidatorState::Invalid) return false;
    destination = qt::toDouble(input.text);
    return true;
}

bool plainConstantCurve(const def::model::DYNARY& curve)
{
    if (curve.ndim != 0 || curve.u68 || curve.values ||
        (curve.ncomp != 0 && curve.ncomp != 1)) return false;
    for (int i = 0; i < 3; ++i)
        if (curve.n[i] != 0 || curve.axis[i]) return false;
    return true;
}

bool constantStepCurve(const def::model::DYNARY& curve)
{
    return curve.type == 0x80 && plainConstantCurve(curve);
}

struct StepCurveRange { std::uintptr_t begin, end; };
bool stepCurveRange(const void* pointer, std::size_t bytes, StepCurveRange& range)
{
    const auto address = reinterpret_cast<std::uintptr_t>(pointer);
    if (!pointer || bytes > (std::numeric_limits<std::uintptr_t>::max)() - address) return false;
    range = {address, address + bytes};
    return true;
}
bool stepCurveOverlap(const StepCurveRange& a, const StepCurveRange& b)
{ return a.begin < b.end && b.begin < a.end; }

bool tableStepCurve(const def::model::DYNARY& curve)
{
    // Candidate owned-table/resource profile, not native table validation.
    if (curve.type != 0x80 || curve.ndim != 1 || curve.ncomp != 1 ||
        (curve.var[0] != 1 && curve.var[0] != 2) || curve.n[0] <= 0 || curve.n[0] > 4096 ||
        curve.n[1] || curve.n[2] || curve.axis[1] || curve.axis[2] || curve.u68) return false;
    const auto bytes = static_cast<std::size_t>(curve.n[0]) * sizeof(double);
    StepCurveRange object{}, axis{}, values{};
    return stepCurveRange(&curve, sizeof(curve), object) &&
        stepCurveRange(curve.axis[0], bytes, axis) && stepCurveRange(curve.values, bytes, values) &&
        axis.begin % alignof(double) == 0 && values.begin % alignof(double) == 0 &&
        !stepCurveOverlap(object, axis) && !stepCurveOverlap(object, values) && !stepCurveOverlap(axis, values);
}
bool supportedStepCurve(const def::model::DYNARY& curve)
{ return constantStepCurve(curve) || tableStepCurve(curve); }

bool independentStepCurves(const def::model::DYNARY& source, const def::model::DYNARY& destination)
{
    std::array<StepCurveRange, 3> from{}, to{};
    auto ranges = [](const def::model::DYNARY& curve, std::array<StepCurveRange, 3>& out) {
        if (!stepCurveRange(&curve, sizeof(curve), out[0])) return std::size_t{0};
        if (curve.ndim == 0) return std::size_t{1};
        const auto bytes = static_cast<std::size_t>(curve.n[0]) * sizeof(double);
        if (!stepCurveRange(curve.axis[0], bytes, out[1]) || !stepCurveRange(curve.values, bytes, out[2]))
            return std::size_t{0};
        return std::size_t{3};
    };
    const auto fromCount = ranges(source, from), toCount = ranges(destination, to);
    if (!fromCount || !toCount) return false;
    for (std::size_t i = 0; i < fromCount; ++i)
        for (std::size_t j = 0; j < toCount; ++j)
            if (stepCurveOverlap(from[i], to[j])) return false;
    return true;
}

bool environmentCurveProfile(const def::model::DYNARY& curve)
{
    if (plainConstantCurve(curve)) return true;
    if (curve.u68 || curve.ndim < 1 || curve.ndim > 3 || curve.ncomp < 1)
        return false;
    std::int64_t count = curve.ncomp;
    for (int i = 0; i < 3; ++i) {
        if (i >= curve.ndim) {
            if (curve.n[i] != 0 || curve.axis[i]) return false;
            continue;
        }
        if (curve.n[i] < 0 || (curve.n[i] > 0) != (curve.axis[i] != nullptr))
            return false;
        count *= curve.n[i];
        if (count > (std::numeric_limits<std::int32_t>::max)()) return false;
    }
    return (count > 0) == (curve.values != nullptr);
}

bool acvcofCurveProfile(const def::model::DYNARY& curve)
{
    if (environmentCurveProfile(curve)) return true;
    // Selection0 changes live ndim without clearing counts/arrays. Subsequent
    // copies retain the counts, allocate no axes, and copy ncomp values for
    // multicomponent curves or types B5/A7/A8/AA/AB (otherwise zero values).
    // A null source values pointer leaves that allocation zero-filled. Keep
    // this reached shape, including positive counts with null axes, without
    // repairing metadata or admitting the zero-count borrowed-axis marker.
    if (curve.ndim != 0 || curve.ncomp < 1 || curve.u68) return false;
    for (int i = 0; i < 3; ++i)
        if (curve.n[i] < 0 || (curve.n[i] == 0 && curve.axis[i])) return false;
    return true;
}

bool environmentOwnersMatch(const EnvironmentCurveTemporaries& owners,
    const std::array<def::model::DYNARY*, 4>& live)
{
    for (std::size_t i = 0; i < owners.curves.size(); ++i) {
        if (!owners.curves[i]) return false;
        for (const auto* curve : live) if (owners.curves[i].get() == curve) return false;
        for (std::size_t j = 0; j < i; ++j)
            if (owners.curves[i].get() == owners.curves[j].get()) return false;
    }
    return true;
}

// @src140ec487e and140ec4970. The original suppresses the DiffBnd callback
// while checking ID1, so neither this display fill nor its repeated invocation
// frees/replaces TIMEID. The tables are cleared on every invocation.
std::optional<EnvironmentTimeIdState> fillEnvironmentTimeId(
    const def::model::DYNARY& table, bool previousChecked)
{
    EnvironmentTimeIdState form;
    form.differenceBoundaryChecked = previousChecked;
    if (table.n[0] <= 0) return form;
    if (!table.axis[0]) return std::nullopt;
    for (std::int32_t row = 0; row < table.n[0]; ++row) {
        if (table.axis[0][row] == 1.0) {
            form.differenceBoundaryChecked = true;
            break;
        }
    }
    for (std::int32_t row = 0; row < table.n[0]; ++row) {
        const double id = table.axis[0][row];
        if (id != 2.0 && id != 3.0 && id != 4.0) continue;
        // Reconstruction boundary, not original input validation: original
        // indexes int32 row*ncomp and reads raw pointers. Bind only a known
        // logical allocation (GUI allocator14089b840/count14089bc90), without
        // reproducing host UB or assuming extra allocator padding exists.
        if (table.ndim != 1 || table.ncomp <= 0 || !table.values)
            return std::nullopt;
        const auto count = static_cast<std::int64_t>(table.n[0]) * table.ncomp;
        if (count > (std::numeric_limits<std::int32_t>::max)()) return std::nullopt;
        const auto first = static_cast<std::int64_t>(row) * table.ncomp;
        const auto last = first + (id == 2.0 ? 1 : 0);
        if (last >= count) return std::nullopt;
        // ID2/ncomp1 intentionally reaches the next row's first value when
        // still allocated. Requiring ncomp>=2 would repair original behavior.
        if (!std::isfinite(table.values[first]) || !std::isfinite(table.values[last]))
            return std::nullopt;
        if (id == 2.0)
            form.betweenTemperatureRows.push_back({qt::number(table.values[first], 'g', 6),
                qt::number(table.values[last], 'g', 6)});
        else if (id == 3.0)
            form.belowTemperatureRows.push_back(qt::number(table.values[first], 'g', 6));
        else
            form.aboveTemperatureRows.push_back(qt::number(table.values[first], 'g', 6));
    }
    return form;
}

template <class T> T& objectField(LLOBJ_DATA* object, std::size_t offset)
{
    return *reinterpret_cast<T*>(reinterpret_cast<std::byte*>(object) + offset);
}

// 0x140eb6310 scans positions but returns position+1, then its callers look
// up that number. Keep this distinction for noncontiguous numbered models.
int rotationObject(GuiModel& model)
{
    for (int i = 1; i <= model.objectCount(); ++i) {
        auto* object = model.objectAt(i);
        if (object && (objectField<double>(object, 0x29e8) != 0.0 ||
            objectField<double>(object, 0x29f0) != 0.0 ||
            objectField<double>(object, 0x29f8) != 0.0)) return i;
    }
    return 1;
}

bool existingMdsSelection(const GuiModel& model, int number)
{
    for (int i = 1; i <= model.objectCount(); ++i) {
        auto* object = model.objectAt(i);
        if (object && objectField<int>(object, 0xc) != 7 &&
            objectField<int>(object, 8) == number) return true;
    }
    return false;
}

bool stoppingWidgetProfile(const StoppingWidgetState& widgets)
{
    for (const auto& reference : widgets.references)
        if (reference.selectedObject < 0 || reference.node < 0 || reference.node > 999999 ||
            (reference.nodeChecked && reference.coordinateChecked)) return false;
    return true;
}

void checkMdsCoordinate(StoppingReferenceControls& reference)
{
    // Native autoexclusive setChecked unchecks the old peer before emitting
    // coordinate.toggled(true). Equal-state setters do not emit either route.
    if (reference.coordinateChecked) return;
    reference.coordinateChecked = true;
    if (reference.nodeChecked) {
        reference.nodeChecked = false;
        reference.nodeSpinEnabled = false;
    }
    reference.coordinatesEnabled = true;
}

void checkMdsNode(StoppingReferenceControls& reference)
{
    // setChecked is permitted on a disabled node radio. In mode0 this can
    // reverse eb6440's coordinate selection without re-enabling the radio.
    if (reference.nodeChecked) return;
    reference.nodeChecked = true;
    if (reference.coordinateChecked) {
        reference.coordinateChecked = false;
        reference.coordinatesEnabled = false;
    }
    reference.nodeSpinEnabled = true;
}

bool stopFill(StoppingFillReport& report, StoppingFillIssue issue)
{
    report.issue = issue;
    return false;
}

// Const collection access must not perform the mutable getter's COW again.
// Only the constructor's nonzero-ID key0 methods qualify this new sync return.
bool stoppingCanonicalNodeVariables(const def::model::Cls_14017ef20& set)
{
    using namespace def::model;
    auto* collection = const_cast<Cls_14017ef20*>(&set);
    constexpr std::size_t count = sizeof(Cls_14017ef20::kRegs) / sizeof(Cls_14017ef20::kRegs[0]);
    if (ext::fun_14032a140(collection) != static_cast<std::int32_t>(count)) return false;
    for (std::size_t i = 0; i < count; ++i) {
        const auto* variable = ext::fun_14032a160(collection, static_cast<std::int32_t>(i));
        const auto& expected = Cls_14017ef20::kRegs[i];
        if (!variable || variable->id != expected.id) return false;
        if (expected.ctor == 0x14032a990) {
            if (typeid(*variable) != typeid(StateVarNode)) return false;
        } else if (expected.ctor == 0x14032aa20) {
            if (typeid(*variable) != typeid(StateVarNodePost)) return false;
        } else {
            return false;
        }
    }
    return true;
}

// @src140854af0 ->854200 ->b3e8e0 ->82dca0. The shared native accessor has
// an always-true sync predicate; the GUI's live predicate is different.
bool stoppingCoordinates(def::model::Cls_14017ef20& set, GuiModel& model,
    StoppingFillReport& report, double*& coordinates)
{
    ++report.coordinateGetterCalls;
    auto* owner = static_cast<def::model::DB_STEP_DATA*>(set.u130);
    if (owner && owner != model.step.get())
        return stopFill(report, StoppingFillIssue::CoordinateOwner);
    if (set.n > 0) {
        if (!owner) return stopFill(report, StoppingFillIssue::CoordinateOwner);
        if (!set.u008) {
            const auto count = static_cast<std::int64_t>(set.n) * owner->u15f8[0];
            // Bound the original signed32 product, without invoking host UB
            // or silently assuming allocator padding supplies extra elements.
            if (count < 0 || count > (std::numeric_limits<std::int32_t>::max)())
                return stopFill(report, StoppingFillIssue::CoordinateExtent);
            ++report.coordinateAllocationAttempts;
            set.u008 = def::model::bc::calloc_doubles(static_cast<std::int32_t>(count));
        }
        ++report.stateVariableDispatches;
        def::model::llobj::ext::f140329e60(&set, set.n, owner->u15f8, 0);
    }
    // Read AFTER allocation and actual StateVar dispatch. Key0 still detaches
    // a shared pointer collection even though the28 native IDs do not match.
    if (set.u130 != owner || (owner && owner != model.step.get()))
        return stopFill(report, StoppingFillIssue::CoordinateOwner);
    if (owner && owner->u1b58 == 0 && owner->dimension == 3 && owner->step_version >= 6000) {
        if (set.n <= 0 || !stoppingCanonicalNodeVariables(set))
            return stopFill(report, StoppingFillIssue::CoordinateSync);
        //14082d7d0 selects actual module tables before its id0x0b cache test.
        // Current dimension3 preserves altered cells. A miss retains its
        // allocation/COW/table/timer prefix before bounded directory admission.
        const auto dimension = owner->dimension;
        def::model::select_index_tables(&dimension);
        const auto marIndex = def::model::g_mar_index[3];
        if (marIndex < 0 || marIndex >= 0x8c || def::model::g_a_index[128] <= 0x0b)
            return stopFill(report, StoppingFillIssue::CoordinateSync);
        if (owner->u26c4[0x0b] == 0) {
            auto& timer = def::model::g_timer_read_obj_node_state_variable;
            timer.start("Start");
            timer.checkpoint("ReadSingleAArrayFromDB", nullptr);
            // The live serialized Project scope owns the handle; neither raw
            // model.db nor actual S.db alone supplies a lifetime or extent.
            const auto* project = currentProject();
            const auto lease = project ? project->dataDb : nullptr;
            if (!lease || lease.get() != model.db)
                return stopFill(report, StoppingFillIssue::CoordinateSync);
            auto* reader = lease->reader.get();
            if (!reader || reader != owner->db)
                return stopFill(report, StoppingFillIssue::CoordinateSync);
            namespace receipts = def::model::directory_receipts;
            const auto receipt = receipts::current(lease.get(), *reader);
            if (!receipt || owner->u740 < 0 ||
                static_cast<std::size_t>(owner->u740) >= receipt->elementCount ||
                !receipts::isCurrent(*receipt))
                return stopFill(report, StoppingFillIssue::CoordinateSync);
            const auto* records = static_cast<const def::model::DbDirRecord*>(receipt->allocation);
            // A low record version would change a later fresh getter's gate.
            // It needs a Stopping continuation owner before query admission.
            if (records[owner->u740].version < 6000)
                return stopFill(report, StoppingFillIssue::CoordinateSync);
            std::int32_t hsize = 0, marSize = 0, aSize = 0, recordVersion = 0;
            (void)def::dbio::dbread::step_dir_sizes(owner, reader, owner->u740,
                &hsize, &marSize, &aSize, &recordVersion);
            // The owned post-query cut publishes a fresh A buffer and stops
            // before the selected-length/cache branch. The query may update
            // step_version, so all admission below is deliberately afterward.
            constexpr auto maxCount = (std::numeric_limits<std::int32_t>::max)() - 2;
            if (owner->dimension == 3 && owner->u1b58 == 0 && owner->step_version >= 6000 &&
                owner->u1b68 > 0 && owner->u1b6c > 0 && owner->u26c4[0x0b] == 0 &&
                owner->u1b78 == nullptr && owner->u2b24 >= 0 && owner->u2b24 <= maxCount) {
                const auto count = static_cast<std::int32_t>(owner->u2b24 + 2);
                owner->u1b78 = def::model::bc::calloc_doubles(count);
            }
            // Selected length/cache publication and the arrays timer remain
            // unopened; a retry with published A still reaches this refusal.
            return stopFill(report, StoppingFillIssue::CoordinateSync);
        }
        // Native cached AL=false is ignored by outer which0, even >=11000.
        // Its unused MAR address calculation reads no MAR/A pointee. Return
        // N+8 without another dispatch or a fabricated body/DB requirement.
    }
    coordinates = set.u008;
    return true;
}

enum class NodeLocation { Available, Unavailable, Unsupported };
NodeLocation stoppingNodeLocation(GuiModel& model, int number, int node,
    std::array<double, 3>& location, StoppingFillReport& report)
{
    auto* object = model.object(number);
    if (!object || node <= 0 || node > objectField<int>(object, 0x880))
        return NodeLocation::Unavailable;
    auto& set = objectField<def::model::Cls_14017ef20>(object, 0x8a0);
    double* coordinates = nullptr;
    if (!stoppingCoordinates(set, model, report, coordinates)) return NodeLocation::Unsupported;
    if (!coordinates) return NodeLocation::Unavailable;
    // eb59a0 calls the getter again for EACH axis, not once per vector.
    for (int axis = 0; axis < 3; ++axis) {
        report.axis = axis;
        if (!stoppingCoordinates(set, model, report, coordinates)) return NodeLocation::Unsupported;
        const auto count = static_cast<std::int64_t>(set.n) * model.step->u15f8[0];
        const auto index = static_cast<std::int64_t>(node - 1) * model.step->u15f8[0] + axis;
        if (!coordinates || set.u130 != model.step.get() || set.n < 0 || count < 0 ||
            count > (std::numeric_limits<std::int32_t>::max)() || index < 0 || index >= count ||
            index > (std::numeric_limits<std::int32_t>::max)()) {
            stopFill(report, StoppingFillIssue::CoordinateExtent);
            return NodeLocation::Unsupported;
        }
        location[axis] = coordinates[index];
    }
    return NodeLocation::Available;
}

bool stoppingNodeChanged(StoppingPage& form, GuiModel& model, int sender,
    StoppingFillReport& report)
{
    report.stage = StoppingFillStage::NodeCallback;
    ++report.nodeCallbacks;
    auto& reference = form.widgets.references[sender];
    std::array<double, 3> location{};
    const auto found = stoppingNodeLocation(model, reference.selectedObject, reference.node, location, report);
    if (found == NodeLocation::Unsupported) return false;
    if (found == NodeLocation::Unavailable) return true;
    // ecda10 uses %.6g, before ebe100 later replaces these with stored %.3f.
    for (int axis = 0; axis < 3; ++axis) {
        report.axis = axis;
        if (!std::isfinite(location[axis])) return stopFill(report, StoppingFillIssue::NonfiniteValue);
        reference.coordinates[axis] = qt::number(location[axis], 'g', 6);
    }
    // ece5c0 ->eb5b00: first reference then second, short-circuit on a failed
    // node lookup. Both vectors are local; eb6180's None-zeroing cannot escape.
    std::array<std::array<double, 3>, 2> vectors{};
    for (int side = 0; side < 2; ++side) {
        report.reference = side;
        const auto& current = form.widgets.references[side];
        if (current.nodeChecked) {
            const auto result = stoppingNodeLocation(model, current.selectedObject, current.node, vectors[side], report);
            if (result == NodeLocation::Unsupported) return false;
            if (result == NodeLocation::Unavailable) return true;
        } else {
            for (int axis = 0; axis < 3; ++axis) vectors[side][axis] = qt::toDouble(current.coordinates[axis]);
        }
    }
    if (!form.widgets.references[0].selectedObject || !form.widgets.references[1].selectedObject) {
        vectors[0].fill(0.0); vectors[1].fill(0.0);
        ++report.referenceVectorZeroings;
    }
    return true;
}

bool fillStoppingReferences(StoppingPage& form, const SimControl& sc, GuiModel& model,
    StoppingFillReport& report)
{
    report.stage = StoppingFillStage::ReferenceChoices;
    const bool selected = existingMdsSelection(model, sc.mdsobj[0]) ||
        existingMdsSelection(model, sc.mdsobj[1]);
    form.referenceChoices.clear();
    for (auto& reference : form.widgets.references) reference.selectedObject = 0;
    if (selected && form.widgets.labelProfile != StoppingLabelProfile::AsciiNoActiveTranslator)
        return stopFill(report, StoppingFillIssue::UnknownLabelProfile);
    form.referenceChoices.push_back(0);
    for (int i = 1; i <= model.objectCount(); ++i) {
        auto* object = model.objectAt(i);
        if (!object || objectField<int>(object, 0xc) == 7) continue;
        const int number = objectField<int>(object, 8);
        // The selected profile binds native label format/parse without
        // unchecked strcpy overflow or an invented non-ASCII codec.
        if (selected && (number < 1 || !asciiTerminated(object->name) ||
            std::find(form.referenceChoices.begin(), form.referenceChoices.end(), number) !=
                form.referenceChoices.end()))
            return stopFill(report, StoppingFillIssue::UnsupportedObjectLabel);
        form.referenceChoices.push_back(number);
    }
    // Native selection occurs only after both complete lists are populated.
    for (int side = 0; side < 2; ++side)
        if (std::find(form.referenceChoices.begin(), form.referenceChoices.end(), sc.mdsobj[side]) !=
            form.referenceChoices.end()) form.widgets.references[side].selectedObject = sc.mdsobj[side];
    // eb6440 runs for both selectors before either ebe100 invocation.
    report.stage = StoppingFillStage::ReferenceAvailability;
    for (int side = 0; side < 2; ++side) {
        report.reference = side;
        auto& reference = form.widgets.references[side];
        auto* object = model.object(reference.selectedObject);
        if (!object) {
            if (reference.selectedObject > 0) return stopFill(report, StoppingFillIssue::MissingCurrentObject);
            continue;
        }
        if (objectField<int>(object, 0xc) != 5) {
            const bool hasMesh = objectField<int>(object, 0x87c) != 0;
            if (!hasMesh) checkMdsCoordinate(reference);
            reference.nodeEnabled = hasMesh;
        }
    }
    form.referenceFillOrder = {-1, -1};
    for (int invocation = 0; invocation < 2; ++invocation) {
        report.stage = StoppingFillStage::ReferenceFill;
        report.invocation = invocation;
        int side = -1;
        if (invocation == 0 && form.widgets.references[0].selectedObject > 0) side = 0;
        else if (form.widgets.references[1].selectedObject > 0) side = 1;
        form.referenceFillOrder[invocation] = side;
        report.reference = side;
        report.axis = -1;
        if (side < 0) continue;
        auto& reference = form.widgets.references[side];
        auto* object = model.object(reference.selectedObject);
        const int mode = objectField<int>(object, 0x1ec8);
        if (mode != 0 && mode != 1 && mode != 2)
            return stopFill(report, StoppingFillIssue::UnsupportedMdsMode);
        if (mode == 1) checkMdsCoordinate(reference);
        else checkMdsNode(reference);
        if (mode == 2) {
            const int node = clampSpin(objectField<int>(object, 0x1ee8), 0, 999999);
            if (node != reference.node) {
                // The changed bounded value is retained BEFORE the callback,
                // including if a later source dependency remains unsupported.
                reference.node = node;
                if (!stoppingNodeChanged(form, model, side, report)) return false;
            }
        }
        // A rejected node callback returns to ebe100, which still overwrites
        // its coordinate texts. Modes0/1 never set the remembered spin.
        report.stage = StoppingFillStage::ReferenceFill;
        report.reference = side;
        for (int axis = 0; axis < 3; ++axis) {
            report.axis = axis;
            if (!std::isfinite(objectField<double>(object, 0x1ed0 + axis * 8)))
                return stopFill(report, StoppingFillIssue::NonfiniteValue);
            reference.coordinates[axis] = qt::number(
                objectField<double>(object, 0x1ed0 + axis * 8), 'f', 3);
        }
    }
    return true;
}

void applyStoppingReferences(GuiModel& model, const StoppingWidgetState& widgets)
{
    const bool bothNodesDisabled = !widgets.references[0].nodeEnabled &&
        !widgets.references[1].nodeEnabled;
    for (const auto& reference : widgets.references) {
        auto* object = model.object(reference.selectedObject);
        if (!object) continue; // eb6da0 returns only from this reference.
        objectField<int>(object, 0x1ec8) = reference.nodeChecked ? 2 :
            (reference.coordinateChecked ? 1 : 0);
        objectField<int>(object, 0x1ee8) = bothNodesDisabled ? 0 : reference.node;
        for (int axis = 0; axis < 3; ++axis)
            objectField<double>(object, 0x1ed0 + axis * 8) = qt::toDouble(reference.coordinates[axis]);
    }
}

bool advancedTimeProfile(const GuiModel& model)
{
    // 1408bfe60 skips missing/kind7 objects. For movementkind6, 140892370
    // skips 140874630 when 14089baf0 finds an all-zero path. Its ndim0 branch
    // compares the actual embedded constant with 0 (both signs, never NaN).
    // Also admit its bounded type7a 1D/four-component table branch below.
    // Other table profiles and nonzero curve evaluation remain unrecovered.
    for (int i = 1; i <= model.objectCount(); ++i) {
        auto* object = model.objectAt(i);
        if (object && objectField<int>(object, 0xc) != 7 &&
            objectField<int>(object, 0x21c8) == 6) {
            const auto& path = objectField<def::model::DYNARY>(object, 0x25a8);
            if (path.ndim == 0) {
                if (path.value != 0.0) return false;
                continue;
            }
            // Bound the actual live array shape before source-ordered reads:
            // 14089baf0 checks all axes, then 14089bc90 yields 4*n0 values.
            // Bound that signed32 product before any array read.
            if (path.type != 0x7a || path.ndim != 1 || path.n[0] < 0 ||
                path.n[0] > (std::numeric_limits<std::int32_t>::max)() / 4 ||
                path.ncomp != 4 || path.u68 ||
                path.n[1] != 0 || path.n[2] != 0 || path.axis[1] || path.axis[2]) return false;
            if (path.n[0] == 0) {
                // Both native loops skip. Keep nonnull/borrowed payloads guarded.
                if (path.axis[0] || path.values) return false;
                continue;
            }
            if (!path.axis[0] || !path.values) return false;
            for (int point = 0; point < path.n[0]; ++point)
                if (path.axis[0][point] != 0.0) return false;
            const int valueCount = 4 * path.n[0];
            for (int value = 0; value < valueCount; ++value)
                if (path.values[value] != 0.0) return false;
        }
    }
    return true;
}

bool elementNodeRadio(int value) { return value == 0 || value == 2 || value == 3; }

bool advancedApplyProfile(SimControl& sc, GuiModel& model, const AdvancedPage& form)
{
    if (!model.step || model.step->dimension != 3 || model.simCtl() != &sc ||
        !advancedTimeProfile(model) || (form.timeRadio != 0 && form.timeRadio != 1)) return false;
    for (int value : form.elementNodeOptions) if (!elementNodeRadio(value)) return false;
    for (const auto& text : form.userStrings) {
        if (text.size() > 80) return false;
        for (char16_t c : text.u16()) if (c < 0x20 || c > 0x7e) return false;
    }
    return true;
}

// 14082aaa0 ids22/3f ->14082acc0/14082ad80. All nonnull objects are
// visited (including kind7), only when the corresponding width changed.
void resizeRecoveryOutputs(GuiModel& model)
{
    const int nodeWidth = model.simCtl()->timeid.n[0]; // step+11c4 (TIMEID, not NRECID)
    const int elementWidth = model.simCtl()->erecid.n[0]; // step+123c
    auto& step = *model.step;
    constexpr int nodeIndex = (0x1680 - 0x15f8) / 4;
    constexpr int elementIndex = (0x16f4 - 0x15f8) / 4;
    if (step.u15f8[nodeIndex] != nodeWidth) {
        step.u15f8[nodeIndex] = nodeWidth;
        for (int i = 1; i <= model.objectCount(); ++i)
            if (auto* object = model.objectAt(i))
                def::model::svkw::set8a0_resize(
                    reinterpret_cast<std::byte*>(object) + 0x8a0, 0x22, nodeWidth);
    }
    if (step.u15f8[elementIndex] != elementWidth) {
        step.u15f8[elementIndex] = elementWidth;
        for (int i = 1; i <= model.objectCount(); ++i)
            if (auto* object = model.objectAt(i))
                def::model::svkw::set9e0_resize(
                    reinterpret_cast<std::byte*>(object) + 0x9e0, 0x3f, elementWidth);
    }
}
} // namespace

namespace detail {
bool stepSizeCurveCopyComplete(const def::model::DYNARY& source, const def::model::DYNARY& copy)
{
    if (constantStepCurve(source)) return true; // Preserve the previous constant-copy contract.
    if (!tableStepCurve(source) || !tableStepCurve(copy) || !independentStepCurves(source, copy) ||
        source.n[0] != copy.n[0] || source.u50 != copy.u50 || source.u54 != copy.u54 ||
        std::memcmp(&source.value, &copy.value, sizeof(double)) != 0) return false;
    for (int i = 0; i < 3; ++i)
        if (source.var[i] != copy.var[i] || source.n[i] != copy.n[i]) return false;
    const auto bytes = static_cast<std::size_t>(source.n[0]) * sizeof(double);
    // u58 is intentionally separate: Apply restores its selected mode before this check.
    return std::memcmp(source.axis[0], copy.axis[0], bytes) == 0 &&
        std::memcmp(source.values, copy.values, bytes) == 0;
}

// Private bridge for the constructor's existing StepSize copy positions.
// No installed header/API; null means a candidate copy/profile refusal.
std::shared_ptr<const def::model::DYNARY> copyStepSizeCurve(const def::model::DYNARY& source)
{
    if (!supportedStepCurve(source)) return {};
    auto copy = std::make_shared<def::model::DYNARY>();
    def::model::dynary_copy(copy.get(), &source);
    if (source.ndim != 0 && (!stepSizeCurveCopyComplete(source, *copy) || source.u58 != copy->u58)) return {};
    return copy;
}
}

EnvironmentCurveTemporaries::EnvironmentCurveTemporaries()
{
    for (auto& curve : curves) curve = std::make_shared<def::model::DYNARY>();
}

GeneralContext generalContextForMo(const Project& project, int dimension, GeneralCapabilityState capabilities)
{
    return {5, dimension, {true, true, capabilities.trans3Enabled,
        capabilities.trans4Trans5HeatEnabled, capabilities.trans4Trans5HeatEnabled,
        capabilities.trans4Trans5HeatEnabled}, false, false, &project};
}

std::optional<GeneralContext> generalContextForMoAvailability(const Project& project,
    int dimension, const MoFeatureAvailability& availability)
{
    // Original General140eb9630: fae10(MIC3) OR fab10(WZD_HEAT_TREAT3),
    // followed by three distinct faeb0(HEATING) availability observations.
    if (dimension != 3 || !availability.microstructure3D) return std::nullopt;
    bool trans3 = *availability.microstructure3D > 0;
    if (!trans3) {
        if (!availability.wizardHeatTreat3D) return std::nullopt;
        trans3 = *availability.wizardHeatTreat3D > 0;
    }
    for (const auto& result : availability.heating)
        if (!result) return std::nullopt;
    return GeneralContext{5, 3, {true, true, trans3,
        *availability.heating[2] > 0, *availability.heating[1] > 0,
        *availability.heating[0] > 0}, false, false, &project};
}

std::optional<bool> pressureFieldsForStartupHeating(const StartupHeatingFeatures& features)
{
    if ((features.twoD && *features.twoD != 0) ||
        (features.threeD && *features.threeD != 0)) return true;
    if (features.twoD && features.threeD) return false;
    return std::nullopt;
}

// @src 0x140eb9630, 0x140ebaa30. Guarded 3D procType1/procType5 branches.
std::optional<GeneralPage> fillGeneral(SimControl& sc, const GeneralContext& context)
{
    if ((context.applicationMode != 1 && context.applicationMode != 5) || context.dimension != 3 ||
        (context.applicationMode == 5 && !context.moProject) ||
        !asciiTerminated(sc.title) || !asciiTerminated(sc.oprnam) ||
        (sc.unit != 1 && sc.unit != 2) ||
        (sc.stype != 1 && sc.stype != 3 && sc.stype != 4 &&
         sc.stype != 5 && sc.stype != 6 && sc.stype != 7)) return std::nullopt;
    GeneralPage form;
    form.title = qt::String::latin1(sc.title);
    // setupUi140eddf2f sets ui138.maxLength20. General fill140eb9630
    // blocks signals while setText clips the widget, leaving SC unchanged.
    // This profile is ASCII, so20 characters are exactly20 UTF16 code units.
    form.operationName = qt::String::latin1(std::string_view(
        sc.oprnam, (std::min)(std::strlen(sc.oprnam), std::size_t{20})));
    // MO hides158 and never assigns it from SC. Its fresh setupUi minimum is1;
    // General Save checks visibility before touching SC.meshno.
    form.meshNumber = context.applicationMode == 5 ? 1 : clampSpin(sc.meshno, 1, 9999);
    form.unit = sc.unit;
    form.simulationType = sc.stype;
    for (int i = 0; i < 5; ++i) form.processChecked[i] = sc.trans[i] != 0;
    form.processChecked[5] = sc.htmthd != 0;
    form.processEnabled = context.processEnabled;
    form.heatMethod2 = sc.trans[2] == 2;
    // General fills three choices and initially selects index 0. Environment
    // fill can subsequently change this combo; callers must preserve that order.
    form.heatMethodCombo = 0;
    form.preserveProcessFlags = context.preserveProcessFlags;
    form.meshNumberEnabled = context.applicationMode == 5 ? false : context.meshNumberEnabled;
    form.moProject = context.applicationMode == 5 ? context.moProject : nullptr;
    updateMode(sc);
    return form;
}

bool applyGeneral(SimControl& sc, const GeneralPage& form)
{
    // Guarded ASCII source strings fit their original fixed buffers. This helper
    // applies initial widgets only; user-edited/oversized strings are not inputs.
    const auto title = qt::toAscii(form.title);
    const auto name = qt::toAscii(form.operationName);
    if (title.size() >= sizeof sc.title || name.size() >= sizeof sc.oprnam)
        return false;
    std::strcpy(sc.title, title.c_str());
    std::strcpy(sc.oprnam, name.c_str());
    if (form.meshNumberEnabled) sc.meshno = form.meshNumber;
    sc.unit = form.unit; // original engine unit singleton is per-call in this port
    sc.stype = form.simulationType;
    bool preserveProcessFlags = form.preserveProcessFlags;
    if (form.moProject) {
        const auto* sub = activeSubProject(*form.moProject); //1400a7390
        preserveProcessFlags = sub && (sub->type == 2 || sub->type == 3); //140010330/360
    }
    if (!preserveProcessFlags) {
        for (int i = 0; i < 5; ++i)
            sc.trans[i] = form.processChecked[i] && form.processEnabled[i] ? 1 : 0;
        sc.htmthd = form.processChecked[5] && form.processEnabled[5] ? 1 : 0;
    }
    if (sc.trans[2] != 0 && form.heatMethod2) sc.trans[2] = 2;
    if (sc.htmthd != 0) sc.htmthd = form.heatMethodCombo + 1;
    updateMode(sc);
    return true;
}

// @src 0x140ebb110, 0x140ebb3f0, 0x140ebb770. MO is procType5, not DOE.
std::optional<StepCountPage> fillStepCount(const SimControl& sc, const StepCountContext& context)
{
    return fillStepCount(sc, context, false);
}

std::optional<StepCountPage> fillStepCount(const SimControl& sc, const StepCountContext& context,
    bool priorNstartChecked)
{
    if (context.applicationMode != 1 && (context.applicationMode != 5 || !context.mo))
        return std::nullopt;
    int startSource = sc.nstart;
    bool checked = priorNstartChecked;
    if (context.applicationMode == 5 && context.mo->operation &&
        !startsFromKey(*context.mo->operation)) {
        // Hidden key/null-operation branches do not call setChecked. The
        // nonwriting-class branch explicitly clears it with signals blocked.
        checked = writesOperationNstart(*context.mo->operation) &&
            context.mo->operation->nstartGiven != 0;
        if (checked) startSource = context.mo->operation->startStepNo;
    }
    // Explicit unsigned wrap retains the original 32-bit NEG/ADD behavior.
    // The positive test uses the selected source, but NEG uses SC.nstart even
    // if that selected positive value came from the operation override.
    const int start = startSource > 0
        ? static_cast<int>(0u - (static_cast<unsigned>(sc.nstart) + 1u)) : startSource;
    const bool selected = std::find(context.primaryDieObjects.begin(),
        context.primaryDieObjects.end(), sc.pdie1) != context.primaryDieObjects.end();
    StepCountPage form{clampSpin(start, -999999, 999999),
                         clampSpin(sc.nstep, 0, 999999),
                         clampSpin(sc.stpinc, 1, 999999), selected ? sc.pdie1 : 0};
    if (context.applicationMode == 5) form.mo = context.mo;
    form.nstartChecked = checked;
    return form;
}

void applyStepCount(SimControl& sc, const StepCountPage& form)
{
    sc.nstart = form.start;
    if (form.mo && form.mo->operation) {
        auto& operation = *form.mo->operation;
        // Original Save re-evaluates startsFromKey instead of freezing it at
        // Open. The checkbox itself is remembered widget state.
        if (startsFromKey(operation)) {
            operation.nstartGiven = 1;
            operation.startStepNo = form.start;
        } else if (!writesOperationNstart(operation)) {
            operation.nstartGiven = 0;
        } else {
            operation.nstartGiven = form.nstartChecked ? 1 : 0;
            if (form.nstartChecked) operation.startStepNo = form.start;
        }
    }
    sc.nstep = form.steps;
    sc.stpinc = form.increment;
    sc.pdie1 = form.primaryDie;
}

// @src 0x140ed01b0, reached from 0x140ec19f0 when the CG radio becomes checked.
void normalizeCgAtOpen(SimControl& sc)
{
    if (sc.solmtd != 2) return;
    if (sc.u2e8 != 4) {
        sc.u2e8 = 4;
        sc.solmtd_p[0] = 2.0; sc.solmtd_p[1] = 4.0;
        sc.solmtd_p[2] = 1.0; sc.solmtd_p[3] = 1.0;
    }
    const int first = integer(sc.solmtd_p[0]);
    if (first < 0 || first > 2) sc.solmtd_p[0] = 2.0;
    if (integer(sc.solmtd_p[1]) <= 0) sc.solmtd_p[1] = 1.0;
    const int fourth = integer(sc.solmtd_p[3]);
    if (fourth != 1 && fourth != 5) sc.solmtd_p[3] = 1.0;
}

// @src 0x140ec19f0 and CG callback. No explicit solver callback is assumed.
std::optional<IterationPage> fillIteration3D(SimControl& sc, int dimension, NumericLocale locale)
{
    if (locale != NumericLocale::KnownC || dimension != 3 ||
        (sc.solmtd != 0 && sc.solmtd != 1 && sc.solmtd != 2 &&
        sc.solmtd != 4 && sc.solmtd != 5 && sc.solmtd != 6) || sc.itrmth < 0 || sc.itrmth > 2 ||
        sc.solmtt < 0 || sc.solmtt > 5 || !std::isfinite(sc.cvgerr[0]) ||
        !std::isfinite(sc.cvgerr[1])) return std::nullopt;
    if (sc.solmtd == 6 && (!std::isfinite(sc.solmtd_p[0]) || !std::isfinite(sc.solmtd_p[1]) ||
        !exactSignedInt32(sc.solmtd_p[2]) || !exactSignedInt32(sc.solmtd_p[3]))) return std::nullopt;
    normalizeCgAtOpen(sc);
    IterationPage form;
    form.solver = sc.solmtd;
    form.explicitControlsVisible = sc.solmtd == 6;
    form.method = sc.itrmth;
    form.thermalSolver = sc.solmtt;
    form.cgFirstCombo = 0; form.cgFourthCombo = 0;
    if (sc.solmtd == 2) {
        form.cgFirstCombo = integer(sc.solmtd_p[0]);
        form.cgFourthCombo = integer(sc.solmtd_p[3]) == 1 ? 0 : 1;
        form.cgSecond = qt::number(integer(sc.solmtd_p[1]));
        form.cgThird = qt::number(integer(sc.solmtd_p[2]));
    } else if (sc.solmtd == 6) {
        // @src140ec2552/25be/2627/2690. Fixed four source doubles are read
        // independently of u2e8; Fill does not rewrite their raw bits/count.
        form.explicitParameterText[0] = qt::number(sc.solmtd_p[0], 'g', 6);
        form.explicitParameterText[1] = qt::number(sc.solmtd_p[1], 'g', 6);
        form.explicitParameterText[2] = qt::number(integer(sc.solmtd_p[2]));
        form.explicitParameterText[3] = qt::number(integer(sc.solmtd_p[3]));
    }
    for (int i = 0; i < 2; ++i)
        form.convergence[i] = {convergenceText(sc.cvgerr[i]), ValidatorState::Acceptable};
    form.deformationBandwidth = sc.defbwd != 0;
    form.thermalBandwidth = sc.tmpbwd != 0;
    form.deformationIterations = {qt::number(sc.itrmxd), ValidatorState::Acceptable};
    form.thermalIterations = {qt::number(sc.itrmxt), ValidatorState::Acceptable};
    return form;
}

// @src 0x140ec3130. Original validators are represented by explicit results,
// preserving Invalid versus Intermediate and the sequential partial writes.
ApplyResult applyIteration3D(SimControl& sc, const IterationPage& form)
{
    if ((form.solver != 0 && form.solver != 1 && form.solver != 2 &&
         form.solver != 4 && form.solver != 5 && form.solver != 6) || form.method < 0 || form.method > 2 ||
        form.thermalSolver < 0 || form.thermalSolver > 5)
        return ApplyResult::UnsupportedSnapshot;
    if (form.solver == 6)
        for (const auto& value : form.explicitParameterText)
            if (!value || !explicitApplyTextProfile(*value)) return ApplyResult::UnsupportedSnapshot;
    sc.u2e8 = 0;
    if (form.solver == 2) {
        sc.u2e8 = 4;
        sc.solmtd_p[0] = form.cgFirstCombo == 0 ? 0.0 :
                           (form.cgFirstCombo == 1 ? 1.0 : 2.0);
        sc.solmtd_p[1] = qt::toDouble(form.cgSecond);
        sc.solmtd_p[2] = qt::toDouble(form.cgThird);
        sc.solmtd_p[3] = form.cgFourthCombo == 0 ? 1.0 : 5.0;
    } else if (form.solver == 6) {
        // @src140ec38c9/3919/3983/39ed/3a57: all four Apply edits use
        // toDouble, including those formatted as integers by initial Fill.
        sc.u2e8 = 4;
        for (int i = 0; i < 4; ++i)
            sc.solmtd_p[i] = qt::toDouble(*form.explicitParameterText[i]);
    }
    sc.solmtd = form.solver;
    sc.itrmth = form.method;
    sc.solmtt = form.thermalSolver;
    if (form.convergence[0].state == ValidatorState::Invalid)
        return ApplyResult::ValidationStopped;
    sc.cvgerr[0] = qt::toDouble(form.convergence[0].text);
    if (form.convergence[1].state == ValidatorState::Invalid)
        return ApplyResult::ValidationStopped;
    sc.cvgerr[1] = qt::toDouble(form.convergence[1].text);
    sc.defbwd = form.deformationBandwidth ? 1 : 0;
    sc.tmpbwd = form.thermalBandwidth ? 1 : 0;
    sc.itrmxd = form.deformationIterations.state == ValidatorState::Invalid
        ? 200 : qt::toInt(form.deformationIterations.text);
    sc.itrmxt = form.thermalIterations.state == ValidatorState::Invalid
        ? 200 : qt::toInt(form.thermalIterations.text);
    return ApplyResult::Completed;
}

// @src0x140ebb950, callbacks0x140ecc770/eccaa0/ecc540, and the final
// constructor refresh0x140ecb340. Normal1D definitions1/2 are idempotent in ecc540.
std::optional<StepSizePage> fillStepSize3D(SimControl& sc, int dimension, NumericLocale locale)
{
    if (dimension != 3 || locale != NumericLocale::KnownC ||
        !supportedStepCurve(sc.dttype) || (sc.dttype.u58 != 0 && sc.dttype.u58 != 1) ||
        sc.stpdef < 1 || sc.stpdef > 3) return std::nullopt;
    const double values[]{sc.dtmax1, sc.dsmax, sc.demax, sc.dplen, sc.dtsub,
                          sc.dtpmax1, sc.dtpmax2_5[0], sc.dtpmax2_5[1], sc.slderr};
    for (double value : values) if (!std::isfinite(value)) return std::nullopt;
    StepSizePage form;
    form.modeRadio = sc.stpdef == 3 ? 2 : sc.dttype.u58;
    form.definitionRadio = sc.stpdef == 1 ? 1 : 2;
    // ecc770 always writes this, even without a Qt sender. The later backed-up
    // refresh copies the already-normalized model curve into the temporary.
    sc.dttype.u58 = form.modeRadio == 0 ? 0 : 1;
    auto curve = detail::copyStepSizeCurve(sc.dttype);
    if (!curve) return std::nullopt; // Keep the already reached mode normalization on refusal.
    form.curve = std::move(curve);
    form.timeMaximum = numericText(sc.dtmax1);
    form.displacementMaximum = numericText(sc.dsmax);
    form.strainMaximum = numericText(sc.demax);
    form.penetration = numericText(sc.dplen);
    form.substeps = numericText(sc.dtsub);
    form.maximumChanges = {numericText(sc.dtpmax1), numericText(sc.dtpmax2_5[0]),
                           numericText(sc.dtpmax2_5[1])};
    form.variableTimeMaximum = numericText(sc.dtmax1);
    form.variableMaximumChanges = form.maximumChanges;
    form.slidingError = numericText(sc.slderr);
    return form;
}

// @src0x140ebc1f0 3D branch. The mode store precedes validation. Each Invalid
// return retains earlier stores and leaves the temporary-curve copy unapplied.
ApplyResult applyStepSize3D(SimControl& sc, const StepSizePage& form)
{
    if (form.modeRadio < 0 || form.modeRadio > 2 ||
        (form.definitionRadio != 1 && form.definitionRadio != 2) ||
        !form.curve || !supportedStepCurve(*form.curve))
        return ApplyResult::UnsupportedSnapshot;
    const bool copyTable = form.curve->ndim != 0;
    if (copyTable && (!supportedStepCurve(sc.dttype) || !independentStepCurves(*form.curve, sc.dttype)))
        return ApplyResult::UnsupportedSnapshot; // Before mode/scalars: dynary_copy frees destination arrays.
    sc.dttype.u58 = form.modeRadio == 0 ? 0 : 1;
    if (sc.dttype.u58 == 1) {
        if (!assignValidated(sc.dtmax1, form.timeMaximum)) return ApplyResult::ValidationStopped;
        sc.dsmax = 0.0;
    } else {
        if (!assignValidated(sc.dsmax, form.displacementMaximum)) return ApplyResult::ValidationStopped;
        sc.dtmax1 = 0.0;
    }
    sc.stpdef = form.modeRadio == 2 ? 3 : form.definitionRadio;
    if (!assignValidated(sc.demax, form.strainMaximum)) return ApplyResult::ValidationStopped;
    if (!assignValidated(sc.dplen, form.penetration)) return ApplyResult::ValidationStopped;
    if (!assignValidated(sc.dtsub, form.substeps)) return ApplyResult::ValidationStopped;
    const auto& changes = form.modeRadio == 2 ? form.variableMaximumChanges : form.maximumChanges;
    if (form.modeRadio == 2 && !assignValidated(sc.dtmax1, form.variableTimeMaximum))
        return ApplyResult::ValidationStopped;
    if (!assignValidated(sc.dtpmax1, changes[0])) return ApplyResult::ValidationStopped;
    if (!assignValidated(sc.dtpmax2_5[0], changes[1])) return ApplyResult::ValidationStopped;
    if (!assignValidated(sc.dtpmax2_5[1], changes[2])) return ApplyResult::ValidationStopped;
    if (!assignValidated(sc.slderr, form.slidingError)) return ApplyResult::ValidationStopped;
    const int mode = sc.dttype.u58;
    def::model::dynary_copy(&sc.dttype, form.curve.get());
    sc.dttype.u58 = mode;
    if (copyTable && !detail::stepSizeCurveCopyComplete(*form.curve, sc.dttype))
        return ApplyResult::UnsupportedSnapshot; // Reached scalar/copy state remains; no fake allocation success.
    return ApplyResult::Completed;
}

// @src0x140ebd180, eb6440, ebe100, ebe800. The three-argument helper has
// explicit fresh-control semantics; the constructor uses retained state below.
std::optional<StoppingPage> fillStopping3D(const SimControl& sc, GuiModel& model,
                                         NumericLocale locale)
{
    StoppingWidgetState fresh;
    fresh.labelProfile = StoppingLabelProfile::AsciiNoActiveTranslator;
    return fillStopping3D(sc, model, locale, fresh);
}

std::optional<StoppingPage> fillStopping3D(const SimControl& sc, GuiModel& model,
                                         NumericLocale locale, const StoppingWidgetState& previous)
{
    StoppingPage form{};
    form.widgets = previous;
    if (!fillStopping3DInto(form, sc, model, locale).complete()) return std::nullopt;
    return form;
}

StoppingFillReport fillStopping3DInto(StoppingPage& form, const SimControl& sc,
    GuiModel& model, NumericLocale locale)
{
    StoppingFillReport report;
    if (!model.step || model.step->dimension != 3 || model.object(0)) {
        report.issue = StoppingFillIssue::InvalidModel; return report;
    }
    if (locale != NumericLocale::KnownC) {
        report.issue = StoppingFillIssue::UnknownNumericLocale; return report;
    }
    if (!stoppingWidgetProfile(form.widgets)) {
        report.issue = StoppingFillIssue::InvalidRetainedState; return report;
    }
    // ebd180: these11 scalar controls precede selector clear/populate and
    // node callbacks. Do not prevalidate a later scalar or rotation field.
    report.stage = StoppingFillStage::Scalars;
    const double values[]{sc.tmax, sc.smax[0], sc.smax[1], sc.smax[2],
        sc.vmin[0], sc.vmin[1], sc.vmin[2], sc.lmax[0], sc.lmax[1], sc.lmax[2], sc.emax};
    ValidatedText* controls[]{&form.timeMaximum, &form.strokeMaximum[0],
        &form.strokeMaximum[1], &form.strokeMaximum[2], &form.velocityMinimum[0],
        &form.velocityMinimum[1], &form.velocityMinimum[2], &form.loadMaximum[0],
        &form.loadMaximum[1], &form.loadMaximum[2], &form.strainMaximum};
    for (int i = 0; i < 11; ++i) {
        report.scalar = i;
        if (!std::isfinite(values[i])) {
            report.issue = StoppingFillIssue::NonfiniteValue; return report;
        }
        *controls[i] = numericText(values[i]);
    }
    if (!fillStoppingReferences(form, sc, model, report)) return report;
    report.stage = StoppingFillStage::Threshold;
    form.mdsModeCombo = sc.mdsobj[2] >= 0 && sc.mdsobj[2] <= 3 ? sc.mdsobj[2] : -1;
    if (!std::isfinite(sc.mdsobj4)) {
        report.issue = StoppingFillIssue::NonfiniteValue; return report;
    }
    form.mdsThreshold = {qt::arg(qt::L("%1"), sc.mdsobj4, 10, 'f', 5), ValidatorState::Acceptable};
    report.stage = StoppingFillStage::Rotation;
    auto* rotation = model.object(rotationObject(model));
    // ebe800 fills the whole vector before the whole center.
    for (int i = 0; i < 6; ++i) {
        report.axis = i;
        const double value = rotation ? objectField<double>(rotation,
            (i < 3 ? 0x29e8 : 0x2a00) + (i % 3) * 8) : 0.0;
        if (!std::isfinite(value)) {
            report.issue = StoppingFillIssue::NonfiniteValue; return report;
        }
        (i < 3 ? form.rotationVector[i] : form.rotationCenter[i - 3]) = qt::number(value, 'g', 6);
    }
    report.stage = StoppingFillStage::Complete;
    return report;
}

// @src0x140ebf8f0 3D branch. Invalid MDSOBJ4 only
// logs in the original; it still converts text and executes the tail.
ApplyResult applyStopping3D(SimControl& sc, GuiModel& model, const StoppingPage& form)
{
    if (!model.step || model.step->dimension != 3 || model.object(0) || form.mdsModeCombo < -1 ||
        form.mdsModeCombo > 3 || !stoppingWidgetProfile(form.widgets)) return ApplyResult::UnsupportedSnapshot;
    if ((form.widgets.references[0].selectedObject || form.widgets.references[1].selectedObject) &&
        form.widgets.labelProfile != StoppingLabelProfile::AsciiNoActiveTranslator)
        return ApplyResult::UnsupportedSnapshot;
    if (!assignValidated(sc.tmax, form.timeMaximum)) return ApplyResult::ValidationStopped;
    for (int i = 0; i < 3; ++i)
        if (!assignValidated(sc.smax[i], form.strokeMaximum[i])) return ApplyResult::ValidationStopped;
    auto* pdie = model.object(sc.pdie1);
    const int movement = pdie ? objectField<int>(pdie, 0x21c8) : 0;
    bool keepCurrentLimits = false;
    if (movement == 3 || movement == 4 || movement == 7 || movement == 8 ||
        movement == 9 || movement == 10) {
        for (int i = 0; i < 3; ++i) {
            if (std::fabs(sc.vmin[i] - qt::toDouble(form.velocityMinimum[i].text)) > 1e-6) {
                keepCurrentLimits = true;
                break;
            }
        }
        if (!keepCurrentLimits) for (int i = 0; i < 3; ++i) {
            if (std::fabs(sc.lmax[i] - qt::toDouble(form.loadMaximum[i].text)) > 1e-6) {
                keepCurrentLimits = true;
                break;
            }
        }
    }
    // question(...,Yes,No|Default,0). The source-proven default No skips these
    // six stores; it does not cancel EMAX or the remaining object callbacks.
    if (!keepCurrentLimits) {
        for (int i = 0; i < 3; ++i)
            if (!assignValidated(sc.vmin[i], form.velocityMinimum[i])) return ApplyResult::ValidationStopped;
        for (int i = 0; i < 3; ++i)
            if (!assignValidated(sc.lmax[i], form.loadMaximum[i])) return ApplyResult::ValidationStopped;
    }
    if (!assignValidated(sc.emax, form.strainMaximum)) return ApplyResult::ValidationStopped;
    sc.mdsobj4 = qt::toDouble(form.mdsThreshold.text); // no Invalid early return
    sc.mdsobj[0] = form.widgets.references[0].selectedObject;
    sc.mdsobj[1] = form.widgets.references[1].selectedObject;
    sc.mdsobj[2] = form.mdsModeCombo;
    applyStoppingReferences(model, form.widgets);
    // ec09a0(&0) is a source-proven no-op even with both references selected.
    // eb71e0 runs eb6310 again against the current model, not the Open selection.
    if (auto* rotation = model.object(rotationObject(model))) {
        for (int i = 0; i < 3; ++i)
            objectField<double>(rotation, 0x29e8 + i * 8) = qt::toDouble(form.rotationVector[i]);
        for (int i = 0; i < 3; ++i)
            objectField<double>(rotation, 0x2a00 + i * 8) = qt::toDouble(form.rotationCenter[i]);
    }
    return ApplyResult::Completed;
}

// Programmatic combo movement is not the native activated(int) callback.
bool setMeshingComboCurrentIndex(MeshingPage& form, int index)
{
    if (index < -1 || (index >= 0 && static_cast<std::size_t>(index) >= form.objects.size())) return false;
    form.comboObjectIndex = index;
    return true;
}

// @src0x140eceb60. Only the four recovered temporary scalars are represented.
MeshingObjectReport transitionMeshingObject3D(GuiModel& model, MeshingPage& form,
    int requestedIndex, MeshingObjectEvent event)
{
    MeshingObjectReport report;
    if (event != MeshingObjectEvent::InitialLoad && event != MeshingObjectEvent::Activated &&
        event != MeshingObjectEvent::SaveCurrentIndex) return report;
    // Event admission binds a delivered/current index to this combo value.
    if (requestedIndex != form.comboObjectIndex) return report;
    // Native negative argument returns before old controls or model queries.
    if (requestedIndex < 0) {
        report.outcome = ApplyResult::Completed; report.stage = MeshingObjectStage::Complete;
        return report;
    }
    if (!model.step || model.step->dimension != 3 ||
        static_cast<std::size_t>(requestedIndex) >= form.objects.size() || form.storedObjectIndex < -1 ||
        (form.storedObjectIndex >= 0 && static_cast<std::size_t>(form.storedObjectIndex) >= form.objects.size()) ||
        (event == MeshingObjectEvent::InitialLoad && form.storedObjectIndex != -1)) return report;
    report.sameRow = form.storedObjectIndex == requestedIndex;
    if (form.storedObjectIndex != -1) {
        const double interference = qt::toDouble(form.firstInterference);
        const double ratio = qt::toDouble(form.firstSizeRatio);
        const double tolerance = qt::toDouble(form.firstTolerance);
        // New stateful profile guards the native nonfinite absolute-value CRT
        // branches. Existing const Apply's value-adapter scope stays separate.
        if (!std::isfinite(interference) || !std::isfinite(ratio) || !std::isfinite(tolerance) ||
            form.firstNodeCount < 0 || form.firstNodeCount > 1000000000) return report;
        auto& previous = form.objects[static_cast<std::size_t>(form.storedObjectIndex)];
        previous.interference = std::fabs(interference);
        if (form.firstInterferenceNegative) previous.interference = -previous.interference;
        previous.sizeRatio = ratio;
        previous.tolerance = std::fabs(tolerance);
        previous.nodeCount = static_cast<double>(form.firstNodeCount);
        report.previousCommitted = true; report.stage = MeshingObjectStage::PreviousCommitted;
    }
    const auto& target = form.objects[static_cast<std::size_t>(requestedIndex)];
    // An unsupported target stops AFTER the previous temporary was committed.
    // Unvisited records need no formatting or finite-value admission.
    if (!std::isfinite(target.interference) || !std::isfinite(target.sizeRatio) ||
        !std::isfinite(target.tolerance)) return report;
    // Node count goes directly through native CVTTSD2SI, not fabs/formatting.
    // Classify bits without touching an sNaN in a floating-point predicate.
    const auto nodeBits = std::bit_cast<std::uint64_t>(target.nodeCount);
    const bool nonfiniteNode = (nodeBits & 0x7ff0000000000000ULL) == 0x7ff0000000000000ULL;
    if (nonfiniteNode && (_mm_getcsr() & _MM_MASK_INVALID) == 0) return report;
    form.firstInterferenceNegative = target.interference < 0.0;
    form.firstInterference = qt::number(std::fabs(target.interference), 'g', 6);
    form.firstSizeRatio = qt::number(target.sizeRatio, 'g', 6);
    form.firstTolerance = qt::number(target.tolerance, 'g', 6);
    form.firstNodeCount = clampSpin(integer(target.nodeCount), 0, 1000000000);
    // No same-row fast return: the original reloads even this same temporary.
    report.controlsReloaded = true; report.stage = MeshingObjectStage::ControlsReloaded;
    form.storedObjectIndex = requestedIndex;
    report.outcome = ApplyResult::Completed; report.stage = MeshingObjectStage::Complete;
    return report;
}

// @src0x140ec0a40,0x140eceb60. Both constructor fills discard any previous
// array and set previous selection=-1, so the newly selected first form is
// filled without first applying stale controls. 14085e050 copies four doubles.
std::optional<MeshingPage> fillMeshing3D(GuiModel& model, const RemeshState& state)
{
    if (!model.step || model.step->dimension != 3 ||
        (state.enabled && !std::isfinite(state.scalingFactor))) return std::nullopt;
    MeshingPage form;
    for (int i = 1; i <= model.objectCount(); ++i) {
        auto* object = model.objectAt(i);
        if (!object) return std::nullopt;
        // Original140ed97c0: existing mesh flag OR object kind5.
        if (objectField<int>(object, 0x87c) == 0 && objectField<int>(object, 0xc) != 5) continue;
        const int number = objectField<int>(object, 8);
        if (number < 1 || std::any_of(form.objects.begin(), form.objects.end(),
            [number](const MeshObjectSnapshot& item) { return item.number == number; }))
            return std::nullopt;
        form.objects.push_back({number, objectField<double>(object, 0x2dc8),
            objectField<double>(object, 0x2dd0), objectField<double>(object, 0x2de0),
            objectField<double>(object, 0x2dd8)});
    }
    // QStringList_sort sorts "%d - %s" labels before combo insertion. For
    // positive unique numbers, decimal prefix order determines the whole label.
    std::sort(form.objects.begin(), form.objects.end(), [](const auto& a, const auto& b) {
        return qt::number(a.number) < qt::number(b.number);
    });
    form.firstInterferenceNegative = false;
    form.firstNodeCount = 0;
    if (!form.objects.empty()) {
        form.comboObjectIndex = 0;
        if (transitionMeshingObject3D(model, form, 0, MeshingObjectEvent::InitialLoad).outcome != ApplyResult::Completed)
            return std::nullopt;
    }
    form.remeshEnabled = state.enabled;
    // setupUi selects method1. Fill changes it only for stored method1 or3.
    form.remeshMethodRadio = state.enabled && state.method == 3 ? 3 : 1;
    form.remeshScalingFactor = qt::number(state.enabled ? state.scalingFactor : 1.2, 'g', 6);
    form.goodInternalChangeableChecked = state.enabled ? !state.goodInternalChangeable : false;
    return form;
}

namespace {
void applyRemeshPageControls(RemeshState& state, const MeshingPage& form)
{
    state.enabled = form.remeshEnabled;
    if (state.enabled) {
        state.method = form.remeshMethodRadio;
        const double scaling = qt::toDouble(form.remeshScalingFactor);
        // Preserve original comparisons: NaN also takes the lower fallback.
        state.scalingFactor = scaling >= 1.0 ? (scaling <= 10.0 ? scaling : 10.0) : 1.0;
        state.goodInternalChangeable = !form.goodInternalChangeableChecked;
    }
}
} // namespace

// @src0x140ec1350. Apply current form0 to its temporary four scalars, then
// copy every remembered item back by its remembered number. Other native mesh
// bytes remain untouched. Finally apply the separate singleton controls.
ApplyResult applyMeshing3D(GuiModel& model, RemeshState& state, const MeshingPage& form)
{
    // Native disabled tail never reads method/scaling/internal-checkbox controls;
    // remembered object copies still precede its enabled-byte store.
    if (!model.step || model.step->dimension != 3 ||
        (form.remeshEnabled && form.remeshMethodRadio != 1 && form.remeshMethodRadio != 3) ||
        (!form.objects.empty() && (form.storedObjectIndex != 0 || form.comboObjectIndex != 0)))
        return ApplyResult::UnsupportedSnapshot;
    for (std::size_t i = 0; i < form.objects.size(); ++i) {
        auto value = form.objects[i];
        if (i == 0) {
            value.interference = std::fabs(qt::toDouble(form.firstInterference));
            if (form.firstInterferenceNegative) value.interference = -value.interference;
            value.sizeRatio = qt::toDouble(form.firstSizeRatio);
            value.tolerance = std::fabs(qt::toDouble(form.firstTolerance));
            value.nodeCount = static_cast<double>(form.firstNodeCount);
        }
        model.copyEditorMeshingObjectValues(value);
    }
    applyRemeshPageControls(state, form);
    return ApplyResult::Completed;
}

// Explicit mutable private path; this name does not alter legacy const callers.
ApplyResult applyMeshingSelection3D(GuiModel& model, RemeshState& state, MeshingPage& form)
{
    if (!model.step || model.step->dimension != 3 ||
        (form.remeshEnabled && form.remeshMethodRadio != 1 && form.remeshMethodRadio != 3))
        return ApplyResult::UnsupportedSnapshot;
    const auto transition = transitionMeshingObject3D(
        model, form, form.comboObjectIndex, MeshingObjectEvent::SaveCurrentIndex);
    if (transition.outcome != ApplyResult::Completed) return ApplyResult::UnsupportedSnapshot;
    // Save copies committed temporaries, without converting the freshly loaded
    // target a second time. Missing live objects skip only their own copy.
    for (const auto& value : form.objects) model.copyEditorMeshingObjectValues(value);
    applyRemeshPageControls(state, form);
    return ApplyResult::Completed;
}

// @src0x140ec3fa0 and final refresh0x140ecb340. Primary getter1412e6910
// is constant1 in the retained original, so ENVATM/ACVCOF always get filled.
std::optional<EnvironmentPage> fillEnvironment3D(SimControl& sc, int dimension,
    NumericLocale locale, bool pressureFieldsPopulated, bool previousDifferenceBoundaryChecked,
    EnvironmentCurveTemporaries* owners)
{
    if (dimension != 3 || locale != NumericLocale::KnownC)
        return std::nullopt;
    const std::array<def::model::DYNARY*, 4> curves{&sc.envtmp, &sc.cnvcof, &sc.envatm, &sc.acvcof};
    if (owners && !environmentOwnersMatch(*owners, curves)) return std::nullopt;
    if (!owners && (sc.acvcof.var[0] != 0 || !plainConstantCurve(sc.acvcof)))
        return std::nullopt;
    for (int i = 0; i < 4; ++i)
        if (!(i < 3 ? environmentCurveProfile(*curves[i]) : acvcofCurveProfile(*curves[i])) ||
            !std::isfinite(curves[i]->value)) return std::nullopt;
    // Original stack+20 retains the preceding ENVATM ndim boolean when an
    // unrecognized variable has nonzero ndim. It is not uninitialized state.
    const int acvSelection = sc.acvcof.var[0] == 3 ? 1 : sc.acvcof.var[0] == 12 ? 2 :
        sc.acvcof.var[0] == 1 ? 3 : sc.acvcof.ndim == 0 ? 0 : (sc.envatm.ndim != 0 ? 1 : 0);
    const double scalars[]{sc.envmpr, sc.envmpt, sc.ehrate, sc.peninf,
                           sc.unte2h, sc.tintgf, sc.blzman};
    for (int i = pressureFieldsPopulated ? 0 : 2; i < 7; ++i)
        if (!std::isfinite(scalars[i])) return std::nullopt;
    auto timeId = fillEnvironmentTimeId(sc.timeid, previousDifferenceBoundaryChecked);
    if (!timeId) return std::nullopt;
    EnvironmentPage form;
    form.timeId = std::move(*timeId);
    for (int i = 0; i < 4; ++i) {
        if (owners) {
            form.curves[i] = owners->curves[i];
        } else {
            auto curve = std::make_shared<def::model::DYNARY>();
            def::model::dynary_copy(curve.get(), curves[i]);
            form.curves[i] = std::move(curve);
        }
        form.constantValues[i] = numericText(curves[i]->value);
        if (i < 3) {
            const bool constant = curves[i]->ndim == 0;
            // Original three callbacks only select radios/enable widgets.
            // Constructor connects clicked(), so this programmatic fill does
            // not invoke an additional user-driven curve-definition action.
            form.curveControls[i] = {constant, !constant, constant, constant, !constant};
        }
    }
    form.acvcofControls = {acvSelection, acvSelection == 0, acvSelection == 0, acvSelection != 0};
    // 140eccf10 writes only the temporary's variable, not its ndim. The actual
    // initial/final copy slots live in the composer, outside this callback.
    constexpr int acvVariables[]{0, 3, 12, 1};
    if (acvSelection == 0) sc.acvcof.ndim = 0;
    sc.acvcof.var[0] = acvVariables[acvSelection];
    if (owners) {
        owners->currentCurveIndex = 3;
        owners->curves[3]->var[0] = acvVariables[acvSelection];
    }
    for (int i = 0; i < 7; ++i) {
        // The original leaves the two pressure QLineEdits empty when disabled.
        // Exact Qt4 validateChars + QDoubleValidator returns Intermediate1.
        form.scalars[i] = !pressureFieldsPopulated && i < 2
            ? ValidatedText{qt::L(""), ValidatorState::Intermediate} : numericText(scalars[i]);
    }
    if (pressureFieldsPopulated && sc.htmthd > 0)
        form.heatMethodComboOverwrite = sc.htmthd <= 3 ? sc.htmthd - 1 : -1;
    form.viewFactorChecked = sc.vfactr_mode != 0;
    // The stateChanged slot only changes this button's enable state.
    form.viewFactorDefinitionEnabled = form.viewFactorChecked;
    return form;
}

void fillEnvironmentSharedControls(GeneralPage& general, const EnvironmentPage& environment)
{
    if (environment.heatMethodComboOverwrite)
        general.heatMethodCombo = *environment.heatMethodComboOverwrite;
}

// @src0x140ec9b30 then0x140ec50f0. Constant-curve validation is not a page
// early return: Invalid retains that remembered constant and still copies it.
// Table radio branches skip text/ndim stores and copy the full remembered curve.
ApplyResult applyEnvironment3D(SimControl& sc, const EnvironmentPage& form,
    std::optional<EnvironmentViewFactorState> viewFactor, EnvironmentCurveTemporaries* owners)
{
    const std::array<def::model::DYNARY*, 4> destinations{&sc.envtmp, &sc.cnvcof, &sc.envatm, &sc.acvcof};
    if (owners) {
        if (!environmentOwnersMatch(*owners, destinations)) return ApplyResult::UnsupportedSnapshot;
        for (int i = 0; i < 4; ++i)
            if (form.curves[i].get() != owners->curves[i].get()) return ApplyResult::UnsupportedSnapshot;
    }
    if (form.acvcofControls.selection < 0 || form.acvcofControls.selection > 3)
        return ApplyResult::UnsupportedSnapshot;
    for (int i = 0; i < 4; ++i)
        if (!form.curves[i] || !(i < 3 ? environmentCurveProfile(*form.curves[i]) :
            acvcofCurveProfile(*form.curves[i]))) return ApplyResult::UnsupportedSnapshot;
    for (int i = 0; i < 4; ++i) {
        auto applyCurve = [&](def::model::DYNARY& temporary) {
            // Original140ec9b30 mutates editor+c8/d0/d8/e0 in place, before
            // copying to SC. Invalid text skips only the value store; neither
            // the ndim store nor the following model copy is skipped.
            if (i < 3 ? form.curveControls[i].constantChecked : form.acvcofControls.selection == 0) {
                temporary.ndim = 0;
                if (form.constantValues[i].state != ValidatorState::Invalid)
                    temporary.value = qt::toDouble(form.constantValues[i].text);
            }
            def::model::dynary_copy(destinations[i], &temporary);
        };
        if (owners) {
            applyCurve(*owners->curves[i]);
        } else {
            // Legacy isolated snapshots have no editor lifetime. Preserve
            // their qualified copy-only semantics; the composer always
            // supplies the actual owners and never enters this branch.
            def::model::DYNARY temporary;
            def::model::dynary_copy(&temporary, form.curves[i].get());
            applyCurve(temporary);
        }
    }
    double* scalars[]{&sc.envmpr, &sc.envmpt, &sc.ehrate, &sc.peninf,
                     &sc.unte2h, &sc.tintgf, &sc.blzman};
    for (int i = 0; i < 7; ++i)
        if (!assignValidated(*scalars[i], form.scalars[i])) return ApplyResult::ValidationStopped;
    sc.vfactr_mode = form.viewFactorChecked ? 1 : 0;
    if (form.viewFactorChecked) {
        if (!viewFactor) return ApplyResult::UnsupportedSnapshot;
        // Original queries the live dimension only after setting mode=1. For
        // non-3D models the curve and definition remain untouched.
        if (viewFactor->dimension == 3) {
            if (!viewFactor->curve || viewFactor->curve == &sc.vfactr)
                return ApplyResult::UnsupportedSnapshot;
            def::model::dynary_copy(&sc.vfactr, viewFactor->curve);
            sc.vfactr_n = viewFactor->definition;
        }
    }
    // Original environment Apply does not consume the TIMEID display tables
    // or DiffBnd checkbox. Preserve the then-current model, including changes
    // made since the last fill; only an actual user callback can replace it.
    return ApplyResult::Completed;
}

// @src0x140ec5bc0. This first branch already has the six-row table, so no
// allocation is needed. Exact signed integers make its initial combo callbacks
// preserve stored frequency bytes; unsupported table shapes are not repaired.
std::optional<AdvancedPage> fillAdvanced3D(const SimControl& sc, GuiModel& model,
    NumericLocale locale)
{
    if (!model.step || model.step->dimension != 3 || model.simCtl() != &sc ||
        locale != NumericLocale::KnownC || !advancedTimeProfile(model) ||
        (sc.tnow3 != 0 && sc.tnow3 != 1) ||
        !std::isfinite(sc.tnow1) || !std::isfinite(sc.tnow2) ||
        sc.frqncy_rows != 6 || sc.frqncy_cols != 1 ||
        sc.frqncy.ncomp != 6 || !sc.frqncy.values) return std::nullopt;
    for (int i = 0; i < 3; ++i)
        if (!elementNodeRadio(sc.elmnod[i])) return std::nullopt;
    for (double value : sc.geoerr)
        if (!std::isfinite(value * 100.0)) return std::nullopt;
    for (const auto& value : sc.usrdef)
        if (!asciiTerminated(value)) return std::nullopt;
    for (int i = 0; i < 6; ++i) {
        const double value = sc.frqncy.values[i];
        const int converted = integer(value);
        if (!std::isfinite(value) || value != static_cast<double>(converted) ||
            (value == 0.0 && std::signbit(value))) return std::nullopt;
    }
    AdvancedPage form;
    form.time = {numericText(sc.tnow1), numericText(sc.tnow2)};
    form.timeRadio = sc.tnow3;
    for (int i = 0; i < 4; ++i) form.geometryErrorPercent[i] = numericText(sc.geoerr[i] * 100.0);
    for (int i = 0; i < 10; ++i) form.userStrings[i] = qt::String::latin1(sc.usrdef[i]);
    for (int i = 0; i < 7; ++i) form.outputFlags[i] = i == 4 || sc.stnout[i] != 0;
    for (int i = 0; i < 3; ++i) form.elementNodeOptions[i] = sc.elmnod[i];
    form.secondaryDieNumber = 0;
    std::vector<int> numbers;
    for (int i = 1; i <= model.objectCount(); ++i) {
        auto* object = model.objectAt(i);
        if (!object || objectField<int>(object, 0xc) == 7 ||
            (objectField<int>(object, 0x87c) == 0 && objectField<int>(object, 0xc) != 5)) continue;
        const int number = objectField<int>(object, 8);
        if (number < 1 || std::find(numbers.begin(), numbers.end(), number) != numbers.end())
            return std::nullopt;
        numbers.push_back(number);
        if (number == sc.pdie2) form.secondaryDieNumber = number;
    }
    for (int i = 0; i < 6; ++i) {
        const int value = integer(sc.frqncy.values[i]);
        form.frequencyCombo[i] = value == 0 ? 0 : (i == 0 && value != -1 ? 2 : 1);
        form.frequencyInputEnabled[i] =
            i == 0 ? form.frequencyCombo[i] == 2 : form.frequencyCombo[i] == 1;
        if (value != 0 && !(i == 0 && value == -1))
            form.populatedFrequencyText[i] = qt::number(value);
    }
    return form;
}

// @src0x140ec7500. Invalid stops this page only and retains earlier stores.
// The final callback is original14082a360(0), the instruction-shape verified
// native twin1401d0fc0. It rebuilds the state widths and remaps object arrays.
ApplyResult applyAdvanced3D(SimControl& sc, GuiModel& model, const AdvancedPage& form)
{
    if (!advancedApplyProfile(sc, model, form)) return ApplyResult::UnsupportedSnapshot;
    std::array<std::string, 10> userBytes;
    for (int i = 0; i < 10; ++i)
        userBytes[i] = form.userStrings[i].utf8();
    if (!assignValidated(sc.tnow1, form.time[0])) return ApplyResult::ValidationStopped;
    if (!assignValidated(sc.tnow2, form.time[1])) return ApplyResult::ValidationStopped;
    sc.tnow3 = form.timeRadio;
    // Original calls1408bfe60 once, then again if either time changed by>1e-6.
    // Both calls are no-ops under advancedTimeProfile, checked before stores.
    for (int i = 0; i < 4; ++i) {
        if (form.geometryErrorPercent[i].state == ValidatorState::Invalid)
            return ApplyResult::ValidationStopped;
        sc.geoerr[i] = qt::toDouble(form.geometryErrorPercent[i].text) / 100.0;
    }
    for (int i = 0; i < 10; ++i) std::strcpy(sc.usrdef[i], userBytes[i].c_str());
    for (int i = 0; i < 7; ++i) sc.stnout[i] = form.outputFlags[i] ? 1 : 0;
    for (int i = 0; i < 3; ++i) sc.elmnod[i] = form.elementNodeOptions[i];
    sc.pdie2 = form.secondaryDieNumber;
    def::model::step_remap_material_state(*model.step, false);
    return ApplyResult::Completed;
}

namespace {
// Ordinary owned B1/B2 storage only. Zero-count borrowed-axis markers and
// opaque nested owners require a separate lifetime contract.
bool ownedRecoveryTableProfile(const def::model::DYNARY& table, int type)
{
    if (table.type != type || table.ndim != 1 || table.var[0] != 0 || table.u68 ||
        table.n[0] < 0 || table.n[1] || table.n[2] || table.axis[1] || table.axis[2]) return false;
    if (table.n[0] == 0)
        return !table.axis[0] && !table.values && table.ncomp >= 0 && table.ncomp <= 2;
    return table.ncomp == 2 && table.n[0] <= (std::numeric_limits<std::int32_t>::max)() / 2 &&
        table.axis[0] && table.values && table.axis[0] != table.values;
}
}

// @src0x140ed5dd0. Actual operation-owned A/B, not arbitrary recovery snapshots.
ApplyResult applyOwnedRecoveryTail3D(SimControl& sc, GuiModel& model, const AdvancedPage& form,
    Operation& operation)
{
    if (operation.operationInfoIdentity().get() != &operation)
        return ApplyResult::UnsupportedSnapshot;
    const auto modelOwner = operation.engineModel;
    auto* const project = operation.project;
    if (!project || modelOwner.get() != &model || !model.step ||
        model.simCtl() != &sc || sc.owner != model.step.get()) return ApplyResult::UnsupportedSnapshot;
    auto* const sub = activeSubProject(*project);
    if (!sub || sub->project != project || sub->type == 4)
        return ApplyResult::UnsupportedSnapshot; // No lazy separate data-root creation.

    // Resolve at this reached tail, after the earlier pages and Extra3D. The
    // native setters reinstall these same pointers; f34b88 skips the old-B free.
    const auto rootOwner = project->recoveryA;
    const auto localOwner = operation.recoveryB;
    if (!rootOwner || !localOwner || recoveryListA(operation) != rootOwner.get() ||
        !advancedApplyProfile(sc, model, form) ||
        !ownedRecoveryTableProfile(sc.nrecid, 0xb2) ||
        !ownedRecoveryTableProfile(sc.erecid, 0xb1)) return ApplyResult::UnsupportedSnapshot;

    // Candidate resource/record admission, not an original format maximum.
    constexpr std::size_t maxRecords = 4096;
    if (rootOwner->size() > maxRecords || localOwner->size() > maxRecords - rootOwner->size())
        return ApplyResult::UnsupportedSnapshot;
    std::int32_t nodeCount = 0, elementCount = 0;
    for (const auto* entries : {rootOwner.get(), localOwner.get()})
        for (const auto& entry : *entries) {
            if (entry.type == 0) ++nodeCount;
            else if (entry.type == 1) ++elementCount;
            else return ApplyResult::UnsupportedSnapshot;
        }

    // Recheck current holders/control after admission; retain their actual
    // shared owners for the call instead of substituting copied list objects.
    if (operation.operationInfoIdentity().get() != &operation || operation.project != project ||
        operation.engineModel.get() != modelOwner.get() ||
        operation.engineModel.owner_before(modelOwner) || modelOwner.owner_before(operation.engineModel) ||
        activeSubProject(*project) != sub || recoveryListA(operation) != rootOwner.get() ||
        project->recoveryA.get() != rootOwner.get() || project->recoveryA.owner_before(rootOwner) ||
        rootOwner.owner_before(project->recoveryA) || operation.recoveryB.get() != localOwner.get() ||
        operation.recoveryB.owner_before(localOwner) || localOwner.owner_before(operation.recoveryB) ||
        model.simCtl() != &sc || sc.owner != model.step.get()) return ApplyResult::UnsupportedSnapshot;

    if (sc.nrecid.n[0] > 0) def::model::dynary_free_arrays(&sc.nrecid); // ed5e87
    sc.nrecid.n[0] = 0; // ed5e91
    sc.nrecid.ncomp = 0; // ed5ea0; ERECID is deliberately untouched here.
    auto rebuild = [&](def::model::DYNARY& table, std::int32_t count, int type) {
        if (count == 0) return true;
        table.n[0] = count;
        table.ncomp = 2;
        def::model::dynary_alloc(&table); // ed5fe7 for nodes, ed61fb for elements.
        if (!table.axis[0] || !table.values) return false; // Retain reached allocation state; no fake OOM result.
        std::size_t row = 0;
        for (const auto* entries : {rootOwner.get(), localOwner.get()})
            for (const auto& entry : *entries) if (entry.type == type) {
                table.axis[0][row] = static_cast<double>(entry.ppvSecondInt);
                table.values[row * 2] = entry.ppvFirstDouble;
                table.values[row * 2 + 1] = entry.ppvSecondDouble;
                ++row;
            }
        return true;
    };
    if (!rebuild(sc.nrecid, nodeCount, 0) || !rebuild(sc.erecid, elementCount, 1))
        return ApplyResult::UnsupportedSnapshot;
    const auto result = applyAdvanced3D(sc, model, form); // ed63ea: intentionally repeated after the page.
    resizeRecoveryOutputs(model); // ed6418/6446, including after ValidationStopped.
    return result;
}

// Legacy explicit empty-list projection. Nonempty input needs the actual owner-bound entry above.
ApplyResult applyEmptyRecoveryTail(SimControl& sc, GuiModel& model, const AdvancedPage& form,
    const std::vector<RecoveryEntry>* rootList,
    const std::vector<RecoveryEntry>* operationList)
{
    if (!rootList || !operationList || !rootList->empty() || !operationList->empty() ||
        !advancedApplyProfile(sc, model, form)) return ApplyResult::UnsupportedSnapshot;
    if (sc.nrecid.n[0] > 0) def::model::dynary_free_arrays(&sc.nrecid);
    sc.nrecid.n[0] = 0;
    sc.nrecid.ncomp = 0;
    // Original does not clear ERECID when the counted element entries are0.
    const auto result = applyAdvanced3D(sc, model, form);
    resizeRecoveryOutputs(model); // still called after a page validation stop
    return result;
}
} // namespace m2m::engine::simcontrol300
