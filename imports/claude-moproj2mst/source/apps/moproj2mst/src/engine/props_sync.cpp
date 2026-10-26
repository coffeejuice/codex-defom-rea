// SfOprBaseProperties props-sync slots (props_sync.hpp). Owner MDT L2; decompiles
// gui_mo/s1/mdt/L2/d1.c (0x1405fb720 .. 0x1405fc180), d2.c (0x1405fcb90).
#include <memory>
#include <string>

#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>

#include "m2m/data/common.hpp"
#include "m2m/engine.hpp"
#include "m2m/p17.hpp"
#include "m2m/props_base.hpp"
#include "props_sync.hpp"

namespace m2m::engine {

using def::model::Cls_1401e7b10;
using qt::L;

namespace {

data::CommonBlock& B(Props& pr)
{
    if (!pr.common) pr.common = std::make_shared<data::CommonBlock>();
    return *pr.common;
}

// The sim control a reader uses: the argument, else the owner's (S+0x750)
template<class F>
    requires std::is_same_v<std::invoke_result_t<F, Cls_1401e7b10*>, bool>
bool withReaderSimCtl(Project& p, Props& pr, Cls_1401e7b10* sc, F&& callback)
{
    // An explicit caller control never acquires the owner's model.
    if (sc) return std::invoke(std::forward<F>(callback), sc);
    return withPropsStep(p, pr, [&](DB_STEP_DATA* s) -> bool {
        return std::invoke(std::forward<F>(callback), s ? &s->sim_ctrl : nullptr);
    });
}

// "%1 %2".arg(v, 0, 'g', -1, ' ').arg(QString(unitName(kind, unit)), 0, ' ')
String valueWithUnit(double v, int kind, int unit)
{
    const String u = String::latin1(units::unitName(kind, unit));   // 0x140917de0
    return qt::arg(qt::arg(L("%1 %2"), v, 0, 'g', -1, u' '), u, 0, u' ');   // 0x14249d1c0..
}

}  // namespace

// @src 0x1405fb720  NumSteps (B+0) = SC NSTEP, StepIncr (B+4) = STPINC, TimePerStep (B+0x40) =
// DTMAX 1 with the time unit
bool propsStepsFromSimCtl(Project& p, Props& pr, Cls_1401e7b10* sc)
{
    return withReaderSimCtl(p, pr, sc, [&](Cls_1401e7b10* sc) -> bool {
        if (sc) {
            B(pr).numSteps = sc->nstep;                         // SC+0x16c
            B(pr).stepIncr = sc->stpinc;                        // SC+0x164
            B(pr).dtmax = valueWithUnit(sc->dtmax1, 0, sc->unit);   // SC+0x1d0, unit 0x1408e01e0
        }
        return sc != nullptr;
    });
}

// @src 0x1405fbae0  EnvTemp (B+0x20) = ENVTMP constant (SC+0x3d0), category 9
bool propsEnvTempFromSimCtl(Project& p, Props& pr, Cls_1401e7b10* sc)
{
    return withReaderSimCtl(p, pr, sc, [&](Cls_1401e7b10* sc) -> bool {
        if (sc) B(pr).envTemp = valueWithUnit(sc->envtmp.value, 9, sc->unit);
        return sc != nullptr;
    });
}

// @src 0x1405fbe30  ConvectionCoef (B+0x30) = CNVCOF constant (SC+0x4c8), category 0x20
bool propsConvectionFromSimCtl(Project& p, Props& pr, Cls_1401e7b10* sc)
{
    return withReaderSimCtl(p, pr, sc, [&](Cls_1401e7b10* sc) -> bool {
        if (sc) B(pr).convection = valueWithUnit(sc->cnvcof.value, 0x20, sc->unit);
        return sc != nullptr;
    });
}

// @src 0x1405fc180  StepBy = DTTYPE mode (SC+0x240, 0x1408da6d0) through 0x1405fb450(mode, no
// signal); mode 0: DistPerStep (B+0x38) = DSMAX (category 1), else TimePerStep (B+0x40) = DTMAX 1
bool propsStepByFromSimCtl(Project& p, Props& pr, Cls_1401e7b10* sc)
{
    return withReaderSimCtl(p, pr, sc, [&](Cls_1401e7b10* sc) -> bool {
        if (!sc) return false;
        base::setStepBy(pr, sc->dttype.u58);
        if (B(pr).dtMode == 0)
            B(pr).dsmax = valueWithUnit(sc->dsmax, 1, sc->unit);    // SC+0x1e0
        else
            B(pr).dtmax = valueWithUnit(sc->dtmax1, 0, sc->unit);   // SC+0x1d0
        return true;
    });
}

// @src 0x1405fcb90  HeatingTime (B+0x18) = t with the time unit of the owner's sim control. The
// owner's model is not checked for null (0x1408221e0(S) reads S+0x750+0xfc); the signal
// 0x1405f8590 (QMetaObject::activate, props signal 1) is UI.
void propsSetHeatingTime(Project& p, Props& pr, double t)
{
    withPropsStep(p, pr, [&](DB_STEP_DATA* s) {
        if (!s) throw Error(M2M_E_UNSUPPORTED, "operation load: heating time without an engine model (0x1405fcb90 reads a null model)");
        B(pr).heatingTime = valueWithUnit(t, 0, s->sim_ctrl.unit);
    });
}

}  // namespace m2m::engine
