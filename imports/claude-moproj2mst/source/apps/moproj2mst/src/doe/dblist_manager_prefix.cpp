#include "dblist_manager_prefix.hpp"

#include <limits>

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"
#include "m2m/model.hpp"

namespace m2m::doe {

qt::String isolatedDbListStoredPathValue(const engine::DbHandle& child)
{
    return child.pathSet ? child.path : qt::String();
}

DbListManagerPrefix::DbListManagerPrefix(const Project* project,
    const std::shared_ptr<StudyIo>& io, const std::shared_ptr<engine::ModelManager>& manager)
    : project_(project), io_(io), manager_(manager) {}

void DbListManagerPrefix::validateAgainst(engine::ConversionSession& session) const
{
    const auto* project = session.liveProject();
    const auto io = io_.lock();
    const auto manager = manager_.lock();
    const auto actualManager = session.modelManagerOwner();
    if (!project || project != project_ || !io || project->doeIo != io ||
        project->doeIo.owner_before(io) || io.owner_before(project->doeIo) ||
        !manager || manager != actualManager ||
        manager.owner_before(actualManager) || actualManager.owner_before(manager) ||
        io->modelManager() != manager.get() || manager->ownerSession() != &session)
        throw Error(M2M_E_UNSUPPORTED,
            "DBLIST manager prefix needs its actual live Session/Project/I/O/manager owners");
}

DbListManagerPrefix bindDbListManagerPrefix(engine::ConversionSession& session)
{
    auto* project = session.liveProject();
    DbListManagerPrefix result(project, project ? project->doeIo : nullptr,
                              session.modelManagerOwner());
    result.validateAgainst(session);
    return result;
}

engine::DbHandle* DbListManagerPrefix::lookupOrStopBeforeConstruction(
    engine::ConversionSession& session, const qt::String& path)
{
    validateAgainst(session);
    const auto manager = manager_.lock();
    if (manager->size() > static_cast<std::size_t>(std::numeric_limits<int>::max()))
        throw Error(M2M_E_UNSUPPORTED, "DBLIST manager count exceeds native QList integer count");
    attempts_.push_back({path});
    auto& attempt = attempts_.back();
    for (std::size_t i = 0; i < manager->size(); ++i) {
        auto* child = manager->modelAt(static_cast<int>(i));
        if (!child)
            throw Error(M2M_E_UNSUPPORTED, "DBLIST manager native null child dereference remains unsupported");
        attempt.visited.push_back(child);
        if (isolatedDbListStoredPathValue(*child) == path) attempt.lastMatch = child;
    }
    if (attempt.lastMatch) return attempt.lastMatch;
    attempt.stoppedBeforeNativeConstruction = true;
    throw Error(M2M_E_UNSUPPORTED,
        "DBLIST absent child requires native manager virtual+68 construction and path-setting effects");
}

} // namespace m2m::doe
