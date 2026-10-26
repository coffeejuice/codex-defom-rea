// ctest-labels: m2m-g0 isolated-cli
// Private job/receipt contract only; synthetic observations are not process acceptance.
#include <testkit/check.hpp>
#include "../support/test_paths.hpp"
#include "../../src/cli/isolated_save_job.hpp"
#include "moproj2mst.h"
#include <array>
#include <atomic>
#include <fstream>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <vector>

namespace fs = std::filesystem;
namespace th = m2m::engine::terminal_harness;
using m2m::cli::IsolatedJob;
namespace {
struct Scratch {
    fs::path root;
    Scratch()
    {
        static std::atomic<unsigned> serial{0};
        root = m2m_test::scratchOutput(m2m_test::scratchRoot() / ("isolated-job-" +
            std::to_string(GetCurrentProcessId()) + "-" + std::to_string(GetTickCount64()) + "-" + std::to_string(++serial)));
        REQUIRE(!fs::exists(root)); REQUIRE(fs::create_directory(root));
    }
    ~Scratch()
    {
        try {
            const auto checked = m2m_test::scratchOutput(root);
            if (checked == fs::weakly_canonical(root) && root.filename().string().starts_with("isolated-job-")) {
                std::error_code ignored; fs::remove_all(checked, ignored);
            }
        } catch (...) {}
    }
    fs::path leaf(const std::string& name = "leaf")
    {
        const auto path = root / name; REQUIRE(fs::create_directory(path)); return path;
    }
    IsolatedJob job() const
    {
        IsolatedJob value;
        value.project = (root / "project" / L"part \u03bb\U0001f642.moproj").u16string();
        value.output = (root / "project" / "result.MST").u16string();
        value.deformDir = (root / "deform").u16string();
        value.workingDirectory = root.u16string();
        value.flags = M2M_FLAG_WRITE_SAVE | M2M_FLAG_STRICT | M2M_FLAG_WRITE_MDT;
        value.codepage = 1252;
        value.nonce = 0x0102030405060708ULL;
        return value;
    }
};
void write(const fs::path& path, const std::string& bytes)
{
    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    REQUIRE(out.is_open()); out.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
    out.close(); REQUIRE(!out.fail());
}
std::string read(const fs::path& path)
{
    std::ifstream in(path, std::ios::binary); REQUIRE(in.is_open());
    std::string bytes{std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
    REQUIRE(!in.bad()); return bytes;
}
template<class F> bool rejects(F&& operation)
{
    try { operation(); } catch (const std::exception&) { return true; }
    return false;
}
void put(std::string& bytes, std::size_t offset, std::uint32_t value)
{ for (unsigned i = 0; i < 4; ++i) bytes.at(offset + i) = static_cast<char>(value >> (8 * i)); }
th::Observation terminal()
{
    th::Observation value;
    value.disposition = th::Disposition::CandidateProcessTerminal;
    value.childCreated = value.resumed = value.childStopped = true;
    value.childPid = 1234; value.childStatus = th::terminalStatus;
    value.sha256Before = value.sha256After = std::string(64, 'a'); value.elapsedMs = 42;
    value.transcript.profile = th::ProtocolProfile::RemeshV2;
    value.transcript.started = value.transcript.boundary = true; value.transcript.frames = 2;
    value.transcript.save = th::SaveSnapshot{th::snapshotMask & ~th::snapshotModalPending, 1, 0x10000, 0};
    return value;
}
} // namespace

TEST_CASE(cli_isolated_job_roundtrips_product_paths_unicode_flags_codepage_and_cwd)
{
    Scratch scratch; const auto leaf = scratch.leaf(); const auto value = scratch.job();
    m2m::cli::writeIsolatedJob(leaf, value);
    const auto actual = m2m::cli::readIsolatedJob(leaf);
    CHECK(actual.project == value.project); CHECK(actual.output == value.output);
    CHECK(actual.deformDir == value.deformDir); CHECK(actual.workingDirectory == value.workingDirectory);
    CHECK_EQ(actual.flags, value.flags); CHECK_EQ(actual.codepage, value.codepage);
    CHECK_EQ(actual.nonce, value.nonce);
    CHECK(!fs::exists(fs::path(value.project))); // Transport does not pre-open selected project.
    CHECK(fs::file_size(leaf / "isolated-save.job") <= 65536);
}
TEST_CASE(cli_isolated_job_optional_output_deform_and_exclusive_no_replace)
{
    Scratch scratch; const auto leaf = scratch.leaf(); auto value = scratch.job();
    value.output.clear(); value.deformDir.clear(); value.codepage = 65001;
    m2m::cli::writeIsolatedJob(leaf, value);
    const auto original = read(leaf / "isolated-save.job");
    const auto actual = m2m::cli::readIsolatedJob(leaf);
    CHECK(actual.output.empty() && actual.deformDir.empty()); CHECK_EQ(actual.codepage, 65001u);
    value.project += u"changed";
    CHECK(rejects([&] { m2m::cli::writeIsolatedJob(leaf, value); }));
    CHECK(read(leaf / "isolated-save.job") == original);
}
TEST_CASE(cli_isolated_job_rejects_bad_input_before_creating_job)
{
    Scratch scratch;
    const std::vector<std::function<void(IsolatedJob&)>> corrupt{
        [](auto& j) { j.project.clear(); }, [](auto& j) { j.project = u"relative.moproj"; },
        [](auto& j) { j.output = u"relative.MST"; }, [](auto& j) { j.deformDir = u"relative-dir"; },
        [](auto& j) { j.workingDirectory.clear(); }, [](auto& j) { j.workingDirectory = u"relative"; },
        [](auto& j) { j.project.push_back(0); }, [](auto& j) { j.project.push_back(0xd800); },
        [](auto& j) { j.project.push_back(0xdc00); }, [](auto& j) { j.project = u"C:/" + std::u16string(16382, u'x'); },
        [](auto& j) { j.flags = 0; }, [](auto& j) { j.flags |= 0x80000000u; },
        [](auto& j) { j.nonce = 0; },
        [](auto& j) { j.codepage = 0xffffffffu; }, [](auto& j) { j.project = u"C:/file:stream"; },
        [](auto& j) { j.project = u"\\\\?\\C:\\device"; }};
    for (std::size_t i = 0; i < corrupt.size(); ++i) {
        const auto leaf = scratch.leaf("bad-" + std::to_string(i)); auto value = scratch.job(); corrupt[i](value);
        CHECK(rejects([&] { m2m::cli::writeIsolatedJob(leaf, value); })); CHECK(!fs::exists(leaf / "isolated-save.job"));
    }
}
TEST_CASE(cli_isolated_job_accepts_exact_total_cap_and_rejects_overflow)
{
    Scratch scratch; const auto leaf = scratch.leaf(); IsolatedJob value;
    value.flags = M2M_FLAG_WRITE_SAVE; value.workingDirectory = u"C:/";
    value.nonce = 1;
    value.project = u"C:/" + std::u16string(16381, u'x');
    value.output = u"C:/" + std::u16string(16352, u'y'); //52+2*(16384+16355+3)=65536.
    m2m::cli::writeIsolatedJob(leaf, value); CHECK_EQ(fs::file_size(leaf / "isolated-save.job"), 65536u);
    CHECK(m2m::cli::readIsolatedJob(leaf).project == value.project);
    value.output.push_back(u'z'); const auto other = scratch.leaf("overflow");
    CHECK(rejects([&] { m2m::cli::writeIsolatedJob(other, value); })); CHECK(!fs::exists(other / "isolated-save.job"));
}
TEST_CASE(cli_isolated_job_rejects_malformed_binary_truncation_lengths_and_utf16)
{
    Scratch scratch; const auto good = scratch.leaf("good"); m2m::cli::writeIsolatedJob(good, scratch.job());
    const auto original = read(good / "isolated-save.job");
    const std::vector<std::function<void(std::string&)>> corrupt{
        [](auto& b) { b[0] ^= 1; }, [](auto& b) { put(b, 8, 2); }, [](auto& b) { put(b, 12, 0); },
        [](auto& b) { put(b, 24, 16385); }, [](auto& b) { put(b, 48, 1); },
        [](auto& b) { b.resize(51); }, [](auto& b) { b.pop_back(); }, [](auto& b) { b.push_back(0); },
        [](auto& b) { b[52] = 0; b[53] = 0; }, [](auto& b) { b[52] = 0; b[53] = static_cast<char>(0xd8); },
        [](auto& b) { put(b, 40, 0); put(b, 44, 0); },
        [](auto& b) { put(b, 16, 0); }, [](auto& b) { put(b, 20, 0xffffffffu); }};
    for (std::size_t i = 0; i < corrupt.size(); ++i) {
        const auto leaf = scratch.leaf("malformed-" + std::to_string(i)); auto bytes = original; corrupt[i](bytes);
        write(leaf / "isolated-save.job", bytes); CHECK(rejects([&] { (void)m2m::cli::readIsolatedJob(leaf); }));
    }
}
TEST_CASE(cli_isolated_job_rejects_missing_oversized_directory_and_relative_storage)
{
    Scratch scratch; const auto missing = scratch.leaf();
    CHECK(rejects([&] { (void)m2m::cli::readIsolatedJob(missing); }));
    write(missing / "isolated-save.job", std::string(65537, 'x'));
    CHECK(rejects([&] { (void)m2m::cli::readIsolatedJob(missing); }));
    const auto directory = scratch.leaf("directory"); fs::create_directory(directory / "isolated-save.job");
    CHECK(rejects([&] { (void)m2m::cli::readIsolatedJob(directory); }));
    CHECK(rejects([&] { m2m::cli::writeIsolatedJob("relative", scratch.job()); }));
}
TEST_CASE(cli_isolated_terminal_receipt_commits_complete_qualified_json_without_replacing)
{
    Scratch scratch; fs::create_directory(scratch.root / "project"); const auto target = scratch.root / "receipt.json";
    const auto value = scratch.job(); m2m::cli::commitIsolatedTerminalReceipt(target, terminal(), value);
    const auto bytes = read(target);
    CHECK(bytes.starts_with("{\n") && bytes.ends_with("}\n"));
    CHECK(bytes.find("\"termination_observed\":true") != std::string::npos);
    CHECK(bytes.find("\"parent_relay\":{\"requested\":true") != std::string::npos);
    CHECK(bytes.find("not_observed_in_this_receipt") != std::string::npos);
    CHECK(bytes.find("CaptureContextUnimplemented") != std::string::npos);
    CHECK(bytes.find("\\u03bb\\ud83d\\ude42") != std::string::npos);
    CHECK(bytes.find("\"pid\":1234") != std::string::npos && bytes.find("\"flags\":1015") != std::string::npos);
    CHECK(rejects([&] { m2m::cli::commitIsolatedTerminalReceipt(target, terminal(), value); }));
    CHECK(read(target) == bytes);
}
TEST_CASE(cli_isolated_terminal_receipt_rejects_untrusted_outcomes_before_create)
{
    Scratch scratch; fs::create_directory(scratch.root / "project"); const auto job = scratch.job();
    const std::vector<std::function<void(th::Observation&)>> corrupt{
        [](auto& o) { o.disposition = th::Disposition::Returned; }, [](auto& o) { o.stopReason = th::StopReason::Timeout; },
        [](auto& o) { o.childCreated = false; }, [](auto& o) { o.resumed = false; }, [](auto& o) { o.childStopped = false; },
        [](auto& o) { o.childPid = 0; }, [](auto& o) { o.childStatus = 0; }, [](auto& o) { o.supervisorStopRequested = true; },
        [](auto& o) { o.transcript.returned = true; }, [](auto& o) { o.transcript.backendFailure = true; },
        [](auto& o) { o.transcript.started = false; }, [](auto& o) { o.transcript.boundary = false; },
        [](auto& o) { o.transcript.frames = 1; }, [](auto& o) { o.transcript.save.reset(); },
        [](auto& o) { o.transcript.save->flags |= 0x10000; }, [](auto& o) { o.sha256After[0] = 'b'; },
        [](auto& o) { o.sha256Before.clear(); }, [](auto& o) { o.transcript.profile = th::ProtocolProfile::SafetyV1; }};
    for (std::size_t i = 0; i < corrupt.size(); ++i) {
        const auto target = scratch.root / ("rejected-" + std::to_string(i) + ".json");
        auto observed = terminal(); corrupt[i](observed);
        CHECK(rejects([&] { m2m::cli::commitIsolatedTerminalReceipt(target, observed, job); })); CHECK(!fs::exists(target));
    }
}
TEST_CASE(cli_isolated_terminal_receipt_accepts_earlier_false_status_and_rejects_project_output_aliases)
{
    Scratch scratch; fs::create_directory(scratch.root / "project"); auto job = scratch.job();
    auto observed = terminal(); observed.transcript.save->flags &= ~th::snapshotMstSucceeded;
    m2m::cli::commitIsolatedTerminalReceipt(scratch.root / "mst-false.json", observed, job);
    CHECK(read(scratch.root / "mst-false.json").find("\"flags\":503") != std::string::npos);
    const auto inside = scratch.root / "project" / "new-receipt.json";
    CHECK(rejects([&] { m2m::cli::commitIsolatedTerminalReceipt(inside, terminal(), job); })); CHECK(!fs::exists(inside));
    job.output = (scratch.root / "external.MST").u16string();
    CHECK(rejects([&] { m2m::cli::commitIsolatedTerminalReceipt(fs::path(job.output), terminal(), job); }));
    CHECK(!fs::exists(fs::path(job.output)));
    CHECK(rejects([&] { m2m::cli::commitIsolatedTerminalReceipt("relative.json", terminal(), job); }));
}

int main() { return testkit::run_all(); }
