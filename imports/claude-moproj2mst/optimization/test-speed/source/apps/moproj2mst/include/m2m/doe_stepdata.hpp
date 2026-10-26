#pragma once

#include <memory>

#include "m2m/doe_property_table.hpp"
#include "m2m/xml.hpp"

namespace m2m::engine { struct GuiModel; }

namespace m2m::doe {
struct StateVarState;
struct StateVarConstructorContext;
class StudyIo;

// GUI-model-owned lifetime seam: +230 property table is deleted by model reset;
// the separately owned StateVar+240 survives it.
// The table is the actual constructor-empty +230 owner, not a complete Qt model.
struct StepDataState {
    std::unique_ptr<PropertyTableState> propertyTable;
    std::shared_ptr<StateVarState> stateVar; //+240 survives property-table reset
    bool propertyTableCreated() const noexcept { return propertyTable != nullptr; }
    void resetPropertyTable() { propertyTable.reset(); }
};

// c90bd0(model,node,true): ordered child dispatch, then lazy property table and
// d7c260 finalization. StateVar uses its explicit-context bounded import; in-range
// Object children update the actual View-owned symmetry state in encounter order.
// Unrecovered Object payloads and invalid negative indices guard at encounter.
// Constructor-empty table finalization uses actual views/Settings. Populated
// item/check-state work guards at its first lookup, after preceding effects.
void readStepData(engine::GuiModel& model, const xml::Node& node,
                  const StateVarConstructorContext* context = nullptr, StudyIo* io = nullptr);

//c90640(model,node,doc,true): StateVar serialization precedes every Object.
//Actual Views serialize display options, empty Slice state and represented
//symmetry. Unrecovered graphic/matrix payloads guard at their native consumer;
//the current detached Object is appended only on complete normal return.
void writeStepData(engine::GuiModel& model, xml::Node& node,
                   const StateVarConstructorContext* context, StudyIo& io);

}  // namespace m2m::doe
