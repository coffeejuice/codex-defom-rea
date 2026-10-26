// m2m/props_forming.hpp: SfOprFormingProperties and its data block (PROPS+0x38). The class and its
// tree are P17's (src/model/classprops.cpp); MDT L3 moved them here so that the props load
// 0x140c09da0 (src/engine/loader_l3.cpp) can fill the block from the engine's PDIE movement.
#pragma once

#include <vector>

#include "m2m/props.hpp"

namespace m2m {

struct FormingData {               // 0x88 bytes, ctor 0x140c0aac0 (+0x00, +0x08, +0x20, +0x80 are
    bool disabled = false;         //   QPointers to tree nodes, kept in Props::tree here)
    int type = 2;                  // +0x14 Equipment (0 .. 7; 2 mechanical, 3 hammer, 4 screw)
    String name;                   // +0x18 EquipName
    // added by MDT L3: the doubles the ctor sets and the props load 0x140c07eb0 copies from the
    // movement (values of the ctor 0x140c0aac0)
    double hammerEnergy = 425.06;          // +0x28 type 3: movement+0x1f0
    double hammerMass = 0.013;             // +0x30 type 3: movement+0x1e8
    double hammerEfficiency = 0.85;        // +0x38 type 3: movement+0x1f8
    double screwEnergy = 79200.0;          // +0x40 type 4: movement+0x280
    double screwEfficiency = 0.9;          // +0x48 type 4: movement+0x288
    double momentOfInertia = 4606.46;      // +0x50 type 4: movement+0x290
    double leadScrewPitch = 21.09;         // +0x58 type 4: movement+0x298
    int mechPressType = 0;                 // +0x60
    double totalStroke = 10.0;             // +0x68 type 2: movement+0x318
    double cyclesSec = 0.6;                // +0x70 type 2: movement+0x320
    double connectingRodLength = 0.0;      // +0x78 type 2: movement+0x328
};

struct FormingProps : Props {      // SfOprFormingProperties (vtable 0x142813af8)
    FormingData d;                 // +0x38
    bool is2D = false;

    // @src 0x140c0ad80  qt_static_metacall slots (0x140c0b840): Equipment and MechPressType rebuild
    // EquipmentSpec (0x140c0a040 clear, 0x140c0a920 -> 0x140c0a270); the rest store values only
    void applyValue(PropNode& n, const std::vector<PropNode*>& anc, const qt::Variant& v) override;
    // @src 0x140c0ad80  the get slots (moproj-ops), then the base's (src/model/classprops.cpp)
    bool getValue(const PropNode& n, const std::vector<PropNode*>& anc, qt::Variant& out) override;
};

// @src 0x140c0a040  EquipmentType children re-made (0x140c0a010 clear, 0x140c09f20 Equipment,
// EquipName) (src/model/classprops.cpp, MDT L3)
void formingRemakeEquipmentType(FormingProps& p);
// @src 0x140c0a920  EquipmentSpec children re-made (0x140c0a8e0 clear, 0x140c0a270 by D+0x14)
void formingRemakeEquipmentSpec(FormingProps& p);

}  // namespace m2m
