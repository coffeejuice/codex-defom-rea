// DOE emitters (owner: P09): output only while the DOE dispatch 0x1401321a0 has set doeMode.
// Spec: s1-05 7.3, s1-02 6.5; decompiles in s1-02 a1s.c (literals checked against the exe copy).
// The geometry block's .COR writer 0x140909a30 is NOT run (plan 2.1: the converter writes only the
// MST). DOE output stays M2M_E_UNSUPPORTED in stage 1 (P17).
#include "m2m/mst/emit.hpp"
#include "m2m/qt4path.hpp"

namespace m2m::mst {

namespace {

// QMap<int,int>::contains / value (0x1413c5630 / 0x140136f40: 0 when absent)
bool mapHas(const std::map<int, int>& m, int k) { return m.find(k) != m.end(); }
int mapValue(const std::map<int, int>& m, int k)
{
    auto it = m.find(k);
    return it == m.end() ? 0 : it->second;
}

// Qt 4.8 QString::lastIndexOf(QString, -1, Qt::CaseSensitive)
int lastIndexOf(const String& s, const String& what)
{
    auto pos = s.u16().rfind(what.u16());
    return pos == std::u16string::npos ? -1 : static_cast<int>(pos);
}

// Qt 4.8 QString::truncate(pos): resize when pos < size; a negative size empties the string
String truncated(const String& s, int pos)
{
    if (pos >= s.size()) return s;
    if (pos < 0) pos = 0;
    return String(s.u16().substr(0, static_cast<size_t>(pos)));
}

String corName(const String& p)
{
    String c = truncated(p, lastIndexOf(p, qt::L(".GEO")));
    return c + qt::L(".COR");
}

}  // namespace

// @src 0x14093a200
int doeLineCount(const String& s)
{
    return static_cast<int>(qt::split(s, qt::L("\n"), qt::SplitBehavior::SkipEmptyParts).size());
}

// @src 0x140132a40
void emitDoeGeometry(W& w, Operation* sim)
{
    const int opId = w.st.doeOpId;                           // 0x143d80268
    if (!w.st.doeMode) return;
    const doe::Model* doe = static_cast<const doe::Model*>(w.st.doeModel);
    std::vector<doe::Var*> gv;
    int count = objSlots(*sim);
    for (int k = 0; k < count; ++k) {
        int id = objId(*sim, k);
        if (id < 0) continue;
        gv = doe->geometryVars(opId, id);                    // vt+0x1b0
        if (!gv.empty()) {
            w.ts << "* DOE Geometry Variables " /*1421aca58*/ << endl;
            emitDoeGeometryBlock(w, sim, opId, id, gv, w.st.doeObjMap);
        }
    }
}

// @src 0x1401322d0
void emitDoeVariables(W& w, Operation* /*sim*/)
{
    Stream& ts = w.ts;
    State& st = w.st;
    if (!st.doeMode || st.doeVarList.empty()) return;
    ts << "* DOE Variables " /*1421aca20*/ << endl;
    std::vector<void*> vars = st.doeVarList;                 // implicitly shared copy
    for (void* pv : vars) {
        const doe::Var* v = static_cast<const doe::Var*>(pv);
        doe::ItemSnapshot items = doe::snapshotItems(*v);
        int id = v->index;
        int n;
        ts << endl;
        ts << "* DOE Study " /*1421aca38*/ << endl;
        const doe::Var* L = v->link;
        if (!L) {
            n = v->number;
        } else {
            id = L->index;
            n = L->number;
            ts << "* Link: " /*1421aca48*/ << v->number << "->" /*1421a90e4*/ << n;
            if (mapHas(st.doeObjMap, id)) ts << ":" /*1421a9204*/ << mapValue(st.doeObjMap, id);
            ts << endl;
        }
        String name = v->name;
        ts << "* (" /*1421a92c4*/ << n << ") " /*1421a9324*/ << name << endl;
        if (!st.doeObjMap.empty() && mapHas(st.doeObjMap, id)) n = mapValue(st.doeObjMap, id);
        for (const doe::ItemSnapshotEntry& it : items) {
            const auto text = doe::itemSnapshotText(it);
            String a = text.before, b = text.after;
            int lb = doeLineCount(b), la = doeLineCount(a);
            ts << "DOEVAR" /*1421aafc4*/ << "\t" /*1421a940c*/ << n + 1 << "\t" /*1421a996c*/ << la
               << "\t" /*1421a9adc*/ << lb << endl;
            ts << a << endl;
            ts << b << endl;
        }
    }
    ts << endl;
}

// @src 0x140132bd0
void emitDoeGeometryBlock(W& w, Operation* sim, int opId, int objId,
                          const std::vector<doe::Var*>& gv, const std::map<int, int>& map)
{
    Stream& ts = w.ts;
    String objName = objDisplayName(*sim, objId);            // 0x14005ff30
    ts << "* Geometry change : " /*1421aca78*/ << objName << endl;
    ts << "* DOE Study " /*1421aca90*/ << endl;
    int pos = slotOfObject(*sim, objId);                     // 0x14005fb80
    int objNo = slotNumber(*sim, pos);                       // 0x14005fc10
    for (const doe::Var* v : gv) {
        String name = v->name;
        ts << "* (" /*1421a9aec*/ << v->number << ") " /*1421a9ba4*/ << name << endl;
    }
    int mode = 2;
    for (const doe::Var* v : gv) {
        if (v->geoType == 100) mode = 0x66;
        if (const doe::Var* L = v->link) {
            int key = L->index;
            int ln = L->number;
            if (!map.empty() && mapHas(map, key)) ln = mapValue(map, key);
            ts << "* Link: " /*1421acaa0*/ << v->number << "->" /*1421a9bd4*/ << ln << endl;
        }
    }
    int nv = static_cast<int>(gv.size());
    ts << "DOEVAR" /*1421ab794*/ << "\t" /*1421a9c04*/ << "0" /*1421a9c34*/ << "\t" /*1421a9d94*/
       << nv * 5 + 3 << "\t" /*1421a9fac*/ << nv << "\t" /*1421aa63c*/ << "1" /*1421ab604*/ << endl;
    ts << "MORP" /*1421acaac*/ << "\t" /*1421ab6b4*/ << nv << "\t" /*1421aba4c*/ << mode << endl;
    for (const doe::Var* v : gv) {
        int id = v->link ? v->link->index : v->index;
        if (!map.empty() && mapHas(map, id)) id = mapValue(map, id);
        ts << id + 1 << endl;
    }
    const String ctrl = qt::L("../../Control/" /*1421acab8*/);
    String f0;
    if (!gv[0]->files.empty()) f0 = gv[0]->files[0].path;    // 0x140817a80(gv[0], 0, &f0)
    f0 = ctrl + qt::fileName(f0);                            // 0x140818f10
    ts << f0 << endl;
    for (const doe::Var* v : gv) {
        String a, b;
        if (!v->files.empty()) {                             // 0x140817940(v, 0, &a, &b)
            a = v->files[0].before;
            b = v->files[0].after;
        }
        String A = ctrl + qt::fileName(a);
        String B = ctrl + qt::fileName(b);
        String aCor, bCor;
        bool useA = !v->files.empty() && v->files[0].useBefore && !v->files[0].before.isEmpty();
        if (!useA) {                                         // 0x140817e20
            ts << "NA" /*1421abffc*/ << endl;
            ts << "NA" /*1421ac16c*/ << endl;
        } else {
            aCor = corName(a);                               // ".GEO" 1421acac8, ".COR" 1421acad0
            String c = corName(ctrl + qt::fileName(a));      // 1421acad8, 1421acae0
            ts << A << endl;
            ts << c << endl;
        }
        bool useB = !v->files.empty() && v->files[0].useAfter && !v->files[0].after.isEmpty();
        if (!useB) {                                         // 0x140817eb0
            ts << "NA" /*1421ac1bc*/ << endl;
            ts << "NA" /*1421ac20c*/ << endl;
        } else {
            bCor = corName(b);                               // 1421acae8, 1421acaf0
            String c = corName(ctrl + qt::fileName(b));      // 1421acaf8, 1421acb00
            ts << B << endl;
            ts << c << endl;
        }
        // 0x140909a30(aCor, bCor, v) writes the .COR side files: not run.
        (void)aCor;
        (void)bCor;
    }
    String g = qt::arg(qt::arg(qt::L("OutputGeoOp%1Obj%2.GEO" /*1421acb08*/), opId, 0, 10, u' '),
                       objId, 0, 10, u' ');
    ts << g << endl;
    ts << endl;
    ts << "GFREAD" /*1421acb20*/ << "\t" /*1421ac25c*/ << objNo << endl;
    ts << g << endl;
    ObjEntry* o = objEntryById(*sim, objId);                 // 0x140061310
    if (o && o->hasMesh /*0x14004d800*/) {
        ts << endl;
        ts << "DEFAMG" /*1421acb28*/ << "\t" /*1421ac2ac*/ << objNo << endl;
        ts << endl;
        ts << "GENBCC" /*1421acb30*/ << "\t" /*1421ac2fc*/ << objNo << endl;
        ts << endl;
    }
}

}  // namespace m2m::mst

// ---- DOE model queries (decompiles in gui_mo/out_p09/d2.c, d3.c) ----------------------------------
namespace m2m::doe {

// @src 0x14091dbb0
bool Var::appliesTo(int opId) const
{
    for (auto it = ops.rbegin(); it != ops.rend(); ++it)   // 0x1400251d0: from the end
        if (*it == opId) return true;
    return false;
}

// @src 0x1400d7f00
int Model::count() const { return static_cast<int>(vars.size()); }

// @src 0x14091b950
Var* Model::varAt(int i) const
{
    Var* v = nullptr;
    if (-1 < i && i < count()) v = vars[static_cast<size_t>(i)];
    return v;
}

// @src 0x14091b9b0
std::vector<Var*> Model::allVars()
{
    std::vector<Var*> l = vars;
    for (int i = 0; i < static_cast<int>(l.size()); ++i) l[static_cast<size_t>(i)]->index = i;
    return vars;
}

// @src 0x14091bbb0
std::vector<Var*> Model::varsForOperation(int opId)
{
    std::vector<Var*> out;
    std::vector<Var*> l = vars;
    for (int i = 0; i < static_cast<int>(l.size()); ++i) {
        l[static_cast<size_t>(i)]->index = i;
        if (l[static_cast<size_t>(i)]->appliesTo(opId)) out.push_back(l[static_cast<size_t>(i)]);
    }
    return out;
}

// @src 0x14091bcb0
std::vector<Var*> Model::unlinkedVars()
{
    std::vector<Var*> out;
    std::vector<Var*> l = vars;
    for (int i = 0; i < static_cast<int>(l.size()); ++i) {
        l[static_cast<size_t>(i)]->index = i;
        if (!l[static_cast<size_t>(i)]->link) out.push_back(l[static_cast<size_t>(i)]);
    }
    return out;
}

// @src 0x14091bdb0
std::map<int, int> Model::linkMap()
{
    std::map<int, int> m;
    std::vector<Var*> unlinked = unlinkedVars();          // vt+0x1c0
    std::vector<Var*> all = allVars();                    // vt+0x1a8
    for (Var* v : all) {
        int key = v->index;
        int val = v->link ? v->link->index : key;
        m[key] = val;
    }
    std::vector<int> keys;                                // 0x14091de30: QMap::keys(), ascending
    for (const auto& kv : m) keys.push_back(kv.first);
    for (int i = 0; i < static_cast<int>(m.size()); ++i) {
        int k = keys[static_cast<size_t>(i)];
        Var* target = varAt(m[k]);                        // vt+0x1a0 of QMap::value
        int pos = -1;                                     // 0x140088be0: QList::indexOf(target, 0)
        for (int j = 0; j < static_cast<int>(unlinked.size()); ++j)
            if (unlinked[static_cast<size_t>(j)] == target) {
                pos = j;
                break;
            }
        m[k] = pos;
    }
    return m;
}

}  // namespace m2m::doe
