#pragma once

#include <cstddef>
#include <optional>

namespace m2m { struct Project; }
namespace m2m::doe {
struct Study;

enum class VariableDimensionPrefixStop { SuppliedRowsCompleted, KeywordItemSnapshotBoundary };
struct VariableDimensionPrefixResult {
    VariableDimensionPrefixStop stop = VariableDimensionPrefixStop::SuppliedRowsCompleted;
    std::size_t visitedRows = 0;
    std::size_t dimensionsStored = 0;
    std::optional<std::size_t> stoppedAt;
    static constexpr bool completeForPublicSave = false;
};

// Isolated per-row value/state prefix of 0x1400d2b60. The supplied Project is
// already resolved by the private caller, not selected from the Study/global.
// Native pre-global snapshot/lazy manager ownership and keyword conversion are
// not supplied by this API. Public nonempty Save remains guarded before it.
VariableDimensionPrefixResult reconcileHeaderDimensions(Study&, const Project& suppliedOperationProject);
} // namespace m2m::doe
