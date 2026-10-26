// MDT L6: loader slots of HeatTreat, Distortion, Diestress and DiestressBM. The corpus has no
// operation of these classes, so each case re-classes corpus 42_vdie's cogging operation (Id 4,
// slots -2,-2,-2,10,11,12,13, flow number 4, no project DB beside the copy) and gives it the props
// of the new class. Expected values are read off the decompiles (docs/re/moproj2mst/mdt/L6/spec.md):
//   HeatTreat props vt+0x68 0x1411c0730 copies SC+0x1d0 / +0x164 / +0x110 into the +0x58 block;
//     with the schedule flag clear, 0x1411b3540 first stores the HEAT TREATMENT defaults
//     (title, +0x16c = 999999, +0x1d0 = 0.001); a HeatTreatWizard.HTWZ is refused.
//   Distortion props vt+0x58 0x1413665c0: NumSteps = SC+0x16c, StepIncr = SC+0x164, StepBy = SC+0x240.
//   Diestress without DB and DB KEY: 0x1402b6870(0) clears the slot list (0x140060eb0), step
//     number 0 (0x1402bbf30), NSTART = -(0 + 1) (0x1402b5e50), SC+0x2e4 = 2 (3D), SC+0x1d0 = 1.0.
//   DiestressBM after the first operation: vt+0x78 0x140299a90 resets the wizard block
//     (0x1402a15d0: -1 / 1 / +0x111 = 1, name "DieStressDb.DB" from 0x1402a2910), then vt+0x1b8
//     refuses (the step comes from the project DB).
// ctest-labels: m2m-mdt mdt-l6
#include <testkit/check.hpp>

#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <string>

#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>

#include "m2m/data/diestress.hpp"
#include "m2m/data/diestressbm.hpp"
#include "m2m/data/distortion.hpp"
#include "m2m/data/heattreat.hpp"
#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4path.hpp"
#include "../../src/engine/engine_internal.hpp"
#include "../../src/engine/save_l6.hpp"

namespace m2m::mst {
void installLoaderSlots();
}

namespace fs = std::filesystem;
using namespace m2m;
using engine::at;

namespace {

Project* openCorpus(Project& p, Ctx& ctx)
{
    const char* corpus = std::getenv("M2M_CORPUS");
    if (!corpus) return nullptr;
    const fs::path f = fs::path(corpus) / "42_vdie" / "vdie.moproj";
    const qt::String file{f.u16string()};
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(p);
    if (!openProject(p, file, ctx) || !p.openedOk) return nullptr;
    activateSubProject(p, 0, ctx);
    return &p;
}

// Operation 4 as an operation of class `key`, with that class's props and no engine model yet
Operation* reclass(Project& p, const char16_t* key)
{
    Operation* op = operationById(p, 4);
    if (!op) return nullptr;
    op->classKey = qt::String(key);
    op->opClass = nullptr;
    mst::OpClass* c = findClass(p, op->classKey);
    if (!c) return nullptr;
    setOprProps(*op, c->makeProps(1));
    op->engineModel.reset();
    c->current = op;
    mst::installLoaderSlots();
    return op;
}

def::model::Cls_1401e7b10* sc(Operation& op)
{
    return op.engineModel ? op.engineModel->simCtl() : nullptr;
}

int scInt(Operation& op, std::size_t off) { return *at<std::int32_t>(sc(op), off); }
double scDbl(Operation& op, std::size_t off) { return *at<double>(sc(op), off); }

std::string g_what;

int loadCode(Project& p, Operation& op, Ctx& ctx)
{
    engine::FileEffects fx;
    g_what.clear();
    try {
        opBase(op)->loadOperation(p, op, ctx, fx);
    } catch (const Error& e) {
        g_what = e.what();
        return e.code;
    }
    return 0;
}

}  // namespace

TEST_CASE(heattreat_schedule_flag_set)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx) != nullptr);
    Operation* op = reclass(p, u"SfOprHeatTreat");
    REQUIRE(op != nullptr);
    auto* hp = dynamic_cast<HeatTreatProps*>(op->props.get());
    REQUIRE(hp != nullptr);
    hp->data->schedule.engineDefaults = true;      // set at open by 0x1411c0940 (model always made)
    CHECK_EQ(loadCode(p, *op, ctx), 0);
    REQUIRE(sc(*op) != nullptr);
    CHECK(op->engineModel->object(4) != nullptr);
    CHECK_EQ(hp->block->stepCount, 0);
    CHECK(hp->block->dtmax == scDbl(*op, 0x1d0));
    CHECK_EQ(hp->block->stepIncr, scInt(*op, 0x164));
    CHECK_EQ(hp->block->b1, scInt(*op, 0x110) != 0);
    CHECK(std::strcmp(at<char>(sc(*op), 0), "HEAT TREATMENT") != 0);
}

TEST_CASE(heattreat_engine_defaults)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx) != nullptr);
    Operation* op = reclass(p, u"SfOprHeatTreat");
    REQUIRE(op != nullptr);
    auto* hp = dynamic_cast<HeatTreatProps*>(op->props.get());
    REQUIRE(hp != nullptr);
    CHECK_EQ(loadCode(p, *op, ctx), 0);
    CHECK(hp->data->schedule.engineDefaults);
    CHECK_EQ(std::string(at<char>(sc(*op), 0)), std::string("HEAT TREATMENT"));
    CHECK_EQ(scInt(*op, 0x16c), 999999);
    CHECK(scDbl(*op, 0x1d0) == 0.001);
    CHECK(hp->block->dtmax == 0.001);
    CHECK(hp->block->dtpmax1 == 5.0);
    CHECK_EQ(hp->block->stpdef, 0);                // SC+0x1c8 = 1
}

TEST_CASE(heattreat_wizard_read_into_schedule_and_block)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx) != nullptr);
    Operation* op = reclass(p, u"SfOprHeatTreat2D");
    REQUIRE(op != nullptr);
    auto* hp = dynamic_cast<HeatTreatProps*>(op->props.get());
    REQUIRE(hp != nullptr);
    hp->data->schedule.engineDefaults = true;
    const qt::String w = qt::convertSeparators(taskDirAbs(*op) + qt::kSep + qt::L("HeatTreatWizard.HTWZ"));
    p.overlay[qt::cleanPath(w)] =
        "HEAT_TREATMENT_SPECS_FILE_V3\nNO_MEDIA  1\nMEDIA   1\nOil\nNO_ZONES   1\nZONE   0\nCNVCOF 0 100\n"
        "RADIATION 0\nACVCOF 0 0\nNO_ECTMFN  0\nNO_PROCS  1\nPROC 1 30 1 0 0 0 0 0 0\nSTART_OPTNO  1\n";
    CHECK_EQ(loadCode(p, *op, ctx), 0);
    const data::HTSchedule& s = hp->data->schedule;
    REQUIRE(s.steps.size() == 1u);
    CHECK(s.steps[0]->time == 30.0);
    CHECK_EQ(s.steps[0]->media, 0);
    REQUIRE(s.media.size() == 1u);
    CHECK(s.media[0]->zones[0]->value == 100.0);
    // the props refresh 0x1411bfd60 ran in the base load (before the schedule read of vt+0x58), so
    // its block still describes the schedule before the read: no steps
    CHECK_EQ(hp->block->stepCount, 0);
}

TEST_CASE(distortion_props_from_sim_control)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx) != nullptr);
    Operation* op = reclass(p, u"SfOprDistortion3D");
    REQUIRE(op != nullptr);
    auto* dp = dynamic_cast<DistortionProps*>(op->props.get());
    REQUIRE(dp != nullptr);
    CHECK_EQ(loadCode(p, *op, ctx), 0);
    REQUIRE(sc(*op) != nullptr);
    CHECK_EQ(dp->common->numSteps, scInt(*op, 0x16c));
    CHECK_EQ(dp->common->stepIncr, scInt(*op, 0x164));
    CHECK_EQ(dp->common->dtMode, scInt(*op, 0x240));
    CHECK_EQ(dp->counts->fixtures, dp->data->fixtureCount);
    CHECK_EQ(dp->counts->passes, dp->data->passCount);
    CHECK_EQ(dp->data->workpiece, 1);              // object 1 is an inherited object (kind 5)
}

TEST_CASE(diestress_without_db)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx) != nullptr);
    REQUIRE(!qt::fileExists(primaryDbPath(p)));
    Operation* op = reclass(p, u"SfOprDiestress");
    REQUIRE(op != nullptr);
    auto* dp = dynamic_cast<DiestressProps*>(op->props.get());
    REQUIRE(dp != nullptr);
    CHECK_EQ(loadCode(p, *op, ctx), 0);
    REQUIRE(sc(*op) != nullptr);
    CHECK(op->objIds.empty());
    CHECK_EQ(op->engineModel->objectCount(), 0);
    CHECK_EQ(dp->data->stepNumber, 0);
    CHECK_EQ(scInt(*op, 0x160), -1);
    CHECK_EQ(scInt(*op, 0x2e4), 2);
    CHECK(scDbl(*op, 0x1d0) == 1.0);
    CHECK(scInt(*op, 0x16c) >= 1);
    CHECK_EQ(sc(*op)->trans[1], 1);                // post-load 0x1402b60f0
    CHECK_EQ(sc(*op)->trans[0], 0);
}

TEST_CASE(diestressbm_after_first_refused)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx) != nullptr);
    Operation* op = reclass(p, u"SfOprDiestressBM3D");
    REQUIRE(op != nullptr);
    auto* bp = dynamic_cast<DiestressBMProps*>(op->props.get());
    REQUIRE(bp != nullptr);
    REQUIRE(flowNumber(*op) > 1);
    CHECK_EQ(loadCode(p, *op, ctx), M2M_E_UNSUPPORTED);
    CHECK(g_what.find("DiestressBM operation after the first") != std::string::npos);
    CHECK_EQ(bp->data->objTotalNum, 0);
    CHECK_EQ(bp->data->objNo[0], -1);
    CHECK_EQ(bp->data->refNo[14], 1);
    CHECK_EQ(int(bp->data->f111), 1);
    CHECK_EQ(bp->data->newDbName.utf8(), std::string("DieStressDb.DB"));
}

// DiestressBM vt+0x80 0x140299b60 -> 0x1402a16c0: QDomDocument("DieStressBmWizard").toByteArray(1)
// in text mode. Attribute order = Qt 4.8 QHash order (qHash % 17: qsNewDbName 2, nObjTotalNum 4,
// bUseNewDb 7; bIntepolate 5, nRefNo 6, dTolerance 7, nObjNo 10), as oracle B 62_B_BM2's
// DieStressBmWizard.Wzd shows for the root element.
TEST_CASE(diestressbm_wizard_xml)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx) != nullptr);
    Operation* op = reclass(p, u"SfOprDiestressBM3D");
    REQUIRE(op != nullptr);
    engine::FileEffects fx;
    REQUIRE(engine::opModel(p, *op) != nullptr);
    engine::diestressBMSaveWizard(p, *op, fx);             // no objects in the fresh model
    REQUIRE(fx.writes.size() == 1u);
    CHECK(fx.writes[0].path.utf8().find("DieStressBmWizard.Wzd") != std::string::npos);
    CHECK_EQ(fx.writes[0].bytes, std::string("<!DOCTYPE DieStressBmWizard>\r\n"
                                             "<DieStressBmWizard qsNewDbName=\"DieStressDb.DB\" nObjTotalNum=\"0\" "
                                             "bUseNewDb=\"0\"/>\r\n"));
    engine::FileEffects fx2;
    engine::GuiModel* m = engine::loadFromKey(p, *op, true, fx2);   // the slots' objects
    REQUIRE(m != nullptr);
    const int n = m->objectCount();
    REQUIRE(n >= 1);
    engine::FileEffects fx3;
    engine::diestressBMSaveWizard(p, *op, fx3);
    REQUIRE(fx3.writes.size() == 1u);
    const std::string& b = fx3.writes[0].bytes;
    const std::string first = "<DieStressBmWizard qsNewDbName=\"DieStressDb.DB\" nObjTotalNum=\"" + std::to_string(n) +
                              "\" bUseNewDb=\"0\">\r\n <ObjId01 bIntepolate=\"0\" nRefNo=\"0\" dTolerance=\"0\" nObjNo=\"" +
                              std::to_string(*at<std::int32_t>(m->objectAt(1), 8)) + "\"/>\r\n";
    CHECK(b.find(first) != std::string::npos);
    CHECK(b.size() > 24 && b.substr(b.size() - 22) == "</DieStressBmWizard>\r\n");
}

// Diestress vt+0xe8 0x1402b5be0: the model as <sub-project DB base>.KEY instead of the task files
TEST_CASE(diestress_save_writes_db_key_only)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx) != nullptr);
    Operation* op = reclass(p, u"SfOprDiestress");
    REQUIRE(op != nullptr);
    CHECK_EQ(loadCode(p, *op, ctx), 0);
    engine::FileEffects fx;
    engine::saveCurrentOperation(p, *op, ctx, fx);
    REQUIRE(fx.writes.size() == 1u);
    SubProject* sp = activeSubProject(p);
    REQUIRE(sp != nullptr);
    CHECK(fx.writes[0].path == app::changeSuffix(subProjectDbPath(p, *sp, true), qt::L(".KEY")));
    CHECK(fx.writes[0].bytes.size() > 0);
}

int main()
{
    return testkit::run_all();
}
