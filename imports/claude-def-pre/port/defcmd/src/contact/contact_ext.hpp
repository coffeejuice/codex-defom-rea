// Calls from the contact commands into functions no packet has ported yet: the on-demand DB
// readers of the node and element sets (objects read from a .DB keep their arrays in the DB
// step until used). Not reached for objects read from KEY files (set +0x130 / +0xf0 are null).
#pragma once

#include <cstdint>

namespace def::cmd::ext {

// 1401d48d0: read the node set's arrays from its DB step (ReadSingleAArrayFromDB family)
void fun_1401d48d0(void* step, std::int32_t* key);
// 1401d5820: read the element set's arrays from its DB step (ReadSingleMARArrayFromDB family)
void fun_1401d5820(void* step, std::int32_t* key);

} // namespace def::cmd::ext
