// m2m/engine_cogging_process.hpp: the Cogging operation's Process editor (SfCoggingProcess) at Save
// (MDT L4, src/engine/cogging_process.cpp).
#pragma once

#include "m2m/model.hpp"

namespace m2m::engine {

// SfCoggingProcess fill 0x140788cc0, then its vt+0x1c0 0x140789410 (signalUpdateDataInWidgetArea)
// on the operation's cogging model and engine objects; true (the editor is shown)
bool cogProcessUpdate(Project& p, Operation& op);

}  // namespace m2m::engine
