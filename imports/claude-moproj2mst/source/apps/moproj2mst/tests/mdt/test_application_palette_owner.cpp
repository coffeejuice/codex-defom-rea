// ctest-labels: m2m-mdt
// Isolated model of QApplicationPrivate::app_pal; not wired into SplitView.
#include <testkit/check.hpp>
#include <memory>
#include <string>
#include "m2m/doe_session_notifications.hpp"
#include "m2m/engine/application_palette_owner.hpp"
#include "m2m/engine/view_manager.hpp"

using namespace m2m::engine;

namespace {
constexpr const char* qualification = "CP77 isolated QApplication app_pal input";

struct PaletteFixture {
    m2m::doe::SessionNotificationState notifications;
    std::shared_ptr<MainViewState> main = std::make_shared<MainViewState>();
    std::shared_ptr<ViewManagerState> manager;
    std::shared_ptr<WidgetRuntimeState> runtime =
        std::make_shared<WidgetRuntimeState>(true, "CP77 isolated palette runtime");

    PaletteFixture()
    {
        manager = main->constructManagerChild(notifications);
        main->bindWidgetRuntime(runtime);
        WidgetRuntimeValueObservation values;
        values.mainView = main;
        values.manager = manager;
        values.qualification = qualification;
        values.paletteData = {{17}};
        values.applicationPalette.defaultValue = WidgetPaletteValueObservation{17, 0x12345678u};
        values.mainViewValues.palette = WidgetPaletteValueObservation{17, 0x12345678u};
        runtime->observeStartupValues(values);
    }

    WidgetPaletteValue value() const
    {
        auto current = runtime->applicationPalette();
        REQUIRE(current.has_value());
        return *current;
    }
};
}

TEST_CASE(qapplication_palette_unknown_slot_does_not_guess_or_call_constructor)
{
    QApplicationPaletteOwnerState owner(qualification);
    int calls = 0;
    const auto result = owner.read([&](std::int32_t) -> std::optional<WidgetPaletteValue> {
        ++calls;
        return std::nullopt;
    });
    CHECK(result.issue == QApplicationPaletteReadIssue::SlotStateUnknown);
    CHECK(result.effects.empty());
    CHECK(!result.output);
    CHECK(calls == 0);
    CHECK(owner.slotKnowledge() == QApplicationPaletteSlotKnowledge::Unknown);
    CHECK(!owner.wrapperOwner());
}

TEST_CASE(qapplication_palette_known_null_constructs_and_caches_once)
{
    PaletteFixture fixture;
    const auto seeded = fixture.value();
    QApplicationPaletteOwnerState owner(qualification);
    REQUIRE(owner.observeNullSlot(qualification));
    CHECK(owner.slotKnowledge() == QApplicationPaletteSlotKnowledge::KnownNull);

    int calls = 0;
    const auto first = owner.read([&](std::int32_t globalColor)
        -> std::optional<WidgetPaletteValue> {
        ++calls;
        CHECK(globalColor == QApplicationPaletteOwnerState::nativeDefaultGlobalColorArgument);
        return seeded;
    });
    CHECK(first.issue == QApplicationPaletteReadIssue::None);
    REQUIRE(first.output.has_value());
    CHECK(first.output->data == seeded.data);
    CHECK(first.output->groupResolveWord == 0x12345678u);
    CHECK(first.effects.size() == 4u);
    CHECK(first.effects[0] == QApplicationPaletteEffect::WrapperAllocated);
    CHECK(first.effects[1] == QApplicationPaletteEffect::GlobalColorPaletteConstructed);
    CHECK(first.effects[2] == QApplicationPaletteEffect::SlotCached);
    CHECK(first.effects[3] == QApplicationPaletteEffect::OutputCopied);
    CHECK(calls == 1);
    CHECK(owner.slotKnowledge() == QApplicationPaletteSlotKnowledge::KnownPresent);
    const auto cached = owner.wrapperOwner();
    REQUIRE(cached);
    CHECK(cached->value.data == seeded.data);

    const auto second = owner.read([&](std::int32_t) -> std::optional<WidgetPaletteValue> {
        ++calls;
        return std::nullopt;
    });
    CHECK(second.issue == QApplicationPaletteReadIssue::None);
    REQUIRE(second.output.has_value());
    CHECK(second.output->data == seeded.data);
    CHECK(second.output->groupResolveWord == 0x12345678u);
    CHECK(second.effects.size() == 1u);
    CHECK(second.effects[0] == QApplicationPaletteEffect::OutputCopied);
    CHECK(calls == 1);
    CHECK(owner.wrapperOwner() == cached);
}

TEST_CASE(qapplication_palette_known_present_copies_without_initialization)
{
    PaletteFixture fixture;
    const auto seeded = fixture.value();
    QApplicationPaletteOwnerState owner(qualification);
    REQUIRE(owner.observePresentSlot(qualification, seeded));
    const auto cached = owner.wrapperOwner();
    REQUIRE(cached);

    int calls = 0;
    const auto result = owner.read([&](std::int32_t) -> std::optional<WidgetPaletteValue> {
        ++calls;
        return std::nullopt;
    });
    CHECK(result.issue == QApplicationPaletteReadIssue::None);
    REQUIRE(result.output.has_value());
    CHECK(result.output->data == seeded.data);
    CHECK(result.output->groupResolveWord == 0x12345678u);
    CHECK(result.effects.size() == 1u);
    CHECK(result.effects[0] == QApplicationPaletteEffect::OutputCopied);
    CHECK(calls == 0);
    CHECK(owner.wrapperOwner() == cached);
}

TEST_CASE(qapplication_palette_owner_rejects_unqualified_or_conflicting_slot_observations)
{
    PaletteFixture fixture;
    const auto seeded = fixture.value();
    QApplicationPaletteOwnerState unqualified("");
    CHECK(!unqualified.observeNullSlot(qualification));
    CHECK(unqualified.read().issue == QApplicationPaletteReadIssue::QualificationUnavailable);

    QApplicationPaletteOwnerState owner(qualification);
    CHECK(!owner.observeNullSlot("different app_pal owner"));
    CHECK(owner.slotKnowledge() == QApplicationPaletteSlotKnowledge::Unknown);
    REQUIRE(owner.observePresentSlot(qualification, seeded));
    CHECK(!owner.observeNullSlot(qualification));
    CHECK(owner.slotKnowledge() == QApplicationPaletteSlotKnowledge::KnownPresent);
    CHECK(owner.wrapperOwner()->value.data == seeded.data);
}

int main() { return testkit::run_all(); }
