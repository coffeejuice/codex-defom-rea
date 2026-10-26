// L2: the qt_static_metacall set and get slots of SfOprDistortionProperties, SfOprDoeOutputProperties
// and SfOprShapeOptimProperties (as props_l2_slots.cpp; Convert2Dto3D and CopyMirror: inline in
// m2m/data/convert.hpp). Method names: the moc string tables at file offsets 0x2e3ab20, 0x2ae6a20,
// 0x21cb3a0; bodies: gui_mo/s1/moproj_rest/sm2a.c.
#include "m2m/data/heattransfer.hpp"
#include "m2m/data/l6_blocks.hpp"
#include "m2m/props.hpp"
#include "m2m/props_variant.hpp"

namespace m2m {

namespace {

[[noreturn]] void noData(const char* cls)
{
    throw Error(M2M_E_UNSUPPORTED, std::string("properties: ") + cls + " without its data block");
}

}  // namespace

// ---- SfOprDistortionProperties: qt_static_metacall 0x141371ad0 (D = +0x38) ---------------------
// @src 0x141365860 setNumFixtures (toInt, D+0x10), 0x141365900 setNumPasses (toInt, D+0x14)
void DistortionProps::applyValue(PropNode& node, const std::vector<PropNode*>& ancestors, const qt::Variant& v)
{
    if (!counts) noData("SfOprDistortionProperties");
    const std::u16string& n = node.name.u16();
    if (n == u"NumFixtures") counts->fixtures = pv::toInt(v);
    else if (n == u"NumPasses") counts->passes = pv::toInt(v);
    else Props::applyValue(node, ancestors, v);
}

// @src 0x1413658b0 getNumFixtures, 0x141365950 getNumPasses
bool DistortionProps::getValue(const PropNode& node, const std::vector<PropNode*>&, qt::Variant& out)
{
    if (!counts) noData("SfOprDistortionProperties");
    const std::u16string& n = node.name.u16();
    if (n == u"NumFixtures") out = pv::vInt(counts->fixtures);
    else if (n == u"NumPasses") out = pv::vInt(counts->passes);
    else return base::getValue(*this, node, out);
    return true;
}

// ---- SfOprDoeOutputProperties: qt_static_metacall 0x140e3a5a0 (D = +0x38) ----------------------
// @src 0x140e35d80 setSelecedOpr (toInt, D+0), 0x140e35e20 setSelectedObj (toInt, D+4)
void DoeOutputProps::applyValue(PropNode& node, const std::vector<PropNode*>& ancestors, const qt::Variant& v)
{
    if (!data) noData("SfOprDoeOutputProperties");
    const std::u16string& n = node.name.u16();
    if (n == u"SelecedOpr") data->selecedOpr = pv::toInt(v);
    else if (n == u"SelectedObj") data->selectedObj = pv::toInt(v);
    else Props::applyValue(node, ancestors, v);
}

// @src 0x140e35dd0 getSelecedOpr, 0x140e35e70 getSelectedObj
bool DoeOutputProps::getValue(const PropNode& node, const std::vector<PropNode*>&, qt::Variant& out)
{
    if (!data) noData("SfOprDoeOutputProperties");
    const std::u16string& n = node.name.u16();
    if (n == u"SelecedOpr") out = pv::vInt(data->selecedOpr);
    else if (n == u"SelectedObj") out = pv::vInt(data->selectedObj);
    else return base::getValue(*this, node, out);
    return true;
}

// ---- SfOprShapeOptimProperties: qt_static_metacall 0x14014b600 (D = +0x38) ---------------------
// @src 0x1401424e0 setMaxSims (toInt, D+8), 0x140142580 setMinObjFuncVal (toDouble, D+0x10)
void ShapeOptimProps::applyValue(PropNode& node, const std::vector<PropNode*>& ancestors, const qt::Variant& v)
{
    if (!data) noData("SfOprShapeOptimProperties");
    const std::u16string& n = node.name.u16();
    if (n == u"MaxSims") data->maxSims = pv::toInt(v);
    else if (n == u"MinObjFuncVal") data->minObjFuncVal = pv::toDouble(v);
    else Props::applyValue(node, ancestors, v);
}

// @src 0x140142530 getMaxSims, 0x1401425d0 getMinObjFuncVal
bool ShapeOptimProps::getValue(const PropNode& node, const std::vector<PropNode*>&, qt::Variant& out)
{
    if (!data) noData("SfOprShapeOptimProperties");
    const std::u16string& n = node.name.u16();
    if (n == u"MaxSims") out = pv::vInt(data->maxSims);
    else if (n == u"MinObjFuncVal") out = pv::vDouble(data->minObjFuncVal);
    else return base::getValue(*this, node, out);
    return true;
}

}  // namespace m2m
