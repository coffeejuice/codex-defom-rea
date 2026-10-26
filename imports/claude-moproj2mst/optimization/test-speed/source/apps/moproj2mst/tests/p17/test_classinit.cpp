// P17: class init vt+0x180 dimension flags (0x1405fd8a0) and their dim() (0x1405f5bf0).
#include <testkit/check.hpp>

#include "m2m/mst/writer.hpp"
#include "m2m/p17.hpp"

using namespace m2m::mst;

static int flagsOf(std::unique_ptr<OpClass> ob)
{
    classInit(*ob);
    return ob->dimFlags;
}

TEST_CASE(family_flags)
{
    CHECK_EQ(flagsOf(makeForming(false)), 0x1a);
    CHECK_EQ(flagsOf(makeForming(true)), 0x19);
    CHECK_EQ(flagsOf(makeHeatTransfer(false)), 0x1a);
    CHECK_EQ(flagsOf(makeHeatTransfer(true)), 0x19);
    CHECK_EQ(flagsOf(makeHeatTransferExpress(false)), 0x16);
    CHECK_EQ(flagsOf(makeHeatTransferExpress(true)), 0x15);
    CHECK_EQ(flagsOf(makeHTFurnace()), 0x1a);
    CHECK_EQ(flagsOf(makePre(false)), 0x5a);
    CHECK_EQ(flagsOf(makePre(true)), 0x59);
}

TEST_CASE(dims)
{
    auto f = makeForming(true);
    classInit(*f);
    CHECK_EQ(f->dim(), 2);
    auto g = makeForming(false);
    classInit(*g);
    CHECK_EQ(g->dim(), 3);
}

TEST_CASE(diestress_heattreat_report_flags)
{
    CHECK_EQ(flagsOf(makeDiestressBM(true)), 0x4000009d);
    CHECK_EQ(flagsOf(makeDiestressBM(false)), 0x4000009e);
    CHECK_EQ(flagsOf(makeDiestress()), 0xdf);
    CHECK_EQ(flagsOf(makeHeatTreat(true)), 0x29);
    CHECK_EQ(flagsOf(makeHeatTreat(false)), 0x2a);
    CHECK_EQ(flagsOf(makeReport()), 0x9b);
}

int main() { return testkit::run_all(); }
