// SfOprShapeOptim (class 0x14013a9a0; owner P16, load slots MDT L7, props and the GL-scene steps
// MDT L4). The slots that need no engine are in report.cpp; the load tail after vt+0x78, vt+0x1b8
// and the save slots vt+0x80 / vt+0xe8, which read the engine models and the GL scene, are in
// src/engine/shapeoptim_load.cpp (docs/re/moproj2mst/mdt/L4/spec.md section 9).
#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "m2m/mst/simcount.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"

#include "l7.hpp"

namespace m2m::mst {

class ShapeOptimClass : public OpClass {
public:
    using OpClass::OpClass;
    // @src 0x14013fb90  vt+0x280: empty
    void writeMst(W&, Project&, const String&, int*, Operation*, int, int) override {}
    // vt+0x298 0x14013fbb0 (P14): empty
    void simList(int pos, int* counter, Operation* op, std::vector<SimEntry>& out) override
    {
        simListNone(*this, pos, counter, op, out);
    }

    // ---- MDT L7: load slots (vt+0xe0, +0xc0 = 0x1405f1c00 and +0x1f8 are the base ones) --------
    void loadOperation(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx) override;
    // @src 0x14013af70  vt+0xb8: SfOprBase 0x1405f1bb0
    void loadPrologue(Project& p, Operation& op) override { OpClass::loadPrologue(p, op); }
    void loadClassData(Project& p, Operation& op) override;
    void propsLoad(Project& p, Operation& op) override;
    bool propsToEngine(Project& p, Operation& op) override;
    std::shared_ptr<Props> makeProps(int guiUnit) const override;
    std::shared_ptr<engine::GuiModel> createModel(Project& p, Operation& op) override;
    void loadData(Project& p, Operation& op, Ctx& ctx, engine::FileEffects& fx) override;
    // @src 0x14013fbf0  vt+0x190: dimension (0x14005f0f0) of the operation 0x140140140 picks
    int dim() const override
    {
        Operation* o = stepOperation();
        if (!o) l7::refuse("SfOprShapeOptim 0x14013fbf0: no operation for the dimension (0x14005f0f0 "
                           "dereferences it)");
        return opDim(*o);
    }

    // ---- MDT L4: the save slots of vt+0x68 0x14013d6f0 (vt+0xd0 is the base one) ---------------
    // vt+0x80 0x14013e4f0
    bool saveAfterFiles(Project& p, Operation& op, engine::FileEffects& fx) override;
    // vt+0xe8 0x14013c8f0
    bool saveOperationFiles(Project& p, Operation& op, engine::FileEffects& fx) override;

private:
    void init();
    Operation* stepOperation() const;
    // @src 0x14013f660  project of the current item, or null
    Project* project() const { return current ? current->project : nullptr; }
    // @src 0x14013fb00  +0xd8 = n, the step models (0x14013c200), then the selected object +0xd4
    void setStepOperation(Project& p, int n, Ctx& ctx, engine::FileEffects& fx);
    // @src 0x14013c200
    void stepModels(Project& p, Ctx& ctx, engine::FileEffects& fx);
    // @src 0x14013d180  the study's working folder, made (QDir::mkpath) when there is a sub-project
    String studyFolder(Project& p, engine::FileEffects& fx);

    // OB fields the load slots write (ctor 0x14013a9a0 -> 0x14013ab00: +0xd8 = +0xd4 = 0, then
    // 0x14013f5a0; the +0xb0 / +0xb8 QPointers start null)
    l7::TaskBlock60 b60_;          // +0x60 (ctor 0x1405e0ca0; its vt+8 0x1405e0e30 is empty)
    std::int32_t fc0_ = 0, fc4_ = 0, fc8_ = 0, fcc_ = 0, fd0_ = 0;   // +0xc0 .. +0xd0
    std::int32_t fd4_ = 0;         // +0xd4
    std::int32_t fd8_ = 0;         // +0xd8 flow number of the operation whose model it follows
    double fe0_ = 0.0;             // +0xe0
    std::uint8_t fe8_ = 0, fe9_ = 0;   // +0xe8, +0xe9
    std::vector<String> listF0_;   // +0xf0 QStringList
    // +0xb0 QPointer<SfOprShapeOptimProperties>: the props vt+0x98 made last (null once deleted)
    mutable std::weak_ptr<ShapeOptimProps> propsB0_;
    // +0xb8 QPointer<SfStepData>: the model the step objects are shown in (0x14013c200)
    engine::GuiModel* modelB8_ = nullptr;

    static void propsFromClass(ShapeOptimProps& pr);
    static void propsToClass(ShapeOptimProps& pr);
};

}  // namespace m2m::mst
