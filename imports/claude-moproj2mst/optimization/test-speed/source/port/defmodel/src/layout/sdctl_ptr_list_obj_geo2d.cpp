// SdctlPtrList<_OBJ_GEO2D>: list of 2D geometry objects, LLOBJ_DATA + 0x3a60 (p1-05).
// Layout: defmodel/layout/sdctl_ptr_list_obj_geo2d.hpp, docs/re/model-layout.md.
#include <defmodel/layout/sdctl_ptr_list_obj_geo2d.hpp>

#include <defmodel/port.hpp>

// The item type is p1-07's class; the deleting call needs its complete type. Until its layout
// header exists, deleting a non-null item when owns is set stops with DEF_NOT_PORTED.
#if __has_include(<defmodel/layout/obj_geo2d.hpp>)
#include <defmodel/layout/obj_geo2d.hpp>
#define DEF_SDCTL_HAVE_OBJ_GEO2D 1
#endif

namespace def::model {

// @src 14019aae0  ctor: SdctlGList ctor 140330ee0, then vptr SdctlPtrList<_OBJ_GEO2D>
SdctlPtrList_OBJ_GEO2D::SdctlPtrList_OBJ_GEO2D() = default;

// @src 14019ab50  slot 0: -> 14019ab70 (count)
std::int32_t SdctlPtrList_OBJ_GEO2D::count() const { return SdctlGList::count(); }

// @src 14019abb0  slot 1: -> 1403321a0 (clear)
void SdctlPtrList_OBJ_GEO2D::clear() { SdctlGList::clear(); }

// @src 14019ab10  dtor: vptr SdctlPtrList<_OBJ_GEO2D>, 14019abb0 (clear, items deleted through
//                 this class's slot 4), SdctlGList dtor 140331030
// @src 14019ad70  slot 2: scalar deleting dtor (dtor, then free when flag & 1)
SdctlPtrList_OBJ_GEO2D::~SdctlPtrList_OBJ_GEO2D() { SdctlPtrList_OBJ_GEO2D::clear(); }

// @src 14019b980  slot 4: if owns != 0 and item != 0, the item's virtual scalar deleting dtor
//                 (vtable slot 0) with flag 1
void SdctlPtrList_OBJ_GEO2D::delete_item(void* item)
{
    if (owns != 0 && item != nullptr) {
#if defined(DEF_SDCTL_HAVE_OBJ_GEO2D)
        delete static_cast<OBJ_GEO2D*>(item);
#else
        DEF_NOT_PORTED(0x14019b980);
#endif
    }
}

} // namespace def::model
