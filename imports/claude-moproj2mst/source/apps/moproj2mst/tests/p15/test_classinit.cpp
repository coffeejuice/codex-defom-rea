// ctest-labels: m2m-p15
// FormingExpress(2D) class init vt+0x180 (0x141413000 / 0x1414126f0): dimension flags 0x16 / 0x15
// and their dim() (0x1405f5bf0). Links m2m_core.
#include <testkit/check.hpp>

#include "m2m/mst/writer.hpp"
#include "m2m/p17.hpp"

using namespace m2m::mst;

TEST_CASE(forming_express_flags)
{
    auto fe = makeFormingExpress(false);
    classInit(*fe);
    CHECK_EQ(fe->dimFlags, 0x16);
    CHECK_EQ(fe->dim(), 3);
    auto fe2 = makeFormingExpress(true);
    classInit(*fe2);
    CHECK_EQ(fe2->dimFlags, 0x15);
    CHECK_EQ(fe2->dim(), 2);
}

int main() { return testkit::run_all(); }
