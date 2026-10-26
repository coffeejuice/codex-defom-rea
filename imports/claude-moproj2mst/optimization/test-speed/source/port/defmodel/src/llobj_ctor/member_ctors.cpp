// Ctors of the LLOBJ_DATA sub-object classes whose dtors p1-04 ported (defmodel/llobj/cls_*.hpp).
// Phase 2, packet kw-g2obj. Decompiles: deform-rea-work/out/g2obj/p2_rest.c.
#include <new>

#include <defmodel/layout/dynary.hpp>
#include <defmodel/llobj/cls_140185c60.hpp>
#include <defmodel/llobj/cls_1401860a0.hpp>
#include <defmodel/llobj/cls_1401889a0.hpp>
#include <defmodel/llobj/cls_140189440.hpp>
#include <defmodel/llobj/cls_14018b220.hpp>
#include <defmodel/llobj/cls_1401956d0.hpp>
#include <defmodel/llobj/cls_140197a50.hpp>
#include <defmodel/llobj/ext.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defmodel/llobj_ctor/obj_defaults.hpp>

namespace def::model {

namespace {
std::byte* bytes_of(void* p) { return static_cast<std::byte*>(p); }
template <class T>
T& at(void* p, std::size_t off)
{
    return *reinterpret_cast<T*>(bytes_of(p) + off);
}
} // namespace

// @src 140185c60  base ctor 140329c70 (p1-10), then 140185ce0
// @src 140185ce0  +8 = 0, +0x10 = 0
Cls_140185c60::Cls_140185c60()
{
    llobj::ext::f140329c70(this);
    at<std::uint64_t>(this, 8) = 0;
    at<std::uint64_t>(this, 0x10) = 0;
}

// @src 140197a50  -> 140197b00
// @src 140197b00  int +0 = 0; +8, +0x10, +0x18 = 0
Cls_140197a50::Cls_140197a50()
{
    at<std::int32_t>(this, 0) = 0;
    at<std::uint64_t>(this, 8) = 0;
    at<std::uint64_t>(this, 0x10) = 0;
    at<std::uint64_t>(this, 0x18) = 0;
}

// @src 1401860a0  DYNARYs at 0xc0 and 0x140, the WPAXIS list (140259830) at 0x200, the OPSTOP
// list (14025ae30) at 0x218, the RSEDEF DYNARY at 0x238, owner 0x2b8 = 0
// @src 140259830  ctor of the list at 0x200 (-> 140259860)
Cls_1401860a0::Cls_1401860a0()
{
    new (s0c0) DYNARY();
    new (s140) DYNARY();
    llobj_ctor::list3_reset_b(s200);
    llobj_ctor::ext::opstop_list_construct(s218);
    new (s238) DYNARY();
    at<std::uint64_t>(this, 0x2b8) = 0;
}

// @src 1401889a0  owner +0x120 = 0, then the defaults 1401889f0
Cls_1401889a0::Cls_1401889a0()
{
    at<std::uint64_t>(this, 0x120) = 0;
    llobj_ctor::geo23_defaults(bytes_of(this));
}

// @src 14018b220  the 16 DYNARYs of the movement sub-object, owner 0xa80 = 0
Cls_14018b220::Cls_14018b220()
{
    for (std::uint32_t off : kTables) new (raw + off) DYNARY();
    at<std::uint64_t>(this, 0xa80) = 0;
}

// @src 140189440  three window shapes (140269250 with owner 0) at 0x40, 0xf8, 0x128; owner +0 = 0,
// +0xe0 = +0xe8 = 0
Cls_140189440::Cls_140189440()
{
    llobj_ctor::shape_construct(bytes_of(this) + 0x40, nullptr);
    llobj_ctor::shape_construct(bytes_of(this) + 0xf8, nullptr);
    llobj_ctor::shape_construct(bytes_of(this) + 0x128, nullptr);
    at<std::uint64_t>(this, 0) = 0;
    at<std::uint64_t>(this, 0xe0) = 0;
    at<std::uint64_t>(this, 0xe8) = 0;
}

// @src 1401956d0  the DYNARY list (1401364a0, as the first member), two lists (140257bf0) at 0x18
// and 0x30, seven window shapes (140269250, owner 0) at 0x48..0x168, owner 0x198 = 0
// @src 140257bf0  ctor of the 0x18-byte list (-> 140257c20)
Cls_1401956d0::Cls_1401956d0()
{
    llobj_ctor::list3_reset(s018);
    llobj_ctor::list3_reset(s030);
    for (int i = 0; i < 7; ++i) llobj_ctor::shape_construct(s048[i], nullptr);
    at<std::uint64_t>(this, 0x198) = 0;
}

} // namespace def::model
