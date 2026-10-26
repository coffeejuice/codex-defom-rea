// ctest-labels: m2m-mdt
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <testkit/check.hpp>
#include "../../src/engine/simcontrol300_remesh_services.hpp"
#include "../support/test_paths.hpp"
#include <cerrno>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>

using namespace m2m::engine::simcontrol300;
namespace terminal = m2m::engine::simcontrol300::detail;
namespace {
struct Scratch {
    std::filesystem::path dir;
    Scratch() {
        static unsigned sequence = 0;
        dir = m2m_test::scratchOutput(m2m_test::scratchRoot() /
            ("r106-" + std::to_string(GetCurrentProcessId()) + "-" + std::to_string(++sequence)));
        std::filesystem::create_directories(dir);
    }
    ~Scratch() {
        std::error_code ec;
        std::filesystem::remove_all(m2m_test::scratchOutput(dir), ec);
    }
    std::filesystem::path path() const { return dir / "LOCAL_REMESH.DAT"; }
    void seed() {
        std::ofstream file(path(), std::ios::binary);
        file << "KEYWORD_DRIVEN\nDONE\n";
        REQUIRE(file.good());
    }
    std::string bytes() const {
        std::ifstream file(path(), std::ios::binary);
        REQUIRE(file.good());
        return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    }
};
struct DenyWrite {
    HANDLE handle;
    explicit DenyWrite(const Scratch& scratch)
        : handle(CreateFileW(scratch.path().c_str(), GENERIC_READ, FILE_SHARE_READ,
                            nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr)) {
        REQUIRE(handle != INVALID_HANDLE_VALUE);
    }
    ~DenyWrite() { CloseHandle(handle); }
};
// These tests never dispatch an admitted endpoint. The counters verify that
// denied profiles and earlier writer branches do not enter the service.
struct UnusedService final : terminal::RemeshTerminalService {
    terminal::RemeshTerminalProfile selected;
    mutable unsigned profileReads = 0;
    unsigned dispatches = 0, failures = 0;
    explicit UnusedService(terminal::RemeshTerminalProfile profile) : selected(profile) {}
    terminal::RemeshTerminalProfile profile() const noexcept override {
        ++profileReads; return selected;
    }
    terminal::RemeshTerminalDispatchFailure dispatch(const terminal::RemeshTerminalRequest&) noexcept override {
        ++dispatches;
        return terminal::RemeshTerminalDispatchFailure::PublicationFailed;
    }
    void failClosed(const terminal::RemeshTerminalRequest&,
                    terminal::RemeshTerminalDispatchFailure) noexcept override { ++failures; }
};
}

TEST_CASE(remesh_terminal_request_identifies_first_invalid_printf_without_changing_errno)
{
    errno = EDOM;
    const auto request = terminal::remeshTerminalRequestFor(terminal::RemeshTerminalProfile::FreshDefault);
    const int after = errno;
    REQUIRE(request);
    CHECK(request->site == terminal::RemeshTerminalSite::FirstNullFilePrintf);
    CHECK(request->proof == terminal::RemeshTerminalProof::NativeFreshDefaultStartup);
    CHECK_EQ(request->sourceAddress, 0x1400abcf0ULL);
    CHECK_EQ(request->expectedProcessStatus, 0xc0000417U);
    CHECK_EQ(request->crtErrno, 22);
    CHECK_EQ(request->formatAttemptOrdinal, 1U);
    CHECK_EQ(after, EDOM);
}

TEST_CASE(remesh_terminal_unknown_custom_and_invalid_profiles_have_no_request)
{
    for (const auto profile : {terminal::RemeshTerminalProfile::Unknown,
             terminal::RemeshTerminalProfile::Custom, static_cast<terminal::RemeshTerminalProfile>(99)})
        CHECK(!terminal::remeshTerminalRequestFor(profile));
}

TEST_CASE(remesh_terminal_failed_open_rejects_unknown_custom_and_invalid_service_profiles)
{
    Scratch scratch; scratch.seed(); const auto before = scratch.bytes();
    DenyWrite denial(scratch);
    RemeshState owner; owner.enabled = true;
    for (const auto profile : {terminal::RemeshTerminalProfile::Unknown,
             terminal::RemeshTerminalProfile::Custom, static_cast<terminal::RemeshTerminalProfile>(99)}) {
        UnusedService service(profile);
        errno = EDOM;
        const auto report = terminal::saveRemeshAtProjectSaveWithTerminalService(
            owner, scratch.dir.string(), NumericLocale::KnownC, &service);
        const int after = errno;
        CHECK(report.outcome == ApplyResult::UnsupportedSnapshot);
        REQUIRE(report.opened); CHECK(!*report.opened);
        CHECK(report.invalidParameterReached); CHECK(!report.removalSucceeded);
        CHECK_EQ(service.profileReads, 1U); CHECK_EQ(service.dispatches, 0U); CHECK_EQ(service.failures, 0U);
        CHECK_EQ(after, EDOM); CHECK_EQ(scratch.bytes(), before);
    }
}

TEST_CASE(remesh_terminal_null_private_service_matches_legacy_failed_open_guard)
{
    Scratch scratch; scratch.seed(); const auto before = scratch.bytes();
    DenyWrite denial(scratch);
    RemeshState owner; owner.enabled = true;
    const auto legacy = saveRemeshAtProjectSave(owner, scratch.dir.string(), NumericLocale::KnownC);
    const auto privateCall = terminal::saveRemeshAtProjectSaveWithTerminalService(
        owner, scratch.dir.string(), NumericLocale::KnownC, nullptr);
    CHECK(legacy.outcome == ApplyResult::UnsupportedSnapshot);
    CHECK(privateCall.outcome == legacy.outcome); CHECK(privateCall.opened == legacy.opened);
    REQUIRE(legacy.opened); CHECK(!*legacy.opened);
    CHECK(privateCall.invalidParameterReached); CHECK(legacy.invalidParameterReached);
    CHECK(!privateCall.removalSucceeded); CHECK(!legacy.removalSucceeded);
    CHECK_EQ(scratch.bytes(), before);
}

TEST_CASE(remesh_terminal_disabled_writer_removes_before_locale_or_service_admission)
{
    Scratch scratch; scratch.seed(); RemeshState owner; owner.enabled = false;
    UnusedService service(terminal::RemeshTerminalProfile::FreshDefault);
    const auto report = terminal::saveRemeshAtProjectSaveWithTerminalService(
        owner, scratch.dir.string(), NumericLocale::Unrecovered, &service);
    CHECK(report.outcome == ApplyResult::Completed);
    REQUIRE(report.removalSucceeded); CHECK(*report.removalSucceeded);
    CHECK(!report.opened); CHECK(!report.invalidParameterReached);
    CHECK(!std::filesystem::exists(scratch.path()));
    CHECK_EQ(service.profileReads, 0U); CHECK_EQ(service.dispatches, 0U); CHECK_EQ(service.failures, 0U);
}

TEST_CASE(remesh_terminal_enabled_unqualified_locale_stops_before_open_and_service)
{
    Scratch scratch; scratch.seed(); const auto before = scratch.bytes();
    RemeshState owner; owner.enabled = true;
    UnusedService service(terminal::RemeshTerminalProfile::FreshDefault);
    const auto report = terminal::saveRemeshAtProjectSaveWithTerminalService(
        owner, scratch.dir.string(), NumericLocale::Unrecovered, &service);
    CHECK(report.outcome == ApplyResult::UnsupportedSnapshot);
    CHECK(!report.opened); CHECK(!report.invalidParameterReached); CHECK(!report.removalSucceeded);
    CHECK_EQ(service.profileReads, 0U); CHECK_EQ(service.dispatches, 0U); CHECK_EQ(service.failures, 0U);
    CHECK_EQ(scratch.bytes(), before);
}

TEST_CASE(remesh_terminal_successful_open_keeps_legacy_body_without_querying_service)
{
    Scratch legacyDisk, privateDisk;
    RemeshState owner; owner.enabled = true; owner.method = 4; owner.filterDistance = 7.5;
    const auto legacy = saveRemeshAtProjectSave(owner, legacyDisk.dir.string(), NumericLocale::KnownC);
    UnusedService service(terminal::RemeshTerminalProfile::FreshDefault);
    const auto report = terminal::saveRemeshAtProjectSaveWithTerminalService(
        owner, privateDisk.dir.string(), NumericLocale::KnownC, &service);
    CHECK(legacy.outcome == ApplyResult::Completed); CHECK(report.outcome == legacy.outcome);
    REQUIRE(legacy.opened); REQUIRE(report.opened); CHECK(*legacy.opened); CHECK(*report.opened);
    CHECK(!report.invalidParameterReached); CHECK(!report.removalSucceeded);
    CHECK_EQ(privateDisk.bytes(), legacyDisk.bytes());
    CHECK_EQ(service.profileReads, 0U); CHECK_EQ(service.dispatches, 0U); CHECK_EQ(service.failures, 0U);
}

int main() { return testkit::run_all(); }
