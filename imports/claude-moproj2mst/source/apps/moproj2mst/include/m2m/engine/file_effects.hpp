#pragma once

#include "m2m/engine.hpp"

namespace m2m::engine {
// Shared81a6e0 QFile removal contract: physical native attempt at this point,
// ignored OS failure, no later replay; recording adapters keep queued effects.
void removeProjectFileAtCall(Project&, const String&, FileEffects&);
}
