// ctest-labels: m2m-mdt mdt-l6 m2m-mdt-l7
#include <testkit/check.hpp>

#include <bit>
#include <cmath>
#include <limits>

#include "m2m/common.hpp"
#include "m2m/doe_slice.hpp"

using namespace m2m;
namespace {
bool refuses(const auto& action, const char* message)
{
    try { action(); }
    catch (const Error& e) {
        return e.code == M2M_E_UNSUPPORTED && std::string(e.what()).find(message) != std::string::npos;
    }
    return false;
}
doe::SliceCoefficients coefficients(double a, double b, double c, double d)
{
    doe::SliceCoefficients result;
    result.values = {a, b, c, d};
    return result;
}
}

TEST_CASE(doe_slice_constructor_records_native_defaults_without_qobject_ownership)
{
    doe::SlicePlaneHeap heap;
    auto& record = heap.allocate(7);
    CHECK(record.name == qt::L("Slicing7") && record.id == 7);
    CHECK(record.flagC && record.flagD && !record.flagE && !record.flagF);
    CHECK(record.field10 == 1 && record.field14Bits == 0x3c23d70a && record.field48 == 1);
    CHECK((record.normal == std::array<double, 3>({1, 0, 0})));
    CHECK((record.point == std::array<double, 3>({0, 0, 0})));
    for (const auto& vector : record.vectors50)
        CHECK((vector == std::array<double, 3>({0, 0, 0})));
    CHECK(!record.auxiliary98 && !record.auxiliaryA0);
    CHECK(heap.retainedCount() == 1 && heap.retainedAt(0) == &record && !heap.retainedAt(1));
}

TEST_CASE(doe_slice_collection_constructor_keeps_view_and_unavailable_model_distinct)
{
    int actualView = 0;
    doe::SliceCollectionState collection(&actualView);
    CHECK(collection.viewIdentity == &actualView);
    CHECK(!collection.model.has_value() && !collection.boundObject);
    CHECK(collection.backendKnownNull && collection.graphicCount == 0);
}

TEST_CASE(doe_slice_collection_factory_updates_actual_runtime_before_coefficient_guard)
{
    int actualView = 0;
    doe::SliceConstructorRuntime runtime;
    CHECK(!runtime.colorScratchBits);
    auto collection = doe::createSliceCollection(&actualView, runtime);
    REQUIRE(collection != nullptr);
    REQUIRE(runtime.colorScratchBits.has_value());
    for (auto bits : *runtime.colorScratchBits) CHECK(bits == 0x3f800000);
    CHECK(refuses([&] { (void)doe::makeSlicePlaneValues({}); }, "coefficient 0"));
    CHECK(collection->viewIdentity == &actualView && !collection->model.has_value());
    CHECK((*runtime.colorScratchBits)[3] == 0x3f800000);
}

TEST_CASE(doe_slice_collection_factories_share_one_runtime_without_view_copies)
{
    int firstView = 0, secondView = 0;
    doe::SliceConstructorRuntime runtime;
    auto first = doe::createSliceCollection(&firstView, runtime);
    runtime.colorScratchBits = std::array<std::uint32_t, 4>{0, 0, 0, 0};
    auto second = doe::createSliceCollection(&secondView, runtime);
    CHECK(first->viewIdentity == &firstView && second->viewIdentity == &secondView);
    REQUIRE(runtime.colorScratchBits.has_value());
    for (auto bits : *runtime.colorScratchBits) CHECK(bits == 0x3f800000);
}

TEST_CASE(doe_slice_empty_cleanup_clears_flag_before_caller_pointer_and_keeps_process_state)
{
    int actualView = 0;
    doe::SliceConstructorRuntime runtime;
    auto collection = doe::createSliceCollection(&actualView, runtime);
    const auto scratch = runtime.colorScratchBits;
    auto* identity = collection.get();
    bool sliceFlag = true;
    int calls = 0;
    doe::prepareEmptySliceCollectionDestruction(*collection, [&] {
        CHECK(collection.get() == identity && collection->viewIdentity == &actualView);
        ++calls;
        sliceFlag = false;
    });
    CHECK(calls == 1 && !sliceFlag && collection.get() == identity);
    CHECK(!collection->model.has_value() && collection->backendKnownNull && collection->graphicCount == 0);
    collection.reset(); //actual owning caller performs deletion/null before model update
    CHECK(!collection && runtime.colorScratchBits == scratch);
}

TEST_CASE(doe_slice_empty_cleanup_domain_guards_precede_flag_or_pointer_effects)
{
    int actualView = 0;
    for (int unsupported = 0; unsupported < 3; ++unsupported) {
        doe::SliceCollectionState collection(&actualView);
        if (unsupported == 0) collection.backendKnownNull = false;
        if (unsupported == 1) collection.graphicCount = 1;
        if (unsupported == 2) collection.opaqueMembersConstructorOnly = false;
        bool flag = true;
        CHECK(refuses([&] { doe::prepareEmptySliceCollectionDestruction(collection, [&] { flag = false; }); },
            "changed collection cleanup"));
        CHECK(flag && collection.viewIdentity == &actualView);
    }
    doe::SliceCollectionState collection(&actualView);
    CHECK(refuses([&] { doe::prepareEmptySliceCollectionDestruction(collection, {}); }, "Settings flag callback"));
    doe::SliceCollectionState explicitNullView(nullptr);
    doe::prepareEmptySliceCollectionDestruction(explicitNullView, {}); //native ctorView-null skips Settings
}

TEST_CASE(doe_slice_empty_cleanup_callback_refusal_has_no_synthetic_rollback_or_deletion)
{
    int actualView = 0;
    auto collection = std::make_unique<doe::SliceCollectionState>(&actualView);
    auto* identity = collection.get();
    bool flag = true;
    CHECK(refuses([&] { doe::prepareEmptySliceCollectionDestruction(*collection, [&] {
        flag = false;
        throw Error(M2M_E_UNSUPPORTED, "injected Settings owner boundary");
    }); }, "Settings owner boundary"));
    CHECK(!flag && collection.get() == identity && collection->graphicCount == 0);
}

TEST_CASE(doe_slice_store_binding_is_weak_idempotent_and_rejects_replacement)
{
    doe::SlicePlaneStore store;
    auto heap = std::make_shared<doe::SlicePlaneHeap>();
    CHECK(refuses([&] { store.bindHeap({}); }, "live native-heap"));
    store.bindHeap(heap);
    store.bindHeap(heap);
    CHECK(store.heapOwner() == heap);
    CHECK(heap.use_count() == 1);
    auto other = std::make_shared<doe::SlicePlaneHeap>();
    CHECK(refuses([&] { store.bindHeap(other); }, "replacement"));
    auto unrelatedControl = std::shared_ptr<doe::SlicePlaneHeap>(heap.get(), [](auto*) {});
    CHECK(refuses([&] { store.bindHeap(unrelatedControl); }, "replacement"));
    CHECK(store.heapOwner() == heap);
}

TEST_CASE(doe_slice_store_teardown_drops_references_but_retains_native_allocation)
{
    auto heap = std::make_shared<doe::SlicePlaneHeap>();
    const doe::SlicePlaneRecord* record = nullptr;
    {
        doe::SlicePlaneStore store;
        store.bindHeap(heap);
        record = doe::findOrCreateSlicePlane(store,
            doe::makeSlicePlaneValues(coefficients(1, 0, 0, -4)));
        CHECK(store.records.size() == 1 && store.records[0] == record);
    }
    CHECK(heap->retainedCount() == 1 && heap->retainedAt(0) == record);
    CHECK(record->point[0] == 4 && record->id == 1);
}

TEST_CASE(doe_slice_shared_process_heap_does_not_merge_separate_database_lists_or_ids)
{
    auto heap = std::make_shared<doe::SlicePlaneHeap>();
    doe::SlicePlaneStore firstStore, secondStore;
    firstStore.bindHeap(heap);
    secondStore.bindHeap(heap);
    const auto values = doe::makeSlicePlaneValues(coefficients(1, 0, 0, -4));
    auto* first = doe::findOrCreateSlicePlane(firstStore, values);
    auto* second = doe::findOrCreateSlicePlane(secondStore, values);
    CHECK(first != second && first->id == 1 && second->id == 1);
    CHECK(firstStore.counter == 1 && secondStore.counter == 1 && heap->retainedCount() == 2);
    CHECK(firstStore.records[0] == first && secondStore.records[0] == second);
}

TEST_CASE(doe_slice_expired_heap_guards_before_dereferencing_retained_raw_references)
{
    doe::SlicePlaneStore store;
    auto heap = std::make_shared<doe::SlicePlaneHeap>();
    store.bindHeap(heap);
    const auto values = doe::makeSlicePlaneValues(coefficients(1, 0, 0, -4));
    (void)doe::findOrCreateSlicePlane(store, values);
    heap.reset();
    CHECK(!store.heapOwner() && store.records.size() == 1);
    CHECK(refuses([&] { (void)doe::findOrCreateSlicePlane(store, values); }, "expired"));
    CHECK(store.counter == 1 && store.records.size() == 1);
    CHECK(refuses([&] { store.bindHeap(std::make_shared<doe::SlicePlaneHeap>()); }, "replacement"));
}

TEST_CASE(doe_slice_intercept_uses_first_nonzero_axis_and_preserves_raw_normal)
{
    const auto first = doe::makeSlicePlaneValues(coefficients(2, 4, 8, -6));
    const auto second = doe::makeSlicePlaneValues(coefficients(0, 2, 8, -6));
    const auto third = doe::makeSlicePlaneValues(coefficients(0, 0, 2, -6));
    CHECK((first.point == std::array<double, 3>({3, 0, 0})));
    CHECK((second.point == std::array<double, 3>({0, 3, 0})));
    CHECK((third.point == std::array<double, 3>({0, 0, 3})));
    CHECK((first.normal == std::array<double, 3>({2, 4, 8})));
}

TEST_CASE(doe_slice_all_zero_normal_does_not_read_unavailable_d)
{
    doe::SliceCoefficients inputs;
    inputs.values[0] = -0.0;
    inputs.values[1] = 0.0;
    inputs.values[2] = -0.0;
    const auto result = doe::makeSlicePlaneValues(inputs);
    for (double value : result.point) CHECK(std::bit_cast<std::uint64_t>(value) == 0);
    CHECK(std::bit_cast<std::uint64_t>(result.normal[0]) == 0x8000000000000000ull);
}

TEST_CASE(doe_slice_missing_consumed_values_guard_in_native_read_order)
{
    doe::SliceCoefficients inputs;
    CHECK(refuses([&] { (void)doe::makeSlicePlaneValues(inputs); }, "coefficient 0"));
    inputs.values[0] = 1;
    CHECK(refuses([&] { (void)doe::makeSlicePlaneValues(inputs); }, "coefficient 3"));
    inputs.values[3] = 0;
    CHECK(refuses([&] { (void)doe::makeSlicePlaneValues(inputs); }, "coefficient 1"));
    inputs.values[1] = 0;
    CHECK(refuses([&] { (void)doe::makeSlicePlaneValues(inputs); }, "coefficient 2"));
}

TEST_CASE(doe_slice_intercept_flips_sign_bit_before_division)
{
    const auto result = doe::makeSlicePlaneValues(coefficients(1, 0, 0, 0.0));
    CHECK(std::bit_cast<std::uint64_t>(result.point[0]) == 0x8000000000000000ull);
}

TEST_CASE(doe_slice_unit_plane_reuses_first_match_without_resetting_record_flags)
{
    auto heap = std::make_shared<doe::SlicePlaneHeap>();
    doe::SlicePlaneStore store;
    store.bindHeap(heap);
    const auto values = doe::makeSlicePlaneValues(coefficients(1, 0, 0, -4));
    auto* record = doe::findOrCreateSlicePlane(store, values);
    record->flagD = false;
    CHECK(doe::findOrCreateSlicePlane(store, values) == record);
    CHECK(!record->flagD && store.counter == 1 && store.records.size() == 1 && heap->retainedCount() == 1);
}

TEST_CASE(doe_slice_raw_lookup_normalized_store_preserves_original_duplicate_behavior)
{
    auto heap = std::make_shared<doe::SlicePlaneHeap>();
    doe::SlicePlaneStore store;
    store.bindHeap(heap);
    const auto values = doe::makeSlicePlaneValues(coefficients(2, 0, 0, -4));
    auto* first = doe::findOrCreateSlicePlane(store, values);
    auto* second = doe::findOrCreateSlicePlane(store, values);
    CHECK(first != second && first->id == 1 && second->id == 2);
    CHECK(first->normal[0] == 1 && first->point[0] == 2);
    CHECK(doe::findOrCreateSlicePlane(store,
        doe::makeSlicePlaneValues(coefficients(1, 0, 0, -2))) == first);
    CHECK(store.counter == 2 && store.records.size() == 2 && heap->retainedCount() == 2);
}

TEST_CASE(doe_slice_normalization_uses_source_threshold_and_xyz_division)
{
    auto heap = std::make_shared<doe::SlicePlaneHeap>();
    doe::SlicePlaneStore store;
    store.bindHeap(heap);
    auto* tiny = doe::findOrCreateSlicePlane(store,
        doe::makeSlicePlaneValues(coefficients(1e-13, 0, 0, 0)));
    for (double value : tiny->normal) CHECK(std::bit_cast<std::uint64_t>(value) == 0);
    auto* above = doe::findOrCreateSlicePlane(store,
        doe::makeSlicePlaneValues(coefficients(1.01e-12, 0, 0, 0)));
    CHECK(above->normal[0] == 1 && above->normal[1] == 0 && above->normal[2] == 0);
    auto* vector = doe::findOrCreateSlicePlane(store,
        doe::makeSlicePlaneValues(coefficients(3, 4, 0, 0)));
    CHECK((vector->normal == std::array<double, 3>({0.6, 0.8, 0})));
}

TEST_CASE(doe_slice_counter_wraps_before_constructor_and_null_entries_do_not_skip)
{
    auto heap = std::make_shared<doe::SlicePlaneHeap>();
    doe::SlicePlaneStore store;
    store.bindHeap(heap);
    store.counter = std::numeric_limits<std::int32_t>::max();
    const auto values = doe::makeSlicePlaneValues(coefficients(1, 0, 0, -4));
    auto* record = doe::findOrCreateSlicePlane(store, values);
    CHECK(store.counter == std::numeric_limits<std::int32_t>::min());
    CHECK(record->name == qt::L("Slicing-2147483648") && record->id == store.counter);
    store.records.insert(store.records.begin(), nullptr);
    CHECK(refuses([&] { (void)doe::findOrCreateSlicePlane(store, values); }, "null plane-record"));
    CHECK(heap->retainedCount() == 1 && store.records.size() == 2);
}

TEST_CASE(doe_slice_unordered_normalization_zeroes_nan_and_lookup_rejects_nan_points)
{
    auto heap = std::make_shared<doe::SlicePlaneHeap>();
    doe::SlicePlaneStore store;
    store.bindHeap(heap);
    const auto values = doe::makeSlicePlaneValues(coefficients(
        std::numeric_limits<double>::quiet_NaN(), 0, 0, 1));
    CHECK(std::isnan(values.point[0]) && std::isnan(values.normal[0]));
    auto* first = doe::findOrCreateSlicePlane(store, values);
    auto* second = doe::findOrCreateSlicePlane(store, values);
    CHECK(first != second && store.counter == 2);
    CHECK(std::isnan(first->point[0]));
    for (double value : first->normal) CHECK(std::bit_cast<std::uint64_t>(value) == 0);
}

TEST_CASE(doe_slice_unordered_normal_component_never_matches_a_retained_record)
{
    auto heap = std::make_shared<doe::SlicePlaneHeap>();
    doe::SlicePlaneStore store;
    store.bindHeap(heap);
    doe::SlicePlaneValues values;
    values.normal = {std::numeric_limits<double>::quiet_NaN(), 0, 0};
    auto* record = doe::findOrCreateSlicePlane(store, values);
    //Actual native state can later contain NaN; JP must reject even equal bits.
    record->normal = values.normal;
    CHECK(doe::findOrCreateSlicePlane(store, values) != record);
    CHECK(store.records.size() == 2 && heap->retainedCount() == 2);
}

int main() { return testkit::run_all(); }
