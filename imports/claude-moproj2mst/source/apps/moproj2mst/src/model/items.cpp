// Object item lookup of an operation (owner P16, slice "objects and item managers" moved from P12).
//
// The rest of the slice is GUI construction whose MST-relevant state is the defaults of the model
// structs (include/m2m/model.hpp) or is inlined by the reader (P01 read.cpp addItem = manager
// vt+0x170 + 0x14006c9c0). The mapping, address by address, is in docs/re/moproj2mst/p16/ledger.tsv.
#include "m2m/model.hpp"

namespace m2m {
namespace {

// @src 0x1400690a0  dynamic_cast<SfMoObjItem*>(SfMoItem*) (src 0x14356ebb0, target 0x14357bbf0)
Item* asObjItem(Item* it)
{
    return (it && it->kind == ItemKind::Object) ? it : nullptr;
}

}  // namespace

// @src 0x140060030  object item of the operation's project for an id, null if none
Item* objItem(const Operation& op, int id)
{
    // 0x1400550d0 (P07): the object manager of the op's project (data root +0x19d0), null without
    // a project
    if (!op.project) return nullptr;
    const ItemManager& mgr = op.project->objects;
    return asObjItem(mgr.find(id));                 // vt+0x1c8 (0x14006cea0: first row with id)
}

// @src 0x140068a80 (SfMoObjItem vt+0x48) / 0x14006b9f0 (SfMoItem vt+0x48)
String itemName(const Item& it)
{
    return it.kind == ItemKind::Object ? it.recName : it.name;   // 0x140068b40 -> 0x14004d730
}

// @src 0x140068a40 (SfMoObjItem vt+0x40) / 0x14006b9c0 (SfMoItem vt+0x40)
void setItemName(Item& it, const String& name)
{
    if (it.kind == ItemKind::Object) it.recName = name;          // 0x140068b40 -> 0x14004d700
    it.name = name;                                              // 0x14006bb40
}

}  // namespace m2m
