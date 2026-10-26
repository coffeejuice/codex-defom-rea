// CNVT3D status and log files (CNVT3D.md sections 4.2 and 7). Packet p9-01.
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/cnvt3d.hpp>

#include <cstdio>
#include <cstring>
#include <share.h>
#include <string>

namespace def::cmd::cnvt3d {

namespace {

// 1401b1880: wrapper ctor {name, null}; 1401b18f0: open with _fsopen(name, mode, 0x40) (14047b934),
// true when it opened; 1401b1940: fclose when open; 1401b18b0: dtor (empty)
bool wrap_open(FileWrap* w, const char* mode)
{
    w->f = _fsopen(w->name, mode, _SH_DENYNO);
    return w->f != nullptr;
}
void wrap_close(FileWrap* w)
{
    if (w->f != nullptr) std::fclose(w->f);
}

// 1401b0cc0 (s by value): s ends with p, compared with A-Z folded to lower case, and s is strictly
// longer than p. Ported as def::pre::ends_with in def_pre/src/menu7.cpp, which defcmd cannot link.
bool ends_with(std::string s, const char* p)
{
    const std::int32_t n = static_cast<std::int32_t>(std::strlen(p));
    if (static_cast<std::int32_t>(s.size()) - n < 1) return false;
    std::string a(s.substr(s.size() - static_cast<std::size_t>(n)).c_str());
    std::string b(p);
    for (std::int32_t i = 0; i < n; i = i + 1) {
        char& x = a[static_cast<std::size_t>(i)];
        char& y = b[static_cast<std::size_t>(i)];
        if ('@' < x && x < '[') x = static_cast<char>(x + ' ');
        if ('@' < y && y < '[') y = static_cast<char>(y + ' ');
    }
    return std::strncmp(a.c_str(), b.c_str(), static_cast<std::size_t>(n)) == 0;
}

} // namespace

// @src 1401b1970
void file_write(FileWrap* w, const void* p, std::uint32_t n)
{
    if (w->f != nullptr) std::fwrite(p, n, 1, w->f);
}

// @src 14013c6b0
void log_message(const char* msg, const char* stem)
{
    FileWrap st{"CONV_STATUS.DAT", nullptr};
    if (!wrap_open(&st, "a")) return;  // str 14051cbcc
    file_write(&st, msg, static_cast<std::uint32_t>(std::strlen(msg)));
    file_write(&st, "\n", 1);
    wrap_close(&st);
    std::string s(stem);  // 1400066a0
    if (ends_with(s, ".DB")) {
        const std::int32_t len = static_cast<std::int32_t>(s.size());  // 140002800
        std::string cut;
        ext().str_left(cut, s, len - 3);  // 1401b1520 (p8-08)
        s = cut;
    }
    char name[0x400];
    std::sprintf(name, "%s%s", s.c_str(), ".LOG");
    FileWrap lg{name, nullptr};
    if (!wrap_open(&lg, "a")) return;  // str 14051cc60
    file_write(&lg, msg, static_cast<std::uint32_t>(std::strlen(msg)));
    file_write(&lg, "\n", 1);
    wrap_close(&lg);
    std::fprintf(stdout, "%s\n", msg);
}

// @src 14013c320
void delete_status_file() { std::remove("CONV_STATUS.DAT"); }

// @src 14013c970
void cnv23_file(std::int32_t mode, std::int32_t sim_no)
{
    if (mode == 0) std::remove("DEF_CNV23.DAT");
    if (mode == 1) {
        FileWrap w{"DEF_CNV23.DAT", nullptr};
        if (wrap_open(&w, "w")) {  // str 14051cc6c
            if (0 < sim_no) {
                // 1401b1bd0 SfOStream on the FILE, 140007660 << "\tSimNo\t", 140159340 << int,
                // 140002830 endl (140007a50: '\n' and flush), 140158be0 dtor
                std::fprintf(w.f, "\tSimNo\t%d\n", sim_no);
                std::fflush(w.f);
            }
            wrap_close(&w);
        }
    }
}

// @src 14013d760
void log_begin() { log_message("$BGN$", " "); }

// @src 14013d790
void log_end() { log_message("$END$", " "); }

} // namespace def::cmd::cnvt3d
