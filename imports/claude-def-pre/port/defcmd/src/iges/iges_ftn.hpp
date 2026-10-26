// Internal declarations of the embedded Fortran IGES library (IgesInterface.f / IgesCore.f,
// PathScale) and its C helpers, packet p8-03. Spec: docs/re/commands/IGES_READER.md.
//
// Conventions (Fortran): every argument is passed by reference, arrays are 1-based.
//   RW  real work array, "slots" of 3 doubles: slot i component c = rw[(i-1)*3 + c-1]
//   IW  integer work array, records of 5 ints:  record i field f = iw[(i-1)*5 + f-1]
//   IW2 output records {kind, k, first, last, source record}; the second RW part (RW2) holds the
//   output points (3 doubles each).
// SAVE variables of the routines that can be read before they are written in a call (and so
// carry a value from one call to the next) live in g_ftn; the others are locals.
#pragma once

#include <cstdint>

#include <emmintrin.h>

namespace def::cmd::iges {

// CVTTSD2SI (0x80000000 for NaN and out-of-range values)
inline std::int32_t ftn_trunc(double x) { return _mm_cvttsd_si32(_mm_set_sd(x)); }

// NINT as PathScale inlines it: |x| >= 2^52 (or NaN) -> x, else x + (x >= 0 or NaN ? 0.5 : -0.5);
// then CVTTSD2SI (constants 140bb2290/98/a8/b0).
inline std::int32_t ftn_nint(double x)
{
    const double ax = _mm_cvtsd_f64(_mm_and_pd(_mm_set_sd(x), _mm_castsi128_pd(_mm_set1_epi64x(0x7fffffffffffffffLL))));
    const bool big = !(ax < 4503599627370496.0);
    const double h = !(x < 0.0) ? 0.5 : -0.5;
    return ftn_trunc(big ? x : h + x);
}

struct FtnState {
    // 14077ac00.. pass-1/pass-2 shared indexes (COMMON)
    double ac00 = 0.0;          // 14077ac00 shortest point distance
    std::int32_t ac08 = 0;      // 14077ac08
    std::int32_t ac0c = 0;      // 14077ac0c start of RW2 (1-based in RW)
    std::int32_t ac10 = 0;      // 14077ac10 start of D in RW
    std::int32_t ac14 = 0;      // 14077ac14
    std::int32_t ac18 = 0;      // 14077ac18 start of IW2 (1-based in IW)
    std::int32_t ac1c = 0;      // 14077ac1c start of the L pairs in IW
    std::int32_t dc4 = 0, dc8 = 0, dcc = 0, dd0 = 0, dd4 = 0; // 140721dc4.. pass-2 int arrays
    std::int32_t dd8 = 0;       // 140721dd8 number of loops
    std::int32_t ddc = 0;       // 140721ddc segment count
    std::int32_t de8 = 0;       // 140721de8 retry flag of the pass-1 reader
    std::int32_t f74 = 0;       // 140721f74 count of type -102 entities
    std::int32_t f78 = 0;       // 140721f78 copy status
    std::int32_t f8c = 0;       // 140721f8c copy piece count
    char tmp[0x100] = {};       // 140721df0 CHARACTER*256 temporary file name
    char suf[0x10] = {};        // 140721ef0 CHARACTER*16 "_TMP_SFTC"
    // values that survive between calls
    double c850 = 0.0;          // 14072c850 last field value (FUN_140468960 keeps it on len < 1)
    std::int32_t c910 = 0;      // 14072c910 best partner record (FUN_140464abc)
    std::int32_t r0a4 = 0;      // 1407220a4 last row (FUN_140459607)
    std::int32_t d698 = 0;      // 14072d698 last row (FUN_1404665f3)
    std::int32_t r708 = 0;      // 140722708 row counter (FUN_14045979e, never reset)
    std::int32_t d628 = 0;      // 14072d628 row counter (FUN_140465bb1, never reset)
    std::int32_t d644 = 0;      // 14072d644 unit argument passed on by FUN_140465bb1 (never set)
    std::int32_t d858 = 0, d85c = 0, d864 = 0, d868 = 0, d86c = 0; // FUN_140467e54
};
inline FtnState g_ftn{};

// ---- pass 1 (iges_pass1.cpp) ----
void fun_1404570f0(char* nm, std::int32_t* len, std::int32_t* na, std::int32_t* nb, std::int32_t* n100,
                   std::int32_t* n110, std::int32_t* n112, std::int32_t* n126, std::int32_t* n124,
                   double* tol, double* rw, std::int32_t* iw, std::int32_t* rwsize, std::int32_t* iwsize,
                   std::int32_t* st);
void fun_1404579fc(char* nm, std::int32_t* unused, double* rw, std::int32_t* na, std::int32_t* nb,
                   std::int32_t* iw, std::int32_t* n100, std::int32_t* n110, std::int32_t* n112,
                   std::int32_t* n126, std::int32_t* n124, std::int32_t* st, std::int32_t nm_len);
void fun_140459c0d(char* src, char* suffix, std::int32_t* count, std::int32_t* st);
void fun_140460f50(char* buf, char* sec, std::int32_t* iw, std::int32_t* nb, std::int32_t* n110,
                   std::int32_t* n100, std::int32_t* n124, std::int32_t* n112, std::int32_t* n126,
                   std::int32_t* n102, std::int32_t unit);
void fun_140468677(std::int32_t* a, const std::int32_t* key, const std::int32_t* n, const std::int32_t* asc,
                   const std::int32_t* ncol);
void fun_140461b95(char* buf, char* sec, double* rw, std::int32_t* iw, std::int32_t* nb, std::int32_t* na,
                   std::int32_t unit);
void fun_140462631(double* a, double* b, std::int32_t* n, std::int32_t* nslots, const std::int32_t* dir);
void fun_1404627bf(double* blk, double* v, std::int32_t* na, std::int32_t* cur, std::int32_t unit,
                   std::int32_t* rec, std::int32_t* cnt, std::int32_t* rd);
void fun_140462b00(double* blk, double* v, std::int32_t* na, std::int32_t* cur, std::int32_t unit,
                   std::int32_t* rec, std::int32_t* cnt, std::int32_t* rd);
void fun_140463e15(double* dest, std::int32_t* got, std::int32_t* cur, const std::int32_t* want,
                   std::int32_t unit);
void fun_14046414e(const double* v, double* slots);
void fun_140464245(const char* rec, double* vals, std::int32_t* n);
void fun_140468960(const char* s, const std::int32_t* len, double* value, std::int32_t* err, std::int32_t slen);

// ---- geometry build and tolerance (iges_geom.cpp) ----
void fun_140457196(char* nm, std::int32_t* len, std::int32_t* na, std::int32_t* nb, std::int32_t* n100,
                   std::int32_t* n110, std::int32_t* n112, std::int32_t* n126, std::int32_t* n124,
                   const std::int32_t* nsamp, double* tol, double* rw, std::int32_t* iw, std::int32_t* rwsize,
                   std::int32_t* iwsize, std::int32_t* st);
void fun_14046519b(double* rw, std::int32_t* iw, double* rw2, std::int32_t* iw2, std::int32_t* na,
                   std::int32_t* nb, const std::int32_t* nsamp);
void fun_140462e15(const double* blk, const double* t, std::int32_t* cur, double* out, const std::int32_t* nsteps);
void fun_140463267(const double* blk, double* out, const std::int32_t* nsamp, std::int32_t* cur);
void fun_1404635be(const double* t, const std::int32_t* j, const std::int32_t* m, const double* u, double* nt);
void fun_1404639eb(const double* ctrl, const double* w, const double* t, const std::int32_t* j,
                   const std::int32_t* m, const double* u, const double* nt, double* out);
void fun_1404645b0(const double* rw, double* rw2, const std::int32_t* iw, const std::int32_t* nb,
                   const std::int32_t* self, const std::int32_t* kind, const std::int32_t* first,
                   const std::int32_t* last, const std::int32_t* trf);
void fun_140464720(const double* r, const double* t, double* p);
void fun_1404648ff(const double* p, const std::int32_t* np, const std::int32_t* iw2, const std::int32_t* nb,
                   double* dmin);
void fun_140467142(const double* src, double* out);
void fun_1404681dc(void* unused, const double* p, const double* q, double* d);
void fun_14046736c(const double* a, const double* b, const double* c, double* ang);
void fun_1404672b2(double* v, const double* ang);

// ---- pass 2 (iges_pass2.cpp) ----
void fun_140457312(std::int32_t* na, std::int32_t* nb, double* tol, double* rw, std::int32_t* iw,
                   double* table, const std::int32_t* rwsize, const std::int32_t* iwsize,
                   const std::int32_t* mode, const std::int32_t* flag, std::int32_t* st);
void fun_140458e6f(const std::int32_t* nb, const double* table, double* p, std::int32_t* iw2, std::int32_t* np);
void fun_140464abc(double* p, const std::int32_t* iw2, const std::int32_t* nb, const std::int32_t* np,
                   std::int32_t* l, const double* tol, double* d);
void fun_140467473(double* p, const std::int32_t* iw2, const double* tol, const std::int32_t* np,
                   const std::int32_t* nb, const std::int32_t* l, double* unused, double* d,
                   std::int32_t* starts, std::int32_t* nloops, std::int32_t* s, std::int32_t* a,
                   std::int32_t* b, std::int32_t* m, const std::int32_t* flag, std::int32_t* st);
void fun_140467e54(const double* p, const std::int32_t* iw2, std::int32_t* s, const std::int32_t* n);
void fun_140459149(const std::int32_t* unused, double* table, const double* p, const std::int32_t* iw2,
                   const std::int32_t* s, const std::int32_t* nloops, const std::int32_t* starts,
                   std::int32_t* nseg);
void fun_1404594a3(std::int32_t* k, double* table, const double* p, const std::int32_t* rev);
void fun_140459607(std::int32_t* k, double* table, const double* p, const std::int32_t* rev,
                   const std::int32_t* n);
void fun_14045979e(double* table, const double* p, const double* comp, std::int32_t* k,
                   const std::int32_t* rev, const std::int32_t* iw2);
void fun_1404587d1(const double* p, const std::int32_t* iw2, const std::int32_t* s, const std::int32_t* nloops,
                   const std::int32_t* starts, const std::int32_t* unit);
void fun_1404665f3(const double* p, std::int32_t* k, const std::int32_t* n, const std::int32_t* rev,
                   const std::int32_t* unit);
void fun_1404668e4(double* p, const std::int32_t* mode, const std::int32_t* k, const std::int32_t* rev,
                   const std::int32_t* unit);
void fun_140465bb1(const double* p, const double* comp, std::int32_t* k, const std::int32_t* rev,
                   const std::int32_t* iw2);

// ---- C helpers (iges_driver.cpp) ----
// @src 1401b9100 (declared here for the Fortran caller 140459c0d)
void fun_1401b9100(const char* src, const char* suffix, std::int32_t* count, std::int32_t* st);

// Resets g_ftn and the SAVE data of the routines (tests).
void reset_ftn_state();
void reset_pass1_state();

} // namespace def::cmd::iges
