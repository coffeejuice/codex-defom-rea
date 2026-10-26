#pragma once

#include "m2m/qt4.hpp"

namespace m2m {
struct Project;
struct SubProject;
namespace engine { struct FileEffects; }
namespace doe {
struct Study;

enum class OutputRegionsMkdirPrefixOutcome {
    AttemptSucceeded,
    WarningModalRequired
};

struct OutputRegionsMkdirPrefixResult {
    qt::String path;
    bool succeeded = false;
    OutputRegionsMkdirPrefixOutcome outcome = OutputRegionsMkdirPrefixOutcome::WarningModalRequired;
};

// Isolated first SfOprDoeOutput Save directory effect (0x140e33210). A failed
// attempt is retained and returned at the original helper's warning-modal
// boundary; this helper neither shows the modal nor resumes Save afterwards.
// Native Save continues after the one-button warning is dismissed; this
// outcome is a continuation barrier, not a terminal Save result.
OutputRegionsMkdirPrefixResult attemptOutputRegionsMkdirPrefix(
    Project& project, SubProject& sub, Study& study, engine::FileEffects& effects);

} // namespace doe
} // namespace m2m
