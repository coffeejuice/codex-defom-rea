#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>

#include "m2m/qt4.hpp"

namespace m2m::doe {

inline constexpr char kQStringPathQtCoreSha256[] =
    "bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160";
inline constexpr std::uint32_t kQStringPathSharedNullRva = 0x2f0b30;

// Explicit observation/profile input, never a default native reference count.
// A diagnostic qualification does not certify an original runtime observation.
struct QStringPathRefObservation {
    std::optional<std::uint32_t> referenceCount;
    std::string qualification;
};
struct QStringPathSharedNullObservation {
    std::string moduleSha256;
    std::uint32_t symbolRva = 0;
    QStringPathRefObservation reference;
};

class QStringPathOwner;
class QStringResourceProcessScope;

// An observer of reconstructed data identity, not an additional QString lease.
// Copying this handle does not change the represented native reference word.
class QStringPathData final {
public:
    friend bool operator==(const QStringPathData& a, const QStringPathData& b) noexcept
    {
        return a.ordinal_ == b.ordinal_ && !a.scope_.owner_before(b.scope_) &&
               !b.scope_.owner_before(a.scope_);
    }
private:
    friend class QStringPathOwner;
    std::weak_ptr<const void> scope_;
    std::size_t ordinal_ = 0;
};

enum class QStringPathLeaseState { Empty, Active, Released };
class QStringPathLease final {
public:
    QStringPathLease() = default;
    QStringPathLease(const QStringPathLease&) = delete;
    QStringPathLease& operator=(const QStringPathLease&) = delete;
    QStringPathLease(QStringPathLease&&) = delete;
    QStringPathLease& operator=(QStringPathLease&&) = delete;
    QStringPathLeaseState state() const noexcept { return state_; }
    // Adapter destruction is inert. Invoke owner.release at the actual reached
    // QString destructor phase; do not invent native cleanup during C++ unwind.
private:
    friend class QStringPathOwner;
    std::weak_ptr<const void> scope_;
    std::size_t ordinal_ = 0;
    QStringPathLeaseState state_ = QStringPathLeaseState::Empty;
};

enum class QStringPathDataKind { Input, SharedNull };
enum class QStringPathDataOrigin { ObservationProfile, ResourceNullCodecProjection };
struct QStringPathDataSnapshot {
    QStringPathDataKind kind;
    std::optional<std::uint32_t> referenceCount;
    bool dataDeallocationPending = false;
    std::string qualification;
    QStringPathDataOrigin origin = QStringPathDataOrigin::ObservationProfile;
};

// Isolated native-data/ref projection. It does not alter qt::String or coerce its
// text/C++ ownership into native counters. The single canonical shared-null
// record belongs to this declared adapter scope, not an inferred live process.
// Operations require caller serialization, even though ref words are atomic.
class QStringPathOwner final {
public:
    QStringPathOwner();
    ~QStringPathOwner();
    QStringPathOwner(const QStringPathOwner&) = delete;
    QStringPathOwner& operator=(const QStringPathOwner&) = delete;

    QStringPathData observeInput(const qt::String& text, const QStringPathRefObservation&);
    QStringPathData bindSharedNull(const QStringPathSharedNullObservation&);
    std::optional<QStringPathData> sharedNull() const;
    QStringPathDataSnapshot snapshot(const QStringPathData&) const;
    const qt::String& text(const QStringPathData&) const;
    QStringPathData data(const QStringPathLease&) const;

    // Adopt one already-observed source QString slot, once per Input record.
    // This is adapter admission, not a constructor call or a ref increment.
    void adoptExistingInput(const QStringPathData&, QStringPathLease& destination);
    // No-backslash QDir branch: same data pointer followed by native ref+1.
    void acquirePathAlias(const QStringPathData&, QStringPathLease& destination);
    void copyAlias(const QStringPathLease& source, QStringPathLease& destination);
    void acquireSharedNull(QStringPathLease& destination);
    // Consume exactly once, decrement, then guard at QString data deallocation
    // if zero. No data free is fabricated; diagnostics survive that boundary.
    void release(QStringPathLease&);

private:
    friend class QStringResourceProcessScope;
    // Source-qualified fixed-resource branch; no caller-supplied reference count.
    // The process scope admits only an explicitly isolated null-codec occurrence.
    QStringPathData constructFixedResourceInput(QStringPathLease& destination);
    struct Impl;
    std::unique_ptr<Impl> impl_;
    std::size_t checked(const QStringPathData&) const;
    std::size_t checked(const QStringPathLease&) const;
    void acquire(std::size_t ordinal, QStringPathLease& destination, bool increment);
};

} // namespace m2m::doe
