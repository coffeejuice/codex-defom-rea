// ctest-labels: m2m-p12
// ctest-standalone: src/ops/boolean.cpp src/ops/convert23.cpp src/ops/copymirror.cpp src/core/basic.cpp tests/p12/doubles.cpp src/ops/props_base.cpp src/model/props_rest_slots.cpp
// Tier-1 test of the P12 writers (s1-04 1, 4, 5) against the test doubles in doubles.cpp: the
// writer-owned lines byte for byte, the emitter calls with their exact arguments, the ICB changes,
// the simulation counter and the onlySim / wrong-props branches. The Boolean expectation is the
// SIMULATION 4 block of the corpus MST MO2_Boolean_Oprations with the emitter lines replaced by
// markers.
#include <cstdio>
#include <memory>
#include <string>

#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"

#include "doubles.hpp"

#if __has_include("m2m/data/boolean.hpp")
#include "m2m/data/boolean.hpp"
#else
namespace m2m::data {
struct BooleanData {
    int workpiece = 1;
    int cutter = 0;
    std::uint8_t remesh = 0;
};
}  // namespace m2m::data
#endif

using namespace m2m;
using namespace m2m::mst;

namespace {

int failures = 0;

void check(const char* what, const std::string& got, const std::string& want)
{
    if (got == want) {
        std::printf("ok   %s\n", what);
        return;
    }
    ++failures;
    size_t i = 0;
    while (i < got.size() && i < want.size() && got[i] == want[i]) ++i;
    std::printf("FAIL %s (first difference at byte %zu)\n--- got\n%s\n--- want\n%s\n", what, i,
                got.c_str(), want.c_str());
}

void checkInt(const char* what, int got, int want)
{
    check(what, std::to_string(got), std::to_string(want));
}

struct Fixture {
    Project prj;
    Operation op;
    Ctx ctx;
    State st;
    Stream ts{1252};
    W w{ts, st, ctx};
    String db = String(u"MO2_Boolean_Oprations.DB");

    Fixture(const char16_t* name, const char16_t* task, int id)
    {
        op.project = &prj;
        op.name = String(name);
        op.taskDir = String(task);
        op.id = id;
    }
};

void testBoolean()
{
    auto cls = makeBoolean(true);
    checkInt("Boolean key", cls->key() == String(u"SfOprBoolean2D"), 1);
    check("Boolean displayName", qt::toAscii(cls->displayName()), "Boolean");

    Fixture f(u"Boolean", u"Operations/Task00004", 4);
    f.op.props = cls->makeProps(1);
    p12test::flowNumberValue = 4;
    int sim = 3;
    cls->current = &f.op;
    cls->writeMst(f.w, f.prj, f.db, &sim, &f.op, 0, 0);
    checkInt("Boolean sim", sim, 4);
    check("Boolean block", f.ts.bytes(),
          "[140131c10 4]\n"
          "[1401343e0 4 0]\n"
          "[140134db0 1]\n"
          "[1401360e0 1]\n"
          "[140135220 1 0]\n"
          "[140135f10 4]\n"
          "[140134680]\n"
          "[140134730 Boolean]\n"
          "[1401347e0 Boolean]\n"
          "[140134900 4 4 0]\n"
          "NSTEP\t0\n"
          "[140136040 4]\n"
          "INICTC    \t1\n"
          " * Keyword file which contains objects \n"
          "KFWRIT\n"
          "Operations/Task00004/Step.KEY\n"
          "\n"
          "EXECMD\t4\n"
          "DEF_MACHDIST_CTRL23.EXE   Operations/Task00004/DEF_MACHDIST_CTRL2.INI  >  "
          "Operations/Task00004/DEF_MACHDIST_CTRL2.OUT\n"
          "KFREAD    \t1\n"
          "Operations/Task00004/DEF_MACHDIST_OUT.KEY\n"
          "[140134a10 4 2 1]\n");
    checkInt("Boolean GENCTC off in the model", f.op.icb.gen, 0);

    // Workpiece object 3 from the BWZ: flag[2] set and kept in the model.
    Fixture g(u"Cut", u"Operations/Task00009", 9);
    auto props = cls->makeProps(1);
    static_cast<BooleanProps*>(props.get())->data->workpiece = 3;
    g.op.props = props;
    sim = 0;
    cls->writeMst(g.w, g.prj, g.db, &sim, &g.op, 0, 0);
    checkInt("Boolean workpiece 3 flag", g.op.icb.flag[2], 1);
    checkInt("Boolean workpiece 3 INICTC", g.ts.bytes().find("INICTC    \t1\nINICTC    \t3\n") != std::string::npos, 1);

    // onlySim skips the block but still counts.
    Fixture h(u"Boolean", u"Operations/Task00004", 4);
    h.op.props = cls->makeProps(1);
    sim = 3;
    cls->writeMst(h.w, h.prj, h.db, &sim, &h.op, 0, 2);
    checkInt("Boolean onlySim sim", sim, 4);
    check("Boolean onlySim output", h.ts.bytes(), "");

    // Props of another class: the dynamic_cast fails, nothing is written or counted.
    Fixture k(u"Boolean", u"Operations/Task00004", 4);
    k.op.props = std::make_shared<Props>();
    sim = 3;
    cls->writeMst(k.w, k.prj, k.db, &sim, &k.op, 0, 0);
    checkInt("Boolean wrong props sim", sim, 3);
    check("Boolean wrong props output", k.ts.bytes(), "");

    checkInt("Boolean3D key", makeBoolean(false)->key() == String(u"SfOprBoolean3D"), 1);
    checkInt("Boolean2D dimFlags", makeBoolean(true)->dimFlags, 0x1000099);
    checkInt("Boolean3D dimFlags", makeBoolean(false)->dimFlags, 0x100009a);
}

void testConvert2Dto3D()
{
    auto cls = makeConvert2Dto3D();
    checkInt("Convert2Dto3D key", cls->key() == String(u"SfOprConvert2Dto3D"), 1);
    checkInt("Convert2Dto3D dimFlags", cls->dimFlags, 0x1100009d);

    Fixture f(u"Convert", u"Operations/Task00007", 7);
    f.op.props = cls->makeProps(1);
    p12test::flowNumberValue = 6;
    p12test::startsFromKeyValue = false;
    cls->current = &f.op;
    int sim = 5;
    cls->writeMst(f.w, f.prj, f.db, &sim, &f.op, 0, 0);
    checkInt("Convert2Dto3D sim", sim, 6);
    check("Convert2Dto3D block", f.ts.bytes(),
          "[140131c10 6]\n"
          "[1401343e0 6 0]\n"
          "[140134db0 6]\n"
          "[140135220 6 0]\n"
          "KFREAD\n"
          "REL(Operations/Task00007/M23_INPUT.KEY)\n"
          "KFREAD\n"
          "REL(Operations/Task00007/SfOprConvert2Dto3D_TEMP_STEP.KEY)\n"
          "[140134730 Convert]\n"
          "[1401347e0 Convert]\n"
          "[140134900 6 6 0]\n"
          "NSTEP\t0\n"
          "[140134a10 6 2 1]\n");

    // First operation (no predecessor or restart key): SimCtrl block instead of DBREAD.
    Fixture g(u"Convert", u"Operations/Task00001", 1);
    g.op.props = cls->makeProps(1);
    p12test::flowNumberValue = 1;
    p12test::startsFromKeyValue = true;
    cls->current = &g.op;
    sim = 0;
    cls->writeMst(g.w, g.prj, g.db, &sim, &g.op, 0, 0);
    check("Convert2Dto3D first", g.ts.bytes().substr(0, 35), "[140131c10 1]\n[140134bf0 1]\n[140134");
    p12test::startsFromKeyValue = false;

    // No current item (a stale GUI state, s1-04 O-3): the directory part is "".
    Fixture h(u"Convert", u"Operations/Task00007", 7);
    h.op.props = cls->makeProps(1);
    cls->current = nullptr;
    sim = 0;
    cls->writeMst(h.w, h.prj, h.db, &sim, &h.op, 0, 0);
    checkInt("Convert2Dto3D no current item",
             h.ts.bytes().find("KFREAD\nREL(/M23_INPUT.KEY)\nKFREAD\nREL(/SfOprConvert2Dto3D_TEMP_STEP.KEY)\n") !=
                 std::string::npos,
             1);

    Fixture k(u"Convert", u"Operations/Task00007", 7);
    k.op.props = cls->makeProps(1);
    sim = 4;
    cls->writeMst(k.w, k.prj, k.db, &sim, &k.op, 0, 9);
    checkInt("Convert2Dto3D onlySim sim", sim, 5);
    check("Convert2Dto3D onlySim output", k.ts.bytes(), "");
}

void testCopyMirror()
{
    auto cls = makeCopyMirror();
    checkInt("CopyMirror key", cls->key() == String(u"SfOprCopyMirror"), 1);
    checkInt("CopyMirror dimFlags", cls->dimFlags, 0x2100009a);
    check("CopyMirror displayName", qt::toAscii(cls->displayName()), "Copy/Mirroring");

    Fixture f(u"Mirror", u"Operations/Task00003", 3);
    f.op.props = cls->makeProps(1);
    f.op.icb.flag[1] = 1;
    p12test::flowNumberValue = 3;
    cls->current = &f.op;
    int sim = 2;
    cls->writeMst(f.w, f.prj, f.db, &sim, &f.op, 0, 0);
    checkInt("CopyMirror sim", sim, 3);
    check("CopyMirror block", f.ts.bytes(),
          "[140131c10 3]\n"
          "[1401343e0 3 0]\n"
          "[140134bf0 1]\n"
          "[140134db0 1]\n"
          "[1401360e0 1]\n"
          "[140135220 1 0]\n"
          "[140134680]\n"
          "[140134730 Mirror]\n"
          "[1401347e0 Mirror]\n"
          "[140134900 3 3 0]\n"
          "[140136040 3]\n"
          "INICTC    \t1\n"
          "INICTC    \t2\n"
          " * Keyword file which contains objects \n"
          "KFWRIT\n"
          "Operations/Task00003/Step.KEY\n"
          "\n"
          "EXECMD\t3\n"
          "DEF_MIRRORMERGE.EXE   Operations/Task00003/mirrormerge.ini  >  "
          "Operations/Task00003/DEF_MIRRORMERGE.OUT\n"
          "KFREAD\n"
          "Operations/Task00003/DEF_MIRRORMERGE_OUT.KEY\n"
          "[140134a10 3 2 1]\n"
          "ARMCTL\t0\t0\t4\n");
    checkInt("CopyMirror GENCTC off in the model", f.op.icb.gen, 0);

    Fixture k(u"Mirror", u"Operations/Task00003", 3);
    k.op.props = std::make_shared<Props>();
    sim = 2;
    cls->writeMst(k.w, k.prj, k.db, &sim, &k.op, 0, 0);
    checkInt("CopyMirror wrong props sim", sim, 2);
}

}  // namespace

int main()
{
    testBoolean();
    testConvert2Dto3D();
    testCopyMirror();
    std::printf("%s (%d failures)\n", failures ? "FAILED" : "PASSED", failures);
    return failures ? 1 : 0;
}
