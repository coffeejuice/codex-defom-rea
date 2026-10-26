#pragma once

#include "m2m/doe_study.hpp"
#include "geometry_variable_prefix.hpp"

namespace m2m::doe {

// Binds the bounded B122 value record to one fresh Study-owned Var. This is
// isolated model state only; it does not perform GEO I/O or public parsing.
Var& bindB122GeometryVariablePrefix(Study& study, const B122GeometryVariableRecord& record);

} // namespace m2m::doe
