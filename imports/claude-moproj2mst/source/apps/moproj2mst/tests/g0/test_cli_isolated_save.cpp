// ctest-labels: g0 cli isolated-save candidate-only
#include <testkit/check.hpp>
#include "../support/test_paths.hpp"

#include "moproj2mst.h"
#include "../../src/engine/terminal_harness_supervisor.hpp"

#include <windows.h>
#include <bcrypt.h>
#include <winnt.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <initializer_list>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <stdexcept>
#include <utility>
#include <vector>

namespace fs = std::filesystem;
namespace {
constexpr std::string_view primarySeed = "DONE\n";
constexpr std::string_view fallbackSeed = "FILTER_DISTANCE 7\nDONE\n";
constexpr std::string_view rawSourceManifest =
    "0ff2038095f238744a0d61a1cbf2b3da01f061ba4080f3a0dff08aa248e0c5e6";
constexpr std::array<std::pair<std::string_view, std::string_view>, 5> saveFiles{{
    // Candidate CLI project-save bytes, captured raw; this is not the historical 133 after hash.
    {"COG_P1_B1.MST", "07b0a85d88c31326ecd1f069d765fdc14e8f5efdc046195ed301eb014e20eb1c"},
    {"Objects/Object00002.KEY", "7adeee1681d831de2c49c972735ee9d76d102fe13d2b5a0917bafe33402f94e1"},
    {"Operations/Task00002/Boolean.BWZ", "0d696c7c8b4ed38bd4b0a98dc2e24f5bf1bd68059a902c1ccc3387c99eb00ca8"},
    {"Operations/Task00002/ObjProperties00004.KEY", "2dc78ce7840dc99712a35dd66e6ee132324960dd97c15902276fcf6fc6fa0f4a"},
    {"Operations/Task00002/SimCtrl.KEY", "6f0dc482e89e19de938530257297c098fc239abfaacb4ba1344f1d530897fe15"}
}};

void need(bool condition, const char* message)
{ if (!condition) throw std::runtime_error(message); }

std::string readBytes(const fs::path& path)
{
    std::ifstream in(path, std::ios::binary);
    need(static_cast<bool>(in), "cannot open expected regular file");
    std::string bytes{std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
    need(!in.bad() && bytes.size() <= 64u * 1024u * 1024u, "bounded file read failed");
    return bytes;
}

std::string sha256(std::string_view bytes)
{
    std::array<UCHAR, 32> digest{};
    need(BCryptHash(BCRYPT_SHA256_ALG_HANDLE, nullptr, 0,
        reinterpret_cast<PUCHAR>(const_cast<char*>(bytes.data())), static_cast<ULONG>(bytes.size()),
        digest.data(), static_cast<ULONG>(digest.size())) == 0, "SHA-256 failed");
    constexpr char hex[] = "0123456789abcdef";
    std::string out;
    for (auto b : digest) { out += hex[b >> 4]; out += hex[b & 15]; }
    return out;
}

struct Tree {
    std::map<std::string, std::string> files;
    std::string error;
};

Tree collectTree(const fs::path& root, bool requireShortPath = true)
{
    Tree tree;
    std::size_t total = 0;
    try {
        for (const auto& entry : fs::recursive_directory_iterator(root)) {
            const DWORD attributes = GetFileAttributesW(entry.path().c_str());
            need(attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_REPARSE_POINT),
                 "project tree contains a reparse point or unreadable entry");
            if (requireShortPath)
                need(entry.path().u16string().size() < 100, "DEFORM project path is 100 or more UTF-16 units");
            if (entry.is_directory()) continue;
            need(tree.files.size() < 512, "project file-count bound exceeded");
            auto bytes = readBytes(entry.path());
            total += bytes.size();
            need(total <= 128u * 1024u * 1024u, "project byte bound exceeded");
            tree.files.emplace(entry.path().lexically_relative(root).generic_string(), std::move(bytes));
        }
    } catch (const std::exception& e) { tree.error = e.what(); }
    return tree;
}

std::string canonicalManifest(const Tree& tree)
{
    need(tree.error.empty(), ("cannot manifest project tree: " + tree.error).c_str());
    std::string manifest;
    for (const auto& [path, bytes] : tree.files)
        manifest += sha256(bytes) + " " + std::to_string(bytes.size()) + " " + path + "\n";
    return sha256(manifest);
}

void writeBytes(const fs::path& path, std::string_view bytes)
{
    fs::create_directories(path.parent_path());
    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    need(static_cast<bool>(out), "cannot create test-owned file");
    out.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
    out.close();
    need(!out.fail(), "cannot finish test-owned file");
}

std::wstring environment(const wchar_t* name)
{
    SetLastError(ERROR_SUCCESS);
    const DWORD count = GetEnvironmentVariableW(name, nullptr, 0);
    if (!count) {
        need(GetLastError() == ERROR_ENVVAR_NOT_FOUND || GetLastError() == ERROR_SUCCESS,
             "cannot read process environment");
        return {};
    }
    std::wstring value(count, L'\0');
    const DWORD used = GetEnvironmentVariableW(name, value.data(), count);
    need(used < count, "environment changed during snapshot");
    value.resize(used);
    return value;
}

class InputMode {
    std::wstring prior_;
    bool existed_ = false;
public:
    InputMode()
    {
        SetLastError(ERROR_SUCCESS);
        const DWORD count = GetEnvironmentVariableW(L"M2M_MO_INPUT_MODE", nullptr, 0);
        existed_ = count != 0;
        if (existed_) prior_ = environment(L"M2M_MO_INPUT_MODE");
        need(SetEnvironmentVariableW(L"M2M_MO_INPUT_MODE", L"0") != FALSE,
             "cannot pin M2M_MO_INPUT_MODE=0");
    }
    ~InputMode()
    { SetEnvironmentVariableW(L"M2M_MO_INPUT_MODE", existed_ ? prior_.c_str() : nullptr); }
};

std::wstring quote(const std::wstring& value)
{
    std::wstring out = L"\"";
    std::size_t slashes = 0;
    for (wchar_t c : value) {
        if (c == L'\\') { ++slashes; continue; }
        if (c == L'\"') { out.append(slashes * 2 + 1, L'\\'); out += c; slashes = 0; continue; }
        out.append(slashes, L'\\'); slashes = 0; out += c;
    }
    out.append(slashes * 2, L'\\');
    out += L'\"';
    return out;
}

struct Pipe {
    HANDLE read = INVALID_HANDLE_VALUE, write = INVALID_HANDLE_VALUE;
    Pipe()
    {
        SECURITY_ATTRIBUTES sa{sizeof(sa), nullptr, TRUE};
        need(CreatePipe(&read, &write, &sa, 0) != FALSE, "capture pipe create failed");
        need(SetHandleInformation(read, HANDLE_FLAG_INHERIT, 0) != FALSE, "capture read end inheritance failed");
    }
    ~Pipe() { if (read != INVALID_HANDLE_VALUE) CloseHandle(read); if (write != INVALID_HANDLE_VALUE) CloseHandle(write); }
};

void drainAvailable(HANDLE handle, std::string& sink)
{
    std::array<char, 4096> bytes{};
    for (;;) {
        DWORD available = 0;
        if (!PeekNamedPipe(handle, nullptr, 0, nullptr, &available, nullptr) || !available) return;
        DWORD got = 0;
        const DWORD count = (std::min)(available, static_cast<DWORD>(bytes.size()));
        if (!ReadFile(handle, bytes.data(), count, &got, nullptr) || !got) return;
        if (sink.size() < 1024u * 1024u) {
            const auto keep = (std::min)(static_cast<std::size_t>(got), 1024u * 1024u - sink.size());
            sink.append(bytes.data(), keep);
        }
    }
}

struct ProcessResult {
    bool started = false, stopped = false, forcedStop = false;
    DWORD status = 0;
    std::string out, err;
};

ProcessResult runCli(const fs::path& exe, const std::vector<std::wstring>& args, const fs::path& cwd)
{
    Pipe out, err;
    SECURITY_ATTRIBUTES sa{sizeof(sa), nullptr, TRUE};
    HANDLE nullInput = CreateFileW(L"NUL", GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
                                   &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    need(nullInput != INVALID_HANDLE_VALUE, "cannot open child stdin");
    struct Close { HANDLE h; ~Close() { if (h != INVALID_HANDLE_VALUE) CloseHandle(h); } } closeInput{nullInput};
    STARTUPINFOW startup{};
    startup.cb = sizeof(startup); startup.dwFlags = STARTF_USESTDHANDLES;
    startup.hStdInput = nullInput; startup.hStdOutput = out.write; startup.hStdError = err.write;
    std::wstring command = quote(exe.wstring());
    for (const auto& arg : args) { command += L" "; command += quote(arg); }
    PROCESS_INFORMATION process{};
    ProcessResult result;
    need(CreateProcessW(exe.c_str(), command.data(), nullptr, nullptr, TRUE, CREATE_NO_WINDOW,
                        nullptr, cwd.c_str(), &startup, &process) != FALSE, "CLI process launch failed");
    result.started = true;
    CloseHandle(process.hThread);
    CloseHandle(out.write); out.write = INVALID_HANDLE_VALUE;
    CloseHandle(err.write); err.write = INVALID_HANDLE_VALUE;
    const ULONGLONG deadline = GetTickCount64() + 30000;
    DWORD wait = WAIT_TIMEOUT;
    while (GetTickCount64() < deadline) {
        drainAvailable(out.read, result.out); drainAvailable(err.read, result.err);
        wait = WaitForSingleObject(process.hProcess, 0);
        if (wait == WAIT_OBJECT_0) break;
        Sleep(2);
    }
    if (wait != WAIT_OBJECT_0) {
        result.forcedStop = true;
        TerminateProcess(process.hProcess, 0xe1110001u);
        result.stopped = WaitForSingleObject(process.hProcess, 2000) == WAIT_OBJECT_0;
    } else result.stopped = true;
    if (result.stopped) need(GetExitCodeProcess(process.hProcess, &result.status) != FALSE, "CLI exit status unavailable");
    CloseHandle(process.hProcess);
    drainAvailable(out.read, result.out); drainAvailable(err.read, result.err);
    return result;
}

struct Files {
    fs::path root, project, output, primary, fallback;
    Tree before;
};

class Workspace {
    fs::path root_;
    bool cleanupSafe_ = true;
public:
    fs::path deform, diagnostics;
    Workspace()
    {
        const fs::path base(L"C:\\PROBLEM");
        fs::create_directories(base);
        const DWORD attrs = GetFileAttributesW(base.c_str());
        need(attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_DIRECTORY) &&
             !(attrs & FILE_ATTRIBUTE_REPARSE_POINT), "C:\\PROBLEM must be a local non-reparse directory");
        static unsigned sequence = 0;
        root_ = base / ("cli-isolated-" + std::to_string(GetCurrentProcessId()) + "-" +
                        std::to_string(GetTickCount64()) + "-" + std::to_string(++sequence));
        need(!fs::exists(root_), "refusing to reuse an existing C:\\PROBLEM test leaf");
        diagnostics = m2m_test::scratchOutput(m2m_test::scratchRoot() /
            ("cli-isolated-" + root_.filename().string() + "-diagnostics"));
        need(!fs::exists(diagnostics) && fs::create_directory(diagnostics), "cannot reserve checkout diagnostics leaf");
        deform = root_ / "deform";
        for (const auto& path : {root_, root_ / "normal" / "COG_P1_B1.moproj",
             root_ / "normal" / "native-output.MST", root_ / "terminal" / "COG_P1_B1.moproj",
             root_ / "terminal" / "native-output.MST", root_ / "deform" / "PACK" / "pack.dat",
             root_ / "normal.receipt.json", root_ / "terminal.receipt.json"})
            need(path.u16string().size() < 100, "planned C:\\PROBLEM path is 100 or more UTF-16 units");
    }
    ~Workspace()
    {
        if (cleanupSafe_) {
            const DWORD attrs = GetFileAttributesW(root_.c_str());
            if (attrs != INVALID_FILE_ATTRIBUTES && !(attrs & FILE_ATTRIBUTE_REPARSE_POINT)) {
                std::error_code ignored; fs::remove_all(root_, ignored);
            }
        }
    }
    const fs::path& root() const { return root_; }
    void retainOwnedLeaf() { cleanupSafe_ = false; }
    void prepareDeform()
    {
        fs::create_directories(deform / "PACK");
        writeBytes(deform / "PACK" / "pack.dat", "VERSION 2\n");
    }
    Files prepare(const std::string& name)
    {
        const fs::path source = m2m_test::configuredPath("M2M_ORACLE_B") / "133_B_DBSTEP_BOOL3_before";
        const fs::path projectRoot = root_ / name;
        need(!fs::exists(projectRoot), "refusing to reuse project leaf");
        for (const auto& entry : fs::recursive_directory_iterator(source)) {
            const DWORD attrs = GetFileAttributesW(entry.path().c_str());
            need(attrs != INVALID_FILE_ATTRIBUTES && !(attrs & FILE_ATTRIBUTE_REPARSE_POINT), "oracle tree contains reparse point");
            const auto planned = projectRoot / entry.path().lexically_relative(source);
            need(planned.u16string().size() < 100, "planned fixture path is 100 or more UTF-16 units");
        }
        const auto sourceTree = collectTree(source, false);
        const auto sourceManifest = canonicalManifest(sourceTree);
        need(sourceTree.files.size() == 64 && sourceManifest == rawSourceManifest,
             ("retained B133 before source identity mismatch: files=" +
              std::to_string(sourceTree.files.size()) + " manifest=" + sourceManifest).c_str());
        fs::copy(source, projectRoot, fs::copy_options::recursive);
        Files files;
        files.root = projectRoot;
        files.project = projectRoot / "COG_P1_B1.moproj";
        files.output = projectRoot / "native-output.MST";
        files.primary = projectRoot / "LOCAL_REMESH.DAT";
        files.fallback = projectRoot / "LOC_DEN_CTRL.DAT";
        writeBytes(files.primary, primarySeed); writeBytes(files.fallback, fallbackSeed);
        files.before = collectTree(projectRoot);
        need(files.before.files.size() == 66, "constructed overlay must contain exactly 66 regular files");
        for (const auto& [relative, bytes] : sourceTree.files) {
            const auto copied = files.before.files.find(relative);
            need(copied != files.before.files.end() && copied->second == bytes,
                 "copied project differs from retained 64-file source");
        }
        need(files.before.files.at("LOCAL_REMESH.DAT") == primarySeed &&
             files.before.files.at("LOC_DEN_CTRL.DAT") == fallbackSeed,
             "constructed remesh overlay bytes differ from seed");
        return files;
    }
};

fs::path cliPath()
{
    const auto value = environment(L"M2M_CLI");
    need(!value.empty(), "M2M_CLI is not configured by CTest");
    const fs::path exe(value);
    need(exe.is_absolute() && fs::is_regular_file(exe), "M2M_CLI must identify the built CLI image");
    return fs::canonical(exe);
}

std::vector<std::wstring> argsFor(const Files& files, const fs::path& deform,
                                  bool isolated, const fs::path& receipt = {})
{
    std::vector<std::wstring> args{files.project.wstring(), files.output.wstring(), L"--write-save",
        L"--codepage", L"1252", L"--deform-dir", deform.wstring()};
    if (isolated) {
        args.push_back(L"--isolated-save"); args.push_back(L"--isolated-receipt"); args.push_back(receipt.wstring());
    }
    return args;
}

void saveChangedRaw(const fs::path& diag, const Files& files, const Tree& after)
{
    need(files.before.error.empty() && after.error.empty(), "manifest capture failed");
    std::ofstream manifest(diag / (files.root.filename().string() + "-manifest.tsv"), std::ios::binary | std::ios::trunc);
    need(static_cast<bool>(manifest), "cannot create diagnostic manifest");
    std::set<std::string> names;
    for (const auto& [name, bytes] : files.before.files) names.insert(name);
    for (const auto& [name, bytes] : after.files) names.insert(name);
    for (const auto& name : names) {
        const auto b = files.before.files.find(name), a = after.files.find(name);
        const bool hasB = b != files.before.files.end(), hasA = a != after.files.end();
        const auto beforeHash = hasB ? sha256(b->second) : "MISSING";
        const auto afterHash = hasA ? sha256(a->second) : "MISSING";
        manifest << name << "\t" << (hasB ? b->second.size() : 0) << "\t" << beforeHash
                 << "\t" << (hasA ? a->second.size() : 0) << "\t" << afterHash << "\n";
        if (hasB && hasA && b->second == a->second) continue;
        if (hasB) writeBytes(diag / "raw" / files.root.filename() / "before" / fs::u8path(name), b->second);
        if (hasA) writeBytes(diag / "raw" / files.root.filename() / "after" / fs::u8path(name), a->second);
    }
    manifest.close(); need(!manifest.fail(), "diagnostic manifest flush failed");
}

void saveProcessCapture(const fs::path& diag, std::string_view label, const ProcessResult& result)
{
    const fs::path base = diag / "process" / std::string(label);
    writeBytes(base / "stdout.txt", result.out);
    writeBytes(base / "stderr.txt", result.err);
    writeBytes(base / "status.txt", "started=" + std::to_string(result.started) +
        "\nstopped=" + std::to_string(result.stopped) + "\nforced_stop=" +
        std::to_string(result.forcedStop) + "\nstatus=" + std::to_string(result.status) + "\n");
}

void checkPrefix(const Files& files)
{
    for (const auto& [relative, hash] : saveFiles)
        CHECK_EQ(sha256(readBytes(files.root / fs::u8path(relative))), hash);
    CHECK(readBytes(files.output).find("END OF MASTER FILE") != std::string::npos);
}

void checkUnchangedExcept(const Tree& before, const Tree& after,
                          std::initializer_list<std::string_view> allowedChanges)
{
    std::set<std::string> allowed;
    for (const auto path : allowedChanges) allowed.emplace(path);
    std::set<std::string> all;
    for (const auto& [path, bytes] : before.files) { (void)bytes; all.insert(path); }
    for (const auto& [path, bytes] : after.files) { (void)bytes; all.insert(path); }
    for (const auto& path : all) {
        if (allowed.count(path)) continue;
        const auto oldFile = before.files.find(path), newFile = after.files.find(path);
        CHECK(oldFile != before.files.end());
        CHECK(newFile != after.files.end());
        if (oldFile != before.files.end() && newFile != after.files.end())
            CHECK_EQ(oldFile->second, newFile->second);
    }
}

void checkCliOutput(const Files& files)
{
    CHECK_EQ(sha256(readBytes(files.output)),
             "9bad363c4454ed9339129c6845ee82ebc044565e9165bcdb846f4fc35ed75ef1");
}
void checkExportSet(const fs::path& dll)
{
    const auto bytes = readBytes(dll);
    auto readValue = [&]<class T>(std::size_t offset) {
        need(offset <= bytes.size() && sizeof(T) <= bytes.size() - offset, "truncated PE header");
        T value{}; std::memcpy(&value, bytes.data() + offset, sizeof(value)); return value;
    };
    need(readValue.template operator()<WORD>(0) == IMAGE_DOS_SIGNATURE, "DLL DOS signature mismatch");
    const auto nt = static_cast<std::size_t>(readValue.template operator()<LONG>(offsetof(IMAGE_DOS_HEADER, e_lfanew)));
    need(readValue.template operator()<DWORD>(nt) == IMAGE_NT_SIGNATURE, "DLL NT signature mismatch");
    const auto fileHeader = nt + sizeof(DWORD);
    const WORD sections = readValue.template operator()<WORD>(fileHeader + offsetof(IMAGE_FILE_HEADER, NumberOfSections));
    const WORD optionalSize = readValue.template operator()<WORD>(fileHeader + offsetof(IMAGE_FILE_HEADER, SizeOfOptionalHeader));
    const auto optional = fileHeader + sizeof(IMAGE_FILE_HEADER);
    const WORD magic = readValue.template operator()<WORD>(optional);
    std::size_t directory = 0;
    if (magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
        directory = optional + offsetof(IMAGE_OPTIONAL_HEADER64, DataDirectory);
    else if (magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
        directory = optional + offsetof(IMAGE_OPTIONAL_HEADER32, DataDirectory);
    else need(false, "unknown DLL optional header");
    need(optionalSize >= directory - optional + sizeof(IMAGE_DATA_DIRECTORY) * (IMAGE_DIRECTORY_ENTRY_EXPORT + 1),
         "DLL export directory missing");
    const DWORD exportRva = readValue.template operator()<DWORD>(directory + sizeof(IMAGE_DATA_DIRECTORY) * IMAGE_DIRECTORY_ENTRY_EXPORT);
    const std::size_t sectionTable = optional + optionalSize;
    auto rawOffset = [&](DWORD rva) -> std::size_t {
        const DWORD sizeHeaders = readValue.template operator()<DWORD>(optional + offsetof(IMAGE_OPTIONAL_HEADER64, SizeOfHeaders));
        if (rva < sizeHeaders) return rva;
        for (WORD i = 0; i < sections; ++i) {
            const auto at = static_cast<std::size_t>(sectionTable) + i * sizeof(IMAGE_SECTION_HEADER);
            IMAGE_SECTION_HEADER s{};
            need(at <= bytes.size() && sizeof(s) <= bytes.size() - at, "truncated DLL section table");
            std::memcpy(&s, bytes.data() + at, sizeof(s));
            const DWORD extent = (std::max)(s.Misc.VirtualSize, s.SizeOfRawData);
            if (rva >= s.VirtualAddress && rva - s.VirtualAddress < extent) {
                const DWORD raw = rva - s.VirtualAddress;
                need(raw < s.SizeOfRawData, "export RVA has no raw file data");
                return static_cast<std::size_t>(s.PointerToRawData) + raw;
            }
        }
        throw std::runtime_error("export RVA is outside DLL sections");
    };
    const auto exportAt = rawOffset(exportRva);
    IMAGE_EXPORT_DIRECTORY exports{};
    need(exportAt <= bytes.size() && sizeof(exports) <= bytes.size() - exportAt, "truncated DLL export directory");
    std::memcpy(&exports, bytes.data() + exportAt, sizeof(exports));
    need(exports.NumberOfFunctions == 6 && exports.NumberOfNames == 6, "DLL no longer has exactly six named exports");
    const auto namesAt = rawOffset(exports.AddressOfNames);
    std::set<std::string> names;
    for (DWORD i = 0; i < exports.NumberOfNames; ++i) {
        const DWORD nameRva = readValue.template operator()<DWORD>(namesAt + i * sizeof(DWORD));
        auto pos = rawOffset(nameRva);
        std::string name;
        while (pos < bytes.size() && bytes[pos] != '\0' && name.size() < 256) name += bytes[pos++];
        need(pos < bytes.size() && bytes[pos] == '\0', "malformed DLL export name");
        names.insert(std::move(name));
    }
    const std::set<std::string> expected{
        "m2m_api_version", "m2m_set_codepage", "m2m_set_gui_unit", "m2m_set_deform_dir_w",
        "m2m_convert_w", "m2m_convert_utf8"};
    CHECK(names == expected);
}

struct FileLock {
    HANDLE value = INVALID_HANDLE_VALUE;
    explicit FileLock(const fs::path& path)
    {
        value = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr,
                            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    }
    ~FileLock() { if (value != INVALID_HANDLE_VALUE) CloseHandle(value); }
};

using ApiVersion = int(__cdecl*)();
using SetCodepage = int(__cdecl*)(unsigned);
using SetDeform = int(__cdecl*)(const wchar_t*);
using ConvertW = int(__cdecl*)(const wchar_t*, const wchar_t*, unsigned, wchar_t*, std::size_t);
using Convert8 = int(__cdecl*)(const char*, const char*, unsigned, char*, std::size_t);

} // namespace

TEST_CASE(cli_isolated_save_normal_and_terminal_outcomes_are_process_owned)
{
    InputMode mode;
    Workspace workspace;
    workspace.prepareDeform();
    const auto exe = cliPath();
    const auto exeBefore = m2m::engine::terminal_harness::executableSha256(exe);
    auto normal = workspace.prepare("normal");
    REQUIRE(normal.before.files.size() == 66);
    const auto normalReceipt = workspace.root() / "normal.receipt.json";
    auto normalResult = runCli(exe, argsFor(normal, workspace.deform, true, normalReceipt), normal.root);
    saveProcessCapture(workspace.diagnostics, "normal", normalResult);
    if (!normalResult.stopped) workspace.retainOwnedLeaf();
    REQUIRE(normalResult.started && normalResult.stopped && !normalResult.forcedStop);
    CHECK_EQ(normalResult.status, 0u);
    CHECK(!fs::exists(normalReceipt)); // Current first cut writes a durable receipt only for terminal relay.
    Tree normalAfter = collectTree(normal.root);
    saveChangedRaw(workspace.diagnostics, normal, normalAfter);
    checkCliOutput(normal);
    CHECK_EQ(sha256(readBytes(normal.fallback)), sha256(std::string(fallbackSeed)));
    CHECK(readBytes(normal.primary).find("KEYWORD_DRIVEN\r\n") != std::string::npos);
    CHECK(readBytes(normal.primary).find("FILTER_DISTANCE 7.000000\r\n") != std::string::npos);
    checkPrefix(normal);

    auto terminal = workspace.prepare("terminal");
    REQUIRE(terminal.before.files.size() == 66);
    const auto terminalReceipt = workspace.root() / "terminal.receipt.json";
    FileLock lock(terminal.primary);
    REQUIRE(lock.value != INVALID_HANDLE_VALUE);
    const auto terminalResult = runCli(exe, argsFor(terminal, workspace.deform, true, terminalReceipt), terminal.root);
    saveProcessCapture(workspace.diagnostics, "terminal", terminalResult);
    if (!terminalResult.stopped) workspace.retainOwnedLeaf();
    REQUIRE(terminalResult.started && terminalResult.stopped && !terminalResult.forcedStop);
    CHECK_EQ(terminalResult.status, 0xc0000417u);
    REQUIRE(fs::is_regular_file(terminalReceipt));
    const auto terminalReceiptBytes = readBytes(terminalReceipt);
    CHECK(terminalReceiptBytes.find("0xC0000417") != std::string::npos);
    CHECK(terminalReceiptBytes.find("\"boundary\":{\"protocol\":\"RemeshV2\",\"site\":2") != std::string::npos);
    CHECK(terminalReceiptBytes.find("\"requested\":true") != std::string::npos);
    CHECK(terminalReceiptBytes.find("not_observed_in_this_receipt") != std::string::npos);
    CHECK(terminalReceiptBytes.find("native_execution_observed\":false") != std::string::npos);
    CHECK(terminalReceiptBytes.find("no-native-Close-claim") != std::string::npos);
    Tree terminalAfter = collectTree(terminal.root);
    saveChangedRaw(workspace.diagnostics, terminal, terminalAfter);
    checkCliOutput(terminal);
    CHECK(readBytes(terminal.primary) == primarySeed);
    CHECK(readBytes(terminal.fallback) == fallbackSeed);
    checkPrefix(terminal);
    CHECK_EQ(m2m::engine::terminal_harness::executableSha256(exe), exeBefore);
}

TEST_CASE(cli_isolated_save_rejects_receipt_collision_and_project_alias_before_child)
{
    InputMode mode;
    Workspace workspace;
    workspace.prepareDeform();
    const auto exe = cliPath();
    auto collision = workspace.prepare("collision");
    const auto receipt = workspace.root() / "existing.receipt.json";
    const std::string sentinel = "do-not-overwrite-receipt\n";
    writeBytes(receipt, sentinel);
    {
        FileLock lock(collision.primary); REQUIRE(lock.value != INVALID_HANDLE_VALUE);
        const auto result = runCli(exe, argsFor(collision, workspace.deform, true, receipt), collision.root);
        saveProcessCapture(workspace.diagnostics, "receipt-collision", result);
        REQUIRE(result.started && result.stopped && !result.forcedStop);
        CHECK_EQ(result.status, 65u); // terminal worker stopped; receipt CREATE_NEW collision blocks parent relay
        CHECK(readBytes(receipt) == sentinel);
        CHECK(readBytes(collision.primary) == primarySeed);
        CHECK(readBytes(collision.fallback) == fallbackSeed);
        Tree after = collectTree(collision.root);
        saveChangedRaw(workspace.diagnostics, collision, after);
        checkCliOutput(collision);
        checkPrefix(collision);
    }

    auto alias = workspace.prepare("alias");
    const auto aliasProject = readBytes(alias.project);
    const auto aliasResult = runCli(exe, argsFor(alias, workspace.deform, true, alias.project), alias.root);
    saveProcessCapture(workspace.diagnostics, "receipt-project-alias", aliasResult);
    REQUIRE(aliasResult.started && aliasResult.stopped && !aliasResult.forcedStop);
    CHECK_EQ(aliasResult.status, 1u);
    CHECK(readBytes(alias.project) == aliasProject);
    CHECK(!fs::exists(alias.output));
}

TEST_CASE(cli_isolated_save_keeps_default_cli_and_six_export_abi_guards)
{
    InputMode mode;
    Workspace workspace;
    workspace.prepareDeform();
    const auto exe = cliPath();
    const fs::path dll = exe.parent_path() / "moproj2mst.dll";
    REQUIRE(fs::is_regular_file(dll));
    checkExportSet(dll);

    auto legacyCli = workspace.prepare("legacy-cli");
    {
        FileLock lock(legacyCli.primary); REQUIRE(lock.value != INVALID_HANDLE_VALUE);
        const auto& before = legacyCli.before;
        const auto result = runCli(exe, argsFor(legacyCli, workspace.deform, false), legacyCli.root);
        saveProcessCapture(workspace.diagnostics, "legacy-cli", result);
        REQUIRE(result.started && result.stopped && !result.forcedStop);
        CHECK_EQ(result.status, 7u);
        CHECK(readBytes(legacyCli.primary) == primarySeed);
        CHECK(readBytes(legacyCli.fallback) == fallbackSeed);
        Tree after = collectTree(legacyCli.root);
        saveChangedRaw(workspace.diagnostics, legacyCli, after);
        checkUnchangedExcept(before, after, {"COG_P1_B1.moproj", "COG_P1_B1.MST",
            "Objects/Object00002.KEY", "Operations/Task00002/Boolean.BWZ",
            "Operations/Task00002/DEF_MACHDIST_CTRL2.INI",
            "Operations/Task00002/ObjProperties00004.KEY",
            "Operations/Task00002/SimCtrl.KEY", "native-output.MST"});
        checkCliOutput(legacyCli);
        CHECK_EQ(before.files.size(), 66u);
        checkPrefix(legacyCli);
    }

    auto wide = workspace.prepare("abi-wide");
    HMODULE module = LoadLibraryW(dll.c_str()); REQUIRE(module != nullptr);
    auto setCodepage = reinterpret_cast<SetCodepage>(GetProcAddress(module, "m2m_set_codepage"));
    auto setDeform = reinterpret_cast<SetDeform>(GetProcAddress(module, "m2m_set_deform_dir_w"));
    auto convertW = reinterpret_cast<ConvertW>(GetProcAddress(module, "m2m_convert_w"));
    auto convert8 = reinterpret_cast<Convert8>(GetProcAddress(module, "m2m_convert_utf8"));
    auto version = reinterpret_cast<ApiVersion>(GetProcAddress(module, "m2m_api_version"));
    REQUIRE(setCodepage && setDeform && convertW && convert8 && version);
    CHECK_EQ(version(), M2M_API_VERSION);
    CHECK_EQ(setCodepage(1252), 0);
    CHECK_EQ(setDeform(workspace.deform.c_str()), 0);
    {
        FileLock lock(wide.primary); REQUIRE(lock.value != INVALID_HANDLE_VALUE);
        wchar_t message[4096]{};
        CHECK_EQ(convertW(wide.project.c_str(), wide.output.c_str(), M2M_FLAG_WRITE_SAVE,
                          message, std::size(message)), M2M_E_UNSUPPORTED);
    }
    Tree wideAfter = collectTree(wide.root);
    saveChangedRaw(workspace.diagnostics, wide, wideAfter);
    CHECK(readBytes(wide.primary) == primarySeed);
    CHECK(readBytes(wide.fallback) == fallbackSeed);
    checkPrefix(wide);
    FreeLibrary(module);

    auto utf8 = workspace.prepare("abi-utf8");
    module = LoadLibraryW(dll.c_str()); REQUIRE(module != nullptr);
    setCodepage = reinterpret_cast<SetCodepage>(GetProcAddress(module, "m2m_set_codepage"));
    setDeform = reinterpret_cast<SetDeform>(GetProcAddress(module, "m2m_set_deform_dir_w"));
    convert8 = reinterpret_cast<Convert8>(GetProcAddress(module, "m2m_convert_utf8"));
    REQUIRE(setCodepage && setDeform && convert8);
    CHECK_EQ(setCodepage(1252), 0);
    CHECK_EQ(setDeform(workspace.deform.c_str()), 0);
    const auto input8 = utf8.project.u8string(), output8 = utf8.output.u8string();
    const std::string input(reinterpret_cast<const char*>(input8.data()), input8.size());
    const std::string output(reinterpret_cast<const char*>(output8.data()), output8.size());
    {
        FileLock lock(utf8.primary); REQUIRE(lock.value != INVALID_HANDLE_VALUE);
        char message[4096]{};
        CHECK_EQ(convert8(input.c_str(), output.c_str(), M2M_FLAG_WRITE_SAVE,
                          message, sizeof(message)), M2M_E_UNSUPPORTED);
    }
    Tree utf8After = collectTree(utf8.root);
    saveChangedRaw(workspace.diagnostics, utf8, utf8After);
    CHECK(readBytes(utf8.primary) == primarySeed);
    CHECK(readBytes(utf8.fallback) == fallbackSeed);
    checkPrefix(utf8);
    FreeLibrary(module);
}

int main() { return testkit::run_all(); }








