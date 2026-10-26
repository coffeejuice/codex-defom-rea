#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace m2m::engine {
class ConversionSession;
}

namespace m2m::doe {
struct SessionNotificationState;

// Admission metadata only. These leases are opaque diagnostic identities, not
// QObjects, native allocations, child ownership or proof of native liveness.
using TableOwnerIdentity = std::shared_ptr<const void>;

enum class TableOwnerField { Unavailable, KnownNull, Cached };
// The native DoeTableRefresh notifier prefix either returns at suppression or
// reaches the still-unresolved registration/receiver dispatch after clearing
// flag90. Neither outcome alone means public Save completed.
enum class RefreshNotifierPrefixOutcome {
    SuppressedCalleeReturned,
    UnsuppressedDispatchRequired
};
enum class TableOwnerPhase {
    Idle, AllocationPending, ConstructorPending, RegistrationPending,
    ConstructorReturnPending, PublicationPending, NullPublicationPending,
    Published, ConstructorInterrupted, Deleting, DestructorReturned
};
enum class TableOwnerEvent {
    InitializeFields, PublishDock, BeginGetter, AllocationReturned,
    AllocationReturnedNull, TimerParentRecorded, ConstructorPrefixReturned,
    RegistrationReturned, ConstructorReturned, ConstructorInterrupted,
    PublishCache, DockInstallReturned, BeginDelete,
    DeletingDestructorReturned, ClearDeletedCache
};

struct TableOwnerObservation {
    // Required for every admitted source boundary. A label does not execute or
    // certify that boundary; a future Session binding must supply real evidence.
    std::string qualification;
    TableOwnerIdentity table, timer, dock;
    std::shared_ptr<SessionNotificationState> notifier;
};

struct TableOwnerGeneration {
    std::uint64_t id = 0;
    TableOwnerIdentity allocation, constructorResult, timer;
    bool allocationReturnedNull = false;
    bool timerParentRecorded = false;
    bool constructorPrefixReturned = false;
    bool registrationReturned = false;
    bool constructorReturned = false;
    bool cachePublished = false;
    bool dockInstallReturned = false;
    bool constructorInterrupted = false;
    bool deletingDestructorReturned = false;
    bool cacheCleared = false;
};

struct TableOwnerReceipt {
    TableOwnerEvent event;
    std::uint64_t sourceSite = 0, generation = 0;
    std::string qualification;
};

struct TableOwnerReport {
    TableOwnerField tableField = TableOwnerField::Unavailable;
    TableOwnerField dockField = TableOwnerField::Unavailable;
    TableOwnerPhase phase = TableOwnerPhase::Idle;
    TableOwnerIdentity cachedTable, dock;
    std::optional<std::uint64_t> cachedGeneration;
    std::vector<TableOwnerGeneration> generations;
    std::vector<TableOwnerReceipt> receipts;
    bool adapterScopeReleased = false;

    // Deliberately unavailable through every metadata transition. In particular
    // constructor/deletion receipts do not implement Qt timer/child teardown.
    std::optional<bool> timerActive, timerStopped, deleteWidgetEnabled;
    std::optional<bool> nativeReceiverAlive;
    std::optional<int> timerId;
};

// Isolated mainwindow+458/+460 source-stage skeleton, awaiting canonical Session
// ownership. It never mutates Session/notifier population, allocates a native
// table, invokes Qt, resolves current Project early or delivers refresh.
// Project release is intentionally independent of this main-window scope.
class DoeTableOwnerState final {
public:
    explicit DoeTableOwnerState(engine::ConversionSession& actualSession);
    ~DoeTableOwnerState();
    DoeTableOwnerState(const DoeTableOwnerState&) = delete;
    DoeTableOwnerState& operator=(const DoeTableOwnerState&) = delete;

    // Reject invalid order before changing metadata. Previously admitted stages
    // remain after later refusal. Successful ctor result must share BOTH pointer
    // and control block with its allocation; native address reuse is a new id.
    void record(engine::ConversionSession&, TableOwnerEvent, const TableOwnerObservation&);
    const TableOwnerReport& report() const noexcept;
    // Ends adapter admission only; never claims native delete or field clear.
    void releaseAdmissionScope() noexcept;
    // Models only the actual Session notifier's suppression/+90 prefix. Does
    // not resolve main-window +458, inspect key/receiver state, or invoke Qt.
    RefreshNotifierPrefixOutcome enterRefreshNotifierPrefix(engine::ConversionSession&) const;
    [[noreturn]] void requireRefreshDelivery(engine::ConversionSession&) const;

private:
    struct State;
    std::unique_ptr<State> state_;
};
} // namespace m2m::doe
