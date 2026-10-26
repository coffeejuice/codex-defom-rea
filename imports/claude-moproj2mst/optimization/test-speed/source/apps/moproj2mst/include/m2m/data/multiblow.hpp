// m2m/data/multiblow.hpp: MultiBlowData, the payload of the MultiBlow properties (owner: P14).
// s1-04 7.3; ctor 0x140618d20, made by 0x14060b5d0, filled by the property setters of
// SfOprMultiBlowProperties (qt_metacall 0x14061ab80, slots below). Offsets are MBD+off.
#pragma once

#include <cstdint>
#include <vector>

#include "m2m/props.hpp"
#include "m2m/qt4.hpp"

namespace m2m::data {
using qt::String;

struct MultiBlowData {             // MBD (0x1d8 bytes)
    std::uint8_t f08 = 0;          // +0x08
    std::uint8_t hammer = 0;       // +0x09 = !(+0x0a): hammer fields (+0x20..+0x38) else screw (+0x40..)
    std::uint8_t equipment = 1;    // +0x0a setEquipment
    String equipName;              // +0x10 setEquipName (movement curve name)
    double hammerEnergy = 0.0;     // +0x20
    double hammerMass = 0.0;       // +0x28
    double hammerEfficiency = 0.0; // +0x30
    int hammerCounterblow = 0;     // +0x38 (int)toDouble
    double screwEnergy = 0.0;      // +0x40
    double screwEfficiency = 0.0;  // +0x48
    double momentOfInertia = 0.0;  // +0x50
    double leadScrewPitch = 0.0;   // +0x58
    double movDir[3] = {0.0, -1.0, 0.0};   // +0x68 +0x70 +0x78 (0x14060e2c0 sets them by dimension)
    std::uint8_t relativeEnergy = 1;       // +0x88 UseRelativeEnergy: DOESCL in percent
    std::uint8_t workpieceRotation = 0;    // +0x89 UseWorkpieceRotation: flip/rotate enabled
    std::uint8_t initTemp = 0;             // +0x8a InitTemp: NDTMP enabled (also blocks heating)
    std::uint8_t initStrain = 0;           // +0x8b InitStrain: STRAIN reset
    std::uint8_t efficiencyFunction = 0;   // +0x8c UseEfficiencyFunctionData: no 2nd DOESCL/MOVCTL
    std::uint8_t heating = 0;              // +0xa0 UseHeating
    double heatingTemperature = 0.0;       // +0xa8 HeatingTemperature (NDTMP, HT ENVTMP)
    double roomTemperature = 20.0;         // +0xb0 RoomTemperature (TI/TO ENVTMP); 68 when English
    double heatingTime = 3600.0;           // +0xb8 HeatingTime (HT total)
    double heatingTimeStep = 20.0;         // +0xc0 HeatingTimeStep
    double transferInTime = 10.0;          // +0xc8 TransferInTime (TI total)
    double transferOutTime = 10.0;         // +0xd0 TransferOutTime (TO total)
    double transferTimeStep = 1.0;         // +0xd8 TransferTimeStep (TI/TO dt)
    std::uint8_t adaptiveReheat = 0;       // +0xe8 UseAdaptiveReheat (OTPRNG)
    double triggerHeatTemp = 0.0;          // +0xf0 TriggerHeatTemp
    double triggerHeatTempUpper = 0.0;     // +0xf8 TriggerHeatTempUpperLimit
    double heatStopTemp = 0.0;             // +0x100 HeatStopTemp
    std::uint8_t dwell = 1;                // +0x110 UseDwell
    double defaultDwellTime = 1.0;         // +0x118 DefaultDwellTime
    String dwellTimeStep;                  // +0x120 DwellTimeStep (unit string, category 0)
    String dwellHeatCoeff;                 // +0x128 DwellHeatCoeff (unit string, category 0x20)
    int heats = 0;                         // +0x140 NumHeats (passes)
    std::vector<int> blows;                // +0x148 NumBlows per heat
    std::vector<double> temperature;       // +0x150 Temperature per heat
    std::vector<std::vector<double>> energy;        // +0x170 BlowEnergy (DOESCL 1)
    std::vector<std::vector<double>> efficiency;    // +0x178 BlowEfficiency (DOESCL 2)
    std::vector<std::vector<double>> dwellTime;     // +0x180 BlowDwellTime
    std::vector<std::vector<std::uint8_t>> reheat;  // +0x188 BlowReheat (NDTMP per blow)
    std::vector<std::vector<std::uint8_t>> flip;    // +0x190 BlowFlip (rotate/flip per blow)
    std::vector<std::vector<double>> rotAngle;      // +0x198 BlowRotAngle
    std::vector<std::vector<std::uint8_t>> rotSelfCenter;   // +0x1a0 BlowRotSelfCenter
    std::vector<std::vector<double>> rotCenter[3];  // +0x1a8 +0x1b0 +0x1b8 BlowRotCenterX/Y/Z
    std::vector<std::vector<double>> rotAxis[3];    // +0x1c0 +0x1c8 +0x1d0 BlowRotAxisX/Y/Z
};

// Rotation record R of one blow (0x40 bytes, ctor 0x140609140: all 0, selfCenter = 1).
struct BlowRotation {
    double angle = 0.0;            // +0x00
    std::uint8_t selfCenter = 1;   // +0x08
    double c[3] = {};              // +0x10 +0x18 +0x20
    double a[3] = {};              // +0x28 +0x30 +0x38
};

// @src 0x140618d20  ctor defaults; unit = the GUI unit global (0x143d766b8 vt+0x260; 2 = English)
void initMultiBlowData(MultiBlowData& d, int unit);
// @src 0x14060b500 + 0x14060b5d0 + 0x14060e2c0  SfOprMultiBlowProperties(dim): MBD, then the
// movement default by dimension (2: (0,-1,0), else (0,0,-1)). The property templates that
// 0x14060e2c0 registers are not ported (tree shape: P02 / s1-04 O-6).
void initMultiBlowProps(MultiBlowProps& p, int dim, int unit);

// @src 0x140612150  property tree of SfOprMultiBlowProperties, built from the templates that
// 0x14060e2c0 registers (globals 0x143d9ef10..0x143d9f0f8). Names are the templates' names, the
// order is the original's. Heat 1 with blow 1 is there from the default NumHeats(1)
// (0x140613f40 -> 0x1406140c0). The base groups the original appends after it (0x1405f9eb0,
// 0x1405fa560, 0x1405faaa0, 0x1405fb020) and the Interobject children (0x1405fad10, 0x1405fa7d0)
// are the base properties' (P02) and are not built here.
std::vector<PropNode> multiBlowPropTree(int dim, const MultiBlowData& d = MultiBlowData());
// @src 0x140612e80  EquipmentSpec children (MBD+0x60): the hammer fields when MBD+9, the screw fields
// when MBD+0xa (templates 0x143d9f000..0x143d9f038)
void equipmentSpecFill(PropNode& spec, const MultiBlowData& d);
// @src 0x1406132b0  0x140613280 (QtProperty children of MBD+0x60 removed, 0x140371580), then 0x140612e80
void equipmentSpecRebuild(PropNode& spec, const MultiBlowData& d);
// Root name of the tree: 0x143d9ef10 is a process-wide template that 0x140375640 fills only while
// it is null, so the first MultiBlow props a GUI process makes names it for both classes. One
// conversion is one fresh GUI process (Project::multiBlowRoot through currentProject(), first-wins
// from null). Without a current project: the props' own name. Plan 8.
String multiBlowRootName(int dim);
// Name of the MultiBlowForging* top-level property saved for operation opId in the .moproj (null if
// none). A saved name that differs from the tree's root is not matched by ApplyProperty, so the
// original keeps the defaults without a message; the writer reports it (plan 8).
String savedMultiBlowRoot(const Project& p, int opId);

// ---- slots of SfOprMultiBlowProperties (qt_metacall 0x14061ab80; even indices are the setters) ---
// Scalar setters take the QVariant as the property reader built it (s1-01 6.3).
// @src 0x140612de0 setEquipment (the tree refresh 0x1406132b0 is not ported)
void mbSetEquipment(MultiBlowData& d, const qt::Variant& v);
// @src 0x140609d60 .. 0x14060b2e0: one per name, see multiblow_props.cpp
// Returns false when `name` is not a MultiBlow setter name.
bool mbSetScalar(MultiBlowData& d, const String& name, const qt::Variant& v);
// @src 0x1406140c0 setNumHeats -> heats and the per-heat lists
void mbSetNumHeats(MultiBlowData& d, int n);
// @src 0x140615190 setNumBlows of heat k (0-based) -> blows[k] and the per-blow lists
void mbSetNumBlows(MultiBlowData& d, int k, int n);
// @src 0x14060b3a0 setTemperature of heat k (0-based)
void mbSetTemperature(MultiBlowData& d, int k, const qt::Variant& v);
// @src 0x1406166f0 .. 0x140618700  per-blow setters (heat k, blow b, both 0-based); false for an
// unknown name. The original drops the value when the node has no blow/heat parent.
bool mbSetBlow(MultiBlowData& d, const String& name, int k, int b, const qt::Variant& v);

// ---- accessors of the writer (props = SfOprMultiBlowProperties; i, j 1-based) -------------------
// @src 0x14060c6f0
int mbHeats(const MultiBlowData& d);
// @src 0x14060c710
int mbBlows(const MultiBlowData& d, int i);
// @src 0x14060c810 / 0x14060cb20 / 0x14060ccc0  energy / efficiency / dwell time of blow (i, j)
double mbEnergy(const MultiBlowData& d, int i, int j);
double mbEfficiency(const MultiBlowData& d, int i, int j);
double mbDwellTime(const MultiBlowData& d, int i, int j);
// @src 0x14060ce60 / 0x14060cfc0  reheat / flip of blow (i, j)
bool mbReheat(const MultiBlowData& d, int i, int j);
bool mbFlip(const MultiBlowData& d, int i, int j);
// @src 0x14060d150  rotation record of blow (i, j)
BlowRotation mbRotation(const MultiBlowData& d, int i, int j);
// @src 0x14060bdd0  heating block wanted after blow (i, j)
bool mbHeatOk(const MultiBlowData& d, int i, int j);
// @src 0x14060c280  dwell block wanted after blow (i, j)
bool mbDwellOn(const MultiBlowData& d, int i, int j);

}  // namespace m2m::data
