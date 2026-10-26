#include "remesh_default_unwind_projection.hpp"
#include <limits>

namespace m2m::engine::simcontrol300::detail {
namespace {
std::uint32_t little32(const OriginalHandlerMetadataBytes& bytes, unsigned at) noexcept
{
    return std::uint32_t(bytes[at]) | (std::uint32_t(bytes[at + 1]) << 8) |
        (std::uint32_t(bytes[at + 2]) << 16) | (std::uint32_t(bytes[at + 3]) << 24);
}
}
OriginalHandlerProjectionResult projectOriginalDefaultHandler(const OriginalHandlerProjectionInputs& in) noexcept
{
    using Refusal = OriginalHandlerProjectionRefusal;
    auto refuse = [](Refusal why) { return OriginalHandlerProjectionResult{why, std::nullopt}; };
    if (in.capturedRip != 0x141470485ULL) return refuse(Refusal::UnsupportedPc);
    if (in.stackState != OriginalHandlerStackState::UnmodifiedBody) return refuse(Refusal::UnsupportedStackState);
    if (!in.bodyRsp) return refuse(Refusal::MissingBodyRsp);
    if (!*in.bodyRsp || (*in.bodyRsp & 15)) return refuse(Refusal::InvalidBodyRsp);
    if (*in.bodyRsp > (std::numeric_limits<std::uint64_t>::max)() - 0x5f0ULL)
        return refuse(Refusal::AddressOverflow);
    OriginalHandlerProjection out{in.lookup, 0, 0, 0, std::nullopt, std::nullopt};
    if (in.lookup == OriginalHandlerLookup::RuntimeEntry) {
        if (!in.runtimeFunction) return refuse(Refusal::MissingRuntimeFunction);
        if (*in.runtimeFunction != originalHandlerRuntimeFunction) return refuse(Refusal::WrongRuntimeFunction);
        if (!in.unwindInfo) return refuse(Refusal::MissingUnwindInfo);
        if (*in.unwindInfo != originalHandlerUnwindInfo) return refuse(Refusal::WrongUnwindInfo);
        if (!in.savedRbx) return refuse(Refusal::MissingSavedRbx);
        const auto& raw = *in.unwindInfo;
        const auto allocation = (std::uint32_t(raw[6]) | (std::uint32_t(raw[7]) << 8)) * 8;
        out.decodedMetadata = OriginalHandlerDecodedMetadata{
            0x140000000ULL + little32(*in.runtimeFunction, 0),
            0x140000000ULL + little32(*in.runtimeFunction, 4), little32(*in.runtimeFunction, 8),
            allocation, std::uint8_t(raw[0] & 7), std::uint8_t(raw[0] >> 3), raw[1], raw[2],
            std::uint8_t(raw[3] & 15), std::uint8_t(raw[9] >> 4)};
        out.restoredRbx = in.savedRbx; // supplied word originally at bodyRsp+5e0
        out.callerRsp = *in.bodyRsp + allocation + 8 + 8; // reverse push, then virtual return
    } else if (in.lookup == OriginalHandlerLookup::ExplicitNoEntry) {
        if (in.runtimeFunction || in.unwindInfo) return refuse(Refusal::ContradictoryNoEntryMetadata);
        out.callerRsp = *in.bodyRsp + 0x5e8; // exact native fallback; no return-slot +8
    } else return refuse(Refusal::UnknownLookup);
    if (!in.callerReturn) return refuse(Refusal::MissingCallerReturn);
    out.callerRip = *in.callerReturn; // supplied word originally at bodyRsp+5e8
    out.exceptionAddress = *in.callerReturn; // native record always uses this original slot
    return {Refusal::None, out};
}

} // namespace m2m::engine::simcontrol300::detail
