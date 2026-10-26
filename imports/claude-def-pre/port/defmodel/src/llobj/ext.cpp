// Default bodies of the calls into other packets (defmodel/llobj/ext.hpp).
#include <defmodel/llobj/ext.hpp>

#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/cls_140269250.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/sdctl/cow.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/layout/op_stop_linked_list.hpp>
#include <defmodel/layout/sdctl_ptr_list_obj_geo2d.hpp>
#include <defmodel/layout/sf_linked_list.hpp>
#include <defmodel/layout/state_var_elem.hpp>
#include <defmodel/layout/state_var_elem_currnt.hpp>
#include <defmodel/layout/state_var_elem_damag.hpp>
#include <defmodel/layout/state_var_elem_mataxi.hpp>
#include <defmodel/layout/state_var_elem_sts.hpp>
#include <defmodel/layout/state_var_elem_teps.hpp>
#include <defmodel/layout/state_var_node.hpp>
#include <defmodel/layout/state_var_node_post.hpp>
#include <defmodel/linked/axis_point_list.hpp>
#include <defmodel/linked/record_e8_list.hpp>
#include <defmodel/members/statevar_collection.hpp>
#include <defmodel/mesh2d/geo2d_mesh_ops.hpp>
#include <defmodel/port.hpp>

#include <cstddef>
#include <list>
#include <new>

namespace def::model::llobj::ext {

namespace {

template <class T>
T* as(std::uint64_t p)
{
    return reinterpret_cast<T*>(static_cast<std::uintptr_t>(p));
}

std::uint64_t ret(const void* p) { return reinterpret_cast<std::uintptr_t>(p); }

// The node walk of the +0x3998 list relies on the MSVC std::list layout {_Myhead, _Mysize}.
static_assert(sizeof(std::list<int>) == 0x10);

// The std::list helpers of the object at LLOBJ_DATA+0x3998 (geo_kw::GeoList = p1-05's
// CowList<GEO2D_AND_MESH2D>, body {refs, MSVC std::list}). An iterator is the node pointer
// (node: +0 next, +8 prev, +0x10 value). Disassembly checked (dumpbin) for each.

// @src 140302090  refs > 1 -> 140302210 (release, clone the body): p1-05 CowList::detach
void list_detach(std::uint64_t list) { as<geo_kw::GeoList>(list)->detach(); }

// @src 1403020c0  iterator ctor: *out = node, returns out
std::uint64_t iter_ctor(std::uint64_t out, std::uint64_t node)
{
    *as<std::uint64_t>(out) = node;
    return out;
}

// @src 140301e30  end(): 140302090, then 1403020c0(out, body->_Myhead)
std::uint64_t list_end(std::uint64_t list, std::uint64_t out)
{
    list_detach(list);
    const std::byte* body = *as<const std::byte* const>(list);
    return iter_ctor(out, *reinterpret_cast<const std::uint64_t*>(body + 8));
}

// @src 140302020  &node->value
std::uint64_t iter_value(std::uint64_t it) { return *as<std::uint64_t>(it) + 0x10; }

// @src 140302040  it++ (post-increment): 140301fc0 copies *it into a temporary, *it = (*it)->next,
//                 140301fc0 copies the temporary into *out; returns out (the int argument unused)
// @src 140301fc0  iterator copy ctor: *this = *other
std::uint64_t iter_post_inc(std::uint64_t it, std::uint64_t out)
{
    const std::uint64_t old = *as<std::uint64_t>(it);
    *as<std::uint64_t>(it) = *as<std::uint64_t>(old);
    *as<std::uint64_t>(out) = old;
    return out;
}

// operator new(size) + the StateVar subclass ctor named by its address (the registration tables
// of 14017f2f0 and 140182b80); `size` is the original's allocation size (0x38 or 0x40).
StateVarBase* new_statevar_by_ctor(std::uint64_t ctor, std::int32_t id, void* field, void* owner)
{
    switch (ctor) {
    case 0x14032a990: return new StateVarNode(id, field);
    case 0x14032aa20: return new StateVarNodePost(id, field);
    case 0x14032c8c0: return new StateVarElem(id, field);
    case 0x14032c950: return new StateVarElemSTS(id, field, owner);
    case 0x14032ca80: return new StateVarElemTEPS(id, field, owner);
    case 0x14032cbb0: return new StateVarElemDAMAG(id, field, owner);
    case 0x14032ccd0: return new StateVarElemMATAXI(id, field);
    case 0x14032ceb0: return new StateVarElemCURRNT(id, field, owner);
    default: DEF_NOT_PORTED(ctor);
    }
}

} // namespace

// Without a test hook, each address is the owner's function (wired at the Phase 1 gate). The
// object at LLOBJ_DATA+0x3998 is kw-g2geo's geometry list (geo_kw::GeoList); 14019a670 / 140301ad0 /
// 140301d10 are its delete, append-default and clear (defmodel/geo_kw/geo_kw.hpp).
std::uint64_t call(std::uint64_t addr, std::uint64_t a0, std::uint64_t a1, std::uint64_t a2, std::uint64_t a3)
{
    if (g_hook) {
        const std::uint64_t args[4] = {a0, a1, a2, a3};
        return g_hook(addr, args, 4);
    }
    switch (addr) {
    case 0x14012f480: dynary_free_arrays(as<DYNARY>(a0)); return 0;
    // LLOBJ_DATA+0x3998: CowList<GEO2D_AND_MESH2D>
    case 0x14019a670: geo_kw::delete_geo_list(as<geo_kw::GeoList>(a0)); return 0;  // flags 1: delete
    case 0x140301d10: geo_kw::geo_list_clear(as<geo_kw::GeoList>(a0)); return 0;
    case 0x140301ad0: return ret(geo_kw::geo_list_append_default(as<geo_kw::GeoList>(a0)));
    case 0x140257c60: as<linked::Cls_140257340List>(a0)->~Cls_140257340List(); return 0;
    case 0x1402581b0: as<linked::Cls_140257340List>(a0)->clear(); return 0;
    case 0x1402598a0: as<linked::Cls_140258580List>(a0)->~Cls_140258580List(); return 0;
    case 0x140259c70: as<linked::Cls_140258580List>(a0)->clear(); return 0;
    case 0x14025ab20: as<SfLinkedList>(a0)->clear(); return 0;
    case 0x14025ae70: as<OpStopLinkedList>(a0)->~OpStopLinkedList(); return 0;
    case 0x140269320: as<Cls_140269250>(a0)->~Cls_140269250(); return 0;
    case 0x1402699c0: as<Cls_140269250>(a0)->clear(); return 0;
    case 0x1402f91f0: as<OBJ_GEO3D>(a0)->~OBJ_GEO3D(); return 0;
    case 0x1402f9340: as<OBJ_GEO3D>(a0)->clear(); return 0;
    case 0x140283f80: return ret(geo2d_face_at(as<GEO2D_AND_MESH2D>(a0), as<const std::int32_t>(a1)));
    // kw-g2obj's LLOBJ_DATA copy (1401613a0, llobj_copy.hpp): p1-07 _OBJ_GEO3D, p1-06 point list,
    // GEO2D_AND_MESH2D copy and the std::list iterator helpers of the list at LLOBJ_DATA+0x3998
    case 0x1400034d0: return static_cast<std::uint32_t>(as<OBJ_GEO3D>(a0)->node_count());
    case 0x140003500: return static_cast<std::uint32_t>(as<OBJ_GEO3D>(a0)->elem_count());
    case 0x1402f9b60: as<OBJ_GEO3D>(a0)->assign(as<OBJ_GEO3D>(a1)); return 0;
    case 0x140257c80: as<linked::Cls_140257340List>(a0)->copy_from(*as<const linked::Cls_140257340List>(a1)); return 0;
    case 0x140280490: as<GEO2D_AND_MESH2D>(a0)->copy_from(*as<GEO2D_AND_MESH2D>(a1)); return 0;
    // the +0x3998 list helpers (see list_detach above)
    case 0x140302090: list_detach(a0); return 0;
    case 0x140301e30: return list_end(a0, a1);
    case 0x140302020: return iter_value(a0);
    case 0x140302040: return iter_post_inc(a0, a1);
    case 0x1403020c0: return iter_ctor(a0, a1);
    // p1-10 Cls_140329c70, the base of the variable sets at offset 0
    case 0x140329c70: ::new (as<void>(a0)) Cls_140329c70(); return 0;
    case 0x140329cb0: as<Cls_140329c70>(a0)->~Cls_140329c70(); return 0;
    case 0x140329d10: as<Cls_140329c70>(a0)->delete_all(); return 0;
    case 0x140329dc0: as<Cls_140329c70>(a0)->reset_all(); return 0;
    case 0x140329e30: as<Cls_140329c70>(a0)->add(as<StateVarBase>(a1)); return 0;
    case 0x140329e60:
        as<Cls_140329c70>(a0)->set_all(static_cast<std::int32_t>(a1), as<const std::int32_t>(a2),
                                       static_cast<std::int32_t>(a3));
        return 0;
    case 0x140329ef0: {
        // the key set is passed by value: the callee owns (and destroys) its own copy
        alignas(CowList<std::int32_t>) unsigned char copy[sizeof(CowList<std::int32_t>)];
        ::new (copy) CowList<std::int32_t>(*as<CowList<std::int32_t>>(a2));
        return as<Cls_140329c70>(a0)->copy_from(as<Cls_140329c70>(a1), copy) ? 1 : 0;
    }
    default:
        break;
    }
    if (addr >= 0x14032a990 && addr <= 0x14032ceb0) {
        // new_statevar: a0 = allocation size, a1 = id, a2 = field, a3 = owner
        return ret(new_statevar_by_ctor(addr, static_cast<std::int32_t>(a1), as<void>(a2), as<void>(a3)));
    }
    DEF_NOT_PORTED(addr);
}

void dynary_dtor(DYNARY* p)
{
    if (g_hook) {
        call(0x14012f410, u(p));
        return;
    }
    p->~DYNARY();
}

DYNARY* new_dynary()
{
    if (g_hook)
        return reinterpret_cast<DYNARY*>(call(0x14012d050, 0x78));
    return new DYNARY();
}

// @src 14019a290  LLOBJ_DATA scalar deleting dtor (dtor 14015f790, then free) = delete
void llobj_delete(LLOBJ_DATA* o)
{
    if (g_hook) {
        call(0x14019a290, u(o), 1);
        return;
    }
    delete o;
}

void sdctl_clear(SdctlPtrList_OBJ_GEO2D* list)
{
    if (g_hook) {
        call(0x14019abb0, u(list));
        return;
    }
    list->clear();
}

// MSVC x64: a virtual member function is called like a free function with this first; slot 0 of
// these classes is the scalar deleting dtor taking the flags.
void vdelete(void* obj)
{
    if (g_hook) {
        call(0x1, u(obj), 0, 1);  // pseudo address 0x1: virtual slot 0
        return;
    }
    using Fn = void* (*)(void*, unsigned);
    (*reinterpret_cast<Fn**>(obj))[0](obj, 1);
}

void vslot1_0(void* obj)
{
    if (g_hook) {
        call(0x2, u(obj), 1, 0);  // pseudo address 0x2: virtual slot 1
        return;
    }
    using Fn = void (*)(void*, int);
    (*reinterpret_cast<Fn**>(obj))[1](obj, 0);
}

} // namespace def::model::llobj::ext
