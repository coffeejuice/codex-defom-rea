// defmodel/dynary_init.hpp: the DYNARY (table / curve object, Cls_14012d050) template family.
// Owned by the key-common packet; bodies in src/table/dynary_template.cpp.
// Spec: docs/re/keywords/_common.md section 3. Layout: defmodel/layout/dynary.hpp.
//
// The axis and value arrays are calloc'd and released with free(), like the original.
//
//   dynary_init(t, type)         14012d2a0  free arrays, reset the shape, apply the template of
//                                           `type`, then allocate (types 0x51, 0x65, 0x66 and
//                                           0x67 stay unallocated). u5c, u68 and u70 untouched.
//                                           Unlisted types (0, 1, 2, 0x15..0x17, 0x1d, 0x1e,
//                                           0x4e, ...) get the default ndim 0, ncomp 1.
//   dynary_reinit_keep(t, type)  14012fc70  dynary_init, keeping u54 and the 8 bytes at 0x60.
//   dynary_alloc(t)              14012fad0  free, clamp, calloc n[i] doubles per axis i < ndim and
//                                           dynary_value_count(t) doubles of values.
//   dynary_value_count(t)        14012ff20  ndim 0: ncomp if ncomp >= 2 or type is 0xb5, 0xa7,
//                                           0xa8, 0xaa or 0xab, else 0; ndim > 0: ncomp * prod(n)
//                                           (32-bit, negative -> 0).
//   dynary_clamp_shape(t)        14012fc00  n[i] < 0 -> 0, ncomp < 1 -> 1.
//   dynary_counts_positive(t)    14012f610  n[i] >= 1 for every i < ndim (the writer's guard).
//   dynary_free_arrays(t)        14012f480  free axis/values unless a dimension with n == 0 still
//                                           has an axis pointer (shared arrays); always clears
//                                           the four pointers. Also in p1-06's map row.
//
// Unit conversion (src/table/dynary_units.cpp). Both do nothing unless mode != 0, every
// n[i] >= 1 and ncomp == 1. With convert_axes, each axis d is converted by the unit kind of its
// variable (kVarUnitKind[var[d]], DAT_14051b7f0): kind 9 (temperature) through
// units.temperature(mode, x), otherwise x * units.factor(kind, mode). Then the constant and the
// dynary_value_count(t) values (while `values` is non-null) are converted as quantity `kind`:
//   dynary_convert_units(t, kind, mode, axes)          14012e400: kind 9 -> temperature(mode, v);
//       else v * f with f = units.factor(kind, mode), or 1.0 / f when t->type == 0x2a.
//   dynary_convert_units_inverse(t, kind, mode, axes)  14012e6d0: kind 9 -> 1.0 / temperature(mode, v);
//       else v * (1.0 / units.factor(kind, mode)).
// The unit functions belong to p1-12 (140235d00 factor, 140235d80 temperature); callers pass them
// in so that defmodel does not depend on where p1-12 puts them.
#pragma once

#include <cstdint>

#include <defmodel/layout/dynary.hpp>

namespace def::model {

void dynary_init(DYNARY* t, int type);
void dynary_reinit_keep(DYNARY* t, int type);
void dynary_alloc(DYNARY* t);
std::int32_t dynary_value_count(const DYNARY* t);
void dynary_clamp_shape(DYNARY* t);
bool dynary_counts_positive(const DYNARY* t);
void dynary_free_arrays(DYNARY* t);

struct UnitFunctions {
    double (*factor)(int kind, int mode);     // 140235d00
    double (*temperature)(int mode, double t); // 140235d80
};

// @data 14051b7f0  unit kind per table variable code. Entries 0..17 are the table; 18..33 are the
// zero bytes that follow it in .rdata (template var codes go up to 0x20).
inline constexpr std::int32_t kVarUnitKind[34] = {11, 0, 1, 9, 7, 5, 7, 11, 18, 11, 11, 2, 16, 1, 35, 7, 7, 11,
                                                 0,  0, 0, 0, 0, 0, 0, 0,  0,  0,  0, 0, 0,  0, 0, 0};

void dynary_convert_units(DYNARY* t, int kind, int mode, bool convert_axes, const UnitFunctions& units);
void dynary_convert_units_inverse(DYNARY* t, int kind, int mode, bool convert_axes, const UnitFunctions& units);

} // namespace def::model
