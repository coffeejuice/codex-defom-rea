// Pure path derivation for the Report .DS sibling document.
#pragma once

#include "m2m/qt4path.hpp"

namespace m2m::report {

// Mirrors the recovered Report path expression: QFileInfo(projectFile).absolutePath(),
// QDir::separator(), cleanPath(QFileInfo(projectFile).baseName()), then ".DS".
// This does not check existence or perform any document/file-system operation.
inline qt::String documentPath(const qt::String& projectFile)
{
    return qt::absolutePath(projectFile) + qt::kSep +
           qt::cleanPath(qt::baseName(projectFile)) + qt::L(".DS");
}

}  // namespace m2m::report
