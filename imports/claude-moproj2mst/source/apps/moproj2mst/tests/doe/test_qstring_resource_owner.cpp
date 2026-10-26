// ctest-labels: m2m-doe
// Explicit isolated profiles; no original runtime codec/Session receipt implied.
#include <testkit/check.hpp>

#include <cstdint>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>

#include "m2m/common.hpp"
#include "m2m/doe_qstring_resource_owner.hpp"

using namespace m2m;
using namespace m2m::doe;

namespace {
const char* profileLabel = "declared isolated codec profile, not a native observation";
template<class Work> std::string refused(const Work& work)
{
    try { work(); }
    catch (const Error& error) {
        CHECK_EQ(error.code, M2M_E_UNSUPPORTED);
        return error.what();
    }
    CHECK(false);
    return {};
}
std::uint32_t refs(const QStringResourceProcessScope& scope, const QStringPathData& data)
{
    const auto snapshot = scope.pathOwner().snapshot(data);
    REQUIRE(snapshot.referenceCount.has_value());
    return *snapshot.referenceCount;
}
void selectNull(QStringResourceProcessScope& scope)
{ scope.setIsolatedCodecProfile(QStringCStringCodecKnowledge::Null, profileLabel); }
}

TEST_CASE(qstring_resource_default_unknown_guards_before_allocation)
{
    QStringResourceProcessScope scope;
    CHECK(scope.codec().knowledge == QStringCStringCodecKnowledge::Unknown);
    CHECK_EQ(scope.codec().epoch, std::uint64_t(0));
    auto occurrence = scope.prepareIsolatedOccurrence();
    CHECK(refused([&] { scope.readIsolatedCodec(occurrence); }).find("state is unavailable") != std::string::npos);
    const auto snapshot = scope.occurrence(0);
    CHECK(snapshot.stage == QStringResourceStage::CodecStateGuard);
    CHECK_EQ(snapshot.nativeAllocationBytesRequested, std::size_t(0));
    CHECK(!snapshot.data);
    CHECK(!snapshot.codeUnitCount);
    CHECK(!scope.pathOwner().sharedNull());
    QStringPathLease destination;
    CHECK(!refused([&] { scope.constructIsolatedResource(occurrence, destination,
        QStringResourceAllocationProfile::Success); }).empty());
    CHECK(destination.state() == QStringPathLeaseState::Empty);
}

TEST_CASE(qstring_resource_nonnull_codec_does_not_invoke_decoder_or_allocate)
{
    QStringResourceProcessScope scope;
    scope.setIsolatedCodecProfile(QStringCStringCodecKnowledge::NonNull, profileLabel);
    auto occurrence = scope.prepareIsolatedOccurrence();
    CHECK(refused([&] { scope.readIsolatedCodec(occurrence); }).find("virtual decoder") != std::string::npos);
    const auto snapshot = scope.occurrence(0);
    CHECK(snapshot.stage == QStringResourceStage::CodecDecoderGuard);
    CHECK(snapshot.selectedCodec == QStringCStringCodecKnowledge::NonNull);
    CHECK_EQ(snapshot.nativeAllocationBytesRequested, std::size_t(0));
    CHECK(!snapshot.data);
}

TEST_CASE(qstring_resource_null_branch_publishes_one_fresh_canonical_ref)
{
    QStringResourceProcessScope scope;
    selectNull(scope);
    auto occurrence = scope.prepareIsolatedOccurrence();
    scope.readIsolatedCodec(occurrence);
    CHECK_EQ(scope.occurrence(0).nativeAllocationBytesRequested, std::size_t(0));
    QStringPathLease destination;
    const auto data = scope.constructIsolatedResource(occurrence, destination, QStringResourceAllocationProfile::Success);
    const auto snapshot = scope.occurrence(0);
    CHECK(snapshot.stage == QStringResourceStage::ConstructorReturned);
    CHECK_EQ(snapshot.nativeAllocationBytesRequested, std::size_t(104));
    REQUIRE(snapshot.codeUnitCount.has_value());
    REQUIRE(snapshot.payloadOffset.has_value());
    REQUIRE(snapshot.knownFlagMask.has_value());
    REQUIRE(snapshot.knownFlagValue.has_value());
    CHECK_EQ(*snapshot.codeUnitCount, std::size_t(36));
    CHECK_EQ(*snapshot.payloadOffset, std::size_t(0x1a));
    CHECK_EQ(*snapshot.knownFlagMask, std::uint16_t(0x1f));
    CHECK_EQ(*snapshot.knownFlagValue, std::uint16_t(0));
    CHECK(snapshot.trailingUtf16Nul);
    CHECK(destination.state() == QStringPathLeaseState::Active);
    CHECK(scope.pathOwner().data(destination) == data);
    CHECK(scope.pathOwner().snapshot(data).origin == QStringPathDataOrigin::ResourceNullCodecProjection);
    CHECK_EQ(refs(scope, data), std::uint32_t(1));
    CHECK(scope.pathOwner().text(data).u16() == u":/SfTool/icon/tool/iconDoe_24_24.png");
    CHECK_EQ(scope.pathOwner().text(data).u16().size(), std::size_t(36));
    CHECK(scope.pathOwner().text(data).u16().c_str()[36] == u'\0');
    const auto observerCopy = data;
    const auto returnedSlotObserver = scope.pathOwner().data(destination);
    CHECK(observerCopy == returnedSlotObserver);
    CHECK_EQ(refs(scope, data), std::uint32_t(1));
    CHECK(!scope.pathOwner().sharedNull());
}

TEST_CASE(qstring_resource_selected_null_allocation_failure_has_no_constructor_return)
{
    QStringResourceProcessScope scope;
    selectNull(scope);
    auto occurrence = scope.prepareIsolatedOccurrence();
    scope.readIsolatedCodec(occurrence);
    QStringPathLease destination;
    CHECK(refused([&] { scope.constructIsolatedResource(occurrence, destination,
        QStringResourceAllocationProfile::ReturnNull); }).find("source malloc returned null") != std::string::npos);
    const auto snapshot = scope.occurrence(0);
    CHECK(snapshot.stage == QStringResourceStage::AllocationReturnedNull);
    CHECK_EQ(snapshot.nativeAllocationBytesRequested, std::size_t(104));
    CHECK(!snapshot.data);
    CHECK(!snapshot.codeUnitCount);
    CHECK(!snapshot.trailingUtf16Nul);
    CHECK(destination.state() == QStringPathLeaseState::Empty);
    CHECK(!refused([&] { scope.constructIsolatedResource(occurrence, destination,
        QStringResourceAllocationProfile::Success); }).empty());
    CHECK(scope.occurrence(0).stage == QStringResourceStage::AllocationReturnedNull);
}

TEST_CASE(qstring_resource_repeated_equal_text_has_distinct_generations)
{
    QStringResourceProcessScope scope;
    selectNull(scope);
    auto first = scope.prepareIsolatedOccurrence();
    auto second = scope.prepareIsolatedOccurrence();
    scope.readIsolatedCodec(first);
    scope.readIsolatedCodec(second);
    QStringPathLease firstSlot, secondSlot;
    const auto a = scope.constructIsolatedResource(first, firstSlot, QStringResourceAllocationProfile::Success);
    const auto b = scope.constructIsolatedResource(second, secondSlot, QStringResourceAllocationProfile::Success);
    CHECK(!(a == b));
    CHECK(scope.pathOwner().text(a).u16() == scope.pathOwner().text(b).u16());
    CHECK_EQ(refs(scope, a), std::uint32_t(1));
    CHECK_EQ(refs(scope, b), std::uint32_t(1));
    CHECK(scope.occurrence(0).generation != scope.occurrence(1).generation);
    QStringPathLease emptySlot;
    CHECK(!refused([&] { scope.constructIsolatedResource(first, emptySlot,
        QStringResourceAllocationProfile::Success); }).empty());
    CHECK(emptySlot.state() == QStringPathLeaseState::Empty);
    CHECK_EQ(scope.occurrenceCount(), std::size_t(2));
}

TEST_CASE(qstring_resource_canonical_aliases_release_at_explicit_source_events)
{
    QStringResourceProcessScope scope;
    selectNull(scope);
    auto occurrence = scope.prepareIsolatedOccurrence();
    scope.readIsolatedCodec(occurrence);
    QStringPathLease source, alias;
    const auto data = scope.constructIsolatedResource(occurrence, source, QStringResourceAllocationProfile::Success);
    scope.acquirePathAlias(data, alias);
    CHECK(scope.pathOwner().data(alias) == data);
    CHECK_EQ(refs(scope, data), std::uint32_t(2));
    scope.release(alias);
    CHECK_EQ(refs(scope, data), std::uint32_t(1));
    CHECK(!refused([&] { scope.release(source); }).empty()); // Reached zero; data-free guard remains.
    CHECK(source.state() == QStringPathLeaseState::Released);
    CHECK_EQ(refs(scope, data), std::uint32_t(0));
    CHECK(scope.pathOwner().snapshot(data).dataDeallocationPending);
    CHECK(!refused([&] { scope.release(source); }).empty());
    CHECK_EQ(refs(scope, data), std::uint32_t(0));
}

TEST_CASE(qstring_resource_stale_pre_read_admission_does_not_select_old_codec)
{
    QStringResourceProcessScope scope;
    selectNull(scope);
    auto stale = scope.prepareIsolatedOccurrence();
    scope.setIsolatedCodecProfile(QStringCStringCodecKnowledge::NonNull, profileLabel);
    CHECK(refused([&] { scope.readIsolatedCodec(stale); }).find("stale") != std::string::npos);
    CHECK(scope.occurrence(0).stage == QStringResourceStage::AwaitingCodecRead);
    CHECK(!scope.occurrence(0).selectedCodec);
    CHECK_EQ(scope.occurrence(0).nativeAllocationBytesRequested, std::size_t(0));
}

TEST_CASE(qstring_resource_branch_already_selected_is_not_retroactively_changed)
{
    QStringResourceProcessScope scope;
    selectNull(scope);
    auto selected = scope.prepareIsolatedOccurrence();
    scope.readIsolatedCodec(selected);
    const auto selectedEpoch = scope.codec().epoch;
    scope.setIsolatedCodecProfile(QStringCStringCodecKnowledge::NonNull, profileLabel);
    QStringPathLease slot;
    const auto data = scope.constructIsolatedResource(selected, slot, QStringResourceAllocationProfile::Success);
    CHECK_EQ(refs(scope, data), std::uint32_t(1));
    REQUIRE(scope.occurrence(0).selectedCodecEpoch.has_value());
    CHECK_EQ(*scope.occurrence(0).selectedCodecEpoch, selectedEpoch);
    auto next = scope.prepareIsolatedOccurrence();
    CHECK(!refused([&] { scope.readIsolatedCodec(next); }).empty());
    CHECK(scope.occurrence(1).stage == QStringResourceStage::CodecDecoderGuard);
    scope.setIsolatedCodecProfile(QStringCStringCodecKnowledge::Unknown, profileLabel);
    auto unknown = scope.prepareIsolatedOccurrence();
    CHECK(!refused([&] { scope.readIsolatedCodec(unknown); }).empty());
    CHECK(scope.occurrence(2).stage == QStringResourceStage::CodecStateGuard);
}

TEST_CASE(qstring_resource_foreign_expired_and_revoked_occurrences_refuse)
{
    static_assert(!std::is_copy_constructible_v<QStringResourceIsolatedOccurrence>);
    QStringResourceProcessScope first, second;
    selectNull(first);
    selectNull(second);
    auto occurrence = first.prepareIsolatedOccurrence();
    CHECK(!refused([&] { second.readIsolatedCodec(occurrence); }).empty());
    CHECK_EQ(second.occurrenceCount(), std::size_t(0));
    first.readIsolatedCodec(occurrence);
    first.revoke();
    QStringPathLease slot;
    CHECK(!refused([&] { first.constructIsolatedResource(occurrence, slot,
        QStringResourceAllocationProfile::Success); }).empty());
    CHECK(slot.state() == QStringPathLeaseState::Empty);
    CHECK_EQ(first.occurrence(0).nativeAllocationBytesRequested, std::size_t(0));
    std::optional<QStringResourceIsolatedOccurrence> expired;
    {
        QStringResourceProcessScope old;
        expired.emplace(old.prepareIsolatedOccurrence());
    }
    CHECK(!refused([&] { second.readIsolatedCodec(*expired); }).empty());
}

TEST_CASE(qstring_resource_destination_and_profile_validation_precede_source_allocation)
{
    QStringResourceProcessScope scope;
    CHECK(!refused([&] { scope.setIsolatedCodecProfile(QStringCStringCodecKnowledge::Null, ""); }).empty());
    CHECK(scope.codec().knowledge == QStringCStringCodecKnowledge::Unknown);
    CHECK_EQ(scope.codec().epoch, std::uint64_t(0));
    selectNull(scope);
    auto one = scope.prepareIsolatedOccurrence();
    scope.readIsolatedCodec(one);
    QStringPathLease occupied;
    const auto data = scope.constructIsolatedResource(one, occupied, QStringResourceAllocationProfile::Success);
    auto two = scope.prepareIsolatedOccurrence();
    scope.readIsolatedCodec(two);
    CHECK(!refused([&] { scope.constructIsolatedResource(two, occupied,
        QStringResourceAllocationProfile::Success); }).empty());
    CHECK(scope.occurrence(1).stage == QStringResourceStage::NullBranchSelected);
    CHECK_EQ(scope.occurrence(1).nativeAllocationBytesRequested, std::size_t(0));
    CHECK_EQ(refs(scope, data), std::uint32_t(1));
}

TEST_CASE(qstring_resource_profile_cannot_admit_actual_session_receiver_source)
{
    QStringResourceProcessScope scope;
    selectNull(scope);
    CHECK(scope.sourceBoundary() == QStringResourceSourceBoundary::MissingSessionReceiverOccurrence);
    QStringPathLease slot;
    CHECK(refused([&] { scope.constructSourceResource(slot); }).find("actual Session/receiver") != std::string::npos);
    CHECK(slot.state() == QStringPathLeaseState::Empty);
    CHECK_EQ(scope.occurrenceCount(), std::size_t(0));
    CHECK(!scope.pathOwner().sharedNull());
    auto occurrence = scope.prepareIsolatedOccurrence();
    scope.readIsolatedCodec(occurrence);
    const auto data = scope.constructIsolatedResource(occurrence, slot, QStringResourceAllocationProfile::Success);
    CHECK(!refused([&] { scope.constructSourceResource(slot); }).empty());
    CHECK_EQ(refs(scope, data), std::uint32_t(1));
    CHECK_EQ(scope.occurrenceCount(), std::size_t(1));
}

int main() { return testkit::run_all(); }
