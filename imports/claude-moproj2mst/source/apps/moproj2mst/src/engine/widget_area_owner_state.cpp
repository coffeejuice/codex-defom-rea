#include "m2m/engine/widget_area_owner_state.hpp"
#include "m2m/model.hpp"

namespace m2m::engine {
namespace {
[[noreturn]] void refuse(const char* issue) { throw Error(M2M_E_UNSUPPORTED, issue); }
template<class T> bool sameLease(const std::shared_ptr<T>& a, const std::shared_ptr<T>& b)
{ return a && b && a.get() == b.get() && !a.owner_before(b) && !b.owner_before(a); }
template<class T> void requireTarget(const std::shared_ptr<T>& p)
{
    if (p && (!p->matchesCanonicalLease(p) || !p->observationScopeAvailable()))
        refuse("Widget-area field: target is not its actual canonical owner/lifetime");
}
template<class T> bool pointsTo(const WidgetAreaReferenceSnapshot<T>& ref, const std::shared_ptr<T>& p)
{ return ref.known && ref.nonNull && sameLease(ref.target, p); }
} // namespace

void WidgetAreaCanonicalCell::requireScope() const
{
    const auto window = window_.lock();
    if (!admitted_ || parent_.expired() ||
        (windowBound_ && (!window || !window->observationScopeAvailable() || window->nativeAlive() == false)))
        refuse("Widget-area field: actual parent observation scope unavailable");
}
void WidgetAreaCanonicalCell::assign(const std::shared_ptr<const void>& value)
{
    known_ = true; nonNull_ = bool(value); pointerIdentity_ = value.get();
    allocation_ = value; typed_.reset(); typedBound_ = false; ++revision_;
}
void WidgetAreaCanonicalCell::observeInitial(const std::shared_ptr<const void>& value)
{
    requireScope();
    if (known_) {
        if (nonNull_ != bool(value) || (value && !sameLease(allocation_.lock(), value)))
            refuse("Widget-area field: initial observation cannot replace known state");
        return;
    }
    if (active_) refuse("Widget-area field: initial observation during getter remains guarded");
    assign(value);
}

WidgetAreaLifetimeOwner::WidgetAreaLifetimeOwner(const std::shared_ptr<const void>& owner,
    const std::string& qualification) : identity_(owner.get()), allocation_(owner)
{
    if (!owner || qualification.empty()) refuse("Widget-area owner: actual allocation and qualification required");
}
void WidgetAreaLifetimeOwner::bindCanonical(const std::shared_ptr<WidgetAreaLifetimeOwner>& self)
{ canonical_ = self; }
bool WidgetAreaLifetimeOwner::observationScopeAvailable() const noexcept
{ return admitted_ && !allocation_.expired() && !canonical_.expired(); }
bool WidgetAreaLifetimeOwner::usableNativeOwner() const noexcept
{ return observationScopeAvailable() && alive_ == true; }
bool WidgetAreaLifetimeOwner::matchesCanonicalLease(
    const std::shared_ptr<WidgetAreaLifetimeOwner>& p) const noexcept
{ return sameLease(canonical_.lock(), p); }
void WidgetAreaLifetimeOwner::requireObservation(const std::string& q) const
{
    if (!observationScopeAvailable() || q.empty())
        refuse("Widget-area owner: live observation scope/provenance required");
}
void WidgetAreaLifetimeOwner::observeNativeAlive(bool value, const std::string& q)
{
    requireObservation(q);
    if (alive_ == false && value) refuse("Widget-area owner: native lifetime cannot be revived");
    alive_ = value;
}

template<class T> void WidgetAreaObservedReference<T>::assignObserved(const std::shared_ptr<T>& p)
{ requireTarget(p); known_ = true; nonNull_ = bool(p); target_ = p; }
template<class T> void WidgetAreaObservedReference<T>::observeInitial(const std::shared_ptr<T>& p)
{
    requireTarget(p);
    if (known_) {
        if (nonNull_ != bool(p) || (p && !sameLease(target_.lock(), p)))
            refuse("Widget-area field: initial observation cannot replace known state");
        return;
    }
    assignObserved(p);
}

#define M2M_AREA_OWNER_FACTORY(Type) \
std::shared_ptr<Type> Type::create(const std::shared_ptr<const void>& owner, const std::string& q) { \
    auto result = std::shared_ptr<Type>(new Type(owner, q)); \
    result->bindCanonical(result); return result; }
M2M_AREA_OWNER_FACTORY(WidgetAreaChildOwner)
M2M_AREA_OWNER_FACTORY(WidgetAreaStackOwner)
M2M_AREA_OWNER_FACTORY(WidgetAreaScrollOwner)
M2M_AREA_OWNER_FACTORY(WidgetAreaImplOwner)
#undef M2M_AREA_OWNER_FACTORY

std::shared_ptr<WidgetAreaWindowOwner> WidgetAreaWindowOwner::create(
    const std::shared_ptr<const void>& owner, const std::string& q)
{
    auto result = std::shared_ptr<WidgetAreaWindowOwner>(new WidgetAreaWindowOwner(owner, q));
    result->bindCanonical(result);
    result->area_->window_ = result;
    result->area_->windowBound_ = true;
    return result;
}

void WidgetAreaChildOwner::observeKind(WidgetAreaChildKind value, const std::string& q)
{
    requireObservation(q);
    if (value == WidgetAreaChildKind::Unknown || (kind_ != WidgetAreaChildKind::Unknown && kind_ != value))
        refuse("Widget-area child: concrete observed kind cannot be invented or replaced");
    kind_ = value;
}
void WidgetAreaStackOwner::observeInitialCurrentIndex(int value, const std::string& q)
{
    requireObservation(q);
    if (index_ && index_ != value) refuse("Widget-area stack: observed index cannot become a selection effect");
    index_ = value;
}

void WidgetAreaWindowOwner::observeInitialArea3c8(const std::shared_ptr<WidgetAreaScrollOwner>& p,
    const std::string& q)
{
    requireObservation(q); requireTarget(p); area_->requireScope();
    // Bind typed identity to an already observed/committed SAME raw allocation.
    // An expired typed record cannot be replaced by a new record at its address.
    if (p && area_->typedBound_ && !sameLease(area_->typed_.lock(), p))
        refuse("Widget-area field: typed target cannot rebind to another record/lifetime");
    area_->observeInitial(p ? p->allocationLease() : std::shared_ptr<const void>{});
    if (p) { area_->typed_ = p; area_->typedBound_ = true; }
}
void WidgetAreaWindowOwner::observeInitialStack480(const std::shared_ptr<WidgetAreaStackOwner>& p,
    const std::string& q) { requireObservation(q); stack_.observeInitial(p); }
void WidgetAreaScrollOwner::observeInitialImpl28(const std::shared_ptr<WidgetAreaImplOwner>& p,
    const std::string& q) { requireObservation(q); impl_.observeInitial(p); }
void WidgetAreaScrollOwner::observeInitialScrollContent(const std::shared_ptr<WidgetAreaChildOwner>& p,
    const std::string& q) { requireObservation(q); content_.observeInitial(p); }
void WidgetAreaImplOwner::observeInitialArea28(const std::shared_ptr<WidgetAreaScrollOwner>& p,
    const std::string& q) { requireObservation(q); area_.observeInitial(p); }
void WidgetAreaImplOwner::observeInitialCurrent18(const std::shared_ptr<WidgetAreaChildOwner>& p,
    const std::string& q) { requireObservation(q); current_.observeInitial(p); }
void WidgetAreaImplOwner::observeInitialSaved20(const std::shared_ptr<WidgetAreaChildOwner>& p,
    const std::string& q) { requireObservation(q); saved_.observeInitial(p); }
void WidgetAreaImplOwner::observeInitialPending38(const std::shared_ptr<WidgetAreaChildOwner>& p,
    const std::string& q) { requireObservation(q); pending_.observeInitial(p); }
void WidgetAreaImplOwner::observeInitialFlags(std::optional<bool> mode, std::optional<bool> placeholder,
    const std::string& q)
{
    requireObservation(q);
    if ((mode_ && mode && mode_ != mode) || (placeholder_ && placeholder && placeholder_ != placeholder))
        refuse("Widget-area impl: initial flags cannot replace known native state");
    if (mode) mode_ = mode;
    if (placeholder) placeholder_ = placeholder;
}

void WidgetAreaImplOwner::requireLiveObservedOld(const std::shared_ptr<WidgetAreaChildOwner>& old) const
{
    requireTarget(old);
    if (!old || !old->usableNativeOwner() || old->kind() == WidgetAreaChildKind::Unknown)
        refuse("Widget-area replacement: explicit live old-current kind/lifetime required");
}
void WidgetAreaImplOwner::observePendingCopyAtDadcc3(const std::shared_ptr<WidgetAreaChildOwner>& old,
    const std::string& q)
{
    requireObservation(q); requireLiveObservedOld(old);
    if (!usableNativeOwner() || mode_ != true || !pointsTo(current_.snapshot(), old))
        refuse("Widget-area replacement: pending copy requires actual delete-mode current owner");
    pending_.assignObserved(old); // source+38 is one overwritten guard, not a new queue
}
void WidgetAreaImplOwner::observeCurrentClearAtDadfec(const std::shared_ptr<WidgetAreaChildOwner>& old,
    const std::string& q)
{
    requireObservation(q); requireLiveObservedOld(old);
    if (!usableNativeOwner() || mode_ != true || !pointsTo(current_.snapshot(), old) ||
        !pointsTo(pending_.snapshot(), old))
        refuse("Widget-area replacement: current clear needs the same pending old/delete-mode branch");
    current_.assignObserved({});
}
void WidgetAreaImplOwner::observeCurrentStoreAtDae213(const std::shared_ptr<WidgetAreaChildOwner>& incoming,
    const std::shared_ptr<WidgetAreaChildOwner>& old, const std::string& q)
{
    requireObservation(q); requireTarget(incoming);
    if (!usableNativeOwner() || !mode_ || !incoming || !incoming->usableNativeOwner() ||
        incoming->kind() == WidgetAreaChildKind::Unknown || sameLease(incoming, old))
        refuse("Widget-area replacement: distinct actual incoming owner and known mode required");
    const auto current = current_.snapshot();
    if (!current.known) refuse("Widget-area replacement: current guard is unavailable");
    if (old) {
        requireLiveObservedOld(old);
        if (*mode_) {
            if (current.nonNull || !pointsTo(pending_.snapshot(), old))
                refuse("Widget-area replacement: delete-mode current store must follow its actual clear");
        } else if (!pointsTo(current, old)) {
            refuse("Widget-area replacement: hide-only branch retains old current until this store");
        }
    } else if (current.nonNull) {
        refuse("Widget-area replacement: null old-current profile cannot replace a live placeholder");
    }
    // Records only a qualified source-point guard write. No Enter/show/layout/
    // notifier/scroll callback completion or public admission follows from it.
    current_.assignObserved(incoming);
}
void WidgetAreaImplOwner::requireAdoptionEffects() const
{ refuse("Widget-area owner skeleton: actual Qt/placeholder/registration adoption effects remain guarded"); }

} // namespace m2m::engine
