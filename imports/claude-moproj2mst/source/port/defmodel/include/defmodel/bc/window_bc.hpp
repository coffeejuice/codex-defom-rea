// Window-list model functions the window BC writer (kw-g2bc family win, writer-map step 49)
// needs and no packet owns: the "has a shape" test of a window and the list removal.
// Bodies in src/bc/window_bc.cpp. Spec: docs/re/keywords/WINTMP.md (KEY writer, step 49).
#pragma once

#include <defmodel/layout/cls_140269250.hpp>
#include <defmodel/window/llwindef_data.hpp>

namespace def::model {

// 1402632d0: true when the window has shape data (not 3D: points at +0x180; 3D: shape 3 needs
// the _OBJ_GEO3D node data, other shapes the points).
bool window_has_shape(const LLWINDEF_DATA* w);

// 1402696f0: unlink `w` from `l` and delete it (count - 1). Returns 1, or 0 when `w` is not in
// the list. Faithful quirk: removing the only node leaves `tail` pointing at the deleted node
// (head is null, count 0; the next append resets both because it tests count == 0).
std::int32_t winlist_remove(Cls_140269250* l, LLWINDEF_DATA* w);

} // namespace def::model
