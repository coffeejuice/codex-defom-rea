// MDT L2: own loader slots of SfOprHeatTransfer, SfOprHTFurnace and SfOprPre on corpus 08_HEAT1
// (op 1 "HeatTransfer", Transfer through air, slot 1 = object 1; op 2 "HeatTransfer (2)", Heat in
// furnace, slot 1 inherited, no SimCtrl.KEY). Expected values are hand-read from
// Operations/Task00001/SimCtrl.KEY (UNIT 1, TRANS 1 0, STNOUT 0 0 0 0 1 0 0, STPINC 1,
// NSTEP 1, STPDEF 1, DTMAX 0.1, DTPMAX 0 0 0, ENVTMP 20, CNVCOF 0.02), the unit names of the
// binary's table 0x1427bc000 (time "sec", temperature "C", 0x20 "N/sec/mm/C") and the constants
// of 0x141358150 (Transfer through air: 15 s, ENVTMP 20 in SI) and 0x1413558b0 / 0x141354ce0.
// ctest-labels: m2m-mdt m2m-mdt-l2
#include <testkit/check.hpp>

#include <cstdlib>
#include <filesystem>
#include <string>

#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/cls_1403753f0.hpp>
#include <defmodel/layout/db_step_data.hpp>

#include "m2m/data/common.hpp"
#include "m2m/data/heattransfer.hpp"
#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/loader_l2.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/p17.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4path.hpp"

namespace m2m::mst {
void installLoaderSlots();
}

namespace fs = std::filesystem;
using namespace m2m;

namespace {

Project* openCorpus(Project& p, Ctx& ctx, const char* caseDir, const char* moproj)
{
    const char* corpus = std::getenv("M2M_CORPUS");
    if (!corpus) return nullptr;
    const fs::path f = fs::path(corpus) / caseDir / moproj;
    const qt::String file{f.u16string()};
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(p);
    if (!openProject(p, file, ctx) || !p.openedOk) return nullptr;
    activateSubProject(p, 0, ctx);
    return &p;
}

void load(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx)
{
    mst::installLoaderSlots();
    mst::OpClass* c = opBase(op);
    c->current = &op;
    c->loadOperation(p, op, ctx, fx);
}

}  // namespace

TEST_CASE(heat1_active_heat_transfer)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx, "08_HEAT1", "HEAT1.moproj") != nullptr);
    engine::FileEffects fx;
    Operation* op = engine::openActiveOperation(p, ctx, fx);
    REQUIRE(op != nullptr);
    CHECK_EQ(op->id, 1);
    CHECK_EQ(op->subType.utf8(), std::string("Transfer through air"));   // index 1, kept
    engine::GuiModel* m = engine::opModel(p, *op);
    REQUIRE(m != nullptr);
    def::model::Cls_1401e7b10* sc = m->simCtl();
    CHECK_EQ(sc->trans[0], 1);
    CHECK_EQ(sc->trans[1], 0);
    auto* pr = dynamic_cast<HeatTransferProps*>(op->props.get());
    REQUIRE(pr != nullptr);
    CHECK_EQ(pr->common->numSteps, 1);
    CHECK_EQ(pr->common->stepIncr, 1);
    CHECK_EQ(pr->common->dtmax.utf8(), std::string("0.1 sec"));
    CHECK_EQ(pr->common->envTemp.utf8(), std::string("20 C"));
    CHECK_EQ(pr->common->convection.utf8(), std::string("0.02 N/sec/mm/C"));
    CHECK_EQ(pr->step->stepDef, 0);
    CHECK_EQ(pr->step->stpInc, 1);
    CHECK(pr->step->dtMax == 0.1);
    CHECK(pr->step->dTempMax == 0.0);
    CHECK(pr->step->dTempLmt0 == 0.0);
    CHECK(pr->step->dTempLmt1 == 0.0);
    CHECK(fx.writes.empty() && fx.deletes.empty());
}

// Sub type missing: the predecessor's "Transfer through air" (index 1) gives "Rest on die"
TEST_CASE(heat1_subtype_from_predecessor)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx, "08_HEAT1", "HEAT1.moproj") != nullptr);
    Operation* op = operationById(p, 2);
    REQUIRE(op != nullptr);
    op->subType = qt::String();
    engine::FileEffects fx;
    load(p, *op, ctx, fx);
    CHECK_EQ(op->subType.utf8(), std::string("Rest on die"));
}

// OprFlag 0 with a "Heat in furnace" predecessor: sub type "Transfer through air", one slot, then
// props vt+0x120 (15 s, ENVTMP 20) and STNOUT/ELMNOD from the predecessor's SimCtrl.KEY
TEST_CASE(heat1_first_setup_defaults)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx, "08_HEAT1", "HEAT1.moproj") != nullptr);
    Operation* op1 = operationById(p, 1);
    Operation* op = operationById(p, 2);
    REQUIRE(op1 != nullptr && op != nullptr);
    op1->subType = qt::L("Heat in furnace");
    op->oprFlag = 0;
    engine::FileEffects fx;
    load(p, *op, ctx, fx);
    CHECK_EQ(op->subType.utf8(), std::string("Transfer through air"));
    def::model::Cls_1401e7b10* sc = engine::opModel(p, *op)->simCtl();
    CHECK(sc->tmax == 15.0);
    CHECK_EQ(sc->envtmp.ndim, 0);
    CHECK(sc->envtmp.value == 20.0);
    CHECK_EQ(op->props->common->heatingTime.utf8(), std::string("15 sec"));
    // STNOUT/ELMNOD are copied from op 1's SimCtrl.KEY, whose values (STNOUT 0 0 0 0 1 0 0,
    // ELMNOD count 10 of zeros) equal the sim-control defaults, so the copy is not observable here.
    CHECK_EQ(sc->stnout[4], 1);
}

// Exercise the internal defaults through its existing postload slot. This entry
// selects Transfer before defaults; it cannot independently cover the other subtypes.
TEST_CASE(heat_transfer_subtype_defaults_step_scope)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx, "08_HEAT1", "HEAT1.moproj") != nullptr);
    Operation* op = operationById(p, 1);
    REQUIRE(op != nullptr);
    auto* pr = dynamic_cast<HeatTransferProps*>(op->props.get());
    REQUIRE(pr != nullptr && pr->owner == op && pr->common != nullptr);
    engine::FileEffects fx;
    load(p, *op, ctx, fx);
    engine::GuiModel* model = engine::opModel(p, *op);
    REQUIRE(model != nullptr && op->engineModel.get() == model);
    auto* step = model->step.get();
    auto* sc = model->simCtl();
    REQUIRE(step != nullptr && sc != nullptr);
    mst::OpClass* cls = opBase(*op);
    REQUIRE(cls != nullptr && mst::g_loaderSlotsL2.htPostLoad != nullptr);
    cls->current = op;
    const auto common = pr->common;

    struct Row { int unit; double time; double ambient; const char* heatingTime; };
    // Independent literals: model/units_table.inc, kind 0, has "sec" for SI and EN;
    // props_sync.cpp's valueWithUnit retains the supplied time (no conversion).
    const Row rows[] = {{1, 15, 20, "15 sec"}, {2, 15, 68, "15 sec"}};
    for (const Row& row : rows) {
        op->oprFlag = 0;
        sc->unit = row.unit;
        sc->tmax = -101;
        sc->envtmp.ndim = 7;
        sc->envtmp.value = -202;
        sc->nstep = 37;
        pr->common->heatingTime = qt::L("heating sentinel");
        mst::g_loaderSlotsL2.htPostLoad(*cls, p, *op, ctx, fx);
        CHECK_EQ(op->subType.utf8(), std::string("Transfer through air"));
        CHECK(sc->tmax == row.time);
        CHECK_EQ(sc->envtmp.ndim, 0);
        CHECK(sc->envtmp.value == row.ambient);
        CHECK_EQ(pr->common->heatingTime.utf8(), std::string(row.heatingTime));
        CHECK_EQ(sc->nstep, 37);
        CHECK_EQ(sc->unit, row.unit);
        CHECK(pr->owner == op && pr->common == common);
        CHECK(op->engineModel.get() == model && engine::opModel(p, *op) == model);
        CHECK(model->step.get() == step && model->simCtl() == sc);
    }
    CHECK(fx.writes.empty() && fx.deletes.empty());
}

// Direct postload calls make the predecessor KEY copy observable and isolate the
// present-object early return from the rest of the operation loader.
TEST_CASE(heat_transfer_postload_step_scope)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx, "08_HEAT1", "HEAT1.moproj") != nullptr);
    Operation* prev = operationById(p, 1);
    Operation* op = operationById(p, 2);
    REQUIRE(prev != nullptr && op != nullptr);
    prev->subType = qt::L("Heat in furnace");
    op->oprFlag = 0;
    engine::FileEffects fx;
    load(p, *op, ctx, fx);
    auto* pr = dynamic_cast<HeatTransferProps*>(op->props.get());
    REQUIRE(pr != nullptr && pr->owner == op && pr->common != nullptr);
    engine::GuiModel* model = engine::opModel(p, *op);
    REQUIRE(model != nullptr && op->engineModel.get() == model);
    auto* step = model->step.get();
    auto* sc = model->simCtl();
    REQUIRE(step != nullptr && sc != nullptr);
    mst::OpClass* cls = opBase(*op);
    REQUIRE(cls != nullptr && mst::g_loaderSlotsL2.htPostLoad != nullptr);
    cls->current = op;
    const auto common = pr->common;
    const int slot = objId(*op, 0);
    const int count = model->objectCount();
    REQUIRE(slotHoldsObject(slot) && count >= 1);
    auto* object = model->object(1); // slotObject's existing slot-0 lookup
    REQUIRE(object != nullptr);
    const auto writes = fx.writes.size(), deletes = fx.deletes.size();
    const auto mkdirs = fx.mkdirs.size(), renames = fx.renames.size();
    const auto directories = fx.directoryAttempts.size(), removals = fx.removalAttempts.size();
    const auto checkOwnersAndEffects = [&] {
        CHECK(operationById(p, 1) == prev && operationById(p, 2) == op);
        CHECK(opBase(*op) == cls && cls->current == op);
        CHECK(op->props.get() == pr && pr->owner == op && pr->common == common);
        CHECK(op->engineModel.get() == model && engine::opModel(p, *op) == model);
        CHECK(model->step.get() == step && model->simCtl() == sc);
        CHECK_EQ(objId(*op, 0), slot);
        CHECK(model->object(1) == object);
        CHECK_EQ(model->objectCount(), count);
        CHECK_EQ(op->subType.utf8(), std::string("Transfer through air"));
        CHECK_EQ(prev->subType.utf8(), std::string("Heat in furnace"));
        CHECK_EQ(sc->unit, 1); CHECK_EQ(sc->nstep, 37);
        CHECK_EQ(fx.writes.size(), writes); CHECK_EQ(fx.deletes.size(), deletes);
        CHECK_EQ(fx.mkdirs.size(), mkdirs); CHECK_EQ(fx.renames.size(), renames);
        CHECK_EQ(fx.directoryAttempts.size(), directories);
        CHECK_EQ(fx.removalAttempts.size(), removals);
    };
    const auto seed = [&] {
        for (int i = 0; i < 7; ++i) sc->stnout[i] = 70 + i;
        for (int i = 0; i < 10; ++i) sc->elmnod[i] = 90 + i;
        sc->unit = 1;
        sc->nstep = 37;
        sc->tmax = -101;
        sc->envtmp.ndim = 7;
        sc->envtmp.value = -202;
        pr->common->heatingTime = qt::L("postload heating sentinel");
    };
    seed();
    op->subType = qt::L("Transfer through air");
    mst::g_loaderSlotsL2.htPostLoad(*cls, p, *op, ctx, fx);
    // Hand-read Task00001/SimCtrl.KEY literals, independent of the production reader.
    const int expectedStnout[] = {0, 0, 0, 0, 1, 0, 0};
    const int expectedElmnod[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < 7; ++i) CHECK_EQ(sc->stnout[i], expectedStnout[i]);
    for (int i = 0; i < 10; ++i) CHECK_EQ(sc->elmnod[i], expectedElmnod[i]);
    CHECK(sc->tmax == 15);
    CHECK_EQ(sc->envtmp.ndim, 0);
    CHECK(sc->envtmp.value == 20);
    CHECK_EQ(pr->common->heatingTime.utf8(), std::string("15 sec"));
    CHECK_EQ(op->oprFlag, 0);
    checkOwnersAndEffects();

    seed();
    op->oprFlag = 1;
    REQUIRE(slotHoldsObject(objId(*op, 0)) && model->objectCount() >= 1);
    REQUIRE(model->object(1) == object);
    mst::g_loaderSlotsL2.htPostLoad(*cls, p, *op, ctx, fx);
    for (int i = 0; i < 7; ++i) CHECK_EQ(sc->stnout[i], 70 + i);
    for (int i = 0; i < 10; ++i) CHECK_EQ(sc->elmnod[i], 90 + i);
    CHECK(sc->tmax == -101);
    CHECK_EQ(sc->envtmp.ndim, 7);
    CHECK(sc->envtmp.value == -202);
    CHECK_EQ(pr->common->heatingTime.utf8(), std::string("postload heating sentinel"));
    CHECK_EQ(op->oprFlag, 1);
    checkOwnersAndEffects();
}

// The same operation as an HT furnace: furnace on, TNOW 3, furnace model and data into the props
TEST_CASE(heat1_as_furnace)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx, "08_HEAT1", "HEAT1.moproj") != nullptr);
    Operation* op = operationById(p, 1);
    REQUIRE(op != nullptr);
    op->classKey = qt::L("SfOprHTFurnace");
    op->opClass = nullptr;
    op->props = htFurnaceProps(1);
    op->props->owner = op;
    engine::FileEffects fx;
    load(p, *op, ctx, fx);
    def::model::Cls_1401e7b10* sc = engine::opModel(p, *op)->simCtl();
    CHECK_EQ(sc->furnace_on, 1);
    CHECK_EQ(sc->tnow3, 1);
    CHECK_EQ(sc->trans[0], 1);
    CHECK(sc->furnace.curve_count() > 0);
    auto* pr = dynamic_cast<HTFurnaceProps*>(op->props.get());
    REQUIRE(pr != nullptr);
    CHECK_EQ(pr->data->name.utf8(), std::string(sc->furnace.name));
    CHECK_EQ(pr->furnace->curve_count(), sc->furnace.curve_count());
    CHECK_EQ(pr->data->stepDef, 0);                         // STPDEF 1
    CHECK_EQ(pr->data->stpInc, 1);
    CHECK(pr->data->dtMax == 0.1);
    CHECK(!pr->data->viewFactor);                           // VFACTR 0
}

// Direct same-owner slot calls isolate the empty and populated furnace branches
// from the later property copy performed by a full operation load.
TEST_CASE(furnace_postload_step_scope)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx, "08_HEAT1", "HEAT1.moproj") != nullptr);
    Operation* op = operationById(p, 1);
    REQUIRE(op != nullptr);
    op->classKey = qt::L("SfOprHTFurnace");
    op->opClass = nullptr;
    op->props = htFurnaceProps(1);
    op->props->owner = op;
    engine::FileEffects fx;
    load(p, *op, ctx, fx);
    engine::GuiModel* model = engine::opModel(p, *op);
    REQUIRE(model != nullptr && op->engineModel.get() == model);
    auto* step = model->step.get();
    auto* sc = model->simCtl();
    REQUIRE(step != nullptr && sc != nullptr);
    auto* pr = dynamic_cast<HTFurnaceProps*>(op->props.get());
    REQUIRE(pr != nullptr && pr->owner == op && pr->furnace != nullptr && pr->data != nullptr);
    mst::OpClass* cls = opBase(*op);
    REQUIRE(cls != nullptr && mst::g_loaderSlotsL2.furnacePostLoad != nullptr);
    cls->current = op;
    const auto* propsFurnace = pr->furnace.get();
    const auto* propsData = pr->data.get();
    const auto propsCount = pr->furnace->curve_count();
    const std::string propsName = pr->furnace->name;
    const auto dataName = pr->data->name;
    const auto writes = fx.writes.size(), deletes = fx.deletes.size();
    const auto mkdirs = fx.mkdirs.size(), renames = fx.renames.size();
    const auto directories = fx.directoryAttempts.size(), removals = fx.removalAttempts.size();
    const auto checkOwnersAndEffects = [&] {
        CHECK(op->engineModel.get() == model && engine::opModel(p, *op) == model);
        CHECK(model->step.get() == step && model->simCtl() == sc);
        CHECK(op->props.get() == pr && pr->owner == op);
        CHECK(pr->furnace.get() == propsFurnace && pr->data.get() == propsData);
        CHECK_EQ(pr->furnace->curve_count(), propsCount);
        CHECK_EQ(std::string(pr->furnace->name), propsName);
        CHECK(pr->data->name == dataName);
        CHECK_EQ(sc->nstep, 37);
        CHECK_EQ(fx.writes.size(), writes); CHECK_EQ(fx.deletes.size(), deletes);
        CHECK_EQ(fx.mkdirs.size(), mkdirs); CHECK_EQ(fx.renames.size(), renames);
        CHECK_EQ(fx.directoryAttempts.size(), directories);
        CHECK_EQ(fx.removalAttempts.size(), removals);
    };

    sc->furnace.clear_curves();
    REQUIRE(sc->furnace.curve_count() == 0);
    sc->furnace_on = 0;
    sc->nstep = 37;
    mst::g_loaderSlotsL2.furnacePostLoad(*cls, p, *op, ctx, fx);
    CHECK_EQ(sc->furnace_on, 1);
    // setup(0x16) makes 21 fixed entries plus one extra; the first default
    // shape is independently recorded by the furnace member's shape table.
    CHECK_EQ(sc->furnace.curve_count(), 22);
    const std::int32_t firstIndex = 1;
    auto* first = sc->furnace.curve(&firstIndex);
    REQUIRE(first != nullptr);
    CHECK_EQ(first->ndim, 1);
    CHECK_EQ(first->n[0], 2);
    CHECK_EQ(first->ncomp, 6);
    checkOwnersAndEffects();

    const auto count = sc->furnace.curve_count();
    first->value = 77.123456789;
    first->u54 = 91;
    for (int repeat = 0; repeat < 2; ++repeat) {
        sc->furnace_on = 0;
        mst::g_loaderSlotsL2.furnacePostLoad(*cls, p, *op, ctx, fx);
        CHECK_EQ(sc->furnace_on, 1);
        CHECK_EQ(sc->furnace.curve_count(), count);
        auto* current = sc->furnace.curve(&firstIndex);
        REQUIRE(current == first);
        CHECK(current->value == 77.123456789);
        CHECK_EQ(current->u54, 91);
        checkOwnersAndEffects();
    }
}

// SfOprPre: its own load is empty, so nothing is read into the model
TEST_CASE(heat1_as_pre)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx, "08_HEAT1", "HEAT1.moproj") != nullptr);
    Operation* op = operationById(p, 1);
    REQUIRE(op != nullptr);
    op->classKey = qt::L("SfOprPre3D");
    op->opClass = nullptr;
    engine::FileEffects fx;
    load(p, *op, ctx, fx);
    engine::GuiModel* m = engine::opModel(p, *op);
    REQUIRE(m != nullptr);
    CHECK_EQ(m->objectCount(), 0);
    CHECK_EQ(m->simCtl()->stpinc, 10);                      // init default (KEY: 1), not read
    // init default, no prologue ran: the GUI init 0x1408d3f50 takes TRANS from the options globals
    // (0 in a GUI whose options dialog was not applied), not DEF_PRE's TRANS 2 = 1
    CHECK_EQ(m->simCtl()->trans[1], 0);
}

int main()
{
    return testkit::run_all();
}
