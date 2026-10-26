// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include "m2m/model.hpp"

#include <memory>
#include <span>
#include <vector>

namespace m2m::engine::simcontrol300 {
bool readPpvForOperation(bool opened, std::span<const qt::String> lines,
                         Operation& operation, bool hasPredecessor);
}

using namespace m2m;

namespace {
qt::String s(const char* text) { return qt::String::latin1(text); }

struct Fixture {
    Project project;
    Operation operation;

    Fixture()
    {
        auto sub = std::make_unique<SubProject>();
        sub->project = &project;
        project.subs.push_back(std::move(sub));
        operation.project = &project;
    }
};
}

TEST_CASE(simctrl_ppv_binding_mutates_native_local_root_and_flag_owners)
{
    Fixture fixture;
    auto& operation = fixture.operation;
    operation.recoveryB->push_back({90, 93.0, 91, 92.0, 93.0});
    fixture.project.recoveryA->push_back({4, 77.0, 5, 1.0, 2.0});
    operation.recoveryFlags = std::make_shared<std::vector<bool>>(std::initializer_list<bool>{false});
    const std::vector<qt::String> lines{
        s("1"), s("0 TRUE 4 5 1.0000005 2 0"),
    };

    CHECK(m2m::engine::simcontrol300::readPpvForOperation(true, lines, operation, true));
    CHECK(operation.recoveryB->empty()); // Successful open clears local OPI+0x20.
    CHECK_EQ(fixture.project.recoveryA->size(), 1u); // First tolerant match: no duplicate point.
    CHECK_EQ(fixture.project.recoveryA->at(0).value, 77.0); // Existing KEY value is preserved.
    CHECK_EQ(operation.recoveryFlags->size(), 2u);
    CHECK(operation.recoveryFlags->at(0)); // Native seven-token path parses token 5.
    CHECK(!operation.recoveryFlags->at(1));
}

TEST_CASE(simctrl_ppv_binding_appends_shared_records_and_inserts_flag_at_index)
{
    Fixture fixture;
    fixture.project.recoveryA->push_back({4, 18.0, 5, 1.0, 2.0});
    fixture.operation.recoveryFlags =
        std::make_shared<std::vector<bool>>(std::initializer_list<bool>{true});
    const std::vector<qt::String> lines{
        s("1"), s("0 true 7 8 9.5 10.5"),
    };

    CHECK(m2m::engine::simcontrol300::readPpvForOperation(
        true, lines, fixture.operation, true));
    CHECK_EQ(fixture.project.recoveryA->size(), 2u);
    const auto& appended = fixture.project.recoveryA->at(1);
    CHECK_EQ(appended.type, 7);
    CHECK_EQ(appended.value, 0.0); // Native PPV constructor leaves KEY BC+0x18 zero.
    CHECK_EQ(appended.ppvSecondInt, 8);
    CHECK_EQ(appended.ppvFirstDouble, 9.5);
    CHECK_EQ(appended.ppvSecondDouble, 10.5);
    CHECK_EQ(fixture.operation.recoveryFlags->size(), 2u);
    CHECK(!fixture.operation.recoveryFlags->at(1));
}

TEST_CASE(simctrl_ppv_binding_routes_local_when_there_is_no_predecessor)
{
    Fixture fixture;
    fixture.project.recoveryA->push_back({30, 40.0, 31, 32.0, 33.0});
    const std::vector<qt::String> lines{
        s("1"), s("0 true 6 7 8.0 9.0"),
    };

    CHECK(m2m::engine::simcontrol300::readPpvForOperation(
        true, lines, fixture.operation, false));
    CHECK_EQ(fixture.operation.recoveryB->size(), 1u);
    const auto& local = fixture.operation.recoveryB->front();
    CHECK_EQ(local.type, 6);
    CHECK_EQ(local.value, 0.0);
    CHECK_EQ(local.ppvSecondInt, 7);
    CHECK_EQ(local.ppvFirstDouble, 8.0);
    CHECK_EQ(local.ppvSecondDouble, 9.0);
    CHECK_EQ(fixture.project.recoveryA->size(), 1u); // Shared/root owner untouched.
}

TEST_CASE(simctrl_ppv_binding_preserves_owners_when_open_fails)
{
    Fixture fixture;
    fixture.operation.recoveryB->push_back({3, 6.0, 4, 5.0, 6.0});
    fixture.project.recoveryA->push_back({7, 8.0, 9, 10.0, 11.0});
    fixture.operation.recoveryFlags =
        std::make_shared<std::vector<bool>>(std::initializer_list<bool>{true});
    const auto localBefore = *fixture.operation.recoveryB;
    const auto rootBefore = *fixture.project.recoveryA;
    const auto flagsBefore = *fixture.operation.recoveryFlags;
    const std::vector<qt::String> lines{s("1"), s("0 true 1 2 3 4")};

    CHECK(!m2m::engine::simcontrol300::readPpvForOperation(
        false, lines, fixture.operation, true));
    CHECK_EQ(fixture.operation.recoveryB->size(), localBefore.size());
    CHECK_EQ(fixture.project.recoveryA->size(), rootBefore.size());
    CHECK_EQ(fixture.operation.recoveryFlags->size(), flagsBefore.size());
    CHECK_EQ(fixture.operation.recoveryB->front().type, localBefore.front().type);
    CHECK_EQ(fixture.project.recoveryA->front().value, rootBefore.front().value);
    CHECK(fixture.operation.recoveryFlags->front());
}

TEST_CASE(simctrl_ppv_binding_commits_partial_rows_before_parse_failure)
{
    Fixture fixture;
    fixture.project.recoveryA->push_back({4, 18.0, 5, 1.0, 2.0});
    fixture.operation.recoveryFlags =
        std::make_shared<std::vector<bool>>(std::initializer_list<bool>{true});
    const std::vector<qt::String> lines{
        s("2"), s("0 true 7 8 9.5 10.5 1"), s("short"),
    };

    CHECK(!m2m::engine::simcontrol300::readPpvForOperation(
        true, lines, fixture.operation, true));
    CHECK(fixture.operation.recoveryB->empty()); // Open succeeded and local was cleared.
    CHECK_EQ(fixture.project.recoveryA->size(), 2u); // Earlier row remains committed.
    CHECK_EQ(fixture.project.recoveryA->at(1).type, 7);
    CHECK_EQ(fixture.operation.recoveryFlags->size(), 2u);
    CHECK(fixture.operation.recoveryFlags->at(0)); // Existing flag remains at index zero.
    CHECK(!fixture.operation.recoveryFlags->at(1)); // Token 5 is "10.5": ignored toInt status yields zero.
}

TEST_CASE(simctrl_ppv_binding_rejects_undersized_flags_before_mutating_owners)
{
    Fixture fixture;
    fixture.operation.recoveryB->push_back({90, 93.0, 91, 92.0, 93.0});
    fixture.project.recoveryA->push_back({4, 18.0, 5, 1.0, 2.0});
    fixture.project.recoveryA->push_back({7, 19.0, 8, 9.5, 10.5});
    fixture.operation.recoveryFlags = std::make_shared<std::vector<bool>>();

    const auto localBefore = *fixture.operation.recoveryB;
    const auto rootBefore = *fixture.project.recoveryA;
    const auto flagsBefore = *fixture.operation.recoveryFlags;
    const std::vector<qt::String> lines{
        s("1"), s("0 true 7 8 9.5 10.5"), // Matches shared point index 1.
    };

    REQUIRE(!m2m::engine::simcontrol300::readPpvForOperation(
        true, lines, fixture.operation, true));

    REQUIRE(fixture.operation.recoveryB->size() == localBefore.size());
    CHECK_EQ(fixture.operation.recoveryB->at(0).type, localBefore[0].type);
    CHECK_EQ(fixture.operation.recoveryB->at(0).value, localBefore[0].value);
    CHECK_EQ(fixture.operation.recoveryB->at(0).ppvSecondInt, localBefore[0].ppvSecondInt);
    CHECK_EQ(fixture.operation.recoveryB->at(0).ppvFirstDouble, localBefore[0].ppvFirstDouble);
    CHECK_EQ(fixture.operation.recoveryB->at(0).ppvSecondDouble, localBefore[0].ppvSecondDouble);

    REQUIRE(fixture.project.recoveryA->size() == rootBefore.size());
    for (std::size_t i = 0; i < rootBefore.size(); ++i) {
        CHECK_EQ(fixture.project.recoveryA->at(i).type, rootBefore[i].type);
        CHECK_EQ(fixture.project.recoveryA->at(i).value, rootBefore[i].value);
        CHECK_EQ(fixture.project.recoveryA->at(i).ppvSecondInt, rootBefore[i].ppvSecondInt);
        CHECK_EQ(fixture.project.recoveryA->at(i).ppvFirstDouble, rootBefore[i].ppvFirstDouble);
        CHECK_EQ(fixture.project.recoveryA->at(i).ppvSecondDouble, rootBefore[i].ppvSecondDouble);
    }

    CHECK_EQ(fixture.operation.recoveryFlags->size(), flagsBefore.size());
}

int main() { return testkit::run_all(); }
