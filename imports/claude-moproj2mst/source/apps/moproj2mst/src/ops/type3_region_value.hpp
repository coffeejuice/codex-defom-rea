#pragma once

#include "m2m/qt4.hpp"

namespace m2m::mst::detail {

// Applies the successful Type3 REGIONS QString rewrite to supplied row text.
// This does not read/split .OPT lines or activate the public Type3 parser.
qt::String deriveType3RegionValue(const qt::String& rowText);

}  // namespace m2m::mst::detail
