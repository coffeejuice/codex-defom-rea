#include "m2m/doe_fileinfo_private_prefix.hpp"

#include <utility>

#include "m2m/common.hpp"

namespace m2m::doe {

const char* DoeFileInfoPrivatePrefix::guardMessage() const noexcept
{
    if (stage_ != DoeFileInfoPrivateStage::ReferenceInitialized)
        return "DOE QFileInfo private prefix: allocation did not return";
    return "DOE QFileInfo private prefix: QString path/shared-null ownership remains unsupported";
}

const DoeFileInfoPrivatePrefix& appendDoeFileInfoPrivatePrefix(
    DoeFileInfoPrivateArena& arena, const DoeIconShellPrefix& icon)
{
    // The empty request/already-populated QIcon branches do not enter this
    // QFileInfo constructor. Scope refusal precedes any new adapter allocation.
    if (icon.stage() != DoeIconShellStage::AddFileBoundary || !icon.shell() ||
        icon.shell()->privateData != nullptr || !icon.size() || !icon.request())
        throw Error(M2M_E_UNSUPPORTED, "DOE QFileInfo private prefix needs the admitted new QIcon shell/request");
    const auto& request = *icon.request();
    if (request.resource.u16() != u":/SfTool/icon/tool/iconDoe_24_24.png" ||
        icon.size()->width != -1 || icon.size()->height != -1 ||
        request.size.width != -1 || request.size.height != -1 || request.mode != 0 || request.state != 1)
        throw Error(M2M_E_UNSUPPORTED, "DOE QFileInfo private prefix supports only the fixed DOE resource request");

    auto prefix = std::unique_ptr<DoeFileInfoPrivatePrefix>(new DoeFileInfoPrivatePrefix);
    arena.prefixes_.push_back(std::move(prefix));
    auto& retained = *arena.prefixes_.back();
    // 670b9a1f: admitted nonnull 0xe0 allocation; 670b8052: ref=0. A user-provided
    // constructor leaves the tail indeterminate: do not value-initialize it.
    retained.value_ = std::unique_ptr<DoeFileInfoPrivateValue>(new DoeFileInfoPrivateValue);
    retained.stage_ = DoeFileInfoPrivateStage::ReferenceInitialized;
    return retained; // Before 670b8059 QDir::fromNativeSeparators/QString ownership.
}

} // namespace m2m::doe
