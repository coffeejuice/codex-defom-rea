// MDT L0: the KEY (batch) loader 0x1400569d0 on corpus 42_vdie's cogging operation (Id 4, slots
// -2,-2,-2,10,11,12,13). Expected values from the corpus files (hand-read):
//   slot 1 inherits op 1's Objects/Object00001.KEY "Workpiece" (op 2 and op 3 hold -2): geometry
//   only (0x140c7ce40 drops the mesh), RZ/DIEGEO extremes +-556.27976216, +-202.16;
//   slot 2 inherits Objects/Object00007.KEY (op 3's own code 7) "TOP_80MN_V_Die_D370_D550";
//   slots 4..7 read their own object files 10..13 ("Left Manipulator 1" .. "Right Manipulator 2").
// ctest-labels: m2m-mdt
#include <testkit/check.hpp>

#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <string>

#include <defcmd/objpos_centre.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/llobj_data.hpp>

#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/data/cogging.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
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

const char* objName(def::model::LLOBJ_DATA* o)
{
    return engine::at<char>(o, 0x828);
}

int objType(def::model::LLOBJ_DATA* o)
{
    return *engine::at<std::int32_t>(o, 0xc);
}

}  // namespace

TEST_CASE(vdie42_cogging_key_load)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx, "42_vdie", "vdie.moproj") != nullptr);
    Operation* op = operationById(p, 4);
    REQUIRE(op != nullptr);
    CHECK_EQ(op->classKey.utf8(), std::string("SfOprCogging"));
    engine::FileEffects fx;
    engine::GuiModel* m = engine::loadFromKey(p, *op, true, fx);
    REQUIRE(m != nullptr);
    CHECK(fx.writes.empty());
    CHECK(fx.deletes.empty());

    def::model::LLOBJ_DATA* b = m->object(1);
    REQUIRE(b != nullptr);
    CHECK_EQ(std::string(objName(b)), std::string("Workpiece"));
    CHECK_EQ(objType(b), 5);
    CHECK(!engine::objHasMesh(b));
    CHECK(engine::objHasGeometry(b));
    double mn[3] = {}, mx[3] = {};
    def::cmd::objpos::surface_bbox(def::model::geo_kw::surface(*b, 0), mn, mx);
    CHECK(mn[0] == -556.27976216 && mx[0] == 556.27976216);
    CHECK(mn[1] == -202.16 && mx[1] == 202.16);
    CHECK(mn[2] == -202.16 && mx[2] == 202.16);

    def::model::LLOBJ_DATA* d = m->object(2);
    REQUIRE(d != nullptr);
    CHECK_EQ(std::string(objName(d)), std::string("TOP_80MN_V_Die_D370_D550"));
    CHECK_EQ(objType(d), 5);

    static const char* manip[4] = {"Left Manipulator 1", "Left Manipulator 2", "Right Manipulator 1",
                                   "Right Manipulator 2"};
    for (int k = 0; k < 4; ++k) {
        def::model::LLOBJ_DATA* o = m->object(4 + k);
        REQUIRE(o != nullptr);
        CHECK_EQ(std::string(objName(o)), std::string(manip[k]));
    }
}

// MDT L1A: File > Open loads the active item (Active = 3, the cogging operation Id 4) through the
// class slots: SfOprCogging vt+0xb8 (0x140750650), the KEY load, vt+0xc0 (0x1407507b0). The
// post-load's 0x140766740 replaces the billet dimensions with the node box of the inherited billet
// KEY (Objects/Object00001.KEY): Length = X extent, Width = Height = Z extent, Dia = 2 x Z extent.
// Hand calculation from the extents above (same double operations as 766740.asm).
TEST_CASE(vdie42_open_active_cogging)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx, "42_vdie", "vdie.moproj") != nullptr);
    engine::FileEffects fx;
    Operation* op = engine::openActiveOperation(p, ctx, fx);
    REQUIRE(op != nullptr);
    CHECK_EQ(op->id, 4);
    CHECK(fx.writes.empty());
    CHECK(fx.deletes.empty());
    auto* cp = dynamic_cast<CoggingProps*>(opBase(*op)->propsOf(op));
    REQUIRE(cp != nullptr && cp->model != nullptr);
    const data::CogBillet& b = cp->model->cog.billet;
    const double dz = 202.16 - -202.16;
    CHECK(b.length == 556.27976216 - -556.27976216);
    CHECK(b.width == dz);
    CHECK(b.height == dz);
    CHECK(b.dia == 2.0 * dz);

    engine::GuiModel* m = engine::opModel(p, *op);
    REQUIRE(m != nullptr);
    def::model::LLOBJ_DATA* o1 = m->object(1);
    REQUIRE(o1 != nullptr);
    CHECK_EQ(*engine::at<std::int32_t>(o1, 0x3aa4), b.geoTyp + 6);
    CHECK(*engine::at<double>(o1, 0x3ab0) == b.length);
    def::model::LLOBJ_DATA* o2 = m->object(2);
    REQUIRE(o2 != nullptr);
    CHECK_EQ(*engine::at<std::int32_t>(o2, 0x3aa4), 9);
    CHECK(*engine::at<double>(o2, 0x3ab0) == cp->model->cog.die.length);
    def::model::LLOBJ_DATA* o4 = m->object(4);
    REQUIRE(o4 != nullptr);
    CHECK_EQ(*engine::at<std::int32_t>(o4, 0x3aa4), 10);
    CHECK(*engine::at<double>(o4, 0x3ab0 + 8 * 8) == 45.0);
    CHECK_EQ(op->lubSystem, 1);
}

int main()
{
    return testkit::run_all();
}
