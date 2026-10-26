#pragma once

#include <cstdint>
#include <optional>
#include <vector>

namespace m2m::doe {

struct FrcrotPairIdentity {
    std::int32_t master = 0;
    std::int32_t slave = 0;
    friend bool operator==(const FrcrotPairIdentity&, const FrcrotPairIdentity&) = default;
};

enum class FrcrotPairEvent {
    ForwardFound,
    ForwardAppended,
    ModeLineFailed,
    ModeLineAccepted,
    TableResetReached,
    DimensionWritten,
    CodeWritten,
    ValueWritten,
    TableReaderCalled,
    ReverseFound,
    ReverseAppended,
    ReverseDirectionSet,
    MirrorCopyReached,
    SelfPairSkippedReverse,
};

struct FrcrotForwardTablePrefix {
    // Writes reached in this parser invocation, not a snapshot of opaque
    // pre-existing native pair/table storage.
    bool resetReached = false;
    std::optional<std::int32_t> dimensionWrite;
    std::optional<std::int32_t> codeWrite;
    std::optional<double> constantWrite;
    std::optional<std::int32_t> integerValueWrite;
};

enum class FrcrotPairPrefixStatus {
    ModeLineFailure,
    TableReadSuccess,
    TableReadFailure,
};

struct FrcrotPairPrefixInput {
    std::vector<FrcrotPairIdentity> existingPairs;
    std::int32_t master = 0;
    std::int32_t slave = 0;
    bool modeLineSucceeded = false;
    std::int32_t mode = 0;
    // Prepared parsed numeric value and its already-qualified integer cast.
    // This helper does not implement floating-point parsing or the cast.
    double constantValue = 0.0;
    std::int32_t integerValue = 0;
    bool tableReadSucceeded = false;
};

struct FrcrotPairPrefixResult {
    std::vector<FrcrotPairIdentity> pairs;
    std::vector<FrcrotPairEvent> events;
    FrcrotForwardTablePrefix forwardTable;
    FrcrotPairPrefixStatus status = FrcrotPairPrefixStatus::ModeLineFailure;
};

// Prepared-value projection of the source-qualified FRCROT pair-list prefix.
// It does not parse text, construct native pair objects, or model table rows or
// mirror payload bytes. Mode success preserves table-field writes even when
// the later table-reader result is failure.
FrcrotPairPrefixResult applyFrcrotPairPrefix(const FrcrotPairPrefixInput& input);

} // namespace m2m::doe
