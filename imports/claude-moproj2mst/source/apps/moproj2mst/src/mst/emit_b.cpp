// Shared MST emitters, half B (owner: P09). Spec: s1-02 5.1 and 6 and the decompiles in s1-02
// a1s.c/a2s.c/a3s.c. Every literal is the .rdata string at the address in its comment (bytes
// checked against the exe copy); "\t" literals are 1 byte. REL(p) of the spec is app::relToDb.
//
// The 4th argument n of the emitters below is unused by every one of them (s1-04 O-1, s1-05 Q7):
// the text does not depend on it.
#include "m2m/mst/emit.hpp"
#include "m2m/qt4path.hpp"

namespace m2m::mst {

namespace {

// The "KFREAD    \t1" line plus the path line, used by several emitters with their own literal.
void kfread(W& w, const char* lit, const String& p)
{
    w.ts << lit << 1 << endl;
    w.ts << p << endl;
}

}  // namespace

// @src 0x140135220
void emitObjects(W& w, Project& /*prj*/, const String& db, int /*n*/, Operation* sim, bool single)
{
    State& st = w.st;
    int count = objSlots(*sim);                          // 0x14005fa80
    st.objMaskCur.resize(static_cast<size_t>(count), 0); // 0x140136c60: keeps the existing prefix
    for (int i = 0; i < count; ++i) {
        st.objMaskCur[static_cast<size_t>(i)] = 0;
        ObjEntry* o = objEntry(*sim, i);                 // 0x140061250
        if (!o || o->objectType == 7 /*0x14004d5a0*/ || o->skipDefine /*0x14004df40*/) continue;
        int id = objId(*sim, i);                         // 0x14005fb30
        int num = i + 1;                                 // 0x14005fc10 (i is in range)
        if (!single || num != 1) {
            if (id != -2) {                              // 0x1400606c0: -2 = from DB
                Item* od = objItem(*sim, id);            // 0x140060030
                if (!od) continue;                       // mask stays 0
                String p = qt::cleanPath(od->keyPath);   // 0x14006ba60
                if (!p.isEmpty()) {
                    w.ts << "OBJTYP\t\t" /*1421acc98*/ << num << "\t" /*1421ac70c*/ << 0 << endl;
                    p = app::relToDb(db, p);
                    kfread(w, "KFREAD    \t" /*1421acca8*/, p);
                }
            }
            if (o->matIds.size() == 1) {                 // 0x14004db50
                int m = o->matIds[0];                    // 0x14004db80
                w.ts << "MTLGRP    \t" /*1421accb8*/ << num << "\t" /*1421ac75c*/ << 0
                     << "\t" /*1421ac7cc*/ << m << endl;
            }
            st.objMaskCur[static_cast<size_t>(i)] = 1;
        } else {
            if (o->matIds.size() == 1) {
                int m = o->matIds[0];
                w.ts << "MTLGRP    \t" /*1421acc88*/ << 1 << "\t" /*1421ac66c*/ << 0
                     << "\t" /*1421ac6bc*/ << m << endl;
            }
            st.objMaskCur[static_cast<size_t>(i)] = 1;
        }
    }
    emitObjtypClose(w, sim);
}

// @src 0x140135680  slot 0 only, no ObjectType test
void emitObjectsSingle(W& w, Project& /*prj*/, const String& db, int /*n*/, Operation* sim,
                       bool single)
{
    State& st = w.st;
    st.objMaskCur.resize(1, 0);                          // 0x140136c60
    for (int i = 0; i < 1; ++i) {
        st.objMaskCur[static_cast<size_t>(i)] = 0;
        ObjEntry* o = objEntry(*sim, i);
        if (!o || o->skipDefine) continue;
        int id = objId(*sim, i);
        int num = i + 1;                                 // 0x14005fc10
        if (num != 1) continue;
        if (!single) {
            if (id != -2) {
                Item* od = objItem(*sim, id);
                if (!od) continue;
                String p = qt::cleanPath(od->keyPath);
                if (!p.isEmpty()) {
                    w.ts << "OBJTYP\t\t" /*1421accd8*/ << 1 << "\t" /*1421ac83c*/ << 0 << endl;
                    p = app::relToDb(db, p);
                    kfread(w, "KFREAD    \t" /*1421acce8*/, p);
                }
            }
            if (o->matIds.size() == 1) {
                int m = o->matIds[0];
                w.ts << "MTLGRP    \t" /*1421accf8*/ << 1 << "\t" /*1421ac88c*/ << 0
                     << "\t" /*1421ac8dc*/ << m << endl;
            }
            st.objMaskCur[static_cast<size_t>(i)] = 1;
        } else {
            if (o->matIds.size() == 1) {
                int m = o->matIds[0];
                w.ts << "MTLGRP    \t" /*1421accc8*/ << 1 << "\t" /*1421ac81c*/ << 0
                     << "\t" /*1421ac82c*/ << m << endl;
            }
            st.objMaskCur[static_cast<size_t>(i)] = 1;
        }
    }
    emitObjtypClose(w, sim);
}

// @src 0x140135b80
void emitObjtypClose(W& w, Operation* sim)
{
    State& st = w.st;
    int nc = static_cast<int>(st.objMaskCur.size());     // 0x140136c10
    int np = static_cast<int>(st.objMaskPrev.size());
    for (int i = 0; i < np; ++i) {
        ObjEntry* o = objEntry(*sim, i);                 // may grow the list
        if (!o) {
            w.ts << "OBJTYP    \t" /*1421acd08*/ << i + 1 << "\t" /*1421ac92c*/ << 0 << endl;
        } else if (o->objectType == 7) {
            w.ts << "OBJTYP    \t" /*1421acd18*/ << i + 1 << "\t" /*1421ac97c*/ << 0 << endl;
        } else if (nc <= i || st.objMaskCur[static_cast<size_t>(i)] == 0) {
            w.ts << "OBJTYP    \t" /*1421acd28*/ << i + 1 << "\t" /*1421ac9cc*/ << 0 << endl;
        }
    }
    st.objMaskPrev.resize(static_cast<size_t>(nc), 0);
    for (int i = 0; i < nc; ++i)
        st.objMaskPrev[static_cast<size_t>(i)] = st.objMaskCur[static_cast<size_t>(i)];
}

// @src 0x140135da0
void emitContact(W& w, Project& /*prj*/, const String& db, int /*n*/, Operation* sim)
{
    String p = interObjectPath(*sim);                    // 0x14005a690
    p = app::relToDb(db, p);
    w.ts << "CNTACT    \t" /*1421acd38*/ << 0 << endl;
    kfread(w, "KFREAD    \t" /*1421acd48*/, p);
}

// @src 0x140135f10
void emitObjpos(W& w, Project& /*prj*/, const String& /*db*/, int /*n*/, Operation* sim)
{
    int count = static_cast<int>(sim->objpos.size());    // 0x140062070
    for (int i = 0; i < count; ++i) {
        const ObjPos& p = *sim->objpos[static_cast<size_t>(i)];   // 0x1400620a0
        if (!is2D(*sim))                                 // 0x14005f150
            emitObjposLine3D(w, p);
        else
            emitObjposLine2D(w, p);
    }
}

// @src 0x140135fc0
void emitTloc(W& w, Project& /*prj*/, const String& /*db*/, int /*n*/, Operation* sim)
{
    double t = sim->tloc;                                // 0x1400ac0e0
    if (0.0 /*1421ad1d0*/ < t) {                         // COMISD: NaN is false
        w.ts << "TLOC    \t" /*1421acd58*/ << sim->tloc << endl;
    }
}

// @src 0x140136040
void emitContactFlags(W& w, Project& /*prj*/, const String& /*db*/, int /*n*/, Operation* sim)
{
    const ContactBlock* c = &sim->icb;                   // 0x140065790 (never null)
    if (c) emitContactBlock(w, *c);
}

// @src 0x1400ac010
void emitContactBlock(W& w, const ContactBlock& c)
{
    if (c.enable /*+0x64*/) {
        for (int k = 0; k < 100; ++k) {
            if (c.flag[static_cast<size_t>(k)])
                w.ts << "INICTC    \t" /*1421425f0*/ << k + 1 << endl;
        }
    }
    if (c.gen /*+0x65*/) w.ts << "GENCTC    \t" /*1421425e0*/ << c.genVal << endl;
}

// @src 0x140136090
void emitStateVars(W& w, Project& /*prj*/, const String& /*db*/, int /*n*/, Operation* sim)
{
    if (objSlots(*sim) > 0) emitObjectStateVars(w, *sim);
}

// @src 0x1401360e0
void emitMeshSettings(W& w, Project& /*prj*/, const String& db, int /*n*/, Operation* sim)
{
    String p;
    int count = objSlots(*sim);
    if (count < 1) return;
    for (int i = 0; i < count; ++i) {
        ObjEntry* o = objEntry(*sim, i);
        if (o && o->meshSetting /*0x14004dfc0*/) {
            p = meshSettingsPath(*sim, i);               // 0x14005cf70
            p = app::relToDb(db, p);
            kfread(w, "KFREAD    \t" /*1421acd68*/, p);
        }
    }
    for (int i = 0; i < count; ++i) {
        ObjEntry* o = objEntry(*sim, i);
        if (o && o->remesh /*0x14004df80*/)
            w.ts << "REMESH    \t" /*1421acd78*/ << o->number /*0x14004d560*/ << endl;
    }
}

// @src 0x1401363b0
void emitBccDef(W& w, Project& /*prj*/, const String& db, int /*n*/, Operation* sim)
{
    String p;
    int count = objSlots(*sim);
    if (count < 1) return;
    for (int i = 0; i < count; ++i) {
        ObjEntry* o = objEntry(*sim, i);
        if (o && o->objectType == 5 /*0x14004d600*/ && o->defineBCC /*0x14004e0c0*/) {
            p = bccDefPath(*sim, i);                     // 0x14005da90
            p = app::relToDb(db, p);
            kfread(w, "KFREAD    \t" /*1421acd88*/, p);
        }
    }
}

// @src 0x1401365f0
void emitObjProperties(W& w, Project& /*prj*/, const String& db, int /*n*/, Operation* sim)
{
    String p;
    int count = objSlots(*sim);
    if (count < 1) return;
    for (int i = 0; i < count; ++i) {
        ObjEntry* o = objEntry(*sim, i);
        if (o && o->objectType == 5 /*0x14004d5a0*/) {
            p = objPropertiesPath(*sim, i);              // 0x14005eb10
            p = app::relToDb(db, p);
            kfread(w, "KFREAD    \t" /*1421acd98*/, p);
        }
    }
}

// @src 0x140136810
void emitEquipment(W& w, Project& /*prj*/, const String& db, int /*n*/, Operation* sim)
{
    Project* owner = sim->project;                       // 0x140055030
    if (!owner) return;                                  // 0x1400a5970: no manager without a project
    int count = objSlots(*sim);
    for (int i = 0; i < count; ++i) {
        ObjEntry* o = objEntry(*sim, i);
        if (!o) continue;
        int e = o->equipmentId;                          // 0x14004de90
        if (e <= 0) continue;
        String p = equipmentKeyPath(*owner, e);          // 0x1400a5970 -> vt+0x208 (P17)
        p = app::relToDb(db, p);
        if (!p.isNull()) {
            w.ts << "IMPEQP    \t" /*1421acda8*/ << i + 1 << endl;
            w.ts << p << endl;
        }
    }
}

}  // namespace m2m::mst
