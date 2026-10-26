#include "m2m/report_ds_path.hpp"

#include "m2m/qt4path.hpp"

namespace m2m {

qt::String reportDsPath(const qt::String& projectPath)
{
    const qt::String directory = qt::convertSeparators(qt::absolutePath(projectPath));
    const qt::String base = qt::cleanPath(qt::baseName(projectPath));
    return directory + qt::kSep + base + qt::String(u".DS");
}

}  // namespace m2m
