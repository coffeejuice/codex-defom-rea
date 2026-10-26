// ctest-labels: m2m-model operation project directory decisions
#include <testkit/check.hpp>
#include "m2m/qt4path.hpp"
#include "../../src/model/operation_project_directories.hpp"
#include "../support/test_paths.hpp"
#include <algorithm>
#include <array>
#include <filesystem>
#include <optional>
#include <vector>

using namespace m2m;

namespace {
std::vector<std::filesystem::path> entries(const std::filesystem::path& directory)
{
    std::vector<std::filesystem::path> result;
    for (const auto& entry : std::filesystem::directory_iterator(directory))
        result.push_back(entry.path().filename());
    std::sort(result.begin(), result.end());
    return result;
}
}

TEST_CASE(operation_project_directory_decisions_keep_original_order_and_clean_paths)
{
    const auto scratch = m2m_test::scratchRoot() / "cp216-op-directory-collector";
    std::filesystem::create_directories(scratch);
    const auto before = entries(scratch);
    const auto scratch16 = scratch.u16string();
    const qt::String scratchBase(std::u16string(scratch16.begin(), scratch16.end()));
    std::vector<qt::String> decisions;

    detail::collectOperationProjectDirectoryDecisions(qt::L("C:/fixture/./project"), 1, 0, decisions);
    const qt::String expected[] = {
        qt::L("C:/fixture/project/Operations"), qt::L("C:/fixture/project/Objects"),
        qt::L("C:/fixture/project/Materials"), qt::L("C:/fixture/project/Equipments"),
        qt::L("C:/fixture/project/Lubricants")};
    REQUIRE(decisions.size() == 5);
    for (std::size_t i = 0; i < decisions.size(); ++i) CHECK(decisions[i] == expected[i]);
    detail::collectOperationProjectDirectoryDecisions(scratchBase, 1, 0, decisions);
    CHECK(entries(scratch) == before);
}

TEST_CASE(operation_project_directory_decisions_preserve_call_multiplicity_in_caller_vector)
{
    const qt::String base = qt::L("C:/fixture/project");
    std::vector<qt::String> decisions{qt::L("prior")};
    detail::collectOperationProjectDirectoryDecisions(base, 0, 4, decisions);
    CHECK_EQ(decisions.size(), 1u);

    const qt::String directory = qt::cleanPath(base);
    detail::collectOperationProjectDirectoryDecisions(directory, 7, std::nullopt, decisions);
    detail::collectOperationProjectDirectoryDecisions(directory, 7, std::nullopt, decisions);
    REQUIRE(decisions.size() == 11);
    for (std::size_t i = 0; i < 5; ++i)
        CHECK(decisions[1 + i] == decisions[6 + i]);
}

TEST_CASE(operation_project_directory_decisions_append_die_stress_only_for_active_type_four)
{
    const qt::String base = qt::L("C:/fixture/./project");
    const qt::String dieStress = qt::L("C:/fixture/project/DieStress");
    const std::array<std::optional<int>, 3> activeTypes = {
        std::optional<int>(4), std::optional<int>(0), std::nullopt};
    for (const std::optional<int> activeType : activeTypes) {
        std::vector<qt::String> decisions;
        detail::collectOperationProjectDirectoryDecisions(base, 5, activeType, decisions);
        REQUIRE(decisions.size() == (activeType == 4 ? 6u : 5u));
        if (activeType == 4) CHECK(decisions.back() == dieStress);
    }

    std::vector<qt::String> negative;
    detail::collectOperationProjectDirectoryDecisions(base, -2, 4, negative);
    CHECK(negative.empty());
}

int main() { return testkit::run_all(); }
