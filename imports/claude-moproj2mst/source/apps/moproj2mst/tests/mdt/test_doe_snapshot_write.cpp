// ctest-labels: m2m-mdt doe-snapshot-write
#include <testkit/check.hpp>

#include <memory>
#include <string>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_statevar.hpp"
#include "m2m/doe_stepdata.hpp"
#include "m2m/engine.hpp"

using namespace m2m;
using qt::L;
namespace {
template<class F> bool refuses(F&& action, const char* detail)
{
    try { action(); }
    catch (const Error& error) {
        return error.code == M2M_E_UNSUPPORTED && std::string(error.what()).find(detail) != std::string::npos;
    }
    return false;
}
doe::StateVarConstructorContext profile()
{
    doe::StateVarConstructorContext out;
    out.profileIdentity = "declared adapter snapshot fixture; not observed original settings";
    out.globals = doe::StateVarGlobalValues{};
    out.globals->colorbarNo = 23;
    out.globals->palette.codes[0] = 71;
    out.deflectionScaleType.observation = doe::SettingObservation::Absent;
    out.deflectionScale.observation = doe::SettingObservation::Absent;
    out.deflectionLineThickness.observation = doe::SettingObservation::Absent;
    out.showDeflectionReference.observation = doe::SettingObservation::Absent;
    out.transparentDeflectionReference.observation = doe::SettingObservation::Absent;
    return out; //no translation/display/colorbar snapshot: writer does not apply plots
}
std::shared_ptr<doe::StateVarState> existing(engine::GuiModel& model)
{
    model.doeStepData = std::make_shared<doe::StepDataState>();
    model.doeStepData->stateVar = std::make_shared<doe::StateVarState>();
    return model.doeStepData->stateVar;
}
qt::String text(const xml::Node& node, const char* name)
{
    const auto children = node.childElements(L(name));
    REQUIRE(children.size() == 1u);
    return children[0]->text();
}
}

TEST_CASE(doe_snapshot_statevar_exact_order_signed_scalars_and_g6_doubles_without_apply)
{
    engine::GuiModel model(nullptr, 3);
    auto io = doe::makeRecordingStudyIo();
    auto state = existing(model);
    state->colorbarNo = -4; state->type = -5; state->plotVar = 777;
    state->plotCompNo = -6; state->plotType = 9; state->rangeType = -7;
    state->rangeMin = 1.23456789; state->rangeMax = -12.3456789;
    state->minimum = -2.5; state->maximum = 1234567;
    state->plotApplyInProgress = true;
    state->postparsePending = true;
    state->auxiliaryPointerDangling = true; //unconsumed by this writer
    state->cachedDataPresent = true;
    auto node = xml::createElement(L("StateVar"));
    doe::writeStateVar(model, *node, nullptr, *io);
    const std::vector<qt::String> names{L("SVColorBarNo"), L("SVType"), L("SVPlotVar"),
        L("SVPlotCompNo"), L("SVPlotType"), L("SVRangeType"), L("SVRangeMin"),
        L("SVRangeMax"), L("SVMin"), L("SVMax")};
    REQUIRE(node->children.size() == names.size());
    for (std::size_t i = 0; i < names.size(); ++i) CHECK(node->children[i]->tag == names[i]);
    CHECK(text(*node, "SVColorBarNo") == L("-4"));
    CHECK(text(*node, "SVPlotVar") == L("777"));
    CHECK(text(*node, "SVRangeMin") == L("1.23457"));
    CHECK(text(*node, "SVRangeMax") == L("-12.3457"));
    CHECK(text(*node, "SVMin") == L("-2.5"));
    CHECK(text(*node, "SVMax") == L("1.23457e+06"));
    CHECK(state->plotApplyInProgress && state->postparsePending && state->auxiliaryPointerDangling);
    CHECK(state->cachedDataPresent);
    CHECK_EQ(state->plotVar, 777);
}

TEST_CASE(doe_snapshot_lazy_statevar_constructor_profile_and_histogram_registrations_once)
{
    engine::GuiModel model(nullptr, 2);
    auto io = doe::makeRecordingStudyIo();
    auto context = profile();
    auto node = xml::createElement(L("StateVar"));
    doe::writeStateVar(model, *node, &context, *io);
    REQUIRE(model.doeStepData && model.doeStepData->stateVar);
    const auto state = model.doeStepData->stateVar;
    REQUIRE(state->histogram != nullptr);
    CHECK(!model.doeStepData->propertyTableCreated());
    CHECK(text(*node, "SVColorBarNo") == L("23"));
    CHECK_EQ(state->type, 1); //constructor, not var0 postparse type2
    CHECK_EQ(state->rangeMax, 0.0);
    CHECK(!state->histogram->graphOn);
    auto& runtime = io->stateVarRuntime();
    REQUIRE(runtime.palette.has_value());
    CHECK_EQ(runtime.palette->codes[0], 71);
    CHECK_EQ(runtime.histogramRegistrations.size(), 4u);
    const auto eventCount = runtime.events.size();
    const auto histogram = state->histogram;
    doe::writeStateVar(model, *node, nullptr, *io); //same owner; no profile reread or reset
    CHECK(model.doeStepData->stateVar == state && state->histogram == histogram);
    CHECK_EQ(runtime.histogramRegistrations.size(), 4u);
    CHECK_EQ(runtime.events.size(), eventCount);
    CHECK_EQ(node->children.size(), 20u); //append, not update/replace existing children
}

TEST_CASE(doe_snapshot_missing_constructor_inputs_preserve_destination_and_no_allocated_state)
{
    engine::GuiModel model(nullptr, 3);
    auto io = doe::makeRecordingStudyIo();
    auto node = xml::createElement(L("StateVar"));
    node->children.push_back(xml::createElement(L("Keep")));
    CHECK(refuses([&] { doe::writeStateVar(model, *node, nullptr, *io); }, "explicit resolved profile"));
    CHECK(model.doeStepData == nullptr);
    CHECK(io->observedStateVarRuntime() == nullptr);
    REQUIRE(node->children.size() == 1u);
    CHECK(node->children[0]->tag == L("Keep"));
}

TEST_CASE(doe_snapshot_histogram_enabled_serializes_live_fields_and_disabled_omits_without_reset)
{
    engine::GuiModel model(nullptr, 3);
    auto io = doe::makeRecordingStudyIo();
    auto state = existing(model);
    state->histogram = std::make_shared<doe::HistogramState>();
    const auto histogram = state->histogram;
    histogram->graphOn = true; histogram->binWidth = -9; histogram->flag30 = true;
    auto node = xml::createElement(L("StateVar"));
    doe::writeStateVar(model, *node, nullptr, *io);
    auto graphs = node->childElements(L("SVHistoGraph"));
    REQUIRE(graphs.size() == 1u);
    REQUIRE(graphs[0]->children.size() == 2u);
    CHECK(graphs[0]->children[0]->tag == L("BinWidth"));
    CHECK(text(*graphs[0], "BinWidth") == L("-9"));
    CHECK(text(*graphs[0], "GraphOn") == L("1"));
    histogram->graphOn = false; //same live object, as a recovered Close receiver may change it
    auto disabled = xml::createElement(L("StateVar"));
    doe::writeStateVar(model, *disabled, nullptr, *io);
    CHECK(disabled->childElements(L("SVHistoGraph")).empty());
    CHECK_EQ(histogram->binWidth, -9);
    CHECK(histogram->flag30);
    CHECK(state->histogram == histogram);
    CHECK(io->stateVarRuntime().histogramRegistrations.empty()); //existing helper not reconstructed again
}

TEST_CASE(doe_snapshot_empty_stepdata_appends_statevar_without_read_finalization_or_model_reset)
{
    engine::GuiModel model(nullptr, 2);
    auto io = doe::makeRecordingStudyIo();
    auto state = existing(model);
    model.doeStepData->propertyTable = std::make_unique<doe::PropertyTableState>(model);
    model.state100 = 91;
    auto node = xml::createElement(L("StepData"));
    node->children.push_back(xml::createElement(L("Keep")));
    doe::writeStepData(model, *node, nullptr, *io);
    REQUIRE(node->children.size() == 2u);
    CHECK(node->children[0]->tag == L("Keep"));
    CHECK(node->children[1]->tag == L("StateVar"));
    CHECK(node->childElements(L("Object")).empty());
    CHECK(model.doeStepData->propertyTableCreated());
    CHECK(model.doeStepData->stateVar == state);
    CHECK_EQ(model.state100, 91);
    CHECK(io->directories().empty() && io->textWrites().empty());
}

TEST_CASE(doe_snapshot_unbound_view_appends_empty_object_after_statevar)
{
    engine::GuiModel model(nullptr, 3);
    auto io = doe::makeRecordingStudyIo();
    float minimum[3]{}, maximum[3]{};
    CHECK(engine::viewerBox(model, 1, minimum, maximum));
    REQUIRE(engine::viewerItemCount(model) == 1);
    auto node = xml::createElement(L("StepData"));
    auto context = profile();
    doe::writeStepData(model, *node, &context, *io);
    REQUIRE(node->children.size() == 2u);
    CHECK(node->children[0]->tag == L("StateVar"));
    REQUIRE(node->childElements(L("Object")).size() == 1);
    CHECK(node->children[1]->attribute(L("No")) == L("0"));
    CHECK(node->children[1]->children.empty());
    REQUIRE(model.doeStepData && model.doeStepData->stateVar);
    CHECK(!model.doeStepData->propertyTableCreated());
    CHECK_EQ(io->stateVarRuntime().histogramRegistrations.size(), 4u);
}

int main() { return testkit::run_all(); }
