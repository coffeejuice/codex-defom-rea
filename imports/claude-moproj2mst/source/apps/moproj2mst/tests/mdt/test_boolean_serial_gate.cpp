// ctest-labels: m2m-mdt
// Declared synthetic prefix contracts, not captured dispatcher/runtime values.
#include <testkit/check.hpp>
#include "m2m/engine/boolean_serial_gate.hpp"
#include "m2m/model.hpp"
#include <limits>

using namespace m2m;
using namespace m2m::engine;
namespace {
template<class F> void refused(F&& f) {
    bool caught = false;
    try { f(); } catch (const Error& e) { caught = e.code == M2M_E_UNSUPPORTED; }
    CHECK(caught);
}
struct Fixture {
    BooleanSerialNativeOwner owner{1, 2, 3};
    std::shared_ptr<BooleanSerialGateState> state;
    BooleanSerialMessageInput input;
    BooleanSerialGate frame;
    explicit Fixture(std::optional<std::uint32_t> processed = std::nullopt)
        : state(BooleanSerialGateState::createQualified(owner, processed, "synthetic canonical owner/field")) {
        input.owner = owner; input.occurrence = 1; input.prefixAdmitted = true;
        input.message = 0x401; input.qualification = "synthetic source-stage input; no original capture";
    }
    const BooleanSerialGateReport& resume() { return frame.resume(state, input); }
};
}

TEST_CASE(boolean_serial_gate_equal_serial_returns_without_dispatch_or_other_state)
{
    Fixture f(0); f.input.serialAtGate = 0;
    const auto& r = f.resume();
    CHECK(r.completed && r.stage == BooleanSerialGateStage::Complete);
    CHECK(r.serialRead == 0u && r.processedBefore == 0u && r.processedAfter == 0u);
    CHECK(!r.dispatchRequest && r.issue.empty());
    CHECK(f.state->processedSerial() == 0u);
    CHECK(f.resume().completed && !f.resume().dispatchRequest);
}

TEST_CASE(boolean_serial_gate_updates_processed_before_unclosed_dispatch_and_never_replays_store)
{
    Fixture f(11); f.input.serialAtGate = 12;
    const auto& r = f.resume();
    CHECK(!r.completed && r.stage == BooleanSerialGateStage::PostedDispatchPending);
    CHECK(r.processedBefore == 11u && r.processedAfter == 12u && f.state->processedSerial() == 12u);
    REQUIRE(r.dispatchRequest);
    const auto* request = &*r.dispatchRequest;
    CHECK(request->owner == f.owner && request->occurrence == 1u);
    CHECK(request->nullReceiver && request->requestedType == 0 && !r.issue.empty());
    CHECK(&*f.resume().dispatchRequest == request);
    refused([&] { f.state->observeProcessedSerial(11, "cannot restore pre-call value"); });
    BooleanSerialGate sameSerial; auto next = f.input; next.occurrence = 2;
    CHECK(sameSerial.resume(f.state, next).completed);
    BooleanSerialGate newerSerial; next.occurrence = 3; next.serialAtGate = 13;
    CHECK(newerSerial.resume(f.state, next).dispatchRequest.has_value());
    CHECK(f.state->processedSerial() == 13u);
    CHECK(f.resume().processedAfter == 12u); // old frame retains its own source effect
    CHECK(f.state->processedSerial() == 13u); // no repeat store/rollback from old resume
}

TEST_CASE(boolean_serial_gate_unknown_inputs_resume_without_fabricated_zero_fields)
{
    Fixture f;
    f.input.prefixAdmitted = false; f.input.message.reset();
    CHECK(f.resume().stage == BooleanSerialGateStage::Admission);
    CHECK(!f.state->processedSerial());
    f.input.prefixAdmitted = true;
    CHECK(f.resume().stage == BooleanSerialGateStage::MessageRoute);
    f.input.message = 0x401;
    CHECK(f.resume().stage == BooleanSerialGateStage::SerialRead);
    CHECK(!f.frame.report().serialRead && !f.state->processedSerial());
    f.input.serialAtGate = 4;
    CHECK(f.resume().stage == BooleanSerialGateStage::ProcessedSerialRead);
    CHECK(f.frame.report().serialRead == 4u && !f.state->processedSerial());
    f.state->observeProcessedSerial(3, "synthetic now-qualified private+ac read");
    CHECK(f.resume().stage == BooleanSerialGateStage::PostedDispatchPending);
    CHECK(f.state->processedSerial() == 4u);
}

TEST_CASE(boolean_serial_gate_uses_exact_unsigned_serial_equality_without_timer_count_inference)
{
    const auto maximum = std::numeric_limits<std::uint32_t>::max();
    Fixture f(0); f.input.serialAtGate = maximum;
    CHECK(f.resume().dispatchRequest.has_value()); CHECK(f.state->processedSerial() == maximum);
    BooleanSerialGate next; auto input = f.input; input.occurrence = 2; input.serialAtGate = 0;
    CHECK(next.resume(f.state, input).dispatchRequest.has_value());
    CHECK(f.state->processedSerial() == 0u); // explicit new input, no clock or implicit increment
}

TEST_CASE(boolean_serial_gate_matching_fallback_timer_is_not_registered_boolean_delivery)
{
    Fixture fallback(10); fallback.input.message = 0x113; fallback.input.serialAtGate = 11;
    fallback.input.fallbackTimerAtMessage = 7; fallback.input.wParam = 7;
    CHECK(fallback.resume().stage == BooleanSerialGateStage::PostedDispatchPending);
    CHECK(fallback.state->processedSerial() == 11u);
    Fixture registered(10); registered.input.message = 0x113; registered.input.serialAtGate = 11;
    registered.input.fallbackTimerAtMessage = 7; registered.input.wParam = 8;
    const auto& r = registered.resume();
    CHECK(!r.completed && r.stage == BooleanSerialGateStage::RegisteredTimerPending);
    CHECK(!r.serialRead && !r.dispatchRequest && registered.state->processedSerial() == 10u);
    Fixture absentFallback(10); absentFallback.input.message = 0x113;
    absentFallback.input.fallbackTimerAtMessage = 0;
    CHECK(absentFallback.resume().stage == BooleanSerialGateStage::RegisteredTimerPending);
    CHECK(!absentFallback.frame.report().serialRead);
}

TEST_CASE(boolean_serial_gate_unknown_timer_discriminator_and_full_width_wparam_stay_distinct)
{
    Fixture f(10); f.input.message = 0x113; f.input.serialAtGate = 11;
    CHECK(f.resume().stage == BooleanSerialGateStage::MessageRoute);
    f.input.fallbackTimerAtMessage = 7;
    CHECK(f.resume().stage == BooleanSerialGateStage::MessageRoute);
    CHECK(f.state->processedSerial() == 10u);
    f.input.wParam = 7; CHECK(f.resume().dispatchRequest.has_value());
    Fixture wide(10); wide.input.message = 0x113; wide.input.serialAtGate = 11;
    wide.input.fallbackTimerAtMessage = 0xffffffffu; wide.input.wParam = 0xffffffffffffffffull;
    CHECK(wide.resume().stage == BooleanSerialGateStage::RegisteredTimerPending);
    Fixture exact(10); exact.input.message = 0x113; exact.input.serialAtGate = 11;
    exact.input.fallbackTimerAtMessage = 0xffffffffu; exact.input.wParam = 0xffffffffull;
    CHECK(exact.resume().stage == BooleanSerialGateStage::PostedDispatchPending);
}

TEST_CASE(boolean_serial_gate_rejects_owner_alias_control_block_and_occurrence_substitution)
{
    Fixture f(10); f.input.serialAtGate = 11; f.resume();
    auto changed = f.input; ++changed.owner.threadData;
    refused([&] { f.frame.resume(f.state, changed); });
    changed = f.input; ++changed.occurrence;
    refused([&] { f.frame.resume(f.state, changed); });
    auto other = BooleanSerialGateState::createQualified(f.owner, 10, "another model owner with same aliases");
    refused([&] { f.frame.resume(other, f.input); });
    auto foreign = std::shared_ptr<BooleanSerialGateState>(f.state.get(), [](BooleanSerialGateState*) {});
    BooleanSerialGate firstForeign;
    auto freshOccurrence = f.input; freshOccurrence.occurrence = 99;
    refused([&] { firstForeign.resume(foreign, freshOccurrence); });
    BooleanSerialGate replay;
    refused([&] { replay.resume(f.state, f.input); });
    CHECK(f.state->processedSerial() == 11u && other->processedSerial() == 10u);
}

TEST_CASE(boolean_serial_gate_rejects_changed_source_values_and_expired_admission_without_rollback)
{
    Fixture f(10); f.input.serialAtGate = 11; f.resume();
    auto changed = f.input; changed.serialAtGate = 12;
    refused([&] { f.frame.resume(f.state, changed); });
    CHECK(f.state->processedSerial() == 11u && f.frame.report().processedAfter == 11u);
    f.state->releaseAdmissionScope();
    refused([&] { f.resume(); });
    refused([&] { f.state->observeProcessedSerial(11, "expired admission"); });
    CHECK(f.state->processedSerial() == 11u); // expiry does not fake native restoration
}

TEST_CASE(boolean_serial_gate_weak_frame_cannot_adopt_a_replacement_owner_after_expiry)
{
    Fixture f(10); f.input.serialAtGate = 11; f.resume();
    f.state.reset();
    auto replacement = BooleanSerialGateState::createQualified(f.owner, 10, "fresh owner, same local aliases");
    refused([&] { f.frame.resume(replacement, f.input); });
    CHECK(replacement->processedSerial() == 10u);
}

TEST_CASE(boolean_serial_gate_unqualified_and_other_messages_do_not_run_posted_dispatch)
{
    refused([&] { BooleanSerialGateState::createQualified({}, 0, "missing native identities"); });
    refused([&] { BooleanSerialGateState::createQualified({1,2,3}, 0, ""); });
    Fixture f(10); f.input.serialAtGate = 11; f.input.qualification.clear();
    refused([&] { f.resume(); }); CHECK(f.state->processedSerial() == 10u);
    f.input.qualification = "synthetic other-message branch"; f.input.message = 0x400;
    CHECK(f.resume().stage == BooleanSerialGateStage::OtherMessagePending);
    CHECK(!f.frame.report().completed && !f.frame.report().dispatchRequest && !f.frame.report().serialRead);
    CHECK(f.state->processedSerial() == 10u);
}

int main() { return testkit::run_all(); }
