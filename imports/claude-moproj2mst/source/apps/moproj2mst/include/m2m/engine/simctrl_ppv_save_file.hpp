// Bounded Operation-to-payload adapter for the native SimCtrl.PPV Save call.
// It snapshots existing model owners but performs no filesystem writes.
#pragma once

#include "m2m/engine/simctrl_ppv_writer.hpp"
#include "m2m/model.hpp"

#include <cstddef>
#include <cstdint>
#include <string>

namespace m2m::engine::simctrl_ppv {

enum class SavePathQualification { Unknown, OperationTaskPath };
enum class SaveCodecQualification { Unknown, KnownAsciiCompatible };
enum class SaveOpenOutcome { Unknown, Failed, Succeeded };

struct OperationSaveInput {
    // The path is derived from Operation::project/taskDir. This flag is an
    // explicit caller qualification of that owner/path binding.
    SavePathQualification path = SavePathQualification::Unknown;
    SaveCodecQualification codec = SaveCodecQualification::Unknown;
    unsigned localeCodepage = 0;
    // QFile::open(WriteOnly | Text) result, supplied by the caller. This seam
    // does not open or mutate a physical file.
    SaveOpenOutcome open = SaveOpenOutcome::Unknown;
};

enum class OperationSaveStatus {
    LocalOwnerUnavailable,
    LocalListEmpty,
    SharedOwnerUnavailable,
    CountNotRepresentable,
    FlagsTooShort,
    PathUnqualified,
    PathUnavailable,
    CodecUnqualified,
    OpenOutcomeUnknown,
    OpenFailed,
    PayloadReady,
};

struct OperationSaveResult {
    OperationSaveStatus status = OperationSaveStatus::LocalOwnerUnavailable;
    qt::String path;
    std::int32_t localCount = 0;
    std::int32_t sharedCount = 0;
    bool openAttempted = false;
    bool writerReturned = false;
    std::string bytes;
};

// Native count and per-list row indexes are signed int fields.
bool ppvCountsRepresentable(std::size_t localCount,
                            std::size_t sharedCount) noexcept;

// Models the native writer boundary for one Operation. The local-list empty
// predicate is evaluated first; only a nonempty local list can reach path,
// codec, shared-owner and QFile-open handling. Shared/flags are selected only
// when the operation has a predecessor. Success returns payload bytes for a
// caller to consume; it does not write a file or alter the owners.
OperationSaveResult writePpvForOperation(Operation&, const OperationSaveInput&);

} // namespace m2m::engine::simctrl_ppv
