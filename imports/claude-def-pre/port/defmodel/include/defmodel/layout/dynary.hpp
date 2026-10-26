// DYNARY (no RTTI, not polymorphic; ctor 14012d050): a tabulated property function (constant,
// or a table over up to 3 independent variables with ncomp values per point). Used for most
// material, phase-pair, simulation-control and inter-object properties.
// The name follows docs/re/keywords/_common_g457.md section 4. That document puts it in the
// "RTTI family DYNARY_DATA", but it is not that class: RTTI DYNARY_DATA (dynary_data.hpp) is a
// polymorphic 0x40-byte SfLinkedData node (operator new(0x40) in 14025ba40). This class has no
// vptr: its ctor writes the int type at offset 0.
//
//   ctor     14012d050 (zero, then 14012d2a0(this, 0))
//   init     14012d2a0(this, type): free (14012f480), set type/shape, per-type defaults, then
//            allocate (14012fad0) unless type is 0x51/0x65/0x66/0x67. 14012fc70 is the same but
//            keeps +0x54 and +0x60 (spec).
//   dtor     14012f410 (14012f480 frees 0x28/0x30/0x38/0x48 unless shared, then deletes the
//            object at 0x68 through its scalar deleting dtor 14013c170(p, 1))
//   size     0x78: back-to-back members in Cls_1401e7b10 (0x9e0, 0xa58, 0xad0, 0xb48), Material
//            (0x670, 0x6e8, 0x760) and PhasePair (0x478, 0x4f0, 0x568); highest write is the
//            8-byte field at 0x68.
//   open     0x5c and 0x70 are never written by ctor/init/dtor. 0x08 is a double (case 0xae
//            stores 1.0 there), but cases 0xaf/0xb3/0xb6 clear it as two ints.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class DYNARY {
public:
    // @src 14012d050  ctor
    DYNARY();
    // @src 14012f410  dtor
    ~DYNARY();

    std::int32_t type;        // 0x00 property id (14012d2a0 param_2)
    std::int32_t ndim;        // 0x04 0 = constant, 1..3 = table dimensions
    double value;             // 0x08 constant value (ndim 0)
    std::int32_t var[3];      // 0x10 independent-variable code per dimension (3 = temperature,
                              //      7 = strain, 8 = strain rate, 1 = time; per the spec)
    std::int32_t n[3];        // 0x1c points per dimension (14012f480 tests > 0 before freeing)
    double* axis[3];          // 0x28 abscissae per dimension (calloc in 14012fad0)
    std::int32_t ncomp;       // 0x40 values per table point (default 1)
    std::byte pad_44[4];      // 0x44
    double* values;           // 0x48 ncomp * prod(n) doubles (14012ff20 gives the count)
    std::int32_t u50;         // 0x50 cleared on init; per-keyword flag (FSTRES table flag)
    std::int32_t u54;         // 0x54 kept by 14012fc70
    std::int32_t u58;         // 0x58 cleared on init
    std::byte pad_5c[4];      // 0x5c never written in ctor/init
    std::int32_t u60;         // 0x60 init [0x18]=0; 0x60..0x67 kept by 14012fc70
    std::int32_t u64;         // 0x64 init [0x19]=0
    void* u68;                // 0x68 owned object, deleted by 14012f410 via 14013c170(p, 1)
    std::byte pad_70[8];      // 0x70 never written in ctor/init/dtor
};

static_assert(sizeof(DYNARY) == 0x78);
DEF_LAYOUT_OFFSET(DYNARY, ndim, 0x04);
DEF_LAYOUT_OFFSET(DYNARY, value, 0x08);
DEF_LAYOUT_OFFSET(DYNARY, var, 0x10);
DEF_LAYOUT_OFFSET(DYNARY, n, 0x1c);
DEF_LAYOUT_OFFSET(DYNARY, axis, 0x28);
DEF_LAYOUT_OFFSET(DYNARY, ncomp, 0x40);
DEF_LAYOUT_OFFSET(DYNARY, values, 0x48);
DEF_LAYOUT_OFFSET(DYNARY, u50, 0x50);
DEF_LAYOUT_OFFSET(DYNARY, u58, 0x58);
DEF_LAYOUT_OFFSET(DYNARY, u60, 0x60);
DEF_LAYOUT_OFFSET(DYNARY, u68, 0x68);

} // namespace def::model
