#include "../support/test_paths.hpp"

// Candidate-only B98 Diestress path regression; the historical GUI bytes are an anchor,
// not fresh-original acceptance evidence.
// ctest-labels: m2m-s mdt-l6
#include <testkit/check.hpp>

#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>

#include "m2m/convert.hpp"
#include "m2m/common.hpp"

namespace fs = std::filesystem;

namespace {

std::string readAll(const fs::path& path)
{
    std::ifstream in(path, std::ios::binary);
    REQUIRE(in.is_open());
    return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}

std::size_t countOf(const std::string& text, const std::string& value)
{
    std::size_t count = 0;
    for (std::size_t at = 0; (at = text.find(value, at)) != std::string::npos; at += value.size())
        ++count;
    return count;
}

}  // namespace

TEST_CASE(b98_diestress_kfread_uses_current_project_key_path)
{
    const fs::path historical = m2m_test::configuredPath("M2M_ORACLE_B") / "98_B_DS3F";
    REQUIRE(fs::is_directory(historical));
    const fs::path historicalMst = historical / "Gear_blank.MST";
    const std::string historicalBytes = readAll(historicalMst);
    const std::string historicalKey =
        "C:/Users/alext/Projects/deform-rea-work/gui_mo/oracle_b/98_B_DS3F/Gear_blank.KEY";
    REQUIRE(countOf(historicalBytes, historicalKey) == 1u);

    const std::string runId = std::to_string(GetCurrentProcessId()) + "-" +
                              std::to_string(GetTickCount64());
    const fs::path scratch = m2m_test::scratchRoot() / ("b98r-" + runId);
    REQUIRE(!fs::exists(scratch));
    fs::create_directories(scratch);
    fs::copy(historical, scratch, fs::copy_options::recursive);

    const fs::path project = scratch / "Gear_blank.moproj";
    const fs::path key = scratch / "Gear_blank.KEY";
    const fs::path candidateMst = scratch / "candidate.MST";
    REQUIRE(fs::exists(project) && fs::exists(key));

    const auto result = m2m::convert(project.u16string(), candidateMst.u16string(),
                                     M2M_FLAG_WRITE_SAVE | M2M_FLAG_STRICT, 1252);
    REQUIRE(result.code == M2M_OK);

    const std::string candidateBytes = readAll(candidateMst);
    const std::string candidateKey = key.generic_string();
    REQUIRE(countOf(candidateBytes, candidateKey) == 1u);
    REQUIRE(countOf(candidateBytes, "KFREAD    \t1") ==
            countOf(historicalBytes, "KFREAD    \t1"));
    const auto keyAt = candidateBytes.find(candidateKey);
    REQUIRE(keyAt != std::string::npos);
    const auto lineBegin = candidateBytes.rfind('\n', keyAt);
    const auto lineEnd = candidateBytes.find('\n', keyAt);
    const std::size_t begin = lineBegin == std::string::npos ? 0 : lineBegin + 1;
    REQUIRE(lineEnd != std::string::npos);
    const std::size_t contentEnd = lineEnd > begin && candidateBytes[lineEnd - 1] == '\r' ? lineEnd - 1 : lineEnd;
    CHECK_EQ(candidateBytes.substr(begin, contentEnd - begin), candidateKey);

    std::string pathQualified = candidateBytes;
    pathQualified.replace(keyAt, candidateKey.size(), historicalKey);
    CHECK_EQ(pathQualified, historicalBytes);
}

int main()
{
    return testkit::run_all();
}
