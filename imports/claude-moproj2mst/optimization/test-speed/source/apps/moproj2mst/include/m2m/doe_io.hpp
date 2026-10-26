#pragma once

#include <memory>
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

}  // namespace m2m::doe
