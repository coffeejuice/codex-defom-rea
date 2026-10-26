#pragma once
#include "m2m/model.hpp"
#include <optional>

namespace m2m {
namespace detail { struct FeatureContextAccess; }
enum class Project::FeatureContextStage : int { PassiveShell, ProxyAllocationPending };
enum class Project::FeatureContextStop : int {
        PreparedMoUnavailable, ConstructorPhaseNotEntered, HeapProfileUnavailable,
        OwnerBindingChanged, ProjectStorageUnavailable
    };
// Passive identity/provenance for native Project+60, not a native allocation,
// zeroed feature struct, provider map or constructor-completion witness.
class Project::FeatureContextAnchor final {
    public:
        ~FeatureContextAnchor() = default;
        FeatureContextAnchor(const FeatureContextAnchor&) = delete;
        FeatureContextAnchor& operator=(const FeatureContextAnchor&) = delete;
        const Project* projectIdentity() const noexcept { return project_; }
        const engine::DbHandle* preparedModelIdentity() const noexcept { return modelIdentity_; }
        FeatureContextStage stage() const noexcept { return stage_; }
        bool storageEpochAlive() const noexcept { return !storage_.expired(); }
        bool sourceOwnerAlive() const noexcept { return projectScopeActive_ && storageEpochAlive(); }
        bool sharesStorageEpoch(const FeatureContextAnchor&) const noexcept;
        std::optional<int> adapterF10AtAttempt() const noexcept { return adapterF10AtAttempt_; }
        bool projectBacklinkWasNullAtAttempt() const noexcept { return backlinkWasNull_; }
        static constexpr bool nativeConstructorCompleted = false;
        static constexpr bool nativeHeapEffectsCommitted = false;
        static constexpr bool nativeFeatureValuesAvailable = false;
        static constexpr bool nativeRegistrationCompleted = false;
        static constexpr bool completeForPublicClose = false;
    private:
        friend struct Project;
        friend struct detail::FeatureContextAccess;
        FeatureContextAnchor(const Project&, const std::shared_ptr<const engine::ProjectStorageLifetime>&,
                             const std::shared_ptr<engine::MoModelSlots>&);
        const Project* project_;
        std::weak_ptr<const engine::ProjectStorageLifetime> storage_;
        std::weak_ptr<engine::MoModelSlots> slots_;
        std::weak_ptr<engine::DbHandle> model_;
        const engine::DbHandle* modelIdentity_ = nullptr; // identity only, never dereference after expiry
        FeatureContextStage stage_ = FeatureContextStage::PassiveShell;
        std::optional<int> adapterF10AtAttempt_; // timing observation of adapter field, not native feature data
        bool backlinkWasNull_ = false;
        bool projectScopeActive_ = true;
        bool bindingChanged_ = false;
    };
struct Project::FeatureContextAttemptReport {
        FeatureContextStage stage;
        FeatureContextStop stop;
        const char* sourceOccurrence; // null before the logical089917 phase
        const char* nextSourceSite;   // first unknown native heap effect, not executed
    };
} // namespace m2m
