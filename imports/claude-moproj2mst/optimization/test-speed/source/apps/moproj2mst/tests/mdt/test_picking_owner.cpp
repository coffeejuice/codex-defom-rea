// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include <algorithm>
#include <limits>
#include "m2m/engine/picking_owner.hpp"
#include <defmodel/layout/db_step_data.hpp>

using namespace m2m::engine;

TEST_CASE(picking_allocation_record_does_not_publish_or_guess_native_member_defaults)
{
    GuiModel model(nullptr, 3); PendingPickingOwner pending(model);
    CHECK(pending.identityMatches(model)); CHECK(pending.guardedModel() == &model);
    CHECK(!pending.boundNativeModel()); CHECK(!pending.nativeModelGuardConstructed());
    CHECK(!pending.reference(0)); CHECK(!pending.reference(1)); CHECK(!pending.reference(2));
    CHECK(!pending.chain()); CHECK(!pending.entryCount()); CHECK(!pending.resetValue407c());
    CHECK(pending.construction().stage == PickingConstructorStage::Allocated);
    CHECK(pending.construction().effects.empty()); CHECK(!pending.registrationRequest());
    CHECK(!PendingPickingOwner::completeForPublication);
    CHECK(!PickingConstructorReport::completeForPublication);
    CHECK(!PendingPickingOwner::postGraphicsStateInitialized);
}

TEST_CASE(picking_constructor_stops_at_graphics_call_after_original_member_and_reset_order)
{
    GuiModel model(nullptr, 3); PendingPickingOwner pending(model);
    model.simCtl()->smode = 99;
    const auto& report = pending.prepareConstructor(model);
    CHECK(report.stage == PickingConstructorStage::AwaitingGraphicsRegistration);
    CHECK(report.issue == PickingConstructorIssue::GraphicsRegistrationUnrecovered);
    const std::vector<PickingConstructorEffect> originalOrder{
        PickingConstructorEffect::ParentlessQObject, PickingConstructorEffect::EmptyEntryList,
        PickingConstructorEffect::FirstReference, PickingConstructorEffect::SecondReference,
        PickingConstructorEffect::NullModelGuard, PickingConstructorEffect::EmptyChain,
        PickingConstructorEffect::BindActualModel, PickingConstructorEffect::ResetValue407c,
        PickingConstructorEffect::EmptyEntrySnapshot, PickingConstructorEffect::ClearEntryList,
        PickingConstructorEffect::ClearSelectionPrefix};
    CHECK(report.effects == originalOrder);
    CHECK(pending.nativeModelGuardConstructed()); CHECK(pending.boundNativeModel() == &model);
    CHECK(pending.entryCount() == 0); CHECK(pending.resetValue407c() == 0);
    CHECK(model.simCtl()->smode == 99); // constructor does not fill editor pages
    CHECK(!report.repeatedPreparationRejected); CHECK(!report.reconstructionException);
    CHECK(!pending.postGraphicsStateInitialized);
}

TEST_CASE(picking_reference_records_keep_distinct_owned_empty_vectors_and_explicit_values)
{
    GuiModel model(nullptr, 3); PendingPickingOwner pending(model);
    pending.prepareConstructor(model);
    const auto* first = pending.reference(0); const auto* second = pending.reference(1);
    REQUIRE(first); REQUIRE(second); CHECK(first != second);
    std::vector<const void*> owners;
    for (const auto* ref : {first, second}) {
        CHECK(ref->nullGuardConstructed); CHECK(ref->nullModelAssignmentReached);
        CHECK(ref->modeAssignmentReached); REQUIRE(ref->values);
        const auto& values = *ref->values;
        CHECK(values.value008 == 0); CHECK(values.value0d0 == 0); CHECK(values.count0e0 == 0);
        CHECK(!values.flag080); CHECK(!values.flag0a0); CHECK(!values.flag0c0);
        CHECK(values.value0c8 == 0.0);
        for (const auto* point : {&values.point010, &values.point088, &values.point0a8})
            CHECK(std::all_of(point->begin(), point->end(), [](double value) { return value == 0.0; }));
        CHECK(values.values028.size() == 22);
        CHECK(std::all_of(values.values028.begin(), values.values028.end(), [](auto value) { return value == 0; }));
        for (const auto* vector : {ref->indices(0), ref->indices(1)}) {
            REQUIRE(vector); CHECK(vector->empty());
            CHECK(std::find(owners.begin(), owners.end(), vector) == owners.end());
            owners.push_back(vector);
        }
        CHECK(!ref->indices(2));
    }
    CHECK(owners.size() == 4);
}

TEST_CASE(picking_null_chain_does_not_execute_model_derived_branch)
{
    GuiModel model(nullptr, 3); PendingPickingOwner pending(model);
    pending.prepareConstructor(model);
    const auto* chain = pending.chain(); REQUIRE(chain);
    CHECK(!chain->first000); CHECK(!chain->last008); CHECK(!chain->model020); CHECK(!chain->value028);
    CHECK(chain->index010 == -1); CHECK(chain->count014 == 0); CHECK(chain->kind018 == 2);
    CHECK(pending.boundNativeModel() == &model); // distinct from chain+20=null
}

TEST_CASE(picking_selection_buffer_preserves_exact_byte_prefix_and_unknown_tail)
{
    GuiModel model(nullptr, 3); PendingPickingOwner pending(model);
    const auto& buffer = pending.selectionBuffer();
    CHECK(buffer.knownByteCount() == 0); CHECK(buffer.knownPrefix().empty());
    for (const auto offset : {0u, 4095u, 4096u, 16383u}) {
        const auto read = buffer.readByte(offset);
        CHECK(read.knowledge == PickingByteKnowledge::Uninitialized); CHECK(!read.value);
    }
    pending.prepareConstructor(model);
    CHECK(buffer.knownByteCount() == 4096); CHECK(buffer.knownPrefix().size() == 4096);
    CHECK(std::all_of(buffer.knownPrefix().begin(), buffer.knownPrefix().end(),
        [](std::byte value) { return value == std::byte{0}; }));
    CHECK(buffer.readByte(0).value == std::byte{0});
    CHECK(buffer.readByte(4095).knowledge == PickingByteKnowledge::Known);
    CHECK(buffer.readByte(4095).value == std::byte{0});
    for (const auto offset : {4096u, 16383u}) {
        CHECK(buffer.readByte(offset).knowledge == PickingByteKnowledge::Uninitialized);
        CHECK(!buffer.readByte(offset).value);
    }
    CHECK(buffer.readByte(16384).knowledge == PickingByteKnowledge::OutOfRange);
    CHECK(!buffer.readByte(16384).value);
    CHECK(buffer.readByte(std::numeric_limits<std::size_t>::max()).knowledge == PickingByteKnowledge::OutOfRange);
}

TEST_CASE(picking_registration_request_identifies_same_owned_buffer_and_entry_count)
{
    GuiModel model(nullptr, 3); PendingPickingOwner pending(model);
    pending.prepareConstructor(model);
    const auto request = pending.registrationRequest(); REQUIRE(request);
    CHECK(request->owner == &pending); CHECK(request->buffer == &pending.selectionBuffer());
    CHECK(request->entryCount == 4096); CHECK(request->nativeCallAddress == 0x14120316d);
    CHECK(request->entryCount * sizeof(std::uint32_t) == PickingSelectionBuffer::nativeByteExtent);
    CHECK(request->buffer->knownByteCount() * 4 == PickingSelectionBuffer::nativeByteExtent);
    CHECK(!pending.completeForPublication); // request is not a GL/context receipt
}

TEST_CASE(picking_repeated_constructor_attempt_keeps_same_partial_owners_and_receipt)
{
    GuiModel model(nullptr, 3), other(nullptr, 3); PendingPickingOwner pending(model);
    const auto& report = pending.prepareConstructor(model);
    const auto effects = report.effects;
    const auto* first = pending.reference(0); REQUIRE(first);
    const auto* vector = first->indices(0);
    const auto* buffer = &pending.selectionBuffer();
    pending.prepareConstructor(other);
    CHECK(report.repeatedPreparationRejected); CHECK(report.effects == effects);
    CHECK(report.issue == PickingConstructorIssue::GraphicsRegistrationUnrecovered);
    CHECK(pending.reference(0) == first); CHECK(first->indices(0) == vector);
    REQUIRE(pending.registrationRequest());
    CHECK(pending.registrationRequest()->buffer == buffer); CHECK(pending.boundNativeModel() == &model);
}

TEST_CASE(picking_substituted_model_refuses_before_native_prefix)
{
    GuiModel original(nullptr, 3), other(nullptr, 3); PendingPickingOwner pending(original);
    const auto& report = pending.prepareConstructor(other);
    CHECK(report.stage == PickingConstructorStage::Refused);
    CHECK(report.issue == PickingConstructorIssue::ModelMismatch);
    CHECK(report.effects.empty()); CHECK(!pending.reference(0)); CHECK(!pending.entryCount());
    CHECK(!pending.boundNativeModel()); CHECK(pending.selectionBuffer().knownByteCount() == 0);
    pending.prepareConstructor(original);
    CHECK(report.repeatedPreparationRejected); CHECK(report.issue == PickingConstructorIssue::ModelMismatch);
    CHECK(report.effects.empty());
}

TEST_CASE(picking_model_guard_expires_without_native_prefix_or_lifetime_extension)
{
    auto original = std::make_shared<GuiModel>(nullptr, 3);
    std::weak_ptr<GuiModel> observed = original;
    PendingPickingOwner pending(*original);
    original.reset(); CHECK(observed.expired()); CHECK(!pending.guardedModel());
    GuiModel replacement(nullptr, 3);
    const auto& report = pending.prepareConstructor(replacement);
    CHECK(report.issue == PickingConstructorIssue::ModelExpired); CHECK(report.effects.empty());
    CHECK(!pending.reference(0)); CHECK(!pending.registrationRequest());
    CHECK(pending.selectionBuffer().knownByteCount() == 0);
}

TEST_CASE(picking_expired_model_keeps_partial_evidence_but_revokes_registration_request)
{
    auto model = std::make_unique<GuiModel>(nullptr, 3);
    PendingPickingOwner pending(*model);
    pending.prepareConstructor(*model);
    REQUIRE(pending.registrationRequest()); const auto* reference = pending.reference(0);
    model.reset();
    CHECK(!pending.guardedModel()); CHECK(!pending.boundNativeModel());
    CHECK(!pending.registrationRequest()); CHECK(pending.reference(0) == reference);
    CHECK(pending.selectionBuffer().readByte(4095).value == std::byte{0});
    CHECK(pending.construction().stage == PickingConstructorStage::AwaitingGraphicsRegistration);
    CHECK(!pending.completeForPublication);
}

int main() { return testkit::run_all(); }
