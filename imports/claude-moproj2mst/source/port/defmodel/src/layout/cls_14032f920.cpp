// Cls_14032f920: handle of the copy-on-write list<CustomVarDef> (packet p1-10).
// Layout: defmodel/layout/cls_14032f920.hpp; list code: members/custom_state_var.cpp.
#include <defmodel/layout/cls_14032f920.hpp>

namespace def::model {

// @src 14032f920  ctor: list = new VarListRep (operator new(0x18), 1403303a0)
Cls_14032f920::Cls_14032f920() { list = new VarListRep(); }

// @src 14032f990  dtor: release the rep (14032fe10)
Cls_14032f920::~Cls_14032f920() { release(list); }

} // namespace def::model
