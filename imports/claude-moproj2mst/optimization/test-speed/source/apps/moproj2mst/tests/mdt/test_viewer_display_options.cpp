// ctest-labels: m2m-mdt mdt-viewer
#include <testkit/check.hpp>
#include <cstdint>
#include <cstring>
#include <string>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include "m2m/common.hpp"
#include "m2m/doe_stepdata.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine_scene.hpp"

using namespace m2m;
namespace {
xml::NodePtr parse(const std::string& text)
{
    xml::Document doc;
    qt::String error;
    int line = 0, column = 0;
    REQUIRE(xml::setContent(doc, text, &error, &line, &column));
    return doc.root;
}
bool refuses(const auto& action, const char* fragment)
{
    try { action(); }
    catch (const Error& e) {
        return e.code == M2M_E_UNSUPPORTED && std::string(e.what()).find(fragment) != std::string::npos;
    }
    return false;
}
void setObjectNumber(engine::LLOBJ_DATA& object, std::int32_t number)
{
    std::memcpy(reinterpret_cast<char*>(&object) + 8, &number, sizeof number);
}
engine::LLOBJ_DATA* addBoundView(engine::GuiModel& model, int number)
{
    auto* object = model.objectWrap(number, true, false);
    REQUIRE(object != nullptr);
    engine::viewerSync(model, object, true);
    REQUIRE(engine::viewerDisplayOptionSnapshot(model, number - 1).has_value());
    return object;
}
}

TEST_CASE(view_display_options_lookup_never_grows_viewer_or_missing_settings)
{
    engine::GuiModel model(nullptr, 2);
    const auto before = model.viewer;
    CHECK(!engine::viewerDisplayOptionSnapshot(model, 0));
    CHECK(refuses([&] { engine::readViewerDisplayOptions(model, 0, *parse("<SfDispOpt/>")); }, "existing viewer item"));
    CHECK(model.viewer == before && engine::viewerItemCount(model) == 0);
    float minimum[3]{}, maximum[3]{};
    CHECK(engine::viewerBox(model, 1, minimum, maximum));
    CHECK(!engine::viewerDisplayOptionSnapshot(model, 0));
    engine::readViewerDisplayOptions(model, 0, *parse("<SfDispOpt><DispModeType>9</DispModeType></SfDispOpt>"));
    CHECK(!engine::viewerDisplayOptionSnapshot(model, 0));
    CHECK(engine::viewerItemCount(model) == 1 && model.objectCount() == 0);
}

TEST_CASE(view_display_options_actual_settings_keep_effects_before_later_slice_guard)
{
    engine::GuiModel model(nullptr, 2);
    addBoundView(model, 1);
    model.step->u1b4c = 7;
    CHECK(refuses([&] { doe::readStepData(model, *parse("<StepData><Object No='0'><SfDispOpt>"
        "<DispModeType>7</DispModeType><DispFlagList ObjectOn='1' MeshOn='1' GeoOn='1'/>"
        "</SfDispOpt><Slice/></Object></StepData>")); }, "Slice"));
    auto settings = engine::viewerDisplayOptionSnapshot(model, 0);
    REQUIRE(settings.has_value());
    CHECK(settings->mode == 7 && settings->flags[0] == 0 && settings->flags[1] == 1);
    CHECK(settings->flags[2] == 1 && settings->flags[3] == 1);
    CHECK(settings->cachedMesh == 1 && settings->cachedGeometry == 1);
    CHECK(settings->flags[8] == 0 && settings->flags[9] == 0);
    CHECK(!model.doeStepData && model.step->u1b4c == 7);
}

TEST_CASE(view_display_options_each_element_resets_actual_settings_then_updates_cached_flags)
{
    engine::GuiModel model(nullptr, 2);
    addBoundView(model, 1);
    doe::readStepData(model, *parse("<StepData><Object No='0'>"
        "<SfDispOpt><DispModeType>7</DispModeType><DispFlagList MeshOn='1' GeoOn='1'/></SfDispOpt>"
        "<SfDispOpt/></Object></StepData>"));
    auto settings = engine::viewerDisplayOptionSnapshot(model, 0);
    REQUIRE(settings.has_value());
    CHECK(settings->mode == 0 && settings->flags[0] == 1 && settings->flags[1] == 1);
    CHECK(settings->flags[2] == 0 && settings->flags[3] == 0);
    CHECK(settings->cachedMesh == 0 && settings->cachedGeometry == 0);
    CHECK(settings->flags[8] == 1 && settings->flags[9] == 1);
    CHECK(settings->preferenceProfile == engine::ViewerDisplayPreferenceProfile::FreshCrtDefaults);
    REQUIRE(model.doeStepData != nullptr);
    CHECK(model.doeStepData->propertyTableCreated());
}

TEST_CASE(view_display_options_use_bound_object_number_before_view_position)
{
    engine::GuiModel model(nullptr, 2);
    auto* first = addBoundView(model, 1);
    addBoundView(model, 2);
    setObjectNumber(*first, 2);
    engine::readViewerDisplayOptions(model, 0, *parse("<SfDispOpt><DispModeType>44</DispModeType>"
        "<DispFlagList MeshOn='1'/></SfDispOpt>"));
    setObjectNumber(*first, 1);
    const auto firstSettings = engine::viewerDisplayOptionSnapshot(model, 0);
    const auto secondSettings = engine::viewerDisplayOptionSnapshot(model, 1);
    REQUIRE(firstSettings && secondSettings);
    CHECK(firstSettings->mode == 0 && secondSettings->mode == 44);
    CHECK(firstSettings->cachedMesh == 0 && secondSettings->cachedMesh == 1);
}

TEST_CASE(view_display_options_settings_without_bound_object_refuse_before_reset)
{
    engine::GuiModel model(nullptr, 2);
    addBoundView(model, 1);
    engine::readViewerDisplayOptions(model, 0, *parse("<SfDispOpt><DispModeType>27</DispModeType></SfDispOpt>"));
    engine::viewerClear(model); // original retains Settings while deleting views
    float minimum[3]{}, maximum[3]{};
    CHECK(engine::viewerBox(model, 1, minimum, maximum));
    CHECK(refuses([&] { engine::readViewerDisplayOptions(model, 0, *parse("<SfDispOpt/>")); }, "no bound object"));
    auto settings = engine::viewerDisplayOptionSnapshot(model, 0);
    REQUIRE(settings.has_value());
    CHECK(settings->mode == 27);
}

int main() { return testkit::run_all(); }
