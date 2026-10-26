// ctest-labels: m2m-mdt model-manager doe
#include <testkit/check.hpp>

#include <memory>
#include <string>

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/model_manager.hpp"

using namespace m2m;
namespace {
template<class F> bool refuses(F&& action)
{
    try { action(); }
    catch (const Error& error) {
        return error.code == M2M_E_UNSUPPORTED &&
            std::string(error.what()).find("nonnull shared owner without replacement") != std::string::npos;
    }
    return false;
}
}

TEST_CASE(doe_manager_binding_distinguishes_unavailable_from_observed_empty)
{
    auto io = doe::makeRecordingStudyIo();
    CHECK(io->modelManager() == nullptr);
    CHECK(refuses([&] { io->bindModelManager({}); }));
    CHECK(io->modelManager() == nullptr);
    auto manager = std::make_shared<engine::ModelManager>();
    io->bindModelManager(manager);
    CHECK(io->modelManager() == manager.get());
    CHECK_EQ(io->modelManager()->currentModelId(), -1);
    CHECK_EQ(manager->size(), 0u);
    CHECK(io->directories().empty());
    CHECK(io->reads().empty());
    CHECK(io->textWrites().empty());
}

TEST_CASE(doe_manager_binding_reads_actual_owner_current_and_id_live)
{
    auto io = doe::makeRecordingStudyIo();
    auto manager = std::make_shared<engine::ModelManager>();
    io->bindModelManager(manager);
    auto first = std::make_shared<engine::DbHandle>(engine::DbHandle::Kind::Mo);
    auto second = std::make_shared<engine::DbHandle>();
    manager->appendCreatedModel(first);
    CHECK_EQ(io->modelManager()->currentModelId(), -1); //registration has not selected
    manager->selectOrdinal(0);
    CHECK_EQ(io->modelManager()->currentModelId(), 1);
    manager->appendCreatedModel(second);
    manager->selectOrdinal(1);
    CHECK(io->modelManager()->currentModel() == second.get());
    CHECK_EQ(io->modelManager()->currentModelId(), 2);
    second->modelId = 32769; //explicit fixture state, later native short narrowing is separate
    CHECK_EQ(io->modelManager()->currentModelId(), 32769);
    manager->clear();
    CHECK_EQ(io->modelManager()->currentModelId(), -1);
}

TEST_CASE(doe_manager_binding_is_idempotent_rejects_replacement_and_holds_shared_owner)
{
    auto io = doe::makeRecordingStudyIo();
    auto manager = std::make_shared<engine::ModelManager>();
    const auto* identity = manager.get();
    std::weak_ptr<engine::ModelManager> lifetime = manager;
    io->bindModelManager(manager);
    io->bindModelManager(manager);
    auto unrelatedOwner = std::shared_ptr<engine::ModelManager>(manager.get(), [](auto*) {});
    CHECK(refuses([&] { io->bindModelManager(unrelatedOwner); }));
    CHECK(refuses([&] { io->bindModelManager(std::make_shared<engine::ModelManager>()); }));
    CHECK(refuses([&] { io->bindModelManager({}); }));
    CHECK(io->modelManager() == identity);
    manager.reset();
    CHECK(!lifetime.expired());
    CHECK(io->modelManager() == identity);
    io.reset();
    CHECK(lifetime.expired());
}

int main() { return testkit::run_all(); }
