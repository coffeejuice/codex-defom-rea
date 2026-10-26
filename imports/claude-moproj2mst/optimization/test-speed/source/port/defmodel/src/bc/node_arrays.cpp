// defmodel/bc/node_arrays.cpp: node / element counts and array allocation of an object
// (defmodel/bc/node_arrays.hpp). Decompiles: deform-rea-work/out/g2bc_nodal/h.c (1401652b0),
// alloc.c (14017ffe0, 140163270, 1401653b0, 140162ff0, 140163090), alloc2.c (1401632a0, 1401836c0).
#include <defmodel/bc/node_arrays.hpp>

#include <cstddef>
#include <cstdlib>

#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/cls_14017ef20.hpp>
#include <defmodel/llobj/cls_140182880.hpp>
#include <defmodel/llobj/ext.hpp>
#include <defmodel/members/ext_deps.hpp>
#include <defrt/rdata.hpp>

namespace def::model::bc {

namespace {

template <class T, class C>
T& at(C& base, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<std::byte*>(&base) + off);
}

// rdata 14051f1f0  20.0 (room temperature, unit system 1)
constexpr double kRoomTempSI = def::rt::rdata::d_14051f1f0;
// rdata 14051f1f8  68.0 (room temperature, otherwise)
constexpr double kRoomTempEng = def::rt::rdata::d_14051f1f8;
// rdata 140509c90  1.0
constexpr double kOne = def::rt::rdata::d_140509c90;

// FUN_140257150 / FUN_140256dc0: calloc(n, 8) / calloc(n, 4), NULL for n == 0. On a failed
// calloc the binary prints "%s Error when allocating %s memory" (14039d3a0) and returns NULL;
// not reproduced.
double* alloc_doubles(std::int32_t n) { return n == 0 ? nullptr : static_cast<double*>(std::calloc(n, 8)); }
std::int32_t* alloc_ints(std::int32_t n)
{
    return n == 0 ? nullptr : static_cast<std::int32_t*>(std::calloc(n, 4));
}

std::int32_t mul(std::int32_t a, std::int32_t b)
{
    return static_cast<std::int32_t>(static_cast<std::uint32_t>(a) * static_cast<std::uint32_t>(b));
}

}  // namespace

// @src 14017ffe0  node set alloc: coordinates if not yet there, int BC arrays, state variables
void node_set_alloc(void* set, std::int32_t n, const std::int32_t* dims, std::int32_t key)
{
    auto& s = *static_cast<std::byte*>(set);
    if (0 < n) {
        if ((key == 0 || key == -2 || key == -1) && at<double*>(s, 8) == nullptr)
            at<double*>(s, 8) = alloc_doubles(mul(n, dims[0]));
        if (key == -2 || key == -1) {
            for (int k = 0; k < 6; ++k)
                at<std::int32_t*>(s, 0xf0 + 8 * k) = alloc_ints(mul(n, dims[0x12 + k]));
            at<std::int32_t>(s, 300) = n;
        }
        llobj::ext::f140329e60(set, n, dims, key);
    }
}

// @src 140163270  free the node arrays (14017f130 on obj+0x8a0)
void free_node_arrays(LLOBJ_DATA& obj) { at<Cls_14017ef20>(obj, 0x8a0).free_arrays(); }

// @src 140162ff0  free and reallocate the node arrays for obj+0x880 nodes
void alloc_node_arrays(LLOBJ_DATA& obj, const std::int32_t* dims)
{
    free_node_arrays(obj);
    // The binary tests the model (obj+0x3a98) with 1401d2b70, which always returns 1; both
    // branches make the same call.
    node_set_alloc(&at<std::byte>(obj, 0x8a0), at<std::int32_t>(obj, 0x880), dims, -1);
}

// @src 1401652b0  set the node count, reallocate, room temperature into obj+0x8e0
void set_node_count(LLOBJ_DATA& obj, std::int32_t n, const std::int32_t* dims)
{
    at<std::int32_t>(obj, 0x880) = n;
    alloc_node_arrays(obj, dims);
    if (dims != nullptr) {
        std::int32_t cnt = mul(n, dims[8]);  // dims+0x20
        double* t = at<double*>(obj, 0x8e0);
        if (ext::fun_140235c10() == 1) {
            for (std::int32_t i = 0; i < cnt; ++i) t[i] = kRoomTempSI;
        } else {
            for (std::int32_t i = 0; i < cnt; ++i) t[i] = kRoomTempEng;
        }
    }
}

// @src 1401836c0  element set alloc: connectivity (n x m) and the +0x18 ints, state variables
void element_set_alloc(void* set, std::int32_t n, const std::int32_t* dims, std::int32_t m, std::int32_t key)
{
    auto& s = *static_cast<std::byte*>(set);
    if (0 < n) {
        if (key == -2 || key == -1) {
            at<std::int32_t*>(s, 8) = alloc_ints(mul(n, m));
            at<std::int32_t*>(s, 0x18) = alloc_ints(mul(n, dims[0x3d]));  // dims+0xf4
            at<std::int32_t>(s, 0xe8) = n;
            at<std::int32_t>(s, 0xec) = m;
        }
        llobj::ext::f140329e60(set, n, dims, key);
    }
}

// @src 1401632a0  free the element arrays (140182a50 on obj+0x9e0)
void free_element_arrays(LLOBJ_DATA& obj) { at<Cls_140182880>(obj, 0x9e0).free_arrays(); }

// @src 140163090  free and reallocate the element arrays for obj+0x884 elements
void alloc_element_arrays(LLOBJ_DATA& obj, const std::int32_t* dims)
{
    free_element_arrays(obj);
    DB_STEP_DATA* model = at<DB_STEP_DATA*>(obj, 0x3a98);
    void* set = &at<std::byte>(obj, 0x9e0);
    const std::int32_t n = at<std::int32_t>(obj, 0x884);
    // 1400015e0(model): model+0x1c == 2. The other branch tests 1401d2b70 (always 1) with the
    // same call on both sides.
    if (at<std::int32_t>(*model, 0x1c) != 2) {
        element_set_alloc(set, n, dims, at<std::int32_t>(obj, 0x894), -1);
    } else if (at<std::int32_t>(obj, 0x39c0) == 3) {
        element_set_alloc(set, n, dims, at<std::int32_t>(obj, 0x894), -1);
    } else {
        element_set_alloc(set, n, dims, 4, -1);
    }
}

// @src 1401653b0  set the element count, reallocate, fill obj+0x9f8 / obj+0xa30 / obj+0xa38
void set_element_count(LLOBJ_DATA& obj, std::int32_t n, const std::int32_t* dims)
{
    at<std::int32_t>(obj, 0x884) = n;
    alloc_element_arrays(obj, dims);
    if (0 < at<std::int32_t>(obj, 0x3980)) {
        const std::int32_t w = dims[0x3d];  // dims+0xf4
        std::int32_t* p = at<std::int32_t*>(obj, 0x9f8);
        for (std::int32_t i = 0; i < mul(n, w); ++i) p[i] = at<std::int32_t>(obj, 0x3980);
    }
    const std::int32_t w1 = dims[0x2c];  // dims+0xb0
    double* a = at<double*>(obj, 0xa30);
    for (std::int32_t i = 0; i < mul(n, w1); ++i) a[i] = kOne;
    const std::int32_t w2 = dims[0x2d];  // dims+0xb4
    double* b = at<double*>(obj, 0xa38);
    for (std::int32_t i = 0; i < mul(n, w2); ++i) b[i] = 0.0;
}

}  // namespace def::model::bc
