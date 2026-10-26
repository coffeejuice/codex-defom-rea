// LLOBJ_DATA: dtor, virtual slots and the reset/cleanup methods (layout/llobj_data.hpp).
// The ctor 14015ec80 belongs to Phase 2 (ledger) and stays a stub in src/layout/llobj_data.cpp.
#include <defmodel/layout/llobj_data.hpp>

#include <cstdlib>
#include <cstring>

#include <defmodel/llobj/cls_14017e740.hpp>
#include <defmodel/llobj/cls_14017ef20.hpp>
#include <defmodel/llobj/cls_140182880.hpp>
#include <defmodel/llobj/cls_140185570.hpp>
#include <defmodel/llobj/cls_140185c60.hpp>
#include <defmodel/llobj/cls_1401860a0.hpp>
#include <defmodel/llobj/cls_1401889a0.hpp>
#include <defmodel/llobj/cls_140189440.hpp>
#include <defmodel/llobj/cls_14018b220.hpp>
#include <defmodel/llobj/cls_1401956d0.hpp>
#include <defmodel/llobj/cls_140197a50.hpp>
#include <defmodel/llobj/ext.hpp>
#include <defmodel/llobj/llobj_parts.hpp>
#include <defmodel/sdctl/cow.hpp>

namespace def::model {

namespace ext = llobj::ext;
using namespace llobj::parts;

// @src 14015f790  dtor (non-virtual). Order: reset(full), delete u3998, then the members in
// reverse order. The typed members u3a60 (14019ab10), u3598 (14012f410), uba8 (14025ba10) and
// ub70 (140300d50) are destroyed by C++ after this body, i.e. after the 0x10 sub-object instead of
// between the others; none of them writes output.
LLOBJ_DATA::~LLOBJ_DATA()
{
    const char full = 1;
    reset(&full);
    delete_u3998();
    // 0x3950..0x3978: COW vector handles (p1-05 templates; element types do not matter to the dtor)
    reinterpret_cast<CowVector<void*>*>(&u3978)->~CowVector();         // 14019aa90
    reinterpret_cast<CowVector<void*>*>(&u3970)->~CowVector();         // 14019a9d0
    reinterpret_cast<CowVector<std::int32_t>*>(&u3968)->~CowVector();  // 140159b40
    reinterpret_cast<CowVector<void*>*>(&u3960)->~CowVector();         // 14019a910
    reinterpret_cast<CowVector<void*>*>(&u3958)->~CowVector();         // 14019a910
    reinterpret_cast<CowVector<void*>*>(&u3950)->~CowVector();         // 14019a910
    part_3920(*this).~Cls_140197a50();                                  // 140197a80
    ext::f1402f91f0(s3458);
    ext::f1402f91f0(s3328);
    part_2e68(*this).~Cls_1401956d0();                                  // 1401958b0
    part_2d10(*this).~Cls_140189440();                                  // 140189520
    part_2be8(*this).~Cls_1401889a0();                                  // 1401889e0
    part_2160(*this).~Cls_14018b220();                                  // 14018b3b0
    part_1ea0(*this).~Cls_1401860a0();                                  // 140186130
    for (int i = 99; i >= 0; --i)                                      // eh vector dtor iterator
        part_be0(*this, i).~Cls_14017e740();                            // 14017e720
    part_b58(*this).~Cls_140185c60();                                   // 140185ca0
    part_ae8(*this).~Cls_140185570();                                   // 1401855b0
    part_9e0(*this).~Cls_140182880();                                   // 1401828e0
    part_8a0(*this).~Cls_14017ef20();                                   // 14017ef80
    // @src 1401999c0  dtor of the sub-object at 0x10 (Cls_140199970): empty
}

// @src 14015fa60  slot 0: strcpy(out, name)
void LLOBJ_DATA::get_name(char* out) const { std::strcpy(out, name); }

// @src 14015faa0  slot 1: strcpy(name, in)
void LLOBJ_DATA::set_name(const char* in) { std::strcpy(name, in); }

// @src 14015fbd0  slot 2: return u3c00[0]
void* LLOBJ_DATA::vslot2() { return u3c00[0]; }

// @src 14015fbf0  clear the object's data
void LLOBJ_DATA::reset(const char* full)
{
    part_8a0(*this).free_arrays();  // 14017f130
    part_9e0(*this).free_arrays();  // 140182a50
    part_ae8(*this).free_arrays();  // 1401856e0
    for (int i = 0; i < 100; i = i + 1) {
        if (u3c00[i] != nullptr && (ext::f1402f9340(u3c00[i]), *full != '\0')) {
            void* g = u3c00[i];
            if (g != nullptr)
                ext::vdelete(g);  // virtual slot 0 (scalar deleting dtor), flags 1
            u3c00[i] = nullptr;
        }
        part_be0(*this, i).free_arrays();          // 14017e800
        part_be0(*this, i).set_owner(get_owner());  // 140158f50, 14017e770
    }
    ext::f1402f9340(s3328);
    ext::f1402f9340(s3458);
    part_1ea0(*this).reset();       // 140186510
    part_2160(*this).reset();       // 14018bc30
    part_2d10(*this).reset();       // 140189ad0
    part_2e68(*this).reset();       // 140195a40
    part_3920(*this).free_arrays(); // 140197b40
    if (u3998 != nullptr) {
        ext::f140301d10(u3998);
        ext::f140301ad0(u3998);
    }
    free_u39c8();
    reset_u39d8();
    ext::sdctl_clear(&u3a60);       // virtual slot 1 (14019abb0)
    free_u39a0();
    if (u3bf8 != 0) {
        ext::vslot1_0(reinterpret_cast<void*>(u3bf8));  // virtual slot 1, argument 0
        u3bf8 = 0;
    }
    u010 = 0;
    u014 = 0;
    u818 = 0;
    u81c = 0;
    u820 = 0;
    std::strcpy(u018, "");  // str 14051f7d6
    std::strcpy(u418, "");  // str 14051f7d7
    u3990 = 0;
    u87c[0] = 0;
    u87c[1] = 0;
    u87c[2] = 0;
    u87c[3] = 0;
    u87c[4] = 0;
}

// @src 140199b10  free u39c8, then 1401999d0
void LLOBJ_DATA::free_u39c8()
{
    if (u39c8 != 0)
        std::free(reinterpret_cast<void*>(u39c8));
    clear_u39c8();
}

// @src 1401999d0  u39c8 = 0, u39d0 = 0
void LLOBJ_DATA::clear_u39c8()
{
    u39c8 = 0;
    u39d0 = 0;
}

// @src 140199b60  free u39d8; u39d8 = 0, u39d4 = u39e0..u39ec = 0, u39f0 = 1
void LLOBJ_DATA::reset_u39d8()
{
    if (u39d8 != 0)
        std::free(reinterpret_cast<void*>(u39d8));
    u39d8 = 0;
    u39d4 = 0;
    u39e0 = 0;
    u39e4 = 0;
    u39e8 = 0;
    u39ec = 0;
    u39f0 = 1;
}

// @src 140162070  free u39a0
void LLOBJ_DATA::free_u39a0()
{
    if (u39a0 != 0) {
        std::free(reinterpret_cast<void*>(u39a0));
        u39a0 = 0;
    }
}

// @src 140197ec0  delete the object at u3998 through 14019a670(p, 1)
void LLOBJ_DATA::delete_u3998()
{
    if (u3998 != nullptr && u3998 != nullptr)
        ext::f14019a670(u3998, 1);
    u3998 = nullptr;
}

// @src 140158f50  return *(this + 15000)
DB_STEP_DATA* LLOBJ_DATA::get_owner() const { return owner; }

// @src 140199a70  IObjectData ctor: stores the vptr only (ledger Phase 2; trivial)
IObjectData::IObjectData() = default;

} // namespace def::model
