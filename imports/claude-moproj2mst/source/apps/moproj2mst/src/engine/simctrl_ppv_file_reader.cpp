#include "m2m/engine/simctrl_ppv_file_reader.hpp"

#include "m2m/datafiles.hpp"
#include "m2m/longpath.hpp"
#include "m2m/qt4path.hpp"

#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <string_view>

namespace m2m::engine::simctrl_ppv {
namespace {

bool acceptedText(char16_t ch)
{
    return ch == u'\t' || ch == u'\r' || ch >= 0x20;
}

bool decodeUtf16Le(std::string_view bytes, std::u16string& text)
{
    if (bytes.size() < 2 || (bytes.size() & 1) != 0) return false;
    text.clear();
    text.reserve((bytes.size() - 2) / 2);
    for (std::size_t i = 2; i < bytes.size(); i += 2) {
        const auto lo = static_cast<unsigned char>(bytes[i]);
        const auto hi = static_cast<unsigned char>(bytes[i + 1]);
        text.push_back(static_cast<char16_t>(lo | (static_cast<unsigned>(hi) << 8)));
    }
    for (std::size_t i = 0; i < text.size(); ++i) {
        const char16_t ch = text[i];
        if (ch >= 0xd800 && ch <= 0xdbff) {
            if (i + 1 == text.size() || text[i + 1] < 0xdc00 || text[i + 1] > 0xdfff)
                return false;
            ++i;
        } else if (ch >= 0xdc00 && ch <= 0xdfff) {
            return false;
        }
    }
    return true;
}

bool decodeAscii(std::string_view bytes, std::u16string& text)
{
    text.clear();
    text.reserve(bytes.size());
    for (const unsigned char byte : bytes) {
        if (byte >= 0x80 || byte == 0) return false;
        text.push_back(static_cast<char16_t>(byte));
    }
    return true;
}

bool hasUtf8Bom(std::string_view bytes)
{
    return bytes.size() >= 3 &&
        static_cast<unsigned char>(bytes[0]) == 0xef &&
        static_cast<unsigned char>(bytes[1]) == 0xbb &&
        static_cast<unsigned char>(bytes[2]) == 0xbf;
}

bool splitReadLines(const std::u16string& text, std::vector<qt::String>& lines)
{
    std::size_t begin = 0;
    while (begin < text.size()) {
        const std::size_t lf = text.find(u'\n', begin);
        std::size_t end = lf == std::u16string::npos ? text.size() : lf;
        if (end > begin && text[end - 1] == u'\r') --end; // QTextStream readLine CRLF path.
        for (std::size_t i = begin; i < end; ++i) {
            if (!acceptedText(text[i])) return false;
        }
        lines.emplace_back(text.substr(begin, end - begin));
        if (lf == std::u16string::npos) break;
        begin = lf + 1;
    }
    return true;
}

FileLines decodePpvBytes(const std::string& bytes)
{
    FileLines result;
    std::u16string text;
    const std::string_view view(bytes);
    bool decoded = false;
    if (view.size() >= 2 && static_cast<unsigned char>(view[0]) == 0xff &&
        static_cast<unsigned char>(view[1]) == 0xfe) {
        decoded = decodeUtf16Le(view, text);
    } else if (hasUtf8Bom(view)) {
        // Qt's BOM helper selects UTF-8. Accept only its ASCII subset until
        // the original UTF-8 decoder's non-ASCII conversion is qualified.
        decoded = decodeAscii(view.substr(3), text);
    } else {
        // BOM-less bytes use codecForLocale in the original default stream.
        // ASCII is common to the qualified locale codecs; other bytes stay guarded.
        decoded = decodeAscii(view, text);
    }
    if (!decoded || !splitReadLines(text, result.lines)) {
        result.status = FileReadStatus::UnsupportedEncodingOrLineEnding;
        result.lines.clear();
        return result;
    }
    result.status = FileReadStatus::Read;
    return result;
}

} // namespace

FileLines readPpvFileLines(const Project& project, const qt::String& path)
{
    std::string bytes;
    if (!datafiles::readFileBytes(project, path, bytes)) return {};
    return decodePpvBytes(bytes);
}

FileLines readPpvFileLinesFromDisk(const qt::String& path)
{
    if (path.isEmpty()) return {};
    std::ifstream input(std::filesystem::path(win32Path(path.u16())),
                        std::ios::binary);
    if (!input.is_open()) return {};
    std::string bytes{std::istreambuf_iterator<char>(input),
                      std::istreambuf_iterator<char>()};
    return decodePpvBytes(bytes);
}

} // namespace m2m::engine::simctrl_ppv

