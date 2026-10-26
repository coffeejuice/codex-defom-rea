// def_pre/m_cli.hpp: the -m utility path of DEF_PRE_64 (packet m-cli).
//
//   DEF_PRE.EXE [-h] [-m {header|stepList|lastStepNo|simStepList}] [-d <db name>] [-o <file name>]
//
// main FUN_14001c9f0 creates NEWEQ.MSG (FUN_14001ce7b), parses the command line
// (FUN_14001b7b0) and, when any option set a flag, runs the dispatcher FUN_14001c500 and exits 0.
// Hidden options the parser also takes: a second -d, -s (twice), -r and -t; only -t is used
// (simStepList step lists). "summary", "reclist" and "diff" are parsed but have no mode.
#pragma once

#include <cstdint>
#include <cstdio>

#include <defmodel/layout/cls_1401dbfe0.hpp>
#include <defrt/stdlib/cli_helpers.hpp>
#include <defrt/stdlib/crt_emulation.hpp>

namespace def::pre {

// The DB info object of the -m modes is p1-03's Cls_1401dbfe0 (loaded by
// def::dbio::load_db_info); the small helpers are p1-01's (defrt/stdlib/cli_helpers.hpp).
using DbInfo = def::model::Cls_1401dbfe0;
using def::rt::abnormal_exit;
using def::rt::kStatusAccessViolation;
using def::rt::kStatusInvalidCrtParameter;
using def::rt::kStatusStackBufferOverrun;
using def::rt::print_help;
using def::rt::set_abnormal_exit_hook;
using def::rt::strip_quotes;

// Flags returned by the parser (FUN_14001b7b0).
inline constexpr std::uint32_t kFlagOut = 0x01;  // -o
inline constexpr std::uint32_t kFlagDb = 0x02;   // first -d
inline constexpr std::uint32_t kFlagDb2 = 0x04;  // second -d
inline constexpr std::uint32_t kFlagS1 = 0x10;   // first -s
inline constexpr std::uint32_t kFlagS2 = 0x20;   // second -s
inline constexpr std::uint32_t kFlagR = 0x40;    // -r
inline constexpr std::uint32_t kFlagT = 0x80;    // -t

// Values main passes to the parser and the dispatcher (its locals at RSP+0x50 .. 0xa450).
struct MArgs {
    std::int32_t mode = 0;  // RSP+0x50: 1 header, 2 stepList, 3 lastStepNo, 4 simStepList,
                            //           5 diff, 6 reclist, 7 summary
    char out[0x400] = {};   // RSP+0xa050  -o
    char db[0x400] = {};    // RSP+0x9c50  first -d
    char db2[0x400] = {};   // RSP+0xa450  second -d
    std::int32_t s1 = 0;    // RSP+0x5c
    std::int32_t s2 = 0;    // RSP+0x64
    std::int32_t r = 0;     // RSP+0x60
    std::int32_t t = 0;     // RSP+0x54  step-list style of simStepList
};

// Thrown where the original calls exit(code); main returns the code (stdio is flushed).
struct ExitRequest {
    int code;
};

// FUN_14001c9f0, the part before the keyword model is built. Returns the exit code when the -m
// path ran (or -h), or -1 through `*handled = false` when no option set a flag and the
// interactive/stdin path (not ported yet) would follow.
int main_m(int argc, char** argv, bool* handled);

// FUN_14001ce7b(-1): unit 1 = NEWEQ.MSG, STATUS='UNKNOWN' / CLOSE 'DELETE' / again 'UNKNOWN'.
void open_neweq_msg();

// FUN_14001b7b0.
std::uint32_t parse_m_args(int argc, char** argv, MArgs& a);



// FUN_14001c500.
void dispatch_m(std::uint32_t flags, MArgs& a);

// The four modes: FUN_14001bc80, FUN_14001be50, FUN_14001bf40, FUN_14001c100.
bool mode_header(char* db, DbInfo& info, const char* out);
bool mode_step_list(char* db, DbInfo& info, const char* out);
bool mode_last_step_no(char* db, DbInfo& info, const char* out);
bool mode_sim_step_list(char* db, DbInfo& info, const char* out, const std::int32_t* t);

// Output writers of the modes, split out so that tier-1 tests can feed them an info object.
// part of @src 14001bc80 / 14001be50 / 14001bf40 / 14001c100
void write_header(std::FILE* f, const DbInfo& info);
void write_step_list(std::FILE* f, const DbInfo& info);
void write_last_step_no(std::FILE* f, const DbInfo& info);
// Returns false when no operation has a stored step (then nothing is opened or written).
bool write_sim_step_list(const char* out, const DbInfo& info, std::int32_t t);

} // namespace def::pre
