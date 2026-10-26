#include "../support/test_paths.hpp"
// Whole-folder check of convert with M2M_FLAG_WRITE_SAVE against the GUI's saves (stage 1's
// "byte-identical folder" rule). Each case copies an input project folder to a scratch folder,
// converts it there (open, Save steps 2 to 5, the .moproj included) and compares
// every file of the result with the folder DEF_GUI_MO_64 left after File > Save:
//   corpus: every oracle-A corpus project with a fresh GUI re-save, oracle_b/RS_<name>_before
//           against RS_<name> (the corpus folders themselves were saved by older versions);
//   oracle B: every <case>_before folder against <case>, and the unchanged re-saves R11, R19, R52
//           of corpus projects.
// The .moproj is compared like every other file; its view parts follow a fresh, maximised GUI, so
// the corpus projects and older saves made at other window sizes differ there (marked "view
// profile?" when only the .moproj differs); the DOE runs (*_B_DOE*) saved after GUI interaction
// with the study, so their view parts (ViewTransform, SfDispView) are masked, and a cycle or group
// item's Expanded is not compared (the one known deviation). The oracle runner's
// screenshot (.png) and
// view_state.json and the corpus's *.MST.oracleA are ignored. Files that differ only in the
// DEFORM version text of a banner line (a folder saved by another service pack) are counted apart.
// Report only: prints per case the number of files that differ, are missing or extra.
// Runs when M2M_FOLDER_SWEEP is set (it copies every project).
// ctest-labels: m2m-s
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <bcrypt.h>

#include <testkit/check.hpp>

#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <cstdint>
#include <cstdarg>
#include <io.h>
#include <fcntl.h>
#include <stdexcept>
#include <algorithm>
#include <climits>
#include <cwchar>
#include <deque>
#include <share.h>
#include <array>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <map>
#include <set>
#include <regex>
#include <string>
#include <vector>

#include "m2m/convert.hpp"

namespace fs = std::filesystem;

namespace {

std::string readAll(const fs::path& f)
{
    std::ifstream in(f, std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

std::string lower(std::string s)
{
    for (char& c : s)
        if (c >= 'A' && c <= 'Z') c = static_cast<char>(c + 32);
    return s;
}

// relative path (lower case, '/') -> file
std::map<std::string, fs::path> files(const fs::path& dir)
{
    std::map<std::string, fs::path> out;
    for (const auto& e : fs::recursive_directory_iterator(dir)) {
        if (!e.is_regular_file()) continue;
        // not written by the GUI's Save: the oracle runner's screenshot and view state, the
        // corpus's copy of the original MST
        if (e.path().extension() == ".png" || e.path().extension() == ".oracleA") continue;
        if (e.path().filename() == "view_state.json") continue;
        out[lower(fs::relative(e.path(), dir).generic_string())] = e.path();
    }
    return out;
}

std::vector<std::string> lines(const std::string& s)
{
    std::vector<std::string> v;
    std::size_t b = 0;
    for (;;) {
        const std::size_t e = s.find('\n', b);
        v.push_back(s.substr(b, e == std::string::npos ? std::string::npos : e - b));
        if (e == std::string::npos) break;
        b = e + 1;
    }
    return v;
}

// The files differ only in the DEFORM version text of banner lines ("*  DEFORM-3D V11.0 (Service
// Pack 1)   KEYWORD FILE (Qt)", the MDT's "DEFORM-2D/3D 11.0 ..."): the folder was saved by another
// installation than the one the conversion names.
bool bannerOnly(const std::string& a, const std::string& b)
{
    const auto la = lines(a), lb = lines(b);
    if (la.size() != lb.size()) return false;
    bool any = false;
    for (std::size_t i = 0; i < la.size(); ++i) {
        if (la[i] == lb[i]) continue;
        if (la[i].find("DEFORM-") == std::string::npos || lb[i].find("DEFORM-") == std::string::npos) return false;
        any = true;
    }
    return any;
}

// The .moproj as compared: a cycle or group item's Expanded blanked (the GUI writes an
// uninitialised byte there, the port "yes": Aleksandr 2026-10-22), and with `views` its view parts
// (ViewTransform, SfDispView) removed
std::string maskView(std::string s, bool views)
{
    static const std::regex groupExpanded(R"(Expanded="[a-z]*"( Operation_Id="[^"]*">\s*<ClassName>SfOpr(Cycle|Group)<))");
    s = std::regex_replace(s, groupExpanded, "$1");
    if (!views) return s;
    for (const char* tag : {"ViewTransform", "SfDispView"}) {
        const std::string open = std::string("<") + tag + ">", close = std::string("</") + tag + ">";
        for (std::size_t a = s.find(open); a != std::string::npos; a = s.find(open, a + 1)) {
            const std::size_t b = s.find(close, a);
            if (b == std::string::npos) break;
            s.erase(a, b + close.size() - a);
        }
    }
    return s;
}

// The GUI saved in the expected folder itself, so absolute paths it wrote (the MST's KFREAD
// <folder>/<name>.KEY, ...) name that folder; the conversion runs in a scratch copy. The expected
// bytes get the scratch folder in place of their own, in the forms the GUI writes ('/' and '\').
std::string relocated(std::string s, const fs::path& from, const fs::path& to)
{
    // whole path components only: "<case>" must not touch "<case>_before/..." paths the GUI
    // carried over from the input unchanged (103, 104 UserHT.KEY)
    auto replaceAll = [&s](const std::string& a, const std::string& b) {
        if (a.empty()) return;
        for (std::size_t i = s.find(a); i != std::string::npos;) {
            const std::size_t e = i + a.size();
            if (e < s.size() && s[e] != '/' && s[e] != '\\') {
                i = s.find(a, e);
                continue;
            }
            s.replace(i, a.size(), b);
            i = s.find(a, i + b.size());
        }
    };
    fs::path f = fs::absolute(from), t = fs::absolute(to);
    replaceAll(f.generic_string(), t.generic_string());
    replaceAll(f.make_preferred().string(), t.make_preferred().string());
    return s;
}

struct Tally {
    int cases = 0, identical = 0, refused = 0, files = 0, banner = 0, viewProfile = 0;
};
struct CaseOutcome {
    enum State { CopySkip = 1, Refused = 2, Compared = 3 } state = CopySkip;
    int code = 0;
    std::string message;
};

// Only the opted-in coordinator records events. The ordinary serial path still
// discovers, converts and prints each case immediately in its original order.
struct FolderEvent {
    enum Kind { Case, Summary, Text } kind = Text;
    fs::path input, expected, moproj, scratch;
    std::string name, output;
    Tally* section = nullptr;
    Tally delta;
    CaseOutcome outcome;
    unsigned ordinal = 0;
};
bool discovering = false;
std::vector<FolderEvent> folderEvents;
unsigned folderOrdinal = 0;
std::string folderWorkerIdentity = "serial";

fs::path rawReceiptRoot;
std::string rawReceiptMode;
struct RawCaseIdentity { unsigned ordinal; std::string name; fs::path expected, work; };
std::vector<RawCaseIdentity> rawCases;
struct RawDetails {
    std::vector<std::pair<std::string, std::string>> decisions;
    std::vector<std::string> beforeView, differ, missing, extra;
    bool expectedView = false, moprojDiffers = false;
    int banner = 0;
};

std::string utf8Path(const fs::path& path)
{
    const auto bytes = path.generic_u8string();
    return std::string(reinterpret_cast<const char*>(bytes.data()), bytes.size());
}
std::string jsonString(const std::string& value)
{
    static const char hex[] = "0123456789abcdef";
    std::string out = "\"";
    for (unsigned char c : value) {
        if (c == '"' || c == '\\') { out += '\\'; out += static_cast<char>(c); }
        else if (c < 32) { out += "\\u00"; out += hex[c >> 4]; out += hex[c & 15]; }
        else out += static_cast<char>(c);
    }
    return out + '"';
}

// Dynamically resolve the system SHA-256 API: no new test linker dependency.
struct RawShaApi {
    HMODULE library = nullptr;
    decltype(&BCryptOpenAlgorithmProvider) open = nullptr;
    decltype(&BCryptGetProperty) property = nullptr;
    decltype(&BCryptCreateHash) create = nullptr;
    decltype(&BCryptHashData) data = nullptr;
    decltype(&BCryptFinishHash) finish = nullptr;
    decltype(&BCryptDestroyHash) destroy = nullptr;
    decltype(&BCryptCloseAlgorithmProvider) close = nullptr;
    RawShaApi()
    {
        library = LoadLibraryExW(L"bcrypt.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
        if (!library) throw std::runtime_error("Cannot load system SHA-256 provider");
#define M2M_RAW_PROC(member, symbol) member = reinterpret_cast<decltype(member)>(GetProcAddress(library, #symbol))
        M2M_RAW_PROC(open, BCryptOpenAlgorithmProvider); M2M_RAW_PROC(property, BCryptGetProperty);
        M2M_RAW_PROC(create, BCryptCreateHash); M2M_RAW_PROC(data, BCryptHashData);
        M2M_RAW_PROC(finish, BCryptFinishHash); M2M_RAW_PROC(destroy, BCryptDestroyHash);
        M2M_RAW_PROC(close, BCryptCloseAlgorithmProvider);
#undef M2M_RAW_PROC
        if (!open || !property || !create || !data || !finish || !destroy || !close) {
            FreeLibrary(library); library = nullptr;
            throw std::runtime_error("Incomplete system SHA-256 provider");
        }
    }
    ~RawShaApi() { if (library) FreeLibrary(library); }
};

std::string sha256File(const fs::path& path, std::uintmax_t* size = nullptr)
{
    static RawShaApi api;
    BCRYPT_ALG_HANDLE algorithm = nullptr;
    BCRYPT_HASH_HANDLE hash = nullptr;
    std::vector<unsigned char> object;
    auto check = [](NTSTATUS status) { if (status < 0) throw std::runtime_error("SHA-256 operation failed"); };
    try {
        check(api.open(&algorithm, BCRYPT_SHA256_ALGORITHM, nullptr, 0));
        DWORD objectLength = 0, returned = 0;
        check(api.property(algorithm, BCRYPT_OBJECT_LENGTH, reinterpret_cast<PUCHAR>(&objectLength),
                           sizeof(objectLength), &returned, 0));
        if (returned != sizeof(objectLength) || objectLength > 1024 * 1024)
            throw std::runtime_error("Invalid SHA-256 object length");
        object.resize(objectLength);
        check(api.create(algorithm, &hash, object.data(), objectLength, nullptr, 0, 0));
        const auto expectedSize = fs::file_size(path);
        std::ifstream in(path, std::ios::binary);
        if (!in) throw std::runtime_error("Cannot read raw inventory file: " + utf8Path(path));
        std::array<unsigned char, 65536> buffer;
        std::uintmax_t bytes = 0;
        while (in) {
            in.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
            const auto count = in.gcount();
            if (count > 0) { check(api.data(hash, buffer.data(), static_cast<ULONG>(count), 0)); bytes += count; }
        }
        if (!in.eof() || bytes != expectedSize || fs::file_size(path) != expectedSize)
            throw std::runtime_error("Incomplete or changing raw inventory file: " + utf8Path(path));
        std::array<unsigned char, 32> digest;
        check(api.finish(hash, digest.data(), static_cast<ULONG>(digest.size()), 0));
        check(api.destroy(hash)); hash = nullptr;
        check(api.close(algorithm, 0)); algorithm = nullptr;
        static const char hex[] = "0123456789abcdef";
        std::string text;
        for (auto byte : digest) { text += hex[byte >> 4]; text += hex[byte & 15]; }
        if (size) *size = bytes;
        return text;
    } catch (...) {
        if (hash) api.destroy(hash);
        if (algorithm) api.close(algorithm, 0);
        throw;
    }
}

void rawVector(std::ostream& out, const std::vector<std::string>& values)
{
    out << '[';
    bool first = true;
    for (const auto& value : values) { if (!first) out << ','; first = false; out << jsonString(value); }
    out << ']';
}

void writeRawCase(const fs::path& expected, const fs::path& work, const std::string& name,
                  unsigned ordinal, const char* state, int code, const std::string& message,
                  const RawDetails& details = {})
{
    if (rawReceiptRoot.empty()) return;
    const auto temporary = rawReceiptRoot / (std::to_string(ordinal) + ".writing");
    const auto destination = rawReceiptRoot / (std::to_string(ordinal) + ".json");
    if (fs::exists(destination) || fs::exists(temporary)) throw std::runtime_error("Duplicate raw case receipt");
    std::ofstream out(temporary, std::ios::binary | std::ios::trunc);
    out << "{\"schema\":1,\"mode\":" << jsonString(rawReceiptMode)
        << ",\"prefetch\":" << (std::getenv("M2M_FOLDER_PREFETCH") &&
                                    std::string(std::getenv("M2M_FOLDER_PREFETCH")) == "1" ? "true" : "false")
        << ",\"case\":" << jsonString(name) << ",\"ordinal\":" << ordinal
        << ",\"writer_pid\":" << GetCurrentProcessId() << ",\"worker\":" << jsonString(folderWorkerIdentity)
        << ",\"expected\":" << jsonString(utf8Path(expected)) << ",\"work\":" << jsonString(utf8Path(work))
        << ",\"state\":" << jsonString(state) << ",\"converter_called\":"
        << (std::string(state) == "copy_failed" ? "false" : "true") << ",\"converter_code\":" << code
        << ",\"message\":" << jsonString(message) << ",\"work_exists\":" << (fs::exists(work) ? "true" : "false")
        << ",\"generated_files\":[";
    std::map<std::string, fs::path> inventory;
    if (fs::exists(work)) for (const auto& entry : fs::recursive_directory_iterator(work))
        if (entry.is_regular_file()) inventory.emplace(utf8Path(fs::relative(entry.path(), work)), entry.path());
    bool first = true;
    for (const auto& [relative, path] : inventory) {
        std::uintmax_t bytes = 0;
        const auto hash = sha256File(path, &bytes);
        if (!first) out << ','; first = false;
        out << "{\"relative\":" << jsonString(relative) << ",\"size\":" << bytes << ",\"sha256\":" << jsonString(hash) << '}';
    }
    out << "],\"decisions\":[";
    first = true;
    for (const auto& [relative, stage] : details.decisions) {
        if (!first) out << ','; first = false;
        out << "{\"relative_key\":" << jsonString(relative) << ",\"stage\":" << jsonString(stage) << '}';
    }
    out << "],\"differ_before_older_window\":"; rawVector(out, details.beforeView);
    out << ",\"differ\":"; rawVector(out, details.differ);
    out << ",\"missing\":"; rawVector(out, details.missing);
    out << ",\"extra\":"; rawVector(out, details.extra);
    out << ",\"expected_older_window\":" << (details.expectedView ? "true" : "false")
        << ",\"moproj_differs\":" << (details.moprojDiffers ? "true" : "false")
        << ",\"banner_only_files\":" << details.banner << ",\"complete\":true}\n";
    out.close();
    if (!out) throw std::runtime_error("Cannot finish raw case receipt");
    fs::rename(temporary, destination);
}

void beginRawRun(const fs::path& scratch, const std::string& mode)
{
    rawReceiptRoot.clear(); rawCases.clear(); rawReceiptMode = mode;
    if (!std::getenv("M2M_FOLDER_RAW_RECEIPT")) return;
    rawReceiptRoot = m2m_test::scratchOutput(scratch.parent_path() /
        ("m2m_folder_raw_" + std::to_string(GetCurrentProcessId()) + "_" + mode + "_" +
         std::to_string(std::chrono::steady_clock::now().time_since_epoch().count())));
    if (!fs::create_directory(rawReceiptRoot)) throw std::runtime_error("Raw receipt root already exists");
}

void finishRawRun()
{
    if (rawReceiptRoot.empty()) return;
    std::set<std::string> expectedFiles;
    for (const auto& entry : rawCases) expectedFiles.insert(std::to_string(entry.ordinal) + ".json");
    if (expectedFiles.size() != rawCases.size()) throw std::runtime_error("Duplicate raw expected ordinal");
    for (const auto& entry : fs::directory_iterator(rawReceiptRoot))
        if (!entry.is_regular_file() || !expectedFiles.erase(entry.path().filename().string()))
            throw std::runtime_error("Unexpected/incomplete raw receipt file");
    if (!expectedFiles.empty()) throw std::runtime_error("Missing raw case receipt");
    std::array<wchar_t, 32768> executable{};
    const auto length = GetModuleFileNameW(nullptr, executable.data(), static_cast<DWORD>(executable.size()));
    if (!length || length == executable.size()) throw std::runtime_error("Cannot identify raw receipt executable");
    const fs::path exe(std::wstring(executable.data(), length));
    const auto temporary = rawReceiptRoot / "complete.writing";
    std::ofstream out(temporary, std::ios::binary | std::ios::trunc);
    out << "{\"schema\":1,\"mode\":" << jsonString(rawReceiptMode) << ",\"coordinator_pid\":" << GetCurrentProcessId()
        << ",\"prefetch\":" << (std::getenv("M2M_FOLDER_PREFETCH") &&
                                    std::string(std::getenv("M2M_FOLDER_PREFETCH")) == "1" ? "true" : "false")
        << ",\"executable\":" << jsonString(utf8Path(exe)) << ",\"executable_sha256\":" << jsonString(sha256File(exe))
        << ",\"eligible_cases\":" << rawCases.size() << ",\"cases\":[";
    bool first = true;
    for (const auto& entry : rawCases) {
        if (!first) out << ','; first = false;
        const auto file = std::to_string(entry.ordinal) + ".json";
        out << "{\"ordinal\":" << entry.ordinal << ",\"case\":" << jsonString(entry.name)
            << ",\"receipt\":" << jsonString(file) << ",\"sha256\":" << jsonString(sha256File(rawReceiptRoot / file))
            << ",\"expected\":" << jsonString(utf8Path(entry.expected)) << ",\"work\":" << jsonString(utf8Path(entry.work)) << '}';
    }
    out << "],\"complete\":true}\n"; out.close();
    if (!out) throw std::runtime_error("Cannot finish raw run manifest");
    fs::rename(temporary, rawReceiptRoot / "complete.json");
}

// Investigation-only byte capture; never enabled by worker count or profiling.
// Keep evidence outside the sweep root, whose normal cleanup remains unchanged.
void capture107(const fs::path& input, const fs::path& expected, const fs::path& work,
                const std::string& name, unsigned ordinal)
{
    if (name != "B:107_B_HTFF" || !std::getenv("M2M_FOLDER_CAPTURE_107")) return;
    const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
    const auto directory = m2m_test::scratchOutput(m2m_test::scratchRoot() /
        ("m2m_folder_capture_" + std::to_string(GetCurrentProcessId()) + "_" + std::to_string(stamp)));
    if (!fs::create_directory(directory)) throw std::runtime_error("Folder capture directory already exists");
    const fs::path relative = fs::path("operations") / "task00001" / "simctrl.key";
    fs::copy_file(work / relative, directory / "generated-simctrl.key");
    fs::copy_file(expected / relative, directory / "expected-simctrl.key");
    std::ofstream receipt(directory / "capture.tsv", std::ios::binary | std::ios::trunc);
    receipt << "schema\tM2M_FOLDER_CAPTURE_107_1\ncase\t" << name
            << "\nworker\t" << folderWorkerIdentity << "\npid\t" << GetCurrentProcessId()
            << "\nordinal\t" << ordinal << "\ninput\t" << input.generic_string()
            << "\nexpected\t" << expected.generic_string() << "\nwork\t" << work.generic_string()
            << "\ngenerated_relative\t" << relative.generic_string() << '\n';
    receipt.close();
    if (!receipt) throw std::runtime_error("Cannot finish targeted folder capture receipt");
}

void capture98(const fs::path& input, const fs::path& expected, const fs::path& work,
               const std::string& name, unsigned ordinal)
{
    if (name != "B:98_B_DS3F" || !std::getenv("M2M_FOLDER_CAPTURE_98")) return;
    const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
    const auto directory = m2m_test::scratchOutput(m2m_test::scratchRoot() /
        ("m2m_folder_capture_" + std::to_string(GetCurrentProcessId()) + "_" + std::to_string(stamp)));
    if (!fs::create_directory(directory)) throw std::runtime_error("Folder capture directory already exists");
    const fs::path relative = "Gear_blank.MST";
    fs::copy_file(work / relative, directory / "generated-Gear_blank.MST");
    fs::copy_file(expected / relative, directory / "expected-Gear_blank.MST");
    std::ofstream receipt(directory / "capture.tsv", std::ios::binary | std::ios::trunc);
    receipt << "schema\tM2M_FOLDER_CAPTURE_98_1\ncase\t" << name
            << "\nworker\t" << folderWorkerIdentity << "\npid\t" << GetCurrentProcessId()
            << "\nordinal\t" << ordinal << "\ninput\t" << input.generic_string()
            << "\nexpected\t" << expected.generic_string() << "\nwork\t" << work.generic_string()
            << "\ngenerated_relative\t" << relative.generic_string() << '\n';
    receipt.close();
    if (!receipt) throw std::runtime_error("Cannot finish targeted MST capture receipt");
}

void diagnostic(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    if (!discovering) {
        std::vprintf(format, args);
    } else {
        va_list measure;
        va_copy(measure, args);
        const int size = std::vsnprintf(nullptr, 0, format, measure);
        va_end(measure);
        if (size < 0) { va_end(args); throw std::runtime_error("Folder diagnostic formatting failed"); }
        std::vector<char> buffer(static_cast<std::size_t>(size) + 1);
        std::vsnprintf(buffer.data(), buffer.size(), format, args);
        FolderEvent event;
        event.output.assign(buffer.data(), static_cast<std::size_t>(size));
        folderEvents.push_back(std::move(event));
    }
    va_end(args);
}

// Optional measurement only. Keep the ordinary case and summary rows byte-for-byte unchanged.
struct CaseProfile {
    std::int64_t copyUs = 0, convertUs = 0, compareUs = 0, cleanupUs = 0;
    std::uintmax_t copyFiles = 0, copyBytes = 0, comparedFiles = 0, readBytes = 0;
};

using Clock = std::chrono::steady_clock;

std::int64_t elapsedUs(Clock::time_point start)
{
    return std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - start).count();
}

void printProfile(const std::string& name, const char* status, const CaseProfile& p)
{
    // A separate fixed-column TSV row makes timings easy to collect without changing diagnostics.
    std::printf("M2M_FOLDER_PROFILE\t%s\t%s\t%lld\t%lld\t%lld\t%lld\t%llu\t%llu\t%llu\t%llu\n",
                name.c_str(), status, static_cast<long long>(p.copyUs), static_cast<long long>(p.convertUs),
                static_cast<long long>(p.compareUs), static_cast<long long>(p.cleanupUs),
                static_cast<unsigned long long>(p.copyFiles), static_cast<unsigned long long>(p.copyBytes),
                static_cast<unsigned long long>(p.comparedFiles), static_cast<unsigned long long>(p.readBytes));
}

struct CopyResult {
    bool failed = false;
    std::string message;
    CaseProfile profile;
};
bool prefetchActive = false;
fs::path prefetchMailbox;
std::string prefetchIdentity;
HANDLE prefetchProcess = nullptr;
CopyResult consumePrefetchedCopy(unsigned ordinal);

CopyResult copyCase(const fs::path& input, const fs::path& work, const fs::path& moproj, bool profiling)
{
    CopyResult result;
    CaseProfile& profile = result.profile;
    const auto copyStart = profiling ? Clock::now() : Clock::time_point{};
    std::error_code ec;
    fs::remove_all(work, ec);
    fs::copy(input, work, fs::copy_options::recursive, ec);
    if (profiling && !ec) {
        for (const auto& e : fs::recursive_directory_iterator(input)) {
            if (!e.is_regular_file()) continue;
            ++profile.copyFiles;
            std::error_code sizeError;
            const auto size = e.file_size(sizeError);
            if (!sizeError) profile.copyBytes += size;
        }
    }
    if (!ec && !fs::exists(work / moproj.filename())) ec = std::make_error_code(std::errc::no_such_file_or_directory);
    if (profiling) profile.copyUs = elapsedUs(copyStart);
    result.failed = static_cast<bool>(ec);
    if (ec) result.message = ec.message();
    return result;
}

// Saves whose .moproj view parts follow an older GUI window size (L7: fresh saves of the same
// projects match the port); a .moproj-only difference there is expected.
bool oldViewProfile(const std::string& name)
{
    return name == "B:R11" || name == "B:R19" || name == "B:82_R52_BEND_2D";
}

bool pipelinePosting = false;
bool pipelineQueued = false;
fs::path pipelineMailbox;
void publishPost(unsigned ordinal, bool compare, const CaseProfile& profile,
                 int conversionCode = M2M_OK, const std::string& conversionMessage = {});
void compareCase(const fs::path& expected, const fs::path& work, const std::string& name,
                 bool profiling, CaseProfile profile, Tally& t, CaseOutcome* outcome, unsigned ordinal,
                 int conversionCode, const std::string& conversionMessage)
{
    std::error_code ec;
    const bool rawReceipt = !rawReceiptRoot.empty();
    RawDetails rawDetails;
    auto decision = [&](const std::string& relative, const char* stage) {
        if (rawReceipt) rawDetails.decisions.emplace_back(relative, stage);
    };
    const auto compareStart = profiling ? Clock::now() : Clock::time_point{};
    if (outcome) outcome->state = CaseOutcome::Compared;
    const auto got = files(work), want = files(expected);
    std::vector<std::string> differ, missing, extra;
    bool moprojDiffers = false;
    int banner = 0;
    const bool doe = name.find("_B_DOE") != std::string::npos;   // saved after GUI interaction
    for (const auto& [rel, path] : want) {
        auto it = got.find(rel);
        if (it == got.end()) {
            missing.push_back(rel);
            decision(rel, "missing");
            continue;
        }
        // an absolute path the GUI carried over from the input names the expected folder too
        // (103/104: UserHT.KEY in the HTWZ and the MST) and stays as it is in the conversion, so
        // the file also passes unrelocated
        const std::string raw = readAll(path), g = readAll(it->second);
        if (profiling) {
            ++profile.comparedFiles;
            profile.readBytes += raw.size() + g.size();
        }
        if (g == raw) { decision(rel, "raw_equal"); continue; }
        const std::string w = relocated(raw, expected, work);
        if (g == w) { decision(rel, "relocated_equal"); continue; }
        if (fs::path(rel).extension() == ".moproj") {
            if (maskView(g, doe) == maskView(w, doe)) { decision(rel, "masked_equal"); continue; }
            moprojDiffers = true;
            differ.push_back(rel);
            decision(rel, "different");
        } else if (bannerOnly(g, w)) {
            ++banner;
            decision(rel, "banner_only");
        } else {
            differ.push_back(rel);
            decision(rel, "different");
        }
    }
    for (const auto& [rel, path] : got)
        if (!want.count(rel)) { extra.push_back(rel); decision(rel, "extra"); }
    if (rawReceipt) rawDetails.beforeView = differ;
    bool expectedView = false;
    if (oldViewProfile(name) && moprojDiffers && differ.size() == 1 && missing.empty() && extra.empty()) {
        differ.clear();
        expectedView = true;
        ++t.viewProfile;
    }
    const std::size_t n = differ.size() + missing.size() + extra.size();
    t.files += static_cast<int>(n);
    t.banner += banner;
    if (n == 0) ++t.identical;
    if (profiling) profile.compareUs = elapsedUs(compareStart);
    const std::string bannerNote = banner ? " (+ " + std::to_string(banner) + " with another version banner)" : "";
    std::printf("%s %s: %zu differ, %zu missing, %zu extra%s%s\n", n ? "DIFF" : "SAME", name.c_str(), differ.size(),
                missing.size(), extra.size(), expectedView                 ? " (+ .moproj view profile of an older window size, expected)"
                : moprojDiffers && n == 1 ? " (.moproj only: view profile?)"
                                          : "",
                bannerNote.c_str());
    auto list = [](const char* what, const std::vector<std::string>& v) {
        for (std::size_t i = 0; i < v.size() && i < 8; ++i) std::printf("   %s %s\n", what, v[i].c_str());
        if (v.size() > 8) std::printf("   %s ... %zu more\n", what, v.size() - 8);
    };
    list("differ ", differ);
    list("missing", missing);
    list("extra  ", extra);
    if (rawReceipt) {
        rawDetails.differ = differ; rawDetails.missing = missing; rawDetails.extra = extra;
        rawDetails.expectedView = expectedView; rawDetails.moprojDiffers = moprojDiffers; rawDetails.banner = banner;
        writeRawCase(expected, work, name, ordinal, "compared", conversionCode, conversionMessage, rawDetails);
    }
    const auto cleanupStart = profiling ? Clock::now() : Clock::time_point{};
    fs::remove_all(work, ec);
    if (profiling) {
        profile.cleanupUs = elapsedUs(cleanupStart);
        printProfile(name, "compared", profile);
    }
}

void runCase(const fs::path& input, const fs::path& expected, const std::string& name, const fs::path& scratch,
             Tally& t, unsigned assignedOrdinal = 0, const fs::path& selectedMoproj = {},
             CaseOutcome* outcome = nullptr)
{
    fs::path moproj = selectedMoproj;
    if (!assignedOrdinal) {
        for (const auto& e : fs::directory_iterator(input))
            if (e.path().extension() == ".moproj") moproj = e.path();
        if (moproj.empty() || !fs::exists(expected)) return;
    }
    if (discovering) {
        FolderEvent event;
        event.kind = FolderEvent::Case;
        event.input = input;
        event.expected = expected;
        event.moproj = moproj;
        event.scratch = scratch;
        event.name = name;
        event.section = &t;
        event.ordinal = ++folderOrdinal;
        folderEvents.push_back(std::move(event));
        return;
    }
    ++t.cases;
    // a folder of its own per case (names carry a ":"; a folder removed just before may still be
    // pending deletion on Windows, so none is reused)
    static int seq = 0;
    const unsigned workOrdinal = assignedOrdinal ? assignedOrdinal : static_cast<unsigned>(++seq);
    const fs::path work = scratch / std::to_string(workOrdinal);
    if (!rawReceiptRoot.empty()) rawCases.push_back({workOrdinal, name, expected, work});
    const bool profiling = std::getenv("M2M_FOLDER_PROFILE") != nullptr;
    const auto copied = prefetchActive ? consumePrefetchedCopy(workOrdinal) : copyCase(input, work, moproj, profiling);
    CaseProfile profile = copied.profile;
    std::error_code ec;
    if (copied.failed) {
        writeRawCase(expected, work, name, workOrdinal, "copy_failed", 0, copied.message);
        if (outcome) outcome->message = copied.message;
        std::printf("SKIP %s: copy failed (%s)\n", name.c_str(), copied.message.c_str());
        if (profiling) printProfile(name, "copy_failed", profile);
        return;
    }
    const auto convertStart = profiling ? Clock::now() : Clock::time_point{};
    const m2m::Result r = m2m::convert((work / moproj.filename()).u16string(), u"", M2M_FLAG_WRITE_SAVE, 1252);
    if (profiling) profile.convertUs = elapsedUs(convertStart);
    if (outcome) { outcome->code = r.code; outcome->message = r.message; }
    if (r.code != M2M_OK) {
        writeRawCase(expected, work, name, workOrdinal, "refused", r.code, r.message);
        if (outcome) {
            outcome->state = CaseOutcome::Refused;
            outcome->code = r.code;
            outcome->message = r.message;
        }
        ++t.refused;
        std::printf("REFUSED %s (%d): %s\n", name.c_str(), r.code, r.message.substr(0, 200).c_str());
        const auto cleanupStart = profiling ? Clock::now() : Clock::time_point{};
        fs::remove_all(work, ec);
        if (profiling) {
            profile.cleanupUs = elapsedUs(cleanupStart);
            printProfile(name, "refused", profile);
        }
        return;
    }
    capture107(input, expected, work, name, workOrdinal);
    capture98(input, expected, work, name, workOrdinal);
    if (pipelinePosting) {
        publishPost(workOrdinal, true, profile, r.code, r.message);
        pipelineQueued = true;
        return;
    }
    compareCase(expected, work, name, profiling, profile, t, outcome, workOrdinal, r.code, r.message);
}

void summary(const char* set, const Tally& t)
{
    if (discovering) {
        FolderEvent event;
        event.kind = FolderEvent::Summary;
        event.name = set;
        event.section = const_cast<Tally*>(&t);
        folderEvents.push_back(std::move(event));
        return;
    }
    std::printf("%s: %d cases, %d identical (%d with an expected older-window .moproj view profile), %d refused, "
                "%d file diffs, %d files with another version banner\n",
                set, t.cases, t.identical, t.viewProfile, t.refused, t.files, t.banner);
}

unsigned folderWorkers()
{
    const char* value = std::getenv("M2M_FOLDER_WORKERS");
    if (!value) return 1;
    unsigned count = 0;
    if (!*value) throw std::runtime_error("M2M_FOLDER_WORKERS must be an integer from 1 through 16");
    for (const char* p = value; *p; ++p) {
        if (*p < '0' || *p > '9' || count > 16)
            throw std::runtime_error("M2M_FOLDER_WORKERS must be an integer from 1 through 16");
        count = count * 10 + static_cast<unsigned>(*p - '0');
    }
    if (count < 1 || count > 16)
        throw std::runtime_error("M2M_FOLDER_WORKERS must be an integer from 1 through 16");
    return count;
}

// Length-framed private IPC. Exact manifest bytes are echoed by the child so a
// stale, partial or unrelated result cannot silently contribute to this sweep.
void putNumber(std::ostream& out, std::uint64_t value)
{
    out.write(reinterpret_cast<const char*>(&value), sizeof(value));
    if (!out) throw std::runtime_error("Folder IPC write failed");
}
std::uint64_t getNumber(std::istream& in)
{
    std::uint64_t value = 0;
    in.read(reinterpret_cast<char*>(&value), sizeof(value));
    if (!in) throw std::runtime_error("Truncated folder IPC");
    return value;
}
void putString(std::ostream& out, const std::string& value)
{
    putNumber(out, value.size());
    out.write(value.data(), static_cast<std::streamsize>(value.size()));
    if (!out) throw std::runtime_error("Folder IPC write failed");
}
std::string readHarnessFile(const fs::path& path)
{
    // Unlike the historical comparator helper, harness transport/capture reads
    // must fail closed: an unreadable capture must never become empty output.
    std::ifstream in(path, std::ios::binary | std::ios::ate);
    if (!in) throw std::runtime_error("Cannot open folder harness file: " + path.string());
    const auto end = in.tellg();
    if (end < 0 || end > 64 * 1024 * 1024)
        throw std::runtime_error("Invalid folder harness file size: " + path.string());
    std::string bytes(static_cast<std::size_t>(end), '\0');
    in.seekg(0);
    in.read(bytes.data(), static_cast<std::streamsize>(bytes.size()));
    if (!in || in.peek() != std::char_traits<char>::eof())
        throw std::runtime_error("Incomplete folder harness read: " + path.string());
    return bytes;
}
std::string getString(std::istream& in)
{
    const auto size = getNumber(in);
    if (size > 64 * 1024 * 1024) throw std::runtime_error("Oversized folder IPC field");
    std::string value(static_cast<std::size_t>(size), '\0');
    in.read(value.data(), static_cast<std::streamsize>(size));
    if (!in) throw std::runtime_error("Truncated folder IPC field");
    return value;
}
void requireEnd(std::istream& in)
{
    if (in.peek() != std::char_traits<char>::eof()) throw std::runtime_error("Unexpected folder IPC tail");
}
void putTally(std::ostream& out, const Tally& t)
{
    for (int value : {t.cases, t.identical, t.refused, t.files, t.banner, t.viewProfile}) putNumber(out, value);
}
Tally getTally(std::istream& in)
{
    Tally t;
    for (int* field : {&t.cases, &t.identical, &t.refused, &t.files, &t.banner, &t.viewProfile}) {
        const auto value = getNumber(in);
        if (value > INT_MAX) throw std::runtime_error("Invalid folder tally");
        *field = static_cast<int>(value);
    }
    if (t.cases != 1 || t.identical > 1 || t.refused > 1 || t.viewProfile > t.identical ||
        t.identical + t.refused > 1)
        throw std::runtime_error("Invalid folder case result");
    return t;
}
void addTally(Tally& to, const Tally& from)
{
    to.cases += from.cases; to.identical += from.identical; to.refused += from.refused;
    to.files += from.files; to.banner += from.banner; to.viewProfile += from.viewProfile;
}

bool bindStandardHandle(FILE* stream, DWORD standardHandle) noexcept
{
    const auto native = _get_osfhandle(_fileno(stream));
    if (native == -1 || native == -2) return false;
    return SetStdHandle(standardHandle, reinterpret_cast<HANDLE>(native)) != FALSE;
}

void putProfile(std::ostream& out, const CaseProfile& p)
{
    for (auto value : {static_cast<std::uint64_t>(p.copyUs), static_cast<std::uint64_t>(p.convertUs),
                      static_cast<std::uint64_t>(p.copyFiles), static_cast<std::uint64_t>(p.copyBytes)})
        putNumber(out, value);
}
CaseProfile getProfile(std::istream& in)
{
    CaseProfile p;
    const auto copy = getNumber(in), convert = getNumber(in);
    if (copy > INT64_MAX || convert > INT64_MAX) throw std::runtime_error("Invalid folder profile");
    p.copyUs = static_cast<std::int64_t>(copy);
    p.convertUs = static_cast<std::int64_t>(convert);
    p.copyFiles = getNumber(in); p.copyBytes = getNumber(in);
    return p;
}

void finishMailbox(std::ofstream& out, const fs::path& temporary, const fs::path& ready)
{
    out.close();
    if (!out) throw std::runtime_error("Cannot finish folder pipeline message");
    fs::rename(temporary, ready);
}

void publishPost(unsigned ordinal, bool compare, const CaseProfile& profile,
                 int conversionCode, const std::string& conversionMessage)
{
    const auto temporary = pipelineMailbox / (std::to_string(ordinal) + ".pending");
    const auto ready = pipelineMailbox / (std::to_string(ordinal) + ".ready");
    std::ofstream out(temporary, std::ios::binary | std::ios::trunc);
    putString(out, "M2M_FOLDER_POST_READY_2");
    putNumber(out, ordinal); putNumber(out, compare ? 1 : 0);
    putProfile(out, profile);
    putNumber(out, static_cast<std::uint32_t>(conversionCode)); putString(out, conversionMessage);
    finishMailbox(out, temporary, ready);
}

void waitMailbox(const fs::path& ready, HANDLE process = nullptr)
{
    const auto start = GetTickCount64();
    while (!fs::exists(ready)) {
        if (process && WaitForSingleObject(process, 0) != WAIT_TIMEOUT)
            throw std::runtime_error("Folder pipeline helper exited before result; inspect " + ready.parent_path().string());
        if (GetTickCount64() - start > 600000)
            throw std::runtime_error("Folder pipeline mailbox exceeded ten minutes: " + ready.string());
        Sleep(2);
    }
}

void requestPrefetch(unsigned ordinal)
{
    const auto temporary = prefetchMailbox / (std::to_string(ordinal) + ".requesting");
    const auto ready = prefetchMailbox / (std::to_string(ordinal) + ".ready");
    if (fs::exists(temporary) || fs::exists(ready)) throw std::runtime_error("Duplicate copy-prefetch request");
    std::ofstream out(temporary, std::ios::binary | std::ios::trunc);
    putString(out, "M2M_FOLDER_COPY_READY_1"); putString(out, prefetchIdentity); putNumber(out, ordinal);
    finishMailbox(out, temporary, ready);
}

CopyResult consumePrefetchedCopy(unsigned ordinal)
{
    const auto done = prefetchMailbox / (std::to_string(ordinal) + ".done");
    waitMailbox(done, prefetchProcess);
    std::ifstream result(done, std::ios::binary);
    if (getString(result) != "M2M_FOLDER_COPY_RESULT_1" || getString(result) != prefetchIdentity ||
        getNumber(result) != ordinal) throw std::runtime_error("Copy-prefetch result identity mismatch");
    const auto failed = getNumber(result);
    if (failed > 1) throw std::runtime_error("Invalid copy-prefetch terminal state");
    CopyResult copied;
    copied.failed = failed != 0;
    copied.message = getString(result); copied.profile = getProfile(result);
    if ((!copied.failed && !copied.message.empty()) || copied.profile.convertUs != 0)
        throw std::runtime_error("Invalid copy-prefetch result metadata");
    requireEnd(result);
    return copied;
}

int folderCopyWorker(const fs::path& manifest)
{
    try {
        FILE* errors = nullptr;
        if (_wfreopen_s(&errors, (manifest.parent_path() / "worker.stderr").c_str(), L"w", stderr) || !errors ||
            !bindStandardHandle(stderr, STD_ERROR_HANDLE))
            throw std::runtime_error("Cannot capture copy helper stderr");
        FILE* output = nullptr;
        if (_wfreopen_s(&output, (manifest.parent_path() / "worker.stdout").c_str(), L"w", stdout) || !output ||
            !bindStandardHandle(stdout, STD_OUTPUT_HANDLE))
            throw std::runtime_error("Cannot capture copy helper stdout");
        const auto identity = readHarnessFile(manifest);
        std::ifstream job(manifest, std::ios::binary);
        if (getString(job) != "M2M_FOLDER_COPY_JOB_1") throw std::runtime_error("Unknown copy job schema");
        const auto count = getNumber(job);
        if (count > 100000) throw std::runtime_error("Oversized copy job");
        std::vector<FolderEvent> cases;
        for (std::uint64_t i = 0; i < count; ++i) {
            FolderEvent event;
            const auto ordinal = getNumber(job);
            if (!ordinal || ordinal > UINT_MAX) throw std::runtime_error("Invalid copy ordinal");
            event.ordinal = static_cast<unsigned>(ordinal);
            event.input = fs::u8path(getString(job));
            event.scratch = m2m_test::scratchOutput(fs::u8path(getString(job))); // exact owned disposable case leaf
            event.moproj = fs::u8path(getString(job));
            cases.push_back(std::move(event));
        }
        requireEnd(job);
        for (const auto& event : cases) {
            const auto ready = manifest.parent_path() / (std::to_string(event.ordinal) + ".ready");
            waitMailbox(ready);
            std::ifstream request(ready, std::ios::binary);
            if (getString(request) != "M2M_FOLDER_COPY_READY_1" || getString(request) != identity ||
                getNumber(request) != event.ordinal) throw std::runtime_error("Copy request identity mismatch");
            requireEnd(request);
            // Filesystem-only: never call convert, model/KEY readers or cleanup
            // after an error. A partial failed copy survives until sweep cleanup.
            const auto copied = copyCase(event.input, event.scratch, event.moproj,
                                         std::getenv("M2M_FOLDER_PROFILE") != nullptr);
            const auto temporary = manifest.parent_path() / (std::to_string(event.ordinal) + ".writing");
            const auto done = manifest.parent_path() / (std::to_string(event.ordinal) + ".done");
            std::ofstream result(temporary, std::ios::binary | std::ios::trunc);
            putString(result, "M2M_FOLDER_COPY_RESULT_1"); putString(result, identity); putNumber(result, event.ordinal);
            putNumber(result, copied.failed ? 1 : 0); putString(result, copied.message); putProfile(result, copied.profile);
            finishMailbox(result, temporary, done);
        }
        return 0;
    } catch (const std::exception& e) {
        std::fprintf(stderr, "Copy helper infrastructure failure: %s\n", e.what());
        return 2;
    }
}

// One persistent post worker reads completed trees only. It never calls convert,
// model readers or engine initialization, and only removes its assigned case leaf.
int folderPostWorker(const fs::path& manifest)
{
    try {
        FILE* errors = nullptr;
        if (_wfreopen_s(&errors, (manifest.parent_path() / "worker.stderr").c_str(), L"w", stderr) || !errors)
            throw std::runtime_error("Cannot capture folder pipeline helper stderr");
        if (!bindStandardHandle(stderr, STD_ERROR_HANDLE))
            throw std::runtime_error("Cannot bind folder pipeline helper stderr handle");
        const auto identity = readHarnessFile(manifest);
        std::ifstream job(manifest, std::ios::binary);
        if (getString(job) != "M2M_FOLDER_POST_JOB_2") throw std::runtime_error("Unknown post job schema");
        rawReceiptRoot = fs::u8path(getString(job));
        if (!rawReceiptRoot.empty()) rawReceiptRoot = m2m_test::scratchOutput(rawReceiptRoot);
        rawReceiptMode = "pipeline"; folderWorkerIdentity = "pipeline-post";
        const auto count = getNumber(job);
        if (count > 100000) throw std::runtime_error("Oversized folder post job");
        std::vector<FolderEvent> cases;
        for (std::uint64_t i = 0; i < count; ++i) {
            FolderEvent event;
            const auto ordinal = getNumber(job);
            if (!ordinal || ordinal > UINT_MAX) throw std::runtime_error("Invalid post ordinal");
            event.ordinal = static_cast<unsigned>(ordinal);
            event.expected = fs::u8path(getString(job));
            event.scratch = fs::u8path(getString(job)); // exact completed case leaf
            event.name = getString(job);
            cases.push_back(std::move(event));
        }
        requireEnd(job);
        for (auto& event : cases) {
            const auto stem = manifest.parent_path() / std::to_string(event.ordinal);
            const auto ready = fs::path(stem.native() + L".ready");
            waitMailbox(ready);
            std::ifstream request(ready, std::ios::binary);
            if (getString(request) != "M2M_FOLDER_POST_READY_2" || getNumber(request) != event.ordinal)
                throw std::runtime_error("Post request identity mismatch");
            const auto compare = getNumber(request);
            if (compare > 1) throw std::runtime_error("Invalid folder post operation");
            const auto profile = getProfile(request);
            const auto conversionCode = getNumber(request);
            const auto conversionMessage = getString(request);
            if (conversionCode > UINT_MAX || (compare && conversionCode != M2M_OK))
                throw std::runtime_error("Invalid post converter status");
            requireEnd(request);
            if (compare) {
                FILE* capture = nullptr;
                const auto path = manifest.parent_path() / "case.stdout";
                if (_wfreopen_s(&capture, path.c_str(), L"w", stdout) || !capture)
                    throw std::runtime_error("Cannot capture folder post stdout");
                if (!bindStandardHandle(stdout, STD_OUTPUT_HANDLE))
                    throw std::runtime_error("Cannot bind folder post stdout capture handle");
                event.delta.cases = 1;
                compareCase(event.expected, event.scratch, event.name,
                            std::getenv("M2M_FOLDER_PROFILE") != nullptr, profile, event.delta, &event.outcome, event.ordinal,
                            static_cast<std::int32_t>(conversionCode), conversionMessage);
                if (std::fflush(stdout)) throw std::runtime_error("Cannot flush folder post stdout");
                FILE* sink = nullptr;
                if (_wfreopen_s(&sink, L"NUL", L"w", stdout) || !sink)
                    throw std::runtime_error("Cannot release folder post stdout capture");
                if (!bindStandardHandle(stdout, STD_OUTPUT_HANDLE))
                    throw std::runtime_error("Cannot bind released folder post stdout handle");
                event.output = readHarnessFile(path);
            }
            const auto temporary = fs::path(stem.native() + L".writing");
            const auto done = fs::path(stem.native() + L".done");
            std::ofstream result(temporary, std::ios::binary | std::ios::trunc);
            putString(result, "M2M_FOLDER_POST_RESULT_1"); putString(result, identity);
            putNumber(result, event.ordinal); putNumber(result, compare);
            if (compare) { putTally(result, event.delta); putString(result, event.output); }
            finishMailbox(result, temporary, done);
        }
        return 0;
    } catch (const std::exception& e) {
        std::fprintf(stderr, "Folder post helper infrastructure failure: %s\n", e.what());
        return 2;
    }
}

int folderWorker(const fs::path& manifest)
{
    try {
        folderWorkerIdentity = manifest.parent_path().filename().string();
        FILE* errorStream = nullptr;
        if (_wfreopen_s(&errorStream, (manifest.parent_path() / "worker.stderr").c_str(), L"wb", stderr) != 0 ||
            !errorStream) throw std::runtime_error("Cannot capture folder worker stderr");
        SetStdHandle(STD_ERROR_HANDLE, reinterpret_cast<HANDLE>(_get_osfhandle(_fileno(stderr))));
        std::ifstream job(manifest, std::ios::binary);
        const std::string identity = readHarnessFile(manifest);
        if (getString(job) != "M2M_FOLDER_JOB_1") throw std::runtime_error("Unknown folder job schema");
        const auto count = getNumber(job);
        if (count > 100000) throw std::runtime_error("Oversized folder job");
        std::vector<FolderEvent> cases;
        for (std::uint64_t i = 0; i < count; ++i) {
            FolderEvent event;
            const auto ordinal = getNumber(job);
            if (!ordinal || ordinal > UINT_MAX) throw std::runtime_error("Invalid folder ordinal");
            event.ordinal = static_cast<unsigned>(ordinal);
            event.input = fs::u8path(getString(job));
            event.expected = fs::u8path(getString(job));
            event.moproj = fs::u8path(getString(job));
            event.scratch = fs::u8path(getString(job));
            event.name = getString(job);
            cases.push_back(std::move(event));
        }
        requireEnd(job);
        std::ofstream result(manifest.parent_path() / "result.bin", std::ios::binary | std::ios::trunc);
        putString(result, "M2M_FOLDER_RESULT_1");
        putString(result, identity);
        putNumber(result, cases.size());
        for (auto& event : cases) {
            // Redirect the CRT descriptor and the Win32 handle: converter output,
            // including Fortran/C writes, is replayed at this case's event position.
            const fs::path capture = manifest.parent_path() / "case.stdout";
            FILE* stream = nullptr;
            if (_wfreopen_s(&stream, capture.c_str(), L"w", stdout) != 0 || !stream)
                throw std::runtime_error("Cannot capture folder worker stdout");
            SetStdHandle(STD_OUTPUT_HANDLE, reinterpret_cast<HANDLE>(_get_osfhandle(_fileno(stdout))));
            runCase(event.input, event.expected, event.name, event.scratch,
                    event.delta, event.ordinal, event.moproj, &event.outcome);
            if (std::fflush(stdout) != 0) throw std::runtime_error("Cannot flush folder worker stdout");
            // MSVC freopen holds the capture exclusively. Release it before
            // opening the file for the strict transport read; the next case
            // redirects stdout to its own capture again.
            FILE* sink = nullptr;
            if (_wfreopen_s(&sink, L"NUL", L"w", stdout) != 0 || !sink)
                throw std::runtime_error("Cannot release folder worker stdout capture");
            SetStdHandle(STD_OUTPUT_HANDLE, reinterpret_cast<HANDLE>(_get_osfhandle(_fileno(stdout))));
            event.output = readHarnessFile(capture);
            putNumber(result, event.ordinal);
            putTally(result, event.delta);
            putNumber(result, event.outcome.state);
            putNumber(result, static_cast<std::uint32_t>(event.outcome.code));
            putString(result, event.outcome.message);
            putString(result, event.output);
        }
        result.close();
        if (!result) throw std::runtime_error("Cannot finish folder worker result");
        return 0;
    } catch (const std::exception& e) {
        std::fprintf(stderr, "Folder worker infrastructure failure: %s\n", e.what());
        return 2;
    }
}

std::wstring quotedArgument(const std::wstring& value)
{
    std::wstring out = L"\"";
    unsigned slashes = 0;
    for (wchar_t c : value) {
        if (c == L'\\') { ++slashes; continue; }
        out.append(c == L'\"' ? 2 * slashes + 1 : slashes, L'\\');
        out += c;
        slashes = 0;
    }
    out.append(2 * slashes, L'\\');
    return out + L"\"";
}

std::vector<wchar_t> workerEnvironment(const fs::path& directory)
{
    wchar_t* inherited = GetEnvironmentStringsW();
    if (!inherited) throw std::runtime_error("Cannot read folder worker environment");
    std::vector<std::wstring> entries;
    for (const wchar_t* p = inherited; *p; p += std::wcslen(p) + 1) {
        const std::wstring entry(p);
        const auto equal = entry.find(L'=', entry[0] == L'=' ? 1 : 0);
        const auto name = entry.substr(0, equal);
        if (_wcsicmp(name.c_str(), L"TEMP") && _wcsicmp(name.c_str(), L"TMP") &&
            _wcsicmp(name.c_str(), L"TMPDIR")) entries.push_back(entry);
    }
    FreeEnvironmentStringsW(inherited);
    for (const wchar_t* name : {L"TEMP", L"TMP", L"TMPDIR"}) {
        const auto path = directory / name;
        fs::create_directories(path);
        entries.push_back(std::wstring(name) + L"=" + path.native());
    }
    std::sort(entries.begin(), entries.end(), [](const std::wstring& a, const std::wstring& b) {
        return _wcsicmp(a.c_str(), b.c_str()) < 0;
    });
    std::vector<wchar_t> block;
    for (const auto& entry : entries) {
        block.insert(block.end(), entry.begin(), entry.end()); block.push_back(0);
    }
    block.push_back(0);
    return block;
}

struct FolderChild {
    HANDLE process = nullptr;
    fs::path directory;
    std::string identity;
    std::vector<std::size_t> events;
    FolderChild() = default;
    FolderChild(const FolderChild&) = delete;
    FolderChild& operator=(const FolderChild&) = delete;
    ~FolderChild()
    {
        if (process) {
            // On any coordinator failure, stop and reap owned children before
            // allowing scratch cleanup. No unrelated process is ever attached.
            if (WaitForSingleObject(process, 0) == WAIT_TIMEOUT) TerminateProcess(process, 2);
            WaitForSingleObject(process, INFINITE);
            CloseHandle(process);
        }
    }
};

class FolderCrtCapture {
public:
    FolderCrtCapture(FILE* stream, DWORD standardHandle, const fs::path& path)
        : stream_(stream), handle_(standardHandle), path_(path)
    {
        if (std::fflush(stream_) != 0) throw std::runtime_error("Cannot flush pipeline capture stream");
        saved_ = _dup(_fileno(stream_));
        if (saved_ < 0) throw std::runtime_error("Cannot duplicate pipeline capture descriptor");
        // The coordinator reads flushed slices while the descriptor remains
        // stable across conversions, so its capture explicitly permits sharing.
        FILE* capture = _wfsopen(path_.c_str(), L"w", _SH_DENYNO);
        if (!capture) {
            _close(saved_); saved_ = -1;
            throw std::runtime_error("Cannot open pipeline capture");
        }
        const int result = _dup2(_fileno(capture), _fileno(stream_));
        std::fclose(capture);
        if (result != 0) {
            _close(saved_); saved_ = -1;
            throw std::runtime_error("Cannot redirect pipeline capture");
        }
        if (!bindStandardHandle(stream_, handle_)) {
            const bool rolledBack = restore();
            // A throwing constructor has no destructor. Make the best checked
            // rollback before reporting the capture setup as a harness failure.
            if (saved_ >= 0) { _close(saved_); saved_ = -1; }
            throw std::runtime_error(rolledBack ? "Cannot bind pipeline capture standard handle"
                                               : "Cannot bind or restore pipeline capture standard handle");
        }
    }
    FolderCrtCapture(const FolderCrtCapture&) = delete;
    FolderCrtCapture& operator=(const FolderCrtCapture&) = delete;
    ~FolderCrtCapture() { restore(); }
    std::string next()
    {
        if (std::fflush(stream_) != 0) throw std::runtime_error("Cannot flush pipeline case capture");
        const auto bytes = readHarnessFile(path_);
        if (bytes.size() < consumed_) throw std::runtime_error("Pipeline capture was truncated");
        const auto part = bytes.substr(consumed_);
        consumed_ = bytes.size();
        return part;
    }
    bool restore() noexcept
    {
        if (saved_ < 0) return true;
        const bool flushed = std::fflush(stream_) == 0;
        const bool restored = _dup2(saved_, _fileno(stream_)) == 0;
        const bool bound = restored && bindStandardHandle(stream_, handle_);
        if (bound) {
            _close(saved_); saved_ = -1;
        }
        // Retain the saved descriptor after failure so the destructor can retry.
        return flushed && restored && bound;
    }
private:
    FILE* stream_;
    DWORD handle_;
    fs::path path_;
    int saved_ = -1;
    std::size_t consumed_ = 0;
};

void runPipeline(const fs::path& scratch, bool prefetch)
{
    const auto ipc = m2m_test::scratchOutput(scratch.parent_path() /
        ("m2m_folder_pipeline_" + std::to_string(GetCurrentProcessId())));
    fs::remove_all(ipc); fs::create_directories(ipc);
    FolderChild child;
    child.directory = ipc;
    const auto manifest = ipc / "job.bin";
    for (std::size_t k = 0; k < folderEvents.size(); ++k)
        if (folderEvents[k].kind == FolderEvent::Case) child.events.push_back(k);
    {
        std::ofstream job(manifest, std::ios::binary | std::ios::trunc);
        putString(job, "M2M_FOLDER_POST_JOB_2"); putString(job, utf8Path(rawReceiptRoot));
        putNumber(job, child.events.size());
        for (auto k : child.events) {
            const auto& event = folderEvents[k];
            putNumber(job, event.ordinal);
            for (const auto& path : {event.expected, event.scratch / std::to_string(event.ordinal)}) {
                const auto utf8 = path.u8string();
                putString(job, std::string(reinterpret_cast<const char*>(utf8.data()), utf8.size()));
            }
            putString(job, event.name);
        }
        job.close();
        if (!job) throw std::runtime_error("Cannot finish folder pipeline manifest");
    }
    child.identity = readHarnessFile(manifest);
    std::vector<wchar_t> exe(32768);
    const auto length = GetModuleFileNameW(nullptr, exe.data(), static_cast<DWORD>(exe.size()));
    if (!length || length == exe.size()) throw std::runtime_error("Cannot resolve pipeline executable");
    const std::wstring executable(exe.data(), length);
    auto environment = workerEnvironment(ipc / "helper-temp");
    std::wstring command = quotedArgument(executable) + L" --folder-post-worker " + quotedArgument(manifest.native());
    STARTUPINFOW startup{}; startup.cb = sizeof(startup);
    PROCESS_INFORMATION process{};
    if (!CreateProcessW(executable.c_str(), command.data(), nullptr, nullptr, FALSE,
                        CREATE_UNICODE_ENVIRONMENT | CREATE_NO_WINDOW, environment.data(),
                        nullptr, &startup, &process))
        throw std::runtime_error("Cannot launch folder post helper: " + std::to_string(GetLastError()));
    child.process = process.hProcess;
    CloseHandle(process.hThread);
    FolderChild copyChild;

    struct PipelineReset {
        std::string previousWorker = folderWorkerIdentity;
        ~PipelineReset() {
            pipelinePosting = false; pipelineMailbox.clear(); folderWorkerIdentity = previousWorker;
            prefetchActive = false; prefetchMailbox.clear(); prefetchIdentity.clear(); prefetchProcess = nullptr;
        }
    } reset;
    pipelineMailbox = ipc;
    folderWorkerIdentity = "pipeline-coordinator";
    if (prefetch) {
        copyChild.directory = ipc / "prefetch";
        fs::create_directories(copyChild.directory);
        const auto copyManifest = copyChild.directory / "job.bin";
        {
            std::ofstream job(copyManifest, std::ios::binary | std::ios::trunc);
            putString(job, "M2M_FOLDER_COPY_JOB_1"); putNumber(job, child.events.size());
            for (auto k : child.events) {
                const auto& event = folderEvents[k];
                putNumber(job, event.ordinal);
                for (const auto& path : {event.input, event.scratch / std::to_string(event.ordinal), event.moproj})
                    putString(job, utf8Path(path));
            }
            job.close();
            if (!job) throw std::runtime_error("Cannot finish copy-prefetch manifest");
        }
        copyChild.identity = readHarnessFile(copyManifest);
        auto copyEnvironment = workerEnvironment(copyChild.directory / "helper-temp");
        std::wstring copyCommand = quotedArgument(executable) + L" --folder-copy-worker " + quotedArgument(copyManifest.native());
        PROCESS_INFORMATION copyProcess{};
        if (!CreateProcessW(executable.c_str(), copyCommand.data(), nullptr, nullptr, FALSE,
                            CREATE_UNICODE_ENVIRONMENT | CREATE_NO_WINDOW, copyEnvironment.data(),
                            nullptr, &startup, &copyProcess))
            throw std::runtime_error("Cannot launch copy-prefetch helper: " + std::to_string(GetLastError()));
        copyChild.process = copyProcess.hProcess; CloseHandle(copyProcess.hThread);
        prefetchMailbox = copyChild.directory; prefetchIdentity = copyChild.identity;
        prefetchProcess = copyChild.process; prefetchActive = true;
        if (!child.events.empty()) requestPrefetch(folderEvents[child.events.front()].ordinal);
    }
    auto collect = [&](std::size_t k) {
        auto& event = folderEvents[k];
        const auto done = ipc / (std::to_string(event.ordinal) + ".done");
        waitMailbox(done, child.process);
        std::ifstream result(done, std::ios::binary);
        if (getString(result) != "M2M_FOLDER_POST_RESULT_1" || getString(result) != child.identity ||
            getNumber(result) != event.ordinal) throw std::runtime_error("Folder post result identity mismatch");
        const auto compared = getNumber(result);
        if (compared > 1 || static_cast<bool>(compared) != (event.outcome.state == CaseOutcome::Compared))
            throw std::runtime_error("Folder post terminal state mismatch");
        if (compared) {
            event.delta = getTally(result);
            if (event.delta.refused) throw std::runtime_error("Comparator returned a converter refusal");
            event.output += getString(result);
        }
        requireEnd(result);
    };
    {
        // One stable descriptor per stream across all conversions. Helpers never
        // write these streams; every case's bytes are retained for ordered replay.
        FolderCrtCapture output(stdout, STD_OUTPUT_HANDLE, ipc / "coordinator.stdout");
        FolderCrtCapture errors(stderr, STD_ERROR_HANDLE, ipc / "coordinator.stderr");
        std::deque<std::size_t> pending;
        for (std::size_t index = 0; index < child.events.size(); ++index) {
            if (pending.size() >= 2) { collect(pending.front()); pending.pop_front(); }
            const auto k = child.events[index];
            auto& event = folderEvents[k];
            // Current was requested in the preceding iteration (or primed once).
            // No request beyond its single successor exists while it converts.
            if (prefetch && index + 1 < child.events.size())
                requestPrefetch(folderEvents[child.events[index + 1]].ordinal);
            pipelinePosting = true; pipelineQueued = false;
            runCase(event.input, event.expected, event.name, event.scratch, event.delta,
                    event.ordinal, event.moproj, &event.outcome);
            pipelinePosting = false;
            event.output = output.next();
            if (!errors.next().empty())
                throw std::runtime_error("Serial converter emitted stderr; inspect " + (ipc / "coordinator.stderr").string());
            if (pipelineQueued) event.outcome.state = CaseOutcome::Compared;
            else publishPost(event.ordinal, false, {});
            pending.push_back(k);
        }
        while (!pending.empty()) { collect(pending.front()); pending.pop_front(); }
        if (!errors.restore() || !output.restore()) throw std::runtime_error("Cannot restore pipeline capture descriptors");
    }
    if (WaitForSingleObject(child.process, 600000) != WAIT_OBJECT_0)
        throw std::runtime_error("Folder post helper wait failed or timed out");
    DWORD status = 0;
    if (!GetExitCodeProcess(child.process, &status) || status != 0)
        throw std::runtime_error("Folder post helper failed; inspect " + (ipc / "worker.stderr").string());
    if (!readHarnessFile(ipc / "worker.stderr").empty())
        throw std::runtime_error("Folder post helper emitted stderr; inspect " + (ipc / "worker.stderr").string());
    if (prefetch) {
        if (WaitForSingleObject(copyChild.process, 600000) != WAIT_OBJECT_0)
            throw std::runtime_error("Copy-prefetch helper wait failed or timed out");
        DWORD copyStatus = 0;
        if (!GetExitCodeProcess(copyChild.process, &copyStatus) || copyStatus != 0)
            throw std::runtime_error("Copy-prefetch helper failed; inspect " + copyChild.directory.string());
        if (!readHarnessFile(copyChild.directory / "worker.stderr").empty() ||
            !readHarnessFile(copyChild.directory / "worker.stdout").empty())
            throw std::runtime_error("Copy-prefetch helper emitted unframed diagnostics; inspect " + copyChild.directory.string());
    }
    for (auto& event : folderEvents) {
        if (event.kind == FolderEvent::Case) addTally(*event.section, event.delta);
        if (event.kind == FolderEvent::Summary) summary(event.name.c_str(), *event.section);
        else {
            std::fflush(stdout);
            const int previousMode = event.kind == FolderEvent::Case ? _setmode(_fileno(stdout), _O_BINARY) : -1;
            const auto written = std::fwrite(event.output.data(), 1, event.output.size(), stdout);
            const auto flushed = std::fflush(stdout);
            if (previousMode != -1) _setmode(_fileno(stdout), previousMode);
            if (written != event.output.size() || flushed) throw std::runtime_error("Cannot replay pipeline output");
        }
    }
    fs::remove_all(ipc);
}

void runShards(unsigned count, const fs::path& scratch)
{
    // A separate child IPC root never enters any compared case leaf.
    const auto ipc = m2m_test::scratchOutput(scratch.parent_path() /
        ("m2m_folder_workers_" + std::to_string(GetCurrentProcessId())));
    fs::remove_all(ipc);
    fs::create_directories(ipc);
    std::vector<wchar_t> exe(32768);
    const DWORD length = GetModuleFileNameW(nullptr, exe.data(), static_cast<DWORD>(exe.size()));
    if (!length || length == exe.size()) throw std::runtime_error("Cannot resolve folder worker executable");
    const std::wstring executable(exe.data(), length);
    std::vector<FolderChild> children(count);
    for (unsigned i = 0; i < count; ++i) {
        auto& child = children[i];
        child.directory = ipc / ("w" + std::to_string(i));
        fs::create_directories(child.directory);
        for (std::size_t k = 0; k < folderEvents.size(); ++k) {
            const auto& event = folderEvents[k];
            if (event.kind == FolderEvent::Case && (event.ordinal - 1) % count == i)
                child.events.push_back(k);
        }
        const auto manifest = child.directory / "job.bin";
        {
            std::ofstream job(manifest, std::ios::binary | std::ios::trunc);
            putString(job, "M2M_FOLDER_JOB_1");
            putNumber(job, child.events.size());
            for (auto k : child.events) {
                const auto& event = folderEvents[k];
                putNumber(job, event.ordinal);
                for (const auto& path : {event.input, event.expected, event.moproj, event.scratch}) {
                    const auto utf8 = path.u8string();
                    putString(job, std::string(reinterpret_cast<const char*>(utf8.data()), utf8.size()));
                }
                putString(job, event.name);
            }
            job.close();
            if (!job) throw std::runtime_error("Cannot finish folder job");
        }
        child.identity = readHarnessFile(manifest);
        auto environment = workerEnvironment(child.directory);
        std::wstring command = quotedArgument(executable) + L" --folder-worker " + quotedArgument(manifest.native());
        STARTUPINFOW startup{};
        startup.cb = sizeof(startup);
        PROCESS_INFORMATION process{};
        if (!CreateProcessW(executable.c_str(), command.data(), nullptr, nullptr, FALSE,
                            CREATE_UNICODE_ENVIRONMENT | CREATE_NO_WINDOW, environment.data(),
                            nullptr, &startup, &process))
            throw std::runtime_error("Cannot launch folder worker: " + std::to_string(GetLastError()));
        child.process = process.hProcess;
        CloseHandle(process.hThread);
    }
    for (auto& child : children) {
        // Bounded infrastructure timeout, independent of converter refusal semantics.
        if (WaitForSingleObject(child.process, 600000) != WAIT_OBJECT_0)
            throw std::runtime_error("Folder worker wait failed or exceeded ten minutes");
        DWORD status = 0;
        if (!GetExitCodeProcess(child.process, &status) || status != 0)
            throw std::runtime_error("Folder worker exited unsuccessfully; inspect " +
                                     (child.directory / "worker.stderr").string());
        // Until stderr can be replayed with the original case boundaries, fail
        // closed instead of silently dropping successful converter diagnostics.
        if (!readHarnessFile(child.directory / "worker.stderr").empty())
            throw std::runtime_error("Folder worker emitted stderr; inspect " +
                                     (child.directory / "worker.stderr").string());
        std::ifstream result(child.directory / "result.bin", std::ios::binary);
        if (getString(result) != "M2M_FOLDER_RESULT_1" || getString(result) != child.identity ||
            getNumber(result) != child.events.size()) throw std::runtime_error("Folder result identity mismatch");
        for (auto k : child.events) {
            auto& event = folderEvents[k];
            if (getNumber(result) != event.ordinal) throw std::runtime_error("Folder result ordinal mismatch");
            event.delta = getTally(result);
            const auto state = getNumber(result), code = getNumber(result);
            if (state < CaseOutcome::CopySkip || state > CaseOutcome::Compared || code > UINT_MAX ||
                (state == CaseOutcome::Refused) != (event.delta.refused == 1) ||
                (state == CaseOutcome::CopySkip && (event.delta.identical || event.delta.files || event.delta.banner)))
                throw std::runtime_error("Invalid folder terminal state");
            event.outcome.state = static_cast<CaseOutcome::State>(state);
            event.outcome.code = static_cast<std::int32_t>(code);
            event.outcome.message = getString(result);
            event.output = getString(result);
        }
        requireEnd(result);
    }
    // Validate every child before printing or summing anything: no partial sweep passes.
    for (auto& event : folderEvents) {
        if (event.kind == FolderEvent::Case) addTally(*event.section, event.delta);
        if (event.kind == FolderEvent::Summary) summary(event.name.c_str(), *event.section);
        else {
            // Case captures already contain CRT newline translation (and any
            // native engine bytes); replay them without translating a second time.
            std::fflush(stdout);
            const int previousMode = event.kind == FolderEvent::Case ? _setmode(_fileno(stdout), _O_BINARY) : -1;
            const auto written = std::fwrite(event.output.data(), 1, event.output.size(), stdout);
            const int flushStatus = std::fflush(stdout);
            if (previousMode != -1) _setmode(_fileno(stdout), previousMode);
            if (written != event.output.size() || flushStatus != 0)
                throw std::runtime_error("Cannot replay folder diagnostic");
        }
    }
    fs::remove_all(ipc);
}

}  // namespace

TEST_CASE(save_folder_sweep)
{
    const char* corpus = std::getenv("M2M_CORPUS");
    if (!std::getenv("M2M_FOLDER_SWEEP") || !corpus) return;
    const unsigned workers = folderWorkers();
    const char* pipelineSetting = std::getenv("M2M_FOLDER_PIPELINE");
    if (pipelineSetting && std::string(pipelineSetting) != "0" && std::string(pipelineSetting) != "1")
        throw std::runtime_error("M2M_FOLDER_PIPELINE must be 0 or 1");
    const bool pipeline = pipelineSetting && std::string(pipelineSetting) == "1";
    const char* prefetchSetting = std::getenv("M2M_FOLDER_PREFETCH");
    if (prefetchSetting && std::string(prefetchSetting) != "0" && std::string(prefetchSetting) != "1")
        throw std::runtime_error("M2M_FOLDER_PREFETCH must be 0 or 1");
    const bool prefetch = prefetchSetting && std::string(prefetchSetting) == "1";
    if (prefetch && !pipeline) throw std::runtime_error("M2M_FOLDER_PREFETCH=1 requires M2M_FOLDER_PIPELINE=1");
    if (pipeline && workers > 1) throw std::runtime_error("Folder pipeline and conversion sharding cannot be combined");
    if (workers > 1 && std::getenv("M2M_FOLDER_RAW_RECEIPT"))
        throw std::runtime_error("Raw folder receipts support serial and pipeline modes; disable conversion sharding");
    folderEvents.clear();
    folderOrdinal = 0;
    discovering = workers > 1 || pipeline;
    struct DiscoveryReset {
        ~DiscoveryReset() { discovering = false; folderEvents.clear(); rawReceiptRoot.clear(); rawCases.clear(); }
    } discoveryReset;
    const fs::path c = corpus;
    const fs::path b = m2m_test::configuredPath("M2M_ORACLE_B");
    const fs::path scratch = m2m_test::scratchRoot() / ("m2m_folder_sweep_" + std::to_string(GetCurrentProcessId()));
    std::error_code ec;
    fs::create_directories(scratch, ec);
    beginRawRun(scratch, pipeline ? "pipeline" : "serial");

    // corpus: the fresh GUI re-save oracle_b/RS_<name> (input RS_<name>_before) is the baseline;
    // a corpus folder itself is not one (saved by older service packs and sessions)
    Tally ta;
    int noResave = 0;
    // only finished re-saves: the oracle runner logs "<name>\tsaved\t..." in _tools/rs_log.tsv
    // once the GUI has written the whole folder
    std::set<std::string> saved;
    {
        std::ifstream log(b / "_tools" / "rs_log.tsv");
        std::string line;
        while (std::getline(log, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            const std::size_t t1 = line.find('\t');
            if (t1 == std::string::npos) continue;
            const std::size_t t2 = line.find('\t', t1 + 1);
            if (line.substr(t1 + 1, t2 == std::string::npos ? std::string::npos : t2 - t1 - 1) == "saved")
                saved.insert(line.substr(0, t1));
        }
    }
    for (const auto& d : fs::directory_iterator(c)) {
        if (!d.is_directory()) continue;
        const std::string name = d.path().filename().string();
        const fs::path rs = b / ("RS_" + name), rsIn = b / ("RS_" + name + "_before");
        if (!saved.count(name) || !fs::exists(rs) || !fs::exists(rsIn)) {
            ++noResave;
            continue;
        }
        runCase(rsIn, rs, "A:RS_" + name, scratch, ta);
    }
    summary("corpus (fresh re-saves RS_*)", ta);
    diagnostic("corpus: %d projects without a finished fresh re-save yet (not compared)\n", noResave);

    Tally tb;
    const std::pair<const char*, const char*> resaves[] = {
        {"11_THEXP", "R11"}, {"19_XY_H01", "R19"}, {"52_BEND_2D", "82_R52_BEND_2D"}};
    for (const auto& [in, out] : resaves) runCase(c / in, b / out, std::string("B:") + out, scratch, tb);
    for (const auto& d : fs::directory_iterator(b)) {
        const std::string n = d.path().filename().string();
        const std::string tail = "_before";
        if (!d.is_directory() || n.size() <= tail.size() || n.compare(n.size() - tail.size(), tail.size(), tail) != 0)
            continue;
        const std::string caseB = n.substr(0, n.size() - tail.size());
        if (caseB.rfind("RS_", 0) == 0) continue;          // the corpus baseline above
        // not stage 1 targets: 120a was a failed DOE attempt in the GUI, 123 is incomplete (the
        // oracle runner could not press Edit Geometry); 114 (a click in the Convert2Dto3D editor)
        // and 119 (a Load Pattern form edit, checked by test_save_l6) had GUI interaction before
        // the Save that the sweep cannot replay
        if (caseB == "120a_B_DOE_57" || caseB == "123_B_DOE_COR") continue;
        if (caseB == "114_B_CNV_CLICK" || caseB == "119_B_HTF_LPEDIT") {
            diagnostic("NOTE B:%s: GUI interaction before the Save, not a stage 1 target (not run)\n", caseB.c_str());
            continue;
        }
        // 130n answered the Setup Type dialog with No; the conversion answers Yes (the default
        // button), so its folder differs by design (the No branch is checked by test_db5_keepmesh)
        if (caseB == "130n_B_DBSTEP_FE_no") {
            diagnostic("NOTE B:%s: Setup Type answered No, the conversion answers Yes (not run)\n", caseB.c_str());
            continue;
        }
        runCase(d.path(), b / caseB, "B:" + caseB, scratch, tb);
    }
    summary("oracle B", tb);
    if (pipeline) {
        discovering = false;
        runPipeline(scratch, prefetch);
    } else if (workers > 1) {
        discovering = false;
        runShards(workers, scratch);
    }
    finishRawRun();
    fs::remove_all(scratch, ec);
}

int wmain(int argc, wchar_t** argv)
{
    if (argc == 3 && std::wstring(argv[1]) == L"--folder-copy-worker")
        return folderCopyWorker(fs::path(argv[2]));
    if (argc == 3 && std::wstring(argv[1]) == L"--folder-post-worker")
        return folderPostWorker(fs::path(argv[2]));
    if (argc == 3 && std::wstring(argv[1]) == L"--folder-worker")
        return folderWorker(fs::path(argv[2]));
    if (argc != 1) {
        std::fprintf(stderr, "Unknown folder test arguments\n");
        return 2;
    }
    return testkit::run_all();
}
