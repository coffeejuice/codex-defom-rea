// MDT L1 part A: own loader slots of SfOprCogging and its properties' load
// (docs/re/moproj2mst/mdt/L1/spec_L1A.md; decompiles gui_mo/s1/mdt/L1/A/a0..a6.c, 766740.asm).
// The class is in src/ops/cogging.cpp; installLoaderSlotsL1() hands it these bodies. One function
// per binary function, in the original's call order.
//
// The new-operation path of the post-load (engine model with fewer than 3 objects: object creation,
// default names, DEF_CNT.DAT) and the equipment import of a top die whose slot has no equipment
// were added by MDT L4 (spec_L1A sections 3, 3.1, 3.4).
#include <cmath>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <memory>
#include <string>

#include <defcmd/key_file.hpp>
#include <defdbio/gendb/counts_frame.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>

#include "engine_internal.hpp"
#include "m2m/data/cogging.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine_cogging_process.hpp"
#include "m2m/mst/loader_l1.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4path.hpp"
#include "cogging_save.hpp"
#include "props_sync.hpp"

namespace m2m::mst {
void baseLoadPrologue(OpClass& c, Project& p, Operation& op);   // 0x1405f1bb0 (class_slots.cpp)
}

namespace m2m::engine {

using def::model::Cls_1401e7b10;
using def::model::LLOBJ_DATA;

namespace {

[[noreturn]] void unsupported(const std::string& what)
{
    throw Error(M2M_E_UNSUPPORTED, "operation load: " + what);
}

// 0x140051230 + __RTDynamicCast to SfOprCoggingProperties, then 0x1401cc6b0 (PROPS+0x40)
data::CogModel* cogModel(mst::OpClass& c, Operation& op)
{
    auto* cp = dynamic_cast<CoggingProps*>(c.propsOf(&op));
    return cp ? cp->model.get() : nullptr;
}

// 0x1401b2660  primitive type (LLOBJ_DATA +0x3aa4)
void setPrimType(LLOBJ_DATA* o, std::int32_t v)
{
    *at<std::int32_t>(o, 0x3aa4) = v;
}

// 0x1401b2470  primitive parameter k (0 <= k < 20: LLOBJ_DATA +0x3ab0 + 8k)
void setPrimParam(LLOBJ_DATA* o, int k, double v)
{
    if (k >= 0 && k < 20) *at<double>(o, 0x3ab0 + 8 * static_cast<std::size_t>(k)) = v;
}

// @src 0x140765c30  default name of cogging object no (1..10), null otherwise
String cogObjectName(int no)
{
    static const char* const kNames[10] = {"Billet", "Top Die", "Bottom Die", "Left Manipulator 1",
                                           "Left Manipulator 2", "Right Manipulator 1", "Right Manipulator 2",
                                           "Right Die", "Left Die", "Mandrel"};   // 0x142678698 .. 0x1426786a0
    if (no < 1 || no > 10) return String();
    return qt::L(kNames[no - 1]);
}

// @src 0x140353840  default die speed of object o: speed type (1), constant (0), no curve name,
// 76.0 (SI, sim control UNIT 1) or 3.0
bool defaultDieSpeed(GuiModel* w, LLOBJ_DATA* o)
{
    if (!w) return false;
    if (!o) unsupported("0x140353840 without an object (0x140c7b140, the primary die, is not reached here)");
    *at<std::int32_t>(o, 0x21c8) = 1;                              // movement type: speed
    *at<std::int32_t>(o, 0x224c) = 0;                              // constant
    at<char>(o, 0x2160)[0] = '\0';                                 // strcpy(o+0x2160, "") (0x142318c2f)
    Cls_1401e7b10* sc = w->simCtl();                               // the binary does not null-check it
    if (!sc) unsupported("0x140353840: model without a sim control (S+0xfc is read unchecked)");
    *at<double>(o, 0x2250) = sc->unit == 1 ? 76.0 : 3.0;           // 0x14231da08 / 0x1421ce530
    return true;
}

}  // namespace

// @src 0x140766740  engine object no's primitive description from the cogging model; an inherited
// billet (type 5) first takes Length / Width / Height / Dia from the node box of its library KEY
void cogSyncObjectGeometry(Project& p, Operation& op, data::CogModel& M, int no)
{
    GuiModel* w = opModel(p, op);                                  // 0x14005f770
    LLOBJ_DATA* o = w ? w->object(no) : nullptr;                   // 0x140c7ae80
    if (!o) return;
    switch (no - 1) {                                              // jump table 0x140766e48
    case 0: {                                                      // Billet
        data::CogBillet& b = M.cog.billet;                         // 0x14075dcb0, 0x14044b5d0
        if (*at<std::int32_t>(o, 0xc) == 5) {
            const int code = resolvedCode(p, op, 0);               // 0x14005fd20
            const String f = managerFile(p.objects, code);         // 0x1400a58f0, vt+0x208
            std::error_code ec;
            if (!f.isEmpty() && std::filesystem::exists(std::filesystem::path(f.u16()), ec)) {   // QFile::exists
                GuiModel t(nullptr, 3);                            // 0x14146eabc(0x278), 0x140c7a5b0
                const std::string local = engineKeyPath(f);
                const char reset = 1;
                const std::int32_t mode = 0;
                def::cmd::read_keyword_file(t.step.get(), local.c_str(), &reset, &mode);   // 0x1408969a0, rc ignored
                LLOBJ_DATA* s1 = t.object(1);
                double mn[3] = {}, mx[3] = {};
                if (s1 && def::dbio::gendb::node_coord_range(at<std::byte>(s1, 0x8a0), mn, mx)) {   // 0x140855d00
                    const double dz = mx[2] - mn[2];
                    b.length = mx[0] - mn[0];
                    b.width = dz;
                    b.height = dz;
                    b.dia = 2.0 * dz;                              // .rdata 0x142101520
                }
            }                                                      // vt+0x18 scalar deleting dtor
        }
        if (M.rotationalSymmetry) setPrimParam(o, 6, 1.0);         // 0x14075dca0, 0x1421249e0
        setPrimType(o, b.geoTyp + 6);
        setPrimParam(o, 0, b.length);
        setPrimParam(o, 1, b.width);
        setPrimParam(o, 2, b.height);
        setPrimParam(o, 3, b.dia);
        setPrimParam(o, 4, b.inDia);
        setPrimParam(o, 5, b.radius);
        break;
    }
    case 1: case 2: case 7: {                                      // Top Die, Bottom Die, Right Die
        const data::CogDie& d = M.cog.die;                         // 0x14075dcf0
        setPrimType(o, 9);
        setPrimParam(o, 0, d.length);
        setPrimParam(o, 1, d.width);
        setPrimParam(o, 2, d.height);
        setPrimParam(o, 3, d.radius);
        break;
    }
    case 3: {                                                      // Left Manipulator 1
        const data::CogManip& m = M.cog.manip;                     // 0x14075dcd0
        setPrimType(o, 10);
        setPrimParam(o, 0, m.length);
        setPrimParam(o, 1, m.width);
        setPrimParam(o, 2, m.height);
        setPrimParam(o, 3, m.radius1);
        setPrimParam(o, 4, m.radius2);
        setPrimParam(o, 5, m.radius3);
        setPrimParam(o, 6, m.height1);
        setPrimParam(o, 7, m.width1);
        setPrimParam(o, 8, m.f490);
        break;
    }
    case 9: {                                                      // Mandrel
        double p0 = 0.0, p3 = 0.0;
        const double p4 = 0.0;
        if (p.unit == 2) { p0 = 40.0; p3 = 20.0; }                 // 0x140054ff0
        else { p0 = 1000.0; p3 = 500.0; }
        setPrimType(o, 6);
        setPrimParam(o, 0, p0);
        setPrimParam(o, 3, p3);
        setPrimParam(o, 4, p4);
        break;
    }
    default: break;                                                // 4, 5, 6, 8
    }
}

namespace {

// @src 0x140750650  SfOprCogging vt+0xb8
void cogLoadPrologue(mst::OpClass& c, Project& p, Operation& op)
{
    mst::baseLoadPrologue(c, p, op);                               // 0x1405f1bb0
    setTrans2(p, op, true);                                        // 0x1405ef6f0 (again, as the original)
    setTrans1(p, op, true);                                        // 0x1405ef750
    if (GuiModel* w = opModel(p, op))                              // 0x14005f820
        if (Cls_1401e7b10* s = w->simCtl()) {                      // 0x140c7b2d0
            s->nstep = 100;
            s->stpinc = 10;
            s->nstart = -1;
            s->dttype.u58 = 0;                                     // 0x1408da6f0: S+0x240 = 0 (DSMAX)
            s->dsmax = 0.1;                                        // .rdata 0x1421bfdd8 = 0x3fb999999999999a
            s->solmtd = 1;
            s->solmtt = 1;
            s->dplen = 0.0;
        }
    op.lubSystem = 1;                                              // 0x140065790, 0x1400ab6f0: OPI+0xe0
}

// @src 0x140765dd0  object no of a new cogging operation: added to the operation (0x140057770), named
// (0x140765c30; sprintf with the name as format, none has a '%'), the slot renamed to it (0x1400585e0),
// mesh kind +0x2d94 = 1, then its geometry from the model (0x140766740)
static void cogInitObject(Project& p, Operation& op, data::CogModel& M, int no, FileEffects& fx)
{
    addObjectNo(p, op, no, fx);                                    // 0x140057770 (result unused)
    GuiModel* w = opModel(p, op);                                  // 0x14005f770 (M+0x548's operation)
    LLOBJ_DATA* o = w ? w->object(no) : nullptr;                   // 0x140c7ae80
    if (!o) return;
    const std::string nm = cogObjectName(no).utf8();               // QString::operator const char* (toAscii; ASCII names)
    std::strcpy(at<char>(o, 0x828), nm.c_str());                   // 0x14146de38 sprintf(o+0x828, nm)
    mst::renameSlot(p, op, no - 1, String::latin1(at<char>(o, 0x828)), fx);   // 0x1400585e0 (QString(o->name), result unused)
    *at<std::int32_t>(o, 0x2d94) = 1;
    cogSyncObjectGeometry(p, op, M, no);                           // 0x140766740
}

// @src 0x1407507b0  SfOprCogging vt+0xc0 (does not call the base 0x1405f1c00)
void cogPostLoad(mst::OpClass& c, Project& p, Operation& op, Ctx&, FileEffects& fx)
{
    GuiModel* w = opModel(p, op);                                  // 0x14005f820
    if (!w) return;
    data::CogModel* M = cogModel(c, op);
    if (!M) return;
    if (w->objectCount() < 3) {                                    // 0x140c7c1a0: a new operation
        cogInitObject(p, op, *M, 1, fx);                           // 0x140765dd0
        cogInitObject(p, op, *M, 2, fx);
        cogInitObject(p, op, *M, 3, fx);
        if (M->cog.useManip != 0) {                                // 0x140765960
            cogInitObject(p, op, *M, 4, fx);
            cogInitObject(p, op, *M, 5, fx);
            cogInitObject(p, op, *M, 6, fx);
            cogInitObject(p, op, *M, 7, fx);
        }
        if ((M->cog.die.four == 1 ? 4 : 2) == 4) {                 // 0x140765b00
            cogInitObject(p, op, *M, 8, fx);
            cogInitObject(p, op, *M, 9, fx);
        }
        if (M->useMandrel) cogInitObject(p, op, *M, 10, fx);       // 0x14075dc00
        // fopen("DEF_CNT.DAT", "wt") (0x1425e3eb8, 0x1425e09fc), fprintf "1\n", fclose: relative to
        // the current folder, which the operation open set to the project folder (0x14003a3c0 ->
        // QDir::setCurrent, loader.cpp); text mode writes "1\r\n"
        fx.writes.push_back({qt::cleanPath(projectDir(p) + qt::kSep + qt::L("DEF_CNT.DAT")), "1\r\n"});
    }
    cogSyncObjectGeometry(p, op, *M, 1);
    cogSyncObjectGeometry(p, op, *M, 2);
    cogSyncObjectGeometry(p, op, *M, 4);
    cogSyncObjectGeometry(p, op, *M, 3);
    cogSyncObjectGeometry(p, op, *M, 8);
    if (M->useMandrel) cogSyncObjectGeometry(p, op, *M, 10);       // 0x14075dc00
    // equipment of the top die (object 2); the manager 0x1400a5970 is created lazily, never null
    LLOBJ_DATA* o2 = w->object(2);                                 // 0x140c7ae80
    if (!o2)
        unsupported("cogging operation without object 2 (the original dereferences it, 0x1407507b0)");
    ObjEntry* rec = objEntryN(op, *at<std::int32_t>(o2, 8));      // 0x1400612d0
    if (!rec || rec->equipmentId >= 1) return;                     // 0x14004de90
    // "Equipment (" + QString("%1").arg(rows + 1, 0, 10, ' ') + ")" (vt+0x1b8 row count)
    const String name = qt::L("Equipment (") + qt::number(static_cast<int>(p.equipment.rows.size()) + 1) +
                        qt::L(")");
    if (*at<std::int32_t>(o2, 0x21c8) == 1 && *at<std::int32_t>(o2, 0x224c) == 0 &&
        std::fabs(*at<double>(o2, 0x2250)) < 1e-6)                 // 0x141470a78, 0x142157420
        defaultDieSpeed(w, o2);                                    // 0x140353840
    // QDir::tempPath() + sep + name + ".KEY": the KEY written there (0x140358190, force 1), imported
    // (0x1400bf510 -> 0x14006c610: the next free id, copied to Equipments/Equipment%05d.KEY), then
    // removed (QFile::remove): the temporary file leaves no trace
    const std::string bytes = equipmentBytes(*w->step, o2);       // with the movement name as it was
    const int id = addEquipment(p, name, &bytes, fx);              // 0x1400bf510
    std::strncpy(at<char>(o2, 0x2160), local8(name).c_str(), 0x50);   // 0x141470020
    // em->filePath(&id) (vt+0x208): result unused
    rec->equipmentId = id;                                         // 0x14004deb0 (-1 when the import failed)
}

// @src 0x14075ee20  SfOprCoggingProperties vt+0x58
void cogPropsLoad(mst::OpClass& c, Project& p, Operation& op)
{
    Props* pr = c.propsOf(&op);
    if (!pr) return;
    propsStepsFromSimCtl(p, *pr, nullptr);                         // 0x1405fb720
    propsEnvTempFromSimCtl(p, *pr, nullptr);                       // 0x1405fbae0
    propsConvectionFromSimCtl(p, *pr, nullptr);                    // 0x1405fbe30
}

}  // namespace

void installLoaderSlotsL1()
{
    mst::g_loaderSlotsL1.cogLoadPrologue = &cogLoadPrologue;
    mst::g_loaderSlotsL1.cogPostLoad = &cogPostLoad;
    mst::g_loaderSlotsL1.cogPropsLoad = &cogPropsLoad;
    mst::g_loaderSlotsL1.cogProcessUpdate = &cogProcessUpdate;   // MDT L4 (cogging_process.cpp)
    mst::g_loaderSlotsL1.cogSaveNames = [](mst::OpClass&, Project& p, Operation& op, FileEffects& fx) {
        coggingAfterSave(p, op, fx);
    };
}

}  // namespace m2m::engine
