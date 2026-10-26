#pragma once

#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include "m2m/engine/widget_area_owner_state.hpp"

namespace m2m::engine {

class WidgetAreaLazyGetter;

// Canonical projection of one actual parent's raw +3c8 field. Null is distinct
// from unavailable. Shared aliases retain diagnostic allocations, not a claim
// of native QObject parenting or recovered destruction behavior.
class WidgetAreaLazyParent final {
public:
    static std::shared_ptr<WidgetAreaLazyParent> createQualified(
        const std::shared_ptr<const void>& actualParentOwner,
        std::optional<std::shared_ptr<const void>> field3c8,
        const std::string& qualification);
    // Both helpers use the SAME live field, not synchronized copies. Unknown
    // constructor/registration state remains unknown. Repeated binding aliases.
    static std::shared_ptr<WidgetAreaLazyParent> bindWindowOwner(
        const std::shared_ptr<WidgetAreaWindowOwner>&, const std::string& qualification);
    WidgetAreaLazyParent(const WidgetAreaLazyParent&) = delete;
    WidgetAreaLazyParent& operator=(const WidgetAreaLazyParent&) = delete;
    void observeInitialField(std::shared_ptr<const void>, const std::string& qualification);
    bool fieldKnown() const noexcept { return cell_->known_; }
    WidgetAreaFieldSnapshot fieldSnapshot() const noexcept { return cell_->snapshot(); }
    const void* canonicalFieldIdentity() const noexcept { return cell_.get(); }
    std::shared_ptr<const void> cachedArea() const noexcept;
    std::size_t commitCount() const noexcept { return cell_->commits_; }
    void releaseAdmissionScope() noexcept { cell_->admitted_ = false; }
    // Even a returned/cached area does not expose an invented impl or register
    // a receiver. The actual QWidget/impl/registration continuation is absent.
    void requireImplRegistration() const;
private:
    friend class WidgetAreaLazyGetter;
    WidgetAreaLazyParent(const std::shared_ptr<const void>&,
                        std::optional<std::shared_ptr<const void>>);
    explicit WidgetAreaLazyParent(std::shared_ptr<WidgetAreaCanonicalCell> cell) : cell_(std::move(cell)) {}
    std::shared_ptr<WidgetAreaCanonicalCell> cell_;
    std::weak_ptr<WidgetAreaLazyParent> canonical_;
};

enum class WidgetAreaConstructorObservation { Unavailable, Returned, Interrupted };
// Explicit source-store observation for a window-bound invocation. This is
// synthetic/retained evidence input, never a native readiness assertion.
struct WidgetAreaParentStoreObservation {
    std::weak_ptr<const void> parentOwner;
    std::weak_ptr<const void> invocationOwner;
    std::size_t expectedFieldRevision = 0, occurrence = 0;
};
struct WidgetAreaLazyObservation {
    std::weak_ptr<const void> parentOwner;
    // Unavailable is not allocator failure. Nonnull allocation/return aliases
    // must have identical pointer and shared owner control block.
    std::optional<bool> allocationSucceeded;
    std::weak_ptr<const void> allocationOwner;
    WidgetAreaConstructorObservation constructor = WidgetAreaConstructorObservation::Unavailable;
    std::weak_ptr<const void> constructorResultOwner;
    std::optional<WidgetAreaParentStoreObservation> parentStoreAt184cb;
    std::string qualification;
};

enum class WidgetAreaLazyStage {
    FieldRead, AllocationPending, ConstructorPending,
    ConstructorInterrupted, ParentCommitPending, Complete
};
struct WidgetAreaLazyReport {
    WidgetAreaLazyStage stage = WidgetAreaLazyStage::FieldRead;
    bool completed = false, cacheHit = false;
    bool allocationRequested = false, constructorRequested = false, constructorReturned = false;
    std::size_t allocationBytes = 0;
    bool constructorParentNull = true;
    unsigned constructorFlag = 1;
    std::shared_ptr<const void> allocationOwner, returnedArea;
    std::string issue;
};

// One invocation of native140018460. Each resume crosses at most one observed
// boundary: lookup -> allocation -> constructor return -> parent-field commit.
// No allocator, constructor, Qt/Win32 callback or Session operation runs here.
// An interrupted constructor leaves the allocation retained and field unchanged.
class WidgetAreaLazyGetter final {
public:
    WidgetAreaLazyGetter() = default;
    ~WidgetAreaLazyGetter();
    WidgetAreaLazyGetter(const WidgetAreaLazyGetter&) = delete;
    WidgetAreaLazyGetter& operator=(const WidgetAreaLazyGetter&) = delete;
    const WidgetAreaLazyReport& resume(const std::shared_ptr<WidgetAreaLazyParent>&,
                                      const WidgetAreaLazyObservation&);
    const WidgetAreaLazyReport& report() const noexcept { return report_; }
    // Identity for declared source observations only; possession never marks
    // allocation, constructor return, native liveness or the store complete.
    std::shared_ptr<const void> sourceInvocationOwner() const noexcept { return invocation_; }
private:
    std::weak_ptr<WidgetAreaLazyParent> parent_;
    std::weak_ptr<WidgetAreaCanonicalCell> cell_;
    std::shared_ptr<const void> invocation_ = std::make_shared<const char>(0);
    WidgetAreaLazyReport report_;
    std::size_t fieldRevision_ = 0;
    bool bound_ = false;
};

} // namespace m2m::engine
