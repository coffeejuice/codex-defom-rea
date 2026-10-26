#pragma once

#include "m2m/qt4path.hpp"
#include <optional>
#include <vector>

namespace m2m::detail {

// Collect the five project-level mkpath decisions reached by the qualifying
// setOperationId branch. This records descriptions only; it performs no I/O.
inline void collectOperationProjectDirectoryDecisions(
    const qt::String& cleanedProjectDirectory, int operationId,
    std::optional<int> activeSubProjectType,
    std::vector<qt::String>& decisions)
{
    if (operationId < 1) return;

    constexpr const char* subdirectories[] = {
        "Operations", "Objects", "Materials", "Equipments", "Lubricants"};
    for (const char* subdirectory : subdirectories) {
        qt::String path = cleanedProjectDirectory;
        path += qt::String(std::u16string(1, qt::kSep)) + qt::L(subdirectory);
        decisions.push_back(qt::cleanPath(path));
    }

    if (activeSubProjectType && *activeSubProjectType == 4) {
        qt::String path = cleanedProjectDirectory;
        path += qt::String(std::u16string(1, qt::kSep)) + qt::L("DieStress");
        decisions.push_back(qt::cleanPath(path));
    }
}

}  // namespace m2m::detail
