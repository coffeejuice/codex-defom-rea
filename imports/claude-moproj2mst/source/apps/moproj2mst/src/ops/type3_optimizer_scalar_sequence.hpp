#pragma once

#include <cstddef>
#include <span>

#include "type3_optimizer_scalar_state.hpp"

namespace m2m::mst::detail {

enum class Type3ScalarSequenceStop {
    AllLinesConsumed,
    EndData,
    UnsupportedSurrogateLine,
    UnsupportedRowBody,
};

struct Type3ScalarSequenceResult {
    Type3ScalarSequenceStop stop = Type3ScalarSequenceStop::AllLinesConsumed;
    // On EndData, the index after the ENDDATA line. On a guard, the index of
    // the line that was not applied. On normal completion, lines.size().
    std::size_t nextLineIndex = 0;
};

// Applies supported scalar keys and the bounded NUM_CON prepared-row profile
// to already-decoded, already-separated lines. This is not a QFile/QTextStream
// parser; REGIONS and unsupported row inputs remain guarded. Earlier mutations
// remain if a later line reaches a guard.
Type3ScalarSequenceResult applyType3OptimizerScalarLines(
    Type3OptimizerScalarState& state,
    std::span<const qt::String> lines);

}  // namespace m2m::mst::detail
