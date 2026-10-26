// Fortran units, OPEN/CLOSE and the record block I/O of datarm23. See dbcore/ftn_io.hpp.
#define _CRT_SECURE_NO_WARNINGS // plain CRT stdio, as the Fortran runtime under it
#include <defdbio/dbcore/ftn_io.hpp>

#include <algorithm>
#include <cctype>
#include <cstring>
#include <filesystem>
#include <map>
#include <vector>

#ifdef _WIN32
#include <io.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace def::dbio::dbcore {

namespace {

struct Unit {
    std::FILE* f = nullptr;
    std::string name;
    bool direct = false;
    bool writable = false;
};

std::map<std::int32_t, Unit>& units()
{
    static std::map<std::int32_t, Unit> u;
    return u;
}

RuntimeFaultHook g_fault_hook = nullptr;

std::string upper_trim(const char* s, std::int32_t len)
{
    std::string r = fortran_name(s, len);
    std::size_t b = 0;
    while (b < r.size() && r[b] == ' ') {
        ++b;
    }
    r.erase(0, b);
    for (char& c : r) {
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }
    return r;
}

std::uint64_t file_size(std::FILE* f)
{
    std::fflush(f);
#ifdef _WIN32
    const long long n = _filelengthi64(_fileno(f));
    return n < 0 ? 0 : static_cast<std::uint64_t>(n);
#else
    const long pos = std::ftell(f);
    std::fseek(f, 0, SEEK_END);
    const long n = std::ftell(f);
    std::fseek(f, pos, SEEK_SET);
    return n < 0 ? 0 : static_cast<std::uint64_t>(n);
#endif
}

bool seek(std::FILE* f, std::uint64_t off)
{
#ifdef _WIN32
    return _fseeki64(f, static_cast<long long>(off), SEEK_SET) == 0;
#else
    return std::fseek(f, static_cast<long>(off), SEEK_SET) == 0;
#endif
}

void close_unit(std::int32_t unit, bool remove_file)
{
    auto it = units().find(unit);
    if (it == units().end()) {
        return;
    }
    std::fclose(it->second.f);
    if (remove_file && !it->second.name.empty()) {
        std::error_code ec;
        std::filesystem::remove(it->second.name, ec);
    }
    units().erase(it);
}

// OPEN with the given STATUS; `read_only` for ACTION='READ'. Returns IOSTAT.
std::int32_t open_unit(std::int32_t unit, const std::string& name, const std::string& status,
                       bool direct, bool binary, bool read_only)
{
    close_unit(unit, false); // OPEN on a connected unit reconnects it
    std::error_code ec;
    const bool exists = !name.empty() && std::filesystem::exists(name, ec);
    const char* rw = binary ? "r+b" : "r+";
    const char* ro = binary ? "rb" : "r";
    const char* cr = binary ? "w+b" : "w+";
    std::FILE* f = nullptr;
    bool writable = !read_only;
    if (status == "SCRATCH") {
        f = std::tmpfile();
        writable = true;
    } else if (name.empty()) {
        return kIosOpenFailed;
    } else if (status == "OLD" || ((status.empty() || status == "UNKNOWN") && exists)) {
        if (!exists) {
            return kIosFileNotFound;
        }
        if (!read_only) {
            f = std::fopen(name.c_str(), rw);
        }
        if (f == nullptr) { // no write access: the runtime falls back to reading
            f = std::fopen(name.c_str(), ro);
            writable = false;
        }
    } else if (status == "NEW") {
        if (exists) {
            return kIosFileExists;
        }
        f = std::fopen(name.c_str(), cr);
    } else { // REPLACE, or UNKNOWN of a file that does not exist
        f = std::fopen(name.c_str(), cr);
    }
    if (f == nullptr) {
        return kIosOpenFailed;
    }
    units()[unit] = Unit{f, name, direct, writable};
    return 0;
}

// FUN_14044b62a / bc76 / c2c2 share their body; only the item size and items per record differ.
void block_io(std::int32_t mode, std::int32_t unit, void* buf, std::int32_t n, std::int32_t* rec,
              std::int32_t* err, std::int32_t item_bytes, std::int32_t per_record)
{
    *err = 0;
    if (n < 1) {
        return;
    }
    if (*rec < 1) {
        *err = 1; // a record number < 1 is an I/O error, taken by ERR=
        return;
    }
    if (mode != 1 && mode != 2) {
        return;
    }
    auto it = units().find(unit);
    if (it == units().end() || !it->second.direct) {
        *err = 1;
        return;
    }
    Unit& u = it->second;
    auto* p = static_cast<unsigned char*>(buf);
    std::int32_t first = 1;
    for (;;) {
        std::int32_t last = first - 1 + per_record;
        if (n <= last) {
            last = n;
        }
        const std::size_t bytes = static_cast<std::size_t>(last - first + 1) * item_bytes;
        const std::uint64_t off = static_cast<std::uint64_t>(*rec - 1) * kRecordBytes;
        unsigned char* src = p + static_cast<std::size_t>(first - 1) * item_bytes;
        if (mode == 1) {
            // A READ needs the whole record in the file.
            if (off + kRecordBytes > file_size(u.f) || !seek(u.f, off) ||
                std::fread(src, 1, bytes, u.f) != bytes) {
                *err = 1;
                return;
            }
        } else {
            static const unsigned char zeros[kRecordBytes] = {};
            if (!u.writable || !seek(u.f, off) || std::fwrite(src, 1, bytes, u.f) != bytes ||
                std::fwrite(zeros, 1, kRecordBytes - bytes, u.f) != kRecordBytes - bytes) {
                *err = 1;
                return;
            }
        }
        first = last + 1;
        if (n < first) {
            // 14044b62a (read): a READ of record 1 into a scratch word follows here; it has no
            // effect the DB code can see and is not reproduced.
            if (mode == 2) {
                std::fflush(u.f);
            }
            return;
        }
        *rec = *rec + 1;
    }
}

} // namespace

std::string fortran_name(const char* name, std::int32_t len)
{
    std::int32_t n = 0;
    while (n < len && name[n] != '\0') {
        ++n;
    }
    while (n > 0 && name[n - 1] == ' ') {
        --n;
    }
    return std::string(name, static_cast<std::size_t>(n));
}

// @src 140407e81  Fortran OPEN/CLOSE wrapper (strings "DIRECT", "UNFORMATTED", "READ" 140bb13e0,
// "OLD" 140bb1380)
void fortran_open(const std::int32_t* mode, const std::int32_t* unit, const char* file,
                  const char* status, std::int32_t* iostat, std::int32_t file_len,
                  std::int32_t status_len)
{
    const std::string st = upper_trim(status, status_len < 0 ? 0 : status_len);
    const std::string name = fortran_name(file, file_len < 0 ? 0 : file_len);
    if (*mode > 0 && *mode < 7 && st == "OLD") {
        // INQUIRE(FILE=file, EXIST=DAT_14070bbf0) without IOSTAT: an all-blank name is a
        // runtime error. The original's runtime reports it and then faults (oracle, see the -m
        // thread: "-d ''" exits 0xC0000005); the report text is the runtime's.
        if (name.empty()) {
            std::fputs("? FORTRAN Runtime Error:\n? Bad file specification\n? INQUIRE(UNIT=", stderr);
            runtime_fault(kStatusAccessViolation);
        }
        std::error_code ec;
        if (!std::filesystem::exists(name, ec)) {
            *iostat = kIosNoExist;
            return;
        }
    }
    switch (*mode) {
    case 0: {
        const bool del = st == "DELETE";
        close_unit(*unit, del);
        *iostat = 0;
        return;
    }
    case 1:
    case 2:
        *iostat = open_unit(*unit, name, st, false, false, false);
        return;
    case 3:
        *iostat = open_unit(*unit, name, st, false, true, false);
        return;
    case 4:
    case 6:
        *iostat = open_unit(*unit, name, st, true, true, false);
        return;
    case 5:
        *iostat = open_unit(*unit, name, st, true, true, true);
        return;
    case 7:
    case 8:
    case 9:
        *iostat = 0;
        return;
    case 10:
        *iostat = open_unit(*unit, name, st, false, false, true);
        return;
    default:
        return;
    }
}

// @src 14044b62a  INTEGER array, 128 words per record (DAT_140638020)
void block_io_int(const std::int32_t* mode, const std::int32_t* unit, void* buf,
                  const std::int32_t* n, std::int32_t* rec, std::int32_t* err)
{
    block_io(*mode, *unit, buf, *n, rec, err, 4, kIntsPerRecord);
}

// @src 14044bc76  CHARACTER*4 array, 128 items per record (DAT_140638024)
void block_io_char4(const std::int32_t* mode, const std::int32_t* unit, void* buf,
                    const std::int32_t* n, std::int32_t* rec, std::int32_t* err)
{
    block_io(*mode, *unit, buf, *n, rec, err, 4, kChars4PerRecord);
}

// @src 14044c2c2  REAL*8 array, 64 words per record (DAT_140638028)
void block_io_real8(const std::int32_t* mode, const std::int32_t* unit, void* buf,
                    const std::int32_t* n, std::int32_t* rec, std::int32_t* err)
{
    block_io(*mode, *unit, buf, *n, rec, err, 8, kRealsPerRecord);
}

bool unit_is_open(std::int32_t unit) { return units().count(unit) != 0; }

std::string unit_file(std::int32_t unit)
{
    auto it = units().find(unit);
    return it == units().end() ? std::string() : it->second.name;
}

void close_all_units()
{
    while (!units().empty()) {
        close_unit(units().begin()->first, false);
    }
}

void set_runtime_fault_hook(RuntimeFaultHook hook) { g_fault_hook = hook; }

void runtime_fault(std::uint32_t status)
{
    std::fflush(stderr);
    if (g_fault_hook) {
        g_fault_hook(status); // a test hook throws
    }
#ifdef _WIN32
    ::TerminateProcess(::GetCurrentProcess(), status);
#endif
    std::_Exit(static_cast<int>(status));
}

} // namespace def::dbio::dbcore
