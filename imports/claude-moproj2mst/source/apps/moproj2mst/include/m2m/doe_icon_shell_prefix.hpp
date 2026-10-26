#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

#include "m2m/qt4.hpp"

namespace m2m::doe {

// QIcon is an eight-byte value wrapper here, not a QObject or an icon engine.
// The recovered default constructor writes only this null private-data slot.
struct DoeIconShellValue {
    const void* privateData;
};
static_assert(sizeof(DoeIconShellValue) == 8, "Recovered x64 QIcon shell must be eight bytes");

struct DoeIconDefaultSize {
    std::int32_t width;
    std::int32_t height;
};

struct DoeIconAddFileRequest {
    qt::String resource;
    DoeIconDefaultSize size;
    int mode;
    int state;
};

// Adapter stages, not native fields or evidence that Qt/resource delivery ran.
enum class DoeIconShellStage {
    AwaitingAllocation, ShellConstructed, SizeConstructed, AddFileBoundary
};
enum class DoeIconShellNextBoundary { AddFile };

class DoeIconShellArena;
class DoeIconShellPrefix;
const DoeIconShellPrefix& appendDoeIconShellPrefix(DoeIconShellArena& arena);

class DoeIconShellPrefix final {
public:
    DoeIconShellStage stage() const noexcept { return stage_; }
    const DoeIconShellValue* shell() const noexcept { return shell_.get(); }
    const std::optional<DoeIconDefaultSize>& size() const noexcept { return size_; }
    const std::optional<DoeIconAddFileRequest>& request() const noexcept { return request_; }
    std::optional<DoeIconShellNextBoundary> nextBoundary() const noexcept
    {
        return stage_ == DoeIconShellStage::AddFileBoundary
            ? std::optional<DoeIconShellNextBoundary>(DoeIconShellNextBoundary::AddFile)
            : std::nullopt;
    }
    const char* guardMessage() const noexcept;

private:
    friend const DoeIconShellPrefix& appendDoeIconShellPrefix(DoeIconShellArena&);
    DoeIconShellPrefix() = default;
    // Payload and metadata are distinct. Neither owns an engine or graphic item.
    std::unique_ptr<DoeIconShellValue> shell_;
    DoeIconShellStage stage_ = DoeIconShellStage::AwaitingAllocation;
    std::optional<DoeIconDefaultSize> size_;
    std::optional<DoeIconAddFileRequest> request_;
};

// Caller-owned adapter allocation retention only. No production Session binding,
// native leak cleanup or Close is inferred. Observations borrow this arena's life.
class DoeIconShellArena final {
public:
    DoeIconShellArena() = default;
    DoeIconShellArena(const DoeIconShellArena&) = delete;
    DoeIconShellArena& operator=(const DoeIconShellArena&) = delete;
    DoeIconShellArena(DoeIconShellArena&&) = delete;
    DoeIconShellArena& operator=(DoeIconShellArena&&) = delete;

    std::size_t size() const noexcept { return prefixes_.size(); }
    const DoeIconShellPrefix& at(std::size_t ordinal) const { return *prefixes_.at(ordinal); }

private:
    friend const DoeIconShellPrefix& appendDoeIconShellPrefix(DoeIconShellArena&);
    std::vector<std::unique_ptr<DoeIconShellPrefix>> prefixes_;
};

// Isolated admitted nonnull-allocation path: QIcon d=null, QSize(-1,-1), fixed
// request, then return at AddFile. Never calls QFileInfo/plugin/engine/graphics,
// dispatches a receiver or claims public Type2 Save completed. A later guard must
// leave this caller's arena intact; arena destruction is adapter cleanup only.
const DoeIconShellPrefix& appendDoeIconShellPrefix(DoeIconShellArena& arena);

} // namespace m2m::doe
