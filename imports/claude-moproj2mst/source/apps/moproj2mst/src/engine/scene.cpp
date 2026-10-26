// Packet V (added by MDT L4): the named models of the main window's 3D display SfGLDisplay
// (m2m/engine_scene.hpp; decompiles gui_mo/s1/mdt/L4/vs/a.c, b.c). The display, its main model and
// its camera are in viewer_camera.cpp. Icons, tool tips and redraws are UI and are named where
// they are dropped.
#include <cstring>

#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>

#include "m2m/engine.hpp"
#include "m2m/engine_scene.hpp"

namespace m2m::engine {

namespace {

[[noreturn]] void unsupported(const std::string& what)
{
    throw Error(M2M_E_UNSUPPORTED, "3D display: " + what);
}

}  // namespace

// @src 0x140cfa0e0
GuiModel* displayNamedModel(Display& d, const String& name)
{
    DisplayModels& dm = displayModels(d);
    for (DisplayModels::Named& n : dm.named)                 // 0x140070920, 0x1400b5260
        if (n.name == name) return n.model.get();            // 0x140d12200
    GuiModel* b = displayMain(d);                            // 0x140cf94f0
    if (!b || !b->step) unsupported("named model without a main model (0x140c7fee0 dereferences it)");
    const int dim = b->dimension();                          // 0x140c7fee0 -> 0x140100980 (+0x1c)
    auto m = std::make_unique<GuiModel>(nullptr, dim);       // new 0x278, 0x140c7a6b0(mem, dim)
    if (name == qt::L("ROIs"))                               // 0x1429f8118
        m->step->u3400 = 1;                                  // 0x140831650(step, 1)
    else if (name == qt::L("Coupons"))                       // 0x1429f8160
        m->step->u3400 = 2;                                  // 0x140831650(step, 2)
    // 0x140c91360 / 0x140c913d0 / 0x140c914a0: the model's name, icon and tool tip (UI)
    m->display = &d;                                         // 0x140c7ade0
    m->step->u744 = b->step->u744;                           // +0x744
    std::strcpy(m->step->u340, b->step->u340);               // 0x14146fec0 (strcpy) +0x340
    if (def::model::Cls_1401e7b10* sc = m->simCtl())         // 0x140c7b2d0
        if (const def::model::Cls_1401e7b10* bsc = b->simCtl()) sc->geotyp = bsc->geotyp;   // +0x100
    GuiModel* r = m.get();
    // the holder 0x140d12250 (QPointer to the model, flag +8 = 0) under the name (0x140c75050)
    auto at = dm.named.begin();
    while (at != dm.named.end() && at->name < name) ++at;
    dm.named.insert(at, DisplayModels::Named{name, std::move(m), false});
    return r;
}

// @src 0x140cf9990
GuiModel* displaySecond(Display& d)
{
    DisplayModels& dm = displayModels(d);
    if (dm.second.get()) return dm.second.get();             // 0x140c1aef0(+0x1b8)
    GuiModel* b = displayMain(d);                            // 0x140cf94f0
    if (!b) return nullptr;
    auto m = std::make_unique<GuiModel>(nullptr, b->dimensionOr(3));   // new 0x278, 0x140c7a6b0(mem, 0x140c7fee0(b))
    displaySetSecond(d, std::move(m));                       // 0x140cbc920(+0x1b8), before backlink/control queries
    GuiModel* second = dm.second.get();
    second->display = &d;                                    // 0x140c7ade0
    if (def::model::Cls_1401e7b10* sc = second->simCtl())    // 0x140c7b2d0
        if (const def::model::Cls_1401e7b10* bsc = b->simCtl()) sc->geotyp = bsc->geotyp;   // +0x100
    return dm.second.get();
}

// @src 0x140cf9930
void displaySetSecond(Display& d, std::unique_ptr<GuiModel> m)
{
    DisplayModels& dm = displayModels(d);
    if (dm.second.get() && dm.second.get() != dm.secondOwned.get())
        unsupported("second model replaced while it is not the display's own (0x140cbc920)");
    dm.second.reset(m.get());
    dm.secondOwned = std::move(m);                           // the old one is deleted: never shown again
}

// @src 0x140cf9bd0
void displayDropSecond(Display& d)
{
    DisplayModels& dm = displayModels(d);
    if (dm.keepSecond || !dm.second.get()) return;           // +0x1c1
    if (dm.second.get() != dm.secondOwned.get())
        unsupported("second model to delete is not the display's own (0x140cf9bd0 vt+0x18)");
    dm.secondOwned.reset();                                  // vt+0x18(1); the dtor clears +0x1b8
    dm.second.reset();                                       // 0x140cbc920(+0x1b8, 0)
}

GuiModel* displayRoiModel(GuiModel* m)
{
    if (!m) return nullptr;                                  // 0x140d0d670: null stays null
    Display* d = m->display;                                 // 0x140c7ae00
    if (!d) return m;
    GuiModel* r = displayNamedModel(*d, qt::L("ROIs"));      // 0x1429f8460
    r->f25c = 2;                                             // 0x140d12560
    return r;
}

}  // namespace m2m::engine
