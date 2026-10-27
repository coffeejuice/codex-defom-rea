// MDT L5: loader slots of SfOprBoolean, SfOprConvert2Dto3D and SfOprCopyMirror on corpus 46
// (MO2_Boolean_Oprations, 2D: op 1 AirConvect2D, op 2 HeatResting2D, op 3 Forming2D, op 4 Boolean2D
// with slot 1 inherited (-2, Objects/Object00001.KEY "Workpiece") and slot 2 own object 5 ("Cutter",
// Objects/Object00005.KEY)) and on corpus 42 (vdie, 3D, for the 3D class SfOprCopyMirror). The
// corpus has no Convert2Dto3D or CopyMirror operation, so those tests give an operation the other
// class (its key and a properties object of that class).
// Expected values are hand-derived from the binary (decompiles in deform-rea-work/gui_mo/s1/mdt/L5):
//   0x1411def50 prologue: TRANS 2 = 1, TRANS 1 = 0, NSTEP 0, STPINC 1, NSTART -1, DTMAX 1.0 (0x1421249e0),
//               ITRMTH 1, SOLMTD 1, after the base TNOW 3 = 1 and TNOW 2 = 0
//   0x1411df020 post-load: a kind-7 workpiece object is replaced by the last slot object of kind 5
//   0x140e25980 vt+0x2a0: CopyMirror gives the inherited object (geometry, no mesh); a viewer item with
//               planes needs 0x140e25c00 (refused)
//   0x140e22950 mirrormerge.ini: one MIRROR_MERGE block of 2 planes for object 1
//   0x1401c3a60 / 0x1401c29d0: SfOprConvert2Dto3D_TEMP_STEP.KEY is written from the work model, whose
//               objects are new (default names) and get parts of the operation's objects; those get
//               GEO23 mode 1401ee4c0(sim control) = 1 for GEOTYP 1 and angle 360 (0x1421249c8)
// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include "../support/test_paths.hpp"

#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <string>

#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>

#include "m2m/data/boolean.hpp"
#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/p17.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4path.hpp"
#include "../../src/engine/engine_internal.hpp"
#include "../../src/engine/mirror_plane_owner.hpp"

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
    if (!fs::exists(f)) return nullptr;
    const qt::String file{f.u16string()};
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(p);
    if (!openProject(p, file, ctx) || !p.openedOk) return nullptr;
    activateSubProject(p, 0, ctx);
    mst::installLoaderSlots();
    return &p;
}

Project* openCorpus46(Project& p, Ctx& ctx)
{
    return openCorpus(p, ctx, "46_MO2_Boolean_Oprations", "MO2_Boolean_Oprations.moproj");
}

// 3D: op 4 SfOprCogging, slots -2, -2, -2, 10..13; slot 1 inherits op 1's Object00001.KEY
// "Workpiece", slot 2 op 3's own Object00007.KEY "TOP_80MN_V_Die_D370_D550" (L0 test)
Project* openCorpus42(Project& p, Ctx& ctx)
{
    return openCorpus(p, ctx, "42_vdie", "vdie.moproj");
}

// Give op the class `key` (registry key and a properties object of that class). SfOprCopyMirror is
// not in the GUI's registry (0x14006dba0 never inserts it, so a project naming it fails to open and
// its slots are unreachable in v11.0.2); the tests insert it to run the ported slots.
void retag(Project& p, Operation& op, const char16_t* key)
{
    if (!p.registry.count(String(key)) && String(key) == String(u"SfOprCopyMirror")) {
        auto ob = mst::makeCopyMirror();
        mst::classInit(*ob);
        p.registry[ob->key()] = std::move(ob);
    }
    op.classKey = String(key);
    op.props = p.registry.at(op.classKey)->makeProps(p.unit);
}

// OB vt+0x58 of the operation's class, as 0x140051c30 calls it
void load(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx)
{
    mst::OpClass* c = opBase(op);
    REQUIRE(c != nullptr);
    c->current = &op;
    engine::GuiModel* m = engine::opModel(p, op);
    REQUIRE(m != nullptr);
    m->reset();
    c->loadOperation(p, op, ctx, fx);
}

const char* objName(def::model::LLOBJ_DATA* o) { return engine::at<char>(o, 0x828); }
int objKind(def::model::LLOBJ_DATA* o) { return *engine::at<std::int32_t>(o, 0xc); }

}  // namespace

TEST_CASE(boolean_prologue_sim_control)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    if (!openCorpus46(p, ctx)) return;
    Operation* op = operationById(p, 4);
    REQUIRE(op != nullptr);
    CHECK_EQ(op->classKey.utf8(), std::string("SfOprBoolean2D"));
    mst::OpClass* c = opBase(*op);
    REQUIRE(c != nullptr);
    c->current = op;
    engine::GuiModel* m = engine::opModel(p, *op);
    REQUIRE(m != nullptr);
    c->loadPrologue(p, *op);
    def::model::Cls_1401e7b10* sc = m->simCtl();
    REQUIRE(sc != nullptr);
    CHECK_EQ(sc->trans[1], 1);
    CHECK_EQ(sc->trans[0], 0);
    CHECK_EQ(sc->nstep, 0);
    CHECK_EQ(sc->stpinc, 1);
    CHECK_EQ(sc->nstart, -1);
    CHECK(sc->dtmax1 == 1.0);
    CHECK_EQ(sc->itrmth, 1);
    CHECK_EQ(sc->solmtd, 1);
    CHECK_EQ(sc->tnow3, 1);
    CHECK(sc->tnow2 == 0.0);
}

TEST_CASE(boolean_load_and_workpiece_pick)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    if (!openCorpus46(p, ctx)) return;
    Operation* op = operationById(p, 4);
    REQUIRE(op != nullptr);
    engine::FileEffects fx;
    load(p, *op, ctx, fx);
    CHECK(fx.writes.empty());
    CHECK(fx.deletes.empty());
    engine::GuiModel* m = engine::opModel(p, *op);
    def::model::LLOBJ_DATA* w = m->object(1);
    REQUIRE(w != nullptr);
    CHECK_EQ(std::string(objName(w)), std::string("Workpiece"));
    CHECK_EQ(objKind(w), 5);
    def::model::LLOBJ_DATA* cut = m->object(2);
    REQUIRE(cut != nullptr);
    CHECK_EQ(std::string(objName(cut)), std::string("Cutter"));

    auto* bp = dynamic_cast<BooleanProps*>(op->props.get());
    REQUIRE(bp != nullptr && bp->data != nullptr);
    const int before = bp->data->workpiece;
    // object `workpiece` exists with kind 5: nothing changes
    opBase(*op)->postLoad(p, *op, ctx, fx);
    CHECK_EQ(bp->data->workpiece, before);
    // a kind-7 workpiece is replaced by the slot object of kind 5 (object 1; object 2 is kind 7 now)
    bp->data->workpiece = 2;
    *engine::at<std::int32_t>(cut, 0xc) = 7;
    opBase(*op)->postLoad(p, *op, ctx, fx);
    CHECK_EQ(bp->data->workpiece, 1);
}

TEST_CASE(copymirror_provides_inherited_object)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    if (!openCorpus42(p, ctx)) return;
    Operation* op3 = operationById(p, 3);
    Operation* op4 = operationById(p, 4);
    REQUIRE(op3 != nullptr && op4 != nullptr);
    retag(p, *op3, u"SfOprCopyMirror");
    engine::FileEffects fx;
    engine::GuiModel* m = engine::loadFromKey(p, *op4, true, fx);   // 0x14005b2f0 walks op 3 first
    REQUIRE(m != nullptr);
    def::model::LLOBJ_DATA* w = m->object(1);
    REQUIRE(w != nullptr);
    CHECK_EQ(std::string(objName(w)), std::string("Workpiece"));
    CHECK_EQ(objKind(w), 5);
    CHECK(engine::objHasGeometry(w));
    CHECK(!engine::objHasMesh(w));
    def::model::LLOBJ_DATA* d = m->object(2);
    REQUIRE(d != nullptr);
    CHECK_EQ(std::string(objName(d)), std::string("TOP_80MN_V_Die_D370_D550"));
    CHECK_EQ(objKind(d), 5);
    // op 3's own model got its slot objects (bare loads); the class's current op is restored
    CHECK(engine::opModel(p, *op3)->object(2) != nullptr);
    CHECK(opBase(*op3)->current == nullptr);
}

TEST_CASE(copymirror_planes_need_viewer)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    if (!openCorpus42(p, ctx)) return;
    Operation* op3 = operationById(p, 3);
    Operation* op4 = operationById(p, 4);
    REQUIRE(op3 != nullptr && op4 != nullptr);
    retag(p, *op3, u"SfOprCopyMirror");
    engine::opModel(p, *op3)->viewerMirrorPlanes = {1};
    engine::FileEffects fx;
    bool refused = false;
    try {
        engine::loadFromKey(p, *op4, true, fx);
    } catch (const Error& e) {
        refused = e.code == M2M_E_UNSUPPORTED && std::string(e.what()).find("0x140e25c00") != std::string::npos;
    }
    CHECK(refused);
}

TEST_CASE(copymirror_postload_positive_builder_stays_guarded)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    if (!openCorpus42(p, ctx)) return;
    Operation* op4 = operationById(p, 4);
    REQUIRE(op4 != nullptr);
    retag(p, *op4, u"SfOprCopyMirror");
    const String ini = qt::cleanPath(projectDir(p) + qt::kSep + taskDirRel(*op4) + qt::kSep + qt::L("mirrormerge.ini"));
    p.overlay[ini] = "! planes of object 1\r\n"
                     "TOLERANCE 0.01\r\n"
                     "MIRROR_MERGE 1 2\r\n"
                     "1 0 0 0.0 0.0 0.0 1.0 0.0 0.0 1\r\n"
                     "2 0 0 0.0 0.0 0.0 0.0 1.0 0.0 0\r\n";
    engine::FileEffects fx;
    bool refused = false;
    try {
        load(p, *op4, ctx, fx);
    } catch (const Error& e) {
        refused = e.code == M2M_E_UNSUPPORTED &&
            std::string(e.what()).find("positive View plane builder") != std::string::npos;
    }
    CHECK(refused);
    engine::GuiModel* m = engine::opModel(p, *op4);
    REQUIRE(m != nullptr);
    const auto owner = engine::detail::mirrorPlaneOwnerSnapshot(*m, 1);
    REQUIRE(owner.has_value());
    CHECK_EQ(owner->allocatedSlotCount, 0);
    CHECK(!owner->records[0].a.has_value());
    CHECK(m->viewerMirrorPlanes.empty());
    CHECK(fx.writes.empty());
}

TEST_CASE(convert2dto3d_load_writes_temp_step)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    if (!openCorpus46(p, ctx)) return;
    Operation* op4 = operationById(p, 4);
    REQUIRE(op4 != nullptr);
    retag(p, *op4, u"SfOprConvert2Dto3D");
    engine::FileEffects fx;
    load(p, *op4, ctx, fx);
    REQUIRE(fx.writes.size() == 1u);
    const String want =
        qt::cleanPath(taskDirAbs(*op4) + qt::kSep + qt::L("SfOprConvert2Dto3D_TEMP_STEP.KEY"));
    CHECK(fx.writes[0].path == want);
    // then the Configuration editor shown at the end of the load is replaced: its vt+0x1d0 finds the
    // object's GEO23 block changed since its init (props vt+0x60 ran first) and 0x1401c7560(-1, 1)
    // removes the file again
    REQUIRE(fx.deletes.size() == 1u);
    CHECK(fx.deletes[0] == want);
    const std::string& key = fx.writes[0].bytes;
    // the work model's objects are made by 0x140c7c950 (default names) and get only the 0x3aa0,
    // 0x3b50, 0x3998, GEO23 and mesh-settings parts of the operation's objects
    CHECK(key.find("Workpiece") != std::string::npos);
    CHECK(key.find("Cutter") == std::string::npos);
    // post-load on the operation's own objects: GEO23 mode = 1401ee4c0(sim control) = 1, angle 360
    engine::GuiModel* m = engine::opModel(p, *op4);
    for (int no = 1; no <= 2; ++no) {
        def::model::LLOBJ_DATA* o = m->object(no);
        REQUIRE(o != nullptr);
        CHECK_EQ(*engine::at<std::int32_t>(o, 0x2be8), 1);
        CHECK(*engine::at<double>(o, 0x2bf0) == 360.0);
    }
}

TEST_CASE(convert2dto3d_chain_head_selects_geometry_type)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    const auto path = m2m_test::configuredPath("M2M_ORACLE_B") /
                      "129_B_DBSTEP_CNV_before" / "COG_P1_B1.moproj";
    const String file(path.u16string());
    const auto db = app::changeSuffix(file, qt::L(".DB"));
    initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(p);
    REQUIRE(openProject(p, file, ctx) && p.openedOk);
    activateSubProject(p, 0, ctx);
    engine::FileEffects fx;
    engine::openActiveOperation(p, ctx, fx);
    auto* op = operationById(p, 2);
    REQUIRE(op != nullptr);
    REQUIRE(isChainHead(*op));
    REQUIRE(op->classKey == qt::L("SfOprConvert2Dto3D"));
    auto* model = engine::opModel(p, *op);
    REQUIRE(model != nullptr);
    CHECK_EQ(engine::guiModelDim(*model), 2);
    // Independent historical screenshot selects Geometry Type, and its saved
    // CRDSYS remains 0. Configuration's default Z radio would change this to 3.
    for (int no = 1; no <= 3; ++no) {
        auto* object = model->object(no);
        REQUIRE(object != nullptr);
        CHECK_EQ(*engine::at<std::int32_t>(object, 0x2c38), 0);
    }
    const auto temp = qt::cleanPath(taskDirAbs(*op) + qt::kSep +
                                   qt::L("SfOprConvert2Dto3D_TEMP_STEP.KEY"));
    bool written = false;
    for (const auto& w : fx.writes) if (w.path == temp) written = true;
    CHECK(written);
    for (const auto& d : fx.deletes) CHECK(d != temp);
    auto* sc = model->simCtl();
    REQUIRE(sc != nullptr);
    // Save reads the already filled radio, rather than choosing Configuration
    // or rereading a subsequently modified model value.
    sc->geotyp = 4;
    REQUIRE(opBase(*op)->updateDataInWidgetArea(p, *op, fx));
    CHECK_EQ(sc->geotyp, 1);
}

int main()
{
    return testkit::run_all();
}
