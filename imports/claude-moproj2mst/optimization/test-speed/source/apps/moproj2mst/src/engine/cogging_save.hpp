// MDT L1 part B: the cogging geometry step of File > Save (SfOprCogging vt+0xd8 = 0x14075c260),
// docs/re/moproj2mst/mdt/L1/spec_L1B.md. It runs on the operation's engine model (engine.hpp) and
// the cogging model (data/cogging.hpp) before the Cogging.MDT writer 0x1407617e0.
//
// Refused with M2M_E_UNSUPPORTED (never guessed): the original's crash paths (object 9 copied from
// a missing object 8 with four dies, a missing die object in the stroke loop, an uninitialised
// centre or billet box read), MAN_COUPLE.DAT next to the project (the GUI looks it up in its
// process's current folder). The sim-name step 0x140054540 is packet S's storeSimNames and the
// slot syncs go to the loader's refreshSlotItems (0x140060100).
#pragma once

#include <cstddef>
#include <vector>

#include "engine_internal.hpp"
#include "m2m/data/cogging.hpp"
#include "m2m/engine.hpp"
#include "m2m/model.hpp"

namespace m2m::engine {

// @src 0x14075c260  SfOprCogging vt+0xd8: props = dynamic_cast<SfOprCoggingProperties*>, model =
// props+0x40, 0x140762ec0(model, 1), then the base 0x1405f22e0. Nothing without cogging props.
void coggingAfterSave(Project& p, Operation& op, FileEffects& fx);

// @src 0x140762ec0  geometry of the model from objects 1, 2, 4 (when full), then the placement of
// billet and dies (0x140763e50) and of the manipulators (0x140762f30)
void coggingGeometryStep(Project& p, Operation& op, data::CogModel& model, bool full, FileEffects& fx);

// @src 0x140766200  model geometry from object n (1 billet, 2 die, 4 manipulator)
void cogGeometryFromObject(Project& p, Operation& op, data::CogModel& model, int n);

// @src 0x140763e50  billet centring, dies 2 / 3 (/ 8 / 9), sim control MDSOBJ / SMAX
void cogPlaceBilletAndDies(Project& p, Operation& op, data::CogModel& model, FileEffects& fx);

// @src 0x140762f30  manipulators 4 .. 7
void cogPlaceManipulators(Project& p, Operation& op, data::CogModel& model, FileEffects& fx);

// @src 0x14075ad80  SfOprCogging vt+0x198: default contact pairs (dies, manipulators, mandrel with
// the billet; with springs also manipulator 4-5 and 6-7)
std::vector<ContactPair> cogDefaultPairs(Project& p, Operation& op);

// @src 0x1405f22e0  SfOprBase vt+0xd8 (procType 5 in DEF_GUI_MO: 0x140054540)
void baseAfterSave(Project& p, Operation& op);

// ---- GUI engine functions of this step (no DEF_PRE twin) ----------------------------------------
// @src 0x140c7ca50  delete object n of the model (engine 0x1408bdf10 = 14019cdc0) and re-pick PDIE
void deleteObject(GuiModel& w, int n);
// @src 0x140c7b630  PDIE re-pick
void repickPdie(GuiModel& w);
// @src 0x14085f6d0  movement defaults of an object's movement block (+0x2160)
void movementDefaults(std::byte* mv);

}  // namespace m2m::engine
