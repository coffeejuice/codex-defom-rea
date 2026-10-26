// LLOBJ_DATA utilities the DEF_GUI_MO_64 loader calls as byte-identical twins (header:
// defmodel/gui_twins.hpp). Decompile: deform-rea-work/out/mdt3/f.c.
#include <defmodel/gui_twins.hpp>

#include <cstddef>
#include <cstring>

#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/llobj_ctor/llobj_copy.hpp>
#include <defmodel/llobj_ctor/obj_defaults.hpp>
#include <defmodel/step/db_step.hpp>

namespace def::model {

// @src 140161770  copy an object, keeping its number (GUI twin 0x140835800)
bool llobj_copy_keep_number(LLOBJ_DATA* dst, const LLOBJ_DATA* src)
{
    const std::int32_t no = field<std::int32_t>(dst, 8);
    DB_STEP_DATA* owner = dst->get_owner(); // 140158f50
    if (owner == nullptr) return false;
    // stack bytes of the call, in the original's order
    const bool p9 = true;  // local_1c
    const bool p8 = false; // local_1b: do not copy +8
    const bool p7 = true;  // local_1a: copy the name
    const bool p6 = true;  // local_19
    const bool p5 = true;  // local_18
    const bool p4 = true;  // local_17
    llobj_copy_from(dst, src, owner->u15f8, &p4, &p5, &p6, &p7, &p8, &p9); // 1401613a0
    field<std::int32_t>(dst, 8) = no;
    return true;
}

// @src 140160f50  kind 7 -> 0, then the unit defaults (GUI twin 0x140834fe0)
void llobj_reset_kind7(LLOBJ_DATA* obj, std::int32_t unit)
{
    if (!llobj_not_kind7(obj)) field<std::int32_t>(obj, 0xc) = 0;               // 140160f10
    llobj_ctor::object_unit_defaults(reinterpret_cast<std::byte*>(obj), unit); // 14015f9a0
}

namespace {
template <std::size_t N>
void cp(std::byte* d, const std::byte* s, std::size_t off)
{
    std::memcpy(d + off, s + off, N);
}
} // namespace

// @src 140188c90  Cls_1401889a0 assignment (GUI twin 0x14085ceb0); the stores in the binary's order
void cls_1401889a0_assign(void* dst, const void* src)
{
    std::byte* d = static_cast<std::byte*>(dst);
    const std::byte* s = static_cast<const std::byte*>(src);
    cp<4>(d, s, 0x00);
    cp<4>(d, s, 0x04);
    cp<8>(d, s, 0x08);
    cp<8>(d, s, 0x10);
    cp<8>(d, s, 0x18);
    cp<4>(d, s, 0x20);
    cp<8>(d, s, 0x28);
    cp<8>(d, s, 0x30);
    cp<1>(d, s, 0x39);
    cp<4>(d, s, 0x3c);
    cp<1>(d, s, 0x38);
    cp<4>(d, s, 0x40);
    cp<4>(d, s, 0x44);
    cp<8>(d, s, 0x58);
    cp<8>(d, s, 0x60);
    cp<8>(d, s, 0x68);
    cp<8>(d, s, 0x70);
    cp<8>(d, s, 0x78);
    cp<1>(d, s, 0x4c);
    cp<4>(d, s, 0x50);
    cp<8>(d, s, 0xa0);
    cp<8>(d, s, 0xa8);
    cp<8>(d, s, 0xb0);
    cp<8>(d, s, 0xb8);
    cp<8>(d, s, 0xc0);
    cp<8>(d, s, 0xc8);
    cp<8>(d, s, 0xf0);
    cp<8>(d, s, 0xf8);
    cp<8>(d, s, 0x100);
    cp<8>(d, s, 0x108);
    cp<8>(d, s, 0x110);
    cp<8>(d, s, 0x118);
}

// @src 1401981f0  copy the +0x3998 list object (GUI twin 0x14086c390)
void llobj_copy_u3998_list(LLOBJ_DATA* dst, const LLOBJ_DATA* src)
{
    llobj_copy::sv_list_copy(dst->u3998, src->u3998); // 1403019d0
}

} // namespace def::model
