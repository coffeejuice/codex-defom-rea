#include "deformdb/file.hpp"

#include <stdexcept>
#include <string>

#ifdef _WIN32
#  ifndef NOMINMAX
#    define NOMINMAX
#  endif
#  include <windows.h>
#else
#  include <fcntl.h>
#  include <sys/stat.h>
#  include <unistd.h>
#endif

namespace deformdb {

#ifdef _WIN32

struct RandomAccessFile::Impl {
    HANDLE h = INVALID_HANDLE_VALUE;
};

RandomAccessFile::RandomAccessFile(const std::filesystem::path& path) : impl_(std::make_unique<Impl>()) {
    // Share read and write: a running solver may still be appending to the DB.
    impl_->h = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                           nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, nullptr);
    if (impl_->h == INVALID_HANDLE_VALUE)
        throw std::runtime_error("cannot open " + path.string() + " (error " + std::to_string(GetLastError()) + ")");
    LARGE_INTEGER sz{};
    GetFileSizeEx(impl_->h, &sz);
    size_ = static_cast<std::uint64_t>(sz.QuadPart);
}

RandomAccessFile::~RandomAccessFile() {
    if (impl_ && impl_->h != INVALID_HANDLE_VALUE) CloseHandle(impl_->h);
}

void RandomAccessFile::read_at(std::uint64_t offset, void* dst, std::size_t bytes) const {
    auto* out = static_cast<char*>(dst);
    while (bytes > 0) {
        const DWORD chunk = static_cast<DWORD>(bytes > (1u << 30) ? (1u << 30) : bytes);
        OVERLAPPED ov{};
        ov.Offset = static_cast<DWORD>(offset & 0xffffffffu);
        ov.OffsetHigh = static_cast<DWORD>(offset >> 32);
        DWORD got = 0;
        if (!ReadFile(impl_->h, out, chunk, &got, &ov) || got == 0)
            throw std::runtime_error("read failed at offset " + std::to_string(offset));
        out += got;
        offset += got;
        bytes -= got;
    }
}

struct WritableFile::Impl {
    HANDLE h = INVALID_HANDLE_VALUE;
};

WritableFile::WritableFile(const std::filesystem::path& path) : impl_(std::make_unique<Impl>()) {
    impl_->h = CreateFileW(path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS,
                           FILE_ATTRIBUTE_NORMAL, nullptr);
    if (impl_->h == INVALID_HANDLE_VALUE)
        throw std::runtime_error("cannot create " + path.string() + " (error " + std::to_string(GetLastError()) + ")");
}

WritableFile::~WritableFile() {
    if (impl_ && impl_->h != INVALID_HANDLE_VALUE) CloseHandle(impl_->h);
}

void WritableFile::write_at(std::uint64_t offset, const void* src, std::size_t bytes) {
    const auto* in = static_cast<const char*>(src);
    while (bytes > 0) {
        const DWORD chunk = static_cast<DWORD>(bytes > (1u << 30) ? (1u << 30) : bytes);
        OVERLAPPED ov{};
        ov.Offset = static_cast<DWORD>(offset & 0xffffffffu);
        ov.OffsetHigh = static_cast<DWORD>(offset >> 32);
        DWORD put = 0;
        if (!WriteFile(impl_->h, in, chunk, &put, &ov) || put == 0)
            throw std::runtime_error("write failed at offset " + std::to_string(offset));
        in += put;
        offset += put;
        bytes -= put;
    }
}

void WritableFile::flush() { FlushFileBuffers(impl_->h); }

#else

struct RandomAccessFile::Impl {
    int fd = -1;
};

RandomAccessFile::RandomAccessFile(const std::filesystem::path& path) : impl_(std::make_unique<Impl>()) {
    impl_->fd = ::open(path.c_str(), O_RDONLY);
    if (impl_->fd < 0) throw std::runtime_error("cannot open " + path.string());
    struct stat st {};
    ::fstat(impl_->fd, &st);
    size_ = static_cast<std::uint64_t>(st.st_size);
}

RandomAccessFile::~RandomAccessFile() {
    if (impl_ && impl_->fd >= 0) ::close(impl_->fd);
}

void RandomAccessFile::read_at(std::uint64_t offset, void* dst, std::size_t bytes) const {
    auto* out = static_cast<char*>(dst);
    while (bytes > 0) {
        const ssize_t got = ::pread(impl_->fd, out, bytes, static_cast<off_t>(offset));
        if (got <= 0) throw std::runtime_error("read failed at offset " + std::to_string(offset));
        out += got;
        offset += static_cast<std::uint64_t>(got);
        bytes -= static_cast<std::size_t>(got);
    }
}

struct WritableFile::Impl {
    int fd = -1;
};

WritableFile::WritableFile(const std::filesystem::path& path) : impl_(std::make_unique<Impl>()) {
    impl_->fd = ::open(path.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (impl_->fd < 0) throw std::runtime_error("cannot create " + path.string());
}

WritableFile::~WritableFile() {
    if (impl_ && impl_->fd >= 0) ::close(impl_->fd);
}

void WritableFile::write_at(std::uint64_t offset, const void* src, std::size_t bytes) {
    const auto* in = static_cast<const char*>(src);
    while (bytes > 0) {
        const ssize_t put = ::pwrite(impl_->fd, in, bytes, static_cast<off_t>(offset));
        if (put <= 0) throw std::runtime_error("write failed at offset " + std::to_string(offset));
        in += put;
        offset += static_cast<std::uint64_t>(put);
        bytes -= static_cast<std::size_t>(put);
    }
}

void WritableFile::flush() { ::fsync(impl_->fd); }

#endif

}  // namespace deformdb
