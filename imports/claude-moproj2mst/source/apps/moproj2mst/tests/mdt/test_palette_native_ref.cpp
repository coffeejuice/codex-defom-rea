// ctest-labels: m2m-mdt
// Isolated native atomic/value primitive; no runtime/private palette integration.
#include <testkit/check.hpp>
#include <memory>
#include "m2m/engine/palette_native_ref.hpp"

using namespace m2m::engine;
namespace {
using Kind = PaletteNativeValueKind;
using Stage = PaletteNativeValueStage;
using Issue = PaletteNativeRefIssue;
constexpr auto qualification = "Isolated current palette-data atomic state";
std::shared_ptr<PaletteNativeRefState> state()
{ return std::make_shared<PaletteNativeRefState>(qualification); }
}

TEST_CASE(palette_native_ref_starts_unknown_without_acquiring_a_value)
{
    const auto data = state(); PaletteNativeValueRef getter(data, Kind::GetterTemporary);
    CHECK(!data->currentBits()); CHECK(data->mutationSequence() == 0u);
    CHECK(data->refMutations() == 0u); CHECK(data->derefMutations() == 0u);
    CHECK(data->nativeValueBalance() == 0u); CHECK(!data->destructionPending());
    CHECK(data->nativeOwnerAlive()); CHECK(getter.stateOwner().lock() == data);
    CHECK(getter.kind() == Kind::GetterTemporary); CHECK(getter.report().stage == Stage::AwaitingRetain);
    CHECK(!getter.retainReceipt()); CHECK(!getter.releaseReceipt());
}

TEST_CASE(palette_native_ref_observation_binds_current_sequence_without_resetting_known_bits)
{
    const auto data = state();
    CHECK(!data->observeCurrent(2, 0, "")); CHECK(!data->observeCurrent(2, 0, "Other data"));
    CHECK(!data->observeCurrent(2, 1, qualification)); CHECK(!data->currentBits());
    CHECK(data->observeCurrent(2, 0, qualification)); CHECK(data->currentBits() == 2u);
    CHECK(data->observeCurrent(2, 0, qualification)); CHECK(!data->observeCurrent(3, 0, qualification));
    CHECK(data->currentBits() == 2u); CHECK(data->mutationSequence() == 0u);
    CHECK(data->refMutations() == 0u); CHECK(data->derefMutations() == 0u);
}

TEST_CASE(palette_native_ref_unknown_getter_retain_occurs_once_and_unknown_release_keeps_it_held)
{
    const auto data = state(); auto getter = std::make_shared<PaletteNativeValueRef>(data, Kind::GetterTemporary);
    CHECK(getter->retain().issue == Issue::None); CHECK(getter->report().stage == Stage::Held);
    REQUIRE(getter->retainReceipt());
    CHECK(getter->retainReceipt()->sequence == 1u); CHECK(!getter->retainReceipt()->beforeBits);
    CHECK(!getter->retainReceipt()->afterBits); CHECK(!getter->retainReceipt()->nonzero.has_value());
    CHECK(data->mutationSequence() == 1u); CHECK(data->refMutations() == 1u);
    CHECK(data->nativeValueBalance() == 1u); CHECK(!data->currentBits());
    const auto receipt = *getter->retainReceipt(); auto diagnosticCopy = getter;
    CHECK(diagnosticCopy->retain().issue == Issue::None);
    CHECK(data->mutationSequence() == 1u); CHECK(data->refMutations() == 1u);
    CHECK(getter->release().issue == Issue::RefCountUnavailable);
    CHECK(getter->report().stage == Stage::Held); CHECK(!getter->releaseReceipt());
    CHECK(data->derefMutations() == 0u); CHECK(data->nativeValueBalance() == 1u);
    CHECK(getter->retainReceipt()->sequence == receipt.sequence);
}

TEST_CASE(palette_native_ref_late_count_includes_getter_retain_and_never_replays_it)
{
    const auto data = state(); PaletteNativeValueRef getter(data, Kind::GetterTemporary);
    CHECK(getter.retain().issue == Issue::None);
    CHECK(!data->observeCurrent(3, 0, qualification));
    REQUIRE(data->observeCurrent(3, 1, qualification));
    CHECK(getter.release().issue == Issue::None); CHECK(getter.report().stage == Stage::Released);
    REQUIRE(getter.releaseReceipt());
    CHECK(getter.releaseReceipt()->beforeBits == 3u); CHECK(getter.releaseReceipt()->afterBits == 2u);
    CHECK(getter.releaseReceipt()->nonzero == true); CHECK(getter.releaseReceipt()->sequence == 2u);
    CHECK(data->currentBits() == 2u); CHECK(data->refMutations() == 1u);
    CHECK(data->derefMutations() == 1u); CHECK(data->nativeValueBalance() == 0u);
    CHECK(getter.release().issue == Issue::None); CHECK(data->mutationSequence() == 2u);
    CHECK(getter.retain().issue == Issue::ValueAlreadyReleased); CHECK(data->mutationSequence() == 2u);
    CHECK(!data->observeCurrent(3, 1, qualification)); CHECK(!data->observeCurrent(3, 2, qualification));
}

TEST_CASE(palette_native_ref_nonzero_temporary_release_and_destination_retain_restore_total)
{
    const auto data = state(); REQUIRE(data->observeCurrent(2, 0, qualification));
    PaletteNativeValueRef getter(data, Kind::GetterTemporary);
    CHECK(getter.retain().issue == Issue::None); CHECK(data->currentBits() == 3u);
    CHECK(getter.release().issue == Issue::None); CHECK(data->currentBits() == 2u);
    PaletteNativeValueRef destination(data, Kind::PrivateDestination);
    CHECK(destination.retain().issue == Issue::None); CHECK(destination.report().stage == Stage::Held);
    CHECK(destination.kind() == Kind::PrivateDestination); REQUIRE(destination.retainReceipt());
    CHECK(destination.retainReceipt()->beforeBits == 2u); CHECK(destination.retainReceipt()->afterBits == 3u);
    CHECK(destination.retainReceipt()->nonzero == true); CHECK(data->mutationSequence() == 3u);
    CHECK(data->refMutations() == 2u); CHECK(data->derefMutations() == 1u);
    CHECK(data->nativeValueBalance() == 1u); CHECK(!data->destructionPending());
    CHECK(destination.retain().issue == Issue::None); CHECK(data->mutationSequence() == 3u);
}

TEST_CASE(palette_native_ref_zero_deref_commits_once_and_blocks_destruction_dependents)
{
    const auto data = state(); PaletteNativeValueRef getter(data, Kind::GetterTemporary);
    CHECK(getter.retain().issue == Issue::None);
    REQUIRE(data->observeCurrent(1, 1, qualification)); // Explicit raw branch input, not valid-UI evidence.
    CHECK(getter.release().issue == Issue::DataDestructionPending);
    CHECK(getter.report().stage == Stage::Released); REQUIRE(getter.releaseReceipt());
    CHECK(getter.releaseReceipt()->beforeBits == 1u); CHECK(getter.releaseReceipt()->afterBits == 0u);
    CHECK(getter.releaseReceipt()->nonzero == false); CHECK(data->currentBits() == 0u);
    CHECK(data->destructionPending()); CHECK(data->nativeOwnerAlive());
    CHECK(data->nativeValueBalance() == 0u); CHECK(data->mutationSequence() == 2u);
    for (int retry = 0; retry != 3; ++retry) {
        CHECK(getter.release().issue == Issue::DataDestructionPending);
        CHECK(getter.retain().issue == Issue::DataDestructionPending);
        CHECK(data->mutationSequence() == 2u); CHECK(data->derefMutations() == 1u);
    }
    PaletteNativeValueRef destination(data, Kind::PrivateDestination);
    CHECK(destination.retain().issue == Issue::DataDestructionPending);
    CHECK(destination.report().stage == Stage::AwaitingRetain); CHECK(!destination.retainReceipt());
    CHECK(data->observeCurrent(0, 2, qualification)); CHECK(!data->observeCurrent(2, 2, qualification));
    CHECK(data->destructionPending()); CHECK(data->mutationSequence() == 2u);
}

TEST_CASE(palette_native_ref_deref_underflow_and_ignored_destination_ref_zero_use_raw32)
{
    const auto data = state(); PaletteNativeValueRef getter(data, Kind::GetterTemporary);
    CHECK(getter.retain().issue == Issue::None); REQUIRE(data->observeCurrent(0, 1, qualification));
    CHECK(getter.release().issue == Issue::None); REQUIRE(getter.releaseReceipt());
    CHECK(getter.releaseReceipt()->afterBits == 0xffffffffu);
    CHECK(getter.releaseReceipt()->nonzero == true); CHECK(!data->destructionPending());
    PaletteNativeValueRef destination(data, Kind::PrivateDestination);
    CHECK(destination.retain().issue == Issue::None); REQUIRE(destination.retainReceipt());
    CHECK(destination.retainReceipt()->afterBits == 0u);
    CHECK(destination.retainReceipt()->nonzero == false);
    CHECK(destination.report().stage == Stage::Held); CHECK(!data->destructionPending());
    CHECK(data->currentBits() == 0u); CHECK(data->mutationSequence() == 3u);
    CHECK(data->nativeValueBalance() == 1u);
}

TEST_CASE(palette_native_ref_signed_boundary_and_negative_bits_have_no_special_case)
{
    for (const std::uint32_t initial : {0x7fffffffu, 0xfffffffeu}) {
        const auto data = state(); REQUIRE(data->observeCurrent(initial, 0, qualification));
        PaletteNativeValueRef getter(data, Kind::GetterTemporary);
        CHECK(getter.retain().issue == Issue::None); REQUIRE(getter.retainReceipt());
        CHECK(getter.retainReceipt()->afterBits == initial + std::uint32_t{1});
        CHECK(getter.retainReceipt()->nonzero == true);
        CHECK(getter.release().issue == Issue::None); CHECK(data->currentBits() == initial);
        CHECK(data->mutationSequence() == 2u); CHECK(!data->destructionPending());
    }
}

TEST_CASE(palette_native_ref_getter_ref_wrap_false_is_diagnostic_not_a_refusal)
{
    const auto data = state(); REQUIRE(data->observeCurrent(0xffffffffu, 0, qualification));
    PaletteNativeValueRef getter(data, Kind::GetterTemporary);
    CHECK(getter.retain().issue == Issue::None); REQUIRE(getter.retainReceipt());
    CHECK(getter.retainReceipt()->nonzero == false); CHECK(getter.report().stage == Stage::Held);
    CHECK(data->currentBits() == 0u); CHECK(!data->destructionPending());
    CHECK(getter.release().issue == Issue::None); CHECK(data->currentBits() == 0xffffffffu);
}

TEST_CASE(palette_native_ref_two_frames_share_numeric_state_but_convert_their_own_tokens)
{
    const auto data = state(); REQUIRE(data->observeCurrent(2, 0, qualification));
    PaletteNativeValueRef first(data, Kind::GetterTemporary), second(data, Kind::GetterTemporary);
    CHECK(first.retain().issue == Issue::None); CHECK(second.retain().issue == Issue::None);
    CHECK(data->currentBits() == 4u); CHECK(data->mutationSequence() == 2u);
    CHECK(first.release().issue == Issue::None);
    PaletteNativeValueRef firstDestination(data, Kind::PrivateDestination);
    CHECK(firstDestination.retain().issue == Issue::None); CHECK(data->currentBits() == 4u);
    CHECK(second.release().issue == Issue::None);
    PaletteNativeValueRef secondDestination(data, Kind::PrivateDestination);
    CHECK(secondDestination.retain().issue == Issue::None); CHECK(data->currentBits() == 4u);
    CHECK(data->mutationSequence() == 6u); CHECK(data->refMutations() == 4u);
    CHECK(data->derefMutations() == 2u); CHECK(data->nativeValueBalance() == 2u);
    CHECK(first.release().issue == Issue::None); CHECK(second.release().issue == Issue::None);
    CHECK(data->mutationSequence() == 6u); CHECK(data->currentBits() == 4u);
}

TEST_CASE(palette_native_ref_late_snapshot_already_contains_both_unknown_getter_refs)
{
    const auto data = state(); PaletteNativeValueRef first(data, Kind::GetterTemporary), second(data, Kind::GetterTemporary);
    CHECK(first.retain().issue == Issue::None); CHECK(second.retain().issue == Issue::None);
    CHECK(!data->currentBits()); REQUIRE(data->observeCurrent(4, 2, qualification));
    CHECK(first.release().issue == Issue::None); CHECK(second.release().issue == Issue::None);
    CHECK(data->currentBits() == 2u); CHECK(data->mutationSequence() == 4u);
    CHECK(data->refMutations() == 2u); CHECK(data->nativeValueBalance() == 0u);
    CHECK(!data->observeCurrent(4, 2, qualification));
}

TEST_CASE(palette_native_ref_host_copies_and_token_destruction_never_mutate_native_count)
{
    const auto data = state(); REQUIRE(data->observeCurrent(2, 0, qualification));
    {
        auto getter = std::make_shared<PaletteNativeValueRef>(data, Kind::GetterTemporary);
        CHECK(getter->retain().issue == Issue::None);
        auto copiedDiagnostic = getter; getter.reset();
        CHECK(data->currentBits() == 3u); CHECK(data->mutationSequence() == 1u);
        CHECK(copiedDiagnostic->retain().issue == Issue::None);
        CHECK(data->refMutations() == 1u);
    }
    // Original native unwind remains unclosed; this is not an automatic deref.
    CHECK(data->currentBits() == 3u); CHECK(data->mutationSequence() == 1u);
    CHECK(data->derefMutations() == 0u); CHECK(data->nativeValueBalance() == 1u);
}

TEST_CASE(palette_native_ref_missing_unqualified_and_foreign_state_cannot_acquire_refs)
{
    PaletteNativeValueRef missing({}, Kind::GetterTemporary);
    CHECK(missing.retain().issue == Issue::StateUnavailable); CHECK(!missing.retainReceipt());
    const auto unqualified = std::make_shared<PaletteNativeRefState>("");
    PaletteNativeValueRef noReceipt(unqualified, Kind::GetterTemporary);
    CHECK(noReceipt.retain().issue == Issue::MissingQualification);
    CHECK(!unqualified->observeCurrent(2, 0, qualification)); CHECK(unqualified->mutationSequence() == 0u);
    auto actual = state(); std::shared_ptr<PaletteNativeRefState> foreign(actual.get(), [](PaletteNativeRefState*) {});
    PaletteNativeValueRef rejected(foreign, Kind::GetterTemporary);
    CHECK(rejected.retain().issue == Issue::StateOwnerMismatch);
    CHECK(actual->mutationSequence() == 0u); actual.reset();
    CHECK(rejected.release().issue == Issue::StateOwnerMismatch); CHECK(!rejected.retainReceipt());
}

TEST_CASE(palette_native_ref_unretained_release_and_lost_owner_keep_history_without_mutation)
{
    auto data = state(); REQUIRE(data->observeCurrent(2, 0, qualification));
    PaletteNativeValueRef getter(data, Kind::GetterTemporary);
    CHECK(getter.release().issue == Issue::ValueNotRetained); CHECK(data->mutationSequence() == 0u);
    CHECK(getter.retain().issue == Issue::None); REQUIRE(getter.retainReceipt());
    const auto sequence = getter.retainReceipt()->sequence;
    data->releaseNativeOwner();
    CHECK(getter.release().issue == Issue::StateUnavailable); CHECK(getter.report().stage == Stage::Held);
    CHECK(!getter.releaseReceipt()); CHECK(!data->observeCurrent(3, 1, qualification));
    CHECK(data->currentBits() == 3u); CHECK(data->nativeValueBalance() == 1u);
    data.reset(); CHECK(getter.stateOwner().expired());
    CHECK(getter.retain().issue == Issue::StateUnavailable);
    CHECK(getter.retainReceipt()->sequence == sequence); CHECK(!getter.releaseReceipt());
}

TEST_CASE(palette_native_ref_other_state_with_equal_qualification_is_not_the_token_owner)
{
    const auto first = state(), other = state();
    REQUIRE(first->observeCurrent(2, 0, qualification)); REQUIRE(other->observeCurrent(2, 0, qualification));
    PaletteNativeValueRef getter(first, Kind::GetterTemporary), otherGetter(other, Kind::GetterTemporary);
    CHECK(getter.retain().issue == Issue::None); CHECK(otherGetter.retain().issue == Issue::None);
    CHECK(getter.stateOwner().lock() == first); CHECK(otherGetter.stateOwner().lock() == other);
    first->releaseNativeOwner(); CHECK(getter.release().issue == Issue::StateUnavailable);
    CHECK(otherGetter.release().issue == Issue::None);
    CHECK(first->currentBits() == 3u); CHECK(other->currentBits() == 2u);
    CHECK(first->mutationSequence() == 1u); CHECK(other->mutationSequence() == 2u);
}

int main() { return testkit::run_all(); }
