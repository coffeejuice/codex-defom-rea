// Object slots of an operation, the OBJ entry and its defaults, and the dimension accessors
// (owner P12, "P02 split, objects" rows of owners.tsv). Spec: s1-01 5.4, 9.3, 9.4, 10.
// Decompiles: s1-01 dc/acc0.c, open4.c, open5.c, opr3.c, kw0.c, item1.c; s1-07 dc/cog0.c.
#include "m2m/objects.hpp"
#include "m2m/mst/writer.hpp"

namespace m2m {

// ---- dimension accessors (opBase vt+0x190) ------------------------------------------------------

// @src 0x14005f0f0
int opDim(const Operation& op)
{
    int d = 3;
    if (opBase(op)) d = opBase(op)->dim();            // 0x140054e90, vt+0x190
    return d;
}

// @src 0x14005f150
bool is2D(const Operation& op)
{
    bool r = false;
    if (opBase(op)) r = opBase(op)->dim() == 2;
    return r;
}

// @src 0x14005f1c0
bool is3D(const Operation& op)
{
    bool r = true;
    if (opBase(op)) r = opBase(op)->dim() == 3;
    return r;
}

// @src 0x14005f230
bool startsFromKey(const Operation& op)
{
    if (isChainHead(op)) return true;                 // 0x140050bc0
    return op.restartGiven != 0;                      // 0x140064740: OPI+0x150
}

// ---- slots ----------------------------------------------------------------------------------------

// @src 0x14005fa80
int objSlots(const Operation& op)
{
    return static_cast<int>(op.objIds.size());
}

// @src 0x14005fb30
int objId(const Operation& op, int slot)
{
    if (!(slot > -1 && slot < objSlots(op))) return -99;   // 0x140060600; 0xffffff9d
    return op.objIds[static_cast<size_t>(slot)];
}

// @src 0x14005fc50
int objIdN(const Operation& op, int n)
{
    return objId(op, n - 1);
}

// @src 0x14005fc80
bool setObjId(Operation& op, int slot, int id)
{
    if (slot < 0) return false;
    if (slot < objSlots(op)) {
        op.objIds[static_cast<size_t>(slot)] = id;
        return true;
    }
    while (!(slot < objSlots(op))) op.objIds.push_back(-1);
    op.objIds[static_cast<size_t>(slot)] = id;
    return true;
}

// @src 0x140061220
int objCount(const Operation& op)
{
    return static_cast<int>(op.objs.size());
}

// @src 0x140061020
ObjEntry* appendObj(Operation& op, int index, std::unique_ptr<ObjEntry> given)
{
    if (!given) {
        given = newObj();                             // operator new(0x1c8) + 0x14004d160
        setObjNumber(*given, index + 1);
    }
    op.objs.push_back(std::move(given));
    return op.objs.back().get();
}

// @src 0x140061250  the entry of a valid slot, appending new entries up to it; null otherwise
ObjEntry* objEntry(Operation& op, int slot)
{
    if (slot > -1 && slot < objSlots(op)) {
        while (objCount(op) <= slot) appendObj(op, objCount(op));
        return op.objs[static_cast<size_t>(slot)].get();
    }
    return nullptr;
}

// @src 0x1400612d0
ObjEntry* objEntryN(Operation& op, int n)
{
    return objEntry(op, n - 1);
}

// ---- OBJ --------------------------------------------------------------------------------------------

// @src 0x14004e260  (the +0x108 table ctor 0x140898dc0 is the empty svTable)
// with the reset @src 0x14004e2a0: every flag and value 0 except NRECVL and ERECVL flags (+0xe0,
// +0xf4) = 1; +0x104 = 0; SVSCAL descriptor 0x140899010(+0x108, 0x81) and +0x180 = 0.
void resetStateVars(StateVars& sv)
{
    sv = StateVars{};
    sv.nrecvlFlag = 1;                                // +0xe0
    sv.erecvlFlag = 1;                                // +0xf4
    sv.objNumber = 0;                                 // +0x104
    sv.svscal = 0;                                    // +0x180
}

// @src 0x14004d450
void objDefaults(ObjEntry& o)
{
    o.objectType = 7;                                 // +0x04
    o.f08 = 7;                                        // +0x08
    o.remesh = false;                                 // +0x1b9
    o.meshSetting = false;                            // +0x1ba
    o.mesh2dAbsolute = false;                         // +0x1bb
    o.mesh3dFinerInternal = false;                    // +0x1bc
    o.f1bd = false;                                   // +0x1bd
    o.defineBCC = false;                              // +0x1be
    o.assignMovement = false;                         // +0x1bf (+0x1c0 is not reset)
    o.thermalStop = false;                            // +0x1c1
    o.hasGeo = false;                                 // +0x18
    o.hasMesh = false;                                // +0x19
    o.skipDefine = false;                             // +0x1b8
    o.matIds.clear();                                 // 0x14004db20: +0x20
    o.equipmentId = 0;                                // 0x14004de70: +0x28 = 0
    objResetStateVars(o);                             // 0x14004ded0
}

// @src 0x14004d160
std::unique_ptr<ObjEntry> newObj()
{
    auto o = std::make_unique<ObjEntry>();
    resetStateVars(o->sv);                            // 0x14004e260 on +0x30
    o->number = 0;
    o->name = String();                               // QString::null
    objDefaults(*o);
    return o;
}

// @src 0x14004d520
void setObjNumber(ObjEntry& o, int n)
{
    o.number = n;
    setSvObjNumber(o.sv, o.number);                   // 0x14004e810
}

}  // namespace m2m
