// Operations of a .moproj (P02 rescue slice, coordinator 2026-10-22): Operation_List, ParseOperation,
// the factory, unique names, the SfOperationInfo ctor, objects and the accessors this path
// uses (NSTART / UserDefinedKey / RestartKey, blobs and Properties are P08's). s1-01 5, 9.3, 9.4, 10; decompiles
// s1-01 dc/open1-5.c, opr2.c, opr3.c, acc0.c, name0.c.
//
// The original's SfMoOprItem (item) and its SfOperationInfo (OPI = item+0x48) are one Operation
// here; Project::operations are the rows of the SfMoOprManager in document order.
#include "m2m/model.hpp"
#include "m2m/objects.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/p17.hpp"
#include "m2m/qt4path.hpp"
#include "operation_project_directories.hpp"

namespace m2m {
using qt::L;

// @src 0x14008cb50
bool parseOperationList(Project& p, const xml::Node& list, Ctx& ctx)
{
    int n = 0;
    for (const xml::Node* c : list.childElements()) {
        if (c->tag == L("Operation")) {
            n++;
            if (!parseOperation(p, *c, true, n, ctx)) return false;
        }
    }
    return true;
}

// @src 0x14008ccf0
bool parseOperation(Project& p, const xml::Node& e, bool withProps, int ordinal, Ctx& ctx)
{
    String lubMethod, name, type;
    int id = 0;
    int oprFlag = 1;
    Operation* item = nullptr;
    // A child that needs the item before <Type> uses an uninitialised pointer in the original;
    // here it is a parse error (s1-01 5.1).
    auto need = [&](const char* tag) {
        if (!item)
            throw Error(M2M_E_PARSE, std::string("Error on reading project file! <") + tag +
                                         "> before <Type> in operation " + name.utf8());
    };
    for (const xml::Node* c : e.childElements()) {
        const String& tag = c->tag;
        if (tag == L("Name")) {
            name = c->text();
        } else if (tag == L("Id")) {
            id = qt::toInt(c->text());
        } else if (tag == L("Type")) {
            type = c->text();
            String diagName = type;
            diagName += L("DiagItem");
            if (id == 0) id = ordinal;
            item = createOperation(p, name, type, diagName, id);
            std::vector<String> decisions;
            const SubProject* activeSub = activeSubProject(p);
            const std::optional<int> activeSubType = activeSub
                ? std::optional<int>(activeSub->type) : std::nullopt;
            detail::collectOperationProjectDirectoryDecisions(
                projectDir(p), id, activeSubType, decisions);
            for (const String& path : decisions)
                ctx.stagedOperationDirectoryDecisions.push_back(path.u16());
        } else if (tag == L("SubType")) {
            need("SubType");
            type = c->text();                         // the original reuses the Type string
            setSubType(*item, type);                  // 0x140050d50
        } else if (tag == L("OprFlag")) {
            oprFlag = qt::toInt(c->text());
        } else if (tag == L("Objects")) {
            need("Objects");
            parseObjects(p, *c, *item, ctx);
        } else if (tag == L("SimCtrlKey")) {
            need("SimCtrlKey");
            storeBlob(p, *c, simCtrlPath(*item), ctx);
        } else if (tag == L("InterObjectKey")) {
            need("InterObjectKey");
            storeBlob(p, *c, interObjectPath(*item), ctx);
        } else if (tag == L("Equipment")) {
            need("Equipment");
            item->equipmentId = qt::toInt(c->attribute(L("Id"), String()));   // 0x14004ca30
        } else if (tag == L("Lubricant")) {
            need("Lubricant");
            item->lubricantId = qt::toInt(c->attribute(L("Id"), String()));   // 0x1407b5170
            lubMethod = c->attribute(L("InterObjectMethod"), String());
            item->lubSystem = lubMethod == L("System") ? 1 : 0;               // 0x140065790 + 0x1400ab6f0
        } else if (tag == L("ActionKeywords")) {
            need("ActionKeywords");
            parseActionKeywords(p, *c, *item, ctx);
        } else if (tag == L("NSTART")) {
            need("NSTART");
            parseNstart(p, *c, *item);
        } else if (tag == L("UserDefinedKey")) {
            need("UserDefinedKey");
            parseUserDefinedKey(p, *c, *item);
        } else if (tag == L("RestartKey")) {
            need("RestartKey");
            parseRestartKey(p, *c, *item);
        } else if (tag == L("Properties")) {
            if (withProps) {
                need("Properties");
                parseProperties(p, *c, *item, ctx);
            }
        }
    }
    if (!item || !item->props) {
        // qCritical "Error on reading project file!", then "   " << item+0x50 (0x14004c900, the
        // diagName) << item->vt[0x48]() text: the open fails (message text only, not MST)
        std::string what = "Error on reading project file!";
        if (item) what += "\n   " + item->name.utf8() + item->diagName.utf8();
        else what += "\n   operation without <Type>: " + name.utf8();
        ctx.warn(what);
        return false;
    }
    setOprFlag(*item, oprFlag);                       // 0x140050930
    item->props->postLoad(p, ctx);                    // PROPS->vt[0xc0]
    return true;
}

namespace {

// The GUI start-up unit the unit-dependent props ctors read (writer.hpp, s1-01 Q2).
int guiUnit(const Project& p)
{
    (void)p;
    return guiStartupUnit();
}

}  // namespace

// @src 0x140047630
String uniqueName(const Project& p, const String& name)
{
    String cand = name;
    int i = 1;
    while (oprNameExists(p, cand)) {             // oprMgr vt+0x1e0 = 0x14006d040 (P07)
        const String fmt = name == L("Cycle") ? L("_%1") : L(" (%1)");
        cand = name + qt::arg(fmt, i + 1, 0, 10, u' ');
        i++;
    }
    return cand;
}

// @src 0x140047c00  registry lookup; qCritical "Error in operation manager" /
// "   Cannot get OprBase for <key>" when the key is missing
mst::OpClass* findClass(const Project& p, const String& classKey)
{
    auto it = p.registry.find(classKey);
    return it == p.registry.end() ? nullptr : it->second.get();
}

// @src 0x140046cc0  CreateOperation(oprMgr, name, type, diagName, id, src = 0)
Operation* createOperation(Project& p, const String& name, const String& type, const String& diagName, int id)
{
    std::unique_ptr<Operation> op = newOprItem(p, id);   // oprMgr vt+0x170 = 0x1400468e0 (P07)
    // a blank QStandardItem for column 0 (vtable 0x14210eb18): not modelled
    String displayName;
    String cls = type;
    String legacy;                                    // null
    if (qt::contains(type, L("SfOprHeatExpress")) || qt::contains(type, L("SfOprAirConvect")) ||
        qt::contains(type, L("SfOprHeatResting")) || qt::contains(type, L("SfOprHeatDwelling"))) {
        cls = qt::contains(type, L("2D")) ? L("SfOprHeatTransferExpress2D") : L("SfOprHeatTransferExpress");
        legacy = type;
    }
    if (type == L("SfOprMultiBlow")) cls = L("SfOprMultiBlow2D");
    mst::OpClass* ob = findClass(p, cls);
    if (ob) {
        displayName = ob->displayName();              // OB->vt[0x88]
        std::shared_ptr<Props> props = ob->makeProps(guiUnit(p));   // OB->vt[0x98]
        if (props) setOprProps(*op, props);           // 0x1400511f0: OPI+0x10, PROPS+0x20 = OPI
    }
    const String uname = uniqueName(p, name.isEmpty() ? displayName : name);
    // blank->setText(""), item->setText(uname), root->appendRow([blank, item])
    Operation* raw = op.get();
    p.operations.push_back(std::move(op));
    setOprName(*raw, uname);                          // 0x140050a90
    if (!(raw->classKey == cls)) raw->classKey = cls; // 0x140050c80 (its model clear: no model here;
                                                      // 0x140054f40(op, 0) never creates the 0x1400c4890 model)
    setSubType(*raw, legacy);                         // 0x140050d50
    raw->diagName = diagName;                         // 0x14004c9a0
    return raw;
}

// @src 0x14004fcd0  SfOperationInfo ctor (QObject base, QPointers and QStrings are the Operation's
// member initialisers)
void initOperation(Operation& op, Project& p, int id)
{
    initList(op.objIds);                              // +0x40 QList<int>
    initOwningList(op.objs);                          // 0x140066860 (+0x50)
    initOwningList(op.objpos);                        // 0x140066860 (+0x60)
    initInterObjectBlock(op);                         // 0x1400651f0 (+0x70, P09)
    operationDefaults(op);                            // 0x140050550
    op.project = &p;                                  // +0x100 = P
    setOperationId(op, id);                           // 0x140050820
}

// @src 0x140050550
void operationDefaults(Operation& op)
{
    op.f110 = 0;
    setOprFlag(op, 0);                                // 0x140050930
    resetOprInfo(op);                                 // 0x140051250 (P07)
    op.f30 = 0;                                       // 0x14005fa60
    resetNstart(op);                                  // 0x14005a370 (P08)
    op.userKeyGiven = 0;                              // +0x140
    op.restartGiven = 0;                              // +0x150
    op.restartKey = L("RestartStep.KEY");             // +0x158
    op.recoveryB = std::make_shared<std::vector<RecoveryEntry>>();   // +0x20 new list (0x1400653e0)
    op.recoveryFlags = std::make_shared<std::vector<bool>>();        // +0x28 new QList
    // +0x160 QPointer = 0: no model here
}

// @src 0x140050820
void setOperationId(Operation& op, int id)
{
    op.id = id;                                       // +0x108
    if (!hasNoId(op)) {                               // 0x140050a50
        String n = L("Operations/Task");
        n += qt::arg(L("%1"), id, 5, 10, u'0');       // QString::sprintf("%05d", id)
        setTaskDirRel(op, n);                         // 0x140051160
        // 0x140051190 makeTaskDir: mkpath of the task and project folders, not done
    }
}

// @src 0x140093c30
void parseObjects(Project& p, const xml::Node& objects, Operation& op, Ctx& ctx)
{
    int slot = 0;
    for (const xml::Node* c : objects.childElements()) {
        if (!(c->tag == L("Object"))) continue;
        const int id = qt::toInt(c->attribute(L("Id"), String()));
        setObjId(op, slot, id);                       // 0x14005fc80 (P12)
        if (ObjEntry* d = objEntry(op, slot)) {          // 0x140061250 (P12)
            const String oname = c->attribute(L("Name"), String());
            const int objType = qt::toInt(c->attribute(L("ObjectType"), String()));
            const bool hasGeo = qt::toInt(c->attribute(L("HasGeo"), String())) != 0;
            const bool hasMesh = qt::toInt(c->attribute(L("HasMesh"), String())) != 0;
            const int eqId = qt::toInt(c->attribute(L("EquipmentID"), String()));
            const String matId = c->attribute(L("MatId"), String());
            setObjName(*d, oname);                    // 0x14004d700
            d->objectType = objType;                  // 0x14004d580
            d->hasGeo = hasGeo;                       // 0x14004d780
            d->hasMesh = hasMesh;                     // 0x14004d7d0
            setObjEquipmentId(*d, eqId);              // 0x14004deb0
            d->matIds.clear();                        // 0x14004db20
            if (!matId.isEmpty())
                for (const String& tok : qt::split(matId, L(" "), qt::SplitBehavior::KeepEmptyParts)) {
                    const int m = qt::toInt(tok);     // 0x14004dc20: add unless present
                    bool have = false;
                    for (int v : d->matIds) have = have || v == m;
                    if (!have) d->matIds.push_back(m);
                }
            auto flag = [&](const char* a) { return qt::toInt(c->attribute(L(a), String())) != 0; };
            auto flagIfPresent = [&](const char* a) {
                return !c->attribute(L(a), String()).isEmpty() && qt::toInt(c->attribute(L(a), String())) != 0;
            };
            const bool remesh = flag("Remesh");
            const bool skipDefine = flag("SkipDefine");
            const bool meshSetting = flag("MeshSetting");
            const bool mesh2dAbs = flagIfPresent("Mesh2dAbsolute");
            const bool mesh3dFiner = flagIfPresent("Mesh3dFinerInternal");
            const bool defineBcc = flagIfPresent("DefineBCC");
            const bool assignMove = flag("AssignMovement");
            const bool defineRot = flag("DefineRotationMovement");
            const bool thermalStop = flag("ThermalStop");
            setObjRemesh(*d, remesh);                 // 0x14004df60
            setObjSkipDefine(*d, skipDefine);         // 0x14004df20
            setObjMeshSetting(*d, meshSetting);       // 0x14004dfa0
            setObjMesh2dAbsolute(*d, mesh2dAbs);      // 0x14004dfe0
            setObjMesh3dFinerInternal(*d, mesh3dFiner);   // 0x14004e020
            setObjDefineBCC(*d, defineBcc);           // 0x14004e0a0
            setObjAssignMovement(*d, assignMove);     // 0x14004e0e0
            setObjDefineRotationMovement(*d, defineRot);   // 0x14004e120
            setObjThermalStop(*d, thermalStop);       // 0x14004e160
            parseObjPropertiesKey(p, *c, op, slot, ctx);
        }
        slot++;
    }
}

}  // namespace m2m
