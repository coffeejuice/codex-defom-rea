// kw-g2geo misc family: model operations behind SYMSUF, WPAXIS and GEO23
// (defmodel/geo_kw/misc_model.hpp). Decompiles: deform-rea-work/out/g2geo/fam_misc.c.
#include <defmodel/geo_kw/misc_model.hpp>

#include <cstddef>

#include <defmodel/linked/axis_point_list.hpp>
#include <defmodel/linked/record_e8_list.hpp>

namespace def::model::geo_kw {

using linked::Cls_140257340;
using linked::Cls_140257340List;
using linked::Cls_140258580;
using linked::Cls_140258580List;

// @src 1402580f0  walk from the head (+0) over +0x38 until id matches
Cls_140257340* symsuf_find(Cls_140257340List& l, std::int32_t id)
{
    Cls_140257340* p = l.head;
    while (p != nullptr && p->id != id) p = p->next;
    return p;
}

// @src 140259950  append (the list's head/tail/count; the record link is +0xe0)
Cls_140258580* wpaxis_add(Cls_140258580List& l, std::int32_t id)
{
    Cls_140258580* r = new Cls_140258580();  // operator new(0xe8), ctor 140258580
    if (l.count == 0) {
        l.tail = r;
        l.head = r;
    } else {
        l.tail->next = r;
        l.tail = r;
    }
    ++l.count;
    std::int32_t v = id;
    if (id == 0) v = l.count;
    r->id = v;
    return r;
}

// @src 140259b60  record k by position (the head for k <= 1)
Cls_140258580* wpaxis_nth(Cls_140258580List& l, std::int32_t k)
{
    Cls_140258580* p = l.head;
    for (std::int32_t i = 1; i < k && p != nullptr; ++i) p = p->next;
    return p;
}

// @src 14019a550
std::int32_t wpaxis_count(Cls_140258580List& l) { return l.count; }

// @src 140259bc0  while count < id: add(0) and set its id to the count; then nth(id)
Cls_140258580* wpaxis_get(Cls_140258580List& l, std::int32_t id)
{
    while (wpaxis_count(l) < id) {
        Cls_140258580* r = wpaxis_add(l, 0);
        r->id = wpaxis_count(l);
    }
    return wpaxis_nth(l, id);
}

// @src 1401ee460  GEOTYP at S+0x100: 1, 3 -> true; 2, 4 -> false; any other value -> true
bool simctl_geotyp_flag(const void* s)
{
    std::int32_t v = *reinterpret_cast<const std::int32_t*>(static_cast<const std::byte*>(s) + 0x100);
    bool r = true;
    if (v == 1) {
        r = true;
    } else if (v == 2 || v == 4) {
        r = false;
    } else if (v == 3) {
        r = true;
    }
    return r;
}

// @src 1401ee4c0
std::int32_t simctl_geo_kind(const void* s) { return simctl_geotyp_flag(s) ? 1 : 2; }

// @src 140189150  owner pointer at G23+0x120 (int index 0x48)
void geo23_set_mode_from_geotyp(void* g23, bool flag)
{
    auto* b = static_cast<std::byte*>(g23);
    void* owner = *reinterpret_cast<void**>(b + 0x120);
    if (owner != nullptr) {
        std::int32_t& mode = *reinterpret_cast<std::int32_t*>(b);
        if (!flag) {
            mode = 0;
        } else {
            mode = simctl_geo_kind(static_cast<std::byte*>(owner) + 0x750);
        }
    }
}

// @src 1401891c0
void geo23_set_kind(void* g23, const std::int32_t* v)
{
    if (-1 < *v && *v < 3) *static_cast<std::int32_t*>(g23) = *v;
}

}  // namespace def::model::geo_kw
