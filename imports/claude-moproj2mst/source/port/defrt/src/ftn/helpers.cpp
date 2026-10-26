// Small Fortran helpers. See defrt/ftn/helpers.hpp.
#include <defrt/ftn/helpers.hpp>
#include <defrt/stdlib/libm.hpp>

#include <bit>
#include <cmath>
#include <cstring>

namespace def::rt::ftn {

// @src 140238025  4-byte word copy (unrolled by 3 in the original, forward)
void copy_words4(const void* src, void* dst, const std::int32_t* n)
{
    const auto* s = static_cast<const std::int32_t*>(src);
    auto* d = static_cast<std::int32_t*>(dst);
    for (std::int32_t i = 1; i <= *n; ++i) {
        d[i - 1] = s[i - 1];
    }
}

// @src 1402380c5  8-byte word copy (forward)
void copy_words8(const void* src, void* dst, const std::int32_t* n)
{
    const auto* s = static_cast<const std::uint64_t*>(src);
    auto* d = static_cast<std::uint64_t*>(dst);
    for (std::int32_t i = 1; i <= *n; ++i) {
        d[i - 1] = s[i - 1];
    }
}

// @src 140437fc0  8-byte word copy
void copy_words8_b(const void* src, void* dst, const std::int32_t* n) { copy_words8(src, dst, n); }

// @src 140443fc4  length of a Fortran string + 1
void last_nonblank_plus1(std::int32_t* len, const char* s)
{
    std::int32_t r = 1;
    std::int32_t i = 1;
    do { // the original tests s(1) even when *len < 1
        const char c = s[i - 1];
        if (c != ' ' && c != '\0') {
            r = i + 1;
        }
        ++i;
    } while (i <= *len);
    *len = r;
}

// @src 14023b1f0  vector difference
void vec3_sub(const double* a, const double* b, double* c)
{
    c[0] = a[0] - b[0];
    c[1] = a[1] - b[1];
    c[2] = a[2] - b[2];
}

// @src 14023b610  vector length; FUN_140484644 is sqrt
double vec3_length(const double* v)
{
    const double s = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    if (s == 0.0) { // DAT_14051bed8 = 0.0
        return 0.0;
    }
    return std::sqrt(s);
}

// @src 14023b720  normalise
double vec3_normalize(double* v)
{
    const double len = vec3_length(v);
    if (len != 0.0) {
        v[0] = v[0] / len;
        v[1] = v[1] / len;
        v[2] = v[2] / len;
    }
    return len;
}

// @src 14043658f  rotation about a centre: sincos through the Fortran runtime (1404b3b30), then
// the listing's order (140436653..140436677): out(2) = dy*c + (dx*s + cy), out(1) = (c*dx + cx) - s*dy
void rotate2d(const double* p, const double* c, const double* ang, double* out)
{
    const double cy = c[1];
    const double cx = c[0];
    const double dx = p[0] - c[0]; // DAT_14070d8f0
    const double dy = p[1] - cy;   // DAT_14070d8f8
    double s;
    double co;
    def::rt::libm::ps_sincos(*ang, &s, &co);
    const double t = dx * s + cy;
    const double u = co * dx + cx;
    out[1] = dy * co + t;
    out[0] = u - s * dy;
}

// @src 140437aa9  thunk to FUN_140437b3b
// @src 140437b3b  column min / max
void column_min_max(const double* a, const std::int32_t* ld, const std::int32_t* n,
                    const std::int32_t* col, double* vmin, double* vmax, const std::int32_t* init)
{
    const std::int64_t lda = *ld < 0 ? 0 : *ld;
    if (*init == 1) {
        *vmin = std::bit_cast<double>(0x4415af1d78b58c40ULL); // DAT_140bb15c8 = 1e20
        *vmax = std::bit_cast<double>(0xc415af1d78b58c40ULL); // DAT_140bb15d0 = -1e20
    }
    for (std::int32_t k = 1; k <= *n; ++k) {
        const double v = a[(*col + (static_cast<std::int64_t>(k) - 1) * lda) - 1];
        if (v < *vmin) {
            *vmin = v;
        }
        if (*vmax <= v && v != *vmax) {
            *vmax = v;
        }
    }
}

// @src 14045a140  angle field of 6-double records
void degrees_to_radians_if_flagged(double* recs, const std::int32_t* n)
{
    for (std::int32_t k = 1; k <= *n; ++k) {
        double* r = recs + static_cast<std::ptrdiff_t>(k - 1) * 6;
        if (r[0] == 2.0) { // DAT_140bb2250
            r[5] = r[5] * std::bit_cast<double>(0x3f91df46a2529d39ULL); // DAT_140bb2258 = pi/180
        } else {
            r[5] = 0.0;
        }
    }
}

// @src 140235620  strncmp wrapper
void ftn_strncmp(const char* a, const char* b, const std::int32_t* len, std::int32_t* res)
{
    *res = std::strncmp(a, b, static_cast<std::size_t>(static_cast<std::int64_t>(*len)));
}

// @src 140235670  strncmp wrapper
void ftn_strncmp_b(const char* a, const char* b, const std::int32_t* len, std::int32_t* res)
{
    ftn_strncmp(a, b, len, res);
}

} // namespace def::rt::ftn
