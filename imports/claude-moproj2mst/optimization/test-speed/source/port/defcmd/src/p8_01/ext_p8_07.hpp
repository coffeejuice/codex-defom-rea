// p8-01 seam to packet p8-07, owner of the launcher FUN_140223790 with its wrappers 140224100 /
// 140224130 (published: origin/claude/p8-07 @f9e7fa1, <defcmd/launch.hpp>, def::cmd::
// launch_program / launch_program_2d / launch_program_3d) and of the KEY file loader
// FUN_1401b5f00 (coordinator decision; not published yet).
//
// The other two wrappers the EXECMD case calls, 1402240d0 (dirKind 0) and 140224160 (dirKind 4),
// are one-line P3 ledger entries without an owner; they are ported here (launch_kind0/4) and call
// the owner's launch_program.
//
// The KEY loader is assumed as `def::cmd::read_keyword_file(model::DB_STEP_DATA* m, const char*
// name, const char* flag, const std::int32_t* mode)` in <defcmd/key_file.hpp>, the same guess
// p5-01's dispatcher makes (defcmd/src/dispatch/hooks.cpp); the coordinator relays the real name. Order in every wrapper: test hook, else the owner's function
// when its header is present, else DEF_NOT_PORTED.
#pragma once

#include <cstdint>
#include <cstdlib>

#include <defrt/port.hpp>

#if __has_include(<defcmd/launch.hpp>)
#include <defcmd/launch.hpp>
#define DEF_P801_HAVE_P8_07_LAUNCH 1
#endif
#if __has_include(<defcmd/key_file.hpp>)
#include <defcmd/key_file.hpp>
#define DEF_P801_HAVE_P8_07_KEYFILE 1
#endif

namespace def::model {
class DB_STEP_DATA;
} // namespace def::model

namespace def::cmd::p801ext {

// FUN_140223790(mode, cmdline, dir_kind); the hook sees the dir_kind of the wrapper that was called
using LaunchHook = void (*)(int mode, const char* cmdline, int dir_kind);
inline LaunchHook g_launch_hook = nullptr;

inline void launch(int mode, const char* cmdline, int dir_kind)
{
    if (g_launch_hook != nullptr) {
        g_launch_hook(mode, cmdline, dir_kind);
        return;
    }
#ifdef DEF_P801_HAVE_P8_07_LAUNCH
    switch (dir_kind) {
    case 2: def::cmd::launch_program_2d(mode, cmdline); return;  // 140224100
    case 3: def::cmd::launch_program_3d(mode, cmdline); return;  // 140224130
    default: def::cmd::launch_program(mode, cmdline, dir_kind); return;
    }
#else
    (void)mode;
    (void)cmdline;
    DEF_NOT_PORTED(0x140223790);
#endif
}

// @src 1402240d0  FUN_140223790(mode, cmdline, 0)
inline void launch_kind0(int mode, const char* cmdline) { launch(mode, cmdline, 0); }
// @src 140224160  FUN_140223790(mode, cmdline, 4)
inline void launch_kind4(int mode, const char* cmdline) { launch(mode, cmdline, 4); }
// 140224100 / 140224130 (p8-07's)
inline void launch_kind2(int mode, const char* cmdline) { launch(mode, cmdline, 2); }
inline void launch_kind3(int mode, const char* cmdline) { launch(mode, cmdline, 3); }

// FUN_1401b5f00(model, name, &flag, &mode): open + KEY interpreter; 0 ok, 1 cannot open
// ("ERROR: Keyword file %s open error!\n"), else the interpreter's status
using ReadKeyFileHook = int (*)(model::DB_STEP_DATA* m, const char* name, const char* flag, const std::int32_t* mode);
inline ReadKeyFileHook g_read_key_file_hook = nullptr;

inline int read_key_file(model::DB_STEP_DATA* m, const char* name, const char* flag, const std::int32_t* mode)
{
    if (g_read_key_file_hook != nullptr) return g_read_key_file_hook(m, name, flag, mode);
#ifdef DEF_P801_HAVE_P8_07_KEYFILE
    return static_cast<int>(def::cmd::read_keyword_file(m, name, flag, mode));
#else
    (void)m;
    (void)name;
    (void)flag;
    (void)mode;
    DEF_NOT_PORTED(0x1401b5f00);
#endif
}

// Not p8-07's: the CRT system() of EXECMD mode 1 (14047e190), with a hook so that tests need not
// run it.
using SystemHook = int (*)(const char* command);
inline SystemHook g_system_hook = nullptr;

inline int crt_system(const char* command)
{
    if (g_system_hook != nullptr) return g_system_hook(command);
    return std::system(command);
}

} // namespace def::cmd::p801ext
