#include "m2m/engine/splitview_placeholder.hpp"
#include "m2m/common.hpp"
#include "m2m/engine/palette_native_ref.hpp"
#include <utility>

namespace m2m::engine {
namespace {
template<class T> bool sameOwner(const std::shared_ptr<T>& left,
    const std::shared_ptr<T>& right) noexcept
{
    return left && right && left.get() == right.get() &&
        !left.owner_before(right) && !right.owner_before(left);
}
}

SplitViewPlaceholderValue::SplitViewPlaceholderValue()
    : widget_(std::shared_ptr<WidgetState>(new WidgetState(this)))
{
    widget_->componentOwner_ = widget_;
}

SplitViewPlaceholderValue::~SplitViewPlaceholderValue()
{
    // Host abandonment invalidates leases, not a completed Qt destructor,
    // registry removal, posted-event cleanup or QWidget counter decrement.
    widget_->releaseNativeOwner();
}

SplitViewPlaceholderCall::SplitViewPlaceholderCall(
    const std::shared_ptr<WidgetRuntimeState>& runtime, std::string qualification)
    : qualification_(std::move(qualification))
{
    effects_.reserve(6);
    if (!runtime) runtimeBindingIssue_ = SplitViewPlaceholderIssue::RuntimeUnavailable;
    else if (!sameOwner(runtime, runtime->weak_from_this().lock()))
        runtimeBindingIssue_ = SplitViewPlaceholderIssue::RuntimeOwnerMismatch;
    else runtime_ = runtime;
}

SplitViewPlaceholderCall::~SplitViewPlaceholderCall() = default;

const SplitViewPlaceholderReport& SplitViewPlaceholderCall::resume()
{
    const auto pending = [&](SplitViewPlaceholderIssue issue) -> const SplitViewPlaceholderReport& {
        report_.issue = issue;
        return report_;
    };
    if (qualification_.empty()) return pending(SplitViewPlaceholderIssue::MissingQualification);
    // Never retain a foreign block around the same raw runtime. Otherwise its
    // lease could outlive the canonical object and expose a dangling pointer.
    if (runtimeBindingIssue_ != SplitViewPlaceholderIssue::None)
        return pending(runtimeBindingIssue_);
    auto runtime = runtime_.lock();
    if (!runtime) return pending(SplitViewPlaceholderIssue::RuntimeUnavailable);
    if (!sameOwner(runtime, runtime->weak_from_this().lock()))
        return pending(SplitViewPlaceholderIssue::RuntimeOwnerMismatch);
    if (runtime->qualification().empty())
        return pending(SplitViewPlaceholderIssue::RuntimeQualificationUnavailable);

    if (!report_.attempted) {
        // Normal nonnull application0x28 and QWidgetPrivate0x1e0 allocation
        // profile. Allocation failure/unwind is not modeled by this prefix.
        auto next = std::unique_ptr<SplitViewPlaceholderValue>(new SplitViewPlaceholderValue());
        runtime->bindWidgetComponent(next->widget_);
        // Runtime affinity is host metadata. Native private+40 stays null
        // until the later QObject constructor, which this call never reaches.
        next->widget_->thread_.reset();
        value_ = std::move(next);
        report_.attempted = true;
        effects_.push_back(SplitViewPlaceholderEffect::PlaceholderStorageAllocated);
        effects_.push_back(SplitViewPlaceholderEffect::WidgetPrivateAllocated);
        auto& widget = *value_->widget_;

        // Source new QObjectPrivate state, not a default for adapted widgets.
        widget.parent_.reset();
        widget.parentOwnsChild_ = false;
        widget.objectName_ = qt::String();
        widget.childSuffix_.clear();
        widget.childPrefixUnrecovered_ = false;
        widget.pendingChildren_.emplace();
        widget.postedEventCount_ = 0; // 67131213: new receiver postedEvents+24.
        widget.events_.objectFilterCount = 0;
        widget.sendChildEvents_ = true;
        widget.receivesChildEvents_ = true;
        widget.observeEmptyDynamicBooleanProperties(false);
        effects_.push_back(SplitViewPlaceholderEffect::QObjectPrivateConstructed);

        // Early QWidgetPrivate null members precede the first palette copy.
        widget.graphicsEffectPresent_ = false;
        widget.fieldD0Present_ = false;
        widget.installedLayout_.reset();
        widget.layoutAliasAssigned_ = false;
        widget.events_.layoutPresent = false;
        widget.autoFillBackground_ = false;
        widget.stylePresent_ = false;
        widget.stylesheetLength_ = 0;
        widget.proxyPresent_ = false;
        // No later opaque masks, extended attributes, QWidget classes/data,
        // font/palette/locale, native thread retain or counters are assigned.
        effects_.push_back(SplitViewPlaceholderEffect::WidgetPrivatePrefix);
        report_.stage = SplitViewPlaceholderStage::PrivatePalette;
    }

    const auto widget = value_ ? value_->widget_ : nullptr;
    if (!widget || !widget->nativeOwnerAlive() || widget->nativeIdentity() != value_.get() ||
        !widget->matchesComponentOwner(widget) || !sameOwner(widget->runtimeOwner(), runtime))
        return pending(SplitViewPlaceholderIssue::WidgetOwnerUnavailable);
    if (widget->threadState() || widget->parentOwnsChild_ || widget->parentWidget())
        return pending(SplitViewPlaceholderIssue::PrefixStateChanged);

    const auto* inputs = runtime->privateConstructionInputs(*widget);
    if (report_.stage == SplitViewPlaceholderStage::PrivatePalette) {
        const auto owner = runtime->applicationPaletteOwner();
        if (!owner) {
            // Legacy explicit inputs remain guarded; they cannot manufacture
            // a getter result in the absence of its actual registered owner.
            if (!inputs || !inputs->defaultPalette)
                return pending(SplitViewPlaceholderIssue::CurrentPaletteUnavailable);
            if (inputs->qualification != qualification_)
                return pending(SplitViewPlaceholderIssue::PrivateInputsQualificationMismatch);
            return pending(SplitViewPlaceholderIssue::PaletteCopyUnrecovered);
        }
        if (inputs && inputs->qualification != qualification_)
            return pending(SplitViewPlaceholderIssue::PrivateInputsQualificationMismatch);
        paletteOwner_ = owner;
        report_.stage = SplitViewPlaceholderStage::ApplicationPaletteGetter;
    }
    if (inputs && inputs->qualification != qualification_)
        return pending(SplitViewPlaceholderIssue::PrivateInputsQualificationMismatch);

    const auto owner = paletteOwner_.lock();
    if (!sameOwner(owner, runtime->applicationPaletteOwner()))
        return pending(SplitViewPlaceholderIssue::ApplicationPaletteOwnerUnavailable);
    if (report_.stage == SplitViewPlaceholderStage::ApplicationPaletteGetter) {
        try {
            paletteRead_ = runtime->readApplicationPaletteFor(widget, owner);
        } catch (const Error& error) {
            if (error.code != M2M_E_UNSUPPORTED) throw;
            return pending(SplitViewPlaceholderIssue::ApplicationPaletteOwnerUnavailable);
        }
        if (paletteRead_->issue != QApplicationPaletteReadIssue::None || !paletteRead_->output)
            return pending(SplitViewPlaceholderIssue::ApplicationPaletteGetterPending);
        effects_.push_back(SplitViewPlaceholderEffect::ApplicationPaletteGetterReturned);
        report_.stage = SplitViewPlaceholderStage::PrivatePaletteCopy;
    }
    const auto token = paletteRead_ ? paletteRead_->nativeReference_ : nullptr;
    const auto data = paletteRead_ && paletteRead_->output ? paletteRead_->output->data : nullptr;
    const std::shared_ptr<const PaletteNativeRefState> refs = token ? token->stateOwner().lock() : nullptr;
    const auto wrapper = owner->wrapperOwner();
    if (!token || !data || !wrapper || !sameOwner(data, wrapper->value.data) ||
        !sameOwner(refs, data->nativeReferenceState()) || !refs->nativeOwnerAlive() || refs->destructionPending() ||
        token->kind() != PaletteNativeValueKind::GetterTemporary ||
        token->report().stage != PaletteNativeValueStage::Held || !token->retainReceipt())
        return pending(SplitViewPlaceholderIssue::PaletteNativeReferenceUnavailable);
    if (report_.stage == SplitViewPlaceholderStage::PrivatePaletteCopy) {
        if (widget->palette_ || widget->paletteConstruction_)
            return pending(SplitViewPlaceholderIssue::PaletteNativeReferenceUnavailable);
        //6504abcb stores only the actual shared data pointer. Word+8 is not
        // initialized until after the unimplemented temporary release branch.
        widget->paletteConstruction_ = WidgetPaletteConstructionSlot{data, std::nullopt};
        effects_.push_back(SplitViewPlaceholderEffect::PrivatePaletteDataStored);
        report_.stage = SplitViewPlaceholderStage::PaletteTemporaryDeref;
    }
    if (!widget->paletteConstruction_ || !sameOwner(widget->paletteConstruction_->data, data) ||
        widget->paletteConstruction_->groupResolveWord || widget->palette_)
        return pending(SplitViewPlaceholderIssue::PaletteNativeReferenceUnavailable);
    // No native decrement, private word0, destination ref or counter follows
    // this cut, even if an isolated primitive snapshot supplies numeric bits.
    return pending(refs->currentBits() ? SplitViewPlaceholderIssue::PaletteTemporaryDerefUnrecovered
                                      : SplitViewPlaceholderIssue::PaletteRefUnavailable);
}

} // namespace m2m::engine
