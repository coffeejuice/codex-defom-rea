// P16: shared setters (include/m2m/setters.hpp), offsets per the decompiles (out_p16/p17slice.c).
#include <testkit/check.hpp>

#include "m2m/mst/writer.hpp"
#include "m2m/setters.hpp"

using namespace m2m;

TEST_CASE(state_var_setters)
{
    StateVars s;
    set::urzFlag(s, 1);
    set::urz2(s, 2.5);
    set::drzFlag(s, 1);
    set::drz0(s, -1.0);
    set::grain(s, 30.0);
    set::lmtstrFlag(s, 1);
    set::damage(s, 0.5);
    set::drz2(s, 7.0);
    CHECK(s.damage.v == 0.5 && s.damage.set == 0);
    CHECK(s.drz[2] == 7.0 && s.drz[1] == 0.0);
    CHECK_EQ(s.urzSet, 1);
    CHECK(s.urz[2] == 2.5 && s.urz[0] == 0.0);
    CHECK_EQ(s.drzSet, 1);
    CHECK(s.drz[0] == -1.0);
    CHECK(s.grain.v == 30.0 && s.grain.set == 0);
    CHECK_EQ(s.lmtstr.set, 1);
    ContactBlock c;
    set::genctcValue(c, 0.25);
    CHECK(c.genVal == 0.25);
}

TEST_CASE(class_key_and_registry)
{
    Project p;
    Operation op;
    set::lubricantId(op, 4);
    CHECK_EQ(op.lubricantId, 4);
    mst::OpClass* fake = reinterpret_cast<mst::OpClass*>(&p);   // only compared, never used
    op.opClass = fake;
    set::classKey(op, qt::L("SfOprForming"));
    CHECK(op.classKey == qt::L("SfOprForming"));
    CHECK(op.opClass == nullptr);
    op.opClass = fake;
    set::classKey(op, qt::L("SfOprForming"));                   // unchanged: cache kept
    CHECK(op.opClass == fake);
    CHECK(!set::registryContains(p, qt::L("SfOprHeatTreat")));
    p.registry[qt::L("SfOprHeatTreat")] = mst::makeReport();
    CHECK(set::registryContains(p, qt::L("SfOprHeatTreat")));
}

int main()
{
    return testkit::run_all();
}
