// P17: the DOE .MST.TEMP path (s1-02 2.4, 3.3, 4.4; s1-05 7). Only reached with M2M_FLAG_DOE_TEMP;
// none of the original's side files (DEF_DOE_PARSER.INI, .COR, DoeStudies folders) is written.
#include "m2m/mst/emit.hpp"
#include "m2m/p17.hpp"
#include "m2m/qt4path.hpp"

namespace m2m::mst {

// @src 0x1401321a0
void dispatchOperationDoe(W& w, Project& prj, const String& db, doe::Model* doe, int opId, int* counter,
                          Operation* sim, const std::map<int, int>& doeMap, int a9, int a10)
{
    OpClass* op = opBase(*sim);                  // 0x140054e90
    if (!op) return;
    w.st.doeMode = true;                         // 0x143d80258
    {                                            // 0x143d80270 = doe->v1b8 (0x14091bbb0, P09)
        const std::vector<doe::Var*> v = doe->varsForOperation(opId);
        w.st.doeVarList.assign(v.begin(), v.end());
    }
    w.st.doeOpId = opId;                         // 0x143d80268
    w.st.doeModel = doe;                         // 0x143d80260
    w.st.doeObjMap = doeMap;                     // 0x143d80298
    op->writeMst(w, prj, db, counter, sim, a9, a10);   // vt+0x280
}

// @src 0x140131a50
void writeSubBlock(W& w, Project& prj, int subIdx)
{
    Stream& ts = w.ts;
    ts << endl;
    ts << "*-------------------------------------------------------------------------*" << endl;   // 0x1421ac5d0
    ts << "*                                                                         *" << endl;   // 0x1421ac620
    ts << "* Sub-Project                                                             *" << endl;   // 0x1421ac670
    ts << "*                                                                         *" << endl;   // 0x1421ac6c0
    ts << "*-------------------------------------------------------------------------*" << endl;   // 0x1421ac710
    ts << "*!SUBPRJ " << (subIdx + 1) << endl;                                                       // 0x1421ac760
    String name = subProjectName(prj, subIdx);   // 0x1400a75a0
    ts << "*!PRJNAM " << name << endl;                                                               // 0x1421ac770
    ts << "*" << endl;                                                                               // 0x1421a8e8c
    ts << "*-------------------------------------------------------------------------*" << endl;   // 0x1421ac780
}

// @src 0x140131530  The original opens the given path (QFile WriteOnly) and returns false when it
// cannot; here the caller owns the file, as for writeMainMst.
bool writeSubMst(W& w, Project& prj, int subIdx)
{
    if (prj.subs.size() < 1) return false;       // 0x1400a7360
    w.st.clearMasks();                           // 0x140131390
    writeHeader(w, prj);                         // 0x140131680
    writeSubBlock(w, prj, subIdx);               // 0x140131a50
    walkOperations(w, prj, subIdx);              // 0x140131df0
    writeTrailer(w, prj);                        // 0x140131d10
    return true;
}

// @src 0x1400ba000
String doeStudyDir(const SubProject& sub, const String* doeName)
{
    const Project* prj = sub.project;            // 0x1400b5c40
    if (!prj) return String();
    String dir = projectDir(*prj);               // 0x1400a4220
    String study;
    if (!sub.doe || !doeName)                    // 0x1400b5a90
        study = String::latin1("DoeStudy") + qt::number(sub.id);   // @0x14214add8, 0x1400b5c60
    else
        study = *doeName;                        // 0x1400ab770 (doe+0x98)
    String s = dir + qt::kSep + String::latin1("DoeStudies") + qt::kSep + study;   // @0x14214ade8
    return qt::cleanPath(s);
}

// @src 0x1400ba350
String doeControlDir(const SubProject& sub, const String* doeName)
{
    String s = doeStudyDir(sub, doeName) + qt::kSep + String::latin1("Control");   // @0x14214adf8
    return qt::cleanPath(s);
}

// @src 0x1400a51e0
String mstTempName(const Project& prj)
{
    return app::appendSuffix(projectBaseName(prj), String::latin1(".MST.TEMP"));   // @0x142140ac8
}

// @src 0x1400b5c80 -> 0x1400a77b0(project, id): id 0 is index 0 without a search; otherwise the
// first index >= 1 whose sub-project has the id, -1 if none
int subProjectIndex(const SubProject& sub)
{
    if (!sub.project) return -1;
    const int id = sub.id;                       // 0x1400b5c60 (+0x1c)
    if (id == 0) return 0;
    const auto& subs = sub.project->subs;
    const int n = static_cast<int>(subs.size()); // 0x1400a7360
    for (int k = 1; k < n; ++k) {
        const SubProject* s = subs[static_cast<size_t>(k)].get();
        if (s && s->id == id) return k;
    }
    return -1;
}

// @src 0x1400ba650 (path part; 0x140131530 writes it, 0x140915eb0 then writes DEF_DOE_PARSER.INI)
String doeTempPath(const SubProject& sub, const String* doeName)
{
    if (!sub.project || sub.type == 0 || sub.type == 4) return String();
    String ctrl = doeControlDir(sub, doeName);
    String name = mstTempName(*sub.project);
    return qt::cleanPath(ctrl + qt::kSep + name);
}

}  // namespace m2m::mst
