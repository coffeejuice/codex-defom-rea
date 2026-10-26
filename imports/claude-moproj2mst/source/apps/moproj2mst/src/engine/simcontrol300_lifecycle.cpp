#include "m2m/engine/simcontrol300_lifecycle.hpp"
#include "engine_internal.hpp"
#include "m2m/engine.hpp"
#include "m2m/p17.hpp"
#include <defcmd/interp.hpp>
#include <defmodel/window/inter_object_ops.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/step/db_step.hpp>
#include <cstdio>
#include <string>
#include <vector>

namespace m2m::engine::simcontrol300 {
namespace detail {
std::shared_ptr<const def::model::DYNARY> copyStepSizeCurve(const def::model::DYNARY&);
bool stepSizeCurveCopyComplete(const def::model::DYNARY&, const def::model::DYNARY&);
}
namespace {
std::shared_ptr<const def::model::DYNARY> copyCurve(const def::model::DYNARY& source)
{
    auto curve = std::make_shared<def::model::DYNARY>();
    def::model::dynary_copy(curve.get(), &source);
    return curve;
}

template <class T> bool retain(T& destination, std::optional<T> source)
{
    if (!source) return false;
    destination = std::move(*source);
    return true;
}

void copyEnvironmentCurveOwners(InitialEditorSession& session, const SimControl& sc)
{
    const def::model::DYNARY* source[]{&sc.envtmp, &sc.cnvcof, &sc.envatm, &sc.acvcof};
    for (int i = 0; i < 4; ++i) {
        def::model::dynary_copy(session.environmentCurves.curves[i].get(), source[i]);
        session.pages.environment.curves[i] = session.environmentCurves.curves[i];
    }
}

void copyEnvironmentTemporaries(InitialEditorSession& session, const SimControl& sc)
{
    copyEnvironmentCurveOwners(session, sc);
    session.viewFactorCurve = copyCurve(sc.vfactr);
    session.viewFactorDefinition = sc.vfactr_n;
}

std::optional<SpringComboLabels> springComboItemLabels(GuiModel& model)
{
    // 140ec8b00 builds visible "%-3d - name" combo text, with a zero/None
    // placeholder, then walks object(i) by object number. 140ec8da0 later
    // parses that text's first integer; QVariant item data is not consulted.
    const int count = model.objectCount();
    if (count < 0 || count > 4096) return std::nullopt; // bounded reconstruction admission
    SpringComboLabels labels;
    labels.first.reserve(static_cast<std::size_t>(count) + 1);
    auto append = [&](int number, std::string_view name) {
        // The native search copies the local-8-bit label into a stack buffer
        // unchecked. Admit only short ASCII labels whose byte conversion is
        // unambiguous. The original builder formats the signed decimal number.
        for (unsigned char c : name)
            if (c < 0x20 || c > 0x7e) return false;
        char buffer[64];
        const int n = std::snprintf(buffer, sizeof(buffer), "%-3d - %.*s",
            number, static_cast<int>(name.size()), name.data());
        if (n < 0 || n >= static_cast<int>(sizeof(buffer))) return false;
        labels.first.push_back(qt::String::latin1(std::string_view(buffer,
            static_cast<std::size_t>(n))));
        return true;
    };
    if (!append(0, "None")) return std::nullopt;
    for (int i = 1; i <= count; ++i) {
        const auto* object = model.object(i);
        if (!object) return std::nullopt;
        if (*at<const std::int32_t>(object, 0xc) == 7) continue;
        const auto* name = at<const char>(object, 0x828);
        std::size_t length = 0;
        while (length < 0x54 && name[length] != '\0') ++length;
        if (length == 0x54 || !append(*at<const std::int32_t>(object, 8),
                std::string_view(name, length))) return std::nullopt;
    }
    labels.second = labels.first;
    return labels;
}
} // namespace

void openInitialEditorInto(InitialEditorSession& session, GuiModel& model,
    RemeshState& remesh, Extra3DIo& io, const InitialEditorContext& context, InitialOpenReport& report)
{
    report = {};
    if (session.constructionStarted) {
        report.issue = InitialOpenIssue::AlreadyStarted;
        return;
    }
    session.constructionStarted = true;
    auto* control = model.simCtl();
    if (!control || !model.step || model.step->dimension != 3) {
        report.issue = InitialOpenIssue::InvalidModel;
        return;
    }
    if (context.numericLocale != NumericLocale::KnownC) {
        report.issue = InitialOpenIssue::UnrecoveredNumericLocale;
        return;
    }
    auto& sc = *control;
    session.sourceModel = &model;
    session.sourceControl = control;
    session.sourceRemesh = &remesh;
    auto& pages = session.pages;
    if (context.stoppingLabelProfile != StoppingLabelProfile::Unknown)
        pages.stopping.widgets.labelProfile = context.stoppingLabelProfile;
    const bool mo = context.general.applicationMode == 5;
    auto fillGeneralPage = [&](std::size_t invocation) {
        auto general = context.general;
        if (mo) {
            if (!general.moProject || !context.moObservations ||
                !context.moObservations->generalFills[invocation]) {
                report.issue = InitialOpenIssue::MissingMoObservation;
                return false;
            }
            auto observed = generalContextForMoAvailability(*general.moProject,
                general.dimension, *context.moObservations->generalFills[invocation]);
            if (!observed) {
                report.issue = InitialOpenIssue::MissingMoObservation;
                return false;
            }
            general = std::move(*observed);
        }
        return retain(pages.general, fillGeneral(sc, general));
    };
    auto fillEnvironmentPage = [&] {
        std::optional<bool> pressure = context.pressureFieldsPopulated;
        if (mo) {
            if (!context.moObservations) {
                report.issue = InitialOpenIssue::MissingMoObservation;
                return false;
            }
            pressure = pressureFieldsForStartupHeating(context.moObservations->startupHeating);
        }
        if (!pressure) {
            report.issue = InitialOpenIssue::MissingMoObservation;
            return false;
        }
        return retain(pages.environment, fillEnvironment3D(sc, 3, context.numericLocale, *pressure,
            pages.environment.timeId.differenceBoundaryChecked, &session.environmentCurves));
    };
    auto fill = [&](Page page, std::size_t generalInvocation) {
        report.reachedPage = page;
        report.issue = InitialOpenIssue::UnsupportedPage;
        bool supported = false;
        switch (page) {
        case Page::General: supported = fillGeneralPage(generalInvocation); break;
        case Page::StepCount: supported = retain(pages.stepCount, fillStepCount(sc, context.stepCount)); break;
        case Page::StepSize: supported = retain(pages.stepSize, fillStepSize3D(sc, 3, context.numericLocale)); break;
        case Page::Stopping:
            report.stoppingFill = fillStopping3DInto(pages.stopping, sc, model,
                context.numericLocale);
            supported = report.stoppingFill->complete();
            break;
        case Page::Meshing: supported = retain(pages.meshing, fillMeshing3D(model, remesh)); break;
        case Page::Iteration: supported = retain(pages.iteration, fillIteration3D(sc, 3, context.numericLocale)); break;
        case Page::Environment:
            supported = fillEnvironmentPage();
            if (supported) fillEnvironmentSharedControls(pages.general, pages.environment);
            break;
        case Page::Advanced: supported = retain(pages.advanced, fillAdvanced3D(sc, model, context.numericLocale)); break;
        case Page::Extra3D: {
            supported = retain(session.extra3d, fillExtra3D(io, context.numericLocale,
                [&model] { return springComboItemLabels(model); }));
            break;
        }
        }
        if (supported) report.issue = InitialOpenIssue::None;
        return supported;
    };
    // 140eb80c0's page0 dispatch, in original order.
    report.stage = InitialOpenStage::InitialFill;
    for (std::size_t i = 0; i < initialFillOrder.size(); ++i) {
        if (!fill(initialFillOrder[i], 0)) return;
        report.initialPages[i] = true;
    }
    // 140eb5630 copies all six temporaries after the initial dispatch.
    report.stage = InitialOpenStage::TemporaryCopy;
    report.reachedPage.reset();
    copyEnvironmentCurveOwners(session, sc);
    auto initialStepCurve = detail::copyStepSizeCurve(sc.dttype);
    if (!initialStepCurve) {
        report.reachedPage = Page::StepSize;
        report.issue = InitialOpenIssue::UnsupportedPage;
        return;
    }
    pages.stepSize.curve = std::move(initialStepCurve);
    session.viewFactorCurve = copyCurve(sc.vfactr);
    session.viewFactorDefinition = sc.vfactr_n;
    report.temporariesCopied = true;
    // 140eb50f0 repeats these fills before the backed-up refresh.
    report.stage = InitialOpenStage::RepeatFill;
    for (std::size_t i = 0; i < constructorRepeatFillOrder.size(); ++i) {
        if (!fill(constructorRepeatFillOrder[i], 0)) return;
        report.repeatPages[i] = true;
    }

    // 140ecb340: construct/initialize a real backup with the same owning step,
    // then use the existing GUI1408df7b0/native1401f3660 copy binding. This is
    // the original field-aware deep copy, not assignment/memcpy of owned arrays.
    report.stage = InitialOpenStage::Backup;
    report.reachedPage.reset();
    SimControl backup;
    // GUI1408d3bc0 seeds UNIT from143d79724 before owner/init. Keep that
    // per-call startup value distinct from loaded SC.unit; the following deep
    // copy supplies the loaded state only after native environment defaults.
    backup.unit = guiStartupUnit();
    def::model::sim_ctrl_set_owner(backup, model.step.get());
    def::cmd::sim_ctrl_copy(&backup, &sc);
    if (!detail::stepSizeCurveCopyComplete(sc.dttype, backup.dttype) ||
        (sc.dttype.ndim != 0 && sc.dttype.u58 != backup.dttype.u58)) {
        report.reachedPage = Page::StepSize;
        report.issue = InitialOpenIssue::UnsupportedPage;
        return; // Partial backup is not a restoration source; live SC remains intact.
    }
    report.backupCreated = true;
    report.stage = InitialOpenStage::BackedRefresh;
    bool supported = true;
    for (std::size_t i = 0; i < constructorBackedUpRefreshOrder.size(); ++i) {
        const auto page = constructorBackedUpRefreshOrder[i];
        std::shared_ptr<const def::model::DYNARY> stepCurve;
        if (page == Page::StepSize) {
            stepCurve = detail::copyStepSizeCurve(sc.dttype);
            if (!stepCurve) {
                report.reachedPage = Page::StepSize;
                report.issue = InitialOpenIssue::UnsupportedPage;
                supported = false;
                break; // Still execute the common SC backup restore below.
            }
        }
        if (page == Page::Environment) copyEnvironmentTemporaries(session, sc);
        if (!fill(page, 1)) { supported = false; break; }
        if (page == Page::StepSize) pages.stepSize.curve = std::move(stepCurve);
        report.refreshedPages[i] = true;
    }
    if (supported) {
        constructorRefreshExtra3D(session.extra3d);
        report.extra3DRefreshed = true;
    }
    def::cmd::sim_ctrl_copy(&sc, &backup);
    report.backupRestored = detail::stepSizeCurveCopyComplete(backup.dttype, sc.dttype) &&
        (backup.dttype.ndim == 0 || backup.dttype.u58 == sc.dttype.u58);
    if (!report.backupRestored) {
        report.reachedPage = Page::StepSize;
        report.issue = InitialOpenIssue::UnsupportedPage;
        supported = false; // Keep reached restore state; never certify an unavailable table copy.
    }
    if (supported) {
        session.pageStateComplete = true;
        report.stage = InitialOpenStage::PageStateComplete;
        report.reachedPage.reset();
    }
    return;
}

std::optional<InitialEditorSession> openInitialEditor(GuiModel& model, RemeshState& remesh,
    Extra3DIo& io, const InitialEditorContext& context)
{
    InitialEditorSession session{};
    InitialOpenReport report;
    openInitialEditorInto(session, model, remesh, io, context, report);
    if (!report.complete()) return std::nullopt;
    return session;
}

static InitialApplyReport applyInitialEditorPageStateBound(InitialEditorSession& session, GuiModel& model,
    RemeshState& remesh, Extra3DIo& io, const std::vector<RecoveryEntry>* rootRecovery,
    const std::vector<RecoveryEntry>* operationRecovery, Operation* actualOperation)
{
    InitialApplyReport report;
    if (!session.pageStateComplete || &model != session.sourceModel || !session.sourceControl ||
        !model.matchesCurrentSimControl(session.sourceControl) || &remesh != session.sourceRemesh) {
        report.outcome = ApplyResult::UnsupportedSnapshot;
        return report;
    }
    // Only the owner-bound entry may reach nonempty recovery rows. Check the
    // actual OPI/model binding before any page or file effect; A/B holders are
    // deliberately resolved later, after Extra3D's callbacks and I/O.
    if (actualOperation && (!actualOperation->project ||
        actualOperation->engineModel.get() != &model ||
        actualOperation->operationInfoIdentity().get() != actualOperation)) {
        report.outcome = ApplyResult::UnsupportedSnapshot;
        return report;
    }
    for (int i = 0; i < 4; ++i) {
        if (!session.environmentCurves.curves[i] || session.pages.environment.curves[i].get() !=
            session.environmentCurves.curves[i].get()) {
            report.outcome = ApplyResult::UnsupportedSnapshot;
            return report;
        }
    }
    auto& sc = *session.sourceControl;
    auto& pages = session.pages;
    auto record = [&](Page page, ApplyResult result) {
        report.pages[static_cast<std::size_t>(page)] = result;
        if (result == ApplyResult::UnsupportedSnapshot || report.outcome == ApplyResult::Completed)
            report.outcome = result;
        return result != ApplyResult::UnsupportedSnapshot;
    };
    if (!record(Page::General, applyGeneral(sc, pages.general)
            ? ApplyResult::Completed : ApplyResult::UnsupportedSnapshot)) return report;
    applyStepCount(sc, pages.stepCount);
    record(Page::StepCount, ApplyResult::Completed);
    if (!record(Page::StepSize, applyStepSize3D(sc, pages.stepSize))) return report;
    if (!record(Page::Stopping, applyStopping3D(sc, model, pages.stopping))) return report;
    if (!record(Page::Meshing, applyMeshingSelection3D(model, remesh, pages.meshing))) return report;
    if (!record(Page::Iteration, applyIteration3D(sc, pages.iteration))) return report;
    if (!record(Page::Environment, applyEnvironment3D(sc, pages.environment,
            EnvironmentViewFactorState{model.step ? model.step->dimension : 3,
                session.viewFactorCurve.get(), session.viewFactorDefinition},
            &session.environmentCurves))) return report;
    if (!record(Page::Advanced, applyAdvanced3D(sc, model, pages.advanced))) return report;
    if (!record(Page::Extra3D, applyExtra3D(io, session.extra3d))) return report;
    // 140eb52f0 frees editor+f8 after the page blocks. A repeated Save cannot
    // silently reuse those freed per-object mesh temporary entries.
    pages.meshing.objects.clear();
    // Extra3D performs external I/O and can run a callback that replaces the
    // current step. Do not pass a now-dangling control reference to the tail.
    if (actualOperation && !model.matchesCurrentSimControl(session.sourceControl)) {
        report.recoveryTail = ApplyResult::UnsupportedSnapshot;
        report.outcome = ApplyResult::UnsupportedSnapshot;
        return report;
    }
    report.recoveryTail = actualOperation
        ? applyOwnedRecoveryTail3D(sc, model, pages.advanced, *actualOperation)
        : applyEmptyRecoveryTail(sc, model, pages.advanced, rootRecovery, operationRecovery);
    if (*report.recoveryTail == ApplyResult::UnsupportedSnapshot || report.outcome == ApplyResult::Completed)
        report.outcome = *report.recoveryTail;
    return report;
}

InitialApplyReport applyInitialEditorPageState(InitialEditorSession& session, GuiModel& model,
    RemeshState& remesh, Extra3DIo& io, const std::vector<RecoveryEntry>* rootRecovery,
    const std::vector<RecoveryEntry>* operationRecovery)
{
    return applyInitialEditorPageStateBound(session, model, remesh, io,
        rootRecovery, operationRecovery, nullptr);
}

InitialApplyReport applyInitialEditorPageState(InitialEditorSession& session, GuiModel& model,
    RemeshState& remesh, Extra3DIo& io, Operation& actualOperation)
{
    return applyInitialEditorPageStateBound(session, model, remesh, io,
        nullptr, nullptr, &actualOperation);
}
} // namespace m2m::engine::simcontrol300
