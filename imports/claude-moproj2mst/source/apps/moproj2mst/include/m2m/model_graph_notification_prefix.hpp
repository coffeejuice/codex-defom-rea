#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

namespace m2m { struct FlowItem; struct ObjectRowPort; }
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
} // namespace m2m
