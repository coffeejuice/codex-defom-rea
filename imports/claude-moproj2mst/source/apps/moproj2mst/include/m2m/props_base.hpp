// m2m/props_base.hpp: SfOprBaseProperties tree adders and slots that fill data::CommonBlock
// (PROPS+0x30) (owner: P15, src/ops/props_base.cpp). Meta-object strings at file offset 0x2497a20:
// 2 signals + set/get pairs NumSteps, StepIncr, HeatingTime, EnvTemp, HeatTransCoef, ConvectionCoef,
// FrictionType, FrictionCoef, StepBy, DistPerStep, TimePerStep (qt_metacall 0x1405f84b0 ->
// 0x1405f7b10). Template names (globals 0x143d9dcd0..0x143d9dd28) are inferred from their use and the
// .rdata name order: TestGroup, StepBy, NumSteps, StepIncr, HeatingTime, EnvTemp, HeatTransCoef,
// ConvectionCoef, DistPerStep, TimePerStep, FrictionType, FrictionCoef. "TestGroup" is a top-level
// node (sibling of the class root), as the corpus .moproj files show.
// `unit` is the GUI unit global (mainWindow vt+0x260; 2 = English) that picks the default strings.
#pragma once

#include <vector>

#include "m2m/data/common.hpp"
#include "m2m/props.hpp"

namespace m2m::base {

// @src 0x1405f9eb0  TestGroup (made when missing), NumSteps = numSteps, StepIncr = stepIncr; with
// `withTime` also TimePerStep (= timeText, enabled = timeEnabled)
void addStepCounts(Props& p, bool withTime, bool timeEnabled, int numSteps, int stepIncr,
                   const qt::String& timeText);
// @src 0x1405fa560  EnvTemp = (unit == 2 ? english : si)
void addEnvTemp(Props& p, int unit, const qt::String& english, const qt::String& si);
// @src 0x1405faaa0  ConvectionCoef = (unit == 2 ? english : si)
void addConvection(Props& p, int unit, const qt::String& english, const qt::String& si);
// @src 0x1405fb020  StepBy (mode = stepBy), DistPerStep / TimePerStep, then 0x1405fb450(stepBy)
void addStepBy(Props& p, int unit, int stepBy, const qt::String& distEnglish, const qt::String& distSi,
               const qt::String& time);
// @src 0x1405fad10  under `group`: FrictionType = 0, FrictionCoef = (unit == 2 ? english : si)
void addFriction(Props& p, PropNode& group, int unit, double english, double si);
// @src 0x1405fa7d0  under `group`: HeatTransCoef = (unit == 2 ? english : si)
void addHeatTransCoef(Props& p, PropNode& group, int unit, const qt::String& english,
                      const qt::String& si);
// @src 0x1405fb450  StepBy mode change: swaps DistPerStep / TimePerStep in TestGroup, B+8 = mode
void setStepBy(Props& p, int mode);

// The base set* slots (0x1405fd0e0 .. 0x1405f89a0). False when `node`'s name is not a base slot.
bool applyBaseValue(Props& p, PropNode& node, const std::vector<PropNode*>& ancestors,
                    const qt::Variant& v);

// The TestGroup node of a props tree (null if absent).
PropNode* testGroup(Props& p);

}  // namespace m2m::base
