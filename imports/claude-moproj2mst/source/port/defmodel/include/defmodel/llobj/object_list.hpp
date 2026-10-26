// defmodel/llobj/object_list.hpp: object-list lookup shared by the model and the KEY packets.
// Defined in src/material/state_remap.cpp (kw-g4b); kw-g1a and kw-g2obj call it.
#pragma once

#include <cstdint>

namespace def::model {

class Cls_14017a6c0;
class LLOBJ_DATA;

// 14017aaf0: the first object of the list whose number (+8) is `id`, or null.
LLOBJ_DATA* object_list_find(const Cls_14017a6c0* l, std::int32_t id);
// 140160f10  object kind (+0xc) is not 7 (src/material/state_remap.cpp)
bool llobj_not_kind7(const LLOBJ_DATA* o);

} // namespace def::model
