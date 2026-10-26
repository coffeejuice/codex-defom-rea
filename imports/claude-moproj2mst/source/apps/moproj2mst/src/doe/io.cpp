#include "m2m/doe_io.hpp"
#include "m2m/doe_xml_output_device.hpp"

#include <filesystem>
#include <fstream>
#include <functional>
#include <iterator>
#include <map>
#include <set>
#include <utility>
#include <windows.h>

#include "m2m/common.hpp"
#include "m2m/engine/model_manager.hpp"
#include "m2m/longpath.hpp"
#include "m2m/qt4path.hpp"

namespace m2m::doe {
namespace {

std::filesystem::path osPath(const qt::String& path)
{
    return std::filesystem::path(win32Path(path.u16()));
}

bool directoryExists(const qt::String& path)
{
    std::error_code ec;
    return std::filesystem::is_directory(osPath(path), ec);
}

bool readRawFile(const qt::String& path, std::string& bytes, bool& succeeded)
{
    bytes.clear();
    std::ifstream in(osPath(path), std::ios::binary);
    if (!in.is_open()) { succeeded = false; return false; }
    bytes.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
    succeeded = !in.bad();
    return true;
}

using SizeObserver = std::function<void(std::optional<std::uint64_t>)>;
using ReadObserver = std::function<void(const std::string&, bool)>;

class NativeReadFile final : public StudyReadFile {
public:
    NativeReadFile(HANDLE handle, SizeObserver sizeObserver, ReadObserver readObserver)
        : handle_(handle), observeSize_(std::move(sizeObserver)), observeRead_(std::move(readObserver)) {}
    ~NativeReadFile() override { CloseHandle(handle_); }
    std::optional<std::uint64_t> size() override
    {
        LARGE_INTEGER length{};
        std::optional<std::uint64_t> result;
        if (GetFileSizeEx(handle_, &length) && length.QuadPart >= 0)
            result = static_cast<std::uint64_t>(length.QuadPart);
        observeSize_(result);
        return result;
    }
    std::string readAll() override
    {
        std::string bytes;
        bool succeeded = true;
        char buffer[65536];
        for (;;) {
            DWORD read = 0;
            if (!ReadFile(handle_, buffer, sizeof(buffer), &read, nullptr)) {
                succeeded = false;
                break;
            }
            if (read == 0) break;
            bytes.append(buffer, read);
        }
        observeRead_(bytes, succeeded);
        return bytes;
    }
private:
    HANDLE handle_;
    SizeObserver observeSize_;
    ReadObserver observeRead_;
};

class MemoryReadFile final : public StudyReadFile {
public:
    MemoryReadFile(std::string bytes, SizeObserver sizeObserver, ReadObserver readObserver)
        : bytes_(std::move(bytes)), observeSize_(std::move(sizeObserver)), observeRead_(std::move(readObserver)) {}
    std::optional<std::uint64_t> size() override
    {
        const auto length = static_cast<std::uint64_t>(bytes_.size());
        observeSize_(length);
        return length;
    }
    std::string readAll() override
    {
        const std::string result = read_ ? "" : bytes_;
        read_ = true;
        observeRead_(result, true);
        return result;
    }
private:
    std::string bytes_;
    bool read_ = false;
    SizeObserver observeSize_;
    ReadObserver observeRead_;
};

class JournalObservedFile final : public StudyXmlFile {
public:
    JournalObservedFile(std::unique_ptr<StudyXmlFile> file,
        std::function<void(std::string_view)> request, StudyXmlObserver observer,
        std::function<void(const StudyXmlFile*)> release)
        : file_(std::move(file)), request_(std::move(request)), observer_(std::move(observer)),
          release_(std::move(release))
    { file_->setAboutToClose([this] { notifyAboutToClose(); }); }
    ~JournalObservedFile() override { close(); file_->setAboutToClose({}); release_(this); }
    std::int64_t write(std::string_view bytes) override
    {
        request_(bytes);
        const auto result = file_->write(bytes);
        observer_.write(bytes, result);
        return result;
    }
    bool flush() override
    { const bool result = file_->flush(); observer_.flush(result); return result; }
    void close() override
    {
        if (!opened()) return;
        observer_.closing();
        file_->close();
        observer_.closed();
    }
    bool opened() const override { return file_->opened(); }
    int error() const override
    { const int result = file_->error(); observer_.error(result); return result; }
private:
    std::unique_ptr<StudyXmlFile> file_;
    std::function<void(std::string_view)> request_;
    StudyXmlObserver observer_;
    std::function<void(const StudyXmlFile*)> release_;
};

class RecordingFaultBinary final : public StudyXmlFile {
public:
    using Request = std::function<std::size_t(std::string_view)>;
    using Returned = std::function<void(std::size_t, std::string_view, std::int64_t)>;
    RecordingFaultBinary(RecordingXmlFault fault, Request request, Returned returned)
        : fault_(std::move(fault)), request_(std::move(request)), returned_(std::move(returned)),
          open_(fault_.opened) {}
    ~RecordingFaultBinary() override { close(); }
    std::int64_t write(std::string_view bytes) override
    {
        const auto ticket = request_(bytes);
        const auto result = !open_ ? -1 : (write_ < fault_.rawWriteReturns.size()
            ? fault_.rawWriteReturns[write_++] : static_cast<std::int64_t>(bytes.size()));
        if (result < -1 || result > static_cast<std::int64_t>(bytes.size()))
            throw Error(M2M_E_UNSUPPORTED, "DOE recording raw-write fault result is outside the requested span");
        returned_(ticket, bytes, result);
        return result;
    }
    bool flush() override
    { return open_ && (flush_ < fault_.flushReturns.size() ? fault_.flushReturns[flush_++] : true); }
    void close() override { if (open_) { notifyAboutToClose(); open_ = false; } }
    bool opened() const override { return open_; }
    int error() const override { return fault_.errorAfterClose; }
private:
    RecordingXmlFault fault_;
    Request request_;
    Returned returned_;
    bool open_;
    std::size_t write_ = 0, flush_ = 0;
};

class NativeStudyIo final : public StudyIo {
public:
    NativeStudyIo() : StudyIo(IoMode::Native) {}
    std::unique_ptr<StudyXmlFile> openInputTableXmlFile(const qt::String& path) override
    {
        const auto attempt = beginInputTableXmlAttempt(path);
        return observeInputTableXmlFile(path, nativeStudyXmlFile(path), attempt);
    }
    bool ensureDirectory(const qt::String& path) override
    {
        // 0x140909660: QDir::exists, then mkpath only when absent. No rollback;
        // create_directories can leave earlier parents when a later component fails.
        if (directoryExists(path)) return recordDirectory(path, true, false, true);
        std::error_code ec;
        std::filesystem::create_directories(osPath(path), ec);
        // No extra existence probe after mkpath: a later external removal must not
        // retroactively turn a successful creation into a reported failure.
        return recordDirectory(path, false, true, !ec);
    }
    bool writeTextFile(const qt::String& path, const std::string& bytes) override
    {
        // The caller supplies Text-mode bytes. Binary mode avoids a second CRLF
        // conversion. Opening truncates in place, so later failure keeps its effects.
        std::ofstream out(osPath(path), std::ios::binary | std::ios::trunc);
        if (!out.is_open()) return recordTextWrite(path, bytes, false, false, false);
        out.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
        out.flush();                                       // QTextStream::endl boundary
        const bool written = out.good();
        out.close();
        return recordTextWrite(path, bytes, true, written, !out.fail());
    }
    bool readFile(const qt::String& path, std::string& bytes) override
    {
        bool succeeded = false;
        const bool opened = readRawFile(path, bytes, succeeded);
        return recordRead(path, bytes, opened, succeeded);
    }
    std::unique_ptr<StudyReadFile> openReadFile(const qt::String& path) override
    { return openNativeReadFile(path); }
};

class RecordingStudyIo final : public StudyIo {
public:
    explicit RecordingStudyIo(std::vector<RecordingXmlFault> faults = {})
        : StudyIo(IoMode::Recording), faults_(std::move(faults)) {}
    std::unique_ptr<StudyXmlFile> openInputTableXmlFile(const qt::String& path) override
    {
        const auto fault = fault_ < faults_.size() ? faults_[fault_] : RecordingXmlFault{};
        auto file = openRecordingInputTableXmlFile(path, fault);
        ++fault_;
        return file;
    }
    bool ensureDirectory(const qt::String& path) override
    {
        const auto key = qt::cleanPath(path);
        const bool existed = simulated_.find(key) != simulated_.end() || directoryExists(path);
        simulated_.insert(key);
        return recordDirectory(path, existed, !existed, true); // simulated, never a native result
    }
    bool writeTextFile(const qt::String& path, const std::string& bytes) override
    {
        // Preserve the caller's spelling in the observation, but recognize the
        // separator and dot-component aliases that refer to one simulated file.
        storeRecordingFile(path, bytes);
        return recordTextWrite(path, bytes, true, true, true); // explicitly simulated
    }
    bool readFile(const qt::String& path, std::string& bytes) override
    {
        const auto found = xmlJournalOwner()->recordedFile(path);
        if (found) {
            bytes = *found;
            return recordRead(path, bytes, true, true); // simulated earlier write
        }
        bool succeeded = false;
        const bool opened = readRawFile(path, bytes, succeeded);
        return recordRead(path, bytes, opened, succeeded);
    }
    std::unique_ptr<StudyReadFile> openReadFile(const qt::String& path) override
    {
        const auto found = xmlJournalOwner()->recordedFile(path);
        if (!found) return openNativeReadFile(path);
        const auto index = recordReadOpen(path, true);
        return std::make_unique<MemoryReadFile>(*found,
            [this, index](auto size) { recordReadSize(index, size); },
            [this, index](const auto& bytes, bool ok) { recordReadData(index, bytes, ok); });
    }

private:
    std::set<qt::String> simulated_;
    std::vector<RecordingXmlFault> faults_;
    std::size_t fault_ = 0;
};

}  // namespace

void StudyIo::bindSliceConstructorRuntime(const std::shared_ptr<SliceConstructorRuntime>& owner)
{
    if (!owner || (sliceConstructorRuntimeWasBound_ &&
        (sliceConstructorRuntime_.lock() != owner || sliceConstructorRuntime_.owner_before(owner) ||
         owner.owner_before(sliceConstructorRuntime_))))
        throw Error(M2M_E_UNSUPPORTED, "DOE Slice constructor runtime requires one actual process owner without replacement");
    sliceConstructorRuntime_ = owner;
    sliceConstructorRuntimeWasBound_ = true;
}

void StudyIo::bindTaskAllocationStore(const std::shared_ptr<TaskAllocationStore>& owner)
{
    if (!owner || (taskAllocationStoreWasBound_ &&
        (taskAllocationStore_.lock() != owner || taskAllocationStore_.owner_before(owner) ||
         owner.owner_before(taskAllocationStore_))))
        throw Error(M2M_E_UNSUPPORTED, "DOE Task allocations require one actual process owner without replacement");
    taskAllocationStore_ = owner;
    taskAllocationStoreWasBound_ = true;
}

void StudyIo::bindTaskFactoryRuntime(const std::shared_ptr<TaskFactoryRuntime>& owner)
{
    if (!owner || (taskFactoryRuntimeWasBound_ &&
        (taskFactoryRuntime_.lock() != owner || taskFactoryRuntime_.owner_before(owner) ||
         owner.owner_before(taskFactoryRuntime_))))
        throw Error(M2M_E_UNSUPPORTED, "DOE Task factory requires one actual main-window owner without replacement");
    taskFactoryRuntime_ = owner;
    taskFactoryRuntimeWasBound_ = true;
}

void StudyIo::bindModelManager(std::shared_ptr<engine::ModelManager> owner)
{
    if (!owner || (modelManager_ && (modelManager_ != owner ||
        modelManager_.owner_before(owner) || owner.owner_before(modelManager_))))
        throw Error(M2M_E_UNSUPPORTED, "DOE model manager must bind a nonnull shared owner without replacement");
    modelManager_ = std::move(owner);
}

bool StudyIo::writeTextFile(const qt::String&, const std::string&)
{
    throw Error(M2M_E_UNSUPPORTED, "DOE I/O sink has no direct text writer");
}

std::unique_ptr<StudyXmlFile> StudyIo::openInputTableXmlFile(const qt::String&)
{
    throw Error(M2M_E_UNSUPPORTED, "DOE I/O sink has no error-aware InputTable XML device");
}

std::optional<std::string> StudyXmlJournal::recordedFile(const qt::String& path) const
{
    const auto found = recordedFiles_.find(qt::cleanPath(path));
    return found == recordedFiles_.end() ? std::nullopt : std::optional<std::string>(found->second);
}

void StudyIo::storeRecordingFile(const qt::String& path, const std::string& bytes)
{
    if (mode() != IoMode::Recording)
        throw Error(M2M_E_UNSUPPORTED, "Native StudyIo cannot acquire simulated file contents");
    xmlJournal_->recordedFiles_[qt::cleanPath(path)] = bytes;
}

std::size_t StudyIo::beginInputTableXmlAttempt(const qt::String& path)
{
    const auto index = xmlJournal_->attempts_.size();
    XmlWriteAttempt attempt;
    attempt.path = path;
    xmlJournal_->attempts_.push_back(std::move(attempt));
    xmlJournal_->events_.push_back({index, XmlJournalEventKind::OpenRequested, {}});
    return index;
}

std::unique_ptr<StudyXmlFile> StudyIo::openRecordingInputTableXmlFile(
    const qt::String& path, const RecordingXmlFault& fault)
{
    if (mode() != IoMode::Recording)
        throw Error(M2M_E_UNSUPPORTED, "XML recording faults cannot be applied to Native StudyIo");
    if ((fault.expectedPath && qt::cleanPath(*fault.expectedPath) != qt::cleanPath(path)) ||
        (fault.expectedAttemptOrdinal && *fault.expectedAttemptOrdinal != xmlJournal_->attempts_.size()))
        throw Error(M2M_E_UNSUPPORTED, "DOE positional recording fault profile path/ordinal mismatch before open");
    const auto index = beginInputTableXmlAttempt(path);
    const auto journal = xmlJournal_;
    const auto key = qt::cleanPath(path);
    journal->attempts_[index].recordingStoreTracked = true;
    journal->attempts_[index].recordingProfile = fault.label;
    if (fault.opened) {
        journal->recordedFiles_[key].clear();
        journal->events_.push_back({index, XmlJournalEventKind::RecordingTruncated, {}});
    }
    auto request = [journal, index](std::string_view bytes) {
        auto& spans = journal->attempts_.at(index).recordingRawSpans;
        const auto ticket = spans.size();
        spans.push_back({std::string(bytes), {}, {}});
        journal->events_.push_back({index, XmlJournalEventKind::RecordingRawWriteRequested, ticket});
        return ticket;
    };
    auto returned = [journal, index, key](std::size_t ticket, std::string_view bytes, std::int64_t count) {
        auto& span = journal->attempts_.at(index).recordingRawSpans.at(ticket);
        if (count > 0) {
            const auto accepted = bytes.substr(0, static_cast<std::size_t>(count));
            journal->recordedFiles_[key].append(accepted);
            span.acceptedRecordingBytes = std::string(accepted);
        }
        span.returned = count;
        journal->events_.push_back({index, XmlJournalEventKind::RecordingRawWriteReturned, ticket});
    };
    auto binary = std::make_unique<RecordingFaultBinary>(fault, std::move(request), std::move(returned));
    return observeInputTableXmlFile(path, textModeStudyXmlFile(std::move(binary)), index);
}

std::unique_ptr<StudyXmlFile> StudyIo::observeInputTableXmlFile(
    const qt::String& path, std::unique_ptr<StudyXmlFile> file, std::optional<std::size_t> started)
{
    if (!file) throw Error(M2M_E_UNSUPPORTED, "DOE XML QFile allocation is unavailable");
    const auto index = started ? *started : beginInputTableXmlAttempt(path);
    const auto journal = xmlJournal_;
    if (index >= journal->attempts_.size() || journal->attempts_[index].path != path)
        throw Error(M2M_E_UNSUPPORTED, "DOE XML journal attempt belongs to another path/owner");
    auto& initial = journal->attempts_[index];
    if (initial.openResultObserved)
        throw Error(M2M_E_UNSUPPORTED, "DOE XML journal attempt already has an open/device binding");
    initial.opened = file->opened();
    initial.openResultObserved = true;
    initial.phases.push_back(XmlWritePhase::Open);
    journal->events_.push_back({index, XmlJournalEventKind::OpenResult, {}});
    auto request = [journal, index](std::string_view bytes) {
        journal->attempts_.at(index).requestedStreamBytes.append(bytes);
        journal->events_.push_back({index, XmlJournalEventKind::StreamWriteRequested, {}});
    };
    StudyXmlObserver observer;
    observer.write = [journal, index](std::string_view, std::int64_t accepted) {
        auto& attempt = journal->attempts_.at(index);
        attempt.acceptedStreamBytes.push_back(accepted);
        attempt.phases.push_back(XmlWritePhase::Write);
        journal->events_.push_back({index, XmlJournalEventKind::StreamWriteReturned, {}});
    };
    observer.flush = [journal, index](bool succeeded) {
        auto& attempt = journal->attempts_.at(index);
        attempt.flushResults.push_back(succeeded);
        attempt.phases.push_back(XmlWritePhase::Flush);
        journal->events_.push_back({index, XmlJournalEventKind::Flush, {}});
    };
    observer.closing = [journal, index] {
        journal->attempts_.at(index).phases.push_back(XmlWritePhase::Closing);
        journal->events_.push_back({index, XmlJournalEventKind::Closing, {}});
    };
    observer.closed = [journal, index] {
        auto& attempt = journal->attempts_.at(index);
        attempt.closed = true;
        attempt.phases.push_back(XmlWritePhase::Closed);
        if (attempt.recordingStoreTracked) attempt.recordedBytesAfterClose = journal->recordedFile(attempt.path);
        journal->events_.push_back({index, XmlJournalEventKind::Closed, {}});
    };
    observer.error = [journal, index](int error) {
        auto& attempt = journal->attempts_.at(index);
        if (!attempt.closed) throw Error(M2M_E_UNSUPPORTED, "DOE XML error query precedes close");
        attempt.errorAfterClose = error;
        attempt.phases.push_back(XmlWritePhase::ErrorAfterClose);
        journal->events_.push_back({index, XmlJournalEventKind::ErrorAfterClose, {}});
    };
    auto release = [journal](const StudyXmlFile* device) { journal->activeDevices_.erase(device); };
    auto observed = std::make_unique<JournalObservedFile>(
        std::move(file), std::move(request), std::move(observer), std::move(release));
    if (!journal->activeDevices_.emplace(observed.get(), index).second)
        throw Error(M2M_E_UNSUPPORTED, "DOE XML device identity is already bound");
    return observed;
}

std::size_t StudyIo::observeInputTableXmlWriterReturn(const StudyXmlFile& file,
    const qt::String& path, bool succeeded, bool streamFailed)
{
    const auto bound = xmlJournal_->activeDevices_.find(&file);
    if (bound == xmlJournal_->activeDevices_.end())
        throw Error(M2M_E_UNSUPPORTED, "DOE XML normal return requires the actual bound file device");
    const auto index = bound->second;
    auto& attempt = xmlJournal_->attempts_.at(index);
    if (attempt.path != path || !attempt.openResultObserved || attempt.normalWriterReturn)
        throw Error(M2M_E_UNSUPPORTED, "DOE XML normal return path/ordinal is unavailable or already observed");
    if ((!attempt.opened && (succeeded || streamFailed || attempt.errorAfterClose)) ||
        (attempt.opened && (!attempt.closed || !attempt.errorAfterClose ||
                           succeeded != (*attempt.errorAfterClose == 0))))
        throw Error(M2M_E_UNSUPPORTED, "DOE XML normal return contradicts reached open/close/error observations");
    XmlNormalWriterReturn observed{succeeded, streamFailed, lastError_};
    xmlJournal_->events_.reserve(xmlJournal_->events_.size() + 1);
    attempt.normalWriterReturn = std::move(observed);
    xmlJournal_->events_.push_back({index, XmlJournalEventKind::NormalWriterReturn, {}});
    return index;
}

void StudyIo::observeInputTableRefreshBoundary(std::size_t index, const qt::String& path)
{
    if (index >= xmlJournal_->attempts_.size())
        throw Error(M2M_E_UNSUPPORTED, "DOE XML refresh boundary has an unavailable attempt ordinal");
    auto& attempt = xmlJournal_->attempts_[index];
    const auto& events = xmlJournal_->events_;
    if (attempt.path != path || !attempt.normalWriterReturn || attempt.refreshBoundaryReached ||
        events.empty() || events.back().kind != XmlJournalEventKind::NormalWriterReturn ||
        events.back().attempt != index)
        throw Error(M2M_E_UNSUPPORTED, "DOE XML refresh boundary requires the latest exact normal return/path/ordinal");
    xmlJournal_->events_.reserve(xmlJournal_->events_.size() + 1);
    attempt.refreshBoundaryReached = true;
    xmlJournal_->events_.push_back({index, XmlJournalEventKind::RefreshBoundaryReached, {}});
}

bool StudyIo::readFile(const qt::String&, std::string&)
{
    throw Error(M2M_E_UNSUPPORTED, "DOE I/O sink has no raw reader");
}

std::unique_ptr<StudyReadFile> StudyIo::openReadFile(const qt::String&)
{
    throw Error(M2M_E_UNSUPPORTED, "DOE I/O sink has no sized read handle");
}

std::unique_ptr<StudyReadFile> StudyIo::openNativeReadFile(const qt::String& path)
{
    // Shipped Qt nativeOpen670f8150 with ReadOnly1 (+engine Unbuffered20):
    // GENERIC_READ, share3, OPEN_EXISTING3, normal80, noninheritable security24.
    // The handle also binds GetFileSizeEx to the opened file, not a later path stat.
    SECURITY_ATTRIBUTES security{sizeof(SECURITY_ATTRIBUTES), nullptr, FALSE};
    HANDLE handle = CreateFileW(win32Path(path.u16()).c_str(), GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE, &security, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    const auto index = recordReadOpen(path, handle != INVALID_HANDLE_VALUE);
    if (handle == INVALID_HANDLE_VALUE) return {};
    return std::make_unique<NativeReadFile>(handle,
        [this, index](auto size) { recordReadSize(index, size); },
        [this, index](const auto& bytes, bool ok) { recordReadData(index, bytes, ok); });
}

std::shared_ptr<StudyIo> makeNativeStudyIo() { return std::make_shared<NativeStudyIo>(); }
std::shared_ptr<StudyIo> makeRecordingStudyIo() { return std::make_shared<RecordingStudyIo>(); }
std::shared_ptr<StudyIo> makeRecordingStudyIo(std::vector<RecordingXmlFault> faults)
{ return std::make_shared<RecordingStudyIo>(std::move(faults)); }

}  // namespace m2m::doe
