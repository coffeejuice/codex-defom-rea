// MDT L7: load slots of SfOprCycle, SfOprGroup, SfOprDoeOutput, SfOprReport, SfOprShapeOptim.
// Corpus 48_MO2_Operation_Cycles2: the active item (Active="2") is the cycle (Id 3) around
// "Forming (2)" (Id 2, SfOprForming2D, GroupInfo col 1 = that item). By 0x14134daf0 the cycle's
// engine model is its first member's, created by SfOprForming2D's vt+0x1b0 with dimension 2.
// ctest-labels: m2m-mdt m2m-mdt-l7
#include <testkit/check.hpp>

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <string>

#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>

#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/qt4path.hpp"
#include "../../src/engine/engine_internal.hpp"

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

// Error text of a load, "" when it passes
std::string loadError(Project& p, Ctx& ctx, engine::FileEffects& fx, Operation** out)
{
    try {
        *out = engine::openActiveOperation(p, ctx, fx);
    } catch (const Error& e) {
        return e.what();
    }
    return {};
}

}  // namespace

TEST_CASE(cycle48_model_is_first_member)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx, "48_MO2_Operation_Cycles2", "MO2_Operation_Cycles2.moproj") != nullptr);
    Operation* cyc = operationById(p, 3);
    Operation* mem = operationById(p, 2);
    REQUIRE(cyc != nullptr && mem != nullptr);
    CHECK_EQ(cyc->classKey.utf8(), std::string("SfOprCycle"));
    engine::FileEffects fx;
    Operation* op = nullptr;
    const std::string err = loadError(p, ctx, fx, &op);
    std::printf("cycle48 load: %s\n", err.empty() ? "ok" : err.c_str());
    // the model belongs to the member, the cycle never gets OPI+0x38
    CHECK(cyc->engineModel == nullptr);
    REQUIRE(mem->engineModel != nullptr);
    CHECK(engine::opModel(p, *cyc) == mem->engineModel.get());
    CHECK_EQ(*engine::at<std::int32_t>(mem->engineModel->step.get(), 0x1c), 2);   // E+0x1c dimension
    // the prologue 0x14134d750 -> 0x141352490 -> 0x1405f1bb0 ran on the member's model
    const def::model::Cls_1401e7b10* sc = mem->engineModel->simCtl();
    REQUIRE(sc != nullptr);
    CHECK_EQ(sc->tnow3, 1);
    CHECK(sc->tnow2 == 0.0);
    CHECK_EQ(sc->trans[0], 1);
    CHECK_EQ(sc->trans[1], 1);
    // The cycle has no object slots and its model no objects, while the operation before it in the
    // chain... is none here, so 0x1405f1c00 counts n = 1 and adds an object (0x140057ba0), which the
    // L0 base body does not port yet. Until it does, the load stops there.
    CHECK(err.empty());
    CHECK(op == cyc);
    // The cycle's slot holds object item 5, which has no file: the KEY load gives the model no
    // object, so the post-load 0x1405f1c00 (no predecessor: n = 1) adds one (0x140057ba0) under the
    // next free item id 6, and the manager refresh drops the now unused item 5. Oracle B (the
    // original GUI opened and saved this project, gui_mo/oracle_b/60_B_HT2, B02_HT2, 62_B_BM2): the
    // saved Object_List is 1 2 3 4 6, the cycle's Object is Id 6 "Workpiece" ObjectType 1, HasGeo 0,
    // HasMesh 0, and item 6 has ObjectType 1, HasGeo 0, HasMesh 0.
    REQUIRE(cyc->objIds.size() == 1u);
    CHECK_EQ(cyc->objIds[0], 6);
    std::string ids;
    for (const auto& it : p.objects.rows) ids += std::to_string(it->id) + " ";
    CHECK_EQ(ids, std::string("1 2 3 4 6 "));
    CHECK(fx.deletes.empty());                            // Objects/Object00005.KEY does not exist
    CHECK(fx.writes.empty());
    CHECK_EQ(mem->engineModel->objectCount(), 1);
    def::model::LLOBJ_DATA* o = mem->engineModel->object(1);
    REQUIRE(o != nullptr);
    CHECK_EQ(*engine::at<std::int32_t>(o, 0xc), 1);
    CHECK_EQ(std::string(engine::at<char>(o, 0x828)), std::string("Workpiece"));
    CHECK(*engine::at<double>(o, 0x2bf0) == 360.0);       // 0x140057c60, .rdata 0x1421249c8
    REQUIRE(!cyc->objs.empty() && cyc->objs[0]);
    const ObjEntry& e = *cyc->objs[0];
    CHECK_EQ(e.objectType, 1);
    CHECK_EQ(e.name.utf8(), std::string("Workpiece"));
    CHECK(!e.hasGeo);
    CHECK(!e.hasMesh);
    const Item* it6 = p.objects.find(6);
    REQUIRE(it6 != nullptr);
    CHECK_EQ(it6->objectType, 1);
    CHECK(!it6->hasGeo);
    CHECK(!it6->hasMesh);
    CHECK(e.defineRotationMovement);                      // oracle B DefineRotationMovement="1" (obj+0x2bd8)
}

// Save current operation 0x140052ec0 on the cycle: SfOprBase vt+0x68 0x1405ef510 with the cycle's
// empty vt+0xe8 (0x14134dae0) and vt+0x80 (0x14134d780): props vt+0x60 (0x1405fb610, nothing),
// vt+0xd8 0x1405f22e0 -> 0x140054540 (OPRNAM = the operation name into the model's sim control),
// and no task file. Oracle B 60_B_HT2 / 62_B_BM2 wrote no file in that step either.
TEST_CASE(cycle51_save_writes_no_task_files)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx, "51_MO2_Operation_Cycles2", "MO2_Operation_Cycles2.moproj") != nullptr);
    engine::FileEffects open, save;
    Operation* op = nullptr;
    REQUIRE(loadError(p, ctx, open, &op).empty());
    REQUIRE(op != nullptr);
    CHECK_EQ(op->classKey.utf8(), std::string("SfOprCycle"));
    engine::setEngineVersion(u"");
    engine::saveCurrentOperation(p, *op, ctx, save);
    CHECK(save.writes.empty());
    CHECK(save.deletes.empty());
    const def::model::Cls_1401e7b10* sc = engine::opModel(p, *op)->simCtl();
    REQUIRE(sc != nullptr);
    CHECK_EQ(std::string(sc->oprnam), op->name.utf8());
    std::printf("cycle51 save: OPRNAM '%s' CURSIM %d %d SIMNAM '%s'\n", sc->oprnam, sc->cursim[0], sc->cursim[1],
                sc->simnam);
}

TEST_CASE(refusals_of_the_viewer_classes)
{
    static const char* keys[3] = {"SfOprDoeOutput", "SfOprShapeOptim", "SfOprReport"};
    for (const char* k : keys) {
        Project p;
        ProjectScope scope(p);
        Ctx ctx;
        REQUIRE(openCorpus(p, ctx, "48_MO2_Operation_Cycles2", "MO2_Operation_Cycles2.moproj") != nullptr);
        Operation* cyc = operationById(p, 3);
        REQUIRE(cyc != nullptr);
        // the active item made an operation of class k (the class objects are the registry's)
        cyc->classKey = qt::L(k);
        cyc->opClass = findClass(p, cyc->classKey);
        REQUIRE(cyc->opClass != nullptr);
        engine::FileEffects fx;
        Operation* op = nullptr;
        const std::string err = loadError(p, ctx, fx, &op);
        std::printf("%s load: %s\n", k, err.c_str());
        CHECK(err.find("operation load: ") == 0);
        CHECK(err.find(k) != std::string::npos);
    }
}

int main()
{
    return testkit::run_all();
}
