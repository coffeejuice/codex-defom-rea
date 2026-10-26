// m2m/ht_schedule.hpp: the HeatTreat schedule (PROPS+0x68) reader and the HeatTreat props refresh
// (owner MDT L6, src/ops/heattreat_htwz.cpp; docs/re/moproj2mst/mdt/L6/spec.md section 6). One body
// each for the open path (props vt+0xc0 0x1411c0940), the loader (HeatTreat vt+0x58) and P16's
// MST writer, which reads the schedule.
#pragma once

#include <cstdio>
#include <memory>
#include <string>
#include <vector>

#include "m2m/data/heattreat.hpp"
#include "m2m/model.hpp"
#include "m2m/props.hpp"

namespace m2m::engine {
struct GuiModel;
}

namespace m2m::ht {

// @src 0x1411b3760  engine model of the schedule's owner operation (made on demand), or null
engine::GuiModel* scheduleModel(Project& p, data::HTSchedule& s);
// @src 0x1411b3540  HEAT TREATMENT defaults on that model, then schedule+0 = 1
void engineDefaults(Project& p, data::HTSchedule& s);
// @src 0x1411b4770  read <owner task folder>/HeatTreatWizard.HTWZ; -1 without an owner
int loadSchedule(Project& p, data::HTSchedule& s);
// @src 0x1411b4870  engine defaults when not applied yet, then the file (fopen "rt"); 0 when it
// does not open
int readWizard(Project& p, data::HTSchedule& s, const qt::String& taskDir);
// @src 0x1411b4ab0  the open file (V3, or V1/V2 through 0x1411b5e50); objNo is the caller's 1.
// m = the model 0x1411b3760 gives (null: only V3 reads)
int readFile(data::HTSchedule& s, std::FILE* f, int objNo, engine::GuiModel* m);
// readFile over bytes (a scratch file opened "rt", as the original reads the file)
int readBytes(data::HTSchedule& s, const std::string& bytes, engine::GuiModel* m);

// @src 0x1411c0730  HeatTreat props vt+0x68: block +0x58 <- sim control (false without one)
bool propsFromSimCtl(Project& p, HeatTreatProps& hp, void* simCtl);
// @src 0x1411bfd60  HeatTreat props vt+0x58
void propsRefresh(Project& p, HeatTreatProps& hp);

// helpers of the reader (heattreat_htwz.cpp) for the save side (src/engine/ht_save.cpp)
data::HTStep* stepAtOf(data::HTSchedule& s, int i);                                  // 0x1411b0a90
data::DYNARY* zoneAtOf(data::HTMedia& md, int j);                                    // 0x1411c2730
data::HTNodeEntry* entryAtOf(std::vector<std::unique_ptr<data::HTNodeEntry>>& l, int i);   // 0x1411c2c90 / 0x1411c28c0
int findEntryOf(const std::vector<std::unique_ptr<data::HTNodeEntry>>& l, int object, int media);   // 0x1411bc580 / 0x1411bc690
int findPairOf(const data::HTNodeEntry& e, int node, int value);                     // 0x1411b3230
void setPairOf(data::HTNodeEntry& e, int idx, int node, int value);                  // 0x1411b32e0
// @src 0x1411b38e0 (src/ops/heattreat.cpp)  <taskDir>\HT_Proc<k+1>.MOKEY
qt::String procPath(const data::HTSchedule& s, int k);
// The Process<i> property nodes of the schedule (owner L2, src/model/props_heattreat_slots.cpp)
// The "Thermal Schedule" group (block+0x40, set by the tree builder 0x1411bf050), or null
PropNode* scheduleGroup(HeatTreatProps& hp);
// @src 0x1411c0980 (tree part)  the first stepCount children of that group removed
void scheduleNodesRemove(HeatTreatProps& hp);
// @src 0x1411bfd60 (second loop)  Process<i+1>: TMAX, MEDNAM, ENVTMP (constant step+0x20 table),
// ENVATM (b1 and a constant step+0x98 table), ENVRAD
void scheduleNodesAdd(HeatTreatProps& hp, int i, const data::HTStep& st);

}  // namespace m2m::ht
