#include "remesh_default_prelude_model.hpp"

namespace m2m::engine::simcontrol300::detail {

DefaultPreludeModel modelDefaultInvalidPrelude(InvalidParameterEvidence evidence, const DefaultPreludeInputs& in) noexcept
{
    DefaultPreludeModel out;
    out.prefix = modelFirstNullFilePrintf(evidence);
    if (out.prefix.branch != InvalidParameterBranch::Default) return out;
    auto effect = [&](DefaultPreludeEffect value) { out.effects[out.effectCount++] = value; };
    effect(DefaultPreludeEffect::ExceptionRecordZeroed);
    out.next = DefaultPreludeNext::CaptureContextUnimplemented;
    if (!in.capturedRip) return out;
    effect(DefaultPreludeEffect::CaptureInputSupplied);
    out.next = DefaultPreludeNext::LookupFunctionUnimplemented;
    if (in.lookup != DefaultPreludeLookup::NoRuntimeFunction && in.lookup != DefaultPreludeLookup::RuntimeFunction) return out;
    effect(DefaultPreludeEffect::LookupInputSupplied);
    if (in.lookup == DefaultPreludeLookup::RuntimeFunction) {
        out.next = DefaultPreludeNext::VirtualUnwindUnimplemented;
        if (!in.suppliedUnwindProjection) return out;
        out.contextProjection = in.suppliedUnwindProjection;
        effect(DefaultPreludeEffect::UnwindProjectionSupplied);
    } else {
        out.next = DefaultPreludeNext::CallerReturnDataUnavailable;
        if (!in.callerReturnValue || !in.callerReturnSlotAddress) return out;
        // Original 1414704dd..4f5: RSP is the slot itself, deliberately no +8.
        out.contextProjection = DefaultPreludeContextProjection{*in.callerReturnValue, *in.callerReturnSlotAddress};
        effect(DefaultPreludeEffect::CallerSlotProjection);
    }
    out.next = DefaultPreludeNext::CallerReturnDataUnavailable;
    if (!in.callerReturnValue) return out;
    // 1414704fd..515 uses the original return slot, not post-unwind RIP.
    out.exceptionAddress = in.callerReturnValue;
    out.exceptionCode = 0xc0000417u; out.exceptionFlags = 1;
    effect(DefaultPreludeEffect::ExceptionRecordPrepared);
    out.next = DefaultPreludeNext::DebuggerQueryUnimplemented;
    if (!in.debuggerPresent) return out;
    effect(DefaultPreludeEffect::DebuggerInputSupplied);
    effect(DefaultPreludeEffect::TopLevelFilterClearModeled); // original ignores previous-filter return
    out.next = DefaultPreludeNext::UnhandledFilterUnimplemented;
    if (!in.filterResult) return out;
    effect(DefaultPreludeEffect::FilterInputSupplied);
    if (*in.filterResult == 0 && !*in.debuggerPresent) {
        out.additionalDebugClear = true;
        effect(DefaultPreludeEffect::DebugFlagClearModeled);
    }
    effect(DefaultPreludeEffect::CurrentProcessCallModeled);
    effect(DefaultPreludeEffect::TerminateCallModeled);
    out.modeledTerminationArgument = 0xc0000417u;
    out.next = DefaultPreludeNext::TerminateProcessUnimplemented;
    if (in.terminationCallReturned) {
        effect(DefaultPreludeEffect::NativeEpilogueModeled);
        out.next = DefaultPreludeNext::CallerContinuationUnrecovered;
    }
    return out;
}

} // namespace m2m::engine::simcontrol300::detail
