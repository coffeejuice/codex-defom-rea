// m2m/p17.hpp: cross-class helpers of packet P17 beyond the G0 seams (owner: P17).
// Units (0x1402ede00 and its table 0x143d76368), sub-project DB paths, the DOE .MST.TEMP path.
// Spec: docs/re/moproj2mst/p17/spec.md.
#pragma once

#include <memory>

#include "m2m/model.hpp"
#include "m2m/props.hpp"

namespace m2m::units {
using qt::String;

// Kind 0x2b is not in the table: a plain number (no unit text, no conversion).
inline constexpr int kPlainNumber = 0x2b;

// @src 0x140917de0  unit name of a kind in a unit system: 1 = SI, anything else = English.
// Kinds 0..0x2b only (the original indexes a 0x28-byte table without a bound check).
const char* unitName(int kind, int unitSys);
// @src 0x1402ed330  index of a unit text in a category, case-insensitive; -1 if none or no category
int unitIndex(int kind, const String& unit);
// @src 0x1402ed5c0  value in unit `from` -> unit `to` of the same category; false (out untouched)
// when the category or a unit is unknown. Kind 0x2b: out = v, false.
bool convert(int kind, const String& from, double v, const String& to, double* out);
// @src 0x1402edb70  parse "<number> [unit]" (split on ' ', empty parts removed). *out = 0 first.
// `unit` receives the unit text when it names a unit of the category.
bool parse(int kind, const String& text, double* out, String* unit);
// @src 0x1402ed750  value in `unit` -> the unit of the unit system (0x140917de0)
bool toSystem(int kind, int unitSys, const String& unit, double v, double* out);
// @src 0x1402ede00  evaluator(table, kind, unitSys, text, &out, &unit): parse, convert to the
// unit system; then `unit` = the system's unit name. The result is the conversion's result; *out
// holds the parsed number when the conversion fails, 0 when the parse fails.
bool evalUnit(int kind, int unitSys, const String& text, double* out, String* unit);

}  // namespace m2m::units

namespace m2m {

// @src 0x1403b5580  the GUI unit global 0x143d79724 that the project ctor 0x140089880 gives P+0x20
// before <Unit> is read (s1-01 Q2). It lives in .bss and is read from no registry, ini file or
// command line: 0 at process start. Known UI writers include New Project (0x14001c110,
// 0x140127960) and the combo path1403b5f50 via1403b5560. This returns the fresh-session
// profile unless the C ABI explicitly overrides it for one conversion.
int guiStartupUnit();
class GuiStartupUnitScope final {
public:
    explicit GuiStartupUnitScope(int overrideUnit);
    ~GuiStartupUnitScope();
    GuiStartupUnitScope(const GuiStartupUnitScope&) = delete;
    GuiStartupUnitScope& operator=(const GuiStartupUnitScope&) = delete;
private:
    int previous_;
};

}  // namespace m2m

namespace m2m {

// @src 0x1400a4f60  changeSuffix(full ? path : completeBaseName(path), ".DB")
String projectDbPath(const Project& p, bool full);
// @src 0x1400bc160  cleanPath(projectDir + sep + "DieStress"), null without a project
String dieStressDir(const SubProject& sub);
// @src 0x1400bc290  first "DieStress%05d.DB" (from 1) no sub-project of the project uses as DbName
String dieStressDbName(const SubProject& sub);
// @src 0x1400bbee0  die-stress DB: DbName (generated and stored when empty), with the folder if asked
String dieStressDbPath(SubProject& sub, bool withDir);
// @src 0x1400b9df0  projectDbPath(project, withDir), null without a project
String subProjectProjectDbPath(const SubProject& sub, bool withDir);
// @src 0x1400b9e80  type 4 (die stress) ? 0x1400bbee0 : 0x1400b9df0
String subProjectDbPath(SubProject& sub, bool withDir);

// Default property trees, OB->vt[0x98] (src/model/classprops.cpp); guiUnit picks the default texts
std::shared_ptr<Props> formingProps(bool is2D, int guiUnit);   // SfOprForming(2D)
std::shared_ptr<Props> preProps(int guiUnit);                   // SfOprPre2D / SfOprPre3D
std::shared_ptr<Props> htFurnaceProps(int guiUnit);             // SfOprHTFurnace
std::shared_ptr<Props> shapeOptimProps(int guiUnit = 1);                  // SfOprShapeOptim
std::shared_ptr<Props> reportProps(int guiUnit);                // SfOprReport
namespace base {
// @src 0x1405fa2c0  HeatingTime under parent (TestGroup when null); value -> CommonBlock heatingTime
void addHeatingTime(Props& p, PropNode* parent, const qt::String& value, const qt::String& label);
}

// @src 0x140050d00  *!SIMTYP text: the class key at OPI+0x120 as stored by 0x140050c80, i.e. the
// registry name after the legacy mapping (s1-01 Q8, s1-03 Q1)
String simTypeName(const Operation& op);

}  // namespace m2m

namespace m2m::mst {

class OpClass;
// OB->vt[0x180]() class init (src/model/classinit.cpp): the dimension flags (info+0x80) of the
// base-writer family, Cogging and ShapeOptim; the registry insert 0x14006f940 calls it
void classInit(OpClass& ob);

// ---- DOE .MST.TEMP path (s1-05 7.1, s1-02 2.4); side files are never written ----------------------
// @src 0x1400ba000  cleanPath(projectDir + sep + "DoeStudies" + sep + (doe ? doe->name : "DoeStudy" + id))
// The DOE study name (0x1400ab770) is passed in: P09 owns the DOE model.
String doeStudyDir(const SubProject& sub, const String* doeName);
// @src 0x1400ba350  cleanPath(doeStudyDir + sep + "Control")
String doeControlDir(const SubProject& sub, const String* doeName);
// @src 0x1400a51e0  appendSuffix(completeBaseName(path), ".MST.TEMP")
String mstTempName(const Project& prj);
// @src 0x1400b5c80  index in project->subs of the sub-project with this id, -1 without a project
int subProjectIndex(const SubProject& sub);
// @src 0x1400ba650 (path part)  cleanPath(doeControlDir + sep + mstTempName); null when the
// original writes no .MST.TEMP (no project, type 0 or 4)
String doeTempPath(const SubProject& sub, const String* doeName);

}  // namespace m2m::mst
