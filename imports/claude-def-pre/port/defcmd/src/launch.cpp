// External program launcher (packet p8-07). Spec: docs/re/commands/common.md.
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/launch.hpp>

#include <cstdlib>
#include <cstring>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include <defdbio/step/db_info_read.hpp>
#include <defrt/port.hpp>

namespace def::cmd {

// @src 140222e20
std::int32_t split_words(char* line, const char* seps, char** argv, std::int32_t max)
{
    std::int32_t n = 0;
    std::uint32_t state = 0;  // 0 start, 1 between words, 2 in a word, 3 in a quote
    char quote = 0;
    for (std::int32_t i = 0; i < max; i = i + 1) argv[i] = nullptr;
    for (char* p = line; *p != '\0'; p = p + 1) {
        if (state < 2) {
            if (std::strchr(seps, *p) == nullptr) {
                // The original stores argv[n] even when n has reached max (its array has 24
                // slots for max 21, so a 22nd word lands in slot 21); never past max here.
                if (n < max) argv[n] = p;
                n = n + 1;
                if (n < max) {
                    if (*p == '\'' || *p == '"') {
                        quote = *p;
                        state = 3;
                    } else {
                        state = 2;
                    }
                }
            } else {
                state = 1;
            }
        } else if (state == 2) {
            if (std::strchr(seps, *p) == nullptr) {
                if (*p == '\'' || *p == '"') {
                    quote = *p;
                    state = 3;
                }
            } else {
                *p = '\0';
                state = 1;
            }
        } else if (state == 3 && *p == quote) {
            state = 2;
        }
    }
    return n;
}

namespace {

bool is_2d3d_folder(const char* dir)
{
    // 140223790: dir ends in '\'; the char before it is d/D and the one before that 2/3
    const std::size_t n = std::strlen(dir);
    if (n < 3) return false;
    const char a = dir[n - 2], b = dir[n - 3];
    return (a == 'd' || a == 'D') && (b == '2' || b == '3');
}

} // namespace

// @src 140223790
void launch_program(std::int32_t mode, const char* cmdline, std::int32_t dir_kind)
{
    char base[0x110] = {};   // local acStack_709+1: DEFORM_PC_PATH with a trailing '\'
    char dir[0x110] = {};    // local_1458: base plus the 2D\ / 3D\ subfolder
    bool in_2d3d = false;    // local_948
    const char* env = std::getenv("DEFORM_PC_PATH");  // str 1405de5b0
    if (env != nullptr) {
        std::strcpy(base, env);
        const std::size_t n = std::strlen(base);
        // the original tests base[n-1]; for an empty value that is the byte before the buffer
        if (n == 0 || base[n - 1] != '\\') std::strcat(base, "\\");  // str 1405de5c0
        std::strcpy(dir, base);
        in_2d3d = is_2d3d_folder(dir);
        if (dir_kind == 2) {
            std::strcat(dir, in_2d3d ? "..\\2D\\" : "2D\\");  // str 1405de5c4 / 1405de5cc
        } else if (dir_kind == 3) {
            std::strcat(dir, in_2d3d ? "..\\3D\\" : "3D\\");  // str 1405de5d0 / 1405de5d8
        }
    }

    char line[0x410];  // local acStack_1879+1: trailing ' ', CR, LF removed
    std::strcpy(line, cmdline);
    char* e = line + std::strlen(line) - 1;
    while (line < e && (*e == ' ' || *e == '\r' || *e == '\n')) e = e - 1;
    e[1] = '\0';

    char* argv[24] = {};  // local_1348 (0xa8 bytes zeroed = 21 slots)
    char words[0x410];    // local_e68
    std::strcpy(words, line);
    const std::int32_t argc = split_words(words, " ,\t\n", argv, 0x15);  // str 1405de5a4

    char prog[0x110];  // local_5e8
    if (std::strlen(argv[0]) < 2 || (argv[0][0] != '\\' && argv[0][1] != ':')) {
        bool found = true;  // local_4cc
        std::strcpy(prog, dir);
        std::strcat(prog, argv[0]);
        if (!def::dbio::file_exists(prog)) {
            char probe[0x110];  // local_a58
            std::strcpy(probe, prog);
            std::strcat(probe, ".EXE");  // str 1405de5dc
            if (!def::dbio::file_exists(probe)) {
                std::strcpy(probe, prog);
                std::strcat(probe, ".COM");  // str 1405de5e4
                if (!def::dbio::file_exists(probe)) {
                    found = false;
                    if (dir_kind == 4 && in_2d3d) {
                        std::strcat(dir, "..\\");  // str 1405de5ec
                        std::strcpy(prog, dir);
                        std::strcat(prog, argv[0]);
                        if (def::dbio::file_exists(prog)) found = true;
                    }
                }
            }
        }
        if (!found) {
            std::strcpy(prog, base);
            std::strcat(prog, argv[0]);
        }
        argv[0] = prog;
    }

    if (mode != 1) {
        // '<' / '>' words through freopen on stdin/stdout, CreateProcessA without waiting: no
        // batch caller passes a mode other than 1 (all callers of 140223790/1402240d0/140224100/
        // 140224130/140224160 pass 1).
        DEF_NOT_PORTED(0x140223790);
    }
    char command[0x410];  // local_1288
    std::strcpy(command, "\"");     // str 1405de5f0
    std::strcat(command, "\"");     // str 1405de5f4
    std::strcat(command, argv[0]);
    std::strcat(command, "\"");     // str 1405de5f8
    for (std::int32_t i = 1; i < argc; i = i + 1) {
        std::strcat(command, " ");  // str 1405de5fc
        std::strcat(command, argv[i]);
    }
    std::strcat(command, "\"");     // str 1405de600
    if (g_launch_system_hook != nullptr) {
        g_launch_system_hook(command);
    } else {
        // Port addition (not in 140223790): the children inherit the error mode, so a program
        // whose DLL is missing (DEF_REM3_GUI -> 32-bit DEF_REM.exe without mpich.dll) fails
        // quietly instead of leaving a "System Error" dialog on the desktop
        const UINT mode = ::SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);
        (void)std::system(command);  // return value ignored
        ::SetErrorMode(mode);
    }
}

// @src 140224100
void launch_program_2d(std::int32_t mode, const char* cmdline) { launch_program(mode, cmdline, 2); }

// @src 140224130
void launch_program_3d(std::int32_t mode, const char* cmdline) { launch_program(mode, cmdline, 3); }

} // namespace def::cmd
