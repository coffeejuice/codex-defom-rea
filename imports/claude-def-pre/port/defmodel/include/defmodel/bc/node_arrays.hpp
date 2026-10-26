// defmodel/bc/node_arrays.hpp: the node and element counts of an object and the allocation of
// its per-node / per-element arrays (owner: kw-g2bc nodal family; these functions are in the p1-04
// address range but in no packet's status file).
//
// Node arrays live in the node state-variable set at LLOBJ_DATA+0x8a0 (Cls_14017ef20, sized by
// the node count obj+0x880; p1-04's header calls it the "element" set, which is wrong: RZ sizes it):
//   obj+0x8a8 coordinates (n x dims[0] doubles), obj+0x990..0x9b8 the int BC arrays BCCDEF,
//   BCCDFN, BCCTMP, BCCTFN, BCCCRB, BCCRHT (n x dims[0x12..0x17]), and the double state
//   variables in the slots obj+0x8b0..0x988 (CRBFLX obj+0x910 = id 0x0e, RHTFLX obj+0x920 =
//   id 0x10, temperature obj+0x8e0 = id 8), which the state variables allocate themselves
//   (140329e60, virtual slot 2 of each, p1-09).
// Element arrays live in the set at LLOBJ_DATA+0x9e0 (Cls_140182880, sized by the element count
// obj+0x884): obj+0x9e8 connectivity (n x obj+0x894 ints), obj+0x9f8 (n x dims[0x3d] ints) and
// the state variables obj+0xa00..0xab8 (HDNOBJ obj+0xa90 = id 0x38).
// `dims` is the model's component table DB_STEP_DATA+0x15f8 (0x4f ints indexed by state-variable
// code; filled by 1401d09c0, p1-03).
#pragma once

#include <cstdint>

namespace def::model {
class LLOBJ_DATA;
}

namespace def::model::bc {

// 1401652b0: obj+0x880 = n, reallocate the node arrays (140162ff0), then set every temperature
// entry (obj+0x8e0, n x dims[8]) to 20.0 (unit system 1) or 68.0 (otherwise). Called by RZ.
void set_node_count(LLOBJ_DATA& obj, std::int32_t n, const std::int32_t* dims);
// 140162ff0: free the node arrays (140163270), then 14017ffe0(obj+0x8a0, obj+0x880, dims, -1).
void alloc_node_arrays(LLOBJ_DATA& obj, const std::int32_t* dims);
// 140163270: Cls_14017ef20::free_arrays (14017f130) on obj+0x8a0.
void free_node_arrays(LLOBJ_DATA& obj);
// 14017ffe0: allocate the node set `set` (obj+0x8a0) for n nodes (see the .cpp).
void node_set_alloc(void* set, std::int32_t n, const std::int32_t* dims, std::int32_t key);

// 1401653b0: obj+0x884 = n, reallocate the element arrays (140163090), fill obj+0x9f8 with
// obj+0x3980 when that is > 0, obj+0xa30 with 1.0 and obj+0xa38 with 0.0. Called by ELMCON.
void set_element_count(LLOBJ_DATA& obj, std::int32_t n, const std::int32_t* dims);
// 140163090: free the element arrays (1401632a0), then 1401836c0(obj+0x9e0, obj+0x884, dims,
// nodes per element, -1); nodes per element = obj+0x894, except 4 in 2D when obj+0x39c0 != 3.
void alloc_element_arrays(LLOBJ_DATA& obj, const std::int32_t* dims);
// 1401632a0: Cls_140182880::free_arrays (140182a50) on obj+0x9e0.
void free_element_arrays(LLOBJ_DATA& obj);
// 1401836c0: allocate the element set `set` (obj+0x9e0) for n elements of m nodes.
void element_set_alloc(void* set, std::int32_t n, const std::int32_t* dims, std::int32_t m, std::int32_t key);

}  // namespace def::model::bc
