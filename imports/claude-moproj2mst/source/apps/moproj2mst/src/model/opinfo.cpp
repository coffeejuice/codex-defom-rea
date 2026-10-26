// Operation info, operation item and project accessors (packet P07, slice from P02).
// Spec: s1-01 5.2, 5.3, 6.2, 9.3, 10; decompiles s1-01 dc/open3-5.c, opr0-3.c, acc0.c, name0-1.c,
// item0.c, kw0.c, mst0.c. The SfMoOprItem is folded into Operation (see model.hpp).
#include "m2m/model.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4path.hpp"

// The copy path ends in NotPorted stubs (src/stubs/p07.cpp) until its list helpers are decompiled;
// the code after them is the original's order and stays.
#pragma warning(disable : 4702)

namespace m2m {
namespace {

// "%1%2.KEY".arg(prefix, 0, ' ').arg(slot + 1, 5, 10, '0') appended to taskDirAbs + sep, cleaned
// (0x14005cf70, 0x14005da90, 0x14005eb10 share this body with their own prefix literal).
String numberedTaskFile(const Operation& op, const char* prefix, int slot)
{
    const String name = qt::arg(qt::arg(qt::L("%1%2.KEY"), qt::L(prefix), 0, u' '), slot + 1, 5, 10, u'0');
    return qt::cleanPath(taskDirAbs(op) + qt::kSep + name);
}

}  // namespace

// ---- OprManager and its items ---------------------------------------------------------------------

// @src 0x140045fe0
void initOprManager(Project& p)
{
    // QStandardItemModel base, +0x28 = 0, current id 0x140047af0(-1) (qDebug "nId < 1", +0x2c = -1),
    // +0x30 = 0, two columns, a QItemSelectionModel wired to slotCurrentChanged: no MST state but
    // the (empty) row list.
    p.operations.clear();
}

// @src 0x1400468e0
std::unique_ptr<Operation> newOprItem(Project& p, int id)
{
    auto item = std::make_unique<Operation>();     // operator new(0x58)
    initOprItem(*item, p, id, nullptr);            // 0x140045c30(item, mgr+0x20 = P, &id, 0)
    return item;
}

// @src 0x140046960
std::unique_ptr<Operation> newOprItem(Project& p, int id, const Operation* src)
{
    auto item = std::make_unique<Operation>();     // operator new(0x58)
    initOprItem(*item, p, id, src);                // 0x140045c30(item, P, &id, src)
    return item;
}

// @src 0x140045c30
void initOprItem(Operation& item, Project& p, int id, const Operation* src)
{
    // 0x14006b6a0: SfMoItem base (item+0x18 = id); +0x50 diagName = null; +0x40 = P. Its
    // 0x14006b7c0 sets the QStandardItem text "New item" (0x14006bb40) and +0x30 = 1: view only,
    // the name the MST uses is set later (0x140050a90)
    item.diagName = String();
    initOprItemData(item, p, id, src);             // 0x140045d10
}

// @src 0x140045d10
void initOprItemData(Operation& item, Project& p, int id, const Operation* src)
{
    // 0x14006b8e0(item, "SfMoOprItem"): item data role text, not modelled
    if (!src) {
        createOprInfo(item, p, id);                // 0x140045f10
    } else {
        initOperationFrom(item, p, id, *src);      // new(0x168) 0x14004fe20(P, id, src)
    }
}

// @src 0x140045f10
void createOprInfo(Operation& item, Project& p, int id)
{
    initOperation(item, p, id);                    // new(0x168) 0x14004fcd0(P, id) (P01)
}

// @src 0x14004fe20
void initOperationFrom(Operation& op, Project& p, int id, const Operation& src)
{
    // QObject base, vtable 0x1421247d8, member ctors (+0x38 QPointer, +0x40/+0x50/+0x60 lists,
    // +0x70 ICB 0x1400651f0, QStrings, +0x160 QPointer): the Operation's member initialisers.
    operationDefaults(op);                         // 0x140050550 (P01)
    copyOperation(op, src);                        // 0x1400500b0
    op.project = &p;                               // +0x100 = P
    setOperationId(op, id);                        // 0x140050820 (P01)
}

// @src 0x1400500b0
void copyOperation(Operation& op, const Operation& src)
{
    op.project = src.project;                      // +0x100
    setOperationId(op, src.id);                    // 0x140050820(src+0x108)
    op.f110 = src.f110;
    // +0x10 = new(0x38) 0x1400652e0(src+0x10): a fresh base SfOprBaseProperties (QObject without
    // parent, empty tree 0x1403536a0, owner null); src's props are not copied.
    op.props = std::make_shared<Props>();
    op.lubricantId = src.lubricantId;              // +0x48
    op.equipmentId = src.equipmentId;              // +0x4c
    op.tloc = src.tloc;                            // +0xe8
    op.f30 = 0;                                    // +0x30
    // +0x38 engine model QPointer = null (0x140cbc920): not modelled
    copyObjList(op, src);                          // +0x50 (P07 stub: paste path only)
    op.objIds = src.objIds;                        // +0x40, 0x140042500 (QList<int> assignment)
    copyObjposList(op, src);                       // +0x60 (P07 stub: paste path only)
    op.icb = src.icb;                              // the 0x78 bytes +0x70..+0xe7
    op.lubSystem = src.lubSystem;                  //   (+0xe0 is inside them)
    op.ff0 = src.ff0;
    op.ff8 = src.ff8;
    op.f110 = src.f110;
    op.name = src.name;
    op.classKey = src.classKey;
    op.subType = src.subType;
    op.f138 = src.f138;
    op.nstartGiven = src.nstartGiven;
    op.startStepNo = src.startStepNo;
    op.userKeyGiven = src.userKeyGiven;
    op.userKey = src.userKey;
    op.restartGiven = src.restartGiven;
    op.restartKey = src.restartKey;
    // +0x160 model QPointer = null: not modelled
}

// @src 0x140045e80
String oprItemText(const Operation& item)
{
    return item.name;                              // 0x140045f90 (item+0x48) -> 0x140050ac0
}

// @src 0x140047910
Operation* asOprItem(Operation* item)
{
    return item;                                   // __RTDynamicCast(item, SfMoItem, SfMoOprItem)
}

// @src 0x14004bd60
Operation* oprItemAt(const Project& p, int row)
{
    // QStandardItemModel::item(row, 1) (null outside the model), then the SfMoOprItem cast
    if (row < 0 || row >= static_cast<int>(p.operations.size())) return nullptr;
    return asOprItem(p.operations[static_cast<size_t>(row)].get());
}

// @src 0x14006d040
bool oprNameExists(const Project& p, const String& name)
{
    const int n = static_cast<int>(p.operations.size());     // vt+0x1b8 rowCount (0x14006cdd0)
    for (int i = 0; i < n; ++i) {
        const Operation* it = oprItemAt(p, i);                 // vt+0x1c0 (0x14006ce30)
        // item vt+0x48 (0x14006b9f0 -> 0x14006baf0: QStandardItem::text), QString ==
        if (oprItemText(*it) == name) return true;
    }
    return false;
}

// ---- SfOperationInfo fields -------------------------------------------------------------------------

// @src 0x140050930
void setOprFlag(Operation& op, int flag) { op.oprFlag = flag; }

// @src 0x1400509d0
bool oprFlagOff(const Operation& op) { return op.oprFlag == 0; }

// @src 0x140050a50
bool hasNoId(const Operation& op) { return op.id < 1; }   // 0x140050910

// @src 0x140050a90
void setOprName(Operation& op, const String& name) { op.name = name; }

// @src 0x140050d50
void setSubType(Operation& op, const String& subType)
{
    if (!(op.subType == subType)) op.subType = subType;
}

// @src 0x140051110
String taskDirRel(const Operation& op) { return op.taskDir; }

// @src 0x140051160
void setTaskDirRel(Operation& op, const String& dir) { op.taskDir = dir; }

// @src 0x140051190
void makeTaskDir(Operation& op)
{
    makePathWithMessage(taskDirAbs(op), true);     // 0x140050df0, 0x14081afb0
    // 0x14008c630(project): mkpath of the project folders (P03's; a side effect, not done)
}

// @src 0x1400511f0
void setOprProps(Operation& op, std::shared_ptr<Props> props)
{
    op.props = std::move(props);
    setPropsOwner(*op.props, &op);                 // 0x1405f8d60
}

// @src 0x140051250
void resetOprInfo(Operation& op)
{
    op.props = nullptr;
    op.lubricantId = -1;
    op.equipmentId = -1;
    op.tloc = 0.0;
    op.ff0 = 0;
    op.ff8 = 0;
}

// @src 0x1405f8d60
void setPropsOwner(Props& props, Operation* op) { props.owner = op; }

// ---- task folder and files --------------------------------------------------------------------------

// @src 0x140050df0
String taskDirAbs(const Operation& op)
{
    const Project* p = op.project;                 // 0x140055030
    if (!p) return String();                       // QString(QString::null)
    const String rel = taskDirRel(op);             // 0x140051110
    String s = projectDir(*p) + qt::kSep;          // 0x1400a4220, QDir::separator()
    s = s + rel;
    return qt::convertSeparators(qt::cleanPath(s));
}

// @src 0x140059290
String simCtrlPath(const Operation& op)
{
    return qt::cleanPath(taskDirAbs(op) + qt::kSep + qt::L("SimCtrl.KEY"));       // 0x142123360
}

// @src 0x14005a690
String interObjectPath(const Operation& op)
{
    return qt::cleanPath(taskDirAbs(op) + qt::kSep + qt::L("InterObject.KEY"));   // 0x142123410
}

// @src 0x14005cf70
String meshSettingsPath(const Operation& op, int slot)
{
    return numberedTaskFile(op, "MeshSettings", slot);     // 0x1421234d8
}

// @src 0x14005da90
String bccDefPath(const Operation& op, int slot)
{
    return numberedTaskFile(op, "BccDef", slot);           // 0x14212350c
}

// @src 0x14005eb10
String objPropertiesPath(const Operation& op, int slot)
{
    return numberedTaskFile(op, "ObjProperties", slot);    // 0x1421235d8
}

// @src 0x14081afb0
bool makePathWithMessage(const String& dir, bool message)
{
    // QDir().mkpath(dir); on failure with `message` a QMessageBox "Can not make the directory %1."
    // The converter creates no folders (plan 2.1, s1-01 11); the original returns true either way.
    (void)dir;
    (void)message;
    return true;
}

// ---- project, sub-project and flow ------------------------------------------------------------------

// @src 0x1400550d0
ItemManager* oprObjManager(Operation& op)
{
    if (!op.project) return nullptr;               // 0x140055030
    return &op.project->objects;                   // 0x1400a58f0 (M+0x19d0)
}

// @src 0x140055150
FlowChart* oprActiveFlow(const Operation& op)
{
    // 0x140055050: active sub-project of the op's project (null without a project)
    SubProject* sub = op.project ? activeSubProject(*op.project) : nullptr;
    if (!sub) return nullptr;
    return subFlow(*sub);                          // 0x1400b5a70
}

// @src 0x140050b20
int flowNumber(const Operation& op)
{
    SubProject* sub = op.project ? activeSubProject(*op.project) : nullptr;   // 0x140055050
    if (!sub) return 0;
    return subFlowNumber(*sub, op.id);             // 0x140050910, 0x1400b5f40
}

// @src 0x140050bc0
bool isChainHead(const Operation& op)
{
    const FlowChart* flow = oprActiveFlow(op);     // 0x140055150
    if (!flow) return false;
    return flowPredecessor(*flow, op.id) == -1;    // 0x140086720
}

// @src 0x1400a7760
int subProjectId(const Project& p, int i)
{
    // 0x1400a74f0: subs[i], null when out of range
    const SubProject* sub =
        (i >= 0 && i < static_cast<int>(p.subs.size())) ? p.subs[static_cast<size_t>(i)].get() : nullptr;
    if (!sub) return -1;
    return sub->id;                                // 0x1400b5c60
}

// @src 0x1400b5a70
FlowChart* subFlow(SubProject& sub) { return &sub.flow; }

// @src 0x1400b5f40
int subFlowNumber(SubProject& sub, int opId)
{
    FlowChart* flow = subFlow(sub);                // 0x1400b5a70 (never null here)
    if (!flow) return 0;
    return flowChainNumber(*flow, opId);           // 0x140085800
}

// ---- object items -----------------------------------------------------------------------------------

// @src 0x1400688f0
void initObjItem(Item& it, int id)
{
    // 0x14006b6a0: SfMoItem base with its id; vtable 0x142129148
    it.kind = ItemKind::Object;
    it.id = id;
    initObjItemData(it);                           // 0x1400689a0
}

// @src 0x1400689a0
void initObjItemData(Item& it)
{
    // 0x14006b8e0(it, "SfMoObjItem"): data role text, not modelled. 0x140068ad0: +0x40 =
    // new(0x1c8) OBJ 0x14004d160 (ctor defaults); the object fields of Item keep those defaults.
    (void)it;
}

}  // namespace m2m
