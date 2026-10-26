// ctest-labels: m2m-p12
// Tier-1 test of the object slots, OBJ defaults and dimension accessors (src/model/objects.cpp,
// s1-01 5.4, 9.3, 9.4, 10), with the expected values taken from the decompiles named there.
#include <cstdio>
#include <string>

#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/objects.hpp"

using namespace m2m;

namespace {

int failures = 0;

void checkInt(const char* what, long long got, long long want)
{
    if (got == want) {
        std::printf("ok   %s\n", what);
        return;
    }
    ++failures;
    std::printf("FAIL %s: got %lld, want %lld\n", what, got, want);
}

void testSlots()
{
    Operation op;
    checkInt("objSlots empty", objSlots(op), 0);
    checkInt("objId out of range = -99", objId(op, 0), -99);
    checkInt("objId negative = -99", objId(op, -1), -99);
    checkInt("setObjId negative slot", setObjId(op, -1, 5), 0);
    checkInt("setObjId slot 2", setObjId(op, 2, 7), 1);
    checkInt("padding to slot 2", objSlots(op), 3);
    checkInt("padding value -1 (slot 0)", objId(op, 0), -1);
    checkInt("padding value -1 (slot 1)", objId(op, 1), -1);
    checkInt("slot 2 id", objId(op, 2), 7);
    checkInt("objIdN(3)", objIdN(op, 3), 7);
    checkInt("setObjId existing slot", setObjId(op, 0, -2), 1);
    checkInt("slot 0 id -2 (Read From DB)", objId(op, 0), -2);
    checkInt("no entries yet", objCount(op), 0);

    // 0x140061250: entries are appended up to the slot, numbered index + 1.
    ObjEntry* e2 = objEntry(op, 2);
    checkInt("objEntry(2) not null", e2 != nullptr, 1);
    checkInt("entries appended", objCount(op), 3);
    checkInt("entry 0 number", op.objs[0]->number, 1);
    checkInt("entry 2 number", e2->number, 3);
    checkInt("entry 2 sv number", e2->sv.objNumber, 3);
    checkInt("objEntry same pointer", objEntry(op, 2) == e2, 1);
    checkInt("objEntryN(3)", objEntryN(op, 3) == e2, 1);
    checkInt("objEntry(3) beyond slots", objEntry(op, 3) == nullptr, 1);
    checkInt("objEntry(-1)", objEntry(op, -1) == nullptr, 1);
    checkInt("objEntryN(0)", objEntryN(op, 0) == nullptr, 1);
    checkInt("no growth past slots", objCount(op), 3);
}

void testObjDefaults()
{
    auto o = newObj();
    checkInt("OBJ number 0", o->number, 0);
    checkInt("OBJ name null", o->name.isNull(), 1);
    checkInt("OBJ type 7", o->objectType, 7);
    checkInt("OBJ +0x08 = 7", o->f08, 7);
    checkInt("OBJ EquipmentID 0", o->equipmentId, 0);
    checkInt("OBJ flags 0", o->remesh + o->meshSetting + o->skipDefine + o->hasGeo + o->hasMesh, 0);
    checkInt("SV NRECVL flag 1", o->sv.nrecvlFlag, 1);
    checkInt("SV ERECVL flag 1", o->sv.erecvlFlag, 1);
    checkInt("SV values 0", o->sv.ndtmp.set + o->sv.strain.set + o->sv.nrecvlCount, 0);

    o->remesh = true;
    o->matIds = {1, 2};
    o->equipmentId = 4;
    o->sv.objNumber = 9;
    objDefaults(*o);
    checkInt("defaults clear remesh", o->remesh, 0);
    checkInt("defaults clear MatId", static_cast<long long>(o->matIds.size()), 0);
    checkInt("defaults EquipmentID 0", o->equipmentId, 0);
    checkInt("defaults reset SV number", o->sv.objNumber, 0);

    setObjNumber(*o, 4);
    checkInt("setObjNumber", o->number, 4);
    checkInt("setObjNumber into SV", o->sv.objNumber, 4);
}

void testDims()
{
    Project prj;
    prj.registry[String(u"SfOprBoolean2D")] = mst::makeBoolean(true);
    prj.registry[String(u"SfOprBoolean3D")] = mst::makeBoolean(false);
    Operation op;
    op.project = &prj;

    op.classKey = String(u"SfOprBoolean2D");
    checkInt("2D class dim", opDim(op), 2);
    checkInt("2D class is2D", is2D(op), 1);
    checkInt("2D class is3D", is3D(op), 0);

    op.classKey = String(u"SfOprBoolean3D");
    checkInt("3D class dim", opDim(op), 3);
    checkInt("3D class is2D", is2D(op), 0);
    checkInt("3D class is3D", is3D(op), 1);

    op.classKey = String(u"NoSuchClass");
    checkInt("no class dim 3", opDim(op), 3);
    checkInt("no class is2D false", is2D(op), 0);
    checkInt("no class is3D true", is3D(op), 1);
}

}  // namespace

int main()
{
    testSlots();
    testObjDefaults();
    testDims();
    std::printf("%s (%d failures)\n", failures ? "FAILED" : "PASSED", failures);
    return failures ? 1 : 0;
}
