// Cls_14018b220 (defmodel/llobj/cls_14018b220.hpp).
#include <defmodel/llobj/cls_14018b220.hpp>

#include <defmodel/layout/dynary.hpp>
#include <defmodel/llobj/ext.hpp>
#include <defmodel/port.hpp>

namespace def::model {

// ctor 14018b220: defined in src/llobj_ctor/member_ctors.cpp (kw-g2obj)

// @src 14018b3b0  dtor: reset, then the tables from the last to the first
Cls_14018b220::~Cls_14018b220()
{
    reset();
    for (int t = 15; t >= 0; --t)
        llobj::ext::dynary_dtor(reinterpret_cast<DYNARY*>(raw + kTables[t]));  // 14012f410
}

// @src 14018bc30  free 13 of the tables (14012f480) in the binary's order; flag 0x8b8 = 0.
// The tables at 0x338, 0x518 and 0x590 are not freed here.
void Cls_14018b220::reset()
{
    static constexpr std::uint32_t before_flag[8] = {0x0e8, 0x070, 0x208, 0x2a0, 0x3b0, 0x448, 0x660, 0x6d8};
    static constexpr std::uint32_t after_flag[5] = {0x8c0, 0x968, 0x9e0, 0x790, 0x808};
    for (std::uint32_t off : before_flag)
        llobj::ext::f14012f480(reinterpret_cast<DYNARY*>(raw + off));
    raw[0x8b8] = std::byte{0};
    for (std::uint32_t off : after_flag)
        llobj::ext::f14012f480(reinterpret_cast<DYNARY*>(raw + off));
}

} // namespace def::model
