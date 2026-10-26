// defmodel/obj_scale.cpp: SVSCAL / DOESCL model functions (defmodel/obj_scale.hpp), packet p67-4.
// Decompiles: deform-rea-work/out/p67/svscal.c, doescl.c; FP order from the listings
// 140132a60..140132ca9 and 140136220 (p674-scratch/lst_interp.txt).
#include <defmodel/obj_scale.hpp>
#include <defmodel/statevar_kw/statevar_kw.hpp>
#include <defmodel/node_access.hpp>

#include <cstddef>

#include <defmodel/bc/node_arrays.hpp>
#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/cls_140269250.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/port.hpp>
#include <defmodel/step/name_arrays.hpp>
#include <defmodel/window/window_list.hpp>
#include <defrt/ftn/sse_ops.hpp>

namespace def::model::scale {

namespace sse = def::rt::sse;

namespace {

template <class T>
T& at(const void* base, std::size_t off)
{
    return *reinterpret_cast<T*>(const_cast<std::byte*>(static_cast<const std::byte*>(base)) + off);
}

DYNARY* table_at(void* base, std::size_t off) { return &at<DYNARY>(base, off); }

} // namespace

// @src 140132a60  segment search on axis 0 (listing: COMISD/JBE above, COMISD/JBE below,
// COMISD/JC both ends inside the loop; SUBSD, SUBSD, DIVSD)
std::int32_t table_segment(const DYNARY* t, double x, double* r)
{
    std::int32_t i = -1;
    if (!dynary_counts_positive(t)) return -1;
    const double* X = t->axis[0];
    const std::int32_t n = t->n[0];
    if (x > X[n - 1]) {
        i = n - 2;
    } else if (X[0] > x) {
        i = 0;
    } else {
        for (std::int32_t k = 0; k < n - 1; ++k) {
            if (x >= X[k] && X[k + 1] >= x) {
                i = k;
                break;
            }
        }
    }
    *r = 0.0;
    // 140132b93: XMM0 = x - X[i]; XMM1 = X[i+1] - X[i]; DIVSD XMM0, XMM1
    if (i >= 0) *r = sse::div(sse::sub(x, X[i]), sse::sub(X[i + 1], X[i]));
    return i;
}

// @src 140132bf0  Y[i] + r * (Y[i+1] - Y[i]) (listing 140132c79: SUBSD, MULSD r, ADDSD Y[i])
double table_interpolate(const DYNARY* t, double x)
{
    double v = 0.0;
    double r = 0.0;
    if (!dynary_counts_positive(t)) return 0.0;
    std::int32_t i = table_segment(t, x, &r);
    if (i >= 0) {
        const double* Y = t->values;
        // 140132c79: XMM0 = Y[i+1] - Y[i]; MULSD XMM1(r), XMM0; ADDSD XMM0(Y[i]), XMM1
        v = sse::add(Y[i], sse::mul(r, sse::sub(Y[i + 1], Y[i])));
    }
    return v;
}

// @src 140136220  one MULSD per value (value * factor)
std::int32_t table_scale(DYNARY* t, const double* factor)
{
    if (t->ndim == 0) {
        t->value = sse::mul(t->value, *factor);  // 140136248: MULSD value, factor
    } else {
        for (std::int32_t k = 0; k < dynary_value_count(t); ++k) t->values[k] = sse::mul(t->values[k], *factor);
    }
    return 1;
}

// 1401d48d0: ported in defmodel/node_access.hpp (fun_1401d48d0)
void region_load(DB_STEP_DATA* m, const std::int32_t* id)
{
    fun_1401d48d0(m, id);
}

// 14017f020: ported in defmodel/node_access.hpp (fun_14017f020)
const std::int32_t* node_set_dims(void* set)
{
    return fun_14017f020(static_cast<Cls_14017ef20*>(set));
}

// 140182980: ported by kw-g2sv (svkw::set9e0_dims)
const std::int32_t* element_set_dims(void* set)
{
    return svkw::set9e0_dims(set);
}

namespace {

// part of @src 140180c40 / 140181320: the node-field accessor body for field `id` at set+off
double* node_field(void* set, std::int32_t id, std::size_t off)
{
    auto* s = static_cast<std::byte*>(set);
    const std::int32_t* dims = node_set_dims(set);
    bc::node_set_alloc(set, at<std::int32_t>(s, 0x12c), dims, id);
    DB_STEP_DATA* m = at<DB_STEP_DATA*>(s, 0x130);
    if (m != nullptr) {
        std::int32_t key = id;
        region_load(m, &key);
    }
    return at<double*>(s, off);
}

} // namespace

// @src 140180c40  NDTMP, id 8
double* node_temperature(void* set) { return node_field(set, 8, 0x40); }

// @src 140181320  nodal strain, id 0x1b
double* node_strain(void* set) { return node_field(set, 0x1b, 0x98); }

// @src 140183fd0  element strain, id 0x2b (count set+0xe8, nodes per element set+0xec)
double* element_strain(void* set)
{
    auto* s = static_cast<std::byte*>(set);
    const std::int32_t* dims = element_set_dims(set);
    bc::element_set_alloc(set, at<std::int32_t>(s, 0xe8), dims, at<std::int32_t>(s, 0xec), 0x2b);
    DB_STEP_DATA* m = at<DB_STEP_DATA*>(s, 0xf0);
    if (m != nullptr) {
        std::int32_t key = 0x2b;
        region_load(m, &key);
    }
    return at<double*>(s, 0x48);
}

// @src 140204bc0  SVSCAL strain remap
void remap_strain(LLOBJ_DATA* obj, const DYNARY* t)
{
    double* e = element_strain(&at<std::byte>(obj, 0x9e0));
    double* nd = node_strain(&at<std::byte>(obj, 0x8a0));
    if (t->var[0] != 7) return;
    DB_STEP_DATA* m = obj->owner;
    std::int32_t w = m->u15f8[0x2b];  // model+0x16a4
    for (std::int32_t i = 0; i < at<std::int32_t>(obj, 0x884) * w; ++i) e[i] = table_interpolate(t, e[i]);
    if (at<std::int32_t>(m, 0x88c) == 2) {
        w = m->u15f8[0x1b];  // model+0x1664
        for (std::int32_t i = 0; i < at<std::int32_t>(obj, 0x880) * w; ++i) nd[i] = table_interpolate(t, nd[i]);
    }
}

// @src 14029b3f0  DOESCL material keywords
std::int32_t scale_material(Material* mat, std::int32_t id, const double* factor, std::int32_t n,
                            const std::int32_t* ints)
{
    (void)n;
    if (id == 0x5e) {
        if (at<std::int32_t>(mat, 0x50) != 1) return 0;
        table_scale(table_at(mat, 0x78), factor);
    } else if (id == 0x5f) {
        table_scale(table_at(mat, 0x670), factor);
    } else {
        if (id != 0x68) return 0;
        if (ints[1] != 0) table_scale(table_at(mat, 0x1ef0), factor);
        if (ints[2] != 0) table_scale(table_at(mat, 0x1f68), factor);
    }
    return 1;
}

// @src 1401954d0  DOESCL MOVCTL by movement type mv+0x68
std::uint8_t scale_movement(std::uint8_t* mv, const double* factor, std::int32_t n, const std::int32_t* ints)
{
    (void)n;
    std::uint8_t rc = 1;
    const std::int32_t type = at<std::int32_t>(mv, 0x68);
    if (type == 0) {
        table_scale(table_at(mv, 0x70), factor);
    } else if (type == 1) {
        table_scale(table_at(mv, 0xe8), factor);
    } else if (type == 2) {
        if (ints[1] == 0) {
            at<double>(mv, 0x1f0) = sse::mul(at<double>(mv, 0x1f0), *factor);  // 140195579
        } else if (ints[1] == 1) {
            at<double>(mv, 0x1f0) = *factor;
        } else if (ints[1] == 2 && at<std::int32_t>(mv, 0x20c) == 0) {
            at<double>(mv, 0x1f8) = *factor;
        }
    } else if (type == 3) {
        if (ints[1] == 0) {
            at<double>(mv, 0x280) = sse::mul(at<double>(mv, 0x280), *factor);  // 140195618
        } else if (ints[1] == 1) {
            at<double>(mv, 0x280) = *factor;
        } else if (ints[1] == 2 && at<std::int32_t>(mv, 0x2a4) == 0) {
            at<double>(mv, 0x288) = *factor;
        }
    } else if (type == 8) {
        table_scale(table_at(mv, 0x590), factor);
    } else {
        rc = 0;
    }
    return rc;
}

// @src 14021d9c0  DOESCL object keywords
std::int32_t scale_object(LLOBJ_DATA* obj, std::int32_t id, const double* factor, std::int32_t n,
                          const std::int32_t* ints)
{
    if (id == 0x97) {
        scale_movement(&at<std::uint8_t>(obj, 0x2160), factor, n, ints);
    } else if (id == 0xaa) {
        double* t = node_temperature(&at<std::byte>(obj, 0x8a0));
        if (t != nullptr) {
            for (std::int32_t k = 0; k < at<std::int32_t>(obj, 0x880); ++k) t[k] = sse::mul(t[k], *factor);  // 14021da70
        }
    } else {
        if (id != 0xe6) return 0;
        auto* w = reinterpret_cast<std::byte*>(winlist_nth(&at<Cls_140269250>(obj, 0x2f40), ints[1]));
        if (w != nullptr) {
            if (ints[2] != 0) table_scale(table_at(w, 0x380), factor);
            if (ints[3] != 0) table_scale(table_at(w, 0x3f8), factor);
            if (ints[4] != 0) table_scale(table_at(w, 0x470), factor);
            if (ints[5] != 0) table_scale(table_at(w, 0x4e8), factor);
            if (ints[6] != 0 && at<std::uint8_t>(w, 0x560) != 0) table_scale(table_at(w, 0x568), factor);
        }
    }
    return 1;
}

} // namespace def::model::scale
