// Pure File > Open/Save Report document sibling path (native 0x1412aeb30).
#pragma once

#include "m2m/qt4.hpp"

namespace m2m {

// Derive the Report document path from the project filename only. This does not
// create/access the SP+0x30 document, read/write a .DS file, or enable Report.
qt::String reportDsPath(const qt::String& projectPath);

}  // namespace m2m
