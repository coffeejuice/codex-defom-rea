// Default property trees: template helpers and the per-class dispatcher (packet P11, framework).
// See include/m2m/props_tree.hpp. Decompiles: gui_mo/out_p11/root_d1.c .. root_d3.c.
#include "m2m/props_tree.hpp"

#include "m2m/data/common.hpp"
#include "m2m/data/heattransfer.hpp"
#include "m2m/p07_trees.hpp"
#include "m2m/p17.hpp"

namespace m2m {
namespace tree {

qt::Variant vInt(int i)
{
    qt::Variant v;
    v.kind = qt::Variant::Int;
    v.i = i;
    return v;
}

qt::Variant vDouble(double d)
{
    qt::Variant v;
    v.kind = qt::Variant::Double;
    v.d = d;
    return v;
}

qt::Variant vText(const String& s)
{
    qt::Variant v;
    v.kind = qt::Variant::Text;
    v.s = s;
    return v;
}

// @src 0x140375640: QtProperty prototype (0x14036e860) with TypeName "group" (0x14232788c) and the
// value QVariant(0); the display text and the enabled flag (0x14036eeb0) are UI only.
Template group(const char* name)
{
    return Template{qt::L(name), qt::L("group"), vInt(0)};
}

// @src 0x140373400: TypeName "int" (0x1423277d0), value QVariant(v); getter/setter names
// "get%1"/"set%1" and the step (0x14036eef0) are UI only.
Template intProp(const char* name, int v)
{
    return Template{qt::L(name), qt::L("int"), vInt(v)};
}

// @src 0x1403738e0: TypeName "double" (0x1423277e4), value QVariant(v); decimals (0x14036ee70)
// and step (0x14036eef0) are UI only.
Template doubleProp(const char* name, double v)
{
    return Template{qt::L(name), qt::L("double"), vDouble(v)};
}

Template other(const char* name, const char* typeName, qt::Variant v)
{
    return Template{qt::L(name), qt::L(typeName), std::move(v)};
}

// @src 0x140375ba0: node 0x14036f810 copies the template's value (+0x28) and type, then
// 0x140376320 appends it to the parent's list.
PropNode& add(std::vector<PropNode>& parent, const Template& t)
{
    PropNode n;
    n.name = t.name;
    n.typeName = t.typeName;
    n.value = t.value;
    parent.push_back(std::move(n));
    return parent.back();
}

}  // namespace tree

// @src OB->vt[0x98] -> props ctor -> class builder (s1-01 Q10). Class keys of s1-01 6.1.
std::vector<PropNode> defaultPropTree(const String& k)
{
    using namespace tree;
    const std::u16string& s = k.u16();
    if (s == u"SfOprCogging") return treeCogging();
    if (s == u"SfOprCycle") return {};                     // 0x14134b9c0: base groups only (P07)
    if (s == u"SfOprDistortion2D" || s == u"SfOprDistortion3D") return trees::distortionPropTree(k);   // P07
    if (s == u"SfOprDiestress") return treeDiestress();
    if (s == u"SfOprDiestressBM2D" || s == u"SfOprDiestressBM3D") return treeDiestressBM(k);
    if (s == u"SfOprConvert2Dto3D") return treeConvert2Dto3D();
    if (s == u"SfOprCopyMirror") return treeCopyMirror();
    if (s == u"SfOprDoeOutput") return treeDoeOutput();
    if (s == u"SfOprHeatTreat" || s == u"SfOprHeatTreat2D") return treeHeatTreat(k);
    if (s == u"SfOprBoolean2D" || s == u"SfOprBoolean3D") return treeBoolean(k);
    if (s == u"SfOprGroup") return treeGroup();
    // FormingExpress(2D) and MultiBlow(2D/3D) build their trees in their own makeProps (P15, P14);
    // the props-level classes go through defaultProps().
    return {};
}

namespace tree {
std::shared_ptr<Props> defaultProps(const String& k, int guiUnit)
{
    const std::u16string& s = k.u16();
    if (s == u"SfOprForming") return formingProps(false, guiUnit);
    if (s == u"SfOprForming2D") return formingProps(true, guiUnit);
    if (s == u"SfOprPre3D" || s == u"SfOprPre2D") return preProps(guiUnit);
    if (s == u"SfOprHTFurnace") return htFurnaceProps(guiUnit);
    if (s == u"SfOprReport") return reportProps(guiUnit);
    if (s == u"SfOprShapeOptim") return shapeOptimProps(guiUnit);
    if (s == u"SfOprHeatTransfer" || s == u"SfOprHeatTransfer2D" || s == u"SfOprHeatTransferCommon")
        return heatTransferProps(k, guiUnit);
    if (s == u"SfOprHeatTransferExpress" || s == u"SfOprHeatTransferExpress2D")
        return heatTransferExpressProps(k, guiUnit);
    std::shared_ptr<Props> p;
    if (s == u"SfOprDoeOutput") {                 // ctor 0x140e35ec0 -> 0x140e35f90 (L2)
        auto d = std::make_shared<DoeOutputProps>();
        d->data = std::make_shared<data::DoeOutputData>();   // new(8), 0x140e363e0
        p = d;
    } else if (s == u"SfOprGroup") {              // SfOprGroupProperties: base slots only (L2)
        p = std::make_shared<BaseSlotsProps>();
    } else {
        p = std::make_shared<Props>();
    }
    data::initCommonBlock(*p->common, guiUnit);    // base ctor 0x1405f8ca0 -> 0x1405fd250
    p->tree = defaultPropTree(k);
    return p;
}
}  // namespace tree

}  // namespace m2m
