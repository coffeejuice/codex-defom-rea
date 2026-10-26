// SfPreLoadPattern: the "Load Pattern" editor of an HTFurnace operation, as far as its values reach
// the engine model. The form is :/SfOprHTFurnaceForm/SfOprHTFurnLoadPattern.ui (QUiLoader in the
// ctor 0x14106e8b0); its ranges: the six spin boxes 1 .. 1000, the six double spin boxes 0 ..
// 999999999 with Qt's default 2 decimals, rbPattnAlgn checked. Widgets editor[0x1a .. 0x2b] in the
// order of the names at 0x142bf8410: rbPattnAlgn, rbPattnRund, rbPattnRand, cbSymRow, cbSymCol,
// cbSymLay, sbRows, sbCols, sbLays, dsbDisRow, dsbDisCol, dsbDisLay, sbRows_Fxt, sbCols_Fxt,
// sbLays_Fxt, dsbDisRow_Fxt, dsbDisCol_Fxt, dsbDisLay_Fxt.
// Decompiles gui_mo/s1/mdt/HTF/a0.c .. a8.c, docs/re/moproj2mst/mdt/L6/spec.md section 8.
#include <cstdlib>
#include <string>

#include <defcmd/objpos_centre.hpp>
#include <defdbio/gendb/counts_frame.hpp>
#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/dynary_table.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include "engine_internal.hpp"
#include "m2m/engine.hpp"

namespace m2m::engine {
namespace {

using def::model::Cls_1401e7b10;
using def::model::DYNARY;
using def::model::LLOBJ_DATA;

[[noreturn]] void unsupported(const std::string& what)
{
    throw Error(M2M_E_UNSUPPORTED, "SfPreLoadPattern: " + what);
}

// QSpinBox::setValue: bound to [minimum, maximum] (the .ui: 1 .. 1000)
int spinValue(int v) { return v < 1 ? 1 : (v > 1000 ? 1000 : v); }

// QDoubleSpinBox::setValue (Qt 4.8): bound(round(v)), round = QString::number(v, 'f', decimals)
// .toDouble() with the default 2 decimals; the .ui range is 0 .. 999999999
double doubleSpinValue(double v)
{
    char buf[400];
    def::rt::crt::snprintf(buf, sizeof buf, "%.2f", v);
    double r = std::strtod(buf, nullptr);
    if (r < 0.0) r = 0.0;
    if (r > 999999999.0) r = 999999999.0;
    return r;
}

// the node box of an object: 0x140855d00 (= DEF_PRE 140181ae0) on its nodes (+0x8a0), or for a
// surface object (+0x3990) 0x140929cb0 (= 1402faca0) on surface 0 (0x14086c7c0: +0x3c00)
void objectBox(LLOBJ_DATA* o, double* mn, double* mx)
{
    if (!*at<std::uint8_t>(o, 0x3990)) {
        def::dbio::gendb::node_coord_range(at<std::byte>(o, 0x8a0), mn, mx);
    } else {
        auto* g = *at<def::model::OBJ_GEO3D*>(o, 0x3c00);
        def::cmd::objpos::surface_bbox(g, mn, mx);
    }
}

// @src 0x140c7d8d0  table 14 of the furnace model rebuilt from the block: 2 rows of 21 values
void blockToFurnaceTable(GuiModel& m)
{
    Cls_1401e7b10* sc = m.simCtl();
    DYNARY* t = nullptr;
    if (sc && sc->furnace_on) {
        const std::int32_t id = 0xe;
        t = sc->furnace.curve(&id);                         // 0x1409f0660 = 140375980
    }
    if (!t) return;
    t->ndim = 1;
    t->var[0] = 0;
    t->n[0] = 2;
    t->ncomp = 0x15;
    def::model::dynary_alloc(t);                            // 0x14089b840 = 14012fad0
    const std::int32_t* b = m.tableF8;
    auto set = [&](double v, int row, int c) {             // 0x14089c570: 1e-30 < |v| < 1e30, else 0
        def::model::llobj::dynary_set(*t, v, row, 0, 0, c);
    };
    static const int cols[8] = {0, 1, 6, 7, 8, 9, 10, 11};
    static const int off0[8] = {0x00, 0x08, 0x10, 0x14, 0x18, 0x28, 0x2c, 0x30};
    static const int off1[8] = {0x04, 0x0c, 0x1c, 0x20, 0x24, 0x34, 0x38, 0x3c};
    for (int k = 0; k < 8; ++k) set(static_cast<double>(b[off0[k] / 4]), 0, cols[k]);
    for (int k = 0; k < 3; ++k) set(m.tableF8d[k], 0, 12 + k);
    for (int k = 0; k < 8; ++k) set(static_cast<double>(b[off1[k] / 4]), 1, cols[k]);
    for (int k = 0; k < 3; ++k) set(m.tableF8d[3 + k], 1, 12 + k);
}

// @src 0x141071060  the form into the block (only widgets that exist; all of them do here)
void formToBlock(GuiModel& m, const LoadPatternForm& f)
{
    std::int32_t* b = m.tableF8;
    if (f.pattern >= 0 && f.pattern <= 2) b[0x8 / 4] = f.pattern;   // the checked radio button
    b[0x28 / 4] = f.sym[0] ? 1 : 0;
    b[0x2c / 4] = f.sym[1] ? 1 : 0;
    b[0x30 / 4] = f.sym[2] ? 1 : 0;
    b[0x10 / 4] = f.count[0];
    b[0x14 / 4] = f.count[1];
    b[0x18 / 4] = f.count[2];
    m.tableF8d[0] = f.dist[0];                              // +0x40
    m.tableF8d[1] = f.dist[1];
    m.tableF8d[2] = f.dist[2];
    b[0x1c / 4] = f.countFxt[0];
    b[0x20 / 4] = f.countFxt[1];
    b[0x24 / 4] = f.countFxt[2];
    m.tableF8d[3] = f.distFxt[0];                           // +0x58
    m.tableF8d[4] = f.distFxt[1];
    m.tableF8d[5] = f.distFxt[2];
    // one min/max pair for both objects (the original reuses the arrays without clearing them)
    double mn[3] = {0.0, 0.0, 0.0}, mx[3] = {0.0, 0.0, 0.0};
    if (LLOBJ_DATA* o = m.objectAt(1)) {                    // 0x140c7aed0(m, 1)
        b[0] = 1;
        objectBox(o, mn, mx);
        for (int i = 0; i < 3; ++i) m.tableF8box[i] = mn[i];      // +0x70
    }
    if (LLOBJ_DATA* o = m.objectAt(2)) {
        b[1] = 2;                                           // +0x4
        objectBox(o, mn, mx);
        for (int i = 0; i < 3; ++i) m.tableF8box[3 + i] = mn[i];  // +0x88
    }
}

}  // namespace

void loadPatternFormInit(GuiModel& m, LoadPatternForm& f)
{
    f = LoadPatternForm{};                                  // the .ui defaults
    // 0x1410703a0 sets the six distances from the objects' node boxes; 0x141070920 below replaces
    // them whenever block+0 >= 0
    Cls_1401e7b10* sc = m.simCtl();
    if (sc && sc->furnace_on) furnaceTableToBlock(m);       // after copying curve list 0x16 (UI)
    // @src 0x141070920  the block into the form
    const std::int32_t* b = m.tableF8;
    if (b[0] < 0)
        unsupported("block +0 < 0: the form keeps the node-box distances of 0x1410703a0 (not ported)");
    if (b[0x8 / 4] >= 0 && b[0x8 / 4] <= 2) f.pattern = b[0x8 / 4];   // setChecked(true), exclusive
    f.sym[0] = b[0x28 / 4] == 1;
    f.sym[1] = b[0x2c / 4] == 1;
    f.sym[2] = b[0x30 / 4] == 1;
    f.count[0] = spinValue(b[0x10 / 4]);
    f.count[1] = spinValue(b[0x14 / 4]);
    f.count[2] = spinValue(b[0x18 / 4]);
    for (int i = 0; i < 3; ++i) f.dist[i] = doubleSpinValue(m.tableF8d[i]);
    f.countFxt[0] = spinValue(b[0x1c / 4]);
    f.countFxt[1] = spinValue(b[0x20 / 4]);
    f.countFxt[2] = spinValue(b[0x24 / 4]);
    for (int i = 0; i < 3; ++i) f.distFxt[i] = doubleSpinValue(m.tableF8d[3 + i]);
    // the button text (0x140cf9cb0), the signal connections (0x1410740f0) and the input check
    // (vt+0x1b8 0x141070800 -> 0x141073ca0: style sheets) are UI; none sets the modified flag
}

void loadPatternUpdate(GuiModel& m, const LoadPatternForm& f)
{
    // 0x140131170: the editor's model (+0x38), result unused
    if (!f.modified) return;                                // 0x140378e70
    formToBlock(m, f);                                      // 0x141071060
    Cls_1401e7b10* sc = m.simCtl();
    if (sc && sc->furnace_on) blockToFurnaceTable(m);       // 0x140c7d8d0
}

}  // namespace m2m::engine
