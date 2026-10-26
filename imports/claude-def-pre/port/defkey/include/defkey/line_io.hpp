// defkey/line_io.hpp: the KEY reader's line helpers (KwdReadUtl family, 0x140413900-0x140416aa0).
// Spec: docs/re/keywords/_common.md section 2 and group2-helpers.md (branch j4c7t9).
//
// Every scalar reader does, in this order:
//   1. sets all its outputs to 0;
//   2. fgets(buf, 0x400, f) one line (normally the rest of the keyword's own line); EOF -> 1;
//   3. replaces every ',' with ' ' (140323970);
//   4. one sscanf with the binary's format (read through crt::sscanf, so "%Flf" works);
//   5. returns 1 when sscanf returned -1 (no conversion: empty or blank line), else 0.
// So a short line leaves the missing outputs at 0, a non-numeric first token gives all 0 and
// return 0, and text after the last conversion is ignored. "%i" accepts sign, 0x hex and leading-0
// octal ("010" is 8). A line longer than 1023 characters is split: the rest stays in the stream
// and is what the next read sees.
//
// Names: read_<fields> with i = int (%i), d = double (%Flf), digits = repeat count.
// Readers with more than five outputs take an array of pointers, in field order.
#pragma once

#include <cstdint>
#include <cstdio>

namespace def::key {

// fgets size every helper uses
inline constexpr int kLineSize = 0x400;

// @src 140323970: replace every `from` in the NUL-terminated `s` with `to`
void replace_char(char* s, char from, char to);

// ---- int readers ----------------------------------------------------------------------------
// 140413de0 "%i". skip_blank: when the first line read is exactly "\n", read one more (only one).
int read_i(std::FILE* f, std::int32_t* a, bool skip_blank = false);
// 140413ee0 "%i %i"
int read_ii(std::FILE* f, std::int32_t* a, std::int32_t* b);
// 140413fb0 "%i %i %i"; nconv (may be null) receives the sscanf result, -1 included
int read_iii(std::FILE* f, std::int32_t* a, std::int32_t* b, std::int32_t* c, int* nconv = nullptr);
// 140414100 "%i %i %i %i"
int read_i4(std::FILE* f, std::int32_t* a, std::int32_t* b, std::int32_t* c, std::int32_t* d);
// 140414200 "%i" x5. skip_blank_lines: keep reading while the line is exactly "\n".
int read_i5(std::FILE* f, std::int32_t* a, std::int32_t* b, std::int32_t* c, std::int32_t* d,
            std::int32_t* e, bool skip_blank_lines = false);
// 1404144b0 "%i" x7
int read_i7(std::FILE* f, std::int32_t* const out[7]);
// 140414600 "%i" x9; skip_blank_lines as read_i5
int read_i9(std::FILE* f, std::int32_t* const out[9], bool skip_blank_lines = false);
// 1404147e0 "%i" x10 into a local array, then copies min(*n, 10) ints to dst (n is not updated).
// The sscanf result is ignored: returns 1 only on EOF. Not zeroed first: fields missing from the
// line copy whatever the stack held in the original (0 in the port).
int read_i10(std::FILE* f, std::int32_t* dst, const int* n);

// ---- double readers -------------------------------------------------------------------------
int read_d(std::FILE* f, double* a);                                   // 140414990 "%Flf"
int read_dd(std::FILE* f, double* a, double* b);                       // 140414a40
int read_d3(std::FILE* f, double* a, double* b, double* c);            // 140414c20
// 140414d10 "%Flf %Flf %Flf", but exactly one converted field resets all three to 0
int read_d3_not_one(std::FILE* f, double* a, double* b, double* c);
int read_d4(std::FILE* f, double* a, double* b, double* c, double* d); // 140414e40
int read_d5(std::FILE* f, double* const out[5]);                       // 140414f50
int read_d6(std::FILE* f, double* const out[6]);                       // 140415080
int read_d7(std::FILE* f, double* const out[7]);                       // 1404151c0
int read_d9(std::FILE* f, double* const out[9]);                       // 140415320

// ---- mixed readers --------------------------------------------------------------------------
int read_i_d(std::FILE* f, std::int32_t* a, double* b);                            // 1404154c0
int read_i_d_i(std::FILE* f, std::int32_t* a, double* b, std::int32_t* c);         // 140415590
int read_i_dd(std::FILE* f, std::int32_t* a, double* b, double* c);                // 140415680
int read_i_d3(std::FILE* f, std::int32_t* a, double* b, double* c, double* d);     // 140415880
int read_i_d4(std::FILE* f, std::int32_t* a, double* const d[4]);                  // 140415990
int read_i_d5(std::FILE* f, std::int32_t* a, double* const d[5]);                  // 140415ac0
int read_i_d7(std::FILE* f, std::int32_t* a, double* const d[7]);                  // 140415d70
int read_ii_d(std::FILE* f, std::int32_t* a, std::int32_t* b, double* c);          // 140415f00
int read_ii_dd(std::FILE* f, std::int32_t* a, std::int32_t* b, double* c, double* d); // 140415ff0
int read_ii_d3(std::FILE* f, std::int32_t* a, std::int32_t* b, double* c, double* d,
               double* e);                                                         // 140416100
int read_ii_d4(std::FILE* f, std::int32_t* a, std::int32_t* b, double* const d[4]); // 140416220
int read_iii_d(std::FILE* f, std::int32_t* a, std::int32_t* b, std::int32_t* c, double* d); // 1404164e0
int read_iii_dd(std::FILE* f, std::int32_t* a, std::int32_t* b, std::int32_t* c, double* d,
                double* e);                                                        // 1404165e0
int read_iii_d_i(std::FILE* f, std::int32_t* a, std::int32_t* b, std::int32_t* c, double* d,
                 std::int32_t* e);                                                 // 140416700
int read_d_i(std::FILE* f, double* a, std::int32_t* b);                            // 140416aa0

// ---- strings --------------------------------------------------------------------------------
// 140413ab0: reads two lines; the first (rest of the keyword line) is discarded, the second is the
// value. Cuts at the first '\n', strncpy(dst, line, n-1) (NUL-padded, not terminated if longer),
// then trims trailing NUL/space/'\n'/'\r' scanning dst[n-2] down to dst[0]. Tabs and leading
// spaces stay. EOF on either line -> 1 (dst untouched).
int read_string_next_line(std::FILE* f, char* dst, int n);
// 140413c60: the same on a single line (no discarded line).
int read_string_line(std::FILE* f, char* dst, int n);

// ---- tokenizer ------------------------------------------------------------------------------
// 140413900: fgets into g_token_line (EOF -> 1), clears token slots 0..min(max_tokens,128)-1,
// strtok(" ,\n\t\r") into g_tokens. Slot 0 is always counted, so a blank line gives *count = 1
// with slot 0 null. Slots past max_tokens keep tokens of earlier lines. Returns 0.
int read_tokens(std::FILE* f, int* count, int max_tokens);
// 140413a50: token i, or "" for i outside 0..127 or an empty slot.
const char* token(int i);
// Conversions the callers use on tokens: strtol(s, NULL, 10) (14047b6c0, decimal only, unlike %i)
// and atof (14048754c).
std::int32_t token_int(int i);
double token_double(int i);

} // namespace def::key
