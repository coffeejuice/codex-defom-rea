// MDT L3: the own loader slots of SfOprForming, SfOprHeatTransferExpress(2D) and their props
// vt+0x58 on corpus operations. Expected values hand-read from the corpus files:
//   49_MO2_Boolean_Oprations op 1 (active, SfOprAirConvect2D): Task00001/Forming.FWZ has
//     FORMING_TYPE 2 and no HEATTRANS key (ctor -1, so 0x1413facf0 derives 1 from the sub type);
//     SimCtrl.KEY: NSTEP 200, STPINC 20, STPDEF 1, DTMAX 3.6, DTPMAX 0, TRANS 1 0, one object.
//   46_MO2_Boolean_Oprations op 2 (SfOprHeatResting2D): heat transfer type 2, two objects -> PDIE 2.
//   53_Cogging_Multiple_Ops_Lab1 op 4 (SfOprHeatExpress): type 0, one object (no merge).
//   56_Multiple_Operations_Example1 op 1 (active Forming): SimCtrl.KEY PDIE 2, NSTEP 100, STPINC 10,
//     DSMAX 1.27, TRANS 0 1; Equipments/Equipment00002.KEY: PRSNAM blank, MOVCTL type 1 ->
//     Equipment 0 (0x140c07eb0).
// ctest-labels: m2m-mdt m2m-mdt-l3
#include <testkit/check.hpp>

#include <cstdlib>
#include <filesystem>
#include <string>

#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>

#include "m2m/data/express.hpp"
#include "m2m/data/heattransfer.hpp"
#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/props_forming.hpp"
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

// OB vt+0x58 of an operation that is not the active item (as 0x140051340 runs it)
engine::GuiModel* loadOp(Project& p, Ctx& ctx, Operation& op, engine::FileEffects& fx)
{
    mst::installLoaderSlots();
    mst::OpClass* ob = opBase(op);
    engine::GuiModel* m = engine::opModel(p, op);
    m->reset();
    ob->current = &op;
    ob->loadOperation(p, op, ctx, fx);
    return m;
}

}  // namespace

TEST_CASE(air_convect_2d_active)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx, "49_MO2_Boolean_Oprations", "MO2_Boolean_Oprations.moproj") != nullptr);
    engine::FileEffects fx;
    Operation* op = engine::openActiveOperation(p, ctx, fx);
    REQUIRE(op != nullptr);
    CHECK_EQ(op->id, 1);
    CHECK_EQ(op->classKey.utf8(), std::string("SfOprHeatTransferExpress2D"));
    CHECK(fx.writes.empty() && fx.deletes.empty());
    auto* pe = dynamic_cast<HeatTransferExpressProps*>(op->props.get());
    REQUIRE(pe != nullptr && pe->data != nullptr && pe->step != nullptr);
    CHECK_EQ(pe->data->formingType, 2);
    CHECK_EQ(pe->data->heatTransType, 1);
    CHECK_EQ(pe->data->stepDefinition, 1);
    CHECK_EQ(pe->common->numSteps, 200);
    CHECK_EQ(pe->common->stepIncr, 20);
    CHECK_EQ(pe->step->stepDef, 0);
    CHECK_EQ(pe->step->stpInc, 20);
    CHECK(pe->step->dtMax == 3.6);
    CHECK(pe->step->dTempMax == 0.0);
    engine::GuiModel* m = engine::opModel(p, *op);
    REQUIRE(m != nullptr);
    CHECK_EQ(m->objectCount(), 1);
    CHECK_EQ(m->simCtl()->pdie1, 1);
    CHECK_EQ(m->simCtl()->trans[0], 1);
    CHECK_EQ(m->simCtl()->trans[1], 0);
    CHECK_EQ(op->lubricantId, 0);
}

TEST_CASE(heat_resting_2d)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx, "46_MO2_Boolean_Oprations", "MO2_Boolean_Oprations.moproj") != nullptr);
    Operation* op = operationById(p, 2);
    REQUIRE(op != nullptr);
    engine::FileEffects fx;
    engine::GuiModel* m = loadOp(p, ctx, *op, fx);
    auto* pe = dynamic_cast<ExpressProps*>(op->props.get());
    REQUIRE(pe != nullptr && pe->data != nullptr);
    CHECK_EQ(pe->data->heatTransType, 2);
    CHECK_EQ(m->objectCount(), 2);
    CHECK_EQ(m->simCtl()->pdie1, 2);
}

TEST_CASE(heat_express_3d)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx, "53_Cogging_Multiple_Ops_Lab1", "Cogging_Multiple_Ops_Lab1.moproj") != nullptr);
    Operation* op = operationById(p, 4);
    REQUIRE(op != nullptr);
    engine::FileEffects fx;
    engine::GuiModel* m = loadOp(p, ctx, *op, fx);
    auto* pe = dynamic_cast<ExpressProps*>(op->props.get());
    REQUIRE(pe != nullptr && pe->data != nullptr);
    CHECK_EQ(pe->data->heatTransType, 0);
    CHECK_EQ(m->objectCount(), 1);
    CHECK_EQ(m->simCtl()->pdie1, 1);
}

TEST_CASE(forming_active)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx, "56_Multiple_Operations_Example1", "Multiple_Operations_Example1.moproj") != nullptr);
    engine::FileEffects fx;
    Operation* op = engine::openActiveOperation(p, ctx, fx);
    REQUIRE(op != nullptr);
    CHECK_EQ(op->classKey.utf8(), std::string("SfOprForming"));
    auto* fp = dynamic_cast<FormingProps*>(op->props.get());
    REQUIRE(fp != nullptr);
    CHECK(fp->d.disabled);
    CHECK_EQ(fp->d.type, 0);
    CHECK(fp->d.name.isEmpty());
    CHECK_EQ(fp->common->numSteps, 100);
    CHECK_EQ(fp->common->stepIncr, 10);
    CHECK_EQ(fp->common->dsmax.utf8(), std::string("1.27 mm"));
    engine::GuiModel* m = engine::opModel(p, *op);
    CHECK_EQ(m->simCtl()->pdie1, 2);
    CHECK_EQ(m->simCtl()->trans[0], 0);
    CHECK_EQ(m->simCtl()->trans[1], 1);
    REQUIRE(!fp->tree.empty());
    const PropNode* et = findProp(fp->tree[0].children, qt::L("EquipmentType"));
    REQUIRE(et != nullptr);
    CHECK_EQ(static_cast<int>(et->children.size()), 2);
    const PropNode* es = findProp(fp->tree[0].children, qt::L("EquipmentSpec"));
    REQUIRE(es != nullptr);
    CHECK(es->children.empty());                   // type 0 has no spec children (0x140c0a270)
}

// 0x140c06130 creates the equipment (packet S createEquipment; oracle B 96 in m2m.s.postload_views)
TEST_CASE(forming_without_equipment_creates_one)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx, "56_Multiple_Operations_Example1", "Multiple_Operations_Example1.moproj") != nullptr);
    Operation* op = operationById(p, 1);
    REQUIRE(op != nullptr);
    op->equipmentId = -1;                          // 0x140c06130 then creates an equipment item
    engine::FileEffects fx;
    int code = 0;
    try {
        loadOp(p, ctx, *op, fx);
    } catch (const Error& e) {
        code = e.code;
    }
    CHECK_EQ(code, 0);
    CHECK(op->equipmentId > 0);
}

int main()
{
    return testkit::run_all();
}
