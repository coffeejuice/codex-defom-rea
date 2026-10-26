// ctest-labels: m2m-doe
// Exact candidate serialization of retained mode-zero DOE sample tables.
#include "../support/test_paths.hpp"
#include <testkit/check.hpp>

#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

#include "m2m/doe_study.hpp"

using namespace m2m;
namespace {
std::string readFixture(const std::string& caseName, const char* fileName)
{
    const auto path = m2m_test::configuredPath("M2M_ORACLE_B") / caseName /
        "DoeStudies" / "DoeStudy1" / "Control" / fileName;
    std::ifstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("DOE sample fixture is unavailable: " + path.string());
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

void checkModeZeroFixture(const std::string& caseName, std::size_t expectedBytes)
{
    const std::string currentBytes = readFixture(caseName, "DEF_DOE.PAR");
    const std::string lhsBytes = readFixture(caseName, "Samples_LHS.par");
    REQUIRE(currentBytes.size() == expectedBytes);
    REQUIRE(lhsBytes.size() == expectedBytes);
    CHECK(currentBytes == lhsBytes);

    int currentMode = -1;
    int lhsMode = -1;
    doe::SampleTable currentRows;
    doe::SampleTable lhsRows;
    doe::parseSampleText(qt::String::latin1(currentBytes), currentMode, currentRows);
    doe::parseSampleText(qt::String::latin1(lhsBytes), lhsMode, lhsRows);
    REQUIRE(currentMode == 0);
    REQUIRE(lhsMode == 0);
    REQUIRE(currentRows.size() == 100u);
    REQUIRE(lhsRows.size() == 100u);

    engine::FileEffects effects;
    const String currentPath = qt::L("C:/candidate/") + qt::String::latin1(caseName) +
        qt::L("/Control/DEF_DOE.PAR");
    const String lhsPath = qt::L("C:/candidate/") + qt::String::latin1(caseName) +
        qt::L("/Control/Samples_LHS.par");
    REQUIRE(doe::writeSampleTable(currentMode, currentRows, currentPath, effects));
    REQUIRE(doe::writeSampleTable(lhsMode, lhsRows, lhsPath, effects));

    REQUIRE(effects.writes.size() == 2u);
    CHECK(effects.writes[0].path == currentPath);
    CHECK(effects.writes[0].bytes == currentBytes);
    CHECK(effects.writes[1].path == lhsPath);
    CHECK(effects.writes[1].bytes == lhsBytes);
    CHECK(effects.deletes.empty());
    CHECK(effects.mkdirs.empty());
    CHECK(effects.renames.empty());
}
} // namespace

TEST_CASE(doe_sample_fixture_serialization_matches_b121_mode_zero_current_and_lhs)
{
    checkModeZeroFixture("121_B_DOE_VAR", 1396u);
}

TEST_CASE(doe_sample_fixture_serialization_matches_b122_mode_zero_current_and_lhs)
{
    checkModeZeroFixture("122_B_DOE_GEO", 1389u);
}

int main() { return testkit::run_all(); }
