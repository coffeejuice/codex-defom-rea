// ctest-standalone: src/mst/emit_a.cpp src/core/basic.cpp tests/p08/doubles.cpp
// P08 tier 1: unit vectors per emitter (s1-02 6) and the P08 lines of COG_P1_B1.MST (corpus case
// 01_COG_P1_B1, simulations 1 and 2) rebuilt from the operations of COG_P1_B1.moproj.
#include <cstdio>
#include <string>

#include "m2m/mst/emit.hpp"

using namespace m2m;
using namespace m2m::mst;

static int g_fail = 0;

static void check(const char* what, const std::string& got, const std::string& want)
{
    if (got == want) return;
    ++g_fail;
    std::printf("FAIL %s\n--- got\n%s--- want\n%s---\n", what, got.c_str(), want.c_str());
}

struct Fixture {
    Stream ts{1252};
    State st;
    Ctx ctx;
    W w{ts, st, ctx};
    Project prj;
    String db = String(u"C:/x/COG_P1_B1.DB");

    Fixture()
    {
        prj.path = String(u"C:/x/COG_P1_B1.moproj");
        for (int id = 1; id <= 3; ++id) {
            auto m = std::make_unique<Item>();
            m->kind = ItemKind::Material;
            m->id = id;
            m->keyPath = String(u"C:/x/Materials\\Material0000") + qt::number(id) + String(u".KEY");
            prj.materials.rows.push_back(std::move(m));
        }
    }
    std::string take()
    {
        std::string s = ts.bytes();
        ts = Stream(1252);
        return s;
    }
    std::unique_ptr<Operation> op(const char* task, std::vector<int> ids,
                                   std::vector<std::vector<int>> mats)
    {
        auto o = std::make_unique<Operation>();
        o->project = &prj;
        o->classKey = String(u"SfOprHeatTransfer");
        o->taskDir = String::latin1(task);
        o->objIds = ids;
        for (size_t i = 0; i < mats.size(); ++i) objEntry(*o, static_cast<int>(i))->matIds = mats[i];
        return o;
    }
};

static const std::string RULE =
    "*-------------------------------------------------------------------------*\n";

static void test_units()
{
    Fixture f;
    auto o = f.op("Operations/Task00001", {}, {});

    emitSimHeader(f.w, 12, o.get());
    check("header", f.take(), "\n" + RULE + "SIMULATION\t12\n*!SIMTYP  \tSfOprHeatTransfer\n" + RULE);

    // 140134250: n == 1 needs nstartGiven and a non-zero step
    emitDimNstart(f.w, f.db, 1, o.get(), 7);
    check("dim none", f.take(), "");
    o->nstartGiven = 1;
    o->startStepNo = 0;
    emitDimNstart(f.w, f.db, 1, o.get(), 7);
    check("dim step 0", f.take(), "");
    o->startStepNo = 15;
    emitDimNstart(f.w, f.db, 1, o.get(), 7);
    check("dim 15", f.take(), "DIM    \t3\nNSTART    \t15\n");
    emitDimNstart(f.w, f.db, 1, nullptr, 7);
    check("dim null", f.take(), "");
    // n != 1: DBREAD with the NSTART step (-1 - v for negatives), else dbreadArg
    emitDimNstart(f.w, f.db, 2, o.get(), 7);
    check("dbread step", f.take(), "DBREAD    \t15\nCOG_P1_B1.DB\n");
    o->startStepNo = -3;
    emitDimNstart(f.w, f.db, 2, o.get(), 7);
    check("dbread neg", f.take(), "DBREAD    \t2\nCOG_P1_B1.DB\n");
    o->nstartGiven = 0;
    emitDimNstart(f.w, f.db, 3, o.get(), 7);
    check("dbread arg", f.take(), "DBREAD    \t7\nCOG_P1_B1.DB\n");
    emitDimNstart(f.w, f.db, 3, nullptr, -4);
    check("dbread null", f.take(), "DBREAD    \t-4\nCOG_P1_B1.DB\n");

    emitNstart(f.w, f.db, 2, 5);
    check("nstart n2", f.take(), "");
    emitDbread(f.w, f.db, 1, 5);
    check("dbread n1", f.take(), "");

    emitKfreadRestart(f.w, f.db, o.get());
    check("restart off", f.take(), "");
    o->restartGiven = 1;
    emitKfreadRestart(f.w, f.db, o.get());
    check("restart", f.take(), "KFREAD    \t1\nOperations/Task00001/RestartStep.KEY\n");

    emitKfreadUser(f.w, o.get());
    check("user off", f.take(), "");
    o->userKeyGiven = 1;
    emitKfreadUser(f.w, o.get());
    check("user empty", f.take(), "");
    o->userKey = String(u"C:\\abs\\User.KEY");
    emitKfreadUser(f.w, o.get());
    check("user", f.take(), "KFREAD    \t1\nC:\\abs\\User.KEY\n");   // not relativised

    emitOprnam(f.w, String(u"Op A"));
    emitSimnam(f.w, String(u"Op A"));
    emitCursim(f.w, 4, 2, 0);
    check("names", f.take(), "OPRNAM\nOp A\nSIMNAM\nOp A\nCURSIM    \t4\t2\t0\n");
    f.st.setCycleMode(9, String(u"Cycle 1"));
    f.st.setCycleIdx(3);
    emitOprnam(f.w, String(u"Op A"));
    emitSimnam(f.w, String(u"Op A"));
    emitCursim(f.w, 4, 2, 0);
    check("names cycle", f.take(), "OPRNAM\nCycle 1\nSIMNAM\nCYCL3-Op A\nCURSIM    \t4\t9\t0\n");
    f.st.clearCycleMode();

    emitGendb(f.w, f.db, 1, 5, false);
    emitGendb(f.w, f.db, 1, 5, true);
    emitGendb(f.w, f.db, 2, 2, false);
    emitGendb(f.w, f.db, 2, 0, true);
    check("gendb", f.take(),
          "GENDB     \t2\t1\t0\nCOG_P1_B1.DB\nGENDB     \t2\t2\t0\nCOG_P1_B1.DB\n"
          "GENDB     \t1\t1\t2\nCOG_P1_B1.DB\nGENDB     \t1\t2\t0\nCOG_P1_B1.DB\n");
    f.st.setKfwrit(String(u"OUT.KEY"));
    emitGendb(f.w, f.db, 2, 2, false);
    check("kfwrit", f.take(), "KFWRIT\nOUT.KEY\n");
    f.st.clearKfwrit();

    // materials: none when the manager is empty or there is no operation
    emitMaterials(f.w, f.prj, f.db, 1, nullptr);
    check("mat null", f.take(), "");
    Project empty;
    emitMaterials(f.w, empty, f.db, 1, o.get());
    check("mat empty mgr", f.take(), "");
    // distinct ids in first-seen order; -1 slots skipped, -2 slots used; unknown id 9 gives nothing
    auto m = f.op("Operations/Task00002", {-2, -1, 5, 6}, {{3, 1}, {2}, {1, 9}, {3, 2}});
    std::vector<String> ids = materialIdStrings(*m);
    std::string joined;
    for (auto& s : ids) joined += qt::toAscii(s) + ",";
    check("mat ids", joined, "3,1,9,2,");
    emitMaterials(f.w, f.prj, f.db, 1, m.get());
    check("mats", f.take(),
          "KFREAD    \t1\nMaterials/Material00003.KEY\nKFREAD    \t1\nMaterials/Material00001.KEY\n"
          "KFREAD    \t1\nMaterials/Material00002.KEY\n");

    check("itemKeyPath none", qt::toAscii(itemKeyPathClean(f.prj.materials, 4)), "");
    if (!itemKeyPathClean(f.prj.materials, 4).isNull()) {
        ++g_fail;
        std::printf("FAIL itemKeyPath null\n");
    }
    check("cyclePrefix", qt::toAscii(cyclePrefix(f.st)), "CYCL3");

    check("kw3", keywordName(3), "KFREAD");
    check("kw10", keywordName(10), "KFWRIT");
    check("kw48", keywordName(0x30), "DSMAX");
    check("kw50", keywordName(0x32), "DTMAX");
    check("kw51", keywordName(0x33), "DTTYPE");
    check("kw0", keywordName(0), "PROBID");
    check("kw missing", keywordName(100000), "ERROR: KEYWORD NOT EXISTS!");
    check("kw index", std::to_string(keywordIndex(-7)), "-1");
}

// COG_P1_B1.MST simulations 1 and 2 (the P08 lines only); objects and MatId as in COG_P1_B1.moproj
// (Task00004 slot 0 is Id -2 with MatId "", so only Material00002 is read).
static void test_cog_p1_b1()
{
    Fixture f;
    auto s1 = f.op("Operations/Task00001", {1}, {{1}});
    s1->nstartGiven = 1;
    s1->startStepNo = -1;
    s1->name = String(u"HeatTransfer");
    int n = 1;
    emitSimHeader(f.w, n, s1.get());
    emitDimNstart(f.w, f.db, n, s1.get(), 0);
    emitSimCtrl(f.w, f.prj, f.db, n, s1.get());
    emitMaterials(f.w, f.prj, f.db, n, s1.get());
    std::string a = f.take();
    check("cog sim1 head", a,
          "\n" + RULE + "SIMULATION\t1\n*!SIMTYP  \tSfOprHeatTransfer\n" + RULE +
              "DIM    \t3\nNSTART    \t-1\n"
              "MDSOBJ    \t  0       0       0    0.0000000000E+000\n"
              "TNOW\t   \t  -1E21       0       1\n"
              "KFREAD    \t1\nOperations/Task00001/SimCtrl.KEY\n"
              "KFREAD    \t1\nMaterials/Material00001.KEY\n");
    emitKfreadUser(f.w, s1.get());
    emitOprnam(f.w, s1->name);
    emitSimnam(f.w, s1->name);
    emitCursim(f.w, n, 1, 0);
    emitGendb(f.w, f.db, n, 2, false);
    check("cog sim1 tail", f.take(),
          "OPRNAM\nHeatTransfer\nSIMNAM\nHeatTransfer\nCURSIM    \t1\t1\t0\nGENDB     \t2\t1\t0\nCOG_P1_B1.DB\n");

    auto s2 = f.op("Operations/Task00004", {-2, 11, 12}, {{}, {2}, {2}});
    s2->nstartGiven = 1;
    s2->startStepNo = -1;
    s2->name = String(u"HeatTransfer (3)");
    n = 2;
    emitSimHeader(f.w, n, s2.get());
    emitDimNstart(f.w, f.db, n, s2.get(), 0);
    emitSimCtrl(f.w, f.prj, f.db, n, s2.get());
    emitMaterials(f.w, f.prj, f.db, n, s2.get());
    check("cog sim2 head", f.take(),
          "\n" + RULE + "SIMULATION\t2\n*!SIMTYP  \tSfOprHeatTransfer\n" + RULE +
              "DBREAD    \t0\nCOG_P1_B1.DB\n"
              "MDSOBJ    \t  0       0       0    0.0000000000E+000\n"
              "TNOW\t   \t  -1E21       0       1\n"
              "KFREAD    \t1\nOperations/Task00004/SimCtrl.KEY\n"
              "KFREAD    \t1\nMaterials/Material00002.KEY\n");
}

int main()
{
    // In the full build (m2m_core) the seams of P02/P04/P05 are stubs until those packets merge;
    // tests/p08/CMakeLists.txt builds this file against doubles.cpp instead.
    try {
        test_units();
        test_cog_p1_b1();
    } catch (const NotPorted& e) {
        std::printf("SKIP: a dependency is not merged yet: %s\n", e.what());
        return 0;
    }
    std::printf(g_fail ? "%d failures\n" : "all P08 tests passed\n", g_fail);
    return g_fail ? 1 : 0;
}
