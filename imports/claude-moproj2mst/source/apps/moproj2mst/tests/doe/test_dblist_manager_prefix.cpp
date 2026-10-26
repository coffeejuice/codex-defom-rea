// ctest-labels: m2m-doe
#include <testkit/check.hpp>

#include <optional>
#include <string>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_session_records.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "../../src/doe/dblist_manager_prefix.hpp"

using namespace m2m;
using namespace m2m::doe;
namespace {
bool unsupported(const auto& work, const char* text = "")
{
    try { work(); }
    catch (const Error& e) {
        return e.code == M2M_E_UNSUPPORTED && std::string(e.what()).find(text) != std::string::npos;
    }
    return false;
}
struct Rig {
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session{driver};
    void bindIo()
    {
        auto& project = session.project();
        project.doeIo = makeRecordingStudyIo();
        project.doeIo->bindModelManager(session.modelManagerOwner());
    }
    // Explicit prepared-child fixture; not native virtual+68/path completion.
    std::shared_ptr<engine::DbHandle> child(const qt::String& path)
    {
        auto result = std::make_shared<engine::DbHandle>(engine::DbHandle::Kind::Mo);
        result->pathSet = true;
        result->path = path;
        const auto manager = session.modelManagerOwner();
        manager->appendCreatedModel(result);
        manager->registerModelSignals(result);
        return result;
    }
};
}

TEST_CASE(dblist_manager_stored_path_true_getter_preserves_raw_value_and_null_state)
{
    engine::DbHandle child;
    child.path = qt::L("C:/PROBLEM/a/../mixed.Db");
    CHECK(isolatedDbListStoredPathValue(child).isNull());
    child.pathSet = true;
    CHECK(isolatedDbListStoredPathValue(child) == child.path);
    CHECK(child.path == qt::L("C:/PROBLEM/a/../mixed.Db"));
    child.path = qt::L("");
    CHECK(!isolatedDbListStoredPathValue(child).isNull());
    CHECK(isolatedDbListStoredPathValue(child).isEmpty());
    child.path = qt::String();
    CHECK(isolatedDbListStoredPathValue(child).isNull());
}

TEST_CASE(dblist_manager_lookup_uses_actual_owner_and_retains_last_exact_matching_child)
{
    Rig rig;
    rig.bindIo();
    const auto first = rig.child(qt::L("C:/PROBLEM/A.DB"));
    const auto other = rig.child(qt::L("C:/PROBLEM/B.DB"));
    const auto last = rig.child(qt::L("C:/PROBLEM/A.DB"));
    const auto manager = rig.session.modelManagerOwner();
    REQUIRE(manager->selectModel(other.get()));
    auto prefix = bindDbListManagerPrefix(rig.session);
    CHECK(prefix.lookupOrStopBeforeConstruction(rig.session, first->path) == last.get());
    REQUIRE(prefix.attempts().size() == 1);
    CHECK(prefix.attempts()[0].visited == std::vector<const engine::DbHandle*>({first.get(), other.get(), last.get()}));
    CHECK(prefix.attempts()[0].lastMatch == last.get());
    CHECK(!prefix.attempts()[0].stoppedBeforeNativeConstruction);
    CHECK(manager->currentModel() == other.get());
    CHECK(manager->size() == 3 && manager->connections().size() == 6);
    CHECK(first->modelId == 1 && other->modelId == 2 && last->modelId == 3);
    CHECK(!rig.session.project().dataDb); // No Project model publication.
}

TEST_CASE(dblist_manager_lookup_has_no_case_or_path_normalization_and_respects_path_set)
{
    Rig rig;
    rig.bindIo();
    const auto child = rig.child(qt::L("C:/PROBLEM/a/../A.DB"));
    auto prefix = bindDbListManagerPrefix(rig.session);
    CHECK(prefix.lookupOrStopBeforeConstruction(rig.session, child->path) == child.get());
    CHECK(unsupported([&] {
        prefix.lookupOrStopBeforeConstruction(rig.session, qt::L("C:/PROBLEM/A.DB"));
    }, "virtual+68"));
    CHECK(unsupported([&] {
        prefix.lookupOrStopBeforeConstruction(rig.session, qt::L("c:/problem/a/../a.db"));
    }, "virtual+68"));
    child->pathSet = false;
    CHECK(unsupported([&] { prefix.lookupOrStopBeforeConstruction(rig.session, child->path); }, "virtual+68"));
    CHECK(child->path == qt::L("C:/PROBLEM/a/../A.DB") && !child->pathSet);
    CHECK(rig.session.modelManagerOwner()->size() == 1);
}

TEST_CASE(dblist_manager_miss_stops_before_child_creation_and_retains_ordered_prior_attempts)
{
    Rig rig;
    rig.bindIo();
    const auto child = rig.child(qt::L("C:/PROBLEM/existing.DB"));
    const auto manager = rig.session.modelManagerOwner();
    manager->selectModel(child.get());
    auto prefix = bindDbListManagerPrefix(rig.session);
    int completed = 0;
    CHECK(unsupported([&] {
        for (const auto& path : std::vector<qt::String>{child->path,
                qt::L("C:/PROBLEM/absent_child.DB"), qt::L("C:/PROBLEM/later.DB")}) {
            (void)prefix.lookupOrStopBeforeConstruction(rig.session, path);
            ++completed;
        }
    }, "native manager virtual+68"));
    CHECK(completed == 1 && prefix.attempts().size() == 2);
    CHECK(prefix.attempts()[0].lastMatch == child.get());
    CHECK(!prefix.attempts()[1].lastMatch && prefix.attempts()[1].stoppedBeforeNativeConstruction);
    CHECK(manager->size() == 1 && manager->connections().size() == 2);
    CHECK(manager->currentModel() == child.get() && child->modelId == 1);
    CHECK(child->pathSet && child->path == qt::L("C:/PROBLEM/existing.DB"));
    CHECK(!child->loaded && !rig.session.project().dataDb);
}

TEST_CASE(dblist_manager_binding_refuses_missing_foreign_replaced_and_released_owners)
{
    Rig first, second;
    CHECK(unsupported([&] { (void)bindDbListManagerPrefix(first.session); }));
    first.session.project().doeIo = makeRecordingStudyIo();
    CHECK(unsupported([&] { (void)bindDbListManagerPrefix(first.session); }));
    first.session.project().doeIo->bindModelManager(second.session.modelManagerOwner());
    CHECK(unsupported([&] { (void)bindDbListManagerPrefix(first.session); }));
    first.bindIo();
    auto prefix = bindDbListManagerPrefix(first.session);
    CHECK(unsupported([&] { prefix.validateAgainst(second.session); }));
    const auto originalIo = first.session.project().doeIo;
    first.session.project().doeIo = makeRecordingStudyIo();
    first.session.project().doeIo->bindModelManager(first.session.modelManagerOwner());
    CHECK(unsupported([&] { prefix.validateAgainst(first.session); }));
    first.session.project().doeIo = std::shared_ptr<doe::StudyIo>(originalIo.get(), [](doe::StudyIo*) {});
    CHECK(unsupported([&] { prefix.validateAgainst(first.session); }));
    first.session.project().doeIo = originalIo;
    prefix.validateAgainst(first.session);
    first.session.releaseProjectAfterCloseEffects();
    CHECK(unsupported([&] { prefix.lookupOrStopBeforeConstruction(first.session, qt::L("C:/PROBLEM/A.DB")); }));
    CHECK(prefix.attempts().empty());
}

TEST_CASE(dblist_session_import_owner_binds_only_the_project_actual_session_chain)
{
    Rig rig;
    auto& project = rig.session.project();
    project.doeIo = makeRecordingStudyIo();
    SessionImportContext context;
    context.io = project.doeIo.get();

    bindSessionImportOwner(context, project);

    CHECK(context.actualSession == nullptr); // Recording I/O alone is not an owner.
    project.doeIo->bindModelManager(rig.session.modelManagerOwner());
    bindSessionImportOwner(context, project);
    CHECK(context.actualSession == &rig.session);
    CHECK(context.actualSession->liveProject() == &project);
}

TEST_CASE(dblist_session_import_owner_refuses_mismatched_io_or_foreign_manager)
{
    Rig first, second;
    second.bindIo();
    auto& project = first.session.project();
    project.doeIo = makeRecordingStudyIo();
    project.doeIo->bindModelManager(second.session.modelManagerOwner());
    SessionImportContext context;
    context.io = project.doeIo.get();

    CHECK(unsupported([&] { bindSessionImportOwner(context, project); },
                      "not owned by this live ConversionSession/Project"));
    CHECK(context.actualSession == nullptr);

    context.io = second.session.project().doeIo.get();
    CHECK(unsupported([&] { bindSessionImportOwner(context, project); },
                      "I/O is not the Project's bound StudyIo"));
    CHECK(context.actualSession == nullptr);
}

TEST_CASE(dblist_manager_prefix_weak_leases_do_not_keep_manager_or_io_alive)
{
    std::optional<DbListManagerPrefix> escaped;
    std::weak_ptr<engine::ModelManager> manager;
    std::weak_ptr<doe::StudyIo> io;
    {
        Rig rig;
        rig.bindIo();
        escaped = bindDbListManagerPrefix(rig.session);
        manager = rig.session.modelManagerOwner();
        io = rig.session.project().doeIo;
        CHECK(!manager.expired() && !io.expired());
    }
    CHECK(manager.expired() && io.expired());
    Rig other;
    CHECK(unsupported([&] { escaped->validateAgainst(other.session); }));
}

int main() { return testkit::run_all(); }
