// QTextStream output as DEF_GUI_MO's MST writers use it (owner: P04). Qt 4.8.4 qtextstream.cpp:
// QTextStreamPrivate::write / flushWriteBuffer / putString with the stream defaults (field width
// 0, so no padding), on a QFile opened WriteOnly without Text (no "\r\n" conversion).
#include "m2m/mst/stream.hpp"
#include <algorithm>

namespace m2m::mst {
namespace {
constexpr size_t kBufferSize = 16384;   // QTEXTSTREAM_BUFFERSIZE
}

void Stream::write(const std::u16string& s)
{
    wbuf_ += s;
    if (wbuf_.size() > kBufferSize) flush();
}

Stream& Stream::flush()
{
    // Shipped Qt0x670c2f50: sticky error guard; clear source text before device
    // write. Short/nonpositive write or QFile::flush failure records status3.
    if (writeFailed_ || wbuf_.empty()) return *this;
    const auto encoded = qt::encodeLocale(String(wbuf_), codepage_);
    wbuf_.clear();
    if (!device_) {
        buf_ += encoded;
        return *this;
    }
    const auto written = device_->write(encoded);
    // This buffer observes bytes accepted by the device, not durability. QFile
    // may retain a partial line for its independent close-time retry.
    if (written > 0) buf_.append(encoded, 0, (std::min)(encoded.size(), static_cast<size_t>(written)));
    if (written <= 0) {
        writeFailed_ = true;
        return *this;
    }
    const bool flushed = device_->flush();
    writeFailed_ = !flushed || static_cast<size_t>(written) != encoded.size();
    return *this;
}

Stream& Stream::operator<<(const char* lit)
{
    write(String::latin1(lit).u16());
    return *this;
}

Stream& Stream::operator<<(int v)
{
    write(String::latin1(qt::formatInt(v)).u16());
    return *this;
}

Stream& Stream::operator<<(double v)
{
    write(String::latin1(qt::formatDouble(v)).u16());
    return *this;
}

Stream& Stream::operator<<(const String& s)
{
    write(s.u16());
    return *this;
}

// @src 0x140041bc0 endl: stream << QLatin1Char('\n') << flush
Stream& Stream::endl()
{
    write(u"\n");
    return flush();
}

}  // namespace m2m::mst
