#include "m2m/engine/widget_events.hpp"
#include "m2m/model.hpp"
#include <set>
#include <map>
#include <limits>

namespace m2m::engine {

std::optional<std::int32_t> WidgetPostEventQueue::begin() const noexcept
{ return known_ ? std::optional<std::int32_t>{begin_} : std::nullopt; }
std::optional<std::int32_t> WidgetPostEventQueue::end() const noexcept
{ return known_ ? std::optional<std::int32_t>{begin_ + static_cast<std::int32_t>(records_.size())} : std::nullopt; }
std::vector<std::shared_ptr<const WidgetPostEventRecordState>> WidgetPostEventQueue::records() const
{
    if (!known_) throw Error(M2M_E_UNSUPPORTED, "Posted queue population is unknown, not empty");
    return {records_.begin(), records_.end()};
}

void WidgetPostEventQueue::observe(const WidgetPostedQueueObservation& o, const std::string& qualification)
{
    if (known_ || lockHeld_ || qualification.empty() || !o.completePopulation || o.begin < 0 ||
        o.capacity < o.begin || o.records.size() > static_cast<std::size_t>(o.capacity - o.begin))
        throw Error(M2M_E_UNSUPPORTED, "Posted queue requires one complete qualified bounded observation");
    std::map<const WidgetState*, std::uint32_t> counts, actual;
    std::vector<std::weak_ptr<WidgetState>> receivers;
    for (const auto& input : o.receivers) {
        const auto& w = input.receiver;
        if (!w || !w->nativeOwnerAlive() || !w->matchesComponentOwner(w) || w->thread_.get() != owner_ ||
            !w->runtimeOwner() || !counts.emplace(w.get(), input.postedCount).second ||
            (w->postedEventCount_ && w->postedEventCount_ != input.postedCount))
            throw Error(M2M_E_UNSUPPORTED, "Posted queue receiver requires the same live component/thread/count");
        receivers.push_back(w); actual.emplace(w.get(), 0u);
    }
    std::set<std::uint32_t> recordAliases, eventAliases;
    std::vector<std::shared_ptr<WidgetPostEventRecordState>> records;
    for (const auto& input : o.records) {
        const auto& w = input.receiver;
        if (!input.alias || !recordAliases.insert(input.alias).second || !w ||
            !w->matchesComponentOwner(w) || !counts.count(w.get()))
            throw Error(M2M_E_UNSUPPORTED, "Posted record identity/actual receiver is unqualified or duplicated");
        auto record = std::shared_ptr<WidgetPostEventRecordState>(new WidgetPostEventRecordState);
        record->alias_ = input.alias; record->receiver_ = w; record->priority_ = input.priority;
        if (input.event) {
            const auto& event = *input.event;
            if (!event.alias || !eventAliases.insert(event.alias).second || !(event.flags & 1u))
                throw Error(M2M_E_UNSUPPORTED, "Queued heap event requires unique identity and its observed posted flag");
            record->event_ = std::shared_ptr<WidgetPostedEventState>(new WidgetPostedEventState(event));
            ++actual.at(w.get());
        }
        records.push_back(std::move(record));
    }
    if (counts != actual)
        throw Error(M2M_E_UNSUPPORTED, "Complete queue observation disagrees with actual receiver posted counts");
    // Observation validates identities before binding any live count. This is
    // input qualification, not an implementation of native posting or rollback.
    for (const auto& input : o.receivers) input.receiver->postedEventCount_ = input.postedCount;
    receivers_ = std::move(receivers); records_ = std::move(records);
    begin_ = o.begin; capacity_ = o.capacity; uniqueBacking_ = o.uniqueBacking;
    cleanupControl38_ = o.cleanupControl38; known_ = true;
}

WidgetQueueRemovalReport WidgetPostEventQueue::removePostedEvents(WidgetThreadState& currentThread,
    const std::shared_ptr<WidgetState>& receiver, std::int32_t requestedType)
{
    if (!known_ || lockHeld_)
        throw Error(M2M_E_UNSUPPORTED, "Receiver cleanup needs the actual qualified unlocked queue");
    if (receiver) {
        bool admitted = false;
        for (const auto& weak : receivers_) if (weak.lock() == receiver) admitted = true;
        if (!admitted || !receiver->nativeOwnerAlive() || !receiver->matchesComponentOwner(receiver) ||
            receiver->thread_.get() != owner_ || !receiver->runtimeOwner() || !receiver->postedEventCount_)
            throw Error(M2M_E_UNSUPPORTED, "Cleanup cannot substitute its receiver/affinity/count owner");
    } else if (&currentThread != owner_) {
        throw Error(M2M_E_UNSUPPORTED, "Null-receiver cleanup selects the actual current thread queue");
    }
    if (requestedType == 0x46) requestedType = 0x43;
    auto matches = [&](const WidgetPostEventRecordState& record) {
        return (!receiver || record.receiver_.lock() == receiver) && record.event_ &&
            (requestedType == 0 || record.event_->type_ == requestedType);
    };
    const bool zeroReceiverCount = receiver && *receiver->postedEventCount_ == 0u;
    // Admit a whole bounded call before entry. Unclosed raw43 payload/shared
    // backing/dynamic destructors are diagnostic guards, never native success.
    if (!zeroReceiverCount) {
        if (!uniqueBacking_ || !cleanupControl38_)
            throw Error(M2M_E_UNSUPPORTED, "Shared-backing or unknown cleanup-control branch is unresolved");
        for (const auto& record : records_) {
            auto w = record->receiver_.lock();
            if (!record->alive_ || !w || !w->nativeOwnerAlive() || !w->runtimeOwner() ||
                w->thread_.get() != owner_ || !w->postedEventCount_)
                throw Error(M2M_E_UNSUPPORTED, "Cleanup record lost its actual receiver/count owner");
            if (matches(*record)) {
                const auto& e = *record->event_;
                if (!e.alive_ || !e.posted() || e.type_ == 0x43 ||
                    e.deletingClass_ != WidgetPostedDeletingClass::PlainQtGuiQEvent ||
                    (e.type_ != 0x4a && e.type_ != 0x4c))
                    throw Error(M2M_E_UNSUPPORTED, "Matched event payload/deleting destructor is not source-closed");
            }
        }
    }
    WidgetQueueRemovalReport report;
    // Reserve diagnostic/deletion storage before native mutation. Allocation
    // failure paths are not claimed by this selected successful helper branch.
    if (records_.size() > (std::numeric_limits<std::size_t>::max() - 2u) / 6u)
        throw Error(M2M_E_UNSUPPORTED, "Posted queue trace size is outside represented bounds");
    report.effects.reserve(records_.size() * 6u + 2u);
    std::vector<std::shared_ptr<WidgetPostedEventState>> deleted;
    deleted.reserve(records_.size());
    auto effect = [&](WidgetQueueRemovalEffectKind kind, std::uint32_t alias = 0) {
        report.effects.push_back({kind, alias});
    };
    lockHeld_ = true; effect(WidgetQueueRemovalEffectKind::Lock);
    if (!zeroReceiverCount) {
        std::size_t kept = 0;
        for (std::size_t read = 0; read < records_.size(); ++read) {
            auto& record = records_[read];
            if (matches(*record)) {
                auto w = record->receiver_.lock();
                --*w->postedEventCount_; // original native32 decrement
                effect(WidgetQueueRemovalEffectKind::ReceiverCountDecrement, record->alias_);
                record->event_->flags_ &= static_cast<std::uint16_t>(0xfffeu);
                effect(WidgetQueueRemovalEffectKind::EventPostedCleared, record->event_->alias_);
                deleted.push_back(record->event_);
                record->event_.reset();
                effect(WidgetQueueRemovalEffectKind::EventTombstoned, record->alias_);
                ++report.matched;
            } else if (*cleanupControl38_ == 0u) {
                if (read != kept) {
                    effect(WidgetQueueRemovalEffectKind::RecordSwap, record->alias_);
                    std::swap(records_[read], records_[kept]); // 6711a5e0
                }
                ++kept;
            }
        }
        if (*cleanupControl38_ == 0u) {
            // 6711ab90 frees only trailing record allocations. Escaped diagnostic
            // leases survive as invalid observers, not native allocation owners.
            for (std::size_t i = kept; i < records_.size(); ++i) {
                records_[i]->alive_ = false;
                effect(WidgetQueueRemovalEffectKind::RecordReleased, records_[i]->alias_);
            }
            // QListData67048ed0 chooses the shorter side. Removing an odd
            // entire population advances begin; other tail erases shrink end.
            const auto removed = records_.size() - kept;
            if (kept == 0 && (removed & 1u)) begin_ += static_cast<std::int32_t>(removed);
            records_.resize(kept);
        }
    }
    lockHeld_ = false; effect(WidgetQueueRemovalEffectKind::Unlock);
    // 6711b830 deletes in match order only AFTER unlocking. The admitted plain
    // QEvent deleting destructor has posted=false; it cannot reenter removal.
    for (const auto& event : deleted) {
        event->alive_ = false;
        effect(WidgetQueueRemovalEffectKind::EventDeleted, event->alias_);
    }
    return report;
}

std::unique_ptr<WidgetEventState> WidgetEventState::dynamicPropertyChange(std::string name)
{ return std::unique_ptr<WidgetEventState>(new WidgetEventState(std::move(name))); }
std::unique_ptr<WidgetEventState> WidgetEventState::create()
{
    auto event = std::unique_ptr<WidgetEventState>(new WidgetEventState(std::string{}));
    event->type_ = 0x0f;
    return event;
}
std::unique_ptr<WidgetEventState> WidgetEventState::childAdded(const std::shared_ptr<GridLayoutState>& child)
{
    if (!child || !child->nativeOwnerAlive() || !child->parentWidget())
        throw Error(M2M_E_UNSUPPORTED, "ChildAdded requires the actual live parented grid identity");
    auto actual = child->allocationOwner_.lock();
    if (!actual || actual.get() != child.get() || actual.owner_before(child) || child.owner_before(actual))
        throw Error(M2M_E_UNSUPPORTED, "ChildAdded cannot substitute a foreign child control block");
    auto event = std::unique_ptr<WidgetEventState>(new WidgetEventState(std::string{}));
    event->type_ = 0x44; event->child_ = child;
    return event;
}

void WidgetEventState::destroyStackEvent()
{
    if (!alive_ || activeDispatch_ || posted_)
        throw Error(M2M_E_UNSUPPORTED, "Stack event destruction requires a live unposted event outside pending dispatch");
    // 6711d680 releases the QByteArray before its base-event teardown. The
    // posted-event removal branch is excluded by this actual stack event flag.
    propertyName_.clear();
    child_.reset(); // borrowed payload; native destructor never deletes child
    alive_ = false;
    lifetime_.reset();
}

std::optional<std::uint32_t> WidgetThreadState::notifyDepth() const noexcept
{
    if (!depthBase_) return {};
    return *depthBase_ + notifyDelta_; // defined native 32-bit wrap
}
void WidgetThreadState::observeNotifyDepth(std::uint32_t depth, const std::string& qualification)
{
    if (qualification.empty() || notifyDelta_ || (depthBase_ && *depthBase_ != depth))
        throw Error(M2M_E_UNSUPPORTED, "Notify-depth observation cannot replace live or pending thread state");
    depthBase_ = depth;
}

const WidgetDispatchReport& WidgetEventDispatch::resume(WidgetRuntimeState& runtime,
    const std::shared_ptr<WidgetState>& receiver, WidgetEventState& event)
{
    if (!receiver || !receiver->nativeOwnerAlive() || !receiver->matchesComponentOwner(receiver) || receiver->runtime_.lock().get() != &runtime ||
        receiver->thread_ != runtime.thread_ || !event.alive_)
        throw Error(M2M_E_UNSUPPORTED, "Event dispatch requires the same live native widget/runtime/thread/event");
    if (event.type_ == 0x44) {
        auto child = event.child_.lock();
        if (!child || !child->nativeOwnerAlive() || child->parentWidget() != receiver)
            throw Error(M2M_E_UNSUPPORTED, "ChildAdded lost its actual borrowed child/receiver identity");
    }
    if (runtime_ && (runtime_ != &runtime || runtimeOwner_.lock().get() != &runtime ||
                     receiver_.lock() != receiver || event_ != &event ||
                     eventLifetime_.lock() != event.lifetime_ || thread_.lock() != runtime.thread_))
        throw Error(M2M_E_UNSUPPORTED, "A pending native event call cannot substitute its owners");
    if (report_.completed) return report_;
    if (!runtime_) {
        if (event.activeDispatch_) throw Error(M2M_E_UNSUPPORTED, "Event already belongs to a pending native call");
        runtime_ = &runtime; runtimeOwner_ = runtime.weak_from_this(); receiver_ = receiver;
        event_ = &event; eventLifetime_ = event.lifetime_; thread_ = runtime.thread_;
        event.activeDispatch_ = true;
        report_.stage = WidgetDispatchStage::InternalCallbacks;
    }
    report_.issue.clear();
    auto pending = [&](const char* issue) -> const WidgetDispatchReport& { report_.issue = issue; return report_; };
    auto finish = [&](bool result, bool helperReturned) -> const WidgetDispatchReport& {
        if (helperReturned) event.spontaneous_ = false; // 65022c60 normal helper tail
        if (depthEntered_) { --runtime.thread_->notifyDelta_; depthEntered_ = false; }
        event.activeDispatch_ = false;
        report_.stage = WidgetDispatchStage::Complete;
        report_.completed = true; report_.originalResult = result;
        return report_;
    };
    const auto& app = runtime.applicationEvents_;
    if (report_.stage == WidgetDispatchStage::InternalCallbacks) {
        if ((event.type_ != 0xaa && event.type_ != 0x0f && event.type_ != 0x44) || event.posted_)
            return pending("This retained stack event type is not source-closed");
        if (app.present != true || app.internalCallbackCount != 0u)
            return pending("Actual application and complete empty QInternal callback population are required");
        // 671195cf. Unknown base is preserved symbolically; no closed callback
        // below consumes the numeric depth. Numeric consumers remain unsupported.
        ++runtime.thread_->notifyDelta_;
        depthEntered_ = true;
        report_.stage = WidgetDispatchStage::Application;
    }
    if (report_.stage == WidgetDispatchStage::Application) {
        if (!app.closing) return pending("Application closing state is unqualified");
        if (*app.closing) return finish(false, false);
        report_.stage = WidgetDispatchStage::ApplicationGestures;
    }
    if (report_.stage == WidgetDispatchStage::ApplicationGestures) {
        // Raw0xaa and raw0x44 bypass this path. Raw0x0f gathers contexts before
        // application filters; empty current contexts skip nonconst iteration.
        if (event.type_ == 0x0f) {
            if (!app.gestureManagerPresent) return pending("Application gesture manager presence is unqualified");
            if (*app.gestureManagerPresent) {
                auto current = receiver; bool ancestor = false;
                std::set<const WidgetState*> seen;
                while (current) {
                    if (!seen.insert(current.get()).second || !current->nativeOwnerAlive())
                        return pending("Gesture ancestor chain is invalid");
                    if (current->events_.gestureContextCount != 0u)
                        return pending("Gesture context population is unknown or populated");
                    if (ancestor && current->events_.gestureMapExclusive != true)
                        return pending("Ancestor gesture-map detach dependency remains unresolved");
                    if (!current->windowFlags_) return pending("Gesture ancestor window boundary is unqualified");
                    if (*current->windowFlags_ & 1u) break;
                    if (!current->parentBindingKnown_) return pending("Gesture parent identity is unqualified");
                    auto next = current->parentWidget();
                    if (!next && current->parentOwnsChild_) return pending("Gesture native parent was released");
                    current = next; ancestor = true;
                }
            }
        }
        report_.stage = WidgetDispatchStage::ApplicationFilters;
    }
    if (report_.stage == WidgetDispatchStage::ApplicationFilters) {
        if (receiver->events_.sameApplicationThread != true || !app.filters)
            return pending("Application filter population or receiver affinity is unqualified");
        for (const auto& filter : *app.filters) {
            if (filter.sameApplicationThread != true)
                return pending("Filter affinity requires qualification; foreign-thread diagnostics are not elided");
            if (filter.kind != WidgetApplicationEventObservation::Filter::Kind::WindowsVistaStyle)
                return pending("Application filter receiver is not source-closed for this event");
            // Actual VistaStyle -> WindowsStyle -> QObject eventFilter returns
            // false for these three types; no callback reads notify depth or changes lists.
        }
        report_.stage = WidgetDispatchStage::ReceiverLayout;
    }
    if (report_.stage == WidgetDispatchStage::ReceiverLayout) {
        // The concrete QLayout raw44 leaf returns without mutation, regardless
        // of its enabled bit. Unknown nonnull layout identity stays guarded.
        if (receiver->layoutPresence() != false && !(event.type_ == 0x44 && receiver->installedLayout()))
            return pending("Actual installed-layout dispatch remains unresolved");
        report_.stage = WidgetDispatchStage::ReceiverFilters;
    }
    if (report_.stage == WidgetDispatchStage::ReceiverFilters) {
        if (receiver->events_.objectFilterCount != 0u)
            return pending("Actual complete object-filter population is not qualified empty");
        report_.stage = WidgetDispatchStage::Receiver;
    }
    if (receiver->eventReceiverKind_ != WidgetEventReceiverKind::QWidget)
        return pending("Actual QWidget event receiver binding is unqualified");
    if (event.type_ == 0x44) {
        if (receiver->events_.childEventHandler != WidgetChildEventHandler::QObjectLeaf)
            return pending("Actual childEvent virtual destination is unqualified");
        // QObject calls the leaf childEvent then returns AL=1, not its result.
        return finish(true, true);
    }
    if (event.type_ == 0xaa && event.propertyName_.compare(0, 12, "_q_customDpi") == 0)
        return pending("The custom-DPI property receiver has additional unresolved effects");
    // QWidget's raw Create branch and the non-custom-DPI property branch
    // return false through source-closed paths without notify-depth reads.
    return finish(false, true);
}

const WidgetConstructorTailReport& WidgetConstructorTail::resume(WidgetRuntimeState& runtime,
    const std::shared_ptr<WidgetState>& receiver)
{
    if (!receiver || !receiver->nativeOwnerAlive() || !receiver->matchesComponentOwner(receiver) ||
        receiver->runtime_.lock().get() != &runtime || receiver->thread_ != runtime.thread_)
        throw Error(M2M_E_UNSUPPORTED, "Widget constructor tail requires its actual live component/runtime/thread");
    if (bound_ && (runtime_.lock().get() != &runtime || receiver_.lock() != receiver))
        throw Error(M2M_E_UNSUPPORTED, "Widget constructor tail cannot replace its actual owners");
    if (!bound_) { bound_ = true; runtime_ = runtime.weak_from_this(); receiver_ = receiver; }
    auto pending = [&](const char* issue) -> const WidgetConstructorTailReport& { report_.issue = issue; return report_; };
    report_.issue.clear();
    if (report_.stage == WidgetConstructorTailStage::FontDevice) {
        if (!receiver->font_ || !receiver->font_->data)
            return pending("Actual private font construction remains incomplete");
        if (runtime.currentQThreadIsApplicationQThread_ != true)
            return pending("Font metric current/application QThread identity or foreign DC path is unqualified");
        if (!runtime.displayDcAlias_ || *runtime.displayDcAlias_ == 0)
            return pending("Actual display DC cache is unknown or reaches GetDC allocation");
        std::optional<std::int32_t> metric;
        auto current = receiver; std::set<const WidgetState*> seen;
        while (current) {
            if (!current->nativeOwnerAlive() || !seen.insert(current.get()).second ||
                current->paintDeviceKind_ != WidgetPaintDeviceKind::QWidget || !current->dpiYOverride_)
                return pending("Actual QWidget metric virtual/DPI override is unqualified");
            if (*current->dpiYOverride_) { metric = *current->dpiYOverride_; break; }
            if (!current->parentBindingKnown_) return pending("Metric parent identity is unqualified");
            auto next = current->parentWidget();
            if (!next) {
                if (current->parentOwnsChild_) return pending("Metric native parent was released");
                metric = runtime.displayLogicalDpiY_; break;
            }
            current = next;
        }
        if (!metric) return pending("Display DC logical DPI capability is unqualified");
        const auto& font = *receiver->font_;
        if (!font.data->comparisonField38() || !font.data->deviceScreenField3c())
            return pending("Shared font metric/screen comparison fields are unqualified");
        if (*font.data->comparisonField38() != static_cast<std::uint32_t>(*metric) || *font.data->deviceScreenField3c() != 0)
            return pending("Font device mismatch reaches unreconstructed font-data copy");
        // Exact matching branch adds/releases only a temporary shared reference.
        // QWidget devType1 skips the printer-engine branch. Same-data assignment
        // preserves this actual FontData owner and resolve mask.
        report_.stage = WidgetConstructorTailStage::Attributes;
    }
    if (report_.stage == WidgetConstructorTailStage::Attributes) {
        receiver->assignAttributeBit(34, true); receiver->assignAttributeBit(35, true);
        report_.stage = WidgetConstructorTailStage::WidgetCounts;
    }
    if (report_.stage == WidgetConstructorTailStage::WidgetCounts) {
        if (!runtime.currentWidgets_ || !runtime.maximumWidgets_)
            return pending("Actual QApplication widget current/maximum counters are unqualified");
        ++*runtime.currentWidgets_;
        // XOR the sign bit to compare native signed32 values without overflow
        // or implementation-defined unsigned-to-signed conversion.
        if ((*runtime.currentWidgets_ ^ 0x80000000u) > (*runtime.maximumWidgets_ ^ 0x80000000u))
            runtime.maximumWidgets_ = runtime.currentWidgets_;
        report_.stage = WidgetConstructorTailStage::Compatibility;
    }
    if (report_.stage == WidgetConstructorTailStage::Compatibility) {
        if (!runtime.applicationCompileVersion_ ||
            (*runtime.applicationCompileVersion_ >= 0x40200 && !runtime.applicationAttributes_))
            return pending("Constructor compatibility predicate is unqualified");
        if (*runtime.applicationCompileVersion_ < 0x40200 || (*runtime.applicationAttributes_ & 1u))
            return pending("Constructor compatibility branch reaches native widget creation");
        create_ = WidgetEventState::create();
        report_.stage = WidgetConstructorTailStage::Create;
    }
    if (report_.stage == WidgetConstructorTailStage::Create) {
        const auto& result = dispatch_.resume(runtime, receiver, *create_);
        if (!result.completed) { report_.issue = result.issue; return report_; }
        report_.stage = WidgetConstructorTailStage::PostPending;
    }
    // 6506e857 precedes private+c8 store and stack Create destruction6506e869.
    // Keep that actual event alive; no fabricated posting or normal teardown.
    return pending("Constructor raw0x4a posting requires the actual shared queue/dispatcher continuation");
}

} // namespace m2m::engine
