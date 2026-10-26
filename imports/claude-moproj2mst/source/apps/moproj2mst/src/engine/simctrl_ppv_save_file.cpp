#include "m2m/engine/simctrl_ppv_save_file.hpp"

#include "engine_internal.hpp"
#include "m2m/qt4path.hpp"

#include <limits>
#include <vector>

namespace m2m::engine::simctrl_ppv {
namespace {

Point toPoint(const RecoveryEntry& entry)
{
    return {entry.type, entry.ppvSecondInt, entry.ppvFirstDouble,
            entry.ppvSecondDouble};
}

std::vector<Point> snapshot(const std::vector<RecoveryEntry>& entries)
{
    std::vector<Point> points;
    points.reserve(entries.size());
    for (const auto& entry : entries) points.push_back(toPoint(entry));
    return points;
}

} // namespace

bool ppvCountsRepresentable(std::size_t localCount,
                            std::size_t sharedCount) noexcept
{
    constexpr auto max = static_cast<std::size_t>((std::numeric_limits<int>::max)());
    return localCount <= max && sharedCount <= max &&
           sharedCount <= max - localCount;
}

OperationSaveResult writePpvForOperation(Operation& operation,
                                         const OperationSaveInput& input)
{
    OperationSaveResult result;
    if (!operation.recoveryB) {
        result.status = OperationSaveStatus::LocalOwnerUnavailable;
        return result;
    }

    // 0x140d4afe0 calls QListData::isEmpty on OPI+0x20 before QFile setup.
    // In particular, a nonempty root list cannot make a local-empty operation
    // enter the writer. Do not inspect the root, path, codec or open outcome on
    // this early-return branch.
    if (operation.recoveryB->empty()) {
        result.status = OperationSaveStatus::LocalListEmpty;
        return result;
    }

    if (input.path != SavePathQualification::OperationTaskPath) {
        result.status = OperationSaveStatus::PathUnqualified;
        return result;
    }
    if (!operation.project) {
        result.status = OperationSaveStatus::PathUnavailable;
        return result;
    }
    result.path = qt::cleanPath(taskDirAbs(operation) + qt::kSep +
                                qt::L("SimCtrl.PPV"));
    if (result.path.isEmpty() || qt::isRelativePath(result.path)) {
        result.status = OperationSaveStatus::PathUnavailable;
        return result;
    }
    if (input.codec != SaveCodecQualification::KnownAsciiCompatible ||
        input.localeCodepage == 0) {
        result.status = OperationSaveStatus::CodecUnqualified;
        return result;
    }

    const bool hasPredecessor = prevOp(*operation.project, operation) != nullptr;
    std::vector<RecoveryEntry>* sharedEntries = nullptr;
    const std::vector<bool>* ownerFlags = nullptr;
    if (hasPredecessor) {
        sharedEntries = recoveryListA(operation);
        if (!sharedEntries) {
            result.status = OperationSaveStatus::SharedOwnerUnavailable;
            return result;
        }
        ownerFlags = operation.recoveryFlags.get();
    }

    const std::size_t sharedSize = sharedEntries ? sharedEntries->size() : 0;
    if (!ppvCountsRepresentable(operation.recoveryB->size(), sharedSize)) {
        result.status = OperationSaveStatus::CountNotRepresentable;
        return result;
    }
    result.localCount = static_cast<std::int32_t>(operation.recoveryB->size());
    result.sharedCount = static_cast<std::int32_t>(sharedSize);
    if (sharedEntries && ownerFlags && ownerFlags->size() < sharedEntries->size()) {
        result.status = OperationSaveStatus::FlagsTooShort;
        return result;
    }

    // Snapshot the actual Operation/Project owners for this call only. The
    // returned payload cannot mutate persistent recovery lists or flags.
    const auto local = snapshot(*operation.recoveryB);
    const auto shared = sharedEntries ? snapshot(*sharedEntries) : std::vector<Point>{};
    const auto flags = ownerFlags
        ? std::vector<bool>(ownerFlags->begin(), ownerFlags->end())
        : std::vector<bool>{};

    result.openAttempted = true;
    if (input.open == SaveOpenOutcome::Unknown) {
        result.status = OperationSaveStatus::OpenOutcomeUnknown;
        return result;
    }
    if (input.open == SaveOpenOutcome::Failed) {
        const auto native = writePpvPayload(false, false, local,
            sharedEntries ? &shared : nullptr,
            ownerFlags ? &flags : nullptr, input.localeCodepage);
        result.openAttempted = native.openAttempted;
        result.writerReturned = native.returned;
        result.bytes = native.bytes;
        result.status = OperationSaveStatus::OpenFailed;
        return result;
    }

    const auto native = writePpvPayload(false, true, local,
        sharedEntries ? &shared : nullptr,
        ownerFlags ? &flags : nullptr, input.localeCodepage);
    result.openAttempted = native.openAttempted;
    result.writerReturned = native.returned;
    result.bytes = native.bytes;
    result.status = OperationSaveStatus::PayloadReady;
    return result;
}

} // namespace m2m::engine::simctrl_ppv
