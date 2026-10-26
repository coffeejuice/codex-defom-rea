// Typed isolated main/layout status-item prefix. Native Qt effects stay guarded.
#pragma once

#include <memory>
#include <string>

#include "m2m/engine/close_status_prefix.hpp"

namespace m2m::engine {
class CloseStatusMainOwner;
class CloseStatusLayoutOwner;
class CloseStatusItemOwner;
class WidgetState;
class CloseStatusOwnerPrefixProgress;
struct CloseStatusOwnerPrefixResult;

enum class CloseStatusItemKnowledge { Unknown, KnownNull, KnownItem };
enum class CloseStatusWidgetFieldKnowledge { Unknown, KnownNull, KnownWidget };
enum class CloseStatusOwnerPrefixStage {
    AwaitNonNullEntry, AwaitLayoutOwner, AwaitStatusItem,
    BeforeStatusBarAllocation, BeforeItemVirtualWidget, AwaitItemWidgetField,
    BeforeWidgetMetaObject
};
enum class CloseStatusOwnerPrefixStop {
    StatusEntryUnavailable, NullMainOutsideNonNullPrefix, ActualLayoutOwnerUnavailable,
    StatusItemKnowledgeUnavailable, StatusBarAllocationUnavailable, ItemVirtualWidgetUnavailable,
    WidgetFieldKnowledgeUnavailable, WidgetIdentityUnavailable, WidgetMetaObjectUnavailable
};
struct CloseStatusOwnerPrefixResult {
    CloseStatusOwnerPrefixStop stop;
    bool entryConsumedThisAttempt;
    bool slotConsumedThisAttempt;
    const char* nextSourceSite;
    bool accessorConsumedThisAttempt = false;
    bool widgetFieldConsumedThisAttempt = false;
};

// Candidate identity/ownership metadata only: no QWidgetItem, widget accessor,
// cast, QStatusBar or native lifetime is constructed by creating this record.
class CloseStatusItemOwner final {
public:
    CloseStatusItemOwner(const CloseStatusItemOwner&) = delete;
    CloseStatusItemOwner& operator=(const CloseStatusItemOwner&) = delete;
    CloseStatusItemOwner(CloseStatusItemOwner&&) = delete;
    CloseStatusItemOwner& operator=(CloseStatusItemOwner&&) = delete;
    const std::string& qualification() const noexcept { return qualification_; }
    std::shared_ptr<CloseStatusLayoutOwner> layoutOwner() const noexcept { return layout_.lock(); }
    // Qualified actual V2 table656a58c8+60 ->651a4ce0 only. No arbitrary
    // virtual result, statusBar pointer or cast-success observation is accepted.
    bool qualifyQWidgetItemV2Accessor(std::string qualification);
    const std::string& widgetAccessorQualification() const noexcept { return accessorQualification_; }
    CloseStatusWidgetFieldKnowledge widgetFieldKnowledge() const noexcept { return widgetKnowledge_; }
    std::shared_ptr<WidgetState> widgetFieldOwner() const noexcept { return widget_.lock(); }
    const std::string& widgetFieldQualification() const noexcept { return widgetQualification_; }
    // Observations of this same actual item's raw nonowning widget+10 field.
    // Empty/expired weak metadata is NOT a native null field without KnownNull.
    bool observeNullWidgetField(std::string qualification);
    void observeWidgetField(const std::shared_ptr<WidgetState>&, std::string qualification);
private:
    friend class CloseStatusLayoutOwner;
    friend CloseStatusOwnerPrefixResult attemptCloseStatusOwnerPrefix(const ConversionSession&,
        const std::shared_ptr<CloseStatusMainOwner>&, CloseStatusOwnerPrefixProgress&,
        const CloseStatusMainObservation&);
    CloseStatusItemOwner(std::weak_ptr<CloseStatusLayoutOwner>, std::string);
    std::weak_ptr<CloseStatusLayoutOwner> layout_;
    std::string qualification_;
    std::string accessorQualification_, widgetQualification_;
    CloseStatusWidgetFieldKnowledge widgetKnowledge_ = CloseStatusWidgetFieldKnowledge::Unknown;
    std::weak_ptr<WidgetState> widget_;
};

// This is the main owner's actual candidate layout field. Slot knowledge is
// independent of its initially empty C++ metadata pointer: Unknown is not null.
class CloseStatusLayoutOwner final : public std::enable_shared_from_this<CloseStatusLayoutOwner> {
public:
    CloseStatusLayoutOwner(const CloseStatusLayoutOwner&) = delete;
    CloseStatusLayoutOwner& operator=(const CloseStatusLayoutOwner&) = delete;
    CloseStatusLayoutOwner(CloseStatusLayoutOwner&&) = delete;
    CloseStatusLayoutOwner& operator=(CloseStatusLayoutOwner&&) = delete;
    CloseStatusItemKnowledge statusItemKnowledge() const noexcept { return knowledge_; }
    std::shared_ptr<CloseStatusItemOwner> statusItemOwner() const noexcept { return item_; }
    std::shared_ptr<CloseStatusMainOwner> mainOwner() const noexcept { return main_.lock(); }
    const std::string& qualification() const noexcept { return qualification_; }
    const std::string& slotQualification() const noexcept { return slotQualification_; }
    // Explicit current-slot observations, not native allocation/delete effects.
    // Replacement changes this actual slot; existing progress keeps its read.
    bool observeNullStatusItem(std::string qualification);
    std::shared_ptr<CloseStatusItemOwner> observeStatusItem(std::string qualification);
private:
    friend class CloseStatusMainOwner;
    CloseStatusLayoutOwner(std::weak_ptr<CloseStatusMainOwner>, std::string);
    std::weak_ptr<CloseStatusMainOwner> main_;
    std::string qualification_, slotQualification_;
    CloseStatusItemKnowledge knowledge_ = CloseStatusItemKnowledge::Unknown;
    std::shared_ptr<CloseStatusItemOwner> item_;
};

// An explicit native-main ownership projection, NOT Session::MainViewState.
// Binding receipts associate these actual candidate identities with the native
// main/layout/item chain; nonblank labels do not authenticate original evidence.
// This is not installed in Session or bound to a running native main window.
class CloseStatusMainOwner final : public std::enable_shared_from_this<CloseStatusMainOwner> {
public:
    CloseStatusMainOwner(const CloseStatusMainOwner&) = delete;
    CloseStatusMainOwner& operator=(const CloseStatusMainOwner&) = delete;
    CloseStatusMainOwner(CloseStatusMainOwner&&) = delete;
    CloseStatusMainOwner& operator=(CloseStatusMainOwner&&) = delete;
    static std::shared_ptr<CloseStatusMainOwner> create(const ConversionSession&,
                                                      std::string nativeMainBindingQualification);
    std::shared_ptr<CloseStatusLayoutOwner> layoutOwner() const noexcept { return layout_; }
    const std::string& qualification() const noexcept { return qualification_; }
    // Missing layout remains unavailable; this explicit observation creates only
    // its candidate identity. Repeated binding preserves the same layout/receipt.
    std::shared_ptr<CloseStatusLayoutOwner> observeLayoutOwner(std::string qualification);
private:
    friend class CloseStatusOwnerPrefixProgress;
    friend CloseStatusOwnerPrefixResult attemptCloseStatusOwnerPrefix(const ConversionSession&,
        const std::shared_ptr<CloseStatusMainOwner>&, CloseStatusOwnerPrefixProgress&,
        const CloseStatusMainObservation&);
    CloseStatusMainOwner(const ConversionSession&, std::string);
    void requireSession(const ConversionSession&) const;
    const ConversionSession* session_;
    std::weak_ptr<ModelManager> manager_;
    std::string qualification_;
    std::shared_ptr<CloseStatusLayoutOwner> layout_;
};

class CloseStatusOwnerPrefixProgress final {
public:
    CloseStatusOwnerPrefixProgress(const ConversionSession&,
        const std::shared_ptr<CloseStatusMainOwner>&, std::string statusOccurrenceQualification);
    CloseStatusOwnerPrefixProgress(const CloseStatusOwnerPrefixProgress&) = delete;
    CloseStatusOwnerPrefixProgress& operator=(const CloseStatusOwnerPrefixProgress&) = delete;
    CloseStatusOwnerPrefixProgress(CloseStatusOwnerPrefixProgress&&) = delete;
    CloseStatusOwnerPrefixProgress& operator=(CloseStatusOwnerPrefixProgress&&) = delete;
    CloseStatusOwnerPrefixStage stage() const noexcept { return stage_; }
    std::shared_ptr<CloseStatusLayoutOwner> consumedLayoutOwner() const noexcept { return layout_; }
    std::shared_ptr<CloseStatusItemOwner> consumedItemOwner() const noexcept { return item_; }
    std::shared_ptr<WidgetState> consumedWidgetOwner() const noexcept { return widget_.lock(); }
private:
    friend CloseStatusOwnerPrefixResult attemptCloseStatusOwnerPrefix(const ConversionSession&,
        const std::shared_ptr<CloseStatusMainOwner>&, CloseStatusOwnerPrefixProgress&,
        const CloseStatusMainObservation&);
    std::weak_ptr<CloseStatusMainOwner> main_;
    CloseStatusPrefixProgress entry_;
    CloseStatusOwnerPrefixStage stage_ = CloseStatusOwnerPrefixStage::AwaitNonNullEntry;
    std::shared_ptr<CloseStatusLayoutOwner> layout_;
    std::shared_ptr<CloseStatusItemOwner> item_;
    std::weak_ptr<WidgetState> widget_; // consumed nonnull raw-target identity, not its lifetime owner
};

// Reuses CP62's one-shot fresh entry guard. Reads actual main->layout->status
// item storage, not a hasStatusBar boolean or caller-supplied QStatusBar result.
// KnownNull653ccd80 leaf stops at653ca63f allocation. KnownItem still guards
// unknown virtual behavior; qualified QWidgetItemV2 returns its actual+10 field.
// KnownNull widget passes only cast's null leaf and reaches the allocation guard.
// Nonnull widget stops BEFORE QObject::metaObject/cast inheritance checks.
// Consumed slot/accessor/widget identity/branch is retained even
// if a later explicit observation replaces the actual slot. Strong metadata
// leases are not evidence that any native Qt target survives destruction.
CloseStatusOwnerPrefixResult attemptCloseStatusOwnerPrefix(const ConversionSession&,
    const std::shared_ptr<CloseStatusMainOwner>&, CloseStatusOwnerPrefixProgress&,
    const CloseStatusMainObservation&);
} // namespace m2m::engine
