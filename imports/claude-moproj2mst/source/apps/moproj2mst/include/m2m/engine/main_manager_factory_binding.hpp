// Diagnostic source-write observations, not native factory/Close execution.
#pragma once

#include <memory>
#include <string>

namespace m2m::engine {
class ModelManager;
inline constexpr const char* mainManagerFactoryProfile =
    "0x1420fb108:+0x2e0=0x1400133f0->0x140045420";
enum class MainManagerFieldKnowledge { Unknown, KnownNull, KnownManager };
enum class MainManagerFactoryState {
    MainOwnerUnavailable, FieldUnavailable, NullFactoryReturn,
    NonNullManagerUnavailable, ConcreteMoIdentityObserved
};
struct MainManagerFactorySnapshot {
    MainManagerFactoryState state;
    MainManagerFieldKnowledge fieldKnowledge;
    const ModelManager* fieldIdentity; // comparison only, never dereference after expiry
    std::shared_ptr<ModelManager> manager; // diagnostic C++ lease, not native lifetime
    bool backlinkObserved;
    bool concreteMoClassObserved;
    static constexpr bool nativeWrapperQualified = false;
    static constexpr bool completeForPublicClose = false;
};

// One supplied factory-occurrence projection. Allocation leases name candidate
// identities and track diagnostic availability only, never native allocation,
// liveness, QObject construction, parenting or current original observation.
// Separate records/labels do not intern/authenticate a shared native object.
class MainManagerFactoryBinding final {
public:
    MainManagerFactoryBinding(const std::shared_ptr<const void>& mainAllocation,
                             std::string sourceProfile, std::string qualification);
    MainManagerFactoryBinding(const MainManagerFactoryBinding&) = delete;
    MainManagerFactoryBinding& operator=(const MainManagerFactoryBinding&) = delete;
    MainManagerFactoryBinding(MainManagerFactoryBinding&&) = delete;
    MainManagerFactoryBinding& operator=(MainManagerFactoryBinding&&) = delete;
    // Observations of the same source identity chain, not executable cursors.
    bool observeManagerBacklinkAtC7822c(const std::shared_ptr<ModelManager>&,
        const std::shared_ptr<const void>& mainAllocation, std::string qualification);
    bool observeConcreteMoClassAt045449(const std::shared_ptr<ModelManager>&,
                                      std::string qualification);
    bool observeManagerStoreAt01344c(const std::shared_ptr<const void>& mainAllocation,
        const std::shared_ptr<ModelManager>&, std::string qualification);
    bool observeNullStoreAt01344c(const std::shared_ptr<const void>& mainAllocation,
                                std::string qualification);
    MainManagerFactorySnapshot inspect() const;
    const std::string& qualification() const noexcept { return qualification_; }
    const std::string& backlinkQualification() const noexcept { return backlinkQualification_; }
    const std::string& classQualification() const noexcept { return classQualification_; }
    const std::string& fieldQualification() const noexcept { return fieldQualification_; }
private:
    void requireMain(const std::shared_ptr<const void>&) const;
    void requireAvailableMain() const;
    void requireManager(const std::shared_ptr<ModelManager>&) const;
    std::weak_ptr<const void> main_;
    const void* mainIdentity_;
    std::weak_ptr<ModelManager> manager_;
    const ModelManager* managerIdentity_ = nullptr;
    MainManagerFieldKnowledge knowledge_ = MainManagerFieldKnowledge::Unknown;
    bool backlink_ = false, moClass_ = false;
    std::string qualification_, backlinkQualification_, classQualification_, fieldQualification_;
};
} // namespace m2m::engine
