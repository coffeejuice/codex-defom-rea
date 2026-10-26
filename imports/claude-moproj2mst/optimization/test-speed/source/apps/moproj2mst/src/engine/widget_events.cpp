#include "m2m/engine/widget_events.hpp"
#include "m2m/model.hpp"
#include <set>

namespace m2m::engine {

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
