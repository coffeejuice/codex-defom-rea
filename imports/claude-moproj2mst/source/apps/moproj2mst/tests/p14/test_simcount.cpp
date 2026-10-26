// P14: +0x298 sim lists and nextSim (s1-04 7.4, s1-05 5.6). Cases that reach another packet's
// seam stub report SKIP until that packet merges (NotPorted).
#include <cstdio>

#include <testkit/check.hpp>

#include "m2m/data/multiblow.hpp"
#include "m2m/mst/simcount.hpp"

using namespace m2m;
using m2m::qt::L;

#define SKIP_IF_NOT_PORTED(stmt)                                                                \
    try {                                                                                       \
        stmt;                                                                                   \
    } catch (const m2m::NotPorted& e) {                                                         \
        std::printf("  SKIP (%s)\n", e.what());                                                 \
        return;                                                                                 \
    }

namespace {
struct TestClass : mst::OpClass {
    TestClass() : OpClass(L("SfOprTest")) {}
};
}  // namespace

TEST_CASE(base_one_entry_per_operation)
{
    Project p;
    Operation op;
    op.project = &p;
    TestClass c;
    std::vector<mst::SimEntry> out;
    int counter = 4;
    c.simList(3, &counter, &op, out);
    REQUIRE(out.size() == 1);
    CHECK_EQ(counter, 5);
    CHECK_EQ(out[0].pos, 3);
    CHECK_EQ(out[0].sim, 5);
}

TEST_CASE(doe_position_override)
{
    Project p;
    p.doeSimPosSet = true;
    p.doeSimPos = 9;
    Operation op;
    op.project = &p;
    std::vector<mst::SimEntry> out;
    int counter = 0;
    mst::simListOne(2, &counter, &op, out);
    REQUIRE(out.size() == 1);
    CHECK_EQ(out[0].pos, 9);
    CHECK_EQ(out[0].sim, 1);
}

TEST_CASE(empty_lists_consume_nothing)
{
    Operation op;
    TestClass c;
    std::vector<mst::SimEntry> out;
    int counter = 7;
    mst::simListNone(c, 1, &counter, &op, out);
    CHECK_EQ(counter, 7);
    CHECK(out.empty());
}

TEST_CASE(multiblow_entries_follow_writer_conditions)
{
    // 1 heat x 2 blows; dwell on blow 1 only; heating with reheat on blow 2: TI, HT, TO
    Project p;
    Operation op;
    op.project = &p;
    auto mp = std::make_shared<MultiBlowProps>();
    data::initMultiBlowProps(*mp, 2, 0);
    data::MultiBlowData& d = *mp->data;
    data::mbSetNumHeats(d, 1);
    data::mbSetNumBlows(d, 0, 2);
    qt::Variant zero;
    zero.kind = qt::Variant::Double;
    zero.d = 0.0;
    qt::Variant yes;
    yes.kind = qt::Variant::Text;
    yes.s = L("true");
    data::mbSetBlow(d, L("BlowDwellTime"), 0, 1, zero);
    data::mbSetBlow(d, L("BlowReheat"), 0, 1, yes);
    d.heating = 1;
    op.props = mp;
    auto cls = mst::makeMultiBlow(true);
    std::vector<mst::SimEntry> out;
    int counter = 0;
    SKIP_IF_NOT_PORTED(cls->simList(1, &counter, &op, out));   // propsOf is P10's
    // BL1, DW1, BL2, TI, HT, TO
    CHECK_EQ(counter, 6);
    REQUIRE(out.size() == 6);
    for (int k = 0; k < 6; ++k) {
        CHECK_EQ(out[static_cast<size_t>(k)].sim, k + 1);
        CHECK_EQ(out[static_cast<size_t>(k)].pos, 1);
    }
}

int main()
{
    return testkit::run_all();
}
