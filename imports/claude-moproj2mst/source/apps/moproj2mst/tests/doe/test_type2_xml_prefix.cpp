// ctest-labels: m2m-doe
#include <testkit/check.hpp>
#include "../support/test_paths.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iterator>
#include <string>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/doe_xml_output_device.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "../../src/doe/type2_xml_prefix.hpp"

using namespace m2m;
using namespace m2m::doe;
namespace {
bool unsupported(const auto& work, const char* detail)
{
    try { work(); }
    catch (const Error& e) { return e.code == M2M_E_UNSUPPORTED && std::string(e.what()).find(detail) != std::string::npos; }
    return false;
}
struct ScriptState {
    bool open = true, closed = false, rejectWrite = false;
    int finalError = 0, errorQueries = 0;
    std::string bytes = "old bytes";
    std::function<void()> onOpen;
};
class ScriptBinary final : public StudyXmlFile {
public:
    explicit ScriptBinary(std::shared_ptr<ScriptState> state) : state_(std::move(state)) {}
    ~ScriptBinary() override { close(); }
    std::int64_t write(std::string_view bytes) override
    {
        if (!opened() || state_->rejectWrite) return -1;
        state_->bytes.append(bytes);
        return static_cast<std::int64_t>(bytes.size());
    }
    bool flush() override { return opened(); }
    void close() override
    {
        if (!opened()) return;
        notifyAboutToClose();
        state_->closed = true;
    }
    bool opened() const override { return state_->open && !state_->closed; }
    int error() const override
    {
        CHECK(state_->closed);
        ++state_->errorQueries;
        return state_->finalError;
    }
private:
    std::shared_ptr<ScriptState> state_;
};
class ScriptIo final : public StudyIo {
public:
    ScriptIo() : StudyIo(IoMode::Recording), state(std::make_shared<ScriptState>()) {}
    bool ensureDirectory(const qt::String& p) override { return recordDirectory(p, false, true, true); }
    std::unique_ptr<StudyXmlFile> openInputTableXmlFile(const qt::String& path) override
    {
        if (state->open) state->bytes.clear();
        if (state->onOpen) state->onOpen();
        return observeInputTableXmlFile(path, textModeStudyXmlFile(std::make_unique<ScriptBinary>(state)));
    }
    std::shared_ptr<ScriptState> state;
};
struct Rig {
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session{driver};
    SubProject* sub;
    xml::NodePtr pending = xml::createElement(qt::L("Sub_Project"));
    Type2SaveNodePrefixResult node;
    explicit Rig(std::shared_ptr<StudyIo> io = makeRecordingStudyIo())
    {
        auto& p = session.project();
        initProject(p, qt::L("C:/PROBLEM"), qt::L("type2_xml"));
        p.doeIo = std::move(io);
        p.doeIo->bindModelManager(session.modelManagerOwner());
        p.doeIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
        p.subs.push_back(makeSubProject(p, 2, 7));
        sub = p.subs.back().get();
        p.activeSub = 1;
        node = appendType2SaveNodePrefix(session, p, *sub, *pending);
    }
    Type2InputTableXmlResult write()
    { return writeType2InputTableXmlPrefix(session, session.project(), *sub, node, 1252); }
};
}

TEST_CASE(type2_xml_recording_writes_locale_text_immediately_then_stops_before_refresh)
{
    Rig rig;
    auto& p = rig.session.project();
    const auto io = p.doeIo;
    rig.sub->ownedDoeStudy->commonMeshOrigName = qt::String(u"\u00e9\u6f22");
    io->setLastError(qt::L("prior error"));
    auto notifier = rig.session.sessionNotificationStateOwner();
    notifier->flag90 = true;
    const auto result = rig.write();
    CHECK(result.opened && result.writerSucceeded && !result.streamFailed);
    CHECK(result.fileErrorAfterClose == 0);
    CHECK(std::string(result.guardMessage()).find("signalDoeTableRefresh()") != std::string::npos);
    std::string bytes;
    REQUIRE(io->readFile(result.path, bytes));
    CHECK(bytes.find(std::string("\xe9?")) != std::string::npos);
    CHECK(bytes.find("\r\n") != std::string::npos && bytes.find("\r\r\n") == std::string::npos);
    CHECK(bytes.find("<DoeVariables") != std::string::npos && bytes.find("<Operations") != std::string::npos);
    CHECK(io->lastError() == qt::L("prior error")); // Success does not clear global error.
    REQUIRE(io->xmlWrites().size() == 1);
    const auto& attempt = io->xmlWrites()[0];
    CHECK(attempt.opened && attempt.closed && attempt.errorAfterClose == 0);
    CHECK(attempt.phases.front() == XmlWritePhase::Open && attempt.phases.back() == XmlWritePhase::ErrorAfterClose);
    const auto closing = std::find(attempt.phases.begin(), attempt.phases.end(), XmlWritePhase::Closing);
    const auto writing = std::find(attempt.phases.begin(), attempt.phases.end(), XmlWritePhase::Write);
    const auto closed = std::find(attempt.phases.begin(), attempt.phases.end(), XmlWritePhase::Closed);
    CHECK(closing < writing && writing < closed); // Buffered text flush belongs to aboutToClose.
    CHECK(notifier->flag90 && notifier->attempts.empty() && io->textWrites().empty());
}

TEST_CASE(type2_xml_open_false_preserves_old_bytes_sets_error_and_still_reaches_refresh_guard)
{
    auto io = std::make_shared<ScriptIo>();
    io->state->open = false;
    Rig rig(io);
    rig.sub->ownedDoeStudy->constraints = 1; // Failed open must not reach DOM serialization guard.
    const auto result = rig.write();
    CHECK(!result.opened && !result.writerSucceeded && !result.fileErrorAfterClose);
    CHECK(io->state->bytes == "old bytes" && io->state->errorQueries == 0);
    CHECK(io->lastError() == qt::arg(qt::L("Failed to open file, %1"), result.path));
    REQUIRE(io->xmlWrites().size() == 1);
    CHECK(io->xmlWrites()[0].phases == std::vector<XmlWritePhase>({XmlWritePhase::Open}));
    CHECK(std::string(result.guardMessage()).find("Refresh") != std::string::npos);
}

TEST_CASE(type2_xml_post_close_file_error_drives_false_separately_from_stream_failure)
{
    auto io = std::make_shared<ScriptIo>();
    io->state->finalError = 7;
    Rig rig(io);
    const auto result = rig.write();
    CHECK(result.opened && !result.writerSucceeded && !result.streamFailed);
    CHECK(result.fileErrorAfterClose == 7 && io->state->closed && io->state->errorQueries == 1);
    CHECK(!io->state->bytes.empty()); // Earlier accepted text is not rolled back.
    CHECK(io->lastError() == qt::arg(qt::L("Error writing file, %1"), result.path));
    CHECK(io->xmlWrites()[0].phases.back() == XmlWritePhase::ErrorAfterClose);
}

TEST_CASE(type2_xml_file_error_zero_can_report_true_despite_sticky_stream_failure)
{
    auto io = std::make_shared<ScriptIo>();
    io->state->rejectWrite = true;
    io->state->finalError = 0; // Explicit QFile post-close error observation, not PAR status.
    Rig rig(io);
    const auto result = rig.write();
    CHECK(result.opened && result.writerSucceeded && result.streamFailed);
    CHECK(result.fileErrorAfterClose == 0 && io->state->bytes.empty());
    REQUIRE(io->xmlWrites()[0].acceptedStreamBytes.size() == 1);
    CHECK(io->xmlWrites()[0].acceptedStreamBytes[0] == -1);
}

TEST_CASE(type2_xml_serialization_refusal_retains_open_truncation_before_any_body_bytes)
{
    Rig rig;
    const auto io = rig.session.project().doeIo;
    const auto target = projectDir(rig.session.project()) + qt::L("/DoeStudies/DoeStudy7/Control/DoeInputTable.XML");
    io->writeTextFile(target, "old bytes");
    rig.sub->ownedDoeStudy->constraints = 1;
    CHECK(unsupported([&] { rig.write(); }, "after XML open"));
    std::string bytes;
    REQUIRE(io->readFile(target, bytes));
    CHECK(bytes.empty());
    REQUIRE(io->xmlWrites().size() == 1);
    CHECK(io->xmlWrites()[0].opened && io->xmlWrites()[0].closed);
    CHECK(io->xmlWrites()[0].requestedStreamBytes.empty() && !io->xmlWrites()[0].errorAfterClose);
    CHECK(!io->xmlWrites()[0].normalWriterReturn && !io->xmlWrites()[0].refreshBoundaryReached);
    CHECK(unsupported([&] { io->observeInputTableRefreshBoundary(0, io->xmlWrites()[0].path); }, "normal return"));
}

TEST_CASE(type2_xml_owner_change_during_open_refuses_after_retaining_truncation)
{
    auto io = std::make_shared<ScriptIo>();
    Rig rig(io);
    io->state->onOpen = [&] { rig.session.project().doeIo = makeRecordingStudyIo(); };
    CHECK(unsupported([&] { rig.write(); }, "cached Study/I/O/manager"));
    CHECK(io->state->bytes.empty() && io->state->closed && io->state->errorQueries == 0);
    REQUIRE(io->xmlWrites().size() == 1);
    CHECK(io->xmlWrites()[0].requestedStreamBytes.empty());
    CHECK(!io->xmlWrites()[0].normalWriterReturn);
}

TEST_CASE(type2_xml_nonempty_variables_guard_precedes_open)
{
    auto io = std::make_shared<ScriptIo>();
    Rig rig(io);
    rig.sub->ownedDoeStudy->vars.push_back(nullptr);
    CHECK(unsupported([&] { rig.write(); }, "empty variables"));
    CHECK(io->xmlWrites().empty() && io->state->bytes == "old bytes");
    rig.sub->ownedDoeStudy->vars.clear();
}

TEST_CASE(type2_xml_native_device_truncates_and_writes_single_text_translation)
{
    namespace fs = std::filesystem;
    const auto path = m2m_test::scratchOutput(m2m_test::scratchRoot() / "type2_xml_prefix" / "native.xml");
    fs::create_directories(path.parent_path());
    { std::ofstream old(path, std::ios::binary); old << "old bytes to truncate"; }
    auto io = makeNativeStudyIo();
    const qt::String target(path.u16string());
    auto file = io->openInputTableXmlFile(target);
    REQUIRE(file->opened());
    { mst::Stream stream(1252, file.get()); stream << qt::String(u"\u00e9\n"); file->close(); }
    CHECK(file->error() == 0);
    std::ifstream input(path, std::ios::binary);
    const std::string bytes{std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
    CHECK(bytes == std::string("\xe9\r\n"));
    const auto journal = io->xmlJournalOwner();
    REQUIRE(journal->attempts().size() == 1);
    CHECK(!journal->recordedFile(target));
    CHECK(!journal->attempts()[0].recordingStoreTracked);
    CHECK(journal->attempts()[0].recordingRawSpans.empty());
    CHECK(!journal->attempts()[0].recordedBytesAfterClose); // Never infer native bytes from requests.
}

TEST_CASE(type2_xml_canonical_recording_open_fault_preserves_shared_store)
{
    RecordingXmlFault fault;
    fault.opened = false;
    fault.label = "declared failed open";
    auto io = makeRecordingStudyIo({fault});
    Rig rig(io);
    const auto target = projectDir(rig.session.project()) + qt::L("/DoeStudies/DoeStudy7/Control/DoeInputTable.XML");
    io->writeTextFile(target, "prior PAR/interface store value");
    const auto journal = io->xmlJournalOwner();
    CHECK(journal->recordedFile(target) == "prior PAR/interface store value");
    const auto result = rig.write();
    CHECK(!result.opened && !result.writerSucceeded);
    REQUIRE(journal->attempts().size() == 1);
    CHECK(&io->xmlWrites() == &journal->attempts());
    CHECK(journal->attempts()[0].recordingProfile == fault.label);
    CHECK(journal->attempts()[0].recordingRawSpans.empty());
    CHECK(!journal->attempts()[0].recordedBytesAfterClose);
    CHECK(journal->recordedFile(target) == "prior PAR/interface store value");
    REQUIRE(journal->events().size() == 3);
    CHECK(journal->events()[0].kind == XmlJournalEventKind::OpenRequested);
    CHECK(journal->events()[1].kind == XmlJournalEventKind::OpenResult);
    CHECK(journal->events()[2].kind == XmlJournalEventKind::NormalWriterReturn);
    REQUIRE(journal->attempts()[0].normalWriterReturn.has_value());
    CHECK(!journal->attempts()[0].normalWriterReturn->writerSucceeded);
    CHECK(!journal->attempts()[0].normalWriterReturn->streamFailed);
    CHECK(journal->attempts()[0].normalWriterReturn->stickyLastError == io->lastError());
}

TEST_CASE(type2_xml_recording_short_raw_span_records_nonprefix_translated_file)
{
    RecordingXmlFault fault;
    fault.rawWriteReturns = {2};
    fault.label = "declared first raw span short";
    auto io = makeRecordingStudyIo({fault});
    const auto path = qt::L("C:/PROBLEM/recorded_raw.xml");
    auto file = io->openInputTableXmlFile(path);
    bool streamFailed = false;
    { mst::Stream stream(1252, file.get()); stream << "abcd\nZ"; file->close(); streamFailed = stream.writeFailed(); }
    CHECK(streamFailed && file->error() == 0);
    const auto journal = io->xmlJournalOwner();
    REQUIRE(journal->attempts().size() == 1);
    const auto& attempt = journal->attempts()[0];
    REQUIRE(attempt.recordingRawSpans.size() == 3);
    CHECK(attempt.recordingRawSpans[0].requestedBytes == "abcd");
    CHECK(attempt.recordingRawSpans[0].returned == 2);
    CHECK(attempt.recordingRawSpans[0].acceptedRecordingBytes == "ab");
    CHECK(attempt.recordingRawSpans[1].acceptedRecordingBytes == "\r\n");
    CHECK(attempt.recordingRawSpans[2].acceptedRecordingBytes == "Z");
    CHECK(attempt.requestedStreamBytes == "abcd\nZ" && attempt.acceptedStreamBytes[0] == 4);
    CHECK(attempt.recordedBytesAfterClose == "ab\r\nZ");
    CHECK(journal->recordedFile(path) == "ab\r\nZ");
    std::string bytes;
    REQUIRE(io->readFile(path, bytes));
    CHECK(bytes == "ab\r\nZ");
    auto reader = io->openReadFile(path);
    REQUIRE(reader);
    CHECK(reader->size() == 5 && reader->readAll() == bytes);
    CHECK(journal->events().front().kind == XmlJournalEventKind::OpenRequested);
    CHECK(journal->events().back().kind == XmlJournalEventKind::ErrorAfterClose);
}

TEST_CASE(type2_xml_common_recording_faults_keep_stream_file_status_and_actual_bytes_separate)
{
    for (int profile = 0; profile < 4; ++profile) {
        RecordingXmlFault fault;
        fault.label = "explicit XML device observation profile";
        fault.expectedPath = qt::L("C:/PROBLEM/DoeStudies/DoeStudy7/Control/DoeInputTable.XML");
        fault.expectedAttemptOrdinal = 0;
        if (profile < 2) fault.rawWriteReturns = {-1};
        if (profile == 2) fault.flushReturns = {false};
        fault.errorAfterClose = profile == 1 ? 0 : 7;
        auto io = makeRecordingStudyIo({fault});
        Rig rig(io);
        io->setLastError(qt::L("prior sticky error"));
        const auto notifier = rig.session.sessionNotificationStateOwner();
        notifier->flag90 = true;
        const auto result = rig.write();
        CHECK(result.opened && result.writerSucceeded == (profile == 1));
        CHECK(result.streamFailed == (profile != 3));
        CHECK(result.fileErrorAfterClose == fault.errorAfterClose);
        const auto journal = io->xmlJournalOwner();
        REQUIRE(journal->attempts().size() == 1);
        const auto& attempt = journal->attempts()[0];
        REQUIRE(attempt.recordedBytesAfterClose.has_value());
        CHECK(attempt.recordedBytesAfterClose->empty() == (profile < 2));
        std::string accepted;
        for (const auto& span : attempt.recordingRawSpans) accepted += span.acceptedRecordingBytes;
        CHECK(accepted == *attempt.recordedBytesAfterClose);
        CHECK(journal->recordedFile(result.path) == accepted);
        CHECK(attempt.errorAfterClose == fault.errorAfterClose);
        REQUIRE(attempt.normalWriterReturn.has_value());
        CHECK(attempt.normalWriterReturn->writerSucceeded == result.writerSucceeded);
        CHECK(attempt.normalWriterReturn->streamFailed == result.streamFailed);
        CHECK(attempt.normalWriterReturn->stickyLastError == io->lastError());
        if (profile == 1) CHECK(attempt.normalWriterReturn->stickyLastError == qt::L("prior sticky error"));
        CHECK(!attempt.refreshBoundaryReached);
        CHECK(journal->events().back().kind == XmlJournalEventKind::NormalWriterReturn);
        io->observeInputTableRefreshBoundary(0, result.path);
        CHECK(attempt.refreshBoundaryReached);
        CHECK(journal->events().back().kind == XmlJournalEventKind::RefreshBoundaryReached);
        CHECK(notifier->flag90);
        CHECK(rig.session.sessionNotificationStateOwner()->attempts.empty());
    }
}

TEST_CASE(type2_xml_recording_attempts_and_faults_are_instance_local_and_ordered)
{
    RecordingXmlFault success, fail;
    success.label = "first success";
    success.expectedAttemptOrdinal = 0;
    fail.opened = false;
    fail.label = "second open failure";
    fail.expectedAttemptOrdinal = 1;
    auto io = makeRecordingStudyIo({success, fail});
    Rig rig(io);
    const auto first = rig.write();
    const auto journal = io->xmlJournalOwner();
    const auto prior = journal->recordedFile(first.path);
    REQUIRE(prior.has_value());
    const auto second = rig.write();
    CHECK(first.writerSucceeded && !second.writerSucceeded);
    REQUIRE(journal->attempts().size() == 2);
    CHECK(journal->attempts()[0].recordingProfile == "first success");
    CHECK(journal->attempts()[1].recordingProfile == "second open failure");
    CHECK(journal->attempts()[0].recordedBytesAfterClose == prior);
    CHECK(!journal->attempts()[1].recordedBytesAfterClose && journal->recordedFile(first.path) == prior);
    CHECK(journal->attempts()[0].normalWriterReturn->writerSucceeded);
    CHECK(!journal->attempts()[1].normalWriterReturn->writerSucceeded);
    CHECK(unsupported([&] { io->observeInputTableRefreshBoundary(0, first.path); }, "latest exact"));
    io->observeInputTableRefreshBoundary(1, second.path);
    CHECK(journal->attempts()[1].refreshBoundaryReached);
    bool sawSecond = false;
    for (const auto& event : journal->events()) {
        if (event.attempt == 1) sawSecond = true;
        if (sawSecond) CHECK(event.attempt == 1);
    }
    Rig separate;
    CHECK(separate.write().writerSucceeded); // No consumed fault/profile leaks across instances.
    CHECK(separate.session.project().doeIo->xmlJournalOwner() != journal);
}

TEST_CASE(type2_xml_readonly_journal_survives_io_release_without_native_owners)
{
    std::shared_ptr<const StudyXmlJournal> journal;
    qt::String path;
    {
        Rig rig;
        journal = rig.session.project().doeIo->xmlJournalOwner();
        const auto result = rig.write();
        path = result.path;
    }
    REQUIRE(journal && journal->attempts().size() == 1);
    CHECK(journal->attempts()[0].closed && journal->attempts()[0].errorAfterClose == 0);
    CHECK(journal->recordedFile(path) == journal->attempts()[0].recordedBytesAfterClose);
    CHECK(journal->events().back().kind == XmlJournalEventKind::NormalWriterReturn);
}

TEST_CASE(type2_xml_return_and_boundary_observations_bind_actual_device_path_and_ordinal)
{
    const auto path = qt::L("C:/PROBLEM/normal_xml_return.xml");
    RecordingXmlFault fault;
    fault.expectedPath = path;
    fault.expectedAttemptOrdinal = 0;
    auto io = makeRecordingStudyIo({fault});
    auto foreign = makeRecordingStudyIo();
    auto file = io->openInputTableXmlFile(path);
    { mst::Stream stream(1252, file.get()); stream << "x\n"; file->close(); }
    CHECK(file->error() == 0);
    CHECK(unsupported([&] { foreign->observeInputTableXmlWriterReturn(*file, path, true, false); }, "actual bound"));
    CHECK(unsupported([&] { io->observeInputTableXmlWriterReturn(*file, qt::L("wrong.xml"), true, false); }, "path/ordinal"));
    CHECK(unsupported([&] { io->observeInputTableXmlWriterReturn(*file, path, false, false); }, "contradicts"));
    io->setLastError(qt::L("snapshot value"));
    const auto ordinal = io->observeInputTableXmlWriterReturn(*file, path, true, false);
    CHECK(ordinal == 0);
    CHECK(unsupported([&] { io->observeInputTableXmlWriterReturn(*file, path, true, false); }, "already observed"));
    CHECK(unsupported([&] { io->observeInputTableRefreshBoundary(99, path); }, "ordinal"));
    CHECK(unsupported([&] { io->observeInputTableRefreshBoundary(ordinal, qt::L("wrong.xml")); }, "exact"));
    io->setLastError(qt::L("later global error"));
    file.reset(); // Normal-return binding is no longer needed; no extra event is invented.
    io->observeInputTableRefreshBoundary(ordinal, path);
    const auto journal = io->xmlJournalOwner();
    CHECK(journal->attempts()[0].normalWriterReturn->stickyLastError == qt::L("snapshot value"));
    CHECK(journal->attempts()[0].refreshBoundaryReached);
    CHECK(unsupported([&] { io->observeInputTableRefreshBoundary(ordinal, path); }, "exact"));
    CHECK(!io->observedSessionNotificationState()); // No lazy messenger/delivery from this marker.
}

TEST_CASE(type2_xml_positional_fault_expectations_refuse_before_open_and_do_not_consume_mismatch)
{
    const auto path = qt::L("C:/PROBLEM/bound_fault.xml");
    RecordingXmlFault fault;
    fault.expectedPath = path;
    fault.expectedAttemptOrdinal = 0;
    fault.opened = false;
    auto io = makeRecordingStudyIo({fault});
    io->writeTextFile(path, "old");
    CHECK(unsupported([&] { (void)io->openInputTableXmlFile(qt::L("C:/PROBLEM/wrong.xml")); }, "before open"));
    CHECK(io->xmlWrites().empty() && io->xmlJournalOwner()->events().empty());
    auto file = io->openInputTableXmlFile(path);
    CHECK(!file->opened()); // Bound fault survived the mismatched path request.
    CHECK(io->xmlJournalOwner()->recordedFile(path) == "old");
    RecordingXmlFault wrongOrdinal;
    wrongOrdinal.expectedAttemptOrdinal = 5;
    auto other = makeRecordingStudyIo({wrongOrdinal});
    CHECK(unsupported([&] { (void)other->openInputTableXmlFile(path); }, "ordinal mismatch"));
    CHECK(other->xmlWrites().empty() && !other->xmlJournalOwner()->recordedFile(path));
}

int main() { return testkit::run_all(); }
