#include "../support/test_paths.hpp"
// MDT L4: the MultiBlow load slots (0x140601f70, 0x140602760, 0x140602010, props 0x140612ba0 with
// the base readers 0x1405fb720 / 0x1405fbae0 / 0x1405fc180 / 0x1405fbe30). No corpus project has a
// MultiBlow operation, so corpus 42_vdie's cogging operation (Id 4, slots -2,-2,-2,10,11,12,13) is
// given the class SfOprMultiBlow and fresh MultiBlow properties. Expected values:
//   constants of the slots (assembly slots.asm, post.asm: NSTEP 0x2710, PDIE = vt+0x120 = 2);
//   Task00004/SimCtrl.KEY, hand-read: UNIT 1, NSTEP 100000, STPINC 100000, DSMAX 2.0, DTTYPE 0 0,
//   ENVTMP 0 20.0, CNVCOF 0 0.02; unit names of 0x140917de0 for SI: 0 "sec", 1 "mm", 9 "C",
//   0x20 "N/sec/mm/C"; QString::arg(double, 0, 'g', -1) gives "2", "20", "0.02", "0.5", "1.25".
// ctest-labels: m2m-mdt-l4
#include <testkit/check.hpp>

#include <cstdlib>
#include <filesystem>
#include <string>

#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>

#include "m2m/engine.hpp"
#include "../../src/engine/engine_internal.hpp"
#include "../../src/engine/props_sync.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
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

// Operation 4 of 42_vdie as a 3D MultiBlow operation with the class's default properties
Operation* multiBlowOp(Project& p)
{
    Operation* op = operationById(p, 4);
    if (!op) return nullptr;
    op->classKey = qt::L("SfOprMultiBlow3D");   // registry key of SfOprMultiBlow (0x140601410)
    mst::OpClass* c = opBase(*op);
    if (!c || c->key().u16() != u"SfOprMultiBlow3D") return nullptr;
    setOprProps(*op, c->makeProps(p.unit));
    return op;
}

}  // namespace

TEST_CASE(multiblow_prologue_and_class_data)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx, "42_vdie", "vdie.moproj") != nullptr);
    mst::installLoaderSlots();
    Operation* op = multiBlowOp(p);
    REQUIRE(op != nullptr);
    mst::OpClass* c = opBase(*op);
    c->current = op;
    engine::GuiModel* m = engine::opModel(p, *op);
    REQUIRE(m != nullptr);
    m->reset();
    def::model::Cls_1401e7b10* sc = m->simCtl();
    REQUIRE(sc != nullptr);
    sc->dttype.u58 = 1;
    sc->trans[0] = sc->trans[1] = 0;
    c->loadPrologue(p, *op);
    CHECK_EQ(sc->tnow3, 1);                 // base 0x1405f1bb0
    CHECK(sc->tnow2 == 0.0);
    CHECK_EQ(sc->trans[0], 1);
    CHECK_EQ(sc->trans[1], 1);
    CHECK_EQ(sc->dttype.u58, 0);            // 0x1408da6f0(sc, &0)
    CHECK_EQ(sc->nstep, 10000);             // MOV [RAX+0x16c], 0x2710
    const int before = sc->nstep;
    c->loadClassData(p, *op);               // 0x140602760: nothing
    CHECK_EQ(sc->nstep, before);
    CHECK(!c->restartCapable());            // vt+0x1f8 = 0x1405f79a0
}

TEST_CASE(multiblow_props_readers)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx, "42_vdie", "vdie.moproj") != nullptr);
    mst::installLoaderSlots();
    Operation* op = multiBlowOp(p);
    REQUIRE(op != nullptr);
    engine::GuiModel* m = engine::opModel(p, *op);
    REQUIRE(m != nullptr);
    def::model::Cls_1401e7b10* sc = m->simCtl();
    REQUIRE(sc != nullptr);
    sc->unit = 1;
    sc->nstep = 123;
    sc->stpinc = 7;
    sc->dtmax1 = 0.5;
    sc->dsmax = 1.25;
    sc->envtmp.value = 20.0;
    sc->cnvcof.value = 0.02;
    sc->dttype.u58 = 0;
    opBase(*op)->propsLoad(p, *op);
    const data::CommonBlock& b = *op->props->common;
    CHECK_EQ(b.numSteps, 123);
    CHECK_EQ(b.stepIncr, 7);
    CHECK_EQ(b.dtmax.utf8(), std::string("0.5 sec"));
    CHECK_EQ(b.envTemp.utf8(), std::string("20 C"));
    CHECK_EQ(b.dtMode, 0);
    CHECK_EQ(b.dsmax.utf8(), std::string("1.25 mm"));
    CHECK_EQ(b.convection.utf8(), std::string("0.02 N/sec/mm/C"));

    // mode 1: TimePerStep instead of DistPerStep (0x1405fc180 else branch)
    sc->dttype.u58 = 1;
    sc->dtmax1 = 0.25;
    sc->dsmax = 9.0;
    CHECK(engine::propsStepByFromSimCtl(p, *op->props, nullptr));
    CHECK_EQ(b.dtMode, 1);
    CHECK_EQ(b.dtmax.utf8(), std::string("0.25 sec"));
    CHECK_EQ(b.dsmax.utf8(), std::string("1.25 mm"));

    // English unit system: unit names of the second table
    sc->unit = 2;
    CHECK(engine::propsEnvTempFromSimCtl(p, *op->props, nullptr));
    CHECK_EQ(b.envTemp.utf8(), std::string("20 F"));
}

TEST_CASE(multiblow_load_operation)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx, "42_vdie", "vdie.moproj") != nullptr);
    mst::installLoaderSlots();
    Operation* op = multiBlowOp(p);
    REQUIRE(op != nullptr);
    REQUIRE(op->oprFlag != 0);              // SimCtrl.KEY is read (0x1400594d0)
    op->lubricantId = 5;
    mst::OpClass* c = opBase(*op);
    c->current = op;
    engine::FileEffects fx;
    c->loadOperation(p, *op, ctx, fx);      // OB vt+0x58 = 0x1405ef430
    engine::GuiModel* m = engine::opModel(p, *op);
    REQUIRE(m != nullptr);
    CHECK(m->objectCount() >= 3);           // post-load adds nothing
    def::model::Cls_1401e7b10* sc = m->simCtl();
    CHECK_EQ(sc->pdie1, 2);                 // vt+0x120
    CHECK_EQ(sc->nstep, 100000);            // SimCtrl.KEY over the prologue's 10000
    ObjEntry* e2 = objEntryN(*op, 2);
    REQUIRE(e2 != nullptr);
    CHECK(e2->assignMovement);              // 0x14004e0e0(slot 2, 1)
    CHECK_EQ(op->lubricantId, 0);           // 0x1407b5170(op, 0)
    const data::CommonBlock& b = *op->props->common;
    CHECK_EQ(b.numSteps, 100000);
    CHECK_EQ(b.stepIncr, 100000);
    CHECK_EQ(b.dtMode, 0);
    CHECK_EQ(b.dsmax.utf8(), std::string("2 mm"));
    CHECK_EQ(b.envTemp.utf8(), std::string("20 C"));
    CHECK_EQ(b.convection.utf8(), std::string("0.02 N/sec/mm/C"));
}

// 0x140057770 for slots 1..3 of an empty model (hand-traced from the disassembly; vdie.moproj, hand-
// read: op 3 slots 1..3 are -2, 7, 8, op 4 slots 1..3 are -2):
//   slot s: a new object item (the smallest unused id, here the same each time) goes into the slot
//   (0x140057c60 without reload), the object gets kind 5 because op 3's slot s names an object, and
//   0x140060100 -> 0x1400601b0 sees kind 5 on a slot coded with that id: 0x140057c60(op, s, -2,
//   reload) makes the slot "Read From DB" again and loads the inherited object (0x14005b2f0); the
//   manager refresh 0x140069620 then drops the unused item (it has no file, so nothing is deleted).
// End state: slots 1..3 coded -2, objects 1..3 inherited with kind 5 (names from the L0 test:
// "Workpiece", "TOP_80MN_V_Die_D370_D550"), no new object item, no file effects.
TEST_CASE(multiblow_post_load_adds_objects)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx, "42_vdie", "vdie.moproj") != nullptr);
    mst::installLoaderSlots();
    Operation* op = multiBlowOp(p);
    REQUIRE(op != nullptr);
    mst::OpClass* c = opBase(*op);
    c->current = op;
    engine::GuiModel* m = engine::opModel(p, *op);
    REQUIRE(m != nullptr);
    m->reset();                             // no objects: slot 1 (-2) has no model object
    const std::size_t items = p.objects.rows.size();
    engine::FileEffects fx;
    c->postLoad(p, *op, ctx, fx);
    CHECK_EQ(objId(*op, 0), -2);
    CHECK_EQ(objId(*op, 1), -2);
    CHECK_EQ(objId(*op, 2), -2);
    CHECK_EQ(p.objects.rows.size(), items);
    CHECK(fx.writes.empty());
    CHECK(fx.deletes.empty());
    CHECK_EQ(m->objectCount(), 3);
    for (int no = 1; no <= 3; ++no) {
        REQUIRE(m->object(no) != nullptr);
        CHECK_EQ(*engine::at<std::int32_t>(m->object(no), 0xc), 5);
    }
    CHECK_EQ(std::string(engine::at<char>(m->object(1), 0x828)), std::string("Workpiece"));
    CHECK_EQ(std::string(engine::at<char>(m->object(2), 0x828)), std::string("TOP_80MN_V_Die_D370_D550"));
}

namespace {

// Oracle B cases 93_B_MB3ACT (75F, Active = 2: SfOprMultiBlow3D, op 3) and 94_B_MB2ACT (92_B_MB2REL,
// Active = 3: SfOprMultiBlow2D, op 4), opened and saved by a fresh GUI (claude/m2m-corpus b6147141,
// runner/oracle_b/README.md). Input: <case>_before; expected values: the files the GUI saved,
// <case>/Operations/TaskN/SimCtrl.KEY and the operation's props in <case>/<name>.moproj.
fs::path oracleB()
{
    if (const char* e = std::getenv("M2M_ORACLE_B")) return fs::path(e);
    return m2m_test::configuredPath("M2M_ORACLE_B");
}

struct Opened {
    Project p;
    Ctx ctx;
    engine::FileEffects fx;
    Operation* op = nullptr;
};

bool openActive(Opened& o, const char* caseDir, const char* moproj)
{
    const fs::path f = oracleB() / caseDir / moproj;
    if (!fs::exists(f)) return false;
    const qt::String file{f.u16string()};
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    initProject(o.p, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(o.p);
    if (!openProject(o.p, file, o.ctx) || !o.p.openedOk) return false;
    activateSubProject(o.p, 0, o.ctx);
    o.op = engine::openActiveOperation(o.p, o.ctx, o.fx);   // OB vt+0x58 of the active item
    return o.op != nullptr;
}

// Saved by the GUI: NSTEP 10000, STPINC 10, PDIE 2, TRANS 1 1 0 0 0, DTMAX 0 0 (time per step);
// props NumSteps 10000, EnvTemp "20 C", ConvectionCoef "0.02 N/sec/mm/C", StepBy 1 with
// TimePerStep "0 sec"; Lubricant Id 0 (was -1).
void checkSaved(Opened& o)
{
    engine::GuiModel* m = engine::opModel(o.p, *o.op);
    REQUIRE(m != nullptr);
    def::model::Cls_1401e7b10* sc = m->simCtl();
    CHECK_EQ(sc->nstep, 10000);
    CHECK_EQ(sc->stpinc, 10);
    CHECK_EQ(sc->pdie1, 2);
    CHECK_EQ(sc->trans[0], 1);
    CHECK_EQ(sc->trans[1], 1);
    CHECK_EQ(sc->trans[2], 0);
    CHECK(sc->dtmax1 == 0.0);
    CHECK_EQ(sc->dttype.u58, 1);
    CHECK_EQ(o.op->lubricantId, 0);
    const data::CommonBlock& b = *o.op->props->common;
    CHECK_EQ(b.numSteps, 10000);
    CHECK_EQ(b.envTemp.utf8(), std::string("20 C"));
    CHECK_EQ(b.convection.utf8(), std::string("0.02 N/sec/mm/C"));
    CHECK_EQ(b.dtMode, 1);
    CHECK_EQ(b.dtmax.utf8(), std::string("0 sec"));
}

}  // namespace

TEST_CASE(oracle_b_93_multiblow3d_active)
{
    Opened o;
    ProjectScope scope(o.p);
    if (!fs::exists(oracleB() / "93_B_MB3ACT_before")) return;
    REQUIRE(openActive(o, "93_B_MB3ACT_before", "Gear_blank.moproj"));
    CHECK_EQ(o.op->classKey.utf8(), std::string("SfOprMultiBlow3D"));
    checkSaved(o);
}

TEST_CASE(oracle_b_94_multiblow2d_active)
{
    Opened o;
    ProjectScope scope(o.p);
    if (!fs::exists(oracleB() / "94_B_MB2ACT_before")) return;
    REQUIRE(openActive(o, "94_B_MB2ACT_before", "MO2_Operation_Cycles2.moproj"));
    CHECK_EQ(o.op->classKey.utf8(), std::string("SfOprMultiBlow2D"));
    checkSaved(o);
}

int main()
{
    return testkit::run_all();
}
