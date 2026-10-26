// ctest-labels: m2m-doe
#include "../support/test_paths.hpp"
#include <testkit/check.hpp>

#include <array>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <utility>

#include "m2m/doe_study.hpp"

namespace fs = std::filesystem;
using namespace m2m;

namespace {
fs::path scratch()
{
    static unsigned next = 0;
    const auto root = m2m_test::scratchOutput(m2m_test::scratchRoot() /
        (std::string("doe-sample-sequence-") + std::to_string(GetCurrentProcessId()) +
         "-" + std::to_string(GetTickCount64()) + "-" + std::to_string(++next)));
    REQUIRE(!fs::exists(root));
    return root;
}

std::string bytes(const fs::path& path)
{
    std::ifstream in(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}

class SequenceIo final : public doe::StudyIo {
public:
    explicit SequenceIo(String failPath = {})
        : StudyIo(doe::IoMode::Native), failPath_(std::move(failPath)) {}

    bool ensureDirectory(const String&) override
    { throw std::runtime_error("sample output must not create directories"); }

    bool writeTextFile(const String& path, const std::string& data) override
    {
        if (!failPath_.isEmpty() && path == failPath_)
            return recordTextWrite(path, data, false, false, false);
        std::ofstream out(fs::path(path.u16()), std::ios::binary | std::ios::trunc);
        if (!out.is_open()) return recordTextWrite(path, data, false, false, false);
        out.write(data.data(), static_cast<std::streamsize>(data.size()));
        out.flush();
        const bool written = out.good();
        out.close();
        return recordTextWrite(path, data, true, written, !out.fail());
    }

private:
    String failPath_;
};

doe::Study populated()
{
    doe::Study study;
    study.name = qt::L("StudyA");
    study.samples.mode = 1;
    study.samples.tables[0] = {{10}};
    study.samples.tables[1] = {{20}};
    study.samples.tables[2] = {{30}};
    study.samples.invalid[0] = false;
    study.samples.invalid[1] = false;
    study.samples.invalid[2] = false;
    return study;
}
} // namespace

TEST_CASE(doe_sample_immediate_sequence_writes_current_then_modes_zero_one_two)
{
    const auto root = scratch();
    const auto control = root / "DoeStudies" / "StudyA" / "Control";
    fs::create_directories(control);
    auto study = populated();
    SequenceIo io;

    const auto result = doe::saveType2SampleSequenceImmediate(
        study, String((root / "DoeStudies").u16string()), io);
    const std::array<const char*, 4> names{
        "DEF_DOE.PAR", "Samples_LHS.par", "Samples_FF.par", "Samples_USR.par"};
    const std::array<int, 4> modes{1, 0, 1, 2};
    REQUIRE(io.textWrites().size() == 4);
    for (std::size_t i = 0; i < names.size(); ++i) {
        const String expectedPath((control / names[i]).u16string());
        CHECK(result.writes[i].mode == modes[i]);
        CHECK(result.writes[i].path == expectedPath);
        CHECK(result.writes[i].state == doe::ImmediateSampleWriteState::Opened);
        CHECK(io.textWrites()[i].path == expectedPath);
        CHECK(io.textWrites()[i].opened && io.textWrites()[i].writeSucceeded);
        CHECK(bytes(control / names[i]) == std::to_string(modes[i]) + "\r\n1\t" +
            std::to_string(modes[i] == 0 ? 10 : modes[i] == 1 ? 20 : 30) + "\t\r\n");
    }
}

TEST_CASE(doe_sample_immediate_sequence_skips_invalid_tables_and_continues_after_open_false)
{
    const auto root = scratch();
    const auto control = root / "DoeStudies" / "StudyA" / "Control";
    fs::create_directories(control);
    auto study = populated();
    study.samples.invalid[1] = true; // Current mode and mode 1 output skip independently.
    const String failedLhs((control / "Samples_LHS.par").u16string());
    SequenceIo io(failedLhs);

    const auto result = doe::saveType2SampleSequenceImmediate(
        study, String((root / "DoeStudies").u16string()), io);
    CHECK(result.writes[0].state == doe::ImmediateSampleWriteState::SkippedInvalid);
    CHECK(result.writes[1].state == doe::ImmediateSampleWriteState::OpenFailed);
    CHECK(result.writes[2].state == doe::ImmediateSampleWriteState::SkippedInvalid);
    CHECK(result.writes[3].state == doe::ImmediateSampleWriteState::Opened);

    REQUIRE(io.textWrites().size() == 2);
    CHECK(io.textWrites()[0].path == failedLhs && !io.textWrites()[0].opened);
    CHECK(io.textWrites()[1].path == String((control / "Samples_USR.par").u16string()));
    CHECK(io.textWrites()[1].opened && io.textWrites()[1].writeSucceeded);
    CHECK(!fs::exists(control / "DEF_DOE.PAR"));
    CHECK(!fs::exists(control / "Samples_LHS.par"));
    CHECK(!fs::exists(control / "Samples_FF.par"));
    CHECK(bytes(control / "Samples_USR.par") == "2\r\n1\t30\t\r\n");
}

TEST_CASE(doe_sample_immediate_current_open_false_still_attempts_all_fixed_modes)
{
    const auto root = scratch();
    const auto control = root / "DoeStudies" / "StudyA" / "Control";
    fs::create_directories(control);
    auto study = populated();
    const auto originalSamples = study.samples;
    const String failedCurrent((control / "DEF_DOE.PAR").u16string());
    SequenceIo io(failedCurrent);

    const auto result = doe::saveType2SampleSequenceImmediate(
        study, String((root / "DoeStudies").u16string()), io);
    const std::array<const char*, 4> names{
        "DEF_DOE.PAR", "Samples_LHS.par", "Samples_FF.par", "Samples_USR.par"};
    const std::array<int, 4> modes{1, 0, 1, 2};
    REQUIRE(io.textWrites().size() == 4);
    for (std::size_t i = 0; i < names.size(); ++i) {
        const String expectedPath((control / names[i]).u16string());
        CHECK(result.writes[i].mode == modes[i]);
        CHECK(result.writes[i].path == expectedPath);
        CHECK(io.textWrites()[i].path == expectedPath);
        if (i == 0) {
            CHECK(result.writes[i].state == doe::ImmediateSampleWriteState::OpenFailed);
            CHECK(!io.textWrites()[i].opened);
            CHECK(!fs::exists(control / names[i]));
        } else {
            CHECK(result.writes[i].state == doe::ImmediateSampleWriteState::Opened);
            CHECK(io.textWrites()[i].opened && io.textWrites()[i].writeSucceeded);
            const int value = modes[i] == 0 ? 10 : modes[i] == 1 ? 20 : 30;
            CHECK(bytes(control / names[i]) == std::to_string(modes[i]) + "\r\n1\t" +
                std::to_string(value) + "\t\r\n");
        }
    }
    CHECK(study.samples.mode == originalSamples.mode);
    CHECK(study.samples.tables == originalSamples.tables);
    CHECK(study.samples.invalid == originalSamples.invalid);
    CHECK(study.samples.counts == originalSamples.counts);
}

int main() { return testkit::run_all(); }
