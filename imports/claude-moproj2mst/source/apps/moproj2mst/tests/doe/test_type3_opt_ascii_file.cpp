// ctest-labels: m2m-doe
#include "../support/test_paths.hpp"

#include <array>
#include <filesystem>
#include <fstream>
#include <string>
#include <utility>

#include <testkit/check.hpp>

#include "m2m/common.hpp"
#include "../../src/ops/type3_opt_ascii_file.hpp"

using namespace m2m;
using namespace m2m::mst::detail;
namespace fs = std::filesystem;

namespace {
fs::path uniqueFile(const char* name)
{
    static unsigned serial = 0;
    const auto root = m2m_test::scratchRoot() /
        (std::string("type3-") + name + "-" + std::to_string(GetCurrentProcessId()) + "-" +
         std::to_string(GetTickCount64()) + "-" + std::to_string(++serial));
    const auto leaf = m2m_test::scratchOutput(root);
    fs::create_directories(leaf);
    return leaf / "sample.opt";
}

void writeBytes(const fs::path& path, const std::string& bytes)
{
    std::ofstream file(path, std::ios::binary | std::ios::trunc);
    REQUIRE(file.good());
    file.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
    REQUIRE(file.good());
}
}

TEST_CASE(type3_ascii_file_preserves_open_header_scalar_region_and_enddata_order)
{
    const auto path = uniqueFile("ordered");
    writeBytes(path,
        "not-a-number\n"
        "OPR_NO 17\n"
        "NUM_CON 1\n"
        "ignored 1021 33 2.5 44\n"
        "REGIONS 2\n"
        "comment#root/first.ext\n"
        "root\\second.part.opt\n"
        "ENDDATA\n"
        "OPR_NO 99\n");

    Type3OptimizerScalarState state;
    CHECK(!state.value48);
    const auto result = parseType3OptAsciiFile(path, state);

    CHECK(result.outcome == Type3OptAsciiFileOutcome::EndData);
    CHECK(state.value48);
    CHECK_EQ(state.value38, 17);
    REQUIRE(state.numConRows.size() == 1u);
    CHECK_EQ(state.numConRows[0].value04, 1021);
    CHECK_EQ(state.numConRows[0].value08, 33);
    CHECK_BITS(state.numConRows[0].value18, 2.5);
    CHECK(state.numConRows[0].value20.isEmpty());
    REQUIRE(state.regionRows.size() == 2u);
    CHECK(state.regionRows[0].value08 == qt::L("first"));
    CHECK(state.regionRows[1].value08 == qt::L("second.part"));
}

TEST_CASE(type3_ascii_file_open_failure_precedes_optimizer_flag_mutation)
{
    auto path = uniqueFile("missing");
    fs::remove(path.parent_path());
    Type3OptimizerScalarState state;
    const auto result = parseType3OptAsciiFile(path, state);
    CHECK(result.outcome == Type3OptAsciiFileOutcome::OpenFailed);
    CHECK(!state.value48);
    CHECK(state.regionRows.empty());
}

TEST_CASE(type3_ascii_file_rejects_cr_codec_and_unterminated_profiles_after_open)
{
    const std::array<std::pair<const char*, std::string>, 3> cases{{
             std::pair{"crlf", std::string("header\r\nOPR_NO 7\r\n")},
             std::pair{"high-byte", std::string("header\nname-") + static_cast<char>(0xff) + "\n"},
             std::pair{"no-final-lf", std::string("header\nOPR_NO 7")}}};
    for (const auto& [name, bytes] : cases) {
        const auto path = uniqueFile(name);
        writeBytes(path, bytes);
        Type3OptimizerScalarState state;
        const auto result = parseType3OptAsciiFile(path, state);
        CHECK(result.outcome == Type3OptAsciiFileOutcome::UnsupportedFileProfile);
        CHECK(state.value48);
        CHECK_EQ(state.value38, 200);
    }
}

TEST_CASE(type3_ascii_file_early_regions_eof_keeps_rows_and_prior_scalar)
{
    const auto path = uniqueFile("early-eof");
    writeBytes(path, "header\nOPR_NO 23\nREGIONS 3\na/one.dat\nb/two.dat\n");
    Type3OptimizerScalarState state;
    const auto result = parseType3OptAsciiFile(path, state);
    CHECK(result.outcome == Type3OptAsciiFileOutcome::EndOfFile);
    CHECK(state.value48);
    CHECK_EQ(state.value38, 23);
    REQUIRE(state.regionRows.size() == 2u);
    CHECK(state.regionRows[0].value08 == qt::L("one"));
    CHECK(state.regionRows[1].value08 == qt::L("two"));
}

TEST_CASE(type3_ascii_file_empty_open_file_sets_flag_and_returns_natural_eof)
{
    const auto path = uniqueFile("empty");
    writeBytes(path, "");
    Type3OptimizerScalarState state;
    const auto result = parseType3OptAsciiFile(path, state);
    CHECK(result.outcome == Type3OptAsciiFileOutcome::EndOfFile);
    CHECK(state.value48);
    CHECK(state.regionRows.empty());
}

int main() { return testkit::run_all(); }
