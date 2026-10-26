#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "isolated_save.hpp"
#include "isolated_save_job.hpp"
#include "../core/conversion_call_services.hpp"
#include "../core/remesh_terminal_call_service.hpp"
#include <cerrno>
#include <clocale>
#include <cstdio>
#include <exception>
#include <limits>
#include <stdexcept>
#include <string_view>
#include <type_traits>

namespace m2m::cli {
namespace {
namespace th = engine::terminal_harness;
namespace terminal = engine::simcontrol300::detail;
constexpr std::wstring_view workerRole = L"--m2m-isolated-save-worker-v1";
constexpr std::wstring_view optIn = L"--allow-owned-worker-termination";

template<class T> bool decimal(std::wstring_view text, T& value) noexcept
{
    static_assert(std::is_unsigned_v<T>);
    if (text.empty() || text.size() > std::numeric_limits<T>::digits10 + 1U) return false;
    T parsed = 0;
    for (const wchar_t c : text) {
        if (c < L'0' || c > L'9') return false;
        const T digit = static_cast<T>(c - L'0');
        if (parsed > ((std::numeric_limits<T>::max)() - digit) / 10) return false;
        parsed = static_cast<T>(parsed * 10 + digit);
    }
    value = parsed;
    return true;
}

bool send(HANDLE pipe, const th::FrameV2& frame) noexcept
{
    const auto wire = th::encodeV2(frame);
    DWORD written = 0;
    return WriteFile(pipe, wire.data(), static_cast<DWORD>(wire.size()), &written, nullptr) &&
        written == static_cast<DWORD>(wire.size());
}
[[noreturn]] void park() noexcept { for (;;) Sleep(INFINITE); }
[[noreturn]] void failAndPark(HANDLE pipe, std::uint64_t nonce, th::FailureTag reason) noexcept
{
    (void)send(pipe, {th::Kind::BackendFailure, nonce, th::remeshSite, 0,
                     static_cast<std::uint32_t>(reason)});
    park();
}
th::SaveSnapshot snapshot(const engine::SaveCallState& call) noexcept
{
    std::uint32_t flags = th::snapshotPresent;
    if (call.opened) flags |= th::snapshotOpened;
    if (call.validationComplete) flags |= th::snapshotValidated;
    if (call.lastOperationModalPending) flags |= th::snapshotModalPending;
    if (call.projectWriteAttempted) flags |= th::snapshotProjectAttempted;
    if (call.projectWriteReturned) flags |= th::snapshotProjectReturned;
    if (call.projectWriteSucceeded) flags |= th::snapshotProjectSucceeded;
    if (call.mstWriteAttempted) flags |= th::snapshotMstAttempted;
    if (call.mstWriteReturned) flags |= th::snapshotMstReturned;
    if (call.mstWriteSucceeded) flags |= th::snapshotMstSucceeded;
    return {flags, call.checkedSubprojects, static_cast<std::uint32_t>(call.answer),
            static_cast<std::uint32_t>(call.pendingDefault)};
}

// Explicit opt-in child policy under a static fresh-default assumption. This
// fixed endpoint does not execute the native context/debugger/filter prelude.
class StaticDefaultEndpoint final : public m2m::detail::RemeshTerminalCallService {
    HANDLE pipe_;
    std::uint64_t nonce_;
public:
    StaticDefaultEndpoint(HANDLE pipe, std::uint64_t nonce) noexcept : pipe_(pipe), nonce_(nonce) {}
    terminal::RemeshTerminalProfile profile() const noexcept override
    { return terminal::RemeshTerminalProfile::FreshDefault; }
    terminal::RemeshTerminalDispatchFailure dispatch(
        const terminal::RemeshTerminalRequest& request, const engine::SaveCallState& liveCopy) noexcept override
    {
        if (request.site != terminal::RemeshTerminalSite::FirstNullFilePrintf ||
            request.proof != terminal::RemeshTerminalProof::NativeFreshDefaultStartup ||
            request.sourceAddress != 0x1400abcf0ULL || request.expectedProcessStatus != th::terminalStatus ||
            request.crtErrno != 22 || request.formatAttemptOrdinal != 1 || errno != 22)
            return terminal::RemeshTerminalDispatchFailure::InvalidFailureCode;
        if (!send(pipe_, {th::Kind::Boundary, nonce_, th::remeshSite,
                request.expectedProcessStatus, static_cast<std::uint32_t>(request.crtErrno),
                request.formatAttemptOrdinal, 1, 1, snapshot(liveCopy)}))
            return terminal::RemeshTerminalDispatchFailure::PublicationFailed;
        (void)TerminateProcess(GetCurrentProcess(), request.expectedProcessStatus);
        return terminal::RemeshTerminalDispatchFailure::EndpointReturned;
    }
    void failClosed(const terminal::RemeshTerminalRequest&, terminal::RemeshTerminalDispatchFailure failure,
                    const engine::SaveCallState&) noexcept override
    {
        th::FailureTag tag = th::FailureTag::InvalidFailureCode;
        switch (failure) {
        case terminal::RemeshTerminalDispatchFailure::PublicationFailed: tag = th::FailureTag::PublicationFailed; break;
        case terminal::RemeshTerminalDispatchFailure::EndpointReturned: tag = th::FailureTag::EndpointReturned; break;
        case terminal::RemeshTerminalDispatchFailure::InvalidFailureCode: break;
        }
        (void)send(pipe_, {th::Kind::BackendFailure, nonce_, th::remeshSite, 0, static_cast<std::uint32_t>(tag)});
        // The leaf writer retains the stack and parks after this callback.
    }
};
struct PipeLease {
    HANDLE value;
    ~PipeLease() { CloseHandle(value); }
};
}

int isolatedSaveWorkerMain(int argc, wchar_t** argv)
{
    if (argc != 7 || !argv) return 65;
    for (int i = 0; i < argc; ++i) if (!argv[i]) return 65;
    unsigned rawCase = 0;
    std::uintptr_t rawPipe = 0, unrelated = 0;
    std::uint64_t nonce = 0;
    if (std::wstring_view(argv[1]) != workerRole || std::wstring_view(argv[2]) != optIn ||
        !decimal(argv[3], rawCase) || !decimal(argv[4], rawPipe) || !decimal(argv[5], nonce) ||
        !decimal(argv[6], unrelated) || !nonce || unrelated || !rawPipe ||
        rawCase != static_cast<unsigned>(th::ChildCase::ProductIsolated) ||
        !th::validWorkerSelection(th::WorkerRole::ProductIsolatedSave, th::ProtocolProfile::RemeshV2,
                                  th::ChildCase::ProductIsolated)) return 65;
    const HANDLE pipe = reinterpret_cast<HANDLE>(rawPipe);
    if (pipe == INVALID_HANDLE_VALUE || GetFileType(pipe) != FILE_TYPE_PIPE) return 65;
    PipeLease lease{pipe};
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);
    if (!send(pipe, {th::Kind::Started, nonce, th::remeshSite, 0, 0, GetCurrentProcessId()}))
        failAndPark(pipe, nonce, th::FailureTag::PublicationFailed);
    try {
        const auto job = readIsolatedJob(std::filesystem::current_path());
        if (job.nonce != nonce) throw std::runtime_error("isolated Save job nonce mismatch");
        const std::filesystem::path workingDirectory(job.workingDirectory);
        if (!SetCurrentDirectoryW(workingDirectory.c_str()))
            throw std::runtime_error("cannot restore isolated Save caller directory");
        if (!std::setlocale(LC_NUMERIC, "C")) return 66;
        StaticDefaultEndpoint endpoint(pipe, nonce);
        m2m::detail::ConversionCallServices services{};
        services.remeshTerminal = &endpoint;
        ConversionDetails details;
        const auto result = m2m::detail::convertDetailedWithServices(
            job.project, job.output, job.flags, job.codepage, details, services, 0, job.deformDir);
        if (result.code < 0 || result.code > 7 ||
            (details.returnedCode && *details.returnedCode != result.code))
            failAndPark(pipe, nonce, th::FailureTag::InvalidFailureCode);
        if (!result.message.empty()) std::fprintf(stderr, "%s\n", result.message.c_str());
        const auto save = details.saveCall ? snapshot(*details.saveCall) : th::SaveSnapshot{};
        if (!send(pipe, {th::Kind::Returned, nonce, th::remeshSite,
                        static_cast<std::uint32_t>(result.code), 0, 0, 0, 0, save}))
            failAndPark(pipe, nonce, th::FailureTag::PublicationFailed);
        return 0; // Transport succeeded; the adapter0..7 is the Returned payload.
    } catch (const std::exception& error) {
        std::fprintf(stderr, "isolated Save worker setup failure: %s\n", error.what());
        return 66; // No Returned record: supervisor failure, not adapter success.
    } catch (...) {
        return 66;
    }
}

} // namespace m2m::cli
