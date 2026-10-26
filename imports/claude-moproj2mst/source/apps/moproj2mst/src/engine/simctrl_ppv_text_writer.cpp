#include "m2m/engine/simctrl_ppv_text_writer.hpp"

#include "m2m/engine/simctrl_ppv_save_file.hpp"
#include "m2m/engine/simctrl_ppv_text_device.hpp"
#include "m2m/engine/simctrl_ppv_writer.hpp"
#include "m2m/mst/file_device.hpp"

#include <string>
#include <string_view>

namespace m2m::engine::simctrl_ppv {
namespace {

bool splitPayload(const std::string& payload, std::vector<std::string>& lines)
{
    std::size_t begin = 0;
    while (begin < payload.size()) {
        const std::size_t end = payload.find("\r\n", begin);
        if (end == std::string::npos) return false;
        lines.emplace_back(payload.substr(begin, end - begin));
        begin = end + 2;
    }
    return !lines.empty();
}

TextWriterResult writeAfterOpen(mst::OutputDevice& file, unsigned localeCodepage,
                                const std::vector<Point>& local,
                                const std::vector<Point>* shared,
                                const std::vector<bool>* flags)
{
    TextWriterResult result;
    result.opened = file.opened();
    if (!result.opened) {
        result.status = TextWriterStatus::OpenFailed;
        return result;
    }

    const WriteResult payload = writePpvPayload(false, true, local, shared, flags,
                                                 localeCodepage);
    std::vector<std::string> lines;
    if (!payload.returned || !splitPayload(payload.bytes, lines)) {
        // The bounded writer profile produces ASCII CRLF lines. If that
        // internal contract changes, do not reinterpret bytes as another codec.
        result.status = TextWriterStatus::CodecUnqualified;
        file.close();
        return result;
    }

    const TextDeviceWriteResult written = writePpvTextSegments(file, localeCodepage, lines);
    result.opened = written.opened;
    result.writerReturned = written.writerReturned;
    result.streamFailed = written.streamFailed;
    result.status = written.writerReturned ? TextWriterStatus::Returned
                                           : TextWriterStatus::OpenFailed;
    return result;
}

} // namespace

TextWriterResult writePpvTextWriterAfterOpen(
    mst::OutputDevice& file, unsigned localeCodepage,
    const std::vector<Point>& local, const std::vector<Point>* shared,
    const std::vector<bool>* flags)
{
    TextWriterResult result;
    if (local.empty()) {
        result.status = TextWriterStatus::LocalListEmpty;
        return result;
    }
    result = writeAfterOpen(file, localeCodepage, local, shared, flags);
    result.openAttempted = true; // The caller supplies a post-open device outcome.
    return result;
}

TextWriterResult writePpvTextWriterFile(
    const qt::String& path, unsigned localeCodepage, bool asciiCompatible,
    const std::vector<Point>& local, const std::vector<Point>* shared,
    const std::vector<bool>* flags)
{
    TextWriterResult result;
    if (local.empty()) {
        result.status = TextWriterStatus::LocalListEmpty;
        return result; // Native local-list gate precedes QFile construction.
    }
    if (!asciiCompatible || localeCodepage == 0) {
        result.status = TextWriterStatus::CodecUnqualified;
        return result;
    }
    if (!ppvCountsRepresentable(local.size(), shared ? shared->size() : 0)) {
        result.status = TextWriterStatus::CountNotRepresentable;
        return result;
    }
    if (shared && flags && flags->size() < shared->size()) {
        result.status = TextWriterStatus::FlagsTooShort;
        return result;
    }

    mst::FileDevice file(path); // QFile WriteOnly | Text modeled by FileDevice + sink.
    result.openAttempted = true;
    if (!file.opened()) {
        result.status = TextWriterStatus::OpenFailed;
        return result;
    }
    result = writeAfterOpen(file, localeCodepage, local, shared, flags);
    result.openAttempted = true;
    return result;
}

} // namespace m2m::engine::simctrl_ppv
