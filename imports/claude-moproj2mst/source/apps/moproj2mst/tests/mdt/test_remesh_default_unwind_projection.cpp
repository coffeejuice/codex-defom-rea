// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include "../../src/engine/remesh_default_unwind_projection.hpp"
#include <cerrno>
#include <limits>

namespace d = m2m::engine::simcontrol300::detail;
namespace {
d::OriginalHandlerProjectionInputs input()
{
    d::OriginalHandlerProjectionInputs in;
    in.capturedRip = 0x141470485ULL; in.stackState = d::OriginalHandlerStackState::UnmodifiedBody;
    in.bodyRsp = 0x100000ULL; in.savedRbx = 0x123456789ULL; in.callerReturn = 0x1414705d8ULL;
    in.lookup = d::OriginalHandlerLookup::RuntimeEntry;
    in.runtimeFunction = d::originalHandlerRuntimeFunction; in.unwindInfo = d::originalHandlerUnwindInfo;
    return in;
}
void refused(const d::OriginalHandlerProjectionInputs& in, d::OriginalHandlerProjectionRefusal why)
{
    const auto out = d::projectOriginalDefaultHandler(in);
    CHECK(out.refusal == why); CHECK(!out.projection);
}
}

TEST_CASE(default_unwind_original_metadata_decodes_exact_function_frame_and_register)
{
    const auto result = d::projectOriginalDefaultHandler(input());
    REQUIRE(result.projection); REQUIRE(result.projection->decodedMetadata);
    const auto& meta = *result.projection->decodedMetadata;
    CHECK_EQ(meta.functionBegin, 0x141470440ULL); CHECK_EQ(meta.functionEnd, 0x141470565ULL);
    CHECK_EQ(meta.unwindRva, 0x347c098u); CHECK_EQ(meta.allocationBytes, 0x5e0u);
    CHECK_EQ(meta.version, 1u); CHECK_EQ(meta.flags, 0u); CHECK_EQ(meta.prologBytes, 9u);
    CHECK_EQ(meta.codeSlots, 3u); CHECK_EQ(meta.frameRegister, 0u); CHECK_EQ(meta.pushedRegister, 3u);
}

TEST_CASE(default_unwind_runtime_projection_restores_supplied_words_and_virtual_return)
{
    auto in = input(); const auto result = d::projectOriginalDefaultHandler(in);
    CHECK(result.refusal == d::OriginalHandlerProjectionRefusal::None); REQUIRE(result.projection);
    const auto& out = *result.projection;
    CHECK(out.hypothesis == d::OriginalHandlerLookup::RuntimeEntry);
    REQUIRE(out.restoredRbx); CHECK_EQ(*out.restoredRbx, 0x123456789ULL);
    CHECK_EQ(out.callerRip, 0x1414705d8ULL); CHECK_EQ(out.callerRsp, 0x1005f0ULL);
    CHECK_EQ(out.exceptionAddress, out.callerRip); CHECK(out.callerRip != in.capturedRip);
    in.savedRbx = 0; in.callerReturn = 0;
    const auto zeroWords = d::projectOriginalDefaultHandler(in);
    REQUIRE(zeroWords.projection); REQUIRE(zeroWords.projection->restoredRbx);
    CHECK_EQ(*zeroWords.projection->restoredRbx, 0ULL); CHECK_EQ(zeroWords.projection->callerRip, 0ULL);
}

TEST_CASE(default_unwind_no_entry_hypothesis_retains_original_nonincremented_slot)
{
    auto in = input(); const auto runtime = d::projectOriginalDefaultHandler(in);
    in.lookup = d::OriginalHandlerLookup::ExplicitNoEntry; in.runtimeFunction.reset(); in.unwindInfo.reset(); in.savedRbx.reset();
    const auto leaf = d::projectOriginalDefaultHandler(in);
    REQUIRE(runtime.projection); REQUIRE(leaf.projection);
    CHECK(leaf.projection->hypothesis == d::OriginalHandlerLookup::ExplicitNoEntry);
    CHECK_EQ(leaf.projection->callerRsp, 0x1005e8ULL);
    CHECK_EQ(runtime.projection->callerRsp - leaf.projection->callerRsp, 8ULL);
    CHECK_EQ(leaf.projection->callerRip, runtime.projection->callerRip);
    CHECK_EQ(leaf.projection->exceptionAddress, 0x1414705d8ULL);
    CHECK(!leaf.projection->restoredRbx); CHECK(!leaf.projection->decodedMetadata);
    in.runtimeFunction = d::originalHandlerRuntimeFunction;
    refused(in, d::OriginalHandlerProjectionRefusal::ContradictoryNoEntryMetadata);
}

TEST_CASE(default_unwind_refuses_every_changed_metadata_byte_and_absent_metadata)
{
    for (unsigned n = 0; n < 12; ++n) {
        auto in = input(); (*in.runtimeFunction)[n] ^= 1;
        refused(in, d::OriginalHandlerProjectionRefusal::WrongRuntimeFunction);
        in = input(); (*in.unwindInfo)[n] ^= 1;
        refused(in, d::OriginalHandlerProjectionRefusal::WrongUnwindInfo);
    }
    auto in = input(); in.runtimeFunction.reset(); refused(in, d::OriginalHandlerProjectionRefusal::MissingRuntimeFunction);
    in = input(); in.unwindInfo.reset(); refused(in, d::OriginalHandlerProjectionRefusal::MissingUnwindInfo);
}

TEST_CASE(default_unwind_refuses_unqualified_pc_stack_route_words_and_arithmetic)
{
    for (std::uint64_t pc : {0ULL, 0x141470440ULL, 0x141470484ULL, 0x141470486ULL, 0x14147055cULL}) {
        auto in = input(); in.capturedRip = pc; refused(in, d::OriginalHandlerProjectionRefusal::UnsupportedPc);
    }
    for (auto state : {d::OriginalHandlerStackState::Unknown, d::OriginalHandlerStackState::Modified,
                       static_cast<d::OriginalHandlerStackState>(999)}) {
        auto in = input(); in.stackState = state; refused(in, d::OriginalHandlerProjectionRefusal::UnsupportedStackState);
    }
    auto in = input(); in.bodyRsp.reset(); refused(in, d::OriginalHandlerProjectionRefusal::MissingBodyRsp);
    in = input(); in.bodyRsp = 0; refused(in, d::OriginalHandlerProjectionRefusal::InvalidBodyRsp);
    in = input(); in.bodyRsp = 0x100001; refused(in, d::OriginalHandlerProjectionRefusal::InvalidBodyRsp);
    in = input(); in.bodyRsp = (std::numeric_limits<std::uint64_t>::max)() & ~15ULL;
    refused(in, d::OriginalHandlerProjectionRefusal::AddressOverflow);
    in = input(); in.lookup = d::OriginalHandlerLookup::Unknown; refused(in, d::OriginalHandlerProjectionRefusal::UnknownLookup);
    in.lookup = static_cast<d::OriginalHandlerLookup>(999); refused(in, d::OriginalHandlerProjectionRefusal::UnknownLookup);
    in = input(); in.savedRbx.reset(); refused(in, d::OriginalHandlerProjectionRefusal::MissingSavedRbx);
    in = input(); in.callerReturn.reset(); refused(in, d::OriginalHandlerProjectionRefusal::MissingCallerReturn);
    in.lookup = d::OriginalHandlerLookup::ExplicitNoEntry; in.runtimeFunction.reset(); in.unwindInfo.reset();
    refused(in, d::OriginalHandlerProjectionRefusal::MissingCallerReturn);
}

TEST_CASE(default_unwind_pure_projection_is_deterministic_without_native_effects)
{
    const auto in = input(); const auto expected = d::projectOriginalDefaultHandler(in);
    const int saved = errno;
    for (int initial : {0, EDOM, ERANGE}) {
        errno = initial;
        CHECK(d::projectOriginalDefaultHandler(in) == expected); CHECK_EQ(errno, initial);
    }
    errno = saved;
}

int main() { return testkit::run_all(); }
