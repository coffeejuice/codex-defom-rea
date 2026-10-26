// Packet p8-08: oracle replay of the REMESH cases (tests/commands/REMESH) through p8_08_oracle_util.hpp.
// Skipped cases (DBREAD, missing cache entry) are listed with the reason; they run at integration.
// ctest-labels: p8-08 oracle
#include "p8_08_oracle_util.hpp"
#include <testkit/check.hpp>

TEST_CASE(remesh_oracle_cases)
{
    const char* labels[] = {
        "REMESH obj1 2D from DB (HAMMER_SI)",
        "REMESH obj1 2D from KEY only (interpolation skipped)",
        "REMESH obj1 3D from DB (Multiple_Operations_Example1)",
        "REMESH obj1 3D from DB (Multiple_Operations_Example1) with 64bit.DAT",
        "REMESH obj1 3D from KEY only (interpolation skipped)",
        "REMESH without object number",
        "REMESH unknown object",
    };
    for (const char* l : labels) {
        const p808oracle::CaseResult r = p808oracle::run_case(l);
        if (r.skipped) {
            std::printf("[skip] %s: %s\n", l, r.why.c_str());
            continue;
        }
        std::printf("[%s] %s: %d equal, %d differ, %d baseline gaps\n%s", r.res.pass ? "pass" : "FAIL", l,
                    r.res.equal, r.res.differ, r.res.gaps, r.res.report.c_str());
        CHECK(r.res.found);
        CHECK(r.res.pass);
    }
}

int main() { return testkit::run_all(); }
