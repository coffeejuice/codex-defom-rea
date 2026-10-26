// ctest-labels: m2m-doe
#include <testkit/check.hpp>

#include "m2m/common.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "../../src/doe/flow_receiver_witness.hpp"

using namespace m2m;
using namespace m2m::doe;
namespace {
bool unsupported(const auto& work)
{
    try { work(); }
    catch (const Error& e) { return e.code == M2M_E_UNSUPPORTED; }
    return false;
}
}

TEST_CASE(flow_receiver_witness_explicit_constructor_events_keep_source_and_order)
{
    auto owner = std::make_shared<SessionNotificationState>();
    FlowReceiverWitnessRegistry registry(owner, "isolated constructor-event fixture");
    int firstToken = 0, secondToken = 0, copyToken = 0;
    auto first = registry.recordNormalSubConstruction(&firstToken);
    auto second = registry.recordNormalSubConstruction(&secondToken);
    auto copy = registry.recordCopyConstruction(&copyToken);
    const auto& order = registry.orderedWitnesses();
    REQUIRE(order.size() == 3);
    CHECK(order[0] == first && order[1] == second && order[2] == copy);
    CHECK(first->ordinal() == 0 && second->ordinal() == 1 && copy->ordinal() == 2);
    CHECK(first->source() == FlowReceiverConstructionSource::NormalSub);
    CHECK(copy->source() == FlowReceiverConstructionSource::Copy);
    CHECK(first->registrationSite() == 0x140071166ULL && copy->registrationSite() == 0x140071a7fULL);
    CHECK(first->nativeReceiverAlive() && !first->actualProject() && !first->backendWitness());
    SetOprHasDoeBindings delivery(owner, "isolated delivery owner");
    CHECK(!delivery.registrations); // Witness history does not qualify production population.
    CHECK(registry.owner() == owner && owner->emptyKeys.empty() && owner->attempts.empty());
}

TEST_CASE(flow_receiver_witness_death_retains_history_and_distinguishes_address_reuse)
{
    auto owner = std::make_shared<SessionNotificationState>();
    FlowReceiverWitnessRegistry registry(owner, "isolated lifetime fixture");
    int token = 0;
    auto old = registry.recordNormalSubConstruction(&token);
    CHECK(unsupported([&] { registry.recordCopyConstruction(&token); }));
    registry.recordReceiverDeath(old);
    registry.recordReceiverDeath(old);
    auto replacement = registry.recordNormalSubConstruction(&token);
    CHECK(old != replacement && !old->nativeReceiverAlive() && replacement->nativeReceiverAlive());
    REQUIRE(registry.orderedWitnesses().size() == 2);
    CHECK(registry.orderedWitnesses()[0] == old && registry.orderedWitnesses()[1] == replacement);
    registry.recordReceiverDeath(old); // An old generation cannot kill the new one.
    CHECK(replacement->nativeReceiverAlive());
}

TEST_CASE(flow_receiver_witness_context_is_late_bound_and_backend_stays_unknown)
{
    Project project;
    SubProject first, second;
    first.project = &project; second.project = &project;
    auto owner = std::make_shared<SessionNotificationState>();
    int token = 0;
    std::shared_ptr<FlowReceiverWitness> escaped;
    {
        FlowReceiverWitnessRegistry registry(owner, "isolated context fixture");
        escaped = registry.recordNormalSubConstruction(&token);
        registry.bindModelContext(escaped, project, first); // May precede SUB-list append.
        registry.bindModelContext(escaped, project, first);
        CHECK(escaped->actualProject() == &project && escaped->actualSubProject() == &first);
        CHECK(escaped->actualFlowModel() == &first.flow && !escaped->backendWitness().has_value());
        CHECK(unsupported([&] { registry.bindModelContext(escaped, project, second); }));
    }
    CHECK(!escaped->nativeReceiverAlive() && !escaped->actualProject() && !escaped->actualFlowModel());
}

TEST_CASE(flow_receiver_witness_rejects_foreign_leases_and_missing_scope)
{
    auto owner = std::make_shared<SessionNotificationState>();
    CHECK(unsupported([&] { FlowReceiverWitnessRegistry registry({}, "isolated"); }));
    CHECK(unsupported([&] { FlowReceiverWitnessRegistry registry(owner, ""); }));
    FlowReceiverWitnessRegistry first(owner, "isolated first"), second(owner, "isolated second");
    int token = 0;
    CHECK(unsupported([&] { first.recordNormalSubConstruction(nullptr); }));
    auto witness = first.recordNormalSubConstruction(&token);
    CHECK(unsupported([&] { second.recordReceiverDeath(witness); }));
    auto foreign = std::shared_ptr<FlowReceiverWitness>(witness.get(), [](FlowReceiverWitness*) {});
    CHECK(unsupported([&] { first.recordReceiverDeath(foreign); }));
    CHECK(witness->nativeReceiverAlive());
    owner.reset();
    CHECK(unsupported([&] { first.recordCopyConstruction(&token); }));
    first.recordReceiverDeath(witness); // Death can be recorded after notifier expiry.
    CHECK(!witness->nativeReceiverAlive());
    first.releaseAdapterScope(); first.releaseAdapterScope();
}

int main() { return testkit::run_all(); }
