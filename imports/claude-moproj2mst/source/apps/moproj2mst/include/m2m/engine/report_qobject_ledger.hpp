#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

namespace m2m::engine {

// Mechanical ledger for a single pending Report QObject prefix. This type is
// deliberately inert: callers must supply every observation, and it performs
// no native calls, memory writes, or WidgetRuntime/counter commits.
inline constexpr char kReportQObjectQtCoreSha256[] =
    "bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160";

enum class ReportQObjectSharedRefKind { QListData, QString, QVectorData };

struct ReportQObjectSharedRefObservation {
    std::string moduleSha256;
    std::uint32_t symbolRva = 0;
    ReportQObjectSharedRefKind kind = ReportQObjectSharedRefKind::QListData;
    std::uint32_t before = 0;
    std::uint32_t after = 0;
    std::uintptr_t constructorIdentity = 0;
};

struct ReportQObjectBacklinkObservation {
    std::uintptr_t objectIdentity = 0;
    std::uintptr_t privateIdentity = 0;
    std::uintptr_t objectPrivatePointer = 0;
    std::uintptr_t privateObjectPointer = 0;
    std::uintptr_t constructorIdentity = 0;
};

struct ReportQObjectTlsObservation {
    std::string moduleSha256;
    std::uint32_t tlsIndexBeforeInitializer = 0xffffffffu;
    std::uint32_t tlsGetValueIndex = 0xffffffffu;
    std::uintptr_t tlsValue = 0;
    std::uintptr_t currentThreadDataReturn = 0;
    std::uintptr_t privateThreadData = 0;
    std::uintptr_t constructorIdentity = 0;
};

struct ReportQObjectThreadRefObservation {
    std::string moduleSha256;
    std::uint32_t helperRva = 0;
    std::uintptr_t selectedThreadData = 0;
    std::uintptr_t privateThreadData = 0;
    std::uintptr_t helperArgument = 0;
    std::uintptr_t refcountAddress = 0;
    std::uint32_t before = 0;
    std::uint32_t after = 0;
    std::uintptr_t constructorIdentity = 0;
};

struct ReportQObjectCounterObservation {
    std::string moduleSha256;
    std::uint32_t counterRva = 0;
    std::uint32_t before = 0;
    std::uint32_t after = 0;
    std::uintptr_t constructorIdentity = 0;
};

enum class ReportQObjectLedgerStage {
    QListFirst,
    QString,
    QVector,
    QListSecond,
    Backlinks,
    ThreadSelection,
    ThreadReference,
    ProcessCounter,
    Complete
};

// The identity is an opaque stable-allocation token supplied by the owner. It
// is not converted into native bytes or treated as a real pointer by this type.
class ReportQObjectSharedRefLedger final {
public:
    explicit ReportQObjectSharedRefLedger(std::uintptr_t stableAllocationIdentity) noexcept;

    bool valid() const noexcept { return allocationIdentity_ != 0; }
    std::uintptr_t allocationIdentity() const noexcept { return allocationIdentity_; }
    ReportQObjectLedgerStage nextStage() const noexcept { return stage_; }
    bool complete() const noexcept { return stage_ == ReportQObjectLedgerStage::Complete; }

    // Requires the exact QtCore identity/RVA/kind for the current native event
    // and an observed uint32 increment of +1, including wraparound.
    bool observeSharedRef(const ReportQObjectSharedRefObservation&);
    bool observeBacklinks(const ReportQObjectBacklinkObservation&) noexcept;
    // Accepts only the initialized-index + nonnull TLS fast path, with exact
    // identity equality through currentThreadData and private+0x40.
    bool observeTlsFastPath(const ReportQObjectTlsObservation&);
    // Accepts the selected QThreadData pointer at the native helper and an
    // observed DWORD[RCX] +1 at the qualified helper RVA.
    bool observeThreadReference(const ReportQObjectThreadRefObservation&);
    // This is only a recorded last-stage observation. It does not call the
    // existing WidgetRuntimeState counter helper.
    bool observeProcessCounter(const ReportQObjectCounterObservation&);

    std::uint32_t retainedDelta(ReportQObjectSharedRefKind) const noexcept;
    const ReportQObjectSharedRefObservation* sharedEvent(std::size_t index) const noexcept;
    const ReportQObjectBacklinkObservation* backlinks() const noexcept;
    const ReportQObjectTlsObservation* tlsFastPath() const noexcept;
    const ReportQObjectThreadRefObservation* threadReference() const noexcept;
    const ReportQObjectCounterObservation* processCounter() const noexcept;

private:
    std::uintptr_t allocationIdentity_ = 0;
    ReportQObjectLedgerStage stage_ = ReportQObjectLedgerStage::QListFirst;
    std::array<ReportQObjectSharedRefObservation, 4> sharedEvents_{};
    std::array<std::uint32_t, 3> retainedDeltas_{};
    std::size_t sharedEventCount_ = 0;
    ReportQObjectBacklinkObservation backlinks_{};
    ReportQObjectTlsObservation tls_{};
    ReportQObjectThreadRefObservation threadRef_{};
    ReportQObjectCounterObservation counter_{};
    bool hasBacklinks_ = false;
    bool hasTls_ = false;
    bool hasThreadRef_ = false;
    bool hasCounter_ = false;
};

} // namespace m2m::engine
