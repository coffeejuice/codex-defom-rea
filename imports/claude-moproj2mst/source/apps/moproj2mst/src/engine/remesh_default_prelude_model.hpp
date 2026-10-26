#pragma once

#include "remesh_invalid_parameter_model.hpp"

namespace m2m::engine::simcontrol300::detail {

enum class DefaultPreludeLookup { Unknown, NoRuntimeFunction, RuntimeFunction };
struct DefaultPreludeContextProjection {
    std::uint64_t rip, rsp;
    bool operator==(const DefaultPreludeContextProjection&) const = default;
};
// Caller-supplied hypothetical values, never evidence that an OS operation ran.
// A two-register projection does not stand for the complete native CONTEXT.
struct DefaultPreludeInputs {
    std::optional<std::uint64_t> capturedRip;
    DefaultPreludeLookup lookup = DefaultPreludeLookup::Unknown;
    std::optional<DefaultPreludeContextProjection> suppliedUnwindProjection;
    std::optional<std::uint64_t> callerReturnValue, callerReturnSlotAddress;
    std::optional<bool> debuggerPresent;
    std::optional<std::int32_t> filterResult;
    bool terminationCallReturned = false; // adverse hypothetical; false never means process death
};
enum class DefaultPreludeEffect {
    ExceptionRecordZeroed, CaptureInputSupplied, LookupInputSupplied,
    UnwindProjectionSupplied, CallerSlotProjection, ExceptionRecordPrepared,
    DebuggerInputSupplied, TopLevelFilterClearModeled, FilterInputSupplied,
    DebugFlagClearModeled, CurrentProcessCallModeled, TerminateCallModeled,
    NativeEpilogueModeled
};
enum class DefaultPreludeNext {
    PrefixGuard, CaptureContextUnimplemented, LookupFunctionUnimplemented,
    VirtualUnwindUnimplemented, CallerReturnDataUnavailable,
    DebuggerQueryUnimplemented, UnhandledFilterUnimplemented,
    TerminateProcessUnimplemented, CallerContinuationUnrecovered
};
struct DefaultPreludeModel {
    RemeshInvalidParameterModel prefix;
    std::array<DefaultPreludeEffect, 12> effects{};
    std::size_t effectCount = 0;
    DefaultPreludeNext next = DefaultPreludeNext::PrefixGuard;
    std::optional<DefaultPreludeContextProjection> contextProjection;
    std::optional<std::uint64_t> exceptionAddress;
    std::uint32_t exceptionCode = 0, exceptionFlags = 0;
    bool additionalDebugClear = false;
    std::optional<std::uint32_t> modeledTerminationArgument;
    bool operator==(const DefaultPreludeModel&) const = default;
};

// Pure extension of CP110's default prefix. Does not capture/unwind a live stack,
// clear a host filter/global, invoke a callback, terminate, or observe a process.
DefaultPreludeModel modelDefaultInvalidPrelude(InvalidParameterEvidence, const DefaultPreludeInputs&) noexcept;

} // namespace m2m::engine::simcontrol300::detail
