#include "m2m/doe_io.hpp"

#include <filesystem>
#include <fstream>
#include <functional>
#include <iterator>
#include <map>
#include <set>
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

class NativeStudyIo final : public StudyIo {
public:
    NativeStudyIo() : StudyIo(IoMode::Native) {}
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
    RecordingStudyIo() : StudyIo(IoMode::Recording) {}
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
        writes_[qt::cleanPath(path)] = bytes;
        return recordTextWrite(path, bytes, true, true, true); // explicitly simulated
    }
    bool readFile(const qt::String& path, std::string& bytes) override
    {
        const auto found = writes_.find(qt::cleanPath(path));
        if (found != writes_.end()) {
            bytes = found->second;
            return recordRead(path, bytes, true, true); // simulated earlier write
        }
        bool succeeded = false;
        const bool opened = readRawFile(path, bytes, succeeded);
        return recordRead(path, bytes, opened, succeeded);
    }
    std::unique_ptr<StudyReadFile> openReadFile(const qt::String& path) override
    {
        const auto found = writes_.find(qt::cleanPath(path));
        if (found == writes_.end()) return openNativeReadFile(path);
        const auto index = recordReadOpen(path, true);
        return std::make_unique<MemoryReadFile>(found->second,
            [this, index](auto size) { recordReadSize(index, size); },
            [this, index](const auto& bytes, bool ok) { recordReadData(index, bytes, ok); });
    }

private:
    std::set<qt::String> simulated_;
    std::map<qt::String, std::string> writes_;
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

}  // namespace m2m::doe
