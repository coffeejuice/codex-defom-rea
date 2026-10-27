// ctest-labels: m2m-mdt object-row-notification-prefix
#include <testkit/check.hpp>

#include <memory>
#include <string>
#include <type_traits>

#include "m2m/common.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/model.hpp"
#include "m2m/model_graph_notification_prefix.hpp"

using namespace m2m;
namespace {
struct Fixture {
    engine::DiagramEffectDriver effects;
    engine::ConversionSession session{effects};
    FlowItem* item = nullptr;
    Fixture()
    {
        Project& p = session.project();
        p.subs.push_back(makeSubProject(p, 0, 0));
        auto row = std::make_unique<FlowItem>();
        row->className = String(u"SfOprForming2D");
        row->expansion.resize(8);
        item = row.get();
        p.subs.back()->flow.items.push_back(std::move(row));
        ensureObjectRowPorts(*item, 0);
    }
};
struct RowFixture : Fixture {
    FlowItem* target = nullptr;
    RowFixture()
    {
        auto row = std::make_unique<FlowItem>();
        row->className = String(u"SfOprForming2D");
        row->expansion.resize(8);
        row->expansion[7].label = String(u"  Read From DB");
        target = row.get();
        session.project().subs.back()->flow.items.push_back(std::move(row));
        ensureObjectRowPorts(*target, 0);
        item->next = target;
        target->prev = item;
    }
};
bool refused(auto&& body)
{
    try { body(); }
    catch (const Error& e) { return e.code == M2M_E_UNSUPPORTED; }
    return false;
}
constexpr const char* destroyedOccurrence =
    "0x140076101:Destroyed2(row input)->0x1402cf3c0";
constexpr const char* createdOccurrence =
    "0x1400763ae:Created2(row input)->0x1402cf3c0";
}

static_assert(!std::is_copy_constructible_v<ObjectRowNotificationProgress>);
static_assert(!std::is_move_constructible_v<ObjectRowNotificationProgress>);
static_assert(!ObjectRowNotificationResult::completeForPublicGraph);

TEST_CASE(object_row_notification_suppressed_preserves_actual_owner_and_retries)
{
    Fixture f;
    auto owner = f.session.sessionNotificationStateOwner();
    owner->suppressed = true; owner->flag90 = true;
    owner->emptyKeys.insert("fixture-key");
    const std::size_t initialAttempts = owner->attempts.size();
    ObjectRowPort* port = &*f.item->objectRowPorts.at(0).input;
    ObjectRowNotificationProgress progress(f.session, *f.item, 0,
        ObjectRowNotificationKind::Destroyed2, destroyedOccurrence,
        "private reached row emitter fixture");
    CHECK(progress.stage() == ObjectRowNotificationStage::AwaitSuppression);
    CHECK(owner->flag90);
    const auto first = attemptObjectRowNotificationPrefix(f.session, progress);
    CHECK(first.stage == ObjectRowNotificationStage::SuppressedCalleeReturn);
    CHECK(first.suppressionReadThisAttempt && !first.flag90StoredThisAttempt);
    CHECK(std::string(first.signal) == "2signalDiagEditorConnectionDestroyed2()");
    CHECK(first.targetInput == port);
    CHECK(owner->flag90 && owner->emptyKeys.count("fixture-key") == 1);
    CHECK(owner->attempts.size() == initialAttempts && port->peers.empty());
    owner->suppressed = false;
    const auto retry = attemptObjectRowNotificationPrefix(f.session, progress);
    CHECK(retry.stage == first.stage);
    CHECK(!retry.suppressionReadThisAttempt && !retry.flag90StoredThisAttempt);
    CHECK(owner->flag90 && owner->attempts.size() == initialAttempts);
}

TEST_CASE(object_row_notification_unsuppressed_stops_before_key_and_retains_reset)
{
    for (bool initialFlag : {false, true}) {
        Fixture f;
        auto owner = f.session.sessionNotificationStateOwner();
        owner->suppressed = false; owner->flag90 = initialFlag;
        owner->emptyKeys.insert("fixture-key");
        const std::size_t initialAttempts = owner->attempts.size();
        ObjectRowNotificationProgress progress(f.session, *f.item, 0,
            ObjectRowNotificationKind::Created2, createdOccurrence,
            "private reached row emitter fixture");
        const auto result = attemptObjectRowNotificationPrefix(f.session, progress);
        CHECK(result.stage == ObjectRowNotificationStage::BeforeKeyQStringConstruction);
        CHECK(result.suppressionReadThisAttempt && result.flag90StoredThisAttempt);
        CHECK(std::string(result.signal) == "2signalDiagEditorConnectionCreated2()");
        CHECK(result.targetInput == &*f.item->objectRowPorts.at(0).input);
        CHECK(!owner->flag90 && owner->emptyKeys.size() == 1);
        CHECK(owner->attempts.size() == initialAttempts);
        const auto retry = attemptObjectRowNotificationPrefix(f.session, progress);
        CHECK(!retry.suppressionReadThisAttempt && !retry.flag90StoredThisAttempt);
        CHECK(!owner->flag90 && owner->attempts.size() == initialAttempts);
    }
}

TEST_CASE(object_row_notification_requires_actual_session_and_live_input)
{
    Fixture f, foreign;
    auto owner = f.session.sessionNotificationStateOwner();
    owner->flag90 = true;
    ObjectRowNotificationProgress progress(f.session, *f.item, 0,
        ObjectRowNotificationKind::Destroyed2, destroyedOccurrence,
        "private reached row emitter fixture");
    CHECK(refused([&] { attemptObjectRowNotificationPrefix(foreign.session, progress); }));
    CHECK(owner->flag90);
    CHECK(refused([&] {
        ObjectRowNotificationProgress bad(f.session, *f.item, 0,
            ObjectRowNotificationKind::Created2, destroyedOccurrence,
            "mismatched event occurrence");
    }));
    CHECK(owner->flag90);
    f.session.project().subs.back()->flow.items.clear();
    CHECK(refused([&] { attemptObjectRowNotificationPrefix(f.session, progress); }));
    CHECK(owner->flag90);
}

TEST_CASE(object_row_connection_suppressed_events_link_after_ordered_clears)
{
    RowFixture f;
    auto owner = f.session.sessionNotificationStateOwner();
    owner->suppressed = true; owner->flag90 = true;
    owner->emptyKeys.insert("fixture-key");
    const std::size_t attempts = owner->attempts.size();
    auto& output = *f.item->objectRowPorts.at(0).output;
    auto& input = *f.target->objectRowPorts.at(0).input;
    const auto result = attemptObjectRowConnectionPrefix(f.session, *f.item, *f.target,
        0, "0x140075e7c:row branch after main link", "private reached row branch fixture");
    CHECK(result.stage == ObjectRowConnectionPrefixStage::CompleteSuppressedEvents);
    CHECK(result.targetInput == &input && result.sourceOutput == &output);
    CHECK(result.destroyed.stage == ObjectRowNotificationStage::SuppressedCalleeReturn);
    CHECK(result.destroyed.targetInput == &input);
    CHECK(result.linked && result.createdEntered);
    CHECK(output.peers == std::vector<ObjectRowPort*>{&input});
    CHECK(input.peers == std::vector<ObjectRowPort*>{&output});
    CHECK(owner->flag90 && owner->emptyKeys.count("fixture-key") == 1);
    CHECK(owner->attempts.size() == attempts);
}

TEST_CASE(object_row_connection_unsuppressed_retains_clear_and_flag_prefix)
{
    RowFixture f;
    auto owner = f.session.sessionNotificationStateOwner();
    owner->suppressed = false; owner->flag90 = true;
    owner->emptyKeys.insert("fixture-key");
    auto extra = std::make_unique<FlowItem>();
    extra->className = String(u"SfOprForming2D");
    extra->expansion.resize(8);
    FlowItem* old = extra.get();
    f.session.project().subs.back()->flow.items.push_back(std::move(extra));
    auto& oldOutput = *ensureObjectRowPorts(*old, 0).output;
    auto& input = *f.target->objectRowPorts.at(0).input;
    auto& output = *f.item->objectRowPorts.at(0).output;
    linkObjectRowPorts(oldOutput, input);
    const auto result = attemptObjectRowConnectionPrefix(f.session, *f.item, *f.target,
        0, "0x140075e7c:row branch after main link", "private reached row branch fixture");
    CHECK(result.stage == ObjectRowConnectionPrefixStage::DestroyedKeyUnavailable);
    CHECK(result.destroyed.suppressionReadThisAttempt && result.destroyed.flag90StoredThisAttempt);
    CHECK(!result.linked && !result.createdEntered);
    CHECK(input.peers.empty() && output.peers.empty() && oldOutput.peers.empty());
    CHECK(!owner->flag90 && owner->emptyKeys.count("fixture-key") == 1);
    CHECK(owner->attempts.empty());
}

TEST_CASE(object_row_connection_requires_owned_main_edge_and_exact_target_label)
{
    RowFixture f;
    auto owner = f.session.sessionNotificationStateOwner();
    owner->flag90 = true;
    f.target->expansion[7].label = String(u"  Ordinary");
    CHECK(refused([&] { attemptObjectRowConnectionPrefix(f.session, *f.item, *f.target,
        0, "0x140075e7c:row branch after main link", "fixture"); }));
    CHECK(owner->flag90);
    f.target->expansion[7].label = String(u"  Read From DB");
    f.item->next = nullptr;
    CHECK(refused([&] { attemptObjectRowConnectionPrefix(f.session, *f.item, *f.target,
        0, "0x140075e7c:row branch after main link", "fixture"); }));
    CHECK(owner->flag90);
}

TEST_CASE(object_row_peer_cleanup_and_progress_lifetime_survive_individual_removal)
{
    RowFixture f;
    auto owner = f.session.sessionNotificationStateOwner();
    owner->suppressed = true;
    attemptObjectRowConnectionPrefix(f.session, *f.item, *f.target, 0,
        "0x140075e7c:row branch after main link", "private reached row branch fixture");
    ObjectRowPort& input = *f.target->objectRowPorts.at(0).input;
    REQUIRE(input.peers.size() == 1);
    auto& items = f.session.project().subs.back()->flow.items;
    items.erase(items.begin()); // only source dies; target port stays live
    CHECK(input.peers.empty());

    ObjectRowNotificationProgress progress(f.session, *f.target, 0,
        ObjectRowNotificationKind::Created2, createdOccurrence,
        "private reached row emitter fixture");
    std::weak_ptr<int> lifetime = f.target->diagnosticLifetime;
    items.clear();
    CHECK(lifetime.expired());
    CHECK(refused([&] { attemptObjectRowNotificationPrefix(f.session, progress); }));

    RowFixture reverse;
    reverse.session.sessionNotificationStateOwner()->suppressed = true;
    attemptObjectRowConnectionPrefix(reverse.session, *reverse.item, *reverse.target, 0,
        "0x140075e7c:row branch after main link", "private reached row branch fixture");
    ObjectRowPort& output = *reverse.item->objectRowPorts.at(0).output;
    REQUIRE(output.peers.size() == 1);
    auto& reverseItems = reverse.session.project().subs.back()->flow.items;
    reverseItems.erase(reverseItems.begin() + 1); // only target dies
    CHECK(output.peers.empty());
}

int main() { return testkit::run_all(); }
