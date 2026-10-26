// Shipped Qt QFile WriteOnly device for the native Save path. See
// analysis/wave3-boolean-save-io-contract.json and the buffering supplement.
#pragma once

#include <memory>
#include "m2m/mst/stream.hpp"

namespace m2m::mst {

// QIODevice Text-mode write, 0x670bb827..0x670bb9aa: split at LF and
// independently write each span and CRLF. Returns consumed source bytes;
// a positive short span is not retried before proceeding to the newline.
// The underlying device is the write-only binary QFile data boundary.
std::int64_t writeTextMode(OutputDevice& device, std::string_view bytes);

class FileDevice final : public OutputDevice {
public:
    explicit FileDevice(const qt::String& path);
    ~FileDevice() override;
    FileDevice(const FileDevice&) = delete;
    FileDevice& operator=(const FileDevice&) = delete;

    std::int64_t write(std::string_view bytes) override;
    bool flush() override;
    void close() override;
    bool opened() const override;
    // QFile::error (private+0x120), separate from QTextStream's sticky status.
    int error() const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace m2m::mst
