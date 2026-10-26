#include "m2m/doe_slice.hpp"

#include <bit>
#include <cmath>
#include <utility>

#include <defrt/ftn/sse_ops.hpp>

#include "m2m/common.hpp"

namespace m2m::doe {
namespace {
double coefficient(const SliceCoefficients& values, int index)
{
    if (!values.values[index])
        throw Error(M2M_E_UNSUPPORTED, "DOE Slice needs an unavailable original coefficient " +
            std::to_string(index));
    return *values.values[index];
}

double negate(double value)
{
    return std::bit_cast<double>(std::bit_cast<std::uint64_t>(value) ^ 0x8000000000000000ull);
}

std::array<double, 3> normalized(const std::array<double, 3>& raw)
{
    //93a8a0/93a920/93aa30: preserve multiply/add order and original1e-12 gate.
    using namespace def::rt::sse;
    const double length = std::sqrt(add(add(mul(raw[0], raw[0]), mul(raw[1], raw[1])),
                                         mul(raw[2], raw[2])));
    //COMISD/JBE also selects zero on unordered (NaN), unlike C++ <=.
    if (!(std::fabs(length) > 1e-12)) return {0, 0, 0};
    return {div(raw[0], length), div(raw[1], length), div(raw[2], length)};
}

bool matches(const SlicePlaneRecord& record, const SlicePlaneValues& values)
{
    for (int index = 0; index < 3; ++index)
        if (!(record.point[index] == values.point[index])) return false;
    for (int index = 0; index < 3; ++index)
        if (!(record.normal[index] == values.normal[index])) return false;
    return true;
}
} // namespace

SlicePlaneRecord::SlicePlaneRecord(std::int32_t number)
    : name(qt::arg(qt::L("Slicing%1"), number)), id(number)
{}

SlicePlaneRecord& SlicePlaneHeap::allocate(std::int32_t id)
{
    auto allocation = std::make_unique<SlicePlaneRecord>(id);
    auto* result = allocation.get();
    retained_.push_back(std::move(allocation));
    return *result;
}

const SlicePlaneRecord* SlicePlaneHeap::retainedAt(std::size_t index) const noexcept
{
    return index < retained_.size() ? retained_[index].get() : nullptr;
}

std::unique_ptr<SliceCollectionState> createSliceCollection(
    const void* actualView, SliceConstructorRuntime& runtime)
{
    auto allocation = std::make_unique<SliceCollectionState>(actualView);
    //Four df3fa0->df07f0->1afa30 members each visit selector0..77. The
    //last selector writes white; no callback can observe intermediate colors.
    //This final-state projection does not emulate partial member alloc failures.
    runtime.colorScratchBits = std::array<std::uint32_t, 4>{
        0x3f800000, 0x3f800000, 0x3f800000, 0x3f800000};
    return allocation;
}

void prepareEmptySliceCollectionDestruction(SliceCollectionState& collection,
    const std::function<void()>& clearActualSliceFlag)
{
    if (!collection.backendKnownNull || collection.graphicCount != 0 ||
        !collection.opaqueMembersConstructorOnly)
        throw Error(M2M_E_UNSUPPORTED, "DOE Slice populated or changed collection cleanup remains unsupported");
    if (collection.viewIdentity) {
        if (!clearActualSliceFlag)
            throw Error(M2M_E_UNSUPPORTED, "DOE Slice cleanup needs its actual Settings flag callback");
        //Empty df8270 has no renderer work. df8460 next clears SliceOn via
        //the actual ctorView; member/map teardown has no application callbacks.
        clearActualSliceFlag();
    }
    //df8460 resets only unconsumed local fields. Empty df8270 repeats during
    //df8210, then empty maps/sentinels and no-op style/vector members release.
    //No global scratch reset or plane-record destruction occurs in this path.
}

void SlicePlaneStore::bindHeap(const std::shared_ptr<SlicePlaneHeap>& heap)
{
    if (!heap)
        throw Error(M2M_E_UNSUPPORTED, "DOE Slice plane store requires a live native-heap binding");
    if (heapWasBound_) {
        const auto old = heap_.lock();
        if (old.get() != heap.get() || heap_.owner_before(heap) || heap.owner_before(heap_))
            throw Error(M2M_E_UNSUPPORTED, "DOE Slice plane store native-heap replacement is unsupported");
        return;
    }
    heap_ = heap;
    heapWasBound_ = true;
}

SlicePlaneValues makeSlicePlaneValues(const SliceCoefficients& coefficients)
{
    SlicePlaneValues result;
    const double a = coefficient(coefficients, 0);
    if (a != 0) {
        result.point[0] = def::rt::sse::div(negate(coefficient(coefficients, 3)), a);
    } else {
        const double b = coefficient(coefficients, 1);
        if (b != 0) {
            result.point[1] = def::rt::sse::div(negate(coefficient(coefficients, 3)), b);
        } else {
            const double c = coefficient(coefficients, 2);
            if (c != 0)
                result.point[2] = def::rt::sse::div(negate(coefficient(coefficients, 3)), c);
        }
    }
    //Native stores the raw normal only after computing its axis intercept.
    for (int index = 0; index < 3; ++index) result.normal[index] = coefficient(coefficients, index);
    return result;
}

SlicePlaneRecord* findOrCreateSlicePlane(SlicePlaneStore& store, const SlicePlaneValues& values)
{
    //An unavailable/expired adapter process must guard BEFORE any raw references
    //are read. This support precondition is not an emulated native alloc failure.
    const auto heap = store.heapOwner();
    if (!heap)
        throw Error(M2M_E_UNSUPPORTED, "DOE Slice native heap is unavailable or expired");
    for (auto* record : store.records) {
        if (!record)
            throw Error(M2M_E_UNSUPPORTED, "DOE Slice reaches an original null plane-record dereference");
        if (matches(*record, values)) return record;
    }
    //Native ADD wraps the32-bit counter before allocation, even at INT_MAX.
    store.counter = std::bit_cast<std::int32_t>(std::bit_cast<std::uint32_t>(store.counter) + 1u);
    auto* record = &heap->allocate(store.counter);
    store.records.push_back(record); //native append precedes all setters below
    record->point = values.point;
    record->normal = normalized(values.normal);
    record->flagC = true;
    record->flagD = true;
    record->flagE = false;
    return record;
}

} // namespace m2m::doe
