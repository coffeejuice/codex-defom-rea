// ctest-labels: m2m-s terminal-worker retained-oracle
// Opt-in owned child only. Constructed remesh overlay; no original execution/native Close claim.
#include <testkit/check.hpp>
#include "../support/test_paths.hpp"
#include "../../src/core/conversion_call_services.hpp"
#include "../../src/core/remesh_terminal_call_service.hpp"
#include "../../src/engine/terminal_harness_supervisor.hpp"
#include "../../src/engine/remesh_invalid_parameter_model.hpp"
#include "../../src/moproj/project_output_device.hpp"
#include "m2m/mst/file_device.hpp"
#include <array>
#include <atomic>
#include <bcrypt.h>
#include <cerrno>
#include <charconv>
#include <clocale>
#include <cstdio>
#include <fstream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace fs = std::filesystem;
namespace th = m2m::engine::terminal_harness;
namespace terminal = m2m::engine::simcontrol300::detail;
using m2m::engine::SaveCallState;

namespace {
constexpr std::string_view role = "--terminal-remesh-conversion-worker";
constexpr std::string_view optIn = "--allow-owned-worker-termination";
constexpr std::string_view primarySeed = "DONE\n";
constexpr std::string_view fallbackSeed = "FILTER_DISTANCE 7\nDONE\n";
constexpr std::string_view mstSentinel = "owned late MST writer-denial sentinel\n";
constexpr std::string_view qualification =
    "constructed-remesh-overlay;retained-B133-raw-prefix;candidate-fixed-endpoint;"
    "native-default-handler-prelude-unimplemented;no-native-Close-claim";
const std::array<fs::path, 5> outputs{
    "COG_P1_B1.MST", "Objects/Object00002.KEY", "Operations/Task00002/Boolean.BWZ",
    "Operations/Task00002/ObjProperties00004.KEY", "Operations/Task00002/SimCtrl.KEY"};
const std::array<std::string_view, 5> outputHashes{
    "9bad363c4454ed9339129c6845ee82ebc044565e9165bcdb846f4fc35ed75ef1",
    "7adeee1681d831de2c49c972735ee9d76d102fe13d2b5a0917bafe33402f94e1",
    "0d696c7c8b4ed38bd4b0a98dc2e24f5bf1bd68059a902c1ccc3387c99eb00ca8",
    "2dc78ce7840dc99712a35dd66e6ee132324960dd97c15902276fcf6fc6fa0f4a",
    "6f0dc482e89e19de938530257297c098fc239abfaacb4ba1344f1d530897fe15"};

void need(bool value, const char* message)
{ if (!value) throw std::runtime_error(message); }

void write(const fs::path& path, std::string_view bytes)
{
    fs::create_directories(path.parent_path());
    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    need(out.is_open(), "cannot open worker diagnostic/output");
    out.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
    out.close(); need(!out.fail(), "cannot finish worker diagnostic/output");
}

std::string sha(std::string_view bytes)
{
    need(bytes.size() <= 64 * 1024 * 1024, "raw hash exceeds file bound");
    std::array<unsigned char, 32> digest{};
    const auto status = BCryptHash(BCRYPT_SHA256_ALG_HANDLE, nullptr, 0,
        reinterpret_cast<PUCHAR>(const_cast<char*>(bytes.data())), static_cast<ULONG>(bytes.size()),
        digest.data(), static_cast<ULONG>(digest.size()));
    need(status == 0, "raw SHA-256 failed");
    constexpr char hex[] = "0123456789abcdef";
    std::string result;
    for (auto byte : digest) { result += hex[byte >> 4]; result += hex[byte & 15]; }
    return result;
}

struct Raw {
    bool regular = false, complete = false;
    std::string bytes, error;
};
Raw read(const fs::path& path)
{
    Raw result;
    try {
        std::error_code error;
        const auto status = fs::symlink_status(path, error);
        if (error && error != std::errc::no_such_file_or_directory) {
            result.error = "status: " + error.message(); return result;
        }
        result.regular = fs::is_regular_file(status);
        if (!result.regular) return result;
        std::ifstream input(path, std::ios::binary);
        if (!input) { result.error = "open failed"; return result; }
        std::array<char, 65536> buffer;
        for (;;) {
            input.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
            const auto count = input.gcount();
            if (count > 0) result.bytes.append(buffer.data(), static_cast<std::size_t>(count));
            if (result.bytes.size() > 64 * 1024 * 1024) { result.error = "file bound"; break; }
            if (input.bad()) { result.error = "read badbit"; break; }
            if (input.eof()) { result.complete = true; break; }
            if (input.fail()) { result.error = "read failbit"; break; }
        }
    } catch (const std::exception& e) { result.error = e.what(); }
    return result;
}
std::string required(const fs::path& path)
{
    auto value = read(path);
    need(value.regular && value.complete && value.error.empty(), "required fixture/job read failed");
    return value.bytes;
}
using Manifest = std::map<std::string, Raw>;
struct Tree { Manifest files; std::string error; };
Tree collect(const fs::path& directory)
{
    Tree result;
    try {
        std::size_t total = 0;
        for (const auto& entry : fs::recursive_directory_iterator(directory)) {
            const auto attrs = GetFileAttributesW(entry.path().c_str());
            need(attrs != INVALID_FILE_ATTRIBUTES && !(attrs & FILE_ATTRIBUTE_REPARSE_POINT),
                 "fixture reparse point or unavailable attributes");
            need(entry.path().u16string().size() < 100, "DEFORM project path reaches 100 characters");
            if (entry.is_directory()) continue;
            need(result.files.size() < 512, "fixture file-count bound");
            auto raw = read(entry.path());
            total += raw.bytes.size(); need(total <= 128 * 1024 * 1024, "fixture byte bound");
            result.files.emplace(entry.path().lexically_relative(directory).generic_string(), std::move(raw));
        }
    } catch (const std::exception& e) { result.error = e.what(); }
    return result;
}
bool valid(const Tree& tree)
{
    if (!tree.error.empty()) return false;
    for (const auto& [path, raw] : tree.files)
        if (!raw.regular || !raw.complete || !raw.error.empty()) return false;
    return true;
}
void retainManifest(const fs::path& directory, const char* name, const Tree& tree)
{
    std::ostringstream out;
    out << "tree_error=" << tree.error << '\n';
    for (const auto& [path, raw] : tree.files)
        out << path << '\t' << raw.bytes.size() << '\t'
            << (raw.complete && raw.error.empty() ? sha(raw.bytes) : "INCOMPLETE")
            << "\tregular=" << raw.regular << "\tcomplete=" << raw.complete
            << "\terror=" << raw.error << '\n';
    write(directory / name, out.str());
}
void retainRaw(const fs::path& directory, const std::string& relative, const Raw* raw)
{
    const auto path = directory / fs::path(relative);
    if (!raw || !raw->regular) {
        write(fs::path(path.native() + L".missing"), "No regular file\n" + (raw ? raw->error : ""));
        return;
    }
    write(path, raw->bytes); // Available bytes are retained even for partial/error reads.
    if (!raw->complete || !raw->error.empty())
        write(fs::path(path.native() + L".error"), raw->error + "\ncomplete=" + std::to_string(raw->complete));
}

th::SaveSnapshot snapshot(const SaveCallState& call) noexcept
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
th::SaveSnapshot expectedSnapshot(th::ChildCase mode)
{
    auto flags = th::snapshotMask & ~th::snapshotModalPending;
    if (mode == th::ChildCase::ConversionDeniedProjectFalse) flags &= ~th::snapshotProjectSucceeded;
    if (mode == th::ChildCase::ConversionDeniedMstFalse) flags &= ~th::snapshotMstSucceeded;
    return {flags, 1,
        static_cast<std::uint32_t>(m2m::engine::DialogAnswer::No),
        static_cast<std::uint32_t>(m2m::engine::DialogAnswer::NoButton)};
}
bool send(HANDLE pipe, const th::FrameV2& frame) noexcept
{
    const auto bytes = th::encodeV2(frame); DWORD written = 0;
    return WriteFile(pipe, bytes.data(), static_cast<DWORD>(bytes.size()), &written, nullptr) &&
        written == bytes.size();
}
[[noreturn]] void park() noexcept { for (;;) Sleep(1000); }
bool backendFailureCase(th::ChildCase mode)
{
    return mode == th::ChildCase::ConversionPublicationFailure || mode == th::ChildCase::ConversionEndpointReturned;
}
bool processTerminalCase(th::ChildCase mode)
{
    return mode == th::ChildCase::ConversionDenied || mode == th::ChildCase::ConversionDeniedProjectFalse ||
        mode == th::ChildCase::ConversionDeniedMstFalse;
}
const char* handlerModelQualificationName(terminal::InvalidParameterQualification value) noexcept
{
    switch (value) {
    case terminal::InvalidParameterQualification::SourceModelOnly: return "SourceModelOnly";
    case terminal::InvalidParameterQualification::StaticDefaultConditional: return "StaticDefaultConditional";
    case terminal::InvalidParameterQualification::DecodedValueWitnessOnly: return "DecodedValueWitnessOnly";
    }
    return "Invalid";
}
std::string ledgerText(const char* event, const th::SaveSnapshot& save, unsigned failures)
{
    std::ostringstream text;
    text << "event=" << event << "\nprofile_reads=1\ndispatches=1\nfailures=" << failures
         << "\nsave_flags=" << save.flags << "\nchecked_subprojects=" << save.checkedSubprojects
         << "\nanswer=" << save.answer << "\npending_default=" << save.pendingDefault << '\n'
         << "handler_model_evidence=StaticFreshDefaultAssumption\n"
            "handler_model_stage=DefaultPrefixComplete\n"
            "handler_model_next=CaptureContextUnimplemented\n"
            "handler_model_qualification=StaticDefaultConditional\n"
            "handler_model_status_present=1\n"
            "handler_model_predicted_status=3221226519\n"
            "handler_model_debug_store=modeled-only\n";
    return text.str();
}

// Observes this child stack only; it does not stand in for native Close.
class UnwindMarker final {
    fs::path path_;
public:
    explicit UnwindMarker(const fs::path& directory) : path_(directory / "child-stack-unwound.txt") {}
    ~UnwindMarker() noexcept
    {
        HANDLE file = CreateFileW(path_.c_str(), GENERIC_WRITE, FILE_SHARE_READ, nullptr,
                                  CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (file != INVALID_HANDLE_VALUE) {
            constexpr char message[] = "candidate conversion child stack unwound;not-native-Close\n";
            DWORD written = 0;
            (void)WriteFile(file, message, static_cast<DWORD>(sizeof(message) - 1), &written, nullptr);
            CloseHandle(file);
        }
    }
};

class ProjectFailureFactory;
class ProjectFailureDevice final : public m2m::moproj::detail::ProjectOutputDevice {
    std::unique_ptr<m2m::mst::FileDevice> file_;
    ProjectFailureFactory& owner_;
public:
    ProjectFailureDevice(std::unique_ptr<m2m::mst::FileDevice>, ProjectFailureFactory&);
    ~ProjectFailureDevice() override;
    std::int64_t write(std::string_view bytes) override { return file_->write(bytes); }
    bool flush() override { return file_->flush(); }
    void close() override;
    bool opened() const override { return file_->opened(); }
    int error() const override;
};
class ProjectFailureFactory final : public m2m::moproj::detail::ProjectOutputFactory {
    fs::path root_, diagnostic_;
    th::ChildCase mode_;
    HANDLE denyMst_ = INVALID_HANDLE_VALUE;
public:
    unsigned opened = 0, aboutToClose = 0, closed = 0, errors = 0;
    ProjectFailureFactory(const fs::path& root, const fs::path& diagnostic, th::ChildCase mode)
        : root_(root), diagnostic_(diagnostic), mode_(mode) {}
    ~ProjectFailureFactory() override { if (denyMst_ != INVALID_HANDLE_VALUE) CloseHandle(denyMst_); }
    void beforeClose()
    {
        ++aboutToClose;
        need(aboutToClose == 1, "project aboutToClose replayed");
        if (mode_ == th::ChildCase::ConversionDeniedMstFalse) {
            const auto mst = root_ / "COG_P1_B1.MST";
            need(mst.parent_path() == root_ && mst.u16string().size() < 100, "late MST path escaped short project");
            write(mst, mstSentinel); // After actual Open/Save2; before main MST open.
            denyMst_ = CreateFileW(mst.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr,
                                   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
            need(denyMst_ != INVALID_HANDLE_VALUE, "cannot acquire late MST writer denial");
        }
        write(diagnostic_ / "project-before-close.txt", "opened=" + std::to_string(opened) +
            "\nabout_to_close=" + std::to_string(aboutToClose) + "\nlate_mst_lock=" +
            std::to_string(denyMst_ != INVALID_HANDLE_VALUE) + "\n");
    }
    int sampledError(int nativeError)
    {
        ++errors;
        need(closed == 1 && errors == 1, "project status sampled before returned close or replayed");
        need(nativeError == 0, "unexpected real project writer error outside declared fixture");
        const int result = mode_ == th::ChildCase::ConversionDeniedProjectFalse ? 7 : 0;
        write(diagnostic_ / "project-after-close.txt", "opened=" + std::to_string(opened) +
            "\nabout_to_close=" + std::to_string(aboutToClose) + "\nclosed=" + std::to_string(closed) +
            "\nerror_queries=" + std::to_string(errors) + "\nnative_error=" + std::to_string(nativeError) +
            "\nerror=" + std::to_string(result) + "\n");
        return result;
    }
    std::unique_ptr<m2m::moproj::detail::ProjectOutputDevice> open(const m2m::qt::String& path) override
    {
        ++opened; need(opened == 1, "project writer open replayed");
        auto file = std::make_unique<m2m::mst::FileDevice>(path);
        need(file->opened(), "real project writer failed before declared late error");
        return std::make_unique<ProjectFailureDevice>(std::move(file), *this);
    }
};
ProjectFailureDevice::ProjectFailureDevice(std::unique_ptr<m2m::mst::FileDevice> file, ProjectFailureFactory& owner)
    : file_(std::move(file)), owner_(owner)
{
    file_->setAboutToClose([this] { owner_.beforeClose(); notifyAboutToClose(); });
}
ProjectFailureDevice::~ProjectFailureDevice()
{
    if (file_->opened()) close();
    file_->setAboutToClose({});
}
void ProjectFailureDevice::close()
{
    if (!file_->opened()) return;
    file_->close(); ++owner_.closed;
}
int ProjectFailureDevice::error() const { return owner_.sampledError(file_->error()); }

class Backend final : public m2m::detail::RemeshTerminalCallService {
    HANDLE pipe_;
    HANDLE ledger_ = INVALID_HANDLE_VALUE;
    std::uint64_t nonce_;
    th::ChildCase mode_;
    terminal::RemeshTerminalProfile selected_;
    std::optional<terminal::RemeshInvalidParameterModel> handlerModel_;
    bool writeLedger(const char* event, const SaveCallState& call) noexcept
    {
        std::array<char, 768> text{};
        const auto save = snapshot(call);
        const auto model = handlerModel_.value_or(
            terminal::modelFirstNullFilePrintf(terminal::InvalidParameterEvidence::Unknown));
        const int count = std::snprintf(text.data(), text.size(),
            "event=%s\nprofile_reads=%u\ndispatches=%u\nfailures=%u\nsave_flags=%u\n"
            "checked_subprojects=%u\nanswer=%u\npending_default=%u\n"
            "handler_model_evidence=%s\nhandler_model_stage=%s\nhandler_model_next=%s\n"
            "handler_model_qualification=%s\nhandler_model_status_present=%u\n"
            "handler_model_predicted_status=%u\nhandler_model_debug_store=%s\n",
            event, profileReads, dispatches, failures, save.flags, save.checkedSubprojects, save.answer, save.pendingDefault,
            terminal::invalidParameterEvidenceName(model.evidence), terminal::invalidParameterStageName(model.stage),
            terminal::invalidParameterNextName(model.next), handlerModelQualificationName(model.qualification),
            model.predictedProcessStatus.has_value() ? 1U : 0U,
            model.predictedProcessStatus.value_or(0), model.modeledDebugStore ? "modeled-only" : "none");
        DWORD written = 0;
        return count > 0 && static_cast<std::size_t>(count) < text.size() &&
            WriteFile(ledger_, text.data(), static_cast<DWORD>(count), &written, nullptr) &&
            written == static_cast<DWORD>(count);
    }
public:
    mutable unsigned profileReads = 0;
    unsigned dispatches = 0, failures = 0;
    Backend(HANDLE pipe, std::uint64_t nonce, th::ChildCase mode, const fs::path& diagnostic)
        : pipe_(pipe), nonce_(nonce), mode_(mode), selected_(mode == th::ChildCase::ConversionUnknown
            ? terminal::RemeshTerminalProfile::Unknown : mode == th::ChildCase::ConversionCustom
            ? terminal::RemeshTerminalProfile::Custom : terminal::RemeshTerminalProfile::FreshDefault)
    {
        ledger_ = CreateFileW((diagnostic / "terminal-backend.txt").c_str(), GENERIC_WRITE,
            FILE_SHARE_READ, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
        need(ledger_ != INVALID_HANDLE_VALUE, "cannot prepare private terminal backend ledger");
    }
    ~Backend() override { if (ledger_ != INVALID_HANDLE_VALUE) CloseHandle(ledger_); }
    terminal::RemeshTerminalProfile profile() const noexcept override
    { ++profileReads; return selected_; }
    terminal::RemeshTerminalDispatchFailure dispatch(
        const terminal::RemeshTerminalRequest& request, const SaveCallState& call) noexcept override
    {
        ++dispatches;
        if (selected_ != terminal::RemeshTerminalProfile::FreshDefault ||
            request.site != terminal::RemeshTerminalSite::FirstNullFilePrintf ||
            request.proof != terminal::RemeshTerminalProof::NativeFreshDefaultStartup ||
            request.sourceAddress != 0x1400abcf0ULL || request.expectedProcessStatus != th::terminalStatus ||
            request.crtErrno != 22 || request.formatAttemptOrdinal != 1 || errno != 22)
            return terminal::RemeshTerminalDispatchFailure::InvalidFailureCode;
        handlerModel_ = terminal::modelFirstNullFilePrintf(
            terminal::InvalidParameterEvidence::StaticFreshDefaultAssumption);
        if (!writeLedger("dispatch", call)) return terminal::RemeshTerminalDispatchFailure::PublicationFailed;
        if (mode_ == th::ChildCase::ConversionPublicationFailure)
            return terminal::RemeshTerminalDispatchFailure::PublicationFailed; // Deliberately omit Boundary.
        if (!send(pipe_, {th::Kind::Boundary, nonce_, th::remeshSite,
                request.expectedProcessStatus, static_cast<std::uint32_t>(request.crtErrno),
                request.formatAttemptOrdinal, 1, 1, snapshot(call)}))
            return terminal::RemeshTerminalDispatchFailure::PublicationFailed;
        if (mode_ == th::ChildCase::ConversionEndpointReturned)
            return terminal::RemeshTerminalDispatchFailure::EndpointReturned; // Deliberately returning test endpoint.
        // Opted-in candidate endpoint only; the native handler prelude remains unimplemented.
        (void)TerminateProcess(GetCurrentProcess(), request.expectedProcessStatus);
        return terminal::RemeshTerminalDispatchFailure::EndpointReturned;
    }
    void failClosed(const terminal::RemeshTerminalRequest&,
                    terminal::RemeshTerminalDispatchFailure reason, const SaveCallState& call) noexcept override
    {
        ++failures;
        (void)writeLedger("failClosed", call);
        th::FailureTag tag = th::FailureTag::InvalidFailureCode;
        switch (reason) {
        case terminal::RemeshTerminalDispatchFailure::PublicationFailed: tag = th::FailureTag::PublicationFailed; break;
        case terminal::RemeshTerminalDispatchFailure::EndpointReturned: tag = th::FailureTag::EndpointReturned; break;
        case terminal::RemeshTerminalDispatchFailure::InvalidFailureCode: break;
        }
        (void)send(pipe_, {th::Kind::BackendFailure, nonce_, th::remeshSite, 0, static_cast<std::uint32_t>(tag)});
    }
};

template<class T> bool number(std::string_view text, T& out)
{
    const auto parsed = std::from_chars(text.data(), text.data() + text.size(), out);
    return !text.empty() && parsed.ec == std::errc{} && parsed.ptr == text.data() + text.size();
}
bool supportedCase(th::ChildCase mode)
{
    return th::validWorkerSelection(th::WorkerRole::RemeshConversion, th::ProtocolProfile::RemeshV2, mode);
}
bool shortRoot(const fs::path& root)
{
    if (!root.is_absolute() || !root.filename().string().starts_with("r108_") ||
        root.u16string().size() >= 100) return false;
    const auto base = fs::canonical(fs::path("C:/PROBLEM"));
    if (fs::canonical(root.parent_path()) != base || fs::canonical(root) != base / root.filename()) return false;
    const auto attrs = GetFileAttributesW(root.c_str());
    return attrs != INVALID_FILE_ATTRIBUTES && !(attrs & FILE_ATTRIBUTE_REPARSE_POINT);
}

int conversionChild(int argc, char** argv)
{
    unsigned rawCase = 0; std::uintptr_t rawPipe = 0, unrelated = 0; std::uint64_t nonce = 0;
    if (argc != 7 || std::string_view(argv[1]) != role || std::string_view(argv[2]) != optIn ||
        !number(argv[3], rawCase) || !number(argv[4], rawPipe) || !number(argv[5], nonce) ||
        !number(argv[6], unrelated) || !nonce || unrelated) return 65;
    const auto mode = static_cast<th::ChildCase>(rawCase);
    if (!supportedCase(mode) || !th::validWorkerSelection(th::WorkerRole::RemeshConversion,
            th::ProtocolProfile::RemeshV2, mode)) return 65;
    const HANDLE pipe = reinterpret_cast<HANDLE>(rawPipe);
    if (GetFileType(pipe) != FILE_TYPE_PIPE) return 65;
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);
    if (!send(pipe, {th::Kind::Started, nonce, th::remeshSite, 0, 0, GetCurrentProcessId()})) park();
    try {
        const auto diagnostic = fs::current_path();
        const auto job = diagnostic / "worker-job.txt";
        need(fs::file_size(job) <= 256, "worker job exceeds bound");
        std::istringstream input(required(job)); std::string schema, selected, projectRoot, extra;
        need(static_cast<bool>(std::getline(input, schema)) && schema == "remesh-worker-job-v1" &&
             static_cast<bool>(std::getline(input, selected)) && selected == std::to_string(rawCase) &&
             static_cast<bool>(std::getline(input, projectRoot)) && !std::getline(input, extra), "invalid worker job");
        const fs::path root(projectRoot);
        need(shortRoot(root), "worker project outside owned short leaf");
        need(std::setlocale(LC_NUMERIC, "C") != nullptr, "worker C numeric locale unavailable");
        UnwindMarker unwind(diagnostic);
        Backend backend(pipe, nonce, mode, diagnostic);
        std::unique_ptr<ProjectFailureFactory> projectOutput;
        if (mode == th::ChildCase::ConversionDeniedProjectFalse || mode == th::ChildCase::ConversionDeniedMstFalse)
            projectOutput = std::make_unique<ProjectFailureFactory>(root, diagnostic, mode);
        m2m::detail::ConversionCallServices services{}; services.remeshTerminal = &backend;
        services.projectOutput = projectOutput.get();
        m2m::ConversionDetails details;
        const auto result = m2m::detail::convertDetailedWithServices(
            (root / "COG_P1_B1.moproj").u16string(), u"", M2M_FLAG_WRITE_SAVE, 1252, details, services);
        // This marker is after actual adapter return/destruction, not native Close.
        std::ostringstream returned;
        returned << "qualification=" << qualification << "\ncode=" << result.code
                 << "\nmessage=" << result.message << "\nprofile_reads=" << backend.profileReads
                 << "\ndispatches=" << backend.dispatches << "\nfailures=" << backend.failures
                 << "\ndetails_returned=" << (details.returnedCode ? *details.returnedCode : -1) << '\n';
        write(diagnostic / "adapter-returned.txt", returned.str());
        need(details.saveCall.has_value(), "returned conversion has no Save state");
        if (!send(pipe, {th::Kind::Returned, nonce, th::remeshSite,
                static_cast<std::uint32_t>(result.code), 0, 0, 0, 0, snapshot(*details.saveCall)})) park();
        return 0;
    } catch (const std::exception& e) {
        std::fprintf(stderr, "remesh worker setup/observation failure: %s\n", e.what());
        return 66; // Missing final frame is harness failure, never a valid guarded return.
    }
}

struct CaseState {
    bool inspected = false, lockHeldAtInspection = false, cleaned = false;
    bool inputValid = false, afterValid = false, rawPrefixMatches = false;
    bool fallbackUnchanged = false, primaryExpected = false;
    std::string projectPath, cleanupError;
};
class Fixture final : public th::PreparedCase {
    fs::path diagnostic_, root_;
    th::ChildCase mode_;
    std::shared_ptr<CaseState> state_;
    Tree before_;
    HANDLE lock_ = INVALID_HANDLE_VALUE;
    bool owned_ = false;
    void cleanup() noexcept
    {
        if (lock_ != INVALID_HANDLE_VALUE) { CloseHandle(lock_); lock_ = INVALID_HANDLE_VALUE; }
        if (!owned_) return;
        try {
            need(shortRoot(root_), "refusing cleanup outside owned short project leaf");
            for (const auto& item : fs::recursive_directory_iterator(root_)) {
                const auto attrs = GetFileAttributesW(item.path().c_str());
                need(attrs != INVALID_FILE_ATTRIBUTES && !(attrs & FILE_ATTRIBUTE_REPARSE_POINT),
                     "refusing project cleanup containing reparse point");
            }
            fs::remove_all(root_); owned_ = false; state_->cleaned = !fs::exists(root_);
        } catch (const std::exception& e) { state_->cleanupError = e.what(); }
    }
public:
    Fixture(const fs::path& directory, th::ChildCase mode, std::shared_ptr<CaseState> state)
        : diagnostic_(directory), mode_(mode), state_(std::move(state))
    {
        try {
            const auto oracle = m2m_test::configuredPath("M2M_ORACLE_B");
            const auto input = oracle / "133_B_DBSTEP_BOOL3_before";
            need(sha(required(input / "COG_P1_B1.moproj")) ==
                "8e5e896cce67164430324cddc25324043449428b789d201bb3bef9c49cc89f79", "input XML identity");
            need(sha(required(input / "COG_P1_B1.DB")) ==
                "707326e90e4f8fb0fee2aa1fa9f174e8d6e40b213fe2a7c0a828be00829671df", "input DB identity");
            need(!fs::exists(input / "LOCAL_REMESH.DAT") && !fs::exists(input / "LOC_DEN_CTRL.DAT"), "overlay already present");
            for (std::size_t i = 0; i < outputs.size(); ++i) {
                auto expected = required(oracle / "133_B_DBSTEP_BOOL3" / outputs[i]);
                need(sha(expected) == outputHashes[i], "retained raw oracle identity");
                write(diagnostic_ / "expected" / outputs[i], expected);
            }
            fs::create_directories("C:/PROBLEM");
            static std::atomic<unsigned> serial{0};
            const auto base = fs::canonical(fs::path("C:/PROBLEM"));
            root_ = base / ("r108_" + std::to_string(GetCurrentProcessId()) + "_" +
                std::to_string(GetTickCount64()) + "_" + std::to_string(++serial));
            need((root_ / "Operations/Task00002/ObjProperties00004.KEY").u16string().size() < 100,
                 "planned DEFORM path reaches 100 characters");
            need(!fs::exists(root_) && fs::create_directory(root_), "cannot reserve own short project leaf");
            owned_ = true; state_->projectPath = root_.generic_string();
            // Validate every retained destination before recursive copy performs a write.
            for (const auto& item : fs::recursive_directory_iterator(input)) {
                const auto attrs = GetFileAttributesW(item.path().c_str());
                need(attrs != INVALID_FILE_ATTRIBUTES && !(attrs & FILE_ATTRIBUTE_REPARSE_POINT),
                     "retained fixture contains a reparse point");
                const auto relative = item.path().lexically_relative(input);
                need(!relative.empty() && !relative.is_absolute() && *relative.begin() != "..",
                     "retained fixture relative path escapes input");
                const auto destination = (root_ / relative).lexically_normal();
                const auto contained = destination.lexically_relative(root_);
                need(!contained.empty() && !contained.is_absolute() && *contained.begin() != ".." &&
                     destination.u16string().size() < 100, "retained destination outside short owned project");
            }
            fs::copy(input, root_, fs::copy_options::recursive);
            write(root_ / "LOCAL_REMESH.DAT", primarySeed);
            write(root_ / "LOC_DEN_CTRL.DAT", fallbackSeed);
            before_ = collect(root_);
            retainManifest(diagnostic_, "before-manifest.tsv", before_);
            state_->inputValid = valid(before_) && before_.files.size() == 66;
            need(state_->inputValid, "invalid copied overlay input manifest");
            std::string retainedManifest;
            for (const auto& [path, raw] : before_.files) {
                if (path == "LOCAL_REMESH.DAT" || path == "LOC_DEN_CTRL.DAT") continue;
                retainedManifest += sha(raw.bytes) + " " + std::to_string(raw.bytes.size()) + " " + path + "\n";
            }
            need(sha(retainedManifest) == "0ff2038095f238744a0d61a1cbf2b3da01f061ba4080f3a0dff08aa248e0c5e6",
                 "retained 64-file before manifest identity");
            if (mode_ != th::ChildCase::ConversionNormal) {
                lock_ = CreateFileW((root_ / "LOCAL_REMESH.DAT").c_str(), GENERIC_READ,
                    FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
                need(lock_ != INVALID_HANDLE_VALUE, "cannot acquire read-compatible writer-denial lock");
            }
            const auto job = "remesh-worker-job-v1\n" + std::to_string(static_cast<unsigned>(mode_)) +
                "\n" + root_.generic_string() + "\n";
            need(job.size() <= 256, "prepared worker job exceeds bound");
            write(diagnostic_ / "worker-job.txt", job);
        } catch (...) { cleanup(); throw; }
    }
    ~Fixture() override { cleanup(); }
    void inspectStoppedCase(const th::Observation& observed) override
    {
        state_->inspected = observed.childStopped;
        if (lock_ != INVALID_HANDLE_VALUE) {
            HANDLE denied = CreateFileW((root_ / "LOCAL_REMESH.DAT").c_str(), GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
            const auto error = GetLastError();
            state_->lockHeldAtInspection = denied == INVALID_HANDLE_VALUE && error == ERROR_SHARING_VIOLATION;
            if (denied != INVALID_HANDLE_VALUE) CloseHandle(denied);
        }
        const auto after = collect(root_);
        retainManifest(diagnostic_, "after-manifest.tsv", after);
        std::set<std::string> paths;
        for (const auto& [path, raw] : before_.files) paths.insert(path);
        for (const auto& [path, raw] : after.files) paths.insert(path);
        std::ostringstream differences;
        for (const auto& path : paths) {
            const auto b = before_.files.find(path);
            const auto a = after.files.find(path);
            const Raw* old = b == before_.files.end() ? nullptr : &b->second;
            const Raw* now = a == after.files.end() ? nullptr : &a->second;
            if (!old || !now || !old->complete || !now->complete || !old->error.empty() ||
                !now->error.empty() || old->bytes != now->bytes) {
                differences << path << '\n';
                retainRaw(diagnostic_ / "differences/before", path, old);
                retainRaw(diagnostic_ / "differences/after", path, now);
            }
        }
        write(diagnostic_ / "raw-differences.txt", differences.str());
        state_->afterValid = valid(after);
        state_->rawPrefixMatches = true;
        for (std::size_t i = 0; i < outputs.size(); ++i) {
            const auto a = after.files.find(outputs[i].generic_string());
            const Raw* actual = a == after.files.end() ? nullptr : &a->second;
            // Always retain available actual bytes/missing/errors before comparison assertions.
            retainRaw(diagnostic_ / "actual-prefix", outputs[i].generic_string(), actual);
            const auto expected = mode_ == th::ChildCase::ConversionDeniedMstFalse && i == 0
                ? std::string(mstSentinel) : required(diagnostic_ / "expected" / outputs[i]);
            write(diagnostic_ / "effective-expected-prefix" / outputs[i], expected);
            state_->rawPrefixMatches = state_->rawPrefixMatches && actual && actual->complete &&
                actual->error.empty() && actual->bytes == expected;
        }
        const auto fallback = after.files.find("LOC_DEN_CTRL.DAT"), primary = after.files.find("LOCAL_REMESH.DAT");
        state_->fallbackUnchanged = fallback != after.files.end() && fallback->second.bytes == fallbackSeed;
        if (primary != after.files.end()) {
            const auto& bytes = primary->second.bytes;
            state_->primaryExpected = mode_ == th::ChildCase::ConversionNormal
                ? bytes != primarySeed && bytes.find("KEYWORD_DRIVEN\r\n") != std::string::npos &&
                    bytes.find("FILTER_DISTANCE 7.000000\r\n") != std::string::npos && bytes.find("DONE") == std::string::npos
                : bytes == primarySeed;
        }
    }
};

const fs::path& runRoot()
{
    static const auto root = [] {
        const auto path = m2m_test::scratchOutput(m2m_test::scratchRoot() /
            ("terminal-remesh-" + std::to_string(GetCurrentProcessId()) + "-" + std::to_string(GetTickCount64())));
        need(!fs::exists(path) && fs::create_directory(path), "cannot reserve terminal remesh diagnostic root");
        return path;
    }();
    return root;
}
void run(th::ChildCase mode)
{
    const auto state = std::make_shared<CaseState>();
    th::Options options;
    options.allowOwnedWorkerTermination = true;
    options.workerRole = th::WorkerRole::RemeshConversion;
    options.protocolProfile = th::ProtocolProfile::RemeshV2;
    options.childCase = mode; options.deadlineMs = 15000;
    options.executable = th::ownExecutable(); options.expectedSha256 = th::executableSha256(options.executable);
    options.directory = runRoot() / ("case-" + std::to_string(static_cast<unsigned>(mode)));
    options.prepareOwnedCase = [mode, state](const fs::path& directory) {
        return std::make_unique<Fixture>(directory, mode, state);
    };
    const auto observed = th::supervise(options);
    std::ostringstream receipt;
    receipt << "qualification=" << qualification << "\ncase=" << static_cast<unsigned>(mode)
        << "\nproject=" << state->projectPath << "\nexpected_sha=" << options.expectedSha256
        << "\nsha_before=" << observed.sha256Before << "\nsha_after=" << observed.sha256After
        << "\ndisposition=" << th::name(observed.disposition) << "\nstop_reason=" << th::name(observed.stopReason)
        << "\nchild_pid=" << observed.childPid << "\nchild_status=" << observed.childStatus
        << "\ncreated=" << observed.childCreated << "\nresumed=" << observed.resumed
        << "\nstopped=" << observed.childStopped << "\nsupervisor_stop=" << observed.supervisorStopRequested
        << "\nprepared=" << observed.casePrepared << "\ninspected=" << observed.caseInspected
        << "\nlease_retained=" << observed.preparationHeldUntilParentExit << "\nelapsed_ms=" << observed.elapsedMs
        << "\nstarted=" << observed.transcript.started << "\nboundary=" << observed.transcript.boundary
        << "\nreturned=" << observed.transcript.returned << "\nbackend_failure=" << observed.transcript.backendFailure
        << "\nfailure_tag=" << observed.transcript.failureTag
        << "\ninvalid=" << observed.transcript.invalid << "\nframes=" << observed.transcript.frames
        << "\nreturned_code=" << observed.transcript.returnedCode << "\ncleanup_error=" << state->cleanupError
        << "\ndiagnostic=" << observed.diagnostic << "\nprotocol_diagnostic=" << observed.transcript.diagnostic << '\n';
    if (observed.transcript.save) {
        const auto& s = *observed.transcript.save;
        receipt << "save_flags=" << s.flags << "\nchecked_subprojects=" << s.checkedSubprojects
            << "\nanswer=" << s.answer << "\npending_default=" << s.pendingDefault << '\n';
    }
    if (fs::exists(options.directory)) write(options.directory / "receipt.txt", receipt.str());
    std::printf("Remesh worker raw/protocol evidence: %s\n", options.directory.string().c_str());
    REQUIRE(observed.childCreated && observed.resumed && observed.childStopped);
    CHECK(observed.casePrepared && observed.caseInspected && state->inspected);
    CHECK(!observed.preparationHeldUntilParentExit);
    const bool backendFailed = backendFailureCase(mode);
    CHECK(observed.stopReason == (backendFailed ? th::StopReason::BackendFailure : th::StopReason::None));
    CHECK(observed.supervisorStopRequested == backendFailed);
    CHECK(observed.sha256Before == options.expectedSha256 && observed.sha256After == options.expectedSha256);
    CHECK(state->cleaned && state->cleanupError.empty());
    CHECK(state->inputValid && state->afterValid && state->rawPrefixMatches);
    CHECK(state->fallbackUnchanged && state->primaryExpected);
    if (mode != th::ChildCase::ConversionNormal) CHECK(state->lockHeldAtInspection);
    REQUIRE(observed.transcript.started && !observed.transcript.invalid);
    CHECK(observed.transcript.profile == th::ProtocolProfile::RemeshV2);
    CHECK(observed.transcript.backendFailure == backendFailed);
    const bool terminal = processTerminalCase(mode);
    const bool returningEndpoint = mode == th::ChildCase::ConversionEndpointReturned;
    const bool publicationFailure = mode == th::ChildCase::ConversionPublicationFailure;
    const bool nonReturning = terminal || backendFailed;
    CHECK(observed.transcript.boundary == (terminal || returningEndpoint));
    CHECK(observed.transcript.returned == !nonReturning);
    CHECK_EQ(observed.transcript.frames, returningEndpoint ? 3u : 2u);
    if (publicationFailure) CHECK(!observed.transcript.save.has_value());
    else {
        REQUIRE(observed.transcript.save.has_value());
        CHECK(*observed.transcript.save == expectedSnapshot(mode));
    }
    CHECK(fs::exists(options.directory / "child-stack-unwound.txt") == !nonReturning);
    if (nonReturning) {
        CHECK(!fs::exists(options.directory / "adapter-returned.txt"));
        const auto first = ledgerText("dispatch", expectedSnapshot(mode), 0);
        CHECK(required(options.directory / "terminal-backend.txt") ==
            first + (backendFailed ? ledgerText("failClosed", expectedSnapshot(mode), 1) : std::string{}));
    }
    if (terminal) {
        CHECK(observed.disposition == th::Disposition::CandidateProcessTerminal);
        CHECK_EQ(observed.childStatus, th::terminalStatus);
        // No synthetic Closed frame exists, and the actual conversion never returned.
    } else if (backendFailed) {
        CHECK(observed.disposition == th::Disposition::HarnessFailure);
        CHECK_EQ(observed.childStatus, th::supervisorStopStatus);
        CHECK_EQ(observed.transcript.failureTag, static_cast<std::uint32_t>(publicationFailure
            ? th::FailureTag::PublicationFailed : th::FailureTag::EndpointReturned));
    } else {
        CHECK(observed.disposition == th::Disposition::Returned);
        CHECK_EQ(observed.childStatus, 0u);
        CHECK_EQ(observed.transcript.returnedCode,
            static_cast<unsigned>(mode == th::ChildCase::ConversionNormal ? M2M_OK : M2M_E_UNSUPPORTED));
        const auto returned = required(options.directory / "adapter-returned.txt");
        CHECK(returned.find(mode == th::ChildCase::ConversionNormal ? "profile_reads=0\n" : "profile_reads=1\n") != std::string::npos);
        CHECK(returned.find("dispatches=0\nfailures=0\n") != std::string::npos);
        const auto expectedCode = mode == th::ChildCase::ConversionNormal ? M2M_OK : M2M_E_UNSUPPORTED;
        CHECK(returned.find("details_returned=" + std::to_string(expectedCode) + "\n") != std::string::npos);
        if (mode != th::ChildCase::ConversionNormal)
            CHECK(returned.find("Save remesh reaches an unrecovered writer or invalid-parameter boundary") != std::string::npos);
    }
    if (mode == th::ChildCase::ConversionDeniedProjectFalse || mode == th::ChildCase::ConversionDeniedMstFalse) {
        const bool deniedMst = mode == th::ChildCase::ConversionDeniedMstFalse;
        CHECK(required(options.directory / "project-before-close.txt") ==
            "opened=1\nabout_to_close=1\nlate_mst_lock=" + std::to_string(deniedMst) + "\n");
        CHECK(required(options.directory / "project-after-close.txt") ==
            "opened=1\nabout_to_close=1\nclosed=1\nerror_queries=1\nnative_error=0\nerror=" + std::to_string(deniedMst ? 0 : 7) + "\n");
    }
}
} // namespace

TEST_CASE(remesh_conversion_worker_enabled_writer_returns_normally)
{ run(th::ChildCase::ConversionNormal); }
TEST_CASE(remesh_conversion_worker_denied_writer_reaches_live_terminal_boundary)
{ run(th::ChildCase::ConversionDenied); }
TEST_CASE(remesh_conversion_worker_unknown_profile_preserves_guard)
{ run(th::ChildCase::ConversionUnknown); }
TEST_CASE(remesh_conversion_worker_custom_profile_preserves_guard)
{ run(th::ChildCase::ConversionCustom); }
TEST_CASE(remesh_conversion_worker_project_false_continues_to_remesh_terminal)
{ run(th::ChildCase::ConversionDeniedProjectFalse); }
TEST_CASE(remesh_conversion_worker_mst_false_continues_to_remesh_terminal)
{ run(th::ChildCase::ConversionDeniedMstFalse); }
TEST_CASE(remesh_conversion_worker_publication_failure_stays_failed_without_unwind)
{ run(th::ChildCase::ConversionPublicationFailure); }
TEST_CASE(remesh_conversion_worker_returning_endpoint_stays_failed_without_unwind)
{ run(th::ChildCase::ConversionEndpointReturned); }

int main(int argc, char** argv)
{
    if (argc > 1 && std::string_view(argv[1]) == role) return conversionChild(argc, argv);
    if (argc > 1) return th::childMain(argc, argv);
    return testkit::run_all();
}
