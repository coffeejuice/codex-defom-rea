// ctest-labels: m2m-p13
// ctest-standalone: src/core/basic.cpp src/ops/cycle.cpp src/ops/noop.cpp src/ops/distortion.cpp tests/p13/doubles.cpp src/ops/props_base.cpp src/model/props_rest_slots.cpp
// Tier-1 test of the P13 writers (s1-04 2, 3, 6) against the test doubles in doubles.cpp: the
// writer-owned lines byte for byte, the emitter calls with their exact arguments, the cycle
// context in mst::State, the simulation counter and the onlySim / firstPass branches.
#include <testkit/check.hpp>

#include <memory>
#include <string>

#include "m2m/data/distortion.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/state.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/props_base.hpp"

#include "doubles.hpp"
#include "ops/p13.hpp"

#if __has_include("m2m/data/cycle.hpp")
#include "m2m/data/cycle.hpp"
#else
namespace m2m::data {
struct CycleData {
    int count = 5;
};
}  // namespace m2m::data
#endif

using namespace m2m;
using namespace m2m::mst;
using m2m::qt::String;

namespace {

// A member class that records what the cycle hands it.
class Recorder : public OpClass {
public:
    Recorder() : OpClass(String(u"SfOprForming2D")) {}
    void writeMst(W& w, Project&, const String& db, int* sim, Operation* op, int a7, int a8) override
    {
        ++*sim;
        w.ts << "[member " << op->id << " sim " << *sim << " k " << w.st.cycleIdx << " mode "
             << (w.st.cycleMode ? 1 : 0) << " cursim " << w.st.cycleCursim << " opr " << w.st.cycleOprName
             << " cur " << (current == op ? 1 : 0) << " db " << db << " " << a7 << " " << a8 << "]" << endl;
    }
};

struct Fixture {
    Project prj;
    Ctx ctx;
    State st;
    Stream ts{1252};
    W w{ts, st, ctx};
    String db = String(u"P.DB");

    Operation& addOp(int id, const char16_t* name, OpClass* cls, std::shared_ptr<Props> props)
    {
        auto op = std::make_unique<Operation>();
        op->id = id;
        op->name = String(name);
        op->taskDir = String(u"Operations/Task0000") + String::latin1(std::to_string(id));
        op->project = &prj;
        op->opClass = cls;
        op->props = std::move(props);
        prj.operations.push_back(std::move(op));
        return *prj.operations.back();
    }
};

}  // namespace

TEST_CASE(cycle_runs_members_count_times)
{
    Fixture f;
    f.prj.subs.push_back(std::make_unique<SubProject>());
    Recorder rec;
    auto cyc = makeCycle();
    CHECK(cyc->key() == String(u"SfOprCycle"));
    CHECK(cyc->displayName() == String(u"Cycle"));
    auto props = cyc->makeProps(1);
    auto* cp = dynamic_cast<CycleProps*>(props.get());
    REQUIRE(cp && cp->data);
    CHECK_EQ(cp->data->count, 5);                       // 0x14134ef10 default
    cp->data->count = 3;
    f.addOp(2, u"Forming (2)", &rec, std::make_shared<Props>());
    Operation& c = f.addOp(3, u"Cycle", cyc.get(), props);
    p13test::members[3] = {2, 7};                       // 7: no such operation, skipped
    p13test::flowNumbers[3] = 2;
    int sim = 1;
    cyc->writeMst(f.w, f.prj, f.db, &sim, &c, 0, 0);
    CHECK_EQ(sim, 4);
    CHECK_EQ(f.ts.bytes(), std::string("[member 2 sim 2 k 1 mode 1 cursim 2 opr Cycle cur 1 db P.DB 0 0]\n"
                                       "[member 2 sim 3 k 2 mode 1 cursim 2 opr Cycle cur 1 db P.DB 0 0]\n"
                                       "[member 2 sim 4 k 3 mode 1 cursim 2 opr Cycle cur 1 db P.DB 0 0]\n"));
    CHECK(!f.st.cycleMode);                             // 0x140136b30
    CHECK_EQ(f.st.cycleIdx, 3);                         // not reset by the original
    CHECK(rec.current == nullptr);                      // restored after each member
}

TEST_CASE(cycle_corpus_shape_nine_iterations)
{
    // MO2_Operation_Cycles2: CycleCount 9, one member, SIMULATION 2..10.
    Fixture f;
    f.prj.subs.push_back(std::make_unique<SubProject>());
    Recorder rec;
    auto cyc = makeCycle();
    auto props = cyc->makeProps(1);
    dynamic_cast<CycleProps*>(props.get())->data->count = 9;
    f.addOp(2, u"Forming (2)", &rec, std::make_shared<Props>());
    Operation& c = f.addOp(3, u"Cycle", cyc.get(), props);
    p13test::members[3] = {2};
    int sim = 1;
    cyc->writeMst(f.w, f.prj, f.db, &sim, &c, 0, 0);
    CHECK_EQ(sim, 10);
}

TEST_CASE(cycle_without_props_or_subproject_writes_nothing)
{
    Fixture f;
    auto cyc = makeCycle();
    Operation& c = f.addOp(3, u"Cycle", cyc.get(), nullptr);
    int sim = 1;
    cyc->writeMst(f.w, f.prj, f.db, &sim, &c, 0, 0);    // no props
    c.props = cyc->makeProps(1);
    cyc->writeMst(f.w, f.prj, f.db, &sim, &c, 0, 0);    // no sub-project
    CHECK_EQ(sim, 1);
    CHECK(f.ts.bytes().empty());
    CHECK(!f.st.cycleMode);
}

TEST_CASE(group_and_doeoutput_write_nothing)
{
    Fixture f;
    auto g = makeGroup();
    auto d = makeDoeOutput();
    CHECK(g->key() == String(u"SfOprGroup"));
    CHECK(d->key() == String(u"SfOprDoeOutput"));
    CHECK(g->displayName() == String(u"Cycle"));
    CHECK(d->displayName() == String(u"DOE Output"));
    Operation& o = f.addOp(1, u"DOE", d.get(), std::make_shared<Props>());
    int sim = 4;
    g->writeMst(f.w, f.prj, f.db, &sim, &o, 0, 0);
    d->writeMst(f.w, f.prj, f.db, &sim, &o, 0, 0);
    CHECK_EQ(sim, 4);
    CHECK(f.ts.bytes().empty());
#if __has_include("m2m/mst/simcount.hpp")
    std::vector<SimEntry> recs;
    int cnt = 0;
    d->simList(1, &cnt, &o, recs);                      // 0x140e32990 (P14): empty
    CHECK(recs.empty());
    g->simList(1, &cnt, &o, recs);                      // base slot (double adds a marker)
    CHECK_EQ(recs.size(), static_cast<size_t>(1));
    makeCycle()->simList(2, &cnt, &o, recs);            // delegates to 0x14134e280
    makeDistortion(true)->simList(3, &cnt, &o, recs);   // delegates to 0x1413642d0
    REQUIRE(recs.size() == 3);
    CHECK_EQ(recs[1].sim, -13);
    CHECK_EQ(recs[2].sim, -14);
#endif
}

namespace {

std::string block(int sim, int i, int n, bool kfread, bool first)
{
    std::string s = "[140131c10 " + std::to_string(sim) + "]\n\n* Operation No " + std::to_string(sim);
    if (i == 0) s += " Original workpiece";
    else if (i == n + 1) s += " Unload";
    else s += " Pass No " + std::to_string(i);
    s += "\n";
    std::string S = std::to_string(sim);
    if (i == 0) {
        if (!first) s += "[1401343e0 " + S + " 0]\n";
        s += "[140134bf0 " + S + "]\n[140134db0 " + S + "]\n[1401360e0 " + S + "]\n[140135220 " + S +
             " 0]\n[140135da0 " + S + "]\n[140135f10 " + S + "]\n[140136040 " + S + "]\n[140136090 " + S +
             "]\n";
    } else {
        s += "[1401343e0 " + S + " 0]\n[140134bf0 " + std::to_string(i) + "]\n";
    }
    s += "[140132a40]\n[1401322d0]\n";
    std::string suf;
    if (i == 0) {
        s += "NSTEP\t0\n";
        suf = "LOAD";
    } else if (i == n + 1) {
        suf = "UNLOAD";
    } else {
        if (kfread) s += "KFREAD    \t1\nREL(C:/prj/Objects/Object00003.KEY)\n";
        suf = (i < 10 ? "PS0" : "PS") + std::to_string(i);
    }
    std::string I = std::to_string(i);
    s += "[140134680]\n[140134730 Machining Distortion]\n[1401347e0 Machining Distortion" + suf +
         "]\n[140134900 " + S + " 4 0]\n";
    s += " * Keyword file which contains objects \nKFWRIT\nOperations/Task00004/Step" + I + ".KEY\n\n";
    s += "EXECMD\t4\n";
    s += "DEF_MACHDIST_CTRL23.EXE   Operations/Task00004/DEF_MACHDIST_CTRL2.INI_" + I +
         "  >  Operations/Task00004/DEF_MACHDIST_CTRL2.OUT\n";
    s += "KFREAD\nOperations/Task00004/DEF_MACHDIST_OUT" + I + ".KEY\n";
    s += "[140134a10 " + S + " 2 " + (i == 0 ? "1" : "0") + "]\n";
    return s;
}

struct DistFixture : Fixture {
    std::unique_ptr<OpClass> cls = makeDistortion(true);
    std::shared_ptr<Props> props = cls->makeProps(1);
    DistortionProps* dp = dynamic_cast<DistortionProps*>(props.get());
    Operation* op = nullptr;

    DistFixture()
    {
        op = &addOp(4, u"Machining Distortion", cls.get(), props);
        op->objIds = {1, 2, 3};
        auto it = std::make_unique<Item>();
        it->id = 3;
        it->keyPath = String(u"C:/prj/Objects\\Object00003.KEY");
        prj.objects.rows.push_back(std::move(it));
        cls->current = op;                               // the MST walk sets it (0x1405ef280)
        p13test::flowNumbers[4] = 4;
        p13test::chainHeads[4] = false;
        dp->data->passCount = 2;
        dp->data->passObjects[0] = 3;                    // object no 3 -> slot 2 -> id 3
        dp->data->passObjects[1] = 9;                    // no such slot: objId -99, no KFREAD
    }
};

}  // namespace

TEST_CASE(distortion_class_basics)
{
    auto c2 = makeDistortion(true);
    auto c3 = makeDistortion(false);
    CHECK(c2->key() == String(u"SfOprDistortion2D"));
    CHECK(c3->key() == String(u"SfOprDistortion3D"));
    CHECK(c2->displayName() == String(u"Machining Distortion"));
    auto p = c2->makeProps(1);
    auto* dp = dynamic_cast<DistortionProps*>(p.get());
    REQUIRE(dp && dp->data);
    CHECK_EQ(dp->data->workpiece, 1);                    // 0x1413666a0 -> 0x141366710
    CHECK_EQ(dp->data->passCount, 0);
    CHECK_EQ(dp->data->firstPass, 0);
    // base adders after the tree (the double defaultPropTree gives an empty tree): TestGroup
    // with NumSteps 2, StepIncr 1 (0x1405f9eb0) and the other base nodes
    const PropNode* tg = base::testGroup(*p);
    REQUIRE(tg != nullptr);
    const PropNode* ns = nullptr;
    for (const PropNode& c : tg->children)
        if (c.name == String(u"NumSteps")) ns = &c;
    REQUIRE(ns != nullptr);
    CHECK_EQ(ns->value.i, 2);
}

TEST_CASE(distortion_writer_dispatch)
{
    DistFixture f;
    int sim = 3;
    f.cls->writeMst(f.w, f.prj, f.db, &sim, f.op, 0, 0);
    CHECK_EQ(sim, 7);
    CHECK_EQ(f.ts.bytes(), block(4, 0, 2, false, false) + block(5, 1, 2, true, false) +
                               block(6, 2, 2, false, false) + block(7, 3, 2, false, false));

    DistFixture g;                                       // props of another class: nothing
    g.op->props = std::make_shared<Props>();
    sim = 3;
    g.cls->writeMst(g.w, g.prj, g.db, &sim, g.op, 0, 0);
    CHECK_EQ(sim, 3);
    CHECK(g.ts.bytes().empty());
}

TEST_CASE(cycle_dim_without_engine_model_is_base)
{
    Fixture f;
    f.prj.subs.push_back(std::make_unique<SubProject>());
    Recorder rec;
    auto cyc = makeCycle();
    f.addOp(2, u"Forming (2)", &rec, std::make_shared<Props>());
    Operation& c = f.addOp(3, u"Cycle", cyc.get(), cyc->makeProps(1));
    p13test::members[3] = {2};
    CHECK_EQ(cyc->dim(), 2);                             // no current: base (double: 2)
    cyc->current = &c;
    CHECK_EQ(cyc->dim(), 2);                             // member without engine model: base
}

TEST_CASE(doeoutput_dim_is_last_operation_dim)
{
    Fixture f;
    auto d = makeDoeOutput();
    Recorder rec;
    Operation& o = f.addOp(1, u"DOE", d.get(), std::make_shared<Props>());
    f.addOp(2, u"Forming", &rec, std::make_shared<Props>());
    f.addOp(5, u"Heat", &rec, std::make_shared<Props>());
    CHECK_EQ(d->dim(), 3);                               // no current
    d->current = &o;
    CHECK_EQ(d->dim(), 3);                               // no sub-project
    f.prj.subs.push_back(std::make_unique<SubProject>());
    f.prj.subs[0]->project = &f.prj;
    p13test::subOps = {};
    CHECK_EQ(d->dim(), 3);                               // no operation: id -1
    p13test::subOps = {5, 2};
    CHECK_EQ(d->dim(), 2);                               // last one (id 2)
    p13test::subOps = {2, 5};
    CHECK_EQ(d->dim(), 4);
}

TEST_CASE(distortion_passes)
{
    DistFixture f;
    int sim = 3;
    p13::writeDistortionPasses(*f.cls, f.w, f.prj, f.db, &sim, f.op, *f.dp, 0, 0);
    CHECK_EQ(sim, 7);                                    // n + 2 blocks
    std::string want = block(4, 0, 2, false, false) + block(5, 1, 2, true, false) +
                       block(6, 2, 2, false, false) + block(7, 3, 2, false, false);
    CHECK_EQ(f.ts.bytes(), want);
}

TEST_CASE(distortion_chain_head_has_no_first_dbread)
{
    DistFixture f;
    p13test::chainHeads[4] = true;
    int sim = 0;
    p13::writeDistortionPasses(*f.cls, f.w, f.prj, f.db, &sim, f.op, *f.dp, 0, 0);
    std::string want = block(1, 0, 2, false, true) + block(2, 1, 2, true, false) +
                       block(3, 2, 2, false, false) + block(4, 3, 2, false, false);
    CHECK_EQ(f.ts.bytes(), want);
}

TEST_CASE(distortion_first_pass_and_only_sim)
{
    DistFixture f;
    f.dp->data->firstPass = 2;                           // passes 0 and 1 skipped, no count
    int sim = 3;
    p13::writeDistortionPasses(*f.cls, f.w, f.prj, f.db, &sim, f.op, *f.dp, 0, 0);
    CHECK_EQ(sim, 5);
    CHECK_EQ(f.ts.bytes(), block(4, 2, 2, false, false) + block(5, 3, 2, false, false));

    DistFixture g;
    sim = 3;
    p13::writeDistortionPasses(*g.cls, g.w, g.prj, g.db, &sim, g.op, *g.dp, 0, 5);   // onlySim 5
    CHECK_EQ(sim, 5);                                    // counts 4 (skipped), writes 5, stops
    CHECK_EQ(g.ts.bytes(), block(5, 1, 2, true, false));

    DistFixture h;
    h.dp->data->passCount = 0;                           // n == 0: nothing
    sim = 3;
    p13::writeDistortionPasses(*h.cls, h.w, h.prj, h.db, &sim, h.op, *h.dp, 0, 0);
    CHECK_EQ(sim, 3);
    CHECK(h.ts.bytes().empty());
}

TEST_CASE(distortion_simnam)
{
    DistFixture f;
    CHECK(p13::distortionSimnam(*f.cls, 0, 1, false) == String(u"Machining DistortionPS01"));
    CHECK(p13::distortionSimnam(*f.cls, 0, 12, false) == String(u"Machining DistortionPS12"));
    CHECK(p13::distortionSimnam(*f.cls, 0, 100, true) == String(u"PS100"));
    CHECK(p13::distortionSimnam(*f.cls, 1, 3, true) == String(u"UNLOAD"));
    CHECK(p13::distortionSimnam(*f.cls, 2, 0, false) == String(u"Machining DistortionLOAD"));
    CHECK(p13::distortionSimnam(*f.cls, 5, 0, true).isNull());
}

int main()
{
    return testkit::run_all();
}
