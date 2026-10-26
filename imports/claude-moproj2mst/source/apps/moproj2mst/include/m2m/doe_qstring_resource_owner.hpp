#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>

#include "m2m/doe_qstring_path_owner.hpp"

namespace m2m::doe {

inline constexpr std::uint32_t kQStringCStringCodecRva = 0x2f3e10;
enum class QStringCStringCodecKnowledge { Unknown, Null, NonNull };
enum class QStringResourceStage {
    AwaitingCodecRead, CodecStateGuard, CodecDecoderGuard, NullBranchSelected,
    AllocationRequested, AllocationReturnedNull, AllocationAccepted, ConstructorReturned
};
enum class QStringResourceSourceBoundary { MissingSessionReceiverOccurrence };
// Explicit isolated allocator decisions, not host malloc or original observations.
enum class QStringResourceAllocationProfile { Success, ReturnNull };

struct QStringResourceCodecSnapshot {
    QStringCStringCodecKnowledge knowledge = QStringCStringCodecKnowledge::Unknown;
    std::uint64_t epoch = 0;
    std::string qualification;
};
struct QStringResourceOccurrenceSnapshot {
    std::size_t generation = 0;
    QStringResourceStage stage = QStringResourceStage::AwaitingCodecRead;
    std::optional<QStringCStringCodecKnowledge> selectedCodec;
    std::optional<std::uint64_t> selectedCodecEpoch;
    std::size_t nativeAllocationBytesRequested = 0;
    std::optional<QStringPathData> data;
    // Qualified initialized fields only; unknown flags/padding have no value.
    std::optional<std::size_t> codeUnitCount;
    std::optional<std::size_t> payloadOffset;
    std::optional<std::uint16_t> knownFlagMask;
    std::optional<std::uint16_t> knownFlagValue;
    bool trailingUtf16Nul = false;
};

// Single process-scope occurrence. Moving transfers the token; no diagnostic
// token can become an actual Session/receiver/source admission.
class QStringResourceIsolatedOccurrence final {
public:
    QStringResourceIsolatedOccurrence(const QStringResourceIsolatedOccurrence&) = delete;
    QStringResourceIsolatedOccurrence& operator=(const QStringResourceIsolatedOccurrence&) = delete;
    QStringResourceIsolatedOccurrence(QStringResourceIsolatedOccurrence&&) noexcept = default;
    QStringResourceIsolatedOccurrence& operator=(QStringResourceIsolatedOccurrence&&) noexcept = default;
private:
    friend class QStringResourceProcessScope;
    QStringResourceIsolatedOccurrence() = default;
    std::weak_ptr<const void> scope_;
    std::size_t ordinal_ = 0;
    std::uint64_t preparedEpoch_ = 0;
};

// One explicitly owned reconstructed process profile, never a host singleton
// or an implicit per-Session global. Caller serializes all state/ref operations.
// Default codec is unknown. The current interface has no actual source producer.
class QStringResourceProcessScope final {
public:
    QStringResourceProcessScope();
    ~QStringResourceProcessScope();
    QStringResourceProcessScope(const QStringResourceProcessScope&) = delete;
    QStringResourceProcessScope& operator=(const QStringResourceProcessScope&) = delete;

    QStringResourceCodecSnapshot codec() const;
    // Explicit diagnostic profile only; a label is not native state evidence.
    // Unknown invalidates prior knowledge; every update advances the epoch.
    void setIsolatedCodecProfile(QStringCStringCodecKnowledge, const std::string& qualification);
    void revoke(); // Invalidates admission; does not replay process/native frees.

    QStringResourceIsolatedOccurrence prepareIsolatedOccurrence();
    void readIsolatedCodec(QStringResourceIsolatedOccurrence&);
    QStringPathData constructIsolatedResource(
        QStringResourceIsolatedOccurrence&, QStringPathLease& destination,
        QStringResourceAllocationProfile);

    QStringResourceSourceBoundary sourceBoundary() const noexcept {
        return QStringResourceSourceBoundary::MissingSessionReceiverOccurrence;
    }
    // Always refuses, even for an isolated null profile. No actual receiver
    // occurrence/Session/process admission has been wired in this source slice.
    void constructSourceResource(QStringPathLease& destination);

    std::size_t occurrenceCount() const noexcept;
    QStringResourceOccurrenceSnapshot occurrence(std::size_t ordinal) const;
    const QStringPathOwner& pathOwner() const noexcept;
    void acquirePathAlias(const QStringPathData&, QStringPathLease&);
    void copyAlias(const QStringPathLease&, QStringPathLease&);
    void release(QStringPathLease&);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
    std::size_t checked(const QStringResourceIsolatedOccurrence&) const;
    void requireLive() const;
};

} // namespace m2m::doe
