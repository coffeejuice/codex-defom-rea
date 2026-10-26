// MDT L0: SfStepData, the GUI wrapper of a DEF_PRE DB_STEP_DATA (spec_A section 7, spec_B 4.4).
// Engine calls go to the DEF_PRE port; the 3D viewer the wrapper keeps in sync (0x140c80280 chain)
// is packet V. Its retained View/Settings/DOE state and guarded geometry effects
// are implemented in viewer.cpp; successful model creation is not full UI recovery.
#include <cstdio>
#include <cstring>
#include <limits>

#include <defcmd/key_file.hpp>
#include <defcmd/key_import.hpp>
#include <defkey/item_writers.hpp>
#include <def_pre/menu56.hpp>
#include <defmodel/gui_twins.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/dynary_table.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defmodel/material/state_remap.hpp>
#include <defmodel/step/db_step.hpp>

#include "cogging_save.hpp"
#include "engine_internal.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine_scene.hpp"
#include "m2m/engine_auxiliary.hpp"
#include "m2m/qt4.hpp"

#include "m2m/doe_stepdata.hpp"
#include "m2m/engine/picking_owner.hpp"
#include "m2m/engine/simcontrol300.hpp"
#include "m2m/engine/simcontrol300_io.hpp"

namespace m2m::engine {

struct GuiModelLifetime {
    GuiModel* value;
    std::uint64_t stepGeneration = 1;
    bool stepObservationLive = true;
};

bool StepIdentity::operator==(const StepIdentity& other) const noexcept
{
    return generation_ == other.generation_
        && !owner_.owner_before(other.owner_) && !other.owner_.owner_before(owner_);
}

bool StepIdentity::isCurrent() const noexcept
{
    const auto live = owner_.lock();
    return live && live->stepObservationLive && generation_ == live->stepGeneration;
}

StepIdentity GuiModel::stepIdentity() const noexcept
{
    return StepIdentity(lifetime_, lifetime_->stepGeneration);
}

bool GuiModel::isCurrentStepIdentity(const StepIdentity& identity) const noexcept
{
    return identity == stepIdentity() && identity.isCurrent();
}

void GuiModelGuard::reset(GuiModel* model)
{
    if (model) lifetime_ = model->lifetime_;
    else lifetime_.reset();
}

GuiModel* GuiModelGuard::get() const noexcept
{
    const auto live = lifetime_.lock();
    return live ? live->value : nullptr;
}

using def::model::Cls_1401e7b10;
using def::model::Material;

namespace {

// GUI options globals the names part reads: TRANS 1..6 defaults (bytes 0x143d787c4..0x143d787c9) and
// ELMNOD 1..3 defaults (ints 0x143d78978 / 0x143d7897c / 0x143d78980). They are .bss and written only
// by the options dialog's apply (0x1403b4dc0 -> 0x1403b5770, 0x1403b51e0 -> 0x1403b5c60 / 0x1403b5ce0 /
// 0x1403b5d60), so a GUI started for a conversion has them all 0.
const std::uint8_t kTransDefaults[6] = {};
const std::int32_t kElmnodDefaults[3] = {};

bool ownerIs2D(const Cls_1401e7b10& c)
{
    return c.owner->dimension == 2;        // 0x140065560(SC+0xbc0): owner+0x1c == 2
}

// @src 0x1408d3f50  names and integer controls (DEF_PRE twin 1401e7e90 differs: TRANS and ELMNOD 1..3
// come from the options globals above)
void guiNames(Cls_1401e7b10& c)
{
    std::strcpy(c.title, "SIMULATION");    // 0x1427b2240
    std::strcpy(c.oprnam, "OPERATION 1");  // 0x1427b2250
    std::strcpy(c.simnam, "SIMULATION 1"); // 0x1427b2260
    c.cursim[0] = 1;
    c.cursim[1] = 0;
    c.cursim[2] = 0;
    c.meshno = 1;
    c.smode = 1;
    c.stype = 1;
    c.geotyp = 1;
    for (int i = 0; i < 6; ++i) *at<std::int32_t>(&c, 0x104 + static_cast<std::size_t>(i) * 4) = 0;
    c.trans[1] = 1;                        // +0x108
    for (int i = 0; i < 6; ++i)            // +0x104 .. +0x118 = (uint) byte global
        *at<std::uint32_t>(&c, 0x104 + static_cast<std::size_t>(i) * 4) = kTransDefaults[i];
    c.stnout[0] = 0;
    c.stnout[1] = 0;
    c.stnout[2] = 0;
    c.stnout[3] = 0;
    c.stnout[4] = 1;                       // +0x12c
    c.stnout[5] = 0;
    c.stnout[6] = 0;
    for (int i = 0; i < 10; ++i) c.elmnod[i] = 0;
    c.elmnod[0] = kElmnodDefaults[0];      // +0x138
    c.elmnod[1] = kElmnodDefaults[1];      // +0x13c
    c.elmnod[2] = kElmnodDefaults[2];      // +0x140
}

// @src 0x1408d45f0  step controls: NSTART -1, NSTEP 100, STPINC 10, PDIE 0 (DEF_PRE's twin 1401e8490
// then writes NSTEP 1; the GUI's does not: oracle B 95, a new operation's SimCtrl.KEY has NSTEP 100)
void guiStepControls(Cls_1401e7b10& c)
{
    c.nstart = -1;                         // +0x160
    c.nstep = 100;                         // +0x16c
    c.stpinc = 10;                         // +0x164
    c.pdie1 = 0;                           // +0x168
}

// @src 0x1408d5800  iteration controls (DEF_PRE twin 1401e96b0 differs: SOLMTD 2 in 3D, not 1)
void guiIterationControls(Cls_1401e7b10& c)
{
    if (!ownerIs2D(c)) {
        c.cvgerr[0] = 0.005;               // 0x142239610
        c.cvgerr[1] = 0.05;                // 0x1421ec2e0
        c.solmtd = 2;                      // +0x2e4
        c.solmtt = 2;                      // +0x318
    } else {
        c.cvgerr[0] = 0.001;               // 0x1421b5050
        c.cvgerr[1] = 0.01;                // 0x142203f60
        c.solmtd = 1;
        c.solmtt = 1;
    }
    c.u2e8 = 0;
    c.solmtd_p[0] = 0.0;
    c.solmtd_p[1] = 0.0;
    c.solmtd_p[2] = 0.0;
    c.solmtd_p[3] = 0.0;
    def::model::sim_ctrl_init_u328(c);     // 0x1408d59a0 = 1401e9850
    if (!ownerIs2D(c)) c.itrmth = 2;       // +0x2e0
    else c.itrmth = 1;
    c.itrmxd = 200;                        // +0x310
    c.defbwd = 1;                          // +0x314
    c.itrmxt = 200;                        // +0x31c
    c.tmpbwd = 1;                          // +0x320
    c.latent = 0;                          // +0x324
}

// @src 0x1408d3d10  the GUI engine's sim-control init (twin of DEF_PRE 1401e7c50 with three GUI-only
// parts). The GUI's DB_STEP_DATA init 0x140821ea0 (twin of 1401c8c00) calls it where DEF_PRE's calls
// 1401e7c50, from the ctor (0x140c7a870) and from the clear 0x1408246a0; the port's DB_STEP_DATA runs
// DEF_PRE's, so GuiModel runs this right after it: every part writes all of its fields, so the
// state is the GUI's.
void guiSimCtrlInit(Cls_1401e7b10& c)
{
    guiNames(c);                           // 0x1408d3f50
    guiStepControls(c);                    // 0x1408d45f0
    def::model::sim_ctrl_init_u1c8(c);     // 0x1408d47a0 = 1401e8650
    def::model::sim_ctrl_init_u170(c);     // 0x1408d5100 = 1401e8fb0
    guiIterationControls(c);               // 0x1408d5800
    def::model::sim_ctrl_init_u3a8(c);     // 0x1408d6800 = 1401ea6b0
    def::model::sim_ctrl_init_u640(c);     // 0x1408d6eb0 = 1401ead60
}

}  // namespace

// The engine of DEF_GUI_MO_64 differs from DEF_PRE's in the sim-control iteration init
// (0x1408d5800: SOLMTD 2 in 3D); every model this converter builds is a GUI engine model.
[[maybe_unused]] const bool g_guiEngine = (def::model::g_gui_engine_variant = true);

GuiModel::GuiModel(DbHandle* h, int dim)
    : lifetime_(std::make_shared<GuiModelLifetime>(GuiModelLifetime{this})),
      step(std::make_unique<DB_STEP_DATA>(dim)), db(h)
{
    guiSimCtrlInit(step->sim_ctrl);        // 0x140c7a870 -> 0x140821ea0 -> 0x1408d3d10
}

GuiModel::~GuiModel()
{
    // Retire observations before cleanup; keep the existing guard/member lifetime
    // ordering. No token locks or dereferences the GuiModel itself.
    lifetime_->stepObservationLive = false;
    // c7aa00 ends by deleting the +250 wrapper, before member/QObject cleanup.
    // Its earlier reset/other cleanup remains separately qualified; reset()
    // itself must preserve this list. Pointees belong to their native managers.
    clearAuxiliaryDrawings(*this);
    if (display) displayForget(*display, this);   // the display's QPointers to this model become null
}

void GuiModel::setModelUnit(std::int32_t unit, bool reset)
{
    def::key::set_model_unit(*step, unit, reset);
}

void GuiModel::requireGeometrySyncAvailable() const
{
    if (geometrySyncBarrier_)
        throw Error(M2M_E_UNSUPPORTED, "geometry query is parked before unrecovered reader continuation");
}

Cls_1401e7b10* GuiModel::simCtl() const
{
    requireGeometrySyncAvailable();
    return step ? &step->sim_ctrl : nullptr;
}

void GuiModel::setHeatExpressPrimaryObject()
{
    auto* control = simCtl();
    if (!control) return;
    (void)*at<std::int32_t>(control, 0x240); // 0x1408da6d0, result unused
    control->pdie1 = objectCount() == 1 ? 1 : 2;
}

bool GuiModel::matchesCurrentSimControl(const Cls_1401e7b10* expected) const
{
    requireGeometrySyncAvailable();
    const auto* current = step ? &step->sim_ctrl : nullptr;
    return current == expected;
}

bool GuiModel::refillEditorStepCountIfControlMatches(
    const Cls_1401e7b10* expected, const simcontrol300::StepCountContext& context,
    bool priorNstartChecked, simcontrol300::StepCountPage& result) const
{
    const auto* control = simCtl();
    if (!control || control != expected) return false;
    auto refill = simcontrol300::fillStepCount(*control, context, priorNstartChecked);
    if (!refill) return false;
    result = std::move(*refill);
    return true;
}

void GuiModel::copyEditorMeshingObjectValues(const simcontrol300::MeshObjectSnapshot& value)
{
    auto* object = this->object(value.number);
    if (!object) return;
    *at<double>(object, 0x2dc8) = value.interference;
    *at<double>(object, 0x2dd0) = value.sizeRatio;
    *at<double>(object, 0x2de0) = value.tolerance;
    *at<double>(object, 0x2dd8) = value.nodeCount;
}

void GuiModel::remapEditorAdvancedState()
{
    def::model::step_remap_material_state(*step, false);
}

void GuiModel::setFurnaceLoadTnow3()
{
    if (step) step->sim_ctrl.tnow3 = 1;
}

void GuiModel::setLoadedStepNumberAfterAdmission(int stepNumber)
{
    step->u744 = stepNumber;
}

void GuiModel::initializeFurnacePostLoadDefaults()
{
    auto* sc = simCtl();
    sc->furnace_on = 1;
    if (sc->furnace.curve_count() == 0) {
        const std::int32_t n = 0x16;
        sc->furnace.setup_curves(&n);
        sc->furnace.set_defaults();
    }
}

void GuiModel::initializeMultiBlowLoadControl()
{
    auto* control = simCtl();
    if (!control) return;
    control->dttype.u58 = 0;
    control->nstep = 10000;
}

void GuiModel::setMultiBlowPrimaryDie(int primary)
{
    auto* control = simCtl();
    if (!control) return;
    control->pdie1 = primary;
}

// @src 0x140054540: finite Save tuple; conversions remain after admission and between stores.
void GuiModel::storeOperationSimulationNames(const String& operationName, int flow,
    const String& simulationName, int simulation)
{
    auto* sc = simCtl();
    if (!sc) return;
    std::strncpy(sc->oprnam, local8(operationName).c_str(), 0x40);
    sc->cursim[1] = flow;
    std::strncpy(sc->simnam, local8(simulationName).c_str(), 0x50);
    sc->cursim[0] = simulation;
}

bool GuiModel::copyEditorTitleIfControlMatches(const Cls_1401e7b10* expected,
    std::string_view ascii)
{
    if (ascii.size() > 80) return false;
    for (unsigned char byte : ascii)
        if (byte < 0x20 || byte > 0x7e) return false;
    requireGeometrySyncAvailable();
    auto* control = step ? &step->sim_ctrl : nullptr;
    if (!control || control != expected) return false;
    if (!ascii.empty()) std::memcpy(control->title, ascii.data(), ascii.size());
    control->title[ascii.size()] = '\0';
    return true;
}

bool GuiModel::applyEditorFrequencyIndexIfControlMatches(
    const Cls_1401e7b10* expected, int row, int selection)
{
    if (row < 0 || row > 5 || selection < 0 || selection > (row == 0 ? 2 : 1))
        return false;
    requireGeometrySyncAvailable();
    auto* control = step ? &step->sim_ctrl : nullptr;
    if (!control || control != expected) return false;
    const int stride = control->frqncy_cols;
    if (stride != 1) return false;
    if (selection == (row == 0 ? 2 : 1)) return true;
    if (control->frqncy_rows != 6) return false;
    if (control->frqncy.ncomp != 6) return false;
    if (!control->frqncy.values) return false;
    control->frqncy.values[row * stride] = row == 0 && selection == 1 ? -1.0 : 0.0;
    return true;
}

bool GuiModel::applyEditorFrequencyTextIfControlMatches(
    const Cls_1401e7b10* expected, int row, const String& deliveredText)
{
    if (row < 0 || row > 5) return false;
    const auto& text = deliveredText.u16();
    if (!text.empty()) {
        if (text.size() > 10 || text.front() < u'1' || text.front() > u'9') return false;
        std::uint64_t magnitude = 0;
        for (char16_t unit : text) {
            if (unit < u'0' || unit > u'9') return false;
            magnitude = magnitude * 10 + (unit - u'0');
        }
        if (magnitude > (std::numeric_limits<std::int32_t>::max)()) return false;
    }
    requireGeometrySyncAvailable();
    auto* control = step ? &step->sim_ctrl : nullptr;
    if (!control || control != expected) return false;
    const int stride = control->frqncy_cols;
    if (control->frqncy_rows != 6 || stride != 1 || control->frqncy.ncomp != 6)
        return false;
    const int converted = qt::toInt(deliveredText);
    auto* values = control->frqncy.values;
    if (!values) return false;
    values[row * stride] = static_cast<double>(converted);
    return true;
}

int GuiModel::simGeometryTypeIf2D() const
{
    requireGeometrySyncAvailable();
    if (!step || step->dimension != 2) return 0;
    return *at<std::int32_t>(&step->sim_ctrl, 0x100);
}

int GuiModel::dimension() const noexcept
{
    return step ? step->dimension : 0;
}

int GuiModel::dimensionOr(int whenNoStep) const noexcept
{
    return step ? step->dimension : whenNoStep;
}

bool GuiModel::readPropertyHeatingTimeUnit(int& result) const noexcept
{
    if (!step) return false;
    result = step->sim_ctrl.unit;
    return true;
}

void GuiModel::setDimension(int dim)
{
    requireGeometrySyncAvailable();
    const std::int32_t d = dim;
    step->set_dimension(&d);
}

void GuiModel::setFormingExpressDimensionFlags()
{
    if (!step) return;
    auto* nativeStep = step.get();
    const bool is2D = nativeStep->dimension == 2;
    const std::int32_t value = is2D ? 1 : 2;
    *at<std::int32_t>(nativeStep, 0xa34) = value;
    *at<std::int32_t>(nativeStep, 0xa68) = value;
    *at<std::int32_t>(nativeStep, 0xa30) = value;
    if (is2D) *at<std::int32_t>(nativeStep, 0x1118) = 0;
}

int GuiModel::lastObjectNumber() const
{
    requireGeometrySyncAvailable();
    return step ? def::model::last_object_number(*step) : 0;
}

PendingPickingOwner& GuiModel::beginPickingConstructionPrefix()
{
    requireGeometrySyncAvailable();
    if (pickingConstruction_)
        throw Error(M2M_E_UNSUPPORTED, "Picking constructor is already pending; original publication is unrecovered");
    pickingConstruction_ = std::make_unique<PendingPickingOwner>(*this);
    (void)pickingConstruction_->prepareConstructor(*this);
    return *pickingConstruction_;
}

void GuiModel::reset()
{
    requireGeometrySyncAvailable();
    if (!step) return;
    // Check before the first destructive effect, and never wrap to revive a token.
    // This adapter observation is deliberately not a native admission authority.
    if (lifetime_->stepGeneration == std::numeric_limits<std::uint64_t>::max())
        throw Error(M2M_E_UNSUPPORTED, "step identity generation exhausted before reset");
    ++lifetime_->stepGeneration;
    viewerClear(*this);                    // 0x140c82c70; 0x140c8ba80: +0x30 (viewer) dropped
    // 0x140c82c70 / 0x140c8ba80: viewer item list and +0x30 (viewer, packet V); of the items only
    // the CopyMirror plane counts are kept (MDT L5)
    viewerMirrorPlanes.clear();
    def::model::step_clear(*step);         // 0x1408246a0 = 1401cb400
    guiSimCtrlInit(step->sim_ctrl);        // the GUI clear 0x1408246a0 re-inits with 0x1408d3d10
    // 0x140c902c0: +0x230 (viewer), then +0x100 = 0 (0x140c833c0)
    if (doeStepData) doeStepData->resetPropertyTable();
    state100 = 0;
}

LLOBJ_DATA* GuiModel::object(int no) const
{
    requireGeometrySyncAvailable();
    if (!step) return nullptr;
    return def::model::object_list_find(&step->objects, no);   // 14017aaf0
}

LLOBJ_DATA* GuiModel::objectAt(int no) const
{
    requireGeometrySyncAvailable();
    if (!step) return nullptr;
    return step->objects.at(no);                               // 14017aa80
}

bool GuiModel::copyEditorSpringComboLabels(simcontrol300::SpringComboLabels& result) const
{
    // 140ec8b00 builds visible "%-3d - name" combo text, with a zero/None
    // placeholder, then walks object(i) by object number. 140ec8da0 later
    // parses that text's first integer; QVariant item data is not consulted.
    const int count = objectCount();
    if (count < 0 || count > 4096) return false; // bounded reconstruction admission
    simcontrol300::SpringComboLabels labels{};
    labels.first.reserve(static_cast<std::size_t>(count) + 1);
    auto append = [&](int number, std::string_view name) {
        // The native search copies the local-8-bit label into a stack buffer
        // unchecked. Admit only short ASCII labels whose byte conversion is
        // unambiguous. The original builder formats the signed decimal number.
        for (unsigned char c : name)
            if (c < 0x20 || c > 0x7e) return false;
        char buffer[64];
        const int n = std::snprintf(buffer, sizeof(buffer), "%-3d - %.*s",
            number, static_cast<int>(name.size()), name.data());
        if (n < 0 || n >= static_cast<int>(sizeof(buffer))) return false;
        labels.first.push_back(qt::String::latin1(std::string_view(buffer,
            static_cast<std::size_t>(n))));
        return true;
    };
    if (!append(0, "None")) return false;
    for (int i = 1; i <= count; ++i) {
        const auto* object = this->object(i);
        if (!object) return false;
        if (*at<const std::int32_t>(object, 0xc) == 7) continue;
        const auto* name = at<const char>(object, 0x828);
        std::size_t length = 0;
        while (length < 0x54 && name[length] != '\0') ++length;
        if (length == 0x54 || !append(*at<const std::int32_t>(object, 8),
                std::string_view(name, length))) return false;
    }
    labels.second = labels.first;
    result = std::move(labels);
    return true;
}

int GuiModel::objectCount() const
{
    return step ? step->u1378 : 0;
}

int GuiModel::countNonType7Objects() const
{
    return def::pre::step_count_objects(step.get());
}

namespace {

// @src 0x140833b40  default object name: QSettings "systemOption/autoObjName" (default true, the
// GUI's fresh-user value; m2m has no user settings) -> Workpiece / Top Die / Bottom Die / Object %d
void defaultObjectName(LLOBJ_DATA* o, int no)
{
    char* name = at<char>(o, 0x828);
    const int type = *at<std::int32_t>(o, 0xc);
    const bool autoName = true;
    if (!autoName) std::sprintf(name, "Object %d", no);
    else if (no == 1 && type != 0) std::strcpy(name, "Workpiece");
    else if (no == 2 && type == 0) std::strcpy(name, "Top Die");
    else if (no == 3 && type == 0) std::strcpy(name, "Bottom Die");
    else std::sprintf(name, "Object %d", no);
}

}  // namespace

LLOBJ_DATA* GuiModel::objectWrap(int no, bool reset, bool withViewer)
{
    requireGeometrySyncAvailable();
    LLOBJ_DATA* o = object(no);
    const bool created = o == nullptr;
    if (created) o = def::model::llobj_ctor::get_or_create(step.get(), no);   // 1408bde10 = 14019ccc0
    if (reset && *at<std::int32_t>(o, 0xc) == 7) {                           // 140834fa0 false
        def::model::llobj_reset_kind7(o, 0);                                   // 140834fe0 = 140160f50
        defaultObjectName(o, no);
    }
    if (created) {
        // @src 0x140c7c260  a deformable object exists (1401cac80 != 0)
        if (def::pre::step_first_deformable(step.get()) == 0) *at<std::int32_t>(o, 0xc) = 1;
        defaultObjectName(o, no);
        repickPdie(*this);                                     // 0x140c7b630 (cogging_save.cpp)
        viewerSync(*this, o, withViewer);                                      // 0x140c80280
    }
    return o;
}

LLOBJ_DATA* GuiModel::appendObject()
{
    requireGeometrySyncAvailable();
    const int no = step ? def::model::last_object_number(*step) + 1 : 1;     // 0x140c7c220: 0x1408301c0
    if (!step) throw Error(M2M_E_UNSUPPORTED, "engine: new object of a model without a step "
                                              "(0x1408bde10 dereferences it)");
    LLOBJ_DATA* o = def::model::llobj_ctor::get_or_create(step.get(), no);   // 1408bde10 = 14019ccc0
    def::model::llobj_reset_kind7(o, 0);                                      // 140834fe0 = 140160f50
    // @src 0x140c7c260  a deformable object exists (1401cac80 != 0)
    if (def::pre::step_first_deformable(step.get()) == 0) *at<std::int32_t>(o, 0xc) = 1;
    defaultObjectName(o, *at<std::int32_t>(o, 8));                            // 0x140833b40
    viewerSync(*this, o, true);                                               // 0x140c80280
    repickPdie(*this);                                                        // 0x140c7b630
    return o;
}

// @src 0x140c7ebe0  primitive table (sim control +0x440, curve 0xd): object type +0x3aa4 and the
// six primitive parameters +0x3ab0 (setters 0x1401b2660 / 0x1401b2470)
void primitiveTable(GuiModel& m)
{
    Cls_1401e7b10* sc = m.simCtl();
    if (!sc || !sc->furnace_on) return;
    const std::int32_t id = 0xd;
    def::model::DYNARY* t = sc->furnace.curve(&id);
    if (!t) return;
    const int n = *at<std::int32_t>(t, 0x1c);
    if (n <= 0) return;
    for (int i = 0; i < n; ++i) {
        const double d0 = def::model::llobj::dynary_get(*t, i, 0, 0, 0);
        LLOBJ_DATA* o = m.object(static_cast<int>(d0));
        if (!o) continue;
        *at<std::int32_t>(o, 0x3aa4) = static_cast<std::int32_t>(def::model::llobj::dynary_get(*t, i, 0, 0, 5));
        for (int k = 0; k < 6; ++k)
            *at<double>(o, 0x3ab0 + 8 * k) = def::model::llobj::dynary_get(*t, i, 0, 0, k + 6);
    }
}

// @src 0x140c7d3a0  furnace table (sim control +0x440, curve 0xe) into the GUI block +0xf8
void furnaceTableToBlock(GuiModel& m)
{
    Cls_1401e7b10* sc = m.simCtl();
    if (!sc || !sc->furnace_on) return;
    const std::int32_t id = 0xe;
    def::model::DYNARY* t = sc->furnace.curve(&id);                          // 1409f0660 = 140375980
    if (!t) return;
    auto v = [&](int row, int c) { return def::model::llobj::dynary_get(*t, row, 0, 0, c); };   // 140130720
    std::int32_t* b = m.tableF8;
    // row 0: (int) c=0,1,6..11 -> +0x00,+0x08,+0x10,+0x14,+0x18,+0x28,+0x2c,+0x30; doubles c=12..14
    // row 1: -> +0x04,+0x0c,+0x1c,+0x20,+0x24,+0x34,+0x38,+0x3c; doubles
    static const int cols[8] = {0, 1, 6, 7, 8, 9, 10, 11};
    static const int off0[8] = {0x00, 0x08, 0x10, 0x14, 0x18, 0x28, 0x2c, 0x30};
    static const int off1[8] = {0x04, 0x0c, 0x1c, 0x20, 0x24, 0x34, 0x38, 0x3c};
    for (int k = 0; k < 8; ++k) b[off0[k] / 4] = static_cast<std::int32_t>(v(0, cols[k]));
    m.tableF8d[0] = v(0, 12);
    m.tableF8d[1] = v(0, 13);
    m.tableF8d[2] = v(0, 14);
    for (int k = 0; k < 8; ++k) b[off1[k] / 4] = static_cast<std::int32_t>(v(1, cols[k]));
    m.tableF8d[3] = v(1, 12);
    m.tableF8d[4] = v(1, 13);
    m.tableF8d[5] = v(1, 14);
}

bool GuiModel::readKey(const String& path)
{
    requireGeometrySyncAvailable();
    if (!step) return false;
    const std::string local = engineKeyPath(path);
    const char reset = 1;
    const std::int32_t mode = 0;
    const int rc = def::cmd::read_keyword_file(step.get(), local.c_str(), &reset, &mode);   // 1408969a0
    if (rc == 0) {
        furnaceTableToBlock(*this);
        primitiveTable(*this);
    }
    return rc == 0;
}

Material* GuiModel::material(int no) const
{
    requireGeometrySyncAvailable();
    if (!step) return nullptr;
    return def::model::material_list_find(&step->materials, no);                // 14029c2c0
}

Material* GuiModel::readMaterial(const String& path, const int* reqNo)
{
    requireGeometrySyncAvailable();
    const std::string local = engineKeyPath(path);
    Material* m = def::cmd::key_import_material(step.get(), local.c_str(), *reqNo, false);   // 140897940
    viewerRebuild(*this, true, true);      // 0x140c7ff80(w, 1, 1)
    return m;
}

bool GuiModel::loadObjectKey(const String& path, const int* no)
{
    requireGeometrySyncAvailable();
    const std::string local = engineKeyPath(path);
    LLOBJ_DATA* o = def::cmd::key_import_object(step.get(), local.c_str(), *no);   // 140897cf0
    if (o) viewerSync(*this, o, true);      // 0x140c80280
    return o != nullptr;
}

bool GuiModel::loadObjectKeyGeometry(const String& path, const int* no, const bool* mark5)
{
    requireGeometrySyncAvailable();
    const std::string local = engineKeyPath(path);
    LLOBJ_DATA* o = def::cmd::key_import_object(step.get(), local.c_str(), *no);
    bool r = false;
    if (o) {
        if (objHasGeometry(o)) {                                      // 140834d50 = 140160cc0
            if (objHasMesh(o)) objDeleteMesh(o);                      // 140834c80 / 140873be0
            r = true;
        } else if (objHasMesh(o)) {
            char c = 1;
            objGeometryFromMesh(o, &c);                               // 140870590 = 1401fc730
            objDeleteMesh(o);
            r = true;
        }
        if (*mark5) *at<std::int32_t>(o, 0xc) = 5;
        viewerSync(*this, o, true);         // 0x140c80280
    }
    return r;
}

int GuiModel::pdie() const
{
    // Value-only diagnostic; do not grant the mutable simCtl() capability.
    const Cls_1401e7b10* sc = step ? &step->sim_ctrl : nullptr;
    return sc ? sc->pdie1 : 0;
}

void GuiModel::setPdie(int v)
{
    requireGeometrySyncAvailable();
    if (Cls_1401e7b10* sc = simCtl()) sc->pdie1 = v;
}

void GuiModel::setHeatTreatPropertyStepDefinition(int selection)
{
    if (auto* control = simCtl())
        control->stpdef = selection == 0 ? 1 : 3;
}

void GuiModel::setHeatTreatPropertyDeformation(bool enabled)
{
    if (auto* control = simCtl()) control->trans[1] = enabled ? 1 : 0;
}

void GuiModel::setHeatTreatPropertyDiffusion(bool enabled)
{
    if (auto* control = simCtl()) control->trans[3] = enabled ? 1 : 0;
}

void GuiModel::setHeatTreatPropertyPhaseTransformation(bool enabled)
{
    if (auto* control = simCtl()) control->trans[2] = enabled ? 1 : 0;
}

}  // namespace m2m::engine
