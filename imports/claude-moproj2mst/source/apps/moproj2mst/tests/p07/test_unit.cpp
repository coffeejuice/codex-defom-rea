// P07 unit test: MST top level (s1-02 2-4) on hand-built projects: bytes of header, main block,
// walk and trailer; the operation-info slice (src/model/opinfo.cpp) and the list helpers.
#include <cstdio>
#include <string>
#include <vector>

#include "m2m/mst/emit.hpp"
#include "m2m/p07_trees.hpp"
#include "m2m/props_tree.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4path.hpp"

using namespace m2m;
using namespace m2m::mst;

namespace {

int g_fail = 0;

void check(bool ok, const std::string& what)
{
    if (!ok) {
        ++g_fail;
        std::printf("FAIL %s\n", what.c_str());
    }
}

const std::string R = "*" + std::string(73, '-') + "*\n";
const std::string B = "*" + std::string(73, ' ') + "*\n";

std::string header(const std::string& title, int geotyp, int unit)
{
    return R + "*  MASTER FILE FOR DEFORM MUlTIPLE OPERATION SIMULATION                   *\n" + B +
           "*  Integrated 2D / 3D                                                     *\n" + B +
           "*!MASTERFORMAT 4                                                          *\n" + R + "*!TITLE  " + title +
           "\n*!GEOTYP " + std::to_string(geotyp) + "\n*!UNIT   " + std::to_string(unit) + "\n*\n" + R + "\n";
}

std::string mainBlock(const std::string& name)
{
    return "\n" + R + B + "* Main Project                                                            *\n" + B + R +
           "*!MAINPRJ \n*!PRJNAM " + name + "\n*\n" + R;
}

const std::string kTrailer =
    "\n" + R + B + "* END OF MASTER FILE                                                      *\n" + B + R + "\n";

// A writer that records its call: "OP <name> <n> <db> <a6> <a7> cur=<current name>".
class EchoClass : public OpClass {
public:
    using OpClass::OpClass;
    void writeMst(W& w, Project&, const String& db, int* sim, Operation* op, int a6, int a7) override
    {
        ++*sim;
        w.ts << "OP " << op->name << " " << *sim << " " << db << " " << a6 << " " << a7 << " cur="
             << (current ? current->name : qt::L("-")) << " doe=" << (w.st.doeMode ? 1 : 0) << endl;
    }
    int dim() const override { return 3; }
};

Operation* addOp(Project& p, int id, const char* name, const char* key)
{
    auto op = std::make_unique<Operation>();
    op->id = id;
    op->project = &p;
    op->name = qt::L(name);
    op->classKey = qt::L(key);
    op->opClass = p.registry.at(op->classKey).get();
    p.operations.push_back(std::move(op));
    return p.operations.back().get();
}

// Chain a -> b -> ... in the sub-project's flow chart, in the given order.
void chain(SubProject& sub, const std::vector<int>& ids)
{
    FlowItem* prev = nullptr;
    int col = static_cast<int>(ids.size());
    for (int id : ids) {
        auto it = std::make_unique<FlowItem>();
        it->opId = id;
        it->row = 0;
        it->col = --col;          // columns descending: the chain, not the cell order, decides
        it->prev = prev;
        if (prev) prev->next = it.get();
        prev = it.get();
        sub.flow.items.push_back(std::move(it));
    }
}

void unitTests()
{
    // 1. No sub-project: nothing written, false (M2M_E_NO_OPERATIONS in the ABI).
    {
        Project p;
        p.path = qt::L("C:/x/A.moproj");
        Stream ts(1252);
        State st;
        Ctx ctx;
        W w{ts, st, ctx};
        check(!writeMainMst(w, p), "no sub-project returns false");
        check(ts.bytes().empty(), "no sub-project writes nothing");
    }
    // 2. Sub-project without operations: header + block + trailer.
    {
        Project p;
        p.path = qt::L("C:/x/COG_P1_B1.moproj");
        p.unit = 2;
        p.geotyp = 0;
        p.title = qt::L("Multiple_Operations_Example1");
        p.subs.push_back(std::make_unique<SubProject>());
        p.subs[0]->project = &p;
        Stream ts(1252);
        State st;
        st.objMaskPrev = {1, 1};
        st.objMaskCur = {1};
        Ctx ctx;
        W w{ts, st, ctx};
        check(writeMainMst(w, p), "empty sub-project returns true");
        check(ts.bytes() == header("Multiple_Operations_Example1", 0, 2) + mainBlock("COG_P1_B1") + kTrailer,
              "empty sub-project bytes");
        check(st.objMaskCur.empty() && st.objMaskPrev.empty(), "masks cleared (0x140131390)");
    }
    // 3. Walk: flow order, shared counter, current item set and restored, unknown ids skipped,
    //    ops without class skipped, doeMode reset, dbPath = cleanPath(cleanPath(primaryDb)).
    {
        Project p;
        p.path = qt::L("C:/x/P.moproj");
        p.unit = 1;
        p.primaryDb = qt::L("C:\\x\\.\\P.DB");
        p.registry[qt::L("SfOprEcho")] = std::make_unique<EchoClass>(qt::L("SfOprEcho"));
        auto* echo = p.registry[qt::L("SfOprEcho")].get();
        Operation* a = addOp(p, 1, "A", "SfOprEcho");
        addOp(p, 2, "B", "SfOprEcho");
        Operation* c = addOp(p, 3, "C", "SfOprEcho");
        c->opClass = nullptr;
        c->classKey = qt::L("SfOprMissing");       // no class: skipped
        addOp(p, 4, "D", "SfOprEcho");
        p.subs.push_back(std::make_unique<SubProject>());
        p.subs[0]->project = &p;
        chain(*p.subs[0], {4, 99, 3, 2, 1});       // 99: no such operation
        echo->current = a;                          // restored after every operation
        Stream ts(1252);
        State st;
        st.doeMode = true;
        Ctx ctx;
        W w{ts, st, ctx};
        walkOperations(w, p, 0);
        check(ts.bytes() == "OP D 1 C:/x/P.DB 0 0 cur=D doe=0\n"
                            "OP B 2 C:/x/P.DB 0 0 cur=B doe=0\n"
                            "OP A 3 C:/x/P.DB 0 0 cur=A doe=0\n",
              "walk output: " + ts.bytes());
        check(echo->current == a, "current item restored");
        Stream ts2(1252);
        W w2{ts2, st, ctx};
        walkOperations(w2, p, 1);                   // out of range: nothing
        walkOperations(w2, p, -1);
        check(ts2.bytes().empty(), "walk of a missing sub-project writes nothing");
    }
    // 4. MST path (0x1400a50a0).
    {
        Project p;
        p.path = qt::L("C:/x/COG_P1_B1.moproj");
        check(mstPath(p, true) == qt::L("C:/x/COG_P1_B1.MST"), "mstPath full");
        // full == false is not on the Save path; its result is P05's changeSuffix of a bare name.
    }
}

// Slice from P02 (src/model/opinfo.cpp) and the list helpers.
void opInfoTests()
{
    Project p;
    p.path = qt::L("C:/PROBLEM/X/X.moproj");
    p.subs.push_back(std::make_unique<SubProject>());
    p.subs[0]->project = &p;
    p.subs[0]->id = 7;
    Operation op;
    op.project = &p;
    op.id = 2;
    op.taskDir = qt::L("Operations/Task00002");
    // task folder and files (s1-01 5.3): native separators for the folder, '/' after cleanPath
    check(taskDirAbs(op) == qt::L("C:\\PROBLEM\\X\\Operations\\Task00002"), "taskDirAbs: " + taskDirAbs(op).utf8());
    check(simCtrlPath(op) == qt::L("C:/PROBLEM/X/Operations/Task00002/SimCtrl.KEY"), "simCtrlPath");
    check(interObjectPath(op) == qt::L("C:/PROBLEM/X/Operations/Task00002/InterObject.KEY"), "interObjectPath");
    check(objPropertiesPath(op, 0) == qt::L("C:/PROBLEM/X/Operations/Task00002/ObjProperties00001.KEY"),
          "objPropertiesPath");
    check(meshSettingsPath(op, 11) == qt::L("C:/PROBLEM/X/Operations/Task00002/MeshSettings00012.KEY"),
          "meshSettingsPath");
    check(bccDefPath(op, 2) == qt::L("C:/PROBLEM/X/Operations/Task00002/BccDef00003.KEY"), "bccDefPath");
    Operation lone;
    check(taskDirAbs(lone).isNull(), "taskDirAbs without project is null");
    // chain 3 -> 2 -> 5 in the active sub-project
    chain(*p.subs[0], {3, 2, 5});
    check(flowNumber(op) == 2, "flowNumber");
    check(!isChainHead(op), "op 2 has a predecessor");
    op.id = 3;
    check(isChainHead(op), "op 3 is the head");
    op.id = 99;
    check(isChainHead(op), "unknown id counts as head (predecessor -1)");
    check(flowNumber(op) == 0, "unknown id has number 0");
    check(!isChainHead(lone) && flowNumber(lone) == 0, "no project: not head, number 0");
    check(subProjectId(p, 0) == 7 && subProjectId(p, 1) == -1, "subProjectId");
    // names and rows
    p.operations.push_back(std::make_unique<Operation>());
    p.operations[0]->name = qt::L("Heat");
    check(oprNameExists(p, qt::L("Heat")) && !oprNameExists(p, qt::L("heat")), "oprNameExists (case-sensitive)");
    check(oprItemAt(p, 0) == p.operations[0].get() && !oprItemAt(p, 1) && !oprItemAt(p, -1), "oprItemAt");
    // setters
    setSubType(op, qt::L("SfOprAirConvect"));
    check(op.subType == qt::L("SfOprAirConvect"), "setSubType");
    check(hasNoId(lone) && !hasNoId(op), "hasNoId");
    setOprFlag(op, 0);
    check(oprFlagOff(op), "oprFlagOff");
    auto props = std::make_shared<Props>();
    setOprProps(op, props);
    check(op.props == props && props->owner == &op, "setOprProps sets the owner");
    resetOprInfo(op);
    check(!op.props && op.lubricantId == -1 && op.equipmentId == -1 && op.tloc == 0.0, "resetOprInfo");
    // list helpers
    std::vector<int> a{1}, b;
    std::vector<std::vector<int>*> l;
    check(!firstEntryNonEmpty(l), "empty list");
    l.push_back(&b);
    check(!firstEntryNonEmpty(l), "entry 0 empty");
    l[0] = &a;
    check(firstEntryNonEmpty(l), "entry 0 non-empty");
    std::vector<std::unique_ptr<ObjEntry>> objs;
    objs.push_back(std::make_unique<ObjEntry>());
    initOwningList(objs);
    check(objs.empty(), "initOwningList");
    // Distortion default tree (0x141366100): root named by the class key, two int children
    const std::vector<PropNode> t = trees::distortionPropTree(qt::L("SfOprDistortion3D"));
    check(t.size() == 1 && t[0].name == qt::L("SfOprDistortion3D") && t[0].typeName == qt::L("group") &&
              t[0].children.size() == 2 && t[0].children[0].name == qt::L("NumFixtures") &&
              t[0].children[1].name == qt::L("NumPasses") && t[0].children[1].typeName == qt::L("int"),
          "distortion tree");
    // Convert2Dto3D / CopyMirror (0x1401cdf30 / 0x140e2cbd0): root + 8 doubles with the data defaults
    for (const auto& c : {tree::treeConvert2Dto3D(), tree::treeCopyMirror()}) {
        bool ok = c.size() == 1 && c[0].typeName == qt::L("group") && c[0].children.size() == 8 &&
                  c[0].children[0].name == qt::L("DTUPAXIS") && c[0].children[7].name == qt::L("DTEXTSTLOC") &&
                  c[0].children[3].value.kind == qt::Variant::Double && c[0].children[3].value.d == 360.0 &&
                  c[0].children[5].value.d == 0.001 && c[0].children[6].value.d == 5.0;
        check(ok, "convert/copymirror tree");
    }
    check(tree::treeConvert2Dto3D()[0].name == qt::L("SfOprConvert2Dto3D") &&
              tree::treeCopyMirror()[0].name == qt::L("SfOprCopyMirror"),
          "convert/copymirror root names");
}

}  // namespace

int main()
{
    try {
        unitTests();
        opInfoTests();
    } catch (const std::exception& e) {
        check(false, std::string("exception: ") + e.what());
    }
    std::printf("p07 unit: %s (%d failures)\n", g_fail ? "FAIL" : "PASS", g_fail);
    return g_fail ? 1 : 0;
}
