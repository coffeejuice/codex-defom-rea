#include "m2m/engine/boolean_lifecycle.hpp"

#include <defmodel/layout/db_step_data.hpp>
#include "m2m/data/boolean.hpp"
#include "m2m/engine.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"

namespace m2m::engine {

void reselectBooleanObjectEditor(Project& project, Operation& operation, FileEffects& effects)
{
    // The outer timer callback must already have resolved the live receiver's
    // current model/item. No changed-operation activation/load is reproduced.
    auto* sub = activeSubProject(project);
    auto* cls = opBase(operation);
    auto* props = dynamic_cast<BooleanProps*>(operation.props.get());
    if (!sub || sub->type != 0 || project.loadingSub || operation.project != &project ||
        !cls || cls->current != &operation ||
        (cls->key().u16() != u"SfOprBoolean2D" && cls->key().u16() != u"SfOprBoolean3D") ||
        !props || !props->data) {
        throw Error(M2M_E_UNSUPPORTED, "Boolean reselection: only the same-current shown Object editor "
                    "of a loaded Type0 Boolean operation is recovered");
    }
    // A loaded Boolean uses the base operation-owned GUI model. Reject an
    // absent model before opModel's general lazy-allocation path can run.
    if (!operation.engineModel)
        throw Error(M2M_E_UNSUPPORTED, "Boolean reselection: missing loaded GUI model");
    auto* model = opModel(project, operation);
    if (!model || !model->step)
        throw Error(M2M_E_UNSUPPORTED, "Boolean reselection: missing GUI model/step");

    // Object root item+0x24 is zero (c5850 -> d4bed0 -> d4e8e0).
    // 5f36d0 -> c7b1f0(0,true) -> 823c80 writes step+0x1b4c before
    // UpdateDataInWidgetArea. Visual refresh/notifications remain qualified.
    model->step->u1b4c = 0;
    if (!cls->updateDataInWidgetArea(project, operation, effects))
        throw Error(M2M_E_UNSUPPORTED, "Boolean reselection: index0/unshown Object editor branch "
                    "is not recovered; original current-object zero prefix was applied");

    // Boolean Leave (editor vt+0x1d0=131160) is empty. The new factory
    // 11e0510 calls c7b1f0(0,false) again before constructor/fill11e25b0.
    // Refill is necessary: the next timer/Save must not reuse stale AddNew text.
    model->step->u1b4c = 0;
    if (!cls->objectTreeFill(project, operation, effects))
        throw Error(M2M_E_UNSUPPORTED, "Boolean reselection: replacement Object editor fill failed");
}

}  // namespace m2m::engine
