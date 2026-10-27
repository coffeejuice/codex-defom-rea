#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <optional>
#include <vector>

namespace m2m::mst::detail {

// Opaque host identities only; this seam begins after SUB's model/flow member
// has been initialized. It does not encode native owner destruction.
struct Type3SubIdentity {
    std::uint64_t value = 0;
    friend bool operator==(const Type3SubIdentity&, const Type3SubIdentity&) = default;
};
struct Type3ModelIdentity {
    std::uint64_t value = 0;
    friend bool operator==(const Type3ModelIdentity&, const Type3ModelIdentity&) = default;
};
struct Type3ManagerIdentity {
    std::uint64_t value = 0;
    friend bool operator==(const Type3ManagerIdentity&, const Type3ManagerIdentity&) = default;
};
struct Type3OptimizerIdentity {
    std::uint64_t value = 0;
    friend bool operator==(const Type3OptimizerIdentity&, const Type3OptimizerIdentity&) = default;
};

enum class Type3SubConstructionEventKind {
    ManagerAllocated,
    ManagerConstructorPrefixCompleted,
    DirectoryAttempt,
    ManagerConstructed,
    ManagerPublished,
    OptimizerAllocated,
    OptimizerConstructed,
    OptimizerPublished,
    InFlightAllocationReleased,
};

struct Type3SubConstructionEvent {
    Type3SubConstructionEventKind kind{};
    std::uint64_t identity = 0;
    std::size_t ordinal = 0;
    bool result = true;
};

struct Type3SubConstructionReceipt {
    // These are the two raw SUB slots after the completed event prefix.
    std::optional<Type3ManagerIdentity> manager;
    std::optional<Type3OptimizerIdentity> optimizer;
    std::vector<Type3SubConstructionEvent> completed;
    bool completedNormally = false;
};

struct Type3SubConstructionHooks {
    std::function<std::optional<Type3ManagerIdentity>()> allocateManager;
    // Represents the recovered constructor work before directory attempts
    // (base/derived setup and initial naming), not native field-by-field state.
    std::function<bool(Type3ManagerIdentity, Type3SubIdentity,
                       Type3ModelIdentity)> managerConstructorPrefix;
    // Represents the constructor's completion boundary after directory work.
    std::function<bool(Type3ManagerIdentity, Type3SubIdentity,
                       Type3ModelIdentity)> constructManager;
    // Ordered like the constructor's directory helper; the first false ends
    // this sequence, but the manager constructor ignores that result.
    std::vector<std::function<bool()>> directoryAttempts;
    std::function<std::optional<Type3OptimizerIdentity>()> allocateOptimizer;
    std::function<bool(Type3OptimizerIdentity)> constructOptimizer;
};

// Models only the post-model phase of native FUN_1400b5470 for Type3. A false
// constructor hook means a nonreturning/failing boundary: only completed
// effects are retained. C++ exceptions, allocator internals and aliasing are
// deliberately outside this host projection.
Type3SubConstructionReceipt constructType3SubOwners(
    Type3SubIdentity sub, Type3ModelIdentity initializedModel,
    const Type3SubConstructionHooks& hooks);

}  // namespace m2m::mst::detail
