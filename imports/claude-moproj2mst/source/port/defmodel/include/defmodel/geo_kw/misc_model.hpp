// defmodel/geo_kw/misc_model.hpp: model operations behind SYMSUF, WPAXIS and GEO23 (packet
// kw-g2geo, misc family).
//
//   SYMSUF  list of Cls_140257340 (id, point, direction) at LLOBJ_DATA+0x2e80 (p1-06 class)
//   WPAXIS  list of 0xe8-byte records Cls_140258580 at LLOBJ_DATA+0x20a0 (p1-06 list class;
//           record fields by offset, WPAXIS.md)
//   GEO23   the sub-structure Cls_1401889a0 at LLOBJ_DATA+0x2be8 (owner model at +0x120)
#pragma once

#include <cstdint>

namespace def::model {
class DB_STEP_DATA;
namespace linked {
class Cls_140257340;
class Cls_140257340List;
class Cls_140258580;
class Cls_140258580List;
}  // namespace linked
}  // namespace def::model

namespace def::model::geo_kw {

// @src 1402580f0  first entry whose id (+0) equals id, or null
linked::Cls_140257340* symsuf_find(linked::Cls_140257340List& l, std::int32_t id);

// @src 140259950  append a new record (operator new(0xe8), ctor 140258580); id = the argument, or
//                 the new count when it is 0; returns the record
linked::Cls_140258580* wpaxis_add(linked::Cls_140258580List& l, std::int32_t id);
// @src 140259b60  record k (1-based by position; the head for k <= 1; null past the end)
linked::Cls_140258580* wpaxis_nth(linked::Cls_140258580List& l, std::int32_t k);
// @src 140259bc0  append records (id = count) while count < id, then wpaxis_nth(id)
linked::Cls_140258580* wpaxis_get(linked::Cls_140258580List& l, std::int32_t id);
// @src 14019a550  count (+0x10)
std::int32_t wpaxis_count(linked::Cls_140258580List& l);

// @src 140189150  G23+0 = (flag ? 1401ee4c0(owner+0x750) : 0), only when the owner (+0x120) is set
void geo23_set_mode_from_geotyp(void* g23, bool flag);
// @src 1401891c0  G23+0 (mode) = *v when 0 <= *v < 3
void geo23_set_kind(void* g23, const std::int32_t* v);
// @src 1401ee4c0  sim control S: 1401ee460(S) ? 1 : 2
std::int32_t simctl_geo_kind(const void* s);
// @src 1401ee460  S+0x100 (GEOTYP) 1 or 3 -> true, 2 or 4 -> false, otherwise true
bool simctl_geotyp_flag(const void* s);

}  // namespace def::model::geo_kw
