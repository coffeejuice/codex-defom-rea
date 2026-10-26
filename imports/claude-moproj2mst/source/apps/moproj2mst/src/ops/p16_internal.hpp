// P16-internal helpers shared by src/ops/{diestress,heattreat}.cpp and tests/p16 (not a seam).
#pragma once

#include <string>

#include "m2m/data/heattreat.hpp"
#include "m2m/qt4.hpp"

namespace m2m {
struct Operation;
}

namespace m2m::p16 {

// QString::replace(before, after, Qt::CaseInsensitive) for an ASCII `before` (Diestress ".DB")
qt::String replaceAllCi(const qt::String& s, const std::u16string& before, const std::u16string& after);

}  // namespace m2m::p16
