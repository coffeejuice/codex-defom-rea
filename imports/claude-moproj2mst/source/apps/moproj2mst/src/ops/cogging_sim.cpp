// Cogging model accessors that P14's simulation list 0x140759d50 reads (simcount.hpp simdata, s1-03 3.3).
#include "cogging.hpp"
#include "m2m/mst/simcount.hpp"

namespace m2m::mst::simdata {
using namespace m2m::ops::cog;

static const CogModel& model(const CoggingProps& p)
{
    if (!p.model) throw Error(M2M_E_INPUT, "cogging operation without process data");
    return *p.model;
}

int cogTempType(const CoggingProps& p) { return model(p).temperatureType; }
bool cogAdaptive(const CoggingProps& p)
{
    int c = 0;
    double a = 0.0, b = 0.0, t = 0.0;
    return adaptive(model(p), &c, &a, &b, &t);
}
bool cogHeatTransfer(const CoggingProps& p) { return model(p).heatTransfer != 0; }
bool cogRemainingDwell(const CoggingProps& p) { return model(p).remainingCycleTimeDwell != 0; }
double cogDwellTime(const CoggingProps& p) { return model(p).dwellTime; }
double cogDecompressionTime(const CoggingProps& p) { return model(p).decompressionTime; }
int cogReheats(const CoggingProps& p) { return numReheats(model(p)); }
int cogPasses(const CoggingProps& p, int r) { return numPasses(model(p), r); }
int cogNoBite(const CoggingProps& p, int r, int ps) { return numBites(model(p), r, ps); }
int cogAutoBite(const CoggingProps& p, int r, int ps) { return pass(model(p).cog, r, ps).autoBite; }
double cogPassHeatTmax(const CoggingProps& p, int r, int ps) { return pass(model(p).cog, r, ps).ht.tmax; }
double cogPassDwell(const CoggingProps& p, int r, int ps) { return pass(model(p).cog, r, ps).dwellTime; }
int cogAutoBites(const CoggingProps& p, int r, int ps) { return autoBites(model(p).cog, r, ps); }
bool cogHasBite(const CoggingProps& p, int r, int ps, int b) { return bite(model(p).cog, r, ps, b) != nullptr; }

}  // namespace m2m::mst::simdata
