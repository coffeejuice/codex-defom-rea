// .moproj open path: file read, DOM walk, item lists (packet P01, s1-01 1-4).
//
// Side effects of the original that the DLL does not repeat (s1-01 11): the FOR003 rename offer
// (0x1400a5290), the UI refresh in 0x14008a9b0 (its data-root DB open is engine::openProjectDb), mkpath and file
// writes of inline MoItem <Data> (native in full Save, overlay in recording mode), and the engine unit globals
// set by 0x1400a2150.
#include <filesystem>
#include <fstream>
#include <iterator>

#include "m2m/blob.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/longpath.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/file_device.hpp"
#include "m2m/native_directories.hpp"
#include "m2m/qt4path.hpp"
#include "m2m/xml.hpp"

namespace m2m::mst {
void installLoaderSlots();   // src/engine/class_slots.cpp
}

namespace m2m {
using qt::L;

// @src 0x14008a9b0
bool openProject(Project& p, const String& file, Ctx& ctx)
{
    p.openedOk = false;                               // P+0x55 (+0x56, +0x57 not modelled)
    // processEvents / busy cursor: UI
    if (app::endsWithSuffix(file, L("moproj")))
        p.path = qt::cleanPath(file);
    else if (app::endsWithSuffix(file, L(".DB")))
        p.path = qt::cleanPath(app::changeSuffix(file, L("moproj")));
    else if (app::endsWithSuffix(file, L(".KEY")))
        p.path = qt::cleanPath(app::changeSuffix(file, L("moproj")));
    else
        return false;
    // 0x1400a4150 / 0x1400a41f0 precede DB and XML loading even when the
    // subsequent reader returns false.
    p.f39 = 1;
    p.f3a = 1;
    // 0x1400a5290 FOR003 rename offer: renames a project file, not done here
    // data root P+8: DB path cleanPath(P+0x30) (0x140cdcdc0) and open (0x140cdc400 -> 0x140cdc480),
    // before the walk (MDT DB spec 1.3); the UI refresh is not modelled
    engine::openProjectDb(p, ctx);
    if (app::endsWithSuffix(file, L("moproj")))
        p.openedOk = readProjectFile(p, ctx);
    else
        // 0x14008b2d0 (.DB) / 0x14008b380 (.KEY) were not followed in s1-01.
        throw Error(M2M_E_UNSUPPORTED, "opening a project from a .DB or .KEY file is not supported");
    qt::setCurrent(projectDir(p));                    // QDir::setCurrent (per thread here, P05)
    // 0x1400aa960 feature-cache registration remains a separate recovery gap.
    // Original08af3f..61 then loads the process-owned remesh state and assigns
    // enabled before2ca7a0(false)'s cursor restoration/processEvents boundary.
    if (ctx.session) {
        auto remeshIo = engine::simcontrol300::makeNativeRemeshIo(qt::currentPath().utf8());
        ctx.session->loadProjectRemesh(*remeshIo);
    }
    // 08a9b0 tail: the accepted-suffix result is independent of P+55. The outer
    // native Open keeps the Project and selects its registered model; Save
    // subsequently inspects P+55. Do not make XML failure destroy the Project.
    return true;
}

// @src 0x14008afa0
bool readProjectFile(Project& p, Ctx& ctx)
{
    (void)ctx;
    // The GUI's engine exists before a project opens: the props post-loads of the walk reach the
    // operation model (SfOprDistortionProperties 0x1413680e0 -> 0x14005f770), so the slot table
    // must be set first (MDT L5S)
    mst::installLoaderSlots();
    const std::u16string& u = p.path.u16();
    std::ifstream f(std::filesystem::path(win32Path(u)), std::ios::binary);   // long paths: m2m/longpath.hpp
    if (!f) return false;   // qDebug "Error: Cannot read file <path>: <errorString>"
    const std::string bytes((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    f.close();
    auto doc = std::make_shared<xml::Document>();
    String err;
    int line = 0, col = 0;
    p.dom = doc;                                      // P+0x48 keeps the parsed document
    if (!xml::setContent(*doc, bytes, &err, &line, &col)) return false;
    return walkProject(p, *doc, ctx);
}

// @src 0x14008b410
bool walkProject(Project& p, const xml::Document& doc, Ctx& ctx)
{
    String tmp;
    int nSub = 0;
    std::vector<int> subType, subId;
    if (doc.root) {
        for (const xml::Node* e : doc.root->childElements()) {
            const String& tag = e->tag;
            if (tag == L("Title")) {
                p.title = e->text();
            } else if (tag == L("GeoTyp")) {
                tmp = e->text();
                p.geotyp = qt::toInt(tmp);
            } else if (tag == L("Unit")) {
                tmp = e->attribute(L("Name"), String());
                setUnit(p, tmp == L("SI") ? 1 : 2);
            } else if (tag == L("Primary_DB_Name")) {
                tmp = e->attribute(L("Name"), String());
                setPrimaryDb(p, tmp);
            } else if (tag == L("Sub_Project")) {
                subType.push_back(qt::toInt(e->attribute(L("Type"), String())));
                int id = qt::toInt(e->attribute(L("Id"), qt::number(-1)));
                if (id == -1) {
                    if (nSub == 0) {
                        id = 0;
                    } else {
                        const String name = e->attribute(L("Name"), String());
                        const std::vector<String> parts = qt::split(name, L(" "), qt::SplitBehavior::KeepEmptyParts);
                        if (!parts.empty()) id = qt::toInt(parts.back());
                    }
                }
                subId.push_back(id);
                nSub++;
            // The item managers are Project members here. The original creates each on demand
            // (0x140044d20 objects, 0x140044dc0 materials, 0x140044f00 lubricants, 0x140044e60
            // equipment, each on the data root 0x140044a90) as a QStandardItemModel: the ctors
            // 0x140068ba0 (8 columns), 0x1400425a0, 0x1400bd680, 0x1400beca0 (2 columns) over the
            // base 0x14006c120 (selection model, data root at +0x20) set only Qt view state.
            } else if (tag == L("Object_List")) {
                parseItemList(p, *e, p.objects, ctx);      // 0x1400a58f0 -> M+0x19d0
            } else if (tag == L("Material_List")) {
                parseItemList(p, *e, p.materials, ctx);    // 0x1400a5930 -> M+0x19d8
            } else if (tag == L("Lubricant_List")) {
                parseItemList(p, *e, p.lubricants, ctx);   // 0x1400a59b0 -> M+0x19e8
            } else if (tag == L("Equipment_List")) {
                parseItemList(p, *e, p.equipment, ctx);    // 0x1400a5970 -> M+0x19e0
            } else if (tag == L("Operation_List")) {
                if (!parseOperationList(p, *e, ctx)) return false;
            }
        }
    }
    // Sub-project 0 exists from the ctor and keeps type 0 / id 0.
    for (int i = 0; i < nSub - 1; i++)
        p.subs.push_back(makeSubProject(p, subType[static_cast<size_t>(i) + 1], subId[static_cast<size_t>(i) + 1]));
    for (int i = 1; i < nSub; i++) {
        SubProject* sub = p.subs[static_cast<size_t>(i)].get();
        if (sub && !sub->loaded) {
            const xml::Node* e = subProjectElement(p, i);
            // 0x1400a78e0 returns a null QDomElement when there is none; 0x14008f200 then reads
            // nothing from it.
            if (e) parseSubProjectDbName(p, *e, i);
        }
    }
    return true;
}

// @src 0x14008e290
void parseItemList(Project& p, const xml::Node& list, ItemManager& mgr, Ctx& ctx)
{
    for (const xml::Node* c : list.childElements())
        if (c->tag == L("MoItem")) parseMoItem(p, *c, mgr, ctx);
}

namespace {

// @src 0x14004dc20  add a material id unless the list already has it
void addMatId(std::vector<int>& ids, int id)
{
    for (int v : ids)
        if (v == id) return;
    ids.push_back(id);
}

// @src 0x140041eb0  material type from its name
void setMatType(Item& m, const String& t)
{
    if (t == L("Single")) m.matType = 0;
    else if (t == L("Mixture")) m.matType = 1;
    else m.matType = 2;
}

// @src 0x1400421b0  phase list: split on " " (KeepEmptyParts), toInt each
void setPhases(Item& m, const String& list)
{
    const std::vector<String> parts = qt::split(list, L(" "), qt::SplitBehavior::KeepEmptyParts);
    m.phases.clear();
    for (const String& t : parts) m.phases.push_back(qt::toInt(t));
}

// manager vt+0x170 + 0x14006c9c0 (vt+0x198): new item, setText(name), +0x28 = file, appendRow.
// vt+0x170 per manager: 0x140068df0 objects (new 0x48), 0x1400427e0 materials (new 0x50, ctor
// 0x140041be0 -> 0x140041c90: data-role text, +0x40 = 0), 0x1400bd8a0 lubricants (0x1400bd580 ->
// 0x1400bd610), 0x1400beec0 equipment (0x1400beba0 -> 0x1400bec30); the role texts are view only.
// Item vt+0x40 setText is 0x14006b9c0 (-> 0x14006bb40).
Item* addItem(ItemManager& mgr, const String& name, const String& file, int id)
{
    auto it = std::make_unique<Item>();
    it->kind = mgr.kind;
    it->id = id;                                      // +0x18
    setItemName(*it, name);                           // vt+0x40 (objects: record name and text)
    it->keyPath = file;                               // 0x14006ba30 -> +0x28
    mgr.rows.push_back(std::move(it));                // invisibleRootItem()->appendRow
    return mgr.rows.back().get();
}

}  // namespace

// @src 0x14008e3d0
void parseMoItem(Project& p, const xml::Node& item, ItemManager& mgr, Ctx& ctx)
{
    String name, path, data, matId, matType, phaseList;
    int id = 0, objType = 0;
    bool hasData = false, hasInfo = false, hasMat = false, hasGeo = false, hasMesh = false;
    for (const xml::Node* c : item.childElements()) {
        const String& tag = c->tag;
        if (tag == L("Name")) {
            name = c->text();
        } else if (tag == L("Id")) {
            id = qt::toInt(c->text());
        } else if (tag == L("Path")) {
            path = c->text();                         // never used afterwards
        } else if (tag == L("Data")) {
            data = c->text();
            hasData = true;
        } else if (tag == L("ObjectInfo")) {
            hasInfo = true;
            objType = qt::toInt(c->attribute(L("ObjectType"), String()));
            hasGeo = qt::toInt(c->attribute(L("HasGeo"), String())) != 0;
            hasMesh = qt::toInt(c->attribute(L("HasMesh"), String())) != 0;
            matId = c->attribute(L("MatId"), String());
        } else if (tag == L("MatType")) {
            hasMat = true;
            matType = c->text();
        } else if (tag == L("PhaseList")) {
            hasMat = true;
            phaseList = c->text();
        }
    }
    if (name.isEmpty()) return;
    const String file = itemKeyPath(p, mgr.kind, id);   // vt+0x178
    Item* it = nullptr;
    if (!hasData) {
        it = addItem(mgr, name, file, id);
    } else {
        // qUncompress(QByteArray::fromHex(data.toAscii())), mkpath(absolutePath(file)), write the
        // file: only successful OPEN gates row creation (14008ee9f..14008eef5).
        // Write/close results are ignored. Multiple Data children replace the
        // retained string above, unlike the operation-blob writer's per-child IO.
        std::string error;
        std::string bytes = qt::qUncompress(qt::fromHex(qt::toAscii(data)), &error);
        if (!error.empty()) ctx.warn(error + " (" + name.utf8() + ")");
        if (ctx.nativeItemBlobs) {
            if (ctx.beforeNativeItemBlob) ctx.beforeNativeItemBlob(file.u16());
            const auto directory = qt::absolutePath(file);
            const bool made = qt::nativeMkpathAbsolute(directory);
            ctx.blobDirectoryAttempts.push_back({directory.u16(), made});
            // Directory failure does not bypass the following QFile attempt;
            // its modal notification remains a qualified event boundary.
            mst::FileDevice target(file);
            if (target.opened()) {
                p.overlay.erase(qt::cleanPath(file));
                target.write(bytes);
                target.close();
                it = addItem(mgr, name, file, id);
            }
        } else {
            p.overlay[qt::cleanPath(file)] = std::move(bytes);
            it = addItem(mgr, name, file, id);
        }
    }
    if (it && hasInfo && it->kind == ItemKind::Object) {      // dynamic_cast<SfMoObjItem*>
        it->objectType = objType;                     // 0x14004d580  d+0x04
        it->hasGeo = hasGeo;                          // 0x14004d780  d+0x18
        it->hasMesh = hasMesh;                        // 0x14004d7d0  d+0x19
        it->matIds.clear();                           // 0x14004db20  d+0x20
        if (!matId.isEmpty())
            for (const String& tok : qt::split(matId, L(" "), qt::SplitBehavior::KeepEmptyParts))
                addMatId(it->matIds, qt::toInt(tok));
    }
    if (it && hasMat && it->kind == ItemKind::Material) {     // dynamic_cast<SfMoMatItem*>
        setMatType(*it, matType);
        if (it->matType == 1) setPhases(*it, phaseList);      // 0x140041e70: Mixture
    }
}

namespace {

// @src 0x1400a4510 / 0x1400a4640 / 0x1400a4770 / 0x1400a48a0  cleanPath(projectDir + sep + X)
String listDir(const Project& p, const char* sub)
{
    String d = projectDir(p);
    d += String(std::u16string(1, qt::kSep)) + L(sub);
    return qt::cleanPath(d);
}

}  // namespace

// @src 0x140068e60 (Object) / 0x140042850 (Material) / 0x1400bef30 (Equipment) / 0x1400bd910 (Lubricant)
String itemKeyPath(const Project& p, ItemKind kind, int id)
{
    const char* k = "Object";
    const char* sub = "Objects";
    switch (kind) {
    case ItemKind::Object: k = "Object"; sub = "Objects"; break;
    case ItemKind::Material: k = "Material"; sub = "Materials"; break;
    case ItemKind::Equipment: k = "Equipment"; sub = "Equipments"; break;
    case ItemKind::Lubricant: k = "Lubricant"; sub = "Lubricants"; break;
    }
    const String name = qt::arg(qt::arg(L("%1%2.KEY"), L(k), 0, u' '), id, 5, 10, u'0');
    // listDir + QDir::separator() + name, not cleaned: "<dir>/Objects\Object00001.KEY"
    String r = listDir(p, sub);
    r += qt::kSep;
    r += name;
    return r;
}

// @src 0x1400a2150
void setUnit(Project& p, int unit)
{
    p.unit = unit;   // P+0x20; 0x140917d60 sets the engine unit globals (per call here: not needed)
}

// @src 0x1400a4da0
void setPrimaryDb(Project& p, const String& name)
{
    if (name.isEmpty()) return;
    String q = projectDir(p);
    q += qt::kSep;
    q += name;
    q = qt::cleanPath(q);
    if (qt::fileExists(q)) {
        p.primaryDb = q;                              // P+0x30
        // 0x1400a4f30: QFile::exists(P+0x30) again, result unused
    }
}

}  // namespace m2m
