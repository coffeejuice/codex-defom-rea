#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>

namespace m2m { struct FlowItem; struct ObjectRowPort; struct SubProject; }
namespace m2m::doe { struct SessionNotificationState; }
namespace m2m::engine { class ConversionSession; }

namespace m2m {
enum class ObjectRowNotificationKind { Destroyed2, Created2 };
enum class ObjectRowNotificationStage {
    AwaitSuppression, SuppressedCalleeReturn, BeforeKeyQStringConstruction
};
struct ObjectRowNotificationResult {
    ObjectRowNotificationStage stage;
    const char* signal;
    ObjectRowPort* targetInput;
    bool suppressionReadThisAttempt;
    bool flag90StoredThisAttempt;
    static constexpr bool completeForPublicGraph = false;
};

// Private diagnostic at an explicitly reached native 2cf3c0 row-event entry.
// Construction does not assert that the outer graph backend gate was reached.
class ObjectRowNotificationProgress final {
public:
    ObjectRowNotificationProgress(engine::ConversionSession&, FlowItem& target,
        std::size_t rowSlot, ObjectRowNotificationKind,
        std::string sourceOccurrence, std::string qualification);
    ObjectRowNotificationProgress(const ObjectRowNotificationProgress&) = delete;
    ObjectRowNotificationProgress& operator=(const ObjectRowNotificationProgress&) = delete;
    ObjectRowNotificationProgress(ObjectRowNotificationProgress&&) = delete;
    ObjectRowNotificationProgress& operator=(ObjectRowNotificationProgress&&) = delete;
    ObjectRowNotificationStage stage() const noexcept { return stage_; }
private:
    friend ObjectRowNotificationResult attemptObjectRowNotificationPrefix(
        engine::ConversionSession&, ObjectRowNotificationProgress&);
    std::uintptr_t sessionIdentity_;
    std::weak_ptr<doe::SessionNotificationState> owner_;
    std::weak_ptr<int> rowLifetime_;
    FlowItem* target_;
    ObjectRowPort* input_;
    std::size_t rowSlot_;
    ObjectRowNotificationKind kind_;
    ObjectRowNotificationStage stage_ = ObjectRowNotificationStage::AwaitSuppression;
};

ObjectRowNotificationResult attemptObjectRowNotificationPrefix(
    engine::ConversionSession&, ObjectRowNotificationProgress&);

enum class ObjectRowConnectionPrefixStage {
    DestroyedKeyUnavailable, CreatedKeyUnavailable, CompleteSuppressedEvents
};
struct ObjectRowConnectionPrefixResult {
    ObjectRowConnectionPrefixStage stage;
    ObjectRowPort* targetInput;
    ObjectRowPort* sourceOutput;
    ObjectRowNotificationResult destroyed;
    bool linked;
    bool createdEntered;
    static constexpr bool completeForPublicGraph = false;
};

// Synchronous, private row-only projection entered after the main edge has
// already been established. It cannot assert that public Open reached the
// native backend gate or that a live callback returned.
ObjectRowConnectionPrefixResult attemptObjectRowConnectionPrefix(
    engine::ConversionSession&, FlowItem& source, FlowItem& target,
    std::size_t rowSlot, std::string sourceOccurrence, std::string qualification);

struct ObjectRowBackendReadContext {
    engine::ConversionSession& session;
    const SubProject& modelOwner;
    FlowItem& source;
    FlowItem& target;
    std::size_t rowSlot;
    std::uint64_t instruction;
};
struct ObjectRowBackendRead {
    // nullopt means unavailable; an engaged nullptr is qualified current null.
    std::optional<const void*> backend;
    std::string qualification;
};
class ObjectRowBackendReadDriver {
public:
    virtual ~ObjectRowBackendReadDriver() = default;
    // This is an observation boundary, not a callback/receiver executor. Each
    // read needs its own current model-generation/occurrence qualification.
    virtual ObjectRowBackendRead readCurrentBackend(const ObjectRowBackendReadContext&)
    { return {}; }
};
enum class ObjectRowBackendConnectionStage {
    DestroyedBackendUnavailable, DestroyedKeyUnavailable,
    CreatedBackendUnavailable, CreatedKeyUnavailable, Complete
};
struct ObjectRowBackendConnectionResult {
    ObjectRowBackendConnectionStage stage;
    ObjectRowPort* targetInput;
    ObjectRowPort* sourceOutput;
    ObjectRowBackendRead beforeDestroyed;
    std::optional<ObjectRowBackendRead> beforeCreated;
    std::optional<ObjectRowNotificationResult> destroyed;
    std::optional<ObjectRowNotificationResult> created;
    // Actual return of the admitted private reciprocal-link projection; the
    // native Created2 branch tests this byte AFTER its second backend read.
    std::optional<std::uint8_t> linkReturnByte;
    bool linked = false;
    static constexpr bool completeForPublicGraph = false;
};

// Private row-only progression with independent 075ebd/07615a backend reads.
// The driver must retain current owner/lifetime evidence across each read;
// neither constructor null nor this diagnostic admits public graph execution.
ObjectRowBackendConnectionResult attemptObjectRowConnectionWithBackendPrefix(
    engine::ConversionSession&, FlowItem& source, FlowItem& target,
    std::size_t rowSlot, std::string sourceOccurrence, std::string qualification,
    ObjectRowBackendReadDriver&);
} // namespace m2m
