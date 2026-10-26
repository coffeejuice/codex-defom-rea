#include "m2m/engine/boolean_lifecycle.hpp"

#include <defmodel/layout/db_step_data.hpp>
#include "m2m/data/boolean.hpp"
#include "m2m/engine.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"

namespace m2m::engine {

namespace {
bool sameOwner(const std::shared_ptr<const void>& a,
               const std::shared_ptr<const void>& b)
{
    return a && b && a.get() == b.get() && !a.owner_before(b) && !b.owner_before(a);
}

void requireBooleanUpdateBinding(Project& project, Operation& operation)
{
    const auto* sub = activeSubProject(project);
    const auto* cls = opBase(operation);
    const auto* props = dynamic_cast<BooleanProps*>(operation.props.get());
    if (!sub || sub->type != 0 || project.loadingSub || operation.project != &project ||
        !cls || cls->current != &operation ||
        (cls->key().u16() != u"SfOprBoolean2D" && cls->key().u16() != u"SfOprBoolean3D") ||
        !props || !props->data || !operation.engineModel || !operation.engineModel->step)
        throw Error(M2M_E_UNSUPPORTED, "Boolean UpdateData: actual loaded same-current Type0 binding required");
}
} // namespace

BooleanSynchronousUpdateData::BooleanSynchronousUpdateData(Project& project, Operation& operation,
    const std::shared_ptr<const void>& receiverOwner,
    const std::shared_ptr<const void>& currentEditorOwner, const std::string& qualification)
    : project_(&project), operation_(&operation), class_(opBase(operation)),
      receiver_(receiverOwner), editor_(currentEditorOwner), model_(operation.engineModel)
{
    requireBooleanUpdateBinding(project, operation);
    if (!receiverOwner || !currentEditorOwner || qualification.empty())
        throw Error(M2M_E_UNSUPPORTED, "Boolean UpdateData: qualified actual receiver/editor owners required");
}

BooleanUpdateDataResult BooleanSynchronousUpdateData::dispatch(Project& project, Operation& operation,
    FileEffects& effects, const BooleanUpdateDataEmission& input)
{
    if (!admitted_ || &project != project_ || &operation != operation_ || opBase(operation) != class_ ||
        !sameOwner(model_.lock(), operation.engineModel))
        throw Error(M2M_E_UNSUPPORTED, "Boolean UpdateData: emission belongs to another operation/model owner");
    requireBooleanUpdateBinding(project, operation);
    // A direct native invocation can nest, but no nested receiver-list/effect
    // ordering has been admitted to this bounded projection.
    if (inFlight_)
        throw Error(M2M_E_UNSUPPORTED, "Boolean UpdateData: nested invocation remains unrepresented");
    if (!input.occurrence || !input.emissionAdmitted || input.qualification.empty() ||
        !input.notifierSuppressed)
        return {BooleanUpdateDataOutcome::Unqualified, false, "Explicit emission/gate observation required"};
    if (consumed_.contains(input.occurrence))
        throw Error(M2M_E_UNSUPPORTED, "Boolean UpdateData: emission already consumed");
    if (*input.notifierSuppressed) {
        consumed_.insert(input.occurrence);
        return {BooleanUpdateDataOutcome::Suppressed, true, {}};
    }
    if (!input.receiverRegistrationAdmitted)
        return {BooleanUpdateDataOutcome::Unqualified, false, "Actual receiver registration required"};
    const auto receiver = receiver_.lock();
    if (!receiver) {
        consumed_.insert(input.occurrence);
        return {BooleanUpdateDataOutcome::ExpiredReceiver, true, {}};
    }
    if (!sameOwner(receiver, input.receiverOwner.lock()))
        throw Error(M2M_E_UNSUPPORTED, "Boolean UpdateData: receiver owner/lifetime differs");
    if (!input.methodResolved)
        return {BooleanUpdateDataOutcome::Unqualified, false, "Actual method resolution required"};
    if (!*input.methodResolved) {
        consumed_.insert(input.occurrence);
        return {BooleanUpdateDataOutcome::MethodNotResolved, true, {}};
    }
    if (!input.currentWidgetMatched)
        return {BooleanUpdateDataOutcome::Unqualified, false, "Actual current-widget guard/cast required"};
    const auto editor = editor_.lock();
    if (!*input.currentWidgetMatched || !editor) {
        consumed_.insert(input.occurrence);
        return {BooleanUpdateDataOutcome::NoCurrentEditor, true, {}};
    }
    if (!sameOwner(editor, input.currentEditorOwner.lock()))
        throw Error(M2M_E_UNSUPPORTED, "Boolean UpdateData: current editor owner/lifetime differs");

    // Original1402cf6c6 uses explicit type1: invoke the existing current editor
    // hook now, before returning to Leave/refill or Save propsLoad. Consume this
    // occurrence first; an exception must not replay partial native effects.
    consumed_.insert(input.occurrence);
    inFlight_ = true;
    try {
        const bool shown = opBase(operation)->updateDataInWidgetArea(project, operation, effects);
        inFlight_ = false;
        return {shown ? BooleanUpdateDataOutcome::Invoked : BooleanUpdateDataOutcome::NoCurrentEditor,
                true, {}};
    } catch (...) {
        inFlight_ = false;
        throw;
    }
}

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
