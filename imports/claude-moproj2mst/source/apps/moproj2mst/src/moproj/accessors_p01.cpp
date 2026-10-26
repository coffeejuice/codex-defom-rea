// Model accessors moved from P02 to P01 (coordinator, 2026-10-22); callers are P09's emitters.
// Decompiles: s1-02 a2s.c, a3s.c, a4s.c, a9s.c, a10s.c, a11s.c.
#include "m2m/model.hpp"

namespace m2m {
namespace {

// @src 0x140060600  slot index valid: 0 <= slot < objSlots
bool validSlot(const Operation& op, int slot)
{
    return slot > -1 && slot < objSlots(op);
}

// @src 0x140060690  id > 0
bool isProjectObject(int id) { return id > 0; }

// @src 0x1400606c0  id == -2 ("Read From DB")
bool isFromDb(int id) { return id == -2; }

// @src 0x140060640  0x140060690 || 0x1400606c0
bool isValidObjectId(int id) { return isProjectObject(id) || isFromDb(id); }

// @src 0x140055050  active sub-project of the operation's project, null without a project
SubProject* activeSub(const Operation& op)
{
    if (!op.project) return nullptr;
    return activeSubProject(*op.project);             // 0x1400a7390
}

// @src 0x1400b9cd0  data root of a sub-project: the project's (P+8), or for a die-stress
// sub-project (type 4) its own root at sub+0x48, created on first use with the sub-project DB path
// (0x1400b9e80, 0x140cda460, 0x140cdcdc0). Only the root's recovery list A is modelled; the
// returned pointer is that list holder.
std::shared_ptr<std::vector<RecoveryEntry>>* dataRootRecoveryA(SubProject& sub)
{
    Project* p = sub.project;                         // 0x1400b5c40
    if (!p) return nullptr;
    std::shared_ptr<std::vector<RecoveryEntry>>* root = &p->recoveryA;   // 0x1400a5890 -> M+0x19a8
    if (sub.type == 4) {                              // 0x140010320
        if (!sub.ownRootRecoveryA) sub.ownRootRecoveryA = std::make_shared<std::vector<RecoveryEntry>>();
        root = &sub.ownRootRecoveryA;
    }
    return root;
}

}  // namespace

// @src 0x140060640  (exported for the MDT L2 loader)
bool slotHoldsObject(int id) { return isValidObjectId(id); }

// @src 0x14005a400
std::vector<RecoveryEntry>* recoveryListA(Operation& op)
{
    SubProject* sub = activeSub(op);
    if (!sub) return nullptr;
    std::shared_ptr<std::vector<RecoveryEntry>>* holder = dataRootRecoveryA(*sub);
    if (!holder) return nullptr;
    return holder->get();                             // *(QList<BC*>**)(root + 0x19a8) (0x1400655b0)
}

// @src 0x14005fc10
int slotNumber(const Operation& op, int slot)
{
    return validSlot(op, slot) ? slot + 1 : 0;
}

// @src 0x14005fb80
int slotOfObject(const Operation& op, int id)
{
    const int n = objSlots(op);                       // 0x14005fa80
    for (int i = 0; i < n; ++i) {
        const int v = objId(op, i);                   // 0x14005fb30
        if (isValidObjectId(v) && v == id) return i;
    }
    return -1;
}

// @src 0x140061310
ObjEntry* objEntryById(Operation& op, int id)
{
    const int slot = slotOfObject(op, id);
    if (slot < 0) return nullptr;
    return objEntry(op, slot);                        // 0x140061250
}

// @src 0x14005ff30
String objDisplayName(Operation& op, int id)
{
    if (isFromDb(id)) return qt::L("Read From DB");   // 0x142123638
    String r;
    if (ObjEntry* o = objEntryById(op, id)) r = o->name;   // 0x14004d730: OBJ+0x10
    return r;
}

}  // namespace m2m
