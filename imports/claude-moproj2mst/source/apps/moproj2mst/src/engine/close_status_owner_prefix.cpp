#include "m2m/engine/close_status_owner_prefix.hpp"

#include <utility>

#include "m2m/common.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"
#include "m2m/engine/view_manager.hpp"

namespace m2m::engine {
namespace {
[[noreturn]] void unavailable(const char* detail)
{
    throw Error(M2M_E_UNSUPPORTED, std::string("Close status owner prefix: ") + detail);
}
void requireQualification(const std::string& value)
{
    if (value.find_first_not_of(" \t\r\n") == std::string::npos)
        unavailable("owner/slot binding qualification is unavailable");
}
}

CloseStatusItemOwner::CloseStatusItemOwner(std::weak_ptr<CloseStatusLayoutOwner> layout,
    std::string qualification) : layout_(std::move(layout)), qualification_(std::move(qualification)) {}

bool CloseStatusItemOwner::qualifyQWidgetItemV2Accessor(std::string qualification)
{
    requireQualification(qualification);
    if (!accessorQualification_.empty()) return false;
    accessorQualification_ = std::move(qualification);
    return true;
}

bool CloseStatusItemOwner::observeNullWidgetField(std::string qualification)
{
    requireQualification(qualification);
    if (widgetKnowledge_ == CloseStatusWidgetFieldKnowledge::KnownNull) return false;
    widget_.reset(); // candidate nonowning field observation, not a native delete
    widgetQualification_ = std::move(qualification);
    widgetKnowledge_ = CloseStatusWidgetFieldKnowledge::KnownNull;
    return true;
}

void CloseStatusItemOwner::observeWidgetField(const std::shared_ptr<WidgetState>& widget,
    std::string qualification)
{
    requireQualification(qualification);
    if (!widget) unavailable("nonnull item widget field requires its actual widget owner");
    widget_ = widget;
    widgetQualification_ = std::move(qualification);
    widgetKnowledge_ = CloseStatusWidgetFieldKnowledge::KnownWidget;
}

CloseStatusLayoutOwner::CloseStatusLayoutOwner(std::weak_ptr<CloseStatusMainOwner> main,
    std::string qualification) : main_(std::move(main)), qualification_(std::move(qualification)) {}

bool CloseStatusLayoutOwner::observeNullStatusItem(std::string qualification)
{
    requireQualification(qualification);
    if (knowledge_ == CloseStatusItemKnowledge::KnownNull) return false;
    item_.reset(); // release candidate metadata only, not native QWidgetItem/Qt payload
    slotQualification_ = std::move(qualification);
    knowledge_ = CloseStatusItemKnowledge::KnownNull;
    return true;
}

std::shared_ptr<CloseStatusItemOwner> CloseStatusLayoutOwner::observeStatusItem(std::string qualification)
{
    requireQualification(qualification);
    auto observed = std::shared_ptr<CloseStatusItemOwner>(
        new CloseStatusItemOwner(shared_from_this(), qualification));
    item_ = observed;
    slotQualification_ = std::move(qualification);
    knowledge_ = CloseStatusItemKnowledge::KnownItem;
    return observed;
}

CloseStatusMainOwner::CloseStatusMainOwner(const ConversionSession& session, std::string qualification)
    : session_(&session), manager_(session.modelManagerOwner()), qualification_(std::move(qualification))
{
    requireQualification(qualification_);
    requireSession(session);
}

std::shared_ptr<CloseStatusMainOwner> CloseStatusMainOwner::create(
    const ConversionSession& session, std::string qualification)
{
    return std::shared_ptr<CloseStatusMainOwner>(new CloseStatusMainOwner(session, std::move(qualification)));
}

void CloseStatusMainOwner::requireSession(const ConversionSession& session) const
{
    const auto actual = session.modelManagerOwner();
    const auto recorded = manager_.lock();
    if (session_ != &session || !actual || !recorded || actual != recorded || actual->ownerSession() != &session)
        unavailable("owner does not bind this live Session manager identity");
}

std::shared_ptr<CloseStatusLayoutOwner> CloseStatusMainOwner::observeLayoutOwner(std::string qualification)
{
    requireQualification(qualification);
    if (!layout_) layout_ = std::shared_ptr<CloseStatusLayoutOwner>(
        new CloseStatusLayoutOwner(shared_from_this(), std::move(qualification)));
    return layout_;
}

CloseStatusOwnerPrefixProgress::CloseStatusOwnerPrefixProgress(const ConversionSession& session,
    const std::shared_ptr<CloseStatusMainOwner>& main, std::string qualification)
    : main_(main), entry_(session, closeStatusSourceOccurrence, std::move(qualification))
{
    if (!main) unavailable("actual main owner is unavailable");
    main->requireSession(session);
}

CloseStatusOwnerPrefixResult attemptCloseStatusOwnerPrefix(const ConversionSession& session,
    const std::shared_ptr<CloseStatusMainOwner>& main, CloseStatusOwnerPrefixProgress& progress,
    const CloseStatusMainObservation& observation)
{
    if (!main || progress.main_.lock() != main)
        unavailable("progress does not bind this actual main owner identity");
    main->requireSession(session);
    bool entryConsumed = false, slotConsumed = false, accessorConsumed = false, widgetConsumed = false;
    if (progress.stage_ == CloseStatusOwnerPrefixStage::AwaitNonNullEntry) {
        const auto entry = attemptCloseStatusPrefix(session, progress.entry_, observation);
        entryConsumed = entry.lookupCommittedThisAttempt;
        if (entry.stop == CloseStatusPrefixStop::MainLookupUnavailable)
            return {CloseStatusOwnerPrefixStop::StatusEntryUnavailable, false, false, entry.nextSourceSite};
        if (entry.stop == CloseStatusPrefixStop::NullMainSkipBeforeCallerQStringRelease)
            return {CloseStatusOwnerPrefixStop::NullMainOutsideNonNullPrefix, entryConsumed, false,
                    entry.nextSourceSite};
        progress.stage_ = CloseStatusOwnerPrefixStage::AwaitLayoutOwner;
    }
    if (progress.stage_ == CloseStatusOwnerPrefixStage::AwaitLayoutOwner) {
        const auto layout = main->layoutOwner();
        if (!layout)
            return {CloseStatusOwnerPrefixStop::ActualLayoutOwnerUnavailable, entryConsumed, false,
                    "0x653ca627:actual main private/layout owner"};
        if (layout->mainOwner() != main) unavailable("layout does not belong to the actual main owner");
        progress.layout_ = layout;
        progress.stage_ = CloseStatusOwnerPrefixStage::AwaitStatusItem;
    }
    if (progress.stage_ == CloseStatusOwnerPrefixStage::AwaitStatusItem) {
        const auto knowledge = progress.layout_->statusItemKnowledge();
        if (knowledge == CloseStatusItemKnowledge::Unknown)
            return {CloseStatusOwnerPrefixStop::StatusItemKnowledgeUnavailable, entryConsumed, false,
                    "0x653ccd84:actual layout status-item+4f8"};
        if (knowledge == CloseStatusItemKnowledge::KnownNull) {
            //653ccd8c/653ccdb0 null leaf, then outer getter sees a null result.
            progress.stage_ = CloseStatusOwnerPrefixStage::BeforeStatusBarAllocation;
        } else {
            const auto item = progress.layout_->statusItemOwner();
            if (!item || item->layoutOwner() != progress.layout_)
                unavailable("status item does not belong to the consumed actual layout");
            progress.item_ = item; // same candidate identity; no widget/cast result is accepted
            progress.stage_ = CloseStatusOwnerPrefixStage::BeforeItemVirtualWidget;
        }
        slotConsumed = true;
    }
    if (progress.stage_ == CloseStatusOwnerPrefixStage::BeforeStatusBarAllocation)
        return {CloseStatusOwnerPrefixStop::StatusBarAllocationUnavailable, entryConsumed, slotConsumed,
                "0x653ca63f:QStatusBar allocation/constructor guard"};
    if (progress.stage_ == CloseStatusOwnerPrefixStage::BeforeItemVirtualWidget) {
        if (progress.item_->widgetAccessorQualification().empty())
            return {CloseStatusOwnerPrefixStop::ItemVirtualWidgetUnavailable, entryConsumed, slotConsumed,
                    "0x653ccd98:status-item virtual+60/widget/cast guard"};
        // Actual V2+60 target651a4ce0 is a pure raw widget+10 load/return.
        progress.stage_ = CloseStatusOwnerPrefixStage::AwaitItemWidgetField;
        accessorConsumed = true;
    }
    if (progress.stage_ == CloseStatusOwnerPrefixStage::AwaitItemWidgetField) {
        const auto knowledge = progress.item_->widgetFieldKnowledge();
        if (knowledge == CloseStatusWidgetFieldKnowledge::Unknown)
            return {CloseStatusOwnerPrefixStop::WidgetFieldKnowledgeUnavailable, entryConsumed, slotConsumed,
                    "0x651a4ce0:actual QWidgetItemV2 widget+10 field", accessorConsumed, false};
        widgetConsumed = true;
        if (knowledge == CloseStatusWidgetFieldKnowledge::KnownNull) {
            // QMetaObject::cast6711dde0 null branch returns0 without slot0.
            progress.stage_ = CloseStatusOwnerPrefixStage::BeforeStatusBarAllocation;
            return {CloseStatusOwnerPrefixStop::StatusBarAllocationUnavailable, entryConsumed, slotConsumed,
                    "0x653ca63f:QStatusBar allocation/constructor guard", accessorConsumed, widgetConsumed};
        }
        progress.widget_ = progress.item_->widget_; // retain raw-target identity even if expired
        progress.stage_ = CloseStatusOwnerPrefixStage::BeforeWidgetMetaObject;
    }
    const auto widget = progress.widget_.lock();
    if (!widget || !widget->nativeOwnerAlive() || widget->runtimeOwner() != session.widgetRuntimeOwner())
        return {CloseStatusOwnerPrefixStop::WidgetIdentityUnavailable, entryConsumed, slotConsumed,
                "0x6711dde5:nonnull widget identity/runtime/liveness unavailable", accessorConsumed, widgetConsumed};
    return {CloseStatusOwnerPrefixStop::WidgetMetaObjectUnavailable, entryConsumed, slotConsumed,
            "0x6711ddeb:QObject::metaObject before QStatusBar inheritance walk", accessorConsumed, widgetConsumed};
}
} // namespace m2m::engine
