// P17: class init vt+0x180 of the base-writer family, Cogging, ShapeOptim (P02 split) and
// FormingExpress(2D) (P15; those two also register the Qt resource 0x1414250b0, GUI only),
// Diestress, DiestressBM2D/3D, HeatTreat(2D), Distortion3D/2D and Report (literals and flags: gui_mo/s1/p17/d5.txt).
// Each init fills the class info (OB+0x28 -> info at +0x10): display texts, icon, colours and mime
// types (GUI only), the key (0x1405fd780, info+0x10), the dimension flags (0x1405fd8a0, info+0x80)
// and for some classes info+0x84 (0x1405fd8d0) and info+0x99 = 0 when a feature global is 0
// (0x1405fd6c0; globals 0x1446ee58e/5aa/5bc, not followed). Flags reach the MST (vt+0x190
// dim -> DIM, 0x1405f5bf0) and diagram connector construction (0x14015ef80).
// Decompiles: gui_mo/s1/p17/b1.c, b7.c; Cycle and DoeOutput: s1/s1-07/dc/init1.c.
#include "m2m/mst/writer.hpp"
#include "m2m/p17.hpp"

namespace m2m::mst {

namespace {

struct Init {
    const char16_t* key;           // 0x1405fd780 literal
    int dimFlags;                  // 0x1405fd8a0 argument
};

// One row per init function, in registry order.
constexpr Init kInits[] = {
    {u"SfOprForming", 0x1a},                 // @src 0x140c05bc0
    {u"SfOprForming2D", 0x19},               // @src 0x140c043e0
    {u"SfOprHeatTransfer", 0x1a},            // @src 0x141353ec0
    {u"SfOprHeatTransfer2D", 0x19},          // @src 0x1413530d0
    {u"SfOprHeatTransferExpress", 0x16},     // @src 0x1413e6060
    {u"SfOprHeatTransferExpress2D", 0x15},   // @src 0x1413e57a0
    {u"SfOprHTFurnace", 0x1a},               // @src 0x140c65420
    {u"SfOprCogging", 0x1a},                 // @src 0x140750140
    {u"SfOprPre3D", 0x5a},                   // @src 0x1413489d0
    {u"SfOprPre2D", 0x59},                   // @src 0x1413474d0
    {u"SfOprShapeOptim", 0x40097},           // @src 0x14013ab80 (also info+0x84 = 3)
    {u"SfOprFormingExpress", 0x16},          // @src 0x141413000 (P15)
    {u"SfOprFormingExpress2D", 0x15},        // @src 0x1414126f0 (P15)
    {u"SfOprDiestressBM2D", 0x4000009d},     // @src 0x14029a880
    {u"SfOprDiestressBM3D", 0x4000009e},     // @src 0x14029c6c0
    {u"SfOprDiestress", 0xdf},               // @src 0x1402b4fb0
    {u"SfOprHeatTreat2D", 0x29},             // @src 0x1411ae690
    {u"SfOprHeatTreat", 0x2a},               // @src 0x1411b0ca0
    {u"SfOprDistortion3D", 0x1a},            // @src 0x141362050 (MDT L6T)
    {u"SfOprDistortion2D", 0x19},            // @src 0x141362050
    {u"SfOprReport", 0x9b},                  // @src 0x1412ad930 (also info+0x84 = 3)
    {u"SfOprCycle", 0x301b},                 // @src 0x14134d2e0; combined 2D/3D connectors
    {u"SfOprDoeOutput", 0x20097},            // @src 0x140e2df30; combined 2D/3D connectors
};

}  // namespace

// OB->vt[0x180]() for the classes above; other classes set their flags in their own ctors.
void classInit(OpClass& ob)
{
    for (const Init& i : kInits) {
        if (ob.key().u16() == i.key) {
            ob.dimFlags = i.dimFlags;
            return;
        }
    }
}

}  // namespace m2m::mst
