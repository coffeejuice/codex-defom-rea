#pragma once

#include "m2m/doe_study.hpp"

namespace m2m::doe {

// 0x1400d7a80 -> 0x140915360, empty-table branch. The misleading load routine
// truncates/writes a mode header. Only failed open prevents its memory effects;
// write/flush/close failure after open does not. This overload does immediate I/O.
// The existing FileEffects overload remains a successful-open planning seam.
bool loadSampleTable(Study& study, int requestedMode, const String& path, StudyIo& io);

}  // namespace m2m::doe
