// defmodel/step/name_arrays.hpp: the user-variable name arrays of DB_STEP_DATA (UNNAME node names
// at +0x3398/+0x33a0, UENAME element names at +0x33a8/+0x33b0). Packet kw-g1b.
//
// Each array is malloc'd: `count` pointers, each to a calloc(0x40, 1) string.
#pragma once

#include <cstdint>

namespace def::model {

class DB_STEP_DATA;

// FUN_1401d0fc0(model, flag): kw-g4b's step_remap_material_state (defmodel/material/state_remap.hpp),
// called by the UNNAME/UENAME resizes, GEOTYP/STNOUT/ELMNOD (kw-g1a) and kw-g57. call_1401d0fc0 runs
// it, or the test hook when one is installed.
inline void (*g_hook_1401d0fc0)(DB_STEP_DATA* m, std::int32_t flag) = nullptr;
void call_1401d0fc0(DB_STEP_DATA* m, std::int32_t flag);

// 1401cb910 / 1401cb660: free every name and the array, set the array pointer to null (the count
// is left as it is).
void free_node_names(DB_STEP_DATA* m);
void free_element_names(DB_STEP_DATA* m);

// 1401cba80 / 1401cb7d0: resize the array from *old_n to *new_n entries: new_n == 0 frees all;
// otherwise frees the names past new_n, reallocs the array and callocs 0x40 bytes per new entry.
// The count member is not touched (the callers store it).
void realloc_node_names(DB_STEP_DATA* m, const std::int32_t* old_n, const std::int32_t* new_n);
void realloc_element_names(DB_STEP_DATA* m, const std::int32_t* old_n, const std::int32_t* new_n);

// 1401d21c0 / 1401d2090: set the count to n when it differs (realloc above, store the count,
// then FUN_1401d0fc0(m, 0)). UNNAME / UENAME call these.
void resize_node_names(DB_STEP_DATA* m, std::int32_t n);
void resize_element_names(DB_STEP_DATA* m, std::int32_t n);

// 1401cbbc0 / 1401cbc80: the count, as the KEY writer asks for it. In 3D they would first rebuild
// the names (1401cf930 / 1401cfc30) when 1401d2b70 returns 0, but 1401d2b70 always returns 1.
std::int32_t node_name_count(DB_STEP_DATA* m);
std::int32_t element_name_count(DB_STEP_DATA* m);

// 1401d2b70: always 1 (when the byte at +0x1b58 is 0 it evaluates the 3D test and drops it).
bool names_up_to_date(DB_STEP_DATA* m);

// 1401d48d0(model, &id): mark the model's DB output variables of state variable *id. It returns
// at once when 1401d2b70 is true, which it always is, so the marking body (calls of 1401d4400 per
// variable class) is never reached. statevar_kw's g_hook_1401d48d0 defaults to this.
void mark_state_var_outputs(DB_STEP_DATA* m, const std::int32_t* id);

}  // namespace def::model
