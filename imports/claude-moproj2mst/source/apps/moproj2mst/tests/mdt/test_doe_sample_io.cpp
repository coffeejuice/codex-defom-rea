#include "../support/test_paths.hpp"
// ctest-labels: m2m-mdt mdt-l6
#include <testkit/check.hpp>

#include <fstream>
#include <iterator>

#include "m2m/doe_sample_io.hpp"

namespace fs = std::filesystem;
using namespace m2m;

namespace {
fs::path scratch(const char* label)
{
    static unsigned next = 0;
    const auto root = m2m_test::scratchOutput(m2m_test::scratchRoot() /
        (std::string("doe-sample-io-") + label + "-" + std::to_string(GetCurrentProcessId()) +
         "-" + std::to_string(GetTickCount64()) + "-" + std::to_string(++next)));
    REQUIRE(!fs::exists(root));
    return root;
}

std::string bytes(const fs::path& path)
{
    std::ifstream in(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
}

doe::Study populated()
{
    doe::Study s;
    s.samples.mode = 0;
    s.samples.tables[0] = {{10}, {11}};
    s.samples.tables[1] = {{20}};
    s.samples.counts[0] = 2;
    s.samples.counts[1] = 1;
    return s;
}

// Deterministic failure after an actual successful truncate/open. A disk-full
// condition is not forced on the user's drive; only its writer result is injected.
class PartialWriter final : public doe::StudyIo {
public:
    PartialWriter() : StudyIo(doe::IoMode::Native) {}
    bool ensureDirectory(const String&) override { throw std::runtime_error("unexpected mkdir"); }
    bool writeTextFile(const String& path, const std::string& data) override
    {
        std::ofstream out(fs::path(path.u16()), std::ios::binary | std::ios::trunc);
        REQUIRE(out.is_open());
        out.write(data.data(), 1);
        out.close();
        return recordTextWrite(path, data, true, false, false);
    }
};
}  // namespace

TEST_CASE(doe_sample_open_failure_preserves_tables_flags_and_missing_parent)
{
    const auto root = scratch("missing");
    auto io = doe::makeNativeStudyIo();
    auto s = populated();
    CHECK(!doe::loadSampleTable(s, 1, String((root / "Samples_FF.par").u16string()), *io));
    CHECK(!fs::exists(root));
    CHECK(s.samples.tables[0] == doe::SampleTable({{10}, {11}}));
    CHECK(s.samples.tables[1] == doe::SampleTable({{20}}));
    CHECK(s.samples.invalid[0] && s.samples.invalid[1]);
    CHECK(*s.samples.counts[0] == 2 && *s.samples.counts[1] == 1);
    REQUIRE(io->textWrites().size() == 1);
    CHECK(!io->textWrites()[0].opened);
    CHECK(io->directories().empty());

    fs::create_directories(root);
    CHECK(!doe::loadSampleTable(s, 1, String(root.u16string()), *io));
    CHECK(fs::is_directory(root));
    CHECK(s.samples.tables[0].size() == 2);
}

TEST_CASE(doe_sample_success_truncates_and_clears_current_not_requested)
{
    const auto root = scratch("truncate");
    fs::create_directories(root);
    const auto path = root / "Samples_FF.par";
    { std::ofstream old(path, std::ios::binary); old << "previous sample contents"; }
    auto io = doe::makeNativeStudyIo();
    auto s = populated();
    CHECK(doe::loadSampleTable(s, 1, String(path.u16string()), *io));
    CHECK(bytes(path) == "1\r\n");
    CHECK(s.samples.tables[0].empty());
    CHECK(s.samples.tables[1] == doe::SampleTable({{20}}));
    CHECK(s.samples.invalid[0]);
    CHECK(!s.samples.invalid[1]);
    CHECK(*s.samples.counts[0] == 2 && *s.samples.counts[1] == 1);
    REQUIRE(io->textWrites().size() == 1);
    CHECK(io->textWrites()[0].opened && io->textWrites()[0].writeSucceeded);
    CHECK(io->directories().empty());
}

TEST_CASE(doe_sample_write_failure_after_open_keeps_partial_file_and_changes_memory)
{
    const auto root = scratch("partial");
    fs::create_directories(root);
    const auto path = root / "Samples_FF.par";
    { std::ofstream old(path, std::ios::binary); old << "previous sample contents"; }
    PartialWriter io;
    auto s = populated();
    CHECK(doe::loadSampleTable(s, 1, String(path.u16string()), io));
    CHECK(bytes(path) == "1");
    CHECK(s.samples.tables[0].empty());
    CHECK(s.samples.tables[1] == doe::SampleTable({{20}}));
    CHECK(s.samples.invalid[0] && !s.samples.invalid[1]);
    REQUIRE(io.textWrites().size() == 1);
    CHECK(io.textWrites()[0].opened);
    CHECK(!io.textWrites()[0].writeSucceeded);
}

TEST_CASE(doe_sample_recording_and_mode_gates_do_not_touch_files)
{
    const auto root = scratch("recording");
    auto io = doe::makeRecordingStudyIo();
    auto s = populated();
    const auto path = String((root / "Samples_USR.par").u16string());
    CHECK(!doe::loadSampleTable(s, -1, path, *io));
    CHECK(!doe::loadSampleTable(s, 3, path, *io));
    CHECK(io->textWrites().empty());
    CHECK(doe::loadSampleTable(s, 2, path, *io)); // empty requested table: no memory change
    CHECK(s.samples.tables[0].size() == 2);
    CHECK(s.samples.invalid[2]);
    REQUIRE(io->textWrites().size() == 1);
    CHECK(io->textWrites()[0].requestedBytes == "2\r\n");
    CHECK(doe::loadSampleTable(s, 1, path, *io));
    CHECK(s.samples.tables[0].empty());
    CHECK(!s.samples.invalid[1]);
    CHECK(!fs::exists(root));
    CHECK(io->mode() == doe::IoMode::Recording);
}

TEST_CASE(doe_sample_undefined_current_mode_refuses_after_file_effect)
{
    const auto root = scratch("invalid-current");
    fs::create_directories(root);
    const auto path = root / "Samples_FF.par";
    auto io = doe::makeNativeStudyIo();
    auto s = populated();
    s.samples.mode = 9; // outside original storage; candidate refuses this undefined branch
    bool refused = false;
    try { doe::loadSampleTable(s, 1, String(path.u16string()), *io); }
    catch (const Error& e) { refused = e.code == M2M_E_UNSUPPORTED; }
    CHECK(refused);
    CHECK(bytes(path) == "1\r\n");
    CHECK(s.samples.invalid[1]);
    CHECK(s.samples.tables[1].size() == 1);
}

int main() { return testkit::run_all(); }
