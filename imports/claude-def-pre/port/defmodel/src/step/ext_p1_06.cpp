// p1-03 calls into p1-06 and the DYNARY family (see ext.hpp), wired to the owners at the Phase 1
// gate.
#include "ext.hpp"

#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/cls_1401fa020.hpp>
#include <defmodel/linked/accessors.hpp>

namespace def::model::ext {

// part of @src 1401fa080  inter-object list reset (Cls_1401fa020::reset)
void fun_1401fa080(Cls_1401fa020* l) { l->reset(); }

// part of @src 1401fade0  inter-object list clear (Cls_1401fa020::clear)
void fun_1401fade0(Cls_1401fa020* l) { l->clear(); }

// part of @src 1401fc040  object material id (p1-06 linked::fun_1401fc040)
std::int32_t fun_1401fc040(void* obj) { return linked::fun_1401fc040(obj); }

// part of @src 14012f480  curve: free the arrays (key-common dynary_free_arrays)
void fun_14012f480(DYNARY* c) { dynary_free_arrays(c); }

} // namespace def::model::ext
