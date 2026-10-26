// Packet p8-08: oracle replay of the DEFINT cases (tests/commands/DEFINT) through p8_08_oracle_util.hpp.
// Skipped cases (DBREAD, missing cache entry) are listed with the reason; they run at integration.
// ctest-labels: p8-08 oracle
#include "p8_08_oracle_util.hpp"
#include <testkit/check.hpp>

TEST_CASE(defint_oracle_cases)
{
    const char* labels[] = {
        "DEFINT obj1 2D (DEF_INT)",
        "DEFINT obj1 3D (DEF_REM3_GUI)",
        "DEFINT obj1 3D (DEF_REM3_GUI) with 64bit.DAT",
        "DEFINT unknown object",
        "DEFINT without DBREAD (skip)",
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
