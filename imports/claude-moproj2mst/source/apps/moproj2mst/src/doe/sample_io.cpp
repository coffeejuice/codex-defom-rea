#include "m2m/doe_sample_io.hpp"

namespace m2m::doe {

bool loadSampleTable(Study& study, int requestedMode, const String& path, StudyIo& io)
{
    if (requestedMode < 0 || requestedMode >= 3) return false;
    if (!io.writeTextFile(path, qt::formatInt(requestedMode) + "\r\n")) return false;
    if (!study.samples.tables[requestedMode].empty()) {
        const int currentMode = study.samples.mode;
        if (currentMode < 0 || currentMode >= 3)
            throw Error(M2M_E_UNSUPPORTED,
                        "DOE current sample mode outside manager storage (0x1400d7a80)");
        // Preserve the original mismatch: clear CURRENT, validate REQUESTED.
        study.samples.tables[currentMode].clear();
        study.samples.invalid[requestedMode] = false;
    }
    return true;
}

}  // namespace m2m::doe
