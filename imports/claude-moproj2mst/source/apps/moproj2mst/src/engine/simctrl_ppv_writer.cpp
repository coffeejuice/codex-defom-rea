#include "m2m/engine/simctrl_ppv_writer.hpp"

#include "m2m/mst/stream.hpp"

namespace m2m::engine::simctrl_ppv {

WriteResult writePpvPayload(bool initialPredicate, bool fileOpenSucceeded,
                            const std::vector<Point>& local,
                            const std::vector<Point>* shared,
                            const std::vector<bool>* flags,
                            unsigned localeCodepage)
{
    WriteResult result;
    if (initialPredicate) return result; // 0x140d4afe0 skips QFile construction/open.
    result.openAttempted = true;
    if (!fileOpenSucceeded) return result;

    mst::Stream text(localeCodepage);
    const int count = static_cast<int>(local.size()) +
        (shared ? static_cast<int>(shared->size()) : 0); // Count includes shared even without flags.
    text << count;
    text.endl();

    if (shared && flags) {
        // Native emits every shared row first, using the matching indexed flag.
        for (std::size_t i = 0; i < shared->size(); ++i) {
            const Point& point = (*shared)[i];
            text << static_cast<int>(i) << "\t" << "true" << "\t"
                 << point.firstIndex << "\t" << point.secondIndex << "\t"
                 << point.firstValue << "\t" << point.secondValue << "\t"
                 << static_cast<int>((*flags)[i]);
            text.endl();
        }
    }
    // Native local row indices restart at zero after shared rows.
    for (std::size_t i = 0; i < local.size(); ++i) {
        const Point& point = local[i];
        text << static_cast<int>(i) << "\t" << "false" << "\t"
             << point.firstIndex << "\t" << point.secondIndex << "\t"
             << point.firstValue << "\t" << point.secondValue << "\t" << 0;
        text.endl();
    }

    // Original QFile is opened with QIODevice::Text. On this Windows target,
    // translate QTextStream's LF endl bytes to the QFile CRLF byte sequence.
    for (const char c : text.bytes()) {
        if (c == '\n') result.bytes += "\r\n";
        else result.bytes.push_back(c);
    }
    result.returned = true; // Native ignores post-open QTextStream/QFile close status.
    return result;
}

} // namespace m2m::engine::simctrl_ppv
