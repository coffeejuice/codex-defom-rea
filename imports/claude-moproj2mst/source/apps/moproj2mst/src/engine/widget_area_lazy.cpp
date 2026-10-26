#include "m2m/engine/widget_area_lazy.hpp"

#include "m2m/model.hpp"
#include <utility>

namespace m2m::engine {
namespace {
template<class A, class B>
bool sameOwner(const std::shared_ptr<A>& a, const std::shared_ptr<B>& b)
{
    return a && b && a.get() == b.get() && !a.owner_before(b) && !b.owner_before(a);
}
[[noreturn]] void refuse(const char* issue) { throw Error(M2M_E_UNSUPPORTED, issue); }
} // namespace

WidgetAreaLazyParent::WidgetAreaLazyParent(const std::shared_ptr<const void>& owner,
    std::optional<std::shared_ptr<const void>> field) : cell_(new WidgetAreaCanonicalCell(owner))
{ if (field) cell_->observeInitial(*field); }

std::shared_ptr<WidgetAreaLazyParent> WidgetAreaLazyParent::createQualified(
    const std::shared_ptr<const void>& owner, std::optional<std::shared_ptr<const void>> field,
    const std::string& qualification)
{
    if (!owner || qualification.empty())
        refuse("Widget-area getter: qualified actual parent owner required");
    auto result = std::shared_ptr<WidgetAreaLazyParent>(new WidgetAreaLazyParent(owner, std::move(field)));
    result->canonical_ = result;
    result->cell_->lazy_ = result;
    return result;
}

std::shared_ptr<WidgetAreaLazyParent> WidgetAreaLazyParent::bindWindowOwner(
    const std::shared_ptr<WidgetAreaWindowOwner>& window, const std::string& qualification)
{
    if (!window || !window->matchesCanonicalLease(window) || qualification.empty())
        refuse("Widget-area getter: actual canonical window binding required");
    window->area_->requireScope();
    if (auto existing = window->area_->lazy_.lock()) return existing;
    auto result = std::shared_ptr<WidgetAreaLazyParent>(new WidgetAreaLazyParent(window->area_));
    result->canonical_ = result;
    window->area_->lazy_ = result;
    return result;
}

void WidgetAreaLazyParent::observeInitialField(std::shared_ptr<const void> area,
                                             const std::string& qualification)
{
    cell_->requireScope();
    if (cell_->known_ || cell_->active_ || qualification.empty())
        refuse("Widget-area getter: initial field observation cannot replace admitted state");
    cell_->observeInitial(area);
}

std::shared_ptr<const void> WidgetAreaLazyParent::cachedArea() const noexcept
{
    return cell_->allocation_.lock();
}

void WidgetAreaLazyParent::requireImplRegistration() const
{
    refuse("Widget-area getter: actual QWidget/impl construction and registration remain guarded");
}

WidgetAreaLazyGetter::~WidgetAreaLazyGetter()
{
    if (const auto cell = cell_.lock(); cell && cell->active_ == this)
        cell->active_ = nullptr;
}

const WidgetAreaLazyReport& WidgetAreaLazyGetter::resume(
    const std::shared_ptr<WidgetAreaLazyParent>& parent, const WidgetAreaLazyObservation& input)
{
    if (!parent || !sameOwner(parent, parent->canonical_.lock()) ||
        !sameOwner(parent->cell_->parent_.lock(), input.parentOwner.lock()))
        refuse("Widget-area getter: actual live canonical parent binding required");
    parent->cell_->requireScope();
    if (bound_) {
        if (!sameOwner(parent, parent_.lock()))
            refuse("Widget-area getter: invocation cannot rebind to another parent");
    } else {
        parent_ = parent;
        cell_ = parent->cell_;
        bound_ = true;
    }
    if (report_.completed || report_.stage == WidgetAreaLazyStage::ConstructorInterrupted)
        return report_;
    if (input.qualification.empty()) {
        report_.issue = "Widget-area getter: source-stage observation unavailable";
        return report_;
    }
    if (parent->cell_->active_ && parent->cell_->active_ != this)
        refuse("Widget-area getter: nested/concurrent lazy construction remains unrepresented");
    if (report_.stage != WidgetAreaLazyStage::FieldRead &&
        fieldRevision_ != parent->cell_->revision_)
        refuse("Widget-area getter: parent field changed during retained invocation");

    switch (report_.stage) {
    case WidgetAreaLazyStage::FieldRead:
        if (!parent->cell_->known_) {
            report_.issue = "Widget-area getter: actual parent+3c8 value unavailable";
            return report_;
        }
        if (parent->cell_->nonNull_) {
            report_.returnedArea = parent->cell_->allocation_.lock();
            const auto typed = parent->cell_->typed_.lock();
            if (!report_.returnedArea || (parent->cell_->typedBound_ &&
                (!typed || !typed->observationScopeAvailable() || typed->nativeAlive() == false))) {
                report_.returnedArea.reset();
                report_.issue = "Widget-area getter: nonnull field allocation binding unavailable";
                return report_;
            }
            report_.cacheHit = report_.completed = true;
            report_.stage = WidgetAreaLazyStage::Complete;
            report_.issue.clear();
            return report_;
        }
        parent->cell_->active_ = this;
        fieldRevision_ = parent->cell_->revision_;
        report_.allocationRequested = true;
        report_.allocationBytes = 0x30; //140018481, before allocator call18486
        report_.stage = WidgetAreaLazyStage::AllocationPending;
        report_.issue = "Widget-area getter: actual allocator result unavailable";
        return report_;

    case WidgetAreaLazyStage::AllocationPending:
        if (!input.allocationSucceeded) return report_;
        if (!*input.allocationSucceeded) {
            if (!input.allocationOwner.expired() ||
                input.constructor != WidgetAreaConstructorObservation::Unavailable)
                refuse("Widget-area getter: null allocation cannot have constructor effects");
            // Native184ae assigns null, then184cb stores it. No constructor call.
            report_.stage = WidgetAreaLazyStage::ParentCommitPending;
            report_.issue = "Widget-area getter: observed null result awaits parent-field store";
            return report_;
        }
        report_.allocationOwner = input.allocationOwner.lock();
        if (!report_.allocationOwner)
            refuse("Widget-area getter: successful allocation requires its actual owner");
        report_.constructorRequested = true; //18498 flag1;1849b parent=null
        report_.stage = WidgetAreaLazyStage::ConstructorPending;
        report_.issue = "Widget-area getter: QWidget/impl/registration constructor continuation unresolved";
        return report_;

    case WidgetAreaLazyStage::ConstructorPending:
        if (input.allocationSucceeded && !*input.allocationSucceeded)
            refuse("Widget-area getter: retained allocation cannot become null");
        if (const auto allocation = input.allocationOwner.lock();
            allocation && !sameOwner(allocation, report_.allocationOwner))
            refuse("Widget-area getter: constructor allocation owner differs");
        if (input.constructor == WidgetAreaConstructorObservation::Unavailable) return report_;
        if (input.constructor == WidgetAreaConstructorObservation::Interrupted) {
            report_.stage = WidgetAreaLazyStage::ConstructorInterrupted;
            report_.issue = "Widget-area getter: constructor did not return; parent store not reached";
            return report_;
        }
        if (!sameOwner(report_.allocationOwner, input.constructorResultOwner.lock()))
            refuse("Widget-area getter: constructor return must be the same allocation/lifetime");
        report_.constructorReturned = true;
        report_.stage = WidgetAreaLazyStage::ParentCommitPending;
        report_.issue = "Widget-area getter: admitted constructor return awaits parent-field store";
        return report_;

    case WidgetAreaLazyStage::ParentCommitPending:
        if (!parent->cell_->known_ || parent->cell_->nonNull_ || parent->cell_->active_ != this)
            refuse("Widget-area getter: actual parent-field store ordering changed");
        if (parent->cell_->windowBound_) {
            if (!input.parentStoreAt184cb) {
                report_.issue = "Widget-area getter: reached184cb parent store observation unavailable";
                return report_;
            }
            const auto& store = *input.parentStoreAt184cb;
            if (!sameOwner(parent->cell_->parent_.lock(), store.parentOwner.lock()) ||
                !sameOwner(invocation_, store.invocationOwner.lock()) ||
                store.expectedFieldRevision != fieldRevision_ || store.occurrence == 0 ||
                store.occurrence <= parent->cell_->lastStoreOccurrence_)
                refuse("Widget-area getter: parent store owner/revision/occurrence mismatch");
            parent->cell_->lastStoreOccurrence_ = store.occurrence;
        }
        //184cb occurs only after admitted return, or the explicit null path.
        parent->cell_->assign(report_.allocationOwner);
        ++parent->cell_->commits_;
        parent->cell_->active_ = nullptr;
        report_.returnedArea = parent->cell_->allocation_.lock();
        report_.completed = true;
        report_.stage = WidgetAreaLazyStage::Complete;
        report_.issue.clear();
        return report_;

    case WidgetAreaLazyStage::ConstructorInterrupted:
    case WidgetAreaLazyStage::Complete:
        return report_;
    }
    return report_;
}

} // namespace m2m::engine
