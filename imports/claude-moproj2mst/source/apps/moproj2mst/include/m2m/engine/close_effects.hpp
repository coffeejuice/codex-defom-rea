// Isolated Close notification effects. Lifecycle invocation remains root-owned.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

#include "m2m/qt4.hpp"

namespace m2m::doe { struct StateVarState; struct SessionNotificationState; class StudyIo; }

namespace m2m::engine {
struct DbHandle;
struct Display;
struct GuiModel;
class ModelManager;
class PostGraphManagerState;

class PostGraphSceneItemState {
public:
    virtual ~PostGraphSceneItemState() = default;
protected:
    PostGraphSceneItemState() = default;
};

// Concrete c353f0 object identity. Native text-template+38 is separate from the
// QGraphicsTextItem's displayed text. Font/metrics/Qt events are not projected.
class SceneTextItemState final : public PostGraphSceneItemState {
public:
    qt::String textTemplate;
    qt::String displayedText;
    bool hidden = false;
    const std::weak_ptr<Display>& displayOwner() const noexcept { return display_; }
private:
    friend class PostGraphSceneState;
    SceneTextItemState(const std::shared_ptr<Display>&, const qt::String&);
    std::weak_ptr<Display> display_; //native+30 is nonowning
};

// Identity of the actual mainwindow+778 scene that embeds Display at+10.
// Creating this adapter binding does not construct a native QGraphicsScene or
// establish its item population. The session must supply its actual Display.
class PostGraphSceneState final {
public:
    explicit PostGraphSceneState(std::shared_ptr<Display> display);
    const std::shared_ptr<Display>& displayOwner() const noexcept { return display_; }
    // Explicit cca680 text allocation/add/hide prefix; identity binding above
    // does not execute this. These objects remain owned during no-match queries.
    void commitInitialTextItems();
    SceneTextItemState* databaseNameItem() noexcept { return databaseNameItem_; }
    SceneTextItemState* stepNumberItem() noexcept { return stepNumberItem_; }
    const auto& items() const noexcept { return items_; }
    // Coverage concerns the actual current collection, not a default fresh-scene
    // assumption. Unknown mutations invalidate coverage without erasing items.
    void qualifyCurrentItemPopulation(std::string);
    void invalidateItemPopulation() noexcept;
    const std::string& itemPopulationQualification() const noexcept { return itemQualification_; }
    bool hasHistogramChart() const;
    // Explicit current selection observation on this same scene. This binds
    // actual item identities and raw native depth bits; it does not run a Qt
    // constructor or select items. Empty qualification leaves coverage unknown.
    void observeCurrentSelection(std::unordered_set<const PostGraphSceneItemState*>,
                                 std::uint32_t depth, std::string qualification);
    void invalidateCurrentSelection() noexcept;
    const auto& currentSelectedItems() const noexcept { return selectedItems_; }
    const auto& selectionChangeDepth() const noexcept { return selectionDepth_; }
    const std::string& selectionQualification() const noexcept { return selectionQualification_; }
    bool clearSelectionPending() const noexcept { return clearSelectionPending_; }
    // 65571720: increment depth, snapshot selected identities, clear set, then
    // decrement. Only a qualified empty set is closed; nonempty item callbacks
    // remain guarded after the increment. An adapter suspension cannot replay it.
    void clearSelection();
    // c0c650 walks all current items and deletes only its seven graph/chart
    // types. Represented text items survive; unknown graph effects guard.
    void removeGraphItems(PostGraphManagerState&);
private:
    std::shared_ptr<Display> display_;
    std::vector<std::unique_ptr<PostGraphSceneItemState>> items_;
    SceneTextItemState* databaseNameItem_ = nullptr; //scene+c98, owned in items_
    SceneTextItemState* stepNumberItem_ = nullptr; //scene+ca0, owned in items_
    bool textCommitStarted_ = false, textCommitFinished_ = false; //adapter boundary
    std::string itemQualification_; //empty means current coverage unavailable
    std::unordered_set<const PostGraphSceneItemState*> selectedItems_; //Qt private+108
    std::optional<std::uint32_t> selectionDepth_; //Qt private+100; unknown initially
    std::string selectionQualification_; //independent of item population coverage
    bool clearSelectionPending_ = false; //adapter suspension, not a native field
};

// Resolved scene result from a qualified nonnull global mainwindow. A qualified
// null scene is permitted by045260; missing binding is unavailable knowledge.
struct PostGraphSceneBinding {
    std::shared_ptr<PostGraphSceneState> owner;
    std::string qualification;
};

struct PostGraphItemState; // populated native graph objects remain unsupported

class PostGraphFactoryState final {
public:
    enum class Kind { Base, Post };
    Kind kind() const noexcept { return kind_; }
    const PostGraphManagerState* manager() const noexcept { return manager_; }
    const std::weak_ptr<DbHandle>& modelOwner() const noexcept { return modelOwner_; }
private:
    friend class PostGraphManagerState;
    PostGraphFactoryState(Kind, const PostGraphManagerState*, const std::shared_ptr<DbHandle>&);
    Kind kind_;
    const PostGraphManagerState* manager_;
    std::weak_ptr<DbHandle> modelOwner_;
};

// SfPstGraphManager at the actual SfModelBase/SfMoModel child's+1970. DbHandle
// represents that same object's DB fields; it is not the outer ModelManager.
// The collection is a real initially-empty guarded list. No public insertion
// path is provided until graph object ownership/destructors are recovered.
class PostGraphManagerState final {
public:
    ~PostGraphManagerState();
    const std::weak_ptr<DbHandle>& modelOwner() const noexcept { return modelOwner_; }
    const std::weak_ptr<PostGraphSceneState>& sceneOwner() const noexcept { return scene_; }
    const PostGraphFactoryState* factory() const noexcept { return factory_.get(); }
    const auto& graphItems() const noexcept { return graphItems_; }
    int field30 = 0, field34 = 0;
    bool flag38 = false;
private:
    friend std::shared_ptr<PostGraphManagerState> ensurePostGraphManager(
        const std::shared_ptr<DbHandle>&, const std::optional<PostGraphSceneBinding>&,
        doe::SessionNotificationState&);
    PostGraphManagerState(const std::shared_ptr<DbHandle>&,
                         std::shared_ptr<PostGraphSceneState>);
    std::weak_ptr<DbHandle> modelOwner_;
    std::weak_ptr<PostGraphSceneState> scene_; // native+28 does not own the scene
    std::vector<std::weak_ptr<PostGraphItemState>> graphItems_; // native+10
    std::unique_ptr<PostGraphFactoryState> factory_; // native+18
};

// cdd8e0 -> actual Mo factory045260. Cached retrieval precedes construction-only
// qualifiers. The caller must resolve the actual current model through the
// ModelManager/Project/subproject chain; this helper does not infer it.
std::shared_ptr<PostGraphManagerState> ensurePostGraphManager(
    const std::shared_ptr<DbHandle>&, const std::optional<PostGraphSceneBinding>&,
    doe::SessionNotificationState&);

// Actual current global scene results for c0cfe0, independently of a manager's
// constructor scene cache. Both getters precede either item traversal. Missing
// qualification is unavailable knowledge; a null scene is a valid result, but
// a null mainwindow would be dereferenced by the native receiver and guards.
struct PostGraphWindowBinding {
    bool mainWindowPresent = false;
    std::optional<PostGraphSceneBinding> primaryScene;
    std::optional<PostGraphSceneBinding> secondaryScene;
    std::string qualification;
};

// Complete 2cf3c0 suppression/empty/expired-only branches for this key, followed
// by qualified live-scene traversal. The unconditional HideSecondary dispatcher
// remains guarded. Neither entry executes accepted Close or infers fresh state.
void removeGraphItems(doe::SessionNotificationState&);
void removeGraphItems(doe::SessionNotificationState&,
                      const std::optional<PostGraphWindowBinding>&);

// d16530 notify=true invokes this after its variable/component stores and before
// rendering. Implementations execute receiver effects, not diagnostic counters.
struct StateVarPlotEffects {
    virtual ~StateVarPlotEffects() = default;
    virtual void changePtrkGraph(GuiModel&, doe::StateVarState&) = 0;
    // d17b90 emits after its type/title/range prefix, before d189b0/legend.
    // First-only implementations retain an exact refusal at this second hook.
    virtual void changeHistoGraph(GuiModel&, doe::StateVarState&);
};

// SfPtrk (3d55c0): this projection owns the object whose identity is cached at
// manager+4c8 and whose +f1 flag is read by321660. The new object's render child
// +d0 is null; populated render/selection payloads are not yet supported.
// Other constructor fields are retained in the evidence contract and are not
// read before this implementation's nonzero-variable graph-update guard.
struct PtrkGraphState final {
    bool graphOn = false; // graph+f1, distinct from manager+4d0
};

// SfPtrkManager (309cc0), lazily owned by the original data root at +1978.
// This is its actual projected state, not a Close-time copy of its flags.
// A nonnull parent initializes +4d0=false before registering the receiver.
// Construction here does not claim the selected Open path created a manager.
class PtrkGraphManagerState final {
public:
    explicit PtrkGraphManagerState(const std::shared_ptr<DbHandle>& owner);
    ~PtrkGraphManagerState();
    const std::weak_ptr<DbHandle>& owner() const noexcept { return owner_; }
    bool graphOn = false; // +4d0; caller must preserve intervening native changes
    PtrkGraphState* cachedGraph() noexcept { return graph_.get(); }
    const PtrkGraphState* cachedGraph() const noexcept { return graph_.get(); }
    // 321520 deletes and clears only the cached graph; it does not reset +4d0.
    void clearCachedGraph() noexcept;

private:
    friend class CloseEffects;
    std::weak_ptr<DbHandle> owner_;
    std::unique_ptr<PtrkGraphState> graph_; // 321500 initializes +4c8=null
};

// Explicit ordered receiver population. An observed empty list is different
// from unavailable knowledge. Qualification identifies its evidence/profile;
// supplying one does not establish that the original application was observed.
struct PtrkGraphRegistrations {
    std::string qualification;
    std::vector<std::weak_ptr<PtrkGraphManagerState>> receivers;
};

// 30a2b0 -> global mainwindow -> scene+10 Display. This lease must identify that
// actual owner, not the emitting model's backlink or the manager's data root.
// A qualified null owner represents an explicitly absent mainwindow/scene;
// unavailable binding is nullopt. Global owner replacement requires rebinding.
struct PtrkGraphDisplayBinding {
    std::shared_ptr<Display> owner;
    std::string qualification;
};

// Shared lease keeps the actual StudyIo/runtime registry alive across Project
// storage release. Receiver ownership remains StateVar's; registrations are weak.
struct HistogramRegistrations {
    std::shared_ptr<doe::StudyIo> owner;
    std::string qualification;
};

// Qualified global mainwindow context for4122f0. Its actual ModelManager is
// shared with StudyIo; current selection is resolved live, never copied by ID.
// A present window may have a qualified null primary scene. Secondary item
// queries require current coverage, including the two initial hidden text items.
struct HistogramGraphBinding {
    bool mainWindowPresent = false;
    std::shared_ptr<ModelManager> modelManager;
    std::optional<PostGraphSceneBinding> primaryScene;
    std::string qualification;
    std::optional<PostGraphSceneBinding> secondaryScene;
};

// These receiver methods require the shared notifier's +90/+141 gates to have
// been handled by lifecycle integration. No duplicate suppression flags here.
// Source-qualified subset of signalChangePtrkGraph delivery. Registration
// lifetime and live receiver state are real. Enabled managers allocate/register
// their cache before the graph flag and selected-model StateVar checks.
class CloseEffects final : public StateVarPlotEffects {
public:
    // nullopt restores unavailable knowledge. Neither this method nor dispatch
    // creates a manager, infers fresh absence, or changes any graph flag.
    void setPtrkGraphRegistrations(std::optional<PtrkGraphRegistrations>);
    const std::optional<PtrkGraphRegistrations>& ptrkGraphRegistrations() const noexcept {
        return ptrkGraphRegistrations_;
    }
    void setPtrkGraphDisplayBinding(std::optional<PtrkGraphDisplayBinding>);
    const std::optional<PtrkGraphDisplayBinding>& ptrkGraphDisplayBinding() const noexcept {
        return ptrkGraphDisplayBinding_;
    }
    void changePtrkGraph(GuiModel&, doe::StateVarState&) override;
    void setHistogramRegistrations(std::optional<HistogramRegistrations>);
    const std::optional<HistogramRegistrations>& histogramRegistrations() const noexcept {
        return histogramRegistrations_;
    }
    void setHistogramGraphBinding(std::optional<HistogramGraphBinding>);
    const std::optional<HistogramGraphBinding>& histogramGraphBinding() const noexcept {
        return histogramGraphBinding_;
    }
    void changeHistoGraph(GuiModel&, doe::StateVarState&) override;

private:
    std::optional<PtrkGraphRegistrations> ptrkGraphRegistrations_;
    std::optional<PtrkGraphDisplayBinding> ptrkGraphDisplayBinding_;
    std::optional<HistogramRegistrations> histogramRegistrations_;
    std::optional<HistogramGraphBinding> histogramGraphBinding_;
};
} // namespace m2m::engine
