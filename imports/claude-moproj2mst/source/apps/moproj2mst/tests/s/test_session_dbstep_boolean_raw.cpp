// Strict public Open/WRITE_SAVE and adapter teardown against a historical Save.
// Does not execute the original or claim accepted native Close/modal parity.
// ctest-labels: m2m-s session retained-oracle
#include "../support/test_paths.hpp"
#include <testkit/check.hpp>

#include <array>
#include <cstdio>
#include <exception>
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
const std::array<fs::path, 5> outputs{
    "COG_P1_B1.MST", "Objects/Object00002.KEY", "Operations/Task00002/Boolean.BWZ",
    "Operations/Task00002/ObjProperties00004.KEY", "Operations/Task00002/SimCtrl.KEY"};

std::string read(const fs::path& path)
{
    std::ifstream stream(path, std::ios::binary);
    REQUIRE(stream.is_open());
    return {std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};
}

struct ActualOutput {
    bool regular = false, opened = false, complete = false;
    std::string bytes, error;
};

ActualOutput collectActual(const fs::path& path)
{
    ActualOutput output;
    try {
        std::error_code statusError;
        const auto status = fs::status(path, statusError);
        if (statusError && statusError != std::errc::no_such_file_or_directory) {
            output.error = "status: " + statusError.message();
            return output;
        }
        output.regular = fs::is_regular_file(status);
        if (!output.regular) return output;
        std::ifstream stream(path, std::ios::binary);
        output.opened = stream.is_open();
        if (!output.opened) {
            output.error = "Could not open regular output file";
            return output;
        }
        std::array<char, 4096> buffer;
        for (;;) {
            stream.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
            const auto count = stream.gcount();
            if (count > 0) output.bytes.append(buffer.data(), static_cast<std::size_t>(count));
            if (stream.bad()) {
                output.error = "Read failed with badbit; bytes are partial";
                break;
            }
            if (stream.eof()) {
                output.complete = true;
                break;
            }
            if (stream.fail()) {
                output.error = "Read failed before EOF; bytes are partial";
                break;
            }
        }
    } catch (const std::exception& error) {
        output.error = "Output collection exception: " + std::string(error.what());
    }
    return output; // No assertion before all available outputs can be retained.
}

void writeDiagnostic(const fs::path& path, const std::string& bytes)
{
    fs::create_directories(path.parent_path());
    std::ofstream stream(path, std::ios::binary | std::ios::trunc);
    REQUIRE(stream.is_open());
    stream.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
    stream.close();
    REQUIRE(!stream.fail());
}

class ShortScratch final {
public:
    explicit ShortScratch(const fs::path& input)
        : root_(base_ / ("g105_" + std::to_string(GetCurrentProcessId()) + "_" +
                         std::to_string(GetTickCount64())))
    {
        REQUIRE(root_.is_absolute() && root_.parent_path() == base_);
        REQUIRE(!fs::exists(root_));
        // A denied short-path write is reported by the test; no fallback or
        // permission broadening is attempted.
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
            root_.filename().string().starts_with("g105_");
    }
    void cleanup() noexcept
    {
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

void retainDifference(const fs::path& work, const Result& result,
                      const std::array<std::string, 5>& expected,
                      const std::array<ActualOutput, 5>& actual)
{
    const auto dump = m2m_test::scratchOutput(m2m_test::scratchRoot() /
        ("b133_raw_" + std::to_string(GetCurrentProcessId()) + "_" + std::to_string(GetTickCount64())));
    REQUIRE(!fs::exists(dump));
    writeDiagnostic(dump / "result.txt", "code=" + std::to_string(result.code) +
        "\nproject=" + (work / "COG_P1_B1.moproj").generic_string() + "\nmessage=" + result.message + "\n");
    for (std::size_t index = 0; index < outputs.size(); ++index) {
        auto expectedPath = dump / outputs[index]; expectedPath += ".expected";
        writeDiagnostic(expectedPath, expected[index]);
        if (actual[index].opened) {
            auto actualPath = dump / outputs[index];
            actualPath += actual[index].complete ? ".actual" : ".partial";
            writeDiagnostic(actualPath, actual[index].bytes);
        }
        if (!actual[index].error.empty()) {
            auto errorPath = dump / outputs[index]; errorPath += ".error";
            writeDiagnostic(errorPath, actual[index].error + "\nbytes_read=" +
                std::to_string(actual[index].bytes.size()) + "\n");
        } else if (!actual[index].regular) {
            auto missingPath = dump / outputs[index]; missingPath += ".missing";
            writeDiagnostic(missingPath, "No regular output file\n");
        }
    }
    std::printf("Full raw retained-oracle comparison evidence: %s\n", dump.generic_string().c_str());
}
} // namespace

TEST_CASE(public_dbstep_boolean_save_matches_five_historical_raw_outputs)
{
    const auto oracles = m2m_test::configuredPath("M2M_ORACLE_B");
    const auto before = oracles / "133_B_DBSTEP_BOOL3_before";
    const auto after = oracles / "133_B_DBSTEP_BOOL3";
    REQUIRE(fs::file_size(before / "COG_P1_B1.DB") == 9216000u);
    std::array<std::string, 5> expected;
    const std::array<std::size_t, 5> expectedSizes{7196, 2411, 182, 169, 2111};
    for (std::size_t index = 0; index < outputs.size(); ++index) {
        expected[index] = read(after / outputs[index]);
        REQUIRE(expected[index].size() == expectedSizes[index]);
        REQUIRE(expected[index].find(":\\") == std::string::npos);
        REQUIRE(expected[index].find(":/") == std::string::npos);
    }
    // This prepared historical profile requires three creations and two changes.
    REQUIRE(read(before / outputs[0]) != expected[0]);
    for (std::size_t index = 1; index < 4; ++index) REQUIRE(!fs::exists(before / outputs[index]));
    REQUIRE(read(before / outputs[4]) != expected[4]);

    ShortScratch scratch(before);
    {
        qt::CurrentDirScope restoreDirectory{qt::currentPath()};
        const auto result = convert((scratch.root() / "COG_P1_B1.moproj").u16string(), u"",
                                    M2M_FLAG_WRITE_SAVE, 1252);
        // Public return includes candidate Session destruction, not native Close.
        std::array<ActualOutput, 5> actual;
        bool matches = true;
        for (std::size_t index = 0; index < outputs.size(); ++index) {
            actual[index] = collectActual(scratch.root() / outputs[index]);
            matches = matches && actual[index].complete && actual[index].error.empty() &&
                actual[index].bytes == expected[index];
        }
        if (result.code != M2M_OK || !matches)
            retainDifference(scratch.root(), result, expected, actual);
        REQUIRE(result.code == M2M_OK);
        for (std::size_t index = 0; index < outputs.size(); ++index) {
            CHECK(actual[index].regular && actual[index].complete && actual[index].error.empty());
            CHECK(actual[index].bytes == expected[index]); // Full raw bytes, without masks or normalization.
        }
        for (const auto& entry : fs::recursive_directory_iterator(scratch.root()))
            CHECK(entry.path().u16string().size() < 100);
    }
    scratch.remove();
}

int main() { return testkit::run_all(); }
