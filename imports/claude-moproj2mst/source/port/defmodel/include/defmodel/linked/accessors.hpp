// defmodel/linked/accessors.hpp: one-line accessors of p1-06's address range whose class is not
// identified yet (or belongs to another packet). They read and write raw offsets exactly as the
// original; when the owning class is known they become its methods (same @src tags).
//
// Callers (from the decompile): 140256710..1402567a0 are used by the geometry/mesh code
// (140280760, 140249700, 1402a9f90, 140399120); 14026a3c0 by the 3D geometry merge 1402fa4f0
// and its relatives (node coordinate arrays).
#pragma once

#include <cstdint>

namespace def::model {
class Cls_1401e7b10;
}

namespace def::model::linked {

// @src 140256710  int at +0x44
std::int32_t get_i32_44(const void* obj);
// @src 140256720  int at +0x18
std::int32_t get_i32_18(const void* obj);
// @src 140256730  8 bytes at +0x18
void set_u64_18(void* obj, std::uint64_t v);
// @src 140256750  8 bytes at +0x20
void set_u64_20(void* obj, std::uint64_t v);
// @src 140256770  int at +0x24
std::int32_t get_i32_24(const void* obj);
// @src 1402568e0  8 bytes at (*handle)+0x10 (callers use the low 32 bits: a count)
std::uint64_t deref_u64_10(void* const* handle);
// @src 140256780  140159c20 (p1-04, same body as 1402568e0) on the handle at +0x10
std::uint64_t member10_deref_u64_10(const void* obj);
// @src 1402567a0  1402568e0 on the handle at +0x20
std::uint64_t member20_deref_u64_10(const void* obj);
// @src 14026a3c0  &element 0 of the 8-byte-element array whose handle is at +0x10
//                 (14026a470(obj + 0x10, 0))
double* member10_array_first(const void* obj);

// @src 1401f4390  simulation control: DSMAX (dsmax, +0x1e0) = v (8-byte store)
void set_dsmax(Cls_1401e7b10* sim, double v);

// @src 1401fc040  int at **(obj+0x9f8) when the flag at +0x87c is set and the pointer is not null,
//                 else 0 (caller 1401d7250 passes it on to 1401d72a0)
std::int32_t fun_1401fc040(const void* obj);

} // namespace def::model::linked
