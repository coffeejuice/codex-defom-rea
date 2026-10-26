#include "m2m/engine/simctrl_ppv_open_sequence.hpp"

#include "m2m/model.hpp"

#include <limits>
#include <span>

namespace m2m::engine::simcontrol300 {
bool readPpvForOperation(bool opened, std::span<const qt::String> lines,
                         Operation& operation, bool hasPredecessor);
}

namespace m2m::engine::simctrl_ppv {
namespace {

bool propagateCount(Operation& operation, bool hasPredecessor)
{
    if (!operation.recoveryB) return false;
    std::size_t count = operation.recoveryB->size();
    if (hasPredecessor) {
        auto* shared = recoveryListA(operation);
        if (!shared) return false;
        count += shared->size();
    }
    if (count > static_cast<std::size_t>(std::numeric_limits<std::int32_t>::max())) return false;
    const auto nativeCount = static_cast<std::int32_t>(count);
    for (const auto& slot : operation.objs)
        if (slot) slot->sv.setPpvCount(nativeCount);
    return true;
}

} // namespace

OpenSequenceStatus readPpvOpenSequenceImpl(const Project& project, const qt::String& path,
                                           Operation& operation, bool hasPredecessor,
                                           bool directFile)
{
    if (!operation.recoveryB || (hasPredecessor && !recoveryListA(operation)))
        return OpenSequenceStatus::OwnerUnavailable;

    const FileLines input = directFile ? readPpvFileLinesFromDisk(path)
                                       : readPpvFileLines(project, path);
    if (input.status == FileReadStatus::UnsupportedEncodingOrLineEnding)
        return OpenSequenceStatus::UnsupportedInput;

    if (input.status == FileReadStatus::OpenFailed) {
        (void)m2m::engine::simcontrol300::readPpvForOperation(false, {}, operation,
                                                               hasPredecessor);
        return propagateCount(operation, hasPredecessor)
            ? OpenSequenceStatus::OpenFailed : OpenSequenceStatus::OwnerUnavailable;
    }

    const bool parsed = m2m::engine::simcontrol300::readPpvForOperation(
        true, std::span<const qt::String>(input.lines), operation, hasPredecessor);
    if (!propagateCount(operation, hasPredecessor)) return OpenSequenceStatus::OwnerUnavailable;
    return parsed ? OpenSequenceStatus::Applied : OpenSequenceStatus::ParserReturnedFalse;
}

OpenSequenceStatus readPpvOpenSequence(const Project& project, const qt::String& path,
                                       Operation& operation, bool hasPredecessor)
{
    return readPpvOpenSequenceImpl(project, path, operation, hasPredecessor, false);
}

OpenSequenceStatus readPpvOpenSequenceFromDisk(const Project& project, const qt::String& path,
                                               Operation& operation, bool hasPredecessor)
{
    return readPpvOpenSequenceImpl(project, path, operation, hasPredecessor, true);
}

} // namespace m2m::engine::simctrl_ppv
