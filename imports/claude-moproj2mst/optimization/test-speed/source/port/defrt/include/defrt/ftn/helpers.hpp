// defrt/ftn/helpers.hpp: small Fortran helpers shared by the model, command and DB code
// (packet p1-12; they sit in defrt because defmodel code calls them). Fortran calling style:
// arguments by pointer, 1-based in the comments.
#pragma once

#include <cstdint>

namespace def::rt::ftn {

// @src 140238025  copy `*n` 4-byte words
void copy_words4(const void* src, void* dst, const std::int32_t* n);
// @src 1402380c5  copy `*n` 8-byte words
void copy_words8(const void* src, void* dst, const std::int32_t* n);
// @src 140437fc0  copy `*n` 8-byte words (a second copy of the same routine)
void copy_words8_b(const void* src, void* dst, const std::int32_t* n);

// @src 140443fc4  `*len` becomes 1 + the position of the last character of s(1:*len) that is
// neither blank nor NUL (1 when there is none)
void last_nonblank_plus1(std::int32_t* len, const char* s);

// @src 14023b1f0  c = a - b for 3-vectors
void vec3_sub(const double* a, const double* b, double* c);
// @src 14023b610  length of a 3-vector (0 exactly when the squared length is 0)
double vec3_length(const double* v);
// @src 14023b720  normalise a 3-vector in place, return its length
double vec3_normalize(double* v);

// @src 14043658f  rotate the 2D point p about c by the angle *ang (radians) into out
void rotate2d(const double* p, const double* c, const double* ang, double* out);

// @src 140437aa9  (thunk, arguments passed through)
// @src 140437b3b  min and max of column *col of a (ld = *ld, *n rows): with *init == 1 the
// bounds start at +1e20 / -1e20
void column_min_max(const double* a, const std::int32_t* ld, const std::int32_t* n,
                    const std::int32_t* col, double* vmin, double* vmax, const std::int32_t* init);

// @src 14045a140  for `*n` records of 6 doubles: if r[1] == 2 then r[6] *= pi/180, else r[6] = 0
void degrees_to_radians_if_flagged(double* recs, const std::int32_t* n);

// @src 140235620  strncmp over *len characters into *res
void ftn_strncmp(const char* a, const char* b, const std::int32_t* len, std::int32_t* res);
// @src 140235670  the same (a second copy)
void ftn_strncmp_b(const char* a, const char* b, const std::int32_t* len, std::int32_t* res);

} // namespace def::rt::ftn
