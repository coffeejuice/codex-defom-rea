// P17: DB paths of the project and of sub-projects, *!SIMTYP source (spec: docs/re/moproj2mst/p17/spec.md 2-3).
#include <cstdio>

#include "m2m/model.hpp"
#include "m2m/mst/emit.hpp"
#include "m2m/p17.hpp"
#include "m2m/qt4path.hpp"

namespace m2m {

// @src 0x1400a4f60
String projectDbPath(const Project& p, bool full)
{
    String base = full ? p.path                  // 0x14008a4b0
                       : projectBaseName(p);     // 0x14008a500
    return app::changeSuffix(base, String::latin1(".DB"));   // 0x140818d50, ".DB" @0x1421408bc
}

// G0 seam: the callers that reach it (0x1400b9df0 from Diestress/DiestressBM) pass full = true.
String projectDbPath(const Project& p)
{
    return projectDbPath(p, true);
}

// @src 0x1400bc160
String dieStressDir(const SubProject& sub)
{
    if (!sub.project) return String();
    String dir = projectDir(*sub.project);       // 0x1400a4220
    String s = dir + qt::kSep + String::latin1("DieStress");   // @0x14214af98
    return qt::cleanPath(s);
}

// @src 0x1400bc290
String dieStressDbName(const SubProject& sub)
{
    String prefix = String::latin1("DieStress");  // @0x14214afa8
    String s;
    const Project* prj = sub.project;
    if (!prj) return String();
    int i = 0;
    bool used = false;
    do {
        ++i;
        char buf[32];
        std::snprintf(buf, sizeof buf, "%05d.DB", i);   // QString::sprintf, "%05d.DB" @0x14214afb8
        s = prefix + String::latin1(buf);
        used = false;
        int n = static_cast<int>(prj->subs.size());     // 0x1400a7360
        for (int k = 0; k < n; ++k) {
            const SubProject* o = prj->subs[static_cast<size_t>(k)].get();   // 0x1400a74f0
            if (o && o->dbName == s) {
                used = true;
                break;
            }
        }
    } while (used);
    return s;
}

// @src 0x1400bbee0
String dieStressDbPath(SubProject& sub, bool withDir)
{
    if (sub.dbName.isEmpty()) sub.dbName = dieStressDbName(sub);
    String s;
    if (!withDir) s = sub.dbName;
    else s = dieStressDir(sub) + qt::kSep + sub.dbName;
    return qt::cleanPath(s);
}

// @src 0x1400b9df0
String subProjectProjectDbPath(const SubProject& sub, bool withDir)
{
    if (!sub.project) return String();
    return projectDbPath(*sub.project, withDir);
}

// @src 0x1400b9e80
String subProjectDbPath(SubProject& sub, bool withDir)
{
    if (sub.type == 4) return dieStressDbPath(sub, withDir);
    return subProjectProjectDbPath(sub, withDir);
}

// G0 seam (const): the same path. When DbName is empty the original stores the generated name in
// the sub-project; the name depends only on the other sub-projects, so a later call gives the same.
String subProjectDbPath(const Project&, const SubProject& sub, bool full)
{
    if (sub.type == 4) {
        String name = sub.dbName.isEmpty() ? dieStressDbName(sub) : sub.dbName;
        return qt::cleanPath(full ? dieStressDir(sub) + qt::kSep + name : name);
    }
    return subProjectProjectDbPath(sub, full);
}

// @src 0x1400a5970 -> vt+0x208  equipment manager (M+0x19e0, created on demand by 0x140044e60), then
// the manager's KEY path of the id: 0x14006d380 (P08, itemKeyPathClean)
String equipmentKeyPath(const Project& prj, int id)
{
    return mst::itemKeyPathClean(prj.equipment, id);
}

// @src 0x140050d00
String simTypeName(const Operation& op)
{
    return op.classKey;
}

}  // namespace m2m
