#include "m2m/engine/brush_gradient_stops.hpp"

#include <utility>

namespace m2m::engine {
namespace {
template<class T> bool conflict(const std::optional<T>& a, const std::optional<T>& b)
{ return a && b && *a != *b; }
template<class T> void merge(std::optional<T>& a, const std::optional<T>& b)
{ if (!a && b) a = b; }
bool sameOwner(const std::shared_ptr<BrushGradientStopDataState>& a,
    const std::shared_ptr<BrushGradientStopDataState>& b) noexcept
{ return a.get() == b.get() && !a.owner_before(b) && !b.owner_before(a); }

// QtGui65120fc0/6511ed10: GlobalColor2/3 build these 14 meaningful bytes.
// QtGui6511b428 and literal65699fa0 provide exact positions0/1.
constexpr BrushGradientStop zeroStop{0x0000000000000000ULL,
    {1, 0, 0, 0, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0}};
constexpr BrushGradientStop oneStop{0x3ff0000000000000ULL,
    {1, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0}};
}

BrushGradientStopDataState::BrushGradientStopDataState(std::uint32_t alias) : alias_(alias) {}
BrushGradientStopDataState::BrushGradientStopDataState(BrushGradientStopOwnerKind kind) : kind_(kind) {}

std::shared_ptr<BrushGradientStopDataState> BrushGradientStopDataState::makeSharedNullSymbol()
{
    auto data = std::shared_ptr<BrushGradientStopDataState>(
        new BrushGradientStopDataState(BrushGradientStopOwnerKind::SharedNullSymbol));
    // QtCore shared_null RVA2f0bb8 immutable header starts1,0,0,1. Atomic refs
    // change at runtime, so do not import its file's1 as a current ref count.
    data->count_ = 0; data->capacity_ = 0; data->flagsLow2_ = 1;
    data->payload_.emplace();
    return data;
}

bool BrushGradientStopDataState::observe(const BrushGradientStopDataObservation& o)
{
    if (kind_ != BrushGradientStopOwnerKind::ObservedSource || !alias_ || !*alias_ ||
        (o.flagsLow2 && *o.flagsLow2 > 3) || conflict(count_, o.count) ||
        conflict(capacity_, o.capacity) || conflict(externalReferences_, o.externalReferences) ||
        conflict(flagsLow2_, o.flagsLow2) || conflict(payload_, o.payload)) return false;
    const auto count = count_ ? count_ : o.count;
    const auto* payload = payload_ ? &*payload_ : (o.payload ? &*o.payload : nullptr);
    if (count && payload && (*count < 0 || static_cast<std::size_t>(*count) != payload->size()))
        return false;
    merge(count_, o.count); merge(capacity_, o.capacity);
    merge(externalReferences_, o.externalReferences); merge(flagsLow2_, o.flagsLow2);
    merge(payload_, o.payload);
    return true;
}

void BrushGradientStopDataState::retainValue() noexcept { ++valueReferences_; }
void BrushGradientStopDataState::releaseValue() noexcept
{
    --valueReferences_;
    if (kind_ == BrushGradientStopOwnerKind::ConstructedDefault && valueReferences_ == 0)
        alive_ = false;
}

bool BrushGradientStopsSource::observeSourceData(
    const std::shared_ptr<BrushGradientStopDataState>& data) noexcept
{
    if (!data || !data->nativeDataAlive() ||
        data->kind() == BrushGradientStopOwnerKind::ConstructedDefault) return false;
    if (data->kind() == BrushGradientStopOwnerKind::ObservedSource &&
        (!data->observationAlias() || !*data->observationAlias())) return false;
    if (data_ && !sameOwner(data_, data)) return false;
    if (!data_) data_ = data;
    return true;
}

BrushGradientStopsCall::BrushGradientStopsCall(std::shared_ptr<const BrushGradientStopsSource> source,
    std::shared_ptr<BrushGradientStopDataState> sharedNull)
    : source_(std::move(source)), sharedNull_(std::move(sharedNull)) {}
BrushGradientStopsCall::~BrushGradientStopsCall() { dropOwnedValues(); }

bool BrushGradientStopsCall::bindSharedNull(
    const std::shared_ptr<BrushGradientStopDataState>& data) noexcept
{
    if (!data || !data->nativeDataAlive() ||
        data->kind() != BrushGradientStopOwnerKind::SharedNullSymbol) return false;
    if (sharedNull_ && !sameOwner(sharedNull_, data)) return false;
    if (!sharedNull_) sharedNull_ = data;
    return true;
}

void BrushGradientStopsCall::dropOwnedValues() noexcept
{
    // Host cleanup for diagnostic call frames. Exception/OOM parity is not
    // established by this cleanup; only the explicit normal path is recovered.
    if (local_) { local_->releaseValue(); local_.reset(); }
    if (returned_) { returned_->releaseValue(); returned_.reset(); }
}

BrushGradientStopsReport BrushGradientStopsCall::resume()
{
    using Stage = BrushGradientStopsStage;
    using Issue = BrushGradientStopsIssue;
    using Effect = BrushGradientStopsEffect;
    const auto pending = [&](Issue issue) { report_.issue = issue; return report_; };
    if (report_.stage == Stage::Returned || report_.stage == Stage::Released) return report_;
    if (!source_) return pending(Issue::SourceUnavailable);
    if (report_.stage == Stage::SourceCount) {
        if (!source_->data_) return pending(Issue::SourceDataUnavailable);
        if (!source_->data_->count_) return pending(Issue::SourceCountUnavailable);
        //6511b3b4 compares count with exactly0, not <=0. Source refs, capacity,
        //flags and payload are not consumed by this zero-count branch.
        effects_.push_back(Effect::SourceCountRead);
        report_.stage = *source_->data_->count_ == 0 ? Stage::DefaultLocal : Stage::NonzeroSource;
    }
    if (report_.stage == Stage::NonzeroSource) return pending(Issue::NonzeroSourceUnrecovered);
    if (report_.stage == Stage::DefaultLocal) {
        if (!sharedNull_) return pending(Issue::SharedNullUnavailable);
        if (sharedNull_->kind_ != BrushGradientStopOwnerKind::SharedNullSymbol)
            return pending(Issue::SharedNullOwnerMismatch);
        //6511b3be..b3cd constructs the local vector from shared_null and refs it.
        local_ = sharedNull_; local_->retainValue();
        effects_.push_back(Effect::LocalValueConstructed);
        effects_.push_back(Effect::SharedNullRetained);

        //6511af09 calls QVectorData::grow(40,count+1,24,true). QtCore6707c5a0
        //true branch computes n+n/2. First n1 => capacity1; then n2 =>3.
        //The successful-allocation path allocates16+24*capacity bytes each time.
        auto first = std::shared_ptr<BrushGradientStopDataState>(
            new BrushGradientStopDataState(BrushGradientStopOwnerKind::ConstructedDefault));
        first->count_ = 0; first->capacity_ = 1; first->flagsLow2_ = 1;
        first->externalReferences_ = 0;
        first->payload_.emplace(); first->retainValue(); allocated_.push_back(first);
        effects_.push_back(Effect::CapacityOneAllocated);
        local_->releaseValue(); local_ = first;
        effects_.push_back(Effect::SharedNullReleased);
        first->payload_->push_back(zeroStop); first->count_ = 1;
        effects_.push_back(Effect::ZeroStopAppended);

        auto second = std::shared_ptr<BrushGradientStopDataState>(
            new BrushGradientStopDataState(BrushGradientStopOwnerKind::ConstructedDefault));
        second->count_ = 0; second->capacity_ = 3; second->flagsLow2_ = 1;
        second->externalReferences_ = 0;
        second->payload_.emplace(); second->retainValue(); allocated_.push_back(second);
        effects_.push_back(Effect::CapacityThreeAllocated);
        second->payload_->push_back(first->payload_->front()); second->count_ = 1;
        effects_.push_back(Effect::ExistingStopCopied);
        local_->releaseValue(); local_ = second;
        effects_.push_back(Effect::CapacityOneReleased);
        effects_.push_back(Effect::CapacityOneFreed);
        second->payload_->push_back(oneStop); second->count_ = 2;
        effects_.push_back(Effect::OneStopAppended);

        //6511b47e..b48f copies returned value, refs and tests known shareable
        //bit0. Then b4a2..b4c6 destroys the intermediate local vector value.
        returned_ = local_; returned_->retainValue();
        effects_.push_back(Effect::ReturnedValueRetained);
        local_->releaseValue(); local_.reset();
        effects_.push_back(Effect::LocalValueReleased);
        effects_.push_back(Effect::LocalValueDestroyed);
        report_.stage = Stage::Returned; report_.issue = Issue::None;
    }
    return report_;
}

bool BrushGradientStopsCall::destroyReturnedValue()
{
    if (report_.stage != BrushGradientStopsStage::Returned) return false;
    returned_->releaseValue();
    effects_.push_back(BrushGradientStopsEffect::ReturnedValueReleased);
    effects_.push_back(BrushGradientStopsEffect::CapacityThreeFreed);
    effects_.push_back(BrushGradientStopsEffect::ReturnedValueDestroyed);
    returned_.reset(); report_.stage = BrushGradientStopsStage::Released;
    return true;
}

} // namespace m2m::engine
