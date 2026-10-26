// Path tables of the "arbitrary path" movement (DYNARY at movement+0x448, types 0x79/0x7a/0x7b).
// Spec: docs/re/commands/OBJPOS_interference.md sections 8.2, 8.3. These are DYNARY methods (the
// key-common packet's class, defmodel/dynary_init.hpp) that no packet had ported; they live here
// as free functions. Floating-point order follows the listings (out/p672/all_lst.txt).
#include <defcmd/ifd/path.hpp>
#include <defmodel/obj_scale.hpp>

#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/window/inter_object_ops.hpp>

#include <cmath>

namespace def::cmd::ifd::path {

// rdata 14051bed8  0.0
// rdata 140509c90  1.0

// @src 14012fd80  DYNARY: all zero (UCOMISD + JP: NaN is "not zero")
bool table_all_zero(const DYNARY* t)
{
    if (t->ndim == 0) {
        if (t->value != 0.0) return false;
    } else {
        for (std::int32_t i = 0; i < t->ndim; i = i + 1) {
            for (std::int32_t j = 0; j < t->n[i]; j = j + 1) {
                if (t->axis[i][j] != 0.0) return false;
            }
        }
        const std::int32_t cnt = model::dynary_value_count(t); // 14012ff20
        for (std::int32_t i = 0; i < cnt; i = i + 1) {
            if (t->values[i] != 0.0) return false;
        }
    }
    return true;
}

// @src 1401350e0  DYNARY: point k of a time path
bool path_point(const DYNARY* t, const std::int32_t* k, double* out)
{
    if (t->type != 0x79 && t->type != 0x7a) return false;
    if (t->var[0] != 1) return false;
    if (!model::dynary_counts_positive(t)) return false; // 14012f610
    const std::int32_t nc = (t->ncomp == 4) ? 3 : 2;
    for (std::int32_t c = 0; c < nc; c = c + 1) {
        out[c] = t->values[*k * t->ncomp + c];
    }
    return true;
}

// 140132a60: ported in defmodel/obj_scale.hpp (scale::table_segment)
// NaN x takes the search loop and finds no interval (-1).
std::int32_t path_interval(const DYNARY* t, double x, double* f)
{
    return model::scale::table_segment(t, x, f);
}

// @src 1401351c0  DYNARY: path position at a time, P_c = p_i,c + f * (p_i+1,c - p_i,c)
bool path_at_time(const DYNARY* t, const double* time, double* out)
{
    if (t->type != 0x79 && t->type != 0x7a) return false;
    if (t->var[0] != 1) return false;
    if (!model::dynary_counts_positive(t)) return false; // 14012f610
    double f = 0.0;
    const std::int32_t i = path_interval(t, *time, &f);
    if (i < 0) return false;
    const std::int32_t nc = (t->ncomp == 4) ? 3 : 2;
    for (std::int32_t c = 0; c < nc; c = c + 1) {
        const double p0 = t->values[i * t->ncomp + c];
        const double p1 = t->values[(i + 1) * t->ncomp + c];
        out[c] = p0 + f * (p1 - p0);
    }
    return true;
}

// @src 140134de0  DYNARY: speed path -> time path. The abscissae of the copy are the speeds; the
// segment length is sqrt of ((0 + d0*d0) + d1*d1) (+ d2*d2), taken only when the sum >= 0.0.
bool path_speed_to_time(DYNARY* t)
{
    if (t->type != 0x79 && t->type != 0x7a && t->type != 0x7b) return false;
    if (t->var[0] != 0x1a) return false;
    if (t->n[0] == 0) return false;
    DYNARY copy;                     // 14012d050
    model::dynary_copy(&copy, t);    // 14012eff0
    const std::int32_t nc = (t->ncomp == 4) ? 3 : 2;
    t->axis[0][0] = 0.0;
    for (std::int32_t k = 1; k < t->n[0]; k = k + 1) {
        const double v = copy.axis[0][k - 1];
        double len = 0.0;
        for (std::int32_t c = 0; c < nc; c = c + 1) {
            const double d = copy.values[k * t->ncomp + c] - copy.values[(k - 1) * t->ncomp + c];
            len = len + d * d;
        }
        if (len >= 0.0) {
            len = std::sqrt(len); // 140484644
        }
        if (v > 0.0) {
            t->axis[0][k] = t->axis[0][k - 1] + len / v;
        } else {
            t->axis[0][k] = t->axis[0][k - 1];
        }
    }
    t->var[0] = 1;
    return true;
    // copy destroyed here (14012f410)
}

// @src 14012fd30  DYNARY: re-initialise as `type` with n points on axis 0, then allocate
void dynary_reshape(DYNARY* t, std::int32_t type, std::int32_t n)
{
    model::dynary_init(t, type); // 14012d2a0
    t->n[0] = n;
    model::dynary_alloc(t);      // 14012fad0
}

// @src 1402416d0  3x3 matrix times vector
void mat3_mul_vec(const double* v, double* out, const double* m)
{
    out[0] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2];
    out[1] = m[3] * v[0] + m[4] * v[1] + m[5] * v[2];
    out[2] = m[6] * v[0] + m[7] * v[1] + m[8] * v[2];
}

// @src 140135bc0  DYNARY: (x, y, z) path of type 0x7b -> type 0x7a with (M (x, y, 1), z)
bool path_transform(DYNARY* t, const double* m)
{
    if (t->type != 0x7b || t->ncomp != 3) return false;
    DYNARY copy;                     // 14012d050
    model::dynary_copy(&copy, t);    // 14012eff0
    dynary_reshape(t, 0x7a, copy.n[0]);
    for (std::int32_t k = 0; k < t->n[0]; k = k + 1) {
        t->axis[0][k] = copy.axis[0][k];
        double* dst = t->values + k * t->ncomp;
        const double* src = copy.values + k * copy.ncomp;
        const double v[3] = {src[0], src[1], 1.0}; // 140509c90
        double out[3];
        mat3_mul_vec(v, out, m);
        dst[0] = out[0];
        dst[1] = out[1];
        dst[2] = out[2];
        dst[3] = src[2];
    }
    return true;
}

} // namespace def::cmd::ifd::path
