#pragma once

#include <memory>
#include <map>
#include <cstdint>
#include <optional>
#include <functional>
#include <string>
#include <vector>

#include "m2m/qt4.hpp"

namespace m2m::engine { class ModelManager; }

namespace m2m::doe {
struct StateVarRuntime;
struct SliceConstructorRuntime;
struct SessionNotificationState;
class TaskAllocationStore;
class TaskFactoryRuntime;
class StudyXmlFile;

enum class IoMode { Native, Recording };

struct DirectoryAttempt {
    qt::String path;
    bool existed = false;
    bool attempted = false;
    bool success = false;
};

struct TextWriteAttempt {
    qt::String path;
    std::string requestedBytes; // already encoded, including Windows Text-mode CRLF
    bool opened = false;
    bool writeSucceeded = false;
    bool streamGoodAfterClose = false; // includes any earlier sticky stream error
};

enum class XmlWritePhase { Open, Write, Flush, Closing, Closed, ErrorAfterClose };
struct XmlRecordingRawSpan {
    std::string requestedBytes; // Actual bytes below Text translation.
    std::optional<std::int64_t> returned;
    std::string acceptedRecordingBytes; // Observed memory-file mutation, never native durability.
};
struct XmlNormalWriterReturn {
    bool writerSucceeded = false;
    bool streamFailed = false;
    qt::String stickyLastError;
};
struct XmlWriteAttempt {
    qt::String path;
    bool opened = false;
    bool openResultObserved = false; // OpenRequested alone does not establish failure.
    std::string requestedStreamBytes; // Locale bytes before QIODevice Text translation; not durability.
    std::vector<XmlWritePhase> phases;
    bool closed = false;
    std::optional<int> errorAfterClose;
    std::vector<std::int64_t> acceptedStreamBytes;
    std::vector<bool> flushResults;
    bool recordingStoreTracked = false;
    std::string recordingProfile;
    std::vector<XmlRecordingRawSpan> recordingRawSpans;
    std::optional<std::string> recordedBytesAfterClose;
    std::optional<XmlNormalWriterReturn> normalWriterReturn;
    bool refreshBoundaryReached = false;
};

enum class XmlJournalEventKind {
    OpenRequested, OpenResult, RecordingTruncated,
    StreamWriteRequested, StreamWriteReturned, RecordingRawWriteRequested,
    RecordingRawWriteReturned, Flush, Closing, Closed, ErrorAfterClose,
    NormalWriterReturn, RefreshBoundaryReached
};
struct XmlJournalEvent {
    std::size_t attempt = 0;
    XmlJournalEventKind kind = XmlJournalEventKind::OpenRequested;
    std::optional<std::size_t> rawSpan;
};
class StudyXmlJournal final {
public:
    const std::vector<XmlWriteAttempt>& attempts() const noexcept { return attempts_; }
    const std::vector<XmlJournalEvent>& events() const noexcept { return events_; }
    // Same canonical recording store used by PAR/XML writes and later reads.
    // Missing optional means unobserved; empty string is a known empty file.
    std::optional<std::string> recordedFile(const qt::String& path) const;
private:
    friend class StudyIo;
    std::vector<XmlWriteAttempt> attempts_;
    std::vector<XmlJournalEvent> events_;
    std::map<qt::String, std::string> recordedFiles_;
    std::map<const StudyXmlFile*, std::size_t> activeDevices_; // Nonowning; erased on device destruction.
};

// Explicit per-instance recording observations. No native/OS failure parity is
// inferred. Positive raw short writes append exactly that span below Text.
struct RecordingXmlFault {
    bool opened = true;
    std::vector<std::int64_t> rawWriteReturns;
    std::vector<bool> flushReturns;
    int errorAfterClose = 0;
    std::string label = "default successful recording simulation";
    std::optional<qt::String> expectedPath; // Optional fixture assertion, normalized like the store.
    std::optional<std::size_t> expectedAttemptOrdinal;
};

struct ReadAttempt {
    qt::String path;
    std::string bytes; // observed raw bytes, including any partial read
    bool opened = false;
    bool readSucceeded = false;
    bool readAttempted = false;
    bool sizeObserved = false;
    std::optional<std::uint64_t> sizeBeforeRead;
};

class StudyReadFile {
public:
    virtual ~StudyReadFile() = default;
    virtual std::optional<std::uint64_t> size() = 0;
    virtual std::string readAll() = 0;
};

// Constructor-time DOE directory effects. Native is configured only by the file-writing
// Save adapter. Recording is an explicit simulation for read-only/convertToBytes callers;
// its successful missing-directory result is not evidence of real I/O parity.
class StudyIo {
public:
    explicit StudyIo(IoMode mode) : mode_(mode) {}
    virtual ~StudyIo() = default;
    virtual bool ensureDirectory(const qt::String& path) = 0;
    // Returns open success, not write/close success (0x140915360). No mkdir,
    // temporary replacement or rollback. Directory-only custom sinks must opt in.
    virtual bool writeTextFile(const qt::String& path, const std::string& bytes);
    // Distinct XML QFile contract: WriteOnly|Text, immediate truncation, and
    // post-close QFile.error rather than the PAR writer's open-only Boolean.
    // Caller must keep this StudyIo alive until the returned device dies.
    virtual std::unique_ptr<StudyXmlFile> openInputTableXmlFile(const qt::String& path);
    const std::vector<XmlWriteAttempt>& xmlWrites() const { return xmlJournal_->attempts(); }
    std::shared_ptr<const StudyXmlJournal> xmlJournalOwner() const noexcept { return xmlJournal_; }
    std::size_t observeInputTableXmlWriterReturn(const StudyXmlFile& actualFile,
        const qt::String& path, bool writerSucceeded, bool streamFailed);
    // Root calls this only after the real XML helper returned. No messenger,
    // receiver, timer or UI delivery is represented by this boundary marker.
    void observeInputTableRefreshBoundary(std::size_t attemptOrdinal, const qt::String& path);
    // Raw bytes for ReadOnly|Text consumers, which apply their own decoding. Return
    // open success; later read errors retain partial bytes for the original parser.
    virtual bool readFile(const qt::String& path, std::string& bytes);
    // Separate open/size/read permits the .DS caller to discard old DOM before
    // a nonempty file read, and to skip reading a genuinely zero-size file.
    virtual std::unique_ptr<StudyReadFile> openReadFile(const qt::String& path);
    IoMode mode() const { return mode_; }
    const std::vector<DirectoryAttempt>& directories() const { return directories_; }
    const std::vector<TextWriteAttempt>& textWrites() const { return textWrites_; }
    const std::vector<ReadAttempt>& reads() const { return reads_; }
    // Original Qt diagnostic observations, separate from converter ABI warnings
    // and STRICT handling. This does not emulate Qt's installed message handler.
    const std::vector<std::string>& qtWarnings() const { return qtWarnings_; }
    void setQtWarningSink(std::function<void(const std::string&)> sink)
    { qtWarningSink_ = std::move(sink); }
    void noteQtWarning(const std::string& text)
    {
        qtWarnings_.push_back(text);
        if (qtWarningSink_) qtWarningSink_(text);
    }
    // Original global143dd9a30 within this isolated per-conversion DOE lifecycle.
    // Other global consumers are not recovered. Success does not clear prior error.
    const qt::String& lastError() const { return lastError_; }
    void setLastError(const qt::String& value) { lastError_ = value; }
    StateVarRuntime& stateVarRuntime();
    //Bind before first observation; never silently discard an existing owner.
    void bindSessionNotificationState(std::shared_ptr<SessionNotificationState> owner);
    SessionNotificationState& sessionNotificationState();
    const SessionNotificationState* observedSessionNotificationState() const
    { return sessionNotificationState_.get(); }
    const StateVarRuntime* observedStateVarRuntime() const { return stateVarRuntime_.get(); }
    // The actual session/main-window manager, shared with other consumers.
    // Null means unavailable; never create an isolated manager or copy its ID.
    void bindModelManager(std::shared_ptr<engine::ModelManager> owner);
    engine::ModelManager* modelManager() const noexcept { return modelManager_.get(); }
    // Borrow the actual fresh-process context even for models without a DB.
    // Expired contexts are unavailable; this sink cannot resurrect a session.
    void bindSliceConstructorRuntime(const std::shared_ptr<SliceConstructorRuntime>& owner);
    std::shared_ptr<SliceConstructorRuntime> sliceConstructorRuntimeOwner() const noexcept
    { return sliceConstructorRuntime_.lock(); }
    // Nonowning access to the actual fresh-process allocation retention store.
    // This is not evidence that the native 17-prototype factory initialized.
    void bindTaskAllocationStore(const std::shared_ptr<TaskAllocationStore>& owner);
    std::shared_ptr<TaskAllocationStore> taskAllocationStoreOwner() const noexcept
    { return taskAllocationStore_.lock(); }
    void bindTaskFactoryRuntime(const std::shared_ptr<TaskFactoryRuntime>& owner);
    std::shared_ptr<TaskFactoryRuntime> taskFactoryRuntimeOwner() const noexcept
    { return taskFactoryRuntime_.lock(); }

protected:
    std::unique_ptr<StudyXmlFile> observeInputTableXmlFile(
        const qt::String& path, std::unique_ptr<StudyXmlFile> file,
        std::optional<std::size_t> startedAttempt = {});
    std::size_t beginInputTableXmlAttempt(const qt::String& path);
    std::unique_ptr<StudyXmlFile> openRecordingInputTableXmlFile(
        const qt::String& path, const RecordingXmlFault& fault);
    void storeRecordingFile(const qt::String& path, const std::string& bytes);
    bool recordDirectory(const qt::String& path, bool existed, bool attempted, bool success)
    {
        directories_.push_back({path, existed, attempted, success});
        return success;
    }
    bool recordTextWrite(const qt::String& path, const std::string& bytes, bool opened,
                         bool writeSucceeded, bool streamGoodAfterClose)
    {
        textWrites_.push_back({path, bytes, opened, writeSucceeded, streamGoodAfterClose});
        return opened;
    }
    bool recordRead(const qt::String& path, const std::string& bytes, bool opened, bool succeeded)
    {
        reads_.push_back({path, bytes, opened, succeeded, opened, false, std::nullopt});
        return opened;
    }
    std::size_t recordReadOpen(const qt::String& path, bool opened)
    {
        reads_.push_back({path, {}, opened, false, false, false, std::nullopt});
        return reads_.size() - 1;
    }
    void recordReadSize(std::size_t index, std::optional<std::uint64_t> size)
    {
        reads_.at(index).sizeObserved = true;
        reads_.at(index).sizeBeforeRead = size;
    }
    void recordReadData(std::size_t index, const std::string& bytes, bool succeeded)
    {
        reads_.at(index).bytes = bytes;
        reads_.at(index).readSucceeded = succeeded;
        reads_.at(index).readAttempted = true;
    }
    std::unique_ptr<StudyReadFile> openNativeReadFile(const qt::String& path);

private:
    IoMode mode_;
    std::vector<DirectoryAttempt> directories_;
    std::vector<TextWriteAttempt> textWrites_;
    std::shared_ptr<StudyXmlJournal> xmlJournal_ = std::make_shared<StudyXmlJournal>();
    std::vector<ReadAttempt> reads_;
    std::vector<std::string> qtWarnings_;
    std::function<void(const std::string&)> qtWarningSink_;
    qt::String lastError_;
    std::shared_ptr<StateVarRuntime> stateVarRuntime_;
    std::shared_ptr<SessionNotificationState> sessionNotificationState_;
    std::shared_ptr<engine::ModelManager> modelManager_;
    std::weak_ptr<SliceConstructorRuntime> sliceConstructorRuntime_;
    bool sliceConstructorRuntimeWasBound_ = false;
    std::weak_ptr<TaskAllocationStore> taskAllocationStore_;
    std::weak_ptr<TaskFactoryRuntime> taskFactoryRuntime_;
    bool taskFactoryRuntimeWasBound_ = false;
    bool taskAllocationStoreWasBound_ = false;
};

std::shared_ptr<StudyIo> makeNativeStudyIo();
std::shared_ptr<StudyIo> makeRecordingStudyIo();
std::shared_ptr<StudyIo> makeRecordingStudyIo(std::vector<RecordingXmlFault> faults);

}  // namespace m2m::doe
