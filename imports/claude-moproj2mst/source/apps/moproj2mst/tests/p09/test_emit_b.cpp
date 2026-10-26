// ctest-standalone: src/mst/emit_b.cpp src/mst/emit_b_lines.cpp src/mst/emit_b_doe.cpp src/doe/item_owner.cpp src/core/basic.cpp tests/p09/doubles.cpp
// P09 tier 1, standalone against the test doubles in doubles.cpp (main build via the line above, or
// tests/p09/CMakeLists.txt before the packets merge): unit vectors per emitter
// (s1-02 5.1, 6) with expected bytes from the spec and from lines of corpus MSTs (COG_P1_B1, HEAT1,
// VDIE, CHAMF1, MO2_Boolean_Oprations).
#include <cmath>
#include <cstdio>
#include <limits>
#include <string>

#include "m2m/mst/doe_model.hpp"
#include "m2m/mst/emit.hpp"

using namespace m2m;
using namespace m2m::mst;

// Read by the doubles (tests/p09/doubles.cpp); unused in the main build.
namespace p09test {
bool g_is2D = false;
std::vector<m2m::RecoveryEntry> g_recA;
bool g_recANull = false;
}  // namespace p09test

static int g_fail = 0;
static int g_checks = 0;

static void check(const char* what, const std::string& got, const std::string& want)
{
    ++g_checks;
    if (got == want) return;
    ++g_fail;
    std::printf("FAIL %s\n--- got\n%s--- want\n%s---\n", what, got.c_str(), want.c_str());
}

struct Fixture {
    Stream ts{1252};
    State st;
    Ctx ctx;
    W w{ts, st, ctx};
    Project prj;
    String db = String(u"C:/x/COG_P1_B1.DB");

    Fixture()
    {
        prj.path = String(u"C:/x/COG_P1_B1.moproj");
        for (int id = 1; id <= 5; ++id) {
            auto o = std::make_unique<Item>();
            o->kind = ItemKind::Object;
            o->id = id;
            o->keyPath = String(u"C:/x/Objects\\Object0000") + qt::number(id) + String(u".KEY");
            prj.objects.rows.push_back(std::move(o));
            auto e = std::make_unique<Item>();
            e->kind = ItemKind::Equipment;
            e->id = id;
            e->keyPath = String(u"C:/x/Equipments\\Equipment0000") + qt::number(id) + String(u".KEY");
            prj.equipment.rows.push_back(std::move(e));
        }
        p09test::g_is2D = false;
        p09test::g_recA.clear();
        p09test::g_recANull = false;
        // recovery list A lives on the active sub-project's data root (0x14005a400)
        auto sub = std::make_unique<SubProject>();
        sub->project = &prj;
        prj.subs.push_back(std::move(sub));
        prj.activeSub = 0;
    }
    std::string take()
    {
        std::string s = ts.bytes();
        ts = Stream(1252);
        return s;
    }
    std::unique_ptr<Operation> op(const char* task, std::vector<int> ids)
    {
        auto o = std::make_unique<Operation>();
        o->project = &prj;
        o->classKey = String(u"SfOprHeatTransfer");
        o->taskDir = String::latin1(task);
        o->objIds = ids;
        for (size_t i = 0; i < ids.size(); ++i) objEntry(*o, static_cast<int>(i));
        return o;
    }
};

static ObjEntry& ent(Operation& o, int slot) { return *objEntry(o, slot); }

// ---- 140135220 / 140135680 / 140135b80 -------------------------------------------------------------
static void test_objects()
{
    Fixture f;
    f.st.clearMasks();
    // COG_P1_B1 simulation 1: one object (id 1, type 1, material 1)
    auto s1 = f.op("Operations/Task00001", {1});
    ent(*s1, 0).objectType = 1;
    ent(*s1, 0).matIds = {1};
    emitObjects(f.w, f.prj, f.db, 1, s1.get(), false);
    check("objects sim1", f.take(),
          "OBJTYP\t\t1\t0\nKFREAD    \t1\nObjects/Object00001.KEY\nMTLGRP    \t1\t0\t1\n");

    // simulation 2: slot 0 from DB (-2, type 5), objects 2 and 3 new; no OBJTYP-0 line
    auto s2 = f.op("Operations/Task00002", {-2, 2, 3});
    ent(*s2, 0).objectType = 5;
    ent(*s2, 1).objectType = 1;
    ent(*s2, 1).matIds = {2};
    ent(*s2, 2).objectType = 1;
    ent(*s2, 2).matIds = {3};
    emitObjects(f.w, f.prj, f.db, 2, s2.get(), false);
    check("objects sim2", f.take(),
          "OBJTYP\t\t2\t0\nKFREAD    \t1\nObjects/Object00002.KEY\nMTLGRP    \t2\t0\t2\n"
          "OBJTYP\t\t3\t0\nKFREAD    \t1\nObjects/Object00003.KEY\nMTLGRP    \t3\t0\t3\n");

    // CHAMF1 form: the previous operation had 5 slots, this one 3 -> "OBJTYP    \t4\t0", 5
    auto s3 = f.op("Operations/Task00003", {-2, -2, -2});
    for (int i = 0; i < 3; ++i) ent(*s3, i).objectType = 5;
    f.st.objMaskPrev.assign(5, 1);
    emitObjects(f.w, f.prj, f.db, 3, s3.get(), false);
    check("objects close", f.take(), "OBJTYP    \t4\t0\nOBJTYP    \t5\t0\n");
    check("mask copied", std::string(f.st.objMaskPrev.begin(), f.st.objMaskPrev.end()),
          std::string("\1\1\1", 3));

    // type 7 slot, skipped slot, unknown object item, two materials (no MTLGRP)
    auto s4 = f.op("Operations/Task00004", {1, 9, 2, 3});
    ent(*s4, 0).objectType = 7;
    ent(*s4, 1).objectType = 1;                  // id 9: no item -> mask 0
    ent(*s4, 2).objectType = 1;
    ent(*s4, 2).skipDefine = true;               // mask 0
    ent(*s4, 3).objectType = 1;
    ent(*s4, 3).matIds = {1, 2};
    emitObjects(f.w, f.prj, f.db, 4, s4.get(), false);
    check("objects mixed", f.take(),
          "OBJTYP\t\t4\t0\nKFREAD    \t1\nObjects/Object00003.KEY\n"
          "OBJTYP    \t1\t0\nOBJTYP    \t2\t0\nOBJTYP    \t3\t0\n");

    // single = true: slot 1 writes only MTLGRP 1; other slots as usual
    f.st.clearMasks();
    auto s5 = f.op("Operations/Task00005", {1, 2});
    ent(*s5, 0).objectType = 1;
    ent(*s5, 0).matIds = {4};
    ent(*s5, 1).objectType = 1;
    ent(*s5, 1).matIds = {5};
    emitObjects(f.w, f.prj, f.db, 1, s5.get(), true);
    check("objects single", f.take(),
          "MTLGRP    \t1\t0\t4\nOBJTYP\t\t2\t0\nKFREAD    \t1\nObjects/Object00002.KEY\n"
          "MTLGRP    \t2\t0\t5\n");

    // 140135680: slot 0 only, no ObjectType test; a later slot of the previous mask gets OBJTYP-0
    f.st.clearMasks();
    f.st.objMaskPrev.assign(2, 1);
    auto s6 = f.op("Operations/Task00006", {1, 2});
    ent(*s6, 0).objectType = 7;
    ent(*s6, 0).matIds = {1};
    emitObjectsSingle(f.w, f.prj, f.db, 1, s6.get(), false);
    check("objects single variant", f.take(),
          "OBJTYP\t\t1\t0\nKFREAD    \t1\nObjects/Object00001.KEY\nMTLGRP    \t1\t0\t1\n"
          "OBJTYP    \t1\t0\nOBJTYP    \t2\t0\n");   // the closing pass does test type 7
    emitObjectsSingle(f.w, f.prj, f.db, 1, s6.get(), true);
    check("objects single variant, single", f.take(), "MTLGRP    \t1\t0\t1\nOBJTYP    \t1\t0\n");
}

// ---- 140135da0, 1401360e0, 1401363b0, 1401365f0, 140136810 ------------------------------------------
static void test_kfread_family()
{
    Fixture f;
    auto o = f.op("Operations/Task00002", {-2, 2, 3});
    ent(*o, 0).objectType = 5;
    ent(*o, 0).equipmentId = -1;
    ent(*o, 1).objectType = 1;
    ent(*o, 1).equipmentId = 2;
    ent(*o, 2).objectType = 1;
    ent(*o, 2).equipmentId = 3;

    emitContact(f.w, f.prj, f.db, 2, o.get());
    check("cntact", f.take(), "CNTACT    \t0\nKFREAD    \t1\nOperations/Task00002/InterObject.KEY\n");

    emitEquipment(f.w, f.prj, f.db, 2, o.get());
    check("impeqp", f.take(),
          "IMPEQP    \t2\nEquipments/Equipment00002.KEY\nIMPEQP    \t3\nEquipments/Equipment00003.KEY\n");
    ent(*o, 2).equipmentId = 7;   // no such item: null path, no line
    emitEquipment(f.w, f.prj, f.db, 2, o.get());
    check("impeqp unknown", f.take(), "IMPEQP    \t2\nEquipments/Equipment00002.KEY\n");

    emitObjProperties(f.w, f.prj, f.db, 2, o.get());
    check("objprops", f.take(), "KFREAD    \t1\nOperations/Task00002/ObjProperties00001.KEY\n");

    emitBccDef(f.w, f.prj, f.db, 2, o.get());
    check("bccdef off", f.take(), "");
    ent(*o, 0).defineBCC = true;
    ent(*o, 1).defineBCC = true;  // type 1: not written
    emitBccDef(f.w, f.prj, f.db, 2, o.get());
    check("bccdef", f.take(), "KFREAD    \t1\nOperations/Task00002/BccDef00001.KEY\n");

    ent(*o, 1).meshSetting = true;
    ent(*o, 2).meshSetting = true;
    ent(*o, 0).remesh = true;
    ent(*o, 2).remesh = true;
    emitMeshSettings(f.w, f.prj, f.db, 2, o.get());
    check("meshsettings", f.take(),
          "KFREAD    \t1\nOperations/Task00002/MeshSettings00002.KEY\n"
          "KFREAD    \t1\nOperations/Task00002/MeshSettings00003.KEY\n"
          "REMESH    \t1\nREMESH    \t3\n");

    auto none = f.op("Operations/Task00009", {});
    emitMeshSettings(f.w, f.prj, f.db, 2, none.get());
    emitBccDef(f.w, f.prj, f.db, 2, none.get());
    emitObjProperties(f.w, f.prj, f.db, 2, none.get());
    emitEquipment(f.w, f.prj, f.db, 2, none.get());
    check("no objects", f.take(), "");
}

// ---- 140135f10, 140065f80, 140065af0, 140135fc0, 140136040 / 1400ac010 -------------------------------
static void test_objpos_tloc_contact()
{
    Fixture f;
    auto o = f.op("Operations/Task00001", {1});
    auto add = [&](int mode, int obj) -> ObjPos& {
        o->objpos.push_back(std::make_unique<ObjPos>());
        ObjPos& p = *o->objpos.back();
        p.mode = mode;
        p.object = obj;
        return p;
    };
    // COG_P1_B1 simulation 3 lines and VDIE's line with an id list
    ObjPos& a = add(2, 2);
    a.d(0x10) = 0.0001;
    a.d(0x28) = -1;
    a.i08 = 1;
    ObjPos& b = add(3, 1);
    b.d(0x60) = 1;
    ObjPos& c = add(2, 4);
    c.d(0x10) = 0.0001;
    c.d(0x28) = -1;
    c.i08 = 5;
    c.list = {-1, -2, -3};
    emitObjpos(f.w, f.prj, f.db, 3, o.get());
    check("objpos 3D corpus", f.take(),
          "OBJPOS\t2\t2\t0.0001\t0\t0\t-1\t1\t0\t0\n"
          "OBJPOS\t1\t3\t0\t0\t0\t1\t0\t0\t0\n"
          "OBJPOS\t4\t2\t0.0001\t0\t0\t-1\t5\t0\t0\t3\n-1\t-2\t-3\n");

    // every 3D form, values = byte offset / 8
    auto fill = [](ObjPos& p) {
        for (unsigned off = 0x10; off <= 0xc8; off += 8) p.d(off) = off / 8.0;
        p.i08 = 77;
    };
    const char* want3[] = {
        "OBJPOS\t9\t1\t6\t7\t8\t0\t0\t0\t0\n",            // 1
        "OBJPOS\t9\t2\t2\t3\t4\t5\t77\t0\t0\n",           // 2
        "OBJPOS\t9\t3\t9\t10\t11\t12\t13\t14\t15\n",      // 3
        "OBJPOS\t9\t4\t\n",                               // 4: nothing
        "OBJPOS\t9\t5\t\n",                               // 5: nothing
        "OBJPOS\t9\t6\t12\t13\t14\t15\t0\t0\t0\n",        // 6
        "OBJPOS\t9\t7\t2\t3\t4\t5\t0\t0\t0\n",            // 7
        "OBJPOS\t9\t8\t2\t3\t4\t5\t0\t0\t0\n",            // 8
        "OBJPOS\t9\t9\t2\t3\t4\t5\t12\t13\t14\n",         // 9
    };
    for (int m = 1; m <= 9; ++m) {
        ObjPos p;
        p.mode = m;
        p.object = 9;
        fill(p);
        emitObjposLine3D(f.w, p);
        check("objpos 3D mode", f.take(), want3[m - 1]);
    }
    for (int m : {11, 12, 102, 112}) {
        ObjPos p;
        p.mode = m;
        p.object = 9;
        fill(p);
        emitObjposLine3D(f.w, p);
        std::string head = "OBJPOS\t9\t" + std::to_string(m) + "\t";
        check("objpos 3D alias", f.take(),
              head + (m == 11 ? "6\t7\t8\t0\t0\t0\t0\n" : "2\t3\t4\t5\t77\t0\t0\n"));
    }
    const char* want2[] = {
        "OBJPOS\t9\t1\t6\t7\t0\t0\n",       // 1
        "OBJPOS\t9\t2\t2\t77\t3\t4\n",      // 2
        "OBJPOS\t9\t3\t9\t10\t15\t0\n",     // 3
        "OBJPOS\t9\t4\t16\t17\t19\t20\n",   // 4
        "OBJPOS\t9\t5\t\n",                 // 5: nothing
        "OBJPOS\t9\t6\t15\t0\t0\t0\n",      // 6
        "OBJPOS\t9\t7\t\n",                 // 7: nothing
    };
    for (int m = 1; m <= 7; ++m) {
        ObjPos p;
        p.mode = m;
        p.object = 9;
        fill(p);
        emitObjposLine2D(f.w, p);
        check("objpos 2D mode", f.take(), want2[m - 1]);
    }
    {   // MO2_Boolean_Oprations 2D line, one list entry, and a "nothing" form with a list
        ObjPos p;
        p.mode = 2;
        p.object = 3;
        p.d(0x10) = 0.0001;
        p.i08 = 1;
        p.d(0x20) = -1;
        emitObjposLine2D(f.w, p);
        check("objpos 2D corpus", f.take(), "OBJPOS\t3\t2\t0.0001\t1\t0\t-1\n");
        p.mode = 5;
        p.list = {7};
        emitObjposLine2D(f.w, p);
        check("objpos list after nothing", f.take(), "OBJPOS\t3\t5\t\t1\n7\n");
    }
    p09test::g_is2D = true;
    auto o2 = f.op("Operations/Task00001", {1});
    o2->objpos.push_back(std::make_unique<ObjPos>());
    o2->objpos.back()->mode = 12;
    o2->objpos.back()->object = 2;
    o2->objpos.back()->d(0x10) = 0.0001;
    o2->objpos.back()->i08 = 1;
    o2->objpos.back()->d(0x20) = -1;
    emitObjpos(f.w, f.prj, f.db, 1, o2.get());
    check("objpos dispatch 2D", f.take(), "OBJPOS\t2\t12\t0.0001\t1\t0\t-1\n");
    p09test::g_is2D = false;

    emitTloc(f.w, f.prj, f.db, 1, o.get());
    o->tloc = -1;
    emitTloc(f.w, f.prj, f.db, 1, o.get());
    o->tloc = std::numeric_limits<double>::quiet_NaN();
    emitTloc(f.w, f.prj, f.db, 1, o.get());
    check("tloc off", f.take(), "");
    o->tloc = 2.5;
    emitTloc(f.w, f.prj, f.db, 1, o.get());
    check("tloc", f.take(), "TLOC    \t2.5\n");

    // HEAT1 / COG_P1_B1: defaults give "INICTC 1" and "GENCTC 0"
    emitContactFlags(f.w, f.prj, f.db, 1, o.get());
    check("contact defaults", f.take(), "INICTC    \t1\nGENCTC    \t0\n");
    o->icb.flag[2] = 1;
    o->icb.flag[99] = 1;
    o->icb.genVal = 0.5;
    emitContactFlags(f.w, f.prj, f.db, 1, o.get());
    check("contact flags", f.take(), "INICTC    \t1\nINICTC    \t3\nINICTC    \t100\nGENCTC    \t0.5\n");
    o->icb.enable = 0;
    emitContactBlock(f.w, o->icb);
    check("contact disabled", f.take(), "GENCTC    \t0.5\n");
    o->icb.gen = 0;
    emitContactBlock(f.w, o->icb);
    check("contact off", f.take(), "");
}

// ---- 1400613b0, 14004e830, 14004f1c0, 14004f050, 14089c490/14089c2a0 -----------------------------
static void set_all(StateVars& sv)
{
    sv.ndtmp = {1, 20.5};
    sv.strain = {1, 0.1};
    sv.stress = {1, 100};
    sv.damage = {1, 0.01};
    sv.urzSet = 1;
    sv.urz[0] = 1, sv.urz[1] = 2, sv.urz[2] = 3;
    sv.drzSet = 1;
    sv.drz[0] = 4, sv.drz[1] = 5, sv.drz[2] = 6;
    sv.grain = {1, 30};
    sv.sizesh = {1, 0.5};
    sv.densty = {1, 0.99};
    sv.avgstr = {1, 0.2};
    sv.lmtstr = {1, 0.3};
}

static void test_state_vars()
{
    Fixture f;
    auto o = f.op("Operations/Task00001", {1});
    StateVars& sv = ent(*o, 0).sv;
    set_all(sv);
    emitSv2D(f.w, sv);
    check("sv 2D", f.take(),
          "NDTMP    \t1\t0\t20.5\nSTRAIN    \t1\t0\t0.1\nSTRESS    \t1\t0\t100\nDAMAGE    \t1\t0\t0.01\n"
          "URZ    \t1\t0\t1\t2\t\nDRZ    \t1\t0\t4\t5\t\nGRAIN    \t1\t0\t16\t0\t30\t30\n"
          "SIZESH    \t1\t0.5\t0\t0\t0\nDENSTY    \t1\t0\t0.99\nAVGSTR    \t1\t0.2\nLMTSTR    \t1\t0.3\n");
    sv.nrecvlFlag = 1, sv.nrecvlCount = 4;
    sv.erecvlFlag = 1, sv.erecvlCount = 2, sv.erecvlValue = 1.5;
    emitSv3D(f.w, sv);
    check("sv 3D", f.take(),
          "NDTMP    \t1\t0\t20.5\nSTRAIN    \t1\t0\t0.1\nSTRESS    \t1\t0\t100\nDAMAGE    \t1\t0\t0.01\n"
          "URZ    \t1\t0\t1\t2\t3\nDRZ    \t1\t0\t4\t5\t6\nGRAIN    \t1\t0\t16\t0\t0\t30\n"
          "SIZESH    \t1\t0.5\t0\t0\t0\nDENSTY    \t1\t0\t0.99\nAVGSTR    \t1\t0.2\nLMTSTR    \t1\t0.3\n"
          "NRECVL    \t1\t0\t0\t4\t1\nERECVL    \t1\t0\t0\t2\t1.5\n");
    sv = StateVars{};
    sv.objNumber = 1;
    sv.urzSet = 0x100;   // only the low byte is tested
    emitSv3D(f.w, sv);
    check("sv flag low byte", f.take(), "");

    // SVSCAL: table kind 1 -> index i * dim[3] + 0
    sv.svscal = 1;
    sv.svCount = 3;
    sv.svX = {0, 0.5, 1};
    sv.svTabKind = 1;
    sv.svTabDim[0] = 3, sv.svTabDim[3] = 2;
    sv.svTable = {10, 11, 20, 21, 30, 31};
    emitSvscal(f.w, sv);
    check("svscal kind 1", f.take(), "SVSCAL    \t1\t1\t3\n0\t10\n0.5\t20\n1\t30\n");
    sv.svTabKind = 2;    // general: j*d1 + i + ..., with d = max(1, dim)
    emitSvscal(f.w, sv);
    check("svscal kind 2", f.take(), "SVSCAL    \t1\t1\t3\n0\t10\n0.5\t11\n1\t20\n");
    sv.svTabKind = 0;    // no table: 0.0
    emitSvscal(f.w, sv);
    check("svscal no table", f.take(), "SVSCAL    \t1\t1\t3\n0\t0\n0.5\t0\n1\t0\n");
    check("svtab index range", std::to_string(svTableIndex(sv, 0, 0, 0, 0)), "-1");
    sv.svTabKind = 2;
    check("svtab index i out", std::to_string(svTableIndex(sv, 3, 0, 0, 0)), "-1");
    check("svtab index j out", std::to_string(svTableIndex(sv, 0, 1, 0, 0)), "-1");
    check("svtab index l", std::to_string(svTableIndex(sv, 1, 0, 0, 1)), "4");

    // 1400613b0: per object 2D/3D lines + SVSCAL, then NRECVL/ERECVL from lists A and B (repeated
    // for every object). A: node 1.5 (flag F[0]=0), element 2, node 3 (F[2]=1); B: element 4, type 7.
    Fixture g;
    auto op = g.op("Operations/Task00001", {1, 2});
    ent(*op, 0).sv.ndtmp = {1, 20};
    *recoveryListA(*op) = {{0, 1.5}, {1, 2}, {0, 3}};
    op->recoveryFlags = std::make_shared<std::vector<bool>>(std::vector<bool>{false, false, true});
    *op->recoveryB = {{1, 4}, {7, 9}};
    emitStateVars(g.w, g.prj, g.db, 1, op.get());
    const std::string rec =
        "NRECVL    \t#\t0\t1\t2\n0,    1\n1.5,    3\n"
        "ERECVL    \t#\t0\t1\t3\n0,    1,    2,    4,    ";   // the type 7 entry is counted but
                                                               // not printed, so no line ends
    std::string r1 = rec, r2 = rec;
    r1.replace(r1.find('#'), 1, "1");
    r1.replace(r1.find('#'), 1, "1");
    r2.replace(r2.find('#'), 1, "2");
    r2.replace(r2.find('#'), 1, "2");
    check("recovery", g.take(), "NDTMP    \t1\t0\t20\n" + r1 + r2);
    // F empty: NRECVL flags default 0, ERECVL flags default 1
    op->recoveryFlags->clear();
    *recoveryListA(*op) = {{0, 1}, {1, 2}};
    op->recoveryB->clear();
    emitObjectStateVars(g.w, *op);
    check("recovery defaults", g.take(),
          "NDTMP    \t1\t0\t20\nNRECVL    \t1\t0\t1\t1\n0\n1\nERECVL    \t1\t0\t1\t1\n1\n2\n"
          "NRECVL    \t2\t0\t1\t1\n0\n1\nERECVL    \t2\t0\t1\t1\n1\n2\n");
    p09test::g_is2D = true;
    recoveryListA(*op)->clear();
    ent(*op, 0).sv.urzSet = 1;
    emitObjectStateVars(g.w, *op);
    check("state vars 2D choice", g.take(), "NDTMP    \t1\t0\t20\nURZ    \t1\t0\t0\t0\t\n");
    p09test::g_is2D = false;
    auto empty = g.op("Operations/Task00002", {});
    emitStateVars(g.w, g.prj, g.db, 1, empty.get());
    check("state vars none", g.take(), "");
}

// ---- 140132a40, 1401322d0, 140132bd0, 14093a200 ---------------------------------------------------
struct FakeDoe : doe::Model {
    std::vector<doe::Var*> geo;
    int askedOp = -1, askedObj = -1;
    std::vector<doe::Var*> geometryVars(int opId, int objId) const override
    {
        auto* self = const_cast<FakeDoe*>(this);
        self->askedOp = opId;
        self->askedObj = objId;
        return objId == 2 ? geo : std::vector<doe::Var*>{};
    }
};

static void test_doe()
{
    check("lines", std::to_string(doeLineCount(String(u"A\n\nB\n"))), "2");
    check("lines empty", std::to_string(doeLineCount(String(u""))), "0");

    Fixture f;
    auto o = f.op("Operations/Task00003", {1, 2});
    emitDoeVariables(f.w, o.get());
    emitDoeGeometry(f.w, o.get());
    check("doe off", f.take(), "");

    doe::Var base;
    base.name = String(u"Speed");
    base.index = 0;
    base.number = 1;
    base.items.emplace_back(String(u"SPEED 1\n"), String(u"SPEED <x>\n"));
    doe::Var linked;
    linked.name = String(u"Speed2");
    linked.index = 1;
    linked.number = 2;
    linked.link = &base;
    f.st.doeMode = true;
    f.st.doeVarList = {&base, &linked};
    f.st.doeObjMap = {{0, 0}, {1, 0}};
    emitDoeVariables(f.w, o.get());
    check("doe vars", f.take(),
          "* DOE Variables \n\n* DOE Study \n* (1) Speed\nDOEVAR\t1\t1\t1\nSPEED 1\n\nSPEED <x>\n\n"
          "\n* DOE Study \n* Link: 2->1:0\n* (1) Speed2\n\n");

    doe::Var g1;
    g1.name = String(u"Die");
    g1.index = 2;
    g1.number = 3;
    g1.geoType = 100;
    g1.files = {{String(u"C:/s/Control/die0.GEO"), String(u"C:/s/Control/dieA.GEO"),
                 String(u"C:/s/Control/dieB.geo"), true, true}};
    FakeDoe model;
    model.geo = {&g1};
    f.st.doeModel = &model;
    f.st.doeOpId = 3;
    f.st.doeObjMap = {{2, 1}};
    ent(*o, 1).hasMesh = true;
    ent(*o, 1).name = String(u"Top Die");
    emitDoeGeometry(f.w, o.get());
    check("doe geometry", f.take(),
          "* DOE Geometry Variables \n* Geometry change : Top Die\n* DOE Study \n* (3) Die\n"
          "DOEVAR\t0\t8\t1\t1\nMORP\t1\t102\n2\n../../Control/die0.GEO\n"
          "../../Control/dieA.GEO\n../../Control/dieA.COR\n"
          "../../Control/dieB.geo\n.COR\n"
          "OutputGeoOp3Obj2.GEO\n\nGFREAD\t2\nOutputGeoOp3Obj2.GEO\n\nDEFAMG\t2\n\nGENBCC\t2\n\n");
    check("doe asked", std::to_string(model.askedOp) + "," + std::to_string(model.askedObj), "3,2");
    g1.files[0].useBefore = false;
    g1.geoType = 0;
    ent(*o, 1).hasMesh = false;
    emitDoeGeometryBlock(f.w, o.get(), 3, 2, model.geo, std::map<int, int>{});
    check("doe geometry NA", f.take(),
          "* Geometry change : Top Die\n* DOE Study \n* (3) Die\n"
          "DOEVAR\t0\t8\t1\t1\nMORP\t1\t2\n3\n../../Control/die0.GEO\nNA\nNA\n"
          "../../Control/dieB.geo\n.COR\n"
          "OutputGeoOp3Obj2.GEO\n\nGFREAD\t2\nOutputGeoOp3Obj2.GEO\n");
}

// ---- 0x14091bbb0, 0x14091bcb0, 0x14091bdb0 and the slots they call -------------------------------
static void test_doe_model()
{
    doe::Var a, b, c, d;
    a.ops = {1, 3};
    b.ops = {3};
    b.link = &a;
    c.ops = {2};
    d.ops = {3, 3};
    d.link = &c;
    a.index = b.index = c.index = d.index = 99;
    FakeDoe m;
    m.vars = {&a, &b, &c, &d};
    auto ids = [](const std::vector<doe::Var*>& l) {
        std::string s;
        for (auto* v : l) s += std::to_string(v->index) + ",";
        return s;
    };
    check("vars for op 3", ids(m.varsForOperation(3)), "0,1,3,");
    check("vars for op 2", ids(m.varsForOperation(2)), "2,");
    check("vars for op 9", ids(m.varsForOperation(9)), "");
    check("unlinked", ids(m.unlinkedVars()), "0,2,");
    check("var at", std::string(m.varAt(4) ? "x" : "null") + (m.varAt(-1) ? "x" : "null") +
                        (m.varAt(2) == &c ? "c" : "?"), "nullnullc");
    std::string lm;
    for (const auto& kv : m.linkMap()) lm += std::to_string(kv.first) + ":" + std::to_string(kv.second) + " ";
    check("link map", lm, "0:0 1:0 2:1 3:1 ");
    c.link = &d;   // a linked target is not among the unlinked vars: -1
    lm.clear();
    for (const auto& kv : m.linkMap()) lm += std::to_string(kv.first) + ":" + std::to_string(kv.second) + " ";
    check("link map to linked", lm, "0:0 1:0 2:-1 3:-1 ");
}

int main()
{
    struct T {
        const char* name;
        void (*fn)();
    } tests[] = {{"objects", test_objects}, {"kfread", test_kfread_family},
                 {"objpos/tloc/contact", test_objpos_tloc_contact}, {"state vars", test_state_vars},
                 {"doe", test_doe}, {"doe model", test_doe_model}};
    int skipped = 0;
    for (const T& t : tests) {
        try {
            t.fn();
        } catch (const NotPorted& e) {
            ++skipped;
            std::printf("SKIP %s: %s\n", t.name, e.what());
        }
    }
    std::printf("p09 emit_b: %d checks, %d failed, %d groups skipped\n", g_checks, g_fail, skipped);
    return g_fail ? 1 : 0;
}
