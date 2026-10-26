#include "output_regions_mkpath_prefix.hpp"

#include <filesystem>

#include "m2m/common.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/p17.hpp"
#include "m2m/native_directories.hpp"
#include "m2m/qt4path.hpp"

namespace m2m::doe {
namespace {
[[noreturn]] void refuse(const char* detail)
{
    throw Error(M2M_E_UNSUPPORTED, std::string("DOE Output Regions mkdir prefix: ") + detail);
}
}

OutputRegionsMkdirPrefixResult attemptOutputRegionsMkdirPrefix(
    Project& project, SubProject& sub, Study& study, engine::FileEffects& effects)
{
    if (!effects.nativeDirectories)
        refuse("immediate native directory profile is unavailable");
    if (sub.project != &project || activeSubProject(project) != &sub || sub.type != 2 ||
        !sub.ownedDoeStudy || sub.ownedDoeStudy.get() != &study || sub.doe != &study ||
        study.sub != &sub || study.name.isEmpty())
        refuse("actual active Type2 Project/SubProject/Study owner is unavailable");

    const auto control = mst::doeControlDir(sub, &study.name);
    const auto path = qt::cleanPath(control + qt::kSep + qt::L("Regions"));
    const std::filesystem::path filesystemPath(path.u16());
    if (path.isEmpty() || !filesystemPath.is_absolute())
        refuse("absolute Control/Regions path required by the candidate native directory adapter");

    //0x140e33210 -> 0x14081afb0(path,true): this is the first DoeOutput
    //effect, before the viewer-model check and Regions cleanup. The original
    //helper displays a modal on false and returns true only after that boundary.
    const bool succeeded = qt::nativeMkpathAbsolute(path);
    effects.directoryAttempts.push_back({path, succeeded});
    return {path, succeeded, succeeded ? OutputRegionsMkdirPrefixOutcome::AttemptSucceeded
                                       : OutputRegionsMkdirPrefixOutcome::WarningModalRequired};
}

} // namespace m2m::doe
