// ctest-labels: core lifecycle-failure
#include "../support/test_paths.hpp"

#include <testkit/check.hpp>

#include <filesystem>
#include <fstream>
#include <iterator>
#include <memory>
#include <string>

#include "m2m/convert_details.hpp"

namespace fs = std::filesystem;

namespace {
struct Scratch {
    fs::path directory, project, output, remesh;
    Scratch()
    {
        directory = m2m_test::scratchOutput(m2m_test::scratchRoot() /
            ("detailed-" + std::to_string(GetCurrentProcessId()) + "-" +
             std::to_string(GetTickCount64())));
        fs::create_directories(directory);
        project = directory / "project.moproj";
        output = directory / "out.MST";
        remesh = directory / "LOC_DEN_CTRL.DAT";
    }
    void writeRemesh(const std::string& bytes)
    { std::ofstream(remesh, std::ios::binary | std::ios::trunc) << bytes; }
    std::string remeshBytes() const
    {
        std::ifstream input(remesh, std::ios::binary);
        return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
    }
};
} // namespace

TEST_CASE(convert_details_reset_then_bind_actual_native_study_io)
{
    Scratch disk;
    const std::string remeshContent = "FILTER_DISTANCE 7\nDONE\n";
    disk.writeRemesh(remeshContent);

    m2m::ConversionDetails details;
    const auto staleJournal = std::make_shared<const m2m::doe::StudyXmlJournal>();
    details.xmlJournal = staleJournal;
    details.ioMode = m2m::doe::IoMode::Recording;
    details.saveCall = m2m::engine::SaveCallState{};
    details.returnedCode = M2M_OK;

    // Legacy no-WRITE_SAVE preflight fails before creating the Project's StudyIo.
    const auto early = m2m::convertDetailed(disk.project.u16string(), disk.output.u16string(),
                                            0, 1252, details);
    CHECK_EQ(early.code, M2M_E_OPEN);
    REQUIRE(details.returnedCode.has_value());
    CHECK_EQ(*details.returnedCode, early.code);
    CHECK(!details.xmlJournal);
    CHECK(!details.ioMode.has_value());
    CHECK(!details.saveCall.has_value());
    CHECK(disk.remeshBytes() == remeshContent);

    // Reuse the same details value for full Save. The actual StudyIo is created
    // before the missing-project open result reaches the Save decision.
    const auto fullSave = m2m::convertDetailed(disk.project.u16string(), disk.output.u16string(),
        M2M_FLAG_WRITE_SAVE, 1252, details);
    CHECK_EQ(fullSave.code, M2M_E_PARSE); // existing missing-open Save contract
    REQUIRE(details.returnedCode.has_value());
    CHECK_EQ(*details.returnedCode, fullSave.code);
    REQUIRE(details.xmlJournal != nullptr);
    CHECK(details.xmlJournal != staleJournal);
    REQUIRE(details.ioMode.has_value());
    CHECK(*details.ioMode == m2m::doe::IoMode::Native);
    CHECK(details.xmlJournal->attempts().empty());
    CHECK(details.xmlJournal->events().empty());
    REQUIRE(details.saveCall.has_value());
    CHECK(!details.saveCall->opened);
    CHECK(details.saveCall->answer == m2m::engine::DialogAnswer::No);
    CHECK(!details.saveCall->projectWriteAttempted);
    CHECK(!details.saveCall->mstWriteAttempted);
    CHECK(!fs::exists(disk.project));
    CHECK(!fs::exists(disk.output));
    CHECK(disk.remeshBytes() == remeshContent);
}

int main() { return testkit::run_all(); }
