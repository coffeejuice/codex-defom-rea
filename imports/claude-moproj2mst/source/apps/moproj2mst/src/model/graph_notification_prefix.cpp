#include "m2m/model_graph_notification_prefix.hpp"

#include <algorithm>
#include <utility>

#include "m2m/common.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/model.hpp"

namespace m2m {
namespace {
[[noreturn]] void unavailable(const char* detail)
{
    throw Error(M2M_E_UNSUPPORTED, std::string("object-row notification prefix: ") + detail);
}
bool nonblank(const std::string& s)
{
    return s.find_first_not_of(" \t\r\n") != std::string::npos;
}
const char* occurrence(ObjectRowNotificationKind kind)
{
    return kind == ObjectRowNotificationKind::Destroyed2 ?
        "0x140076101:Destroyed2(row input)->0x1402cf3c0" :
        "0x1400763ae:Created2(row input)->0x1402cf3c0";
}
const char* signal(ObjectRowNotificationKind kind)
{
    return kind == ObjectRowNotificationKind::Destroyed2 ?
        "2signalDiagEditorConnectionDestroyed2()" :
        "2signalDiagEditorConnectionCreated2()";
}
bool owns(const engine::ConversionSession& session, const FlowItem* target)
{
    // Compare addresses before dereference: a released diagram item cannot
    // be resurrected by a retained diagnostic pointer.
    auto& mutableSession = const_cast<engine::ConversionSession&>(session);
    const Project* project = mutableSession.liveProject();
    if (!project) return false;
    for (const auto& sub : project->subs)
        if (sub && std::any_of(sub->flow.items.begin(), sub->flow.items.end(),
            [target](const auto& item) { return item.get() == target; })) return true;
    return false;
}
ObjectRowPort* inputOf(FlowItem& target, std::size_t slot)
{
    const auto it = target.objectRowPorts.find(slot);
    if (it == target.objectRowPorts.end() || !it->second.input) return nullptr;
    return &*it->second.input;
}
}

ObjectRowNotificationProgress::ObjectRowNotificationProgress(
    engine::ConversionSession& session, FlowItem& target, std::size_t rowSlot,
    ObjectRowNotificationKind kind, std::string sourceOccurrence,
    std::string qualification)
    : sessionIdentity_(reinterpret_cast<std::uintptr_t>(&session)),
      target_(&target), input_(nullptr), rowSlot_(rowSlot), kind_(kind)
{
    if (sourceOccurrence != occurrence(kind) || !nonblank(qualification))
        unavailable("reached emitter occurrence is unqualified");
    auto owner = session.sessionNotificationStateOwner();
    if (!owner || !owns(session, &target)) unavailable("actual Session/row owner unavailable");
    input_ = inputOf(target, rowSlot);
    if (!input_ || input_->owner != &target ||
        input_->direction != ObjectRowPortDirection::Input ||
        input_->type != qt::L("Object2d"))
        unavailable("stable target row input unavailable");
    owner_ = owner;
    rowLifetime_ = target.diagnosticLifetime;
}

ObjectRowNotificationResult attemptObjectRowNotificationPrefix(
    engine::ConversionSession& session, ObjectRowNotificationProgress& progress)
{
    const auto actual = session.sessionNotificationStateOwner();
    const auto recorded = progress.owner_.lock();
    const auto rowLifetime = progress.rowLifetime_.lock();
    if (progress.sessionIdentity_ != reinterpret_cast<std::uintptr_t>(&session) ||
        !actual || !recorded || actual.get() != recorded.get() ||
        actual.owner_before(recorded) || recorded.owner_before(actual) ||
        !rowLifetime ||
        !owns(session, progress.target_) ||
        progress.target_->diagnosticLifetime.get() != rowLifetime.get() ||
        inputOf(*progress.target_, progress.rowSlot_) != progress.input_)
        unavailable("progress does not bind the live Session/row input");
    bool read = false, stored = false;
    if (progress.stage_ == ObjectRowNotificationStage::AwaitSuppression) {
        read = true;
        if (actual->suppressed)
            progress.stage_ = ObjectRowNotificationStage::SuppressedCalleeReturn;
        else {
            actual->flag90 = false; // native 2cf408 before key/map/list work
            stored = true;
            progress.stage_ = ObjectRowNotificationStage::BeforeKeyQStringConstruction;
        }
    }
    return {progress.stage_, signal(progress.kind_), progress.input_, read, stored};
}

namespace {
struct RowEndpoints { ObjectRowPort& output; ObjectRowPort& input; };
RowEndpoints admitRowConnection(
    engine::ConversionSession& session, FlowItem& source, FlowItem& target,
    std::size_t rowSlot, const std::string& sourceOccurrence, const std::string& qualification)
{
    if (sourceOccurrence != "0x140075e7c:row branch after main link" ||
        !nonblank(qualification) || !owns(session, &source) || !owns(session, &target) ||
        source.next != &target || target.prev != &source ||
        source.className != qt::L("SfOprForming2D") ||
        target.className != qt::L("SfOprForming2D") ||
        rowSlot > static_cast<std::size_t>(-1) - 7 ||
        target.expansion.size() <= 7 + rowSlot ||
        target.expansion[7 + rowSlot].label != qt::L("  Read From DB"))
        unavailable("reached row branch or owned main edge is unqualified");
    const auto sourceIt = source.objectRowPorts.find(rowSlot);
    const auto targetIt = target.objectRowPorts.find(rowSlot);
    if (sourceIt == source.objectRowPorts.end() ||
        targetIt == target.objectRowPorts.end() ||
        !sourceIt->second.output || !targetIt->second.input)
        unavailable("required stable row connectors are absent");
    ObjectRowPort& output = *sourceIt->second.output;
    ObjectRowPort& input = *targetIt->second.input;
    if (output.owner != &source || input.owner != &target ||
        output.direction != ObjectRowPortDirection::Output ||
        input.direction != ObjectRowPortDirection::Input ||
        output.type != qt::L("Object2d") || input.type != qt::L("Object2d") ||
        output.capacity != 1 || input.capacity != 1)
        unavailable("Object2d row connector admission is unqualified");
    return {output, input};
}

std::uint8_t linkClearedRowPorts(ObjectRowPort& output, ObjectRowPort& input)
{
    if (!input.peers.empty() || !output.peers.empty())
        unavailable("row endpoints changed after Destroyed2 return");
    output.peers.push_back(&input);
    input.peers.push_back(&output);
    return 1; // admitted native 161e94 normal reciprocal-link return
}
} // namespace

ObjectRowConnectionPrefixResult attemptObjectRowConnectionPrefix(
    engine::ConversionSession& session, FlowItem& source, FlowItem& target,
    std::size_t rowSlot, std::string sourceOccurrence, std::string qualification)
{
    auto [output, input] = admitRowConnection(session, source, target, rowSlot,
                                             sourceOccurrence, qualification);

    clearObjectRowPort(input);  // 075e7c target first
    clearObjectRowPort(output); // 075eaf source second
    ObjectRowNotificationProgress destroyed(session, target, rowSlot,
        ObjectRowNotificationKind::Destroyed2,
        occurrence(ObjectRowNotificationKind::Destroyed2), qualification);
    const auto destroyedResult = attemptObjectRowNotificationPrefix(session, destroyed);
    if (destroyedResult.stage == ObjectRowNotificationStage::BeforeKeyQStringConstruction)
        return {ObjectRowConnectionPrefixStage::DestroyedKeyUnavailable,
                &input, &output, destroyedResult, false, false};

    // 076148: the two already-cleared capacity-one endpoints link without
    // another clear or intervening callback. The suppressed dispatcher returned.
    linkClearedRowPorts(output, input);
    ObjectRowNotificationProgress created(session, target, rowSlot,
        ObjectRowNotificationKind::Created2,
        occurrence(ObjectRowNotificationKind::Created2), qualification);
    const auto createdResult = attemptObjectRowNotificationPrefix(session, created);
    if (createdResult.stage == ObjectRowNotificationStage::BeforeKeyQStringConstruction)
        return {ObjectRowConnectionPrefixStage::CreatedKeyUnavailable,
                &input, &output, destroyedResult, true, true};
    return {ObjectRowConnectionPrefixStage::CompleteSuppressedEvents,
            &input, &output, destroyedResult, true, true};
}

ObjectRowBackendConnectionResult attemptObjectRowConnectionWithBackendPrefix(
    engine::ConversionSession& session, FlowItem& source, FlowItem& target,
    std::size_t rowSlot, std::string sourceOccurrence, std::string qualification,
    ObjectRowBackendReadDriver& driver)
{
    auto [output, input] = admitRowConnection(session, source, target, rowSlot,
                                             sourceOccurrence, qualification);
    const Project* project = session.liveProject();
    const SubProject* modelOwner = nullptr;
    for (const auto& sub : project->subs) {
        if (!sub) continue;
        const auto contains = [&](const FlowItem* item) {
            return std::any_of(sub->flow.items.begin(), sub->flow.items.end(),
                [item](const auto& entry) { return entry.get() == item; });
        };
        if (contains(&source) && contains(&target)) { modelOwner = sub.get(); break; }
    }
    const auto notifier = session.sessionNotificationStateOwner();
    if (!modelOwner || modelOwner->project != project || !notifier)
        unavailable("backend gates require the same live model and notifier owner");
    const std::weak_ptr<int> sourceLifetime = source.diagnosticLifetime;
    const std::weak_ptr<int> targetLifetime = target.diagnosticLifetime;
    const auto requireBinding = [&] {
        const auto sourceAlive = sourceLifetime.lock();
        const auto targetAlive = targetLifetime.lock();
        const auto currentNotifier = session.sessionNotificationStateOwner();
        if (!sourceAlive || !targetAlive || session.liveProject() != project ||
            currentNotifier != notifier || currentNotifier.owner_before(notifier) ||
            notifier.owner_before(currentNotifier) || !owns(session, &source) || !owns(session, &target))
            unavailable("backend read lost its live model/row generation");
        if (source.diagnosticLifetime.get() != sourceAlive.get() ||
            target.diagnosticLifetime.get() != targetAlive.get())
            unavailable("backend read changed its row generation");
        const auto ports = admitRowConnection(session, source, target, rowSlot,
                                              sourceOccurrence, qualification);
        if (&ports.input != &input || &ports.output != &output ||
            std::none_of(project->subs.begin(), project->subs.end(),
                [modelOwner](const auto& sub) { return sub.get() == modelOwner; }))
            unavailable("backend read changed its model/port owner");
        const auto contains = [&](const FlowItem* item) {
            return std::any_of(modelOwner->flow.items.begin(), modelOwner->flow.items.end(),
                [item](const auto& entry) { return entry.get() == item; });
        };
        if (modelOwner->project != project || !contains(&source) || !contains(&target))
            unavailable("backend read changed its actual model membership");
    };
    const auto read = [&](std::uint64_t instruction) {
        requireBinding();
        const auto inputPeers = input.peers;
        const auto outputPeers = output.peers;
        auto value = driver.readCurrentBackend({session, *modelOwner, source, target, rowSlot, instruction});
        requireBinding();
        if (input.peers != inputPeers || output.peers != outputPeers)
            unavailable("backend observation changed reciprocal row peers");
        if (!nonblank(value.qualification)) value.backend.reset();
        return value;
    };

    clearObjectRowPort(input);  // 075e7c
    clearObjectRowPort(output); // 075eaf
    ObjectRowBackendConnectionResult result{
        ObjectRowBackendConnectionStage::DestroyedBackendUnavailable, &input, &output,
        read(0x140075ebdULL), {}, {}, {}, {}, false};
    if (!result.beforeDestroyed.backend) return result;
    if (*result.beforeDestroyed.backend) {
        ObjectRowNotificationProgress progress(session, target, rowSlot,
            ObjectRowNotificationKind::Destroyed2, occurrence(ObjectRowNotificationKind::Destroyed2), qualification);
        result.destroyed = attemptObjectRowNotificationPrefix(session, progress);
        if (result.destroyed->stage == ObjectRowNotificationStage::BeforeKeyQStringConstruction) {
            result.stage = ObjectRowBackendConnectionStage::DestroyedKeyUnavailable;
            return result;
        }
    }
    result.linkReturnByte = linkClearedRowPorts(output, input); // 076148, saved07614b
    result.linked = true;
    result.beforeCreated = read(0x14007615aULL); // must precede return-byte test076165
    if (!result.beforeCreated->backend) {
        result.stage = ObjectRowBackendConnectionStage::CreatedBackendUnavailable;
        return result;
    }
    if (*result.beforeCreated->backend && *result.linkReturnByte != 0) {
        ObjectRowNotificationProgress progress(session, target, rowSlot,
            ObjectRowNotificationKind::Created2, occurrence(ObjectRowNotificationKind::Created2), qualification);
        result.created = attemptObjectRowNotificationPrefix(session, progress);
        if (result.created->stage == ObjectRowNotificationStage::BeforeKeyQStringConstruction) {
            result.stage = ObjectRowBackendConnectionStage::CreatedKeyUnavailable;
            return result;
        }
    }
    result.stage = ObjectRowBackendConnectionStage::Complete;
    return result;
}
} // namespace m2m
