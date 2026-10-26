// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include <algorithm>
#include <memory>
#include <utility>
#include "m2m/engine/paint_device_active_copy.hpp"

using namespace m2m::engine;
namespace {
using Stage = PaintDeviceActiveCopyStage;
using Issue = PaintDeviceActiveCopyIssue;
using Effect = PaintDeviceActiveCopyEffect;
constexpr auto sourceQualification = "Isolated actual source QImage";
const std::vector expectedPending{
    Effect::OuterImageConstructed, Effect::SourcePaintingWordRead, Effect::OuterNullInitialized,
    Effect::DefaultRectangleConstructed, Effect::CopySourceDataRead, Effect::InnerImageConstructed,
    Effect::AssignmentInnerWordRead, Effect::AssignmentInnerDataRead, Effect::AssignmentOuterDataRead,
    Effect::AssignmentInnerDataReread, Effect::OuterNullAssigned,
    Effect::InnerQImageClassInstalled, Effect::InnerImageDataRead, Effect::InnerPrefixTailReached};
const std::vector expectedReturned = [] {
    auto r = expectedPending; r.push_back(Effect::InnerCleanupReturned);
    r.push_back(Effect::InnerImageDestroyed); r.push_back(Effect::ActiveCopyReturned); return r;
}();
struct Fixture {
    std::shared_ptr<PaintDeviceActiveCopySourceState> source = std::make_shared<PaintDeviceActiveCopySourceState>(sourceQualification);
    std::shared_ptr<PaintDeviceCleanupGlobals> globals = std::make_shared<PaintDeviceCleanupGlobals>("Actual QtGui globals");
    PaintDeviceActiveCopyCall call{source, "Copy frame"};
    PaintDeviceActiveCopySourceObservation observation() const {
        PaintDeviceActiveCopySourceObservation o; o.receiver = source; o.qualification = sourceQualification; return o;
    }
    void nullSource(std::optional<std::uint16_t> word = 1) {
        auto o = observation(); o.paintingWord = word; o.imageData = std::shared_ptr<BrushTextureImageDataState>{};
        REQUIRE(source->observe(o));
    }
    PaintDeviceCleanupGlobalObservation globalObservation() const {
        PaintDeviceCleanupGlobalObservation o; o.receiver = globals; o.qualification = globals->qualification();
        o.moduleSha256 = "c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887";
        o.pointerRva = 0x9cb858u; return o;
    }
    void existing(std::optional<std::int32_t> value) {
        auto o = globalObservation(); o.counters = {{31, value}}; o.currentCounterAlias = 31;
        REQUIRE(globals->observe(o)); REQUIRE(call.bindCleanupGlobals(globals));
    }
};
}

TEST_CASE(active_copy_null_source_returns_only_after_inner_prefix_and_cleanup)
{
    for (unsigned word : {1u, 0x8000u, 0xffffu}) {
        Fixture f; f.nullSource(static_cast<std::uint16_t>(word)); f.existing(0);
        CHECK(f.call.resume().stage == Stage::Returned); CHECK(f.call.report().issue == Issue::None);
        CHECK(f.call.effects() == expectedReturned); CHECK(f.call.report().readSourcePaintingWord == word);
        CHECK(f.source->paintingWord() == word); REQUIRE(f.source->imageData()); CHECK(!*f.source->imageData());
        REQUIRE(f.call.defaultRectangle()); const std::array<std::int32_t, 4> rectangle{0, 0, -1, -1};
        CHECK(*f.call.defaultRectangle() == rectangle);
        auto outer = f.call.returnedImagePrefixState(), inner = f.call.innerImagePrefixState();
        REQUIRE(outer); REQUIRE(inner); CHECK(outer != inner); CHECK(outer->paintingWord() == 0); CHECK(inner->paintingWord() == 0);
        CHECK(outer->classIdentity() == PaintDevicePrefixClass::QImage); CHECK(outer->nativeOwnerAlive());
        CHECK(inner->classIdentity() == PaintDevicePrefixClass::QPaintDevice); CHECK(!inner->nativeOwnerAlive());
        REQUIRE(f.call.innerPrefixCall()); REQUIRE(f.call.innerCleanupCall());
        CHECK(f.call.innerPrefixCall()->report().stage == PaintDevicePrefixStage::TailReady);
        CHECK(f.call.innerCleanupCall()->report().stage == PaintDeviceCleanupStage::Returned);
        CHECK(!f.call.report().completeForPublicSave); CHECK(f.call.resume().stage == Stage::Returned);
        CHECK(f.call.effects() == expectedReturned);
    }
}

TEST_CASE(active_copy_missing_qualification_does_not_construct_outer_value)
{
    PaintDeviceActiveCopyCall absent{nullptr, "Frame"}; CHECK(absent.resume().issue == Issue::SourceUnavailable);
    CHECK(absent.effects().empty()); Fixture f;
    CHECK(f.call.resume().issue == Issue::SourceQualificationUnavailable); CHECK(!f.call.returnedImagePrefixState());
    f.nullSource(); PaintDeviceActiveCopyCall empty{f.source, ""};
    CHECK(empty.resume().issue == Issue::CallQualificationUnavailable); CHECK(empty.effects().empty());
}

TEST_CASE(active_copy_unknown_word_holds_after_outer_construction_without_replay)
{
    Fixture f; f.nullSource(std::nullopt);
    CHECK(f.call.resume().issue == Issue::SourcePaintingWordUnavailable);
    const std::vector expected{Effect::OuterImageConstructed}; CHECK(f.call.effects() == expected);
    auto outer = f.call.returnedImagePrefixState(); REQUIRE(outer); CHECK(outer->paintingWord() == 0);
    CHECK(!f.call.defaultRectangle()); CHECK(!f.call.innerImagePrefixState()); CHECK(!f.call.takeReturnedImage());
    f.nullSource(1); CHECK(f.call.effects() == expected); CHECK(f.call.returnedImagePrefixState() == outer);
    CHECK(f.call.resume().issue == Issue::CleanupGlobalsUnavailable); CHECK(f.call.effects() == expectedPending);
}

TEST_CASE(active_copy_unknown_data_waits_after_reached_rectangle_and_null_initialization)
{
    Fixture f; auto o = f.observation(); o.paintingWord = 1; REQUIRE(f.source->observe(o));
    CHECK(f.call.resume().issue == Issue::SourceImageDataUnavailable); CHECK(f.call.report().stage == Stage::CopySourceData);
    const std::vector expected{Effect::OuterImageConstructed, Effect::SourcePaintingWordRead,
        Effect::OuterNullInitialized, Effect::DefaultRectangleConstructed};
    CHECK(f.call.effects() == expected); REQUIRE(f.call.defaultRectangle()); CHECK(!f.call.innerImagePrefixState());
    CHECK(f.call.resume().issue == Issue::SourceImageDataUnavailable); CHECK(f.call.effects() == expected);
    f.nullSource(); CHECK(f.call.effects() == expected); CHECK(f.call.resume().issue == Issue::CleanupGlobalsUnavailable);
    CHECK(f.call.effects() == expectedPending);
}

TEST_CASE(active_copy_inactive_and_nonnull_sources_keep_distinct_continuation_guards)
{
    Fixture inactive; inactive.nullSource(0);
    CHECK(inactive.call.resume().issue == Issue::InactiveSourceUnrecovered);
    CHECK(inactive.call.report().stage == Stage::InactiveSourceContinuation); CHECK(!inactive.call.defaultRectangle());
    CHECK(inactive.call.effects().size() == 2u); CHECK(!inactive.call.innerImagePrefixState());
    const auto inactiveEffects = inactive.call.effects(); CHECK(inactive.call.resume().issue == Issue::InactiveSourceUnrecovered);
    CHECK(inactive.call.effects() == inactiveEffects);
    Fixture active; auto data = std::make_shared<BrushTextureImageDataState>(31);
    auto o = active.observation(); o.paintingWord = 1; o.imageData = data; REQUIRE(active.source->observe(o));
    CHECK(active.call.resume().issue == Issue::NonNullActiveCopyUnrecovered);
    CHECK(active.call.report().stage == Stage::NonNullSourceContinuation); REQUIRE(active.call.defaultRectangle());
    CHECK(active.call.effects().size() == 5u); CHECK(!active.call.innerImagePrefixState());
    const auto activeEffects = active.call.effects(); CHECK(active.call.resume().issue == Issue::NonNullActiveCopyUnrecovered);
    CHECK(active.call.effects() == activeEffects); CHECK(data->temporaryValueReferences() == 0u);
    CHECK(*active.source->imageData() == data);
}

TEST_CASE(active_copy_late_global_binding_does_not_publish_return_or_repeat_assignment)
{
    Fixture f; f.nullSource(); CHECK(f.call.resume().issue == Issue::CleanupGlobalsUnavailable);
    CHECK(f.call.report().stage == Stage::InnerCleanup); CHECK(f.call.effects() == expectedPending);
    CHECK(!f.call.innerCleanupCall()); CHECK(!f.call.takeReturnedImage());
    auto inner = f.call.innerImagePrefixState(), outer = f.call.returnedImagePrefixState();
    REQUIRE(inner); REQUIRE(outer); CHECK(inner->nativeOwnerAlive()); CHECK(outer->nativeOwnerAlive());
    f.existing(0); CHECK(f.call.effects() == expectedPending); CHECK(f.call.report().stage == Stage::InnerCleanup);
    CHECK(f.call.resume().stage == Stage::Returned); CHECK(f.call.effects() == expectedReturned);
    CHECK(f.call.innerImagePrefixState() == inner); CHECK(f.call.returnedImagePrefixState() == outer);
}

TEST_CASE(active_copy_unknown_pointer_and_null_pointer_retain_cleanup_guards)
{
    for (bool nullPointer : {false, true}) {
        Fixture f; f.nullSource(); auto o = f.globalObservation(); if (nullPointer) o.currentCounterAlias = 0;
        REQUIRE(f.globals->observe(o)); REQUIRE(f.call.bindCleanupGlobals(f.globals));
        CHECK(f.call.resume().issue == Issue::InnerCleanupPending); CHECK(f.call.effects() == expectedPending);
        REQUIRE(f.call.innerCleanupCall()); const auto expectedIssue = nullPointer
            ? PaintDeviceCleanupIssue::LazyGlobalUnrecovered : PaintDeviceCleanupIssue::CounterPointerUnavailable;
        CHECK(f.call.innerCleanupCall()->report().issue == expectedIssue);
        CHECK(f.call.innerCleanupCall()->effects().empty()); CHECK(!f.call.takeReturnedImage());
        CHECK(f.call.resume().issue == Issue::InnerCleanupPending); CHECK(f.call.effects() == expectedPending);
    }
}

TEST_CASE(active_copy_late_counter_value_resumes_same_inner_cleanup_frame)
{
    Fixture f; f.nullSource(); f.existing(std::nullopt);
    CHECK(f.call.resume().issue == Issue::InnerCleanupPending); const auto* cleanup = f.call.innerCleanupCall(); REQUIRE(cleanup);
    CHECK(cleanup->report().issue == PaintDeviceCleanupIssue::CounterValueUnavailable); CHECK(cleanup->effects().size() == 2u);
    f.existing(0); CHECK(f.call.effects() == expectedPending); CHECK(f.call.innerCleanupCall() == cleanup);
    CHECK(f.call.resume().stage == Stage::Returned); CHECK(f.call.effects() == expectedReturned);
    CHECK(cleanup->effects().size() == 3u); CHECK(f.globals->counterFor(31)->value() == 0);
}

TEST_CASE(active_copy_nonzero_counter_stops_before_copy_return_without_image_ref_effects)
{
    for (std::int32_t n : {-1, 1, -2147483647, 2147483647}) {
        Fixture f; f.nullSource(); f.existing(n); auto unrelatedData = std::make_shared<BrushTextureImageDataState>(31);
        CHECK(f.call.resume().issue == Issue::InnerCleanupPending); REQUIRE(f.call.innerCleanupCall());
        CHECK(f.call.innerCleanupCall()->report().issue == PaintDeviceCleanupIssue::NonzeroCleanupUnrecovered);
        CHECK(f.call.effects() == expectedPending); CHECK(f.call.innerCleanupCall()->effects().size() == 3u);
        CHECK(f.call.resume().issue == Issue::InnerCleanupPending); CHECK(f.call.effects() == expectedPending);
        CHECK(f.globals->counterFor(31)->value() == n); CHECK(unrelatedData->temporaryValueReferences() == 0u);
        CHECK(f.call.innerImagePrefixState()->nativeOwnerAlive()); CHECK(!f.call.takeReturnedImage());
    }
}

TEST_CASE(active_copy_binding_requires_live_qualified_canonical_global_owner)
{
    Fixture f; f.nullSource(); CHECK(!f.call.bindCleanupGlobals(nullptr)); CHECK(!f.call.bindCleanupGlobals(f.globals));
    REQUIRE(f.globals->observe(f.globalObservation()));
    std::shared_ptr<PaintDeviceCleanupGlobals> rewrapped(f.globals.get(), [](PaintDeviceCleanupGlobals*) {});
    CHECK(!f.call.bindCleanupGlobals(rewrapped)); REQUIRE(f.call.bindCleanupGlobals(f.globals));
    REQUIRE(f.call.bindCleanupGlobals(f.globals)); Fixture other;
    REQUIRE(other.globals->observe(other.globalObservation())); CHECK(!f.call.bindCleanupGlobals(other.globals));
    CHECK(f.call.effects().empty()); f.globals->releaseNativeOwner(); CHECK(!f.call.bindCleanupGlobals(f.globals));
    CHECK(!f.call.bindCleanupGlobals(other.globals)); CHECK(f.call.resume().issue == Issue::CleanupGlobalsUnavailable);
}

TEST_CASE(active_copy_source_observation_rejects_invalid_envelope_atomically)
{
    Fixture f, other; auto o = f.observation(); o.paintingWord = 1; o.imageData = std::make_shared<BrushTextureImageDataState>(0);
    CHECK(!f.source->observe(o)); CHECK(!f.source->qualified()); CHECK(!f.source->paintingWord());
    CHECK(!f.source->imageData()); o.imageData = std::shared_ptr<BrushTextureImageDataState>{};
    o.receiver = other.source; CHECK(!f.source->observe(o));
    std::shared_ptr<PaintDeviceActiveCopySourceState> rewrapped(f.source.get(), [](PaintDeviceActiveCopySourceState*) {});
    o.receiver = rewrapped; CHECK(!f.source->observe(o)); o.receiver.reset(); CHECK(!f.source->observe(o));
    o = f.observation(); o.paintingWord = 1; o.qualification = "Other source"; CHECK(!f.source->observe(o));
    CHECK(!f.source->qualified()); CHECK(f.call.effects().empty());
}

TEST_CASE(active_copy_source_word_and_data_replacement_do_not_refine_earlier_fields)
{
    Fixture f; f.nullSource(std::nullopt); auto o = f.observation(); o.paintingWord = 1;
    o.imageData = std::make_shared<BrushTextureImageDataState>(31);
    CHECK(!f.source->observe(o)); CHECK(!f.source->paintingWord()); REQUIRE(f.source->imageData()); CHECK(!*f.source->imageData());
    f.nullSource(1); o = f.observation(); o.paintingWord = 0; CHECK(!f.source->observe(o)); CHECK(f.source->paintingWord() == 1);
    Fixture nonnull; auto actual = std::make_shared<BrushTextureImageDataState>(31);
    o = nonnull.observation(); o.paintingWord = 1; o.imageData = actual; REQUIRE(nonnull.source->observe(o));
    std::shared_ptr<BrushTextureImageDataState> rewrapped(actual.get(), [](BrushTextureImageDataState*) {});
    o.imageData = rewrapped; CHECK(!nonnull.source->observe(o)); CHECK(*nonnull.source->imageData() == actual);
}

TEST_CASE(active_copy_source_native_lifetime_and_identity_override_diagnostic_leases)
{
    Fixture f; f.nullSource(); std::shared_ptr<PaintDeviceActiveCopySourceState> rewrapped(f.source.get(), [](PaintDeviceActiveCopySourceState*) {});
    PaintDeviceActiveCopyCall foreign{rewrapped, "Foreign frame"}; CHECK(foreign.resume().issue == Issue::SourceUnavailable);
    CHECK(foreign.effects().empty()); CHECK(f.call.resume().issue == Issue::CleanupGlobalsUnavailable);
    auto diagnostic = f.source; f.source->releaseNativeOwner(); CHECK(!diagnostic->nativeOwnerAlive());
    CHECK(!diagnostic->observe(f.observation())); CHECK(f.call.resume().issue == Issue::SourceUnavailable);
    CHECK(f.call.effects() == expectedPending);
}

TEST_CASE(active_copy_expired_source_does_not_stay_alive_through_call)
{
    std::unique_ptr<PaintDeviceActiveCopyCall> call;
    {
        Fixture f; f.nullSource(); call = std::make_unique<PaintDeviceActiveCopyCall>(f.source, "Frame");
    }
    CHECK(call->resume().issue == Issue::SourceUnavailable); CHECK(call->effects().empty());
}

TEST_CASE(active_copy_released_module_during_inner_cleanup_cannot_be_replaced)
{
    Fixture f; f.nullSource(); f.existing(std::nullopt); CHECK(f.call.resume().issue == Issue::InnerCleanupPending);
    auto diagnostic = f.globals; f.globals->releaseNativeOwner();
    CHECK(f.call.resume().issue == Issue::CleanupGlobalsUnavailable); CHECK(f.call.effects() == expectedPending);
    CHECK(diagnostic->counterFor(31) == nullptr); Fixture other; REQUIRE(other.globals->observe(other.globalObservation()));
    CHECK(!f.call.bindCleanupGlobals(other.globals)); CHECK(!f.call.takeReturnedImage());
}

TEST_CASE(active_copy_released_temporary_stops_pending_continuation)
{
    Fixture f; f.nullSource(); CHECK(f.call.resume().issue == Issue::CleanupGlobalsUnavailable);
    auto inner = f.call.innerImagePrefixState(); REQUIRE(inner); inner->releaseNativeOwner();
    f.existing(0); CHECK(f.call.resume().issue == Issue::TemporaryOwnerUnavailable);
    CHECK(f.call.effects() == expectedPending); CHECK(!f.call.innerCleanupCall()); CHECK(!f.call.takeReturnedImage());
}

TEST_CASE(active_copy_two_calls_share_source_and_globals_but_not_native_values_or_cursors)
{
    Fixture f; f.nullSource(); f.existing(std::nullopt);
    PaintDeviceActiveCopyCall second{f.source, "Second frame"}; REQUIRE(second.bindCleanupGlobals(f.globals));
    CHECK(f.call.resume().issue == Issue::InnerCleanupPending); CHECK(second.resume().issue == Issue::InnerCleanupPending);
    CHECK(f.call.returnedImagePrefixState() != second.returnedImagePrefixState());
    CHECK(f.call.innerImagePrefixState() != second.innerImagePrefixState()); CHECK(f.call.innerCleanupCall() != second.innerCleanupCall());
    CHECK(f.call.innerCleanupCall()->counterOwner() == second.innerCleanupCall()->counterOwner());
    f.existing(0); CHECK(f.call.resume().stage == Stage::Returned); CHECK(second.report().stage == Stage::InnerCleanup);
    CHECK(second.innerImagePrefixState()->nativeOwnerAlive()); CHECK(second.resume().stage == Stage::Returned);
    CHECK(second.effects() == expectedReturned); CHECK(f.source->paintingWord() == 1);
}

TEST_CASE(active_copy_outer_prefix_and_cleanup_are_separate_after_copy_return)
{
    Fixture f; f.nullSource(); f.existing(0); CHECK(f.call.resume().stage == Stage::Returned);
    auto returned = f.call.takeReturnedImage(); REQUIRE(returned); REQUIRE(returned->imageData()); CHECK(!*returned->imageData());
    auto outer = returned->prefixState(); REQUIRE(outer); CHECK(outer->nativeOwnerAlive());
    CHECK(outer->classIdentity() == PaintDevicePrefixClass::QImage); CHECK(!f.call.takeReturnedImage());
    PaintDevicePrefixCall prefix{outer}; CHECK(prefix.resume().stage == PaintDevicePrefixStage::TailReady);
    PaintDeviceCleanupCall cleanup{f.globals}; CHECK(cleanup.effects().empty());
    CHECK(cleanup.resume().stage == PaintDeviceCleanupStage::Returned);
    CHECK(cleanup.effects().size() == 3u); CHECK(f.call.innerCleanupCall()->effects().size() == 3u);
    CHECK(cleanup.counterOwner() == f.call.innerCleanupCall()->counterOwner());
    outer->releaseNativeOwner(); CHECK(f.call.resume().stage == Stage::Returned); CHECK(f.call.effects() == expectedReturned);
}

TEST_CASE(active_copy_abandonment_does_not_run_outer_cleanup_or_extend_native_value_life)
{
    for (int point = 0; point != 3; ++point) {
        std::shared_ptr<PaintDevicePrefixState> outer, inner;
        {
            Fixture f;
            if (point == 0) { f.nullSource(std::nullopt); CHECK(f.call.resume().issue == Issue::SourcePaintingWordUnavailable); }
            if (point == 1) { f.nullSource(); CHECK(f.call.resume().issue == Issue::CleanupGlobalsUnavailable); }
            if (point == 2) { f.nullSource(); f.existing(0); CHECK(f.call.resume().stage == Stage::Returned); }
            outer = f.call.returnedImagePrefixState(); inner = f.call.innerImagePrefixState();
            REQUIRE(outer); CHECK(outer->nativeOwnerAlive());
        }
        CHECK(!outer->nativeOwnerAlive()); if (inner) CHECK(!inner->nativeOwnerAlive());
    }
}

TEST_CASE(active_copy_explicit_returned_value_transfer_owns_lifetime_beyond_call)
{
    std::unique_ptr<PaintDeviceActiveCopyValue> returned;
    std::shared_ptr<PaintDevicePrefixState> diagnostic;
    {
        Fixture f; f.nullSource(); f.existing(0); CHECK(f.call.resume().stage == Stage::Returned);
        returned = f.call.takeReturnedImage(); REQUIRE(returned); diagnostic = returned->prefixState();
    }
    CHECK(diagnostic->nativeOwnerAlive()); CHECK(diagnostic->classIdentity() == PaintDevicePrefixClass::QImage);
    returned.reset(); CHECK(!diagnostic->nativeOwnerAlive());
}

TEST_CASE(active_copy_inner_word_zero_cannot_be_replaced_by_active_source_word)
{
    Fixture f; f.nullSource(0xffff); CHECK(f.call.resume().issue == Issue::CleanupGlobalsUnavailable);
    auto inner = f.call.innerImagePrefixState(); REQUIRE(inner); PaintDevicePrefixObservation o;
    o.receiver = inner; o.qualification = inner->qualification(); o.paintingWord = 0xffff;
    CHECK(!inner->observe(o)); CHECK(inner->paintingWord() == 0); REQUIRE(f.call.innerPrefixCall());
    CHECK(f.call.innerPrefixCall()->report().issue == PaintDevicePrefixIssue::None);
    CHECK(f.call.innerPrefixCall()->report().stage == PaintDevicePrefixStage::TailReady);
    CHECK(f.source->paintingWord() == 0xffff); CHECK(f.call.effects() == expectedPending);
}

namespace {
using FinishStage = PaintDeviceNullImageFinishStage;
using FinishIssue = PaintDeviceNullImageFinishIssue;
using FinishEffect = PaintDeviceNullImageFinishEffect;
const std::vector expectedFinish{
    FinishEffect::PredicateImageDataRead, FinishEffect::AlphaPredicateReturnedFalse,
    FinishEffect::OpacityNegated, FinishEffect::QImageClassInstalled, FinishEffect::QImageDataRead,
    FinishEffect::PaintDevicePrefixTailReached, FinishEffect::CleanupReturned,
    FinishEffect::OuterImageDestroyed, FinishEffect::OpacityReturned};
}

TEST_CASE(null_image_finish_inherits_exact_module_origin_only_after_inner_return)
{
    Fixture f; f.nullSource(); f.existing(std::nullopt);
    CHECK(f.call.resume().issue == Issue::InnerCleanupPending); CHECK(!f.call.takeReturnedImage());
    f.existing(0); CHECK(f.call.report().stage == Stage::InnerCleanup);
    CHECK(f.call.resume().stage == Stage::Returned); const auto producerEffects = f.call.effects();
    auto value = f.call.takeReturnedImage(); REQUIRE(value);
    const auto origin = value->cleanupGlobalsOwner().lock(); REQUIRE(origin);
    CHECK(origin.get() == f.globals.get()); CHECK(!origin.owner_before(f.globals)); CHECK(!f.globals.owner_before(origin));
    auto prefix = value->prefixState(); REQUIRE(prefix);
    PaintDeviceNullImageFinishCall finish{std::move(value)}; CHECK(!value);
    CHECK(finish.valuePrefixState() == prefix); CHECK(finish.cleanupGlobalsOwner().lock() == origin);
    CHECK(!finish.report().opaque); CHECK(finish.effects().empty());
    CHECK(f.call.effects() == producerEffects); CHECK(!f.call.takeReturnedImage());
}

TEST_CASE(null_image_finish_keeps_predicate_private_until_same_outer_destruction)
{
    for (unsigned word : {1u, 0x8000u, 0xffffu}) {
        Fixture f; f.nullSource(static_cast<std::uint16_t>(word)); f.existing(0);
        CHECK(f.call.resume().stage == Stage::Returned); auto value = f.call.takeReturnedImage(); REQUIRE(value);
        auto outer = value->prefixState(), inner = f.call.innerImagePrefixState(); REQUIRE(outer); REQUIRE(inner);
        PaintDeviceNullImageFinishCall finish{std::move(value)};
        CHECK(!finish.report().opaque); CHECK(finish.resume().stage == FinishStage::Returned);
        CHECK(finish.report().issue == FinishIssue::None); CHECK(finish.report().opaque == true);
        CHECK(finish.effects() == expectedFinish); CHECK(finish.valuePrefixState() == outer);
        CHECK(outer != inner); CHECK(!outer->nativeOwnerAlive()); CHECK(!inner->nativeOwnerAlive());
        CHECK(outer->classIdentity() == PaintDevicePrefixClass::QPaintDevice); CHECK(outer->paintingWord() == 0);
        REQUIRE(finish.prefixCall()); REQUIRE(finish.cleanupCall());
        const std::vector prefixEffects{PaintDevicePrefixEffect::PaintingWordRead,
            PaintDevicePrefixEffect::BaseClassInstalled, PaintDevicePrefixEffect::CleanupTailReached};
        const std::vector cleanupEffects{PaintDeviceCleanupEffect::FirstGetterReturned,
            PaintDeviceCleanupEffect::SecondGetterReturned, PaintDeviceCleanupEffect::CounterValueRead};
        CHECK(finish.prefixCall()->effects() == prefixEffects); CHECK(finish.cleanupCall()->effects() == cleanupEffects);
        CHECK(finish.cleanupCall() != f.call.innerCleanupCall());
        CHECK(finish.cleanupCall()->counterOwner() == f.call.innerCleanupCall()->counterOwner());
        CHECK(f.call.innerCleanupCall()->effects() == cleanupEffects); CHECK(f.source->paintingWord() == word);
        REQUIRE(f.source->imageData()); CHECK(!*f.source->imageData()); CHECK(!finish.report().completeForPublicSave);
    }
}

TEST_CASE(null_image_finish_missing_and_premature_tokens_emit_no_native_effects)
{
    PaintDeviceNullImageFinishCall absent{nullptr}; CHECK(absent.resume().issue == FinishIssue::ValueUnavailable);
    CHECK(absent.effects().empty()); CHECK(!absent.report().opaque);
    Fixture f; f.nullSource(); CHECK(f.call.resume().issue == Issue::CleanupGlobalsUnavailable);
    PaintDeviceNullImageFinishCall premature{f.call.takeReturnedImage()};
    CHECK(premature.resume().issue == FinishIssue::ValueUnavailable); CHECK(premature.effects().empty());
    CHECK(!premature.report().opaque); CHECK(f.call.effects() == expectedPending);
}

TEST_CASE(null_image_finish_weak_origin_survives_source_frame_but_not_module_loss)
{
    std::unique_ptr<PaintDeviceActiveCopyValue> value;
    std::shared_ptr<PaintDeviceCleanupGlobals> module;
    std::shared_ptr<PaintDevicePrefixState> outer;
    {
        Fixture f; f.nullSource(); f.existing(0); CHECK(f.call.resume().stage == Stage::Returned);
        value = f.call.takeReturnedImage(); REQUIRE(value); outer = value->prefixState(); module = f.globals;
        f.source->releaseNativeOwner(); CHECK(!f.source->nativeOwnerAlive());
    }
    REQUIRE(outer); CHECK(outer->nativeOwnerAlive()); REQUIRE(value->cleanupGlobalsOwner().lock());
    PaintDeviceNullImageFinishCall finish{std::move(value)};
    CHECK(finish.resume().stage == FinishStage::Returned); CHECK(finish.report().opaque == true);
    CHECK(finish.cleanupGlobalsOwner().lock() == module); CHECK(!outer->nativeOwnerAlive());
}

TEST_CASE(null_image_finish_released_module_blocks_before_predicate_despite_host_leases)
{
    Fixture f; f.nullSource(); f.existing(0); CHECK(f.call.resume().stage == Stage::Returned);
    auto value = f.call.takeReturnedImage(); REQUIRE(value); auto outer = value->prefixState();
    auto diagnosticModule = f.globals; f.globals->releaseNativeOwner();
    PaintDeviceNullImageFinishCall finish{std::move(value)};
    CHECK(finish.resume().issue == FinishIssue::ModuleOriginUnavailable); CHECK(finish.effects().empty());
    CHECK(!finish.report().opaque); CHECK(!finish.prefixCall()); CHECK(!finish.cleanupCall());
    CHECK(outer->nativeOwnerAlive()); CHECK(!diagnosticModule->nativeOwnerAlive());
    CHECK(finish.resume().issue == FinishIssue::ModuleOriginUnavailable); CHECK(finish.effects().empty());
}

TEST_CASE(null_image_finish_origin_is_weak_and_cannot_keep_actual_module_alive)
{
    Fixture f; f.nullSource(); f.existing(0); CHECK(f.call.resume().stage == Stage::Returned);
    auto value = f.call.takeReturnedImage(); REQUIRE(value);
    std::weak_ptr<PaintDeviceCleanupGlobals> module = f.globals;
    PaintDeviceNullImageFinishCall finish{std::move(value)}; f.globals.reset(); CHECK(module.expired());
    CHECK(finish.cleanupGlobalsOwner().expired()); CHECK(finish.resume().issue == FinishIssue::ModuleOriginUnavailable);
    CHECK(finish.effects().empty()); CHECK(!finish.report().opaque);
}

TEST_CASE(null_image_finish_equal_profile_module_is_not_substituted_for_inherited_owner)
{
    Fixture first, other; first.nullSource(); first.existing(0); other.nullSource(); other.existing(0);
    CHECK(first.call.resume().stage == Stage::Returned); auto value = first.call.takeReturnedImage(); REQUIRE(value);
    CHECK(value->cleanupGlobalsOwner().lock() == first.globals); CHECK(value->cleanupGlobalsOwner().lock() != other.globals);
    PaintDeviceNullImageFinishCall finish{std::move(value)}; CHECK(finish.resume().stage == FinishStage::Returned);
    REQUIRE(finish.cleanupCall()); CHECK(finish.cleanupCall()->counterOwner() == first.globals->counterFor(31));
    CHECK(finish.cleanupCall()->counterOwner() != other.globals->counterFor(31));
    CHECK(other.globals->counterFor(31)->value() == 0); CHECK(other.call.effects().empty());
}

TEST_CASE(null_image_finish_released_value_and_claimed_prefix_do_not_replay_destruction)
{
    for (bool release : {false, true}) {
        Fixture f; f.nullSource(); f.existing(0); CHECK(f.call.resume().stage == Stage::Returned);
        auto value = f.call.takeReturnedImage(); REQUIRE(value); auto outer = value->prefixState();
        if (release) outer->releaseNativeOwner();
        else { PaintDevicePrefixCall previous{outer}; CHECK(previous.resume().stage == PaintDevicePrefixStage::TailReady); }
        PaintDeviceNullImageFinishCall finish{std::move(value)};
        const auto issue = release ? FinishIssue::ValueUnavailable : FinishIssue::ValueClassUnrecovered;
        CHECK(finish.resume().issue == issue); CHECK(finish.effects().empty()); CHECK(!finish.report().opaque);
        CHECK(!finish.prefixCall()); CHECK(!finish.cleanupCall()); CHECK(f.call.effects() == expectedReturned);
    }
}

TEST_CASE(null_image_finish_distinct_returned_values_share_only_canonical_module)
{
    Fixture f; f.nullSource(); f.existing(0);
    PaintDeviceActiveCopyCall second{f.source, "Second frame"}; REQUIRE(second.bindCleanupGlobals(f.globals));
    CHECK(f.call.resume().stage == Stage::Returned); CHECK(second.resume().stage == Stage::Returned);
    auto firstValue = f.call.takeReturnedImage(), secondValue = second.takeReturnedImage(); REQUIRE(firstValue); REQUIRE(secondValue);
    auto firstPrefix = firstValue->prefixState(), secondPrefix = secondValue->prefixState(); CHECK(firstPrefix != secondPrefix);
    PaintDeviceNullImageFinishCall first{std::move(firstValue)}, last{std::move(secondValue)};
    CHECK(first.resume().stage == FinishStage::Returned); CHECK(!firstPrefix->nativeOwnerAlive());
    CHECK(secondPrefix->nativeOwnerAlive()); CHECK(!last.report().opaque); CHECK(last.effects().empty());
    CHECK(last.resume().stage == FinishStage::Returned); CHECK(!secondPrefix->nativeOwnerAlive());
    CHECK(first.cleanupCall() != last.cleanupCall()); CHECK(first.prefixCall() != last.prefixCall());
    CHECK(first.cleanupCall()->counterOwner() == last.cleanupCall()->counterOwner());
    CHECK(first.effects() == expectedFinish); CHECK(last.effects() == expectedFinish);
}

TEST_CASE(null_image_finish_abandonment_marks_value_loss_without_native_success)
{
    Fixture f; f.nullSource(); f.existing(0); CHECK(f.call.resume().stage == Stage::Returned);
    auto value = f.call.takeReturnedImage(); REQUIRE(value); auto outer = value->prefixState();
    {
        PaintDeviceNullImageFinishCall abandoned{std::move(value)};
        CHECK(abandoned.effects().empty()); CHECK(!abandoned.report().opaque); CHECK(outer->nativeOwnerAlive());
        CHECK(!abandoned.prefixCall()); CHECK(!abandoned.cleanupCall());
    }
    CHECK(!outer->nativeOwnerAlive()); CHECK(outer->classIdentity() == PaintDevicePrefixClass::QImage);
    CHECK(f.call.innerCleanupCall()->effects().size() == 3u); CHECK(f.globals->counterFor(31)->value() == 0);
}

TEST_CASE(null_image_finish_returned_receipt_is_stable_after_native_owner_loss)
{
    Fixture f; f.nullSource(); f.existing(0); CHECK(f.call.resume().stage == Stage::Returned);
    PaintDeviceNullImageFinishCall finish{f.call.takeReturnedImage()}; CHECK(finish.resume().stage == FinishStage::Returned);
    const auto prefixEffects = finish.prefixCall()->effects();
    const auto cleanupEffects = finish.cleanupCall()->effects();
    f.globals->releaseNativeOwner(); f.source->releaseNativeOwner();
    CHECK(finish.resume().stage == FinishStage::Returned); CHECK(finish.report().opaque == true);
    CHECK(finish.effects() == expectedFinish); CHECK(finish.prefixCall()->effects() == prefixEffects);
    CHECK(finish.cleanupCall()->effects() == cleanupEffects); CHECK(!finish.valuePrefixState()->nativeOwnerAlive());
}

TEST_CASE(bound_copy_view_requires_payload_and_live_canonical_qualified_module)
{
    Fixture f; auto payload = std::make_shared<BrushTextureImageState>();
    CHECK(!PaintDeviceActiveCopySourceState::fromTextureImageSource(payload, f.globals, "View"));
    f.existing(0);
    CHECK(!PaintDeviceActiveCopySourceState::fromTextureImageSource(nullptr, f.globals, "View"));
    CHECK(!PaintDeviceActiveCopySourceState::fromTextureImageSource(payload, nullptr, "View"));
    CHECK(!PaintDeviceActiveCopySourceState::fromTextureImageSource(payload, f.globals, ""));
    std::shared_ptr<PaintDeviceCleanupGlobals> rewrapped(f.globals.get(), [](PaintDeviceCleanupGlobals*) {});
    CHECK(!PaintDeviceActiveCopySourceState::fromTextureImageSource(payload, rewrapped, "View"));
    auto view = PaintDeviceActiveCopySourceState::fromTextureImageSource(payload, f.globals, "View"); REQUIRE(view);
    CHECK(view->textureImageBound()); CHECK(view->qualified()); CHECK(view->nativeOwnerAlive());
    CHECK(view->textureImageSource().get() == payload.get());
    CHECK(!view->textureImageSource().owner_before(payload)); CHECK(!payload.owner_before(view->textureImageSource()));
    CHECK(view->cleanupGlobalsOwner().lock() == f.globals); CHECK(!view->paintingWord()); CHECK(!view->imageData());
    f.globals->releaseNativeOwner(); CHECK(!view->nativeOwnerAlive());
    CHECK(!PaintDeviceActiveCopySourceState::fromTextureImageSource(payload, f.globals, "View"));
}

TEST_CASE(bound_copy_view_delegates_late_fields_without_new_source_observations)
{
    Fixture f; f.existing(0); auto payload = std::make_shared<BrushTextureImageState>();
    auto view = PaintDeviceActiveCopySourceState::fromTextureImageSource(payload, f.globals, "View"); REQUIRE(view);
    PaintDeviceActiveCopyCall copy{view, "Bound copy"}; CHECK(copy.effects().empty());
    CHECK(copy.resume().issue == Issue::SourcePaintingWordUnavailable);
    const auto constructed = copy.effects(); CHECK(constructed.size() == 1u);
    BrushTextureImageObservation o; o.imagePaintingActiveWord = 1; REQUIRE(payload->observe(o));
    CHECK(view->paintingWord() == 1); CHECK(copy.effects() == constructed);
    CHECK(copy.resume().issue == Issue::SourceImageDataUnavailable);
    const auto reached = copy.effects(); CHECK(reached.size() == 4u);
    o = {}; o.imageData = std::shared_ptr<BrushTextureImageDataState>{}; REQUIRE(payload->observe(o));
    CHECK(&view->imageData() == &payload->imageDataOwner()); CHECK(copy.effects() == reached);
    CHECK(copy.resume().stage == Stage::Returned); CHECK(copy.effects() == expectedReturned);
    auto value = copy.takeReturnedImage(); REQUIRE(value); CHECK(value->cleanupGlobalsOwner().lock() == f.globals);
    PaintDeviceNullImageFinishCall finish{std::move(value)}; CHECK(finish.resume().stage == FinishStage::Returned);
    CHECK(finish.report().opaque == true); CHECK(finish.cleanupGlobalsOwner().lock() == f.globals);
}

TEST_CASE(bound_copy_view_rejects_standalone_observe_and_invalid_payload_refinement)
{
    Fixture f; f.existing(0); auto payload = std::make_shared<BrushTextureImageState>();
    auto view = PaintDeviceActiveCopySourceState::fromTextureImageSource(payload, f.globals, "View"); REQUIRE(view);
    PaintDeviceActiveCopySourceObservation duplicate; duplicate.receiver = view; duplicate.qualification = "View";
    duplicate.paintingWord = 1; duplicate.imageData = std::shared_ptr<BrushTextureImageDataState>{};
    CHECK(!view->observe(duplicate)); CHECK(!view->paintingWord()); CHECK(!view->imageData());
    BrushTextureImageObservation invalid; invalid.imagePaintingActiveWord = 1;
    invalid.imageData = std::make_shared<BrushTextureImageDataState>(0);
    CHECK(!payload->observe(invalid)); CHECK(!view->paintingWord()); CHECK(!view->imageData());
    invalid.imageData = std::shared_ptr<BrushTextureImageDataState>{}; REQUIRE(payload->observe(invalid));
    CHECK(view->paintingWord() == 1); REQUIRE(view->imageData()); CHECK(!*view->imageData());
    CHECK(!view->observe(duplicate)); CHECK(view->textureImageSource() == payload);
}

TEST_CASE(bound_copy_view_keeps_actual_nonnull_data_owner_and_guard)
{
    Fixture f; f.existing(0); auto payload = std::make_shared<BrushTextureImageState>();
    auto data = std::make_shared<BrushTextureImageDataState>(31);
    BrushTextureImageObservation o; o.imagePaintingActiveWord = 1; o.imageData = data; REQUIRE(payload->observe(o));
    auto view = PaintDeviceActiveCopySourceState::fromTextureImageSource(payload, f.globals, "View"); REQUIRE(view);
    REQUIRE(view->imageData()); CHECK(*view->imageData() == data); CHECK(&view->imageData() == &payload->imageDataOwner());
    PaintDeviceActiveCopyCall copy{view, "Bound copy"}; CHECK(copy.resume().issue == Issue::NonNullActiveCopyUnrecovered);
    CHECK(!copy.takeReturnedImage()); CHECK(data->temporaryValueReferences() == 0u);
    CHECK(!copy.innerCleanupCall()); CHECK(!copy.innerImagePrefixState());
}

TEST_CASE(bound_copy_producer_origin_is_fixed_before_first_native_effect)
{
    Fixture f, other; f.existing(0); other.existing(0); auto payload = std::make_shared<BrushTextureImageState>();
    BrushTextureImageObservation o; o.imagePaintingActiveWord = 1; o.imageData = std::shared_ptr<BrushTextureImageDataState>{};
    REQUIRE(payload->observe(o));
    auto view = PaintDeviceActiveCopySourceState::fromTextureImageSource(payload, f.globals, "View"); REQUIRE(view);
    PaintDeviceActiveCopyCall copy{view, "Bound copy"};
    CHECK(!copy.bindCleanupGlobals(other.globals)); REQUIRE(copy.bindCleanupGlobals(f.globals)); CHECK(copy.effects().empty());
    CHECK(copy.resume().stage == Stage::Returned); auto value = copy.takeReturnedImage(); REQUIRE(value);
    CHECK(value->cleanupGlobalsOwner().lock() == f.globals); CHECK(value->cleanupGlobalsOwner().lock() != other.globals);
    CHECK(copy.innerCleanupCall()->counterOwner() == f.globals->counterFor(31));
    CHECK(copy.innerCleanupCall()->counterOwner() != other.globals->counterFor(31));
}

TEST_CASE(bound_copy_view_metadata_lease_does_not_extend_native_module_lifetime)
{
    Fixture f; f.existing(0); auto payload = std::make_shared<BrushTextureImageState>();
    BrushTextureImageObservation o; o.imagePaintingActiveWord = 1; o.imageData = std::shared_ptr<BrushTextureImageDataState>{};
    REQUIRE(payload->observe(o));
    auto view = PaintDeviceActiveCopySourceState::fromTextureImageSource(payload, f.globals, "View"); REQUIRE(view);
    std::weak_ptr<BrushTextureImageState> diagnosticPayload = payload; payload.reset(); CHECK(!diagnosticPayload.expired());
    PaintDeviceActiveCopyCall copy{view, "Bound copy"}; std::weak_ptr<PaintDeviceCleanupGlobals> module = f.globals;
    f.globals.reset(); CHECK(module.expired()); CHECK(!view->nativeOwnerAlive());
    CHECK(view->paintingWord() == 1); // Historical metadata remains valid; no native continuation.
    CHECK(copy.resume().issue == Issue::SourceUnavailable); CHECK(copy.effects().empty());
    auto replacement = std::make_shared<PaintDeviceCleanupGlobals>("Other module");
    CHECK(!copy.bindCleanupGlobals(replacement));
}

TEST_CASE(bound_copy_released_view_cannot_resume_or_rebind)
{
    Fixture f; f.existing(0); auto payload = std::make_shared<BrushTextureImageState>();
    auto view = PaintDeviceActiveCopySourceState::fromTextureImageSource(payload, f.globals, "View"); REQUIRE(view);
    PaintDeviceActiveCopyCall copy{view, "Bound copy"}; view->releaseNativeOwner();
    CHECK(!view->nativeOwnerAlive()); CHECK(f.globals->nativeOwnerAlive()); CHECK(!copy.bindCleanupGlobals(f.globals));
    CHECK(copy.resume().issue == Issue::SourceUnavailable); CHECK(copy.effects().empty());
    PaintDeviceActiveCopySourceObservation o; o.receiver = view; o.qualification = "View";
    CHECK(!view->observe(o)); CHECK(!view->nativeOwnerAlive());
}

TEST_CASE(bound_copy_standalone_source_mode_retains_its_explicit_binding_contract)
{
    Fixture f; f.nullSource(); CHECK(!f.source->textureImageBound()); CHECK(!f.source->textureImageSource());
    CHECK(f.source->cleanupGlobalsOwner().expired()); CHECK(f.call.resume().issue == Issue::CleanupGlobalsUnavailable);
    f.existing(0); CHECK(f.call.resume().stage == Stage::Returned); CHECK(f.call.effects() == expectedReturned);
}

int main() { return testkit::run_all(); }
