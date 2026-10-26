#include "../support/test_paths.hpp"
// Release check: converting a project never changes it (plan section 3: "Reads only. Creates or
// replaces only out_mst_path"). For every tier-2 case the project folder is copied to a scratch
// folder and the CLI converts the copy twice:
//   A. output next to the .moproj (the default path, replacing the saved MST);
//   B. output to a folder outside the project.
// Before and after each run every file and folder of the copy is recorded: the set of entries,
// size, SHA-256, attributes, creation and last-write times. Any difference fails, except that in
// run A, when the CLI returns M2M_OK, the output MST itself and the last-write time of its folder
// may change. In run B nothing in the project may change. A failed conversion must leave the old
// MST as it was, and no temporary file may remain anywhere.
// Last-access times are not compared (Windows updates them lazily, or not at all).
// ctest-labels: m2m-release corpus
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <bcrypt.h>

#include <testkit/check.hpp>

#include <array>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#pragma comment(lib, "bcrypt.lib")

namespace fs = std::filesystem;

namespace {

using Clock = std::chrono::steady_clock;
struct Profile {
    long long hashUs = 0;
    long long metadataUs = 0;
};
Profile* activeProfile = nullptr;

long long elapsedUs(Clock::time_point start)
{
    return std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - start).count();
}

struct Entry {
    bool dir = false;
    unsigned long long size = 0;
    unsigned long attrs = 0;
    unsigned long long created = 0, written = 0;
    std::string sha256;
    bool operator==(const Entry&) const = default;
};
using Snapshot = std::map<std::wstring, Entry>;   // relative path (lower case) -> entry

unsigned long long ft(const FILETIME& f) { return (static_cast<unsigned long long>(f.dwHighDateTime) << 32) | f.dwLowDateTime; }

std::string sha256_file(const fs::path& p)
{
    const auto started = activeProfile ? Clock::now() : Clock::time_point{};
    // Read every byte in blocks. The iterator constructor previously copied each byte
    // through a growing vector before hashing; metadata never substitutes for a hash.
    std::ifstream in(p, std::ios::binary);
    if (!in) return "<hash failed>";
    struct Hash {
        BCRYPT_HASH_HANDLE handle = nullptr;
        ~Hash() { if (handle) BCryptDestroyHash(handle); }
    } hash;
    if (BCryptCreateHash(BCRYPT_SHA256_ALG_HANDLE, &hash.handle, nullptr, 0, nullptr, 0, 0) != 0)
        return "<hash failed>";
    std::array<unsigned char, 64 * 1024> block;
    do {
        in.read(reinterpret_cast<char*>(block.data()), static_cast<std::streamsize>(block.size()));
        const auto n = in.gcount();
        if (n > 0 && BCryptHashData(hash.handle, block.data(), static_cast<ULONG>(n), 0) != 0)
            return "<hash failed>";
    } while (in);
    if (!in.eof() || in.bad()) return "<hash failed>";
    unsigned char h[32];
    if (BCryptFinishHash(hash.handle, h, 32, 0) != 0)
        return "<hash failed>";
    static const char* hex = "0123456789abcdef";
    std::string s;
    s.reserve(64);
    for (unsigned char c : h) {
        s += hex[c >> 4];
        s += hex[c & 15];
    }
    if (activeProfile) activeProfile->hashUs += elapsedUs(started);
    return s;
}

Entry stat_entry(const fs::path& p)
{
    const auto started = activeProfile ? Clock::now() : Clock::time_point{};
    Entry e;
    WIN32_FILE_ATTRIBUTE_DATA a{};
    if (!GetFileAttributesExW(p.c_str(), GetFileExInfoStandard, &a)) return e;
    e.dir = (a.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
    e.attrs = a.dwFileAttributes & ~FILE_ATTRIBUTE_ARCHIVE;   // the archive bit is not content
    e.created = ft(a.ftCreationTime);
    e.written = ft(a.ftLastWriteTime);
    if (!e.dir) {
        e.size = (static_cast<unsigned long long>(a.nFileSizeHigh) << 32) | a.nFileSizeLow;
        if (activeProfile) activeProfile->metadataUs += elapsedUs(started);
        e.sha256 = sha256_file(p);
    } else if (activeProfile) activeProfile->metadataUs += elapsedUs(started);
    return e;
}

std::wstring key(const fs::path& root, const fs::path& p)
{
    std::wstring k = fs::relative(p, root).wstring();
    for (auto& c : k) c = static_cast<wchar_t>(towlower(c));
    return k.empty() ? L"." : k;
}

Snapshot snapshot(const fs::path& root)
{
    Snapshot s;
    s[L"."] = stat_entry(root);
    for (const auto& e : fs::recursive_directory_iterator(root)) s[key(root, e.path())] = stat_entry(e.path());
    return s;
}

std::string u8(const std::wstring& w)
{
    int n = WideCharToMultiByte(CP_UTF8, 0, w.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string s(static_cast<size_t>(n > 0 ? n - 1 : 0), '\0');
    if (n > 1) WideCharToMultiByte(CP_UTF8, 0, w.c_str(), -1, s.data(), n, nullptr, nullptr);
    return s;
}

// Differences between two snapshots, minus the allowed ones.
std::vector<std::string> compare(const Snapshot& before, const Snapshot& after, const std::wstring& allowed_file,
                                 const std::wstring& allowed_dir)
{
    std::vector<std::string> out;
    for (const auto& [k, e] : before) {
        auto it = after.find(k);
        if (it == after.end()) {
            out.push_back("removed: " + u8(k));
            continue;
        }
        if (it->second == e) continue;
        if (k == allowed_file) continue;
        if (k == allowed_dir) {
            Entry a = it->second, b = e;
            a.written = b.written = 0;
            if (a == b) continue;
        }
        std::string what;
        if (it->second.sha256 != e.sha256 || it->second.size != e.size) what += " content";
        if (it->second.written != e.written) what += " last-write-time";
        if (it->second.created != e.created) what += " creation-time";
        if (it->second.attrs != e.attrs) what += " attributes";
        out.push_back("changed:" + what + ": " + u8(k));
    }
    for (const auto& [k, e] : after)
        if (!before.count(k) && k != allowed_file) out.push_back("added: " + u8(k));
    return out;
}

// Runs the CLI with an explicit argv; returns its exit code, or -1 on a launch failure/timeout.
int run_cli(const std::wstring& cli, const std::vector<std::wstring>& args)
{
    std::wstring cmd = L"\"" + cli + L"\"";
    for (const auto& a : args) cmd += L" \"" + a + L"\"";
    STARTUPINFOW si{};
    si.cb = sizeof si;
    PROCESS_INFORMATION pi{};
    if (!CreateProcessW(cli.c_str(), cmd.data(), nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi))
        return -1;
    DWORD code = static_cast<DWORD>(-1);
    if (WaitForSingleObject(pi.hProcess, 120000) == WAIT_OBJECT_0) GetExitCodeProcess(pi.hProcess, &code);
    else TerminateProcess(pi.hProcess, 99);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return static_cast<int>(code);
}

std::wstring env_w(const wchar_t* name)
{
    const wchar_t* v = _wgetenv(name);
    return v ? std::wstring(v) : std::wstring();
}

// tier2.txt case ids
std::vector<std::wstring> tier2_cases(const fs::path& repo)
{
    std::vector<std::wstring> ids;
    std::wifstream in(repo / "docs/re/moproj2mst/runner/tier2.txt");
    std::wstring line;
    while (std::getline(in, line)) {
        if (line.empty() || !iswdigit(line[0])) continue;
        ids.push_back(line.substr(0, line.find_first_of(L" \t")));
    }
    return ids;
}

fs::path find_moproj(const fs::path& dir)
{
    for (const auto& e : fs::directory_iterator(dir)) {
        std::wstring ext = e.path().extension().wstring();
        for (auto& c : ext) c = static_cast<wchar_t>(towlower(c));
        if (e.is_regular_file() && ext == L".moproj") return e.path();
    }
    return {};
}

}  // namespace

// Known hashes cover empty input, a short final block, a full block followed by
// EOF, and a short second block. They are independent of snapshot comparisons.
void check_hash_vectors()
{
    const fs::path root = m2m_test::scratchRoot() / (L"m2m_hash_self_" + std::to_wstring(GetCurrentProcessId()));
    fs::remove_all(root);
    fs::create_directories(root);
    const fs::path file = root / "hash.dat";
    auto write = [&](const std::string& bytes) {
        std::ofstream out(file, std::ios::binary | std::ios::trunc);
        out.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
    };
    write("");
    CHECK(sha256_file(file) == "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
    write("abc");
    CHECK(sha256_file(file) == "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
    write(std::string(65536, 'a'));
    CHECK(sha256_file(file) == "bf718b6f653bebc184e1479f1935b8da974d701b893afcf49e701f3e2f9f9c5a");
    write(std::string(65537, 'a'));
    CHECK(sha256_file(file) == "008ffc88d3c96a9f307524eb361e47c5222a887fc45fa0c1fb8d429c5c23b430");
    fs::remove_all(root);
}

// The comparison itself catches each kind of change (self-test, no CLI involved).
TEST_CASE(harness_detects_changes)
{
    check_hash_vectors();
    const fs::path root = m2m_test::scratchRoot() / (L"m2m_unchanged_self_" + std::to_wstring(GetCurrentProcessId()));
    fs::remove_all(root);
    fs::create_directories(root / "sub");
    auto write = [](const fs::path& p, const char* text) { std::ofstream(p, std::ios::binary) << text; };
    write(root / "a.KEY", "abc");
    write(root / "P.MST", "old");
    write(root / "sub" / "b.DAT", "xyz");
    const Snapshot s0 = snapshot(root);
    CHECK(compare(s0, snapshot(root), L"", L"").empty());

    auto restamp = [](const fs::path& p, unsigned long long t) {
        HANDLE h = CreateFileW(p.c_str(), FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                               OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, nullptr);
        FILETIME f{static_cast<DWORD>(t), static_cast<DWORD>(t >> 32)};
        SetFileTime(h, nullptr, nullptr, &f);
        CloseHandle(h);
    };
    // same size, same last-write time, different bytes: only the hash sees it
    const Entry a0 = s0.at(L"a.key");
    write(root / "a.KEY", "abd");
    restamp(root / "a.KEY", a0.written);
    auto d = compare(s0, snapshot(root), L"", L"");
    CHECK(d.size() == 1 && d[0].find("content") != std::string::npos);
    write(root / "a.KEY", "abc");
    restamp(root / "a.KEY", a0.written);
    // timestamp only
    restamp(root / "sub" / "b.DAT", s0.at(L"sub\\b.dat").written + 10000000);
    d = compare(s0, snapshot(root), L"", L"");
    CHECK(d.size() == 1 && d[0].find("last-write-time") != std::string::npos);
    restamp(root / "sub" / "b.DAT", s0.at(L"sub\\b.dat").written);
    // attributes
    const DWORD attrs = GetFileAttributesW((root / "a.KEY").c_str());
    SetFileAttributesW((root / "a.KEY").c_str(), attrs | FILE_ATTRIBUTE_READONLY);
    d = compare(s0, snapshot(root), L"", L"");
    CHECK(d.size() == 1 && d[0].find("attributes") != std::string::npos);
    SetFileAttributesW((root / "a.KEY").c_str(), attrs);
    // a new file (and the folder time it changes)
    write(root / "sub" / "P.MST.tmp", "x");
    d = compare(s0, snapshot(root), L"", L"");
    for (const auto& x : d) std::printf("  self-test new file: %s\n", x.c_str());
    CHECK(d.size() == 2);
    fs::remove(root / "sub" / "P.MST.tmp");
    restamp(root / "sub", s0.at(L"sub").written);
    // the allowed output: its own change and its folder's last-write time are not differences
    write(root / "P.MST", "new");
    d = compare(s0, snapshot(root), L"p.mst", L".");
    for (const auto& x : d) std::printf("  self-test allowed: %s\n", x.c_str());
    CHECK(d.empty());
    CHECK(!compare(s0, snapshot(root), L"", L"").empty());
    // a removed file
    fs::remove(root / "a.KEY");
    d = compare(s0, snapshot(root), L"p.mst", L".");
    CHECK(d.size() == 1 && d[0].find("removed") != std::string::npos);
    fs::remove_all(root);
}

TEST_CASE(project_files_unchanged)
{
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);
    const std::wstring corpus = env_w(L"M2M_CORPUS"), cli = env_w(L"M2M_CLI"), repo = env_w(L"DEF_PORT_REPO");
    if (corpus.empty() || cli.empty() || repo.empty() || !fs::exists(corpus)) {
        std::printf("M2M_CORPUS / M2M_CLI / DEF_PORT_REPO not set: skipped\n");
        return;
    }
    const auto ids = tier2_cases(repo);
    REQUIRE(!ids.empty());
    const fs::path scratch = m2m_test::scratchRoot() / (L"m2m_unchanged_" + std::to_wstring(GetCurrentProcessId()));
    fs::remove_all(scratch);
    fs::create_directories(scratch);

    int runs = 0;
    const bool profiling = std::getenv("M2M_RELEASE_PROFILE") != nullptr;
    for (const auto& id : ids) {
        const fs::path src = fs::path(corpus) / id;
        if (!fs::exists(src)) {
            std::printf("[skip] %s: not in the corpus\n", u8(id).c_str());
            continue;
        }
        for (int mode = 0; mode < 2; ++mode) {
            const fs::path proj = scratch / id;
            const fs::path outdir = scratch / (id + L"_out");
            fs::remove_all(proj);
            fs::remove_all(outdir);
            const auto copyStarted = profiling ? Clock::now() : Clock::time_point{};
            fs::copy(src, proj, fs::copy_options::recursive);
            fs::create_directories(outdir);
            const long long copyUs = profiling ? elapsedUs(copyStarted) : 0;
            const fs::path moproj = find_moproj(proj);
            REQUIRE(!moproj.empty());
            const fs::path mst = moproj.parent_path() / (moproj.stem().wstring() + L".MST");
            const fs::path out = mode == 0 ? mst : outdir / mst.filename();

            Profile profile;
            activeProfile = profiling ? &profile : nullptr;
            const auto snapshotStarted = profiling ? Clock::now() : Clock::time_point{};
            const Snapshot before = snapshot(proj);
            const long long beforeSnapshotUs = profiling ? elapsedUs(snapshotStarted) : 0;
            std::vector<std::wstring> args{moproj.wstring()};
            if (mode == 1) args.push_back(out.wstring());
            const auto cliStarted = profiling ? Clock::now() : Clock::time_point{};
            const int rc = run_cli(cli, args);
            const long long cliUs = profiling ? elapsedUs(cliStarted) : 0;
            const auto afterStarted = profiling ? Clock::now() : Clock::time_point{};
            const Snapshot after = snapshot(proj);
            const long long afterSnapshotUs = profiling ? elapsedUs(afterStarted) : 0;
            activeProfile = nullptr;

            const bool ok = rc == 0;
            std::wstring allowed_file, allowed_dir;
            if (mode == 0 && ok) {
                allowed_file = key(proj, mst);
                allowed_dir = key(proj, mst.parent_path());
            }
            const auto compareStarted = profiling ? Clock::now() : Clock::time_point{};
            auto diffs = compare(before, after, allowed_file, allowed_dir);
            // run B: the output folder holds the MST (when OK) and nothing else
            for (const auto& e : fs::directory_iterator(outdir))
                if (!(ok && e.path().filename() == out.filename()))
                    diffs.push_back("stray file in the output folder: " + u8(e.path().filename().wstring()));
            const long long compareUs = profiling ? elapsedUs(compareStarted) : 0;
            std::printf("[%s] %s %s: exit %d, %zu differences\n", diffs.empty() ? " ok " : "FAIL", u8(id).c_str(),
                        mode == 0 ? "default-out" : "outside-out", rc, diffs.size());
            if (profiling)
                std::printf("M2M_RELEASE_PROFILE\t%s\t%s\t%lld\t%lld\t%lld\t%lld\t%lld\t%lld\t%lld\n",
                            u8(id).c_str(), mode == 0 ? "default-out" : "outside-out", copyUs, cliUs,
                            beforeSnapshotUs, afterSnapshotUs, compareUs, profile.hashUs, profile.metadataUs);
            for (const auto& d : diffs) std::printf("    %s\n", d.c_str());
            CHECK(rc >= 0);
            CHECK(diffs.empty());
            ++runs;
        }
    }
    CHECK(runs > 0);
    std::error_code ec;
    fs::remove_all(scratch, ec);
}

int main() { return testkit::run_all(); }
