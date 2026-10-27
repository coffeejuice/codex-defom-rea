#include "m2m/engine/opt_writer_unwind_prefix.hpp"

namespace m2m::engine {

OptWriterUnwindPrefix projectOptWriterState4Unwind(
    const OptWriterUnwindPrepared& prepared)
{
    OptWriterUnwindPrefix out;
    out.identities = prepared.identities;
    out.exceptionIdentity = prepared.exceptionIdentity;
    if (prepared.admission != OptWriterUnwindAdmission::AdmittedCppUnwindAtState4)
        return out;

    const auto& ids = prepared.identities;
    if (!prepared.exceptionIdentity || !ids.stream || !ids.file || !ids.path ||
        ids.stream == ids.file || ids.stream == ids.path || ids.file == ids.path) {
        out.stop = OptWriterUnwindStop::InvalidPreparedIdentity;
        return out;
    }

    const std::array<OptWriterUnwindResource, 3> resources{
        OptWriterUnwindResource::Stream,
        OptWriterUnwindResource::File,
        OptWriterUnwindResource::Path};
    const std::array<std::uint64_t, 3> tokens{ids.stream, ids.file, ids.path};
    const std::array<int, 3> successors{1, 0, -1};
    out.state = 4;
    for (std::size_t i = 0; i < resources.size(); ++i) {
        const int from = *out.state;
        const int to = successors[i];
        out.state = to;
        out.events.push_back({OptWriterUnwindEventKind::SuccessorStatePublished,
            resources[i], tokens[i], from, to});
        out.events.push_back({OptWriterUnwindEventKind::CleanupEntered,
            resources[i], tokens[i], from, to});
        out.awaitingReturn = resources[i];
        if (prepared.cleanupReturns[i] != OptWriterCleanupReturn::Returned) {
            out.stop = OptWriterUnwindStop::CleanupReturnUnavailable;
            return out;
        }
        out.events.push_back({OptWriterUnwindEventKind::CleanupReturned,
            resources[i], tokens[i], from, to});
        out.awaitingReturn.reset();
    }
    out.stop = OptWriterUnwindStop::State4CleanupReturned;
    return out;
}

} // namespace m2m::engine
