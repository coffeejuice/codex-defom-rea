#include "../support/test_paths.hpp"
// MDT L4 (DB-5): OB vt+0xc8 of MultiBlow (0x140602230 -> 0x140602260, 0x14060b6b0) and of the Express
// family (0x1413fbd10 with vt+0x2c0 0x1413fc840 / vt+0x2b8 0x1413fc6b0). The oracle-B projects
// 93_B_MB3ACT_before (MultiBlow active) and 95_B_FE3ACT_before (FormingExpress active) are opened
// and loaded; the test then sets the engine object and props fields the slot reads and checks what
// it writes. Expected values: the copies and assignments of the disassembly, by hand.
// ctest-labels: m2m-mdt m2m-mdt-l4
#include <testkit/check.hpp>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <string>

#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>

#include "m2m/data/express.hpp"
#include "m2m/data/multiblow.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine_multiblow.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4path.hpp"
#include "../../src/ops/express.hpp"

namespace fs = std::filesystem;
using namespace m2m;

namespace {

fs::path oracleB()
{
    if (const char* e = std::getenv("M2M_ORACLE_B")) return fs::path(e);
    return m2m_test::configuredPath("M2M_ORACLE_B");
}

// Opens <case>_before and loads its active operation; null (SKIP) without the oracle-B tree
Operation* openCase(Project& p, Ctx& ctx, const char* caseB, const char* moproj)
{
    const fs::path before = oracleB() / (std::string(caseB) + "_before");
    if (!fs::exists(before)) {
        std::printf("SKIP %s: no oracle B\n", caseB);
        return nullptr;
    }
    const qt::String file{(before / moproj).u16string()};
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(p);
    if (!openProject(p, file, ctx) || !p.openedOk) return nullptr;
    activateSubProject(p, 0, ctx);
    engine::FileEffects fx;
    return engine::openActiveOperation(p, ctx, fx);
}

template <class T> void put(def::model::LLOBJ_DATA* o, std::size_t off, T v)
{
    std::memcpy(reinterpret_cast<unsigned char*>(o) + off, &v, sizeof v);
}

int two() { return 2; }   // MultiBlow vt+0x120 0x140601f50

bool hasWarning(const Ctx& ctx, const char* text)
{
    for (const std::string& w : ctx.warnings)
        if (w.find(text) != std::string::npos) return true;
    return false;
}

}  // namespace

TEST_CASE(multiblow_sync_hammer)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    Operation* op = openCase(p, ctx, "93_B_MB3ACT", "Gear_blank.moproj");
    if (!op) return;
    mst::OpClass* c = p.registry.at(op->classKey).get();
    engine::GuiModel* m = engine::opModel(p, *op);
    REQUIRE(m != nullptr);
    def::model::LLOBJ_DATA* o = m->object(2);
    REQUIRE(o != nullptr);
    // movement block +0x2160: name, type 2 (hammer), the hammer fields, a direction, 3 curve points
    std::memset(reinterpret_cast<unsigned char*>(o) + 0x2160, 0, 0x40);
    std::strcpy(reinterpret_cast<char*>(o) + 0x2160, "Hammer A");
    put<std::int32_t>(o, 0x2160 + 0x68, 2);
    put<double>(o, 0x2160 + 0x1e8, 250.0);    // -> +0x28 mass
    put<double>(o, 0x2160 + 0x1f0, 12.5);     // -> +0x20 and +0x40 energy
    put<double>(o, 0x2160 + 0x1f8, 0.75);     // -> +0x30 efficiency
    put<std::int32_t>(o, 0x2160 + 0x200, 1);  // -> +0x38 counterblow
    put<double>(o, 0x2160 + 0x50, 0.0);
    put<double>(o, 0x2160 + 0x58, 0.0);
    put<double>(o, 0x2160 + 0x60, -1.0);
    put<std::int32_t>(o, 0x2160 + 0x208 + 4, 3);
    engine::FileEffects fx;
    c->syncFromModelTail(p, *op, ctx, fx);                   // vt+0xc8 of the class
    auto* pr = dynamic_cast<MultiBlowProps*>(op->props.get());
    REQUIRE(pr != nullptr);
    const data::MultiBlowData& d = *pr->data;
    CHECK(d.equipName.utf8() == "Hammer A");
    CHECK_EQ(int(d.hammer), 1);
    CHECK_EQ(int(d.equipment), 0);
    CHECK(d.hammerEnergy == 12.5);
    CHECK(d.screwEnergy == 12.5);
    CHECK(d.hammerMass == 250.0);
    CHECK(d.hammerEfficiency == 0.75);
    CHECK_EQ(d.hammerCounterblow, 1);
    CHECK(d.movDir[2] == -1.0);
    CHECK_EQ(int(d.efficiencyFunction), 1);
    CHECK(ctx.warnings.empty() || !hasWarning(ctx, "Multi-Blow"));
}

TEST_CASE(multiblow_sync_messages)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    Operation* op = openCase(p, ctx, "93_B_MB3ACT", "Gear_blank.moproj");
    if (!op) return;
    mst::OpClass* c = p.registry.at(op->classKey).get();
    engine::GuiModel* m = engine::opModel(p, *op);
    REQUIRE(m != nullptr);
    def::model::LLOBJ_DATA* o = m->object(2);
    REQUIRE(o != nullptr);
    put<std::int32_t>(o, 0x2160 + 0x68, 1);   // neither hammer nor screw
    CHECK(!engine::multiBlowSyncFromModelTail(*c, p, ctx, &two));
    CHECK(hasWarning(ctx, "Currently Multi-Blow can support only hammer and screw type."));
    auto none = []() { return 99; };          // no object 99
    CHECK(!engine::multiBlowSyncFromModelTail(*c, p, ctx, +none));
    CHECK(hasWarning(ctx, "Currently Multi-Blow requires primary die defined as object number 2."));
}

TEST_CASE(express_sync_keep_mesh)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    Operation* op = openCase(p, ctx, "95_B_FE3ACT", "Gear_blank.moproj");
    if (!op) return;
    mst::OpClass* c = p.registry.at(op->classKey).get();
    auto* pr = dynamic_cast<ExpressProps*>(op->props.get());
    REQUIRE(pr != nullptr);
    REQUIRE(pr->data != nullptr);
    engine::GuiModel* m = engine::opModel(p, *op);
    REQUIRE(m != nullptr);
    // a rigid die (kind 0) with nodes: vt+0x2c0(OB, 1) finds it while the setting is
    // formingType 0; keep = 1, so no mesh is deleted
    def::model::LLOBJ_DATA* die = nullptr;
    for (int i = 1; i <= m->objectCount() && !die; ++i) {
        def::model::LLOBJ_DATA* o = m->objectAt(i);
        if (o && *reinterpret_cast<std::int32_t*>(reinterpret_cast<unsigned char*>(o) + 0xc) == 0) die = o;
    }
    REQUIRE(die != nullptr);
    std::int32_t nodes = 0;
    std::memcpy(&nodes, reinterpret_cast<unsigned char*>(die) + 0x87c, sizeof nodes);
    if (nodes == 0) put<std::int32_t>(die, 0x87c, 1);
    op->oprFlag = 0;
    pr->data->stepDefinition = 0;
    pr->data->formingType = 0;
    pr->data->dieTemperatureType = 1;
    engine::FileEffects fx;
    c->syncFromModelTail(p, *op, ctx, fx);                   // vt+0xc8 of the class
    CHECK_EQ(pr->data->stepDefinition, 1);                       // 0x1413f6ae0(d, 0)
    CHECK(hasWarning(ctx, "Solution step mode has been switched to User defined."));
    CHECK(hasWarning(ctx, "Keep Mesh of Dies?"));
    CHECK_EQ(pr->data->formingType, 2);                          // Yes: 0x1413fd9d0(d, 2)
    CHECK_EQ(pr->data->dieTemperatureType, 0);                   // Yes: 0x141371aa0(d, 0)
    // vt+0x2b8: SfOprFormingExpress -> TRANS 2 = 1; formingType 2 -> TRANS 1 = 1
    CHECK_EQ(m->simCtl()->trans[1], 1);
    CHECK_EQ(m->simCtl()->trans[0], 1);
    if (nodes == 0) put<std::int32_t>(die, 0x87c, 0);
}

int main()
{
    return testkit::run_all();
}
