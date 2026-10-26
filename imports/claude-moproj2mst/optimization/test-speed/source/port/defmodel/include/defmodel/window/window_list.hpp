// Methods of the window list Cls_140269250 (layout/cls_140269250.hpp, ctor/reset/owner/clear by
// p1-06) that p1-06 did not port; packet kw-g3. Bodies in src/window/window_list.cpp.
#pragma once

#include <cstdint>

#include <defmodel/layout/cls_140269250.hpp>
#include <defmodel/window/llwindef_data.hpp>

namespace def::model {

std::int32_t winlist_count(const Cls_140269250* l);                  // 14019a590 (+0x14)
LLWINDEF_DATA* winlist_new(Cls_140269250* l);                        // 1402694a0
void winlist_link(Cls_140269250* l, LLWINDEF_DATA* w);               // 1402693f0
LLWINDEF_DATA* winlist_nth(const Cls_140269250* l, std::int32_t n);  // 140269900
LLWINDEF_DATA* winlist_extend(Cls_140269250* l, std::int32_t n);     // 140269bc0
void winlist_copy(Cls_140269250* dst, const Cls_140269250* src);     // 140269340

} // namespace def::model
