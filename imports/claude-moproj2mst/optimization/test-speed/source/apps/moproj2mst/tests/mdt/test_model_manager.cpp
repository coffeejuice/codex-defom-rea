// ctest-labels: m2m-mdt model-manager
#include <testkit/check.hpp>

#include <memory>
#include <string>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/model_manager.hpp"

using namespace m2m;
namespace {
template<class F> bool refuses(F&& f, const char* detail)
{
    try { f(); }
    catch (const Error& e) {
        return e.code == M2M_E_UNSUPPORTED && std::string(e.what()).find(detail) != std::string::npos;
    }
    return false;
}
}

TEST_CASE(model_manager_fresh_constructor_has_no_current_or_children)
{
    engine::ModelManager manager;
    CHECK_EQ(manager.size(), 0u);
    CHECK(manager.currentModel() == nullptr);
    CHECK_EQ(manager.currentModelId(), -1);
    CHECK_EQ(manager.nextModelId(), 1);
    CHECK(manager.modelWithId(0) == nullptr);
    CHECK(manager.modelWithId(-1) == nullptr);
}

TEST_CASE(model_manager_append_orders_ids_without_selecting_or_changing_child_kind)
{
    engine::ModelManager manager;
    auto first = std::make_shared<engine::DbHandle>(engine::DbHandle::Kind::Mo);
    auto second = std::make_shared<engine::DbHandle>();
    first->path = qt::L("C:/PROBLEM/DOEUnit/a.DB");
    CHECK_EQ(first->modelId, 0);
    CHECK_EQ(manager.appendCreatedModel(first), 1);
    CHECK_EQ(manager.appendCreatedModel(second), 2);
    CHECK(manager.modelAt(0) == first.get());
    CHECK(manager.modelAt(1) == second.get());
    CHECK(manager.modelWithId(1) == first.get());
    CHECK(first->kind == engine::DbHandle::Kind::Mo);
    CHECK(second->kind == engine::DbHandle::Kind::Base);
    CHECK(first->path == qt::L("C:/PROBLEM/DOEUnit/a.DB"));
    CHECK(manager.currentModel() == nullptr);
}

TEST_CASE(model_manager_current_requires_membership_and_invalid_selection_retains_it)
{
    engine::ModelManager manager;
    auto member = std::make_shared<engine::DbHandle>();
    auto alien = std::make_shared<engine::DbHandle>();
    manager.appendCreatedModel(member);
    CHECK(manager.selectModel(member.get()));
    CHECK(manager.currentModel() == member.get());
    CHECK_EQ(manager.currentModelId(), 1);
    CHECK(!manager.selectModel(alien.get()));
    CHECK(!manager.selectModel(nullptr));
    CHECK(!manager.selectOrdinal(-1));
    CHECK(!manager.selectOrdinal(1));
    CHECK(manager.currentModel() == member.get());
    CHECK(manager.selectOrdinal(0));
}

TEST_CASE(model_manager_id_allocator_reuses_gaps_and_reads_current_native_id_live)
{
    engine::ModelManager manager;
    auto first = std::make_shared<engine::DbHandle>();
    auto second = std::make_shared<engine::DbHandle>();
    manager.appendCreatedModel(first);
    manager.appendCreatedModel(second);
    manager.selectModel(second.get());
    first->modelId = 3; //explicit declared fixture state, not a native setter observation
    CHECK_EQ(manager.nextModelId(), 1);
    second->modelId = -25;
    CHECK_EQ(manager.currentModelId(), -25); //current-ID query does not reject negative IDs
    CHECK(manager.modelWithId(-25) == nullptr); //lookup-by-ID does reject them
    auto third = std::make_shared<engine::DbHandle>();
    CHECK_EQ(manager.appendCreatedModel(third), 1);
    CHECK(manager.currentModel() == second.get());
}

TEST_CASE(model_manager_rejects_unrepresented_null_and_duplicate_factory_inputs)
{
    engine::ModelManager manager;
    CHECK(refuses([&] { manager.appendCreatedModel({}); }, "null construction"));
    auto child = std::make_shared<engine::DbHandle>();
    manager.appendCreatedModel(child);
    manager.selectModel(child.get());
    child->modelId = 8;
    CHECK(refuses([&] { manager.appendCreatedModel(child); }, "fresh native factory"));
    CHECK_EQ(manager.size(), 1u);
    CHECK_EQ(child->modelId, 8);
    CHECK(manager.currentModel() == child.get());
}

TEST_CASE(model_manager_clear_releases_owned_children_and_current_without_fabricating_external_deletion)
{
    engine::ModelManager manager;
    auto child = std::make_shared<engine::DbHandle>();
    std::weak_ptr<engine::DbHandle> lifetime = child;
    manager.appendCreatedModel(child);
    manager.selectModel(child.get());
    child.reset();
    CHECK(!lifetime.expired());
    manager.clear();
    CHECK(lifetime.expired());
    CHECK_EQ(manager.size(), 0u);
    CHECK_EQ(manager.currentModelId(), -1);
    auto leased = std::make_shared<engine::DbHandle>();
    manager.appendCreatedModel(leased);
    manager.selectModel(leased.get());
    manager.clear();
    CHECK(leased != nullptr); //adapter lease; explicitly not native QObject deletion parity
    CHECK(manager.currentModel() == nullptr);
    CHECK(!manager.selectModel(leased.get()));
}

TEST_CASE(model_manager_destructor_releases_sole_owned_children_in_encounter_order)
{
    std::vector<int> released;
    std::weak_ptr<engine::DbHandle> selected;
    {
        engine::ModelManager manager;
        for (int index = 0; index < 3; ++index) {
            auto child = std::shared_ptr<engine::DbHandle>(new engine::DbHandle(),
                [&](engine::DbHandle* value) {
                    released.push_back(value->modelId); //adapter observation, no native callback claim
                    delete value;
                });
            manager.appendCreatedModel(child);
            if (index == 1) { manager.selectModel(child.get()); selected = child; }
        }
        CHECK(released.empty());
        CHECK(!selected.expired());
    }
    CHECK(selected.expired());
    REQUIRE(released.size() == 3u);
    CHECK_EQ(released[0], 1);
    CHECK_EQ(released[1], 2);
    CHECK_EQ(released[2], 3);
}

int main() { return testkit::run_all(); }
