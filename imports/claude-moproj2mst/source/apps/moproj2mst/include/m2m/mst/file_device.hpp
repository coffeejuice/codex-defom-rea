// Shipped Qt QFile WriteOnly device for the native Save path. See
// analysis/wave3-boolean-save-io-contract.json and the buffering supplement.
#pragma once

#include <memory>
#include "m2m/mst/stream.hpp"

namespace m2m::mst {

// QIODevice Text-mode write, 0x670bb827..0x670bb9aa: split at LF and
// independently write each span and CRLF. Closed devices ignore Text-mode
// setter calls because native QIODevice rejects changes without an open handle.
// Returns consumed source bytes;
// a positive short span is not retried before proceeding to the newline.
// The underlying device is the write-only binary QFile data boundary.
std::int64_t writeTextMode(OutputDevice& device, std::string_view bytes);

class FileDevice final : public OutputDevice {
public:
    explicit FileDevice(const qt::String& path, bool textMode = false);
    ~FileDevice() override;
    FileDevice(const FileDevice&) = delete;
    FileDevice& operator=(const FileDevice&) = delete;

    std::int64_t write(std::string_view bytes) override;
    bool flush() override;
    void close() override;
    bool opened() const override;
    bool isTextModeEnabled() const override;
    void setTextModeEnabled(bool enabled) override;
    // QFile::error (private+0x120), separate from QTextStream's sticky status.
    int error() const;

private:
    std::int64_t writeRaw(std::string_view bytes);
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace m2m::mst
