#include <testkit/check.hpp>
#include "../../src/ops/type3_opt_count_section.hpp"

#include <cstring>
#include <vector>

using namespace m2m::mst::detail;

namespace {
using Events = std::vector<Type3OptCountEvent>;

Events plan(std::int32_t count)
{
    Events events;
    planType3OptCountSection(count, [&](const Type3OptCountEvent& event) {
        events.push_back(event);
    });
    return events;
}

bool same(const Type3OptCountEvent& event, Type3OptCountEventKind kind,
          Type3OptCountLiteral role, std::int32_t value = 0,
          const char* literalText = nullptr)
{
    return event.kind == kind && event.literal == role && event.integer == value &&
           ((event.literalText == nullptr && literalText == nullptr) ||
            (event.literalText != nullptr && literalText != nullptr &&
             std::strcmp(event.literalText, literalText) == 0));
}
} // namespace

TEST_CASE(type3_opt_count_section_preserves_zero_count_and_final_endl)
{
    const auto events = plan(0);
    CHECK_EQ(events.size(), std::size_t(4));
    CHECK(same(events[0], Type3OptCountEventKind::literal, Type3OptCountLiteral::countLine, 0, "DOE_PAR    \t\t"));
    CHECK(same(events[1], Type3OptCountEventKind::integer, Type3OptCountLiteral::countLine, 0));
    CHECK(same(events[2], Type3OptCountEventKind::endl, Type3OptCountLiteral::countLine));
    CHECK(same(events[3], Type3OptCountEventKind::endl, Type3OptCountLiteral::countLine));
}

TEST_CASE(type3_opt_count_section_preserves_nonpositive_count_branch)
{
    const auto events = plan(-2);
    CHECK_EQ(events.size(), std::size_t(4));
    CHECK(same(events[1], Type3OptCountEventKind::integer, Type3OptCountLiteral::countLine, -2));
    CHECK(same(events[3], Type3OptCountEventKind::endl, Type3OptCountLiteral::countLine));
}

TEST_CASE(type3_opt_count_section_orders_each_index_literal_and_endl)
{
    const auto events = plan(2);
    CHECK_EQ(events.size(), std::size_t(10));
    CHECK(same(events[0], Type3OptCountEventKind::literal, Type3OptCountLiteral::countLine, 0, "DOE_PAR    \t\t"));
    CHECK(same(events[1], Type3OptCountEventKind::integer, Type3OptCountLiteral::countLine, 2));
    CHECK(same(events[2], Type3OptCountEventKind::endl, Type3OptCountLiteral::countLine));
    CHECK(same(events[3], Type3OptCountEventKind::integer, Type3OptCountLiteral::countLine, 1));
    CHECK(same(events[4], Type3OptCountEventKind::literal, Type3OptCountLiteral::indexedLine, 0, "   0.0  1.0  0.5  0.01   0"));
    CHECK(same(events[5], Type3OptCountEventKind::endl, Type3OptCountLiteral::countLine));
    CHECK(same(events[6], Type3OptCountEventKind::integer, Type3OptCountLiteral::countLine, 2));
    CHECK(same(events[7], Type3OptCountEventKind::literal, Type3OptCountLiteral::indexedLine, 0, "   0.0  1.0  0.5  0.01   0"));
    CHECK(same(events[8], Type3OptCountEventKind::endl, Type3OptCountLiteral::countLine));
    CHECK(same(events[9], Type3OptCountEventKind::endl, Type3OptCountLiteral::countLine));
}

int main() { return testkit::run_all(); }
