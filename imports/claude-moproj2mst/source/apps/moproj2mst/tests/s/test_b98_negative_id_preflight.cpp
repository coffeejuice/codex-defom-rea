#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include "../support/test_paths.hpp"
#include <windows.h>
#include <bcrypt.h>
#pragma comment(lib, "bcrypt.lib")
// Candidate-only negative operation-ID preflight; no original process or Save.
// ctest-labels: m2m-s m2m-b98-open-only
#include <testkit/check.hpp>

#include <filesystem>
#include <fstream>
#include <iterator>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/qt4path.hpp"

namespace fs = std::filesystem;
using namespace m2m;
namespace {
struct Entry {
    bool directory = false;
    std::uintmax_t size = 0;
    std::string digest;
    bool operator==(const Entry&) const = default;
};
using Manifest = std::map<std::wstring, Entry>;

std::string sha256(const fs::path& path)
{
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("cannot read file: " + path.string());
    const std::vector<unsigned char> bytes((std::istreambuf_iterator<char>(in)), {});
    unsigned char hash[32]{};
    if (bytes.size() > static_cast<size_t>(ULONG_MAX) ||
        BCryptHash(BCRYPT_SHA256_ALG_HANDLE, nullptr, 0,
                   const_cast<PUCHAR>(bytes.data()), static_cast<ULONG>(bytes.size()), hash, 32) != 0)
        throw std::runtime_error("SHA-256 failed");
    static constexpr char hex[] = "0123456789abcdef";
    std::string out;
    for (unsigned char b : hash) { out.push_back(hex[b >> 4]); out.push_back(hex[b & 15]); }
    return out;
}

Manifest tree(const fs::path& root)
{
    Manifest result;
    for (const auto& item : fs::recursive_directory_iterator(root)) {
        Entry e;
        e.directory = item.is_directory();
        if (!e.directory) { e.size = item.file_size(); e.digest = sha256(item.path()); }
        result.emplace(item.path().lexically_relative(root).generic_wstring(), std::move(e));
    }
    return result;
}

std::string readBytes(const fs::path& path)
{
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("cannot read project bytes");
    return std::string((std::istreambuf_iterator<char>(in)), {});
}

void replaceDirectId(std::string& bytes, const std::string& name, const std::string& oldId,
                     const std::string& newId)
{
    const std::string nameTag = "<Name>" + name + "</Name>";
    const auto namePos = bytes.find(nameTag);
    REQUIRE(namePos != std::string::npos);
    const auto opStart = bytes.rfind("<Operation>", namePos);
    REQUIRE(opStart != std::string::npos);
    const auto opEnd = bytes.find("</Operation>", namePos);
    REQUIRE(opEnd != std::string::npos);
    const auto idPos = bytes.find("<Id>" + oldId + "</Id>", namePos);
    REQUIRE(idPos != std::string::npos && idPos < opEnd);
    REQUIRE(bytes.find("<Id>" + oldId + "</Id>", idPos + 1) == std::string::npos ||
            bytes.find("<Id>" + oldId + "</Id>", idPos + 1) >= opEnd);
    const auto digit = idPos + 4;
    REQUIRE(newId == "-" + oldId);
    bytes.insert(digit, newId.substr(0, 1));
    CHECK(bytes.compare(digit + 1, oldId.size(), oldId) == 0);
}
}

TEST_CASE(b98_negative_operation_ids_candidate_open_preflight)
{
    const fs::path source = m2m_test::configuredPath("M2M_ORACLE_B") / "98_B_DS3F_before";
    const fs::path sourceProject = source / "Gear_blank.moproj";
    constexpr const char* sourceHash = "e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd";
    REQUIRE(fs::is_regular_file(sourceProject));
    CHECK(sha256(sourceProject) == sourceHash);

    const fs::path scratch = m2m_test::scratchOutput(m2m_test::scratchRoot() / "b98_negative_id_preflight");
    std::error_code ec;
    fs::remove_all(scratch, ec);
    if (ec) throw fs::filesystem_error("clear negative-ID scratch", scratch, ec);
    fs::create_directories(scratch);
    fs::copy(source, scratch, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
    const fs::path projectPath = scratch / "Gear_blank.moproj";

    std::string original = readBytes(sourceProject);
    std::string variant = original;
    replaceDirectId(variant, "Die Stress", "1", "-1");
    replaceDirectId(variant, "Forming (2)", "2", "-2");
    std::string expected = original;
    const auto id1 = expected.find("<Name>Die Stress</Name>");
    const auto digit1 = expected.find("<Id>1</Id>", id1) + 4;
    expected.insert(digit1, "-");
    const auto id2 = expected.find("<Name>Forming (2)</Name>");
    const auto digit2 = expected.find("<Id>2</Id>", id2) + 4;
    expected.insert(digit2, "-");
    CHECK(variant == expected);
    { std::ofstream out(projectPath, std::ios::binary | std::ios::trunc); out.write(variant.data(), static_cast<std::streamsize>(variant.size())); }
    REQUIRE(readBytes(projectPath) == expected);
    const std::string variantHash = sha256(projectPath);
    const Manifest before = tree(scratch);

    Project project;
    Ctx context;
    const qt::String file{projectPath.u16string()};
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    initProject(project, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(project);
    bool acceptedProject = false;
    bool openedOk = false;
    std::string openError;
    try {
        acceptedProject = openProject(project, file, context);
        openedOk = project.openedOk;
    } catch (const std::exception& e) {
        openError = e.what();
    }
    const Manifest after = tree(scratch);
    std::vector<std::string> delta;
    for (const auto& [path, value] : after) {
        const auto it = before.find(path);
        if (it == before.end()) delta.push_back("added:" + std::string(path.begin(), path.end()));
        else if (!(it->second == value)) delta.push_back("changed:" + std::string(path.begin(), path.end()));
    }
    for (const auto& [path, _] : before)
        if (!after.contains(path)) delta.push_back("removed:" + std::string(path.begin(), path.end()));
    std::printf("source_sha256=%s variant_sha256=%s source_bytes=%zu variant_bytes=%zu\n",
                sourceHash, variantHash.c_str(), original.size(), variant.size());
    std::printf("candidate_acceptedProject=%s candidate_openedOk=%s tree_unchanged=%s\n",
                acceptedProject ? "true" : "false", openedOk ? "true" : "false",
                after == before ? "true" : "false");
    std::printf("candidate_open_error=%s\n", openError.empty() ? "<none>" : openError.c_str());
    for (const auto& item : delta) std::printf("tree_delta=%s\n", item.c_str());
    CHECK(variant.size() == original.size() + 2);
    CHECK(acceptedProject);
    CHECK(openedOk);
    CHECK(after == before);
}

int main() { return testkit::run_all(); }
