// Operation class base slots (P02 rescue slice; Properties/ApplyProperty are P08's): the OpClass base slots vt+0x88 (display name) and vt+0x98 (properties) for the
// classes whose packets do not override them. s1-01 6.3; decompiles s1-01 dc/open3.c, open4.c and
// gproj_p01 (vt+0x88 / vt+0x98 of the classes, strings read from the exe copy).
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/props_tree.hpp"

namespace m2m {
using qt::L;

// defaultPropTree (OB->vt[0x98] per class) is the P11 dispatcher in props_tree.cpp.

// Find a node by name path ("Group/Name"); null if absent.
const PropNode* findProp(const std::vector<PropNode>& tree, const String& path)
{
    const std::vector<String> parts = qt::split(path, L("/"), qt::SplitBehavior::SkipEmptyParts);
    const std::vector<PropNode>* level = &tree;
    const PropNode* hit = nullptr;
    for (const String& part : parts) {
        hit = nullptr;
        for (const PropNode& n : *level)
            if (n.name == part) {
                hit = &n;
                break;
            }
        if (!hit) return nullptr;
        level = &hit->children;
    }
    return hit;
}

}  // namespace m2m

namespace m2m::mst {

namespace {
// vt+0x88 strings (tr(), no translator loaded) of the classes that use the base slots here.
struct NameRow {
    const char16_t* key;
    const char* text;
    std::uint64_t src;
};
const NameRow kNames[] = {
    {u"SfOprForming", "Forming", 0x140c07000},
    {u"SfOprForming2D", "Forming", 0x140c05940},
    {u"SfOprHeatTransfer", "HeatTransfer", 0x141354940},
    {u"SfOprHeatTransfer2D", "HeatTransfer", 0x141353c40},
    {u"SfOprHeatTransferExpress", "Heat Transfer Express", 0x1413e6600},
    {u"SfOprHeatTransferExpress2D", "Heat Transfer Express", 0x1413e5d30},
    {u"SfOprHTFurnace", "Furnace-Heating", 0x140c66320},
    {u"SfOprPre3D", "Pre processor", 0x141349ba0},
    {u"SfOprPre2D", "Pre processor", 0x141348760},
    {u"SfOprCogging", "Cogging", 0x140751cd0},
    {u"SfOprConvert2Dto3D", "Convert 2D to 3D", 0x1401c5480},
    {u"SfOprDiestress", "Die Stress", 0x1402b6740},
    {u"SfOprDiestressBM2D", "Die Stress", 0x14029b790},
    {u"SfOprDiestressBM3D", "Die Stress", 0x14029d5d0},
    {u"SfOprHeatTreat", "Heat Wizard", 0x1411b2260},
    {u"SfOprHeatTreat2D", "Heat Wizard", 0x1411afc40},
    {u"SfOprReport", "Report", 0x1412b0550},
    {u"SfOprShapeOptim", "Shape Optimization", 0x14013bd30},
    {u"SfOprDoeOutput", "DOE Output", 0x140e31830},
    {u"SfOprFormingExpress", "Forming", 0x14141e7f0},
    {u"SfOprFormingExpress2D", "Forming", 0x14141e7f0},
};
}  // namespace

// vt+0x88: per class (table above); SfOprBase's own slot is _purecall (0x141470184).
String OpClass::displayName() const
{
    for (const NameRow& r : kNames)
        if (key() == String(r.key)) return L(r.text);
    not_ported(0x141470184, "vt+0x88 display name of this class");
}

// vt+0x98: new SfOpr*Properties of the class with its default tree. The family classes allocate
// SfOprFormingProperties (0x140c07d00), SfOprHeatTransferProperties (0x141355f00),
// SfOprHeatTransferExpressProperties (0x1413eb5d0, an SfOprExpressProperties: its vt+0xc0 reads
// Forming.FWZ), SfOprHTFurnaceProperties (0x140c67820), SfOprPreProperties (0x14134a880).
std::shared_ptr<Props> OpClass::makeProps(int guiUnit) const
{
    return tree::defaultProps(key(), guiUnit);       // per-class dispatch (P11, props_tree.cpp)
}

}  // namespace m2m::mst
