// Operation class registry (P02 rescue slice, coordinator 2026-10-22: P02 had not pushed).
// s1-01 6.1; decompiles s1-01 dc/reg1.c (0x14006f940), dc/reg2.c (0x14006dba0), s1-02 a2s.c
// (0x140054e90), a3s.c (0x140070650).
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/p17.hpp"

namespace m2m {
namespace {

// @src 0x14006f940  insert: OB->vt[0x180]() (init: the class key, set by our factories),
// info = OB->vt[0x188](), info->vt[0x70](reg), map1[info name] = info (not modelled),
// map2[info name] = OB (QMap<QString, SfOprBase*>, a later key replaces an earlier one).
void insertClass(Project& p, std::unique_ptr<mst::OpClass> ob)
{
    mst::classInit(*ob);                  // OB->vt[0x180]() (P17)
    const String key = ob->key();
    p.registry[key] = std::move(ob);
}

}  // namespace

// @src 0x14006dba0  one singleton per class, inserted in this order (reg+8 slot in brackets).
// SfOprGroup and SfOprCopyMirror are NOT registered here: 0x14006f940 has no other caller (callees
// closure), and nothing calls their ctors, so a <Type> naming either class fails the open
// ("Error on reading project file!") as in the original. No corpus project uses them.
void makeRegistry(Project& p)
{
    using namespace mst;
    // first a 0x1d0 object (0x1400706f0) that is not inserted
    insertClass(p, makeForming(false));             // [0x88]  0x140c05af0 SfOprForming
    insertClass(p, makeForming(true));              // [0x90]  0x140c04310 SfOprForming2D
    insertClass(p, makeHeatTransfer(false));        // [0x98]  0x141353df0 SfOprHeatTransfer
    insertClass(p, makeHeatTransfer(true));         // [0xa0]  0x141353000 SfOprHeatTransfer2D
    insertClass(p, makeMultiBlow(false));           // [0xa8]  0x140601410 SfOprMultiBlow3D
    insertClass(p, makeMultiBlow(true));            // [0xb0]  0x1406009e0 SfOprMultiBlow2D
    insertClass(p, makeFormingExpress(false));      // [0xb8]  0x141412f90 SfOprFormingExpress
    insertClass(p, makeFormingExpress(true));       // [0xc0]  0x141412680 SfOprFormingExpress2D
    insertClass(p, makeDistortion(false));          // [0xc8]  0x141361f70(0) SfOprDistortion3D
    insertClass(p, makeDistortion(true));           // [0xd0]  0x141361f70(1) SfOprDistortion2D
    insertClass(p, makeHeatTransferExpress(false)); // [0x18]  0x1413e5f90 SfOprHeatTransferExpress
    insertClass(p, makeHeatTransferExpress(true));  // [0x20]  0x1413e56d0 SfOprHeatTransferExpress2D
    insertClass(p, makeHTFurnace());                // [0x70]  0x140c65340 SfOprHTFurnace
    insertClass(p, makeHeatTreat(false));           // [0x78]  0x1411b0bd0 SfOprHeatTreat
    insertClass(p, makeHeatTreat(true));            // [0x80]  0x1411ae5c0 SfOprHeatTreat2D
    insertClass(p, makeCogging());                  // [0xe0]  0x140750070 SfOprCogging
    insertClass(p, makePre(false));                 // [0xe8]  0x141348900 SfOprPre3D
    insertClass(p, makePre(true));                  // [0xf0]  0x141347400 SfOprPre2D
    insertClass(p, makeReport());                   // [0xf8]  0x1412ad860 SfOprReport
    insertClass(p, makeShapeOptim());               // [0x100] 0x14013a9a0 SfOprShapeOptim
    insertClass(p, makeDoeOutput());                // [0x108] 0x140e2dc70 SfOprDoeOutput
    insertClass(p, makeConvert2Dto3D());            // [0x128] 0x1401c2330 SfOprConvert2Dto3D
    insertClass(p, makeBoolean(true));              // [0x130] 0x1411de8d0(1) SfOprBoolean2D
    insertClass(p, makeBoolean(false));             // [0x138] 0x1411de8d0(0) SfOprBoolean3D
    insertClass(p, makeCycle());                    // [0x148] 0x14134d210 SfOprCycle
    insertClass(p, makeDiestress());                // [0x110] 0x1402b4ec0 SfOprDiestress
    insertClass(p, makeDiestressBM(true));          // [0x118] 0x1400707b0 SfOprDiestressBM2D
    insertClass(p, makeDiestressBM(false));         // [0x120] 0x140070810 SfOprDiestressBM3D
}

// @src 0x140054e90  registry[classKey(op)] (0x140050d00, 0x140070650); null when absent. The
// original reaches the registry through the main window; here it is the operation's project.
mst::OpClass* opBase(const Operation& op)
{
    if (!op.project) return nullptr;
    // 0x140070650: contains() first, so a missing key is not inserted
    auto it = op.project->registry.find(op.classKey);
    return it == op.project->registry.end() ? nullptr : it->second.get();
}

}  // namespace m2m
