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

#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <utility>

#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/window/inter_object_ops.hpp>

#include "../../src/engine/engine_internal.hpp"
#include "../../src/ops/express.hpp"
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

TEST_CASE(forming_express_dimension_flag_command)
{
    for (const int dimension : {2, 3, 7}) {
        engine::GuiModel model(nullptr, dimension);
        auto* step = model.step.get();
        REQUIRE(step != nullptr);
        *engine::at<std::int32_t>(step, 0xa34) = -11;
        *engine::at<std::int32_t>(step, 0xa68) = -12;
        *engine::at<std::int32_t>(step, 0xa30) = -13;
        *engine::at<std::int32_t>(step, 0x1118) = 77;

        model.setFormingExpressDimensionFlags();

        const std::int32_t expected = dimension == 2 ? 1 : 2;
        CHECK_EQ(*engine::at<std::int32_t>(step, 0xa34), expected);
        CHECK_EQ(*engine::at<std::int32_t>(step, 0xa68), expected);
        CHECK_EQ(*engine::at<std::int32_t>(step, 0xa30), expected);
        CHECK_EQ(*engine::at<std::int32_t>(step, 0x1118), dimension == 2 ? 0 : 77);
    }
}

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

    // Exercise the admitted operation's actual cached step, with distinct TNOW branches.
    REQUIRE(m->step != nullptr);
    engine::DB_STEP_DATA* originalStep = m->step.get();
    const double heatingTime = engine::propsHeatingTime(*op->props);
    auto& sc = m->step->sim_ctrl;
    sc.tnow1 = 13.0;
    sc.tnow2 = 29.0;
    sc.tnow3 = 0;
    sc.tmax = -101.0;
    engine::hteSaveTmax(p, *op);
    CHECK_EQ(sc.tmax, sc.tnow1 + heatingTime);
    sc.tnow3 = 1;
    sc.tmax = -103.0;
    engine::hteSaveTmax(p, *op);
    CHECK_EQ(sc.tmax, sc.tnow2 + heatingTime);

    // Internal synthetic null-step coverage; public Save admission is not exercised here.
    sc.tmax = -107.0;
    auto savedStep = std::move(m->step);
    try {
        engine::hteSaveTmax(p, *op);
    } catch (...) {
        m->step = std::move(savedStep);
        throw;
    }
    m->step = std::move(savedStep);
    CHECK(engine::opModel(p, *op) == m);
    CHECK(m->step.get() == originalStep);
    CHECK_EQ(m->step->sim_ctrl.tmax, -107.0);
    CHECK(fx.writes.empty() && fx.deletes.empty());
}

// Internal helper mechanics on a HeatTransferExpress fixture. Production classDefaultPairs
// dispatches this helper for FormingExpress; this is not a public FormingExpress Save test.
TEST_CASE(fe_default_pairs_step_scope)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx, "49_MO2_Boolean_Oprations", "MO2_Boolean_Oprations.moproj") != nullptr);
    engine::FileEffects fx;
    Operation* op = engine::openActiveOperation(p, ctx, fx);
    REQUIRE(op != nullptr && op->props != nullptr && op->props->common != nullptr);
    engine::GuiModel* m = engine::opModel(p, *op);
    REQUIRE(m != nullptr && m->step != nullptr);
    REQUIRE(m->objectCount() == 1);
    auto* object = m->objectAt(1);
    REQUIRE(object != nullptr);
    REQUIRE(*engine::at<std::int32_t>(object, 0xc) != 7);
    REQUIRE(*engine::at<std::int32_t>(m->step.get(), 0x1c) == 2);
    engine::DB_STEP_DATA* originalStep = m->step.get();
    REQUIRE(def::model::pair_append(&m->step->inter_obj) != nullptr);
    REQUIRE(m->step->inter_obj.count > 0);
    const double friction = m2m::propsFriction(*op->props);
    const double ihtc = m2m::propsIhtc(*op->props);
    const auto pairs = engine::feDefaultPairs(p, *op);
    REQUIRE(pairs.size() == 1);
    CHECK_EQ(pairs[0].master, 1);
    CHECK_EQ(pairs[0].slave, 1);
    CHECK_EQ(pairs[0].lub, 0);
    CHECK_EQ(pairs[0].f19, 1);
    CHECK_EQ(pairs[0].frc, friction);
    CHECK_EQ(pairs[0].ihtc, ihtc);
    CHECK_EQ(m->step->inter_obj.count, 0);

    // Keep a real nonempty list on the detached step so null handling cannot clear it.
    REQUIRE(def::model::pair_append(&m->step->inter_obj) != nullptr);
    const auto savedCount = m->step->inter_obj.count;
    REQUIRE(savedCount > 0);
    auto savedStep = std::move(m->step);
    try {
        CHECK(engine::feDefaultPairs(p, *op).empty());
        CHECK_EQ(savedStep->inter_obj.count, savedCount);
    } catch (...) {
        m->step = std::move(savedStep);
        throw;
    }
    m->step = std::move(savedStep);
    CHECK(engine::opModel(p, *op) == m);
    CHECK(m->step.get() == originalStep);
    CHECK(m->objectAt(1) == object);
    CHECK_EQ(m->step->inter_obj.count, savedCount);
    CHECK(fx.writes.empty() && fx.deletes.empty());
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

    // Direct helper coverage: two eligible 2D objects must retain ascending master order.
    REQUIRE(m->step != nullptr && op->props != nullptr && op->props->common != nullptr);
    REQUIRE(*engine::at<std::int32_t>(m->step.get(), 0x1c) == 2);
    for (int i = 1; i <= 2; ++i) {
        auto* object = m->objectAt(i);
        REQUIRE(object != nullptr);
        REQUIRE(*engine::at<std::int32_t>(object, 0xc) != 7);
    }
    const auto pairs = engine::feDefaultPairs(p, *op);
    REQUIRE(pairs.size() == 2);
    for (int i = 0; i < 2; ++i) {
        CHECK_EQ(pairs[i].master, i + 1);
        CHECK_EQ(pairs[i].slave, 1);
        CHECK_EQ(pairs[i].lub, 0);
        CHECK_EQ(pairs[i].f19, 1);
        CHECK_EQ(pairs[i].frc, m2m::propsFriction(*op->props));
        CHECK_EQ(pairs[i].ihtc, m2m::propsIhtc(*op->props));
    }
    CHECK_EQ(m->step->inter_obj.count, 0);
    CHECK(fx.writes.empty() && fx.deletes.empty());
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
    REQUIRE(m->simCtl() != nullptr);
    CHECK_EQ(m->simCtl()->pdie1, 1);

    // Exercise the admitted model's real eligible object and a nonzero window.
    auto* object = m->objectAt(1);
    REQUIRE(object != nullptr);
    const auto kind = *engine::at<std::int32_t>(object, 0xc);
    REQUIRE(kind != 0 && kind != 3);
    REQUIRE(m->step != nullptr);
    engine::DB_STEP_DATA* originalStep = m->step.get();
    auto& lower = *engine::at<double>(object, 0x1ef8);
    auto& upper = *engine::at<double>(object, 0x1f00);
    lower = 83.0;
    upper = 19.0;
    m->step->sim_ctrl.envtmp.value = 211.0;
    engine::expressSaveOperation(p, *op);
    CHECK_EQ(lower, 179.0);
    CHECK_EQ(upper, 243.0);

    // Internal synthetic null-step coverage; public Save admission is not exercised here.
    lower = -109.0;
    upper = 307.0;
    auto savedStep = std::move(m->step);
    try {
        engine::expressSaveOperation(p, *op);
    } catch (...) {
        m->step = std::move(savedStep);
        throw;
    }
    m->step = std::move(savedStep);
    CHECK(engine::opModel(p, *op) == m);
    CHECK(m->step.get() == originalStep);
    CHECK(m->objectAt(1) == object);
    CHECK_EQ(lower, -109.0);
    CHECK_EQ(upper, 307.0);
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
