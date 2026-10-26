#pragma once

#include <cstdint>
#include <optional>

#include "m2m/doe_qstring_resource_owner.hpp"

namespace m2m::engine {

struct ProcessCodecSnapshot {
    doe::QStringCStringCodecKnowledge knowledge = doe::QStringCStringCodecKnowledge::Unknown;
    std::optional<std::uint64_t> moduleGeneration;
    std::optional<std::uintptr_t> cStringsPointer;
    std::optional<std::uint64_t> codecObjectGeneration;
};
enum class ProcessCodecSourceBoundary { CodecKnowledgeUnavailable };

// One explicit reconstructed process profile. Default construction has only
// fixed value state: no heap, Qt startup, native store or QString data owner.
// The owner outlives borrowed Sessions/Projects and is never a host singleton.
// This slice deliberately provides no production setter/read admission API.
class ProcessCodecOwner final {
public:
    constexpr ProcessCodecOwner() noexcept = default;
    ~ProcessCodecOwner() = default; // Adapter disposal is not native codec/CRT cleanup.
    ProcessCodecOwner(const ProcessCodecOwner&) = delete;
    ProcessCodecOwner& operator=(const ProcessCodecOwner&) = delete;
    ProcessCodecOwner(ProcessCodecOwner&&) = delete;
    ProcessCodecOwner& operator=(ProcessCodecOwner&&) = delete;

    // Static source identity only, not an observed loaded module/generation.
    static constexpr const char* qtCoreModuleSha256 = doe::kQStringPathQtCoreSha256;
    static constexpr std::uint32_t cStringsSymbolRva = doe::kQStringCStringCodecRva;
    constexpr ProcessCodecSnapshot snapshot() const noexcept { return state_; }
    constexpr ProcessCodecSourceBoundary sourceBoundary() const noexcept
    { return ProcessCodecSourceBoundary::CodecKnowledgeUnavailable; }

private:
    ProcessCodecSnapshot state_;
};

} // namespace m2m::engine
