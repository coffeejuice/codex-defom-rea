// defkey/globals_key_common.hpp: globals of the KEY line tokenizer (140413900 / 140413a50).
#pragma once

namespace def::key {

// @data 14070d060  line buffer of the tokenizer (fgets 0x400); tokens point into it
inline char g_token_line[0x400] = {};

// @data 14070cc60  token pointers (128 slots, strtok results into g_token_line)
inline char* g_tokens[0x80] = {};

// Clear both, as at process start.
void reset_globals();

} // namespace def::key
