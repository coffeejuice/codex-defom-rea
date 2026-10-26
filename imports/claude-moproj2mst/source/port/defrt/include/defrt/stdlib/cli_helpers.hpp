// defrt/stdlib/cli_helpers.hpp: the small helpers of DEF_PRE_64's command-line code that sit in
// the runtime range (packet p1-01). The -m CLI port (def_pre/m_cli.cpp) kept local copies of
// these; they live here so that every caller shares one tagged port.
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>

namespace def::rt {

// FUN_14001b4e0: the -h help screen, on stdout (__iob_func() + 0x30).
void print_help();
// The same text on any stream (tier-1 tests).
void print_help(std::FILE* f);

// FUN_14001b700: removes one leading and one trailing double quote, in place. The string is
// copied into a 1024-byte stack buffer first; a longer value overruns the stack cookie and the
// process ends with 0xC0000409 (abnormal_exit). The result is terminated at the old end minus
// the trailing quote, not at end - start: "\"ab\"" becomes "abb". Callers never pass "".
void strip_quotes(char* s);

// FUN_14001c090: splits s with strtok into table[0..]; *count = number of tokens and
// table[*count] is the terminating null pointer. The table needs room for the tokens plus one.
void split_tokens(char* s, const char* delim, char** table, std::int32_t* count);

// FUN_14001bfe0: first and last stored step of operation *op in the DB directory.
//   records     the directory array (0x28-byte entries; the info object keeps it at +0xad8):
//               +0x04 step, +0x14 simulation (operation) number, +0x1c dimension,
//               +0x24 stored flag
//   nrec        number of entries
//   dim         +0x1c of the operation's first stored entry, first_step its step
//   last_step   step of its last stored entry (unchanged when there is only one)
// Returns true when the operation has a stored entry; otherwise nothing is written.
inline constexpr std::size_t kDirRecordSize = 0x28;
bool operation_steps(const std::int32_t* op, const void* records, const std::int32_t* nrec,
                     std::int32_t* dim, std::int32_t* first_step, std::int32_t* last_step);

} // namespace def::rt
