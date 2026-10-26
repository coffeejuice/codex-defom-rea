// SfOprMultiBlowProperties get slots for the .moproj writer (MDT L4, moproj-ops): the odd cases
// 1..99 of qt_metacall 0x14061ab80, get<Name>(SfProperty* node) -> QVariant. Decompiles
// gui_mo/s1/mdt/L4/mc.c (metacall), get.c (the 50 slots). The scalar slots return one field of the
// MultiBlowData (props+0x38); NumBlows and Temperature use the node's index (0x140370fd0); the Blow*
// slots use the indexes of the node's parent (blow) and grandparent (heat) through 0x140370f40.

#include <vector>

#include "m2m/data/multiblow.hpp"
#include "m2m/props.hpp"

namespace m2m {

namespace {

using data::MultiBlowData;

qt::Variant vD(double d)
{
    qt::Variant v;
    v.kind = qt::Variant::Double;
    v.d = d;
    return v;
}

qt::Variant vI(int i)
{
    qt::Variant v;
    v.kind = qt::Variant::Int;
    v.i = i;
    return v;
}

qt::Variant vB(bool b)
{
    qt::Variant v;
    v.kind = qt::Variant::Bool;
    v.i = b ? 1 : 0;
    return v;
}

qt::Variant vS(const String& s)
{
    qt::Variant v;
    v.kind = qt::Variant::Text;
    v.s = s;
    return v;
}

// Scalar slots: name -> the field the slot reads (address of the get slot, MBD offset)
bool scalarGet(const MultiBlowData& d, const std::u16string& n, qt::Variant& out)
{
    if (n == u"Equipment") out = vB(d.equipment != 0);                         // @src 0x140612d90  +0x0a
    else if (n == u"EquipName") out = vS(d.equipName);                         // @src 0x140609de0  +0x10
    else if (n == u"HammerEnergy") out = vD(d.hammerEnergy);                   // @src 0x140609e80  +0x20
    else if (n == u"HammerMass") out = vD(d.hammerMass);                       // @src 0x140609f20  +0x28
    else if (n == u"HammerEfficiency") out = vD(d.hammerEfficiency);           // @src 0x140609fc0  +0x30
    else if (n == u"HammerCounterblow") out = vI(d.hammerCounterblow);         // @src 0x14060a060  +0x38
    else if (n == u"ScrewEnergy") out = vD(d.screwEnergy);                     // @src 0x14060a100  +0x40
    else if (n == u"ScrewEfficiency") out = vD(d.screwEfficiency);             // @src 0x14060a1a0  +0x48
    else if (n == u"MomentOfInertia") out = vD(d.momentOfInertia);             // @src 0x14060a240  +0x50
    else if (n == u"LeadScrewPitch") out = vD(d.leadScrewPitch);               // @src 0x14060a2e0  +0x58
    else if (n == u"MovDirX") out = vD(d.movDir[0]);                           // @src 0x14060a380  +0x68
    else if (n == u"MovDirY") out = vD(d.movDir[1]);                           // @src 0x14060a420  +0x70
    else if (n == u"MovDirZ") out = vD(d.movDir[2]);                           // @src 0x14060a4c0  +0x78
    else if (n == u"UseWorkpieceRotation") out = vB(d.workpieceRotation != 0); // @src 0x14060a570  +0x89
    else if (n == u"InitTemp") out = vB(d.initTemp != 0);                      // @src 0x140618c90  +0x8a
    else if (n == u"InitStrain") out = vB(d.initStrain != 0);                  // @src 0x14060a630  +0x8b
    else if (n == u"UseEfficiencyFunctionData") out = vB(d.efficiencyFunction != 0);   // @src 0x14060a6f0  +0x8c
    else if (n == u"UseHeating") out = vB(d.heating != 0);                     // @src 0x14060a7b0  +0xa0
    else if (n == u"HeatingTemperature") out = vD(d.heatingTemperature);       // @src 0x14060a860  +0xa8
    else if (n == u"RoomTemperature") out = vD(d.roomTemperature);             // @src 0x14060a910  +0xb0
    else if (n == u"TransferInTime") out = vD(d.transferInTime);               // @src 0x14060ab20  +0xc8
    else if (n == u"HeatingTime") out = vD(d.heatingTime);                     // @src 0x14060a9c0  +0xb8
    else if (n == u"TransferOutTime") out = vD(d.transferOutTime);             // @src 0x14060abd0  +0xd0
    else if (n == u"TransferTimeStep") out = vD(d.transferTimeStep);           // @src 0x14060ac80  +0xd8
    else if (n == u"HeatingTimeStep") out = vD(d.heatingTimeStep);             // @src 0x14060aa70  +0xc0
    else if (n == u"UseAdaptiveReheat") out = vB(d.adaptiveReheat != 0);       // @src 0x14060ad40  +0xe8
    else if (n == u"TriggerHeatTemp") out = vD(d.triggerHeatTemp);             // @src 0x14060adf0  +0xf0
    else if (n == u"TriggerHeatTempUpperLimit") out = vD(d.triggerHeatTempUpper);   // @src 0x14060aea0  +0xf8
    else if (n == u"HeatStopTemp") out = vD(d.heatStopTemp);                   // @src 0x14060af50  +0x100
    else if (n == u"UseDwell") out = vB(d.dwell != 0);                         // @src 0x14060b010  +0x110
    else if (n == u"DefaultDwellTime") out = vD(d.defaultDwellTime);           // @src 0x14060b0c0  +0x118
    else if (n == u"DwellTimeStep") out = vS(d.dwellTimeStep);                 // @src 0x14060b1a0  +0x120
    else if (n == u"DwellHeatCoeff") out = vS(d.dwellHeatCoeff);               // @src 0x14060b280  +0x128
    else if (n == u"UseRelativeEnergy") out = vB(d.relativeEnergy != 0);       // @src 0x14060b340  +0x88
    else if (n == u"NumHeats") out = vI(d.heats);                              // @src 0x1406166a0  +0x140
    else return false;
    return true;
}

// @src 0x14060b440  Temperature: +0x150[node index] when the index is in range, else 0.0
qt::Variant temperatureGet(const MultiBlowData& d, int k)
{
    if (k > -1 && k < static_cast<int>(d.temperature.size())) return vD(d.temperature[static_cast<size_t>(k)]);
    return vD(0.0);
}

// The Blow* slots (0x140616830 .. 0x140618840): node->parent (the blow, index b) and its parent
// (the heat, index k) through QPointer copies; list[k][b] (QList::at, 0x140619390 / 0x1401407d0),
// QVariant(int 0) when a parent is missing.
bool blowGet(const MultiBlowData& d, const std::u16string& n, const std::vector<PropNode*>& path,
             qt::Variant& out)
{
    const std::vector<std::vector<double>>* ld = nullptr;
    const std::vector<std::vector<std::uint8_t>>* lb = nullptr;
    if (n == u"BlowEnergy") ld = &d.energy;                    // @src 0x140616830  +0x170
    else if (n == u"BlowEfficiency") ld = &d.efficiency;       // @src 0x140616ae0  +0x178
    else if (n == u"BlowDwellTime") ld = &d.dwellTime;         // @src 0x140616d90  +0x180
    else if (n == u"BlowReheat") lb = &d.reheat;               // @src 0x140617030  +0x188
    else if (n == u"BlowFlip") lb = &d.flip;                   // @src 0x1406172d0  +0x190
    else if (n == u"BlowRotAngle") ld = &d.rotAngle;           // @src 0x140617580  +0x198
    else if (n == u"BlowRotSelfCenter") lb = &d.rotSelfCenter; // @src 0x140617820  +0x1a0
    else if (n == u"BlowRotCenterX") ld = &d.rotCenter[0];     // @src 0x140617ad0  +0x1a8
    else if (n == u"BlowRotCenterY") ld = &d.rotCenter[1];     // @src 0x140617d80  +0x1b0
    else if (n == u"BlowRotCenterZ") ld = &d.rotCenter[2];     // @src 0x140618030  +0x1b8
    else if (n == u"BlowRotAxisX") ld = &d.rotAxis[0];         // @src 0x1406182e0  +0x1c0
    else if (n == u"BlowRotAxisY") ld = &d.rotAxis[1];         // @src 0x140618590  +0x1c8
    else if (n == u"BlowRotAxisZ") ld = &d.rotAxis[2];         // @src 0x140618840  +0x1d0
    else return false;
    if (path.size() < 2) {                                     // no blow or no heat node
        out = vI(0);
        return true;
    }
    const int b = path[path.size() - 1]->index;                // 0x140370fd0(parent)
    const int k = path[path.size() - 2]->index;                // 0x140370fd0(grandparent)
    // QList::at asserts in range in a debug Qt only; out of range is undefined in the original
    if (k < 0 || b < 0)
        throw Error(M2M_E_UNSUPPORTED, "project file: MultiBlow blow property outside the heat/blow lists");
    const size_t K = static_cast<size_t>(k), B = static_cast<size_t>(b);
    if (ld) {
        if (K >= ld->size() || B >= (*ld)[K].size())
            throw Error(M2M_E_UNSUPPORTED, "project file: MultiBlow blow property outside the heat/blow lists");
        out = vD((*ld)[K][B]);
    } else {
        if (K >= lb->size() || B >= (*lb)[K].size())
            throw Error(M2M_E_UNSUPPORTED, "project file: MultiBlow blow property outside the heat/blow lists");
        out = vB((*lb)[K][B] != 0);
    }
    return true;
}

}  // namespace

// @src 0x14061ab80  get<Name> cases, then the SfOprBaseProperties ones (0x1405f84b0 -> 0x1405f7b10)
bool MultiBlowProps::getValue(const PropNode& node, const std::vector<PropNode*>& ancestors, qt::Variant& out)
{
    if (!data) throw Error(M2M_E_UNSUPPORTED, "project file: MultiBlow properties without data (props+0x38)");
    {
        const MultiBlowData& d = *data;
        const std::u16string& n = node.name.u16();
        if (scalarGet(d, n, out)) return true;
        if (n == u"NumBlows") {                                // @src 0x1406165d0  +0x148[node index]
            const int k = node.index;
            if (k < 0 || k >= static_cast<int>(d.blows.size()))
                throw Error(M2M_E_UNSUPPORTED, "project file: MultiBlow NumBlows outside the heat list");
            out = vI(d.blows[static_cast<size_t>(k)]);
            return true;
        }
        if (n == u"Temperature") {
            out = temperatureGet(d, node.index);
            return true;
        }
        if (blowGet(d, n, ancestors, out)) return true;     // parent chain, top level first (0x140370f40)
    }
    return base::getValue(*this, node, out);
}

}  // namespace m2m
