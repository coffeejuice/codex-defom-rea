#include <testkit/check.hpp>
#include "../../src/ops/type3_shapeoptim_disposal.hpp"

using namespace m2m::mst::detail;
namespace {
using Event = Type3ShapeOptimDisposalEvent;
using Result = Type3ShapeOptimDisposalResult;
Type3ShapeOptimIdentityGraph graph() {
    Type3ShapeOptimIdentityGraph g;
    g.operations.emplace  (10, Type3ShapeOptimIdentityOperation{});
    g.operations.at(10).property = 20;
    g.properties.emplace(20, Type3ShapeOptimIdentityProperty{});
    g.properties.at(20).data = 30;
    g.data.emplace(30, Type3ShapeOptimPropertyValues{200, 0.001});
    return g;
}
Type3ShapeOptimDisposalAssumptions assumptions() {
    Type3ShapeOptimDisposalAssumptions a;
    a.oldTreeRequiresRelease = true;
    a.qobjectHasGuards = true;
    a.qobjectHasChildren = true;
    a.guardDestinations = {1, 2};
    return a;
}
const std::vector<Event> ordered = {
    Event::DerivedVtables, Event::TreeRelease, Event::DerivedListPublished,
    Event::BaseVtables, Event::BaseListReset, Event::OperationGuardRemoved,
    Event::SecondaryListReset, Event::SecondaryListDestroyed, Event::QObjectGuardClear,
    Event::QObjectChildCleanup, Event::QObjectCompletion, Event::PropertyReleased,
    Event::OperationPropertyCleared
};
}

TEST_CASE(disposal_complete_and_graph_unchanged) {
    auto g = graph(); auto p = projectType3ShapeOptimDisposal(g, 10, assumptions());
    CHECK(p.result == Result::Complete); CHECK(p.events == ordered);
    CHECK(!p.operationProperty); CHECK(p.property == Type3ShapeOptimOptionalIdentity{20});
    CHECK(p.data == Type3ShapeOptimOptionalIdentity{30});
    CHECK(p.listPublishedEmpty); CHECK(p.operationGuardRemoved);
    CHECK(p.secondaryListDestroyed); CHECK(p.qobjectCompleted); CHECK(p.propertyReleased);
    CHECK(p.clearedGuards == assumptions().guardDestinations);
    CHECK(g.operations.at(10).property == Type3ShapeOptimOptionalIdentity{20});
    CHECK(g.properties.at(20).data == Type3ShapeOptimOptionalIdentity{30});
    CHECK_EQ(g.data.at(30).maxSims, 200); CHECK_BITS(g.data.at(30).minObjective, 0.001);
}

TEST_CASE(disposal_each_refusal_retains_exact_completed_prefix) {
    auto g = graph();
    for (std::size_t i = 0; i < ordered.size(); ++i) {
        auto p = projectType3ShapeOptimDisposal(g, 10, assumptions(), ordered[i]);
        CHECK(p.result == Result::Refused);
        CHECK(p.events == std::vector<Event>(ordered.begin(), ordered.begin() + i));
        CHECK(p.operationProperty == Type3ShapeOptimOptionalIdentity{20});
        CHECK(p.listPublishedEmpty == (i > 2));
        CHECK(p.operationGuardRemoved == (i > 5));
        CHECK(p.secondaryListDestroyed == (i > 7));
        CHECK((!p.clearedGuards.empty()) == (i > 8));
        CHECK(p.qobjectCompleted == (i > 10));
        CHECK(p.propertyReleased == (i > 11));
    }
    CHECK(g.operations.at(10).property == Type3ShapeOptimOptionalIdentity{20});
    CHECK_EQ(g.data.at(30).maxSims, 200);
}

TEST_CASE(disposal_null_and_invalid_references_have_no_effects) {
    auto g = graph();
    CHECK(projectType3ShapeOptimDisposal(g, 999, assumptions()).result == Result::InvalidPreparedIdentity);
    g.operations.at(10).property.reset();
    auto p = projectType3ShapeOptimDisposal(g, 10, assumptions());
    CHECK(p.result == Result::Complete); CHECK(p.events.empty()); CHECK(!p.operationProperty);
    g.operations.at(10).property = 99;
    p = projectType3ShapeOptimDisposal(g, 10, assumptions());
    CHECK(p.result == Result::InvalidPreparedIdentity); CHECK(p.events.empty());
    g.operations.at(10).property = 20; g.properties.at(20).data = 99;
    p = projectType3ShapeOptimDisposal(g, 10, assumptions());
    CHECK(p.result == Result::InvalidPreparedIdentity); CHECK(p.events.empty());
}

TEST_CASE(disposal_conditional_boundaries_and_guard_assumptions) {
    auto g = graph(); Type3ShapeOptimDisposalAssumptions a;
    auto p = projectType3ShapeOptimDisposal(g, 10, a, Event::TreeRelease);
    CHECK(p.result == Result::Complete); CHECK(p.clearedGuards.empty());
    a.guardDestinations = {1};
    p = projectType3ShapeOptimDisposal(g, 10, a);
    CHECK(p.result == Result::InvalidPreparedIdentity); CHECK(p.events.empty());
}

TEST_CASE(disposal_sparse_sequence_and_null_data) {
    auto g = graph();
    g.properties.at(20).data.reset();
    Type3ShapeOptimDisposalAssumptions a;
    auto p = projectType3ShapeOptimDisposal(g, 10, a);
    const std::vector<Event> expected = {
        Event::DerivedVtables, Event::DerivedListPublished, Event::BaseVtables,
        Event::BaseListReset, Event::OperationGuardRemoved, Event::SecondaryListReset,
        Event::SecondaryListDestroyed, Event::QObjectCompletion, Event::PropertyReleased,
        Event::OperationPropertyCleared
    };
    CHECK(p.result == Result::Complete); CHECK(p.events == expected);
    CHECK(!p.data); CHECK(!g.properties.at(20).data);
    CHECK_EQ(g.data.size(), 1u); CHECK_EQ(g.data.at(30).maxSims, 200);
}
int main() { return testkit::run_all(); }
