// P11: SfOprCogData / CogModel accessors of the Cogging writer (s1-03 3.3).
#include <testkit/check.hpp>

#include "../../src/ops/cogging.hpp"

using namespace m2m;
using namespace m2m::ops::cog;

static data::CogModel model1(int autoBite, int posMethod, int movDir)
{
    data::CogModel m;
    m.cog.nReheats = 1;
    m.cog.reheats.resize(1);
    auto& R = m.cog.reheats[0];
    R.nPass = 2;
    R.passes.resize(2);
    for (auto& P : R.passes) {
        P.autoBite = autoBite;
        P.noBite = 3;
        P.diePosMethod = posMethod;
        P.dieMovDir = movDir;
        P.dieStrtPos = 0.1;
        P.dieStopPos = 0.2;
        P.bites.resize(autoBite ? 1 : 3);
        for (size_t i = 0; i < P.bites.size(); i++) P.bites[i].nomBite = 10.0 * static_cast<double>(i + 1);
    }
    m.cog.billet.length = 100.0;
    m.cog.billet.geoTyp = 1;
    m.cog.billet.width = 50.0;
    return m;
}

TEST_CASE(tables)
{
    CHECK_EQ(dieId(0), 2);
    CHECK_EQ(dieId(1), 3);
    CHECK_EQ(dieId(2), 8);
    CHECK_EQ(dieId(3), 9);
    CHECK_EQ(dieId(4), 0);
    CHECK_EQ(manipId(0), 4);
    CHECK_EQ(manipId(3), 7);
    double v[3];
    dieDir(0, v);
    CHECK(v[0] == 0.0 && v[1] == 0.0 && v[2] == -1.0);
    dieDir(3, v);
    CHECK(v[0] == 0.0 && v[1] == 1.0 && v[2] == 0.0);
    data::CogModel m;
    CHECK_EQ(dieCount(m), 2);
    m.cog.die.four = 1;
    CHECK_EQ(dieCount(m), 4);
}

TEST_CASE(sizes)
{
    auto m = model1(0, 0, -1);
    CHECK(cogSize(m.cog, 1, 0) == 100.0);
    CHECK(cogSize(m.cog, 1, 2) == 50.0);     // GeoTyp 1 -> Width
    m.cog.billet.geoTyp = 7;
    CHECK(cogSize(m.cog, 1, 2) == 50.0);     // default -> Width
    m.cog.billet.geoTyp = 0;
    m.cog.billet.dia = 33.0;
    CHECK(cogSize(m.cog, 1, 2) == 33.0);
    m.cog.die.length = 9.0;
    CHECK(cogSize(m.cog, 2, 0) == 9.0);
    CHECK(cogSize(m.cog, 2, 1) == 0.0);
}

TEST_CASE(bites)
{
    auto m = model1(0, 0, -1);
    CHECK_EQ(numReheats(m), 1);
    CHECK_EQ(numPasses(m, 1), 2);
    CHECK_EQ(numBites(m, 1, 2), 3);
    CHECK(bite(m.cog, 1, 1, 3)->nomBite == 30.0);
    auto a = model1(1, 0, -1);
    CHECK(bite(a.cog, 1, 1, 3) == bite(a.cog, 1, 1, 1));   // auto: always bite 1
    // L = ((1 - 0.1) - 0.2) * 100 = 70, nom 10 -> x = 7 -> n + 1 = 8
    CHECK_EQ(autoBites(a.cog, 1, 1), 8);
    a.cog.reheats[0].passes[0].bites[0].nomBite = 0.0;
    CHECK_EQ(autoBites(a.cog, 1, 1), 1);
    bool threw = false;
    try { (void)pass(m.cog, 2, 1); } catch (const Error& e) { threw = e.code == M2M_E_INPUT; }
    CHECK(threw);
}

TEST_CASE(translation)
{
    auto m = model1(0, 0, -1);
    // method 0: d * strt * L = -1 * 0.1 * 100 = -10, then + d * nom of previous bites
    CHECK(testkit::same_bits(translationX(m.cog, 1, 1, 1), -1.0 * 0.1 * 100.0 + 0.0));
    CHECK(testkit::same_bits(translationX(m.cog, 1, 1, 3), ((-1.0 * 0.1 * 100.0 + 0.0) + -10.0) + -20.0));
    auto a = model1(1, 1, 1);
    // method 1, auto, d == 1: strt + (b-1) * nom1
    CHECK(testkit::same_bits(translationX(a.cog, 1, 1, 2), (0.1 + 0.0) + 1.0 * 10.0));
}

TEST_CASE(adaptive_getter)
{
    data::CogModel m;
    m.useAdaptiveReheat = 1;
    m.checkingMethod = 2;
    m.tempRange[0] = 1.5;
    m.tempRange[1] = 2.5;
    m.adaptiveHeatingTime = 3.5;
    int c = 0;
    double a = 0, b = 0, t = 0;
    CHECK(adaptive(m, &c, &a, &b, &t));
    CHECK(c == 2 && a == 1.5 && b == 2.5 && t == 3.5);
}

int main() { return testkit::run_all(); }
