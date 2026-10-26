#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include "isolated_save.hpp"
#include "isolated_save_job.hpp"
#include "../engine/terminal_harness_supervisor.hpp"
#include "moproj2mst.h"

#include <atomic>
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

namespace m2m::cli {
namespace {
namespace th = engine::terminal_harness;
constexpr int rejected = 65;
constexpr int timedOut = 66;
constexpr unsigned allowedFlags = M2M_FLAG_DOE_TEMP | M2M_FLAG_STRICT | M2M_FLAG_WRITE_MDT | M2M_FLAG_WRITE_SAVE;

bool samePath(std::filesystem::path left, std::filesystem::path right)
{
    left = left.lexically_normal(); right = right.lexically_normal();
    return CompareStringOrdinal(left.c_str(), -1, right.c_str(), -1, TRUE) == CSTR_EQUAL;
}

bool withinPath(const std::filesystem::path& target, const std::filesystem::path& parent)
{
    auto child = target.begin();
    for (auto base = parent.begin(); base != parent.end(); ++base, ++child) {
        if (child == target.end() || CompareStringOrdinal(base->c_str(), -1, child->c_str(), -1, TRUE) != CSTR_EQUAL)
            return false;
    }
    return true;
}

std::filesystem::path absoluteRequestPath(const std::wstring& value, bool filePath = true)
{
    if (value.empty() || value.find(L'\0') != std::wstring::npos) throw std::runtime_error("isolated Save path is empty or invalid");
    const std::filesystem::path path(value);
    if (!path.is_absolute()) throw std::runtime_error("isolated Save path must be absolute");
    const auto normalized = path.lexically_normal();
    if (filePath && (!normalized.has_filename() || normalized.filename() == L"." || normalized.filename() == L".."))
        throw std::runtime_error("isolated Save target must be an absolute file path");
    return normalized;
}

std::filesystem::path scratchRoot()
{
    std::wstring value(32768, L'\0');
    const DWORD length = GetTempPathW(static_cast<DWORD>(value.size()), value.data());
    if (!length || length >= value.size()) throw std::runtime_error("cannot resolve OS temporary directory");
    value.resize(length);
    const auto root = std::filesystem::canonical(std::filesystem::path(value));
    if (!root.is_absolute() || !std::filesystem::is_directory(root))
        throw std::runtime_error("OS temporary directory is not a usable absolute directory");
    return root;
}

std::filesystem::path uniqueLeaf(const std::filesystem::path& root)
{
    static std::atomic<unsigned long long> sequence{0};
    for (unsigned attempt = 0; attempt != 32; ++attempt) {
        const auto id = ++sequence;
        auto leaf = root / (L"m2m-isolated-save-" + std::to_wstring(GetCurrentProcessId()) + L"-" +
                            std::to_wstring(GetTickCount64()) + L"-" + std::to_wstring(id));
        if (!std::filesystem::exists(leaf)) return leaf;
    }
    throw std::runtime_error("cannot allocate isolated Save scratch leaf");
}

bool returnedNormally(const th::Observation& observed)
{
    const auto& t = observed.transcript;
    return observed.disposition == th::Disposition::Returned && observed.stopReason == th::StopReason::None &&
        observed.childCreated && observed.resumed && observed.childStopped && observed.childStatus == 0 &&
        !observed.supervisorStopRequested && !observed.preparationHeldUntilParentExit &&
        t.profile == th::ProtocolProfile::RemeshV2 && t.started && t.returned && !t.boundary &&
        !t.backendFailure && !t.invalid && t.frames == 2 && t.returnedCode <= 7;
}

bool naturalTerminal(const th::Observation& observed)
{
    const auto& t = observed.transcript;
    return observed.disposition == th::Disposition::CandidateProcessTerminal &&
        observed.stopReason == th::StopReason::None && observed.childCreated && observed.resumed &&
        observed.childStopped && observed.childStatus == th::terminalStatus && !observed.supervisorStopRequested &&
        !observed.preparationHeldUntilParentExit && t.profile == th::ProtocolProfile::RemeshV2 &&
        t.started && t.boundary && !t.returned && !t.backendFailure && !t.invalid && t.frames == 2 &&
        t.failureTag == 0 && t.save.has_value();
}

struct OwnedScratch {
    std::filesystem::path path;
    bool keep = false;
    ~OwnedScratch()
    {
        if (!keep && !path.empty()) {
            std::error_code ignored;
            std::filesystem::remove_all(path, ignored);
        }
    }
};

class ProductJobLease final : public th::PreparedCase {
    std::filesystem::path leaf_;
    std::shared_ptr<IsolatedJob> job_;
public:
    ProductJobLease(std::filesystem::path leaf, std::shared_ptr<IsolatedJob> job)
        : leaf_(std::move(leaf)), job_(std::move(job)) {}
    void bindNonce(std::uint64_t nonce) override
    {
        if (!nonce || job_->nonce) throw std::runtime_error("isolated Save nonce binding invalid");
        job_->nonce = nonce;
        writeIsolatedJob(leaf_, *job_);
    }
};
}

int runIsolatedSave(const IsolatedSaveRequest& request)
{
    try {
        if (!(request.flags & M2M_FLAG_WRITE_SAVE) || (request.flags & ~allowedFlags) ||
            !IsValidCodePage(request.codepage)) return M2M_E_ARG;
        const auto project = absoluteRequestPath(request.project);
        const auto output = absoluteRequestPath(request.output);
        const auto deform = absoluteRequestPath(request.deformDir, false);
        const auto receipt = absoluteRequestPath(request.receipt);
        const auto receiptParent = std::filesystem::canonical(receipt.parent_path());
        const auto normalizedReceipt = receiptParent / receipt.filename();
        const auto canonicalOrNormal = [](const std::filesystem::path& path) {
            std::error_code ec;
            const auto resolved = std::filesystem::weakly_canonical(path, ec);
            if (ec) throw std::filesystem::filesystem_error("resolve isolated Save path", path, ec);
            return resolved.lexically_normal();
        };
        const auto projectIdentity = canonicalOrNormal(project);
        const auto outputIdentity = canonicalOrNormal(output);
        const auto receiptIdentity = canonicalOrNormal(normalizedReceipt);
        const auto projectTree = canonicalOrNormal(project.parent_path());
        if (withinPath(receiptIdentity, projectTree)) return M2M_E_ARG;
        if (samePath(receiptIdentity, projectIdentity) || samePath(receiptIdentity, outputIdentity)) return M2M_E_ARG;
        const auto existingAlias = [](const std::filesystem::path& left, const std::filesystem::path& right) {
            if (!std::filesystem::exists(left) || !std::filesystem::exists(right)) return false;
            std::error_code ec;
            return std::filesystem::equivalent(left, right, ec) && !ec;
        };
        if (existingAlias(receipt, project) || existingAlias(receipt, output)) return M2M_E_ARG;

        auto job = std::make_shared<IsolatedJob>();
        job->project = project.u16string(); job->output = output.u16string();
        job->deformDir = deform.u16string(); job->workingDirectory = std::filesystem::current_path().u16string();
        job->flags = request.flags; job->codepage = request.codepage;
        const auto root = scratchRoot();
        OwnedScratch scratch{uniqueLeaf(root)};
        th::Options options;
        options.allowOwnedWorkerTermination = true;
        options.executable = th::ownExecutable();
        options.expectedSha256 = th::executableSha256(options.executable);
        options.directory = scratch.path;
        options.explicitScratchRoot = root;
        options.childCase = th::ChildCase::ProductIsolated;
        options.protocolProfile = th::ProtocolProfile::RemeshV2;
        options.workerRole = th::WorkerRole::ProductIsolatedSave;
        options.deadlineMs = 20000;
        options.prepareOwnedCase = [job](const std::filesystem::path& leaf) {
            return std::make_unique<ProductJobLease>(leaf, job);
        };
        const auto observed = th::supervise(options);
        if (observed.preparationHeldUntilParentExit) scratch.keep = true;
        if (observed.stopReason == th::StopReason::Timeout || observed.disposition == th::Disposition::Timeout)
            return timedOut;
        if (returnedNormally(observed)) return static_cast<int>(observed.transcript.returnedCode);
        if (!naturalTerminal(observed)) return rejected;

        // Keep the supervisor leaf if receipt validation/commit fails so the
        // stopped child's bounded diagnostics remain available for review.
        scratch.keep = true;
        commitIsolatedTerminalReceipt(receipt, observed, *job);
        (void)TerminateProcess(GetCurrentProcess(), th::terminalStatus);
        for (;;) Sleep(INFINITE); // Never turn an unexpected relay return into ordinary success.
    } catch (...) {
        return rejected;
    }
}

} // namespace m2m::cli
