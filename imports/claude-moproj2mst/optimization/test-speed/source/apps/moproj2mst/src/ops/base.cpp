// Base operation writer and the OpClass base slots (packet P10).
// Spec: s1-02 5 (writer 0x1405f5340, body 0x1405f56c0), 7 (dimension 0x1405f5bf0); s1-03 2;
// s1-01 10 (props 0x1405ef320). The base +0x298 body 0x1405f5880 is P14's (src/model/simcount.cpp).
#include "m2m/mst/emit.hpp"
#include "m2m/mst/writer.hpp"

namespace m2m::mst {

// @src 0x1405f5340  vt+0x280 base writer (Base, HeatTransfer family, Forming(2D), HTFurnace,
// Pre(2D), ExpressBase, FormingExpressCommon, DiestressBM). onlySim (param 8) is not read.
void OpClass::writeMst(W& w, Project& prj, const String& db, int* sim, Operation* op, int dbreadArg,
                       int /*onlySim*/)
{
    *sim = *sim + 1;
    emitSimHeader(w, *sim, op);
    int gendbMode = 2;
    if (!op->restartGiven) {                       // 0x140064740: OPI+0x150
        if (!writesDimNstart())                    // vt+0x200
            emitDbread(w, db, *sim, dbreadArg);
        else
            emitDimNstart(w, db, *sim, op, dbreadArg);
        writeSetup(w, prj, db, *sim, op);          // vt+0x288
        emitDoeGeometry(w, op);
        emitDoeVariables(w, op);
        emitObjpos(w, prj, db, *sim, op);
        emitContactFlags(w, prj, db, *sim, op);
        emitStateVars(w, prj, db, *sim, op);
        emitKfreadUser(w, op);
        gendbMode = 2;
    } else {
        emitKfreadRestart(w, db, op);
        emitDoeGeometry(w, op);
        emitDoeVariables(w, op);
        emitKfreadUser(w, op);
        gendbMode = 0;
    }
    emitOprnam(w, op->name);                       // 0x140050ac0: OPI+0x118
    emitSimnam(w, op->name);
    const int b = 0;                               // 0x140050b10: constant 0, called first
    const int a = flowNumber(*op);                 // 0x140050b20
    emitCursim(w, *sim, a, b);
    emitGendb(w, db, *sim, gendbMode, false);
}

// @src 0x1405f56c0  vt+0x288 setup lines, the same function for every class
void OpClass::writeSetup(W& w, Project& prj, const String& db, int n, Operation* op)
{
    emitSimCtrl(w, prj, db, n, op);                // 0x140134bf0 MDSOBJ, TNOW, KFREAD SimCtrl.KEY
    emitMaterials(w, prj, db, n, op);              // 0x140134db0
    emitObjects(w, prj, db, n, op, false);         // 0x140135220
    emitContact(w, prj, db, n, op);                // 0x140135da0 CNTACT, KFREAD InterObject.KEY
    emitMeshSettings(w, prj, db, n, op);           // 0x1401360e0
    emitBccDef(w, prj, db, n, op);                 // 0x1401363b0
    emitEquipment(w, prj, db, n, op);              // 0x140136810 IMPEQP
    emitObjProperties(w, prj, db, n, op);          // 0x1401365f0
    writeExtra(w, prj, db, n, op);                 // vt+0x290
    emitTloc(w, prj, db, n, op);                   // 0x140135fc0
}

// @src 0x1405f79b0  vt+0x200 default: false (no DIM/NSTART)
bool OpClass::writesDimNstart() const
{
    return false;
}

// @src 0x1405f5860  vt+0x290: empty for every class
void OpClass::writeExtra(W&, Project&, const String&, int, Operation*) {}

// @src 0x1405f5bf0  vt+0x190: f = vbase->v58() = 0x1405fece0 = *(op+0x38)+0x80, the vbase from
// @src 0x141354350 / 0x140750600  vt+0x188 (HeatTransfer / Cogging: op+0x50); OpClass::dimFlags holds f
int OpClass::dim() const
{
    const int f = dimFlags;
    return (f & 3) == 3 ? 4 : (f & 1) ? 2 : (f & 2) ? 3 : 0;
}

// @src 0x1405ef320  props of op, or of the current item when op is null
Props* OpClass::propsOf(Operation* op) const
{
    Operation* o = op ? op : current;
    return o ? o->props.get() : nullptr;
}

}  // namespace m2m::mst
