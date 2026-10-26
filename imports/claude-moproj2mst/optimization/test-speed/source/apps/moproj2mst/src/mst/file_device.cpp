#include "m2m/mst/file_device.hpp"
#include "m2m/longpath.hpp"
#include <algorithm>
#include <cstring>
#include <deque>
#include <windows.h>

namespace m2m::mst {

std::int64_t writeTextMode(OutputDevice& device, std::string_view bytes)
{
    if (!device.opened()) return -1;
    std::int64_t consumed = 0;
    std::size_t start = 0;
    for (;;) {
        const auto lf = bytes.find('\n', start);
        const auto end = lf == std::string_view::npos ? bytes.size() : lf;
        if (end > start) {
            const auto wrote = device.write(bytes.substr(start, end - start));
            if (wrote <= 0) return consumed ? consumed : wrote;
            consumed += wrote;
        }
        if (lf == std::string_view::npos) return consumed;
        const auto wrote = device.write("\r\n");
        if (wrote <= 0) return consumed ? consumed : wrote;
        ++consumed; // even a positive short CRLF counts as one source LF
        start = lf + 1;
    }
}

struct FileDevice::Impl {
    static constexpr std::size_t block = 0x4000;
    HANDLE handle = INVALID_HANDLE_VALUE;
    std::deque<std::string> buffers;
    std::size_t head = 0, tail = 0, pending = 0;
    int fileError = 0, engineError = 0;

    int writeError() const { return engineError == 8 ? 2 : engineError; }

    // QFSFileEnginePrivate::nativeWrite, 0x670f8690: retry successful
    // partial writes, stop on zero or failure, and preserve positive progress.
    std::int64_t nativeWrite(std::string_view bytes)
    {
        std::size_t done = 0;
        DWORD remaining = static_cast<DWORD>(bytes.size()); // original low32 count
        while (done < bytes.size()) {
            DWORD count = std::min<DWORD>(remaining, 0x02000000), wrote = 0;
            if (!WriteFile(handle, bytes.data() + done, count, &wrote, nullptr)) {
                if (done) return static_cast<std::int64_t>(done);
                engineError = 2;                         // nativeWrite sets WriteError only here
                return -1;
            }
            if (!wrote) break;
            done += wrote;
            remaining -= wrote;
        }
        return static_cast<std::int64_t>(done);
    }

    // QRingBuffer::reserve, 0x670b5340. Storage length (rather than STL
    // capacity) controls the original half-full grow/new-segment branch.
    void append(std::string_view bytes)
    {
        if (!pending) {
            buffers.clear();
            buffers.emplace_back((std::max)(block, bytes.size()), '\0');
            head = tail = 0;
        } else if (tail + bytes.size() > buffers.back().size()) {
            if (tail < buffers.back().size() / 2) {
                buffers.back().resize(tail + bytes.size());
            } else {
                buffers.back().resize(tail);
                buffers.emplace_back((std::max)(block, bytes.size()), '\0');
                tail = 0;
            }
        }
        std::memcpy(buffers.back().data() + tail, bytes.data(), bytes.size());
        tail += bytes.size();
        pending += bytes.size();
    }

    // 0x670b5d80 writes one contiguous segment, not the whole ring. A
    // positive prefix is consumed even when QFile::flush will return false.
    bool flush()
    {
        if (handle == INVALID_HANDLE_VALUE) return false;
        if (!pending) return true; // native QFS flush 0x670f7ee0 is a no-op
        const std::size_t before = pending;
        const std::size_t first = (buffers.size() == 1 ? tail : buffers.front().size()) - head;
        const auto wrote = nativeWrite(std::string_view(buffers.front().data() + head, first));
        if (wrote > 0) {
            const auto count = static_cast<std::size_t>(wrote);
            pending -= count;
            head += count;
            if (count == first) {
                if (buffers.size() > 1) {
                    buffers.pop_front();
                    head = 0;
                } else {
                    buffers.clear();
                    head = tail = 0;
                }
            }
        }
        if (wrote < 0 || static_cast<std::size_t>(wrote) != before) {
            fileError = writeError();                    // QFile::flush0x670b5e67..5e86
            return false;
        }
        return true;
    }
};

FileDevice::FileDevice(const qt::String& path) : impl_(std::make_unique<Impl>())
{
    // QFile mode2 -> QFS WriteOnly|Unbuffered|Truncate. nativeOpen uses
    // OPEN_ALWAYS, then setSize(0), whose result is ignored (0x670f8150).
    SECURITY_ATTRIBUTES security{sizeof(SECURITY_ATTRIBUTES), nullptr, FALSE};
    impl_->handle = CreateFileW(win32Path(path.u16()).c_str(), GENERIC_WRITE,
                               FILE_SHARE_READ | FILE_SHARE_WRITE, &security,
                               OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (opened()) (void)SetEndOfFile(impl_->handle); // fresh native handle is at offset0
    else impl_->fileError = impl_->engineError = 5; // nativeOpen/QFile::open: OpenError
}

FileDevice::~FileDevice() { close(); }

bool FileDevice::opened() const { return impl_->handle != INVALID_HANDLE_VALUE; }
int FileDevice::error() const { return impl_->fileError; }

std::int64_t FileDevice::write(std::string_view bytes)
{
    if (!opened()) return -1;
    if (bytes.empty()) return 0;
    impl_->fileError = 0;                                 // writeData0x670b6039, not engine error
    // QFile::writeData 0x670b6010. Its own mode2 retains buffering even
    // though QFile::open passes Unbuffered to the underlying file engine.
    if (impl_->pending + bytes.size() > Impl::block && !impl_->flush()) return -1;
    if (bytes.size() > Impl::block) {
        const auto wrote = impl_->nativeWrite(bytes);
        if (wrote < 0) impl_->fileError = impl_->writeError();
        return wrote;
    }
    impl_->append(bytes);
    return static_cast<std::int64_t>(bytes.size());
}

bool FileDevice::flush() { return impl_->flush(); }

void FileDevice::close()
{
    if (!opened()) return;
    // QFile::close 0x670b5f30 retries the remaining QFile buffer once.
    // QIODevice::close emits aboutToClose while still open (0x670ba491):
    // the attached QTextStream can flush pending text before buffer discard.
    const bool flushed = impl_->flush();                  // saved BEFORE aboutToClose
    notifyAboutToClose();
    impl_->buffers.clear();
    impl_->head = impl_->tail = impl_->pending = 0;
    const bool closed = CloseHandle(impl_->handle) != FALSE;
    if (!closed) impl_->engineError = 8;                  // nativeClose: UnspecifiedError
    if (flushed) impl_->fileError = closed ? 0 : impl_->engineError;
    impl_->handle = INVALID_HANDLE_VALUE;
}

} // namespace m2m::mst
