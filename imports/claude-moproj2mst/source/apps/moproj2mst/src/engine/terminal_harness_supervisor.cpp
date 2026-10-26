#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <bcrypt.h>
#pragma comment(lib, "bcrypt.lib")

#include "terminal_harness_supervisor.hpp"
#include <algorithm>
#include <array>
#include <charconv>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace m2m::engine::terminal_harness {
namespace {
class Handle {
public:
    HANDLE value = nullptr;
    Handle() = default;
    explicit Handle(HANDLE h) : value(h) {}
    ~Handle() { reset(); }
    Handle(const Handle&) = delete;
    Handle& operator=(const Handle&) = delete;
    void reset(HANDLE h = nullptr) {
        if (value && value != INVALID_HANDLE_VALUE) CloseHandle(value);
        value = h;
    }
    explicit operator bool() const { return value && value != INVALID_HANDLE_VALUE; }
};
void require(bool ok, const char* what)
{
    if (!ok) throw std::runtime_error(std::string(what) + ": Win32=" + std::to_string(GetLastError()));
}
std::uint64_t fileSize(HANDLE file)
{
    LARGE_INTEGER n{};
    require(GetFileSizeEx(file, &n) != FALSE && n.QuadPart >= 0, "capture size");
    return static_cast<std::uint64_t>(n.QuadPart);
}
bool samePreferredPath(std::filesystem::path a, std::filesystem::path b)
{
    a.make_preferred(); b.make_preferred();
    return CompareStringOrdinal(a.c_str(), -1, b.c_str(), -1, TRUE) == CSTR_EQUAL;
}
void nonReparseDirectoryComponents(const std::filesystem::path& path)
{
    if (!path.is_absolute()) throw std::runtime_error("absolute explicit scratch root required");
    auto current = path.root_path();
    auto check = [](const std::filesystem::path& component) {
        const auto attributes = GetFileAttributesW(component.c_str());
        if (attributes == INVALID_FILE_ATTRIBUTES || !(attributes & FILE_ATTRIBUTE_DIRECTORY) ||
            (attributes & FILE_ATTRIBUTE_REPARSE_POINT))
            throw std::runtime_error("explicit scratch component missing, non-directory or reparse");
    };
    check(current);
    for (const auto& component : path.relative_path()) {
        if (component == L"." || component == L"..") throw std::runtime_error("noncanonical explicit scratch component");
        if (component.empty()) continue;
        current /= component;
        check(current);
    }
}
std::filesystem::path scratchDirectory(const Options& options)
{
    if (!options.explicitScratchRoot.empty()) {
        if (options.workerRole != WorkerRole::ProductIsolatedSave)
            throw std::runtime_error("explicit scratch root requires product worker role");
        nonReparseDirectoryComponents(options.explicitScratchRoot); // inspect aliases before canonical resolves them
        const auto root = std::filesystem::canonical(options.explicitScratchRoot);
        if (!samePreferredPath(options.explicitScratchRoot, root))
            throw std::runtime_error("canonical explicit scratch root required");
        if (!options.directory.is_absolute() || options.directory.filename().empty() ||
            options.directory.filename() == L"." || options.directory.filename() == L".." ||
            !samePreferredPath(options.directory.parent_path(), root) ||
            !samePreferredPath(options.directory, root / options.directory.filename()))
            throw std::runtime_error("new direct child of explicit scratch root required");
        const auto directory = root / options.directory.filename();
        if (!std::filesystem::create_directory(directory)) throw std::runtime_error("explicit scratch leaf already exists");
        nonReparseDirectoryComponents(directory);
        if (!samePreferredPath(directory, std::filesystem::canonical(directory)))
            throw std::runtime_error("explicit scratch leaf changed identity");
        return directory;
    }
    std::array<wchar_t, 32768> setting{};
    const DWORD n = GetEnvironmentVariableW(L"M2M_TEST_SCRATCH", setting.data(),
                                           static_cast<DWORD>(setting.size()));
    if (!n || n >= setting.size()) throw std::runtime_error("M2M_TEST_SCRATCH is required");
    const auto root = std::filesystem::canonical(std::filesystem::path(setting.data()));
    auto build = ownExecutable().parent_path();
    while (!build.empty() && build.filename() != L"build") {
        const auto parent = build.parent_path();
        if (parent == build) break;
        build = parent;
    }
    if (build.filename() != L"build") throw std::runtime_error("self-test image is outside build");
    auto inside = [](const std::filesystem::path& path, const std::filesystem::path& base) {
        auto at = path.begin();
        for (const auto& component : base) {
            if (at == path.end() || CompareStringOrdinal(at->c_str(), -1, component.c_str(), -1, TRUE) != CSTR_EQUAL)
                return false;
            ++at;
        }
        return at != path.end();
    };
    if (!inside(root, std::filesystem::canonical(build))) throw std::runtime_error("scratch is outside build");
    if (!options.directory.is_absolute() || options.directory.filename().empty() ||
        options.directory.filename() == L"." || options.directory.filename() == L"..")
        throw std::runtime_error("new absolute scratch leaf required");
    const auto directory = std::filesystem::canonical(options.directory.parent_path()) / options.directory.filename();
    if (!inside(directory, root) || directory == root) throw std::runtime_error("scratch leaf escapes root");
    if (!std::filesystem::create_directory(directory)) throw std::runtime_error("scratch leaf already exists");
    return directory;
}
template<class Integer> bool number(const char* text, Integer& out)
{
    const std::string_view view(text);
    const auto parsed = std::from_chars(view.data(), view.data() + view.size(), out);
    return parsed.ec == std::errc{} && parsed.ptr == view.data() + view.size() && !view.empty();
}
bool sendBytes(HANDLE pipe, std::span<const std::uint8_t> bytes)
{
    DWORD written = 0;
    return WriteFile(pipe, bytes.data(), static_cast<DWORD>(bytes.size()), &written, nullptr) &&
           written == bytes.size();
}
bool send(HANDLE pipe, const Frame& frame) { return sendBytes(pipe, encode(frame)); }
// A returned backend enters an explicit stopped worker state, never Save or unwinding.
[[noreturn]] void stoppedWorker()
{
    for (;;) Sleep(INFINITE);
}
void backendThatReturns() {} // adverse self-test; deliberately has no noreturn annotation
[[noreturn]] void fixedSelfTestEndpoint(HANDLE pipe, std::uint64_t nonce)
{
    TerminateProcess(GetCurrentProcess(), terminalStatus);
    // Unexpected OS/backend return cannot become ordinary completion or terminal success.
    (void)send(pipe, {Kind::BackendFailure, nonce, selfTestSite, 0, 2, 0});
    stoppedWorker();
}
struct UnwindProbe {
    ~UnwindProbe() {
        Handle marker(CreateFileW(L"unwound.marker", GENERIC_WRITE, FILE_SHARE_READ, nullptr,
                                  CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr));
        if (marker) {
            const char text[] = "self-test helper scope destroyed\n";
            DWORD wrote = 0;
            WriteFile(marker.value, text, static_cast<DWORD>(sizeof(text) - 1), &wrote, nullptr);
        }
    }
};
}

bool validWorkerSelection(WorkerRole role, ProtocolProfile profile, ChildCase mode) noexcept
{
    const auto code = static_cast<unsigned>(mode);
    if (role == WorkerRole::ProductIsolatedSave)
        return profile == ProtocolProfile::RemeshV2 && mode == ChildCase::ProductIsolated;
    if (role == WorkerRole::RemeshConversion)
        return profile == ProtocolProfile::RemeshV2 &&
               code >= static_cast<unsigned>(ChildCase::ConversionNormal) &&
               code <= static_cast<unsigned>(ChildCase::ConversionEndpointReturned);
    if (role != WorkerRole::SelfTest || code > static_cast<unsigned>(ChildCase::PreparedFileProbe)) return false;
    if (profile == ProtocolProfile::SafetyV1) return mode != ChildCase::PreparedFileProbe;
    if (profile != ProtocolProfile::RemeshV2) return false;
    return mode != ChildCase::Terminal && mode != ChildCase::TerminalWithoutBoundary &&
           mode != ChildCase::OutputFlood && mode != ChildCase::SmallOutput;
}

std::filesystem::path ownExecutable()
{
    std::array<wchar_t, 32768> path{};
    const DWORD size = GetModuleFileNameW(nullptr, path.data(), static_cast<DWORD>(path.size()));
    if (!size || size >= path.size()) throw std::runtime_error("cannot resolve self-test image");
    return std::filesystem::path(std::wstring(path.data(), size));
}
std::string executableSha256(const std::filesystem::path& path)
{
    Handle file(CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL, nullptr));
    require(static_cast<bool>(file), "hash image open");
    const auto size = fileSize(file.value);
    if (!size || size > 64 * 1024 * 1024) throw std::runtime_error("image hash size exceeds bound");
    std::vector<unsigned char> bytes(static_cast<std::size_t>(size));
    std::size_t at = 0;
    while (at != bytes.size()) {
        const DWORD count = static_cast<DWORD>((std::min)(bytes.size() - at, std::size_t{65536}));
        DWORD got = 0;
        require(ReadFile(file.value, bytes.data() + at, count, &got, nullptr) != FALSE && got != 0,
                "hash image read");
        at += got;
    }
    std::array<unsigned char, 32> digest{};
    if (BCryptHash(BCRYPT_SHA256_ALG_HANDLE, nullptr, 0, bytes.data(), static_cast<ULONG>(bytes.size()),
                   digest.data(), static_cast<ULONG>(digest.size())) != 0)
        throw std::runtime_error("image SHA-256 failed");
    constexpr char hex[] = "0123456789abcdef";
    std::string result;
    for (auto byte : digest) { result += hex[byte >> 4]; result += hex[byte & 15]; }
    return result;
}

Observation supervise(const Options& requested)
{
    // A preparation callback cannot change the selected image/role/root via its caller's Options.
    const Options options = requested;
    Observation result;
    const auto began = GetTickCount64();
    std::unique_ptr<PreparedCase> prepared; // outlives all child/job handles, including exception cleanup
    Handle process, thread, job, inputRead, protocolRead, protocolWrite, output, error;
    bool assigned = false;
    std::unique_ptr<Collector> collector;
    std::unique_ptr<CollectorV2> collectorV2;
    auto transcript = [&]() -> const Transcript& {
        return collectorV2 ? collectorV2->transcript() : collector->transcript();
    };
    auto feed = [&](std::span<const std::uint8_t> bytes) {
        return collectorV2 ? collectorV2->feed(bytes) : collector->feed(bytes);
    };
    auto finish = [&] { if (collectorV2) collectorV2->finish(); else collector->finish(); };
    auto stop = [&](StopReason reason, const std::string& diagnostic) {
        if (result.stopReason != StopReason::None) return;
        result.stopReason = reason;
        result.diagnostic = diagnostic;
    };
    try {
        if (!options.allowOwnedWorkerTermination) throw std::runtime_error("explicit self-test opt-in required");
        if (!validWorkerSelection(options.workerRole, options.protocolProfile, options.childCase))
            throw std::runtime_error("unsupported worker role/profile/case");
        if (!options.explicitScratchRoot.empty() && options.workerRole != WorkerRole::ProductIsolatedSave)
            throw std::runtime_error("explicit scratch root requires product worker role");
        if (!options.executable.is_absolute() || !std::filesystem::equivalent(options.executable, ownExecutable()))
            throw std::runtime_error("worker must be this self-test image");
        if (options.deadlineMs < 50 || options.deadlineMs > 20000 ||
            options.captureLimitBytes == 0 || options.captureLimitBytes > 1024 * 1024)
            throw std::runtime_error("self-test bounds invalid");
        if (options.protocolProfile != ProtocolProfile::SafetyV1 && options.protocolProfile != ProtocolProfile::RemeshV2)
            throw std::runtime_error("explicit protocol profile required");
        result.transcript.profile = options.protocolProfile;
        result.sha256Before = executableSha256(options.executable);
        if (result.sha256Before != options.expectedSha256) throw std::runtime_error("worker SHA-256 mismatch");
        const auto directory = scratchDirectory(options);
        if (options.prepareOwnedCase) {
            try {
                prepared = options.prepareOwnedCase(directory);
                if (!prepared) throw std::runtime_error("preparation returned no lease");
                result.casePrepared = true;
            } catch (const std::exception& e) {
                stop(StopReason::Preparation, e.what()); throw;
            }
        }
        std::uint64_t nonce = 0;
        if (BCryptGenRandom(nullptr, reinterpret_cast<PUCHAR>(&nonce), static_cast<ULONG>(sizeof(nonce)), BCRYPT_USE_SYSTEM_PREFERRED_RNG) != 0 || !nonce)
            throw std::runtime_error("nonce generation failed");
        if (prepared) {
            try { prepared->bindNonce(nonce); }
            catch (const std::exception& e) { stop(StopReason::Preparation, e.what()); throw; }
        }
        if (GetTickCount64() - began >= options.deadlineMs) {
            stop(StopReason::Timeout, "preparation exceeded child deadline");
            throw std::runtime_error("no child launched after preparation deadline");
        }
        SECURITY_ATTRIBUTES security{sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE};
        HANDLE read = nullptr, write = nullptr;
        require(CreatePipe(&read, &write, &security, 4096) != FALSE, "protocol pipe");
        protocolRead.reset(read); protocolWrite.reset(write);
        require(SetHandleInformation(protocolRead.value, HANDLE_FLAG_INHERIT, 0) != FALSE, "read noninherit");
        output.reset(CreateFileW((directory / "stdout.txt").c_str(), GENERIC_READ | GENERIC_WRITE,
                                FILE_SHARE_READ | FILE_SHARE_WRITE, &security, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr));
        error.reset(CreateFileW((directory / "stderr.txt").c_str(), GENERIC_READ | GENERIC_WRITE,
                               FILE_SHARE_READ | FILE_SHARE_WRITE, &security, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr));
        inputRead.reset(CreateFileW(L"NUL", GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, &security,
                                   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr));
        require(output && error && inputRead, "capture files/NUL");
        job.reset(CreateJobObjectW(nullptr, nullptr));
        require(static_cast<bool>(job), "owned job");
        JOBOBJECT_EXTENDED_LIMIT_INFORMATION limits{};
        limits.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
        require(SetInformationJobObject(job.value, JobObjectExtendedLimitInformation, &limits, sizeof(limits)) != FALSE,
                "job kill-on-close");
        SIZE_T attributeBytes = 0;
        InitializeProcThreadAttributeList(nullptr, 1, 0, &attributeBytes);
        std::vector<std::uint8_t> attributes(attributeBytes);
        STARTUPINFOEXW startup{};
        startup.StartupInfo.cb = sizeof(startup);
        startup.lpAttributeList = reinterpret_cast<LPPROC_THREAD_ATTRIBUTE_LIST>(attributes.data());
        require(InitializeProcThreadAttributeList(startup.lpAttributeList, 1, 0, &attributeBytes) != FALSE, "attributes init");
        struct AttributeCleanup {
            LPPROC_THREAD_ATTRIBUTE_LIST pointer;
            ~AttributeCleanup() { DeleteProcThreadAttributeList(pointer); }
        } attributeCleanup{startup.lpAttributeList};
        HANDLE inherited[]{protocolWrite.value, output.value, error.value, inputRead.value};
        require(UpdateProcThreadAttribute(startup.lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_HANDLE_LIST,
                                           inherited, sizeof(inherited), nullptr, nullptr) != FALSE, "restricted handles");
        startup.StartupInfo.dwFlags = STARTF_USESTDHANDLES;
        startup.StartupInfo.hStdInput = inputRead.value;
        startup.StartupInfo.hStdOutput = output.value;
        startup.StartupInfo.hStdError = error.value;
        const auto role = options.workerRole == WorkerRole::ProductIsolatedSave ? L"--m2m-isolated-save-worker-v1" :
            (options.workerRole == WorkerRole::RemeshConversion ? L"--terminal-remesh-conversion-worker" :
             (options.protocolProfile == ProtocolProfile::SafetyV1 ? L"--terminal-harness-child" : L"--terminal-harness-remesh-v2-child"));
        std::wstring command = L"\"" + options.executable.wstring() + L"\" " + role +
            L" --allow-owned-worker-termination " +
            std::to_wstring(static_cast<unsigned>(options.childCase)) + L" " +
            std::to_wstring(reinterpret_cast<std::uintptr_t>(protocolWrite.value)) + L" " +
            std::to_wstring(nonce) + L" " + std::to_wstring(options.unrelatedHandle);
        PROCESS_INFORMATION created{};
        if (GetTickCount64() - began >= options.deadlineMs) {
            stop(StopReason::Timeout, "launch preparation exceeded child deadline");
            throw std::runtime_error("no child launched after preparation deadline");
        }
        require(CreateProcessW(options.executable.c_str(), command.data(), nullptr, nullptr, TRUE,
                               CREATE_SUSPENDED | CREATE_NO_WINDOW | EXTENDED_STARTUPINFO_PRESENT,
                               nullptr, directory.c_str(), &startup.StartupInfo, &created) != FALSE, "self-test launch");
        process.reset(created.hProcess); thread.reset(created.hThread);
        result.childCreated = true; result.childPid = created.dwProcessId;
        if (options.protocolProfile == ProtocolProfile::SafetyV1) collector = std::make_unique<Collector>(nonce, result.childPid);
        else collectorV2 = std::make_unique<CollectorV2>(nonce, result.childPid);
        if (options.failBeforeJobAssignment) throw std::runtime_error("requested pre-assignment failure");
        require(AssignProcessToJobObject(job.value, process.value) != FALSE, "job assignment");
        assigned = true;
        require(ResumeThread(thread.value) != static_cast<DWORD>(-1), "child resume");
        result.resumed = true;
        thread.reset(); protocolWrite.reset(); inputRead.reset();
        const auto deadline = began + options.deadlineMs;
        bool eof = false;
        for (;;) {
            if (GetTickCount64() >= deadline) { stop(StopReason::Timeout, "end-to-end child deadline"); break; }
            result.stdoutBytes = fileSize(output.value); result.stderrBytes = fileSize(error.value);
            if (result.stdoutBytes > options.captureLimitBytes || result.stderrBytes > options.captureLimitBytes) {
                stop(StopReason::CaptureLimit, "bounded capture exceeded"); break;
            }
            DWORD available = 0;
            if (!eof && !PeekNamedPipe(protocolRead.value, nullptr, 0, nullptr, &available, nullptr)) {
                if (GetLastError() == ERROR_BROKEN_PIPE) eof = true;
                else { stop(StopReason::Protocol, "protocol peek failed"); break; }
            }
            if (available) {
                // Single reader: read no more than already available bytes. No blocking drain/join.
                std::array<std::uint8_t, 256> bytes{};
                DWORD got = 0;
                const auto count = (std::min)(available, static_cast<DWORD>(bytes.size()));
                if (!ReadFile(protocolRead.value, bytes.data(), count, &got, nullptr) || !got) {
                    stop(StopReason::Protocol, "protocol available read failed"); break;
                }
                if (!feed(std::span(bytes.data(), got))) {
                    stop(StopReason::Protocol, transcript().diagnostic); break;
                }
                if (transcript().backendFailure) {
                    stop(StopReason::BackendFailure, "worker reported endpoint failure/return"); break;
                }
                continue; // deadline checked before every further bounded chunk
            }
            const bool exited = WaitForSingleObject(process.value, 0) == WAIT_OBJECT_0;
            if (eof || exited) {
                finish();
                if (transcript().invalid) { stop(StopReason::Protocol, transcript().diagnostic); break; }
            }
            if (exited) break;
            Sleep(2);
        }
    } catch (const std::exception& e) {
        stop(result.childCreated ? StopReason::LaunchFailure : StopReason::Identity, e.what());
    }
    if (process) {
        if (result.stopReason != StopReason::None && WaitForSingleObject(process.value, 0) != WAIT_OBJECT_0) {
            // EOF with no final V2 outcome is still a sticky protocol failure, but a child
            // returning at the same time can lose the race against this cleanup path.
            // Give only that EOF case a short chance to publish its natural exit status.
            // The wait remains inside the original deadline and keeps checking capture bounds.
            constexpr std::uint64_t eofExitGraceMs = 1000;
            if (result.stopReason == StopReason::Protocol && collectorV2 &&
                transcript().diagnostic == "v2 EOF without final outcome") {
                const auto now = GetTickCount64();
                const auto deadline = began + options.deadlineMs;
                const auto graceEnd = (std::min)(deadline, now + eofExitGraceMs);
                while (WaitForSingleObject(process.value, 0) != WAIT_OBJECT_0 &&
                       GetTickCount64() < graceEnd) {
                    try {
                        result.stdoutBytes = fileSize(output.value);
                        result.stderrBytes = fileSize(error.value);
                    } catch (const std::exception& e) {
                        // Keep the original protocol failure sticky, then continue into
                        // bounded owned-child cleanup rather than escaping supervise().
                        result.diagnostic += std::string("; EOF grace capture inspection failed: ") + e.what();
                        break;
                    }
                    if (result.stdoutBytes > options.captureLimitBytes ||
                        result.stderrBytes > options.captureLimitBytes)
                        break;
                    const auto tick = GetTickCount64();
                    if (tick >= graceEnd) break;
                    const auto remaining = graceEnd - tick;
                    WaitForSingleObject(process.value,
                        static_cast<DWORD>((std::min)(remaining, std::uint64_t{10})));
                }
            }
        }
        if (result.stopReason != StopReason::None && WaitForSingleObject(process.value, 0) != WAIT_OBJECT_0) {
            result.supervisorStopRequested = true;
            if (assigned) TerminateJobObject(job.value, supervisorStopStatus);
            else TerminateProcess(process.value, supervisorStopStatus);
        }
        // Bounded cleanup is separate from, and cannot clear, the sticky run failure.
        result.childStopped = WaitForSingleObject(process.value, 2000) == WAIT_OBJECT_0;
        if (result.childStopped) {
            DWORD status = 0;
            if (GetExitCodeProcess(process.value, &status)) result.childStatus = status;
            else stop(StopReason::LaunchFailure, "child status unavailable");
        } else stop(StopReason::LaunchFailure, "owned child cleanup not confirmed");
    }
    if (collector || collectorV2) result.transcript = transcript();
    try {
        if (!result.sha256Before.empty()) {
            result.sha256After = executableSha256(options.executable);
            if (result.sha256After != result.sha256Before) stop(StopReason::Identity, "worker image changed");
        }
        if (output) result.stdoutBytes = fileSize(output.value);
        if (error) result.stderrBytes = fileSize(error.value);
    } catch (const std::exception& e) { stop(StopReason::Identity, e.what()); }
    result.disposition = classify(result.transcript, result.childStopped, result.childStatus, result.stopReason);
    if (prepared && result.childStopped) {
        try {
            prepared->inspectStoppedCase(result);
            result.caseInspected = true;
        } catch (const std::exception& e) { stop(StopReason::Inspection, e.what()); }
    }
    if (prepared && result.childCreated && !result.childStopped) {
        // Catastrophic OS stop failure: never release parent denial handles to an unconfirmed live child.
        // The job still closes on return; lease OS resources stay held until parent process teardown.
        (void)prepared.release();
        result.preparationHeldUntilParentExit = true;
    }
    result.elapsedMs = GetTickCount64() - began;
    result.disposition = classify(result.transcript, result.childStopped, result.childStatus, result.stopReason);
    return result;
}

int childMain(int argc, char** argv)
{
    unsigned rawCase = 0;
    std::uintptr_t pipeValue = 0, unrelated = 0;
    std::uint64_t nonce = 0;
    if (argc != 7 ||
        (std::string_view(argv[1]) != "--terminal-harness-child" &&
         std::string_view(argv[1]) != "--terminal-harness-remesh-v2-child") ||
        std::string_view(argv[2]) != "--allow-owned-worker-termination" ||
        !number(argv[3], rawCase) || rawCase > static_cast<unsigned>(ChildCase::PreparedFileProbe) ||
        !number(argv[4], pipeValue) || !number(argv[5], nonce) || !number(argv[6], unrelated) || !nonce)
        return 65;
    HANDLE pipe = reinterpret_cast<HANDLE>(pipeValue);
    if (GetFileType(pipe) != FILE_TYPE_PIPE) return 65;
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);
    const auto mode = static_cast<ChildCase>(rawCase);
    if (std::string_view(argv[1]) == "--terminal-harness-remesh-v2-child") {
        // V2 protocol/control fixtures only; no remesh source dispatch or v2 terminal endpoint.
        if (mode == ChildCase::Terminal || mode == ChildCase::TerminalWithoutBoundary ||
            mode == ChildCase::OutputFlood || mode == ChildCase::SmallOutput) return 65;
        UnwindProbe probe;
        auto publish = [&](const FrameV2& frame) { return sendBytes(pipe, encodeV2(frame)); };
        if (!publish({Kind::Started, nonce, remeshSite, 0, 0, GetCurrentProcessId()})) stoppedWorker();
        FrameV2 fixture{Kind::Boundary, nonce, remeshSite, terminalStatus, 22, 1, 1, 1,
            {snapshotPresent | snapshotOpened | snapshotValidated | snapshotProjectAttempted |
             snapshotProjectReturned | snapshotProjectSucceeded | snapshotMstAttempted |
             snapshotMstReturned | snapshotMstSucceeded, 1, 0x10000, 0}};
        auto bytes = encodeV2(fixture);
        if (mode == ChildCase::HalfFrameStall || mode == ChildCase::TruncatedExit) {
            (void)sendBytes(pipe, std::span(bytes.data(), frameV2Bytes / 2));
            if (mode == ChildCase::HalfFrameStall) stoppedWorker();
            return 0;
        }
        if (mode == ChildCase::BadChecksum) { bytes[60] ^= 1; (void)sendBytes(pipe, bytes); stoppedWorker(); }
        if (mode == ChildCase::StaleNonce) { fixture.nonce ^= 1; (void)publish(fixture); stoppedWorker(); }
        if (mode == ChildCase::ProtocolFlood) for (;;) if (!sendBytes(pipe, bytes)) stoppedWorker();
        if (mode == ChildCase::PublicationFailure) { CloseHandle(pipe); (void)sendBytes(pipe, bytes); stoppedWorker(); }
        if (mode == ChildCase::BoundaryStall || mode == ChildCase::BackendReturned ||
            mode == ChildCase::BoundaryNormalExit || mode == ChildCase::ConflictingReturn) {
            if (!sendBytes(pipe, bytes)) stoppedWorker();
            if (mode == ChildCase::BoundaryStall) stoppedWorker();
            if (mode == ChildCase::BackendReturned) {
                (void)publish({Kind::BackendFailure, nonce, remeshSite, 0,
                              static_cast<std::uint32_t>(FailureTag::EndpointReturned)});
                stoppedWorker();
            }
            if (mode == ChildCase::BoundaryNormalExit) return 0;
        }
        if (mode == ChildCase::InheritanceProbe && unrelated) SetEvent(reinterpret_cast<HANDLE>(unrelated));
        if (mode == ChildCase::PreparedFileProbe) {
            Handle input(CreateFileW(L"prepared.dat", GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
                                     nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr));
            std::array<char, 4> contents{};
            DWORD count = 0;
            if (!input || !ReadFile(input.value, contents.data(), static_cast<DWORD>(contents.size()), &count, nullptr) ||
                count != 4 || std::string_view(contents.data(), contents.size()) != "KEEP") return 66;
            Handle denied(CreateFileW(L"prepared.dat", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
                                      nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr));
            if (denied || GetLastError() != ERROR_SHARING_VIOLATION) return 66;
        }
        if (!publish({Kind::Returned, nonce})) stoppedWorker();
        return 0;
    }
    if (mode == ChildCase::PreparedFileProbe) return 65; // not a v1 case
    UnwindProbe probe;
    if (!send(pipe, {Kind::Started, nonce, selfTestSite, 0, 0, GetCurrentProcessId()})) stoppedWorker();
    auto boundary = encode({Kind::Boundary, nonce, selfTestSite, terminalStatus, 0, 1});
    if (mode == ChildCase::HalfFrameStall || mode == ChildCase::TruncatedExit) {
        (void)sendBytes(pipe, std::span(boundary.data(), frameBytes / 2));
        if (mode == ChildCase::HalfFrameStall) stoppedWorker();
        return 0;
    }
    if (mode == ChildCase::BadChecksum) {
        boundary[36] ^= 1; (void)sendBytes(pipe, boundary); stoppedWorker();
    }
    if (mode == ChildCase::StaleNonce) {
        (void)send(pipe, {Kind::Boundary, nonce ^ 1, selfTestSite, terminalStatus, 0, 1}); stoppedWorker();
    }
    if (mode == ChildCase::ProtocolFlood) {
        for (;;) if (!sendBytes(pipe, boundary)) stoppedWorker();
    }
    if (mode == ChildCase::OutputFlood || mode == ChildCase::SmallOutput) {
        std::array<char, 4096> bytes{};
        const unsigned blocks = mode == ChildCase::OutputFlood ? 128 : 8;
        for (unsigned i = 0; i < blocks; ++i) {
            DWORD wrote = 0;
            WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), bytes.data(), static_cast<DWORD>(bytes.size()), &wrote, nullptr);
            WriteFile(GetStdHandle(STD_ERROR_HANDLE), bytes.data(), static_cast<DWORD>(bytes.size()), &wrote, nullptr);
        }
    }
    if (mode == ChildCase::PublicationFailure) {
        CloseHandle(pipe); // actual failed write, then stopped worker rather than normal return
        (void)sendBytes(pipe, boundary); stoppedWorker();
    }
    if (mode == ChildCase::TerminalWithoutBoundary) fixedSelfTestEndpoint(pipe, nonce);
    if (mode == ChildCase::Terminal || mode == ChildCase::BackendReturned ||
        mode == ChildCase::BoundaryStall || mode == ChildCase::BoundaryNormalExit ||
        mode == ChildCase::ConflictingReturn) {
        if (!sendBytes(pipe, boundary)) stoppedWorker();
        if (mode == ChildCase::Terminal) fixedSelfTestEndpoint(pipe, nonce);
        if (mode == ChildCase::BoundaryStall) stoppedWorker();
        if (mode == ChildCase::BackendReturned) {
            backendThatReturns();
            (void)send(pipe, {Kind::BackendFailure, nonce, selfTestSite, 0, 1, 0}); stoppedWorker();
        }
        if (mode == ChildCase::BoundaryNormalExit) return 0;
    }
    if (mode == ChildCase::InheritanceProbe && unrelated) SetEvent(reinterpret_cast<HANDLE>(unrelated));
    if (!send(pipe, {Kind::Returned, nonce})) stoppedWorker();
    return 0;
}
} // namespace m2m::engine::terminal_harness
