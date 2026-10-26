// Model functions of the local boundary-condition keywords (packet kw-g2bc, family "local").
// Declarations: defmodel/bc/local_bc.hpp. Decompile: deform-rea-work/out/g2bc_local/w.c.
#include <defmodel/bc/local_bc.hpp>

#include <defmodel/llobj/dynary_table.hpp>

namespace def::model {

// @src 1401368c0  first node whose +0x54 equals `no` (walks the +0x60 links from the head)
DYNARY* dynary_list_find(const Cls_1401364a0* l, std::int32_t no)
{
    DYNARY* p = static_cast<DYNARY*>(l->head);
    while (p != nullptr && p->u54 != no) p = llobj::dynary_next(p);
    return p;
}

} // namespace def::model
