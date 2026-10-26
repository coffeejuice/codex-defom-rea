// SfOprDiestress and SfOprDiestressBM2D/3D (owner P16). Spec: s1-05 2 and 3.
//
// The DiestressBM wizard block (DieStressBmWizard.Wzd, PROPS+0x38) is read at open by P06
// (0x14029eea0); the Diestress post-load reader 0x1402b95a0 (DieStress.Wzd) is ported here from
// P16's own decompile (out_p16/postload.c in the work folder). The simulation lists are P14's
// (Diestress 0x1402b7380; DiestressBM uses the base 0x1405f5880 through 0x14029a4b0).
#include <memory>
#include <vector>

#include "m2m/data/diestress.hpp"
#include "m2m/datafiles.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/emit.hpp"
#include "m2m/mst/loader_l6.hpp"
#include "m2m/mst/simcount.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/props_base.hpp"
#include "m2m/qt4path.hpp"
#include "m2m/xml.hpp"
#include "p16_internal.hpp"

#if __has_include("m2m/data/diestressbm.hpp")
#include "m2m/data/diestressbm.hpp"
#else
// Until P06 merges include/m2m/data/diestressbm.hpp (same members as P06's branch).
#include <array>
#include <cstdint>
namespace m2m::data {
struct DiestressBMData {
    int objTotalNum = 0;                          // +0x000 nObjTotalNum
    std::array<std::uint8_t, 16> interpolate{};   // +0x004 + k bIntepolate
    std::array<int, 15> objNo{};                  // +0x014 + 4k nObjNo
    std::array<int, 16> refNo{};                  // +0x050 + 4k nRefNo
    std::array<double, 15> tolerance{};           // +0x090 + 8k dTolerance
    qt::String newDbName;                         // +0x108 qsNewDbName
    std::uint8_t useNewDb = 0;                    // +0x110 bUseNewDb
};
}  // namespace m2m::data
#endif

namespace m2m {

// @src 0x1402b95a0  SfOprDiestressProperties vt+0xc0: read <taskDir>/DieStress.Wzd
void DiestressProps::postLoad(Project& p, Ctx&)
{
    if (!data) data = std::make_shared<data::DiestressData>();
    String dir = owner ? taskDirAbs(*owner) : String();      // 0x1405f8d90 -> 0x140050df0
    String path = qt::convertSeparators(dir + qt::kSep + String(u"DieStress.Wzd"));   // @0x1422b7fc0
    std::string bytes;
    // QFile(path).open(ReadOnly | Text); on failure the original only prints a qDebug line.
    if (!datafiles::readFileBytes(p, path, bytes)) return;
    xml::Document doc;                                       // QDomDocument("DieStressWizard")
    xml::setContent(doc, bytes, nullptr, nullptr, nullptr);  // result not checked
    // firstChildElement("DieStressWizard").attribute("nStepNumber", QString()).toInt()
    String v;
    if (doc.root && doc.root->kind == xml::Node::Element && doc.root->tag == String(u"DieStressWizard"))
        v = doc.root->attribute(String(u"nStepNumber"), String());
    data->stepNumber = qt::toInt(v);                         // *(PROPS+0x38)+0x80
}

}  // namespace m2m

namespace m2m {

// QString::replace(before, after, Qt::CaseInsensitive), Qt 4.8: every non-overlapping match from
// the left. `before` is ASCII here (".DB"); no non-ASCII character case-folds to an ASCII letter
// other than its own pair, so an ASCII fold of both sides gives the same matches.
String p16::replaceAllCi(const String& s, const std::u16string& before, const std::u16string& after)
{
    auto fold = [](char16_t c) { return (c >= u'A' && c <= u'Z') ? char16_t(c + 32) : c; };
    if (s.isNull()) return s;
    const std::u16string& in = s.u16();
    std::u16string out;
    size_t i = 0;
    while (i < in.size()) {
        bool hit = i + before.size() <= in.size();
        for (size_t k = 0; hit && k < before.size(); ++k)
            hit = fold(in[i + k]) == fold(before[k]);
        if (hit) {
            out += after;
            i += before.size();
        } else {
            out += in[i++];
        }
    }
    return String(std::move(out));
}

}  // namespace m2m

namespace m2m::mst {
namespace {
using p16::replaceAllCi;

// The base groups that end both tree builders (0x1402bb080, 0x14029f0a0; same arguments).
void addBaseGroups(Props& p, int guiUnit)
{
    base::addStepCounts(p, false, false, 100, 10, qt::L("0.0 sec"));          // 0x1405f9eb0
    base::addEnvTemp(p, guiUnit, qt::L("68.00 F"), qt::L("20.00 C"));         // 0x1405fa560
    base::addConvection(p, guiUnit, qt::L("0.0 Btu/sec/in^2/F"),              // 0x1405faaa0
                        qt::L("0.0 N/sec/mm/C"));
    base::addStepBy(p, guiUnit, 0, qt::L("0.0 in"), qt::L("0.0 mm"),           // 0x1405fb020
                    qt::L("0.0 sec"));
}

// @src 0x1401344a0  "KFREAD    \t" 1, then the path (s1-02 range, used only by Diestress)
void emitKfreadOne(W& w, const String& path)
{
    w.ts << "KFREAD    \t" << 1 << endl;       // @0x1421acb68
    w.ts << path << endl;
}

// Sub-project DB path of the operation's (active) sub-project: 0x140055050 -> 0x1400b9e80(sp, true)
String opSubProjectDb(const Operation& op, SubProject** spOut)
{
    SubProject* sp = op.project ? activeSubProject(*op.project) : nullptr;   // 0x1400a7390
    if (spOut) *spOut = sp;
    if (!sp) return String();
    return subProjectDbPath(*op.project, *sp);
}

class DiestressClass : public OpClass {
public:
    DiestressClass() : OpClass(qt::L("SfOprDiestress")) {}
    void writeMst(W& w, Project& prj, const String& db, int* sim, Operation* op, int dbreadArg,
                  int onlySim) override;
    std::shared_ptr<Props> makeProps(int guiUnit) const override;
    // vt+0x298 0x1402b7380 (P14)
    void simList(int pos, int* counter, Operation* op, std::vector<SimEntry>& out) override
    {
        simListDiestress(*this, pos, counter, op, out);
    }
    // ---- operation loader (MDT L6, bodies in src/engine/l6_slots.cpp); vt+0x58 is the base ----
    // vt+0xb8  @src 0x1402b59a0
    void loadPrologue(Project& p, Operation& op) override { l6Slot(g_l6Slots.diestressPrologue)(*this, p, op); }
    // vt+0x78  @src 0x1402b63b0: empty
    void loadClassData(Project&, Operation&) override {}
    // vt+0xc0  @src 0x1402b60f0
    void postLoad(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx) override
    {
        l6Slot(g_l6Slots.diestressPostLoad)(*this, p, op, ctx, fx);
    }
    // vt+0x1b8  @src 0x1402b59e0
    void loadData(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx) override
    {
        l6Slot(g_l6Slots.diestressLoadData)(*this, p, op, ctx, fx);
    }
    // vt+0x1b0  @src 0x1402b5600
    std::shared_ptr<engine::GuiModel> createModel(Project& p, Operation& op) override
    {
        return l6Slot(g_l6Slots.diestressCreateModel)(*this, p, op);
    }
    // vt+0xe8  @src 0x1402b5be0: the model as <sub-project DB base>.KEY (MDT L6S); vt+0xd8 0x1402b6220
    // is the base
    bool saveOperationFiles(Project& p, Operation& op, engine::FileEffects& fx) override
    {
        return l6Slot(g_l6Slots.diestressSaveFiles)(*this, p, op, fx);
    }
    // vt+0x80  @src 0x1402b63c0: empty (MDT L6S)
    bool saveAfterFiles(Project&, Operation&, engine::FileEffects&) override { return true; }
    // vt+0xa8  @src 0x1402b6410 (MDT L6S)
    bool objectTreeFill(Project& p, Operation& op, engine::FileEffects& fx) override
    {
        return l6Slot(g_l6Slots.dieStressTreeFill)(*this, p, op, fx);
    }
};

// @src 0x1402b5410
void DiestressClass::writeMst(W& w, Project&, const String&, int* sim, Operation* op, int,
                              int target)
{
    auto* props = dynamic_cast<DiestressProps*>(propsOf(nullptr));   // the current item's props
    String dbPath = opSubProjectDb(*op, nullptr);
    String key = dbPath;
    key = replaceAllCi(key, u".DB", u".KEY");     // @0x1422a9a54 -> @0x1422ad0bc
    if (!props) return;
    ++*sim;
    if (target == 0 || *sim == target) {
        emitSimHeader(w, *sim, op);
        emitKfreadOne(w, key);
        emitGendb(w, dbPath, *sim, 2, false);
    }
}

// SfOprDiestressProperties (+0x38 block read by 0x1402b95a0)
std::shared_ptr<Props> DiestressClass::makeProps(int guiUnit) const
{
    auto p = std::make_shared<DiestressProps>();
    data::initCommonBlock(*p->common, guiUnit);    // base ctor 0x1405f8ca0 -> 0x1405fd250
    p->data = std::make_shared<data::DiestressData>();   // equipment block 0x1402a2a20 at +0x38
    p->tree = defaultPropTree(key());              // 0x1402b9b60 + 0x1402bb080 (diestress_tree.cpp)
    addBaseGroups(*p, guiUnit);
    return p;
}

class DiestressBMClass : public OpClass {
public:
    explicit DiestressBMClass(bool is2D)
        : OpClass(qt::L(is2D ? "SfOprDiestressBM2D" : "SfOprDiestressBM3D"))
    {
    }
    void writeMst(W& w, Project& prj, const String& db, int* sim, Operation* op, int dbreadArg,
                  int onlySim) override;
    std::shared_ptr<Props> makeProps(int guiUnit) const override;
    // vt+0x298 0x14029a4b0 -> base 0x1405f5880 (P14)
    void simList(int pos, int* counter, Operation* op, std::vector<SimEntry>& out) override
    {
        simListDiestressBM(*this, pos, counter, op, out);
    }
    // ---- operation loader (MDT L6, bodies in src/engine/l6_slots.cpp); vt+0x58 is the base,
    // vt+0x1b0 (0x14029b440 / 0x14029d280) only calls the base 0x1405f6410 ----
    // vt+0xb8  @src 0x140299230
    void loadPrologue(Project& p, Operation& op) override { l6Slot(g_l6Slots.diestressBMPrologue)(*this, p, op); }
    // vt+0x78  @src 0x140299a90
    void loadClassData(Project& p, Operation& op) override { l6Slot(g_l6Slots.diestressBMClassData)(*this, p, op); }
    // vt+0xc0  @src 0x140299740
    void postLoad(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx) override
    {
        l6Slot(g_l6Slots.diestressBMPostLoad)(*this, p, op, ctx, fx);
    }
    // vt+0x1b8  @src 0x14029b570 (2D) / 0x14029d3b0 (3D)
    void loadData(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx) override
    {
        l6Slot(g_l6Slots.diestressBMLoadData)(*this, p, op, ctx, fx);
    }
    // vt+0xd8  @src 0x140299870 (MDT L6S)
    void saveNames(Project& p, Operation& op, engine::FileEffects& fx) override
    {
        l6Slot(g_l6Slots.diestressBMSaveNames)(*this, p, op, fx);
    }
    // vt+0x80  @src 0x140299b60 (MDT L6S)
    bool saveAfterFiles(Project& p, Operation& op, engine::FileEffects& fx) override
    {
        return l6Slot(g_l6Slots.diestressBMSaveAfter)(*this, p, op, fx);
    }
    // vt+0xa8  @src 0x140299c50 (MDT L6S)
    bool objectTreeFill(Project& p, Operation& op, engine::FileEffects& fx) override
    {
        return l6Slot(g_l6Slots.dieStressTreeFill)(*this, p, op, fx);
    }

private:
    int dieObjects(int* out) const;
};

// @src 0x14029a550  object numbers of the current item's objects of type 5 or 1 (qDebug only
// otherwise: "set WP No = <id>   Type = <type>")
int DiestressBMClass::dieObjects(int* out) const
{
    int n = 0;
    Operation* node = current;                    // 0x1405ef2b0
    if (!node) return 0;
    for (int k = 0; k < objSlots(*node); ++k) {
        ObjEntry* o = objEntry(*node, k);
        if (!o) continue;
        int id = o->number;                       // 0x14004d560: +0x00
        int type = o->objectType;                 // 0x14004d5a0: +0x04
        if (type == 5 || type == 1) out[n++] = id;
    }
    return n;
}

// @src 0x14029ad10 (2D) / 0x14029cb50 (3D): identical code and literals
void DiestressBMClass::writeMst(W& w, Project& prj, const String& db, int* sim, Operation* op,
                                int dbreadArg, int target)
{
    auto* p = dynamic_cast<DiestressBMProps*>(propsOf(nullptr));
    if (!p) return;
    int nObj = objSlots(*op);                     // 0x14005fa80
    if (nObj <= 0) return;
    std::vector<int> ids(static_cast<size_t>(nObj));
    int nDie = dieObjects(ids.data());
    ++*sim;
    if (target == 0 || *sim == target) {
        emitSimHeader(w, *sim, op);
        emitDbread(w, db, *sim, dbreadArg);
        emitSimCtrl(w, prj, db, *sim, op);
        emitMaterials(w, prj, db, *sim, op);
        emitMeshSettings(w, prj, db, *sim, op);
        emitObjects(w, prj, db, *sim, op, false);
        emitContact(w, prj, db, *sim, op);
        emitDoeGeometry(w, op);
        emitDoeVariables(w, op);
        emitObjpos(w, prj, db, *sim, op);
        emitContactFlags(w, prj, db, *sim, op);
        const data::DiestressBMData& d = *p->data;
        if (d.objTotalNum > static_cast<int>(d.objNo.size()))
            throw Error(M2M_E_INPUT, "DieStressBmWizard.Wzd: more objects than the wizard block holds");
        for (int k = 0; k < d.objTotalNum; ++k) {
            if (d.interpolate[static_cast<size_t>(k)] == 1) {
                int a = d.objNo[static_cast<size_t>(k)];
                int b = d.refNo[static_cast<size_t>(k)];
                double tol = d.tolerance[static_cast<size_t>(k)];
                if (a != b && b > 0)
                    w.ts << "DEFFRZ" << "\t" << a << "\t" << b << "\t" << tol << endl;   // @0x14227b6a4
            }
        }
        for (int k = 0; k < nDie; ++k)
            w.ts << "OBJTYP" << "\t" << ids[static_cast<size_t>(k)] << "\t" << 0 << endl;   // @0x14227b6ac
        emitKfreadUser(w, op);
        emitOprnam(w, op->name);                                   // 0x140050ac0
        emitSimnam(w, String(u"Die Stress Simulation"));           // @0x14227b6b8
        emitCursim(w, *sim, flowNumber(*op), 0);                    // 0x140050b10 always 0
        SubProject* sp = nullptr;
        String dbOut;
        String dir;
        SubProject* cur = op->project ? activeSubProject(*op->project) : nullptr;   // 0x140055050
        if (cur && cur->project) dir = projectDir(*cur->project);  // 0x1400a4220(0x1400b5c40(sp))
        dbOut = opSubProjectDb(*op, &sp);
        if (d.useNewDb) {
            dbOut = dir + String(u"/") + d.newDbName;               // @0x142278a8c
            *sim = 1;                                              // resets the running counter
        }
        emitGendb(w, dbOut, *sim, 2, false);
    }
}

// SfOprDiestressBMProperties; the wizard block defaults are P06's open item
std::shared_ptr<Props> DiestressBMClass::makeProps(int guiUnit) const
{
    auto p = std::make_shared<DiestressBMProps>();
    data::initCommonBlock(*p->common, guiUnit);    // base ctor 0x1405f8ca0 -> 0x1405fd250
    p->data = std::make_shared<data::DiestressBMData>();
    p->tree = defaultPropTree(key());              // 0x14029f680 + 0x14029f0a0 (diestress_tree.cpp)
    addBaseGroups(*p, guiUnit);
    return p;
}

}  // namespace

std::unique_ptr<OpClass> makeDiestress()
{
    return std::make_unique<DiestressClass>();
}

std::unique_ptr<OpClass> makeDiestressBM(bool is2D)
{
    return std::make_unique<DiestressBMClass>(is2D);
}

}  // namespace m2m::mst
