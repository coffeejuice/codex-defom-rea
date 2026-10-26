#include "../support/test_paths.hpp"
// MDT DB (docs/re/moproj2mst/mdt/DB): the project DB handle (DB-1), the step availability check
// (DB-2), the step read (DB-3) and the GUI sync (DB-4).
// Inputs are imported M2M_DB_FIXTURE copies in local scratch (skipped when missing).
// (project + 9 MB COG_P1_B1.DB) and the oracle B cases 124_B_DBSTEP / 125_B_DBSTEP_NODB /
// 126_B_DBSTEP_REN (deform-rea-work/gui_mo/oracle_b, COG_P1_B1 copies with op Id 2 "EXIT" at
// OprFlag 0 and active; <case>_before = input, <case> = the files after the GUI's Save).
// Expected values come from routes independent of the port: the deformdb reader (directory, step
// headers), DEF_PRE's KEY export of step -2 (oracle cache dbkey/COG_P1_B1/-2, STEP_M2.KEY:
// NSTART -2, TMAX 0.1, MTNAME 1 "IN718_6um[1650-2200F(900-1200C)]", MTNAME 2 "AISI-H-13",
// MSTMTR n 0, OBJNAM 1..3 "Workpiece", "Top 650 New", "Bot 650 New"), the class-flag table of the
// binary (MDT DB spec 2.2) and the files the GUI saved in 124_B_DBSTEP.
// ctest-labels: m2m-mdt
#include <testkit/check.hpp>

#include <process.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <set>
#include <string>
#include <vector>

#include <deformdb/database.hpp>
#include <defdbio/db_query.hpp>
#include <defdbio/step/db_header_cache.hpp>
#include <defmodel/layout/cls_1401dbfe0.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defmodel/step/db_step.hpp>

#include "m2m/convert.hpp"
#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/qt4path.hpp"
#include "../../src/engine/engine_internal.hpp"

namespace fs = std::filesystem;
using namespace m2m;

namespace {

const fs::path kCog = m2m_test::configuredPath("M2M_DB_FIXTURE");

fs::path oracleB()
{
    if (const char* e = std::getenv("M2M_ORACLE_B")) return fs::path(e);
    return m2m_test::configuredPath("M2M_ORACLE_B");
}

std::string readAll(const fs::path& f)
{
    std::ifstream in(f, std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

fs::path tempRoot(const char* tag)
{
    const fs::path d = m2m_test::scratchRoot() / ("m2m_db_" + std::to_string(_getpid())) / tag;
    std::error_code ec;
    fs::remove_all(d, ec);
    fs::create_directories(d);
    return d;
}

// copy a folder tree (no png/json screenshots needed, but they are small)
void copyTree(const fs::path& from, const fs::path& to)
{
    fs::create_directories(to);
    fs::copy(from, to, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
}

// The DB copy is made read only; its bytes and time stamp are compared after the test
struct ReadOnlyDb {
    fs::path f;
    std::string bytes;
    fs::file_time_type t;
    explicit ReadOnlyDb(const fs::path& db) : f(db)
    {
        fs::permissions(f, fs::perms::owner_write | fs::perms::group_write | fs::perms::others_write,
                        fs::perm_options::remove);
        bytes = readAll(f);
        t = fs::last_write_time(f);
    }
    bool unchanged() const { return readAll(f) == bytes && fs::last_write_time(f) == t; }
    ~ReadOnlyDb()
    {
        std::error_code ec;
        fs::permissions(f, fs::perms::owner_write, fs::perm_options::add, ec);
    }
};

Project* openProj(Project& p, Ctx& ctx, const fs::path& moproj)
{
    const qt::String file{moproj.u16string()};
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(p);
    if (!openProject(p, file, ctx) || !p.openedOk) return nullptr;
    activateSubProject(p, 0, ctx);
    return &p;
}

// deformdb: per stored entry (MAR size >= 1, the directory's +0x24) the operation number (int)H[10]
std::vector<int> deformdbOpNumbers(const fs::path& db)
{
    auto ref = deformdb::Database::open(db);
    std::vector<int> out;
    int last = -1;
    for (const auto& e : ref->entries()) {
        if (e.mar_size < 1) continue;
        deformdb::ReadOptions o;
        o.arrays = false;
        o.dynamic = false;
        const deformdb::StepData sd = ref->read_step(e, o);
        const int op = static_cast<int>(sd.header[10]);
        if (op > last) {
            last = op;
            out.push_back(op);
        }
    }
    return out;
}

}  // namespace

// DB-1: setPath / open on a copy of COG_P1_B1.DB; the directory count and the per-operation list
// against the deformdb reader; a missing file stays unloaded without an error; read only.
TEST_CASE(db_handle_open)
{
    if (!fs::exists(kCog / "COG_P1_B1.DB")) {
        std::printf("SKIP: no %s\n", (kCog / "COG_P1_B1.DB").string().c_str());
        return;
    }
    const fs::path dir = tempRoot("handle");
    fs::copy_file(kCog / "COG_P1_B1.DB", dir / "COG_P1_B1.DB");
    {
        ReadOnlyDb ro(dir / "COG_P1_B1.DB");
        auto ref = deformdb::Database::open(dir / "COG_P1_B1.DB");
        REQUIRE(ref != nullptr);
        Ctx ctx;
        engine::DbHandle h;
        // 0x140cdcdc0: ".DB" appended to a name without it, cleanPath
        engine::dbSetPath(h, qt::String((dir / "COG_P1_B1").u16string()), false);
        CHECK(h.pathSet);
        CHECK(qt::endsWith(h.path, qt::L("/COG_P1_B1.DB")));
        CHECK(!h.loaded);
        CHECK(engine::dbOpen(h, ctx, true, false));
        CHECK(h.loaded);
        CHECK(ctx.warnings.empty());                       // no split chain: no "2G Split DBs" question
        CHECK_EQ(h.reader->uac8, static_cast<std::int32_t>(ref->entries().size()));
        CHECK_EQ(h.reader->uac8, 32);
        CHECK(h.reader->u1900 != 0);                       // header cache built (1401dd250)
        const std::vector<engine::DbOperation> ops = engine::dbOperationList(*h.reader);
        const std::vector<int> want = deformdbOpNumbers(dir / "COG_P1_B1.DB");
        REQUIRE(ops.size() == want.size());
        for (std::size_t i = 0; i < ops.size(); ++i) CHECK_EQ(ops[i].opNo, want[i]);
        // the step read (DB-3) on the last stored entry: object count = (int)H[0] of deformdb
        int last = -1;
        for (std::size_t i = 0; i < ref->entries().size(); ++i)
            if (ref->entries()[i].mar_size >= 1) last = static_cast<int>(i);
        REQUIRE(last >= 0);
        deformdb::ReadOptions o;
        o.arrays = false;
        o.dynamic = false;
        const deformdb::StepData sd = ref->read_step(ref->entries()[static_cast<std::size_t>(last)], o);
        engine::GuiModel m(&h, 3);
        CHECK(engine::readDbEntry(m, &last, true));
        CHECK_EQ(m.objectCount(), static_cast<int>(sd.header[0]));
        CHECK_EQ(m.step->u744, ref->entries()[static_cast<std::size_t>(last)].step);
        CHECK_EQ(m.state100, 1);
        // refresh at an operation open: unchanged file -> header re-read only, still loaded
        CHECK(engine::dbRefreshIfChanged(h));
        CHECK(h.loaded);
        CHECK(ro.unchanged());
    }
    // a missing file: not loaded, no error
    Ctx ctx;
    engine::DbHandle miss;
    engine::dbSetPath(miss, qt::String((dir / "NONE.DB").u16string()), false);
    CHECK(!engine::dbOpen(miss, ctx, true, false));
    CHECK(!miss.loaded);
    CHECK(engine::dbOperationList(*miss.reader).empty());
    CHECK(!engine::dbRefreshIfChanged(miss));
    // FOR003 keeps its name
    engine::DbHandle f3;
    engine::dbSetPath(f3, qt::L("C:/x/FOR003"), false);
    CHECK_EQ(f3.path.utf8(), std::string("C:/x/FOR003"));
}

// DB-3: step -2 read into a GuiModel against DEF_PRE's KEY export of the same step
TEST_CASE(db_step_read_vs_key_export)
{
    if (!fs::exists(kCog / "COG_P1_B1.DB")) return;
    const fs::path dir = tempRoot("step");
    fs::copy_file(kCog / "COG_P1_B1.DB", dir / "COG_P1_B1.DB");
    ReadOnlyDb ro(dir / "COG_P1_B1.DB");
    Ctx ctx;
    engine::DbHandle h;
    engine::dbSetPath(h, qt::String((dir / "COG_P1_B1.DB").u16string()), false);
    REQUIRE(engine::dbOpen(h, ctx, true, false));
    engine::GuiModel m(&h, 3);
    const int step = -2;
    REQUIRE(engine::readDbStep(m, &step, true));
    CHECK_EQ(m.step->u744, -2);
    CHECK_EQ(m.objectCount(), 3);
    const def::model::Cls_1401e7b10* sc = m.simCtl();
    CHECK_EQ(sc->nstart, -2);
    CHECK(sc->tmax == 0.1);
    CHECK_EQ(m.step->n_materials, 2);
    def::model::Material* m1 = def::model::material_list_find(&m.step->materials, 1);
    def::model::Material* m2 = def::model::material_list_find(&m.step->materials, 2);
    REQUIRE(m1 != nullptr && m2 != nullptr);
    CHECK_EQ(std::string(m1->name), std::string("IN718_6um[1650-2200F(900-1200C)]"));
    CHECK_EQ(std::string(m2->name), std::string("AISI-H-13"));
    CHECK_EQ(m1->mstmtr_flag, 0);
    static const char* names[3] = {"Workpiece", "Top 650 New", "Bot 650 New"};
    for (int i = 0; i < 3; ++i) {
        def::model::LLOBJ_DATA* o = m.object(i + 1);
        REQUIRE(o != nullptr);
        CHECK_EQ(std::string(engine::at<char>(o, 0x828)), std::string(names[i]));
    }
    const int missing = 99999;                                // not in the directory: 1401ddf60 -> -1
    CHECK(!engine::readDbStep(m, &missing, true));
    CHECK(ro.unchanged());
}

// OB vt+0x1f0 (0x1405f68e0): class-info flag 0x01000000 only for Convert2Dto3D, CopyMirror and
// Boolean2D/3D (flags 0x1100009d, 0x2100009a, 0x01000099 / 0x0100009a; MDT DB spec 2.2)
TEST_CASE(class_flag_one_step_prev)
{
    Project p;
    ProjectScope scope(p);
    makeRegistry(p);
    for (const auto& kv : p.registry) {
        const std::string k = kv.first.utf8();
        const bool want = k == "SfOprConvert2Dto3D" || k == "SfOprCopyMirror" || k == "SfOprBoolean2D" ||
                          k == "SfOprBoolean3D";
        if (kv.second->oneStepPrev() != want) std::printf("class %s: vt+0x1f0 %d\n", k.c_str(), kv.second->oneStepPrev());
        CHECK(kv.second->oneStepPrev() == want);
    }
}

namespace {

struct Row {
    int step;
    int stored;
    int op;
    const char* name;
};

// a hand-built DB reader: directory (+0x04 step, +0x24 stored) and header cache (+0x38 operation
// number, +0x4c operation name), cache built
void fillReader(def::model::Cls_1401dbfe0& db, const std::vector<Row>& rows)
{
    namespace st = def::dbio::step;
    const int n = static_cast<int>(rows.size());
    db.uac8 = n;
    auto* r = new def::model::DbDirRecord[static_cast<std::size_t>(n > 0 ? n : 1)];
    auto* c = new def::model::DbInfoEntryE0[static_cast<std::size_t>(n > 0 ? n : 1)];
    for (int i = 0; i < n; ++i) {
        st::init_cache_entry(c[i]);
        r[i].step = rows[static_cast<std::size_t>(i)].step;
        r[i].stored = rows[static_cast<std::size_t>(i)].stored;
        r[i].dim = 3;
        st::entry_i32(c[i], 0x38) = rows[static_cast<std::size_t>(i)].op;
        std::strncpy(st::entry_chars(c[i], 0x4c), rows[static_cast<std::size_t>(i)].name, 0x40);
    }
    db.uad8 = r;
    db.uae8 = c;
    db.u1900 = 1;
}

}  // namespace

// DB-2: 0x14005f520 / 0x1400b8b80 on the oracle B 125 project (no DB: the handle is not loaded)
// with hand-built DB readers. The chain's operation names come from the project; op Id 2 "EXIT"
// is at chain position 4 (the GUI's dialog text "[Op. 4] EXIT", oracle B 124).
TEST_CASE(db_has_steps_for_synthetic)
{
    const fs::path src = oracleB() / "125_B_DBSTEP_NODB_before";
    if (!fs::exists(src / "COG_P1_B1.moproj")) {
        std::printf("SKIP: no %s\n", src.string().c_str());
        return;
    }
    const fs::path dir = tempRoot("synth");
    copyTree(src, dir);
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openProj(p, ctx, dir / "COG_P1_B1.moproj") != nullptr);
    engine::DbHandle* h = engine::projectDb(p);
    CHECK(!h->loaded);                                      // no COG_P1_B1.DB in the case
    Operation* op = operationById(p, 2);
    REQUIRE(op != nullptr);
    CHECK_EQ(flowNumber(*op), 4);
    SubProject* sp = activeSubProject(p);
    REQUIRE(sp != nullptr);
    const std::vector<Operation*> chain = flowOperations(*sp);
    REQUIRE(chain.size() >= 4);
    CHECK(chain[3] == op);
    const std::string n1 = chain[0]->name.utf8(), n2 = chain[1]->name.utf8(), n3 = chain[2]->name.utf8();
    Operation* prev = engine::prevOp(p, *op);
    REQUIRE(prev == chain[2]);
    mst::OpClass* prevClass = opBase(*prev);
    REQUIRE(prevClass != nullptr);
    CHECK(!prevClass->oneStepPrev());

    int lm = 0, un = 0;
    // empty DB (125): no match, false (the GUI: no dialog, KEY load)
    CHECK(!engine::dbHasStepsFor(p, *op, &lm, &un));
    CHECK_EQ(lm, 0);

    auto reset = [&](const std::vector<Row>& rows) {
        h->reader = std::make_unique<def::model::Cls_1401dbfe0>();
        fillReader(*h->reader, rows);
    };
    // ops 1..3 match, op 3 has two stored steps -> true, lastMatch 3 (< idx 4: last step of op 3)
    reset({{-1, 1, 1, n1.c_str()}, {1, 1, 1, n1.c_str()}, {-2, 1, 2, n2.c_str()}, {2, 1, 2, n2.c_str()},
           {-3, 1, 3, n3.c_str()}, {5, 1, 3, n3.c_str()}});
    lm = 0;
    CHECK(engine::dbHasStepsFor(p, *op, &lm, &un));
    CHECK_EQ(lm, 3);
    CHECK_EQ(def::dbio::query::op_last_step(*h->reader, 3), 5);
    // op 3 with one stored step: n = 1 -> false for a class without the flag, true with it
    reset({{1, 1, 1, n1.c_str()}, {2, 1, 2, n2.c_str()}, {-3, 1, 3, n3.c_str()}, {3, 0, 3, n3.c_str()}});
    lm = 0;
    CHECK(!engine::dbHasStepsFor(p, *op, &lm, &un));
    CHECK_EQ(lm, 3);
    const int saved = prevClass->dimFlags;
    prevClass->dimFlags |= 0x01000000;
    lm = 0;
    CHECK(engine::dbHasStepsFor(p, *op, &lm, &un));
    prevClass->dimFlags = saved;
    // the name of op 2 differs: the prefix stops at 1 -> 1 + 1 < 4, false
    reset({{1, 1, 1, n1.c_str()}, {2, 1, 2, "other"}, {-3, 1, 3, n3.c_str()}, {5, 1, 3, n3.c_str()}});
    lm = 0;
    CHECK(!engine::dbHasStepsFor(p, *op, &lm, &un));
    CHECK_EQ(lm, 1);
    // operation numbers that are not 1, 2, 3: no match
    reset({{1, 1, 2, n1.c_str()}, {2, 1, 3, n2.c_str()}, {5, 1, 4, n3.c_str()}});
    lm = 0;
    CHECK(!engine::dbHasStepsFor(p, *op, &lm, &un));
    CHECK_EQ(lm, 0);
    // the DB holds op 4 too (name "EXIT"): lastMatch 4 >= idx (first step of op 4)
    reset({{1, 1, 1, n1.c_str()}, {2, 1, 2, n2.c_str()}, {-3, 1, 3, n3.c_str()}, {5, 1, 3, n3.c_str()},
           {-6, 1, 4, op->name.utf8().c_str()}, {7, 1, 4, op->name.utf8().c_str()}});
    lm = 0;
    CHECK(engine::dbHasStepsFor(p, *op, &lm, &un));
    CHECK(lm >= 4);
    CHECK_EQ(def::dbio::query::op_first_step(*h->reader, 4), -6);
    // a stored step list shorter than the chain stops the prefix at its end (spec Q4 deviation)
    reset({{1, 1, 1, n1.c_str()}});
    lm = 0;
    CHECK(!engine::dbHasStepsFor(p, *op, &lm, &un));
    CHECK_EQ(lm, 1);
    // 0x14005f270: flow number < 2 -> true; otherwise the same test (position 2: op 1 matches, but it
    // has one stored step and HeatTransfer has no vt+0x1f0 flag -> false; position 4: as above)
    CHECK_EQ(flowNumber(*chain[0]), 1);
    CHECK(engine::interactiveSetup(p, *chain[0]));
    CHECK(!engine::interactiveSetup(p, *chain[1]));
    CHECK(!engine::interactiveSetup(p, *op));
    reset({{-1, 1, 1, n1.c_str()}, {1, 1, 1, n1.c_str()}});
    CHECK(engine::interactiveSetup(p, *chain[1]));
}

namespace {

struct Opened {
    Project p;
    Ctx ctx;
    engine::FileEffects open, save;
    Operation* op = nullptr;
};

}  // namespace

// 125 (no DB) and 126 (DB renamed COG_REN.DB, <Database Name> edited): no Setup Type question,
// the KEY load (the handle reads <moproj base>.DB, spec Q3)
TEST_CASE(oracle_b_125_126_key_load)
{
    for (const char* c : {"125_B_DBSTEP_NODB_before", "126_B_DBSTEP_REN_before"}) {
        const fs::path src = oracleB() / c;
        if (!fs::exists(src / "COG_P1_B1.moproj")) continue;
        const fs::path dir = tempRoot(c);
        copyTree(src, dir);
        Opened o;
        ProjectScope scope(o.p);
        REQUIRE(openProj(o.p, o.ctx, dir / "COG_P1_B1.moproj") != nullptr);
        CHECK(!engine::projectDb(o.p)->loaded);
        CHECK(qt::endsWith(engine::projectDb(o.p)->path, qt::L("/COG_P1_B1.DB")));
        o.op = engine::openActiveOperation(o.p, o.ctx, o.open);
        REQUIRE(o.op != nullptr);
        CHECK_EQ(o.op->id, 2);
        CHECK(o.ctx.warnings.empty());                      // no Setup Type question
        CHECK_EQ(o.op->restartGiven, 0);
        CHECK_EQ(o.op->nstartGiven, 0);
        CHECK_EQ(o.op->oprFlag, 1);
    }
}

// 124_B_DBSTEP: the Setup Type question answered Yes, the operation loaded from the DB step, then
// the files of Save step 2 against the ones the GUI saved
TEST_CASE(oracle_b_124_db_step_load)
{
    const fs::path src = oracleB() / "124_B_DBSTEP_before";
    const fs::path gui = oracleB() / "124_B_DBSTEP";
    if (!fs::exists(src / "COG_P1_B1.DB") || !fs::exists(gui / "COG_P1_B1.moproj")) {
        std::printf("SKIP: no oracle B 124\n");
        return;
    }
    const fs::path dir = tempRoot("124");
    copyTree(src, dir);
    ReadOnlyDb ro(dir / "COG_P1_B1.DB");
    Opened o;
    ProjectScope scope(o.p);
    REQUIRE(openProj(o.p, o.ctx, dir / "COG_P1_B1.moproj") != nullptr);
    CHECK(engine::projectDb(o.p)->loaded);
    engine::setEngineVersion(u"");
    o.op = engine::openActiveOperation(o.p, o.ctx, o.open);
    REQUIRE(o.op != nullptr);
    CHECK_EQ(o.op->id, 2);
    REQUIRE(o.ctx.warnings.size() == 1);                   // the Setup Type question
    CHECK(o.ctx.warnings[0].find("[Op. 4] EXIT") != std::string::npos);
    // .moproj after the GUI's Save: StartStepNo -4, RestartKey RestartStep.KEY, OprFlag 1
    CHECK_EQ(o.op->nstartGiven, 1);
    CHECK_EQ(o.op->startStepNo, -4);
    CHECK_EQ(o.op->restartGiven, 1);
    CHECK_EQ(o.op->oprFlag, 1);
    // Objects: Id -2 x 3, names, MatId 1 / 2 / 2, object 2 AssignMovement 1 EquipmentID 5
    REQUIRE(o.op->objIds.size() == 3);
    static const char* names[3] = {"Workpiece", "Top 650 New", "Bot 650 New"};
    static const int mats[3] = {1, 2, 2};
    for (int s = 0; s < 3; ++s) {
        CHECK_EQ(o.op->objIds[static_cast<std::size_t>(s)], -2);
        ObjEntry* e = objEntry(*o.op, s);
        REQUIRE(e != nullptr);
        CHECK_EQ(e->name.utf8(), std::string(names[s]));
        CHECK(e->matIds.size() == 1 && e->matIds[0] == mats[s]);
        CHECK_EQ(e->assignMovement, s == 1);
        CHECK_EQ(e->equipmentId, s == 1 ? 5 : 0);
    }
    // Equipment_List: item 5 "New item", Equipments/Equipment00005.KEY
    const Item* eq = o.p.equipment.find(5);
    REQUIRE(eq != nullptr);
    CHECK_EQ(eq->name.utf8(), std::string("New item"));
    // load-time Material KEY writes: the GUI's files after Save are byte-identical to them
    std::set<std::string> mat;
    for (const auto& w : o.open.writes) {
        const std::string rel = m2m_test::effectRelative(fs::path(w.path.u16()), dir).generic_string();
        if (rel.rfind("Materials/", 0) != 0) continue;
        mat.insert(rel);
        const bool same = readAll(gui / rel) == w.bytes;
        if (!same) std::printf("DIFF load %s\n", rel.c_str());
        CHECK(same);
    }
    CHECK(mat.count("Materials/Material00001.KEY") == 1);
    CHECK(mat.count("Materials/Material00002.KEY") == 1);
    CHECK(o.open.deletes.empty());
    // Save step 2: every file written against the GUI's
    engine::saveCurrentOperation(o.p, *o.op, o.ctx, o.save);
    int compared = 0, diff = 0;
    for (const auto& w : o.save.writes) {
        const std::string rel = m2m_test::effectRelative(fs::path(w.path.u16()), dir).generic_string();
        const fs::path exp = gui / rel;
        if (!fs::exists(exp)) {
            std::printf("EXTRA %s\n", rel.c_str());
            ++diff;
            continue;
        }
        ++compared;
        if (readAll(exp) != w.bytes) {
            ++diff;
            std::printf("DIFF save %s (%zu vs %zu bytes)\n", rel.c_str(), w.bytes.size(), readAll(exp).size());
            if (const char* dump = m2m_test::scratchSetting("M2M_DB_DUMP")) {
                const fs::path dd = fs::path(dump) / rel;
                fs::create_directories(m2m_test::scratchOutput(dd).parent_path());
                std::ofstream(m2m_test::scratchOutput(dd), std::ios::binary) << w.bytes;
            }
        }
    }
    std::printf("124 save step 2: %d compared, %d different\n", compared, diff);
    CHECK(compared > 0);
    CHECK_EQ(diff, 0);
    CHECK(ro.unchanged());
}

// 124: the MST against the one the GUI wrote at its Save
TEST_CASE(oracle_b_124_mst)
{
    const fs::path src = oracleB() / "124_B_DBSTEP_before";
    const fs::path gui = oracleB() / "124_B_DBSTEP";
    if (!fs::exists(src / "COG_P1_B1.DB") || !fs::exists(gui / "COG_P1_B1.MST")) return;
    const fs::path dir = tempRoot("124mst");
    copyTree(src, dir);
    ReadOnlyDb ro(dir / "COG_P1_B1.DB");
    std::string mst;
    std::u16string outPath;
    const Result r = convertToBytes((dir / "COG_P1_B1.moproj").u16string(), u"", 0, 1252, mst, outPath);
    if (r.code != M2M_OK) std::printf("convert: %d %s\n", r.code, r.message.c_str());
    CHECK_EQ(r.code, M2M_OK);
    const std::string want = readAll(gui / "COG_P1_B1.MST");
    if (mst != want) {
        std::size_t i = 0;
        while (i < mst.size() && i < want.size() && mst[i] == want[i]) ++i;
        std::printf("MST differs at byte %zu (%zu vs %zu bytes)\n", i, mst.size(), want.size());
    }
    CHECK(mst == want);
    CHECK(ro.unchanged());
}

int main()
{
    const int rc = testkit::run_all();
    std::error_code ec;
    fs::remove_all(m2m_test::scratchRoot() / ("m2m_db_" + std::to_string(_getpid())), ec);
    return rc;
}
