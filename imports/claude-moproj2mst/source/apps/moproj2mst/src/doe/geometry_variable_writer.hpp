#pragma once

#include "m2m/engine.hpp"
#include "m2m/doe_study.hpp"

namespace m2m::doe {

// Candidate-only byte projection for the retained one-row B122 geometry
// fixture. This does not write GEO payloads or participate in public Save.
bool writeB122GeometryVariablePrefix(const String& path,
                                     const Study& study,
                                     engine::FileEffects& effects);

} // namespace m2m::doe
