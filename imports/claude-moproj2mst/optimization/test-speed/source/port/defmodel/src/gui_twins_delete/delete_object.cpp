// DEF_PRE "delete object" 14019cdc0 and its callees (defmodel/gui_twins_delete.hpp). Faithful to
// the decompile deform-rea-work/out/mdtdel/del.c and del2.c; the DEF_GUI_MO_64 twins are 0x1408bdf10,
// 0x1408cad60, 0x1408e6d70, 0x1408bdfc0 (docs/re/moproj2mst/mdt/L1/spec_L1B.md 7.6).
#include <defmodel/gui_twins_delete.hpp>

#include <cstdint>

#include <defmodel/layout/cls_14017a6c0.hpp>
#include <defmodel/layout/cls_1401f46e0.hpp>
#include <defmodel/layout/cls_1401fa020.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/ext.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defmodel/port.hpp>
#include <defmodel/step/db_step.hpp>

namespace def::model {

namespace {

template <class T> T& at(void* p, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<unsigned char*>(p) + off);
}

}  // namespace

// @src 14019cdc0  delete object no
void step_delete_object(DB_STEP_DATA& s, std::int32_t no, bool* null_hit)
{
    LLOBJ_DATA* o = object_list_find(&s.objects, no);   // 14017aaf0 (s + 0x1380)
    if (o == nullptr) return;
    step_clear_bc_refs(s, no);                          // 1401a9640
    pair_remove_object(&s.inter_obj, no);               // 1401fae90 (s + 0x13c8)
    llobj_ctor::make_empty(o);                          // 140161260
    // @src 1401b05a0  getter of +0x3bf8 (inlined here)
    void* v = reinterpret_cast<void*>(o->u3bf8);
    if (v != nullptr) llobj::ext::vslot1_0(v);          // (*vtbl[1])(v, 0); the field is not cleared
    const char flag = 0;
    step_trim_empty_tail(s, &flag, null_hit);           // 14019ce70
}

// @src 1401a9640  clear the BC entries -no of the other meshed objects
void step_clear_bc_refs(DB_STEP_DATA& s, std::int32_t no)
{
    for (std::int32_t i = 0; i < s.u1378; i = i + 1) {
        auto* o = static_cast<LLOBJ_DATA*>(step_object_at(s, i + 1));   // 1401d6ed0
        if (o != nullptr && at<std::int32_t>(o, 0x87c) != 0 && at<std::int32_t>(o, 8) != no) {
            const std::int32_t nn = at<std::int32_t>(o, 0x880);
            const std::int32_t nf = at<std::int32_t>(&s, 0x1640);
            for (std::int32_t k = 0; k < nn * nf; k = k + 1) {
                std::int32_t* bc = at<std::int32_t*>(o, 0x990);
                if (bc[k] == -no) bc[k] = 0;
            }
        }
    }
}

// @src 1401fae90  remove the inter-object records of object no
void pair_remove_object(Cls_1401fa020* l, std::int32_t no)
{
    Cls_1401f46e0* next = l->head;
    Cls_1401f46e0* p;
    while (p = next, p != nullptr) {
        if (p->u168 == no || (next = p->next, p->u16c == no)) {
            pair_remove(l, p);                          // 1401fab70
            next = l->head;
        }
    }
}

// @src 1401fab70  unlink and delete one record
bool pair_remove(Cls_1401fa020* l, Cls_1401f46e0* p)
{
    Cls_1401f46e0* prev = l->head;
    Cls_1401f46e0* q = l->head;
    while (q != nullptr && q != p) {
        prev = q;
        q = q->next;
    }
    if (q == nullptr) return false;
    prev->next = q->next;
    if (l->head == q) l->head = q->next;
    if (l->tail == q) l->tail = prev;
    if (q != nullptr) delete q;                         // 1401fb200(q, 1)
    l->count = l->count - 1;
    return true;
}

// @src 14019ce70  remove the empty objects at the end of the list
bool step_trim_empty_tail(DB_STEP_DATA& s, const char* flag, bool* null_hit)
{
    bool removed = false;
    for (std::int32_t pos = s.u1378; 0 < pos; pos = pos - 1) {
        auto* o = static_cast<LLOBJ_DATA*>(step_object_at(s, pos));   // 1401d6ed0
        if (o == nullptr) {
            // the original reads o->+0xc here (null dereference)
            if (null_hit != nullptr) *null_hit = true;
            return removed;
        }
        if (*flag != 0) {
            // 140160cc0 is owned by defdbio (gendb::object_has_geometry), above this library; no
            // caller passes flag 1 (14019cdc0 passes 0)
            DEF_NOT_PORTED(0x140160cc0);
        }
        if (llobj_not_kind7(o)) break;                  // 140160f10
        object_list_remove_number(&s.objects, pos);     // 1401b05c0 (s + 0x1380)
        s.u1378 = s.u1378 - 1;
        removed = true;
    }
    return removed;
}

// @src 1401b05c0  remove the object of number no
void object_list_remove_number(Cls_14017a6c0* l, std::int32_t no)
{
    LLOBJ_DATA* o = object_list_find(l, no);            // 14017aaf0
    object_list_remove(l, o);                           // 14017a960
}

// @src 14017a960  unlink and delete one object
bool object_list_remove(Cls_14017a6c0* l, LLOBJ_DATA* o)
{
    LLOBJ_DATA* prev = l->head;
    LLOBJ_DATA* q = l->head;
    while (q != nullptr && q != o) {
        prev = q;
        q = q->next;
    }
    if (q == nullptr) return false;
    prev->next = q->next;
    if (l->head == q) l->head = q->next;
    if (l->tail == q) l->tail = prev;
    if (q != nullptr) llobj::ext::llobj_delete(q);      // 14019a290(q, 1)
    l->count = l->count - 1;
    return true;
}

}  // namespace def::model
