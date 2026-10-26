// p1-03 calls into p1-04 (see ext.hpp), wired to the owner's methods at the Phase 1 gate.
#include "ext.hpp"

#include <defmodel/layout/cls_14017a6c0.hpp>

namespace def::model::ext {

// part of @src 14017a700  objects list reset (Cls_14017a6c0::reset)
void fun_14017a700(Cls_14017a6c0* l) { l->reset(); }

// part of @src 14017ab70  objects list clear (Cls_14017a6c0::clear)
void fun_14017ab70(Cls_14017a6c0* l) { l->clear(); }

// part of @src 14017aa80  objects list node n (Cls_14017a6c0::at)
void* fun_14017aa80(Cls_14017a6c0* l, std::int32_t n) { return l->at(n); }

} // namespace def::model::ext
