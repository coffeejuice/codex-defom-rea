#include "../support/test_paths.hpp"
// Public WRITE_MDT boundary on a disposable, parsed two-reheat Cogging project.
// This pins the candidate refusal and deferred file application, not original OOB output.
// ctest-labels: m2m-s
#include <testkit/check.hpp>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>

#include "m2m/convert.hpp"
#include "m2m/data/cogging.hpp"
#include "m2m/datafiles.hpp"
#include "../../src/datafiles/readers.hpp"

namespace fs = std::filesystem;
using namespace m2m;

namespace {

std::string readAll(const fs::path& path)
{
    std::ifstream in(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}

void writeAll(const fs::path& path, const std::string& bytes)
{
    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    REQUIRE(out.is_open());
    out.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
    REQUIRE(out.good());
}

std::size_t lineEnd(const std::string& text, std::size_t at)
{
    const auto end = text.find("\r\n", at);
    if (end == std::string::npos) throw std::runtime_error("expected CRLF-terminated Cogging MDT line");
    return end;
}

void setLineInteger(std::string& text, const std::string& key, int value, std::size_t from = 0)
{
    const auto keyAt = text.find(key, from);
    if (keyAt == std::string::npos) throw std::runtime_error("missing Cogging MDT key: " + key);
    const auto end = lineEnd(text, keyAt);
    auto digit = keyAt + key.size();
    while (digit < end && (text[digit] == ' ' || text[digit] == '\t')) ++digit;
    auto last = digit;
    while (last < end && text[last] >= '0' && text[last] <= '9') ++last;
    if (last == digit) throw std::runtime_error("missing integer value for Cogging MDT key: " + key);
    text.replace(digit, last - digit, std::to_string(value));
}

std::string replaceAll(std::string text, const std::string& from, const std::string& to)
{
    std::size_t at = 0;
    while ((at = text.find(from, at)) != std::string::npos) {
        text.replace(at, from.size(), to);
        at += to.size();
    }
    return text;
}

void addSecondSerializedReheat(std::string& text)
{
    const auto simKey = text.find("SIMLST");
    if (simKey == std::string::npos) throw std::runtime_error("missing SIMLST");
    const auto simEnd = lineEnd(text, simKey);
    const auto valueAt = text.find_first_of("0123456789", simKey + 6);
    if (valueAt == std::string::npos || valueAt >= simEnd)
        throw std::runtime_error("missing SIMLST count");
    const int simCount = std::stoi(text.substr(valueAt, simEnd - valueAt));
    if (simCount <= 0) throw std::runtime_error("expected nonempty one-reheat SIMLST control");

    const std::size_t rowsAt = simEnd + 2;
    std::size_t rowsEnd = rowsAt;
    for (int i = 0; i < simCount; ++i) rowsEnd = lineEnd(text, rowsEnd) + 2;
    const std::string simRows = text.substr(rowsAt, rowsEnd - rowsAt);
    const std::string secondSimRows = replaceAll(simRows, "RH01", "RH02");
    setLineInteger(text, "SIMLST", simCount * 2);
    // Recompute after changing the count line, which can shift the following offsets.
    const auto newSimKey = text.find("SIMLST");
    const auto newSimEnd = lineEnd(text, newSimKey);
    std::size_t newRowsEnd = newSimEnd + 2;
    for (int i = 0; i < simCount; ++i) newRowsEnd = lineEnd(text, newRowsEnd) + 2;
    text.insert(newRowsEnd, secondSimRows);

    const auto noReheatsAt = text.find("NoReheats", newRowsEnd + secondSimRows.size());
    if (noReheatsAt == std::string::npos) throw std::runtime_error("missing NoReheats");
    setLineInteger(text, "NoReheats", 2, noReheatsAt);

    const auto reheatAt = text.find("* Reheat 1\r\n", noReheatsAt);
    const auto processAt = text.find("* Process Data\r\n", reheatAt);
    if (reheatAt == std::string::npos || processAt == std::string::npos || processAt < reheatAt)
        throw std::runtime_error("could not locate the complete serialized REHEAT block");
    const auto blockStart = text.rfind("*\r\n", reheatAt);
    if (blockStart == std::string::npos || blockStart < noReheatsAt)
        throw std::runtime_error("could not locate REHEAT block banner");
    const auto processStart = text.rfind("*\r\n", processAt);
    if (processStart == std::string::npos || processStart < reheatAt)
        throw std::runtime_error("could not locate Process Data banner");
    std::string secondBlock = text.substr(blockStart, processStart - blockStart);
    secondBlock = replaceAll(std::move(secondBlock), "Reheat 1", "Reheat 2");
    text.insert(processStart, secondBlock);
}

std::map<std::string, std::string> inventory(const fs::path& root)
{
    std::map<std::string, std::string> files;
    for (const auto& entry : fs::recursive_directory_iterator(root)) {
        if (!entry.is_regular_file()) continue;
        files.emplace(entry.path().lexically_relative(root).generic_string(), readAll(entry.path()));
    }
    return files;
}

}  // namespace

TEST_CASE(public_write_mdt_two_reheat_guard_does_not_apply_mdt_or_mst)
{
    const fs::path corpus = m2m_test::configuredPath("M2M_CORPUS");
    const fs::path source = corpus / "42_vdie";
    const fs::path root = m2m_test::scratchOutput(m2m_test::scratchRoot() /
        ("cogging-two-reheat-" + std::to_string(GetCurrentProcessId()) + "-" +
         std::to_string(GetTickCount64())));
    REQUIRE(!fs::exists(root));
    fs::copy(source, root, fs::copy_options::recursive);

    const fs::path project = root / "vdie.moproj";
    const fs::path cogging = root / "Operations" / "Task00004" / "Cogging.MDT";
    const fs::path mst = root / "candidate-output.MST";
    REQUIRE(fs::exists(project));
    REQUIRE(fs::exists(cogging));

    std::string fixture = readAll(cogging);
    REQUIRE(!fixture.empty());
    addSecondSerializedReheat(fixture);
    writeAll(cogging, fixture);

    // Validate the parsed model, rather than relying only on the edited NoReheats token.
    data::CogModel parsed;
    Project::CogGlobals globals;
    datafiles::TextFile input(fixture);
    REQUIRE(datafiles::readMdtFile(input, parsed, globals) == datafiles::MdtResult::Ok);
    REQUIRE(parsed.cog.nReheats == 2);
    REQUIRE(parsed.cog.reheats.size() == 2);

    const auto before = inventory(root);
    REQUIRE(!fs::exists(mst));
    const Result result = m2m::convert(project.u16string(), mst.u16string(), M2M_FLAG_WRITE_MDT, 1252);
    CHECK_EQ(result.code, M2M_E_UNSUPPORTED);
    CHECK(result.message.find("NoReheats 2") != std::string::npos);
    CHECK(inventory(root) == before);
    CHECK(readAll(cogging) == fixture);
    CHECK(!fs::exists(mst));
}

int main()
{
    return testkit::run_all();
}
