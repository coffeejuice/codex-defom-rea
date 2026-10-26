// MDT L6S: save-current-operation slot bodies of the HeatTreat, Distortion, Diestress and
// DiestressBM classes (src/engine/save_l6.cpp), called from packet S's saveCurrentOperation.
#pragma once

#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/props.hpp"
#include "m2m/data/heattreat.hpp"

namespace def::model {
class Cls_1401e7b10;
}

namespace m2m::engine {

// @src 0x1411c05a0  HeatTreat props vt+0x70: sim control <- block +0x58
void heatTreatPropsToSimCtl(Project& p, HeatTreatProps& hp, def::model::Cls_1401e7b10* sc);
// @src 0x1411bfa20  HeatTreat props vt+0x60
void heatTreatPropsToEngine(Project& p, Operation& op);
// @src 0x1402b5be0  SfOprDiestress vt+0xe8
void diestressSaveKey(Project& p, Operation& op, FileEffects& fx);
// @src 0x140299870  SfOprDiestressBM vt+0xd8 before its base call 0x1405f22e0
void diestressBMBeforeSimNames(Project& p, Operation& op);
// @src 0x140299b60  SfOprDiestressBM vt+0x80
void diestressBMSaveWizard(Project& p, Operation& op, FileEffects& fx);
// @src 0x1411b1320 / 0x1411aed00  HeatTreat vt+0x68 after its base: 0x1411b71c0(sched, 0), 0x1411b3b20
void heatTreatSaveSchedule(Project& p, Operation& op, FileEffects& fx);
// @src 0x1411b3b20  HeatTreat schedule: ECC/ECTMFN from the model (0x1411bae90(1)), then HTWZ
void heatTreatWriteSchedule(Project& p, data::HTSchedule& s, FileEffects& fx, const Ctx* ctx = nullptr);
// fills the L6 save entries of mst::g_l6Slots
void installL6SaveSlots();

}  // namespace m2m::engine
