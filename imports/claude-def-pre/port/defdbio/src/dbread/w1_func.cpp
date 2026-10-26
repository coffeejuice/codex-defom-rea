// dbread/w1_func.cpp: function tables (DYNARY) from IIFNC/LLFNC, the movement-table conversions,
// 3x3 matrix helpers and the text byte swap of DBREAD (packet p-dbread, helper W1).
// Decompiles: deform-rea-work/out/dbread/w1/w1_d0.c, listings w1_listing.txt (unoptimised code:
// every FP expression below is in the listing's order).
#include <defdbio/dbread/w1.hpp>

#include <cmath>
#include <cstring>

#include <defdbio/gendb/gate_ports.hpp>
#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/window/inter_object_ops.hpp>

#include "dbread/w1_access.hpp"

namespace def::dbio::dbread {

namespace w1 {
std::FILE* out_stream() { return stdout; }
} // namespace w1

// @src 140130920
double* func_record(std::int32_t* iifnc, double* llfnc, std::int32_t fn)
{
    if (fn < 1) {
        return nullptr;
    }
    const std::int32_t start = iifnc[fn * 2 - 2];
    if (iifnc[fn << 1] - start < 1) {
        return nullptr;
    }
    return llfnc + (start - 1);
}

// @src 1401309b0  (ReadFuncFromDBArray)
void read_func_from_db_array(DYNARY* f, std::int32_t* iifnc, double* llfnc, std::int32_t fn, const char* with_shape)
{
    model::dynary_clamp_shape(f); // 14012fc00
    if (-1 < f->ndim && (f->ndim != 0 || 1 < f->ncomp) && 0 < fn) {
        const std::int32_t start = iifnc[fn * 2 - 2];
        const std::int32_t len = iifnc[fn * 2] - start;
        if (0 < len) {
            f->u54 = iifnc[fn * 2 - 1];
            double* p = llfnc + (start - 1);
            if (f->ndim == 1) {
                if (f->type == 0x7c) {
                    model::dynary_alloc(f); // 14012fad0
                    std::memset(f->axis[0], 0, 8);
                    for (std::int32_t k = 0; k < f->ncomp; k = k + 1) {
                        f->values[k] = p[k];
                    }
                } else {
                    if (f->ncomp == 1) {
                        f->n[0] = len / 2;
                    }
                    model::dynary_alloc(f);
                    for (std::int32_t i = 0; i < f->n[0]; i = i + 1) {
                        f->axis[0][i] = p[i * (f->ncomp + 1)];
                        for (std::int32_t k = 0; k < f->ncomp; k = k + 1) {
                            f->values[i * f->ncomp + k] = p[i * (f->ncomp + 1) + 1 + k];
                        }
                    }
                }
            } else {
                if (*with_shape != 0) {
                    if (f->ndim == 2) {
                        f->n[0] = static_cast<std::int32_t>(p[0]);
                        f->n[1] = static_cast<std::int32_t>(p[1]);
                        p = p + 2;
                    } else if (f->ndim == 3) {
                        f->n[0] = static_cast<std::int32_t>(p[0]);
                        f->n[1] = static_cast<std::int32_t>(p[1]);
                        f->n[2] = static_cast<std::int32_t>(p[2]);
                        p = p + 3;
                    }
                }
                model::dynary_alloc(f);
                std::int32_t off = 0;
                for (std::int32_t d = 0; d < f->ndim; d = d + 1) {
                    for (std::int32_t i = 0; i < f->n[d]; i = i + 1) {
                        f->axis[d][i] = p[off + i];
                    }
                    off = off + f->n[d];
                }
                const std::int32_t cnt = model::dynary_value_count(f); // 14012ff20
                std::memcpy(f->values, p + off, static_cast<std::size_t>(static_cast<std::int64_t>(cnt) << 3));
            }
        }
    }
}

// @src 140131ea0
void read_table_rows_1d(DYNARY* f, double* rows)
{
    model::dynary_clamp_shape(f);
    model::dynary_alloc(f);
    for (std::int32_t i = 0; i < f->n[0]; i = i + 1) {
        std::memcpy(&f->axis[0][i], &rows[i * (f->ncomp + 1)], 8);
        for (std::int32_t k = 0; k < f->ncomp; k = k + 1) {
            std::memcpy(&f->values[i * f->ncomp + k], &rows[i * (f->ncomp + 1) + 1 + k], 8);
        }
    }
}

// @src 14012fd30
void dynary_init_n0(DYNARY* f, std::int32_t type, std::int32_t n0)
{
    model::dynary_init(f, type); // 14012d2a0
    f->n[0] = n0;
    model::dynary_alloc(f);
}

// @src 140134b00
bool dynary_time_axis_to_speed(DYNARY* f)
{
    if (f->type != 0x79 && f->type != 0x7a && f->type != 0x7b) {
        return false;
    }
    if (f->var[0] != 1) {
        return false;
    }
    if (f->n[0] == 0) {
        return false;
    }
    DYNARY tmp;                    // 14012d050
    model::dynary_copy(&tmp, f);   // 14012eff0
    const std::int32_t nc = (f->ncomp == 4) ? 3 : 2;
    for (std::int32_t k = 0; k < f->n[0] - 1; k = k + 1) {
        const double dt = tmp.axis[0][k + 1] - tmp.axis[0][k];
        double s = 0.0;
        for (std::int32_t j = 0; j < nc; j = j + 1) {
            const double d = tmp.values[(k + 1) * f->ncomp + j] - tmp.values[k * f->ncomp + j];
            s = s + d * d;
        }
        if (s >= 0.0) { // 140134cfe COMISD / JC: NaN and negatives skip
            s = std::sqrt(s); // 140484644
        }
        if (dt > 0.0) { // 140134d28 COMISD / JBE
            f->axis[0][k] = s / dt;
        } else {
            f->axis[0][k] = 0.0;
        }
    }
    f->axis[0][f->n[0] - 1] = 0.0;
    f->var[0] = 0x1a;
    return true; // tmp: 14012f410
}

namespace {

bool rotate_7a(DYNARY* f, double* mat, bool shift, double cx, double cy, double cz)
{
    if (f->type != 0x7a || f->ncomp != 4) {
        return false;
    }
    DYNARY tmp;
    model::dynary_copy(&tmp, f);
    dynary_init_n0(f, 0x7b, tmp.n[0]); // 14012fd30
    for (std::int32_t k = 0; k < f->n[0]; k = k + 1) {
        std::memcpy(&f->axis[0][k], &tmp.axis[0][k], 8);
        double* out = f->values + k * f->ncomp;
        const double* in = tmp.values + k * tmp.ncomp;
        double v[3];
        if (shift) {
            v[0] = in[0] - cx;
            v[1] = in[1] - cy;
            v[2] = in[2] - cz;
        } else {
            std::memcpy(v, in, 24);
        }
        double r[3];
        mat3_mul_vec(v, r, mat); // 1402416d0
        std::memcpy(&out[0], &r[0], 8);
        std::memcpy(&out[1], &r[1], 8);
        std::memcpy(&out[2], &in[3], 8);
    }
    return true;
}

} // namespace

// @src 140135dd0
bool dynary_rotate_7a(DYNARY* f, double* mat) { return rotate_7a(f, mat, false, 0.0, 0.0, 0.0); }

// @src 140135fd0
bool dynary_rotate_7a_about(DYNARY* f, double* mat, double cx, double cy, double cz)
{
    return rotate_7a(f, mat, true, cx, cy, cz);
}

// @src 140194e30
std::int32_t movement_frame_inverse(void* mv, double* inv)
{
    double frame[11];
    gendb::movement_frame(mv, frame); // 140194d40
    return mat3_inverse_or_2d(frame, inv);
}

// @src 140241390
double mat3_det(double* a)
{
    return ((((-a[5] * a[7] * a[0] + a[8] * a[4] * a[0]) - a[8] * a[1] * a[3]) + a[1] * a[5] * a[6]) -
            a[2] * a[4] * a[6]) +
           a[2] * a[3] * a[7];
}

// @src 140241470
bool mat3_inverse(double* a, double* inv)
{
    const double det = mat3_det(a);
    const bool ok = det != 0.0;
    if (ok) {
        inv[0] = (a[8] * a[4] - a[5] * a[7]) / det;
        inv[1] = (a[2] * a[7] - a[8] * a[1]) / det;
        inv[2] = (a[1] * a[5] - a[2] * a[4]) / det;
        inv[3] = (a[5] * a[6] - a[8] * a[3]) / det;
        inv[4] = (a[8] * a[0] - a[2] * a[6]) / det;
        inv[5] = (a[2] * a[3] - a[0] * a[5]) / det;
        inv[6] = (a[3] * a[7] - a[4] * a[6]) / det;
        inv[7] = (a[1] * a[6] - a[0] * a[7]) / det;
        inv[8] = (a[0] * a[4] - a[1] * a[3]) / det;
    }
    return ok;
}

// @src 1402416d0
void mat3_mul_vec(double* v, double* out, double* m)
{
    out[0] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2];
    out[1] = m[3] * v[0] + m[4] * v[1] + m[5] * v[2];
    out[2] = m[6] * v[0] + m[7] * v[1] + m[8] * v[2];
}

// @src 1402417d0
void mat3_zero(double* a)
{
    for (int k = 0; k < 9; ++k) a[k] = 0.0;
}

// @src 140241860
std::int32_t mat3_inverse_or_2d(double* a, double* inv)
{
    if (mat3_inverse(a, inv)) {
        return 1;
    }
    mat3_zero(inv);
    double d = a[0] * a[4] - a[1] * a[3];
    if (d == 0.0) {
        d = a[3] * a[7] - a[4] * a[6];
        if (d == 0.0) {
            d = a[0] * a[7] - a[1] * a[6];
            if (d == 0.0) {
                return 0;
            }
            inv[0] = a[7] / d;
            inv[2] = -a[1] / d;
            inv[3] = -a[6] / d;
            inv[5] = a[0] / d;
            return 2;
        }
        inv[1] = a[7] / d;
        inv[2] = -a[4] / d;
        inv[4] = -a[6] / d;
        inv[5] = a[3] / d;
        return 2;
    }
    inv[0] = a[4] / d;
    inv[1] = -a[1] / d;
    inv[3] = -a[3] / d;
    inv[4] = a[0] / d;
    return 2;
}

// @src 140323de0
void swap4(void* p)
{
    auto* b = static_cast<unsigned char*>(p);
    unsigned char t = b[0];
    b[0] = b[3];
    b[3] = t;
    t = b[1];
    b[1] = b[2];
    b[2] = t;
}

// @src 140323e50
void swap_words(void* s, std::int32_t n)
{
    for (std::int32_t k = 0; k < n; k = k + 1) {
        swap4(static_cast<unsigned char*>(s) + (k << 2));
    }
}

// @src 140323ec0
void swap_halves(void* s, std::int32_t n)
{
    for (std::int32_t k = 0; k < n; k = k + 1) {
        unsigned char* lo = static_cast<unsigned char*>(s) + (k << 3);
        unsigned char* hi = static_cast<unsigned char*>(s) + (k * 8 + 4);
        unsigned char t[4];
        for (int c = 0; c < 4; ++c) t[c] = lo[c];
        for (int c = 0; c < 4; ++c) lo[c] = hi[c];
        for (int c = 0; c < 4; ++c) hi[c] = t[c];
    }
}

// @src 140324020
void text_byteswap(void* s, std::int32_t platform, std::int32_t n, std::int32_t keep_order)
{
    // 140324000 (ported as gendb's file-local platform_code): returns 2
    const std::int32_t code = 2 * 100 + platform;
    bool swap = false;
    if (0 < code) {
        if (code < 4) {
            swap = true;
        } else if (code == 100) {
            swap = true;
        } else if (code == 200) {
            swap = true;
        } else if (code == 300) {
            swap = true;
        }
    }
    if (swap) {
        swap_words(s, n << 1);
        if (keep_order == 0) {
            swap_halves(s, n);
        }
    }
}

} // namespace def::dbio::dbread
