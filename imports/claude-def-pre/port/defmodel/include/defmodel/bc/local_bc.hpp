// defmodel/bc/local_bc.hpp: model functions of the local boundary-condition keywords (packet
// kw-g2bc, family "local": BCCFNC, LOCTMP, LOCATM, LOCDEF, RSEDEF) that no other packet owns.
// Bodies in src/bc/local_bc.cpp.
//
// The other lists these keywords use are ported elsewhere:
//   BCCFNC  LLOBJ_DATA+0x2e68 = Cls_1401364a0 (p1-04: add 140136670, at 140136860, size 14013c1b0)
//   LOC*    LLOBJ_DATA+0x2f40 / +0x2f70 / +0x2fa0 = Cls_140269250 window lists (kw-g3:
//           winlist_extend 140269bc0, winlist_nth 140269900, winlist_count 14019a590;
//           window_reset 14025d1a0)
#pragma once

#include <cstdint>

#include <defmodel/layout/cls_1401364a0.hpp>
#include <defmodel/layout/dynary.hpp>

namespace def::model {

// FUN_1401368c0: the first DYNARY of the list whose +0x54 (function number) is `no`, or null.
DYNARY* dynary_list_find(const Cls_1401364a0* l, std::int32_t no);

} // namespace def::model
