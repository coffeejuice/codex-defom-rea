#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "m2m/engine/simcontrol300_remesh.hpp"
#include <algorithm>
#include <array>
#include <cerrno>
#include <clocale>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <cstdio>
#include <fcntl.h>
#include <io.h>
#include "datafiles/cogging_file.hpp"
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/stdlib/stdio_compat.hpp>

namespace m2m::engine::simcontrol300 {
namespace {
using Reader = datafiles::TokenReader;
struct UnrecoveredInput {};
void validateChunk(const std::string& chunk)
{
    if (chunk.size() > 1999) throw UnrecoveredInput{};
    // TokenReader consumes a C-string prefix even when fgets read through NUL.
    // Original table143dd8250 reaches line buffer143dd8660 at pointer index130.
    // At most128 tokens plus null at129 avoid original table/line overlap.
    // The previous memory-profile claim of259 was also unsafe for the host's
    // 260-pointer table: its null at260 would exceed that independent array.
    const auto end = chunk.find('\0');
    const std::string_view visible(chunk.data(), end == std::string::npos ? chunk.size() : end);
    bool inToken = false;
    unsigned tokens = 0;
    for (const unsigned char c : visible) {
        if (c > 127) throw UnrecoveredInput{};
        const bool separator = std::string_view(" ,:=\n\t\r").find(static_cast<char>(c)) != std::string_view::npos;
        if (!separator && !inToken && ++tokens > 128) throw UnrecoveredInput{};
        inToken = !separator;
    }
}

class NativeReadFile final : public RemeshReadFile {
public:
    explicit NativeReadFile(const std::string& path)
        : text_(datafiles::TextFile::fromLineReader([this](std::string& out, int n) {
            if (!stream_) throw Error(M2M_E_UNSUPPORTED, "remesh read after close");
            if (n < 2 || n > 2000) throw Error(M2M_E_UNSUPPORTED, "remesh fgets size is outside recovered profile");
            std::array<char, 2000> chunk;
            if (!std::fgets(chunk.data(), n, stream_)) return false;
            // fgets already consumed the whole chunk, including any bytes after
            // embedded NUL. The original tokenizer sees only this prefix.
            out.assign(chunk.data());
            return true;
        }))
    {
        SECURITY_ATTRIBUTES security{sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE};
        HANDLE handle = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
                                    &security, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (handle == INVALID_HANDLE_VALUE) return;
        if (GetFileType(handle) != FILE_TYPE_DISK) {
            CloseHandle(handle);
            throw Error(M2M_E_UNSUPPORTED, "remesh input is not a regular disk file");
        }
        const int fd = _open_osfhandle(reinterpret_cast<std::intptr_t>(handle), _O_RDONLY | _O_TEXT);
        if (fd == -1) { CloseHandle(handle); return; }
        stream_ = _fdopen(fd, "rt");
        if (!stream_) { _close(fd); return; }
        // Original normal _getbuf allocation is4096 bytes. Its allocation-failed
        // two-byte fallback is not silently replaced with a current-CRT default.
        if (std::setvbuf(stream_, buffer_.data(), _IOFBF, buffer_.size()) != 0) {
            close();
            throw Error(M2M_E_UNSUPPORTED, "remesh CRT buffer profile unavailable");
        }
    }
    ~NativeReadFile() override { close(); }
    bool opened() const { return stream_ != nullptr; }
    datafiles::TextFile& tokens() override { return text_; }
    void rewind() override {
        if (!stream_) throw Error(M2M_E_UNSUPPORTED, "remesh rewind after close");
        std::rewind(stream_); // Clears EOF/error; original ignores seek failure.
    }
    void close() override {
        if (stream_) { std::fclose(stream_); stream_ = nullptr; }
    }
private:
    std::FILE* stream_ = nullptr;
    std::array<char, 4096> buffer_;
    datafiles::TextFile text_;
};

class NativeIo final : public RemeshIo {
public:
    explicit NativeIo(std::string directory) : directory_(std::move(directory))
    {
        const bool drive = directory_.size() >= 3 &&
            ((directory_[0] >= 'A' && directory_[0] <= 'Z') || (directory_[0] >= 'a' && directory_[0] <= 'z')) &&
            directory_[1] == ':' && (directory_[2] == '\\' || directory_[2] == '/');
        if (!drive || directory_.size() + 1 + std::string_view("LOCAL_REMESH.DAT").size() >= MAX_PATH)
            throw Error(M2M_E_UNSUPPORTED, "remesh directory is outside ordinary narrow absolute-path profile");
        for (std::size_t i = 0; i < directory_.size(); ++i) {
            const unsigned char c = directory_[i];
            if (c < 32 || c > 126 || std::string_view("\"*?<>|").find(static_cast<char>(c)) != std::string_view::npos ||
                (c == ':' && i != 1))
                throw Error(M2M_E_UNSUPPORTED, "remesh directory requires unrecovered path encoding or namespace");
        }
        if (directory_.back() != '\\' && directory_.back() != '/') directory_ += '\\';
    }
    std::unique_ptr<RemeshReadFile> openRead(std::string_view name) override
    {
        if (name != "LOCAL_REMESH.DAT" && name != "LOC_DEN_CTRL.DAT")
            throw Error(M2M_E_ARG, "remesh native reader accepts only original fixed filenames");
        auto file = std::make_unique<NativeReadFile>(directory_ + std::string(name));
        if (!file->opened()) return {};
        return file;
    }
    const std::string& directory() const { return directory_; }
private:
    std::string directory_;
};
bool space(unsigned char c) { return c == 32 || (c >= 9 && c <= 13); }
std::optional<int> originalInt(std::string_view token)
{
    std::size_t i = 0;
    while (i < token.size() && space(static_cast<unsigned char>(token[i]))) ++i;
    bool negative = false;
    if (i < token.size() && (token[i] == '+' || token[i] == '-')) negative = token[i++] == '-';
    const std::uint64_t limit = negative ? 2147483648ULL : 2147483647ULL;
    std::uint64_t value = 0;
    while (i < token.size() && token[i] >= '0' && token[i] <= '9') {
        const unsigned digit = static_cast<unsigned>(token[i++] - '0');
        if (value > (limit - digit) / 10) return std::nullopt;
        value = value * 10 + digit;
    }
    if (negative && value == 2147483648ULL) return (std::numeric_limits<int>::min)();
    return negative ? -static_cast<int>(value) : static_cast<int>(value);
}
std::optional<double> originalReal(std::string token)
{
    for (char& c : token) if (c == 'd' || c == 'D') c = 'E';
    auto start = token.find_first_not_of(" \t\r\n\v\f");
    if (start != std::string::npos && (token[start] == '+' || token[start] == '-')) ++start;
    // Native CRT hexadecimal/nonfinite/range branches are separate recovery.
    if (start != std::string::npos && token.size() >= start + 2 && token[start] == '0' &&
        (token[start + 1] == 'x' || token[start + 1] == 'X')) return std::nullopt;
    errno = 0;
    const double value = std::strtod(token.c_str(), nullptr);
    if (!std::isfinite(value) || errno == ERANGE) return std::nullopt;
    return value;
}
struct Close {
    RemeshReadFile* file;
    ~Close() { if (file) file->close(); }
    void finish() { if (file) file->close(); file = nullptr; }
};
}

std::unique_ptr<RemeshIo> makeNativeRemeshIo(std::string directory)
{
    return std::make_unique<NativeIo>(std::move(directory));
}

ApplyResult applyRemeshUniformEdgeToken(RemeshState& state, std::string_view token)
{
    if (token.size() > 25 || token.find('\0') != std::string_view::npos)
        return ApplyResult::UnsupportedSnapshot;
    // Original strcpy has no bound. Model only the independently recovered
    // +10..29 span, retaining all untouched suffix bytes between fields.
    std::array<std::byte, 26> bytes;
    std::memcpy(bytes.data(), &state.uniformInternalEdge, 8);
    bytes[8] = static_cast<std::byte>(state.improveContact);
    std::copy(state.contactToScalingBytes.begin(), state.contactToScalingBytes.end(), bytes.begin() + 9);
    std::memcpy(bytes.data() + 16, &state.scalingFactor, 8);
    bytes[24] = static_cast<std::byte>(state.goodSurfaceChangeable);
    bytes[25] = static_cast<std::byte>(state.enabled);
    if (!token.empty()) std::memcpy(bytes.data(), token.data(), token.size());
    bytes[token.size()] = std::byte{0};
    std::memcpy(&state.uniformInternalEdge, bytes.data(), 8);
    state.improveContact = std::to_integer<std::uint8_t>(bytes[8]);
    std::copy(bytes.begin() + 9, bytes.begin() + 16, state.contactToScalingBytes.begin());
    std::memcpy(&state.scalingFactor, bytes.data() + 16, 8);
    state.goodSurfaceChangeable = std::to_integer<std::uint8_t>(bytes[24]);
    state.enabled = std::to_integer<std::uint8_t>(bytes[25]);
    return ApplyResult::Completed;
}

static RemeshReadReport readRemeshImpl(RemeshState& state, RemeshIo& io, NumericLocale locale)
{
    const char* numeric = std::setlocale(LC_NUMERIC, nullptr);
    if (locale != NumericLocale::KnownC || !numeric || std::strcmp(numeric, "C") != 0)
        return {ApplyResult::UnsupportedSnapshot, std::nullopt};
    Reader reader;
    auto file = io.openRead("LOCAL_REMESH.DAT");
    Close close{file.get()};
    auto input = datafiles::TextFile::fromLineReader([&](std::string& out, int n) {
        if (!file->tokens().fgets(out, n)) return false;
        validateChunk(out);
        return true;
    });
    bool keywordDriven = false;
    std::string keyword;
    if (file) {
        for (;;) {
            reader.read(input, Reader::Upper, &keyword, Reader::kNewLine);
            if (keyword.size() >= 208) return {ApplyResult::UnsupportedSnapshot, std::nullopt};
            if (keyword == "KEYWORD_DRIVEN") { keywordDriven = true; break; }
            if (keyword == "DONE") break;
        }
        file->rewind(); // Original rewinds even when the scan did not find marker.
    }
    if (!keywordDriven) {
        close.finish();
        file = io.openRead("LOC_DEN_CTRL.DAT");
        close.file = file.get();
        if (!file) return {ApplyResult::Completed, false};
    }
    std::optional<int> lastInt; // Original shared local; reject first unassigned use.
    auto integer = [&]() -> std::optional<int> {
        std::string token;
        const int result = reader.read(input, Reader::Text, &token);
        if (result >= 0) lastInt = originalInt(token);
        return lastInt;
    };
    auto real = [&](double& destination) {
        std::string token;
        if (reader.read(input, Reader::Text, &token) < 0) return true;
        auto value = originalReal(std::move(token));
        if (!value) return false;
        destination = *value;
        return true;
    };
    for (;;) {
        reader.read(input, Reader::Upper, &keyword, Reader::kNewLine);
        if (keyword.size() >= 208) return {ApplyResult::UnsupportedSnapshot, std::nullopt};
        if (keyword == "INTERNAL_ELEMENT_SIZE_CONTROL_METHOD") {
            const auto value = integer(); if (!value) return {ApplyResult::UnsupportedSnapshot, std::nullopt};
            state.method = (std::min)((std::max)(*value, 0), 5);
        } else if (keyword == "GOOD_INTERNAL_ELEMENT_CHANGEABLE" ||
                   keyword == "FURTHER_IMPROVE_CONTACT_ELEMENTS" ||
                   keyword == "GOOD_SURFACE_ELEMENT_CHANGEABLE") {
            const auto value = integer(); if (!value) return {ApplyResult::UnsupportedSnapshot, std::nullopt};
            if (keyword == "GOOD_INTERNAL_ELEMENT_CHANGEABLE") state.goodInternalChangeable = *value != 0;
            else if (keyword == "FURTHER_IMPROVE_CONTACT_ELEMENTS") state.improveContact = *value != 0;
            else state.goodSurfaceChangeable = *value != 0;
        } else if (keyword == "FILTER_DISTANCE") {
            if (!real(state.filterDistance)) return {ApplyResult::UnsupportedSnapshot, std::nullopt};
        } else if (keyword == "UNIFORM_INTERNAL_EDGE_LENGTH") {
            std::string token;
            if (reader.read(input, Reader::Raw, &token) >= 0 &&
                applyRemeshUniformEdgeToken(state, token) != ApplyResult::Completed)
                return {ApplyResult::UnsupportedSnapshot, std::nullopt};
        } else if (keyword == "SCALING_FACTOR_EACH_LAYER_INWARD") {
            if (!real(state.scalingFactor)) return {ApplyResult::UnsupportedSnapshot, std::nullopt};
            const double value = state.scalingFactor;
            state.scalingFactor = value >= 1.0 ? (value <= 10.0 ? value : 10.0) : 1.0;
        }
        if (keyword == "DONE") break;
    }
    return {ApplyResult::Completed, true};
}

RemeshReadReport readRemesh(RemeshState& state, RemeshIo& io, NumericLocale locale)
{
    try { return readRemeshImpl(state, io, locale); }
    catch (const UnrecoveredInput&) { return {ApplyResult::UnsupportedSnapshot, std::nullopt}; }
}

RemeshReadReport loadRemeshAtProjectOpen(RemeshState& state, RemeshIo& io, NumericLocale locale)
{
    auto report = readRemesh(state, io, locale);
    if (report.loaded) state.enabled = *report.loaded;
    return report;
}

RemeshSaveReport saveRemeshAtProjectSave(const RemeshState& state, const std::string& directory,
                                       NumericLocale locale)
{
    // Reuse exactly the reader's ordinary narrow-directory profile. No stream
    // is opened by NativeIo construction; original uses a fixed relative name.
    NativeIo profile(directory);
    const auto path = profile.directory() + "LOCAL_REMESH.DAT";
    RemeshSaveReport report;
    if (!state.enabled) {
        // Qt4 QFile::remove ->QFSFileEngine->670ff860->DeleteFileW. This is
        // unconditional removal, with no CRT read probe or delayed replay.
        const std::wstring wide(path.begin(), path.end());
        report.removalSucceeded = DeleteFileW(wide.c_str()) != 0;
        return report; // Original ignores result, including missing/denied.
    }
    const char* numeric = std::setlocale(LC_NUMERIC, nullptr);
    if (locale != NumericLocale::KnownC || !numeric || std::strcmp(numeric, "C") != 0) {
        report.outcome = ApplyResult::UnsupportedSnapshot;
        return report;
    }
    datafiles::detail::CoggingFile file(path);
    report.opened = file.get() != nullptr;
    if (!*report.opened) {
        // Source0abcf0 calls fprintf withNULL. Do not substitute the other KEY
        // writers' ignored fopen failure or invoke host undefined behavior.
        report.outcome = ApplyResult::UnsupportedSnapshot;
        report.invalidParameterReached = true;
        return report;
    }
    def::rt::init_stdio_compat();
    auto* f = file.get();
    // Exact31 source fprintf calls, including comments/blanklines. No DONE.
    def::rt::crt::fprintf(f, "* this file is generated by local remeshing program\n");
    def::rt::crt::fprintf(f, "* this file is keyword driven, the sequence does not matter.\n");
    def::rt::crt::fprintf(f, "%s\n\n", "KEYWORD_DRIVEN");
    def::rt::crt::fprintf(f, "* internal mesh control method\n");
    def::rt::crt::fprintf(f, "*     1 - use neighboring average edge length for internal nodes\n");
    def::rt::crt::fprintf(f, "*     2 - use the distance filter and uniform edge length for internal nodes\n");
    def::rt::crt::fprintf(f, "*     3 - use a scaling factor for each layer toward inside\n");
    def::rt::crt::fprintf(f, "%s %d\n", "INTERNAL_ELEMENT_SIZE_CONTROL_METHOD", state.method);
    def::rt::crt::fprintf(f, "\n");
    def::rt::crt::fprintf(f, "* good internal elements changeable or not\n");
    def::rt::crt::fprintf(f, "*     1 - changeable; 0 - unchangeable\n");
    def::rt::crt::fprintf(f, "%s %d\n", "GOOD_INTERNAL_ELEMENT_CHANGEABLE", static_cast<int>(state.goodInternalChangeable));
    def::rt::crt::fprintf(f, "\n");
    def::rt::crt::fprintf(f, "* the following two data is only valid/needed when the internal mesh control method = 2\n");
    def::rt::crt::fprintf(f, "* filter distance, within this distance, use the density system calculated\n");
    def::rt::crt::fprintf(f, "%s %lf\n", "FILTER_DISTANCE", state.filterDistance);
    def::rt::crt::fprintf(f, "\n");
    def::rt::crt::fprintf(f, "* uniform edge length for the nodes that are beyond the filter distance\n");
    def::rt::crt::fprintf(f, "%s %lf\n", "UNIFORM_INTERNAL_EDGE_LENGTH", state.uniformInternalEdge);
    def::rt::crt::fprintf(f, "\n");
    def::rt::crt::fprintf(f, "* should the contact elements be further improved or not\n");
    def::rt::crt::fprintf(f, "*     1 - further improve; 0 - not further improve\n");
    def::rt::crt::fprintf(f, "%s %d\n", "FURTHER_IMPROVE_CONTACT_ELEMENTS", static_cast<int>(state.improveContact));
    def::rt::crt::fprintf(f, "\n");
    def::rt::crt::fprintf(f, "* scaling factor for each layer inward\n");
    def::rt::crt::fprintf(f, "* only useful when the internal mesh control is by using a scaling factor \n");
    def::rt::crt::fprintf(f, "%s %lf\n", "SCALING_FACTOR_EACH_LAYER_INWARD", state.scalingFactor);
    def::rt::crt::fprintf(f, "\n");
    def::rt::crt::fprintf(f, "* good surface elements changeable or not\n");
    def::rt::crt::fprintf(f, "*     1 - changeable; 0 - unchangeable\n");
    def::rt::crt::fprintf(f, "%s %d\n", "GOOD_SURFACE_ELEMENT_CHANGEABLE", static_cast<int>(state.goodSurfaceChangeable));
    file.close(); // Original ignores all write and fclose results.
    return report;
}
} // namespace m2m::engine::simcontrol300
