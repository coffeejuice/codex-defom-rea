// ctest-standalone: src/ops/base.cpp src/ops/family.cpp src/core/basic.cpp tests/p10/stream_double.cpp
// P10 tier 1: call order and arguments of the base writer 0x1405f5340 and of 0x1405f56c0
// (s1-02 5, 5.1; checked against the decompile in s1-02 a1s.c), with every emitter replaced by a
// recorder. Links src/ops/base.cpp and src/ops/family.cpp only; the emitters, flowNumber and the
// P02/P14 slots are defined here.
#include <cstdio>
#include <memory>
#include <string>
#include <vector>

#include "m2m/mst/emit.hpp"
#include "m2m/mst/writer.hpp"

namespace {
std::vector<std::string> g_log;
void rec(const char* name, int n = -999, int x = -999)
{
    std::string s = name;
    if (n != -999) s += " " + std::to_string(n);
    if (x != -999) s += " " + std::to_string(x);
    g_log.push_back(s);
}
int g_flowNumber = 0;
}  // namespace

namespace m2m {
int flowNumber(const Operation&) { return g_flowNumber; }
}  // namespace m2m

namespace m2m::mst {
std::shared_ptr<Props> OpClass::makeProps(int) const { return nullptr; }
String OpClass::displayName() const { return key(); }
void OpClass::simList(int, int*, Operation*, std::vector<SimEntry>&) {}   // P14

void emitSimHeader(W&, int n, Operation*) { rec("SIMHDR", n); }
void emitDimNstart(W&, const String&, int n, Operation*, int a) { rec("DIMNSTART", n, a); }
void emitDbread(W&, const String&, int n, int a) { rec("DBREAD", n, a); }
void emitKfreadRestart(W&, const String&, Operation*) { rec("KFREAD_RESTART"); }
void emitKfreadUser(W&, Operation*) { rec("KFREAD_USER"); }
void emitOprnam(W&, const String& s) { rec(("OPRNAM " + s.utf8()).c_str()); }
void emitSimnam(W&, const String& s) { rec(("SIMNAM " + s.utf8()).c_str()); }
void emitCursim(W&, int n, int a, int b) { rec(("CURSIM " + std::to_string(n)).c_str(), a, b); }
void emitGendb(W&, const String&, int n, int mode, bool f) { rec(f ? "GENDB*" : "GENDB", n, mode); }
void emitSimCtrl(W&, Project&, const String&, int n, Operation*) { rec("SIMCTRL", n); }
void emitMaterials(W&, Project&, const String&, int n, Operation*) { rec("MATERIALS", n); }
void emitObjects(W&, Project&, const String&, int n, Operation*, bool s) { rec("OBJECTS", n, s); }
void emitContact(W&, Project&, const String&, int n, Operation*) { rec("CNTACT", n); }
void emitObjpos(W&, Project&, const String&, int n, Operation*) { rec("OBJPOS", n); }
void emitTloc(W&, Project&, const String&, int n, Operation*) { rec("TLOC", n); }
void emitContactFlags(W&, Project&, const String&, int n, Operation*) { rec("INICTC", n); }
void emitStateVars(W&, Project&, const String&, int n, Operation*) { rec("STATEVARS", n); }
void emitMeshSettings(W&, Project&, const String&, int n, Operation*) { rec("MESHSET", n); }
void emitBccDef(W&, Project&, const String&, int n, Operation*) { rec("BCCDEF", n); }
void emitObjProperties(W&, Project&, const String&, int n, Operation*) { rec("OBJPROPS", n); }
void emitEquipment(W&, Project&, const String&, int n, Operation*) { rec("IMPEQP", n); }
void emitDoeGeometry(W&, Operation*) { rec("DOEGEO"); }
void emitDoeVariables(W&, Operation*) { rec("DOEVAR"); }
}  // namespace m2m::mst

using namespace m2m;
using namespace m2m::mst;

static int g_fail = 0;

static void expect(const char* what, const std::vector<std::string>& want)
{
    if (g_log == want) return;
    ++g_fail;
    std::printf("FAIL %s\n  got :", what);
    for (auto& s : g_log) std::printf(" [%s]", s.c_str());
    std::printf("\n  want:");
    for (auto& s : want) std::printf(" [%s]", s.c_str());
    std::printf("\n");
}

static void check(const char* what, bool ok)
{
    if (!ok) {
        ++g_fail;
        std::printf("FAIL %s\n", what);
    }
}

int main()
{
    Stream ts(1252);
    State st;
    Ctx ctx;
    W w{ts, st, ctx};
    Project prj;
    String db = qt::L("C:/x/HEAT1.DB");
    Operation op;
    op.name = qt::L("HeatTransfer");

    // DIM/NSTART class (HeatTransfer), body path, counter 0 -> 1, dbreadArg 7.
    auto ht = makeHeatTransfer(false);
    int counter = 0;
    g_flowNumber = 3;
    g_log.clear();
    ht->writeMst(w, prj, db, &counter, &op, 7, 0);
    check("counter incremented", counter == 1);
    expect("HeatTransfer body", {"SIMHDR 1", "DIMNSTART 1 7", "SIMCTRL 1", "MATERIALS 1", "OBJECTS 1 0",
                                 "CNTACT 1", "MESHSET 1", "BCCDEF 1", "IMPEQP 1", "OBJPROPS 1", "TLOC 1",
                                 "DOEGEO", "DOEVAR", "OBJPOS 1", "INICTC 1", "STATEVARS 1", "KFREAD_USER",
                                 "OPRNAM HeatTransfer", "SIMNAM HeatTransfer", "CURSIM 1 3 0",
                                 "GENDB 1 2"});

    // Plain class (HTFurnace): DBREAD instead of DIM/NSTART, n continues from the shared counter.
    auto hf = makeHTFurnace();
    g_log.clear();
    hf->writeMst(w, prj, db, &counter, &op, 0, 0);
    check("counter shared", counter == 2);
    check("HTFurnace first line", g_log.size() > 1 && g_log[1] == "DBREAD 2 0");

    // Restart KEY path (OPI+0x150): KFREAD of the own KEY, GENDB mode 0, no setup lines.
    op.restartGiven = 1;
    g_log.clear();
    ht->writeMst(w, prj, db, &counter, &op, 0, 0);
    expect("restart path", {"SIMHDR 3", "KFREAD_RESTART", "DOEGEO", "DOEVAR", "KFREAD_USER",
                            "OPRNAM HeatTransfer", "SIMNAM HeatTransfer", "CURSIM 3 3 0", "GENDB 3 0"});

    // +0x200 split (s1-03 2.1) and keys.
    check("Forming +0x200", makeForming(false)->writesDimNstart());
    check("Forming2D +0x200", makeForming(true)->writesDimNstart());
    check("HeatTransfer2D +0x200", makeHeatTransfer(true)->writesDimNstart());
    check("HTExpress +0x200", makeHeatTransferExpress(false)->writesDimNstart());
    check("HTExpress2D +0x200", makeHeatTransferExpress(true)->writesDimNstart());
    check("HTFurnace +0x200", !makeHTFurnace()->writesDimNstart());
    check("Pre +0x200", !makePre(false)->writesDimNstart());
    check("Pre2D +0x200", !makePre(true)->writesDimNstart());
    check("Pre key", makePre(false)->key() == qt::L("SfOprPre3D"));
    check("HT2D key", makeHeatTransfer(true)->key() == qt::L("SfOprHeatTransfer2D"));

    // vt+0x190 from the dimension flags (s1-02 7).
    const int want[4] = {0, 2, 3, 4};
    for (int f = 0; f < 4; ++f) {
        ht->dimFlags = f | 0x10;
        check("dim()", ht->dim() == want[f]);
    }

    // propsOf: op, else the current item.
    Operation cur;
    cur.props = nullptr;
    ht->current = &cur;
    check("propsOf current", ht->propsOf(nullptr) == cur.props.get());
    check("propsOf null", makePre(true)->propsOf(nullptr) == nullptr);

    std::printf("%s\n", g_fail ? "p10 base order: FAILED" : "p10 base order: OK");
    return g_fail ? 1 : 0;
}
