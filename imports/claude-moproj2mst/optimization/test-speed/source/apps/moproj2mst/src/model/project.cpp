// P03: the project record P (s1-01 1.1, 9.1, 10) and its simple accessors.
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"   // complete OpClass for Project's registry
#include "m2m/qt4path.hpp"

namespace m2m {
using qt::L;

namespace {

// Start-up value of the global 0x143d79724 the ctor copies into P+0x20 (s1-01 Q2): 0 in a fresh
// GUI (found by P17). Every corpus project has <Unit>, which the walk applies afterwards (0x14008b410).
constexpr int kStartupUnit = 0;

// @src 0x1400a4cc0  default DB path: P+0x30 = cleanPath(changeSuffix(P+0x18, ".DB")); the call
// that hands it to the data root (0x140cdcdc0) has no MST effect.
void setDefaultDbPath(Project& p)
{
    p.primaryDb = qt::cleanPath(app::changeSuffix(p.path, L(".DB")));
}

// @src 0x14008a470  set P+0x18, then the default DB path
void setPathAndDb(Project& p, const String& path)
{
    p.path = path;
    setDefaultDbPath(p);
}

}  // namespace

// @src 0x140089880
void initProject(Project& p, const String& dir, const String& base)
{
    p.tokMode = 0;                                    // 0x140906740
    p.f10 = 5;
    String path;                                      // null
    if (!dir.isEmpty() && !base.isEmpty())
        path = qt::cleanPath(dir + qt::kSep + base);
    if (!path.isEmpty()) {
        setPathAndDb(p, path);
        p.title = base;
        p.f39 = 1;                                    // 0x1400a4150
    } else {
        // "NewProject" path (0x14008a550): only for a project without a DB name, which the
        // converter never creates (the data root always gets changeSuffix(file, ".DB")).
        not_ported(0x14008a550, "new project path");
    }
    p.geotyp = 0;
    setUnit(p, kStartupUnit);                         // 0x1400a2150 with the start-up global
    p.f3a = 0;                                        // 0x1400a41f0
    p.f38 = 1;
    p.f3c = 5;
    p.f40 = 0;
    // P+0x48: the "ProjectFile" skeleton DOM is replaced by setContent at open; not kept.
    // P+0x60 (0x1412f10d0): the class-name registry that only the post-open 0x1400aa960 fills
    // (out of scope); not modelled. The project is the data root M+0x19c0 (0x140044a90).
    p.activeSub = 0;
    p.loadingSub = false;
    p.openedOk = false;
    p.subs.clear();
    p.subs.push_back(makeSubProject(p, 0, 0));
    // The skeleton's first sub-project element (0x140099040) belongs to the replaced DOM too.
}

// @src 0x1400a7390  subs[activeSub] through 0x1400a74f0 (null when out of range)
SubProject* activeSubProject(const Project& p)
{
    const int i = p.activeSub;                        // 0x1400a73c0
    if (i < 0 || i >= static_cast<int>(p.subs.size())) return nullptr;   // 0x1400a7360
    return p.subs[static_cast<size_t>(i)].get();
}

// @src 0x1400a4220
String projectDir(const Project& p)
{
    return qt::cleanPath(qt::absolutePath(p.path));
}

// @src 0x14008a500  0x140819090 = QFileInfo(P+0x18).completeBaseName()
String projectBaseName(const Project& p)
{
    return qt::completeBaseName(p.path);
}

// @src 0x1400a4ee0
String primaryDbPath(const Project& p)
{
    return qt::cleanPath(p.primaryDb);
}

// @src 0x1400a7670
String primaryDbPathClean(const Project& p)
{
    return qt::cleanPath(primaryDbPath(p));
}

// @src 0x140047dc0  0x140047950 finds the OprManager row whose id matches; 0x140045f90 = its OPI.
// The manager's rows are the operations in creation (document) order.
Operation* operationById(const Project& p, int id)
{
    for (const auto& op : p.operations)
        if (op && op->id == id) return op.get();
    return nullptr;
}

}  // namespace m2m
