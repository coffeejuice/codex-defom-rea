// P14: MultiBlow writer 0x140602810 (s1-04 7.1, 7.2). Spec test: the writer's own lines, in order,
// for a 2D operation with 1 heat x 2 blows (dwell after blow 1, heating TI/HT/TO after blow 1,
// flip before blow 2). Lines of the shared emitters (P08/P09) are not checked here. The case
// reports SKIP while a seam it reaches is still a stub (NotPorted). No corpus project has MultiBlow,
// so the oracle check is pending (plan 4, spec-only type).
#include <cstdio>
#include <sstream>

#include <testkit/check.hpp>

#include "m2m/data/multiblow.hpp"
#include "m2m/mst/writer.hpp"

using namespace m2m;
using m2m::qt::L;

static std::vector<std::string> lines(const std::string& s)
{
    std::vector<std::string> out;
    std::istringstream in(s);
    for (std::string l; std::getline(in, l);) out.push_back(l);
    return out;
}

// Every expected line appears in order; a trailing '*' matches any rest of the line.
static bool subsequence(const std::vector<std::string>& got, const std::vector<std::string>& want,
                        std::string* missing)
{
    size_t k = 0;
    for (const std::string& g : got) {
        if (k == want.size()) break;
        const std::string& w = want[k];
        const bool pre = !w.empty() && w.back() == '*' && w.size() > 1 && w[w.size() - 2] == '\t';
        if (pre ? g.compare(0, w.size() - 1, w, 0, w.size() - 1) == 0 : g == w) ++k;
    }
    if (k < want.size()) *missing = want[k];
    return k == want.size();
}

TEST_CASE(writer_2d_heat_dwell_flip)
{
    Project prj;
    prj.unit = 1;
    auto sub = std::make_unique<SubProject>();
    sub->project = &prj;
    prj.subs.push_back(std::move(sub));
    auto op = std::make_unique<Operation>();
    op->project = &prj;
    op->id = 1;
    op->name = L("Multi Blow");
    op->classKey = L("SfOprMultiBlow2D");
    prj.registry[op->classKey] = mst::makeMultiBlow(true);
    mst::OpClass* cls = prj.registry[op->classKey].get();
    op->opClass = cls;
    auto mp = std::make_shared<MultiBlowProps>();
    data::initMultiBlowProps(*mp, 2, 0);
    data::MultiBlowData& d = *mp->data;
    data::mbSetNumHeats(d, 1);
    data::mbSetNumBlows(d, 0, 2);
    qt::Variant zero;
    zero.kind = qt::Variant::Double;
    qt::Variant yes;
    yes.kind = qt::Variant::Text;
    yes.s = L("true");
    data::mbSetBlow(d, L("BlowDwellTime"), 0, 1, zero);   // no dwell after blow 2
    data::mbSetBlow(d, L("BlowReheat"), 0, 0, yes);       // heating after blow 1
    data::mbSetBlow(d, L("BlowFlip"), 0, 0, yes);         // flip before blow 2
    d.heating = 1;
    d.workpieceRotation = 1;
    op->props = mp;
    mp->owner = op.get();
    Operation* opp = op.get();
    prj.operations.push_back(std::move(op));

    mst::Stream ts(1252);
    mst::State st;
    Ctx ctx;
    mst::W w{ts, st, ctx};
    int sim = 0;
    try {
        cls->writeMst(w, prj, L("PROJECT.DB"), &sim, opp, 0, 0);
    } catch (const NotPorted& e) {
        std::printf("  SKIP (%s)\n", e.what());
        return;
    }
    CHECK_EQ(sim, 6);   // BL1, DW1, TI, HT, TO, BL2
    const std::vector<std::string> want = {
        // BL1 (sim 1)
        "PDIE \t2", "* Generate the contacts", "INICTC \t1", "GENCTC \t0.0001",
        "* TRANS will not be redefined in Blow operation.", "* Definition in SimCtrl.KEY will be used.",
        "* Define next simulation number - if minimum die distance criteria met, jump to next operation",
        "NEXTSIMNO\t3", "2\t2\t*",
        // DW1 (sim 2)
        "* Positioning", "*    Move Top die away", "OBJPOS \t2\t1\t0\t100", "* Generate the contacts",
        "INICTC \t1", "GENCTC \t0.0001", "IHTCOF \t1\t2\t0\t1", "IHTCOF \t1\t3\t0\t1",
        "MOVCTL \t2\t1\t0\t-90\t0", "MOVCTL \t3\t1\t0\t90\t0", "DSMAX \t0", "DTMAX \t0.1", "NSTEP \t10",
        "* Turn off Deformation in Dwell", "TRANS \t1\t0\t-1\t-1\t-1\t-1",
        // TI (sim 3)
        "PDIE \t1", "OTPRNG\t1\t0\t0\t0\t1", "OBJTYP\t2\t0", "OBJTYP\t3\t0", "DSMAX \t0", "DTMAX \t1",
        "NSTEP \t10", "ENVTMP\t0\t20", "* Generate the contacts", "INICTC \t1", "GENCTC \t0.0001",
        "* Turn off Deformation in Heating", "TRANS \t1\t0\t-1\t-1\t-1\t-1",
        // HT (sim 4)
        "PDIE \t1", "OTPRNG\t1\t0\t0\t0\t1", "OBJTYP\t2\t0", "OBJTYP\t3\t0", "DSMAX \t0", "DTMAX \t20",
        "NSTEP \t180", "ENVTMP\t0\t0", "* Turn off Deformation in Heating",
        // TO (sim 5)
        "PDIE \t1", "DTMAX \t1", "NSTEP \t10", "ENVTMP\t0\t20", "* Turn off Deformation in Heating",
        // BL2 (sim 6)
        "PDIE \t2", "* Flip",
        "*    Flip the workpiece with respect to a line perpendicular to the movement direction",
        "OBJPOS \t1\t4\t0\t0\t-1\t0", "* Positioning", "*    Put the flipped Workpiece on the Bottom die",
        "OBJPOS \t1\t2\t0.0001\t3\t0\t-1", "*    Put Top die on Workpiece", "OBJPOS \t2\t2\t0.0001\t1\t0\t-1",
        "* Generate the contacts", "NEXTSIMNO\t3", "7\t7\t*"};
    std::string missing;
    const bool ok = subsequence(lines(ts.bytes()), want, &missing);
    if (!ok) std::printf("  first missing line: [%s]\n--- output ---\n%s", missing.c_str(), ts.bytes().c_str());
    CHECK(ok);
}

int main()
{
    return testkit::run_all();
}
