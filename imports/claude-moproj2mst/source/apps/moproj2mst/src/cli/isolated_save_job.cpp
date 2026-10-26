#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include "isolated_save_job.hpp"
#include "moproj2mst.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <vector>
#include <utility>

namespace m2m::cli {
namespace {
namespace th = engine::terminal_harness;
constexpr std::size_t maxJob = 64 * 1024, maxPath = 16 * 1024, headerSize = 52;
constexpr std::array<unsigned char, 8> magic{'M', '2', 'M', 'I', 'S', 'J', '1', 0};
constexpr unsigned knownFlags = M2M_FLAG_DOE_TEMP | M2M_FLAG_STRICT | M2M_FLAG_WRITE_MDT | M2M_FLAG_WRITE_SAVE;
void require(bool condition, const char* message)
{ if (!condition) throw std::runtime_error(message); }

class Handle final {
    HANDLE value_ = INVALID_HANDLE_VALUE;
public:
    explicit Handle(HANDLE value) : value_(value) {}
    ~Handle() { if (value_ != INVALID_HANDLE_VALUE) CloseHandle(value_); }
    HANDLE get() const noexcept { return value_; }
    void close()
    {
        const auto value = value_; value_ = INVALID_HANDLE_VALUE;
        require(CloseHandle(value) != FALSE, "isolated file close failed");
    }
    Handle(const Handle&) = delete;
    Handle(Handle&& other) noexcept : value_(std::exchange(other.value_, INVALID_HANDLE_VALUE)) {}
};
std::wstring wide(std::u16string_view text)
{
    static_assert(sizeof(wchar_t) == sizeof(char16_t));
    return std::wstring(text.begin(), text.end());
}
void validatePath(std::u16string_view text, bool optional)
{
    require((optional || !text.empty()) && text.size() <= maxPath, "isolated path length invalid");
    if (text.empty()) return;
    for (std::size_t i = 0; i < text.size(); ++i) {
        const auto c = static_cast<std::uint16_t>(text[i]);
        require(c >= 32 && c != '"' && c != '<' && c != '>' && c != '|' && c != '*' && c != '?',
                "isolated path contains forbidden character");
        if (c == ':') require(i == 1, "isolated path contains alternate stream or misplaced colon");
        if (c >= 0xd800 && c <= 0xdbff) {
            require(i + 1 < text.size() && text[i + 1] >= 0xdc00 && text[i + 1] <= 0xdfff,
                    "isolated path has unpaired high surrogate");
            ++i;
        } else require(c < 0xdc00 || c > 0xdfff, "isolated path has unpaired low surrogate");
    }
    const auto value = wide(text);
    require(!value.starts_with(L"\\\\.\\") && !value.starts_with(L"\\\\?\\"), "isolated device namespace rejected");
    require(std::filesystem::path(value).is_absolute(), "isolated path must be absolute");
}
void validate(const IsolatedJob& job)
{
    validatePath(job.project, false); validatePath(job.output, true); validatePath(job.deformDir, true);
    validatePath(job.workingDirectory, false);
    require(job.nonce != 0, "isolated job nonce must be nonzero");
    require((job.flags & M2M_FLAG_WRITE_SAVE) && !(job.flags & ~knownFlags), "isolated flags invalid");
    require(IsValidCodePage(job.codepage) != FALSE, "isolated codepage invalid");
    require(headerSize + 2 * (job.project.size() + job.output.size() + job.deformDir.size() + job.workingDirectory.size()) <= maxJob,
            "isolated job exceeds byte bound");
}
void localDirectory(const std::filesystem::path& directory)
{
    validatePath(directory.u16string(), false);
    const auto name = directory.native();
    require(name.size() >= 3 && name[1] == L':' && (name[2] == L'\\' || name[2] == L'/'),
            "isolated storage directory must be local absolute drive path");
    for (const auto& part : directory)
        require(part != L"." && part != L"..", "isolated storage dot component rejected");
    for (auto path = directory; !path.empty(); path = path.parent_path()) {
        const auto attrs = GetFileAttributesW(path.c_str());
        require(attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_DIRECTORY) &&
                !(attrs & FILE_ATTRIBUTE_REPARSE_POINT), "isolated storage parent missing or reparse point");
        if (path == path.parent_path()) break;
    }
}
Handle pinDirectory(const std::filesystem::path& path)
{
    localDirectory(path);
    Handle directory(CreateFileW(path.c_str(), FILE_READ_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT, nullptr));
    require(directory.get() != INVALID_HANDLE_VALUE, "isolated directory pin failed");
    BY_HANDLE_FILE_INFORMATION info{};
    require(GetFileInformationByHandle(directory.get(), &info) && (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
            !(info.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT), "isolated directory identity rejected");
    return directory;
}
std::wstring finalName(HANDLE file)
{
    const DWORD count = GetFinalPathNameByHandleW(file, nullptr, 0, FILE_NAME_NORMALIZED | VOLUME_NAME_DOS);
    require(count && count <= 32768, "isolated file final-name bound");
    std::wstring value(count, L'\0');
    const DWORD got = GetFinalPathNameByHandleW(file, value.data(), count, FILE_NAME_NORMALIZED | VOLUME_NAME_DOS);
    require(got && got < count, "isolated file final-name failed"); value.resize(got); return value;
}
void sameParent(HANDLE directory, HANDLE file)
{
    auto parent = finalName(directory);
    auto actual = std::filesystem::path(finalName(file)).parent_path().native();
    while (!parent.empty() && (parent.back() == L'\\' || parent.back() == L'/')) parent.pop_back();
    while (!actual.empty() && (actual.back() == L'\\' || actual.back() == L'/')) actual.pop_back();
    require(CompareStringOrdinal(parent.c_str(), -1, actual.c_str(), -1, TRUE) == CSTR_EQUAL,
            "isolated file escaped pinned parent");
}
void put(std::vector<unsigned char>& bytes, std::uint32_t value)
{ for (unsigned i = 0; i < 4; ++i) bytes.push_back(static_cast<unsigned char>(value >> (i * 8))); }
std::uint32_t get(const std::vector<unsigned char>& bytes, std::size_t offset)
{
    require(offset + 4 <= bytes.size(), "isolated job header truncated");
    std::uint32_t value = 0;
    for (unsigned i = 0; i < 4; ++i) value |= static_cast<std::uint32_t>(bytes[offset + i]) << (i * 8);
    return value;
}
void completeWrite(HANDLE file, const unsigned char* bytes, std::size_t size)
{
    for (std::size_t offset = 0; offset < size;) {
        DWORD written = 0;
        const auto count = static_cast<DWORD>((std::min)(size - offset, std::size_t{16384}));
        require(WriteFile(file, bytes + offset, count, &written, nullptr) && written && written <= count,
                "isolated complete file write failed");
        offset += written;
    }
    require(FlushFileBuffers(file) != FALSE, "isolated file flush failed");
}
void createExclusive(const std::filesystem::path& target, const unsigned char* data, std::size_t size)
{
    const auto parent = target.parent_path();
    auto directory = pinDirectory(parent);
    Handle file(CreateFileW(target.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_NEW,
                           FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OPEN_REPARSE_POINT, nullptr));
    require(file.get() != INVALID_HANDLE_VALUE, "isolated exclusive file create failed");
    require(GetFileType(file.get()) == FILE_TYPE_DISK, "isolated output is not a regular disk file");
    sameParent(directory.get(), file.get());
    completeWrite(file.get(), data, size);
    file.close(); // Successful return requires flush AND confirmed close; no relay before this point.
}
std::string json(std::u16string_view text)
{
    constexpr char hex[] = "0123456789abcdef";
    std::string out = "\"";
    for (char16_t c : text) {
        if (c >= 32 && c <= 126 && c != '"' && c != '\\') out += static_cast<char>(c);
        else if (c == '"' || c == '\\') { out += '\\'; out += static_cast<char>(c); }
        else {
            out += "\\u";
            for (int shift = 12; shift >= 0; shift -= 4) out += hex[(c >> shift) & 15];
        }
    }
    out += '"'; return out;
}
bool hash(const std::string& text)
{
    if (text.size() != 64) return false;
    for (char c : text) if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))) return false;
    return true;
}
void terminalObservation(const th::Observation& o)
{
    const auto& t = o.transcript;
    require(o.disposition == th::Disposition::CandidateProcessTerminal && o.stopReason == th::StopReason::None &&
        o.childCreated && o.resumed && o.childStopped && o.childPid && o.childStatus == th::terminalStatus &&
        !o.supervisorStopRequested && !o.preparationHeldUntilParentExit && t.profile == th::ProtocolProfile::RemeshV2 &&
        t.started && t.boundary && !t.returned && !t.backendFailure && !t.invalid && t.frames == 2 &&
        t.failureTag == 0 && t.save.has_value(), "isolated receipt lacks validated natural terminal observation");
    require(hash(o.sha256Before) && o.sha256Before == o.sha256After, "isolated worker image identity invalid");
    const auto& s = *t.save;
    constexpr auto required = th::snapshotPresent | th::snapshotProjectAttempted | th::snapshotProjectReturned |
        th::snapshotMstAttempted | th::snapshotMstReturned;
    const auto answer = [](std::uint32_t value) { return value == 0 || value == 0x4000 || value == 0x10000 || value == 0x400000; };
    require(!(s.flags & ~th::snapshotMask) && (s.flags & required) == required && answer(s.answer) && answer(s.pendingDefault),
            "isolated receipt SaveSnapshot invalid");
}
bool within(const std::filesystem::path& target, const std::filesystem::path& parent)
{
    auto p = parent.begin(), t = target.begin();
    for (; p != parent.end(); ++p, ++t) {
        if (t == target.end() || CompareStringOrdinal(p->c_str(), -1, t->c_str(), -1, TRUE) != CSTR_EQUAL) return false;
    }
    return true;
}
} // namespace

void writeIsolatedJob(const std::filesystem::path& leaf, const IsolatedJob& job)
{
    validate(job);
    const auto total = headerSize + 2 * (job.project.size() + job.output.size() + job.deformDir.size() + job.workingDirectory.size());
    std::vector<unsigned char> bytes(magic.begin(), magic.end());
    put(bytes, 1); put(bytes, static_cast<std::uint32_t>(total)); put(bytes, job.flags); put(bytes, job.codepage);
    put(bytes, static_cast<std::uint32_t>(job.project.size())); put(bytes, static_cast<std::uint32_t>(job.output.size()));
    put(bytes, static_cast<std::uint32_t>(job.deformDir.size())); put(bytes, static_cast<std::uint32_t>(job.workingDirectory.size()));
    put(bytes, static_cast<std::uint32_t>(job.nonce)); put(bytes, static_cast<std::uint32_t>(job.nonce >> 32)); put(bytes, 0);
    for (const auto* path : {&job.project, &job.output, &job.deformDir, &job.workingDirectory})
        for (char16_t c : *path) { bytes.push_back(static_cast<unsigned char>(c)); bytes.push_back(static_cast<unsigned char>(c >> 8)); }
    createExclusive(leaf / L"isolated-save.job", bytes.data(), bytes.size());
}
IsolatedJob readIsolatedJob(const std::filesystem::path& leaf)
{
    auto directory = pinDirectory(leaf);
    Handle file(CreateFileW((leaf / L"isolated-save.job").c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr,
        OPEN_EXISTING, FILE_FLAG_OPEN_REPARSE_POINT, nullptr));
    require(file.get() != INVALID_HANDLE_VALUE && GetFileType(file.get()) == FILE_TYPE_DISK, "isolated job open failed");
    sameParent(directory.get(), file.get());
    BY_HANDLE_FILE_INFORMATION info{}; LARGE_INTEGER size{};
    require(GetFileInformationByHandle(file.get(), &info) && !(info.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_REPARSE_POINT)) &&
            GetFileSizeEx(file.get(), &size) && size.QuadPart >= headerSize && size.QuadPart <= maxJob,
            "isolated job file type/size invalid");
    std::vector<unsigned char> bytes(static_cast<std::size_t>(size.QuadPart));
    for (std::size_t offset = 0; offset < bytes.size();) {
        DWORD got = 0;
        require(ReadFile(file.get(), bytes.data() + offset, static_cast<DWORD>(bytes.size() - offset), &got, nullptr) && got,
                "isolated job read incomplete");
        offset += got;
    }
    file.close();
    require(std::equal(magic.begin(), magic.end(), bytes.begin()) && get(bytes, 8) == 1 && get(bytes, 12) == bytes.size() &&
            get(bytes, 48) == 0, "isolated job magic/version/size/reserved invalid");
    const std::array<std::uint32_t, 4> lengths{get(bytes, 24), get(bytes, 28), get(bytes, 32), get(bytes, 36)};
    std::size_t expected = headerSize;
    for (auto length : lengths) { require(length <= maxPath, "isolated job path count invalid"); expected += length * 2; }
    require(expected == bytes.size(), "isolated job path payload/trailing bytes invalid");
    IsolatedJob job; job.flags = get(bytes, 16); job.codepage = get(bytes, 20);
    job.nonce = get(bytes, 40) | (static_cast<std::uint64_t>(get(bytes, 44)) << 32);
    std::size_t offset = headerSize, index = 0;
    for (auto* path : {&job.project, &job.output, &job.deformDir, &job.workingDirectory}) {
        for (std::uint32_t i = 0; i < lengths[index]; ++i, offset += 2)
            path->push_back(static_cast<char16_t>(bytes[offset] | (static_cast<unsigned>(bytes[offset + 1]) << 8)));
        ++index;
    }
    validate(job); return job;
}
void commitIsolatedTerminalReceipt(const std::filesystem::path& absoluteTarget,
                                   const th::Observation& observed, const IsolatedJob& job)
{
    validate(job); terminalObservation(observed); validatePath(absoluteTarget.u16string(), false);
    require(absoluteTarget.has_filename() && absoluteTarget.filename() != L"." && absoluteTarget.filename() != L"..",
            "isolated receipt filename invalid");
    const auto filename = absoluteTarget.filename().native();
    require(filename.back() != L'.' && filename.back() != L' ', "isolated receipt trailing alias rejected");
    localDirectory(absoluteTarget.parent_path());
    const auto target = std::filesystem::canonical(absoluteTarget.parent_path()) / absoluteTarget.filename();
    const auto projectTree = std::filesystem::weakly_canonical(std::filesystem::path(wide(job.project)).parent_path());
    require(!within(target, projectTree), "isolated receipt must be outside project tree");
    if (!job.output.empty())
        require(!within(target, std::filesystem::weakly_canonical(std::filesystem::path(wide(job.output)))),
                "isolated receipt aliases selected output");
    const auto& save = *observed.transcript.save;
    std::ostringstream out;
    out << "{\n  \"schema\":\"m2m-isolated-terminal-receipt-v1\",\n"
        << "  \"qualification\":\"candidate-fixed-endpoint;native-default-handler-prelude-unimplemented;no-native-Close-claim\",\n"
        << "  \"handler_model\":{\"evidence\":\"StaticFreshDefaultAssumption\",\"stage\":\"DefaultPrefixComplete\","
           "\"next\":\"CaptureContextUnimplemented\",\"native_execution_observed\":false},\n"
        << "  \"project\":" << json(job.project) << ",\n  \"output\":" << json(job.output)
        << ",\n  \"job_nonce\":" << job.nonce
        << ",\n  \"deform_dir\":" << json(job.deformDir) << ",\n  \"flags\":" << job.flags << ",\n  \"codepage\":" << job.codepage
        << ",\n  \"working_directory\":" << json(job.workingDirectory)
        << ",\n  \"worker\":{\"termination_observed\":true,\"pid\":" << observed.childPid
        << ",\"status\":3221226519,\"status_hex\":\"0xC0000417\",\"sha256_before\":\"" << observed.sha256Before
        << "\",\"sha256_after\":\"" << observed.sha256After << "\",\"elapsed_ms\":" << observed.elapsedMs << "},\n"
        << "  \"boundary\":{\"protocol\":\"RemeshV2\",\"site\":2,\"source\":\"0x1400abcf0\",\"errno\":22,\"ordinal\":1,"
           "\"started\":true,\"boundary\":true,\"returned\":false,\"backend_failure\":false,\"parent_stop_requested\":false},\n"
        << "  \"save\":{\"flags\":" << save.flags << ",\"checked_subprojects\":" << save.checkedSubprojects
        << ",\"answer\":" << save.answer << ",\"pending_default\":" << save.pendingDefault << "},\n"
        << "  \"parent_relay\":{\"requested\":true,\"status_hex\":\"0xC0000417\","
           "\"observation\":\"not_observed_in_this_receipt\"}\n}\n";
    const auto bytes = out.str(); require(bytes.size() <= 256 * 1024, "isolated receipt exceeds bound");
    createExclusive(target, reinterpret_cast<const unsigned char*>(bytes.data()), bytes.size());
}
} // namespace m2m::cli
