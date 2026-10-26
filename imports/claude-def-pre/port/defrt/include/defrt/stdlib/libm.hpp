// defrt/stdlib/libm.hpp: the elementary functions DEF_PRE_64 links, ported from its own machine
// code so that results are bit-identical to the original (packet p1-01).
//
// Two libraries are linked:
//
//   * 0x1404b3440..0x1404b4dc4, the Fortran-runtime math library (Cody & Waite style algorithms,
//     scalar SSE2): what Fortran SIN, COS, TAN, ATAN, ATAN2, ASIN, ACOS, LOG, LOG10, SQRT, x**y
//     and x**n call, with degree (SIND ...) and by-reference entry points. Functions ps_*.
//   * 0x140bb3000 / 0x140bb3490, the MSVC CRT sin and cos (AMD LibM code) that C++ code such as
//     1403077e0 calls, with __remainder_piby2 (1404a0430) for |x| >= 5e5. Functions crt_*.
//
// None of these equals the UCRT's std::sin etc. bit for bit (e.g. ps_cos(0.5) differs in the last
// place), so ported code calls these, never <cmath>, wherever the original called them.
//
// Every function keeps the binary's operation order and its comparisons' NaN behaviour; the
// build's FP flags (/fp:precise, SSE2, no FMA contraction) are required.
#pragma once

#include <cstdint>

namespace def::rt::libm {

// ---- Fortran-runtime library: double ----
double ps_fabs(double x);                    // 1404b4898
double ps_sin(double x);                     // 1404b3ab0
double ps_cos(double x);                     // 1404b3ab8
double ps_sind(double x);                    // 1404b3ac4 (argument in degrees)
double ps_cosd(double x);                    // 1404b3ad4
void ps_sincos(double x, double* s, double* c); // 1404b3b30
double ps_tan(double x);                     // 1404b3df0
double ps_tand(double x);                    // 1404b3f18
double ps_atan(double x);                    // 1404b3f48
double ps_atand(double x);                   // 1404b40c8 (result in degrees)
double ps_atan2(double y, double x);         // 1404b3878
double ps_atan2d(double y, double x);        // 1404b3918
double ps_asin(double x);                    // 1404b3d50
double ps_acos(double x);                    // 1404b3d58
double ps_asind(double x);                   // 1404b3d64
double ps_acosd(double x);                   // 1404b3d7c
double ps_sqrt(double x);                    // 1404b4ac0
double ps_log(double x);                     // 1404b4264
double ps_log10(double x);                   // 1404b44c8
double ps_pow(double x, double y);           // 1404b3440 (x**y)
double ps_powi(double x, std::int32_t n);    // 1404b47fc (x**n, INTEGER*4 n)
double ps_powi64(double x, std::int64_t n);  // 1404b4848 (x**n, INTEGER*8 n)

// by-reference entry points (Fortran calling convention)
double ps_sin_r(const double* x);            // 1404b3ae8
double ps_cos_r(const double* x);            // 1404b3af4
double ps_sind_r(const double* x);           // 1404b3b04
double ps_cosd_r(const double* x);           // 1404b3b18
double ps_tan_r(const double* x);            // 1404b3f28
double ps_tand_r(const double* x);           // 1404b3f34
double ps_atan_r(const double* x);           // 1404b40e0
double ps_atand_r(const double* x);          // 1404b40ec
double ps_atan2_r(const double* y, const double* x);  // 1404b3930
double ps_atan2d_r(const double* y, const double* x); // 1404b3940
double ps_asin_r(const double* x);           // 1404b3d98
double ps_acos_r(const double* x);           // 1404b3da4
double ps_asind_r(const double* x);          // 1404b3db4
double ps_acosd_r(const double* x);          // 1404b3dd0
double ps_sqrt_r(const double* x);           // 1404b4b48
double ps_log_r(const double* x);            // 1404b44e0
double ps_log10_r(const double* x);          // 1404b44ec
double ps_fabs_r(const double* x);           // 1404b48b8

// ---- Fortran-runtime library: float (REAL*4) ----
float ps_fabsf(float x);                     // 1404b4d98
float ps_sinf(float x);                      // 1404b4a40
float ps_cosf(float x);                      // 1404b4a48
float ps_sindf(float x);                     // 1404b4a54
float ps_cosdf(float x);                     // 1404b4a64
void ps_sincosf(float x, float* s, float* c); // 1404b3b6c
float ps_logf(float x);                      // 1404b45a0
float ps_log10f(float x);                    // 1404b4794
float ps_powif(float x, std::int32_t n);     // 1404b4508
float ps_powif64(float x, std::int64_t n);   // 1404b4550
float ps_sinf_r(const float* x);             // 1404b4a78
float ps_cosf_r(const float* x);             // 1404b4a84
float ps_sindf_r(const float* x);            // 1404b4a94
float ps_cosdf_r(const float* x);            // 1404b4aa8
float ps_logf_r(const float* x);             // 1404b47ac
float ps_log10f_r(const float* x);           // 1404b47c4
float ps_fabsf_r(const float* x);            // 1404b4dac

// ---- MSVC CRT (AMD LibM) ----
double crt_sin(double x);                    // 140bb3000
double crt_cos(double x);                    // 140bb3490
// __remainder_piby2: x (>= 5e5, finite, positive) = n*pi/2 + r + rr, region = n mod 4.
void crt_remainder_piby2(double x, double* r, double* rr, std::int32_t* region); // 1404a0430

double crt_atan(double x);                   // 1404865ac
double crt_atan2(double y, double x);        // 140485d6c (y in xmm0, x in xmm1)
double crt_pow(double x, double y);          // 140483f60 (x**y; x87 FYL2X/F2XM1/FSCALE core)
double crt_acos(double x);                   // 140486fd4
double crt_tan(double x);                    // 140487b70 (core 14048799c)

} // namespace def::rt::libm
