#include "m2m/doe_statevar.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <utility>

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_stepdata.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine_scene.hpp"
#include "m2m/engine/close_effects.hpp"

namespace m2m::doe {
namespace {
constexpr const char* settingKeys[] = {
    "displayOption/dispDeflectionScaleType", "displayOption/dispDeflectionScale",
    "displayOption/dispDeflectionLineThkType", "displayOption/dispDeflectionRefShow",
    "displayOption/dispDeflectionRefTransparent"
};

std::shared_ptr<StateVarState> construct(const StateVarConstructorContext* context,
                                        StudyIo& io)
{
    // This is a support precondition, not an emulated original settings failure.
    if (!context || context->profileIdentity.empty() || !context->globals ||
        !context->deflectionScaleType.result() || !context->deflectionScale.result() ||
        !context->deflectionLineThickness.result() || !context->showDeflectionReference.result() ||
        !context->transparentDeflectionReference.result())
        throw Error(M2M_E_UNSUPPORTED, "DOE StepData StateVar constructor needs explicit resolved profile/global inputs");

    auto& runtime = io.stateVarRuntime();
    auto state = std::make_shared<StateVarState>();
    state->constructorProfile = context->profileIdentity;
    state->constructorGlobals = *context->globals;
    state->colorbarNo = context->globals->colorbarNo; //d25050 before settings
    state->deflectionScaleType = *context->deflectionScaleType.result();
    state->deflectionScale = *context->deflectionScale.result();
    state->deflectionLineThickness = *context->deflectionLineThickness.result();
    state->showDeflectionReference = *context->showDeflectionReference.result();
    state->transparentDeflectionReference = *context->transparentDeflectionReference.result();
    for (const auto* key : settingKeys) runtime.events.push_back(std::string("setting:") + key);
    //d31610 copies three global arrays after principal-mode/color initialization.
    runtime.palette = context->globals->palette;
    runtime.events.push_back("globalPaletteCopy");
    return state; //c905b0 stores the model pointer only after constructor returns
}

std::shared_ptr<StateVarState> ensureStateVar(engine::GuiModel& model,
    const StateVarConstructorContext* context, StudyIo& io)
{
    auto state = model.doeStepData ? model.doeStepData->stateVar : nullptr;
    if (!state) {
        auto created = construct(context, io);
        if (!model.doeStepData) model.doeStepData = std::make_shared<StepDataState>();
        model.doeStepData->stateVar = std::move(created);
        state = model.doeStepData->stateVar;
    }
    return state;
}

void ensureHistogram(StateVarState& state, StateVarRuntime& runtime)
{
    if (!state.histogram) {
        state.histogram = std::make_shared<HistogramState>(); //411640/12c08c0
        constexpr const char* signals[] = {"signalChangeHistoGraph", "signalAssignHistoGraph",
            "signalHistoGraphOff", "signalGetHistoGraphOn"};
        constexpr const char* slots[] = {"slotChangeHistoGraph", "slotAssignHistoGraph",
            "slotHistoGraphOff", "slotGetHistoGraphOn"};
        for (int index = 0; index < 4; ++index) {
            runtime.histogramRegistrations.push_back({signals[index], slots[index], state.histogram});
            runtime.events.push_back(std::string("register:") + signals[index]);
        }
    }
}

void resetHistogram(StateVarState& state, StateVarRuntime& runtime)
{
    ensureHistogram(state, runtime);
    state.histogram->binWidth = 10; //d328c0 always4117b0 before children
    state.histogram->graphOn = false;
    state.histogram->flag30 = false;
    runtime.events.push_back("histogramReset");
}

void readHistogram(HistogramState& state, const xml::Node& node)
{
    for (const auto* child : node.childElements()) {
        if (child->tag == qt::L("BinWidth")) state.binWidth = qt::toInt(child->text());
        else if (child->tag == qt::L("GraphOn")) state.graphOn = qt::toInt(child->text()) != 0;
    }
}

void replaceColorbar(StateVarState& state, const StateVarConstructorContext* context,
                     StateVarRuntime& runtime)
{
    if (state.auxiliaryPointerDangling)
        throw Error(M2M_E_UNSUPPORTED, "DOE StepData StateVar colorbar auxiliary has a prior dangling pointer");
    if (state.auxiliaryColorbar) {
        //53f560 ->20db90 ->0ad070 ->025650/025840 clears list storage;
        //constantfalse node-destructor branches never delete point payloads.
        for (const auto index : state.auxiliaryColorbar->firstAllocations)
            runtime.colorbarPointAllocations.at(index).orphanedByRecoveredDelete = true;
        for (const auto index : state.auxiliaryColorbar->secondAllocations)
            runtime.colorbarPointAllocations.at(index).orphanedByRecoveredDelete = true;
        state.auxiliaryColorbar.reset();
        state.auxiliaryPointerDangling = true; //original+448 not cleared by delete
        runtime.events.push_back("colorbarDelete");
    }
    if (!context || !context->colorbars)
        throw Error(M2M_E_UNSUPPORTED, "DOE StepData StateVar colorbar auxiliary needs explicit list snapshot");
    const auto& sources = *context->colorbars;
    if (sources.empty())
        throw Error(M2M_E_UNSUPPORTED, "DOE StepData StateVar colorbar auxiliary empty list reaches invalid QList access");
    if (sources.size() > static_cast<std::size_t>(std::numeric_limits<int>::max()))
        throw Error(M2M_E_UNSUPPORTED, "DOE StepData StateVar colorbar snapshot exceeds original QList count");
    std::uint32_t selected = static_cast<std::uint32_t>(state.colorbarNo);
    if (selected >= sources.size()) selected = 0; //53f380 uint out-of-range clamp
    const auto& source = sources[selected];
    runtime.events.push_back("colorbarLookup:" + std::to_string(selected));
    state.auxiliaryColorbar = std::make_unique<StateVarColorbar>(); //+448 before clone
    state.auxiliaryPointerDangling = false;
    runtime.events.push_back("colorbarConstruct");
    auto& destination = *state.auxiliaryColorbar;
    destination.field00 = source.field00;
    destination.field10Bits = source.field10Bits;
    destination.field18Bits = source.field18Bits;
    destination.flag20 = source.flag20;
    destination.title = source.title;
    for (std::size_t index = 0; index < source.second.size(); ++index) {
        if (index >= source.first.size())
            throw Error(M2M_E_UNSUPPORTED, "DOE StepData StateVar colorbar short first list reaches invalid QList access");
        auto allocation = runtime.colorbarPointAllocations.size();
        runtime.colorbarPointAllocations.push_back({source.first[index], false});
        destination.firstAllocations.push_back(allocation);
        runtime.events.push_back("colorbarFirst:" + std::to_string(index));
        allocation = runtime.colorbarPointAllocations.size();
        runtime.colorbarPointAllocations.push_back({source.second[index], false});
        destination.secondAllocations.push_back(allocation);
        runtime.events.push_back("colorbarSecond:" + std::to_string(index));
    }
    state.auxiliaryColorbarPending = false;
}
[[noreturn]] void postparseUnsupported(const char* reason)
{
    throw Error(M2M_E_UNSUPPORTED, std::string("DOE StepData StateVar d15b40 postparse ") + reason);
}

void refreshDisplay(engine::GuiModel& model, const StateVarConstructorContext* context,
                    StateVarRuntime& runtime)
{
    auto* display = model.display; //d16a80
    if (!display || !engine::displayMain(*display)) return; //cff380
    if (!context || !context->displayContext ||
        context->displayContext->boundDisplay != display)
        postparseUnsupported("needs observations bound to the actual display");
    const auto& input = *context->displayContext;
    if (!input.mainWindowAvailable)
        postparseUnsupported("needs observed global main-window availability");
    if (*input.mainWindowAvailable) { //d0d0a0 before active-model resolution
        if (input.backend != StateVarDisplayBackend::Scene)
            postparseUnsupported("alternative or unavailable legend backend remains unsupported");
        auto found = std::find_if(runtime.legends.begin(), runtime.legends.end(),
            [display](const auto& legend) { return legend.display == display; });
        if (found == runtime.legends.end()) {
            if (input.legendSlot != StateVarPresence::Absent)
                postparseUnsupported("first Scene legend needs observed absent slot");
            if (!input.sceneSize || !input.legendGlobals)
                postparseUnsupported("Scene legend needs explicit geometry and global inputs");
            const double width = (*input.sceneSize)[0], height = (*input.sceneSize)[1];
            //Input-domain support guard, not a repaired original geometry branch.
            if (!std::isfinite(width) || !std::isfinite(height) || width < 5 || height < 2 ||
                width > std::numeric_limits<int>::max() || height > std::numeric_limits<int>::max())
                postparseUnsupported("Scene geometry outside supported safe positive domain");
            const int w = static_cast<int>(width), h = static_cast<int>(height);
            StateVarLegendState legend;
            legend.display = display;
            legend.width = w < 121 ? w / 5 : 120;
            legend.height = static_cast<int>(h * 0.5);
            legend.x = w - legend.width - 5;
            legend.y = static_cast<int>(h * 0.25);
            legend.aspect = double(legend.height) / legend.width;
            legend.globals = *input.legendGlobals;
            //ccb2c0 stores Scene+c90 only after c0e470 returns, hides, then adds.
            runtime.legends.push_back(std::move(legend));
            found = runtime.legends.end() - 1;
            runtime.events.push_back("legendConstruct");
            runtime.events.push_back("legendInitialHide");
            found->insertedIntoScene = true;
            runtime.events.push_back("legendAddItem");
        }
        if (found->cacheMode != 0) { //d0d0a0 update(empty QRectF), before d0d1b0
            ++found->cacheInvalidations;
            runtime.events.push_back("legendCacheUpdate");
        }
        //cfac50 calls d0c920 even when MAIN is selected. A populated table can
        //override the active model; no constructor-null assumption is made.
        if (engine::displayMain(*display) != &model || !engine::displayMainShown(*display))
            postparseUnsupported("alternative active model remains unsupported");
        if (input.propertyTable != StateVarPresence::Absent)
            postparseUnsupported("active model requires observed absent property table");
        if (!engine::displayModels(*display).named.empty())
            postparseUnsupported("named-model plot scan remains unsupported");
        //d15920 on this empty-view MAIN is false. No named plots can change it;
        //therefore d0d1b0 hides regardless of the unconsumed display+A30 byte.
        found->visible = false;
        runtime.events.push_back("legendHide");
    }
    //cff380's later lookup passes false: never creates a property table.
    if (input.propertyTable != StateVarPresence::Absent)
        postparseUnsupported("display property-table population remains unsupported");
}

void postparse(engine::GuiModel& model, StateVarState& state, int variable, int component,
               bool notify, const StateVarConstructorContext* context, StateVarRuntime& runtime,
               StudyIo& io, engine::StateVarPlotEffects* effects)
{
    state.postparsePending = true;
    state.plotApplyInProgress = true; //d15b80 ->d25340, normal-return clear only
    if (engine::viewerItemCount(model) != 0) //d15bf0 first
        postparseUnsupported("view normalization remains unsupported");
    if (state.cachedVariable != variable) { //d302f0(state,null,var)
        state.cachedDataPresent = false;
        state.cachedVariable = -1;
    }
    state.plotVar = variable;
    state.plotCompNo = component;
    if (notify) { //d16530 first emit, before rendering/type/range work
        auto& notifications = io.sessionNotificationState();
        if (!notifications.suppressed) {
            notifications.flag90 = false;
            if (!effects) postparseUnsupported("signalChangePtrkGraph receiver dispatch is unavailable");
            effects->changePtrkGraph(model, state);
        }
    }
    if (state.plotVar != 0) //d16ae0: nonzero rendering/global branch
        postparseUnsupported("nonzero variable rendering remains unsupported");
    //d16340 is a no-op for the proven empty view list.
    if (model.objectCount() != 0) //d30480
        postparseUnsupported("object display updates remain unsupported");
    state.type = 2; //82cf50 variable0, irrespective of component/plot type
    if (!context || !context->translatedNoneTitle)
        postparseUnsupported("needs explicit translated None title");
    state.plotTitle = *context->translatedNoneTitle; //d267a0
    state.minimum = 0;
    state.maximum = 1; //d17b90, no view contributions
    if (notify) { //d17b90 second emit, before d189b0 and display/legend work
        auto& notifications = io.sessionNotificationState();
        if (!notifications.suppressed) {
            notifications.flag90 = false;
            if (!effects) postparseUnsupported("signalChangeHistoGraph receiver dispatch is unavailable");
            effects->changeHistoGraph(model, state);
        }
    }
    //d189b0's unused cached-radius query is pure (1239630/121a310); variable0
    //returns before its visibility loop. d15fb0's empty view loop is also a no-op.
    refreshDisplay(model, context, runtime);
    state.plotApplyInProgress = false; //d25360, only after normal return
    state.postparsePending = false;
}
} // namespace

StateVarRuntime& StudyIo::stateVarRuntime()
{
    if (!stateVarRuntime_) stateVarRuntime_ = std::make_shared<StateVarRuntime>();
    return *stateVarRuntime_;
}

void writeStateVar(engine::GuiModel& model, xml::Node& node,
    const StateVarConstructorContext* context, StudyIo& io)
{
    auto state = ensureStateVar(model, context, io); //c905b0; constructor only if absent
    const auto append = [](xml::Node& parent, const char* name, const qt::String& text) {
        auto child = xml::createElement(qt::L(name));
        child->children.push_back(xml::createTextNode(text));
        parent.children.push_back(std::move(child));
    };
    append(node, "SVColorBarNo", qt::number(state->colorbarNo));
    append(node, "SVType", qt::number(state->type));
    append(node, "SVPlotVar", qt::number(state->plotVar));
    append(node, "SVPlotCompNo", qt::number(state->plotCompNo));
    append(node, "SVPlotType", qt::number(state->plotType));
    append(node, "SVRangeType", qt::number(state->rangeType));
    append(node, "SVRangeMin", qt::number(state->rangeMin, 'g', 6));
    append(node, "SVRangeMax", qt::number(state->rangeMax, 'g', 6));
    append(node, "SVMin", qt::number(state->minimum, 'g', 6));
    append(node, "SVMax", qt::number(state->maximum, 'g', 6));
    //d314d0(true) runs AFTER the scalar children even when graphOn is false.
    //Unlike d328c0 import, Save never resets an existing Histogram.
    ensureHistogram(*state, io.stateVarRuntime());
    if (state->histogram->graphOn) {
        auto histogram = xml::createElement(qt::L("SVHistoGraph"));
        append(*histogram, "BinWidth", qt::number(state->histogram->binWidth));
        append(*histogram, "GraphOn", qt::number(state->histogram->graphOn ? 1 : 0));
        node.children.push_back(std::move(histogram));
    }
}

void applyStateVarSelection(engine::GuiModel& model, int variable, int component,
    bool notify, const StateVarConstructorContext* context, StudyIo* io,
    engine::StateVarPlotEffects* effects)
{
    if (!io)
        throw Error(M2M_E_UNSUPPORTED, "DOE StateVar apply needs per-conversion runtime state");
    auto state = ensureStateVar(model, context, *io);
    if (state->auxiliaryColorbarPending)
        throw Error(M2M_E_UNSUPPORTED, "DOE StateVar apply cannot resume a prior unsupported colorbar prefix");
    postparse(model, *state, variable, component, notify, context, io->stateVarRuntime(), *io, effects);
}

void readStateVar(engine::GuiModel& model, const xml::Node& node,
                  const StateVarConstructorContext* context, StudyIo* io)
{
    if (!io)
        throw Error(M2M_E_UNSUPPORTED, "DOE StepData StateVar needs per-conversion runtime state");
    auto state = ensureStateVar(model, context, *io);
    auto& runtime = io->stateVarRuntime();
    if (state->auxiliaryColorbarPending)
        throw Error(M2M_E_UNSUPPORTED, "DOE StepData StateVar cannot resume a prior unsupported colorbar prefix");
    resetHistogram(*state, runtime);
    for (const auto* child : node.childElements()) {
        const auto& tag = child->tag;
        if (tag == qt::L("SVColorBarNo")) {
            const int old = state->colorbarNo;
            state->colorbarNo = qt::toInt(child->text()); //assignment precedes branch
            if (old != state->colorbarNo) {
                state->auxiliaryColorbarPending = true;
                replaceColorbar(*state, context, runtime);
            }
        } else if (tag == qt::L("SVType")) state->type = qt::toInt(child->text());
        else if (tag == qt::L("SVPlotVar")) state->plotVar = qt::toInt(child->text());
        else if (tag == qt::L("SVPlotCompNo")) state->plotCompNo = qt::toInt(child->text());
        else if (tag == qt::L("SVPlotType")) state->plotType = qt::toInt(child->text());
        else if (tag == qt::L("SVRangeType")) state->rangeType = qt::toInt(child->text());
        // Original uses toInt even for values assigned to double fields.
        else if (tag == qt::L("SVRangeMin")) state->rangeMin = qt::toInt(child->text());
        else if (tag == qt::L("SVRangeMax")) state->rangeMax = qt::toInt(child->text());
        else if (tag == qt::L("SVMin")) state->minimum = qt::toInt(child->text());
        else if (tag == qt::L("SVMax")) state->maximum = qt::toInt(child->text());
        else if (tag == qt::L("SVHistoGraph")) readHistogram(*state->histogram, *child);
    }
    applyStateVarSelection(model, state->plotVar, state->plotCompNo, false, context, io);
}

} // namespace m2m::doe
