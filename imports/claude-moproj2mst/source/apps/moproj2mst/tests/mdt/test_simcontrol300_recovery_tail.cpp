// ctest-labels: m2m-mdt
// Owner-bound private Save tail; no native QWidget/public Session completion.
#include <testkit/check.hpp>
#include <algorithm>
#include <bit>
#include <cstdint>
#include <initializer_list>
#include <limits>
#include <memory>
#include <span>
#include <vector>
#include "m2m/engine.hpp"
#include "m2m/engine/simcontrol300_io.hpp"
#include "m2m/engine/simcontrol300_owner.hpp"
#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/llobj/llobj_parts.hpp>

namespace m2m::engine::simcontrol300 {
bool readPpvForOperation(bool, std::span<const qt::String>, Operation&, bool);
}

using namespace m2m;
using namespace m2m::engine::simcontrol300;
namespace {
struct Fixture {
    Project project;
    Operation operation;
    Fixture()
    {
        auto sub = std::make_unique<SubProject>(); sub->project = &project;
        project.subs.push_back(std::move(sub));
        operation.project = &project;
        operation.engineModel = std::make_shared<engine::GuiModel>(nullptr, 3);
        operation.recoveryFlags = std::make_shared<std::vector<bool>>();
    }
    engine::GuiModel& model() { return *operation.engineModel; }
    SimControl& control() { return *model().simCtl(); }
    AdvancedPage form()
    {
        auto result = fillAdvanced3D(control(), model(), NumericLocale::KnownC);
        REQUIRE(result); return *result;
    }
    void ppv(bool shared, std::initializer_list<const char*> rows)
    {
        std::vector<qt::String> lines{qt::number(static_cast<int>(rows.size()))};
        for (const auto* row : rows) lines.push_back(qt::L(row));
        REQUIRE(readPpvForOperation(true, lines, operation, shared));
    }
    ApplyResult apply(const AdvancedPage& page)
    { return applyOwnedRecoveryTail3D(control(), model(), page, operation); }
};
void table(def::model::DYNARY& value, std::initializer_list<double> axis,
    int components, std::initializer_list<double> data)
{
    def::model::dynary_free_arrays(&value);
    value.ndim = 1; value.var[0] = 0;
    value.n[0] = static_cast<int>(axis.size()); value.n[1] = value.n[2] = 0;
    value.ncomp = components;
    REQUIRE(data.size() == axis.size() * components);
    def::model::dynary_alloc(&value); REQUIRE(value.axis[0]); REQUIRE(value.values);
    std::copy(axis.begin(), axis.end(), value.axis[0]);
    std::copy(data.begin(), data.end(), value.values);
}
void output(const def::model::DYNARY& value, int type,
    std::initializer_list<double> axis, std::initializer_list<double> data)
{
    CHECK_EQ(value.type, type); CHECK_EQ(value.ndim, 1); CHECK_EQ(value.ncomp, 2);
    REQUIRE(value.n[0] == static_cast<int>(axis.size())); REQUIRE(value.axis[0]); REQUIRE(value.values);
    REQUIRE(data.size() == axis.size() * 2);
    std::size_t at = 0;
    for (double expected : axis) CHECK(value.axis[0][at++] == expected);
    at = 0; for (double expected : data) CHECK(value.values[at++] == expected);
}
bool sameEntries(const std::vector<RecoveryEntry>& left, const std::vector<RecoveryEntry>& right)
{
    if (left.size() != right.size()) return false;
    for (std::size_t i = 0; i < left.size(); ++i) {
        const auto& a = left[i]; const auto& b = right[i];
        if (a.type != b.type || a.ppvSecondInt != b.ppvSecondInt ||
            std::bit_cast<std::uint64_t>(a.value) != std::bit_cast<std::uint64_t>(b.value) ||
            std::bit_cast<std::uint64_t>(a.ppvFirstDouble) != std::bit_cast<std::uint64_t>(b.ppvFirstDouble) ||
            std::bit_cast<std::uint64_t>(a.ppvSecondDouble) != std::bit_cast<std::uint64_t>(b.ppvSecondDouble)) return false;
    }
    return true;
}
}

TEST_CASE(owned_recovery_tail_rebuilds_real_ppv_lists_in_root_then_local_order)
{
    Fixture f;
    f.ppv(true, {"0 true 1 9 3.75 -4.5", "0 true 0 7 1.25 2.5"});
    f.ppv(false, {"0 true 0 11 5.25 6.5", "0 true 1 13 7.75 8.5"});
    const auto rootOwner = f.project.recoveryA, localOwner = f.operation.recoveryB;
    rootOwner->at(0).value = 99; localOwner->at(0).value = -99; // KEY value is not a payload input.
    const auto rootBefore = *rootOwner, localBefore = *localOwner;
    auto& sc = f.control(); auto page = f.form();
    table(sc.nrecid, {42}, 2, {43, 44}); table(sc.erecid, {45}, 2, {46, 47});
    CHECK(f.apply(page) == ApplyResult::Completed);
    output(sc.nrecid, 0xb2, {7, 11}, {1.25, 2.5, 5.25, 6.5});
    output(sc.erecid, 0xb1, {9, 13}, {3.75, -4.5, 7.75, 8.5});
    CHECK(sc.nrecid.axis[0] != sc.erecid.axis[0]); CHECK(sc.nrecid.values != sc.erecid.values);
    CHECK(f.project.recoveryA.get() == rootOwner.get()); CHECK(f.operation.recoveryB.get() == localOwner.get());
    CHECK(!rootOwner.owner_before(f.project.recoveryA) && !f.project.recoveryA.owner_before(rootOwner));
    CHECK(sameEntries(*rootOwner, rootBefore)); CHECK(sameEntries(*localOwner, localBefore));
    CHECK(!StagedEditor300::completeForPublicSave);
}

TEST_CASE(owned_recovery_tail_node_only_keeps_existing_element_storage_and_values)
{
    Fixture f; f.ppv(false, {"0 true 0 -2147483648 1.25 2.5", "0 true 0 2147483647 3.25 4.5"});
    auto& sc = f.control(); auto page = f.form();
    table(sc.nrecid, {1}, 2, {2, 3}); table(sc.erecid, {71}, 2, {72, 73});
    const auto* axis = sc.erecid.axis[0]; const auto* values = sc.erecid.values;
    CHECK(f.apply(page) == ApplyResult::Completed);
    output(sc.nrecid, 0xb2, {-2147483648.0, 2147483647.0}, {1.25, 2.5, 3.25, 4.5});
    output(sc.erecid, 0xb1, {71}, {72, 73});
    CHECK(sc.erecid.axis[0] == axis); CHECK(sc.erecid.values == values);
}

TEST_CASE(owned_recovery_tail_keeps_actual_list_aliases_and_counts_both_native_occurrences)
{
    Fixture f; f.ppv(true, {"0 true 0 7 1.25 2.5", "0 true 1 9 3.75 4.5"});
    f.operation.recoveryB = f.project.recoveryA; // Actual holders share the same real list/control block.
    const auto owner = f.project.recoveryA; const auto before = *owner;
    auto page = f.form(); CHECK(f.apply(page) == ApplyResult::Completed);
    output(f.control().nrecid, 0xb2, {7, 7}, {1.25, 2.5, 1.25, 2.5});
    output(f.control().erecid, 0xb1, {9, 9}, {3.75, 4.5, 3.75, 4.5});
    CHECK(f.project.recoveryA.get() == owner.get() && f.operation.recoveryB.get() == owner.get());
    CHECK(sameEntries(*owner, before)); // No deduplication or persistent row duplication.
}

TEST_CASE(owned_recovery_tail_copies_payload_bits_without_numeric_formatting)
{
    Fixture f; f.ppv(false, {"0 true 0 7 1 2"});
    auto& row = f.operation.recoveryB->front();
    row.ppvFirstDouble = std::bit_cast<double>(std::uint64_t{0x7ff8000000000042});
    row.ppvSecondDouble = -0.0;
    const auto before = *f.operation.recoveryB; auto page = f.form();
    CHECK(f.apply(page) == ApplyResult::Completed); REQUIRE(f.control().nrecid.values);
    CHECK_EQ(std::bit_cast<std::uint64_t>(f.control().nrecid.values[0]), std::uint64_t{0x7ff8000000000042});
    CHECK_EQ(std::bit_cast<std::uint64_t>(f.control().nrecid.values[1]), std::uint64_t{0x8000000000000000});
    CHECK(sameEntries(*f.operation.recoveryB, before));
}

TEST_CASE(owned_recovery_tail_element_only_and_empty_paths_clear_only_node_table)
{
    for (bool element : {true, false}) {
        Fixture f; if (element) f.ppv(false, {"0 true 1 23 24.25 25.5"});
        auto& sc = f.control(); auto page = f.form();
        table(sc.nrecid, {1}, 2, {2, 3}); table(sc.erecid, {71}, 2, {72, 73});
        const auto* axis = sc.erecid.axis[0]; const auto* values = sc.erecid.values;
        CHECK(f.apply(page) == ApplyResult::Completed);
        CHECK_EQ(sc.nrecid.n[0], 0); CHECK_EQ(sc.nrecid.ncomp, 0);
        CHECK(!sc.nrecid.axis[0] && !sc.nrecid.values);
        if (element) output(sc.erecid, 0xb1, {23}, {24.25, 25.5});
        else {
            output(sc.erecid, 0xb1, {71}, {72, 73});
            CHECK(sc.erecid.axis[0] == axis); CHECK(sc.erecid.values == values);
        }
    }
}

TEST_CASE(owned_recovery_tail_reapplies_advanced_after_its_earlier_page_occurrence)
{
    Fixture f; f.ppv(false, {"0 true 0 1 2 3"});
    auto& sc = f.control(); auto page = f.form();
    page.time[0] = {qt::L("3.25"), ValidatorState::Acceptable};
    CHECK(applyAdvanced3D(sc, f.model(), page) == ApplyResult::Completed);
    CHECK(sc.tnow1 == 3.25);
    page.time[0] = {qt::L("7.5"), ValidatorState::Acceptable};
    CHECK(f.apply(page) == ApplyResult::Completed); CHECK(sc.tnow1 == 7.5);
}

TEST_CASE(owned_recovery_tail_resizes_actual_output_arrays_after_validation_stop)
{
    Fixture f;
    f.ppv(false, {"0 true 0 1 2 3", "0 true 0 4 5 6", "0 true 1 7 8 9", "0 true 1 10 11 12"});
    auto* object = f.model().objectWrap(1, true, false); REQUIRE(object);
    auto& nodes = def::model::llobj::parts::part_8a0(*object);
    auto& elements = def::model::llobj::parts::part_9e0(*object);
    // Explicit current-model widths before actual array allocation, not an inferred startup default.
    f.model().step->u15f8[(0x1680 - 0x15f8) / 4] = 0;
    f.model().step->u15f8[(0x16f4 - 0x15f8) / 4] = 0;
    nodes.alloc(1, f.model().step->u15f8); elements.alloc(1, f.model().step->u15f8, 4);
    auto& sc = f.control(); auto page = f.form();
    CHECK_EQ(f.model().step->u15f8[(0x1680 - 0x15f8) / 4], 0);
    CHECK_EQ(f.model().step->u15f8[(0x16f4 - 0x15f8) / 4], 0);
    CHECK(!*reinterpret_cast<double**>(reinterpret_cast<std::byte*>(&nodes) + 0xb8));
    CHECK(!*reinterpret_cast<double**>(reinterpret_cast<std::byte*>(&elements) + 0xd8));
    table(sc.timeid, {1}, 1, {0}); // TIMEID width1 deliberately differs from NRECID width2.
    page.time[0] = {qt::L("bad"), ValidatorState::Invalid}; sc.tnow1 = 19;
    CHECK(f.apply(page) == ApplyResult::ValidationStopped); CHECK(sc.tnow1 == 19);
    output(sc.nrecid, 0xb2, {1, 4}, {2, 3, 5, 6});
    output(sc.erecid, 0xb1, {7, 10}, {8, 9, 11, 12});
    CHECK_EQ(f.model().step->u15f8[(0x1680 - 0x15f8) / 4], 1);
    CHECK_EQ(f.model().step->u15f8[(0x16f4 - 0x15f8) / 4], 2);
    auto* nodeValues = *reinterpret_cast<double**>(reinterpret_cast<std::byte*>(&nodes) + 0xb8);
    auto* elementValues = *reinterpret_cast<double**>(reinterpret_cast<std::byte*>(&elements) + 0xd8);
    REQUIRE(nodeValues); REQUIRE(elementValues);
}

TEST_CASE(owned_recovery_tail_guards_actual_owner_and_lazy_root_before_table_clear)
{
    for (int mode = 0; mode < 6; ++mode) {
        Fixture f; auto& sc = f.control(); auto page = f.form(); table(sc.nrecid, {61}, 2, {62, 63});
        const auto* axis = sc.nrecid.axis[0]; const auto* values = sc.nrecid.values;
        const auto root = f.project.recoveryA, local = f.operation.recoveryB;
        if (mode == 0) f.operation.project = nullptr;
        if (mode == 1) f.project.recoveryA.reset();
        if (mode == 2) f.operation.recoveryB.reset();
        if (mode == 3) f.project.subs[0]->type = 4;
        if (mode == 5) REQUIRE(f.operation.commitOperationInfoQObjectBoundary(
            "declared private recovery-tail OPI boundary; not native destruction"));
        if (mode == 4) {
            engine::GuiModel other(nullptr, 3);
            CHECK(applyOwnedRecoveryTail3D(sc, other, page, f.operation) == ApplyResult::UnsupportedSnapshot);
        } else CHECK(applyOwnedRecoveryTail3D(sc, f.model(), page, f.operation) == ApplyResult::UnsupportedSnapshot);
        output(sc.nrecid, 0xb2, {61}, {62, 63});
        CHECK(sc.nrecid.axis[0] == axis && sc.nrecid.values == values);
        CHECK(!f.project.subs[0]->ownRootRecoveryA); // The excluded type4 branch allocated no root.
        f.operation.project = &f.project; f.project.recoveryA = root; f.operation.recoveryB = local;
    }
}

TEST_CASE(owned_recovery_tail_excludes_unknown_types_resource_overflow_and_borrowed_shapes)
{
    for (int mode = 0; mode < 5; ++mode) {
        Fixture f; auto& sc = f.control(); auto page = f.form(); table(sc.nrecid, {61}, 2, {62, 63});
        const auto* axis = sc.nrecid.axis[0]; const auto* values = sc.nrecid.values;
        if (mode == 0) f.ppv(false, {"0 true 2 3 4 5"});
        if (mode == 1) f.operation.recoveryB->assign(4097, RecoveryEntry{0, 0, 1, 2, 3});
        if (mode == 2) sc.erecid.type = 0x80;
        if (mode == 3) sc.nrecid.n[0] = 0; // Actual allocation retained; native borrowed-marker profile excluded.
        if (mode == 4) sc.nrecid.u68 = new def::model::DYNARY;
        CHECK(f.apply(page) == ApplyResult::UnsupportedSnapshot);
        CHECK(sc.nrecid.axis[0] == axis && sc.nrecid.values == values);
        CHECK_EQ(sc.nrecid.ncomp, 2); CHECK(sc.nrecid.values[0] == 62 && sc.nrecid.values[1] == 63);
        if (mode == 3) sc.nrecid.n[0] = 1; // Restore real ownership for ordinary destructor cleanup.
        CHECK_EQ(sc.nrecid.n[0], 1);
    }
}

TEST_CASE(owned_recovery_tail_keeps_legacy_vector_only_nonempty_refusal)
{
    Fixture f; f.ppv(false, {"0 true 0 1 2 3"});
    auto& sc = f.control(); auto page = f.form(); table(sc.nrecid, {61}, 2, {62, 63});
    CHECK(applyEmptyRecoveryTail(sc, f.model(), page, f.project.recoveryA.get(), f.operation.recoveryB.get())
        == ApplyResult::UnsupportedSnapshot);
    output(sc.nrecid, 0xb2, {61}, {62, 63});
    CHECK(f.apply(page) == ApplyResult::Completed);
    output(sc.nrecid, 0xb2, {1}, {2, 3});
}

int main() { return testkit::run_all(); }
