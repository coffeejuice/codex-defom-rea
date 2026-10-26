// ctest-labels: m2m-mdt main-manager-factory-binding
#include <testkit/check.hpp>

#include <memory>
#include <string>
#include <type_traits>

#include "m2m/common.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/main_manager_factory_binding.hpp"
#include "m2m/engine/model_manager.hpp"

using namespace m2m;
using namespace m2m::engine;
namespace {
template<class F> bool refuses(F&& body, const char* detail)
{
    try { body(); }
    catch (const Error& error) {
        return error.code == M2M_E_UNSUPPORTED &&
            std::string(error.what()).find(detail) != std::string::npos;
    }
    return false;
}
}
static_assert(!std::is_copy_constructible_v<MainManagerFactoryBinding>);
static_assert(!std::is_move_constructible_v<MainManagerFactoryBinding>);
static_assert(!MainManagerFactorySnapshot::nativeWrapperQualified);
static_assert(!MainManagerFactorySnapshot::completeForPublicClose);

TEST_CASE(main_manager_factory_identity_starts_unknown_without_native_construction_claim)
{
    std::shared_ptr<const void> main = std::make_shared<int>(1); // declared candidate lease
    MainManagerFactoryBinding binding(main, mainManagerFactoryProfile, "declared source projection");
    const auto state = binding.inspect();
    CHECK(state.state == MainManagerFactoryState::FieldUnavailable);
    CHECK(state.fieldKnowledge == MainManagerFieldKnowledge::Unknown);
    CHECK(state.fieldIdentity == nullptr && !state.manager);
    CHECK(!state.backlinkObserved && !state.concreteMoClassObserved);
    CHECK(binding.backlinkQualification().empty() && binding.classQualification().empty());
    CHECK(binding.fieldQualification().empty());
}

TEST_CASE(main_manager_factory_observes_same_actual_manager_in_source_order)
{
    std::shared_ptr<const void> main = std::make_shared<int>(1);
    auto manager = std::make_shared<ModelManager>();
    MainManagerFactoryBinding binding(main, mainManagerFactoryProfile, "declared factory occurrence");
    REQUIRE(binding.observeManagerBacklinkAtC7822c(manager, main, "declared backlink store"));
    auto before = binding.inspect();
    CHECK(before.backlinkObserved && !before.concreteMoClassObserved);
    CHECK(before.fieldKnowledge == MainManagerFieldKnowledge::Unknown && !before.manager);
    REQUIRE(binding.observeConcreteMoClassAt045449(manager, "declared Mo vtable store"));
    CHECK(binding.inspect().fieldKnowledge == MainManagerFieldKnowledge::Unknown);
    REQUIRE(binding.observeManagerStoreAt01344c(main, manager, "declared factory field store"));
    const auto state = binding.inspect();
    CHECK(state.state == MainManagerFactoryState::ConcreteMoIdentityObserved);
    CHECK(state.fieldKnowledge == MainManagerFieldKnowledge::KnownManager);
    CHECK(state.fieldIdentity == manager.get() && state.manager == manager);
    CHECK(state.backlinkObserved && state.concreteMoClassObserved);
    CHECK(manager->size() == 0 && manager->currentModel() == nullptr);
    CHECK(manager->connections().empty() && manager->ownerSession() == nullptr);
}

TEST_CASE(main_manager_factory_order_guards_leave_observed_fields_unchanged)
{
    std::shared_ptr<const void> main = std::make_shared<int>(1);
    auto manager = std::make_shared<ModelManager>();
    MainManagerFactoryBinding binding(main, mainManagerFactoryProfile, "declared occurrence");
    CHECK(refuses([&] { binding.observeConcreteMoClassAt045449(manager, "class"); }, "backlink"));
    CHECK(refuses([&] { binding.observeManagerStoreAt01344c(main, manager, "field"); }, "must precede"));
    CHECK(!binding.inspect().backlinkObserved && binding.classQualification().empty());
    binding.observeManagerBacklinkAtC7822c(manager, main, "backlink");
    CHECK(refuses([&] { binding.observeManagerStoreAt01344c(main, manager, "field"); }, "must precede"));
    CHECK(refuses([&] { binding.observeNullStoreAt01344c(main, "null"); }, "conflicts"));
    CHECK(binding.inspect().fieldKnowledge == MainManagerFieldKnowledge::Unknown);
    CHECK(binding.fieldQualification().empty());
}

TEST_CASE(main_manager_factory_repetition_preserves_first_receipts_and_rejects_other_owners)
{
    std::shared_ptr<const void> main = std::make_shared<int>(1), otherMain = std::make_shared<int>(2);
    auto manager = std::make_shared<ModelManager>(), other = std::make_shared<ModelManager>();
    MainManagerFactoryBinding binding(main, mainManagerFactoryProfile, "factory");
    binding.observeManagerBacklinkAtC7822c(manager, main, "first backlink");
    CHECK(!binding.observeManagerBacklinkAtC7822c(manager, main, "later backlink"));
    CHECK(refuses([&] { binding.observeManagerBacklinkAtC7822c(other, main, "other"); }, "manager identity"));
    CHECK(refuses([&] { binding.observeManagerBacklinkAtC7822c(manager, otherMain, "other"); }, "main allocation"));
    binding.observeConcreteMoClassAt045449(manager, "first class");
    CHECK(!binding.observeConcreteMoClassAt045449(manager, "later class"));
    CHECK(refuses([&] { binding.observeConcreteMoClassAt045449(other, "other"); }, "manager identity"));
    binding.observeManagerStoreAt01344c(main, manager, "first field");
    CHECK(!binding.observeManagerStoreAt01344c(main, manager, "later field"));
    CHECK(refuses([&] { binding.observeManagerStoreAt01344c(main, other, "other"); }, "manager identity"));
    CHECK(refuses([&] { binding.observeNullStoreAt01344c(main, "null"); }, "conflicts"));
    CHECK(binding.backlinkQualification() == "first backlink");
    CHECK(binding.classQualification() == "first class" && binding.fieldQualification() == "first field");
}

TEST_CASE(main_manager_factory_same_address_different_control_block_is_not_canonical_owner)
{
    std::shared_ptr<const void> main = std::make_shared<int>(1);
    auto manager = std::make_shared<ModelManager>();
    std::shared_ptr<const void> fakeMain(main.get(), [](const void*) {});
    std::shared_ptr<ModelManager> fakeManager(manager.get(), [](ModelManager*) {});
    MainManagerFactoryBinding binding(main, mainManagerFactoryProfile, "factory");
    CHECK(refuses([&] { binding.observeManagerBacklinkAtC7822c(manager, fakeMain, "alias"); }, "control block"));
    CHECK(!binding.inspect().backlinkObserved);
    binding.observeManagerBacklinkAtC7822c(manager, main, "actual");
    CHECK(refuses([&] { binding.observeConcreteMoClassAt045449(fakeManager, "alias"); }, "control block"));
    CHECK(!binding.inspect().concreteMoClassObserved);
    binding.observeConcreteMoClassAt045449(manager, "actual class");
    CHECK(refuses([&] { binding.observeManagerStoreAt01344c(fakeMain, manager, "alias"); }, "control block"));
    CHECK(binding.inspect().fieldKnowledge == MainManagerFieldKnowledge::Unknown);
}

TEST_CASE(main_manager_factory_allocation_null_is_explicit_without_a_manager_or_wrapper_success)
{
    std::shared_ptr<const void> main = std::make_shared<int>(1);
    MainManagerFactoryBinding binding(main, mainManagerFactoryProfile, "declared null allocation branch");
    REQUIRE(binding.observeNullStoreAt01344c(main, "first explicit null store"));
    CHECK(!binding.observeNullStoreAt01344c(main, "later null store"));
    const auto state = binding.inspect();
    CHECK(state.state == MainManagerFactoryState::NullFactoryReturn);
    CHECK(state.fieldKnowledge == MainManagerFieldKnowledge::KnownNull);
    CHECK(!state.manager && state.fieldIdentity == nullptr);
    CHECK(!state.backlinkObserved && !state.concreteMoClassObserved);
    auto manager = std::make_shared<ModelManager>();
    CHECK(refuses([&] { binding.observeManagerBacklinkAtC7822c(manager, main, "nonnull"); }, "conflicts"));
    CHECK(binding.fieldQualification() == "first explicit null store");
}

TEST_CASE(main_manager_factory_expired_manager_keeps_nonnull_field_identity_unavailable)
{
    std::shared_ptr<const void> main = std::make_shared<int>(1);
    auto manager = std::make_shared<ModelManager>();
    auto* identity = manager.get();
    MainManagerFactoryBinding binding(main, mainManagerFactoryProfile, "factory");
    binding.observeManagerBacklinkAtC7822c(manager, main, "backlink");
    binding.observeConcreteMoClassAt045449(manager, "class");
    binding.observeManagerStoreAt01344c(main, manager, "field");
    manager.reset();
    const auto state = binding.inspect();
    CHECK(state.state == MainManagerFactoryState::NonNullManagerUnavailable);
    CHECK(state.fieldKnowledge == MainManagerFieldKnowledge::KnownManager);
    CHECK(state.fieldIdentity == identity && !state.manager);
    CHECK(refuses([&] { binding.observeNullStoreAt01344c(main, "weak expired"); }, "conflicts"));
}

TEST_CASE(main_manager_factory_expired_main_scope_does_not_become_native_null)
{
    std::shared_ptr<const void> main = std::make_shared<int>(1);
    auto manager = std::make_shared<ModelManager>();
    MainManagerFactoryBinding binding(main, mainManagerFactoryProfile, "factory");
    binding.observeManagerBacklinkAtC7822c(manager, main, "backlink");
    binding.observeConcreteMoClassAt045449(manager, "class");
    binding.observeManagerStoreAt01344c(main, manager, "field");
    main.reset();
    const auto state = binding.inspect();
    CHECK(state.state == MainManagerFactoryState::MainOwnerUnavailable);
    CHECK(state.fieldKnowledge == MainManagerFieldKnowledge::KnownManager);
    CHECK(state.fieldIdentity == manager.get());
    CHECK(refuses([&] { binding.observeConcreteMoClassAt045449(manager, "expired"); }, "scope"));
}

TEST_CASE(main_manager_factory_preserves_actual_model_population_current_and_connections)
{
    std::shared_ptr<const void> main = std::make_shared<int>(1);
    auto manager = std::make_shared<ModelManager>();
    auto child = std::make_shared<DbHandle>();
    manager->appendCreatedModel(child);
    REQUIRE(manager->selectModel(child.get()));
    MainManagerFactoryBinding binding(main, mainManagerFactoryProfile, "declared identity only");
    binding.observeManagerBacklinkAtC7822c(manager, main, "backlink");
    binding.observeConcreteMoClassAt045449(manager, "class");
    binding.observeManagerStoreAt01344c(main, manager, "field");
    CHECK(binding.inspect().state == MainManagerFactoryState::ConcreteMoIdentityObserved);
    CHECK(manager->size() == 1 && manager->currentModel() == child.get());
    CHECK(manager->connections().empty() && manager->ownerSession() == nullptr);
}

TEST_CASE(main_manager_factory_requires_profile_nonnull_owners_and_qualified_source_observations)
{
    std::shared_ptr<const void> main = std::make_shared<int>(1);
    auto manager = std::make_shared<ModelManager>();
    CHECK(refuses([&] { MainManagerFactoryBinding b({}, mainManagerFactoryProfile, "factory"); }, "profile"));
    CHECK(refuses([&] { MainManagerFactoryBinding b(main, "another profile", "factory"); }, "profile"));
    CHECK(refuses([&] { MainManagerFactoryBinding b(main, mainManagerFactoryProfile, " \n"); }, "qualification"));
    MainManagerFactoryBinding binding(main, mainManagerFactoryProfile, "factory");
    CHECK(refuses([&] { binding.observeManagerBacklinkAtC7822c({}, main, "null manager"); }, "nonnull manager"));
    CHECK(refuses([&] { binding.observeManagerBacklinkAtC7822c(manager, main, "\t"); }, "qualification"));
    CHECK(refuses([&] { binding.observeNullStoreAt01344c(main, " "); }, "qualification"));
    CHECK(binding.inspect().fieldKnowledge == MainManagerFieldKnowledge::Unknown);
}

int main() { return testkit::run_all(); }
