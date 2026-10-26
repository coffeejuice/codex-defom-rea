#pragma once

#include <array>
#include <cstdint>
#include <optional>

namespace m2m::engine::simcontrol300::detail {

using OriginalHandlerMetadataBytes = std::array<std::uint8_t, 12>;
inline constexpr OriginalHandlerMetadataBytes originalHandlerRuntimeFunction{
    0x40, 0x04, 0x47, 0x01, 0x65, 0x05, 0x47, 0x01, 0x98, 0xc0, 0x47, 0x03};
inline constexpr OriginalHandlerMetadataBytes originalHandlerUnwindInfo{
    0x01, 0x09, 0x03, 0x00, 0x09, 0x01, 0xbc, 0x00, 0x02, 0x30, 0x00, 0x00};

enum class OriginalHandlerLookup { Unknown, RuntimeEntry, ExplicitNoEntry };
enum class OriginalHandlerStackState { Unknown, UnmodifiedBody, Modified };
enum class OriginalHandlerProjectionRefusal {
    None, UnsupportedPc, UnsupportedStackState, MissingBodyRsp, InvalidBodyRsp,
    AddressOverflow, UnknownLookup, MissingRuntimeFunction, WrongRuntimeFunction,
    MissingUnwindInfo, WrongUnwindInfo, ContradictoryNoEntryMetadata,
    MissingSavedRbx, MissingCallerReturn
};
struct OriginalHandlerProjectionInputs {
    std::uint64_t capturedRip = 0;
    OriginalHandlerStackState stackState = OriginalHandlerStackState::Unknown;
    std::optional<std::uint64_t> bodyRsp, savedRbx, callerReturn;
    OriginalHandlerLookup lookup = OriginalHandlerLookup::Unknown;
    std::optional<OriginalHandlerMetadataBytes> runtimeFunction, unwindInfo;
};
struct OriginalHandlerDecodedMetadata {
    std::uint64_t functionBegin, functionEnd;
    std::uint32_t unwindRva, allocationBytes;
    std::uint8_t version, flags, prologBytes, codeSlots, frameRegister, pushedRegister;
    bool operator==(const OriginalHandlerDecodedMetadata&) const = default;
};
// Supplied numeric words are never dereferenced. This is not a full CONTEXT,
// captured-stack witness, API result, or evidence that a process terminated.
struct OriginalHandlerProjection {
    OriginalHandlerLookup hypothesis;
    std::uint64_t callerRip, callerRsp, exceptionAddress;
    std::optional<std::uint64_t> restoredRbx;
    std::optional<OriginalHandlerDecodedMetadata> decodedMetadata;
    bool operator==(const OriginalHandlerProjection&) const = default;
};
struct OriginalHandlerProjectionResult {
    OriginalHandlerProjectionRefusal refusal = OriginalHandlerProjectionRefusal::UnknownLookup;
    std::optional<OriginalHandlerProjection> projection;
    bool operator==(const OriginalHandlerProjectionResult&) const = default;
};

// Only the original capture-return PC 141470485 and exact retained metadata are
// admitted. ExplicitNoEntry is a separate hypothetical branch, not a claim that
// the original image lacks the statically present runtime-function entry.
OriginalHandlerProjectionResult projectOriginalDefaultHandler(const OriginalHandlerProjectionInputs&) noexcept;

} // namespace m2m::engine::simcontrol300::detail
