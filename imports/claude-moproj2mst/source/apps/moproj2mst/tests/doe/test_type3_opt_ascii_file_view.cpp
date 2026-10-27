// ctest-labels: m2m-doe
#include "../support/test_paths.hpp"

#include <filesystem>
#include <fstream>
#include <string>

#include <testkit/check.hpp>

#include "../../src/ops/type3_opt_ascii_file.hpp"
#include "../../src/ops/type3_optimizer_defopt_view.hpp"

using namespace m2m::mst::detail;
namespace fs = std::filesystem;

namespace {
fs::path uniqueFile()
{
    const auto root = m2m_test::scratchOutput(m2m_test::scratchRoot() /
        (std::string("type3-parser-view-") + std::to_string(GetCurrentProcessId()) + "-" +
         std::to_string(GetTickCount64())));
    fs::create_directories(root);
    return root / "sample.opt";
}
}

TEST_CASE(type3_admitted_ascii_parser_projects_values_through_defopt_getters)
{
    const auto path = uniqueFile();
    // Candidate parser profile: BOM-free ASCII, LF-only, final LF.
    {
        std::ofstream file(path, std::ios::binary | std::ios::trunc);
        REQUIRE(file.good());
        file << "not-a-number\n"
                "OPR_NO 17\n"
                "NUM_CON 1\n"
                "ignored 1021 33 2.5 44\n"
                "REGIONS 2\n"
                "comment#root/first.ext\n"
                "root\\second.part.opt\n"
                "ENDDATA\n";
        REQUIRE(file.good());
    }

    Type3OptimizerScalarState state;
    const auto parsed = parseType3OptAsciiFile(path, state);
    CHECK(parsed.outcome == Type3OptAsciiFileOutcome::EndData);

    const Type3OptimizerDefOptView view(state);
    CHECK(view.value48());
    CHECK(view.value38() == 17);
    CHECK(view.numConCount() == 1);
    CHECK(view.numConField1At(0) == 1021);
    CHECK(view.numConField2At(0) == 33);
    CHECK(view.numConField3At(0) == 2.5);
    CHECK(view.numConField4At(0) == 44);
    CHECK(view.numConAt(-1) == nullptr);
    CHECK(view.numConField1At(1) == -1);
    CHECK(view.regionCount() == 2);
    CHECK(view.regionNameAt(0) == m2m::qt::L("first"));
    CHECK(view.regionNameAt(1) == m2m::qt::L("second.part"));
    CHECK(view.regionNameAt(-1) == m2m::qt::L("Selected Object"));
}

int main() { return testkit::run_all(); }
