#include "m2m/doe_icon_shell_prefix.hpp"

#include <utility>

namespace m2m::doe {

const char* DoeIconShellPrefix::guardMessage() const noexcept
{
    if (stage_ != DoeIconShellStage::AddFileBoundary)
        return "DOE QIcon shell: construction was interrupted before the addFile boundary";
    return "DOE QIcon shell: addFile QFileInfo/plugin/engine ownership remains unsupported";
}

const DoeIconShellPrefix& appendDoeIconShellPrefix(DoeIconShellArena& arena)
{
    // Publish adapter metadata before native-represented payload allocation. If
    // vector growth throws, no payload existed; later preparation cannot drop it.
    auto prefix = std::unique_ptr<DoeIconShellPrefix>(new DoeIconShellPrefix);
    arena.prefixes_.push_back(std::move(prefix));
    auto& retained = *arena.prefixes_.back();

    // 140086d0d/non-null -> QtGui65014130: one eight-byte d slot, initially0.
    // This store cannot throw. Native null-allocation/fault behavior is excluded;
    // host allocation exceptions leave an incomplete adapter attempt, not false.
    retained.shell_ = std::make_unique<DoeIconShellValue>(DoeIconShellValue{nullptr});
    retained.stage_ = DoeIconShellStage::ShellConstructed;

    // 140086d53 -> QtCore6700ae00: signed width/height -1, not a24x24 load result.
    retained.size_ = DoeIconDefaultSize{-1, -1};
    retained.stage_ = DoeIconShellStage::SizeConstructed;
    retained.request_.emplace(DoeIconAddFileRequest{
        qt::L(":/SfTool/icon/tool/iconDoe_24_24.png"), *retained.size_, 0, 1});
    retained.stage_ = DoeIconShellStage::AddFileBoundary;
    return retained; // Before QIcon::addFile/engine or any graphic lookup/transfer.
}

} // namespace m2m::doe
