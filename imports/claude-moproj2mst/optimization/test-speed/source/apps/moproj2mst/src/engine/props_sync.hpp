// SfOprBaseProperties: data block (PROPS+0x30) from an engine sim control, called by the classes'
// props vt+0x58 on load (owner: MDT L2; the Express family of L3 calls them too).
// A null sim control means the props owner's (S+0x750, 0x1405f8df0); false when there is none.
#pragma once

#include "m2m/model.hpp"
#include "m2m/props.hpp"

namespace def::model {
class Cls_1401e7b10;
class DB_STEP_DATA;
}

namespace m2m::engine {

// @src 0x1405f8df0  DB_STEP_DATA of the props' owner operation, or null
def::model::DB_STEP_DATA* propsStep(Project& p, Props& pr);
// @src 0x1405fb720  NumSteps, StepIncr, TimePerStep (DTMAX 1)
bool propsStepsFromSimCtl(Project& p, Props& pr, def::model::Cls_1401e7b10* sc);
// @src 0x1405fbae0  EnvTemp (ENVTMP constant)
bool propsEnvTempFromSimCtl(Project& p, Props& pr, def::model::Cls_1401e7b10* sc);
// @src 0x1405fbe30  ConvectionCoef (CNVCOF constant)
bool propsConvectionFromSimCtl(Project& p, Props& pr, def::model::Cls_1401e7b10* sc);
// @src 0x1405fc180  StepBy (DTTYPE mode), then DistPerStep or TimePerStep
bool propsStepByFromSimCtl(Project& p, Props& pr, def::model::Cls_1401e7b10* sc);
// @src 0x1405fcb90  HeatingTime = t with the time unit of the owner's sim control
void propsSetHeatingTime(Project& p, Props& pr, double t);

}  // namespace m2m::engine
