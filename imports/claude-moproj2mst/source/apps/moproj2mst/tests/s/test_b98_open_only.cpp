#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include "../support/test_paths.hpp"
#include <windows.h>
#include <bcrypt.h>
#pragma comment(lib, "bcrypt.lib")
// Candidate-only B98 Open boundary. No conversion driver, Save, or original process is invoked.
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
#include "m2m/mst/writer.hpp"
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
    if (!in) throw std::runtime_error("cannot read fixture file: " + path.string());
    const std::vector<unsigned char> bytes((std::istreambuf_iterator<char>(in)), {});
    unsigned char hash[32]{};
    if (bytes.size() > static_cast<size_t>(ULONG_MAX) ||
        BCryptHash(BCRYPT_SHA256_ALG_HANDLE, nullptr, 0,
                   const_cast<PUCHAR>(bytes.data()), static_cast<ULONG>(bytes.size()), hash, 32) != 0)
        throw std::runtime_error("fixture SHA-256 failed");
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
}

TEST_CASE(b98_open_acceptance_read_status_and_tree_delta)
{
    const fs::path source = m2m_test::configuredPath("M2M_ORACLE_B") / "98_B_DS3F_before";
    const fs::path sourceProject = source / "Gear_blank.moproj";
    constexpr const char* expectedProjectHash = "e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd";
    REQUIRE(fs::is_regular_file(sourceProject));
    CHECK(sha256(sourceProject) == expectedProjectHash);
    const Manifest sourceState = tree(source);
    std::size_t sourceFiles = 0;
    for (const auto& [_, entry] : sourceState) if (!entry.directory) ++sourceFiles;
    CHECK_EQ(sourceFiles, 27u);

    const fs::path scratch = m2m_test::scratchOutput(m2m_test::scratchRoot() / "b98_open_only");
    std::error_code ec;
    fs::remove_all(scratch, ec);
    if (ec) throw fs::filesystem_error("clear B98 Open scratch", scratch, ec);
    fs::create_directories(scratch);
    fs::copy(source, scratch, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
    const Manifest before = tree(scratch);
    CHECK(before == sourceState);

    const fs::path projectPath = scratch / "Gear_blank.moproj";
    Project project;
    Ctx context;
    const qt::String file{projectPath.u16string()};
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    initProject(project, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(project);
    const bool acceptedProject = openProject(project, file, context);
    const bool openedOk = project.openedOk;
    const Manifest after = tree(scratch);

    std::printf("fixture=Gear_blank.moproj sha256=%s files=%zu acceptedProject=%s openedOk=%s treeUnchanged=%s\n",
                expectedProjectHash, sourceFiles, acceptedProject ? "true" : "false",
                openedOk ? "true" : "false", after == before ? "true" : "false");
    CHECK(acceptedProject);
    CHECK(openedOk);
    CHECK(after == before);
}

int main() { return testkit::run_all(); }
