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

#include <testkit/check.hpp>

#include <cstdio>
#include <cstdlib>
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
};

// Discovery buffers only the opted-in pipeline. Conversion still runs serially
// in this process, in the same directory-iterator order as the ordinary sweep.
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
bool pipelinePosting = false;
bool pipelineQueued = false;
fs::path pipelineMailbox;
void publishPost(unsigned ordinal, bool compare);

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

// Saves whose .moproj view parts follow an older GUI window size (L7: fresh saves of the same
// projects match the port); a .moproj-only difference there is expected.
bool oldViewProfile(const std::string& name)
{
    return name == "B:R11" || name == "B:R19" || name == "B:82_R52_BEND_2D";
}

void compareCase(const fs::path& expected, const fs::path& work, const std::string& name, Tally& t)
{
    std::error_code ec;
    const auto got = files(work), want = files(expected);
    std::vector<std::string> differ, missing, extra;
    bool moprojDiffers = false;
    int banner = 0;
    const bool doe = name.find("_B_DOE") != std::string::npos;   // saved after GUI interaction
    for (const auto& [rel, path] : want) {
        auto it = got.find(rel);
        if (it == got.end()) {
            missing.push_back(rel);
            continue;
        }
        // an absolute path the GUI carried over from the input names the expected folder too
        // (103/104: UserHT.KEY in the HTWZ and the MST) and stays as it is in the conversion, so
        // the file also passes unrelocated
        const std::string raw = readAll(path), g = readAll(it->second);
        if (g == raw) continue;
        const std::string w = relocated(raw, expected, work);
        if (g == w) continue;
        if (fs::path(rel).extension() == ".moproj") {
            if (maskView(g, doe) == maskView(w, doe)) continue;
            moprojDiffers = true;
            differ.push_back(rel);
        } else if (bannerOnly(g, w)) {
            ++banner;
        } else {
            differ.push_back(rel);
        }
    }
    for (const auto& [rel, path] : got)
        if (!want.count(rel)) extra.push_back(rel);
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
    fs::remove_all(work, ec);
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
    std::error_code ec;
    fs::remove_all(work, ec);
    fs::copy(input, work, fs::copy_options::recursive, ec);
    if (!ec && !fs::exists(work / moproj.filename())) ec = std::make_error_code(std::errc::no_such_file_or_directory);
    if (ec) {
        std::printf("SKIP %s: copy failed (%s)\n", name.c_str(), ec.message().c_str());
        return;
    }
    const m2m::Result r = m2m::convert((work / moproj.filename()).u16string(), u"", M2M_FLAG_WRITE_SAVE, 1252);
    if (r.code != M2M_OK) {
        if (outcome) outcome->state = CaseOutcome::Refused;
        ++t.refused;
        std::printf("REFUSED %s (%d): %s\n", name.c_str(), r.code, r.message.substr(0, 200).c_str());
        fs::remove_all(work, ec);
        return;
    }
    if (pipelinePosting) {
        publishPost(workOrdinal, true);
        pipelineQueued = true;
        return;
    }
    if (outcome) outcome->state = CaseOutcome::Compared;
    compareCase(expected, work, name, t);
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

constexpr unsigned kMailboxOpenAttempts = 10;
constexpr DWORD kMailboxOpenRetryMs = 10;

template<class OpenAttempt>
std::ifstream openMailbox(const fs::path& path, OpenAttempt&& openAttempt)
{
    // Retry only an unsuccessful open. Once a stream is open, every parse/read
    // error remains fatal and is never retried as if it were a transient lock.
    for (unsigned attempt = 1; attempt <= kMailboxOpenAttempts; ++attempt) {
        std::ifstream in;
        openAttempt(in, path);
        if (in.is_open()) return in;
        if (attempt < kMailboxOpenAttempts) Sleep(kMailboxOpenRetryMs);
    }
    throw std::runtime_error("Cannot open folder IPC mailbox after " +
                             std::to_string(kMailboxOpenAttempts) + " attempts: " + path.string());
}

std::ifstream openMailbox(const fs::path& path)
{
    return openMailbox(path, [](std::ifstream& in, const fs::path& file) {
        in.open(file, std::ios::binary);
    });
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

struct PostRequest {
    unsigned ordinal = 0;
    bool compare = false;
};

PostRequest readPostRequest(std::istream& request, unsigned expectedOrdinal)
{
    if (getString(request) != "M2M_FOLDER_POST_READY_MINIMAL_1")
        throw std::runtime_error("Unknown post request schema");
    const auto ordinal = getNumber(request);
    if (!ordinal || ordinal > UINT_MAX || ordinal != expectedOrdinal)
        throw std::runtime_error("Post request identity mismatch");
    const auto compare = getNumber(request);
    if (compare > 1) throw std::runtime_error("Invalid folder post operation");
    requireEnd(request);
    return {static_cast<unsigned>(ordinal), compare != 0};
}

struct PostResult {
    bool compared = false;
    Tally delta;
    std::string output;
};

Tally getTally(std::istream& in);

PostResult readPostResult(std::istream& result, const std::string& identity,
                          unsigned expectedOrdinal, bool expectedCompared)
{
    if (getString(result) != "M2M_FOLDER_POST_RESULT_MINIMAL_1" || getString(result) != identity ||
        getNumber(result) != expectedOrdinal)
        throw std::runtime_error("Folder post result identity mismatch");
    const auto compared = getNumber(result);
    if (compared > 1 || static_cast<bool>(compared) != expectedCompared)
        throw std::runtime_error("Folder post terminal state mismatch");
    PostResult parsed;
    parsed.compared = compared != 0;
    if (parsed.compared) {
        parsed.delta = getTally(result);
        if (parsed.delta.refused) throw std::runtime_error("Comparator returned a converter refusal");
        parsed.output = getString(result);
    }
    requireEnd(result);
    return parsed;
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

void finishMailbox(std::ofstream& out, const fs::path& temporary, const fs::path& ready)
{
    out.close();
    if (!out) throw std::runtime_error("Cannot finish folder pipeline message");
    fs::rename(temporary, ready);
}

void publishPost(unsigned ordinal, bool compare)
{
    const auto temporary = pipelineMailbox / (std::to_string(ordinal) + ".pending");
    const auto ready = pipelineMailbox / (std::to_string(ordinal) + ".ready");
    std::ofstream out(temporary, std::ios::binary | std::ios::trunc);
    putString(out, "M2M_FOLDER_POST_READY_MINIMAL_1");
    putNumber(out, ordinal); putNumber(out, compare ? 1 : 0);
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
        if (getString(job) != "M2M_FOLDER_POST_JOB_MINIMAL_1") throw std::runtime_error("Unknown post job schema");
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
            auto request = openMailbox(ready);
            const auto parsedRequest = readPostRequest(request, event.ordinal);
            if (parsedRequest.compare) {
                FILE* capture = nullptr;
                const auto path = manifest.parent_path() / "case.stdout";
                if (_wfreopen_s(&capture, path.c_str(), L"w", stdout) || !capture)
                    throw std::runtime_error("Cannot capture folder post stdout");
                if (!bindStandardHandle(stdout, STD_OUTPUT_HANDLE))
                    throw std::runtime_error("Cannot bind folder post stdout capture handle");
                event.delta.cases = 1;
                compareCase(event.expected, event.scratch, event.name, event.delta);
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
            putString(result, "M2M_FOLDER_POST_RESULT_MINIMAL_1"); putString(result, identity);
            putNumber(result, event.ordinal); putNumber(result, parsedRequest.compare ? 1 : 0);
            if (parsedRequest.compare) { putTally(result, event.delta); putString(result, event.output); }
            finishMailbox(result, temporary, done);
        }
        return 0;
    } catch (const std::exception& e) {
        std::fprintf(stderr, "Folder post helper infrastructure failure: %s\n", e.what());
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

void runPipeline(const fs::path& scratch)
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
        putString(job, "M2M_FOLDER_POST_JOB_MINIMAL_1");
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

    struct PipelineReset {
        ~PipelineReset() {
            pipelinePosting = false; pipelineQueued = false; pipelineMailbox.clear();
        }
    } reset;
    pipelineMailbox = ipc;
    auto collect = [&](std::size_t k) {
        auto& event = folderEvents[k];
        const auto done = ipc / (std::to_string(event.ordinal) + ".done");
        waitMailbox(done, child.process);
        auto result = openMailbox(done);
        auto parsedResult = readPostResult(result, child.identity, event.ordinal,
                                           event.outcome.state == CaseOutcome::Compared);
        if (parsedResult.compared) {
            event.delta = parsedResult.delta;
            event.output += parsedResult.output;
        }
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
            pipelinePosting = true; pipelineQueued = false;
            runCase(event.input, event.expected, event.name, event.scratch, event.delta,
                    event.ordinal, event.moproj, &event.outcome);
            pipelinePosting = false;
            event.output = output.next();
            if (!errors.next().empty())
                throw std::runtime_error("Serial converter emitted stderr; inspect " + (ipc / "coordinator.stderr").string());
            if (pipelineQueued) event.outcome.state = CaseOutcome::Compared;
            else publishPost(event.ordinal, false);
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

}  // namespace

TEST_CASE(folder_pipeline_mailbox_open_and_truncation_checks)
{
    const fs::path scratch = m2m_test::scratchRoot() /
        ("folder_mailbox_faults_" + std::to_string(GetCurrentProcessId()) + "_" +
         std::to_string(GetTickCount64()));
    struct Cleanup {
        fs::path path;
        bool active = false;
        ~Cleanup() { if (active) { std::error_code ec; fs::remove_all(path, ec); } }
    } cleanup{scratch};
    std::error_code ec;
    const bool created = fs::create_directory(scratch, ec);
    cleanup.active = created && !ec;
    REQUIRE(created && !ec);

    const fs::path readyPath = scratch / "transient.ready";
    {
        std::ofstream out(readyPath, std::ios::binary | std::ios::trunc);
        putString(out, "M2M_FOLDER_POST_READY_MINIMAL_1");
        putNumber(out, 42); putNumber(out, 1);
        out.close();
        REQUIRE(!out.fail());
    }
    unsigned transientAttempts = 0;
    auto validRequest = openMailbox(readyPath, [&](std::ifstream& in, const fs::path& path) {
        ++transientAttempts;
        if (transientAttempts >= 3) in.open(path, std::ios::binary);
        else in.setstate(std::ios::failbit);
    });
    const auto request = readPostRequest(validRequest, 42);
    CHECK_EQ(transientAttempts, 3u);
    CHECK_EQ(request.ordinal, 42u);
    CHECK(request.compare);

    unsigned deniedAttempts = 0;
    std::string deniedDiagnostic;
    bool deniedOpened = false;
    try {
        auto denied = openMailbox(readyPath, [&](std::ifstream& in, const fs::path&) {
            ++deniedAttempts; // deterministic permanent-open denial, before any bytes are read
            in.setstate(std::ios::failbit);
        });
        deniedOpened = denied.is_open();
    } catch (const std::runtime_error& e) {
        deniedDiagnostic = e.what();
    }
    CHECK(!deniedOpened);
    CHECK_EQ(deniedAttempts, kMailboxOpenAttempts);
    CHECK(deniedDiagnostic.find("Cannot open folder IPC mailbox after") != std::string::npos);
    CHECK(deniedDiagnostic.find(readyPath.string()) != std::string::npos);

    const fs::path truncatedReadyPath = scratch / "truncated.ready";
    {
        std::ofstream out(truncatedReadyPath, std::ios::binary | std::ios::trunc);
        putString(out, "M2M_FOLDER_POST_READY_MINIMAL_1");
        putNumber(out, 43); // operation field intentionally absent
        out.close();
        REQUIRE(!out.fail());
    }
    unsigned truncatedReadyOpenAttempts = 0;
    std::string truncatedReadyDiagnostic;
    try {
        auto truncated = openMailbox(truncatedReadyPath, [&](std::ifstream& in, const fs::path& path) {
            ++truncatedReadyOpenAttempts;
            in.open(path, std::ios::binary);
        });
        (void)readPostRequest(truncated, 43);
    } catch (const std::runtime_error& e) {
        truncatedReadyDiagnostic = e.what();
    }
    CHECK_EQ(truncatedReadyOpenAttempts, 1u);
    CHECK_EQ(truncatedReadyDiagnostic, std::string("Truncated folder IPC"));

    const fs::path truncatedDonePath = scratch / "truncated.done";
    {
        std::ofstream out(truncatedDonePath, std::ios::binary | std::ios::trunc);
        putString(out, "M2M_FOLDER_POST_RESULT_MINIMAL_1");
        putString(out, "exact job identity");
        putNumber(out, 7); putNumber(out, 1);
        for (const std::uint64_t value : {1, 1, 0, 0, 0}) putNumber(out, value);
        out.close(); // view-profile tally field intentionally absent
        REQUIRE(!out.fail());
    }
    unsigned truncatedDoneOpenAttempts = 0;
    std::string truncatedDoneDiagnostic;
    try {
        auto truncated = openMailbox(truncatedDonePath, [&](std::ifstream& in, const fs::path& path) {
            ++truncatedDoneOpenAttempts;
            in.open(path, std::ios::binary);
        });
        (void)readPostResult(truncated, "exact job identity", 7, true);
    } catch (const std::runtime_error& e) {
        truncatedDoneDiagnostic = e.what();
    }
    CHECK_EQ(truncatedDoneOpenAttempts, 1u);
    CHECK_EQ(truncatedDoneDiagnostic, std::string("Truncated folder IPC"));
}

TEST_CASE(save_folder_sweep)
{
    const char* corpus = std::getenv("M2M_CORPUS");
    if (!std::getenv("M2M_FOLDER_SWEEP") || !corpus) return;
    const char* pipelineSetting = std::getenv("M2M_FOLDER_PIPELINE");
    if (pipelineSetting && std::string(pipelineSetting) != "0" && std::string(pipelineSetting) != "1")
        throw std::runtime_error("M2M_FOLDER_PIPELINE must be 0 or 1");
    const bool pipeline = pipelineSetting && std::string(pipelineSetting) == "1";
    folderEvents.clear();
    folderOrdinal = 0;
    discovering = pipeline;
    struct DiscoveryReset {
        ~DiscoveryReset() { discovering = false; folderEvents.clear(); }
    } discoveryReset;
    const fs::path c = corpus;
    const fs::path b = m2m_test::configuredPath("M2M_ORACLE_B");
    const fs::path scratch = m2m_test::scratchRoot() / ("m2m_folder_sweep_" + std::to_string(GetCurrentProcessId()));
    std::error_code ec;
    fs::create_directories(scratch, ec);

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
        runPipeline(scratch);
    }
    fs::remove_all(scratch, ec);
}

int wmain(int argc, wchar_t** argv)
{
    if (argc == 3 && std::wstring(argv[1]) == L"--folder-post-worker")
        return folderPostWorker(fs::path(argv[2]));
    if (argc != 1) {
        std::fprintf(stderr, "Unknown folder test arguments\n");
        return 2;
    }
    return testkit::run_all();
}
