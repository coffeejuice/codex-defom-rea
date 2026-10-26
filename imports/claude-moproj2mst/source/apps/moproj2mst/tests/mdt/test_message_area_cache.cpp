// ctest-labels: m2m-mdt message-area-cache
#include <testkit/check.hpp>

#include <memory>
#include <string>
#include <type_traits>

#include "m2m/common.hpp"
#include "m2m/engine/message_area_cache.hpp"

using namespace m2m;
using namespace m2m::engine;
namespace {
auto processCache() { return MessageAreaProcessCache::create("declared fixture one native process owner"); }
template<class F> bool refuses(F&& body, const char* detail)
{
    try { body(); }
    catch (const Error& error) {
        return error.code == M2M_E_UNSUPPORTED && std::string(error.what()).find(detail) != std::string::npos;
    }
    return false;
}
}

static_assert(!std::is_copy_constructible_v<MessageAreaProcessCache>);
static_assert(!std::is_move_constructible_v<MessageAreaProcessCache>);
static_assert(!std::is_copy_constructible_v<MessageAreaCacheProgress>);
static_assert(!std::is_move_constructible_v<MessageAreaCacheProgress>);

TEST_CASE(message_cache_unknown_is_not_assumed_initialized_empty)
{
    auto process = processCache();
    MessageAreaCacheProgress progress(process, qt::String{}, false, messageAreaCacheSourceOccurrence,
        "declared fixture handler entry");
    CHECK(attemptMessageAreaCache(process, progress).stop == MessageAreaCacheStop::InitializationKnowledgeUnavailable);
    CHECK(process->knowledge() == MessageAreaCacheKnowledge::Unknown);
    CHECK(!process->initializationWord());
    CHECK(!process->lastMessage());
    CHECK(progress.stage() == MessageAreaCacheStage::AwaitInitialization);
}

TEST_CASE(message_cache_known_uninitialized_guards_before_flag_store_and_qstring_construction)
{
    auto process = processCache();
    process->bindObservedUninitialized(0x100u, "declared fixture observed raw flag bit0 clear");
    MessageAreaCacheProgress progress(process, qt::String{}, true, messageAreaCacheSourceOccurrence,
        "declared fixture handler with clear requested");
    CHECK(attemptMessageAreaCache(process, progress).stop == MessageAreaCacheStop::NativeInitializationEffectsUnavailable);
    REQUIRE(process->initializationWord());
    CHECK(*process->initializationWord() == 0x100u);
    CHECK(!process->lastMessage());
    CHECK(progress.stage() == MessageAreaCacheStage::AwaitInitialization); // initialization precedes GUI clear
    CHECK(refuses([&] { process->bindObservedInitialized(0x101u, qt::String{}, "unqualified attempt to bypass initialization"); }, "rebind"));
}

TEST_CASE(message_cache_known_duplicate_skips_and_retry_does_not_repeat_comparison)
{
    auto process = processCache();
    const auto message = qt::L("All projects have been closed.");
    process->bindObservedInitialized(0x101u, message, "declared fixture initialized process value");
    MessageAreaCacheProgress progress(process, message, false, messageAreaCacheSourceOccurrence,
        "declared fixture normal Close handler entered");
    const auto first = attemptMessageAreaCache(process, progress);
    CHECK(first.stop == MessageAreaCacheStop::DuplicateMessageSkipped);
    CHECK(first.comparisonCommittedThisAttempt);
    CHECK(progress.stage() == MessageAreaCacheStage::DuplicateSkip);
    CHECK(!attemptMessageAreaCache(process, progress).comparisonCommittedThisAttempt);
    CHECK(*process->lastMessage() == message);
    CHECK(*process->initializationWord() == 0x101u);
}

TEST_CASE(message_cache_distinct_message_guards_before_color_append_without_early_cache_assignment)
{
    auto process = processCache();
    const auto old = qt::L("previous process message");
    process->bindObservedInitialized(1, old, "declared fixture initialized process value");
    MessageAreaCacheProgress progress(process, qt::L("new message"), false, messageAreaCacheSourceOccurrence,
        "declared fixture distinct handler input");
    const auto result = attemptMessageAreaCache(process, progress);
    CHECK(result.stop == MessageAreaCacheStop::ColorAppendEffectsUnavailable);
    CHECK(result.comparisonCommittedThisAttempt);
    CHECK(progress.stage() == MessageAreaCacheStage::BeforeColorAppend);
    CHECK(*process->lastMessage() == old);
    CHECK(attemptMessageAreaCache(process, progress).stop == MessageAreaCacheStop::ColorAppendEffectsUnavailable);
    CHECK(!attemptMessageAreaCache(process, progress).comparisonCommittedThisAttempt);
    CHECK(refuses([&] { process->bindObservedInitialized(1, qt::L("new message"),
        "attempted pre-append cache overwrite"); }, "rebind"));
}

TEST_CASE(message_cache_clear_virtual_precedes_duplicate_comparison)
{
    auto process = processCache();
    const auto message = qt::L("same message");
    process->bindObservedInitialized(1, message, "declared fixture initialized process value");
    MessageAreaCacheProgress progress(process, message, true, messageAreaCacheSourceOccurrence,
        "declared fixture non-Close clear input");
    const auto result = attemptMessageAreaCache(process, progress);
    CHECK(result.stop == MessageAreaCacheStop::ClearVirtualUnavailable);
    CHECK(!result.comparisonCommittedThisAttempt);
    CHECK(progress.stage() == MessageAreaCacheStage::BeforeClearVirtual);
    CHECK(*process->lastMessage() == message);
}

TEST_CASE(message_cache_comparison_preserves_qstring_null_empty_and_utf16_rules)
{
    auto process = processCache();
    process->bindObservedInitialized(1, qt::String{}, "declared fixture initialized null QString");
    MessageAreaCacheProgress empty(process, qt::String(u""), false, messageAreaCacheSourceOccurrence,
        "declared fixture empty incoming string");
    CHECK(attemptMessageAreaCache(process, empty).stop == MessageAreaCacheStop::DuplicateMessageSkipped);
    CHECK(process->lastMessage()->isNull());
    auto unicodeProcess = processCache();
    const qt::String unicode(u"\u03a9\U0001f600");
    unicodeProcess->bindObservedInitialized(1, unicode, "declared fixture UTF16 process value");
    MessageAreaCacheProgress equal(unicodeProcess, unicode, false, messageAreaCacheSourceOccurrence,
        "declared fixture equal Unicode message");
    CHECK(attemptMessageAreaCache(unicodeProcess, equal).stop == MessageAreaCacheStop::DuplicateMessageSkipped);
    MessageAreaCacheProgress changed(unicodeProcess, qt::String(u"\u03c9\U0001f600"), false,
        messageAreaCacheSourceOccurrence, "declared fixture case-distinct UTF16 message");
    CHECK(attemptMessageAreaCache(unicodeProcess, changed).stop == MessageAreaCacheStop::ColorAppendEffectsUnavailable);
}

TEST_CASE(message_cache_one_process_owner_survives_receiver_progress_teardown_and_reset_requests)
{
    auto process = processCache();
    const auto message = qt::L("shared last process message");
    process->bindObservedInitialized(1, message, "declared fixture initialized process value");
    {
        MessageAreaCacheProgress receiverA(process, message, false, messageAreaCacheSourceOccurrence,
            "declared fixture receiver A handler entry");
        CHECK(attemptMessageAreaCache(process, receiverA).stop == MessageAreaCacheStop::DuplicateMessageSkipped);
    }
    for (const auto cause : {MessageAreaCacheResetCause::NormalClose,
        MessageAreaCacheResetCause::ReceiverTeardown, MessageAreaCacheResetCause::ProcessExit}) {
        const auto reset = attemptMessageAreaCacheReset(process, cause);
        CHECK(reset.stop == MessageAreaCacheResetStop::NativeResetUnavailable);
        CHECK(reset.unavailableBoundary != nullptr);
        CHECK(process->knowledge() == MessageAreaCacheKnowledge::KnownInitialized);
        CHECK(*process->lastMessage() == message);
        CHECK(*process->initializationWord() == 1);
    }
    MessageAreaCacheProgress receiverB(process, message, false, messageAreaCacheSourceOccurrence,
        "declared fixture receiver B shares same process cache");
    CHECK(attemptMessageAreaCache(process, receiverB).stop == MessageAreaCacheStop::DuplicateMessageSkipped);
}

TEST_CASE(message_cache_binding_receipts_cannot_mutate_existing_native_state_or_drop_raw_flags)
{
    auto process = processCache();
    const auto message = qt::L("old");
    CHECK(process->bindObservedInitialized(0x801u, message, "first binding"));
    CHECK(!process->bindObservedInitialized(0x801u, message, "duplicate later binding"));
    CHECK(process->bindingQualification() == "first binding");
    CHECK(refuses([&] { process->bindObservedInitialized(1, message, "loses other flag bits"); }, "rebind"));
    CHECK(refuses([&] { process->bindObservedUninitialized(0, "attempted native reset"); }, "rebind"));
    CHECK(*process->initializationWord() == 0x801u);
    auto unknown = processCache();
    CHECK(refuses([&] { unknown->bindObservedInitialized(0, message, "wrong flag"); }, "bit0"));
    CHECK(refuses([&] { unknown->bindObservedUninitialized(1, "wrong flag"); }, "bit0"));
    CHECK(unknown->knowledge() == MessageAreaCacheKnowledge::Unknown);
}

TEST_CASE(message_cache_requires_process_identity_source_occurrence_and_qualifications)
{
    CHECK(refuses([&] { MessageAreaProcessCache::create(" \r\n"); }, "qualification"));
    auto process = processCache();
    CHECK(refuses([&] { process->bindObservedInitialized(1, qt::String{}, " "); }, "qualification"));
    CHECK(refuses([&] { MessageAreaCacheProgress bad(process, qt::String{}, false,
        "unqualified other receiver", "declared fixture"); }, "occurrence"));
    CHECK(refuses([&] { MessageAreaCacheProgress bad(process, qt::String{}, false,
        messageAreaCacheSourceOccurrence, " "); }, "qualification"));
    MessageAreaCacheProgress progress(process, qt::String{}, false, messageAreaCacheSourceOccurrence,
        "declared fixture handler entry");
    CHECK(refuses([&] { attemptMessageAreaCache(processCache(), progress); }, "identity"));
    process.reset();
    CHECK(refuses([&] { attemptMessageAreaCache(processCache(), progress); }, "identity"));
}

int main() { return testkit::run_all(); }
