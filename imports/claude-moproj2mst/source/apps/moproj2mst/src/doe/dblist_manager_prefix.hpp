#pragma once

#include <memory>
#include <vector>

#include "m2m/qt4.hpp"

namespace m2m {
struct Project;
namespace engine { struct DbHandle; class ConversionSession; class ModelManager; }
namespace doe {
class StudyIo;

// Native cdd190(...,true): the false pathSet branch returns a null QString;
// the true branch copies the stored path without basename conversion.
qt::String isolatedDbListStoredPathValue(const engine::DbHandle& child);

struct DbListManagerLookupAttempt {
    qt::String resolvedPath;
    std::vector<const engine::DbHandle*> visited; // Diagnostic aliases only.
    engine::DbHandle* lastMatch = nullptr;
    bool stoppedBeforeNativeConstruction = false;
};

// Isolated c78920/c78e80 existing-file branch, after the caller's independently
// qualified mode/gate, path resolution and QFileInfo::exists()==true. This
// helper does not establish those inputs or perform filesystem access.
class DbListManagerPrefix final {
public:
    // c78c10 retains the last matching child. Returned identity is borrowed
    // from the actual manager, not a new child or selected current model.
    // A miss refuses before virtual+68 native construction; no child/path/ID,
    // list append, QObject connections or current selection is fabricated.
    engine::DbHandle* lookupOrStopBeforeConstruction(
        engine::ConversionSession& actualSession, const qt::String& resolvedExistingPath);
    const auto& attempts() const noexcept { return attempts_; }
    void validateAgainst(engine::ConversionSession& actualSession) const;

private:
    friend DbListManagerPrefix bindDbListManagerPrefix(engine::ConversionSession& actualSession);
    DbListManagerPrefix(const Project*, const std::shared_ptr<StudyIo>&,
                        const std::shared_ptr<engine::ModelManager>&);
    const Project* project_;
    std::weak_ptr<StudyIo> io_;
    std::weak_ptr<engine::ModelManager> manager_;
    std::vector<DbListManagerLookupAttempt> attempts_;
};

// Requires the live Session Project's already bound StudyIo and the same
// mainwindow+50 manager. Does not create/rebind I/O or model owners.
DbListManagerPrefix bindDbListManagerPrefix(engine::ConversionSession& actualSession);

} // namespace doe
} // namespace m2m
