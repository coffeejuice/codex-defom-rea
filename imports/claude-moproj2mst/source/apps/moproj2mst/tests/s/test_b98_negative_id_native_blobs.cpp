#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include "../support/test_paths.hpp"
#include <windows.h>
#include <bcrypt.h>
#pragma comment(lib, "bcrypt.lib")
// Candidate-only B98 negative-ID Open with native operation blob writes. No Save or original process.
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
    if (!in) throw std::runtime_error("cannot read project bytes: " + path.string());
    return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}

void replaceDirectId(std::string& bytes, const std::string& name, const std::string& oldId,
                     const std::string& newId)
{
    const std::string nameTag = "<Name>" + name + "</Name>";
    const auto namePos = bytes.find(nameTag);
    REQUIRE(namePos != std::string::npos);
    const auto opEnd = bytes.find("</Operation>", namePos);
    REQUIRE(opEnd != std::string::npos);
    const auto idPos = bytes.find("<Id>" + oldId + "</Id>", namePos);
    REQUIRE(idPos != std::string::npos && idPos < opEnd);
    REQUIRE(bytes.find("<Id>" + oldId + "</Id>", idPos + 1) == std::string::npos ||
            bytes.find("<Id>" + oldId + "</Id>", idPos + 1) >= opEnd);
    const auto digit = idPos + 4;
    REQUIRE(newId == "-" + oldId);
    bytes.insert(digit, "-");
}

std::vector<std::string> deltas(const Manifest& before, const Manifest& after)
{
    std::vector<std::string> result;
    for (const auto& [path, value] : after) {
        const auto it = before.find(path);
        if (it == before.end()) result.push_back("added:" + std::string(path.begin(), path.end()));
        else if (!(it->second == value)) result.push_back("changed:" + std::string(path.begin(), path.end()));
    }
    for (const auto& [path, _] : before)
        if (!after.contains(path)) result.push_back("removed:" + std::string(path.begin(), path.end()));
    return result;
}

}

TEST_CASE(b98_negative_id_native_operation_blobs_match_fixture)
{
    const fs::path source = m2m_test::configuredPath("M2M_ORACLE_B") / "98_B_DS3F_before";
    const fs::path sourceProject = source / "Gear_blank.moproj";
    constexpr const char* sourceHash = "e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd";
    const std::map<std::wstring, std::string> expectedRootHashes{
        {L"ObjProperties00001.KEY", "021caff357de2d72e5043483a36c35ef5848233258ba55530c0fde9499a0b713"},
        {L"ObjProperties00002.KEY", "26e9d62de1e0faeb32262d6ef2897a50530452f3246089bdcedb2c5cf6ceac2e"},
        {L"ObjProperties00003.KEY", "4e19975afc35c73512b5a1906044799959d5cb7113a1e104039d5b026f2360e8"}};
    REQUIRE(fs::is_regular_file(sourceProject));
    CHECK(sha256(sourceProject) == sourceHash);

    const fs::path scratch = m2m_test::scratchOutput(m2m_test::scratchRoot() / "b98_negative_id_native_blobs");
    std::error_code ec;
    fs::remove_all(scratch, ec);
    if (ec) throw fs::filesystem_error("clear native-blob test scratch", scratch, ec);
    fs::create_directories(scratch);
    fs::copy(source, scratch, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
    const fs::path projectPath = scratch / "Gear_blank.moproj";

    const std::string original = readBytes(sourceProject);
    std::string variant = original;
    replaceDirectId(variant, "Die Stress", "1", "-1");
    replaceDirectId(variant, "Forming (2)", "2", "-2");
    std::string expected = original;
    const auto id1 = expected.find("<Name>Die Stress</Name>");
    REQUIRE(id1 != std::string::npos);
    expected.insert(expected.find("<Id>1</Id>", id1) + 4, "-");
    const auto id2 = expected.find("<Name>Forming (2)</Name>");
    REQUIRE(id2 != std::string::npos);
    expected.insert(expected.find("<Id>2</Id>", id2) + 4, "-");
    REQUIRE(variant == expected);
    { std::ofstream out(projectPath, std::ios::binary | std::ios::trunc); out.write(variant.data(), static_cast<std::streamsize>(variant.size())); }
    REQUIRE(readBytes(projectPath) == expected);
    const std::string variantHash = sha256(projectPath);
    const Manifest before = tree(scratch);
    std::map<std::wstring, std::string> fixtureProperties;
    for (const auto& [name, expectedHash] : expectedRootHashes) {
        const fs::path fixture = source / "Operations/Task00001" / fs::path(name);
        REQUIRE(fs::is_regular_file(fixture));
        const std::string bytes = readBytes(fixture);
        CHECK(sha256(fixture) == expectedHash);
        fixtureProperties.emplace(name, bytes);
    }

    Project project;
    Ctx context;
    context.nativeOperationBlobs = true;
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
    const auto delta = deltas(before, after);

    const std::vector<std::string> expectedDeltas{
        "added:ObjProperties00001.KEY", "added:ObjProperties00002.KEY", "added:ObjProperties00003.KEY"};
    std::printf("source_sha256=%s variant_sha256=%s source_bytes=%zu variant_bytes=%zu\n",
                sourceHash, variantHash.c_str(), original.size(), variant.size());
    std::printf("candidate_acceptedProject=%s candidate_openedOk=%s candidate_tree_delta_count=%zu\n",
                acceptedProject ? "true" : "false", openedOk ? "true" : "false", delta.size());
    std::printf("candidate_open_error=%s\n", openError.empty() ? "<none>" : openError.c_str());
    for (const auto& item : delta) std::printf("candidate_tree_delta=%s\n", item.c_str());
    bool generatedRootBytesMatch = true;
    for (const auto& [name, expectedHash] : expectedRootHashes) {
        const fs::path generated = scratch / fs::path(name);
        REQUIRE(fs::is_regular_file(generated));
        const std::string bytes = readBytes(generated);
        const bool equal = bytes == fixtureProperties.at(name);
        const std::string digest = sha256(generated);
        generatedRootBytesMatch = generatedRootBytesMatch && equal && digest == expectedHash;
        std::printf("candidate_root_objproperties path=%s bytes=%zu sha256=%s fixture_byte_equal=%s\n",
                    fs::path(name).generic_string().c_str(), bytes.size(), digest.c_str(),
                    equal ? "true" : "false");
    }

    CHECK(variant.size() == original.size() + 2);
    CHECK(acceptedProject);
    CHECK(openedOk);
    CHECK(delta == expectedDeltas);
    CHECK(generatedRootBytesMatch);
}

int main() { return testkit::run_all(); }
