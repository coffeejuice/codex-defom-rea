#include <testkit/check.hpp>

#include <string>
#include <vector>

#include "../../src/ops/type3_opt_region_section.hpp"
#include "../../src/ops/type3_optimizer_defopt_view.hpp"

using namespace m2m::mst::detail;

TEST_CASE(type3_opt_region_section_plans_nonpositive_count_as_literal_and_endl)
{
    Type3OptimizerScalarState state;
    const auto events = planType3OptRegionSection(Type3OptimizerDefOptView(state));
    REQUIRE(events.size() == 2);
    CHECK(events[0].kind == Type3OptRegionSectionEvent::Kind::Literal);
    CHECK(events[0].literal != nullptr);
    CHECK(std::string(events[0].literal) == "REGIONS\t\t0");
    CHECK(events[1].kind == Type3OptRegionSectionEvent::Kind::Endl);
}

TEST_CASE(type3_opt_region_section_plans_count_and_ordered_prepared_qstrings)
{
    Type3OptimizerScalarState state;
    appendType3RegionPreparedRow(state, m2m::qt::L("first"));
    appendType3RegionPreparedRow(state, m2m::qt::L("second.part"));
    const auto events = planType3OptRegionSection(Type3OptimizerDefOptView(state));
    REQUIRE(events.size() == 7);
    CHECK(events[0].kind == Type3OptRegionSectionEvent::Kind::Literal);
    CHECK(events[0].literal != nullptr);
    CHECK(std::string(events[0].literal) == "REGIONS    \t\t");
    CHECK(events[1].kind == Type3OptRegionSectionEvent::Kind::Integer);
    CHECK(events[1].integer == 2);
    CHECK(events[2].kind == Type3OptRegionSectionEvent::Kind::Endl);
    CHECK(events[3].kind == Type3OptRegionSectionEvent::Kind::String);
    CHECK(events[3].string == m2m::qt::L("Opt/first.GEO"));
    CHECK(events[4].kind == Type3OptRegionSectionEvent::Kind::Endl);
    CHECK(events[5].kind == Type3OptRegionSectionEvent::Kind::String);
    CHECK(events[5].string == m2m::qt::L("Opt/second.part.GEO"));
    CHECK(events[6].kind == Type3OptRegionSectionEvent::Kind::Endl);
}

TEST_CASE(type3_opt_region_section_emits_each_row_before_the_next_getter)
{
    std::vector<std::int32_t> requestedIndices;
    std::vector<Type3OptRegionSectionEvent> emitted;
    int endlCount = 0;
    bool caught = false;
    try {
        emitType3OptRegionSectionWithCallbacks(
            2,
            [&requestedIndices](std::int32_t index) {
                requestedIndices.push_back(index);
                return index == 0 ? m2m::qt::L("first") : m2m::qt::L("second");
            },
            [&emitted, &endlCount](const Type3OptRegionSectionEvent& event) {
                emitted.push_back(event);
                if (event.kind == Type3OptRegionSectionEvent::Kind::Endl && ++endlCount == 2)
                    throw 17;
            });
    } catch (int value) {
        caught = value == 17;
    }

    CHECK(caught);
    REQUIRE(requestedIndices.size() == 1);
    CHECK(requestedIndices[0] == 0);
    REQUIRE(emitted.size() == 5);
    CHECK(emitted[3].kind == Type3OptRegionSectionEvent::Kind::String);
    CHECK(emitted[3].string == m2m::qt::L("Opt/first.GEO"));
    CHECK(emitted[4].kind == Type3OptRegionSectionEvent::Kind::Endl);
}

int main() { return testkit::run_all(); }
