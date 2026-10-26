// p8-01: object-field accessors and the member bodies of the Uic2CObject family (gfread.hpp).
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/gfread.hpp>

#include <cstdlib>
#include <cstring>

#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/sdctl_g_list.hpp>
#include <defmodel/sdctl/sdctl_node.hpp>

#include <new>

#if __has_include(<defcmd/uic2c_object.hpp>)
#include <defcmd/uic2c_object.hpp>
#define DEF_P801_HAVE_UIC2C_CLASS 1
#endif
#if __has_include(<defcmd/iges.hpp>)
#include <defcmd/iges.hpp>
#define DEF_P801_HAVE_IGES_CLASS 1
#endif
#if __has_include(<defcmd/dxf.hpp>)
#include <defcmd/dxf.hpp>
#define DEF_P801_HAVE_DXF_CLASS 1
#endif

namespace def::cmd::gfread {

namespace {

template <class T>
T& at(void* p, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<unsigned char*>(p) + off);
}
template <class T>
const T& at(const void* p, std::size_t off)
{
    return *reinterpret_cast<const T*>(static_cast<const unsigned char*>(p) + off);
}

} // namespace

// @src 14019a650  obj+0xbc8 = n
void fun_14019a650(LLOBJ_DATA* obj, std::int32_t n) { at<std::int32_t>(obj, 0xbc8) = n; }

// @src 140256670  obj+0x3a40 = type
void fun_140256670(LLOBJ_DATA* obj, std::int32_t type) { at<std::int32_t>(obj, 0x3a40) = type; }

// @src 140256690  obj+0x3a40
std::int32_t fun_140256690(const LLOBJ_DATA* obj) { return at<std::int32_t>(obj, 0x3a40); }

// @src 1404098d0  obj+0x3a48 = v
void fun_1404098d0(LLOBJ_DATA* obj, double v) { at<double>(obj, 0x3a48) = v; }

// @src 140409910  obj+0x3a50
std::int32_t fun_140409910(const LLOBJ_DATA* obj) { return at<std::int32_t>(obj, 0x3a50); }

// @src 1404098f0  -> 140409910
std::int32_t fun_1404098f0(const LLOBJ_DATA* obj) { return fun_140409910(obj); }

// @src 140409930  obj+0x3a58
double fun_140409930(const LLOBJ_DATA* obj) { return at<double>(obj, 0x3a58); }

// @src 14040cde0  curve+0x18 = data (8 bytes)
void fun_14040cde0(model::OBJ_GEO2D* curve, double* data) { at<double*>(curve, 0x18) = data; }

// @src 140331790  SdctlGList append: node = new 0x18 {slot3(item), prev = tail, next = 0}; link it
//                 after the tail (or as head), current = tail = node, cur_index = n, ++n
// @src 140333340  node ctor: node->item = item
static void sdctl_glist_append(model::SdctlGList* l, void* item)
{
    auto* node = static_cast<model::SdctlGListNode*>(std::malloc(sizeof(model::SdctlGListNode)));
    // 1403317b5: a failed allocation leaves local_18 = 0 and the stores below fault; the port
    // treats that as out of memory too.
    if (node == nullptr) std::abort();
    node->item = l->vslot3_140333f50(item);  // vtable + 0x18
    node->next = nullptr;
    node->u08 = l->tail;
    if (node->u08 == nullptr) {
        l->head = node;
    } else {
        static_cast<model::SdctlGListNode*>(l->tail)->next = node;
    }
    l->u20 = node;
    l->tail = node;
    l->cur_index = l->n;
    l->n = l->n + 1;
}

// @src 14019ab80  -> 140331790
void fun_14019ab80(model::SdctlGList* list, void* item) { sdctl_glist_append(list, item); }

// ---- Uic2CObject family ------------------------------------------------------------------------

// 140454a90 (owned by p8-03, C++ member; this is the raw-layout fallback)  set the name buffer at +0x10
void fun_140454a90(Uic2CObject* self, const char* name)
{
    char*& buf = at<char*>(self, 0x10);
    if (name == nullptr) {
        *buf = '\0';
        return;
    }
    const int len = static_cast<int>(std::strlen(name));
    if (len < 1) {
        *buf = '\0';
        return;
    }
    if (0x7f < len) {
        std::free(buf);
        buf = static_cast<char*>(std::malloc(static_cast<std::size_t>(len + 1)));  // 1404797d8
    }
    std::strcpy(buf, name);  // 140484f90
}

// With p8-03's C++ classes present (defcmd/uic2c_object.hpp, defcmd/iges.hpp) the member bodies
// below call them, so that an object is never initialised or freed twice (coordinator decision):
// the base ctor/dtor bodies only run inside a derived C++ ctor/dtor (the base is abstract), so
// 140454900 sets the name and 140454980 leaves the freeing to the C++ dtor; the IGES ctor/dtor
// bodies construct / destroy the C++ object on the caller's storage. The raw-layout code is the
// fallback when the classes are absent.

// 140454900 (owned by p8-03, C++ member; this is the raw-layout fallback)  Uic2CObject ctor body (the vptr store is the C++ ctor's)
void fun_140454900(Uic2CObject* self, const char* name)
{
#ifdef DEF_P801_HAVE_UIC2C_CLASS
    self->set_name(name);
#else
    at<std::int32_t>(self, 0x08) = 0;
    at<char*>(self, 0x10) = nullptr;
    at<char*>(self, 0x10) = static_cast<char*>(std::malloc(0x80));  // 1404797d8
    *at<char*>(self, 0x10) = '\0';
    fun_140454a90(self, name);
#endif
}

// 140454980 (owned by p8-03, C++ member; this is the raw-layout fallback)  Uic2CObject dtor body
void fun_140454980(Uic2CObject* self)
{
#ifdef DEF_P801_HAVE_UIC2C_CLASS
    (void)self;  // the C++ dtor ~Uic2CObject frees the name buffer
#else
    std::free(at<char*>(self, 0x10));
#endif
}

// 140409a50 (owned by p8-03, C++ member; this is the raw-layout fallback)  +0x18 = 0, +0x1c = 0, +0x20 = 0
void fun_140409a50(Uic2CObjectIgesFile* self)
{
#ifdef DEF_P801_HAVE_IGES_CLASS
    self->reset();
#else
    at<std::int32_t>(self, 0x18) = 0;
    at<std::int32_t>(self, 0x1c) = 0;
    at<double>(self, 0x20) = 0.0;
#endif
}

// 1404099d0 (owned by p8-03, C++ member; this is the raw-layout fallback)  Uic2CObjectIgesFile ctor body (on raw storage of 0x28 bytes)
void fun_1404099d0(Uic2CObjectIgesFile* self, const char* name)
{
#ifdef DEF_P801_HAVE_IGES_CLASS
    new (static_cast<void*>(self)) iges::Uic2CObjectIgesFile(name);
#else
    fun_140454900(reinterpret_cast<Uic2CObject*>(self), name);
    at<std::int32_t>(self, 0x18) = 0;
    at<std::int32_t>(self, 0x1c) = 0;
    at<double>(self, 0x20) = 0.0;
    fun_140409a50(self);
#endif
}

// 140409a90 (owned by p8-03, C++ member; this is the raw-layout fallback)  Uic2CObjectIgesFile dtor body
void fun_140409a90(Uic2CObjectIgesFile* self)
{
#ifdef DEF_P801_HAVE_IGES_CLASS
    self->~Uic2CObjectIgesFile();
#else
    fun_140454980(reinterpret_cast<Uic2CObject*>(self));
#endif
}

// @src 140409330  +0x18 = 0, +0x1c = 0, +0x20 = 0
void fun_140409330(Uic2CObjectDxfFile* self)
{
    at<std::int32_t>(self, 0x18) = 0;
    at<std::int32_t>(self, 0x1c) = 0;
    at<double>(self, 0x20) = 0.0;
}

// @src 1404092b0  Uic2CObjectDxfFile ctor body (on raw storage of 0x28 bytes). With p8-04's class
// present: its C++ ctor (which runs 140409330); never on top of a constructed object.
void fun_1404092b0(Uic2CObjectDxfFile* self, const char* name)
{
#ifdef DEF_P801_HAVE_DXF_CLASS
    new (static_cast<void*>(self)) Uic2CObjectDxfFile(name);
#else
    fun_140454900(reinterpret_cast<Uic2CObject*>(self), name);
    at<std::int32_t>(self, 0x18) = 0;
    at<std::int32_t>(self, 0x1c) = 0;
    at<double>(self, 0x20) = 0.0;
    fun_140409330(self);
#endif
}

// @src 140409370  Uic2CObjectDxfFile dtor body
void fun_140409370(Uic2CObjectDxfFile* self)
{
#ifdef DEF_P801_HAVE_DXF_CLASS
    self->~Uic2CObjectDxfFile();
#else
    fun_140454980(reinterpret_cast<Uic2CObject*>(self));
#endif
}

} // namespace def::cmd::gfread


namespace def::cmd::gfread {

// @src 1402566b0  +0x1c
std::int32_t fun_1402566b0(const Uic2CObjectIgesFile* self) { return at<std::int32_t>(self, 0x1c); }

// @src 1402566c0  +0x1c
std::int32_t fun_1402566c0(const Uic2CObjectDxfFile* self) { return at<std::int32_t>(self, 0x1c); }

// @src 14040ce20  +0x18 = v
void fun_14040ce20(Uic2CObjectIgesFile* self, std::int32_t v) { at<std::int32_t>(self, 0x18) = v; }

// @src 14040ce40  -> 14040ce20(self, 1)
void fun_14040ce40(Uic2CObjectIgesFile* self) { fun_14040ce20(self, 1); }

// @src 14040ce60  +0x1c = v
void fun_14040ce60(Uic2CObjectIgesFile* self, std::int32_t v) { at<std::int32_t>(self, 0x1c) = v; }

} // namespace def::cmd::gfread

#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/obj_geo2d.hpp>

namespace def::cmd::gfread {

// @src 14019a620  -> OBJ_GEO2D::assign 140276140 (dst, src). No packet lists it (ledger P6,
//                 owner none); ported here with its caller.
static void copy_curve(model::OBJ_GEO2D* dst, model::OBJ_GEO2D* src) { dst->assign(src); }

// @src 140197bc0  undo copy of the 2D curves into obj+0x3a60 (DXF_READER.md section on the
//                 undo snapshot). Arguments 2..4 of the original are only forwarded to
//                 1401982c0, which ignores them.
std::int64_t fun_140197bc0(LLOBJ_DATA* obj)
{
    obj->u3a60.clear();  // vtable + 8
    std::int32_t n = model::geo_kw::curve_count(*obj);
    if (0 < n) {
        std::int32_t i = 0;
        while (i < model::geo_kw::curve_count(*obj)) {
            model::OBJ_GEO2D* src = model::geo_kw::curve(*obj, i);
            // 140197c3d: operator new(0x30) (1404797d8) and ctor 140275cd0; null -> 0 is passed on
            auto* copy = new model::OBJ_GEO2D();
            copy_curve(copy, src);
            fun_14019ab80(&obj->u3a60, copy);
            i = i + 1;
        }
        (void)obj->u3a60.count();  // 140197cc5: vtable slot 0, result unused
    }
    return 0;
}

} // namespace def::cmd::gfread
