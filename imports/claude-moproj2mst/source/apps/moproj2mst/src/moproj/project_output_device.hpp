#pragma once

#include <memory>

#include "m2m/moproj_write.hpp"
#include "m2m/mst/stream.hpp"

namespace m2m::moproj::detail {

// Private per-call dependency for the native project writer. Ordinary Save
// still constructs a stack FileDevice; no factory is retained by the project.
class ProjectOutputDevice : public mst::OutputDevice {
public:
    virtual int error() const = 0;
};

class ProjectOutputFactory {
public:
    virtual ~ProjectOutputFactory() = default;
    // Return an object even when opened() is false. A null result is an
    // invalid service configuration, not the original failed-open outcome.
    virtual std::unique_ptr<ProjectOutputDevice> open(const qt::String& path) = 0;
};

bool saveProjectFileNativeWithFactory(
    Project& p, bool withBlobs, DiagramViews& views,
    const std::vector<const engine::FileEffects*>& earlier,
    engine::ConversionSession* session, engine::FileEffects* observation,
    ProjectOutputFactory& factory);

} // namespace m2m::moproj::detail
