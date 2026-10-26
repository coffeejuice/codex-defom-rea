// ctest-labels: m2m-p15
// ctest-standalone: src/ops/express.cpp src/ops/express_tree.cpp src/ops/props_base.cpp src/core/basic.cpp tests/p15/doubles.cpp
// Tier-1 test of the FormingExpress writer (s1-05 5 with the P15 corrections) against the doubles in
// doubles.cpp: writer-owned lines byte for byte, emitter calls as "[...]" markers, the simulation
// counter, and the base / restart / onlySim branches. No sample project has a FormingExpress, so the
// expectations come from the decompiles (s1-05 d4s.c 0x14141fc50, 0x1414216e0, 0x141422320).
#include <testkit/check.hpp>

#include <memory>
#include <string>

#include "m2m/data/common.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/mst/simcount.hpp"
#include "m2m/props_base.hpp"
#include "ops/express.hpp"

#include "doubles.hpp"

using namespace m2m;
using namespace m2m::mst;

namespace {

struct Fixture {
    Project prj;
    Operation op;
    std::unique_ptr<FormingExpressClass> cls;
    std::shared_ptr<FormingExpressProps> props;
    Ctx ctx;
    State st;
    Stream ts{1252};
    W w{ts, st, ctx};

    explicit Fixture(bool is2D)
    {
        p15test::is2DValue = is2D;
        p15test::flowNumberValue = 4;
        p15test::nextSimValue = 9;
        p15test::unitOk = true;
        p15test::unitValues = {{u"0.1 sec", 0.1}, {u"1.0 N/sec/mm/C", 11.0}, {u"IH", 5.0},
                               {u"DS", 0.05}, {u"DT", 0.02}};
        prj.unit = 1;
        prj.subs.push_back(std::make_unique<SubProject>());
        auto eq = std::make_unique<Item>();
        eq->kind = ItemKind::Equipment;
        eq->id = 7;
        eq->keyPath = qt::L("E/Eq7.KEY");
        prj.equipment.rows.push_back(std::move(eq));
        cls = std::make_unique<FormingExpressClass>(is2D);
        props = std::static_pointer_cast<FormingExpressProps>(cls->makeProps(1));
        props->owner = &op;
        props->common = std::make_shared<data::CommonBlock>();
        props->common->friction = 0.3;
        props->common->ihtc = qt::L("IH");
        props->common->dsmax = qt::L("DS");
        props->common->dtmax = qt::L("DT");
        op.props = props;
        op.project = &prj;
        op.opClass = cls.get();
        op.name = qt::L("FX");
        op.objIds = {1, 2, 3};
        auto& d = *props->fe;
        d.type = 3;
        d.express = 1;
    }
    std::string run(int onlySim = 0, int* simOut = nullptr)
    {
        int sim = 0;
        cls->writeMst(w, prj, qt::L("C:/p/X.DB"), &sim, &op, 0, onlySim);
        if (simOut) *simOut = sim;
        return ts.bytes();
    }
};

const char* const kContacts3 =
    "* Generate the contacts\n"
    "INICTC \t1\n"
    "GENCTC \t0.0001\n"
    "FRCFAC \t1\t2\t1\t0\t0.3\n"
    "IHTCOF \t1\t2\t0\t5\n"
    "IHTCOF \t1\t3\t0\t5\n";

std::string blTail(int n, int s, double doescl)
{
    char b[32];
    std::snprintf(b, sizeof b, "%.6g", doescl);
    return std::string("IMPEQP \t2\nREL:E/Eq7.KEY\nDOESCL \t") + b +
           "\nMOVCTL \t2\n"
           "DSMAX\t0.05\n"
           "DTTYPE\t0\t0\n"
           "NSTEP \t10000\n"
           "TRANS \t1\t1\t0\t0\t0\t0\n"
           "* Define next simulation number - if minimum die distance criteria met, jump to next operation\n"
           "NEXTSIMNO\t3\n" +
           std::to_string(n + 1) + "\t" + std::to_string(n + 1) + "\t9\n" +
           "[KFREAD_USER]\n[OPRNAM FX]\n[SIMNAM FX-BL" + std::to_string(s) + "]\n[CURSIM " +
           std::to_string(n) + " 4 0]\n[GENDB " + std::to_string(n) + " 2]\n";
}

}  // namespace

TEST_CASE(express_3d_two_blows)
{
    Fixture f(false);
    auto& d = *f.props->fe;
    d.blows = 2;
    d.steps = {2, 1};
    d.gap = {{0.5, 0.0}, {0.0}};
    d.drop = {{0, 1}, {0}};
    d.flip = {{0, 0}, {0}};
    d.stroke = {{50.0, 60.0}, {70.0}};
    f.op.objs.clear();
    objEntry(f.op, 1)->equipmentId = 7;
    int sim = 0;
    std::string got = f.run(0, &sim);
    std::string want =
        // b1 s1: BL (first block: DIM/NSTART and the object section)
        std::string("[SIMHDR 1]\n[DIMNSTART 1 0]\n[SIMCTRL 1]\n[MATERIALS 1]\n[OBJECTS 1 0]\n"
                    "[MESHSET 1]\n[OBJPROPS 1]\nPDIE \t2\n* Positioning\n[OBJPOS 1]\n[CNTACT 1]\n") +
        kContacts3 + blTail(1, 1, 0.5) +
        // b1 s1: DW (gap 0.5 > 0, dt 0.1 -> NSTEP 5)
        "[SIMHDR 2]\n[DBREAD 2 0]\n* Positioning\n*    Move Top die away\n"
        "OBJPOS \t2\t1\t-0\t-0\t100\n"
        "* Generate the contacts\nINICTC \t1\nGENCTC \t0.0001\n"
        "IHTCOF \t1\t2\t0\t11\nIHTCOF \t1\t3\t0\t11\n"
        "MOVCTL \t2\t1\t0\t0\t0\t-1\t0\nMOVCTL \t3\t1\t0\t0\t0\t1\t0\n"
        "DSMAX \t0\nDTMAX \t0.1\nNSTEP \t5\nTRANS \t1\t0\t0\t0\t0\t0\n"
        "[KFREAD_USER]\n[OPRNAM FX]\n[SIMNAM FX-DW1]\n[CURSIM 2 4 0]\n[GENDB 2 2]\n"
        // b1 s2: BL (previous step had a gap: top die back)
        "[SIMHDR 3]\n[DBREAD 3 0]\nPDIE \t2\n[CNTACT 3]\n"
        "* Positioning\n*    Move Top die back\nOBJPOS \t2\t1\t0\t0\t-100\n" +
        kContacts3 + blTail(3, 2, 0.6) +
        // b1 s2: TO (drop on, 10 / 1 -> NSTEP 10; no DW in this step, so the top die moves too)
        "[SIMHDR 4]\n[DBREAD 4 0]\nPDIE \t1\n"
        "* Initialize node temperature\nNDTMP \t1\t0\t950\n"
        "* Positioning\n*    Move Top die away\nOBJPOS \t2\t1\t-0\t-0\t100\n"
        "*    Move Bottom die away\nOBJPOS \t3\t1\t0\t0\t-100\n"
        "* Generate the contacts\nINICTC \t1\nGENCTC \t0.0001\n"
        "DSMAX \t0\nDTMAX \t1\nNSTEP \t10\nTRANS \t1\t0\t0\t0\t0\t0\n"
        "[KFREAD_USER]\n[OPRNAM FX]\n[SIMNAM FX-TO2]\n[CURSIM 4 4 0]\n[GENDB 4 2]\n"
        // b2 s1: BL only (SIMNAM uses the step number)
        "[SIMHDR 5]\n[DBREAD 5 0]\nPDIE \t2\n[CNTACT 5]\n" +
        kContacts3 + blTail(5, 1, 0.7);
    CHECK(got == want);
    if (got != want) std::printf("--- got\n%s--- want\n%s", got.c_str(), want.c_str());
    CHECK_EQ(sim, 5);
    CHECK_EQ(p15test::nextSimArg, 5);
    CHECK(f.cls->moveDist == 100.0);
}

TEST_CASE(express_2d_flip_and_drop)
{
    Fixture f(true);
    auto& d = *f.props->fe;
    f.props->common->dtMode = 1;
    d.toStep = 4.0;                       // 10 / 4 + 0.5 = 3 -> NSTEP 3
    d.initStrain = 1;
    d.blows = 1;
    d.steps = {3};
    d.gap = {{0.0, 0.0, 0.0}};
    d.drop = {{1, 0, 0}};
    d.flip = {{0, 1, 0}};
    d.stroke = {{0.0, 0.0, 0.0}};
    f.op.objs.clear();                    // no equipment: no IMPEQP / DOESCL
    std::string got = f.run();
    // The 2D direction (0, -1, 0): back = (-0, 100), angle = -90.
    std::string c2 = std::string("* Generate the contacts\nINICTC \t1\nGENCTC \t0.0001\n"
                                 "FRCFAC \t1\t2\t1\t0\t0.3\nIHTCOF \t1\t2\t0\t5\nIHTCOF \t1\t3\t0\t5\n"
                                 "DTMAX\t0.02\t0\nDTTYPE\t0\t0\nNSTEP \t10000\nTRANS \t1\t1\t0\t0\t0\t0\n"
                                 "* Define next simulation number - if minimum die distance criteria met, jump to next operation\n"
                                 "NEXTSIMNO\t3\n");
    std::string want =
        std::string("[SIMHDR 1]\n[DIMNSTART 1 0]\n[SIMCTRL 1]\n[MATERIALS 1]\n[OBJECTS 1 0]\n"
                    "[MESHSET 1]\n[OBJPROPS 1]\nPDIE \t2\n* Positioning\n[OBJPOS 1]\n[CNTACT 1]\n") +
        c2 + "2\t2\t9\n[KFREAD_USER]\n[OPRNAM FX]\n[SIMNAM FX-BL1]\n[CURSIM 1 4 0]\n[GENDB 1 2]\n" +
        // TO after step 1 (2D OBJPOS with two components)
        "[SIMHDR 2]\n[DBREAD 2 0]\nPDIE \t1\n"
        "* Initialize node temperature\nNDTMP \t1\t0\t950\n"
        "* Initialize strain\nSTRAIN \t1\t0\t0\n"
        "* Positioning\n*    Move Top die away\nOBJPOS \t2\t1\t-0\t100\n"
        "*    Move Bottom die away\nOBJPOS \t3\t1\t0\t-100\n"
        "* Generate the contacts\nINICTC \t1\nGENCTC \t0.0001\n"
        "DSMAX \t0\nDTMAX \t4\nNSTEP \t3\nTRANS \t1\t0\t0\t0\t0\t0\n"
        "[KFREAD_USER]\n[OPRNAM FX]\n[SIMNAM FX-TO1]\n[CURSIM 2 4 0]\n[GENDB 2 2]\n"
        // step 2: previous step dropped -> both dies back
        "[SIMHDR 3]\n[DBREAD 3 0]\nPDIE \t2\n[CNTACT 3]\n"
        "* Positioning\n*    Move Top die back\nOBJPOS \t2\t1\t0\t-100\n"
        "*    Move Bottom die back\nOBJPOS \t3\t1\t-0\t100\n" +
        c2 + "4\t4\t9\n[KFREAD_USER]\n[OPRNAM FX]\n[SIMNAM FX-BL2]\n[CURSIM 3 4 0]\n[GENDB 3 2]\n" +
        // step 3: previous step flipped -> flip line, then the flipped placement
        "[SIMHDR 4]\n[DBREAD 4 0]\nPDIE \t2\n[CNTACT 4]\n"
        "* Flip\n*    Flip the workpiece with respect to a line perpendicular to the movement direction\n"
        "OBJPOS \t1\t4\t0\t0\t-1\t0\n"
        "* Positioning\n*    Put the flipped Workpiece on the Bottom die\n"
        "OBJPOS \t1\t2\t0.0001\t3\t0\t-1\n"
        "*    Put Top die on Workpiece\nOBJPOS \t2\t2\t0.0001\t1\t0\t-1\n" +
        c2 + "5\t5\t9\n[KFREAD_USER]\n[OPRNAM FX]\n[SIMNAM FX-BL3]\n[CURSIM 4 4 0]\n[GENDB 4 2]\n";
    CHECK(got == want);
    if (got != want) std::printf("--- got\n%s--- want\n%s", got.c_str(), want.c_str());
}

TEST_CASE(dw_angle_2d)
{
    Fixture f(true);
    auto& d = *f.props->fe;
    d.blows = 1;
    d.steps = {1};
    d.gap = {{0.25}};
    std::string got = f.run();
    CHECK(got.find("MOVCTL \t2\t1\t0\t-90\t0\nMOVCTL \t3\t1\t0\t90\t0\n") != std::string::npos);
    CHECK(got.find("OBJPOS \t2\t1\t-0\t100\n") != std::string::npos);   // DW top die away, 2D
    CHECK(got.find("NSTEP \t3\n") != std::string::npos);                // 0.25 / 0.1 + 0.5 = 3
}

TEST_CASE(not_express_uses_base_writer)
{
    Fixture f(false);
    f.props->fe->express = 0;
    CHECK(f.run() == "[BASE 1]\n");
    Fixture g(false);
    g.props->fe->type = 2;               // flag set but type not 3/4: base writer
    CHECK(g.run() == "[BASE 1]\n");
}

TEST_CASE(restart_first_block_and_only_sim)
{
    Fixture f(false);
    auto& d = *f.props->fe;
    d.blows = 1;
    d.steps = {2};
    d.gap = {{0.0, 0.0}};
    f.op.restartGiven = 1;
    std::string got = f.run();
    std::string head = "[SIMHDR 1]\n[KFREAD_RESTART]\n[KFREAD_USER]\n[OPRNAM FX]\n[SIMNAM FX-BL1]\n"
                       "[CURSIM 1 4 0]\n[GENDB 1 0]\n[SIMHDR 2]\n[DBREAD 2 0]\nPDIE \t2\n[CNTACT 2]\n"
                       "* Positioning\n*    Put Top die on Workpiece\n"
                       "OBJPOS \t2\t2\t0.0001\t0\t0\t-1\t1\n";
    CHECK(got.compare(0, head.size(), head) == 0);

    Fixture g(false);
    auto& e = *g.props->fe;
    e.blows = 1;
    e.steps = {2};
    e.gap = {{0.5, 0.0}};
    int sim = 0;
    std::string only = g.run(2, &sim);   // only the DW of step 1; the walk stops after it
    CHECK(only.compare(0, 24, "[SIMHDR 2]\n[DBREAD 2 0]\n") == 0);
    CHECK(only.find("[SIMNAM FX-DW1]") != std::string::npos);
    CHECK(only.find("BL") == std::string::npos);
    CHECK_EQ(sim, 2);
}

TEST_CASE(base_groups_fill_common_block)
{
    FormingExpressClass c(false);
    auto p = std::static_pointer_cast<FormingExpressProps>(c.makeProps(1));
    REQUIRE(p->common != nullptr);
    const auto& b = *p->common;
    CHECK_EQ(b.numSteps, 100);
    CHECK_EQ(b.stepIncr, 10);
    CHECK_EQ(b.dtMode, 0);
    CHECK_EQ(b.frictionType, 0);
    CHECK(b.friction == 0.0);
    CHECK(b.ihtc.utf8() == "0.0 N/sec/mm/C");
    CHECK(b.envTemp.utf8() == "20.00 C");
    CHECK(b.convection.utf8() == "0.0 N/sec/mm/C");
    CHECK(b.dsmax.utf8() == "0.0 mm");
    CHECK(b.dtmax.utf8() == "0.0 sec");
    REQUIRE(p->tree.size() == 2);
    CHECK(p->tree[1].name.utf8() == "TestGroup");
    std::string names;
    for (auto& n : p->tree[1].children) names += n.name.utf8() + ",";
    CHECK(names == "NumSteps,StepIncr,EnvTemp,ConvectionCoef,StepBy,DistPerStep,");
    names.clear();
    for (auto& n : p->tree[0].children.back().children) names += n.name.utf8() + ",";
    CHECK(names == "FrictionType,FrictionCoef,HeatTransCoef,");
    // StepBy = 1: TimePerStep replaces DistPerStep; base slots reached through the FE hook
    std::vector<PropNode*> anc{&p->tree[1]};
    p->applyValue(p->tree[1].children[4], anc, [] { qt::Variant v; v.kind = qt::Variant::Int; v.i = 1; return v; }());
    CHECK_EQ(p->common->dtMode, 1);
    CHECK(p->tree[1].children.back().name.utf8() == "TimePerStep");
    std::vector<PropNode*> anc2{&p->tree[0], &p->tree[0].children.back()};
    qt::Variant f;
    f.kind = qt::Variant::Double;
    f.d = 0.3;
    p->applyValue(p->tree[0].children.back().children[1], anc2, f);   // FrictionCoef
    CHECK(p->common->friction == 0.3);
    auto e = std::static_pointer_cast<FormingExpressProps>(c.makeProps(2));
    CHECK(e->common->envTemp.utf8() == "68.00 F");
    CHECK(e->common->dsmax.utf8() == "0.0 in");
    CHECK(e->common->ihtc.utf8() == "0.0 Btu/sec/in^2/F");
}

TEST_CASE(engine_facts_fresh_model)
{
    Fixture f(false);
    EngineFacts e = engineFacts(f.op);              // doubles: OpClass::dim() = 3
    CHECK(e.exists);
    CHECK_EQ(e.dim, 3);
    CHECK(e.is3D && !e.is2D);
    CHECK_EQ(e.objectCount, 0);
    CHECK_EQ(e.ecctmpWidth, 1);
    CHECK_EQ(e.ectmfnWidth, 1);
    f.op.project = nullptr;
    CHECK(!engineFacts(f.op).exists);
}

TEST_CASE(no_project_writes_nothing)
{
    Fixture f(false);
    f.props->fe->blows = 1;
    f.props->fe->steps = {1};
    f.op.project = nullptr;
    f.prj.subs.clear();
    int sim = 0;
    CHECK(f.run(0, &sim).empty());
    CHECK_EQ(sim, 0);
}

TEST_CASE(defaults_and_accessors)
{
    data::FormingExpressData d;
    data::initFormingExpressData(d, 2);
    CHECK(d.toTemp == 1800.0);
    CHECK(d.roomTemp == 68.0);
    CHECK(d.dwIhtc.utf8() == "0.0003 Btu/sec/in^2/F");
    CHECK(d.dwStep.utf8() == "0.1 sec");
    CHECK_EQ(d.type, 2);
    FormingExpressClass c3(false), c2(true);
    auto p3 = std::static_pointer_cast<FormingExpressProps>(c3.makeProps(1));
    auto p2 = std::static_pointer_cast<FormingExpressProps>(c2.makeProps(1));
    CHECK(p3->fe->dir[2] == -1.0 && p3->fe->dir[1] == 0.0);
    CHECK(p2->fe->dir[1] == -1.0 && p2->fe->dir[2] == 0.0);
    CHECK(p3->fe->toTemp == 950.0);
    p3->fe->steps = {4};
    p3->fe->gap = {{1.0}};
    CHECK_EQ(feStepCount(*p3, 1), 4);
    CHECK_EQ(feStepCount(*p3, 2), 0);
    CHECK(feGapAt(*p3, 1, 1) == 1.0);
    CHECK(feGapAt(*p3, 1, 2) == 0.0);
    CHECK(feGapAt(*p3, 0, 1) == 0.0);
    CHECK(!feDropAt(*p3, 3, 1));
    FormingExpressClass c(false);
    Operation op;
    op.name = qt::L("Op");
    CHECK(c.simNameBL(&op, 9, 2, false).utf8() == "Op-BL2");
    CHECK(c.simNameDW(&op, 9, 3, true).utf8() == "DW3");
    CHECK(c.simNameTO(&op, 1, 12, false).utf8() == "Op-TO12");
    // simcount.hpp data seams (P14's 0x141423390 body)
    p3->fe->express = 1;
    p3->fe->blows = 2;
    p3->fe->drop = {{1}};
    CHECK(simdata::feExpress(*p3));
    CHECK_EQ(simdata::feBlows(*p3), 2);
    CHECK_EQ(simdata::feSteps(*p3, 1), 4);
    CHECK(simdata::feGap(*p3, 1, 1) == 1.0);
    CHECK(simdata::feDrop(*p3, 1, 1));
    CHECK(simdata::feToTime(*p3) == 10.0);
    ExpressProps other;                   // not a FormingExpress props: the cast fails
    CHECK(!simdata::feExpress(other));
    CHECK_EQ(simdata::feBlows(other), 0);
    int counter = 0;
    std::vector<SimEntry> out;
    c.simList(1, &counter, &op, out);     // forwards to simListFormingExpress
    CHECK_EQ(counter, 100);
}

TEST_CASE(simctrl_from_props)
{
    p15test::unitOk = true;
    p15test::unitValues = {{u"DS", 0.05}, {u"DT", 0.02}};
    Props p;
    p.common = std::make_shared<data::CommonBlock>();
    p.common->numSteps = 7;
    p.common->stepIncr = 8;
    p.common->dsmax = qt::L("DS");
    p.common->dtmax = qt::L("DT");
    SimCtrlVals sc;
    initSimCtrlVals(sc);
    CHECK_EQ(sc.dtMode, 1);
    propsToSimCtrl(p, sc);                // mode 0: DSMAX, DTMAX cleared after 0x1405fb980 set it
    CHECK_EQ(sc.i16c, 7);
    CHECK_EQ(sc.i164, 8);
    CHECK_EQ(sc.dtMode, 0);
    CHECK(sc.dsmax == 0.05);
    CHECK(sc.dtmax == 0.0);
    p.common->dtMode = 1;
    initSimCtrlVals(sc);
    propsToSimCtrl(p, sc);
    CHECK(sc.dtmax == 0.02);
    CHECK(sc.dsmax == 0.0);
    p15test::unitOk = false;              // failed conversions leave the defaults
    initSimCtrlVals(sc);
    propsToSimCtrl(p, sc);
    CHECK(sc.dtmax == 0.0);
}

int main() { return testkit::run_all(); }
