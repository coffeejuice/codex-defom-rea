// m2m/objects.hpp: per-operation object slots (OPI+0x40 ids, OPI+0x50 OBJ entries), OBJ ctor and
// field setters (owner: P12, the "P02 split, objects" rows of owners.tsv; the item-manager rows went to
// P16). Spec: s1-01 5.4, 9.3, 9.4, 10.
//
// The model.hpp seams objSlots/objId/objEntry/objEntryN/opDim/is2D/is3D/startsFromKey are
// implemented in src/model/objects.cpp too. The setters below are the original's one-line field
// writers; the open path (P01 parseObjects 0x140093c30, parseMoItem 0x14008e3d0) may call them or
// write the same fields directly.
#pragma once

#include <memory>

#include "m2m/model.hpp"

namespace m2m {

// ---- OPI object slots ---------------------------------------------------------------------------
// @src 0x14005fc80  OPI+0x40[slot] = id, padding with -1 up to slot; false for slot < 0
bool setObjId(Operation& op, int slot, int id);
// @src 0x140061220  count of OPI+0x50
int objCount(const Operation& op);
// @src 0x14005fc50  objId(op, n - 1)
int objIdN(const Operation& op, int n);
// @src 0x140061020  append an OBJ to OPI+0x50: `given`, or a new one numbered index + 1
ObjEntry* appendObj(Operation& op, int index, std::unique_ptr<ObjEntry> given = nullptr);
// @src 0x14005fa60  OPI+0x30 = 0 (engine pointer slot; nothing to model)
inline void clearOpi30(Operation&) {}

// ---- OBJ (0x1c8) ------------------------------------------------------------------------------------
// @src 0x14004d160  ctor: name null, number 0, state variables reset (0x14004e260), defaults
std::unique_ptr<ObjEntry> newObj();
// @src 0x14004d450  defaults: types 7, flags 0 (not +0x1c0), MatId list cleared (0x14004db20),
// EquipmentID 0 (0x14004de70), state variables reset (0x14004ded0)
void objDefaults(ObjEntry& o);
// @src 0x14004d520  slot number (1-based), also into the state variables (0x14004e810)
void setObjNumber(ObjEntry& o, int n);
// @src 0x14004e810
inline void setSvObjNumber(StateVars& sv, int n) { sv.objNumber = n; }
// @src 0x14004e260  state-variable block ctor (+0x108 table ctor 0x140898dc0, reset 0x14004e2a0)
void resetStateVars(StateVars& sv);
// @src 0x14004ded0
inline void objResetStateVars(ObjEntry& o) { resetStateVars(o.sv); }
// @src 0x14004df00  OBJ+0x30
inline StateVars& objStateVars(ObjEntry& o) { return o.sv; }
// @src 0x14004d700
inline void setObjName(ObjEntry& o, const String& s) { o.name = s; }
// @src 0x14004deb0
inline void setObjEquipmentId(ObjEntry& o, int id) { o.equipmentId = id; }
// @src 0x14004df20
inline void setObjSkipDefine(ObjEntry& o, bool v) { o.skipDefine = v; }
// @src 0x14004df60
inline void setObjRemesh(ObjEntry& o, bool v) { o.remesh = v; }
// @src 0x14004dfa0
inline void setObjMeshSetting(ObjEntry& o, bool v) { o.meshSetting = v; }
// @src 0x14004dfe0
inline void setObjMesh2dAbsolute(ObjEntry& o, bool v) { o.mesh2dAbsolute = v; }
// @src 0x14004e020
inline void setObjMesh3dFinerInternal(ObjEntry& o, bool v) { o.mesh3dFinerInternal = v; }
// @src 0x14004e0a0
inline void setObjDefineBCC(ObjEntry& o, bool v) { o.defineBCC = v; }
// @src 0x14004e0e0
inline void setObjAssignMovement(ObjEntry& o, bool v) { o.assignMovement = v; }
// @src 0x14004e120
inline void setObjDefineRotationMovement(ObjEntry& o, bool v) { o.defineRotationMovement = v; }
// @src 0x14004e160
inline void setObjThermalStop(ObjEntry& o, bool v) { o.thermalStop = v; }

// ---- OPI / SfMoOprItem fields -----------------------------------------------------------------------
// @src 0x14004ca30  OPI+0x4c Equipment Id
inline void setOpEquipmentId(Operation& op, int id) { op.equipmentId = id; }
// @src 0x14004c9a0  item+0x50 "<Type>DiagItem"
inline void setDiagName(Operation& op, const String& s) { op.diagName = s; }
// @src 0x14004c900
inline String diagName(const Operation& op) { return op.diagName; }

}  // namespace m2m
