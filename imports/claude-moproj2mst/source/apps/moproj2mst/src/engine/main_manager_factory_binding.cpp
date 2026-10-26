#include "m2m/engine/main_manager_factory_binding.hpp"

#include <utility>

#include "m2m/common.hpp"
#include "m2m/engine/model_manager.hpp"

namespace m2m::engine {
namespace {
[[noreturn]] void unavailable(const char* detail)
{
    throw Error(M2M_E_UNSUPPORTED, std::string("Main manager factory binding: ") + detail);
}
void requireQualification(const std::string& text)
{
    if (text.find_first_not_of(" \t\r\n") == std::string::npos)
        unavailable("source qualification is unavailable");
}
template<class T> bool sameOwner(const std::shared_ptr<T>& left, const std::shared_ptr<T>& right)
{
    return left && right && left.get() == right.get() &&
        !left.owner_before(right) && !right.owner_before(left);
}
}

MainManagerFactoryBinding::MainManagerFactoryBinding(const std::shared_ptr<const void>& main,
    std::string profile, std::string qualification)
    : main_(main), mainIdentity_(main.get()), qualification_(std::move(qualification))
{
    requireQualification(qualification_);
    if (!main || profile != mainManagerFactoryProfile)
        unavailable("main identity/fixed Mo factory profile is unavailable");
}
void MainManagerFactoryBinding::requireMain(const std::shared_ptr<const void>& main) const
{
    if (!sameOwner(main, main_.lock()) || main.get() != mainIdentity_)
        unavailable("main allocation identity/control block is unavailable or different");
}
void MainManagerFactoryBinding::requireAvailableMain() const
{
    if (!main_.lock()) unavailable("main allocation observation scope is unavailable");
}
void MainManagerFactoryBinding::requireManager(const std::shared_ptr<ModelManager>& manager) const
{
    if (!sameOwner(manager, manager_.lock()) || manager.get() != managerIdentity_)
        unavailable("manager identity/control block is unavailable or different");
}

bool MainManagerFactoryBinding::observeManagerBacklinkAtC7822c(
    const std::shared_ptr<ModelManager>& manager, const std::shared_ptr<const void>& main,
    std::string qualification)
{
    requireQualification(qualification);
    requireMain(main);
    if (!manager || knowledge_ == MainManagerFieldKnowledge::KnownNull)
        unavailable("nonnull manager backlink conflicts with the factory outcome");
    if (backlink_) { requireManager(manager); return false; }
    manager_ = manager;
    managerIdentity_ = manager.get();
    backlink_ = true;
    backlinkQualification_ = std::move(qualification);
    return true;
}
bool MainManagerFactoryBinding::observeConcreteMoClassAt045449(
    const std::shared_ptr<ModelManager>& manager, std::string qualification)
{
    requireQualification(qualification);
    requireAvailableMain();
    if (!backlink_) unavailable("same-owner backlink observation must precede concrete class");
    requireManager(manager);
    if (moClass_) return false;
    moClass_ = true;
    classQualification_ = std::move(qualification);
    return true;
}
bool MainManagerFactoryBinding::observeManagerStoreAt01344c(
    const std::shared_ptr<const void>& main, const std::shared_ptr<ModelManager>& manager,
    std::string qualification)
{
    requireQualification(qualification);
    requireMain(main);
    if (!backlink_ || !moClass_)
        unavailable("backlink/concrete Mo class observations must precede main+50 store");
    requireManager(manager);
    if (knowledge_ == MainManagerFieldKnowledge::KnownManager) return false;
    if (knowledge_ != MainManagerFieldKnowledge::Unknown)
        unavailable("main+50 factory outcome cannot be rebound");
    knowledge_ = MainManagerFieldKnowledge::KnownManager;
    fieldQualification_ = std::move(qualification);
    return true;
}
bool MainManagerFactoryBinding::observeNullStoreAt01344c(
    const std::shared_ptr<const void>& main, std::string qualification)
{
    requireQualification(qualification);
    requireMain(main);
    if (backlink_ || moClass_ || knowledge_ == MainManagerFieldKnowledge::KnownManager)
        unavailable("null factory outcome conflicts with observed manager construction fields");
    if (knowledge_ == MainManagerFieldKnowledge::KnownNull) return false;
    knowledge_ = MainManagerFieldKnowledge::KnownNull;
    fieldQualification_ = std::move(qualification);
    return true;
}
MainManagerFactorySnapshot MainManagerFactoryBinding::inspect() const
{
    const auto manager = manager_.lock();
    const auto state = !main_.lock() ? MainManagerFactoryState::MainOwnerUnavailable
        : knowledge_ == MainManagerFieldKnowledge::Unknown ? MainManagerFactoryState::FieldUnavailable
        : knowledge_ == MainManagerFieldKnowledge::KnownNull ? MainManagerFactoryState::NullFactoryReturn
        : !manager ? MainManagerFactoryState::NonNullManagerUnavailable
        : MainManagerFactoryState::ConcreteMoIdentityObserved;
    return {state, knowledge_, knowledge_ == MainManagerFieldKnowledge::KnownManager ? managerIdentity_ : nullptr,
            knowledge_ == MainManagerFieldKnowledge::KnownManager ? manager : nullptr, backlink_, moClass_};
}
} // namespace m2m::engine
