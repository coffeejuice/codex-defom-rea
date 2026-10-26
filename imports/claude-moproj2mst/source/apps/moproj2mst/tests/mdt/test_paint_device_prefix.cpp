// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include <memory>
#include <string>
#include "m2m/engine/paint_device_prefix.hpp"
#include "m2m/engine/paint_device_cleanup.hpp"
#include "m2m/engine/brush_texture_image.hpp"

using namespace m2m::engine;
namespace {
using Stage = PaintDevicePrefixStage;
using Issue = PaintDevicePrefixIssue;
using Effect = PaintDevicePrefixEffect;
constexpr auto qualification = "Isolated returned image temporary";
const std::vector zeroEffects{Effect::PaintingWordRead, Effect::BaseClassInstalled, Effect::CleanupTailReached};
const std::vector warningEffects{Effect::PaintingWordRead, Effect::BaseClassInstalled};
struct Fixture {
    std::shared_ptr<PaintDevicePrefixState> state = std::make_shared<PaintDevicePrefixState>(qualification);
    PaintDevicePrefixCall call{state};
    PaintDevicePrefixObservation observation(std::optional<std::uint16_t> word = std::nullopt) const {
        PaintDevicePrefixObservation o; o.receiver = state; o.qualification = qualification; o.paintingWord = word;
        return o;
    }
};
PaintDeviceCleanupGlobalObservation globalObservation(const std::shared_ptr<PaintDeviceCleanupGlobals>& globals) {
    PaintDeviceCleanupGlobalObservation o; o.receiver = globals; o.qualification = globals->qualification();
    o.moduleSha256 = "c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887";
    o.pointerRva = 0x9cb858u; return o;
}
}

TEST_CASE(paint_prefix_constructed_returned_image_zero_reaches_only_cleanup_tail)
{
    auto state = PaintDevicePrefixState::fromReturnedImageConstruction(qualification); REQUIRE(state);
    CHECK(state->qualified()); CHECK(state->paintingWord() == 0);
    CHECK(state->classIdentity() == PaintDevicePrefixClass::QImage);
    PaintDevicePrefixCall call{state}; CHECK(call.effects().empty());
    CHECK(call.resume().stage == Stage::TailReady); CHECK(call.report().issue == Issue::None);
    CHECK(call.report().readPaintingWord == 0); CHECK(call.effects() == zeroEffects);
    CHECK(state->paintingWord() == 0); CHECK(state->classIdentity() == PaintDevicePrefixClass::QPaintDevice);
    CHECK(state->nativeOwnerAlive()); CHECK(!call.report().completeForPublicSave);
    CHECK(call.resume().stage == Stage::TailReady); CHECK(call.effects() == zeroEffects);
}

TEST_CASE(paint_prefix_missing_and_unqualified_owners_emit_no_effects)
{
    PaintDevicePrefixCall missing{nullptr}; CHECK(missing.resume().issue == Issue::OwnerUnavailable);
    CHECK(missing.effects().empty()); Fixture f;
    CHECK(f.call.resume().issue == Issue::QualificationUnavailable); CHECK(f.call.effects().empty());
    CHECK(!f.state->paintingWord()); CHECK(f.state->classIdentity() == PaintDevicePrefixClass::Unprojected);
    CHECK(!PaintDevicePrefixState::fromReturnedImageConstruction(""));
}

TEST_CASE(paint_prefix_unknown_word_guards_before_class_store)
{
    Fixture f; REQUIRE(f.state->observe(f.observation()));
    CHECK(f.call.resume().issue == Issue::PaintingWordUnavailable);
    CHECK(f.call.report().stage == Stage::PaintingWord); CHECK(!f.call.report().readPaintingWord);
    CHECK(f.call.effects().empty()); CHECK(f.state->classIdentity() == PaintDevicePrefixClass::Unprojected);
    CHECK(f.call.resume().issue == Issue::PaintingWordUnavailable); CHECK(f.call.effects().empty());
}

TEST_CASE(paint_prefix_late_zero_observation_does_not_resume_or_replay)
{
    Fixture f; REQUIRE(f.state->observe(f.observation()));
    CHECK(f.call.resume().issue == Issue::PaintingWordUnavailable);
    REQUIRE(f.state->observe(f.observation(0))); CHECK(f.call.effects().empty());
    CHECK(f.call.report().stage == Stage::PaintingWord);
    CHECK(f.state->classIdentity() == PaintDevicePrefixClass::Unprojected);
    CHECK(f.call.resume().stage == Stage::TailReady); CHECK(f.call.effects() == zeroEffects);
    REQUIRE(f.state->observe(f.observation(0))); REQUIRE(f.state->observe(f.observation()));
    CHECK(f.call.resume().stage == Stage::TailReady); CHECK(f.call.effects() == zeroEffects);
}

TEST_CASE(paint_prefix_all_nonzero_words_install_base_before_warning_guard)
{
    for (unsigned word : {1u, 2u, 0x7fffu, 0x8000u, 0xffffu}) {
        Fixture f; REQUIRE(f.state->observe(f.observation(static_cast<std::uint16_t>(word))));
        CHECK(f.call.resume().issue == Issue::WarningUnrecovered);
        CHECK(f.call.report().stage == Stage::WarningContinuation);
        CHECK(f.call.report().readPaintingWord == word); CHECK(f.call.effects() == warningEffects);
        CHECK(f.state->classIdentity() == PaintDevicePrefixClass::QPaintDevice);
        CHECK(f.state->paintingWord() == word);
        CHECK(!f.state->observe(f.observation(0))); CHECK(f.state->paintingWord() == word);
        CHECK(f.call.resume().issue == Issue::WarningUnrecovered); CHECK(f.call.effects() == warningEffects);
    }
}

TEST_CASE(paint_prefix_invalid_qualification_does_not_publish_word_or_profile)
{
    Fixture f;
    for (const std::string q : {std::string{}, std::string{"Other temporary"}}) {
        auto o = f.observation(0); o.qualification = q;
        CHECK(!f.state->observe(o)); CHECK(!f.state->qualified()); CHECK(!f.state->paintingWord());
    }
    auto empty = std::make_shared<PaintDevicePrefixState>("");
    PaintDevicePrefixObservation o; o.receiver = empty; o.paintingWord = 0;
    CHECK(!empty->observe(o)); CHECK(!empty->qualified()); CHECK(!empty->paintingWord());
    CHECK(f.call.effects().empty());
}

TEST_CASE(paint_prefix_foreign_rewrapped_and_missing_observation_receivers_rejected)
{
    Fixture f, other; auto o = f.observation(0); o.receiver = other.state;
    CHECK(!f.state->observe(o)); CHECK(!f.state->qualified()); CHECK(!other.state->qualified());
    std::shared_ptr<PaintDevicePrefixState> rewrapped(f.state.get(), [](PaintDevicePrefixState*) {});
    o.receiver = rewrapped; CHECK(!f.state->observe(o)); CHECK(!f.state->paintingWord());
    o.receiver.reset(); CHECK(!f.state->observe(o)); CHECK(!f.state->qualified());
}

TEST_CASE(paint_prefix_rewrapped_call_cannot_execute_actual_temporary)
{
    auto state = PaintDevicePrefixState::fromReturnedImageConstruction(qualification); REQUIRE(state);
    std::shared_ptr<PaintDevicePrefixState> rewrapped(state.get(), [](PaintDevicePrefixState*) {});
    PaintDevicePrefixCall foreign{rewrapped}; CHECK(foreign.resume().issue == Issue::OwnerUnavailable);
    CHECK(foreign.effects().empty()); CHECK(state->classIdentity() == PaintDevicePrefixClass::QImage);
    PaintDevicePrefixCall actual{state}; CHECK(actual.resume().stage == Stage::TailReady);
}

TEST_CASE(paint_prefix_conflicting_word_rejected_without_changing_completed_class)
{
    auto state = PaintDevicePrefixState::fromReturnedImageConstruction(qualification); REQUIRE(state);
    PaintDevicePrefixCall call{state}; CHECK(call.resume().stage == Stage::TailReady);
    PaintDevicePrefixObservation o; o.receiver = state; o.qualification = qualification; o.paintingWord = 1;
    CHECK(!state->observe(o)); CHECK(state->paintingWord() == 0);
    CHECK(state->classIdentity() == PaintDevicePrefixClass::QPaintDevice);
    CHECK(call.effects() == zeroEffects); CHECK(call.report().readPaintingWord == 0);
}

TEST_CASE(paint_prefix_expired_temporary_cannot_be_kept_alive_by_call)
{
    std::unique_ptr<PaintDevicePrefixCall> call;
    PaintDevicePrefixObservation o;
    {
        auto state = PaintDevicePrefixState::fromReturnedImageConstruction(qualification);
        o.receiver = state; call = std::make_unique<PaintDevicePrefixCall>(state);
    }
    CHECK(o.receiver.expired()); CHECK(call->resume().issue == Issue::OwnerUnavailable);
    CHECK(call->effects().empty());
}

TEST_CASE(paint_prefix_released_native_owner_blocks_late_refinement_despite_host_lease)
{
    Fixture f; REQUIRE(f.state->observe(f.observation()));
    CHECK(f.call.resume().issue == Issue::PaintingWordUnavailable); auto diagnostic = f.state;
    f.state->releaseNativeOwner(); CHECK(!diagnostic->nativeOwnerAlive());
    CHECK(!f.state->observe(f.observation(0))); CHECK(!diagnostic->paintingWord());
    CHECK(f.call.resume().issue == Issue::OwnerUnavailable); CHECK(f.call.effects().empty());
    CHECK(diagnostic->classIdentity() == PaintDevicePrefixClass::Unprojected);
}

TEST_CASE(paint_prefix_distinct_values_do_not_share_prefix_or_image_reference_effects)
{
    auto data = std::make_shared<BrushTextureImageDataState>(31);
    auto first = PaintDevicePrefixState::fromReturnedImageConstruction(qualification);
    auto second = PaintDevicePrefixState::fromReturnedImageConstruction(qualification);
    REQUIRE(first); REQUIRE(second); CHECK(first != second);
    PaintDevicePrefixCall a{first}, b{second}; CHECK(a.resume().stage == Stage::TailReady);
    CHECK(b.effects().empty()); CHECK(second->classIdentity() == PaintDevicePrefixClass::QImage);
    CHECK(b.resume().stage == Stage::TailReady); CHECK(a.effects() == zeroEffects); CHECK(b.effects() == zeroEffects);
    CHECK(data->temporaryValueReferences() == 0u); // No image-data owner input/ref operation in this helper.
    first->releaseNativeOwner(); CHECK(second->nativeOwnerAlive());
}

TEST_CASE(paint_prefix_abandonment_after_class_install_cannot_replay_on_new_call)
{
    Fixture f; REQUIRE(f.state->observe(f.observation(1)));
    {
        PaintDevicePrefixCall first{f.state}; CHECK(first.resume().issue == Issue::WarningUnrecovered);
        CHECK(first.effects() == warningEffects);
    }
    CHECK(f.state->classIdentity() == PaintDevicePrefixClass::QPaintDevice);
    PaintDevicePrefixCall later{f.state}; CHECK(later.resume().issue == Issue::PrefixAlreadyStarted);
    CHECK(later.effects().empty()); CHECK(!later.report().readPaintingWord);
}

TEST_CASE(paint_prefix_tail_receipt_does_not_authorize_second_invocation)
{
    auto state = PaintDevicePrefixState::fromReturnedImageConstruction(qualification); REQUIRE(state);
    PaintDevicePrefixCall first{state}, later{state}; CHECK(first.resume().stage == Stage::TailReady);
    CHECK(later.resume().issue == Issue::PrefixAlreadyStarted); CHECK(later.effects().empty());
    state->releaseNativeOwner(); CHECK(first.resume().stage == Stage::TailReady);
    CHECK(first.effects() == zeroEffects); CHECK(later.resume().issue == Issue::OwnerUnavailable);
}

TEST_CASE(paint_prefix_pending_frames_cannot_both_claim_one_later_qualified_value)
{
    Fixture f; REQUIRE(f.state->observe(f.observation())); PaintDevicePrefixCall second{f.state};
    CHECK(f.call.resume().issue == Issue::PaintingWordUnavailable);
    CHECK(second.resume().issue == Issue::PaintingWordUnavailable);
    REQUIRE(f.state->observe(f.observation(0))); CHECK(second.resume().stage == Stage::TailReady);
    CHECK(f.call.resume().issue == Issue::PrefixAlreadyStarted); CHECK(f.call.effects().empty());
}

TEST_CASE(paint_prefix_abandonment_before_word_read_does_not_claim_native_prefix)
{
    Fixture f; REQUIRE(f.state->observe(f.observation()));
    {
        PaintDevicePrefixCall abandoned{f.state};
        CHECK(abandoned.resume().issue == Issue::PaintingWordUnavailable); CHECK(abandoned.effects().empty());
    }
    REQUIRE(f.state->observe(f.observation(0))); CHECK(f.call.resume().stage == Stage::TailReady);
    CHECK(f.call.effects() == zeroEffects);
}

TEST_CASE(paint_prefix_tail_ready_remains_separate_from_global_cleanup_completion)
{
    for (int branch = 0; branch != 5; ++branch) {
        auto state = PaintDevicePrefixState::fromReturnedImageConstruction(qualification); REQUIRE(state);
        PaintDevicePrefixCall prefix{state}; CHECK(prefix.resume().stage == Stage::TailReady);
        auto globals = std::make_shared<PaintDeviceCleanupGlobals>("Independent runtime globals");
        auto o = globalObservation(globals);
        if (branch == 1) o.currentCounterAlias = 0;
        if (branch >= 2) { o.counters = {{31, branch == 2 ? std::nullopt : std::optional<std::int32_t>{branch == 3 ? 1 : 0}}}; o.currentCounterAlias = 31; }
        REQUIRE(globals->observe(o)); PaintDeviceCleanupCall cleanup{globals};
        const auto result = cleanup.resume();
        CHECK((result.stage == PaintDeviceCleanupStage::Returned) == (branch == 4));
        if (branch != 4) CHECK(result.issue != PaintDeviceCleanupIssue::None);
        CHECK(prefix.resume().stage == Stage::TailReady); CHECK(prefix.effects() == zeroEffects);
        CHECK(!prefix.report().completeForPublicSave);
    }
}

TEST_CASE(paint_prefix_nonzero_word_does_not_reach_tail_despite_zero_global_counter)
{
    Fixture f; REQUIRE(f.state->observe(f.observation(1)));
    auto globals = std::make_shared<PaintDeviceCleanupGlobals>("Independent runtime globals");
    auto o = globalObservation(globals); o.counters = {{31, 0}}; o.currentCounterAlias = 31;
    REQUIRE(globals->observe(o)); PaintDeviceCleanupCall cleanup{globals};
    CHECK(f.call.resume().issue == Issue::WarningUnrecovered); CHECK(f.call.effects() == warningEffects);
    CHECK(cleanup.effects().empty()); CHECK(cleanup.report().stage == PaintDeviceCleanupStage::FirstGetter);
    CHECK(f.call.resume().stage == Stage::WarningContinuation); CHECK(cleanup.effects().empty());
}

int main() { return testkit::run_all(); }
