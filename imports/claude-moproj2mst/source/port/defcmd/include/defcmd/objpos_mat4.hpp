// defcmd/objpos_mat4.hpp: 4x4 transform helpers of OBJPOS (packet p67-3; p67-1 uses them through
// __has_include). Matrices are double[16], row-major M[i*4+j]; points are row vectors.
// Spec: docs/re/commands/OBJPOS_drop.md §4 step 6, §8.
#pragma once

namespace def::cmd::objpos {

// @src 140240fc0  M = identity (1.0 is DAT_140509c90)
void fun_140240fc0(double* m);
// @src 140241070  dst = src (16 doubles)
void fun_140241070(double* dst, const double* src);
// @src 140241110  M := X * M: out[c][a] = sum_k X[c][k] * M[k][a], summed from 0.0 in k order
void fun_140241110(double* m, const double* x);

} // namespace def::cmd::objpos
