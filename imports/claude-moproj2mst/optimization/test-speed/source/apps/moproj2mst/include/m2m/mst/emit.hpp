// m2m/mst/emit.hpp: MST top level and shared emitters (seam). Spec: s1-02 (sections in brackets).
// Owners: P07 master (src/mst/master.cpp), P08 emitters A (src/mst/emit_a.cpp), P09 emitters B
// (src/mst/emit_b.cpp), P17 DOE (src/mst/doe.cpp). `n` is the SIMULATION number (or the pass
// index when the Cogging writer calls; s1-03 1). Arguments follow the original's order.
#pragma once

#include <map>

#include "m2m/mst/doe_model.hpp"
#include "m2m/mst/writer.hpp"

namespace m2m::mst {

// =================================================================================================
// P07: top level (s1-02 2-4)
// =================================================================================================
// @src 0x1400a20d0  saveMst: main MST, then the DOE part when the active sub-project is DOE
//                   (the DOE part is P17's and M2M_E_UNSUPPORTED for now)
bool saveMst(W& w, Project& prj);
// @src 0x1400a50a0  mstPath(prj, full): changeSuffix(full ? path : completeBaseName(path), ".MST")
String mstPath(const Project& prj, bool full);
// @src 0x1401313c0  writeMainMst: false when there is no sub-project; the file itself is opened
//                   by the caller (the C ABI writes w.ts to a temporary file and renames it)
bool writeMainMst(W& w, Project& prj);
// @src 0x140131680  header block [3.1]
void writeHeader(W& w, Project& prj);
// @src 0x1401318b0  main-project block [3.2]
void writeMainBlock(W& w, Project& prj);
// @src 0x140131df0  operation walk over sub-project subIdx [4.1]
void walkOperations(W& w, Project& prj, int subIdx);
// @src 0x140132100  non-DOE dispatch [4.3]
void dispatchOperation(W& w, Project& prj, const String& db, int* counter, Operation* sim,
                       int a6, int a7);
// @src 0x140131d10  trailer [3.4]
void writeTrailer(W& w, Project& prj);

// =================================================================================================
// P08: shared emitters A (s1-02 6)
// =================================================================================================
// @src 0x140131c10  SIMULATION header
void emitSimHeader(W& w, int n, Operation* sim);
// @src 0x140134250  DIM + NSTART (n == 1) or DBREAD with the NSTART step
void emitDimNstart(W& w, const String& db, int n, Operation* sim, int dbreadArg);
// @src 0x140134360  NSTART (n == 1)
void emitNstart(W& w, const String& db, int n, int v);
// @src 0x1401343e0  DBREAD (n != 1)
void emitDbread(W& w, const String& db, int n, int arg);
// @src 0x140134510  KFREAD of the operation's own KEY (restart key)
void emitKfreadRestart(W& w, const String& db, Operation* sim);
// @src 0x140134680  extra KFREAD (UserDefinedKey, not relativised)
void emitKfreadUser(W& w, Operation* sim);
// @src 0x140134730  OPRNAM
void emitOprnam(W& w, const String& name);
// @src 0x1401347e0  SIMNAM
void emitSimnam(W& w, const String& name);
// @src 0x140134900  CURSIM
void emitCursim(W& w, int n, int a, int b);
// @src 0x140134a10  GENDB (or KFWRIT)
void emitGendb(W& w, const String& db, int n, int mode, bool flag5);
// @src 0x140134bf0  MDSOBJ, TNOW, KFREAD SimCtrl.KEY
void emitSimCtrl(W& w, Project& prj, const String& db, int n, Operation* sim);
// @src 0x140134db0  KFREAD material files
void emitMaterials(W& w, Project& prj, const String& db, int n, Operation* sim);
// @src 0x140058e30 / 0x140058f30  material id strings of the operation's objects [6.1]
std::vector<String> materialIdStrings(Operation& sim);
// @src 0x140922160  index of keyword `id` in the DEF_PRE keyword table at 0x1427c06e0, -1 if absent
int keywordIndex(int id);
// @src 0x140921f30  name of keyword `id` (s1-04 0.2: kw(3) = "KFREAD", kw(10) = "KFWRIT";
// "ERROR: KEYWORD NOT EXISTS!" if absent)
const char* keywordName(int id);
// @src 0x140136b60  "CYCL" + QString::number(cycleIdx), without the "-" (SIMNAM 0x1401347e0 adds it)
String cyclePrefix(const State& st);
// @src 0x14006d380  item manager vt+0x208: cleanPath(keyPath) of the item with that id, null if none
//                   (materials in 0x140134db0, equipment in 0x140136810)
String itemKeyPathClean(const ItemManager& mgr, int id);

// =================================================================================================
// P09: shared emitters B (s1-02 6)
// =================================================================================================
// @src 0x140135220  OBJTYP / KFREAD object / MTLGRP (single = false from the base) [6.2]
void emitObjects(W& w, Project& prj, const String& db, int n, Operation* sim, bool single);
// @src 0x140135680  single-object variant (MultiBlow)
void emitObjectsSingle(W& w, Project& prj, const String& db, int n, Operation* sim, bool single);
// @src 0x140135b80  closing OBJTYP-0 lines and mask copy [6.2]
void emitObjtypClose(W& w, Operation* sim);
// @src 0x140135da0  CNTACT + KFREAD InterObject.KEY
void emitContact(W& w, Project& prj, const String& db, int n, Operation* sim);
// @src 0x140135f10  OBJPOS lines [6.3]
void emitObjpos(W& w, Project& prj, const String& db, int n, Operation* sim);
// @src 0x140065f80  one OBJPOS line, 3D
void emitObjposLine3D(W& w, const ObjPos& p);
// @src 0x140065af0  one OBJPOS line, 2D
void emitObjposLine2D(W& w, const ObjPos& p);
// @src 0x140135fc0  TLOC (t > 0)
void emitTloc(W& w, Project& prj, const String& db, int n, Operation* sim);
// @src 0x140136040 -> 0x1400ac010  INICTC / GENCTC
void emitContactFlags(W& w, Project& prj, const String& db, int n, Operation* sim);
// @src 0x1400ac010  INICTC / GENCTC lines of a contact block
void emitContactBlock(W& w, const ContactBlock& c);
// @src 0x140136090 -> 0x1400613b0  per-object state variables and NRECVL/ERECVL [6.4]
void emitStateVars(W& w, Project& prj, const String& db, int n, Operation* sim);
// @src 0x1400613b0
void emitObjectStateVars(W& w, Operation& sim);
// @src 0x14004e830  state variables of one object, 2D
void emitSv2D(W& w, const StateVars& sv);
// @src 0x14004f1c0  state variables of one object, 3D
void emitSv3D(W& w, const StateVars& sv);
// @src 0x14004f050  SVSCAL lines
void emitSvscal(W& w, const StateVars& sv);
// @src 0x1401360e0  KFREAD MeshSettings, REMESH
void emitMeshSettings(W& w, Project& prj, const String& db, int n, Operation* sim);
// @src 0x1401363b0  KFREAD BccDef
void emitBccDef(W& w, Project& prj, const String& db, int n, Operation* sim);
// @src 0x1401365f0  KFREAD ObjProperties
void emitObjProperties(W& w, Project& prj, const String& db, int n, Operation* sim);
// @src 0x140136810  IMPEQP
void emitEquipment(W& w, Project& prj, const String& db, int n, Operation* sim);
// @src 0x140132a40  DOE geometry variables (empty unless doeMode)
void emitDoeGeometry(W& w, Operation* sim);
// @src 0x1401322d0  DOE variables (empty unless doeMode)
void emitDoeVariables(W& w, Operation* sim);
// @src 0x140132bd0  DOE geometry block (writes .COR files in the original: s1-02 O5; here the
//                   .COR writer 0x140909a30 is not run). gv: geometry variables, map: doeObjMap
void emitDoeGeometryBlock(W& w, Operation* sim, int opId, int objId,
                          const std::vector<doe::Var*>& gv, const std::map<int, int>& map);
// @src 0x14093a200  number of non-empty '\n'-separated parts (DOEVAR line counts)
int doeLineCount(const String& s);
// @src 0x14089c490  SVSCAL table value at (i, j, k, l), 0.0 when the index is invalid
double svTableValue(const StateVars& sv, int i, int j, int k, int l);
// @src 0x14089c2a0  SVSCAL table flat index of (i, j, k, l), -1 when out of range
int svTableIndex(const StateVars& sv, int i, int j, int k, int l);

// =================================================================================================
// P17: DOE path (s1-02 2.4, 3.3, 4.4; s1-05 7), only with M2M_FLAG_DOE_TEMP
// =================================================================================================
// @src 0x1401321a0  DOE dispatch; doeMap = the 8th argument, doe->v1c8() (0x14091bdb0), built once by the walk
void dispatchOperationDoe(W& w, Project& prj, const String& db, doe::Model* doe, int opId, int* counter,
                          Operation* sim, const std::map<int, int>& doeMap, int a9, int a10);
// @src 0x140131a50  sub-project block (*!SUBPRJ)
void writeSubBlock(W& w, Project& prj, int subIdx);
// @src 0x140131530  .MST.TEMP of a DOE sub-project
bool writeSubMst(W& w, Project& prj, int subIdx);

}  // namespace m2m::mst
