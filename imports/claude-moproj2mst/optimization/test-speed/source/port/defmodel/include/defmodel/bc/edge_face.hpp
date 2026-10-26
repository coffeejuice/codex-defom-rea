// defmodel/bc/edge_face.hpp: the 12-array edge/face boundary-condition block of an object
// (obj+0xae8, Cls_140185570; EC* keywords, packet kw-g2bc): pointers (int, int, double) x 4 in the
// order ECCDEF ECDEFN ECPRES ECCTMP ECTMFN ECHFLX ECCATM ECATFN ECAFLX ECCRHT ECRHFN ECRFLX, each
// with one entry per boundary slot (defmodel/bc/boundary_topology.hpp).
// Spec: docs/re/keywords/ECCDEF.md "Model storage".
#pragma once

#include <cstdint>

#include <defmodel/bc/boundary_topology.hpp>

namespace def::model {

class LLOBJ_DATA;
class Cls_14017e740;
class Cls_140185570;

namespace bc {

// 1401858d0: allocate the 12 arrays with n entries each (mode -1 or -2, n > 0), zero filled.
void alloc_edge_face_arrays(Cls_140185570& block, std::int32_t n, std::int32_t mode);
// 1401632d0: free the 12 arrays (Cls_140185570::free_arrays, 1401856e0).
void free_edge_face_arrays(LLOBJ_DATA& obj);
// 1401631e0: free, then allocate the 12 arrays with edge_face_slot_count entries.
void realloc_edge_face_arrays(LLOBJ_DATA& obj);

// Array k (0..11) of the block at obj+0xae8 (int* for k % 3 != 2, double* for k % 3 == 2).
void* edge_face_array(LLOBJ_DATA& obj, int k);

} // namespace bc
} // namespace def::model
