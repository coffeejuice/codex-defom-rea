// MDT L4: the SfOprShapeOptim slots that read the engine models and the main window's GL scene
// (packet V, m2m/engine_scene.hpp): the load vt+0x58 0x14013beb0 with its tail (vt+0x1b8, the step
// models 0x14013fb00 / 0x14013c200, the base vt+0x1c0 0x1405f65d0) and the save slots vt+0x80 /
// vt+0xe8 of vt+0x68 0x14013d6f0 (docs/re/moproj2mst/mdt/L4/spec.md section 9; decompiles
// gui_mo/s1/mdt/L7/d0.c, s0.c, gui_mo/s1/mdt/L4/vs/*.c). The class and its other slots are in
// src/ops/shapeoptim.hpp and report.cpp. UI-only calls (messenger signals, status bar, tree
// items, viewer item flags, the main window's redraws) are named where they are dropped.
#include <algorithm>
#include <cstdint>
#include <vector>

#include <defcmd/objpos.hpp>
#include <defcmd/objpos_centre.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj_ctor/llobj_copy.hpp>

#include "engine_internal.hpp"
#include "m2m/data/heattransfer.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine_scene.hpp"
#include "m2m/native_directories.hpp"
#include "m2m/p17.hpp"
#include "m2m/qt4path.hpp"
#include "ops/shapeoptim.hpp"

namespace m2m::mst {

using engine::GuiModel;
using engine::FileEffects;
using def::model::LLOBJ_DATA;

namespace detail {

// Private test seam for the native QDir::mkpath attempt at 0x14013d180.
// A failed native attempt is retained before stopping at the unrecovered
// one-OK warning modal; do not silently treat that branch as continuation.
void completeShapeOptimStudyFolderEffect(const String& path, FileEffects& effects, bool immediate)
{
    if (path.isEmpty() || !immediate) return;
    if (effects.mkdirs.empty() || effects.mkdirs.back() != path)
        throw Error(M2M_E_UNSUPPORTED, "ShapeOptim study-folder effect lost its ordered FileEffects entry");
    effects.mkdirs.pop_back();
    const bool succeeded = qt::nativeMkpathAbsolute(path);
    effects.directoryAttempts.push_back({path, succeeded});
    if (!succeeded)
        throw Error(M2M_E_UNSUPPORTED,
                    "ShapeOptim study-folder mkpath warning modal delivery is not recovered (0x14081afb0)");
}

} // namespace detail

namespace {

std::int32_t kind(const LLOBJ_DATA* o)
{
    return *engine::at<const std::int32_t>(o, 0xc);
}

}  // namespace

// @src 0x14013beb0  vt+0x58
void ShapeOptimClass::loadOperation(Project& p, Operation&, Ctx& ctx, FileEffects& fx)
{
    init();                                              // 0x14013f5a0
    fd8_ = 0;                                            // +0xd8
    fd4_ = 0;                                            // +0xd4
    // 0x140054820(current): messenger signals 1 and 2 (UI)
    if (!current) l7::refuse("SfOprShapeOptim 0x14013beb0: no current item (the original dereferences it)");
    Operation* prev = engine::prevOp(p, *current);       // 0x140062960
    GuiModel* m = engine::opModel(p, *current);          // 0x14005f770
    if (fd8_ < 1) {
        if (!prev) l7::refuse("SfOprShapeOptim 0x14013beb0: no predecessor (0x140050b20 dereferences it)");
        fd8_ = flowNumber(*prev);                        // 0x140050b20
    }
    if (!m) l7::refuse("SfOprShapeOptim 0x14013beb0: no engine model (0x140c7b720 dereferences it)");
    m->reset();                                          // 0x140c7b720
    Operation* so = stepOperation();                    // 0x140140140
    if (!so) l7::refuse("SfOprShapeOptim 0x14013beb0: no operation for the dimension (0x14005f0f0 "
                        "dereferences it)");
    if (!g_loaderSlots.setDimension) loaderSlotsMissing();
    g_loaderSlots.setDimension(*m, opDim(*so));          // 0x14005f0f0, 0x140c7acd0(m)->vt[0]
    init();                                              // 0x14013f5a0
    loadClassData(p, *current);                          // vt+0x78
    // 0x1402c8090(APP) vt+0x778: the GL widget; 0x140cf9360(widget+0x10, m)
    engine::displayShow(p, m);
    loadData(p, *current, ctx, fx);                      // vt+0x1b8
    if (Project* prj = project())                        // 0x14013f660
        if (activeSubProject(*prj)) setStepOperation(p, fd8_, ctx, fx);   // 0x1400a7390, 0x14013fb00
    GuiModel* r = engine::displayRoiModel(engine::opModel(p, *current));   // 0x14005f770, 0x140c7ae00, 0x140d0d670
    if (!r) l7::refuse("SfOprShapeOptim 0x14013beb0: no engine model (0x140c82c70 dereferences it)");
    engine::viewerClear(*r);                             // 0x140c82c70
    for (int i = 0; i < r->objectCount(); ++i) {         // 0x140c7c1a0
        LLOBJ_DATA* o = r->object(i + 1);                // 0x140c7ae80
        engine::viewerSync(*r, o, true);                 // 0x140c80280
        // 0x140c82e40, 0x1411f1a10, 0x1411eeaf0(2, 0, 1), (0xf, 1, 1): the item's display flags (viewer)
    }
    // 0x1402ce8a0(APP status, "viewFit"): UI
}

// @src 0x14013cbe0  vt+0x1b8
void ShapeOptimClass::loadData(Project& p, Operation&, Ctx& ctx, FileEffects& fx)
{
    if (!current) l7::refuse("SfOprShapeOptim 0x14013cbe0: no current item (0x14005f770 dereferences it)");
    GuiModel* m = engine::opModel(p, *current);          // 0x14005f770
    if (!m) return;
    Project* prj = project();                            // 0x14013f660
    if (!prj || !activeSubProject(*prj)) return;         // 0x1400a7390
    // studyFolder computes/queues first, then this site commits only the
    // native attempt for a full WRITE_SAVE Open. The immediate attempt occurs
    // before the listF0_ geometry-file guard below.
    const String folder = studyFolder(p, fx);             // 0x14013d180
    detail::completeShapeOptimStudyFolderEffect(folder, fx,
                                                 ctx.nativeOperationBlobs || fx.nativeDirectories);
    m = engine::displayRoiModel(m);                        // 0x140c7ae00, 0x140d0d670
    if (!listF0_.empty())
        // per name: 0x140819600, "<folder><name>.GEO", 0x140c7c950(m, i + 1, 0, 1), name +0x828,
        // kind 3, then the geometry reader 0x1408f37d0 / 0x1408f3840 (and 0x14084d090)
        l7::refuse("SfOprShapeOptim 0x14013cbe0: geometry files of the optimisation study into the ROIs model "
                   "(0x1408f3840) are not reproduced");
    engine::postLoadViews(p, *current, ctx, fx);         // 0x1400548c0(current, 1) -> 0x140054950
    // 0x14018f9e0: QList<int> of the numbers (+8) of the ROIs model's objects that are not empty (7)
    std::vector<std::int32_t> ids;
    for (int i = 0; i < m->objectCount(); ++i) {         // 0x140c7c1a0
        const LLOBJ_DATA* o = m->object(i + 1);          // 0x140c7ae80
        if (o && kind(o) != 7) ids.push_back(*engine::at<const std::int32_t>(o, 8));   // 0x140834fa0
    }
    // +0xd4, +0x7c, +0x80, +0x84: kept when in the list, else its first number
    auto keep = [&ids](std::int32_t& v) {                // 0x140307410, 0x1400251d0, 0x1400250c0
        if (ids.empty()) return;
        if (std::find(ids.begin(), ids.end(), v) == ids.end()) v = ids.front();
    };
    keep(fd4_);
    keep(b60_.f7c);
    keep(b60_.f80);
    keep(b60_.f84);
}

// @src 0x14013d180  "<project folder>\DoeStudies\<DOE study name>\Control\Opt\" (QDir::separator),
// made with 0x14081afb0(path, 1) (QDir::mkpath; a message box only when that fails); empty without
// a model or a project, and without a sub-project the empty string is returned unmade
String ShapeOptimClass::studyFolder(Project& p, FileEffects& fx)
{
    if (!current) l7::refuse("SfOprShapeOptim 0x14013d180: no current item (0x14005f770 dereferences it)");
    if (!engine::opModel(p, *current)) return String();  // 0x14005f770
    Project* prj = project();                            // 0x14013f660
    if (!prj) return String();
    SubProject* sp = activeSubProject(*prj);             // 0x1400a7390
    if (!sp) return String();
    // 0x1400b9f40: the DOE study's name (SP+0x20 -> +0x98), empty without a study.
    // A nonempty source-backed value is available only for the current Type2 owner alias;
    // Type3's separate optimization-study object and unowned/mismatched aliases stay guarded.
    String study;
    if (sp->doe) {
        if (&p != prj || sp->project != prj || sp->type != 2 || !sp->ownedDoeStudy ||
            sp->doe != sp->ownedDoeStudy.get() || sp->ownedDoeStudy->sub != sp ||
            sp->ownedDoeStudy->name.isEmpty())
            l7::refuse("SfOprShapeOptim 0x14013d180: DOE study name lacks the exact Type2 SubProject owner");
        study = sp->ownedDoeStudy->name;
    }
    const String control = projectDir(*prj) + qt::kSep + qt::L("DoeStudies") + qt::kSep + study + qt::kSep +
                           qt::L("Control");             // 0x1400a4220, 0x1421b4a18, 0x1427baf10
    const String r = control + qt::kSep + qt::L("Opt") + qt::kSep;   // 0x1421b10b4
    fx.mkdirs.push_back(r);                              // 0x14081afb0(r, 1)
    return r;
}

// @src 0x14013fb00
void ShapeOptimClass::setStepOperation(Project& p, int n, Ctx& ctx, FileEffects& fx)
{
    fd8_ = n;                                            // +0xd8
    stepModels(p, ctx, fx);                              // 0x14013c200
    if (!modelB8_) l7::refuse("SfOprShapeOptim 0x14013fb00: no step model (+0xb8; 0x140c7c1a0 dereferences it)");
    if (modelB8_->objectCount() < fd4_) {                // 0x140c7c1a0
        fd4_ = 1;
        // (+0x60)->vt+8(1) = 0x1405e0e30: empty
    }
}

// @src 0x1405f65d0  SfOprBase vt+0x1c0 (ShapeOptim keeps it): the n-th operation of the flow
// (0x1400b94f0, 1-based) is loaded (0x140055360(op, flag)), then the current item's views
// (0x1400548c0(current, 1))
static void loadFlowOperation(OpClass& c, Project& p, int n, bool flag, Ctx& ctx, FileEffects& fx)
{
    Operation* cur = c.current;                          // 0x1405ef2b0
    if (!cur || !cur->project) return;                   // 0x140055030
    SubProject* sp = activeSubProject(*cur->project);    // 0x1400a7390
    if (!sp) return;
    const std::vector<Operation*> ops = flowOperations(*sp);   // 0x1400b94f0
    if (ops.empty() || static_cast<int>(ops.size()) < n) return;
    if (n < 1) l7::refuse("SfOprBase 0x1405f65d0: flow operation number < 1 (the list is read at n - 1)");
    Operation* op = ops[static_cast<std::size_t>(n - 1)];
    if (!op) return;
    engine::decideAndLoad(p, *op, flag, ctx, fx);        // 0x140055360(op, flag)
    // 0x140054820(op): messenger signals (UI); 0x140054250(op): viewport name (UI)
    engine::postLoadViews(p, *cur, ctx, fx);             // 0x1400548c0(current, 1)
}

// @src 0x14013c200
void ShapeOptimClass::stepModels(Project& p, Ctx& ctx, FileEffects& fx)
{
    // 0x1402c8090(APP) vt+0x778: the GL widget (its scene below)
    // 0x14013fd20(this, list, 0): a list of the sub-project's simulations (ProcType 5) or of the
    // database's steps, built and destroyed unread
    if (!current) l7::refuse("SfOprShapeOptim 0x14013c200: no current item (0x14005f770 dereferences it)");
    GuiModel* cm = engine::opModel(p, *current);         // 0x14005f770
    // vt+0xf0 0x14013f6b0(this, path, 1): the project's database (0x1400a4f60(P, path, 1)); null
    // without a project
    const String db = current->project ? projectDbPath(*current->project, true) : String();
    if (!qt::fileExists(db)) {                           // QFile::exists
        Operation* so = stepOperation();                 // 0x140140140
        if (!so) l7::refuse("SfOprShapeOptim 0x14013c200: no step operation (0x14005f770 dereferences it)");
        if (GuiModel* sm = engine::opModel(p, *so)) sm->reset();   // 0x14005f770, 0x140c7b720
        loadFlowOperation(*this, p, fd8_, true, ctx, fx);   // vt+0x1c0(this, +0xd8, 1)
        GuiModel* sm = engine::opModel(p, *so);          // 0x14005f770
        if (sm) {
            modelB8_ = engine::displayMain(engine::projectDisplay(p));   // 0x140cf94f0(widget+0x10) into +0xb8
            if (!modelB8_) l7::refuse("SfOprShapeOptim 0x14013c200: the display shows no model "
                                      "(0x140c82c70 dereferences it)");
            engine::viewerClear(*modelB8_);              // 0x140c82c70
            // 0x1402c8090(APP) vt+0x728(+0xb8): the main window shows the model (UI)
            const int n = sm->objectCount();             // 0x140c7c1a0
            for (int i = 0; i < n; ++i) {
                LLOBJ_DATA* o = sm->object(i + 1);       // 0x140c7ae80
                if (!o || kind(o) == 7) continue;        // 0x140834fa0
                LLOBJ_DATA* d = modelB8_->appendObject();   // 0x140c7c8b0
                if (!d) continue;
                const bool p4 = true, p5 = true, p6 = true, p7 = true, p8 = false, p9 = true;
                def::model::llobj_copy_from(d, o, sm->step->u15f8, &p4, &p5, &p6, &p7, &p8, &p9);   // 0x140835430 = 1401613a0
                *engine::at<std::int32_t>(d, 0xc) = 3;   // kind 3
                if (!def::cmd::objpos::has_geometry(d)) {   // 0x140834d50 = 140160cc0
                    char one = 1;
                    def::cmd::objpos::fun_1401fc730(d, &one);   // 0x140870590 = 1401fc730
                }
                engine::viewerSync(*modelB8_, d, true);  // 0x140c82e40, 0x140c80280
                // 0x1411f1a10, 0x1411eeaf0(2, 0, 1), (4, 1, 1) when 0x1400c3530: item flags (viewer)
            }
            modelB8_->step->u1b4c = 1;                   // 0x140c7b1f0(+0xb8, 1, 0): 0x140823c80
        }
    } else {
        modelB8_ = cm;                                   // 0x140cbc920(+0xb8, model)
        // 0x1402c80b0(APP) -> 0x140cdab70: the open database; 0x14080fc60 reads step +0xd8 of it,
        // 0x140c7b870 loads that step into the model
        l7::refuse("SfOprShapeOptim 0x14013c200: the step of the project database (0x14080fc60, 0x140c7b870) "
                   "is not reproduced");
    }
    // 0x1402ca890(APP): UI
}

// @src 0x14013e4f0  vt+0x80
bool ShapeOptimClass::saveAfterFiles(Project& p, Operation&, FileEffects&)
{
    if (auto b0 = propsB0_.lock()) propsToClass(*b0);    // +0xb0 vt+0x60 = 0x140142d00
    if (!current) l7::refuse("SfOprShapeOptim 0x14013e4f0: no current item (0x14005f770 dereferences it)");
    GuiModel* m = engine::opModel(p, *current);          // 0x14005f770
    if (!m) return true;
    Project* prj = project();                            // 0x14013f660
    if (!prj) return true;
    SubProject* sp = activeSubProject(*prj);             // 0x1400a7390
    if (!sp) return true;
    if (m->display) m = engine::displayNamedModel(*m->display, qt::L("ROIs"));   // 0x140c7ae00, 0x140cfa0e0 (0x1421b4a58)
    // 0x14018f9e0: the numbers of the model's objects that are not empty (7); read only for the study
    std::vector<std::int32_t> ids;
    for (int i = 0; i < m->objectCount(); ++i) {         // 0x140c7c1a0
        const LLOBJ_DATA* o = m->object(i + 1);          // 0x140c7ae80
        if (o && kind(o) != 7) ids.push_back(*engine::at<const std::int32_t>(o, 8));
    }
    // 0x140051230(current), dynamic_cast to SfOprShapeOptimProperties: the "StopCritGroup" values
    // through the get slots 0x140142530 (toInt) and 0x1401425d0 (toDouble) into +0xcc / +0xe0
    if (auto* pr = dynamic_cast<ShapeOptimProps*>(current->props.get())) {
        if (!pr->data) l7::refuse("SfOprShapeOptim 0x14013e4f0: props without data (+0x38)");
        for (const PropNode& top : pr->tree) {
            if (top.name.u16() != u"StopCritGroup") continue;   // 0x1421b4a60
            for (const PropNode& n : top.children) {
                if (n.name.u16() == u"MaxSims") fcc_ = pr->data->maxSims;                // 0x1421b4a70
                else if (n.name.u16() == u"MinObjFuncVal") fe0_ = pr->data->minObjFuncVal;   // 0x1421b4a78
            }
        }
    }
    // 0x1400b5ab0(SP): the optimisation study (SP+0x28, made only for a type-3 sub-project), which
    // takes +0xd8, +0xcc, +0xe0, the objective list and the ROIs objects
    if (sp->type == 3)
        l7::refuse("SfOprShapeOptim 0x14013e4f0: the optimisation study (SP+0x28, ctor 0x140917ff0) is not "
                   "modelled");
    return true;
}

// @src 0x14013c8f0  vt+0xe8
bool ShapeOptimClass::saveOperationFiles(Project& p, Operation&, FileEffects& fx)
{
    if (!current) l7::refuse("SfOprShapeOptim 0x14013c8f0: no current item (0x14005f770 dereferences it)");
    GuiModel* m = engine::opModel(p, *current);          // 0x14005f770
    if (!m) return true;
    Project* prj = project();                            // 0x14013f660
    if (!prj || !activeSubProject(*prj)) return true;    // 0x1400a7390
    const String folder = studyFolder(p, fx);             // 0x14013d180
    detail::completeShapeOptimStudyFolderEffect(folder, fx, fx.nativeDirectories);
    m = engine::displayRoiModel(m);                        // 0x140c7ae00, 0x140d0d670
    for (int i = 0; i < m->objectCount(); ++i) {         // 0x140c7c1a0
        const LLOBJ_DATA* o = m->object(i + 1);          // 0x140c7ae80
        if (o && kind(o) != 7)                           // 0x140834fa0
            // "<folder><name +0x828>.GEO" (0x140819600, 0x1421b4a08), mesh kind 2 when 0x140065800
            // (0x14084d090), then the geometry writer 0x1408f37d0 / 0x1408f4760
            l7::refuse("SfOprShapeOptim 0x14013c8f0: the ROIs objects' .GEO files (0x1408f4760) are not "
                       "reproduced");
    }
    return true;
}

}  // namespace m2m::mst
