// Strict public WRITE_SAVE regression against a retained historical GUI Save.
// It neither runs the original nor claims native Close or CP97 mesh admission.
// ctest-labels: m2m-s geometry session retained-oracle
#include "../support/test_paths.hpp"
#include <testkit/check.hpp>

#include <array>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <system_error>
#include <windows.h>

#include "m2m/convert.hpp"
#include "m2m/qt4path.hpp"

namespace fs = std::filesystem;
using namespace m2m;

namespace {
std::string read(const fs::path& path)
{
    std::ifstream stream(path, std::ios::binary);
    REQUIRE(stream.is_open());
    return {std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};
}

class ShortScratch final {
public:
    explicit ShortScratch(const fs::path& input)
        : root_(base_ / ("g103_" + std::to_string(GetCurrentProcessId()) + "_" +
                         std::to_string(GetTickCount64())))
    {
        REQUIRE(root_.is_absolute() && root_.parent_path() == base_);
        REQUIRE(!fs::exists(root_));
        // This test follows the requested short Project path. A denied write
        // is a test failure; there is no silent fallback or permission change.
        fs::create_directories(base_);
        REQUIRE(fs::create_directory(root_));
        owned_ = true;
        try {
            fs::copy(input, root_, fs::copy_options::recursive);
            for (const auto& entry : fs::recursive_directory_iterator(root_))
                REQUIRE(entry.path().u16string().size() < 100);
        } catch (...) {
            cleanup();
            throw;
        }
    }
    ~ShortScratch() { cleanup(); }
    ShortScratch(const ShortScratch&) = delete;
    const fs::path& root() const noexcept { return root_; }
    void remove()
    {
        REQUIRE(owned_ && bounded());
        fs::remove_all(root_);
        owned_ = false;
        CHECK(!fs::exists(root_));
    }
private:
    bool bounded() const
    {
        return root_.is_absolute() && root_.parent_path() == base_ &&
            root_.filename().string().starts_with("g103_");
    }
    void cleanup() noexcept
    {
        // Only this newly created numeric leaf may be removed recursively.
        if (owned_ && bounded()) {
            std::error_code ignored;
            fs::remove_all(root_, ignored);
            if (!ignored) owned_ = false;
        }
    }
    const fs::path base_{"C:/PROBLEM"};
    const fs::path root_;
    bool owned_ = false;
};
} // namespace

TEST_CASE(public_session_cogging_save_matches_retained_nonempty_geometry_outputs)
{
    const auto oracles = m2m_test::configuredPath("M2M_ORACLE_B");
    const auto before = oracles / "RS_42_vdie_before";
    const auto expected = oracles / "RS_42_vdie";
    const std::array<fs::path, 5> geometryFiles{
        "Operations/Task00004/Cogging.MDT", "Objects/Object00010.KEY",
        "Objects/Object00011.KEY", "Objects/Object00012.KEY", "Objects/Object00013.KEY"};
    std::array<std::string, 5> beforeBytes, expectedBytes;
    for (std::size_t index = 0; index < geometryFiles.size(); ++index) {
        beforeBytes[index] = read(before / geometryFiles[index]);
        expectedBytes[index] = read(expected / geometryFiles[index]);
        REQUIRE(!expectedBytes[index].empty());
    }
    // The historical Save changes billet extents and two manipulator files.
    // A no-op that merely preserves the input cannot satisfy these assertions.
    REQUIRE(beforeBytes[0] != expectedBytes[0]);
    REQUIRE(beforeBytes[3] != expectedBytes[3]);
    REQUIRE(beforeBytes[4] != expectedBytes[4]);
    REQUIRE(expectedBytes[0].find("Length      1.1125596E+003") != std::string::npos);
    REQUIRE(expectedBytes[0].find("Dia         4.0432001E+002") != std::string::npos);

    ShortScratch scratch(before);
    {
        // Restore the thread's directory before removing its disposable Project.
        qt::CurrentDirScope restoreDirectory{qt::currentPath()};
        const auto result = convert((scratch.root() / "vdie.moproj").u16string(), u"",
                                    M2M_FLAG_WRITE_SAVE, 1252);
        if (result.code != M2M_OK)
            std::printf("Public retained Cogging Save refused (%d): %s\n", result.code, result.message.c_str());
        REQUIRE(result.code == M2M_OK);
        CHECK(fs::exists(scratch.root() / "vdie.MST"));
        for (std::size_t index = 0; index < geometryFiles.size(); ++index) {
            const auto actual = read(scratch.root() / geometryFiles[index]);
            if (actual != expectedBytes[index])
                std::printf("Strict retained geometry mismatch: %s\n", geometryFiles[index].generic_string().c_str());
            CHECK(actual == expectedBytes[index]); // Raw bytes: no relocation, view or banner mask.
        }
        for (const auto& entry : fs::recursive_directory_iterator(scratch.root()))
            CHECK(entry.path().u16string().size() < 100);
    }
    scratch.remove();
}

int main() { return testkit::run_all(); }
