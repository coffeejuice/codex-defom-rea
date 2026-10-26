// defcmd/launch.hpp: launching the unchanged external DEFORM programs (DEF_AMG, DEF_AMG3_GUI,
// MESH3D_GUI, BRKMSH, CHKVOL, DEF_INT, DEF_REM, COATINGMESH, ...). Owner: packet p8-07; used by
// p8-05..p8-08 and every other command that starts a program. Spec: docs/re/commands/common.md
// "Launching external programs".
//
// The program folder comes from the environment variable DEFORM_PC_PATH (the original sets it to
// its own folder at start-up, 1401b8e10; the port's caller or test sets it to the DEFORM install).
// The working directory is never changed: all exchange files are relative to the current one.
#pragma once

#include <cstdint>

namespace def::cmd {

// @src 140223790  launch `cmdline` (program name + arguments, redirections are ordinary words).
// mode 1: system("\"\"<prog>\" args...\"") and wait. dir_kind: 0 base folder, 2 "2D\", 3 "3D\",
// 4 also tries "..\" from a 2D\/3D\ base. Only mode 1 has callers in batch mode; the other modes
// (CreateProcessA without wait, '<'/'>' via freopen) abort as not ported.
void launch_program(std::int32_t mode, const char* cmdline, std::int32_t dir_kind);
// @src 140224100  launch_program(mode, cmdline, 2)
void launch_program_2d(std::int32_t mode, const char* cmdline);
// @src 140224130  launch_program(mode, cmdline, 3)
void launch_program_3d(std::int32_t mode, const char* cmdline);

// @src 140222e20  split `line` in place into at most `max` words separated by any char of `seps`;
// a word starting with ' or " runs to the matching quote (the quotes stay in the word). Returns
// the number of words; argv[0..max) is zeroed first.
std::int32_t split_words(char* line, const char* seps, char** argv, std::int32_t max);

// Test seam: when set, called with the final system() command line instead of system()
// (default null = the real system(); the program build never sets it).
using SystemHook = int (*)(const char* command);
inline SystemHook g_launch_system_hook = nullptr;

} // namespace def::cmd
