// Helpers for defkey tier-1 tests: FILE* over a string, captured output, legacy printf options.
#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <stdio.h>
#include <string>
#if defined(_WIN32)
#include <process.h>
#else
#include <unistd.h>
#endif

namespace keytest {

// A temp-file name unique to this process (tests of several worktrees or a parallel ctest may
// run at the same time): "<pid>_<base>".
inline std::string tmp_name(const std::string& base)
{
#if defined(_WIN32)
    unsigned long pid = static_cast<unsigned long>(_getpid());
#else
    unsigned long pid = static_cast<unsigned long>(getpid());
#endif
    return std::to_string(pid) + "_" + base;
}

// What def::rt::init_stdio_compat() (p1-01) sets process-wide: three-digit exponents, old
// INF/NaN text, old half-up rounding (standard rounding off).
inline void legacy_printf()
{
    unsigned long long& o = *__local_stdio_printf_options();
    o = (o | _CRT_INTERNAL_PRINTF_LEGACY_THREE_DIGIT_EXPONENTS | _CRT_INTERNAL_PRINTF_LEGACY_MSVCRT_COMPATIBILITY) &
        ~static_cast<unsigned long long>(_CRT_INTERNAL_PRINTF_STANDARD_ROUNDING);
}

// A file in the test's working directory (named per process, see tmp_name) holding `text` (binary mode, so "\n" stays "\n"),
// opened for reading in text mode like the original's KEY files.
class InFile {
public:
    InFile(const char* name, const std::string& text) : name_(tmp_name(name))
    {
        std::FILE* w = std::fopen(name_.c_str(), "wb");
        std::fwrite(text.data(), 1, text.size(), w);
        std::fclose(w);
        f_ = std::fopen(name_.c_str(), "r");
    }
    ~InFile()
    {
        if (f_ != nullptr) std::fclose(f_);
        std::remove(name_.c_str());
    }
    std::FILE* get() const { return f_; }
    // what is left in the stream
    std::string rest()
    {
        std::string s;
        int c;
        while ((c = std::fgetc(f_)) != EOF) s += static_cast<char>(c);
        return s;
    }

private:
    std::string name_;
    std::FILE* f_ = nullptr;
};

// Output file; text() closes it and returns the bytes (written in binary mode).
class OutFile {
public:
    explicit OutFile(const char* name) : name_(tmp_name(name)) { f_ = std::fopen(name_.c_str(), "wb"); }
    ~OutFile()
    {
        if (f_ != nullptr) std::fclose(f_);
        std::remove(name_.c_str());
    }
    std::FILE* get() const { return f_; }
    std::string text()
    {
        if (f_ != nullptr) std::fclose(f_);
        f_ = nullptr;
        std::string s;
        std::FILE* r = std::fopen(name_.c_str(), "rb");
        int c;
        while ((c = std::fgetc(r)) != EOF) s += static_cast<char>(c);
        std::fclose(r);
        return s;
    }

private:
    std::string name_;
    std::FILE* f_ = nullptr;
};

} // namespace keytest
