#pragma once
#include "m2m/qt4.hpp"

namespace m2m::qt {
// QtCore 670feeb0, for the absolute filesystem paths produced by Save.
// Returns false at the first failed component, retaining earlier directories.
// Relative/QDir resource-engine paths require their own recovered binding.
bool nativeMkpathAbsolute(const String& path);
}
