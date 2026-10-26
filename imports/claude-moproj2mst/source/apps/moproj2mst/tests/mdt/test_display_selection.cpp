// ctest-labels: m2m-mdt isolated-close-effects
#include <testkit/check.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include "m2m/engine.hpp"
#include "m2m/engine_auxiliary.hpp"
#include "m2m/engine_scene.hpp"
#include "m2m/mst/writer.hpp"

using namespace m2m;
using namespace m2m::engine;

TEST_CASE(selected_model_override_survives_main_change_and_unmatched_selection_clears_it)
{
    Project project;
    GuiModel first(nullptr, 3), second(nullptr, 3), unregistered(nullptr, 3);
    displayShow(project, &first);
    auto& display = *project.display;
    CHECK(displaySelectedModel(display) == &first);
    CHECK(selectDisplayModel(display, &first) == &first);
    displayShow(project, &second);
    CHECK(displayMain(display) == &second);
    CHECK(displaySelectedModel(display) == &first); // main setter does not replace +1e0
    CHECK(selectDisplayModel(display, &unregistered) == &first);
    CHECK(displaySelectedModel(display) == &second);
    CHECK(selectDisplayModel(display, &second) == &second);
    CHECK(displayMainShown(display));
}

TEST_CASE(selected_guard_expires_with_model_even_after_its_display_backlink_changes)
{
    Project project, other;
    auto selected = std::make_unique<GuiModel>(nullptr, 3);
    GuiModel fallback(nullptr, 3);
    GuiModelGuard observer(selected.get());
    displayShow(project, selected.get());
    selectDisplayModel(*project.display, selected.get());
    displayShow(project, &fallback);
    displayShow(other, selected.get()); // one backlink; old Display guard must remain independent
    CHECK(displaySelectedModel(*project.display) == selected.get());
    selected->reset();
    CHECK(observer.get() == selected.get()); // reset is not QObject destruction
    selected.reset();
    CHECK(observer.get() == nullptr);
    CHECK(displaySelectedModel(*project.display) == &fallback);
    CHECK(displayMain(*other.display) == nullptr);
}

TEST_CASE(null_selection_clears_override_and_still_sets_the_null_third_slot_flag)
{
    Project project;
    GuiModel first(nullptr, 3), second(nullptr, 3);
    displayShow(project, &first);
    selectDisplayModel(*project.display, &first);
    displayShow(project, &second);
    auto& models = displayModels(*project.display);
    CHECK(!models.thirdShown && !models.secondShown);
    CHECK(selectDisplayModel(*project.display, nullptr) == &first);
    CHECK(displaySelectedModel(*project.display) == &second);
    CHECK(models.thirdShown && !models.secondShown);
}

TEST_CASE(main_guard_expires_in_every_display_without_changing_model_ownership)
{
    Project first, second;
    auto model = std::make_unique<GuiModel>(nullptr, 3);
    displayShow(first, model.get());
    displayShow(second, model.get());
    CHECK(displayMain(*first.display) == model.get());
    CHECK(displayMain(*second.display) == model.get());
    model.reset(); // only second is in model+10, but both native QPointers expire
    CHECK(displayMain(*first.display) == nullptr);
    CHECK(displaySelectedModel(*first.display) == nullptr);
    CHECK(displayMain(*second.display) == nullptr);
}

TEST_CASE(selection_named_then_third_then_main_then_second_preserves_other_flags)
{
    Project project;
    GuiModel main(nullptr, 3);
    displayShow(project, &main);
    auto& display = *project.display;
    auto* named = displayNamedModel(display, qt::L("ROIs"));
    auto& models = displayModels(display);
    models.third.reset(named); // named match must win over third alias
    CHECK(selectDisplayModel(display, named) == &main);
    CHECK(displaySelectedModel(display) == named);
    REQUIRE(models.named.size() == 1);
    CHECK(models.named.front().flag && !models.thirdShown);
    models.third.reset();
    auto* second = displaySecond(display);
    REQUIRE(second != nullptr);
    CHECK(selectDisplayModel(display, second) == named);
    CHECK(displaySelectedModel(display) == second);
    CHECK(models.secondShown && models.named.front().flag);
    CHECK(!models.keepSecond);
    displayDropSecond(display);
    CHECK(models.second.get() == nullptr);
    CHECK(displaySelectedModel(display) == &main); // guard expires with the actual owned model
    CHECK(models.secondShown); // deletion does not invoke the flag reset helper
}

TEST_CASE(display_second_uses_default_dimension_when_main_step_is_absent_and_reuses_owned_model)
{
    Project project;
    auto main = std::make_unique<GuiModel>(nullptr, 2);
    displayShow(project, main.get());
    auto& display = *project.display;
    auto& models = displayModels(display);
    main->step.reset();
    CHECK(displayMain(display) == main.get());
    CHECK(!main->step);

    GuiModel* second = displaySecond(display);
    REQUIRE(second != nullptr);
    CHECK_EQ(second->dimension(), 3);
    CHECK(second->display == &display);
    CHECK(models.second.get() == second);
    CHECK(models.secondOwned.get() == second);
    CHECK(displayMain(display) == main.get());
    CHECK(!main->step);

    CHECK(displaySecond(display) == second);
    CHECK(models.secondOwned.get() == second);
    main.reset();
    CHECK(displayMain(display) == nullptr);
    CHECK(displaySecond(display) == second);

    displayDropSecond(display);
    CHECK(models.second.get() == nullptr);
    CHECK(models.secondOwned == nullptr);
    CHECK(displaySecond(display) == nullptr);
    CHECK(models.secondOwned == nullptr);
}

TEST_CASE(display_second_preserves_live_dimensions_including_present_zero)
{
    for (const int dimension : {2, 3, 0}) {
        Project project;
        GuiModel main(nullptr, dimension);
        displayShow(project, &main);
        auto& display = *project.display;

        GuiModel* second = displaySecond(display);
        REQUIRE(second != nullptr);
        CHECK_EQ(second->dimension(), dimension);
        CHECK(displaySecond(display) == second);
        CHECK(displayModels(display).secondOwned.get() == second);
    }
}

TEST_CASE(named_model_copies_live_main_dimension_and_existing_name_precedes_missing_step_refusal)
{
    for (const int dimension : {2, 3, 0}) {
        Project project;
        GuiModel main(nullptr, dimension);
        displayShow(project, &main);
        auto& display = *project.display;

        auto* named = displayNamedModel(display, qt::L("DimensionProbe"));
        REQUIRE(named != nullptr);
        CHECK_EQ(named->dimension(), dimension);
        CHECK(named->display == &display);
        REQUIRE(displayModels(display).named.size() == 1);
        CHECK(displayNamedModel(display, qt::L("DimensionProbe")) == named);
        CHECK_EQ(displayModels(display).named.size(), 1u);

        main.step.reset();
        CHECK(displayNamedModel(display, qt::L("DimensionProbe")) == named);
        bool refused = false;
        try {
            (void)displayNamedModel(display, qt::L("NewName"));
        } catch (const Error& error) {
            refused = true;
            CHECK_EQ(error.code, M2M_E_UNSUPPORTED);
        }
        CHECK(refused);
    }
}

TEST_CASE(third_alias_wins_over_main_and_main_wins_over_second)
{
    Project project;
    GuiModel main(nullptr, 3), next(nullptr, 3);
    displayShow(project, &main);
    auto& display = *project.display;
    auto& models = displayModels(display);
    models.third.reset(&main);
    models.second.reset(&main); // explicit borrowed aliases at this selection boundary
    CHECK(selectDisplayModel(display, &main) == &main);
    CHECK(models.thirdShown && !models.secondShown);
    displayShow(project, &next);
    CHECK(displaySelectedModel(display) == &main);
    models.third.reset();
    models.second.reset(&next);
    CHECK(selectDisplayModel(display, &next) == &main);
    CHECK(displayMainShown(display) && !models.secondShown);
    CHECK(displaySelectedModel(display) == &next);
}

TEST_CASE(secondary_guards_expire_even_when_the_model_backlink_points_elsewhere)
{
    Project project, other;
    GuiModel main(nullptr, 3);
    auto borrowed = std::make_unique<GuiModel>(nullptr, 3);
    displayShow(project, &main);
    auto& display = *project.display;
    auto& models = displayModels(display);
    models.second.reset(borrowed.get());
    models.third.reset(borrowed.get());
    displayShow(other, borrowed.get());
    selectDisplayModel(display, borrowed.get());
    CHECK(displaySelectedModel(display) == borrowed.get());
    borrowed.reset();
    CHECK(models.second.get() == nullptr && models.third.get() == nullptr);
    CHECK(displaySelectedModel(display) == &main);
    CHECK(models.thirdShown); // lifetime clearing does not erase the shown flag
}

TEST_CASE(null_named_holder_precedes_null_third_and_repeated_selection_retains_flags)
{
    Project project;
    GuiModel main(nullptr, 3), unregistered(nullptr, 3);
    displayShow(project, &main);
    auto& display = *project.display;
    auto& models = displayModels(display);
    displayNamedModel(display, qt::L("ROIs"));
    models.named.front().model.reset(); // declared holder whose guarded model has died
    CHECK(selectDisplayModel(display, nullptr) == &main);
    CHECK(models.named.front().flag && !models.thirdShown);
    CHECK(displaySelectedModel(display) == &main);
    CHECK(selectDisplayModel(display, &unregistered) == &main);
    CHECK(models.named.front().flag && !models.thirdShown);
}

TEST_CASE(auxiliary_registry_is_lazy_preserves_duplicates_and_does_not_clear_on_model_reset)
{
    GuiModel model(nullptr, 3);
    int first = 1, second = 2, third = 3;
    CHECK(!model.auxiliaryDrawings);
    auto& registry = modelAuxiliaryDrawings(model);
    REQUIRE(model.auxiliaryDrawings);
    CHECK(registry.entries.empty());
    registry.entries = {&first, &second, &first}; // declared imported/native list state
    registerAuxiliaryDrawing(model, &first);
    CHECK(registry.entries.size() == 3); // suppress another duplicate, do not normalize existing ones
    registerAuxiliaryDrawing(model, &third);
    REQUIRE(registry.entries.size() == 4);
    CHECK(registry.entries.back() == &third);
    auto* identity = model.auxiliaryDrawings.get();
    model.reset();
    CHECK(model.auxiliaryDrawings.get() == identity && registry.entries.size() == 4);
    clearAuxiliaryDrawings(model);
    CHECK(!model.auxiliaryDrawings);
    clearAuxiliaryDrawings(model);
    CHECK(modelAuxiliaryDrawings(model).entries.empty());
}

TEST_CASE(auxiliary_registry_does_not_own_pointees_or_prune_deleted_object_identities)
{
    struct Probe {
        explicit Probe(bool& flag) : destroyed(flag) {}
        bool& destroyed;
        ~Probe() { destroyed = true; }
    };
    bool destroyed = false;
    auto pointee = std::make_unique<Probe>(destroyed);
    {
        GuiModel model(nullptr, 3);
        registerAuxiliaryDrawing(model, pointee.get());
        clearAuxiliaryDrawings(model);
        CHECK(!destroyed);
        registerAuxiliaryDrawing(model, pointee.get());
        pointee.reset();
        CHECK(destroyed);
        REQUIRE(model.auxiliaryDrawings);
        CHECK(model.auxiliaryDrawings->entries.size() == 1); // retained raw address, never dereferenced
    }
}

int main() { return testkit::run_all(); }
