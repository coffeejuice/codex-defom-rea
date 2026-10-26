// Cls_14017ef20 (no RTTI): node state-variable set (NUMNP) of an object, LLOBJ_DATA+0x8a0.
// Base Cls_140329c70 (p1-10, 8 bytes: COW list of StateVar pointers) at 0. The ctor registers 28
// node state variables (p1-09 classes 14032a990/14032aa20), each bound to one of the 8-byte
// data slots u010[] (ids below), and the node arrays are (re)allocated per node count.
//
//   ctor     14017ef20 (base ctor 140329c70, u130 = 0, u138 = 3, init 14017f060)
//   dtor     14017ef80 (free 14017f130, base dtor 140329cb0)
//   alloc    140180150(n, dims)    copy 140180390(other, n, dims, keep_id17)
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class Cls_14017ef20 {
public:
    Cls_14017ef20();
    ~Cls_14017ef20();
    Cls_14017ef20(const Cls_14017ef20&) = delete;
    Cls_14017ef20& operator=(const Cls_14017ef20&) = delete;

    void init();                                            // 14017f060
    void free_arrays();                                     // 14017f130
    void register_vars();                                   // 14017f2f0
    void alloc(std::int32_t n, const std::int32_t* dims);   // 140180150
    void copy_from(const Cls_14017ef20& other, std::int32_t n, const std::int32_t* dims,
                   bool keep_id17);                         // 140180390
    std::int32_t count() const;                             // 140182840
    void set_u138(std::int32_t v);                          // 14019a370

    // State-variable ids and data slots registered by 14017f2f0, in registration order.
    struct Reg {
        std::uint64_t ctor;   // p1-09 class ctor
        std::int32_t id;
        std::uint32_t slot;   // offset of the data slot in this object
    };
    static const Reg kRegs[28];

    void* base_list;          // 0x000 base Cls_140329c70 (p1-10)
    double* u008;             // 0x008 n * dims[0] doubles
    void* u010[28];           // 0x010..0x0e8 data slots of the registered variables
    std::int32_t* u0f0[7];    // 0x0f0..0x120 int arrays (0xf0..0x118 sized by dims[0x12..0x17])
    std::int32_t u128;        // 0x128 init = 300
    std::int32_t n;           // 0x12c node count (140180150; 140182840 returns it)
    void* u130;               // 0x130 ctor = 0; owner (set by 14017efc0)
    std::int32_t u138;        // 0x138 ctor = 3
    std::int32_t u13c;        // 0x13c init/free = 0
};

static_assert(sizeof(Cls_14017ef20) == 0x140);
DEF_LAYOUT_OFFSET(Cls_14017ef20, u008, 0x008);
DEF_LAYOUT_OFFSET(Cls_14017ef20, u010, 0x010);
DEF_LAYOUT_OFFSET(Cls_14017ef20, u0f0, 0x0f0);
DEF_LAYOUT_OFFSET(Cls_14017ef20, u128, 0x128);
DEF_LAYOUT_OFFSET(Cls_14017ef20, n, 0x12c);
DEF_LAYOUT_OFFSET(Cls_14017ef20, u130, 0x130);
DEF_LAYOUT_OFFSET(Cls_14017ef20, u138, 0x138);
DEF_LAYOUT_OFFSET(Cls_14017ef20, u13c, 0x13c);

} // namespace def::model
