#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <vector>

#include "m2m/qt4.hpp"

namespace def::model { class LLOBJ_DATA; }
namespace m2m::engine { struct GuiModel; }

namespace m2m::doe {

//141200e50 uses function-local, initially unavailable coefficients. Repeated
//Slice elements in ONE Object call retain coefficients omitted by later rows.
struct SliceCoefficients {
    std::array<std::optional<double>, 4> values;
};

struct SlicePlaneValues {
    std::array<double, 3> point{};
    std::array<double, 3> normal{}; //raw input vector used by native lookup
};

//Actual process-global scratch143d82540..54c, written by material selector
//1b0730 during each opaque base-record constructor. Initial contents unknown.
struct SliceConstructorRuntime {
    std::optional<std::array<std::uint32_t, 4>> colorScratchBits;
};

//Plain native0xa8 record (1af770->1af850), not a QObject. Unnamed fields retain
//their source offsets; no unconsumed padding or full C++ layout is inferred.
struct SlicePlaneRecord {
    explicit SlicePlaneRecord(std::int32_t id);
    qt::String name;
    std::int32_t id;
    bool flagC = true, flagD = true, flagE = false, flagF = false;
    std::int32_t field10 = 1;
    std::uint32_t field14Bits = 0x3c23d70a;
    std::array<double, 3> normal{1, 0, 0};
    std::array<double, 3> point{};
    std::int32_t field48 = 1;
    std::array<std::array<double, 3>, 3> vectors50{};
    void* auxiliary98 = nullptr;
    void* auxiliaryA0 = nullptr;
};

//Explicit adapter retention of original raw native allocations. A Conversion-
//Session represents one fresh process; DB teardown drops references without
//freeing these records. This does not establish every normal-Close cleanup path.
class SlicePlaneHeap {
public:
    SlicePlaneRecord& allocate(std::int32_t id);
    std::size_t retainedCount() const noexcept { return retained_.size(); }
    const SlicePlaneRecord* retainedAt(std::size_t index) const noexcept;
private:
    std::vector<std::unique_ptr<SlicePlaneRecord>> retained_;
};

//Same native SfModelBase child as DbHandle: counter+10 and NONOWNING QList+18.
//A weak heap binding prevents an adapter lease from reviving an ended process.
class SlicePlaneStore {
public:
    SlicePlaneStore() = default;
    SlicePlaneStore(const SlicePlaneStore&) = delete;
    SlicePlaneStore& operator=(const SlicePlaneStore&) = delete;
    void bindHeap(const std::shared_ptr<SlicePlaneHeap>& heap);
    std::shared_ptr<SlicePlaneHeap> heapOwner() const noexcept { return heap_.lock(); }
    std::int32_t counter = 0;
    std::vector<SlicePlaneRecord*> records;
private:
    bool heapWasBound_ = false;
    std::weak_ptr<SlicePlaneHeap> heap_;
};

//Private View owns native+338 collection. Constructor establishes an empty
//graphics list/null backend; unrecovered renderer state remains opaque.
struct SliceCollectionState {
    explicit SliceCollectionState(const void* ownerView) : viewIdentity(ownerView) {}
    std::optional<engine::GuiModel*> model; //+3f30 unavailable until df8520
    const void* viewIdentity; //+3f38 actual View stored by ctor AND df8520
    def::model::LLOBJ_DATA* boundObject = nullptr; //+3f40
    bool backendKnownNull = true;
    std::size_t graphicCount = 0;
    //Adapter support-domain marker, not a native member. Backend/member writes
    //must revoke this before their unconsumed state can reach empty teardown.
    bool opaqueMembersConstructorOnly = true;
};

//Native allocation precedes constructors; the final scratch write precedes
//the caller's View+338 commit. Internal member-allocation failures are qualified.
std::unique_ptr<SliceCollectionState> createSliceCollection(
    const void* actualView, SliceConstructorRuntime& runtime);

//df8460 + constructor-only empty member teardown. Domain guards precede effects;
//the callback resolves actual Settings AFTER native obj-null/list-pop effects.
//Caller then deletes/nulls its actual owner and performs the separate c80560
//model update. This helper never clears the owner's pointer or native heap.
void prepareEmptySliceCollectionDestruction(SliceCollectionState& collection,
    const std::function<void()>& clearActualSliceFlag);

//1fe3f0 computes the point before model/DB lookup. Missing coefficient reads
//guard at their first consumed value; all-zero normal does not read d.
SlicePlaneValues makeSlicePlaneValues(const SliceCoefficients& coefficients);

//Native lookup compares point and RAW normal, then increments ID, constructs
//and appends before the point/normal/flag setters. Allocator lifetime is guarded
//before dereferencing adapter raw references. No graphics insertion here.
SlicePlaneRecord* findOrCreateSlicePlane(SlicePlaneStore& store, const SlicePlaneValues& values);

} // namespace m2m::doe
