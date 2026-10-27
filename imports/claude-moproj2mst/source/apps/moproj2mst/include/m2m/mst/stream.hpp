// m2m/mst/stream.hpp: the MST output stream, QTextStream as DEF_GUI_MO uses it (seam; owner P04).
// s1-02 1: QFile WriteOnly (no Text flag: bare LF), no BOM, stream defaults never changed, the
// locale codec, endl writes '\n' and flushes.
//
// Qt 4.8 QTextStream keeps written text in a UTF-16 write buffer and encodes it with the codec
// when it flushes: at endl / flush, and when the buffer grows past 16384 characters. Literals
// (operator<<(const char*)) enter that buffer as QLatin1String, so they go through the codec too.
// QFile::close emits aboutToClose while still open; the attached QTextStream flushes its
// pending text there. A sticky stream error suppresses that flush, independently of QFile's
// initial retry of its own byte buffer. bytes() observes accepted output, not durable bytes.
#pragma once

#include <string>
#include <string_view>
#include <cstdint>
#include <functional>

#include "m2m/qt4.hpp"

namespace m2m::mst {
using qt::String;

// QFile-facing boundary. The stream owns text/status; the device owns buffered
// bytes and its handle. The caller closes the device before destroying Stream,
// matching the explicit QFile::close in the original writers.
class OutputDevice {
public:
    virtual ~OutputDevice() = default;
    virtual std::int64_t write(std::string_view bytes) = 0;
    virtual bool flush() = 0;
    virtual void close() = 0;
    virtual bool opened() const = 0;
    virtual bool isTextModeEnabled() const { return false; }
    virtual void setTextModeEnabled(bool) {}
    // One stream per device in this recovered lifecycle. Qt uses an embedded
    // QObject receiver; this scoped callback preserves its ownership and order.
    void setAboutToClose(std::function<void()> callback) { aboutToClose_ = std::move(callback); }
protected:
    void notifyAboutToClose() { if (aboutToClose_) aboutToClose_(); }
private:
    std::function<void()> aboutToClose_;
};

class Stream {
public:
    explicit Stream(unsigned codepage, OutputDevice* device = nullptr)
        : codepage_(codepage), device_(device)
    {
        if (device_) device_->setAboutToClose([this] { flush(); });
    }
    ~Stream()
    {
        // ~QTextStream0x670c4f20 flushes pending text before destroying the
        // receiver; it neither owns nor closes the external QFile.
        flush();
        if (device_) device_->setAboutToClose({});
    }
    Stream(const Stream&) = delete;
    Stream& operator=(const Stream&) = delete;
    Stream(Stream&& other) : Stream(other.codepage_) { *this = std::move(other); }
    Stream& operator=(Stream&& other)
    {
        if (this == &other) return *this;
        flush();
        if (device_) device_->setAboutToClose({});
        buf_ = std::move(other.buf_);
        wbuf_ = std::move(other.wbuf_);
        codepage_ = other.codepage_;
        device_ = other.device_;
        writeFailed_ = other.writeFailed_;
        other.device_ = nullptr;
        other.wbuf_.clear();
        if (device_) device_->setAboutToClose([this] { flush(); });
        return *this;
    }

    Stream& operator<<(const char* lit);     // QLatin1String(lit)
    Stream& operator<<(int v);               // qt::formatInt
    Stream& operator<<(double v);            // qt::formatDouble
    Stream& operator<<(const String& s);     // qt::encodeLocale(s, codepage) at flush
    Stream& endl();                          // "\n" + flush (0x140041bc0)
    Stream& flush();                         // QTextStream::flush

    // Bytes written to the file so far (flushed text only).
    const std::string& bytes() const { return buf_; }
    // Text still in the write buffer (close notification flushes unless status failed).
    const std::u16string& pending() const { return wbuf_; }
    unsigned codepage() const { return codepage_; }
    // QTextStreamPrivate+0xcc: 3 is WriteFailed and suppresses later flushes.
    bool writeFailed() const { return writeFailed_; }

private:
    // Avoid accidental bool/char/size_t overloads: the original only streams these four types.
    Stream& operator<<(bool) = delete;
    Stream& operator<<(char) = delete;
    Stream& operator<<(unsigned) = delete;
    Stream& operator<<(long long) = delete;

    void write(const std::u16string& s);   // QTextStreamPrivate::write

    std::string buf_;
    std::u16string wbuf_;
    unsigned codepage_;
    OutputDevice* device_ = nullptr; // non-owning, must outlive this stream
    bool writeFailed_ = false;
};

// QTextStream manipulator form: ts << "x" << endl;
struct EndlTag {};
inline constexpr EndlTag endl{};
inline Stream& operator<<(Stream& s, EndlTag) { return s.endl(); }

}  // namespace m2m::mst
