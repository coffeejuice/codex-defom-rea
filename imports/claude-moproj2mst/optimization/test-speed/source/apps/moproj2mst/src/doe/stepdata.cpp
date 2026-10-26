#include "m2m/doe_stepdata.hpp"

#include <utility>

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_statevar.hpp"
#include "m2m/doe_slice.hpp"
#include "m2m/doe_view_stepdata.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine_scene.hpp"

namespace m2m::doe {

void writeStepData(engine::GuiModel& model, xml::Node& node,
                   const StateVarConstructorContext* context, StudyIo& io)
{
    auto stateVar = xml::createElement(qt::L("StateVar"));
    writeStateVar(model, *stateVar, context, io);
    node.children.push_back(std::move(stateVar)); //only after d318e0 returns
    engine::writeViewerStepDataObjects(model, node);
    //c90640 has no property-table/read/postparse finalization.
}

void readStepData(engine::GuiModel& model, const xml::Node& node,
                  const StateVarConstructorContext* context, StudyIo* io)
{
    for (const auto* child : node.childElements()) {
        if (child->tag == qt::L("StateVar")) readStateVar(model, *child, context, io);
        if (child->tag == qt::L("Object")) {
            const int index = qt::toInt(child->attribute(qt::L("No")));
            //The original checks only index<count before QList::at(index).
            //A negative index can reach invalid access even for an empty list.
            if (index < 0)
                throw Error(M2M_E_UNSUPPORTED, "DOE StepData negative Object index reaches original invalid QList access");
            if (index < engine::viewerItemCount(model)) {
                const auto sliceRuntime = io ? io->sliceConstructorRuntimeOwner() : nullptr;
                readObjectStepData(engine::viewerStepDataState(model, index), *child,
                    [&model, index](const xml::Node& options) {
                        engine::readViewerDisplayOptions(model, index, options);
                    }, [&model, index, sliceRuntime](const SliceCoefficients& coefficients) {
                        engine::readViewerSlice(model, index, coefficients, sliceRuntime.get());
                    });
            }
        }
    }
    //c90210/d78100: lazily own the property-table state only after child import.
    if (!model.doeStepData) model.doeStepData = std::make_shared<StepDataState>();
    if (!model.doeStepData->propertyTable)
        model.doeStepData->propertyTable = std::make_unique<PropertyTableState>(model);
    //d7c260 copies the actual View pointer list BEFORE current-object refresh.
    //The engine seam then resolves each View's actual owner/Settings in order.
    engine::finalizeStepDataPropertyTable(model, *model.doeStepData->propertyTable);
}

}  // namespace m2m::doe
