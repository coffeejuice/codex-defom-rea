// ctest-labels: m2m-mdt mdt-viewer
#include <testkit/check.hpp>
#include <memory>
#include <string>
#include "m2m/common.hpp"
#include "m2m/doe_slice.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine_scene.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"
#include "m2m/mst/writer.hpp"

using namespace m2m;
namespace {
bool refuses(const auto& action, const char* fragment)
{
    try { action(); }
    catch (const Error& e) {
        return e.code == M2M_E_UNSUPPORTED && std::string(e.what()).find(fragment) != std::string::npos;
    }
    return false;
}
void emptyView(engine::GuiModel& model)
{
    float minimum[3]{}, maximum[3]{};
    CHECK(engine::viewerBox(model, 1, minimum, maximum));
}
doe::SliceCoefficients unitPlane()
{
    doe::SliceCoefficients coefficients;
    coefficients.values = {1.0, 0.0, 0.0, -2.0};
    return coefficients;
}
}

TEST_CASE(slice_collection_allocation_precedes_missing_coefficient_guard_and_no_db_return)
{
    doe::SliceConstructorRuntime runtime;
    engine::GuiModel model(nullptr, 2);
    CHECK(!engine::viewerSliceState(model, 0));
    CHECK(refuses([&] { engine::readViewerSlice(model, 0, unitPlane(), &runtime); }, "existing viewer item"));
    CHECK(engine::viewerItemCount(model) == 0);
    emptyView(model);
    CHECK(refuses([&] { engine::readViewerSlice(model, 0, {}, &runtime); }, "Slice"));
    const auto* collection = engine::viewerSliceState(model, 0);
    REQUIRE(collection != nullptr);
    REQUIRE(runtime.colorScratchBits.has_value());
    CHECK((*runtime.colorScratchBits == std::array<std::uint32_t, 4>{0x3f800000, 0x3f800000, 0x3f800000, 0x3f800000}));
    CHECK(!collection->model.has_value() && collection->viewIdentity && !collection->boundObject);
    CHECK(collection->backendKnownNull && collection->graphicCount == 0);
    engine::readViewerSlice(model, 0, unitPlane(), &runtime); // no DB, including no bound object
    CHECK(engine::viewerSliceState(model, 0) == collection);
    CHECK(!collection->model.has_value() && collection->viewIdentity);
    CHECK(!engine::viewerDisplayOptionSnapshot(model, 0));
}

TEST_CASE(slice_no_db_all_zero_normal_does_not_consume_missing_d)
{
    doe::SliceConstructorRuntime runtime;
    engine::GuiModel model(nullptr, 2);
    emptyView(model);
    doe::SliceCoefficients zero;
    zero.values[0] = 0.0; zero.values[1] = -0.0; zero.values[2] = 0.0;
    engine::readViewerSlice(model, 0, zero, &runtime);
    REQUIRE(engine::viewerSliceState(model, 0) != nullptr);
    CHECK(!engine::viewerSliceState(model, 0)->model);
    const auto scratch = runtime.colorScratchBits;
    model.reset();
    CHECK(!engine::viewerSliceState(model, 0) && engine::viewerItemCount(model) == 0);
    CHECK(!engine::viewerPendingSliceCleanup(model));
    CHECK(runtime.colorScratchBits == scratch); //empty destruction does not reset process scratch
    model.reset();
}

TEST_CASE(slice_actual_session_database_keeps_plane_and_bindings_before_backend_guard)
{
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session(driver);
    session.prepareProjectModel(qt::L("slice-prefix.DB"));
    auto db = session.project().dataDb;
    REQUIRE(db && db->slicePlanes);
    const auto heap = session.slicePlaneHeapOwner();
    CHECK(db->slicePlanes->heapOwner() == heap);
    auto& runtime = *session.sliceConstructorRuntimeOwner();
    engine::GuiModel model(db.get(), 2);
    emptyView(model);
    CHECK(refuses([&] { engine::readViewerSlice(model, 0, unitPlane(), &runtime); }, "backend"));
    REQUIRE(db->slicePlanes->records.size() == 1);
    CHECK(db->slicePlanes->counter == 1 && heap->retainedCount() == 1);
    CHECK(db->slicePlanes->records[0] == heap->retainedAt(0));
    CHECK(db->slicePlanes->records[0]->point[0] == 2.0);
    const auto* collection = engine::viewerSliceState(model, 0);
    REQUIRE(collection != nullptr);
    CHECK(collection->model == &model && collection->viewIdentity != nullptr);
    CHECK(!collection->boundObject && collection->graphicCount == 0 && collection->backendKnownNull);
    CHECK(refuses([&] { engine::readViewerSlice(model, 0, unitPlane(), &runtime); }, "backend"));
    CHECK(db->slicePlanes->records.size() == 1 && heap->retainedCount() == 1);
    CHECK(engine::viewerSliceState(model, 0) == collection);
}

TEST_CASE(slice_unbound_allocator_refuses_after_collection_before_database_mutation)
{
    engine::DbHandle db;
    doe::SliceConstructorRuntime runtime;
    engine::GuiModel model(&db, 2);
    emptyView(model);
    CHECK(refuses([&] { engine::readViewerSlice(model, 0, unitPlane(), &runtime); }, "heap"));
    REQUIRE(engine::viewerSliceState(model, 0) != nullptr);
    CHECK(!engine::viewerSliceState(model, 0)->model);
    CHECK(db.slicePlanes->counter == 0 && db.slicePlanes->records.empty());
}

TEST_CASE(slice_direct_database_destruction_does_not_release_retained_plane_records)
{
    auto heap = std::make_shared<doe::SlicePlaneHeap>();
    const doe::SlicePlaneRecord* plane = nullptr;
    {
        engine::DbHandle db;
        db.slicePlanes->bindHeap(heap);
        plane = doe::findOrCreateSlicePlane(*db.slicePlanes, doe::makeSlicePlaneValues(unitPlane()));
        CHECK(db.slicePlanes->records.size() == 1);
    }
    REQUIRE(heap->retainedCount() == 1);
    CHECK(heap->retainedAt(0) == plane);
    CHECK(plane->id == 1 && plane->point[0] == 2.0);
}

TEST_CASE(slice_session_heap_expires_without_reviving_escaped_database_owner)
{
    std::shared_ptr<engine::DbHandle> escapedDb;
    std::weak_ptr<doe::SlicePlaneHeap> heap;
    {
        engine::DiagramEffectDriver driver;
        engine::ConversionSession session(driver);
        session.prepareProjectModel(qt::L("slice-lifetime.DB"));
        escapedDb = session.project().dataDb;
        heap = session.slicePlaneHeapOwner();
        doe::findOrCreateSlicePlane(*escapedDb->slicePlanes, doe::makeSlicePlaneValues(unitPlane()));
    }
    CHECK(heap.expired());
    CHECK(!escapedDb->slicePlanes->heapOwner());
    CHECK(refuses([&] {
        doe::findOrCreateSlicePlane(*escapedDb->slicePlanes, doe::makeSlicePlaneValues(unitPlane()));
    }, "heap")); // guard before inspecting the expired nonowning plane refs
}

TEST_CASE(slice_type4_database_uses_same_process_heap_and_independent_plane_list)
{
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session(driver);
    session.prepareProjectModel(qt::L("slice-parent.DB"));
    auto& project = session.project();
    SubProject child;
    child.project = &project;
    child.type = 4;
    child.dbName = qt::L("slice-child.DB");
    auto* db = engine::subProjectDb(project, child);
    REQUIRE(db && db != project.dataDb.get());
    CHECK(db->slicePlanes->heapOwner() == session.slicePlaneHeapOwner());
    doe::findOrCreateSlicePlane(*db->slicePlanes, doe::makeSlicePlaneValues(unitPlane()));
    CHECK(db->slicePlanes->records.size() == 1);
    CHECK(project.dataDb->slicePlanes->records.empty());
    CHECK(project.dataDb->slicePlanes->counter == 0);
    CHECK(session.slicePlaneHeapOwner()->retainedCount() == 1);
}

TEST_CASE(slice_missing_shared_runtime_refuses_before_collection_constructor)
{
    engine::GuiModel model(nullptr, 2);
    emptyView(model);
    CHECK(refuses([&] { engine::readViewerSlice(model, 0, unitPlane()); }, "runtime"));
    CHECK(!engine::viewerSliceState(model, 0));
}

TEST_CASE(slice_two_views_share_actual_session_scratch_and_existing_collection_does_not_reset_it)
{
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session(driver);
    auto runtime = session.sliceConstructorRuntimeOwner();
    REQUIRE(runtime != nullptr);
    CHECK(!runtime->colorScratchBits);
    engine::GuiModel first(nullptr, 2), second(nullptr, 2);
    emptyView(first); emptyView(second);
    engine::readViewerSlice(first, 0, unitPlane(), runtime.get());
    REQUIRE(runtime->colorScratchBits);
    const auto white = *runtime->colorScratchBits;
    runtime->colorScratchBits = std::array<std::uint32_t, 4>{1, 2, 3, 4};
    engine::readViewerSlice(first, 0, unitPlane(), runtime.get());
    CHECK((*runtime->colorScratchBits == std::array<std::uint32_t, 4>{1, 2, 3, 4}));
    engine::readViewerSlice(second, 0, unitPlane(), runtime.get());
    CHECK(*runtime->colorScratchBits == white);
    CHECK(engine::viewerSliceState(first, 0) != engine::viewerSliceState(second, 0));
}

TEST_CASE(slice_study_io_borrows_session_runtime_without_replacement_or_lifetime_extension)
{
    auto io = doe::makeRecordingStudyIo();
    CHECK(!io->sliceConstructorRuntimeOwner());
    CHECK(refuses([&] { io->bindSliceConstructorRuntime({}); }, "owner"));
    {
        engine::DiagramEffectDriver driver;
        engine::ConversionSession session(driver);
        auto runtime = session.sliceConstructorRuntimeOwner();
        io->bindSliceConstructorRuntime(runtime);
        io->bindSliceConstructorRuntime(runtime); //same actual shared owner is idempotent
        CHECK(io->sliceConstructorRuntimeOwner() == runtime);
        CHECK(refuses([&] { io->bindSliceConstructorRuntime(std::make_shared<doe::SliceConstructorRuntime>()); }, "replacement"));
    }
    CHECK(!io->sliceConstructorRuntimeOwner());
    CHECK(refuses([&] { io->bindSliceConstructorRuntime(std::make_shared<doe::SliceConstructorRuntime>()); }, "replacement"));
}

TEST_CASE(slice_rebind_for_canonical_2d_owner_preserves_constructor_only_collection)
{
    doe::SliceConstructorRuntime runtime;
    engine::GuiModel model(nullptr, 2);
    auto* object = model.objectWrap(1, true, false);
    REQUIRE(object != nullptr);
    engine::viewerSync(model, object, true);
    engine::readViewerSlice(model, 0, unitPlane(), &runtime);
    const auto* collection = engine::viewerSliceState(model, 0);
    REQUIRE(collection != nullptr);
    const void* const viewIdentity = collection->viewIdentity;
    REQUIRE(viewIdentity != nullptr);
    const auto scratch = runtime.colorScratchBits;
    REQUIRE(scratch.has_value());
    engine::viewerSync(model, object, false);
    REQUIRE(engine::viewerSliceState(model, 0) == collection);
    REQUIRE(collection->viewIdentity == viewIdentity);
    CHECK(!collection->model.has_value());
    CHECK(collection->boundObject == nullptr);
    CHECK(collection->backendKnownNull);
    CHECK(collection->graphicCount == 0);
    CHECK(collection->opaqueMembersConstructorOnly);
    CHECK(runtime.colorScratchBits == scratch);
    engine::viewerSync(model, object, false);
    REQUIRE(engine::viewerSliceState(model, 0) == collection);
    REQUIRE(collection->viewIdentity == viewIdentity);
    CHECK(!collection->model.has_value());
    CHECK(collection->boundObject == nullptr);
    CHECK(collection->backendKnownNull && collection->graphicCount == 0);
    CHECK(collection->opaqueMembersConstructorOnly);
    CHECK(runtime.colorScratchBits == scratch);
}

TEST_CASE(slice_rebind_for_canonical_3d_null_db_owner_preserves_collection)
{
    doe::SliceConstructorRuntime runtime;
    engine::GuiModel model(nullptr, 3);
    auto* object = model.objectWrap(1, true, false);
    REQUIRE(object != nullptr);
    engine::viewerSync(model, object, true);
    engine::readViewerSlice(model, 0, unitPlane(), &runtime);
    const auto* collection = engine::viewerSliceState(model, 0);
    REQUIRE(collection != nullptr);
    const void* const viewIdentity = collection->viewIdentity;
    REQUIRE(viewIdentity != nullptr);
    const auto scratch = runtime.colorScratchBits;
    REQUIRE(scratch.has_value());

    engine::viewerSync(model, object, false);
    REQUIRE(engine::viewerSliceState(model, 0) == collection);
    REQUIRE(collection->viewIdentity == viewIdentity);
    CHECK(!collection->model.has_value());
    CHECK(collection->boundObject == nullptr);
    CHECK(collection->backendKnownNull);
    CHECK(collection->graphicCount == 0);
    CHECK(collection->opaqueMembersConstructorOnly);
    CHECK(runtime.colorScratchBits == scratch);
    engine::viewerSync(model, object, false);
    REQUIRE(engine::viewerSliceState(model, 0) == collection);
    REQUIRE(collection->viewIdentity == viewIdentity);
    CHECK(!collection->model.has_value());
    CHECK(collection->boundObject == nullptr);
    CHECK(collection->backendKnownNull && collection->graphicCount == 0);
    CHECK(collection->opaqueMembersConstructorOnly);
    CHECK(runtime.colorScratchBits == scratch);
}

TEST_CASE(slice_escaped_model_manager_cannot_bind_a_child_through_expired_session)
{
    std::shared_ptr<engine::ModelManager> manager;
    {
        engine::DiagramEffectDriver driver;
        engine::ConversionSession session(driver);
        manager = session.modelManagerOwner();
    }
    auto db = std::make_shared<engine::DbHandle>();
    CHECK(refuses([&] { manager->appendCreatedModel(db); }, "expired"));
    CHECK(manager->size() == 0 && db->modelId == 0);
    CHECK(!db->slicePlanes->heapOwner());
}

TEST_CASE(slice_clear_deletes_empty_collections_and_updates_actual_remaining_ctor_only_views)
{
    doe::SliceConstructorRuntime runtime;
    engine::GuiModel model(nullptr, 2);
    float minimum[3]{}, maximum[3]{};
    CHECK(engine::viewerBox(model, 3, minimum, maximum));
    engine::readViewerSlice(model, 0, unitPlane(), &runtime);
    engine::readViewerSlice(model, 0, unitPlane(), &runtime);
    engine::readViewerSlice(model, 2, unitPlane(), &runtime);
    CHECK(engine::viewerItemCount(model) == 3);
    engine::viewerClear(model);
    CHECK(engine::viewerItemCount(model) == 0 && !engine::viewerPendingSliceCleanup(model));
    CHECK(!engine::viewerSliceState(model, 0));
    CHECK(runtime.colorScratchBits.has_value());
}

TEST_CASE(slice_clear_releases_partial_backend_prefix_without_freeing_database_plane_records)
{
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session(driver);
    session.prepareProjectModel(qt::L("slice-clear.DB"));
    auto db = session.project().dataDb;
    auto runtime = session.sliceConstructorRuntimeOwner();
    auto heap = session.slicePlaneHeapOwner();
    engine::GuiModel model(db.get(), 2);
    emptyView(model);
    CHECK(refuses([&] { engine::readViewerSlice(model, 0, unitPlane(), runtime.get()); }, "backend"));
    REQUIRE(db->slicePlanes->records.size() == 1);
    const auto* plane = db->slicePlanes->records[0];
    engine::viewerClear(model);
    CHECK(engine::viewerItemCount(model) == 0 && !engine::viewerPendingSliceCleanup(model));
    CHECK(db->slicePlanes->records[0] == plane && heap->retainedAt(0) == plane);
    CHECK(plane->point[0] == 2.0 && db->slicePlanes->counter == 1);
}

TEST_CASE(slice_populated_cleanup_guard_retains_extracted_owner_without_reinserting_visible_view)
{
    doe::SliceConstructorRuntime runtime;
    engine::GuiModel model(nullptr, 2);
    emptyView(model);
    engine::readViewerSlice(model, 0, unitPlane(), &runtime);
    // Explicit unsupported-state fixture: no reconstructed renderer or backend
    // is asserted; this exercises the reached domain guard and extraction order.
    auto* collection = const_cast<doe::SliceCollectionState*>(engine::viewerSliceState(model, 0));
    REQUIRE(collection != nullptr);
    collection->backendKnownNull = false;
    CHECK(refuses([&] { engine::viewerClear(model); }, "Slice"));
    CHECK(engine::viewerItemCount(model) == 0 && !engine::viewerSliceState(model, 0));
    CHECK(engine::viewerPendingSliceCleanup(model) == collection);
    CHECK(!collection->backendKnownNull);
    CHECK(refuses([&] { model.reset(); }, "pending"));
    CHECK(engine::viewerPendingSliceCleanup(model) == collection);
}

int main() { return testkit::run_all(); }
