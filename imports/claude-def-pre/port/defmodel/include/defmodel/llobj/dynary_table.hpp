// defmodel/llobj/dynary_table.hpp: p1-04's functions on DYNARY (the 0x78-byte table object,
// layout/dynary.hpp, owned by the material threads) and on Cls_1401364a0, the list of DYNARY.
#pragma once

#include <cstdint>

#include <defmodel/layout/dynary.hpp>
#include <defrt/rdata.hpp>

namespace def::model::llobj {

// rdata 140509b50  1e30 (upper bound for a stored table value)
inline constexpr double kDynaryValueMax = def::rt::rdata::d_140509b50;
// @data 14051bee0  1e-30 (lower bound; smaller magnitudes are stored as 0)
inline constexpr double kDynaryValueMin = 1e-30;

// Index of (i, j, k) in dimensions 0..2 and component c into DYNARY::values, -1 when out of range
// or ndim == 0. ndim == 1 stores i * ncomp + c; otherwise i + j*n0 + k*n0*n1 + c*n0*n1*n2 (each
// n clamped to at least 1).
std::int32_t dynary_index(const DYNARY& d, std::int32_t i, std::int32_t j, std::int32_t k, std::int32_t c);
// values[index], or 0 when values is null or the index is -1.
double dynary_get(const DYNARY& d, std::int32_t i, std::int32_t j, std::int32_t k, std::int32_t c);
// values[index] = v when 1e-30 < |v| < 1e30, else 0 (no-op when values is null or index -1).
void dynary_set(DYNARY& d, double v, std::int32_t i, std::int32_t j, std::int32_t k, std::int32_t c);

// DYNARY scalar deleting dtor: dtor, then free when flags & 1.
DYNARY* dynary_delete(DYNARY* p, unsigned flags);

// The pointer at DYNARY+0x60 links the nodes of Cls_1401364a0 (DYNARY's u60/u64 as one field).
DYNARY* dynary_next(const DYNARY* p);
void dynary_set_next(DYNARY* p, DYNARY* next);

// int32 at +0x10 / +0x1c of the caller's object (the caller class is not identified yet; on a
// DYNARY these are var[0] and n[0]).
std::int32_t int_at_10(const void* self);
std::int32_t int_at_1c(const void* self);

} // namespace def::model::llobj
