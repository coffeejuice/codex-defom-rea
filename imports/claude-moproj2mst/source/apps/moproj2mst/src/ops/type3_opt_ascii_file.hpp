#pragma once

#include <filesystem>

#include "type3_optimizer_scalar_state.hpp"

namespace m2m::mst::detail {

enum class Type3OptAsciiFileOutcome {
    OpenFailed,
    UnsupportedPath,
    ReadFailureUnsupported,
    UnsupportedFileProfile,
    BodyGuard,
    EndData,
    EndOfFile,
};

struct Type3OptAsciiFileResult {
    Type3OptAsciiFileOutcome outcome = Type3OptAsciiFileOutcome::UnsupportedPath;
    // Candidate parser acceptance/status only. The original parser's observed
    // boolean return remains false even after supported parsing completes.
};

// Private candidate-only reader for complete, BOM-free ASCII files with LF
// line endings and a final LF. It does not activate public Type3 loading.
Type3OptAsciiFileResult parseType3OptAsciiFile(
    const std::filesystem::path& path,
    Type3OptimizerScalarState& state);

}  // namespace m2m::mst::detail
