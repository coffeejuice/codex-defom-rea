// MST top level (packet P07). Spec: docs/re/moproj2mst/s1-02/spec.md sections 2-4.
// The original opens the .MST with QFile(WriteOnly) and writes through a QTextStream; here the
// native full Save binds w.ts to the already-open QFile-compatible device; other adapter
// modes retain memory output. The caller owns the open/close and result mapping.
#include "m2m/mst/emit.hpp"
#include "m2m/qt4path.hpp"

namespace m2m::mst {
namespace {

// 75-byte box lines: RULE75 = '*' 73x'-' '*', BOX75 = '*' 73x' ' '*' (s1-02 notation).
constexpr const char* kRule75 = "*-------------------------------------------------------------------------*";
constexpr const char* kBox75 = "*                                                                         *";

}  // namespace

// @src 0x1400a20d0
bool saveMst(W& w, Project& prj)
{
    const bool r = writeMainMst(w, prj);                   // 0x1401313c0
    SubProject* sub = activeSubProject(prj);               // 0x1400a7390
    if (sub && (sub->type == 2 || sub->type == 3)) {       // 0x140010330 / 0x140010360
        // 0x1400ba650 writes <DoeStudies>/<n>/Control/<name>.MST.TEMP (0x140131530) and
        // 0x1400ba800 creates the DOE folders. Both are file-system side effects of another file;
        // the main MST above does not depend on them (s1-02 O5). Only on request, and P17's.
        if (w.ctx.flags & M2M_FLAG_DOE_TEMP)
            throw Error(M2M_E_UNSUPPORTED, "the DOE .MST.TEMP form is not supported yet");
    }
    return r;                                              // only the main-MST result
}

// @src 0x1400a50a0
String mstPath(const Project& prj, bool full)
{
    const String base = full ? prj.path                   // 0x14008a4b0
                             : projectBaseName(prj);       // 0x14008a500
    return app::changeSuffix(base, qt::L(".MST"));         // 0x140818d50, ".MST" @0x142140abc
}

// @src 0x1401313c0
bool writeMainMst(W& w, Project& prj)
{
    if (prj.subs.size() < 1) return false;                 // 0x1400a7360
    // The original computes mstPath(prj, true) here and opens it; the caller owns the file.
    w.st.clearMasks();                                     // 0x140131390
    writeHeader(w, prj);                                   // 0x140131680
    writeMainBlock(w, prj);                                // 0x1401318b0
    walkOperations(w, prj, 0);                             // 0x140131df0
    writeTrailer(w, prj);                                  // 0x140131d10
    return true;
}

// @src 0x140131680
void writeHeader(W& w, Project& prj)
{
    Stream& ts = w.ts;
    ts << kRule75 << endl;                                                                 // 1421ac120
    ts << "*  MASTER FILE FOR DEFORM MUlTIPLE OPERATION SIMULATION                   *" << endl;  // 1421ac170
    ts << kBox75 << endl;                                                                  // 1421ac1c0
    ts << "*  Integrated 2D / 3D                                                     *" << endl;  // 1421ac210
    ts << kBox75 << endl;                                                                  // 1421ac260
    ts << "*!MASTERFORMAT 4                                                          *" << endl;  // 1421ac2b0
    ts << kRule75 << endl;                                                                 // 1421ac300
    ts << "*!TITLE  " << prj.title << endl;                // 1421ac350, title 0x140ceed20
    ts << "*!GEOTYP " << prj.geotyp << endl;               // 1421ac360, GEOTYP 0x140c0df50
    ts << "*!UNIT   " << prj.unit << endl;                 // 1421ac370, UNIT 0x1400a2180
    ts << "*" << endl;                                     // 1421a8ccc
    ts << kRule75 << endl;                                 // 1421ac380
    ts << endl;
}

// @src 0x1401318b0
void writeMainBlock(W& w, Project& prj)
{
    Stream& ts = w.ts;
    ts << endl;
    ts << kRule75 << endl;                                                                 // 1421ac3d0
    ts << kBox75 << endl;                                                                  // 1421ac420
    ts << "* Main Project                                                            *" << endl;  // 1421ac470
    ts << kBox75 << endl;                                                                  // 1421ac4c0
    ts << kRule75 << endl;                                                                 // 1421ac510
    ts << "*!MAINPRJ " << endl;                            // 1421ac560 (trailing space)
    ts << "*!PRJNAM " << projectBaseName(prj) << endl;     // 1421ac570, 0x14008a500
    ts << "*" << endl;                                     // 1421a8dc4
    ts << kRule75 << endl;                                 // 1421ac580
}

// @src 0x140131df0
void walkOperations(W& w, Project& prj, int subIdx)
{
    // 0x1400a74f0: subs[subIdx], null when out of range
    SubProject* sub = (subIdx >= 0 && subIdx < static_cast<int>(prj.subs.size()))
                          ? prj.subs[static_cast<size_t>(subIdx)].get()
                          : nullptr;
    if (!sub) return;
    doe::Model* doe = sub->doe;                                  // 0x1400b5a90
    const String dbPath = primaryDbPathClean(prj);         // 0x1400a7670 -> 0x1400a4ee0
    const std::vector<int> order = mstOrder(*sub);         // 0x1400b6630
    int counter = 0;                                       // SIMULATION number, shared by all writers
    std::map<int, int> doeMap;                             // QMap<int,int>
    if (doe)                                               // doe->v1c8(): DOE is P17's, no seam yet
        M2M_STUB(0x14091bdb0, "DefDoeModel object map (DOE MST)");
    for (size_t i = 0; i < order.size(); ++i) {
        const int id = order[i];
        Operation* item = operationById(prj, id);          // 0x1400a58b0 + 0x140047dc0
        if (!item) continue;
        OpClass* op = opBase(*item);                       // 0x140054e90
        if (!op) continue;
        const auto saved = op->current.snapshot();         // preserve nonowning identity across dispatch
        op->current = item;                                // 0x1405ef280
        if (!doe)
            dispatchOperation(w, prj, dbPath, &counter, item, 0, 0);               // 0x140132100
        else
            dispatchOperationDoe(w, prj, dbPath, doe, id, &counter, item, doeMap, 0, 0);   // 0x1401321a0
        op->current.restore(saved); // explicit normal-return point; no new exception restoration
    }
}

// @src 0x140132100
void dispatchOperation(W& w, Project& prj, const String& db, int* counter, Operation* sim,
                       int a6, int a7)
{
    OpClass* op = opBase(*sim);                            // 0x140054e90
    if (!op) return;
    w.st.doeMode = false;                                  // byte 0x143d80258
    op->writeMst(w, prj, db, counter, sim, a6, a7);        // vt+0x280
}

// @src 0x140131d10
void writeTrailer(W& w, Project& prj)
{
    (void)prj;
    Stream& ts = w.ts;
    ts << endl;
    ts << kRule75 << endl;                                                                 // 1421ac890
    ts << kBox75 << endl;                                                                  // 1421ac8e0
    ts << "* END OF MASTER FILE                                                      *" << endl;  // 1421ac930
    ts << kBox75 << endl;                                                                  // 1421ac980
    ts << kRule75 << endl;                                                                 // 1421ac9d0
    ts << endl;
}

}  // namespace m2m::mst
