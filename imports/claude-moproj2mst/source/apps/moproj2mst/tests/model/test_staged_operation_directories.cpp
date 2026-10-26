// ctest-labels: m2m-model operation staged Open directory decisions
#include <testkit/check.hpp>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/qt4path.hpp"
#include "../support/test_paths.hpp"

namespace fs = std::filesystem;
using namespace m2m;

namespace {
std::string readFile(const fs::path& path)
{
    std::ifstream in(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}

void replaceId(std::string& bytes, const std::string& name, const std::string& oldId,
               const std::string& newId)
{
    const auto namePos = bytes.find("<Name>" + name + "</Name>");
    REQUIRE(namePos != std::string::npos);
    const auto end = bytes.find("</Operation>", namePos);
    const auto idPos = bytes.find("<Id>" + oldId + "</Id>", namePos);
    REQUIRE(end != std::string::npos && idPos != std::string::npos && idPos < end);
    bytes.replace(idPos + 4, oldId.size(), newId);
}

std::vector<fs::path> treeEntries(const fs::path& root)
{
    std::vector<fs::path> result;
    for (const auto& entry : fs::recursive_directory_iterator(root))
        result.push_back(entry.path().lexically_relative(root));
    std::sort(result.begin(), result.end());
    return result;
}

qt::String asQtPath(const fs::path& path) { return qt::String(path.u16string()); }
}

TEST_CASE(staged_open_operation_directory_decisions_use_effective_ids_and_survive_parse_failure)
{
    const fs::path source = m2m_test::configuredPath("M2M_ORACLE_B") / "98_B_DS3F_before" / "Gear_blank.moproj";
    REQUIRE(fs::is_regular_file(source));
    const fs::path scratch = m2m_test::scratchOutput(m2m_test::scratchRoot() / "cp218_staged_operation_directories");
    std::error_code ec;
    fs::remove_all(scratch, ec);
    REQUIRE(!ec);
    fs::create_directories(scratch);
    const fs::path projectPath = scratch / source.filename();

    std::string bytes = readFile(source);
    REQUIRE(!bytes.empty());
    // Two source operations provide a positive ID and a zero ID that parseOperation
    // replaces with its ordinal. Reuse the first operation to prove repeated decisions
    // retain multiplicity, then a negative-ID copy, followed by a parse failure.
    replaceId(bytes, "Die Stress", "1", "1");
    replaceId(bytes, "Forming (2)", "2", "0");
    const auto opBegin = bytes.find("<Operation>");
    const auto opEnd = bytes.find("</Operation>", opBegin);
    REQUIRE(opBegin != std::string::npos && opEnd != std::string::npos);
    const std::string positive = bytes.substr(opBegin, opEnd + std::string("</Operation>").size() - opBegin);
    const auto listEnd = bytes.find("</Operation_List>");
    REQUIRE(listEnd != std::string::npos);
    bytes.insert(listEnd, positive);
    std::string negative = positive;
    const auto positiveId = negative.find("<Id>1</Id>");
    REQUIRE(positiveId != std::string::npos);
    negative.replace(positiveId + 4, 1, "-7");
    const auto updatedListEnd = bytes.find("</Operation_List>");
    bytes.insert(updatedListEnd, negative);
    // The final malformed row makes Open return a parse failure after prior operations
    // have already contributed their staged metadata.
    bytes.insert(bytes.find("</Operation_List>"), "<Operation><Name>Malformed</Name></Operation>");
    { std::ofstream out(projectPath, std::ios::binary | std::ios::trunc); out.write(bytes.data(), static_cast<std::streamsize>(bytes.size())); }
    const auto before = treeEntries(scratch);

    Project project;
    Ctx context;
    const qt::String file = asQtPath(projectPath);
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    initProject(project, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(project);
    REQUIRE(openProject(project, file, context));
    CHECK(!project.openedOk);

    const qt::String base = projectDir(project);
    const qt::String expected[] = {
        qt::cleanPath(base + qt::L("/Operations")), qt::cleanPath(base + qt::L("/Objects")),
        qt::cleanPath(base + qt::L("/Materials")), qt::cleanPath(base + qt::L("/Equipments")),
        qt::cleanPath(base + qt::L("/Lubricants"))};
    REQUIRE(context.stagedOperationDirectoryDecisions.size() == 15);
    for (std::size_t i = 0; i < 5; ++i) {
        CHECK(context.stagedOperationDirectoryDecisions[i] == expected[i].u16());
        CHECK(context.stagedOperationDirectoryDecisions[5 + i] == expected[i].u16());
        CHECK(context.stagedOperationDirectoryDecisions[10 + i] == expected[i].u16());
    }
    // applyEffects consumes FileEffects only; these staged decisions do not enter it.
    engine::FileEffects effects;
    CHECK(effects.mkdirs.empty());
    CHECK(treeEntries(scratch) == before);
}

int main() { return testkit::run_all(); }
