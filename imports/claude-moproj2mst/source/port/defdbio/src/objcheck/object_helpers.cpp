// defdbio/src/objcheck/object_helpers.cpp: the small LLOBJ_DATA predicates and sub-object
// accessors that the object check 140167e90 uses (packet p4-03).
#include <defdbio/gendb/checks_p4_04.hpp>
#include <defmodel/node_access.hpp>
#include <defdbio/objcheck/objcheck.hpp>

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <defmodel/linked/axis_point_list.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include "ext.hpp"
#include "raw.hpp"

namespace def::dbio::objcheck {

using namespace raw;

namespace {
constexpr double kTol = 1e-06;   // DAT_14051f1b8
constexpr double kTiny = 1e-12;  // DAT_14051f260
constexpr double kTiny20 = 1e-20; // DAT_140522fa0
} // namespace

// ---- edge numbering ---------------------------------------------------------------------------------

// @src 140305520  index of boundary edge *edge (1-based over all boundaries, each boundary record
// 0x30 bytes with its edge count at +0) in the 2D edge arrays: edge - 1 + 2 * boundary; -1 when
// the edge is past the last boundary.
std::int32_t boundary_edge_index(const std::int32_t* edge, const std::int32_t* n_bnd, const void* bnd)
{
    std::int32_t first = 1;
    for (std::int32_t b = 0; b < *n_bnd; ++b) {
        std::int32_t next = first - 1 + i32(bnd, static_cast<std::int64_t>(b) * 0x30);
        if (first <= *edge && *edge < next) return *edge - 1 + b * 2;
        first = next;
    }
    return -1;
}

// @src 1403054e0  boundary_edge_index over the object's boundaries (+0xbd8 count, +0xbe0 records)
std::int32_t obj_edge_index(LLOBJ_DATA* obj, const std::int32_t* edge)
{
    return boundary_edge_index(edge, &i32(obj, 0xbd8), add(obj, 0xbe0));
}

namespace {
// The edge loop of 1401790a0 / 140179190: true when some edge's code in the array at `codes` is 2.
bool any_edge_code_2(LLOBJ_DATA* obj, std::int64_t codes)
{
    for (std::int32_t i = 0; i < i32(obj, 0x88c); ++i) {
        std::int32_t e;
        if (!is3d(owner(obj))) {
            std::int32_t edge = i + 1;
            e = obj_edge_index(obj, &edge);
        } else {
            e = i;
        }
        if (static_cast<const std::int32_t*>(ptr(obj, codes))[e] == 2) return true;
    }
    return false;
}
} // namespace

// @src 1401790a0  some edge has heat exchange code 2 (array +0xb00)
bool obj_has_heat_exchange_bc(LLOBJ_DATA* obj) { return any_edge_code_2(obj, 0xb00); }

// @src 140179190  some edge has diffusion code 2 (array +0xb18)
bool obj_has_diffusion_bc(LLOBJ_DATA* obj) { return any_edge_code_2(obj, 0xb18); }

// @src 140179150  heat exchange definitions (14019a590 count of +0x2e68+0x78) > 0
bool obj_has_heat_exchange(LLOBJ_DATA* obj) { return 0 < ext::fun_140195b60(add(obj, 0x2e68)); }

// @src 140179240  diffusion definitions > 0
bool obj_has_diffusion(LLOBJ_DATA* obj) { return 0 < ext::fun_140195b90(add(obj, 0x2e68)); }

// ---- movement -----------------------------------------------------------------------------------------

// @src 1401792d0  the object has a translational movement control (type at +0x21c8)
bool obj_has_translation(LLOBJ_DATA* obj)
{
    bool r = true;
    switch (i32(obj, 0x21c8)) {
    case 0:
        if (ext::fun_1401ee580(sim(owner(obj))) == 1) {
            if (i32(obj, 0x2780) == 0 &&
                ((i32(obj, 0x21d4) == 0 && std::fabs(f64(obj, 0x21d8)) < kTol) ||
                 dynary_values_zero(add(obj, 0x21d0)) || dynary_all_zero(add(obj, 0x21d0))))
                r = false;
        } else {
            r = false;
        }
        break;
    case 1:
        if (i32(obj, 0x2780) == 0 && i32(obj, 0x224c) == 0 && std::fabs(f64(obj, 0x2250)) < kTol)
            r = false;
        break;
    case 2: {
        bool none;
        if (i32(obj, 0x236c) == 0)
            none = std::fabs(f64(obj, 0x2358)) <= kTol;
        else
            none = dynary_values_zero(add(obj, 0x2368));
        r = !(none || std::fabs(f64(obj, 0x2348)) <= kTol);
        break;
    }
    case 3: {
        bool none;
        if (i32(obj, 0x2404) == 0)
            none = std::fabs(f64(obj, 0x23e8)) <= kTol;
        else
            none = dynary_values_zero(add(obj, 0x2400));
        bool b = !(none || std::fabs(f64(obj, 0x23f0)) <= kTol);
        r = !(!b || std::fabs(f64(obj, 0x23f8)) <= kTol);
        break;
    }
    case 4:
        if (u8(obj, 0x2490) == 0) {
            r = !(std::fabs(f64(obj, 0x2478)) <= kTol || std::fabs(f64(obj, 0x2480)) <= kTol);
        } else {
            bool b = i32(obj, 0x249c) == 1 && !dynary_all_zero(add(obj, 0x2498));
            r = !(!b || std::fabs(f64(obj, 0x2480)) <= kTol);
        }
        break;
    case 5:
        r = !dynary_all_zero(add(obj, 0x2510));
        break;
    case 6:
        if (dynary_values_zero(add(obj, 0x25a8)) || dynary_all_zero(add(obj, 0x25a8))) r = false;
        break;
    case 8:
        if (i32(obj, 0x2780) == 0 && i32(obj, 0x26f4) == 0 && std::fabs(f64(obj, 0x26f8)) < kTol)
            r = false;
        break;
    default:
        break;
    }
    return r;
}

// @src 140179840  rotational movement 1 defined (+0x2788 kind 0: +0x27c4 / +0x27c8, kind 1:
// +0x283c / +0x2840)
bool obj_has_rot_move1(LLOBJ_DATA* obj)
{
    bool r = false;
    if (i32(obj, 0x2788) == 0) {
        r = i32(obj, 0x27c4) != 0 || kTol <= std::fabs(f64(obj, 0x27c8));
    } else if (i32(obj, 0x2788) == 1) {
        r = i32(obj, 0x283c) != 0 || kTol <= std::fabs(f64(obj, 0x2840));
    }
    return r;
}

// @src 140179900  rotational movement 2 defined (+0x28b8 kind)
bool obj_has_rot_move2(LLOBJ_DATA* obj)
{
    bool r = false;
    if (i32(obj, 0x28b8) == 0) {
        r = i32(obj, 0x28f4) != 0 || kTol <= std::fabs(f64(obj, 0x28f8));
    } else if (i32(obj, 0x28b8) == 1) {
        r = i32(obj, 0x296c) != 0 || kTol <= std::fabs(f64(obj, 0x2970));
    }
    return r;
}

// @src 1401797e0  rotation 1, or (3D) rotation 2
bool obj_has_rotation(LLOBJ_DATA* obj)
{
    bool r = obj_has_rot_move1(obj);
    if (is3d(owner(obj)) && !r) r = obj_has_rot_move2(obj);
    return r;
}

// @src 140179280  translation or rotation
bool obj_has_movement(LLOBJ_DATA* obj)
{
    bool r = obj_has_translation(obj);
    if (!r) r = obj_has_rotation(obj);
    return r;
}

// @src 1401799c0  rotation axis 1 set: 2D always; 3D |axis (+0x27a8)|^2 > 1e-12
bool obj_has_rot_axis1(LLOBJ_DATA* obj)
{
    if (!is3d(owner(obj))) return true;
    return kTiny < f64(obj, 0x27a8) * f64(obj, 0x27a8) + f64(obj, 0x27b0) * f64(obj, 0x27b0) +
                       f64(obj, 0x27b8) * f64(obj, 0x27b8);
}

// @src 140179a90  rotation axis 2 set (3D only; 2D prints a message and returns true)
bool obj_has_rot_axis2(LLOBJ_DATA* obj)
{
    if (!is3d(owner(obj))) {
        def::rt::crt::fprintf(stdout, "Wrong function call ~ LLOBJ_DATA::hasRotMoveAxis2()\n");
        return true;
    }
    return kTiny < f64(obj, 0x28d8) * f64(obj, 0x28d8) + f64(obj, 0x28e0) * f64(obj, 0x28e0) +
                       f64(obj, 0x28e8) * f64(obj, 0x28e8);
}

// @src 140179b70  rotational symmetry boundary entries (140186da0 on +0x1ea0) > 0
bool obj_has_rotsym_bc(LLOBJ_DATA* obj) { return 0 < ext::fun_140186da0(add(obj, 0x1ea0)); }

// ---- boundary conditions --------------------------------------------------------------------------------

// @src 14016c4e0  mark function *fn as used (1 <= *fn <= *n_fn): used[*fn] = 1, *any = 1
bool obj_mark_function(LLOBJ_DATA* obj, char* used, const std::int32_t* fn, char* any, const std::int32_t* n_fn)
{
    (void)obj;
    if (used == nullptr) return false;
    if (*fn < 1 || *n_fn < *fn) return false;
    used[*fn] = 1;
    *any = 1;
    return true;
}

// @src 14016c550  clear edge functions whose edge code is 0 (ECDEFN 0xae8/0xaf0, ECTMFN
// 0xb00/0xb08, ECATFN 0xb18/0xb20, ECRHFN 0xb30/0xb38); counts what was cleared
bool obj_clean_edge_bcs(LLOBJ_DATA* obj, std::int32_t* n_ecdefn, std::int32_t* n_ectmfn, std::int32_t* n_ecatfn,
                        std::int32_t* n_ecrhfn)
{
    *n_ecdefn = 0;
    *n_ectmfn = 0;
    *n_ecatfn = 0;
    *n_ecrhfn = 0;
    std::int32_t n = ext::fun_140165170(obj);
    auto clean = [&](std::int64_t code_off, std::int64_t fn_off, std::int32_t i, std::int32_t* cnt) {
        auto* code = static_cast<std::int32_t*>(ptr(obj, code_off));
        auto* fn = static_cast<std::int32_t*>(ptr(obj, fn_off));
        if (code[i] == 0 && fn[i] != 0) {
            fn[i] = 0;
            *cnt = *cnt + 1;
        }
    };
    for (std::int32_t i = 0; i < n; ++i) {
        clean(0xae8, 0xaf0, i, n_ecdefn);
        clean(0xb00, 0xb08, i, n_ectmfn);
        clean(0xb18, 0xb20, i, n_ecatfn);
        clean(0xb30, 0xb38, i, n_ecrhfn);
    }
    return !(*n_ecdefn == 0 && *n_ectmfn == 0 && *n_ecatfn == 0 && *n_ecrhfn == 0);
}

// @src 14016c780  3D: delete the symmetry planes (list at +0x2e80) that no node references with
// BCC code 6 (+0x990 codes, +0x998 plane numbers)
void obj_remove_unused_symmetry_planes(LLOBJ_DATA* obj)
{
    void* step = owner(obj);
    if (!is3d(step)) return;
    void* list = add(obj, 0x2e80);
    std::int32_t nmax = plane_list_max_id(list);
    if (0 < nmax) {
        auto* used = static_cast<char*>(std::malloc(static_cast<std::size_t>(nmax + 1)));
        for (std::int32_t i = 0; i <= nmax; ++i) used[i] = 0;
        std::int32_t numnp = i32(obj, 0x880);
        std::int32_t ndof = i32(step, 0x1644);
        auto* code = static_cast<const std::int32_t*>(ptr(obj, 0x990));
        auto* plane = static_cast<const std::int32_t*>(ptr(obj, 0x998));
        for (std::int32_t i = 0; i < numnp * ndof; ++i) {
            if (code[i] == 6 && 0 < plane[i]) {
                std::int32_t p = plane[i];
                if (0 < p && p <= nmax) used[p - 1] = 1;
            }
        }
        (void)i32(list, 0x10); // 140158e70: count (result unused)
        for (std::int32_t i = 0; i <= nmax; ++i) {
            if (used[i] == 0) {
                void* node = ext::fun_1402580f0(list, i + 1);
                if (node != nullptr) plane_list_remove(list, node);
            }
        }
        (void)i32(list, 0x10); // 140158e70
        std::free(used);
    }
}

// @src 14016ca40  "%s of Object %6d Undefined" when (mask & DB_STEP_DATA+0x13c) and defined == '?'
void obj_undefined(LLOBJ_DATA* obj, const char* key, std::uint32_t mask, char defined, std::int32_t* err)
{
    std::uint32_t m = step_u13c(static_cast<const DB_STEP_DATA*>(owner(obj)));
    if ((mask & m) != 0 && defined == '?') {
        *err = 1;
        def::rt::crt::fprintf(stdout, "ERROR  : %s of Object %6d Undefined\n", key, i32(obj, 8));
        std::int32_t code = 0;
        std::int32_t zero = 0;
        ext::fun_14001cd90(&code, key, add(obj, 8), &zero);
    }
}

// @src 140201320  some boundary facet node (facets +0xbf8, 4 per facet) has BCC code *code
// in one of its 3 directions
bool obj_has_free_surface_code(LLOBJ_DATA* obj, const std::int32_t* code)
{
    if (i32(obj, 0x87c) == 0 || i32(obj, 0x880) < 1 || ptr(obj, 0x990) == nullptr) return false;
    bool found = false;
    auto* bcc = static_cast<const std::int32_t*>(ptr(obj, 0x990));
    auto* fac = static_cast<const std::int32_t*>(ptr(obj, 0xbf8));
    for (std::int32_t f = 0; f < i32(obj, 0xbf0); ++f) {
        for (std::int32_t k = 0; k < 4; ++k) {
            for (std::int32_t d = 0; d < 3; ++d) {
                if (*code == bcc[(fac[f * 4 + k] - 1) * 3 + d]) {
                    found = true;
                    break;
                }
            }
        }
    }
    return found;
}

// @src 1402014c0  some boundary facet node has a contact code (< 0) with object `other`
// (any object when other == 0)
bool obj_touches_object(LLOBJ_DATA* obj, std::int32_t other)
{
    if (i32(obj, 0x87c) == 0 || i32(obj, 0x880) < 1 || ptr(obj, 0x990) == nullptr) return false;
    auto* bcc = static_cast<const std::int32_t*>(ptr(obj, 0x990));
    auto* fac = static_cast<const std::int32_t*>(ptr(obj, 0xbf8));
    for (std::int32_t f = 0; f < i32(obj, 0xbf0); ++f) {
        for (std::int32_t k = 0; k < 4; ++k) {
            for (std::int32_t d = 0; d < 3; ++d) {
                std::int32_t c = bcc[(fac[f * 4 + k] - 1) * 3 + d];
                if (c < 0) {
                    if (other == 0) return true;
                    if (other == -c) return true;
                }
            }
        }
    }
    return false;
}

// @src 140203fa0  some node has BCC code 7 or 8 (rotational symmetry), 3 per node
bool obj_has_rotsym_code(LLOBJ_DATA* obj)
{
    for (std::int32_t n = 0; n < i32(obj, 0x880); ++n) {
        for (std::int32_t d = 0; d < 3; ++d) {
            auto* bcc = static_cast<const std::int32_t*>(ptr(obj, 0x990));
            if (bcc[n * 3 + d] == 7 || bcc[n * 3 + d] == 8) return true;
        }
    }
    return false;
}

// @src 140204040  some node has BCC code 3 (velocity); DOF per node from DB_STEP_DATA+0x1640
// (2 when the object has no owner)
bool obj_has_velocity_code(LLOBJ_DATA* obj)
{
    std::int32_t ndof = 2;
    if (i32(obj, 0x87c) == 0 || ptr(obj, 0x990) == nullptr) return false;
    if (owner(obj) != nullptr) ndof = i32(owner(obj), 0x1640);
    for (std::int32_t n = 0; n < i32(obj, 0x880); ++n) {
        for (std::int32_t d = 0; d < ndof; ++d) {
            if (static_cast<const std::int32_t*>(ptr(obj, 0x990))[n * ndof + d] == 3) return true;
        }
    }
    return false;
}

// ---- node temperatures (+0x8e0) ------------------------------------------------------------------------

// @src 14020aa50  with *count: counts nodes with T <= 0 (T < 0 also makes the result true) and
// scans all nodes; without: true at the first T > 1e-6. Result: some temperature is set.
bool obj_node_temperatures(LLOBJ_DATA* obj, const char* count, std::int32_t* n_bad)
{
    bool r = false;
    *n_bad = 0;
    for (std::int32_t i = 0; i < i32(obj, 0x880); ++i) {
        const double* t = static_cast<const double*>(ptr(obj, 0x8e0));
        if (*count != 0) {
            if (0.0 < t[i] || t[i] == 0.0) {
                if (t[i] == 0.0) *n_bad = *n_bad + 1;
            } else {
                *n_bad = *n_bad + 1;
                r = true;
            }
        }
        if (kTol < t[i]) {
            r = true;
            if (*count == 0) return r;
        }
    }
    return r;
}

// @src 14020ab60  temperature range: values that differ from the running min / max by more
// than 1e-6 (with the sign of the difference) replace it
void obj_temperature_range(LLOBJ_DATA* obj, double* tmin, double* tmax)
{
    *tmax = 0.0;
    *tmin = 0.0;
    const double* t = static_cast<const double*>(ptr(obj, 0x8e0));
    if (t == nullptr) return;
    *tmax = t[0];
    *tmin = *tmax;
    for (std::int32_t i = 0; i < i32(obj, 0x880); ++i) {
        if (t[i] - *tmin < kTol) *tmin = t[i];
        if (kTol < t[i] - *tmax) *tmax = t[i];
    }
}

// @src 1401fbfc0  some element (+0x9f8, numel +0x884) has material `material`
bool obj_contains_material(LLOBJ_DATA* obj, std::int32_t material)
{
    if (i32(obj, 0x87c) == 0) return false;
    if (ptr(obj, 0x9f8) == nullptr) return false;
    for (std::int32_t e = 0; e < i32(obj, 0x884); ++e) {
        if (material == static_cast<const std::int32_t*>(ptr(obj, 0x9f8))[e]) return true;
    }
    return false;
}

// ---- sub-objects -----------------------------------------------------------------------------------------

// 1401808d0  node block (Cls_14017ef20) coordinates: the p67-3 port (defmodel/node_access)
double* node_block_coords(void* blk) { return model::fun_1401808d0(static_cast<model::Cls_14017ef20*>(blk)); }

namespace {
// 140184050 / 1401842f0 / 140184520: element block (Cls_140182880) array `id` at `off`
double* elem_block_array(void* blk, std::int32_t id, std::int64_t off)
{
    void* widths = ext::fun_140182980(blk);
    ext::fun_1401836c0(blk, i32(blk, 0xe8), widths, i32(blk, 0xec), id);
    if (ptr(blk, 0xf0) != nullptr) {
        std::int32_t k = id;
        ext::fun_1401d48d0(ptr(blk, 0xf0), &k);
    }
    return static_cast<double*>(ptr(blk, off));
}
} // namespace

// @src 140184050  element block: density (array 0x2c at +0x50)
double* elem_block_density(void* blk) { return elem_block_array(blk, 0x2c, 0x50); }

// @src 1401842f0  element block: volume fractions (array 0x31 at +0x78)
double* elem_block_volume_fractions(void* blk) { return elem_block_array(blk, 0x31, 0x78); }

// @src 140186bc0  3D: rotational symmetry angle (+0x1b8) >= 1e-20
bool rotsym_angle_set(void* blk)
{
    if (!is3d(ptr(blk, 0x2b8))) return false;
    return kTiny20 < f64(blk, 0x1b8) || kTiny20 == f64(blk, 0x1b8);
}

// @src 140186c10  3D: angle = 0
void rotsym_angle_reset(void* blk)
{
    if (is3d(ptr(blk, 0x2b8))) f64(blk, 0x1b8) = 0.0;
}

// @src 140186c50  3D: |axis (+0x1d8)|^2 >= 1e-20
bool rotsym_axis_set(void* blk)
{
    if (!is3d(ptr(blk, 0x2b8))) return false;
    return kTiny20 <= f64(blk, 0x1d8) * f64(blk, 0x1d8) + f64(blk, 0x1e0) * f64(blk, 0x1e0) +
                          f64(blk, 0x1e8) * f64(blk, 0x1e8);
}

// @src 14018b0f0  mesh window block: only +0x30 set (> 0), +0x18..+0x28 and +0x38 zero
bool window_unset(void* blk)
{
    if (f64(blk, 0x30) <= 0.0 || f64(blk, 0x18) != 0.0 || f64(blk, 0x20) != 0.0 ||
        f64(blk, 0x28) != 0.0 || f64(blk, 0x38) != 0.0)
        return false;
    return true;
}

// @src 14018cd10  movement block (Cls_14018b220): direction (+0x50..) is not zero (2 components
// unless the owner is 3D)
bool movement_direction_set(void* blk)
{
    if (!is3d(ptr(blk, 0xa80))) {
        return !(f64(blk, 0x50) == 0.0 && f64(blk, 0x58) == 0.0);
    }
    return !(f64(blk, 0x50) == 0.0 && f64(blk, 0x58) == 0.0 && f64(blk, 0x60) == 0.0);
}

// @src 140194f90  stroke . unit(dir), 3D or 2D (0 when dir is zero)
double press_stroke_projection(void* blk, const double* stroke, const double* dir)
{
    double d[3] = {dir[0], dir[1], 0.0};      // local_30
    double s[3] = {stroke[0], stroke[1], 0.0}; // local_48
    double r = 0.0;
    if (!is2d(ptr(blk, 0xa80))) {
        d[2] = dir[2];
        s[2] = stroke[2];
        if (0.0 < ext::fun_14023b720(d)) r = ext::fun_14023b3a0(d, s);
    } else {
        if (0.0 < ext::fun_1403091c0(d)) r = ext::fun_1403090e0(d, s);
    }
    return r;
}

// @src 1401951d0  range (max - min) of the press curve values (+0x380, count +0x354) when the
// curve is used (+0x33c)
double press_curve_range(void* blk)
{
    if (i32(blk, 0x33c) == 0 || i32(blk, 0x354) == 0) return 0.0;
    const double* v = static_cast<const double*>(ptr(blk, 0x380));
    double lo = v[0];
    double hi = v[0];
    for (std::int32_t i = 0; i < i32(blk, 0x354); ++i) {
        if (v[i] < lo) lo = v[i];
        if (hi < v[i]) hi = v[i];
    }
    return hi - lo;
}

// @src 1401952e0  stroke vector: unit(movement direction +0x50) * (stroke - stroke0), or zero
// when the direction or the stroke is below 1e-6
void press_stroke_vector(void* blk, double stroke0, double stroke, double* out)
{
    double d[3];
    vec3_copy(d, static_cast<const double*>(static_cast<void*>(add(blk, 0x50))));
    double u[3];
    ext::fun_140239b40(u, d);
    double r[3];
    if (kTol < std::fabs(ext::fun_140239a30(u)) && kTol < std::fabs(stroke)) {
        vec3_scale(r, u, stroke - stroke0);
    } else {
        r[1] = 0.0;
        r[0] = 0.0;
        r[2] = 0.0;
    }
    out[0] = r[0];
    out[1] = r[1];
    out[2] = r[2];
}

// ---- symmetry plane lists -------------------------------------------------------------------------------

// @src 1402575d0  copy the plane's direction (+0x20)
void plane_direction(const void* node, double* out)
{
    if (out != nullptr) std::memcpy(out, static_cast<const char*>(node) + 0x20, 0x18);
}

// @src 140257670  direction is zero (every |component| <= 1e-6)
bool plane_direction_zero(const void* node)
{
    for (int i = 0; i <= 2; ++i) {
        if (kTol < std::fabs(f64(node, 0x20 + i * 8))) return false;
    }
    return true;
}

// @src 140257f70  unlink and delete node from the list (head +0, tail +8, count +0x10)
bool plane_list_remove(void* list, void* node)
{
    void* prev = ptr(list, 0);
    void* p = ptr(list, 0);
    while (p != nullptr && p != node) {
        prev = p;
        p = ptr(p, 0x38);
    }
    if (p == nullptr) return false;
    ptr(prev, 0x38) = ptr(p, 0x38);
    if (ptr(list, 0) == p) ptr(list, 0) = ptr(p, 0x38);
    if (ptr(list, 8) == p) ptr(list, 8) = prev;
    if (p != nullptr) delete static_cast<model::linked::Cls_140257340*>(p); // 14025bf00(p, 1)
    i32(list, 0x10) = i32(list, 0x10) - 1;
    return true;
}

// @src 1402582b0  max(count, largest plane id)
std::int32_t plane_list_max_id(void* list)
{
    std::int32_t m = i32(list, 0x10); // 140158e70
    for (void* p = ptr(list, 0); p != nullptr; p = ptr(p, 0x38)) {
        m = i32(p, 0) < m ? m : i32(p, 0);
    }
    return m;
}

// ---- DYNARY ------------------------------------------------------------------------------------------------

// 14012fd80 / 14012fe90: the p4-04 ports (gendb/checks_p4_04.hpp), one copy each
bool dynary_all_zero(const void* t) { return gendb::dynary_all_zero(static_cast<const model::DYNARY*>(t)); }
bool dynary_values_zero(const void* t) { return gendb::dynary_values_zero(static_cast<const model::DYNARY*>(t)); }

// ---- 3-vectors -----------------------------------------------------------------------------------------------

// @src 140239800  copy
double* vec3_copy(double* out, const double* v)
{
    double t[3] = {v[0], v[1], v[2]};
    std::memcpy(out, t, sizeof t);
    return out;
}

// @src 140239ea0  s * v
double* vec3_scale(double* out, const double* v, double s)
{
    double t[3] = {s * v[0], s * v[1], s * v[2]};
    std::memcpy(out, t, sizeof t);
    return out;
}

// @src 14023aa90  |a - b| < tol in every component
bool vec3_near(const double* a, const double* b, double tol)
{
    return std::fabs(a[0] - b[0]) < tol && std::fabs(a[1] - b[1]) < tol && std::fabs(a[2] - b[2]) < tol;
}

// ---- small getters -----------------------------------------------------------------------------------------------

// @src 140199cc0  DB_STEP_DATA+0x13c (keyword-group mask the "Undefined" checks test)
std::uint32_t step_u13c(const DB_STEP_DATA* step) { return static_cast<std::uint32_t>(i32(step, 0x13c)); }

// @src 1401f3c90  simulation control +0x114
std::int32_t sim_u114(const void* s) { return i32(s, 0x114); }

// @src 14028d120  material has ANISO data (+0x1e40 != 0)
bool material_has_aniso(const void* mat) { return i32(mat, 0x1e40) != 0; }

// @src 14019d5e0  n-th inter-object relation (list at +0x13c8)
void* step_inter_object_at(DB_STEP_DATA* step, std::int32_t n) { return ext::fun_1401fac90(add(step, 0x13c8), n); }

// @src 1401a9820  relation between objects a and b: 0 when one exists with the right
// master/slave sense (+0x170: 1 = first is master... see the cases), -0xca when it exists with
// the other sense, -0xc9 when there is none
std::int32_t step_inter_object_relation(DB_STEP_DATA* step, std::int32_t a, std::int32_t b)
{
    bool found = false;
    bool ok = false;
    for (std::int32_t i = 0; i < ext::fun_1400015d0(add(step, 0x13c8)); ++i) {
        void* r = step_inter_object_at(step, i + 1);
        if (r == nullptr || i32(r, 0x170) == 0) continue;
        if (a == b) {
            if (i32(r, 0x168) == a && i32(r, 0x16c) == b) {
                found = true;
                ok = true;
                break;
            }
        } else {
            if (i32(r, 0x168) == a && i32(r, 0x16c) == b) {
                found = true;
                ok = i32(r, 0x170) == 2;
                break;
            }
            if (i32(r, 0x168) == b && i32(r, 0x16c) == a) {
                found = true;
                ok = i32(r, 0x170) == 1;
                break;
            }
        }
    }
    if (!found) return -0xc9;
    return ok ? 0 : -0xca;
}

} // namespace def::dbio::objcheck
