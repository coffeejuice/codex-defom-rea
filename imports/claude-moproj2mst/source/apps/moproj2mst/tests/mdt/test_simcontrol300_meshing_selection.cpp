// ctest-labels: m2m-mdt private-editor300
// Actual GuiModel plus finite private controls, not native widgets/public Editor300 acceptance.
#include <testkit/check.hpp>
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <utility>
#include <xmmintrin.h>
#include "m2m/engine/simcontrol300.hpp"
#include "m2m/engine.hpp"
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/gui_twins_delete.hpp>

using namespace m2m;
using namespace m2m::engine::simcontrol300;
namespace {
template<class T> T& field(def::model::LLOBJ_DATA* object, std::size_t offset)
{
    return *reinterpret_cast<T*>(reinterpret_cast<std::byte*>(object) + offset);
}
void seed(def::model::LLOBJ_DATA* object, const std::array<double, 4>& values)
{
    REQUIRE(object); field<int>(object, 0x87c) = 1;
    field<double>(object, 0x2dc8) = values[0]; field<double>(object, 0x2dd0) = values[1];
    field<double>(object, 0x2de0) = values[2]; field<double>(object, 0x2dd8) = values[3];
}
std::array<double, 4> values(const MeshObjectSnapshot& row)
{
    return {row.interference, row.sizeRatio, row.tolerance, row.nodeCount};
}
std::array<double, 4> values(def::model::LLOBJ_DATA* object)
{
    return {field<double>(object, 0x2dc8), field<double>(object, 0x2dd0),
        field<double>(object, 0x2de0), field<double>(object, 0x2dd8)};
}
void checkBits(const std::array<double, 4>& actual, const std::array<double, 4>& expected)
{
    for (std::size_t i = 0; i < actual.size(); ++i)
        CHECK_EQ(std::bit_cast<std::uint64_t>(actual[i]), std::bit_cast<std::uint64_t>(expected[i]));
}
struct Fixture {
    engine::GuiModel model{nullptr, 3};
    RemeshState remesh;
    def::model::LLOBJ_DATA *two, *ten, *thirty;
    MeshingPage page{};
    Fixture() {
        two = model.objectWrap(2, true, false); ten = model.objectWrap(10, true, false);
        thirty = model.objectWrap(30, true, false);
        seed(ten, {-6.12345678, 7.12345678, -8.12345678, 9.75});
        seed(two, {2.12345678, 3.12345678, -4.12345678, 5.75});
        seed(thirty, {std::bit_cast<double>(std::uint64_t{0x7ff8000000001234ULL}), 31.2, -32.3, 33.5});
        auto filled = fillMeshing3D(model, remesh); REQUIRE(filled);
        page = std::move(*filled); REQUIRE(page.objects.size() == 3);
        REQUIRE(page.objects[0].number == 10); REQUIRE(page.objects[1].number == 2);
        REQUIRE(page.objects[2].number == 30);
    }
};
void checkLoadedFirst(const MeshingPage& page)
{
    CHECK_EQ(page.storedObjectIndex, 0); CHECK_EQ(page.comboObjectIndex, 0);
    CHECK(page.firstInterference == qt::L("6.12346")); CHECK(page.firstInterferenceNegative);
    CHECK(page.firstSizeRatio == qt::L("7.12346")); CHECK(page.firstTolerance == qt::L("-8.12346"));
    CHECK_EQ(page.firstNodeCount, 9);
}
}

TEST_CASE(meshing_selection_fill_and_activation_preserve_one_temporary_owner_until_apply)
{
    Fixture f; checkLoadedFirst(f.page);
    const auto beforeTen = values(f.ten), beforeTwo = values(f.two), beforeThirty = values(f.thirty);
    const auto unvisited = values(f.page.objects[2]);
    f.page.firstInterference = qt::L("-12.25"); f.page.firstInterferenceNegative = true;
    f.page.firstSizeRatio = qt::L("1.23456789"); f.page.firstTolerance = qt::L("-7.5");
    f.page.firstNodeCount = 77;
    REQUIRE(setMeshingComboCurrentIndex(f.page, 1));
    CHECK_EQ(f.page.storedObjectIndex, 0); checkBits(values(f.page.objects[0]), beforeTen);
    const auto outward = transitionMeshingObject3D(f.model, f.page, 1, MeshingObjectEvent::Activated);
    CHECK(outward.outcome == ApplyResult::Completed); CHECK(outward.previousCommitted);
    CHECK(!outward.sameRow); CHECK(outward.controlsReloaded); CHECK(outward.stage == MeshingObjectStage::Complete);
    checkBits(values(f.page.objects[0]), {-12.25, 1.23456789, 7.5, 77.0});
    CHECK_EQ(f.page.storedObjectIndex, 1); CHECK(f.page.firstSizeRatio == qt::L("3.12346"));
    CHECK(f.page.firstTolerance == qt::L("-4.12346")); CHECK_EQ(f.page.firstNodeCount, 5);
    f.page.firstSizeRatio = qt::L("-3.75"); f.page.firstNodeCount = 41;
    REQUIRE(setMeshingComboCurrentIndex(f.page, 0));
    const auto back = transitionMeshingObject3D(f.model, f.page, 0, MeshingObjectEvent::Activated);
    CHECK(back.outcome == ApplyResult::Completed); CHECK(back.previousCommitted); CHECK(back.controlsReloaded);
    checkBits(values(f.page.objects[1]), {2.12346, -3.75, 4.12346, 41.0});
    CHECK(f.page.firstSizeRatio == qt::L("1.23457")); CHECK_EQ(f.page.storedObjectIndex, 0);
    checkBits(values(f.page.objects[2]), unvisited);
    checkBits(values(f.ten), beforeTen); checkBits(values(f.two), beforeTwo); checkBits(values(f.thirty), beforeThirty);
}

TEST_CASE(meshing_same_row_always_reloads_precision_and_negative_zero_radio)
{
    Fixture f;
    f.page.firstInterference = qt::L("0"); f.page.firstInterferenceNegative = true;
    f.page.firstSizeRatio = qt::L("1.23456789"); f.page.firstTolerance = qt::L("-2.34567891");
    f.page.firstNodeCount = 8;
    const auto same = transitionMeshingObject3D(f.model, f.page, 0, MeshingObjectEvent::Activated);
    CHECK(same.outcome == ApplyResult::Completed); CHECK(same.previousCommitted);
    CHECK(same.sameRow); CHECK(same.controlsReloaded);
    CHECK_EQ(std::bit_cast<std::uint64_t>(f.page.objects[0].interference), std::uint64_t{0x8000000000000000ULL});
    CHECK(!f.page.firstInterferenceNegative); CHECK(f.page.firstInterference == qt::L("0"));
    CHECK(f.page.firstSizeRatio == qt::L("1.23457")); CHECK(f.page.firstTolerance == qt::L("2.34568"));
    CHECK(f.page.objects[0].sizeRatio == 1.23456789); CHECK(f.page.objects[0].tolerance == 2.34567891);
    const auto repeated = transitionMeshingObject3D(f.model, f.page, 0, MeshingObjectEvent::SaveCurrentIndex);
    CHECK(repeated.outcome == ApplyResult::Completed); CHECK(repeated.sameRow); CHECK(repeated.controlsReloaded);
    CHECK_EQ(std::bit_cast<std::uint64_t>(f.page.objects[0].interference), std::uint64_t{0});
    CHECK(f.page.objects[0].sizeRatio == 1.23457); CHECK(f.page.objects[0].tolerance == 2.34568);
}

TEST_CASE(meshing_stateful_save_uses_actual_combo_and_does_not_commit_fresh_target_twice)
{
    Fixture f;
    const auto targetBefore = values(f.page.objects[1]); const auto thirdBefore = values(f.page.objects[2]);
    f.page.firstSizeRatio = qt::L("6.543210987");
    REQUIRE(setMeshingComboCurrentIndex(f.page, 1));
    CHECK_EQ(f.page.storedObjectIndex, 0); CHECK(f.page.objects[0].sizeRatio == 7.12345678);
    seed(f.ten, {99.0, 99.0, 99.0, 99.0}); seed(f.two, {88.0, 88.0, 88.0, 88.0});
    f.page.remeshEnabled = false; f.page.remeshMethodRadio = 99;
    f.page.remeshScalingFactor = qt::L("bad"); f.remesh.enabled = 1; f.remesh.method = -9;
    f.remesh.scalingFactor = 8.25; f.remesh.goodInternalChangeable = true;
    CHECK(applyMeshingSelection3D(f.model, f.remesh, f.page) == ApplyResult::Completed);
    checkBits(values(f.ten), {-6.12346, 6.543210987, 8.12346, 9.0});
    checkBits(values(f.two), targetBefore); checkBits(values(f.thirty), thirdBefore);
    CHECK_EQ(f.page.storedObjectIndex, 1); CHECK_EQ(f.page.comboObjectIndex, 1);
    CHECK(f.page.firstSizeRatio == qt::L("3.12346")); CHECK_EQ(f.page.firstNodeCount, 5);
    CHECK_EQ(f.remesh.enabled, 0); CHECK_EQ(f.remesh.method, -9);
    CHECK(f.remesh.scalingFactor == 8.25); CHECK(f.remesh.goodInternalChangeable);
}

TEST_CASE(meshing_negative_current_index_skips_old_commit_but_save_copies_remembered_items)
{
    Fixture f; const auto remembered = values(f.page.objects[0]);
    f.page.firstInterference = qt::L("unread"); f.page.firstSizeRatio = qt::L("unread");
    f.page.firstNodeCount = -77;
    REQUIRE(setMeshingComboCurrentIndex(f.page, -1));
    const auto none = transitionMeshingObject3D(f.model, f.page, -1, MeshingObjectEvent::SaveCurrentIndex);
    CHECK(none.outcome == ApplyResult::Completed); CHECK(!none.previousCommitted); CHECK(!none.controlsReloaded);
    seed(f.ten, {99.0, 99.0, 99.0, 99.0});
    CHECK(applyMeshingSelection3D(f.model, f.remesh, f.page) == ApplyResult::Completed);
    checkBits(values(f.ten), remembered); checkBits(values(f.page.objects[0]), remembered);
    CHECK_EQ(f.page.storedObjectIndex, 0); CHECK_EQ(f.page.comboObjectIndex, -1);
    CHECK(f.page.firstInterference == qt::L("unread")); CHECK_EQ(f.page.firstNodeCount, -77);
}

TEST_CASE(meshing_target_guard_retains_previous_commit_and_leaves_target_controls_unloaded)
{
    Fixture f; const auto targetBits = values(f.page.objects[2]); const auto modelBefore = values(f.ten);
    f.page.firstSizeRatio = qt::L("9.5");
    REQUIRE(setMeshingComboCurrentIndex(f.page, 2));
    const auto stopped = transitionMeshingObject3D(f.model, f.page, 2, MeshingObjectEvent::Activated);
    CHECK(stopped.outcome == ApplyResult::UnsupportedSnapshot); CHECK(stopped.previousCommitted);
    CHECK(!stopped.controlsReloaded); CHECK(stopped.stage == MeshingObjectStage::PreviousCommitted);
    CHECK(f.page.objects[0].sizeRatio == 9.5); CHECK_EQ(f.page.storedObjectIndex, 0);
    CHECK(f.page.firstSizeRatio == qt::L("9.5")); CHECK_EQ(f.page.firstNodeCount, 9);
    checkBits(values(f.page.objects[2]), targetBits); checkBits(values(f.ten), modelBefore);
    f.remesh.enabled = 1;
    CHECK(applyMeshingSelection3D(f.model, f.remesh, f.page) == ApplyResult::UnsupportedSnapshot);
    CHECK_EQ(f.remesh.enabled, 1); checkBits(values(f.ten), modelBefore);
}

TEST_CASE(meshing_preflight_binds_event_index_and_all_old_controls_before_any_commit)
{
    Fixture f; const auto before = values(f.page.objects[0]);
    auto mismatched = f.page;
    const auto wrongEvent = transitionMeshingObject3D(f.model, mismatched, 1, MeshingObjectEvent::SaveCurrentIndex);
    CHECK(wrongEvent.outcome == ApplyResult::UnsupportedSnapshot); CHECK(!wrongEvent.previousCommitted);
    checkBits(values(mismatched.objects[0]), before);
    CHECK(!setMeshingComboCurrentIndex(mismatched, -2)); CHECK(!setMeshingComboCurrentIndex(mismatched, 3));
    CHECK_EQ(mismatched.comboObjectIndex, 0);
    CHECK(transitionMeshingObject3D(f.model, mismatched, 0, MeshingObjectEvent::InitialLoad).outcome == ApplyResult::UnsupportedSnapshot);
    mismatched.comboObjectIndex = -1;
    CHECK(transitionMeshingObject3D(f.model, mismatched, 0, MeshingObjectEvent::Activated).outcome == ApplyResult::UnsupportedSnapshot);
    for (int badControl = 0; badControl < 4; ++badControl) {
        auto page = f.page; page.firstSizeRatio = qt::L("9.5");
        if (badControl == 0) page.firstInterference = qt::L("inf");
        if (badControl == 1) page.firstSizeRatio = qt::L("nan");
        if (badControl == 2) page.firstTolerance = qt::L("inf");
        if (badControl == 3) page.firstNodeCount = -1;
        REQUIRE(setMeshingComboCurrentIndex(page, 1));
        const auto refused = transitionMeshingObject3D(f.model, page, 1, MeshingObjectEvent::Activated);
        CHECK(refused.outcome == ApplyResult::UnsupportedSnapshot); CHECK(!refused.previousCommitted);
        CHECK(refused.stage == MeshingObjectStage::Preflight); checkBits(values(page.objects[0]), before);
    }
    engine::GuiModel wrong(nullptr, 2);
    CHECK(transitionMeshingObject3D(wrong, f.page, 0, MeshingObjectEvent::Activated).outcome == ApplyResult::UnsupportedSnapshot);
    checkBits(values(f.page.objects[0]), before);
    f.page.remeshEnabled = true; f.page.remeshMethodRadio = 99;
    CHECK(applyMeshingSelection3D(f.model, f.remesh, f.page) == ApplyResult::UnsupportedSnapshot);
    checkBits(values(f.page.objects[0]), before); CHECK_EQ(f.remesh.enabled, 0);
}

TEST_CASE(meshing_const_first_form_compatibility_is_separate_from_mutable_selection_save)
{
    Fixture f; const auto visible = f.page.firstSizeRatio;
    CHECK(applyMeshing3D(f.model, f.remesh, f.page) == ApplyResult::Completed);
    CHECK(f.page.firstSizeRatio == visible); CHECK(f.page.objects[0].sizeRatio == 7.12345678);
    REQUIRE(setMeshingComboCurrentIndex(f.page, 1));
    CHECK(transitionMeshingObject3D(f.model, f.page, 1, MeshingObjectEvent::Activated).outcome == ApplyResult::Completed);
    seed(f.two, {99.0, 99.0, 99.0, 99.0});
    CHECK(applyMeshing3D(f.model, f.remesh, f.page) == ApplyResult::UnsupportedSnapshot);
    checkBits(values(f.two), {99.0, 99.0, 99.0, 99.0});
    CHECK(applyMeshingSelection3D(f.model, f.remesh, f.page) == ApplyResult::Completed);
    checkBits(values(f.two), {2.12346, 3.12346, 4.12346, 5.0});
}

TEST_CASE(meshing_selection_spin_conversion_and_missing_live_object_keep_original_order)
{
    Fixture f;
    f.page.objects[1].nodeCount = 2147483648.0; // Finite CVTTSD2SI invalid -> INT_MIN -> spin0.
    REQUIRE(setMeshingComboCurrentIndex(f.page, 1));
    CHECK(transitionMeshingObject3D(f.model, f.page, 1, MeshingObjectEvent::Activated).outcome == ApplyResult::Completed);
    CHECK_EQ(f.page.firstNodeCount, 0); CHECK(f.page.objects[1].nodeCount == 2147483648.0);
    REQUIRE(def::model::object_list_remove(&f.model.step->objects, f.ten));
    f.ten = nullptr; // Deleted native-adapter allocation is never used again.
    seed(f.two, {99.0, 99.0, 99.0, 99.0}); f.remesh.enabled = 1;
    f.page.remeshEnabled = false; f.page.remeshMethodRadio = 99;
    CHECK(applyMeshingSelection3D(f.model, f.remesh, f.page) == ApplyResult::Completed);
    CHECK(f.model.object(10) == nullptr); CHECK_EQ(f.remesh.enabled, 0);
    checkBits(values(f.two), {2.12346, 3.12346, 4.12346, 0.0});
    checkBits(values(f.thirty), values(f.page.objects[2]));
    engine::GuiModel empty(nullptr, 3); RemeshState remesh;
    auto page = fillMeshing3D(empty, remesh); REQUIRE(page); CHECK(page->objects.empty());
    CHECK_EQ(page->storedObjectIndex, -1); CHECK_EQ(page->comboObjectIndex, -1);
    CHECK(applyMeshingSelection3D(empty, remesh, *page) == ApplyResult::Completed);
}

namespace {
struct MxcsrFixture {
    unsigned entry = _mm_getcsr();
    bool active = true;
    void invalidMasked(bool masked) {
        // Test-only known masks for other exceptions; preserve rounding/DAZ/FTZ.
        // Clear pending flags before an unmasked-invalid test can begin.
        unsigned profile = (entry | 0x1f80u) & ~0x3fu;
        if (!masked) profile &= ~static_cast<unsigned>(_MM_MASK_INVALID);
        _mm_setcsr(profile);
    }
    void restore() {
        if (active) { _mm_setcsr(entry); active = false; }
    }
    ~MxcsrFixture() { restore(); }
};
constexpr std::array<std::uint64_t, 5> nonfiniteNodeBits{
    0x7ff0000000000000ULL, 0xfff0000000000000ULL, // +/- infinity
    0x7ff8000000000001ULL, 0xfff8000000000002ULL, // distinct signed quiet NaNs
    0x7ff0000000000001ULL}; // signaling NaN; classification must not use FP comparison.
}

TEST_CASE(meshing_masked_nonfinite_first_node_displays_zero_without_rewriting_raw_source)
{
    for (auto bits : nonfiniteNodeBits) {
        MxcsrFixture controls; controls.invalidMasked(true);
        engine::GuiModel model(nullptr, 3); auto* object = model.objectWrap(1, true, false);
        seed(object, {1.25, 2.5, 3.75, std::bit_cast<double>(bits)});
        RemeshState remesh; auto page = fillMeshing3D(model, remesh); REQUIRE(page);
        REQUIRE(page->objects.size() == 1);
        CHECK_EQ(page->storedObjectIndex, 0); CHECK_EQ(page->comboObjectIndex, 0);
        CHECK_EQ(page->firstNodeCount, 0);
        CHECK_EQ(std::bit_cast<std::uint64_t>(page->objects[0].nodeCount), bits);
        CHECK_EQ(std::bit_cast<std::uint64_t>(field<double>(object, 0x2dd8)), bits);
        CHECK(applyMeshingSelection3D(model, remesh, *page) == ApplyResult::Completed);
        CHECK_EQ(std::bit_cast<std::uint64_t>(page->objects[0].nodeCount), std::uint64_t{0});
        CHECK_EQ(std::bit_cast<std::uint64_t>(field<double>(object, 0x2dd8)), std::uint64_t{0});
        controls.restore(); CHECK_EQ(_mm_getcsr(), controls.entry);
    }
}

TEST_CASE(meshing_masked_nonfinite_target_node_keeps_payload_until_next_old_form_commit)
{
    for (auto bits : nonfiniteNodeBits) {
        MxcsrFixture controls; controls.invalidMasked(true);
        Fixture f;
        field<double>(f.two, 0x2dd8) = std::bit_cast<double>(bits);
        auto filled = fillMeshing3D(f.model, f.remesh); REQUIRE(filled); f.page = std::move(*filled);
        REQUIRE(setMeshingComboCurrentIndex(f.page, 1));
        const auto loaded = transitionMeshingObject3D(f.model, f.page, 1, MeshingObjectEvent::Activated);
        CHECK(loaded.outcome == ApplyResult::Completed); CHECK(loaded.previousCommitted); CHECK(loaded.controlsReloaded);
        CHECK_EQ(f.page.firstNodeCount, 0); CHECK_EQ(f.page.storedObjectIndex, 1);
        CHECK_EQ(std::bit_cast<std::uint64_t>(f.page.objects[1].nodeCount), bits);
        CHECK_EQ(std::bit_cast<std::uint64_t>(field<double>(f.two, 0x2dd8)), bits);
        const auto committed = transitionMeshingObject3D(f.model, f.page, 1, MeshingObjectEvent::SaveCurrentIndex);
        CHECK(committed.outcome == ApplyResult::Completed); CHECK(committed.sameRow); CHECK(committed.controlsReloaded);
        CHECK_EQ(std::bit_cast<std::uint64_t>(f.page.objects[1].nodeCount), std::uint64_t{0});
        CHECK_EQ(std::bit_cast<std::uint64_t>(field<double>(f.two, 0x2dd8)), bits); // Still no model copy.
        CHECK(applyMeshingSelection3D(f.model, f.remesh, f.page) == ApplyResult::Completed);
        CHECK_EQ(std::bit_cast<std::uint64_t>(field<double>(f.two, 0x2dd8)), std::uint64_t{0});
        controls.restore(); CHECK_EQ(_mm_getcsr(), controls.entry);
    }
}

TEST_CASE(meshing_programmatic_save_copies_new_nonfinite_target_raw_before_later_zero_commit)
{
    MxcsrFixture controls; controls.invalidMasked(true);
    Fixture f; constexpr auto bits = std::uint64_t{0x7ff8000000000042ULL};
    field<double>(f.two, 0x2dd8) = std::bit_cast<double>(bits);
    auto filled = fillMeshing3D(f.model, f.remesh); REQUIRE(filled); f.page = std::move(*filled);
    f.page.firstSizeRatio = qt::L("7.75"); REQUIRE(setMeshingComboCurrentIndex(f.page, 1));
    f.page.remeshEnabled = false; f.page.remeshMethodRadio = 99; f.remesh.enabled = 1;
    CHECK(applyMeshingSelection3D(f.model, f.remesh, f.page) == ApplyResult::Completed);
    CHECK(field<double>(f.ten, 0x2dd0) == 7.75); CHECK_EQ(f.remesh.enabled, 0);
    CHECK_EQ(f.page.firstNodeCount, 0);
    CHECK_EQ(std::bit_cast<std::uint64_t>(f.page.objects[1].nodeCount), bits);
    CHECK_EQ(std::bit_cast<std::uint64_t>(field<double>(f.two, 0x2dd8)), bits);
    CHECK(applyMeshingSelection3D(f.model, f.remesh, f.page) == ApplyResult::Completed);
    CHECK_EQ(std::bit_cast<std::uint64_t>(field<double>(f.two, 0x2dd8)), std::uint64_t{0});
    controls.restore(); CHECK_EQ(_mm_getcsr(), controls.entry);
}

TEST_CASE(meshing_unmasked_nonfinite_node_stops_after_old_commit_without_target_or_model_effects)
{
    MxcsrFixture controls; controls.invalidMasked(true);
    Fixture f; constexpr auto bits = std::uint64_t{0x7ff0000000000001ULL};
    field<double>(f.two, 0x2dd8) = std::bit_cast<double>(bits);
    auto filled = fillMeshing3D(f.model, f.remesh); REQUIRE(filled); f.page = std::move(*filled);
    f.page.firstSizeRatio = qt::L("9.5"); REQUIRE(setMeshingComboCurrentIndex(f.page, 1));
    const auto modelBefore = values(f.ten); f.remesh.enabled = 1;
    engine::GuiModel initial(nullptr, 3); auto* object = initial.objectWrap(1, true, false);
    seed(object, {1.25, 2.5, 3.75, std::bit_cast<double>(bits)});
    RemeshState initialRemesh;
    controls.invalidMasked(false);
    const auto stopped = transitionMeshingObject3D(f.model, f.page, 1, MeshingObjectEvent::Activated);
    const auto applied = applyMeshingSelection3D(f.model, f.remesh, f.page);
    const auto refusedInitial = fillMeshing3D(initial, initialRemesh);
    controls.restore(); // Restore BEFORE framework assertions or numeric formatting.
    CHECK_EQ(_mm_getcsr(), controls.entry);
    CHECK(stopped.outcome == ApplyResult::UnsupportedSnapshot); CHECK(stopped.previousCommitted);
    CHECK(stopped.stage == MeshingObjectStage::PreviousCommitted); CHECK(!stopped.controlsReloaded);
    CHECK(f.page.objects[0].sizeRatio == 9.5); CHECK_EQ(f.page.storedObjectIndex, 0);
    CHECK_EQ(f.page.comboObjectIndex, 1); CHECK_EQ(f.page.firstNodeCount, 9);
    CHECK(f.page.firstSizeRatio == qt::L("9.5"));
    CHECK_EQ(std::bit_cast<std::uint64_t>(f.page.objects[1].nodeCount), bits);
    CHECK(applied == ApplyResult::UnsupportedSnapshot); CHECK_EQ(f.remesh.enabled, 1);
    checkBits(values(f.ten), modelBefore);
    CHECK_EQ(std::bit_cast<std::uint64_t>(field<double>(f.two, 0x2dd8)), bits);
    CHECK(!refusedInitial);
}

int main() { return testkit::run_all(); }
