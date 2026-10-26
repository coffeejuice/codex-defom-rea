// def_pre/menu7.hpp: main menu item 7, "DB generation" (FUN_14000a370), and the console input and
// string helpers it uses (Phase 4 packet p4-01; the main menu loop that calls them is Phase 5).
// Input comes from std::cin (the original's istream at PTR_DAT_14063a030), output goes to stdout.
#pragma once

#include <cstdint>
#include <string>

namespace def::model {
class Cls_14001c770;
}

namespace def::pre {

// 14000a370: asks "Old DB / New DB", the DB name, overwrite confirmation, then runs GENDB
// (check 1, dbopt 1 or 2) on the main object's model. Gives up after 20 trials with exit(-1).
void menu_db_generation(model::Cls_14001c770& main);

// ---- console input -----------------------------------------------------------------------------
// 140009c50: flush stdout, getline (255 chars) from cin, trimmed (1401b1060); a line of blanks
// becomes empty; a final "\n" left by the trim is dropped.
std::string read_line(model::Cls_14001c770& main);
// 140008600: prints `prompt`, reads a line; empty input takes the text at prompt[0x17] (the
// default the prompt shows). Not a number: -0xf423d (0xfff0bdc3). In [lo, hi]: the number. Else,
// with `complain`, " ??? Invalid Data; Valid Data Range = (...)" and ask again.
std::int32_t read_int(model::Cls_14001c770& main, std::int32_t lo, std::int32_t hi,
                      const std::string& prompt, bool complain);
// 140009e90: "\n Hit <CR> to continue ..." and one line of input.
void hit_return();

// ---- string helpers ----------------------------------------------------------------------------
// 1401b1060: trims tab, LF, VT, FF, CR, blank and '?' on both sides (an all-blank string is kept)
std::string trim(std::string s);
// 1401b1390: characters [start, size-start) are all digits (an empty range is true)
bool is_digits(std::string s, std::int32_t start);
// 1401b0870: sscanf("%d"), 0 when it does not convert
std::int32_t to_int(std::string s);
// 1401b0940: s starts with p (ASCII case folded unless `exact`)
bool starts_with(std::string s, const char* p, bool exact);
// 1401b0cc0: s ends with p, case folded, and is strictly longer than p
bool ends_with(std::string s, const char* p);
// 140002a60: vsprintf_s(buf, 200, fmt, ...)
void format200(char* buf, const char* fmt, ...);

} // namespace def::pre
