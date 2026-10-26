#include "../support/test_paths.hpp"
// ctest-labels: m2m-mdt
// 0x14005b2f0 -> 0x14005cc00 -> 0x14005c890. The query cache is synthetic;
// actual object data is read from a scratch copy of the independently qualified
// COG_P1_B1.DB fixture (step -2: OBJNAM 1 Workpiece, DEF_PRE KEY export).
#include <testkit/check.hpp>
#include <process.h>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

#include <defdbio/step/db_header_cache.hpp>
#include <defmodel/layout/cls_1401dbfe0.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/step/db_step.hpp>
#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/qt4path.hpp"
#include "../../src/engine/engine_internal.hpp"

namespace fs = std::filesystem;
using namespace m2m;

namespace {
std::string bytes(const fs::path& p)
{
    std::ifstream in(p, std::ios::binary);
    return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}

bool fixtureAvailable()
{
    const auto fixture = m2m_test::configuredPath("M2M_DB_FIXTURE");
    if (fs::exists(fixture / "COG_P1_B1.DB") &&
        fs::exists(fixture / "Objects" / "Object00001.KEY")) return true;
    std::printf("SKIP: inherited DB fixture unavailable: %s\n", fixture.string().c_str());
    return false;
}

struct Rig {
    engine::DbHandle query; // must outlive the model's borrowed handle
    Project p;
    Operation* current = nullptr;
    fs::path dir;

    explicit Rig(const char* tag, bool previousSim = true)
    {
        static unsigned serial = 0;
        dir = m2m_test::scratchOutput(m2m_test::scratchRoot() /
            ("m2m_inherited_" + std::to_string(_getpid())) /
            (std::string(tag) + "_" + std::to_string(++serial)));
        fs::create_directories(dir);
        initProject(p, qt::String(dir.u16string()), qt::L("Inherited"));
        setUnit(p, 1);
        makeRegistry(p);
        auto* previous = createOperation(p, qt::L("Previous"),
            previousSim ? qt::L("SfOprForming") : qt::L("SfOprReport"), {}, 1);
        current = createOperation(p, qt::L("Current"), qt::L("SfOprForming"), {}, 2);
        REQUIRE(previous != nullptr);
        REQUIRE(current != nullptr);
        previous->objIds = {1};
        current->objIds = {-2};
        auto slot = std::make_unique<ObjEntry>();
        slot->number = 1;
        slot->matIds = {1};
        current->objs.push_back(std::move(slot));
        auto a = std::make_unique<FlowItem>();
        auto b = std::make_unique<FlowItem>();
        a->opId = 1; a->className = previous->classKey;
        b->opId = 2; b->className = current->classKey; b->col = 1;
        a->next = b.get(); b->prev = a.get();
        auto& flow = p.subs[0]->flow;
        flow.items.push_back(std::move(a));
        flow.items.push_back(std::move(b));
        flow.activeItem = 1;
        engine::dbSetPath(query, qt::String((dir / "absent-query.DB").u16string()), false);
        current->engineModel = std::make_shared<engine::GuiModel>(&query, 3);
    }

    void cache(const std::vector<int>& steps)
    {
        auto& db = *query.reader;
        db.uac8 = static_cast<int>(steps.size());
        auto* records = new def::model::DbDirRecord[steps.size()];
        auto* headers = new def::model::DbInfoEntryE0[steps.size()];
        for (std::size_t i = 0; i < steps.size(); ++i) {
            records[i].step = steps[i];
            records[i].stored = 1;
            records[i].sim_no = 1;
            records[i].dim = 3;
            def::dbio::step::init_cache_entry(headers[i]);
            def::dbio::step::entry_i32(headers[i], 0x38) = 1;
        }
        db.uad8 = records;
        db.uae8 = headers;
        db.u1900 = 1;
    }

    fs::path copyDb(bool dieStress = false)
    {
        fs::path dest = dir / "Inherited.DB";
        if (dieStress) {
            p.subs[0]->type = 4;
            p.subs[0]->dbName = qt::L("selected.DB");
            fs::create_directories(dir / "DieStress");
            dest = dir / "DieStress" / "selected.DB";
        }
        fs::copy_file(m2m_test::configuredPath("M2M_DB_FIXTURE") / "COG_P1_B1.DB", dest);
        return dest;
    }

    void fallbackKey()
    {
        std::string key = bytes(m2m_test::configuredPath("M2M_DB_FIXTURE") /
                                "Objects" / "Object00001.KEY");
        const auto at = key.find("Workpiece");
        REQUIRE(at != std::string::npos);
        key.replace(at, std::strlen("Workpiece"), "FILE_ONLY");
        const auto path = dir / "fallback.KEY";
        { std::ofstream out(path, std::ios::binary); out << key; }
        auto item = std::make_unique<Item>();
        item->kind = ItemKind::Object;
        item->id = 1;
        item->keyPath = qt::String(path.u16string());
        p.objects.rows.push_back(std::move(item));
    }

    def::model::LLOBJ_DATA* load()
    {
        ProjectScope scope(p);
        engine::FileEffects fx;
        auto* model = engine::loadFromKey(p, *current, true, fx);
        REQUIRE(model != nullptr);
        auto* obj = model->object(1);
        REQUIRE(obj != nullptr);
        CHECK_EQ(*engine::at<std::int32_t>(obj, 8), 1);
        CHECK_EQ(*engine::at<std::int32_t>(obj, 0xc), 5);
        CHECK_EQ(current->objs[0]->objectType, 5);
        CHECK(fx.writes.empty());
        return obj;
    }
};

std::string name(const def::model::LLOBJ_DATA* obj)
{
    return reinterpret_cast<const char*>(obj) + 0x828;
}
} // namespace

TEST_CASE(inherited_db_last_step_ignores_failed_refresh)
{
    if (!fixtureAvailable()) return;
    Rig r("last");
    const auto db = r.copyDb();
    const auto before = bytes(db);
    const auto stamp = fs::last_write_time(db);
    r.cache({99999, -2}); // first step cannot load; the original requests last=true
    CHECK(!r.query.loaded);
    CHECK(name(r.load()) == "Workpiece"); // no manager KEY exists: DB success bypasses it
    CHECK_EQ(r.query.reader->uac8, 2);     // missing refresh path retained its query cache
    CHECK(bytes(db) == before);
    CHECK(fs::last_write_time(db) == stamp);
}

TEST_CASE(inherited_db_reads_active_subproject_path)
{
    if (!fixtureAvailable()) return;
    Rig r("active_sub");
    r.copyDb(true);
    r.cache({-2});
    CHECK(!fs::exists(r.dir / "Inherited.DB"));
    CHECK(name(r.load()) == "Workpiece");
}

TEST_CASE(inherited_db_missing_file_and_missing_step_use_key)
{
    if (!fixtureAvailable()) return;
    for (bool fileExists : {false, true}) {
        Rig r(fileExists ? "missing_step" : "missing_file");
        if (fileExists) r.copyDb();
        r.cache({fileExists ? 99999 : -2});
        r.fallbackKey();
        CHECK(name(r.load()) == "FILE_ONLY");
    }
}

TEST_CASE(inherited_db_refresh_precedes_invalid_simulation)
{
    if (!fixtureAvailable()) return;
    Rig r("refresh_before_sim", false); // Report contributes no simulation; previous sim is zero
    const auto db = r.copyDb();
    engine::dbSetPath(r.query, qt::String(db.u16string()), false);
    CHECK_EQ(r.query.reader->uac8, 0);
    r.fallbackKey();
    CHECK(name(r.load()) == "FILE_ONLY");
    CHECK_EQ(r.query.reader->uac8, 32); // refresh happened although the DB selection was rejected
}

int main()
{
    return testkit::run_all();
}
