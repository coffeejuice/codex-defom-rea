// Strict public WRITE_SAVE comparison against the retained B109 GUI Save.
// It does not run the original or claim native Close/GUI parity.
// ctest-labels: m2m-s session retained-oracle
#include "../support/test_paths.hpp"
#include <testkit/check.hpp>

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
const fs::path formingOutput{"Operations/Task00003/Forming.FWZ"};

std::string read(const fs::path& path)
{
    std::ifstream stream(path, std::ios::binary);
    REQUIRE(stream.is_open());
    return {std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};
}

class ShortScratch final {
public:
    explicit ShortScratch(const fs::path& input)
        : root_(base_ / ("b109_" + std::to_string(GetCurrentProcessId()) + "_" +
                         std::to_string(GetTickCount64())))
    {
        REQUIRE(root_.is_absolute() && root_.parent_path() == base_);
        REQUIRE(!fs::exists(root_));
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
            root_.filename().string().starts_with("b109_");
    }
    void cleanup() noexcept
    {
        // Recursive removal is limited to this newly-created unique leaf.
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

TEST_CASE(public_session_b109_forming_fwz_matches_retained_raw_gui_save)
{
    const auto oracles = m2m_test::configuredPath("M2M_ORACLE_B");
    const auto before = oracles / "109_B_FE7_before";
    const auto after = oracles / "109_B_FE7";
    const auto beforeBytes = read(before / formingOutput);
    const auto expectedBytes = read(after / formingOutput);
    REQUIRE(beforeBytes.size() == 1071);
    REQUIRE(expectedBytes.size() == 1098);
    REQUIRE(beforeBytes != expectedBytes);
    REQUIRE(expectedBytes.find(":\\") == std::string::npos);
    REQUIRE(expectedBytes.find(":/") == std::string::npos);

    ShortScratch scratch(before);
    {
        // Restore the caller directory before removing the disposable Project.
        qt::CurrentDirScope restoreDirectory{qt::currentPath()};
        const auto result = convert((scratch.root() / "Gear_blank.moproj").u16string(), u"",
                                    M2M_FLAG_WRITE_SAVE, 1252);
        if (result.code != M2M_OK)
            std::printf("Public retained B109 Save refused (%d): %s\n", result.code, result.message.c_str());
        REQUIRE(result.code == M2M_OK);
        const auto actualBytes = read(scratch.root() / formingOutput);
        if (actualBytes != expectedBytes)
            std::printf("Strict B109 raw FWZ mismatch: %s\n", formingOutput.generic_string().c_str());
        CHECK(actualBytes == expectedBytes); // No relocation, line-ending or numeric normalization.
        for (const auto& entry : fs::recursive_directory_iterator(scratch.root()))
            CHECK(entry.path().u16string().size() < 100);
    }
    scratch.remove();
}

int main() { return testkit::run_all(); }
