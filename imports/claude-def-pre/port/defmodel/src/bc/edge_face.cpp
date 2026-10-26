// The EC* array block (defmodel/bc/edge_face.hpp, packet kw-g2bc). Decompile:
// deform-rea-work/out/g2bc_ec/ec.c, elm.c, m2.c.
#include <defmodel/bc/edge_face.hpp>

#include <cstddef>

#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/cls_140185570.hpp>

namespace def::model::bc {

namespace {
constexpr std::size_t kBlock = 0xae8;  // Cls_140185570, the 12 EC arrays
template <class T>
T& at(LLOBJ_DATA& obj, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<std::byte*>(&obj) + off);
}
} // namespace

// @src 1401858d0  allocate the 12 arrays (int, int, double) x 4 with n entries each
void alloc_edge_face_arrays(Cls_140185570& block, std::int32_t n, std::int32_t mode)
{
    if (0 < n && (mode == -2 || mode == -1)) {
        block.arrays[0] = calloc_ints(n);
        block.arrays[1] = calloc_ints(n);
        block.arrays[2] = calloc_doubles(n);
        block.arrays[3] = calloc_ints(n);
        block.arrays[4] = calloc_ints(n);
        block.arrays[5] = calloc_doubles(n);
        block.arrays[6] = calloc_ints(n);
        block.arrays[7] = calloc_ints(n);
        block.arrays[8] = calloc_doubles(n);
        block.arrays[9] = calloc_ints(n);
        block.arrays[10] = calloc_ints(n);
        block.arrays[11] = calloc_doubles(n);
    }
}

// @src 1401632d0  free the 12 arrays (1401856e0 on obj+0xae8)
void free_edge_face_arrays(LLOBJ_DATA& obj) { at<Cls_140185570>(obj, kBlock).free_arrays(); }

// @src 1401631e0  free and reallocate the 12 arrays with NS entries
void realloc_edge_face_arrays(LLOBJ_DATA& obj)
{
    free_edge_face_arrays(obj);
    // The original branches on owner != 0 && 1401d2b70(owner) (which always returns 1); both
    // branches make the same call.
    std::int32_t n = edge_face_slot_count(obj);
    alloc_edge_face_arrays(at<Cls_140185570>(obj, kBlock), n, -1);
}

void* edge_face_array(LLOBJ_DATA& obj, int k) { return at<Cls_140185570>(obj, kBlock).arrays[k]; }

} // namespace def::model::bc
