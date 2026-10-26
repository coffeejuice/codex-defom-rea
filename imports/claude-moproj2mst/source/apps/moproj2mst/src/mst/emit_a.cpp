// Shared MST emitters, half A (owner: P08). Spec: s1-02 6 and the decompiles in s1-02 a1s.c/a2s.c.
// Every literal is the .rdata string at the address in its comment; "\t" literals are 1 byte.
#include "m2m/mst/emit.hpp"
#include "m2m/qt4path.hpp"

namespace m2m::mst {

// @src 0x140131c10
void emitSimHeader(W& w, int n, Operation* sim)
{
    Stream& ts = w.ts;
    ts << endl;
    ts << "*-------------------------------------------------------------------------*" /*1421ac7d0*/
       << endl;
    ts << "SIMULATION\t" /*1421ac820*/ << n << endl;
    String type = sim->classKey;   // 0x140050d00
    ts << "*!SIMTYP  \t" /*1421ac830*/ << type << endl;
    ts << "*-------------------------------------------------------------------------*" /*1421ac840*/
       << endl;
}

// @src 0x140134250
void emitDimNstart(W& w, const String& db, int n, Operation* sim, int dbreadArg)
{
    if (n == 1) {
        if (sim && sim->nstartGiven /*0x14005a2a0*/) {
            int v = sim->startStepNo;   // 0x14005a2e0
            if (v != 0) {
                int d = opDim(*sim);     // 0x14005f0f0
                w.ts << "DIM    \t" /*1421acb38*/ << d << endl;
                emitNstart(w, db, 1, v);
            }
        }
    } else {
        if (sim && sim->nstartGiven) dbreadArg = startStep(*sim);   // 0x14005a300
        emitDbread(w, db, n, dbreadArg);
    }
}

// @src 0x140134360
void emitNstart(W& w, const String& /*db*/, int n, int v)
{
    if (n == 1) w.ts << "NSTART    \t" /*1421acb48*/ << v << endl;
}

// @src 0x1401343e0
void emitDbread(W& w, const String& db, int n, int arg)
{
    if (n != 1) {
        String name = qt::fileName(db);   // 0x140818f10
        w.ts << "DBREAD    \t" /*1421acb58*/ << arg << endl;
        w.ts << name << endl;
    }
}

// @src 0x140134510
void emitKfreadRestart(W& w, const String& db, Operation* sim)
{
    if (!sim->restartGiven) return;   // 0x140064740
    String p = restartKeyPath(*sim, true);   // 0x140064a50
    if (!p.isEmpty()) {
        p = app::relToDb(db, p);   // QDir(0x140819590(db)).relativeFilePath(p)
        w.ts << "KFREAD    \t" /*1421acb78*/ << 1 << endl;
        w.ts << p << endl;
    }
}

// @src 0x140134680
void emitKfreadUser(W& w, Operation* sim)
{
    if (!sim->userKeyGiven) return;   // 0x140062e40
    String s = sim->userKey;          // 0x140062e80
    if (!s.isEmpty()) {
        w.ts << "KFREAD    \t" /*1421acb88*/ << 1 << endl;
        w.ts << s << endl;
    }
}

// @src 0x140134730
void emitOprnam(W& w, const String& name)
{
    if (!w.st.cycleMode) {
        w.ts << "OPRNAM" /*1421acb94*/ << endl << name << endl;
    } else {
        w.ts << "OPRNAM" /*1421acb9c*/ << endl << w.st.cycleOprName << endl;
    }
}

// @src 0x140136b60
String cyclePrefix(const State& st)
{
    String num = qt::number(st.cycleIdx);
    String cycl = qt::L("CYCL" /*1421acdb4*/);
    return cycl + num;
}

// @src 0x1401347e0
void emitSimnam(W& w, const String& name)
{
    if (!w.st.cycleMode) {
        w.ts << "SIMNAM" /*1421acba4*/ << endl << name << endl;
    } else {
        String dash = qt::L("-" /*1421ac34c*/);
        String prefix = cyclePrefix(w.st);
        w.ts << "SIMNAM" /*1421acbac*/ << endl << prefix << dash << name << endl;
    }
}

// @src 0x140134900
void emitCursim(W& w, int n, int a, int b)
{
    if (!w.st.cycleMode) {
        w.ts << "CURSIM    \t" /*1421acbb8*/ << n << "\t" /*1421ac3cc*/ << a << "\t" /*1421ac41c*/ << b
             << endl;
    } else {
        w.ts << "CURSIM    \t" /*1421acbc8*/ << n << "\t" /*1421ac46c*/ << w.st.cycleCursim
             << "\t" /*1421ac4bc*/ << b << endl;
    }
}

// @src 0x140134a10
void emitGendb(W& w, const String& db, int n, int mode, bool flag5)
{
    if (!w.st.kfwrit) {
        int c = flag5 ? 2 : 1;
        if (n == 1) {
            w.ts << "GENDB     \t" /*1421acbd8*/ << 2 << "\t" /*1421ac50c*/ << c << "\t" /*1421ac55c*/ << 0
                 << endl;
        } else {
            w.ts << "GENDB     \t" /*1421acbe8*/ << 1 << "\t" /*1421ac5cc*/ << c << "\t" /*1421ac61c*/
                 << mode << endl;
        }
        String name = qt::fileName(db);   // 0x140818f10
        w.ts << name << endl;
    } else {
        w.ts << "KFWRIT" /*1421acbf4*/ << endl;
        w.ts << w.st.kfwritPath << endl;
    }
}

// @src 0x140134bf0  (prj and n are not read)
void emitSimCtrl(W& w, Project& /*prj*/, const String& db, int /*n*/, Operation* sim)
{
    String p = simCtrlPath(*sim);   // 0x140059290
    if (!p.isEmpty()) {
        w.ts << "MDSOBJ    \t" /*1421acc00*/ << "  0       0       0    0.0000000000E+000" /*1421acc10*/
             << endl;
        w.ts << "TNOW\t   \t" /*1421acc40*/ << "  -1E21       0       1" /*1421acc50*/ << endl;
        p = app::relToDb(db, p);
        w.ts << "KFREAD    \t" /*1421acc68*/ << 1 << endl;
        w.ts << p << endl;
    }
}

// @src 0x14006d380
String itemKeyPathClean(const ItemManager& mgr, int id)
{
    const Item* it = mgr.find(id);   // vt+0x1c8
    if (!it) return String();
    return qt::cleanPath(it->keyPath);   // 0x14006ba60
}

// @src 0x140058f30  distinct material ids of the defined objects, first-seen order
static std::vector<int> materialIds(Operation& sim)
{
    std::vector<int> ids;
    int count = objSlots(sim);   // 0x14005fa80
    for (int i = 0; i < count; ++i) {
        int id = objId(sim, i);   // 0x14005fb30
        if (id == -1 || id == -99) continue;
        ObjEntry* o = objEntry(sim, i);   // 0x140061250
        if (!o) continue;
        int nm = static_cast<int>(o->matIds.size());   // 0x14004db50
        for (int k = 0; k < nm; ++k) {
            int m = o->matIds[static_cast<size_t>(k)];   // 0x14004db80 (k is in range)
            bool seen = false;
            for (int x : ids)
                if (x == m) { seen = true; break; }
            if (!seen) ids.push_back(m);
        }
    }
    return ids;
}

// @src 0x140058e30
std::vector<String> materialIdStrings(Operation& sim)
{
    std::vector<String> out;
    for (int m : materialIds(sim)) out.push_back(qt::number(m));
    return out;
}

// @src 0x140134db0  (n is not read)
void emitMaterials(W& w, Project& prj, const String& db, int /*n*/, Operation* sim)
{
    String p;
    const ItemManager& mm = prj.materials;              // 0x1400a5930
    int cnt = static_cast<int>(mm.rows.size());         // vt+0x1b8 rowCount
    if (!sim || cnt == 0) return;
    std::vector<String> ids = materialIdStrings(*sim);
    for (const String& s : ids) {                       // Q_FOREACH over a copy
        int id = qt::toInt(s);
        p = itemKeyPathClean(mm, id);                   // vt+0x208
        if (!p.isEmpty()) {
            p = app::relToDb(db, p);
            w.ts << "KFREAD    \t" /*1421acc78*/ << 1 << endl;
            w.ts << p << endl;
        }
    }
}

namespace {
struct KeywordRecord {
    int id;            // +0x00
    const char* name;  // +0x08
};
// @data 0x1427c06e0
constexpr KeywordRecord kKeywords[] = {
#include "keyword_table.inc"
};
static_assert(sizeof kKeywords / sizeof kKeywords[0] == 0x130);
}  // namespace

// @src 0x140922160
int keywordIndex(int id)
{
    for (int i = 0; i <= 0x12f; ++i)
        if (id == kKeywords[i].id) return i;
    return -1;
}

// @src 0x140921f30
const char* keywordName(int id)
{
    int i = keywordIndex(id);
    if (i < 0) return "ERROR: KEYWORD NOT EXISTS!" /*1427c3fe0*/;
    return kKeywords[i].name;
}

}  // namespace m2m::mst
