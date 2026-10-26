// Cls_140182880 (no RTTI): element state-variable set (NUMEL) of an object, LLOBJ_DATA+0x9e0.
// Base Cls_140329c70 (p1-10, 8 bytes: COW list of StateVar pointers) at 0. The ctor registers 25
// element state variables (p1-09 classes 14032c8c0, 14032c950, 14032ca80, 14032cbb0, 14032ccd0,
// 14032ceb0), each bound to one of the data slots u020[].
//
//   ctor     140182880 (base ctor, u0f0 = 0, u0f8 = 3, init 1401829c0)
//   dtor     1401828e0 (free 140182a50, base dtor 140329cb0)
//   alloc    140183770(n, src, m)
//   copy     140183900(other, n, m, src, keep 0x3c, keep 0x31..0x34, keep 0x28)
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>

namespace def::model {

class LLOBJ_DATA;

class Cls_140182880 {
public:
    Cls_140182880();
    ~Cls_140182880();
    Cls_140182880(const Cls_140182880&) = delete;
    Cls_140182880& operator=(const Cls_140182880&) = delete;

    void init();                                                        // 1401829c0
    void free_arrays();                                                 // 140182a50
    void register_vars();                                               // 140182b10
    void alloc(std::int32_t n, const void* src, std::int32_t m);        // 140183770
    void copy_from(const Cls_140182880& other, std::int32_t n, std::int32_t m, const void* src,
                   bool keep_3c, bool keep_31_34, bool keep_28);        // 140183900
    std::int32_t fun_140183c20(std::int32_t i) const;                   // 140183c20
    bool u0f8_is_3() const;                                             // 14019a450
    void set_u0f8(std::int32_t v);                                      // 14019a3f0
    std::int32_t count() const;                                         // 1401854f0
    std::int32_t width() const;                                         // 140185530

    struct Reg {
        std::uint64_t ctor;   // p1-09 class ctor
        std::uint32_t size;   // operator new size
        std::int32_t id;
        std::uint32_t slot;   // offset of the data slot in this object
        bool pass_owner;      // ctor takes this as a 4th argument
    };
    static const Reg kRegs[25];

    void* base_list;          // 0x000 base Cls_140329c70 (p1-10)
    std::int32_t* u008;       // 0x008 n * m ints
    void* u010;               // 0x010 freed by 140182a50 (allocated elsewhere)
    std::int32_t* u018;       // 0x018 n * (int at src+0xf4) ints
    void* u020[25];           // 0x020..0x0e0 data slots of the registered variables
    std::int32_t n;           // 0x0e8 element count
    std::int32_t m;           // 0x0ec values per element
    LLOBJ_DATA* u0f0;         // 0x0f0 ctor = 0; owner (set by 140182920); 140183c20 reads +0x888
    std::int32_t u0f8;        // 0x0f8 ctor = 3 (14019a450 tests == 3)
    std::int32_t u0fc;        // 0x0fc init = 0
    std::int32_t u100;        // 0x100 init = 0
    std::byte pad_104[4];     // 0x104
};

static_assert(sizeof(Cls_140182880) == 0x108);
DEF_LAYOUT_OFFSET(Cls_140182880, u008, 0x008);
DEF_LAYOUT_OFFSET(Cls_140182880, u018, 0x018);
DEF_LAYOUT_OFFSET(Cls_140182880, u020, 0x020);
DEF_LAYOUT_OFFSET(Cls_140182880, n, 0x0e8);
DEF_LAYOUT_OFFSET(Cls_140182880, m, 0x0ec);
DEF_LAYOUT_OFFSET(Cls_140182880, u0f0, 0x0f0);
DEF_LAYOUT_OFFSET(Cls_140182880, u0f8, 0x0f8);
DEF_LAYOUT_OFFSET(Cls_140182880, u100, 0x100);

} // namespace def::model
