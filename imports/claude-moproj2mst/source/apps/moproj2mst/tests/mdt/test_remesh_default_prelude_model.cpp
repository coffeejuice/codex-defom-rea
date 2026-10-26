// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include "../../src/engine/remesh_default_prelude_model.hpp"
#include <cerrno>
#include <limits>

namespace d = m2m::engine::simcontrol300::detail;
namespace {
constexpr auto assumed = d::InvalidParameterEvidence::StaticFreshDefaultAssumption;
d::DefaultPreludeInputs leaf()
{
    d::DefaultPreludeInputs in;
    in.capturedRip = 0x141470485ULL; in.lookup = d::DefaultPreludeLookup::NoRuntimeFunction;
    in.callerReturnValue = 0x1414705d8ULL; in.callerReturnSlotAddress = 0x100008ULL;
    in.debuggerPresent = false; in.filterResult = 0;
    return in;
}
bool has(const d::DefaultPreludeModel& model, d::DefaultPreludeEffect effect)
{
    for (std::size_t n = 0; n < model.effectCount; ++n) if (model.effects[n] == effect) return true;
    return false;
}
}

TEST_CASE(default_prelude_refuses_unknown_custom_and_invalid_evidence)
{
    for (auto evidence : {d::InvalidParameterEvidence::Unknown, d::InvalidParameterEvidence::SameOriginalModuleDecodedCustom,
                          static_cast<d::InvalidParameterEvidence>(999)}) {
        const auto model = d::modelDefaultInvalidPrelude(evidence, leaf());
        CHECK(model.next == d::DefaultPreludeNext::PrefixGuard); CHECK_EQ(model.effectCount, 0u);
        CHECK(!model.exceptionAddress); CHECK(!model.modeledTerminationArgument);
    }
}

TEST_CASE(default_prelude_requires_each_external_input_in_native_order)
{
    d::DefaultPreludeInputs in;
    auto model = d::modelDefaultInvalidPrelude(assumed, in);
    CHECK_EQ(model.effectCount, 1u); CHECK(model.effects[0] == d::DefaultPreludeEffect::ExceptionRecordZeroed);
    CHECK(model.next == d::DefaultPreludeNext::CaptureContextUnimplemented);
    in.capturedRip = 0; // supplied zero is distinct from absent evidence
    model = d::modelDefaultInvalidPrelude(assumed, in);
    CHECK(model.next == d::DefaultPreludeNext::LookupFunctionUnimplemented);
    in.lookup = static_cast<d::DefaultPreludeLookup>(999);
    CHECK(d::modelDefaultInvalidPrelude(assumed, in).next == d::DefaultPreludeNext::LookupFunctionUnimplemented);
    in.lookup = d::DefaultPreludeLookup::RuntimeFunction;
    CHECK(d::modelDefaultInvalidPrelude(assumed, in).next == d::DefaultPreludeNext::VirtualUnwindUnimplemented);
    in.suppliedUnwindProjection = d::DefaultPreludeContextProjection{7, 8};
    CHECK(d::modelDefaultInvalidPrelude(assumed, in).next == d::DefaultPreludeNext::CallerReturnDataUnavailable);
    in.callerReturnValue = 9;
    CHECK(d::modelDefaultInvalidPrelude(assumed, in).next == d::DefaultPreludeNext::DebuggerQueryUnimplemented);
    in.debuggerPresent = false;
    model = d::modelDefaultInvalidPrelude(assumed, in);
    CHECK(model.next == d::DefaultPreludeNext::UnhandledFilterUnimplemented);
    CHECK(has(model, d::DefaultPreludeEffect::TopLevelFilterClearModeled)); CHECK(!model.modeledTerminationArgument);
}

TEST_CASE(default_prelude_leaf_uses_exact_return_slot_without_stack_increment)
{
    auto in = leaf();
    const auto model = d::modelDefaultInvalidPrelude(assumed, in);
    REQUIRE(model.contextProjection); REQUIRE(model.exceptionAddress);
    CHECK_EQ(model.contextProjection->rip, *in.callerReturnValue);
    CHECK_EQ(model.contextProjection->rsp, *in.callerReturnSlotAddress);
    CHECK_EQ(*model.exceptionAddress, *in.callerReturnValue);
    CHECK_EQ(model.exceptionCode, 0xc0000417u); CHECK_EQ(model.exceptionFlags, 1u);
    CHECK(model.effects[3] == d::DefaultPreludeEffect::CallerSlotProjection);
    CHECK(model.effects[4] == d::DefaultPreludeEffect::ExceptionRecordPrepared);
    in.callerReturnSlotAddress.reset();
    CHECK(d::modelDefaultInvalidPrelude(assumed, in).next == d::DefaultPreludeNext::CallerReturnDataUnavailable);
}

TEST_CASE(default_prelude_unwind_projection_does_not_replace_exception_caller_address)
{
    auto in = leaf(); in.lookup = d::DefaultPreludeLookup::RuntimeFunction;
    in.suppliedUnwindProjection = d::DefaultPreludeContextProjection{0x700000ULL, 0x800000ULL};
    in.callerReturnSlotAddress.reset(); // runtime-entry branch does not need fallback slot address
    const auto model = d::modelDefaultInvalidPrelude(d::InvalidParameterEvidence::SameOriginalModuleDecodedNull, in);
    REQUIRE(model.contextProjection); REQUIRE(model.exceptionAddress);
    CHECK(*model.contextProjection == *in.suppliedUnwindProjection);
    CHECK_EQ(*model.exceptionAddress, *in.callerReturnValue);
    CHECK(*model.exceptionAddress != model.contextProjection->rip);
    CHECK(has(model, d::DefaultPreludeEffect::UnwindProjectionSupplied));
    CHECK(!has(model, d::DefaultPreludeEffect::CallerSlotProjection));
    CHECK(model.prefix.qualification == d::InvalidParameterQualification::DecodedValueWitnessOnly);
}

TEST_CASE(default_prelude_filter_debugger_matrix_repeats_debug_clear_only_for_two_zero_results)
{
    for (bool debug : {false, true}) for (std::int32_t result : {0, 1, -1, (std::numeric_limits<std::int32_t>::min)()}) {
        auto in = leaf(); in.debuggerPresent = debug; in.filterResult = result;
        const auto model = d::modelDefaultInvalidPrelude(assumed, in);
        CHECK_EQ(model.additionalDebugClear, !debug && result == 0);
        CHECK_EQ(has(model, d::DefaultPreludeEffect::DebugFlagClearModeled), !debug && result == 0);
        REQUIRE(model.modeledTerminationArgument); CHECK_EQ(*model.modeledTerminationArgument, 0xc0000417u);
        CHECK(model.next == d::DefaultPreludeNext::TerminateProcessUnimplemented);
        CHECK(!has(model, d::DefaultPreludeEffect::NativeEpilogueModeled));
        CHECK(has(model, d::DefaultPreludeEffect::TopLevelFilterClearModeled));
    }
}

TEST_CASE(default_prelude_termination_return_is_native_epilogue_hypothesis_and_model_is_pure)
{
    auto in = leaf(); in.terminationCallReturned = true;
    const auto expected = d::modelDefaultInvalidPrelude(assumed, in);
    CHECK(expected.next == d::DefaultPreludeNext::CallerContinuationUnrecovered);
    CHECK(expected.effects[expected.effectCount - 1] == d::DefaultPreludeEffect::NativeEpilogueModeled);
    CHECK_EQ(expected.effectCount, expected.effects.size());
    const int saved = errno;
    for (int initial : {0, EDOM, ERANGE}) {
        errno = initial;
        CHECK(d::modelDefaultInvalidPrelude(assumed, in) == expected);
        CHECK_EQ(errno, initial);
    }
    errno = saved;
}

int main() { return testkit::run_all(); }
