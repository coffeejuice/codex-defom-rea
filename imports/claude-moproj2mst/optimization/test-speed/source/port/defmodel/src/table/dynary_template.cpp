// DYNARY (Cls_14012d050, the 0x78-byte table / curve object): the template family
// (defmodel/dynary_init.hpp). Spec: docs/re/keywords/_common.md section 3. Owned by key-common; the
// ctor/dtor stay in src/layout/dynary.cpp (p1-06).
//
// The arrays are allocated with calloc and released with free, like the original, so a table
// filled here can be released by the original-style free path and vice versa.
#include <defmodel/dynary_init.hpp>

#include <cstdlib>
#include <cstring>
#include <defrt/rdata.hpp>

namespace def::model {

namespace {
// rdata 140509c90  1.0 (default constant of template 0xae)
constexpr double kTemplateAeDefault = def::rt::rdata::d_140509c90;
} // namespace

// @src 14012f480  free the axis and value arrays unless one of them looks shared
// (a dimension with count 0 but a non-null array marks the table as borrowing its arrays);
// all four pointers are cleared either way. Also listed by p1-06; one copy goes at merge.
void dynary_free_arrays(DYNARY* t)
{
    bool shared = false;
    if (t->n[0] == 0 && t->axis[0] != nullptr) shared = true;
    if (t->n[1] == 0 && t->axis[1] != nullptr) shared = true;
    if (t->n[2] == 0 && t->axis[2] != nullptr) shared = true;
    if (0 < t->n[0] && !shared) {
        if (t->axis[0] != nullptr) std::free(t->axis[0]);
        t->axis[0] = nullptr;
    }
    if (0 < t->n[1] && !shared) {
        if (t->axis[1] != nullptr) std::free(t->axis[1]);
        t->axis[1] = nullptr;
    }
    if (0 < t->n[2] && !shared) {
        if (t->axis[2] != nullptr) std::free(t->axis[2]);
        t->axis[2] = nullptr;
    }
    if (!shared) {
        if (t->values != nullptr) std::free(t->values);
        t->values = nullptr;
    }
    t->values = nullptr;
    t->axis[2] = nullptr;
    t->axis[1] = nullptr;
    t->axis[0] = nullptr;
}

// @src 14012fc00  clamp the shape before allocation
void dynary_clamp_shape(DYNARY* t)
{
    if (t->n[0] < 0) t->n[0] = 0;
    if (t->n[1] < 0) t->n[1] = 0;
    if (t->n[2] < 0) t->n[2] = 0;
    if (t->ncomp < 1) t->ncomp = 1;
}

// @src 14012ff20  number of doubles in `values`. ndim 0: ncomp when ncomp >= 2 or the type is one of
// 0xb5/0xa7/0xa8/0xaa/0xab, else 0. ndim > 0: ncomp * n[0..ndim-1] (int32 product, < 0 -> 0).
std::int32_t dynary_value_count(const DYNARY* t)
{
    std::int32_t total = t->ncomp;
    if (t->ndim == 0) {
        if (t->ncomp < 2 && t->type != 0xb5 && t->type != 0xa7 && t->type != 0xa8 && t->type != 0xaa && t->type != 0xab)
            total = 0;
        else
            total = t->ncomp;
    } else {
        // The original indexes int [7 + i] for i < ndim with no clamp to 3; templates and
        // headers only produce ndim <= 3, so n[i] is the same memory. The product wraps like the
        // original's 32-bit imul.
        for (std::int32_t i = 0; i < t->ndim; ++i)
            total = static_cast<std::int32_t>(static_cast<std::uint32_t>(total) *
                                              static_cast<std::uint32_t>(t->n[i]));
        if (total < 0) total = 0;
    }
    return total;
}

// @src 14012f610  true when n[i] >= 1 for every i < ndim (true for ndim <= 0)
bool dynary_counts_positive(const DYNARY* t)
{
    for (std::int32_t i = 0; i < t->ndim; ++i) {
        if (t->n[i] < 1) return false;
    }
    return true;
}

// @src 14012fad0  (re)allocate: free, clamp, calloc each axis with n[i] doubles and the value
// array with value_count() doubles (each then memset to 0, which calloc already did).
void dynary_alloc(DYNARY* t)
{
    dynary_free_arrays(t);
    dynary_clamp_shape(t);
    for (std::int32_t i = 0; i < t->ndim; ++i) {
        if (0 < t->n[i]) {
            t->axis[i] = static_cast<double*>(std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(t->n[i])), 8));
            if (t->axis[i] != nullptr)
                std::memset(t->axis[i], 0, static_cast<std::size_t>(static_cast<std::int64_t>(t->n[i])) << 3);
        }
    }
    if (0 < dynary_value_count(t)) {
        t->values = static_cast<double*>(std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(dynary_value_count(t))), 8));
        if (t->values != nullptr)
            std::memset(t->values, 0, static_cast<std::size_t>(static_cast<std::int64_t>(dynary_value_count(t))) << 3);
    }
}

// @src 14012d2a0  reset and apply the template of `type_id`, then allocate (except for types
// 0x65, 0x66, 0x67 and 0x51, which stay unallocated). value is zeroed as two ints; u5c, u68 and
// u70 are not touched.
void dynary_init(DYNARY* t, int type_id)
{
    dynary_free_arrays(t);
    t->u60 = 0;
    t->u64 = 0;
    t->type = type_id;
    std::memset(&t->value, 0, sizeof t->value); // [2] = [3] = 0
    t->u54 = 0;
    t->ndim = 0;
    t->ncomp = 1;
    t->u50 = 0;
    t->u58 = 0;
    for (int i = 0; i < 3; ++i) {
        t->var[i] = 0;
        t->n[i] = 0;
    }
    // Decompile index -> member: [1] ndim, [4..6] var[0..2], [7] n[0], [0x10] ncomp.
    switch (type_id) {
    case 7: t->ndim = 1; t->var[0] = 3; break;
    case 10: t->ndim = 1; t->var[0] = 5; break;
    case 0xd: case 0xe: case 0xf: case 0x10: case 0x11: case 0x12: case 0x13: case 0x14:
        t->ndim = 1; t->var[0] = 1; break;
    case 0x1c: t->ndim = 1; t->var[0] = 0x1f; break;
    case 0x27: t->ndim = 1; t->var[0] = 3; t->ncomp = 2; break;
    case 0x28: t->ndim = 1; t->var[0] = 0xc; t->ncomp = 2; break;
    case 0x29: t->ndim = 2; t->var[0] = 3; t->var[1] = 0xc; break;
    case 0x2c: t->ndim = 3; t->ncomp = 2; t->var[0] = 0xc; t->var[1] = 6; t->var[2] = 3; break;
    case 0x2d: t->ndim = 3; t->ncomp = 2; t->var[0] = 7; t->var[1] = 0xe; t->var[2] = 3; break;
    case 0x2e: t->ndim = 3; t->ncomp = 2; t->var[0] = 0xc; t->var[1] = 6; t->var[2] = 3; break;
    case 0x2f: t->ndim = 3; t->ncomp = 2; t->var[0] = 7; t->var[1] = 0xe; t->var[2] = 3; break;
    case 0x30: t->ndim = 3; t->ncomp = 2; t->var[0] = 0xc; t->var[1] = 0xf; t->var[2] = 0x10; break;
    case 0x32: t->ndim = 1; t->var[0] = 3; break;
    case 0x36: t->ndim = 1; t->var[0] = 3; break;
    case 0x37: t->ndim = 3; t->ncomp = 2; t->var[0] = 0xc; t->var[1] = 6; t->var[2] = 3; break;
    case 0x38: t->ndim = 1; t->var[0] = 3; break;
    case 0x43: t->ndim = 3; t->var[0] = 1; t->var[1] = 6; t->var[2] = 3; break;
    case 0x44: t->ndim = 3; t->var[0] = 7; t->var[1] = 6; t->var[2] = 3; break;
    case 0x45: t->ndim = 1; t->var[0] = 3; t->ncomp = 2; break;
    case 0x46: t->ndim = 1; t->var[0] = 3; t->ncomp = 2; break;
    case 0x47: t->ndim = 1; t->var[0] = 3; t->ncomp = 5; break;
    case 0x48: t->ndim = 1; t->var[0] = 3; t->ncomp = 3; break;
    case 0x4f: t->ndim = 3; t->var[0] = 7; t->var[1] = 8; t->var[2] = 3; break;
    case 0x50: t->ndim = 3; t->var[0] = 7; t->var[1] = 0xc; t->var[2] = 3; break;
    case 0x51: t->ndim = 3; t->var[0] = 7; t->var[1] = 8; t->var[2] = 3; break;
    case 0x52: t->ndim = 2; t->var[0] = 0xd; t->var[1] = 0xc; break;
    case 0x53: t->ndim = 1; t->var[0] = 1; break;
    case 0x54: t->ndim = 1; t->ncomp = 6; t->var[0] = 3; break;
    case 0x55: t->ndim = 1; t->ncomp = 4; t->var[0] = 3; break;
    case 0x56: t->ndim = 1; t->ncomp = 7; t->var[0] = 3; break;
    case 0x57: t->ndim = 1; t->ncomp = 6; t->var[0] = 3; break;
    case 0x58: t->ndim = 1; t->ncomp = 7; t->var[0] = 3; break;
    case 0x59: t->ndim = 1; t->ncomp = 6; t->var[0] = 3; break;
    case 0x5a: t->ndim = 1; t->ncomp = 9; t->var[0] = 3; break;
    case 0x5b: t->ndim = 1; t->ncomp = 6; t->var[0] = 3; break;
    case 0x5c: t->ndim = 1; t->ncomp = 3; t->var[0] = 3; break;
    case 0x5d: t->ndim = 0; break;
    case 0x5e: case 0x5f: case 0x60: t->ndim = 0; break;
    case 0x61: case 0x62: case 99: case 100: t->ndim = 0; break;
    case 0x65: t->ncomp = 6; break;
    case 0x66: t->ncomp = 3; break;
    case 0x67: t->ncomp = 1; break;
    case 0x68: t->ndim = 1; t->ncomp = 6; break;
    case 0x69: t->ndim = 1; t->ncomp = 3; break;
    case 0x70: t->ndim = 1; t->ncomp = 1; t->var[0] = 3; break;
    case 0x71: t->ndim = 1; t->ncomp = 1; t->var[0] = 3; break;
    case 0x72: t->ndim = 1; t->ncomp = 1; break;
    case 0x73: t->ndim = 1; t->ncomp = 1; break;
    case 0x74: t->ndim = 1; t->ncomp = 1; t->var[0] = 10; break;
    case 0x75: t->ndim = 1; t->ncomp = 1; t->var[0] = 10; break;
    case 0x79: t->ndim = 1; t->n[0] = 1; t->ncomp = 2; t->var[0] = 1; break;
    case 0x7a: t->ndim = 1; t->n[0] = 1; t->ncomp = 4; t->var[0] = 1; break;
    case 0x7b: t->ndim = 1; t->n[0] = 1; t->ncomp = 3; t->var[0] = 1; break;
    case 0x7c: t->ndim = 1; t->n[0] = 1; t->ncomp = 1; t->var[0] = 0x16; break;
    case 0x80: t->ndim = 0; t->n[0] = 0; t->ncomp = 0; break;
    case 0x81: t->ndim = 1; t->ncomp = 1; t->var[0] = 7; break;
    case 0x82: t->ndim = 1; t->ncomp = 1; t->var[0] = 3; break;
    case 0x83: case 0x84: case 0x85: case 0x86: case 0x87: case 0x88: case 0x89: case 0x8a:
        t->ndim = 1; t->ncomp = 1; break;
    case 0x8b: t->ndim = 1; t->ncomp = 1; t->var[0] = 2; break;
    case 0x8c: t->ndim = 3; t->var[0] = 2; t->var[1] = 8; t->var[2] = 3; break;
    case 0x8d: t->ndim = 1; t->ncomp = 1; t->var[0] = 7; break;
    case 0x8e: case 0x8f: case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95:
    case 0x96: case 0x97: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9e:
    case 0x9f: case 0xa0: case 0xa1: case 0xa2: case 0xa3:
        t->ndim = 1; t->n[0] = 0; t->ncomp = 0; t->var[0] = 0; break;
    case 0x98: t->ndim = 1; t->n[0] = 1; t->ncomp = 1; t->var[0] = 1; break;
    case 0xa6: t->ndim = 1; t->n[0] = 1; t->ncomp = 2; t->var[0] = 0; break;
    case 0xa7: t->ndim = 0; t->ncomp = 1; break;
    case 0xa8: t->ndim = 0; t->ncomp = 1; break;
    case 0xa9: t->ndim = 1; t->n[0] = 1; t->ncomp = 2; t->var[0] = 0; break;
    case 0xaa: t->ndim = 0; t->ncomp = 1; break;
    case 0xab: t->ndim = 0; t->ncomp = 1; break;
    case 0xac: t->ndim = 2; t->var[0] = 3; t->var[1] = 8; break;
    case 0xad: t->ndim = 0; t->ncomp = 1; break;
    case 0xae: t->ndim = 0; t->ncomp = 1; t->value = kTemplateAeDefault; break; // _DAT_140509c90
    case 0xaf: t->ndim = 0; t->ncomp = 2; std::memset(&t->value, 0, sizeof t->value); break;
    case 0xb0: t->ndim = 1; t->var[0] = 0; break;
    case 0xb1: t->ndim = 1; t->var[0] = 0; break;
    case 0xb2: t->ndim = 1; t->var[0] = 0; break;
    case 0xb3: t->ndim = 0; t->ncomp = 1; std::memset(&t->value, 0, sizeof t->value); break;
    case 0xb4: t->ndim = 1; t->ncomp = 1; t->var[0] = 0x20; break;
    case 0xb5: t->ndim = 0; t->ncomp = 0; break;
    case 0xb6: t->ndim = 0; std::memset(&t->value, 0, sizeof t->value); break;
    case 0xb7: t->ndim = 1; t->ncomp = 1; t->var[0] = 7; break;
    case 0xb8: t->ndim = 1; t->ncomp = 1; t->var[0] = 7; break;
    case 0xb9: t->ndim = 1; t->ncomp = 1; t->var[0] = 1; break;
    case 0xba: t->ndim = 1; t->ncomp = 1; t->var[0] = 1; break;
    case 0xbb: t->ndim = 1; t->ncomp = 1; t->var[0] = 1; break;
    default: break;
    }
    if (type_id != 0x65 && type_id != 0x66 && type_id != 0x67 && type_id != 0x51) dynary_alloc(t);
}

// @src 14012fc70  init(type_id) but keep u54 and u60 (list link etc.) as they were
void dynary_reinit_keep(DYNARY* t, int type_id)
{
    const std::int32_t keep54 = t->u54;
    // 14012fc70 saves 8 bytes at 0x60 (u60 and u64) and restores them.
    const std::int32_t keep60 = t->u60;
    const std::int32_t keep64 = t->u64;
    dynary_init(t, type_id);
    t->u54 = keep54;
    t->u60 = keep60;
    t->u64 = keep64;
}

} // namespace def::model
