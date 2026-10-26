#include "m2m/engine/paint_device_cleanup.hpp"

#include <set>
#include <utility>

namespace m2m::engine {
namespace {
template<class T> bool sameOwner(const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) noexcept
{ return a.get() == b.get() && !a.owner_before(b) && !b.owner_before(a); }
}

PaintDeviceCleanupGlobals::PaintDeviceCleanupGlobals(std::string qualification)
    : qualification_(std::move(qualification)) {}
PaintDeviceCleanupGlobals::~PaintDeviceCleanupGlobals() { releaseNativeOwner(); }

bool PaintDeviceCleanupGlobals::observe(const PaintDeviceCleanupGlobalObservation& o)
{
    const auto self = weak_from_this().lock(); const auto receiver = o.receiver.lock();
    if (!alive_ || !self || !receiver || !sameOwner(self, receiver) || qualification_.empty() ||
        o.qualification != qualification_ ||
        o.moduleSha256 != "c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887" ||
        o.pointerRva != 0x9cb858u) return false;
    auto next = counters_;
    std::set<std::uint32_t> aliases;
    std::vector<std::pair<std::shared_ptr<PaintDeviceCleanupCounterState>, std::optional<std::int32_t>>> updates;
    for (const auto& entry : o.counters) {
        if (!entry.alias || !aliases.insert(entry.alias).second) return false;
        auto found = next.find(entry.alias);
        auto owner = found == next.end()
            ? std::shared_ptr<PaintDeviceCleanupCounterState>(new PaintDeviceCleanupCounterState(entry.alias)) : found->second;
        if (owner->value_ && entry.value && *owner->value_ != *entry.value) return false;
        next.emplace(entry.alias, owner); updates.emplace_back(owner, entry.value);
    }
    auto pointer = currentCounter_;
    if (o.currentCounterAlias) {
        std::shared_ptr<PaintDeviceCleanupCounterState> owner;
        if (*o.currentCounterAlias) {
            auto found = next.find(*o.currentCounterAlias);
            if (found == next.end()) return false;
            owner = found->second;
        }
        if (pointer && !sameOwner(*pointer, owner)) return false;
        pointer = std::move(owner);
    }
    // Whole-envelope validation precedes changing any existing owner, pointer,
    // module qualification or alias registry. No absolute atomic value is seeded.
    for (const auto& [owner, value] : updates) if (!owner->value_ && value) owner->value_ = value;
    counters_ = std::move(next); currentCounter_ = std::move(pointer); qualified_ = true;
    return true;
}

std::shared_ptr<const PaintDeviceCleanupCounterState> PaintDeviceCleanupGlobals::counterFor(std::uint32_t alias) const noexcept
{
    if (!alive_ || !qualified_) return {};
    const auto found = counters_.find(alias);
    return found == counters_.end() ? nullptr : found->second;
}

PaintDeviceCleanupCall::PaintDeviceCleanupCall(const std::shared_ptr<PaintDeviceCleanupGlobals>& globals)
    : globals_(globals) {}

PaintDeviceCleanupReport PaintDeviceCleanupCall::resume()
{
    using Stage = PaintDeviceCleanupStage;
    using Issue = PaintDeviceCleanupIssue;
    using Effect = PaintDeviceCleanupEffect;
    const auto pending = [&](Issue issue) { report_.issue = issue; return report_; };
    if (report_.stage == Stage::Returned) return report_;
    const auto globals = globals_.lock();
    if (!globals || !globals->nativeOwnerAlive()) return pending(Issue::GlobalsUnavailable);
    if (!globals->qualified()) return pending(Issue::GlobalQualificationUnavailable);
    if (report_.stage == Stage::FirstGetter) {
        //65146e3d..e47: existing pointer skips all lazy creation/registration.
        if (!globals->currentCounter_) return pending(Issue::CounterPointerUnavailable);
        if (!*globals->currentCounter_) return pending(Issue::LazyGlobalUnrecovered);
        counter_ = *globals->currentCounter_; // borrowed native pointer, no Qt ref.
        effects_.push_back(Effect::FirstGetterReturned);
        report_.stage = Stage::SecondGetter;
    }
    if (report_.stage == Stage::SecondGetter) {
        const auto first = counter_.lock();
        if (!first) return pending(Issue::CounterOwnerUnavailable);
        if (!globals->currentCounter_ || !*globals->currentCounter_ ||
            !sameOwner(first, *globals->currentCounter_)) return pending(Issue::GetterOwnerChanged);
        //65148866 performs a second getter call; its pointer is not a synthetic
        //success flag or a second counter owner. Null/replacement is unsupported.
        effects_.push_back(Effect::SecondGetterReturned);
        report_.stage = Stage::CounterComparison;
    }
    if (report_.stage == Stage::CounterComparison) {
        const auto counter = counter_.lock();
        if (!counter) return pending(Issue::CounterOwnerUnavailable);
        if (!counter->value()) return pending(Issue::CounterValueUnavailable);
        effects_.push_back(Effect::CounterValueRead);
        //Imported QBasicAtomicInt::operator==(0), not <=0 or an image-data ref.
        if (*counter->value() != 0) {
            report_.stage = Stage::NonzeroContinuation;
            return pending(Issue::NonzeroCleanupUnrecovered);
        }
        report_.stage = Stage::Returned; report_.issue = Issue::None;
    }
    if (report_.stage == Stage::NonzeroContinuation) return pending(Issue::NonzeroCleanupUnrecovered);
    return report_;
}

} // namespace m2m::engine
