// P06: every data/ header compiles on its own and together.
#include <testkit/check.hpp>

#include "m2m/data/boolean.hpp"
#include "m2m/data/cogging.hpp"
#include "m2m/data/cycle.hpp"
#include "m2m/data/diestressbm.hpp"
#include "m2m/data/express.hpp"
#include "m2m/datafiles.hpp"

TEST_CASE(data_defaults)
{
    m2m::data::CogModel m;
    CHECK_EQ(m.temperatureType, 1);
    CHECK_EQ(m.numStepsDsmax, 4);
    CHECK_EQ(m.cog.prbid, std::string("COGGING"));
    m2m::data::BooleanData b;
    CHECK_EQ(b.workpiece, 1);
    m2m::data::CycleData c;
    CHECK_EQ(c.count, 5);
    m2m::data::ExpressData e;
    CHECK_EQ(int(e.loaded), 0);
    m2m::data::DiestressBMData d;
    CHECK_EQ(d.objTotalNum, 0);
}

int main()
{
    return testkit::run_all();
}
