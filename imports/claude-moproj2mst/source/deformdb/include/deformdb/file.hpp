// Read-only positional file access. Every call is independent, so several
// threads can read different steps of the same open file at the same time.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>

namespace deformdb {

inline constexpr std::size_t kRecordBytes = 512;  // Fortran direct-access record

class RandomAccessFile {
public:
    explicit RandomAccessFile(const std::filesystem::path& path);
    ~RandomAccessFile();
    RandomAccessFile(const RandomAccessFile&) = delete;
    RandomAccessFile& operator=(const RandomAccessFile&) = delete;

    std::uint64_t size() const noexcept { return size_; }

    // Reads exactly `bytes` bytes at `offset`; throws on short read.
    void read_at(std::uint64_t offset, void* dst, std::size_t bytes) const;

    // Reads `bytes` bytes starting at 1-based record `record`.
    void read_records(std::int64_t record, void* dst, std::size_t bytes) const {
        read_at(static_cast<std::uint64_t>(record - 1) * kRecordBytes, dst, bytes);
    }

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
    std::uint64_t size_ = 0;
};

// Positional writer for a new database file (created or truncated on open).
class WritableFile {
public:
    explicit WritableFile(const std::filesystem::path& path);
    ~WritableFile();
    WritableFile(const WritableFile&) = delete;
    WritableFile& operator=(const WritableFile&) = delete;

    // Writes exactly `bytes` bytes at `offset`; throws on failure.
    void write_at(std::uint64_t offset, const void* src, std::size_t bytes);

    void write_records(std::int64_t record, const void* src, std::size_t bytes) {
        write_at(static_cast<std::uint64_t>(record - 1) * kRecordBytes, src, bytes);
    }

    void flush();

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace deformdb
