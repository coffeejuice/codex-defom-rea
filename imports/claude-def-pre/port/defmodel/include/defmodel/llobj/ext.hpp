// defmodel/llobj/ext.hpp: calls from p1-04 code into functions other packets own.
//
// Each wrapper names the binary function it stands for and the packet that ports it. Until that
// packet lands, a wrapper either forwards to the owner's declared C++ (layout headers) or ends in
// DEF_NOT_PORTED. Integration replaces a wrapper's body with the owner's call; the call sites in
// p1-04 code do not change.
//
// Calls whose target is a virtual slot of an unidentified class use pseudo addresses in the hook:
// 0x1 = slot 0 with flags 1 (vdelete), 0x2 = slot 1 with argument 0 (vslot1_0).
//
// Tier-1 tests install g_hook to observe the calls (address, arguments) and to supply return
// values, so the p1-04 logic can be checked against the decompile without the other packets.
#pragma once

#include <cstdint>

#include <defmodel/sdctl/cow.hpp>

namespace def::model {
class DYNARY;
class LLOBJ_DATA;
class SdctlPtrList_OBJ_GEO2D;
} // namespace def::model

namespace def::model::llobj::ext {

// Test hook: called for every wrapper below instead of its default body. args[0] is usually the
// object (this) pointer; returns the function's result (0 for void functions).
using Hook = std::uint64_t (*)(std::uint64_t addr, const std::uint64_t* args, int nargs);
inline Hook g_hook = nullptr;

// Dispatch: g_hook when set, otherwise the owner's port through the switch in src/llobj/ext.cpp
// (an address with no case there ends in DEF_NOT_PORTED(addr); docs/re/status/hook_audit.md).
std::uint64_t call(std::uint64_t addr, std::uint64_t a0 = 0, std::uint64_t a1 = 0, std::uint64_t a2 = 0,
                   std::uint64_t a3 = 0);

inline std::uint64_t u(const void* p) { return reinterpret_cast<std::uintptr_t>(p); }

// ---- DYNARY (material threads; dynary.hpp) ----------------------------------------------------
void dynary_dtor(DYNARY* p);                  // 14012f410 (default: p->~DYNARY())
inline void f14012f480(DYNARY* p) { call(0x14012f480, u(p)); } // free the arrays, keep the object
// operator new(0x78) + DYNARY ctor 14012d050 (default: new DYNARY)
DYNARY* new_dynary();

// ---- p1-05 (Sdctl lists, COW templates in defmodel/sdctl/cow.hpp) -----------------------------
// 14019a290(o, 1): LLOBJ_DATA scalar deleting dtor, absorbed by p1-04 (default: delete o).
void llobj_delete(LLOBJ_DATA* o);
// 14019a670(p, 1): scalar deleting dtor of the object at LLOBJ_DATA+0x3998 (dtor 140301990, p1-09)
inline void f14019a670(void* obj, int flags) { call(0x14019a670, u(obj), static_cast<std::uint64_t>(flags)); }
// The COW handles (CowVector/CowList, 14019a910, 14019acb0, ...) are used directly from
// defmodel/sdctl/cow.hpp.

// ---- p1-06 (linked lists) ---------------------------------------------------------------------
inline void f140257c60(void* p) { call(0x140257c60, u(p)); }
inline void f1402581b0(void* p) { call(0x1402581b0, u(p)); }
inline void f1402598a0(void* p) { call(0x1402598a0, u(p)); }
inline void f140259c70(void* p) { call(0x140259c70, u(p)); }
inline void f14025ab20(void* p) { call(0x14025ab20, u(p)); }
inline void f14025ae70(void* p) { call(0x14025ae70, u(p)); }
inline void f140269320(void* p) { call(0x140269320, u(p)); }                // dtor of Cls_140269250
inline void f1402699c0(void* p) { call(0x1402699c0, u(p)); }                // reset of Cls_140269250

// ---- p1-07 (_OBJ_GEO3D) -----------------------------------------------------------------------
inline void f1402f91f0(void* geo3d) { call(0x1402f91f0, u(geo3d)); }        // dtor
inline void f1402f9340(void* geo3d) { call(0x1402f9340, u(geo3d)); }        // reset

// ---- p1-08 -------------------------------------------------------------------------------------
inline void* f140283f80(void* self, std::int32_t* idx)
{
    return reinterpret_cast<void*>(call(0x140283f80, u(self), u(idx)));
}

// ---- p1-09 (state variables) ------------------------------------------------------------------
inline void f140301ad0(void* sv) { call(0x140301ad0, u(sv)); }
inline void f140301d10(void* sv) { call(0x140301d10, u(sv)); }
// operator new(size) then the StateVar subclass ctor (id, field[, owner]); returns the object
// (0 when new fails, as the original's null check after operator new).
inline void* new_statevar(std::uint64_t ctor, std::uint64_t size, std::int32_t id, void* field,
                          void* owner = nullptr)
{
    return reinterpret_cast<void*>(
        call(ctor, size, static_cast<std::uint64_t>(static_cast<std::uint32_t>(id)), u(field), u(owner)));
}

// ---- p1-10 (base Cls_140329c70 of the variable sets: COW list of StateVar pointers at +0) -------
inline void f140329c70(void* self) { call(0x140329c70, u(self)); }          // base ctor
inline void f140329cb0(void* self) { call(0x140329cb0, u(self)); }          // base dtor
inline void f140329d10(void* self) { call(0x140329d10, u(self)); }          // delete all items
inline void f140329dc0(void* self) { call(0x140329dc0, u(self)); }          // reset each item
inline void f140329e30(void* self, void* item) { call(0x140329e30, u(self), u(item)); } // append
inline void f140329e60(void* self, std::int32_t n, const void* dims, std::int32_t key)   // slot 2 on each
{
    call(0x140329e60, u(self), static_cast<std::uint32_t>(n), u(dims), static_cast<std::uint32_t>(key));
}
// Copy the variables' values from other, skipping the ids in skip (by value: the callee
// releases its copy, 14019acf0 inside 140329ef0).
inline bool f140329ef0(void* self, const void* other, CowList<std::int32_t> skip)
{
    return call(0x140329ef0, u(self), u(other), u(&skip)) != 0;
}

// ---- virtual calls on objects whose class another packet owns ---------------------------------
// u3a60.clear(): virtual slot 1 of SdctlPtrList<_OBJ_GEO2D> (14019abb0, p1-05)
void sdctl_clear(SdctlPtrList_OBJ_GEO2D* list);
// (*obj->vtbl[0])(obj, 1): scalar deleting dtor (u3c00[i] geometry objects, p1-07).
void vdelete(void* obj);
// (*obj->vtbl[1])(obj, 0) on the object at LLOBJ_DATA+0x3bf8 (class not identified).
void vslot1_0(void* obj);

} // namespace def::model::llobj::ext
