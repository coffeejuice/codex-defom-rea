// ctest-labels: m2m-mdt model-manager-constructor-prefix
#include <testkit/check.hpp>

#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>

#include "m2m/common.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"
#include "m2m/engine/session_model_manager_read.hpp"
#include "m2m/model.hpp"

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
static_assert(!std::is_default_constructible_v<ModelManagerConstructorPrefixReceipt>);
static_assert(!std::is_copy_assignable_v<ModelManagerConstructorPrefixReceipt>);
static_assert(std::is_same_v<decltype(std::declval<const ModelManager&>().constructorPrefixReceipt()),
    ModelManagerConstructorPrefixReceipt>);
static_assert(std::is_same_v<decltype(std::declval<const ModelManagerConstructorPrefixReceipt&>()
    .ownerIdentityToken()), std::uintptr_t>);
static_assert(!ModelManagerConstructorPrefixReceipt::nativeQObjectConstructed);
static_assert(!ModelManagerConstructorPrefixReceipt::nativeConstructorCompleted);
static_assert(!ModelManagerConstructorPrefixReceipt::nativeConcreteMoClassObserved);
static_assert(!ModelManagerConstructorPrefixReceipt::nativeMain50HandoffObserved);
static_assert(!ModelManagerConstructorPrefixReceipt::nativeWrapperQualified);
static_assert(!ModelManagerConstructorPrefixReceipt::mainWindowReady);
static_assert(!ModelManagerConstructorPrefixReceipt::completeForPublicClose);

TEST_CASE(model_manager_constructor_prefix_records_standalone_null_owner_and_no_write_skip)
{
    ModelManager manager;
    const auto prefix = manager.constructorPrefixReceipt();
    CHECK(prefix.origin() == ModelManagerConstructorPrefixOrigin::CandidateFreshConstructor);
    CHECK(prefix.ownerAssignmentProjected() && prefix.freshNullEqualitySkipProjected());
    CHECK(prefix.ownerIdentityToken() == 0);
    CHECK(manager.ownerSession() == nullptr);
    CHECK(manager.size() == 0 && manager.currentModel() == nullptr);
    CHECK(manager.currentModelId() == -1 && manager.connections().empty());
    CHECK(std::string(prefix.currentSourceSite).find("0x140cbc93b") != std::string::npos);
}

TEST_CASE(model_manager_constructor_prefix_belongs_to_actual_session_manager_before_open)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    const auto manager = session.modelManagerOwner();
    REQUIRE(manager && manager->ownerSession() == &session);
    const auto prefix = manager->constructorPrefixReceipt();
    CHECK(prefix.ownerIdentityToken() == reinterpret_cast<std::uintptr_t>(&session));
    CHECK(prefix.ownerAssignmentProjected() && prefix.freshNullEqualitySkipProjected());
    CHECK(manager->size() == 0 && manager->currentModel() == nullptr);
    CHECK(manager->connections().empty());
    CHECK(!session.modelManagerOpenReadReceipt());
    CHECK(refuses([&] { session.currentViewManagerOwner(); }, "View_Default/Primary"));
}

TEST_CASE(model_manager_constructor_prefix_and_open_read_resolve_same_existing_owner)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    const auto manager = session.modelManagerOwner();
    const auto before = manager->constructorPrefixReceipt();
    session.prepareProjectModel(qt::L("C:/PROBLEM/C80/prefix.DB"));
    const auto read = session.modelManagerOpenReadReceipt();
    REQUIRE(read);
    CHECK(read->requireManagerFor(session) == manager);
    CHECK(session.modelManagerOwner() == manager);
    CHECK(read->modelCountAtRead() == 0 && read->connectionCountAtRead() == 0);
    REQUIRE(manager->size() == 1 && manager->connections().size() == 2);
    const auto model = session.project().dataDb;
    REQUIRE(model);
    CHECK(manager->modelAt(0) == model.get() && model->modelId == 1);
    CHECK(model->moProject == nullptr && manager->currentModel() == nullptr);
    const auto after = manager->constructorPrefixReceipt();
    CHECK(after.ownerIdentityToken() == before.ownerIdentityToken());
    CHECK(after.ownerAssignmentProjected() && after.freshNullEqualitySkipProjected());
    CHECK(!after.nativeMain50HandoffObserved && !read->nativeFactoryCompleted);
    CHECK(refuses([&] { session.currentViewManagerOwner(); }, "View_Default/Primary"));
}

TEST_CASE(model_manager_constructor_prefix_is_historical_after_project_selection)
{
    DiagramEffectDriver effects;
    ConversionSession session(effects);
    session.prepareProjectModel(qt::L("C:/PROBLEM/C80/selected.DB"));
    session.commitProjectModelConstruction();
    session.commitProjectModelSelection();
    auto manager = session.modelManagerOwner();
    auto model = session.project().dataDb;
    REQUIRE(manager->currentModel() == model.get());
    const auto prefix = manager->constructorPrefixReceipt();
    CHECK(prefix.freshNullEqualitySkipProjected()); // constructor history, not current-null proof
    CHECK(manager->currentModel() == model.get() && manager->currentModelId() == 1);
    CHECK(model->moProject == session.liveProject());
    CHECK(session.modelManagerOpenReadReceipt()->requireManagerFor(session) == manager);
    CHECK(!prefix.nativeWrapperQualified && !prefix.completeForPublicClose);
}

TEST_CASE(model_manager_constructor_prefix_is_not_replayed_by_selection_or_adapter_clear)
{
    ModelManager manager;
    const auto before = manager.constructorPrefixReceipt();
    auto first = std::make_shared<DbHandle>();
    auto second = std::make_shared<DbHandle>();
    manager.appendCreatedModel(first);
    manager.appendCreatedModel(second);
    REQUIRE(manager.selectModel(second.get()));
    CHECK(manager.currentModelId() == 2);
    CHECK(manager.constructorPrefixReceipt().freshNullEqualitySkipProjected());
    CHECK(manager.currentModel() == second.get());
    manager.clear(); // existing adapter clear, not native constructor or barrier completion
    CHECK(manager.size() == 0 && manager.currentModel() == nullptr);
    const auto after = manager.constructorPrefixReceipt();
    CHECK(after.ownerIdentityToken() == before.ownerIdentityToken());
    CHECK(after.ownerAssignmentProjected() == before.ownerAssignmentProjected());
    CHECK(after.freshNullEqualitySkipProjected() == before.freshNullEqualitySkipProjected());
    CHECK(!after.nativeConstructorCompleted && !after.nativeMain50HandoffObserved);
}

TEST_CASE(model_manager_constructor_prefix_copy_outlives_owner_as_opaque_non_authorizing_history)
{
    DiagramEffectDriver effects;
    std::optional<ModelManagerConstructorPrefixReceipt> history;
    std::weak_ptr<ModelManager> manager;
    std::uintptr_t ownerToken = 0;
    {
        ConversionSession session(effects);
        manager = session.modelManagerOwner();
        history.emplace(session.modelManagerOwner()->constructorPrefixReceipt());
        ownerToken = reinterpret_cast<std::uintptr_t>(&session);
        REQUIRE(history->ownerIdentityToken() == ownerToken);
    }
    CHECK(manager.expired());
    REQUIRE(history);
    CHECK(history->ownerIdentityToken() == ownerToken);
    CHECK(history->ownerAssignmentProjected() && history->freshNullEqualitySkipProjected());
    CHECK(!history->nativeQObjectConstructed && !history->nativeWrapperQualified);
    CHECK(!history->mainWindowReady && !history->completeForPublicClose);
}

int main() { return testkit::run_all(); }
