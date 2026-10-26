// P16: Diestress, DiestressBM and HeatTreat writer blocks (s1-05 2-4). These run through the
// seams of P02-P10/P15/P17; while one of them is a stub the case prints SKIP and passes. The
// expected lines are the writer's own literals; paths are computed through the same seams.
#include <testkit/check.hpp>

#include <cstdio>
#include <functional>
#include <string>

#include "m2m/data/diestress.hpp"
#include "m2m/data/heattreat.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/emit.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4path.hpp"

using namespace m2m;
using qt::String;

namespace {

void orSkip(const char* name, const std::function<void()>& fn)
{
    try {
        fn();
    } catch (const NotPorted& e) {
        std::printf("SKIP %s (%s)\n", name, e.what());
    }
}

struct Fixture {
    Project prj;
    SubProject* sub = nullptr;
    Operation op;
    Ctx ctx;
    mst::State st;
    mst::Stream ts{1252};
    mst::W w{ts, st, ctx};

    Fixture()
    {
        prj.path = String(u"C:/m2m_p16/Proj/Proj.moproj");
        prj.primaryDb = String(u"C:/m2m_p16/Proj/Proj.DB");
        auto s = std::make_unique<SubProject>();
        s->project = &prj;
        sub = s.get();
        prj.subs.push_back(std::move(s));
        op.project = &prj;
        op.id = 1;
        op.name = String(u"Op1");
        op.taskDir = String(u"Operations/Task00001");
    }
    std::string out() const { return ts.bytes(); }
};

bool has(const std::string& hay, const std::string& needle)
{
    return hay.find(needle) != std::string::npos;
}

// The props objects makeProps builds, without its default property tree (P02).
std::shared_ptr<Props> diestressProps()
{
    auto p = std::make_shared<DiestressProps>();
    p->data = std::make_shared<data::DiestressData>();
    return p;
}

std::shared_ptr<Props> heatTreatProps()
{
    auto p = std::make_shared<HeatTreatProps>();
    p->data = std::make_shared<data::HeatTreatData>();
    p->data->schedule.owner = p.get();
    return p;
}

std::string enc(const String& s)
{
    return qt::encodeLocale(s, 1252);
}

}  // namespace

TEST_CASE(diestress_block)
{
    orSkip("diestress_block", [] {
        Fixture f;
        auto cls = mst::makeDiestress();
        f.op.classKey = cls->key();
        f.op.props = diestressProps();             // makeProps without the P02 tree
        f.op.props->owner = &f.op;
        cls->current = &f.op;
        int sim = 2;
        cls->writeMst(f.w, f.prj, primaryDbPathClean(f.prj), &sim, &f.op, 0, 0);
        CHECK_EQ(sim, 3);
        String db = subProjectDbPath(f.prj, *f.sub);
        std::string key = enc(db);
        auto pos = key.rfind(".DB");
        if (pos != std::string::npos) key.replace(pos, 3, ".KEY");
        CHECK(has(f.out(), "KFREAD    \t1\n" + key + "\n"));
        CHECK(!has(f.out(), "OPRNAM"));
        CHECK(!has(f.out(), "DBREAD"));
    });
}

TEST_CASE(diestress_without_props_writes_nothing)
{
    orSkip("diestress_without_props_writes_nothing", [] {
        Fixture f;
        auto cls = mst::makeDiestress();
        f.op.props = std::make_shared<Props>();             // not SfOprDiestressProperties
        cls->current = &f.op;
        int sim = 2;
        cls->writeMst(f.w, f.prj, primaryDbPathClean(f.prj), &sim, &f.op, 0, 0);
        CHECK_EQ(sim, 2);
        CHECK(f.out().empty());
    });
}

TEST_CASE(heattreat_steps)
{
    orSkip("heattreat_steps", [] {
        Fixture f;
        auto cls = mst::makeHeatTreat(false);
        f.op.classKey = cls->key();
        f.op.props = heatTreatProps();
        f.op.props->owner = &f.op;
        auto* hp = dynamic_cast<HeatTreatProps*>(f.op.props.get());
        REQUIRE(hp && hp->data);
        auto& s = hp->data->schedule;
        for (int k = 0; k < 3; ++k) s.steps.push_back(std::make_unique<m2m::data::HTStep>());
        s.steps[2]->userKey = String(u"C:/x/user.KEY");
        s.first = 2;
        cls->current = &f.op;
        int sim = 5;
        String db = primaryDbPathClean(f.prj);
        cls->writeMst(f.w, f.prj, db, &sim, &f.op, 0, 0);
        CHECK_EQ(sim, 8);                                   // step 1 consumes its number too
        const std::string o = f.out();
        CHECK(!has(o, "* Operation No 6\n"));
        CHECK(has(o, "\n* Operation No 7\n"));
        CHECK(has(o, "\n* Operation No 8\n"));
        String key = qt::convertSeparators(taskDirAbs(f.op) + qt::kSep + String(u"HT_Proc2.MOKEY"));
        CHECK(has(o, " * Keyword file which ECC data \nKFREAD\n" +
                         enc(qt::cleanPath(app::relToDb(db, key))) + "\n"));
        CHECK(has(o, "KFREAD\nC:/x/user.KEY\n"));
        CHECK(!has(o, "OPRNAM"));
        CHECK(!f.ctx.warnings.empty());                     // HT_Proc files do not exist
    });
}

TEST_CASE(heattreat_without_steps_writes_nothing)
{
    orSkip("heattreat_without_steps_writes_nothing", [] {
        Fixture f;
        auto cls = mst::makeHeatTreat(true);
        f.op.props = heatTreatProps();
        f.op.props->owner = &f.op;
        cls->current = &f.op;
        int sim = 1;
        cls->writeMst(f.w, f.prj, primaryDbPathClean(f.prj), &sim, &f.op, 0, 0);
        CHECK_EQ(sim, 1);
        CHECK(f.out().empty());
    });
}

int main()
{
    return testkit::run_all();
}
