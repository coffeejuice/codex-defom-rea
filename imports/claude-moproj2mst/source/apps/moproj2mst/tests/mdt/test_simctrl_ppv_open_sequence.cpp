// ctest-labels: m2m-mdt
#include "../support/test_paths.hpp"
#include <testkit/check.hpp>

#include "m2m/engine/simctrl_ppv_open_sequence.hpp"
#include "m2m/model.hpp"
#include "m2m/qt4path.hpp"

#include <memory>
#include <string>

using namespace m2m;
using namespace m2m::engine::simctrl_ppv;

namespace {

struct Fixture {
    Project project;
    Operation operation;
    qt::String path = qt::L("C:/scratch/Operations/Task00001/SimCtrl.PPV");

    Fixture()
    {
        auto sub = std::make_unique<SubProject>();
        sub->project = &project;
        project.subs.push_back(std::move(sub));
        operation.project = &project;
        operation.objs.push_back(std::make_unique<ObjEntry>());
        operation.objs.push_back(std::make_unique<ObjEntry>());
    }

    void put(const std::string& bytes)
    {
        project.overlay[qt::cleanPath(path)] = bytes;
    }
};

} // namespace

TEST_CASE(simctrl_ppv_open_sequence_parses_then_propagates_local_count_to_every_slot)
{
    Fixture fixture;
    fixture.put("1\r\n0 true 2 3 4.5 6.5\r\n");

    CHECK(readPpvOpenSequence(fixture.project, fixture.path, fixture.operation, false) ==
          OpenSequenceStatus::Applied);
    REQUIRE(fixture.operation.recoveryB->size() == 1u);
    CHECK_EQ(fixture.operation.objs[0]->sv.ppvCount(), 1);
    CHECK_EQ(fixture.operation.objs[1]->sv.ppvCount(), 1);
}

TEST_CASE(simctrl_ppv_open_sequence_open_failure_preserves_lists_but_updates_count)
{
    Fixture fixture;
    const auto missing = m2m_test::scratchRoot() / "ppv-open-missing" / "SimCtrl.PPV";
    REQUIRE(!std::filesystem::exists(missing));
    fixture.path = qt::String::latin1(missing.generic_string().c_str());
    fixture.operation.recoveryB->push_back({4, 12.0, 5, 1.0, 2.0});
    fixture.project.recoveryA->push_back({6, 13.0, 7, 3.0, 4.0});
    fixture.operation.objs[0]->sv.setPpvCount(99);

    CHECK(readPpvOpenSequence(fixture.project, fixture.path, fixture.operation, true) ==
          OpenSequenceStatus::OpenFailed);
    CHECK_EQ(fixture.operation.recoveryB->size(), 1u);
    CHECK_EQ(fixture.project.recoveryA->size(), 1u);
    CHECK_EQ(fixture.operation.objs[0]->sv.ppvCount(), 2);
    CHECK_EQ(fixture.operation.objs[1]->sv.ppvCount(), 2);
}

TEST_CASE(simctrl_ppv_open_sequence_parser_failure_keeps_partial_rows_and_updates_count)
{
    Fixture fixture;
    fixture.operation.recoveryB->push_back({90, 91.0, 92, 93.0, 94.0});
    fixture.put("2\n0 false 2 3 4.5 6.5\nshort row\n");

    CHECK(readPpvOpenSequence(fixture.project, fixture.path, fixture.operation, false) ==
          OpenSequenceStatus::ParserReturnedFalse);
    REQUIRE(fixture.operation.recoveryB->size() == 1u);
    CHECK_EQ(fixture.operation.recoveryB->front().type, 2);
    CHECK_EQ(fixture.operation.objs[0]->sv.ppvCount(), 1);
    CHECK_EQ(fixture.operation.objs[1]->sv.ppvCount(), 1);
}

TEST_CASE(simctrl_ppv_open_sequence_refuses_unknown_decode_before_mutation_or_count)
{
    Fixture fixture;
    fixture.operation.recoveryB->push_back({90, 91.0, 92, 93.0, 94.0});
    fixture.operation.objs[0]->sv.setPpvCount(77);
    // The current reader accepts a UTF-8 BOM with ASCII payload; a bare high
    // byte still depends on the original process locale codec.
    fixture.put(std::string("\x80" "1\n", 3));

    CHECK(readPpvOpenSequence(fixture.project, fixture.path, fixture.operation, false) ==
          OpenSequenceStatus::UnsupportedInput);
    CHECK_EQ(fixture.operation.recoveryB->size(), 1u);
    CHECK_EQ(fixture.operation.objs[0]->sv.ppvCount(), 77);
}

int main() { return testkit::run_all(); }
