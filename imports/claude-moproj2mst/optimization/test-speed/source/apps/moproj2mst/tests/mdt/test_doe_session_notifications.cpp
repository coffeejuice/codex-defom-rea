// ctest-labels: m2m-mdt mdt-l6 m2m-mdt-l7
#include <testkit/check.hpp>

#include <string>
#include <memory>
#include <utility>

#include "m2m/common.hpp"
#include "m2m/doe_session.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/doe_task_runtime.hpp"
#include "m2m/doe_task_factory.hpp"

using namespace m2m;

namespace {
void setRoot(doe::SessionDocument& document, const std::string& bytes)
{
    xml::Document parsed;
    qt::String error;
    int line = 0, column = 0;
    REQUIRE(xml::setContent(parsed, bytes, &error, &line, &column));
    document.dom = std::move(parsed);
    document.partRoot = document.dom.root;
}

bool refuses(const auto& action, const char* contains)
{
    try { action(); }
    catch (const Error& error) {
        return error.code == M2M_E_UNSUPPORTED &&
            std::string(error.what()).find(contains) != std::string::npos;
    }
    return false;
}
} // namespace

TEST_CASE(doe_session_final_selection_first_entity_zero_precedes_reset_and_four_empty_dispatches)
{
    for (const bool native : {false, true}) {
        auto allocations = std::make_shared<doe::TaskAllocationStore>();
        auto io = native ? doe::makeNativeStudyIo() : doe::makeRecordingStudyIo();
        io->bindTaskAllocationStore(allocations);
        auto taskProjection = doe::bindBoundedParsedDoeProjection(*io, allocations);
        doe::SessionDocument document;
        setRoot(document, "<PartRoot><SessionRoot PartEntityNo='4'><Session/></SessionRoot>"
            "<SessionRoot PartEntityNo='0'><Session/><Session/></SessionRoot>"
            "<SessionRoot PartEntityNo='0'><Session/></SessionRoot></PartRoot>");
        io->sessionNotificationState().flag90 = true; //declared helper-entry flag
        doe::readSessionRecords(document, {[] {}, {}, {}, io.get()});
        REQUIRE(document.currentPart && *document.currentPart == 1);
        CHECK(document.parts[0].currentSession == 0 && document.parts[1].currentSession == -1);
        CHECK(document.parts[2].currentSession == 0); //duplicate entity0 did not win
        const auto& state = io->sessionNotificationState();
        REQUIRE(state.attempts.size() == 4);
        const auto& first = state.attempts[0];
        CHECK(first.signal == "2signalChangeStepRange()" && !first.suppressed);
        CHECK(first.currentPart && *first.currentPart == 1);
        CHECK(first.currentSession && *first.currentSession == 1 && !first.argument);
        CHECK(state.attempts[1].signal == "2signalOpenSession()" && !state.attempts[1].argument);
        CHECK(state.attempts[2].signal == "2signalChapterChanged()");
        CHECK(state.attempts[2].argument && *state.attempts[2].argument == -1);
        CHECK(state.attempts[3].signal == "2signalNumChaptersChanged()");
        CHECK(state.attempts[3].argument && *state.attempts[3].argument == 2);
        for (std::size_t index = 1; index < 4; ++index) {
            CHECK(state.attempts[index].currentPart && *state.attempts[index].currentPart == 1);
            CHECK(state.attempts[index].currentSession && *state.attempts[index].currentSession == -1);
        }
        CHECK(!state.flag90 && state.emptyKeys.size() == 4);
        for (const auto& attempt : state.attempts) CHECK(attempt.emptyKeyInserted);
        CHECK(io->reads().empty() && io->textWrites().empty() && io->directories().empty());
    }
}

TEST_CASE(doe_session_no_entity_zero_observes_null_current_before_ordinal_fallback)
{
    auto allocations = std::make_shared<doe::TaskAllocationStore>();
    auto io = doe::makeRecordingStudyIo();
    io->bindTaskAllocationStore(allocations);
    auto taskProjection = doe::bindBoundedParsedDoeProjection(*io, allocations);
    doe::SessionDocument document;
    setRoot(document, "<PartRoot><SessionRoot PartEntityNo='7'><Session/></SessionRoot>"
        "<SessionRoot PartEntityNo='8'><Session/><Session/></SessionRoot></PartRoot>");
    doe::readSessionRecords(document, {[] {}, {}, {}, io.get()});
    REQUIRE(document.currentPart && *document.currentPart == 0);
    CHECK(document.parts[0].currentSession == -1 && document.parts[1].currentSession == 1);
    const auto& attempts = io->sessionNotificationState().attempts;
    REQUIRE(attempts.size() == 4);
    CHECK(!attempts[0].currentPart && !attempts[0].currentSession);
    CHECK(attempts[1].currentPart && *attempts[1].currentPart == 0);
    CHECK(attempts[3].argument && *attempts[3].argument == 1);
}

TEST_CASE(doe_session_missing_notifier_state_guards_after_selection_before_reset_or_dispatch)
{
    doe::SessionDocument document;
    // No Session allocation is requested, so the later notifier boundary can
    // be exercised independently of the earlier allocation-store dependency.
    setRoot(document, "<PartRoot><SessionRoot PartEntityNo='9'/>"
        "<SessionRoot PartEntityNo='0'/></PartRoot>");
    CHECK(refuses([&] { doe::readSessionRecords(document, {[] {}}); }, "signalChangeStepRange"));
    REQUIRE(document.currentPart && *document.currentPart == 1);
    CHECK(document.parts[0].currentSession == -1 && document.parts[1].currentSession == -1);
}

TEST_CASE(doe_session_suppressed_dispatch_preserves_flag_and_map_but_selection_still_finishes)
{
    auto allocations = std::make_shared<doe::TaskAllocationStore>();
    auto io = doe::makeRecordingStudyIo();
    io->bindTaskAllocationStore(allocations);
    auto& state = io->sessionNotificationState();
    state.suppressed = true; //explicit helper-entry state, not fresh-process default
    state.flag90 = true;
    doe::SessionDocument document;
    setRoot(document, "<PartRoot><SessionRoot><Session/></SessionRoot></PartRoot>");
    doe::readSessionRecords(document, {[] {}, {}, {}, io.get()});
    CHECK(document.currentPart && *document.currentPart == 0);
    CHECK(document.parts[0].currentSession == -1);
    CHECK(state.suppressed && state.flag90 && state.emptyKeys.empty());
    REQUIRE(state.attempts.size() == 4);
    for (const auto& attempt : state.attempts)
        CHECK(attempt.suppressed && !attempt.emptyKeyInserted);
}

TEST_CASE(doe_session_empty_keys_reuse_across_reads_without_rewriting_old_observations)
{
    auto allocations = std::make_shared<doe::TaskAllocationStore>();
    auto io = doe::makeRecordingStudyIo();
    io->bindTaskAllocationStore(allocations);
    auto taskProjection = doe::bindBoundedParsedDoeProjection(*io, allocations);
    doe::SessionDocument document;
    setRoot(document, "<PartRoot><SessionRoot><Session/></SessionRoot></PartRoot>");
    doe::readSessionRecords(document, {[] {}, {}, {}, io.get()});
    setRoot(document, "<PartRoot><SessionRoot><Session/><Session/></SessionRoot></PartRoot>");
    doe::readSessionRecords(document, {[] {}, {}, {}, io.get()});
    const auto& state = io->sessionNotificationState();
    REQUIRE(state.attempts.size() == 8);
    CHECK(state.emptyKeys.size() == 4);
    CHECK(state.attempts[0].currentSession && *state.attempts[0].currentSession == 0);
    CHECK(state.attempts[4].currentSession && *state.attempts[4].currentSession == 1);
    CHECK(state.attempts[3].argument && *state.attempts[3].argument == 1);
    CHECK(state.attempts[7].argument && *state.attempts[7].argument == 2);
    for (std::size_t index = 4; index < 8; ++index) CHECK(!state.attempts[index].emptyKeyInserted);
}

TEST_CASE(doe_session_empty_document_and_unproven_signal_keep_their_earlier_guards)
{
    auto io = doe::makeRecordingStudyIo();
    doe::SessionDocument document;
    document.parts.emplace_back();
    document.currentPart = 0;
    setRoot(document, "<PartRoot/>");
    CHECK(refuses([&] { doe::readSessionRecords(document, {[] {}, {}, {}, io.get()}); }, "0x1406469d0"));
    CHECK(document.parts.empty() && !document.currentPart && document.partRoot->children.empty());
    CHECK(io->observedSessionNotificationState() == nullptr);
    CHECK(refuses([&] { doe::observeEmptySessionNotification(*io,
        "2signalChangePtrkGraph()", {}, {}); }, "no recovered empty binding"));
    CHECK(io->observedSessionNotificationState() == nullptr);
}

TEST_CASE(doe_notification_shared_owner_survives_io_release_and_is_not_copied)
{
    auto owner = std::make_shared<doe::SessionNotificationState>();
    auto first = doe::makeRecordingStudyIo();
    auto second = doe::makeRecordingStudyIo();
    first->bindSessionNotificationState(owner);
    first->bindSessionNotificationState(owner); //idempotent
    second->bindSessionNotificationState(owner);
    doe::observeEmptySessionNotification(*first, "2signalOpenSession()", {}, {});
    CHECK(first->observedSessionNotificationState() == owner.get());
    first.reset();
    CHECK(owner->attempts.size() == 1 && second->observedSessionNotificationState() == owner.get());
    owner->suppressed = true;
    owner->flag90 = true;
    doe::observeEmptySessionNotification(*second, "2signalChapterChanged()", {}, {}, -1);
    REQUIRE(owner->attempts.size() == 2);
    CHECK(owner->attempts[1].suppressed && owner->flag90);
    CHECK(owner->emptyKeys.size() == 1);
}

TEST_CASE(doe_notification_owner_binding_rejects_discarding_existing_or_null_state)
{
    auto io = doe::makeRecordingStudyIo();
    auto owner = std::make_shared<doe::SessionNotificationState>();
    io->bindSessionNotificationState(owner);
    doe::observeEmptySessionNotification(*io, "2signalOpenSession()", {}, {});
    CHECK(refuses([&] { io->bindSessionNotificationState(nullptr); }, "before observations"));
    CHECK(refuses([&] { io->bindSessionNotificationState(
        std::make_shared<doe::SessionNotificationState>()); }, "before observations"));
    CHECK(io->observedSessionNotificationState() == owner.get() && owner->attempts.size() == 1);
    auto isolated = doe::makeRecordingStudyIo();
    auto* old = &isolated->sessionNotificationState();
    CHECK(refuses([&] { isolated->bindSessionNotificationState(owner); }, "before observations"));
    CHECK(isolated->observedSessionNotificationState() == old);
}

TEST_CASE(doe_equipment_empty_notification_uses_shared_suppression_flag_and_key_for_both_ids)
{
    auto io = doe::makeRecordingStudyIo();
    auto owner = std::make_shared<doe::SessionNotificationState>();
    io->bindSessionNotificationState(owner);
    const std::string signal = "2signalMoEquipManagerChanged()";
    owner->suppressed = true;
    owner->flag90 = true;
    doe::observeEmptySessionNotification(*io, signal, {}, {}, -1); //wrapper no-row observation
    CHECK(owner->flag90);
    CHECK(!owner->emptyKeys.contains(signal));
    REQUIRE(owner->attempts.size() == 1u);
    CHECK(owner->attempts.back().suppressed);
    CHECK(owner->attempts.back().argument == -1);
    owner->suppressed = false;
    doe::observeEmptySessionNotification(*io, signal, {}, {}, 17); //add ID
    CHECK(!owner->flag90);
    CHECK(owner->emptyKeys.contains(signal));
    CHECK(owner->attempts.back().emptyKeyInserted);
    CHECK(owner->attempts.back().argument == 17);
    owner->flag90 = true;
    doe::observeEmptySessionNotification(*io, signal, {}, {}, 16); //remove ID-1, same key
    CHECK(!owner->flag90);
    CHECK(!owner->attempts.back().emptyKeyInserted);
    CHECK(owner->attempts.back().argument == 16);
    CHECK_EQ(owner->emptyKeys.size(), 1u);
    CHECK(io->observedSessionNotificationState() == owner.get());
}

int main() { return testkit::run_all(); }
