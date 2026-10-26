// ctest-labels: m2m-doe
// Explicit isolated reference profiles, not original live counts or QFileInfo admission.
#include <testkit/check.hpp>

#include <cstdint>
#include <limits>
#include <memory>
#include <string>

#include "m2m/common.hpp"
#include "m2m/doe_qstring_path_owner.hpp"

using namespace m2m;
using namespace m2m::doe;

namespace {
QStringPathRefObservation profile(std::optional<std::uint32_t> count)
{ return {count, "declared isolated data/ref profile; no native observation claimed"}; }
QStringPathSharedNullObservation nullProfile(std::optional<std::uint32_t> count)
{ return {kQStringPathQtCoreSha256, kQStringPathSharedNullRva, profile(count)}; }
qt::String path() { return qt::L(":/SfTool/icon/tool/iconDoe_24_24.png"); }
std::uint32_t refs(const QStringPathOwner& owner, const QStringPathData& data)
{
    const auto snapshot = owner.snapshot(data);
    REQUIRE(snapshot.referenceCount.has_value());
    return *snapshot.referenceCount;
}
template<class Work> bool refused(const Work& work)
{
    try { work(); }
    catch (const Error& error) { CHECK_EQ(error.code, M2M_E_UNSUPPORTED); return true; }
    return false;
}
} // namespace

TEST_CASE(qstring_path_unknown_counts_and_shared_null_preserve_unconstructed_destination)
{
    QStringPathOwner owner;
    const auto input = owner.observeInput(path(), profile(std::nullopt));
    QStringPathLease destination;
    CHECK(!owner.snapshot(input).referenceCount);
    CHECK(refused([&] { owner.acquirePathAlias(input, destination); }));
    CHECK(refused([&] { owner.adoptExistingInput(input, destination); }));
    CHECK(destination.state() == QStringPathLeaseState::Empty);
    CHECK(!owner.sharedNull());
    CHECK(refused([&] { owner.acquireSharedNull(destination); }));
    const auto null = owner.bindSharedNull(nullProfile(std::nullopt));
    CHECK(!owner.snapshot(null).referenceCount);
    CHECK(refused([&] { owner.acquireSharedNull(destination); }));
    CHECK(destination.state() == QStringPathLeaseState::Empty);
    CHECK(!owner.snapshot(null).referenceCount); // Unknown never became a default0/1.
}

TEST_CASE(qstring_path_aliases_share_data_and_keep_native_refs_separate_from_observers)
{
    QStringPathOwner owner;
    const auto input = owner.observeInput(path(), profile(2));
    const auto observerCopy = input;
    CHECK_EQ(refs(owner, input), std::uint32_t(2));
    CHECK(observerCopy == input);
    QStringPathLease source, temporary, filename;
    owner.adoptExistingInput(input, source);
    CHECK_EQ(refs(owner, input), std::uint32_t(2)); // Admission, no native increment.
    owner.acquirePathAlias(input, temporary);
    CHECK_EQ(refs(owner, input), std::uint32_t(3));
    owner.copyAlias(temporary, filename);
    CHECK_EQ(refs(owner, input), std::uint32_t(4));
    CHECK(owner.data(source) == input);
    CHECK(owner.data(temporary) == owner.data(filename));
    CHECK(owner.text(input) == path());
    owner.release(temporary);
    CHECK_EQ(refs(owner, input), std::uint32_t(3));
    owner.release(filename);
    CHECK_EQ(refs(owner, input), std::uint32_t(2));
    owner.release(source);
    CHECK_EQ(refs(owner, input), std::uint32_t(1)); // Other observed reference remains.
}

TEST_CASE(qstring_path_file_entry_sequence_distinguishes_input_and_shared_null_owners)
{
    QStringPathOwner owner;
    const auto input = owner.observeInput(path(), profile(2));
    const auto null = owner.bindSharedNull(nullProfile(5));
    CHECK(!(input == null));
    CHECK(owner.snapshot(null).kind == QStringPathDataKind::SharedNull);
    CHECK(owner.text(null).isNull());
    QStringPathLease temporary, filename, nativePath;
    owner.acquirePathAlias(input, temporary);    // First fromNativeSeparators.
    owner.copyAlias(temporary, filename);       // File-entry filename copy.
    owner.acquireSharedNull(nativePath);
    CHECK_EQ(refs(owner, input), std::uint32_t(4));
    CHECK_EQ(refs(owner, null), std::uint32_t(6));
    owner.release(temporary);
    CHECK_EQ(refs(owner, input), std::uint32_t(3));
    owner.release(nativePath);                  // Native-path slot first.
    CHECK_EQ(refs(owner, null), std::uint32_t(5));
    CHECK_EQ(refs(owner, input), std::uint32_t(3));
    owner.release(filename);
    CHECK_EQ(refs(owner, input), std::uint32_t(2));
    CHECK(!owner.snapshot(null).dataDeallocationPending);
}

TEST_CASE(qstring_path_shared_null_binds_one_exact_module_symbol_without_default_count)
{
    QStringPathOwner owner;
    auto wrong = nullProfile(7);
    wrong.symbolRva = 0x2f0900; // QByteArray::shared_null is a different owner.
    CHECK(refused([&] { (void)owner.bindSharedNull(wrong); }));
    CHECK(!owner.sharedNull());
    wrong = nullProfile(7);
    wrong.moduleSha256 = "another QtCore image";
    CHECK(refused([&] { (void)owner.bindSharedNull(wrong); }));
    CHECK(!owner.sharedNull());
    const auto null = owner.bindSharedNull(nullProfile(7));
    REQUIRE(owner.sharedNull().has_value());
    CHECK(*owner.sharedNull() == null);
    CHECK(refused([&] { (void)owner.bindSharedNull(nullProfile(99)); }));
    CHECK_EQ(refs(owner, null), std::uint32_t(7));
    QStringPathLease first, second;
    owner.acquireSharedNull(first);
    owner.acquireSharedNull(second);
    CHECK(owner.data(first) == owner.data(second));
    CHECK_EQ(refs(owner, null), std::uint32_t(9));
    owner.release(first);
    owner.release(second);
    CHECK_EQ(refs(owner, null), std::uint32_t(7));
}

TEST_CASE(qstring_path_destination_and_existing_slot_guards_precede_reference_changes)
{
    QStringPathOwner owner;
    const auto input = owner.observeInput(path(), profile(2));
    const auto other = owner.observeInput(qt::L("other.png"), profile(5));
    QStringPathLease source, occupied, duplicate, empty;
    owner.adoptExistingInput(input, source);
    owner.acquirePathAlias(input, occupied);
    CHECK(refused([&] { owner.acquirePathAlias(other, occupied); }));
    CHECK(refused([&] { owner.copyAlias(occupied, occupied); }));
    CHECK(refused([&] { owner.adoptExistingInput(input, duplicate); }));
    CHECK(owner.data(occupied) == input);
    CHECK(duplicate.state() == QStringPathLeaseState::Empty);
    CHECK_EQ(refs(owner, input), std::uint32_t(3));
    CHECK_EQ(refs(owner, other), std::uint32_t(5));
    owner.release(occupied);
    CHECK(refused([&] { owner.copyAlias(occupied, empty); }));
    CHECK(empty.state() == QStringPathLeaseState::Empty);
    CHECK_EQ(refs(owner, input), std::uint32_t(2));
}

TEST_CASE(qstring_path_last_reference_is_consumed_before_explicit_data_free_guard)
{
    QStringPathOwner owner;
    const auto input = owner.observeInput(path(), profile(1));
    QStringPathLease source, destination;
    owner.adoptExistingInput(input, source);
    CHECK(refused([&] { owner.release(source); }));
    CHECK(source.state() == QStringPathLeaseState::Released);
    CHECK_EQ(refs(owner, input), std::uint32_t(0));
    CHECK(owner.snapshot(input).dataDeallocationPending);
    CHECK(refused([&] { owner.release(source); })); // No repeated decrement/free.
    CHECK(refused([&] { owner.acquirePathAlias(input, destination); }));
    CHECK(destination.state() == QStringPathLeaseState::Empty);
    CHECK_EQ(refs(owner, input), std::uint32_t(0));
    CHECK(owner.text(input) == path()); // Retained diagnostics, not live native data.
}

TEST_CASE(qstring_path_reference_arithmetic_preserves_uint32_wrap_without_saturation)
{
    QStringPathOwner owner;
    const auto maximum = (std::numeric_limits<std::uint32_t>::max)();
    const auto input = owner.observeInput(path(), profile(maximum));
    QStringPathLease alias;
    owner.acquirePathAlias(input, alias);
    CHECK_EQ(refs(owner, input), std::uint32_t(0));
    CHECK(!owner.snapshot(input).dataDeallocationPending); // Increment does not call free.
    owner.release(alias);
    CHECK_EQ(refs(owner, input), maximum);
    CHECK(!owner.snapshot(input).dataDeallocationPending);
}

TEST_CASE(qstring_path_scope_and_unsupported_value_guards_do_not_invent_owner_state)
{
    QStringPathOwner first, second;
    const auto input = first.observeInput(path(), profile(2));
    QStringPathLease destination;
    CHECK(refused([&] { second.acquirePathAlias(input, destination); }));
    CHECK(destination.state() == QStringPathLeaseState::Empty);
    CHECK_EQ(refs(first, input), std::uint32_t(2));
    const auto backslash = first.observeInput(qt::L("C:\\icon.png"), profile(3));
    CHECK(refused([&] { first.acquirePathAlias(backslash, destination); }));
    CHECK_EQ(refs(first, backslash), std::uint32_t(3));
    CHECK(refused([&] { (void)first.observeInput(qt::String(), profile(1)); }));
    CHECK(refused([&] { (void)first.observeInput(path(), profile(0)); }));
    CHECK(refused([&] { (void)first.observeInput(path(), {2, {}}); }));
    CHECK(refused([&] { (void)first.bindSharedNull(nullProfile(0)); }));
    CHECK(!first.sharedNull());
}

TEST_CASE(qstring_path_adapter_lease_destruction_is_not_a_native_qstring_destructor)
{
    QStringPathOwner owner;
    const auto input = owner.observeInput(path(), profile(2));
    {
        QStringPathLease alias;
        owner.acquirePathAlias(input, alias);
        CHECK_EQ(refs(owner, input), std::uint32_t(3));
    }
    CHECK_EQ(refs(owner, input), std::uint32_t(3)); // Explicit reached release is required.
}

TEST_CASE(qstring_path_expired_scope_observers_cannot_be_used_by_another_owner)
{
    QStringPathData expired;
    {
        auto owner = std::make_unique<QStringPathOwner>();
        expired = owner->observeInput(path(), profile(2));
    }
    QStringPathOwner current;
    QStringPathLease destination;
    CHECK(refused([&] { current.acquirePathAlias(expired, destination); }));
    CHECK(refused([&] { (void)current.snapshot(expired); }));
    CHECK(destination.state() == QStringPathLeaseState::Empty);
}

int main() { return testkit::run_all(); }
