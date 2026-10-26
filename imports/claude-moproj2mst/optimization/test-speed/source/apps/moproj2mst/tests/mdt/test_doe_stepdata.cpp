// ctest-labels: m2m-mdt mdt-l6 m2m-mdt-l7
#include <testkit/check.hpp>

#include <defmodel/layout/db_step_data.hpp>

#include "m2m/common.hpp"
#include "m2m/doe_stepdata.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_slice.hpp"
#include "m2m/doe_view_stepdata.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine_scene.hpp"

using namespace m2m;
using qt::L;

namespace {
xml::NodePtr parse(const std::string& text)
{
    xml::Document doc;
    qt::String error;
    int line = 0, column = 0;
    REQUIRE(xml::setContent(doc, text, &error, &line, &column));
    return doc.root;
}

bool refuses(const auto& action, const char* contains)
{
    try { action(); }
    catch (const Error& error) {
        return error.code == M2M_E_UNSUPPORTED &&
            std::string(error.what()).find(contains) != std::string::npos;
    }
    return false;
}

void addEmptyView(engine::GuiModel& model)
{
    float minimum[3]{}, maximum[3]{};
    // viewerBox reports an allocated view, even when its DB object is absent.
    CHECK(engine::viewerBox(model, 1, minimum, maximum));
    CHECK(model.objectCount() == 0);
    CHECK(engine::viewerItemCount(model) == 1);
}
} // namespace

TEST_CASE(doe_stepdata_empty_view_finalization_refreshes_current_and_reset_reuses_owner)
{
    engine::GuiModel model(nullptr, 2);
    model.step->u1b4c = 7;
    doe::readStepData(model, *parse("<StepData/>"));
    REQUIRE(model.doeStepData != nullptr);
    const auto owner = model.doeStepData;
    CHECK(owner->propertyTableCreated());
    const auto* table = owner->propertyTable.get();
    CHECK(table->owner() == &model && table->constructorEmpty());
    CHECK(model.step->u1b4c == 0);
    CHECK(model.objectCount() == 0 && engine::viewerItemCount(model) == 0);
    doe::readStepData(model, *parse("<StepData/>"));
    CHECK(model.doeStepData == owner && owner->propertyTableCreated());
    CHECK(owner->propertyTable.get() == table);
    model.reset();
    CHECK(model.doeStepData == owner && !owner->propertyTableCreated());
    CHECK(!owner->propertyTable);
    doe::readStepData(model, *parse("<StepData/>"));
    CHECK(model.doeStepData == owner && owner->propertyTableCreated());
}

TEST_CASE(doe_stepdata_out_of_range_objects_skip_and_negative_current_id_is_retained)
{
    engine::GuiModel model(nullptr, 2);
    model.step->u1b4c = -1;
    doe::readStepData(model, *parse("<StepData>raw<Unknown/><Object No='0'/>"
        "<Object No='99'/><Object No='invalid'/></StepData>"));
    REQUIRE(model.doeStepData != nullptr);
    CHECK(model.doeStepData->propertyTableCreated());
    CHECK(model.step->u1b4c == -1);
    CHECK(model.objectCount() == 0 && engine::viewerItemCount(model) == 0);
}

TEST_CASE(doe_stepdata_negative_object_index_guards_before_finalization)
{
    engine::GuiModel model(nullptr, 2);
    model.step->u1b4c = 7;
    CHECK(refuses([&] { doe::readStepData(model, *parse("<StepData><Object No='-1'/></StepData>")); },
        "negative Object index"));
    CHECK(!model.doeStepData && model.step->u1b4c == 7);
}

TEST_CASE(doe_stepdata_object_bound_uses_viewer_count_instead_of_database_count)
{
    engine::GuiModel model(nullptr, 2);
    addEmptyView(model);
    model.step->u1b4c = 7;
    doe::readStepData(model, *parse("<StepData><Object/></StepData>"));
    REQUIRE(model.doeStepData != nullptr);
    CHECK(model.doeStepData->propertyTableCreated() && model.step->u1b4c == 0);
    CHECK(engine::viewerItemCount(model) == 1 && model.objectCount() == 0);
}

TEST_CASE(doe_stepdata_unbound_view_finalizer_skips_cells_on_actual_table)
{
    engine::GuiModel model(nullptr, 2);
    addEmptyView(model);
    model.step->u1b4c = 7;
    doe::readStepData(model, *parse("<StepData/>"));
    REQUIRE(model.doeStepData != nullptr);
    CHECK(model.doeStepData->propertyTableCreated() && model.step->u1b4c == 0);
    CHECK(engine::viewerItemCount(model) == 1 && model.objectCount() == 0);
    CHECK(model.doeStepData->propertyTable->owner() == &model);
    CHECK(model.doeStepData->propertyTable->queryCount() == 0);
}

TEST_CASE(doe_stepdata_statevar_encounter_guards_before_table_or_current_mutation)
{
    engine::GuiModel model(nullptr, 2);
    model.step->u1b4c = 7;
    CHECK(refuses([&] { doe::readStepData(model,
        *parse("<StepData><Object No='99'/><StateVar/><Object No='-1'/></StepData>")); },
        "StateVar"));
    CHECK(!model.doeStepData && model.step->u1b4c == 7);
}

TEST_CASE(doe_stepdata_actual_view_symmetry_persists_before_later_object_guard)
{
    engine::GuiModel model(nullptr, 2);
    addEmptyView(model);
    model.step->u1b4c = 7;
    auto* state = &engine::viewerStepDataState(model, 0);
    CHECK(refuses([&] { doe::readStepData(model, *parse("<StepData><Object No='0'>"
        "<SfSymmetryInfo ShowSymm='1'><NumOfSymmData>2</NumOfSymmData>"
        "<SfSymmetryData1><IsActive>1</IsActive></SfSymmetryData1></SfSymmetryInfo>"
        "<Slice/></Object></StepData>")); }, "Slice"));
    CHECK(&engine::viewerStepDataState(model, 0) == state);
    CHECK(state->showSymmetry && state->symmetryCount == 2 && state->symmetry[1].isActive);
    CHECK(!model.doeStepData && model.step->u1b4c == 7);
    CHECK(engine::viewerItemCount(model) == 1 && model.objectCount() == 0);
}

TEST_CASE(doe_stepdata_view_state_access_never_creates_or_grows_viewer)
{
    engine::GuiModel model(nullptr, 2);
    const auto before = model.viewer;
    CHECK(refuses([&] { (void)engine::viewerStepDataState(model, 0); }, "existing viewer item"));
    CHECK(model.viewer == before && engine::viewerItemCount(model) == 0);
    addEmptyView(model);
    CHECK(refuses([&] { (void)engine::viewerStepDataState(model, -1); }, "existing viewer item"));
    CHECK(refuses([&] { (void)engine::viewerStepDataState(model, 1); }, "existing viewer item"));
    CHECK(engine::viewerItemCount(model) == 1 && model.objectCount() == 0);
    const auto& state = engine::viewerStepDataState(model, 0);
    CHECK(!state.showSymmetry && state.symmetryCount == 0 && !state.symmetry[0].matrix);
}

TEST_CASE(doe_stepdata_repeated_objects_reuse_actual_view_before_finalization)
{
    engine::GuiModel model(nullptr, 2);
    addEmptyView(model);
    model.step->u1b4c = 7;
    doe::readStepData(model, *parse("<StepData><Object No='0'>"
        "<SfSymmetryInfo ShowSymm='1'><NumOfSymmData>8</NumOfSymmData></SfSymmetryInfo>"
        "</Object><Object No='99'><Slice/></Object><Object No='bad'><SfSymmetryInfo/>"
        "</Object></StepData>"));
    const auto& state = engine::viewerStepDataState(model, 0);
    CHECK(!state.showSymmetry && state.symmetryCount == 8 && !state.symmetry[0].matrix);
    REQUIRE(model.doeStepData != nullptr);
    CHECK(model.doeStepData->propertyTableCreated() && model.step->u1b4c == 0);
    CHECK(engine::viewerItemCount(model) == 1 && model.objectCount() == 0);
}

TEST_CASE(doe_stepdata_display_options_without_settings_skip_before_later_symmetry)
{
    engine::GuiModel model(nullptr, 2);
    addEmptyView(model); //viewerBox grows a view without creating Settings
    CHECK(!engine::viewerDisplayOptionSnapshot(model, 0));
    model.step->u1b4c = 7;
    CHECK(refuses([&] { doe::readStepData(model, *parse("<StepData><Object No='0'><SfDispOpt>"
        "<DispModeType>6</DispModeType></SfDispOpt><SfSymmetryInfo ShowSymm='1'>"
        "<NumOfSymmData>2</NumOfSymmData></SfSymmetryInfo><Slice/></Object></StepData>")); }, "Slice"));
    const auto& state = engine::viewerStepDataState(model, 0);
    CHECK(state.showSymmetry && state.symmetryCount == 2);
    CHECK(!model.doeStepData && model.step->u1b4c == 7);
    CHECK(!engine::viewerDisplayOptionSnapshot(model, 0) && engine::viewerItemCount(model) == 1);
}

TEST_CASE(doe_stepdata_no_database_slice_returns_then_reaches_actual_finalization)
{
    engine::GuiModel model(nullptr, 2);
    auto io = doe::makeRecordingStudyIo();
    auto runtime = std::make_shared<doe::SliceConstructorRuntime>();
    io->bindSliceConstructorRuntime(runtime);
    addEmptyView(model);
    model.step->u1b4c = 7;
    doe::readStepData(model, *parse("<StepData><Object><Slice><a>1</a>"
        "<b>0</b><c>0</c><d>-4</d></Slice><SfSymmetryInfo ShowSymm='1'/></Object></StepData>"),
        nullptr, io.get());
    const auto* collection = engine::viewerSliceState(model, 0);
    REQUIRE(collection != nullptr);
    CHECK(collection->viewIdentity && !collection->model.has_value() && !collection->boundObject);
    CHECK(collection->backendKnownNull && collection->graphicCount == 0);
    REQUIRE(runtime->colorScratchBits.has_value());
    CHECK((*runtime->colorScratchBits)[0] == 0x3f800000);
    CHECK(engine::viewerStepDataState(model, 0).showSymmetry);
    REQUIRE(model.doeStepData != nullptr);
    CHECK(model.doeStepData->propertyTableCreated() && model.step->u1b4c == 0);
    //All-zero normal does not read missing d. Allocation is reused, not replaced.
    doe::readStepData(model, *parse("<StepData><Object><Slice><a>0</a>"
        "<b>0</b><c>0</c></Slice></Object></StepData>"), nullptr, io.get());
    CHECK(engine::viewerSliceState(model, 0) == collection && engine::viewerItemCount(model) == 1);
}

TEST_CASE(doe_stepdata_unqualified_existing_table_does_not_guard_skipped_unbound_views)
{
    engine::GuiModel model(nullptr, 2);
    addEmptyView(model);
    doe::readStepData(model, *parse("<StepData/>"));
    const auto* table = model.doeStepData->propertyTable.get();
    model.doeStepData->propertyTable->markPopulationUnrecovered();
    model.step->u1b4c = 7;
    doe::readStepData(model, *parse("<StepData/>"));
    CHECK(model.doeStepData->propertyTable.get() == table);
    CHECK(!table->constructorEmpty() && table->queryCount() == 0);
    CHECK(model.step->u1b4c == 0 && engine::viewerItemCount(model) == 1);
}

TEST_CASE(doe_stepdata_later_object_refusal_keeps_prior_actual_table_without_finalizing_again)
{
    engine::GuiModel model(nullptr, 2);
    addEmptyView(model);
    doe::readStepData(model, *parse("<StepData/>"));
    const auto* table = model.doeStepData->propertyTable.get();
    model.step->u1b4c = 7;
    CHECK(refuses([&] { doe::readStepData(model, *parse("<StepData><Object No='0'>"
        "<SfSymmetryInfo ShowSymm='1'/></Object><Object No='-1'/></StepData>")); },
        "negative Object index"));
    CHECK(model.doeStepData->propertyTable.get() == table && table->queryCount() == 0);
    CHECK(engine::viewerStepDataState(model, 0).showSymmetry && model.step->u1b4c == 7);
}

TEST_CASE(doe_stepdata_display_options_without_settings_and_symmetry_complete_finalization)
{
    engine::GuiModel model(nullptr, 2);
    addEmptyView(model);
    model.step->u1b4c = 7;
    doe::readStepData(model, *parse("<StepData><Object><SfDispOpt>"
        "<DispModeType>6</DispModeType></SfDispOpt><SfSymmetryInfo ShowSymm='1'>"
        "<NumOfSymmData>2</NumOfSymmData></SfSymmetryInfo></Object></StepData>"));
    const auto& view = engine::viewerStepDataState(model, 0);
    CHECK(view.showSymmetry && view.symmetryCount == 2);
    CHECK(!engine::viewerDisplayOptionSnapshot(model, 0));
    REQUIRE(model.doeStepData && model.doeStepData->propertyTable);
    CHECK(model.doeStepData->propertyTable->queryCount() == 0 && model.step->u1b4c == 0);
    CHECK(engine::viewerItemCount(model) == 1 && model.objectCount() == 0);
}

TEST_CASE(doe_stepdata_empty_object_writer_leaves_current_and_property_table_untouched)
{
    engine::GuiModel model(nullptr, 2);
    model.step->u1b4c = 7;
    auto node = parse("<StepData><Earlier/></StepData>");
    engine::writeViewerStepDataObjects(model, *node);
    CHECK(node->childElements().size() == 1 && node->childElements()[0]->tag == qt::L("Earlier"));
    CHECK(!model.doeStepData && model.step->u1b4c == 7 && engine::viewerItemCount(model) == 0);
}

TEST_CASE(doe_stepdata_no_database_empty_slice_writes_empty_object_without_unknown_matrix)
{
    engine::GuiModel model(nullptr, 2);
    //The binding is weak; retain the actual shared runtime through this import.
    const auto runtime = std::make_shared<doe::SliceConstructorRuntime>();
    auto retainedIo = doe::makeRecordingStudyIo();
    retainedIo->bindSliceConstructorRuntime(runtime);
    addEmptyView(model);
    doe::readStepData(model, *parse("<StepData><Object><Slice><a>0</a><b>0</b><c>0</c>"
        "</Slice></Object></StepData>"), nullptr, retainedIo.get());
    const auto* collection = engine::viewerSliceState(model, 0);
    REQUIRE(collection && collection->graphicCount == 0);
    const auto* table = model.doeStepData->propertyTable.get();
    auto node = parse("<StepData/>");
    engine::writeViewerStepDataObjects(model, *node);
    const auto objects = node->childElements();
    REQUIRE(objects.size() == 1);
    CHECK(objects[0]->tag == qt::L("Object") && objects[0]->attribute(qt::L("No")) == qt::L("0"));
    CHECK(objects[0]->childElements().empty());
    CHECK(!engine::viewerStepDataState(model, 0).symmetry[0].matrix);
    CHECK(engine::viewerSliceState(model, 0) == collection && model.doeStepData->propertyTable.get() == table);
    CHECK(!engine::viewerDisplayOptionSnapshot(model, 0) && engine::viewerItemCount(model) == 1);
}

TEST_CASE(doe_stepdata_object_writer_appends_only_completed_objects_before_matrix_guard)
{
    engine::GuiModel model(nullptr, 2);
    float minimum[3]{}, maximum[3]{};
    CHECK(engine::viewerBox(model, 2, minimum, maximum));
    REQUIRE(engine::viewerItemCount(model) == 2);
    engine::viewerStepDataState(model, 1).symmetryCount = 1;
    auto node = parse("<StepData/>");
    CHECK(refuses([&] { engine::writeViewerStepDataObjects(model, *node); }, "matrix"));
    const auto objects = node->childElements();
    REQUIRE(objects.size() == 1);
    CHECK(objects[0]->attribute(qt::L("No")) == qt::L("0") && objects[0]->childElements().empty());
    CHECK(!model.doeStepData && engine::viewerItemCount(model) == 2);
}

int main() { return testkit::run_all(); }
