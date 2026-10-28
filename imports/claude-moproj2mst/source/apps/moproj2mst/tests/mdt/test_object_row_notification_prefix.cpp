// ctest-labels: m2m-mdt object-row-notification-prefix
#include <testkit/check.hpp>

#include <memory>
#include <string>
#include <type_traits>
#include <vector>

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
struct BackendReads : ObjectRowBackendReadDriver {
    RowFixture& fixture;
    std::vector<ObjectRowBackendRead> supplied;
    std::vector<std::uint64_t> instructions;
    explicit BackendReads(RowFixture& f) : fixture(f) {}
    ObjectRowBackendRead readCurrentBackend(const ObjectRowBackendReadContext& context) override
    {
        CHECK(&context.session == &fixture.session);
        CHECK(&context.modelOwner == fixture.session.project().subs.back().get());
        CHECK(&context.source == fixture.item && &context.target == fixture.target);
        CHECK(context.rowSlot == 0);
        auto& input = *fixture.target->objectRowPorts.at(0).input;
        auto& output = *fixture.item->objectRowPorts.at(0).output;
        if (instructions.empty()) {
            CHECK(context.instruction == 0x140075ebdULL);
            CHECK(input.peers.empty() && output.peers.empty());
        } else {
            CHECK(context.instruction == 0x14007615aULL);
            CHECK(input.peers == std::vector<ObjectRowPort*>{&output});
            CHECK(output.peers == std::vector<ObjectRowPort*>{&input});
        }
        const auto index = instructions.size();
        instructions.push_back(context.instruction);
        if (index >= supplied.size()) return {};
        return supplied[index];
    }
};
ObjectRowBackendConnectionResult runBackendPrefix(RowFixture& f, ObjectRowBackendReadDriver& driver)
{
    return attemptObjectRowConnectionWithBackendPrefix(f.session, *f.item, *f.target,
        0, "0x140075e7c:row branch after main link", "private reached row branch fixture", driver);
}
}

static_assert(!std::is_copy_constructible_v<ObjectRowNotificationProgress>);
static_assert(!std::is_move_constructible_v<ObjectRowNotificationProgress>);
static_assert(!ObjectRowNotificationResult::completeForPublicGraph);
static_assert(!ObjectRowBackendConnectionResult::completeForPublicGraph);

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

TEST_CASE(object_row_backend_null_then_nonnull_enters_created_only_after_link)
{
    RowFixture f;
    auto owner = f.session.sessionNotificationStateOwner();
    owner->suppressed = false; owner->flag90 = true;
    BackendReads reads(f);
    reads.supplied = {{static_cast<const void*>(nullptr), "current null at Destroyed2 gate"},
                      {static_cast<const void*>(&f), "current backend at Created2 gate"}};
    const auto result = runBackendPrefix(f, reads);
    CHECK(result.stage == ObjectRowBackendConnectionStage::CreatedKeyUnavailable);
    CHECK(!result.destroyed && result.created.has_value());
    REQUIRE(result.created.has_value());
    CHECK(result.created->flag90StoredThisAttempt && result.created->suppressionReadThisAttempt);
    REQUIRE(result.linkReturnByte.has_value());
    CHECK(*result.linkReturnByte == 1 && result.linked);
    CHECK(result.beforeCreated.has_value() && reads.instructions.size() == 2);
    CHECK(!owner->flag90 && owner->attempts.empty());
}

TEST_CASE(object_row_backend_nonnull_then_null_skips_created_without_reusing_first_read)
{
    RowFixture f;
    auto owner = f.session.sessionNotificationStateOwner();
    owner->suppressed = true; owner->flag90 = true;
    BackendReads reads(f);
    reads.supplied = {{static_cast<const void*>(&f), "current backend at Destroyed2 gate"},
                      {static_cast<const void*>(nullptr), "current null at Created2 gate"}};
    const auto result = runBackendPrefix(f, reads);
    CHECK(result.stage == ObjectRowBackendConnectionStage::Complete);
    REQUIRE(result.destroyed.has_value());
    CHECK(result.destroyed->stage == ObjectRowNotificationStage::SuppressedCalleeReturn);
    CHECK(!result.created && result.linked && reads.instructions.size() == 2);
    CHECK(owner->flag90 && owner->attempts.empty());
}

TEST_CASE(object_row_backend_unknown_after_link_retains_link_without_created_entry)
{
    RowFixture f;
    auto owner = f.session.sessionNotificationStateOwner();
    owner->suppressed = false; owner->flag90 = true;
    BackendReads reads(f);
    reads.supplied = {{static_cast<const void*>(nullptr), "current null at Destroyed2 gate"}};
    const auto result = runBackendPrefix(f, reads);
    CHECK(result.stage == ObjectRowBackendConnectionStage::CreatedBackendUnavailable);
    CHECK(!result.destroyed && !result.created && result.linked);
    REQUIRE(result.beforeCreated.has_value());
    CHECK(!result.beforeCreated->backend && reads.instructions.size() == 2);
    CHECK(owner->flag90 && owner->attempts.empty());
    CHECK(f.target->objectRowPorts.at(0).input->peers.size() == 1);
    CHECK(f.item->objectRowPorts.at(0).output->peers.size() == 1);
}

TEST_CASE(object_row_backend_unknown_first_read_retains_clears_without_dispatch_or_link)
{
    RowFixture f;
    auto owner = f.session.sessionNotificationStateOwner();
    owner->flag90 = true;
    auto& input = *f.target->objectRowPorts.at(0).input;
    auto& output = *f.item->objectRowPorts.at(0).output;
    linkObjectRowPorts(output, input);
    ObjectRowBackendReadDriver unknown;
    const auto result = runBackendPrefix(f, unknown);
    CHECK(result.stage == ObjectRowBackendConnectionStage::DestroyedBackendUnavailable);
    CHECK(!result.beforeDestroyed.backend && !result.beforeCreated);
    CHECK(!result.destroyed && !result.created && !result.linkReturnByte && !result.linked);
    CHECK(input.peers.empty() && output.peers.empty());
    CHECK(owner->flag90 && owner->attempts.empty());
}

TEST_CASE(object_row_backend_null_both_gates_bypasses_unsuppressed_dispatcher)
{
    RowFixture f;
    auto owner = f.session.sessionNotificationStateOwner();
    owner->suppressed = false; owner->flag90 = true;
    owner->emptyKeys.insert("fixture-key");
    BackendReads reads(f);
    reads.supplied = {{static_cast<const void*>(nullptr), "first current null"},
                      {static_cast<const void*>(nullptr), "second current null"}};
    const auto result = runBackendPrefix(f, reads);
    CHECK(result.stage == ObjectRowBackendConnectionStage::Complete);
    CHECK(!result.destroyed && !result.created && result.linked);
    CHECK(reads.instructions.size() == 2 && owner->flag90);
    CHECK(owner->emptyKeys.count("fixture-key") == 1 && owner->attempts.empty());
}

TEST_CASE(object_row_backend_unqualified_value_and_unsuppressed_nonnull_stop_before_link)
{
    RowFixture f;
    auto owner = f.session.sessionNotificationStateOwner();
    owner->suppressed = false; owner->flag90 = true;
    BackendReads unqualified(f);
    unqualified.supplied = {{static_cast<const void*>(nullptr), " \t"}};
    const auto rejected = runBackendPrefix(f, unqualified);
    CHECK(rejected.stage == ObjectRowBackendConnectionStage::DestroyedBackendUnavailable);
    CHECK(owner->flag90 && !rejected.linked);
    BackendReads reads(f);
    reads.supplied = {{static_cast<const void*>(&f), "current nonnull first gate"}};
    const auto result = runBackendPrefix(f, reads);
    CHECK(result.stage == ObjectRowBackendConnectionStage::DestroyedKeyUnavailable);
    CHECK(result.destroyed.has_value() && !result.created && !result.beforeCreated);
    CHECK(!result.linked && !result.linkReturnByte && reads.instructions.size() == 1);
    CHECK(!owner->flag90 && owner->attempts.empty());
}

TEST_CASE(object_row_backend_observer_peer_mutation_refuses_at_either_read)
{
    struct MutatingReads : BackendReads {
        bool second;
        MutatingReads(RowFixture& f, bool atSecond) : BackendReads(f), second(atSecond)
        {
            supplied = {{static_cast<const void*>(nullptr), "first current null"},
                        {static_cast<const void*>(nullptr), "second current null"}};
        }
        ObjectRowBackendRead readCurrentBackend(const ObjectRowBackendReadContext& context) override
        {
            auto value = BackendReads::readCurrentBackend(context);
            if ((context.instruction == 0x14007615aULL) == second) {
                auto& input = *fixture.target->objectRowPorts.at(0).input;
                auto& output = *fixture.item->objectRowPorts.at(0).output;
                if (second) clearObjectRowPort(input);
                else linkObjectRowPorts(output, input);
            }
            return value;
        }
    };
    for (bool second : {false, true}) {
        RowFixture f;
        auto owner = f.session.sessionNotificationStateOwner();
        owner->suppressed = false; owner->flag90 = true;
        MutatingReads reads(f, second);
        CHECK(refused([&] { runBackendPrefix(f, reads); }));
        CHECK(reads.instructions.size() == (second ? 2 : 1));
        // Refusal preserves the observer's reached mutation; it neither
        // reports Complete nor rolls back an already changed endpoint.
        auto& input = *f.target->objectRowPorts.at(0).input;
        auto& output = *f.item->objectRowPorts.at(0).output;
        CHECK(input.peers.size() == (second ? 0 : 1));
        CHECK(output.peers.size() == (second ? 0 : 1));
        CHECK(owner->flag90 && owner->attempts.empty());
    }
}

int main() { return testkit::run_all(); }
