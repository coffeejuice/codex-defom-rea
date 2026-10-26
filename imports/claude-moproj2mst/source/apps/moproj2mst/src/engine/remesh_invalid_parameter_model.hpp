#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>

namespace m2m::engine::simcontrol300::detail {

// Evidence supplied to a pure source model, never an installed host handler.
// SameOriginalModule cases require a separate same-call/module witness; the
// candidate worker may select only StaticFreshDefaultAssumption.
enum class InvalidParameterEvidence {
    Unknown,
    StaticFreshDefaultAssumption,
    SameOriginalModuleDecodedNull,
    SameOriginalModuleDecodedCustom
};
enum class InvalidParameterBranch { Unknown, Default, Custom };
enum class InvalidParameterStage { ErrnoWritten, CustomDispatchPending, DefaultPrefixComplete };
enum class InvalidParameterNext {
    DecodeHandlerUnavailable,
    InvokeCustomHandlerUnimplemented,
    CaptureContextUnimplemented
};
enum class InvalidParameterQualification { SourceModelOnly, StaticDefaultConditional, DecodedValueWitnessOnly };
enum class InvalidParameterModeledEffect {
    FirstNullFileChecked, ErrnoWritten, DecodedNullSelected, DecodedCustomSelected, DebugFlagCleared
};
struct InvalidParameterDebugStore {
    std::uint64_t address;
    std::uint32_t value;
    bool operator==(const InvalidParameterDebugStore&) const = default;
};

// One occurrence value. Every effect here is modeled, including a debug store;
// this type carries neither observed process status nor executed native effects.
struct RemeshInvalidParameterModel {
    std::uint64_t sourceAddress = 0x1400abcf0ULL;
    unsigned formatAttemptOrdinal = 1;
    int modeledErrno = 22;
    InvalidParameterEvidence evidence = InvalidParameterEvidence::Unknown;
    InvalidParameterBranch branch = InvalidParameterBranch::Unknown;
    InvalidParameterStage stage = InvalidParameterStage::ErrnoWritten;
    InvalidParameterNext next = InvalidParameterNext::DecodeHandlerUnavailable;
    InvalidParameterQualification qualification = InvalidParameterQualification::SourceModelOnly;
    std::array<InvalidParameterModeledEffect, 4> effects{};
    std::size_t effectCount = 0;
    std::optional<InvalidParameterDebugStore> modeledDebugStore;
    std::optional<std::uint32_t> predictedProcessStatus;
    bool operator==(const RemeshInvalidParameterModel&) const = default;
};

// No host errno write, callback, native pointer decode, stack unwind or OS call.
// Unknown/custom retain their guards; default stops before capture/unwind/filter.
RemeshInvalidParameterModel modelFirstNullFilePrintf(InvalidParameterEvidence) noexcept;
const char* invalidParameterEvidenceName(InvalidParameterEvidence) noexcept;
const char* invalidParameterStageName(InvalidParameterStage) noexcept;
const char* invalidParameterNextName(InvalidParameterNext) noexcept;

} // namespace m2m::engine::simcontrol300::detail
